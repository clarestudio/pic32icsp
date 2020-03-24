#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd.h"
#include "tapdef.h"
#include "icsp.h"
#include "picdev.h"

picdev_t const *curdev;

int icsp_enter()
{
    cmd_icsp_enter();
    return 0;
}

int icsp_leave()
{
    cmd_icsp_leave();
    return 0;
}

int icsp_check_idcode()
{
    uint32_t idcode;
    picdev_t const *dev;

    icsp_command(MTAP_SW_MTAP);
    icsp_command(TAP_IDCODE);
    idcode = icsp_xfer_dword(0);
    printf("reading idcode: %08X\n", idcode);
    if (idcode == 0 || idcode == 0xFFFFFFFF)
    {
        printf("error: invalid idcode\n");
	return -1;
    }
    dev = picdev_lookup(idcode);
    if (!dev->idcode)
    {
        printf("warning: device unknown\n");
    }
    else
    {
        printf("    device name: %s\n", dev->name);
        printf("    device revision: %d\n", idcode >> 28);
        printf("    size of row: %d bytes\n", dev->rowsz);
        printf("    size of text ROM: %d bytes\n", dev->romsz);
        printf("    size of boot ROM: %d bytes\n", dev->bootsz);
        printf("    size of data RAM: %d bytes\n", dev->ramsz);
    }
    curdev = dev;

    return 0;
}

void icsp_tap_idle()
{
    cmd_tap_idle();
}

void icsp_command(uint8_t cmd)
{
    cmd_tap_command(cmd);
}

uint8_t icsp_xfer_byte(uint8_t idata)
{
    uint8_t odata;

    odata = cmd_xfer_byte(idata);
    return odata;
}

uint32_t icsp_xfer_dword(uint32_t idata)
{
    uint32_t odata = 0;

    odata = cmd_xfer_dword(idata);
    return odata;
}

uint32_t icsp_xfer_fastdata(uint32_t idata)
{
    uint32_t odata = 0;

    odata = cmd_xfer_fastdata(idata);
    return odata;
}

void icsp_xfer_insn(uint32_t insn)
{
    cmd_xfer_insn(insn);
}

uint32_t icsp_read_dword(uint32_t addr)
{
    uint32_t data;

    cmd_set_addr(addr);
    data = cmd_read_dword();
    return data;
}

int icsp_erase_chip()
{
    uint8_t cv;

    printf("erasing chip:");
    fflush(stdout);
    icsp_command(MTAP_SW_MTAP);
    icsp_command(MTAP_COMMAND);
    icsp_xfer_byte(MCHP_ERASE);
    do {
        usleep(10000);
	cv = icsp_xfer_byte(MCHP_STATUS);
	printf(" %02X", cv);
        fflush(stdout);
    } while ((cv & 0b1100) != 0b1000);
    printf("\n");
    return 0;
}

int icsp_enter_serial_exec()
{
    uint8_t cv;

    printf("entering serial execution mode\n");
    icsp_command(MTAP_SW_MTAP);
    icsp_command(MTAP_COMMAND);
    cv = icsp_xfer_byte(MCHP_STATUS);
    if ((cv & 0b10000000) == 0)
    {
        printf("chip is code-protected, erase first\n");
	return -1;
    }
    icsp_xfer_byte(MCHP_ASSERT_RST);
    icsp_command(MTAP_SW_ETAP);
    icsp_command(ETAP_EJTAGBOOT);
    icsp_command(MTAP_SW_MTAP);
    icsp_command(MTAP_COMMAND);
    icsp_xfer_byte(MCHP_DE_ASSERT_RST);
    icsp_xfer_byte(MCHP_FLASH_ENABLE);
    icsp_command(MTAP_SW_ETAP);
    return 0;
}

/*
 * transfer at most 64KB.
 */
void icsp_write_mem(uint32_t addr, uint8_t const *buf, size_t len)
{
    uint32_t const *p;
    int offs;

    if ((intptr_t)buf & 3)
    {
        printf("data must be aligned by address\n");
	abort();
    }
    if (len & 3)
    {
        printf("data must be aligned by length\n");
	abort();
    }

    cmd_set_addr(addr);
    p = (uint32_t const *)buf;
    for (offs = 0; offs < len; offs+=sizeof(*p), p++)
        cmd_write_dword(*p);
}

void icsp_read_mem(uint32_t addr, uint8_t *buf, size_t len)
{
    uint32_t *p;
    int offs;

    if ((intptr_t)buf & 3)
    {
        printf("data must be aligned by address\n");
	abort();
    }
    if (len & 3)
    {
        printf("data must be aligned by length\n");
	abort();
    }

    cmd_set_addr(addr);
    p = (uint32_t *)buf;
    for (offs = 0; offs < len; offs+=sizeof(*p), p++)
        *p = cmd_read_dword();
}

