/*
 * PIC32 ICSP bridge device
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "cmddef.h"
#include "tapdef.h"
#include "tty.c"
#include "pin.c"
#include "icsp_low.c"
#include "icsp_high.c"
#include "icsp.c"

int main(void)
{
    uint8_t cmd;

    CLKPR = _BV(CLKPCE);
    CLKPR = 0; /* disable CPU clock prescaler */
    ACSR  = _BV(ACD); /* disable analog comparator */
    DDRD  = 0x00; /* all input */
    PORTD = 0x00;
    DDRB  = 0x00; /* all input */
    PORTB = 0x00;

    tty_init();
    for (;;)
    {
        cmd = tty_getc();
	switch (cmd)
	{
        case CMD_NOP:
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_VERSION:
	    tty_putc(2);
	    break;

        case CMD_RESET_LOW:
	    pin_reset_low();
	    pin_reset_output();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_RESET_HIGH:
	    pin_reset_high();
	    pin_reset_output();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_RESET_INPUT:
	    pin_reset_input();
	    if (pin_read() & (1<<P_RESET))
	        tty_putc(1);
	    else
	        tty_putc(0);
	    break;

        case CMD_CLOCK_LOW:
	    pin_clock_low();
	    pin_clock_output();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_CLOCK_HIGH:
	    pin_clock_high();
	    pin_clock_output();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_CLOCK_INPUT:
	    pin_clock_input();
	    if (pin_read() & (1<<P_CLOCK))
	        tty_putc(1);
	    else
	        tty_putc(0);
	    break;

        case CMD_DATA_LOW:
	    pin_data_low();
	    pin_data_output();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_DATA_HIGH:
	    pin_data_high();
	    pin_data_output();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_DATA_INPUT:
	    pin_data_input();
	    if (pin_read() & (1<<P_DATA))
	        tty_putc(1);
	    else
	        tty_putc(0);
	    break;

        case CMD_ICSP_ENTER:
	    icsp_enter();
	    tty_putc(STATUS_ACK);
	    break;
        case CMD_ICSP_LEAVE:
	    icsp_leave();
	    tty_putc(STATUS_ACK);
	    break;

	case CMD_TAP_IDLE:
	    icsp_tap_idle();
	    tty_putc(STATUS_ACK);
	    break;
	case CMD_TAP_COMMAND:
	    {
	        uint8_t cmd;

		cmd = tty_getc();
		icsp_tap_command(cmd);
	    }
	    tty_putc(STATUS_ACK);
	    break;
	case CMD_XFER_BYTE:
	    {
	        uint8_t dat;
		uint8_t rv;

		dat = tty_getc();
		rv = icsp_xfer_byte(dat);
		tty_putc(rv);
	    }
	    break;
	case CMD_XFER_DWORD:
	    {
	        uint8_t d0, d1, d2, d3;
		uint8_t r0, r1, r2, r3;
		uint32_t dat, rv;

		d0 = tty_getc();
		d1 = tty_getc();
		d2 = tty_getc();
		d3 = tty_getc();
		dat = ((uint32_t)d0 <<  0)
		    | ((uint32_t)d1 <<  8)
		    | ((uint32_t)d2 << 16)
		    | ((uint32_t)d3 << 24)
		    ;
                rv = icsp_xfer_dword(dat);
		r0 = rv >>  0;
		r1 = rv >>  8;
		r2 = rv >> 16;
		r3 = rv >> 24;
		tty_putc(r0);
		tty_putc(r1);
		tty_putc(r2);
		tty_putc(r3);
	    }
	    break;

        case CMD_XFER_FASTDATA:
	    {
	        uint8_t d0, d1, d2, d3;
		uint8_t r0, r1, r2, r3;
		uint32_t dat, rv;

		d0 = tty_getc();
		d1 = tty_getc();
		d2 = tty_getc();
		d3 = tty_getc();
		dat = ((uint32_t)d0 <<  0)
		    | ((uint32_t)d1 <<  8)
		    | ((uint32_t)d2 << 16)
		    | ((uint32_t)d3 << 24)
		    ;
                rv = icsp_xfer_fastdata(dat);
		r0 = rv >>  0;
		r1 = rv >>  8;
		r2 = rv >> 16;
		r3 = rv >> 24;
		tty_putc(r0);
		tty_putc(r1);
		tty_putc(r2);
		tty_putc(r3);
	    }
	    break;

        case CMD_XFER_INSN:
	    {
	        uint8_t d0, d1, d2, d3;
		uint32_t dat;

		d0 = tty_getc();
		d1 = tty_getc();
		d2 = tty_getc();
		d3 = tty_getc();
		dat = ((uint32_t)d0 <<  0)
		    | ((uint32_t)d1 <<  8)
		    | ((uint32_t)d2 << 16)
		    | ((uint32_t)d3 << 24)
		    ;
                icsp_xfer_insn(dat);
		tty_putc(STATUS_ACK);
	    }
	    break;

        case CMD_SET_ADDR:
	    {
	        uint8_t d0, d1, d2, d3;
		uint32_t dat;

		d0 = tty_getc();
		d1 = tty_getc();
		d2 = tty_getc();
		d3 = tty_getc();
		dat = ((uint32_t)d0 <<  0)
		    | ((uint32_t)d1 <<  8)
		    | ((uint32_t)d2 << 16)
		    | ((uint32_t)d3 << 24)
		    ;
                icsp_set_addr(dat);
		tty_putc(STATUS_ACK);
	    }
	    break;

	case CMD_REWIND:
	    {
		icsp_rewind();
		tty_putc(STATUS_ACK);
	    }

        case CMD_READ_DWORD:
	    {
		uint8_t r0, r1, r2, r3;
		uint32_t rv;

                rv = icsp_read_dword();
		r0 = rv >>  0;
		r1 = rv >>  8;
		r2 = rv >> 16;
		r3 = rv >> 24;
		tty_putc(r0);
		tty_putc(r1);
		tty_putc(r2);
		tty_putc(r3);
	    }
	    break;

        case CMD_WRITE_DWORD:
	    {
	        uint8_t d0, d1, d2, d3;
		uint32_t dat;

		d0 = tty_getc();
		d1 = tty_getc();
		d2 = tty_getc();
		d3 = tty_getc();
		dat = ((uint32_t)d0 <<  0)
		    | ((uint32_t)d1 <<  8)
		    | ((uint32_t)d2 << 16)
		    | ((uint32_t)d3 << 24)
		    ;
                icsp_write_dword(dat);
		tty_putc(STATUS_ACK);
	    }
	    break;

        default:
	    tty_putc(STATUS_NAK);
	    break;
	}
    }
    return 0;
}
