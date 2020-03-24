#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "icsp.h"
#include "pgbuf.h"
#include "writer.h"

int
writer_flash(pgbuf_t *pg)
{
    uint32_t romaddr;
    uint32_t bufaddr;
    uint32_t ramaddr;
    extern uint32_t const currowsz;

    romaddr = pg->pg_addr;
    bufaddr = 0xA0000000;
    ramaddr = bufaddr & 0x1FFFFFFF;
    printf(" loading");
    fflush(stdout);
    icsp_write_mem(bufaddr, pg->pg_buf, currowsz);
    printf(" writing");
    fflush(stdout);
    icsp_flash_row(romaddr, ramaddr);
    usleep(1000);
    return 0;
}

int
writer_verify(pgbuf_t *pg)
{
    extern uint32_t const currowsz;
    uint8_t buf[512];
    int e;
    uint32_t addr;

    addr = pg->pg_addr | 0xA0000000;
    icsp_read_mem(addr, buf, currowsz);
    e = memcmp(buf, pg->pg_buf, currowsz);
    return e;
}

int
writer_all()
{
    extern pgbuf_t *pgbuf_top;
    pgbuf_t *pg;
    int rv;

    for (pg = pgbuf_top; pg; pg = pg->pg_next)
    {
        printf("flashing at 0x%08X", pg->pg_addr);
	fflush(stdout);
	writer_flash(pg);
	printf("\n");
    }
    rv = 0;
    for (pg = pgbuf_top; pg; pg = pg->pg_next)
    {
        printf("reading at 0x%08X", pg->pg_addr);
	fflush(stdout);
	rv |= writer_verify(pg);
	printf("\n");
    }
    if (rv)
        printf("verify failure\n");
    else
        printf("verify good\n");
    return rv;
}
