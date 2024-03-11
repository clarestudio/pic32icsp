#include <inttypes.h>
#include "picdev.h"

picdev_t const picdevtab[] =
{
    {
        .name   = "PIC32MX250",
	.idcode = 0x04D00053,
	.rowsz  = 128,
	.romsz  = 1024 * 128,
	.ramsz  = 1024 * 32,
	.bootsz = 1024 * 3,
    },
    {
        .name   = "PIC32MX230",
	.idcode = 0x04D01053,
	.rowsz  = 128,
	.romsz  = 1024 * 64,
	.ramsz  = 1024 * 16,
	.bootsz = 1024 * 3,
    },
    {
	.name   = "PIC32MX220",
	.idcode = 0x04A00053,
	.rowsz  = 128,
	.romsz  = 1024 * 32,
	.ramsz  = 1024 * 8,
	.bootsz = 1024 * 3,
    },
    {
	.name   = "PIC32MX210",
	.idcode = 0x04A01053,
	.rowsz  = 128,
	.romsz  = 1024 * 16,
	.ramsz  = 1024 * 4,
	.bootsz = 1024 * 3,
    },
    {
        .name = "unknown",
	.idcode = 0,
    }
};

picdev_t const *picdev_lookup(uint32_t idcode)
{
    picdev_t const *p;

    /* mask chip revision */
    idcode &= 0x0FFFFFFF;
    /* linear search */
    for (p=picdevtab; p->idcode; p++)
        if (p->idcode == idcode)
	    return p;
    return p;
}
