/* $Id: main.c,v 1.23 2017/03/26 11:53:59 clare Exp $ */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd.h"
#include "tap.h"
#include "serial.h"
#include "icsp.h"
#include "ihex.h"
#include "writer.h"

void
usage(void)
{
    printf("This is pic32icsp version 0.3\n");
    printf("The ICSP programmer for the Microchip PIC32MX[12]xx devices.\n");
    printf("Copyright (C) 2016, 2020 clare.  All rights reserved.\n");
    printf("See https://www.clarestudio.org/elec/pic32 for details.\n");
    printf("\n");
    printf("pic32prog -d <tty-device> [options]\n");
    printf("    -b baud     ... set communication speed\n");
    printf("    -e          ... just erase the chip.\n");
    printf("    -w file.hex ... load hex-file, erase the chip, and write it.\n");
    printf("    -v          ... verbose printing\n");
}

int
main(int argc, char **argv)
{
    int ch, rv;
    uint8_t ver;
    uint8_t eflag, dflag, wflag, vflag;

    dflag = 0;
    eflag = 0;
    wflag = 0;
    vflag = 0;
    while (ch=getopt(argc, argv, "d:b:ew:v"), ch!=-1)
    {
        switch (ch)
	{
	case 'd':
	    serial_set_fname(optarg);
	    dflag = 1;
	    break;
        case 'b':
	    serial_baud(atoi(optarg));
	    break;
        case 'e':
	    eflag = 1;
	    break;
        case 'w':
	    wflag = 1;
	    eflag = 1;
	    ihex_set_fname(optarg);
	    rv = ihex_open();
	    if (rv)
	        goto bad;
            printf("parsing hex file\n");
            rv = ihex_parse();
	    if (rv)
	    {
	        ihex_close();
		goto bad;
	    }
	    if (vflag)
	        rv = ihex_dump();
	    ihex_close();
	    break;
        case 'v':
	    vflag = 1;
	    break;
	}
    }
    if (!dflag)
    {
        usage();
	return 0;
    }
    printf("serial open\n");
    rv = serial_open();
    if (rv)
    {
        fprintf(stderr, "open failed\n");
        goto bad;
    }
    rv = cmd_nop();
    if (rv)
    {
        fprintf(stderr, "bad device programmer\n");
	goto bad1;
    }
    ver = cmd_version();
    printf("programmer protocol version %d\n", ver);
    if (ver != 2)
    {
        printf("please check version of programmer\n");
	goto bad1;
    }
    printf("entering ICSP mode\n");
    icsp_enter();
    icsp_tap_idle();
    rv = icsp_check_idcode();
    if (rv)
        goto bad2;
    if (tap_status())
    {
        fprintf(stderr, "bad device or bad communication\n");
	goto bad2;
    }
    if (eflag)
        icsp_erase_chip();
    icsp_enter_serial_exec();
    if (wflag)
    {
        rv = writer_all();
	if (rv)
	    goto bad2;
    }

    printf("leaving ICSP mode\n");
    icsp_leave();
    printf("serial close\n");
    serial_close();
    return 0;

bad2:
    printf("leaving ICSP mode\n");
    cmd_icsp_leave();
bad1:
    printf("serial close\n");
    serial_close();
bad:
    return 1;
}
