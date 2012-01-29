/*
+----------------------------------------------------------------------------+
|                                                                            |
|  floppy.h - routines to manage floppy I/O                                  |
|                                                                            |
+----------------------------------------------------------------------------+
*/

#ifndef HDR_FLOPPY
	#define HDR_FLOPPY

/* drive geometries */
#define DG144_HEADS       2	/* heads per drive (1.44M) */
#define DG144_TRACKS     80	/* number of tracks (1.44M) */
#define DG144_SPT        18	/* sectors per track (1.44M) */
#define DG144_GAP3FMT  0x54	/* gap3 while formatting (1.44M) */
#define DG144_GAP3RW   0x1b	/* gap3 while reading/writing (1.44M) */

#define DG168_HEADS       2	/* heads per drive (1.68M) */
#define DG168_TRACKS     80	/* number of tracks (1.68M) */
#define DG168_SPT        21	/* sectors per track (1.68M) */
#define DG168_GAP3FMT  0x0c	/* gap3 while formatting (1.68M) */
#define DG168_GAP3RW   0x1c	/* gap3 while reading/writing (1.68M) */

/* IO ports */
#define FDC_DOR  (0x3f2)	/* Digital Output Register */
#define FDC_MSR  (0x3f4)	/* Main Status Register (input) */
#define FDC_DRS  (0x3f4)	/* Data Rate Select Register (output) */
#define FDC_DATA (0x3f5)	/* Data Register */
#define FDC_DIR  (0x3f7)	/* Digital Input Register (input) */
#define FDC_CCR  (0x3f7)	/* Configuration Control Register (output) */

/* command bytes (these are 765 commands + options such as MFM, etc) */
#define CMD_SPECIFY (0x03)	/* specify drive timings */
#define CMD_WRITE   (0xc5)	/* write data (+ MT,MFM) */
#define CMD_READ    (0xe6)	/* read data (+ MT,MFM,SK) */
#define CMD_RECAL   (0x07)	/* recalibrate */
#define CMD_SENSEI  (0x08)	/* sense interrupt status */
#define CMD_FORMAT  (0x4d)	/* format track (+ MFM) */
#define CMD_SEEK    (0x0f)	/* seek track */
#define CMD_VERSION (0x10)	/* FDC version */

typedef struct __DMACHANNEL
{
	byte page;	 /* page register */
	byte offset; /* offset register */
	byte length; /* length register */
} DMACHANNEL;

typedef struct __DRIVEGEOMETRY
{
	byte heads;
	byte tracks;
	byte spt;	/* sectors per track */
} DRIVEGEOMETRY;

extern void usleep(int n);
extern void DMATransfer(int channel, long physaddr, int length, BOOL read);
extern void FloppyTimeout(void);
extern void FloppySendByte(int byte);
extern int  FloppyGetByte();
extern BOOL FloppyWaitFDC(BOOL sensei);
extern void FloppyBlockToHTS(int block, int *head, int *track, int *sector);
extern void FloppyReset(void);
extern BOOL FloppyDiskChange(void);
extern void FloppyMotorOn(void);
extern void FloppyMotorOff(void);
extern void FloppyRecalibrate(void);
extern BOOL FloppySeek(int track);
extern BOOL FloppyLogDisk(DRIVEGEOMETRY *g);
extern void FloppyInterrupt(void);


extern void floppy_init(void);
extern void FloppyTimeout(void);
extern void FloppyReset(void);
extern BOOL floppy_read_block(int block, byte *blockbuff);
extern BOOL floppy_write_block(int block, byte *blockbuff);
extern BOOL floppy_read_write(int block, byte *blockbuff, BOOL read);
extern BOOL floppyFormatTrack(byte track, DRIVEGEOMETRY *g);

#endif
