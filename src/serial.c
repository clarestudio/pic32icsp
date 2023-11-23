/*
 * POSIX termios handler
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "serial.h"

static char const *devfn;
static int devfd;
static int devbaud;
static struct termios devsave;

void serial_set_fname(char const *name)
{
    devfn = name;
}

void serial_baud(int baud)
{
    devbaud = baud;
}

int serial_open(void)
{
    int fd;
    int rv;

    if (devfd >= 0)
        serial_close();
    if (devbaud == 0)
        devbaud = 38400; /* the default */

    fd = open(devfn, O_RDWR);
    if (fd == -1)
    {
        perror(devfn);
	devfd = -1;
	goto bad;
    }
    devfd = fd;
    rv = serial_save();
    if (rv)
    {
        perror(devfn);
	goto bad1;
    }
    serial_init();
    return 0;

bad1:
    close(devfd);
    devfd = -1;
bad:
    return -1;
}

int serial_close(void)
{
    int fd;

    fd = devfd;
    if (fd >= 0)
    {
        serial_restore();
        close(fd);
	devfd = -1;
    }
    return 0;
}

int serial_save()
{
    int rv;

    rv = tcgetattr(devfd, &devsave);
    return rv;
}

int serial_restore()
{
    int rv;

    rv = tcsetattr(devfd, TCSADRAIN, &devsave);
    return rv;
}

int serial_init()
{
    struct termios ios;

    memset(&ios, 0, sizeof(ios));
    cfsetspeed(&ios, devbaud);
    ios.c_cflag |= CS8|CLOCAL|CREAD;
    ios.c_cc[VMIN] = 1;
    ios.c_cc[VTIME] = 0;
    tcsetattr(devfd, TCSADRAIN, &ios);
    return 0;
}

int serial_dtr_set(int val)
{
    unsigned int v;

    ioctl(devfd, TIOCMGET, &v);
    if (val)
        v |= TIOCM_DTR;
    else
        v &= ~TIOCM_DTR;
    ioctl(devfd, TIOCMSET, &v);
    return val;
}

int serial_dsr_get(void)
{
    unsigned int v;
    
    ioctl(devfd, TIOCMGET, &v);
    return v&TIOCM_DSR;
}

int serial_rts_set(int val)
{
    unsigned int v;

    ioctl(devfd, TIOCMGET, &v);
    if (val)
        v |= TIOCM_RTS;
    else
        v &= ~TIOCM_RTS;
    ioctl(devfd, TIOCMSET, &v);
    return val;
}

int serial_cts_get(void)
{
    unsigned int v;
    
    ioctl(devfd, TIOCMGET, &v);
    return v&TIOCM_CTS;
}

int serial_cd_get(void)
{
    unsigned int v;
    
    ioctl(devfd, TIOCMGET, &v);
    return v&TIOCM_CD;
}

int serial_ri_get(void)
{
    unsigned int v;

    ioctl(devfd, TIOCMGET, &v);
    return v&TIOCM_RI;
}

int serial_putc(unsigned char ch)
{
    int rv;

    rv = write(devfd, &ch, sizeof(ch));
    if (rv < 0)
        return -1;
    return 0;
}

int serial_getc(void)
{
    int rv;
    uint8_t p;

    rv = read(devfd, &p, sizeof(p));
    if (rv < 0)
        return -1;
    return p;
}

int serial_putmem(void const *buf, size_t len)
{
    int rv;
    int left = len;
    uint8_t const *ptr = buf;

    while (left > 0)
    {
        rv = write(devfd, ptr, left);
        if (rv == -1)
            return -1;
        if (rv == 0)
            return -1;
        left -= rv;
	ptr += rv;
    }
    return 0;
}

int serial_getmem(void *buf, size_t len)
{
    int rv;
    int left = len;
    uint8_t *ptr = buf;

    while (left > 0)
    {
        rv = read(devfd, ptr, left);
        if (rv == -1)
            return -1;
        if (rv == 0)
	    return -1;
        left -= rv;
	ptr += rv;
    }
    return 0;
}
