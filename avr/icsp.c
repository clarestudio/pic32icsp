/* $Id: icsp.c,v 1.16 2016/05/22 08:13:34 clare Exp $ */

uint32_t addrreg;

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

static void
icsp_send_msb(uint8_t byte)
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
    icsp_send_msb(0x4D);
    icsp_send_msb(0x43);
    icsp_send_msb(0x48);
    icsp_send_msb(0x50);
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

uint8_t icsp_last_tdo;

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

/*
 * Send TAP state to idle
 */
static void
icsp_tap_idle(void)
{
    uint8_t cmd = 0b011111;
    uint8_t dat = 0;
    uint8_t bc = 6;

    icsp_io(cmd, dat, bc);
}

/*
 * Send 5-bit MTAP/ETAP command
 */
static void
icsp_tap_command(uint8_t bits)
{
    uint8_t cmd;
    uint8_t dat;
    uint8_t bc;

    cmd = 0b0011;
    dat = 0;
    bc = 4;
    icsp_io(cmd, dat, bc);

    cmd = 0b10000;
    dat = bits;
    bc = 5;
    icsp_io(cmd, dat, bc);

    cmd = 0b01;
    dat = 0;
    bc = 2;
    icsp_io(cmd, dat, bc);
}

static uint8_t
icsp_xfer_byte(uint8_t data)
{
    uint8_t rv;

    icsp_io(0b001, 0, 3); /* start */
    rv = icsp_io(0b10000000, data, 8); /* xfer */
    icsp_io(0b01, 0, 2); /* finish */
    return rv;
}

static uint32_t
icsp_xfer_dword(uint32_t data)
{
    uint8_t d0, d1, d2, d3;
    uint8_t r0, r1, r2, r3;
    uint32_t rv;

    d0 = data >>  0;
    d1 = data >>  8;
    d2 = data >> 16;
    d3 = data >> 24;
    icsp_io(0b001, 0, 3); /* start */
    r0 = icsp_io(0b00000000, d0, 8);
    r1 = icsp_io(0b00000000, d1, 8);
    r2 = icsp_io(0b00000000, d2, 8);
    r3 = icsp_io(0b10000000, d3, 8);
    icsp_io(0b01, 0, 2); /* finish */
    rv = ((uint32_t)r0 <<  0)
       | ((uint32_t)r1 <<  8)
       | ((uint32_t)r2 << 16)
       | ((uint32_t)r3 << 24)
       ;
    return rv;
}

static uint32_t
icsp_xfer_fastdata(uint32_t data)
{
    uint8_t d0, d1, d2, d3;
    uint8_t r0, r1, r2, r3;
    uint32_t rv;
    uint8_t flag;
    uint8_t limit;

    d0 = data >>  0;
    d1 = data >>  8;
    d2 = data >> 16;
    d3 = data >> 24;
    limit = 0;
    do {
        icsp_io(0b001, 0, 3); /* start */
        flag = icsp_io(0, 0, 1);
        r0 = icsp_io(0b00000000, d0, 8);
        r1 = icsp_io(0b00000000, d1, 8);
        r2 = icsp_io(0b00000000, d2, 8);
        r3 = icsp_io(0b10000000, d3, 8);
        icsp_io(0b01, 0, 2); /* finish */
	if (--limit == 0)
	    break;
    } while ((flag & 1) == 0);
    rv = ((uint32_t)r0 <<  0)
       | ((uint32_t)r1 <<  8)
       | ((uint32_t)r2 << 16)
       | ((uint32_t)r3 << 24)
       ;
    return rv;
}

static void
icsp_xfer_insn(uint32_t insn)
{
    uint32_t cv;
    uint8_t limit;

    icsp_tap_command(ETAP_CONTROL);
    limit = 0;
    do {
        cv = icsp_xfer_dword(0x0004C000L);
    } while ((cv & (1L<<18)) == 0 || --limit == 0); /* wait for PrAcc == 1 */
    icsp_tap_command(ETAP_DATA);
    icsp_xfer_dword(insn);
    icsp_tap_command(ETAP_CONTROL);
    icsp_xfer_dword(0x0000C000L); /* exec CPU */
}

static void
icsp_set_addr(uint32_t addr)
{
    addrreg = addr;
}

static uint32_t
icsp_read_dword(void)
{
    uint32_t insn;
    uint32_t data;

    /*
     * lui s3, DMSEG
     */
    insn = 0x3C130000;
    insn |= 0xFF20; /* DMSEG */
    icsp_xfer_insn(insn);

    /*
     * lui t0, hi(addr)
     * ori t0, lo(addr)
     */
    insn = 0x3C080000;
    insn |= (addrreg >> 16);
    icsp_xfer_insn(insn);
    insn = 0x35080000;
    insn |= (addrreg & 0xFFFF);
    icsp_xfer_insn(insn);
    addrreg += sizeof(uint32_t);

    /*
     * lw t1,(t0)
     * sw t1,(s3)
     * nop
     */
    insn = 0x8D090000;
    icsp_xfer_insn(insn);
    insn = 0xAE690000;
    icsp_xfer_insn(insn);
    insn = 0x00000000;
    icsp_xfer_insn(insn);

    icsp_tap_command(ETAP_FASTDATA);
    data = icsp_xfer_fastdata(0);

    return data;
}

static void
icsp_write_dword(uint32_t data)
{
    uint32_t insn;

    /*
     * lui s0,hi(addr)
     */
    insn = 0x3C100000;
    insn |= addrreg >> 16;
    icsp_xfer_insn(insn);

    /*
     * lui t0,hi(data)
     * ori t0,lo(data)
     * sw  t0,(s0 + offset)
     */
    insn = 0x3C080000;
    insn |= (data >> 16);
    icsp_xfer_insn(insn);
    insn = 0x35080000;
    insn |= (data & 0xFFFF);
    icsp_xfer_insn(insn);
    insn = 0xAE080000;
    insn |= (addrreg & 0xFFFF);
    icsp_xfer_insn(insn);
    addrreg += sizeof(uint32_t);
}
