/*
 * Intel HEX file loader
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgbuf.h"
#include "ihex.h"

char const *ihex_fname;
FILE *ihex_fp;
int ihex_linecnt;
int ihex_lineoffs;
uint8_t ihex_ch;
uint8_t ihex_linebuf[256];
uint16_t ihex_bank;

void
ihex_set_fname(char const *name)
{
    ihex_fname = name;
}

int
ihex_open()
{
    FILE *fp;

    fp = fopen(ihex_fname, "r");
    if (fp == NULL)
    {
        perror("open ihex");
	goto bad;
    }
    ihex_fp = fp;
    ihex_linecnt = 1;
    return 0;

bad:
    return -1;
}

int
ihex_close()
{
    fclose(ihex_fp);
    ihex_fp = NULL;
    return 0;
}

void
ihex_nextch()
{
    ihex_ch = fgetc(ihex_fp);
}

int
ihex_load_line()
{
    int val;
    uint8_t sum;

    ihex_lineoffs = 0;

    if (ihex_ch != ':')
    {
        printf("error: bad line header: %02X -- line %d\n", ihex_ch, ihex_linecnt);
        goto bad;
    }
    ihex_nextch();
    sum = 0;
    while (ihex_ch >= 32)
    {
        val = 0;

	/*
	 * first digit
	 */
        if (ihex_ch >= '0' && ihex_ch <= '9')
	{
	    val = (val << 4) + ihex_ch - '0';
	    ihex_nextch();
        }
        else if (ihex_ch >= 'A' && ihex_ch <= 'F')
	{
	    val = (val << 4) + ihex_ch - 'A' + 10;
	    ihex_nextch();
        }
        else
	    goto bad;

	/*
	 * second digit
	 */
        if (ihex_ch >= '0' && ihex_ch <= '9')
	{
	    val = (val << 4) + ihex_ch - '0';
	    ihex_nextch();
        }
        else if (ihex_ch >= 'A' && ihex_ch <= 'F')
	{
	    val = (val << 4) + ihex_ch - 'A' + 10;
	    ihex_nextch();
        }
        else
	    goto bad;

        if (ihex_lineoffs >= sizeof(ihex_linebuf))
	{
	    printf("error: ihex buffer overrun\n");
	    goto bad;
	}
        ihex_linebuf[ihex_lineoffs] = val;
	ihex_lineoffs++;
	sum += val;
    }
    if (sum != 0)
    {
        printf("error: ihex checksum failure\n");
	goto bad;
    }
    if (ihex_ch == '\r')
        ihex_nextch();
    if (ihex_ch == '\n')
        ihex_nextch(), ihex_linecnt++;
    return 0;

bad:
    return -1;
}

int
ihex_apply_line()
{
    uint8_t *ptr;
    int left;
    uint8_t ilen;
    uint16_t iaddr;
    uint8_t itype;
    uint8_t ch;

    ptr = ihex_linebuf;
    left = ihex_lineoffs;

    ilen = *ptr++; left--;
    iaddr = *ptr++ << 8; left--;
    iaddr |= *ptr++; left--;
    itype = *ptr++; left--;

    if (left < 0)
    {
        printf("error: ihex line too short\n");
        goto bad;
    }
    switch (itype)
    {
    case 0: /* data record */
        while (ilen > 0)
	{
	    uint32_t addr;
	    uint32_t offs;
	    pgbuf_t *pg;

	    ch = *ptr++; ilen--;
	    addr = iaddr; iaddr++;
	    addr |= (uint32_t)ihex_bank << 16;

            offs = addr & pgbuf_getmask();
	    pg = pgbuf_alloc(addr);
	    pg->pg_buf[offs] = ch;
	}
	break;
    case 1: /* EOF record */
	break;
    case 4: /* 32bit address record */
        {
	    uint16_t bank;

            bank = *ptr++ << 8; left--;
	    bank |= *ptr++; left--;
	    ihex_bank = bank;
	}
    case 5: /* 32bit entry-point record */
        break;
    }
    return 0;

bad:
    return -1;
}

int
ihex_parse(void)
{
    int rv;

    /* fetch first char */
    ihex_nextch();
    while (ihex_ch != 0xFF)
    {
        rv = ihex_load_line();
	if (rv)
	    goto bad;
	rv = ihex_apply_line();
	if (rv)
	    goto bad;
    }
    return 0;

bad:
    return -1;
}

int
ihex_dump(void)
{
    int i;
    int offs;
    pgbuf_t *pg;
    extern int32_t const currowsz;

    for (pg=pgbuf_top; pg; pg=pg->pg_next)
    {
        offs = 0;
	while (offs < currowsz)
	{
	    printf("%08X+%02X:", pg->pg_addr, offs);
            for (i=0; i<16; i++)
	        printf(" %02X", pg->pg_buf[offs+i]);
	    offs += 16;
	    printf("\n");
	}
    }
    return 0;
}
