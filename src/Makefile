CC=gcc
INSTALL=/usr/bin/install
BINDIR=/usr/local/bin
PROG=pic32icsp
NOMAN=1
SRCS=main.c serial.c cmd.c tap.c icsp.c picdev.c pgbuf.c ihex.c writer.c
OBJS=$(SRCS:%.c=%.o)
CFLAGS+=-Wall -O3
LDFLAGS=

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

install: $(PROG)
	mkdir -p $(BINDIR)
	$(INSTALL) -m 444 -o root -g wheel $(PROG) $(BINDIR)

clean:
	rm -f $(PROG)
	rm -f $(OBJS)

test:
	./${PROG} -d /dev/dtyU0

test1:
	./${PROG} -d /dev/dtyU0 -e -w /var/tmp/leddrv.hex
