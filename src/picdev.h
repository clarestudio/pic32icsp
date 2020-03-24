/* $Id: picdev.h,v 1.3 2016/05/22 04:04:55 clare Exp $ */

typedef struct picdev picdev_t;

struct picdev
{
    char const *name;
    uint32_t idcode;
    uint32_t rowsz;
    uint32_t romsz;
    uint32_t bootsz;
    uint32_t ramsz;
};

picdev_t const *picdev_lookup(uint32_t idcode);
