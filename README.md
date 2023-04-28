# pic32icsp

A very tiny, low-cost tool to program the PIC32MX2 series MCUs via 2-wire ICSP protocol.

## Target Host OS

* NetBSD/amd64 or other 64bit architecture
* GNU/Linux for 64bit architecture

## Target Devices

* PIC32MX210
* PIC32MX220
* PIC32MX230
* PIC32MX250

PIC32MX270 was planned, but not yet done.

## Hardware

You need a ATtiny2313 as a device programmer.

* pin 20 (Vcc) -- 3.3V power supply
* pin 10 (GND) -- common ground
* pin 2 (RxD) -- upstream serial port (connect to the PC UART)
* pin 3 (TxD) -- upstream serial port (connect to the PC UART)
* pin 14 (PB2) -- MCLR# of the target PIC32MX
* pin 13 (PB1) -- PGEC of the target PIC32MX
* pin 12 (PB0) -- PGED of the target PIC32MX

AVR ISP pins are reserved and not connected as a device programmer.
The fuse bits I used is the factory default.

## How to build firmware

To obtain the firmware binary image,
you need gmake, avr-binutils, avr-gcc, and avr-libc.
Adjust Makefiles and run (g)make.

The versions of the controller and userland program should be matched.
This is experimental project,
there is no compatibility plan between versions.

## How to use

./pic32icsp -d /dev/ttyUSB01 -w target.hex
