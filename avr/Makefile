# $Id: Makefile,v 1.8 2016/05/21 10:27:28 clare Exp $
# Makefile

MCU?=attiny2313
PROG=pic32icsp-${MCU}
CFLAGS=-Os -mmcu=${MCU} -Wall -DF_CPU=9216000L

SRCS=main.c
OBJS=${SRCS:%.c=%.o}

TOOLDIR=/usr/cross
# for Fedora 31 with avr-gcc package
#TOOLDIR=/usr

CC=${TOOLDIR}/bin/avr-gcc
OBJDUMP=${TOOLDIR}/bin/avr-objdump
OBJCOPY=${TOOLDIR}/bin/avr-objcopy
SIZE=${TOOLDIR}/bin/avr-size
AVRDUDE=${TOOLDIR}/bin/avrdude

.SUFFIXES: .c .o .S .s

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<

.c.s:
	${CC} ${CFLAGS} -S -o $@ $<

.S.o:
	${CC} ${CFLAGS} -c -o $@ $<

.s.o:
	${CC} ${CFLAGS} -c -o $@ $<

all: ${PROG}.elf ${PROG}.hex

${PROG}.elf: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS}
	LANG=C ${OBJDUMP} -d $@ > ${PROG}.dump
	@echo >> ${PROG}.dump
	LANG=C ${SIZE} $@ >> ${PROG}.dump

${PROG}.hex: ${PROG}.elf
	${OBJCOPY} -I elf32-avr -O ihex ${PROG}.elf ${PROG}.hex

clean:
	rm -f *.s *.o *.elf *.dump *.hex

write:
	${AVRDUDE} -P /dev/dtyU0 -c ft232serbb -p ${MCU} -U flash:w:${PROG}.hex

fuse-attiny2313:
	${AVRDUDE} -P /dev/dtyU0 -c ft232serbb -p ${MCU} \
		-U efuse:w:0xFF:m \
		-U hfuse:w:0xDF:m \
		-U lfuse:w:0x6F:m \

main.o: main.c tty.c pin.c icsp.c cmddef.h tapdef.h
