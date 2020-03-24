#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgbuf.h"

uint32_t curpagesz = 1024;
uint32_t currowsz = 128;
pgbuf_t *pgbuf_top;

pgbuf_t *pgbuf_create(void)
{
    pgbuf_t *p;

    p = malloc(sizeof(*p));
    p->pg_next = 0;
    p->pg_addr = 0;
    p->pg_buf = malloc(currowsz);
    memset(p->pg_buf, -1, currowsz);
    return p;
}

void pgbuf_free(pgbuf_t *p)
{
    free(p->pg_buf);
    free(p);
}

pgbuf_t *pgbuf_lookup(uint32_t addr)
{
    uint32_t mask;
    pgbuf_t *ptr;

    addr &= 0x1FFFFFFF;
    mask = currowsz - 1;
    addr &= ~mask;
    for (ptr=pgbuf_top; ptr; ptr=ptr->pg_next)
        if (ptr->pg_addr == addr)
	    return ptr;
    return NULL;
}

pgbuf_t *pgbuf_alloc(uint32_t addr)
{
    uint32_t mask;
    pgbuf_t *ptr;

    addr &= 0x1FFFFFFF;
    mask = currowsz - 1;
    addr &= ~mask;
    ptr = pgbuf_lookup(addr);
    if (ptr)
        return ptr;
    ptr = pgbuf_create();
    ptr->pg_addr = addr;
    ptr->pg_next = pgbuf_top;
    pgbuf_top = ptr;
    return ptr;
}

uint32_t pgbuf_getmask()
{
    uint32_t mask;

    mask = currowsz - 1;
    return mask;
}
