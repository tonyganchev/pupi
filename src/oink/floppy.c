#include <types.h>
#include <ports.h>
#include <intr.h>
#include <conio.h>
#include <string.h>

/*
+----------------------------------------------------------------------------+
| Types                                                                      |
+----------------------------------------------------------------------------+
*/

typedef struct DMACHANNEL_
{
	byte page;	 /* page register */
	byte offset; /* offset register */
	byte length; /* length register */
} DMACHANNEL;

typedef struct DRIVEGEOMETRY_
{
	byte heads;
	byte tracks;
	byte spt;	/* sectors per track */
} DRIVEGEOMETRY;

/*
+----------------------------------------------------------------------------+
| Constants                                                                  |
+----------------------------------------------------------------------------+
*/

/* drive geometries ------------------------------------------------------- */
#define DG144_HEADS      2	/* heads per drive (1.44M)                      */
#define DG144_TRACKS    80	/* number of tracks (1.44M)                     */
#define DG144_SPT       18	/* sectors per track (1.44M)                    */
#define DG144_GAP3FMT 0x54	/* gap3 while formatting (1.44M)                */
#define DG144_GAP3RW  0x1b	/* gap3 while reading/writing (1.44M)           */

#define DG168_HEADS      2	/* heads per drive (1.68M)                      */
#define DG168_TRACKS    80	/* number of tracks (1.68M)                     */
#define DG168_SPT       21	/* sectors per track (1.68M)                    */
#define DG168_GAP3FMT 0x0c	/* gap3 while formatting (1.68M)                */
#define DG168_GAP3RW  0x1c	/* gap3 while reading/writing (1.68M)           */

/* IO ports --------------------------------------------------------------- */
#define FDC_DOR  (0x3f2)	/* Digital Output Register                      */
#define FDC_MSR  (0x3f4)	/* Main Status Register (input)                 */
#define FDC_DRS  (0x3f4)	/* Data Rate Select Register (output)           */
#define FDC_DATA (0x3f5)	/* Data Register */
#define FDC_DIR  (0x3f7)	/* Digital Input Register (input)               */
#define FDC_CCR  (0x3f7)	/* Configuration Control Register (output)      */

/* command bytes (these are 765 commands + options such as MFM, etc) ------ */
#define CMD_SPECIFY (0x03)	/* specify drive timings                        */
#define CMD_WRITE   (0xc5)	/* write data (+ MT,MFM)                        */
#define CMD_READ    (0xe6)	/* read data (+ MT,MFM,SK)                      */
#define CMD_RECAL   (0x07)	/* recalibrate                                  */
#define CMD_SENSEI  (0x08)	/* sense interrupt status                       */
#define CMD_FORMAT  (0x4d)	/* format track (+ MFM)                         */
#define CMD_SEEK    (0x0f)	/* seek track                                   */
#define CMD_VERSION (0x10)	/* FDC version                                  */

/*
+----------------------------------------------------------------------------+
| Globals                                                                    |
+----------------------------------------------------------------------------+
*/

volatile bool done = false;
bool dchange = false;
bool motor = false;
int mtick = 0;
volatile int tmout = 0;
byte status[7] = { 0 };
byte statsz = 0;
byte sr0 = 0;
byte fdc_track = 0xFF;
DRIVEGEOMETRY geometry = { DG144_HEADS, DG144_TRACKS, DG144_SPT };
int tbaddr = 0x80000L; /* transfer buffer address */
/* definition of DMA channels */
const DMACHANNEL dmainfo[] = {
	{ 0x87, 0x00, 0x01 },
	{ 0x83, 0x02, 0x03 },
	{ 0x81, 0x04, 0x05 },
	{ 0x82, 0x06, 0x07 }
};

/*
+----------------------------------------------------------------------------+
| Prototypes                                                                 |
+----------------------------------------------------------------------------+
*/

/* routines for external use */
void floppy_init(void);
bool floppy_read_block(int block, byte *blockbuff);
bool floppy_write_block(int block, byte *blockbuff);
bool floppy_read_write(int block, byte *blockbuff, bool read);
bool floppy_format_track(byte track, DRIVEGEOMETRY *g);

/* routines for internal use ---------------------------------------------- */
static void usleep(int n);
static void dma_transfer(int channel, long physaddr, int length, bool read);
static void timeout(void);
static void send_byte(int byte);
static int  receive_byte();
static bool wait_controller(bool sensei);
static void block2hts(int block, int *head, int *track, int *sector);
static void reset(void);
static bool disk_changed(void);
static void motor_on(void);
static void motor_off(void);
static void recalibrate(void);
static bool seek(int track);
static bool log_disk(DRIVEGEOMETRY *g);
static void ih_floppy(void);

