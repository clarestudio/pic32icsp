# pic32icsp

This is a very tiny, low-cost tool to program
the Microchip PIC32MX2xx series MCUs via 2-wire ICSP protocol.

You can eliminate buying PICkit or similar costly vendor tools for PIC32MX210/220/230/250.
You can program the microcontroller from the NetBSD, other than Linux and/or Windows.

## Target Host OS

* NetBSD/amd64 and/or other 64bit architecture
* GNU/Linux for 64bit architecture

## Target Devices

* PIC32MX210 (PIC32MX210F016B)
* PIC32MX220 (PIC32MX220F032B)
* PIC32MX230 (PIC32MX230F064B)
* PIC32MX250 (PIC32MX250F128B)

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

AVR-ISP pins (a.k.a. SPI-bus pins) are reserved and not connected/used as a device programmer.
The fuse bits I used is the factory default.

If you caught communication stability problems,
you can attach a quartz crystal device of 8MHz with adjusted fuse bits.

## How to build firmware

To obtain the firmware binary image,
you need gmake, avr-binutils, avr-gcc, and avr-libc.
Adjust Makefiles and run (g)make.

## How to build host program

To obtain the host program binary,
you need gmake and gcc.
Adjust Makefiles and run (g)make.

The versions of the controller and host program should be matched.
This is experimental project,
there is no compatibility plan(s) between versions.

## How to use

To write a program file "target.hex" via host serial port "ttyUSB01":

./pic32icsp -d /dev/ttyUSB01 -w target.hex
