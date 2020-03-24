# pic32icsp

PIC32MX2 programming software via 2-wire ICSP protocol

example hardware image  
![photo1]

# hardware

you need a ATtiny2313 or similar MCU for ICSP adapter.
the factory default fuse value can be used as of my experience.

* pin 20 (Vcc) -- 3.3V power supply
* pin 10 (GND) -- common ground
* pin 2 (RxD) -- upstream serial port (connec to the PC)
* pin 3 (TxD) -- upstream serial port (connec to the PC)
* pin 14 (PB2) -- MCLR# of the target PIC32MX
* pin 13 (PB1) -- PGEC of the target PIC32MX
* pin 12 (PB0) -- PGED of the target PIC32MX

[photo1]: https://www.clarestudio.org/elec/pic32/pic32prog-hw.jpg

# build

to obtain the fimrware binary, you need gmake, avr-binutils, avr-gcc, and avr-libc.
just type make all.