/*
+----------------------------------------------------------------------------+
| Implementation                                                             |
+----------------------------------------------------------------------------+
*/

void usleep(int n)
{
	for (; n > 0; n--)
		__asm__ __volatile__ ("nop");
}

/*
 * this sets up a DMA trasfer between a device and memory.  Pass the DMA
 * channel number (0..3), the physical address of the buffer and transfer
 * length.  If 'read' is true, then transfer will be from memory to device,
 * else from the device to memory.
 */
void dma_transfer(int channel, long physaddr, int length, bool read)
{
	long page, offset;

	if (channel < 4)
	{

		/* calculate dma page and offset */
		page = physaddr >> 16;
		offset = physaddr & 0xffff;
		length -= 1;

		intr_disable();

		/* set the mask bit for the channel */
		outportb(0x0a, channel | 4);

		/* clear flipflop */
		outportb(0x0c, 0);

		/* set DMA mode (write+single+r/w) */
		outportb(0x0b, (read ? 0x48 : 0x44) + channel);

		/* set DMA page */
		outportb(dmainfo[channel].page, page);

		/* set DMA offset */
		outportb(dmainfo[channel].offset, offset & 0xff);
		outportb(dmainfo[channel].offset, offset >> 8);

		/* set DMA length */
		outportb(dmainfo[channel].length, length & 0xff);
		outportb(dmainfo[channel].length, length >> 8);

		/* clear DMA mask bit */
		outportb(0x0a, channel);

		intr_enable();  /* enable irq's */
	}
}



/* init driver */
void floppy_init(void)
{
	int i;

	intr_set(38, ih_floppy, 8, 0x8E00);
	intr_set(40, timeout, 8, 0x8E00);
	//InterruptSet(0x70, FloppyTimeout, 8, 0x8E00);

	reset();

	/* get floppy controller version */
	send_byte(CMD_VERSION);
	i = receive_byte();

	if (i == 0x80)
		kputs("NEC765 controller found\r\n");
	else
		kputs("enhanced controller found\r\n");
}



/* FloppySendByte() routine from intel manual */
void send_byte(int byte)
{
	volatile int msr;
	int tmo;

	for (tmo = 0; tmo < 128; tmo++)
	{
		msr = inportb(FDC_MSR);
		if ((msr & 0xc0) == 0x80)
		{
			outportb(FDC_DATA, byte);
			return;
		}
		inportb(0x80);
	}
}



/* FloppyGetByte() routine from intel manual */
int receive_byte()
{
	volatile int msr;
	int tmo;

	for (tmo = 0; tmo < 128; tmo++)
	{
		msr = inportb(FDC_MSR);
		if ((msr & 0xd0) == 0xd0)
			return inportb(FDC_DATA);
		inportb(0x80); /* delay */
	}

	return -1;   /* read timeout */
}



/* this waits for FDC command to complete */
bool wait_controller(bool sensei)
{
	tmout = 50000;   /* set timeout to 1 second */

	/* wait for IRQ6 handler to signal command finished */
	while (!done && tmout)
		/* NOP */;

	/* read in command result bytes */
	statsz = 0;
	while ((statsz < 7) && (inportb(FDC_MSR) & (1 << 4)))
		status[statsz++] = receive_byte();

	if (sensei)
	{
		/* send a "sense interrupt status" command */
		send_byte(CMD_SENSEI);
		sr0 = receive_byte();
		fdc_track = receive_byte();
	}

	done = false;

	if (!tmout)
	{
		/* timed out! */
		if (inportb(FDC_DIR) & 0x80)  /* check for diskchange */
			dchange = true;
		return false;
	}
	else
		return true;
}



/* This is the IRQ6 handler */
void ih_floppy(void)
{
	START_INTERRUPT();

	done = true;

	END_INTERRUPT();
}



/* This is the timer (int 1ch) handler */
void timeout(void)
{
	START_INTERRUPT();

	if (tmout)
		--tmout;

	if (mtick > 0)
		--mtick;
	else if (!mtick && motor)
	{
			outportb(FDC_DOR,0x0c);
			motor = false;
	}

	END_INTERRUPT();
}



/*
 * converts linear block address to head/track/sector
 *
 * blocks are numbered 0..heads*tracks*spt-1
 * blocks 0..spt-1 are serviced by head #0
 * blocks spt..spt*2-1 are serviced by head 1
 *
 * WARNING: garbage in == garbage out
 */

