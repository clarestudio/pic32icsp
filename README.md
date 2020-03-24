# pic32icsp

A very tiny, low-cost tool to program the PIC32MX2 series MCUs via 2-wire ICSP protocol.

![photo1](https://www.clarestudio.org/elec/pic32/pic32prog-hw.jpg "hardware photo")

## Target OS

* NetBSD/amd64
* Fedora 31 (compilation passed, but not yet tested)

## Target MCU

* PIC32MX210
* PIC32MX220
* PIC32MX230
* PIC32MX250

## Hardware

You need a ATtiny2313.
A FT232 USB-UART module and avrdude serial-bitbang method is sufficient to program the AVR.

* pin 20 (Vcc) -- 3.3V power supply
* pin 10 (GND) -- common ground
* pin 2 (RxD) -- upstream serial port (connect to the PC UART)
* pin 3 (TxD) -- upstream serial port (connect to the PC UART)
* pin 14 (PB2) -- MCLR# of the target PIC32MX
* pin 13 (PB1) -- PGEC of the target PIC32MX
* pin 12 (PB0) -- PGED of the target PIC32MX

The factory default fuse value (8MHz internal clock) is sufficient as of my experience.

## How to build binary

To obtain the firmware binary image, you need gmake, avr-binutils, avr-gcc, and avr-libc.
Adjust Makefiles and run (g)make.

The versions of the controller and userland program should be matched.
This is experimental project,
there is no compatibility plan between versions.

## How to use

./pic32icsp -d /dev/ttyUSB01 -w target.hex
