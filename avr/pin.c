/* $Id: pin.c,v 1.7 2016/05/21 13:20:59 clare Exp $ */
/*
 * pin-layout of ATtiny2313
 * 01	RESET#		tied to ISP
 * 02	RxD		tied to UART
 * 03	TxD		tied to UART
 * 04	XTAL2		tied to 8MHz quarts
 * 05	XTAL1		tied to 8MHz quarts
 * 06	INT0		-
 * 07	INT1		-
 * 08	PD4		-
 * 09	PD5		-
 * 10	GND		ground, tied to ISP
 *
 * 20	Vcc		+3.3V power supply, tied to ISP
 * 19	PB7		SCK for ISP
 * 18	PB6		MISO for ISP
 * 17	PB5		MOSI for ISP
 * 16	PB4		-
 * 15	PB3		-
 * 14	PB2		RESET for PIC32
 * 13	PB1		PGEC for PIC32
 * 12	PB0		PGED for PIC32
 * 11	PD6		-
 */

#define		P_RESET		PB2
#define		P_CLOCK		PB1
#define		P_DATA		PB0

static uint8_t
pin_read()
{
    return PINB;
}

static void
pin_reset_high()
{
    PORTB |= 1<<P_RESET;
}

static void
pin_reset_low()
{
    PORTB &= ~(1<<P_RESET);
}

static void
pin_reset_output()
{
    DDRB |= 1<<P_RESET;
}

static void
pin_reset_input()
{
    DDRB &= ~(1<<P_RESET);
    PORTB &= ~(1<<P_RESET); /* release pull-up */
}

static void
pin_clock_high()
{
    PORTB |= 1<<P_CLOCK;
}

static void
pin_clock_low()
{
    PORTB &= ~(1<<P_CLOCK);
}

static void
pin_clock_output()
{
    DDRB |= 1<<P_CLOCK;
}

static void
pin_clock_input()
{
    DDRB &= ~(1<<P_CLOCK);
    PORTB &= ~(1<<P_CLOCK); /* release pull-up */
}

static void
pin_data_high()
{
    PORTB |= 1<<P_DATA;
}

static void
pin_data_low()
{
    PORTB &= ~(1<<P_DATA);
}

static void
pin_data_output()
{
    DDRB |= 1<<P_DATA;
}

static void
pin_data_input()
{
    DDRB &= ~(1<<P_DATA);
    PORTB &= ~(1<<P_DATA); /* release pull-up */
}
