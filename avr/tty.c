/* $Id: tty.c,v 1.4 2016/05/08 16:11:19 clare Exp $ */

static inline __attribute__((always_inline)) void
tty_init()
{
    uint16_t  rate;

    /* speed is 115200bps */
    rate = (F_CPU/115200/16) - 1;
    UBRRH = rate >> 8;
    UBRRL = rate;
    /*
     * asynchronous, non-parity, 1-bit stop, 8-bit data,
     * default polarity
     */
    UCSRC = 0b00000110;
    /*
     * receiver enable (force RXD pin is being input),
     * transmitter enable (force TXD pin is being output).
     */
    UCSRB = _BV(RXEN)|_BV(TXEN);
}

static uint8_t
tty_getc()
{
    while ((UCSRA & _BV(RXC)) == 0)
        ;
    return UDR;
}

static void
tty_putc(uint8_t data)
{
    while ((UCSRA & _BV(UDRE)) == 0)
        ;
    UDR = data;
}

#if 0
static void
tty_puts(uint8_t __flash const *ptr)
{
    uint8_t ch;
    
    while (ch = *ptr++, ch != 0)
        tty_putc(ch);
}
#endif