int icsp_flash_row(uint32_t addr, uint32_t srcaddr)
{
    uint32_t insn;

    /*
     * step 1
     */
    insn = 0x34054003;		/* ori a1,zero,0x4003 */
    icsp_xfer_insn(insn);
    insn = 0x34068000;		/* ori a2,zero,0x8000 */
    icsp_xfer_insn(insn);
    insn = 0x34074000;		/* ori a3,zero,0x4000 */
    icsp_xfer_insn(insn);
    insn = 0x3C11AA99;		/* lui s1,0xAA99 */
    icsp_xfer_insn(insn);
    insn = 0x36316655;		/* ori s1,s1,0x6655 */
    icsp_xfer_insn(insn);
    insn = 0x3C125566;		/* lui s2,0x5566 */
    icsp_xfer_insn(insn);
    insn = 0x365299AA;		/* ori s2,s2,0x99AA */
    icsp_xfer_insn(insn);
    insn = 0x3C100000;		/* lui s0,0x0000 */
    icsp_xfer_insn(insn);
    /*
     * step 2: set a0 to NVM base address
     */
    insn = 0x3C04BF80;		/* lui a0,0xBF80 */
    icsp_xfer_insn(insn);
    insn = 0x3484F400;		/* ori a0,a0,0xF400 */
    icsp_xfer_insn(insn);
    /*
     * step 4: set the NVMADDR register (DMA destination)
     */
    insn = 0x3C080000;		/* lui t0,hi(addr) */
    insn |= (addr >> 16);
    icsp_xfer_insn(insn);
    insn = 0x35800000;		/* ori t0,t0,lo(addr) */
    insn |= (addr & 0xFFFF);
    icsp_xfer_insn(insn);
    insn = 0xAC880020;		/* sw t0,(a0 + 32) */
    icsp_xfer_insn(insn);
    /*
     * step 5: set the NVMSRCADDR register (DMA source)
     */
    insn = 0x36100000;		/* ori s0,s0,lo(srcaddr) */
    insn |= (srcaddr & 0xFFFF);
    icsp_xfer_insn(insn);
    insn = 0xAC900040;		/* sw s0,(a0 + 64) */
    icsp_xfer_insn(insn);
    /*
     * step 6: setup the NVMCON register
     */
    insn = 0xAC850000;		/* sw a1,(a0 + 0) */
    icsp_xfer_insn(insn);
    /*
     * step 7: poll the LVDSTAT register
     */
    usleep(1000);
    insn = 0x8C880000;		/* lw t0,(a0 + 0) */
    icsp_xfer_insn(insn);
    insn = 0x31080800;		/* andi t0,t0,0x0800 */
    icsp_xfer_insn(insn);
    insn = 0x1500FFFD;		/* bne t0,zero,step7 */
    icsp_xfer_insn(insn);
    insn = 0x00000000;		/* nop */
    icsp_xfer_insn(insn);
    /*
     * step 8: unlock the NVMCON register and start the write.
     */
    insn = 0xAC910010;		/* sw s1,(a0 + 16) */
    icsp_xfer_insn(insn);
    insn = 0xAC920010;		/* sw s2,(a0 + 16) */
    icsp_xfer_insn(insn);
    insn = 0xAC860008;		/* sw a2,(a0 + 8) */
    icsp_xfer_insn(insn);
    /* 
     * step 9: loop until the WR bit is clear.
     */
    usleep(1000);
    insn = 0x8C880000;		/* lw t0,(a0 + 0) */
    icsp_xfer_insn(insn);
    insn = 0x01064024;		/* and t0,t0,a2 */
    icsp_xfer_insn(insn);
    insn = 0x1500FFFD;		/* bne t0,zero,step9 */
    icsp_xfer_insn(insn);
    insn = 0x00000000;		/* nop */
    icsp_xfer_insn(insn);
    /*
     * step 10: wait a bit
     */
    insn = 0x00000000;		/* nop */
    icsp_xfer_insn(insn);
    insn = 0x00000000;		/* nop */
    icsp_xfer_insn(insn);
    insn = 0x00000000;		/* nop */
    icsp_xfer_insn(insn);
    insn = 0x00000000;		/* nop */
    icsp_xfer_insn(insn);
    /*
     * step 11: clear the WREN bit
     */
    insn = 0xAC870004;		/* sw a3,(a0 + 4) */
    icsp_xfer_insn(insn);

    return 0;
}