void block2hts(int block, int *head, int *track, int *sector)
{
	*head = (block % (geometry.spt * geometry.heads)) / (geometry.spt);
	*track = block / (geometry.spt * geometry.heads);
	*sector = block % geometry.spt + 1;
}



/* this gets the FDC to a known state */
void reset(void)
{
	/* stop the motor and disable IRQ/DMA */
	outportb(FDC_DOR, 0);

	mtick = 0;
	motor = false;

	/* program data rate (500K/s) */
	// TONY : BOCHS : NOT SUPPORTED
	//outportb(FDC_DRS, 0);

	/* re-enable interrupts */
	outportb(FDC_DOR, 0x0c);

	/* resetting triggered an interrupt - handle it */
	done = true;

	wait_controller(true);

	/* specify drive timings (got these off the BIOS) */
	send_byte(CMD_SPECIFY);
	send_byte(0xdf);  /* SRT = 3ms, HUT = 240ms */
	send_byte(0x02);  /* HLT = 16ms, ND = 0 */

	/* clear "disk change" status */
	seek(1);
	recalibrate();

	dchange = false;
}



/* this returns whether there was a disk change */
bool disk_changed(void)
{
	return dchange;
}



/* this turns the motor on */
void motor_on(void)
{
	if (!motor)
	{
		mtick = -1;     /* stop motor kill countdown */
		outportb(FDC_DOR, 0x1c);
		usleep(500000); /* delay 500ms for motor to spin up */
		motor = true;
	}
}



/* this turns the motor off */
void motor_off(void)
{
	if (motor)
		mtick = 36;   /* start motor kill countdown: 36 ticks ~ 2s */
}



/* FloppyRecalibrate the drive */
void recalibrate(void)
{
	/* turn the motor on */
	motor_on();

	/* send actual command bytes */
	send_byte(CMD_RECAL);
	send_byte(0);

	/* wait until FloppySeek finished */
	wait_controller(true);

	/* turn the motor off */
	motor_off();
}



/* FloppySeek to track */
bool seek(int track)
{
	if (fdc_track == track)  /* already there? */
	 return true;

	motor_on();

	/* send actual command bytes */
	send_byte(CMD_SEEK);
	send_byte(0);
	send_byte(track);

	/* wait until FloppySeek finished */
	if (!wait_controller(true))
		return false;     /* timeout! */

	/* now let head settle for 15ms */
	usleep(15000);

	motor_off();

	/* check that FloppySeek worked */
	if ((sr0 != 0x20) || (fdc_track != track))
		return false;
	else
		return true;
}



/* checks drive geometry - call this after any disk change */
bool log_disk(DRIVEGEOMETRY *g)
{
	/* get drive in a known status before we do anything */
	reset();

	/* assume disk is 1.68M and try and read block #21 on first track */
	geometry.heads = DG168_HEADS;
	geometry.tracks = DG168_TRACKS;
	geometry.spt = DG168_SPT;

	if (floppy_read_block(20, NULL))
	{
		/* disk is a 1.68M disk */
		if (g)
		{
			g->heads = geometry.heads;
			g->tracks = geometry.tracks;
			g->spt = geometry.spt;
		}
		return true;
	}

	/* OK, not 1.68M - try again for 1.44M reading block #18 on first track */
	geometry.heads = DG144_HEADS;
	geometry.tracks = DG144_TRACKS;
	geometry.spt = DG144_SPT;

	if (floppy_read_block(17, NULL))
	{
		/* disk is a 1.44M disk */
		if (g)
		{
			g->heads = geometry.heads;
			g->tracks = geometry.tracks;
			g->spt = geometry.spt;
		}
		return true;
	}

	/* it's not 1.44M or 1.68M - we don't support it */
	return false;
}



/* read block (blockbuff is 512 byte buffer) */
bool floppy_read_block(int block, byte *blockbuff)
{
	return floppy_read_write(block, blockbuff, true);
}



/* write block (blockbuff is 512 byte buffer) */
bool floppy_write_block(int block, byte *blockbuff)
{
	return floppy_read_write(block, blockbuff, false);
}



/*
 * since reads and writes differ only by a few lines, this handles both.  This
 * function is called by FloppyReadBlock() and FloppyWriteBlock()
 */
