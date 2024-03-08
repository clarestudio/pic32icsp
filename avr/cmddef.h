/* $Id: cmddef.h,v 1.7 2016/05/22 12:22:04 clare Exp $ */
/*
 * basic commands
 */
#define		CMD_NOP			0
#define		CMD_VERSION		1

/*
 * low-level control commands
 */
#define		CMD_DATA_LOW		16
#define		CMD_DATA_HIGH		17
#define		CMD_DATA_INPUT		18

#define		CMD_CLOCK_LOW		20
#define		CMD_CLOCK_HIGH		21
#define		CMD_CLOCK_INPUT		22

#define		CMD_RESET_LOW		24
#define		CMD_RESET_HIGH		25
#define		CMD_RESET_INPUT		26

/*
 * high-level control commands
 */
#define		CMD_ICSP_ENTER		32
#define		CMD_ICSP_LEAVE		33
#define		CMD_TAP_IDLE		40
#define		CMD_TAP_COMMAND		41
#define		CMD_XFER_BYTE		42
#define		CMD_XFER_DWORD		43
#define		CMD_XFER_FASTDATA	44
#define		CMD_XFER_INSN		45
#define		CMD_SET_ADDR		46
#define		CMD_READ_DWORD		47
#define		CMD_WRITE_DWORD		48
#define		CMD_REWIND		49

#define		STATUS_ACK	0x06
#define		STATUS_NAK	0x15
