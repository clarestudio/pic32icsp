/* $Id: pgbuf.h,v 1.3 2016/05/19 06:26:35 clare Exp $ */

typedef struct pgbuf pgbuf_t;

struct pgbuf
{
    struct pgbuf *pg_next;
    uint8_t *pg_buf;
    uint32_t pg_addr;
};

extern pgbuf_t *pgbuf_top;
pgbuf_t *pgbuf_alloc(uint32_t addr);
uint32_t pgbuf_getmask(void);
