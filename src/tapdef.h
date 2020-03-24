/* $Id: tapdef.h,v 1.2 2016/05/15 16:05:28 clare Exp $ */
/*
 * MTAP instruction register has 5bit width
 */
#define	TAP_HIGHZ		0x00
#define	TAP_IDCODE		0x01
#define	TAP_SAMPLE		0x02
#define	MTAP_SW_MTAP		0x04
#define	MTAP_SW_ETAP		0x05
#define	TAP_EXTEST		0x06
#define	MTAP_COMMAND		0x07
#define	TAP_BYPASS		0x1F

/*
 * ETAP instruction register has 5bit width
 */
/*	ETAP_UNUSED		0x00 */
#define	ETAP_IDCODE		0x01
/*	ETAP_UNUSED		0x02 */
#define	ETAP_IMPCODE		0x03
#define	ETAP_SW_MTAP		0x04
#define	ETAP_SW_ETAP		0x05
#define	ETAP_ADDRESS		0x08
#define	ETAP_DATA		0x09
#define	ETAP_CONTROL		0x0A
#define	ETAP_ALL		0x0B
#define	ETAP_EJTAGBOOT		0x0C
#define	ETAP_NORMALBOOT		0x0D
#define	ETAP_FASTDATA		0x0E
#define	ETAP_BYPASS		0xFF

/*
 * MTAP has 8bit command data register
 */
#define	MCHP_STATUS		0x00
#define MCHP_DE_ASSERT_RST	0xD0
#define	MCHP_ASSERT_RST		0xD1
#define	MCHP_ERASE		0xFC
#define MCHP_FLASH_DISABLE	0xFD
#define	MCHP_FLASH_ENABLE	0xFE
#define	MCHP_READ_CONFIG	0xFF