bool floppy_read_write(int block, byte *blockbuff, bool read)
{
	int head, track, sector, tries;

	/* convert logical address into physical address */
	block2hts(block, &head, &track, &sector);
	//   printf("block %d = %d:%02d:%02d\n",block,head,track,sector);

	/* spin up the disk */
	motor_on();

	if (!read && blockbuff)
	{
		/* copy data from data buffer into track buffer */
		memcpy((void *)tbaddr, (void *)blockbuff, 512);
	}

	for (tries = 0; tries < 3; tries++)
	{
		/* check for diskchange */
		if (inportb(FDC_DIR) & 0x80)
		{
			dchange = true;
			seek(1);  /* clear "disk change" status */
			recalibrate();
			motor_off();
			return false;
		}
		/* move head to right track */
		if (!seek(track))
		{
			motor_off();
			return false;
		}
		/* program data rate (500K/s) */
		outportb(FDC_CCR, 0);
		/* send command */
		if (read)
		{
			dma_transfer(2, tbaddr, 512, false);
			send_byte(CMD_READ);
		}
		else
		{
			dma_transfer(2, tbaddr, 512, true);
			send_byte(CMD_WRITE);
		}

		send_byte(head << 2);
		send_byte(track);
		send_byte(head);
		send_byte(sector);
		send_byte(2);               /* 512 bytes/sector */
		send_byte(geometry.spt);

		if (geometry.spt == DG144_SPT)
			send_byte(DG144_GAP3RW);  /* gap 3 size for 1.44M read/write */
		else
			send_byte(DG168_GAP3RW);  /* gap 3 size for 1.68M read/write */

		send_byte(0xff);            /* DTL = unused */

		/* wait for command completion */
		/* read/write don't need "sense interrupt status" */
		if (!wait_controller(false))
			return false;   /* timed out! */
		if ((status[0] & 0xc0) == 0)
			break;   /* worked! outta here! */

		recalibrate();  /* oops, try again... */
	}

	/* stop the motor */
	motor_off();

	if (read && blockbuff)
	{
		/* copy data from track buffer into data buffer */
		memcpy((void *)blockbuff, (void *)tbaddr, 512);
	}

	/* status bytes:
	for (i = 0; i < statsz; i++)
		printf("%02x ", status[i]); */

	return (tries != 3);
}



/* this formats a track, given a certain geometry */
bool floppy_format_track(byte track, DRIVEGEOMETRY *g)
{
	int i, h, r, r_id, split;
	byte tmpbuff[256];

	/* check geometry */
	if (g->spt != DG144_SPT && g->spt != DG168_SPT)
		return false;

	/* spin up the disk */
	motor_on();

	/* program data rate (500K/s) */
	outportb(FDC_CCR,0);

	seek(track);  /* FloppySeek to track */

	/* precalc some constants for interleave calculation */
	split = g->spt / 2;
	if (g->spt & 1)
		split++;

	for (h = 0; h < g->heads; h++)
	{
		/* for each head... */

		/* check for diskchange */
		if (inportb(FDC_DIR) & 0x80)
		{
			dchange = true;
			seek(1);  /* clear "disk change" status */
			recalibrate();
			motor_off();
			return false;
		}

		i = 0;   /* reset buffer index */
		for (r = 0; r < g->spt; r++)
		{
			/* for each sector... */

			/* calculate 1:2 interleave (seems optimal in my system) */
			r_id = r / 2 + 1;
			if (r & 1)
				r_id += split;

			/* add some head skew (2 sectors should be enough) */
			if (h & 1)
			{
				r_id -= 2;
				if (r_id < 1)
					r_id += g->spt;
			}

			/* add some track skew (1/2 a revolution) */
			if (track & 1)
			{
				r_id -= g->spt / 2;
				if (r_id < 1)
					r_id += g->spt;
			}

			/* interleave now calculated - sector ID is stored in r_id */

			/* fill in sector ID's */
			tmpbuff[i++] = track;
			tmpbuff[i++] = h;
			tmpbuff[i++] = r_id;
			tmpbuff[i++] = 2;
		}

		/* copy sector ID's to track buffer */
		memcpy((void *)tbaddr, (void *)tmpbuff, i);

		/* start dma xfer */
		dma_transfer(2, tbaddr, i, true);

		/* prepare "format track" command */
		send_byte(CMD_FORMAT);
		send_byte(h << 2);
		send_byte(2);
		send_byte(g->spt);
		if (g->spt == DG144_SPT)
			send_byte(DG144_GAP3FMT);    /* gap3 size for 1.44M format */
		else
			send_byte(DG168_GAP3FMT);    /* gap3 size for 1.68M format */
		send_byte(0);     /* filler byte */

		/* wait for command to finish */
		if (!wait_controller(false))
			return false;

		if (status[0] & 0xc0)
		{
			motor_off();
			return false;
		}
	}

	motor_off();

	return true;
}


