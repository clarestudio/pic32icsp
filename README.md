# pic32icsp

Program PIC32MX via 2-wire ICSP protocol.
This program uses a ATtiny2313 for ICSP adapter.

![photo1]
[photo1]: https://www.clarestudio.org/elec/pic32/pic32prog-hw.jpg

# hardware

You need a ATtiny2313 or similar MCU for ICSP adapter.
The factory default fuse value (8MHz interal clock) can be used as of my experience.

* pin 20 (Vcc) -- 3.3V power supply
* pin 10 (GND) -- common ground
* pin 2 (RxD) -- upstream serial port (connected to the PC UART)
* pin 3 (TxD) -- upstream serial port (connected to the PC UART)
* pin 14 (PB2) -- MCLR# of the target PIC32MX
* pin 13 (PB1) -- PGEC of the target PIC32MX
* pin 12 (PB0) -- PGED of the target PIC32MX

# build software

To obtain the firmware binary image, you need gmake, avr-binutils, avr-gcc, and avr-libc.
Just type make all.

The versions of the controller and userland program should be matched.
There is no compatibility plan between versions.

# how to use

./pic32icsp -d /dev/ttyUSB01 -w target.hex
