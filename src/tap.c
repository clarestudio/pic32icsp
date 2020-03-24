#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "icsp.h"
#include "tap.h"
#include "tapdef.h"

int
tap_status(void)
{
    uint8_t buf;

    icsp_tap_idle();
    icsp_command(MTAP_COMMAND);
    buf = icsp_xfer_byte(MCHP_STATUS);
    printf("MTAP status: %02x", buf);
    if ((buf & 0b10000000) == 0)
        printf(" CPS");
    if (buf & 0b1000)
        printf(" CFGRDY");
    if (buf & 0b0100)
        printf(" FCBUSY");
    printf("\n");
    if ((buf & 0b1100) != 0b1000)
	return -1;
    return 0;
}
