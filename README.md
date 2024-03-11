# pic32icsp

This is a very tiny, low-cost tool to program flash memory of
the Microchip PIC32MX2xx series MCUs via 2-wire ICSP protocol.

## Status

This is still in an experimental project,
there is no compatibility plan(s) between versions.

## Target Host OS

* NetBSD/amd64 and/or other little endian, 64bit architecture
* GNU/Linux, little endian, 64bit architecture

## Target Devices

* PIC32MX210 (PIC32MX210F016B)
* PIC32MX220 (PIC32MX220F032B)
* PIC32MX230 (PIC32MX230F064B)
* PIC32MX250 (PIC32MX250F128B)

PIC32MX270 was planned, but not yet done.

## Bill of Materials

* Atmel/Microchip ATtiny2313 microcontroller.
I think you can use attiny2313a or attiny4313 instead of attiny2313.
I used attiny2313 simply because it was in stock.
* 9.216MHz quartz crystal timing device.
* two 22pF ceramic capacitors for quartz crystal.
* some 0.1uF ceramic capacitors to bypassing of power rail.
* 3.3V LDO voltage regulator device and reasonable bypass capacitors.
* 3.3V I/O compatible USB-UART module.
* your favorite connectors and wires.
* your favorite universal board.
* your favorite PIC32MX2xx device as a target.

## Hardware wirings

ATtiny2313 connections:

* pin 20 (Vcc) -- 3.3V power supply
* pin 10 (GND) -- common ground
* pin 2 (RxD) -- upstream serial port (connect to the USB UART module)
* pin 3 (TxD) -- upstream serial port (connect to the USB UART module)
* pin 4 (XTAL2) -- connect quartz crystal and a load capacitor (may be 22pF)
* pin 5 (XTAL1) -- connect quartz crystal and a load capacitor (may be 22pF)
* pin 14 (PB2) -- connect to MCLR# of the target PIC32MX
* pin 13 (PB1) -- connect to PGEC of the target PIC32MX
* pin 12 (PB0) -- connect to PGED of the target PIC32MX

## How to build firmware

To obtain the firmware binary image,
you need gmake, avr-binutils, avr-gcc, avr-libc, and avrdude.
Adjust Makefiles and run (g)make.

Suggested fuse bits for ATtiny2313:
* efuse = 0xFF
* hfuse = 0xDF
* lfuse = 0x6F

## How to build host program

To obtain the host program binary, you need gmake and gcc.
Adjust Makefiles and run (g)make.

The versions of the controller and host programs should be matched.

## How to use

To write a program file "target.hex" via host serial port "ttyUSB01":

./pic32icsp -d /dev/ttyUSB01 -w target.hex
