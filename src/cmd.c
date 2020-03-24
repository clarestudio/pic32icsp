/*
 * remote procedure call
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmddef.h"
#include "cmd.h"
#include "serial.h"

int cmd_nop(void)
{
    uint8_t rv;

    serial_putc(CMD_NOP);
    rv = serial_getc();
    if (rv != STATUS_ACK)
    {
        printf("error: cmd_nop failed\n");
	return -1;
    }
    return 0;
}

int cmd_version(void)
{
    uint8_t rv;

    serial_putc(CMD_VERSION);
    rv = serial_getc();
    return rv;
}

int cmd_icsp_enter(void)
{
    uint8_t rv;

    serial_putc(CMD_ICSP_ENTER);
    rv = serial_getc();
    if (rv != STATUS_ACK)
    {
        printf("error: cmd_icsp_enter failed\n");
        return -1;
    }
    return 0;
}

int cmd_icsp_leave(void)
{
    uint8_t rv;

    serial_putc(CMD_ICSP_LEAVE);
    rv = serial_getc();
    if (rv != STATUS_ACK)
    {
        printf("error: cmd_icsp_leave failed\n");
        return -1;
    }
    return 0;
}

int cmd_tap_idle()
{
    uint8_t rv;

    serial_putc(CMD_TAP_IDLE);
    rv = serial_getc();
    if (rv == STATUS_ACK)
        return 0;
    else
        return -1;
}

int cmd_tap_command(uint8_t data)
{
    uint8_t cbuf[2];
    uint8_t rv;

    cbuf[0] = CMD_TAP_COMMAND;
    cbuf[1] = data;
    serial_putmem(cbuf, sizeof(cbuf));
    rv = serial_getc();
    if (rv == STATUS_ACK)
        return 0;
    else
        return -1;
}

uint8_t cmd_xfer_byte(uint8_t data)
{
    uint8_t cbuf[2];
    uint8_t rv;

    cbuf[0] = CMD_XFER_BYTE;
    cbuf[1] = data;
    serial_putmem(cbuf, sizeof(cbuf));
    rv = serial_getc();
    return rv;
}

uint32_t cmd_xfer_dword(uint32_t data)
{
    uint8_t cbuf[5];
    uint8_t rbuf[4];
    uint32_t rv;

    cbuf[0] = CMD_XFER_DWORD;
    cbuf[1] = data >> 0;
    cbuf[2] = data >> 8;
    cbuf[3] = data >> 16;
    cbuf[4] = data >> 24;
    serial_putmem(cbuf, sizeof(cbuf));
    serial_getmem(rbuf, sizeof(rbuf));
    rv = ((uint32_t)(rbuf[0]) << 0)
       | ((uint32_t)(rbuf[1]) << 8)
       | ((uint32_t)(rbuf[2]) << 16)
       | ((uint32_t)(rbuf[3]) << 24)
       ;
    return rv;
}

uint32_t cmd_xfer_fastdata(uint32_t data)
{
    uint8_t cbuf[5];
    uint8_t rbuf[4];
    uint32_t rv;

    cbuf[0] = CMD_XFER_FASTDATA;
    cbuf[1] = data >> 0;
    cbuf[2] = data >> 8;
    cbuf[3] = data >> 16;
    cbuf[4] = data >> 24;
    serial_putmem(cbuf, sizeof(cbuf));
    serial_getmem(rbuf, sizeof(rbuf));
    rv = ((uint32_t)(rbuf[0]) << 0)
       | ((uint32_t)(rbuf[1]) << 8)
       | ((uint32_t)(rbuf[2]) << 16)
       | ((uint32_t)(rbuf[3]) << 24)
       ;
    return rv;
}

int cmd_xfer_insn(uint32_t data)
{
    uint8_t cbuf[5];
    uint8_t rv;

    cbuf[0] = CMD_XFER_INSN;
    cbuf[1] = data >> 0;
    cbuf[2] = data >> 8;
    cbuf[3] = data >> 16;
    cbuf[4] = data >> 24;
    serial_putmem(cbuf, sizeof(cbuf));
    rv = serial_getc();
    if (rv != STATUS_ACK)
    {
        printf("error: cmd_xfer_insn failed: got %02X\n", rv);
        return -1;
    }
    return 0;
}

void cmd_set_addr(uint32_t data)
{
    uint8_t cbuf[5];
    uint8_t rv;

    cbuf[0] = CMD_SET_ADDR;
    cbuf[1] = data >> 0;
    cbuf[2] = data >> 8;
    cbuf[3] = data >> 16;
    cbuf[4] = data >> 24;
    serial_putmem(cbuf, sizeof(cbuf));
    rv = serial_getc();
    if (rv != STATUS_ACK)
        printf("error: cmd_set_addr failed: got %02X\n", rv);
}

uint32_t cmd_read_dword()
{
    uint8_t cbuf[1];
    uint8_t rbuf[4];
    uint32_t rv;

    cbuf[0] = CMD_READ_DWORD;
    serial_putmem(cbuf, sizeof(cbuf));
    serial_getmem(rbuf, sizeof(rbuf));
    rv = ((uint32_t)(rbuf[0]) << 0)
       | ((uint32_t)(rbuf[1]) << 8)
       | ((uint32_t)(rbuf[2]) << 16)
       | ((uint32_t)(rbuf[3]) << 24)
       ;
    return rv;
}

void cmd_write_dword(uint32_t data)
{
    uint8_t cbuf[5];
    uint8_t rv;

    cbuf[0] = CMD_WRITE_DWORD;
    cbuf[1] = data >> 0;
    cbuf[2] = data >> 8;
    cbuf[3] = data >> 16;
    cbuf[4] = data >> 24;
    serial_putmem(cbuf, sizeof(cbuf));
    rv = serial_getc();
    if (rv != STATUS_ACK)
        printf("error: cmd_write_dword failed: got %02X\n", rv);
}
