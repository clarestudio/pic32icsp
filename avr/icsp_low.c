/*
 * ICSP physical layer functions
 */

/*
 * delay approx 8 clocks of 8MHz
 */
static void
icsp_delay_bit()
{
    uint8_t i;

    for (i=0; i<4; i++)
        __asm__("nop");
}

/*
 * special I/O function to send data in MSB first
 */
static void
icsp_send_magic(uint8_t byte)
{
    uint8_t i;

    for (i=0; i<8; i++)
    {
        if (byte & 0x80)
	{
	    pin_data_high();
	    icsp_delay_bit();
	    pin_clock_high();
	    icsp_delay_bit();
	    pin_clock_low();
	}
	else
	{
	    pin_data_low();
	    icsp_delay_bit();
	    pin_clock_high();
	    icsp_delay_bit();
	    pin_clock_low();
	}
	byte <<= 1;
    }
}

static void
icsp_enter()
{
    pin_reset_low();
    pin_reset_output();
    pin_clock_low();
    pin_clock_output();
    pin_data_low();
    pin_data_output();
    pin_reset_high();
    icsp_delay_bit();
    pin_reset_low();
    icsp_send_magic(0x4D);
    icsp_send_magic(0x43);
    icsp_send_magic(0x48);
    icsp_send_magic(0x50);
    pin_data_low();
    pin_reset_high();
}

static void
icsp_leave()
{
    pin_reset_low();
    pin_reset_output();
    icsp_delay_bit();
    pin_clock_input();
    pin_data_input();
    pin_reset_high();
    pin_reset_input();
}

static uint8_t icsp_last_tdo;

/*
 * generic JTAG over ICSP I/O function
 *
 * cmd: TMS bits sent by LSB first
 * dat: TDO bits sent by LSB first
 * bc: bit count
 * return: TDI bits received by LSB first
 */
static uint8_t
icsp_io(uint8_t cmd, uint8_t dat, uint8_t bc)
{
    uint8_t i;
    uint8_t rv;

    rv = icsp_last_tdo & 0x80;
    for (i=0; i<bc; i++)
    {
        /*
	 * TDI
	 */
        pin_clock_high();
	if (dat & 1)
	    pin_data_high();
        else
	    pin_data_low();
        pin_data_output();
	icsp_delay_bit(); /* setup */
        pin_clock_low(); /* shift */
	icsp_delay_bit(); /* hold */
	dat >>= 1;

        /*
	 * TMS
	 */
        pin_clock_high();
        if (cmd & 1)
            pin_data_high();
        else
            pin_data_low();
        icsp_delay_bit(); /* setup */
        pin_clock_low(); /* shift */
        icsp_delay_bit(); /* hold */
	cmd >>= 1;
	
	/*
	 * TDO
	 */
        pin_clock_high();
	pin_data_input();
	pin_clock_low(); /* gate on */
	icsp_delay_bit();
        pin_clock_high(); /* center */
	if ((i+1) < bc)
	{
	    rv >>= 1;
	    if (pin_read() & (1<<P_DATA))
	        rv |= 0x80;
	}
	else
	{
	    if (pin_read() & (1<<P_DATA))
	        icsp_last_tdo |= 0x80;
            else
	        icsp_last_tdo &= ~0x80;
	}
        pin_clock_low(); /* gate off */
    }
    for (i=0; i<8-bc; i++)
        rv >>= 1;
    return rv;
}
