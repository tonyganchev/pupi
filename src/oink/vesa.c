#include "types.h"
#include "bits.h"
#include "ports.h"

enum VGAREG
{
	//3B4h -- CRTC Controller Address Register
	VGAREG_CRTC_A_ADDR = 0x3B4,
	//3B5h -- CRTC Controller Data Register
	VGAREG_CRTC_A_DATA = 0x3B5,
	// word register
	VGAREG_CRTC_A = VGAREG_CRTC_A_ADDR,
	
	//3BAh Read -- Input Status #1 Register
	VGAREG_INPUT_STATUS_1_A = 0x3BA,
	//3BAh Write -- Feature Control Register
	VGAREG_FEATURE_CONTROL_A = 0x3BA,
	//3C0h -- Attribute Address/Data Register
	VGAREG_ATTR_ADDR_DATA = 0x3C0,
	//3C1h -- Attribute Data Read Register
	VGAREG_ATTR_DATA_READ = 0x3C1,
	//3C2h Read -- Input Status #0 Register
	VGAREG_INPUT_STATUS_0 = 0x3C2,
	//3C2h Write -- Miscellaneous Output Register
	VGAREG_MISC_OUTPUT_A = 0x3C2,
	//3C4h -- Sequencer Address Register
	VGAREG_SEQ_ADDR = 0x3C4,
	//3C5h -- Sequencer Data Register
	VGAREG_SEQ_DATA = 0x3C5,
	// word register
	VGAREG_SEQ = 0x3C4,
	//3C7h Read -- DAC State Register
	VGAREG_DAC_STATE = 0x3C7,
	//3C7h Write -- DAC Address Read Mode Register
	VGAREG_DAC_ADDR_MODE_READ = 0x3C7,
	//3C8h -- DAC Address Write Mode Register
	VGAREG_DAC_ADDR_MODE_WRITE = 0x3C8,
	//3C9h -- DAC Data Register
	VGAREG_DAC_DATA = 0x3C9,
	//3CAh Read -- Feature Control Register
	VGAREG_FEATURE_CONTROL_B = 0x3CA,
	//3CCh Read -- Miscellaneous Output Register
	VGAREG_MISC_OUTPUT_B = 0x3CC,
	//3CEh -- Graphics Controller Address Register
	VGAREG_GRAPH_ADDR = 0x3CE,
	//3CFh -- Graphics Controller Data Register
	VGAREG_GRAPH_DATA = 0x3CF,
	// word register
	VGAREG_GRAPH = 0x3CE,
	//3D4h -- CRTC Controller Address Register
	VGAREG_CRTC_B_ADDR = 0x3D4,
	//3D5h -- CRTC Controller Data Register
	VGAREG_CRTC_B_DATA = 0x3D5,
	// word register
	VGAREG_CRTC_B = 0x3D4,
	//3DAh Read -- Input Status #1 Register
	VGAREG_INPUT_STATUS_1_B = 0x3DA,
	//3DAh Write -- Feature Control Register
	VGAREG_FEATURE_CONTROL_C = 0x3DA
};

enum VGAREGIDXGRAPH
{
	VGAREGIDXGRAPH_RESET           = 0x00,
	VGAREGIDXGRAPH_ENABLE_RESET    = 0x01,
	VGAREGIDXGRAPH_COLOR_COMPARE   = 0x02,
	VGAREGIDXGRAPH_DATA_ROTATE     = 0x03,
	VGAREGIDXGRAPH_READ_MAP_SELECT = 0x04,
	VGAREGIDXGRAPH_MODE            = 0x05,
	VGAREGIDXGRAPH_MISC            = 0x06,
	VGAREGIDXGRAPH_COLOR_DONTCARE  = 0x07,
	VGAREGIDXGRAPH_BIT_MASK        = 0x08
};

enum VGAREGIDXCRTC
{
	//	Index 00h -- Horizontal Total Register
	VGAREGIDXCRTC_HORIZONTAL_TOTAL = 0x00,
	//	Index 01h -- End Horizontal Display Register
	VGAREGIDXCRTC_END_HORIZONTAL_DISPLAY = 0x01,
	//	Index 02h -- Start Horizontal Blanking Register
	VGAREGIDXCRTC_HORIZONTAL_BLANKING_START = 0x02,
	//	Index 03h -- End Horizontal Blanking Register
	VGAREGIDXCRTC_HORIZONTAL_BLANKING_END = 0x03,
	//	Index 04h -- Start Horizontal Retrace Register
	VGAREGIDXCRTC_HORIZONTAL_RETRACE_START = 0x04,
	//	Index 05h -- End Horizontal Retrace Register
	VGAREGIDXCRTC_HORIZONTAL_RETRACE_END = 0x05,
	//	Index 06h -- Vertical Total Register
	VGAREGIDXCRTC_VERTICAL_TOTAL = 0x06,
	//	Index 07h -- Overflow Register
	VGAREGIDXCRTC_OVERFLOW_TOTAL = 0x07,
	//	Index 08h -- Preset Row Scan Register
	VGAREGIDXCRTC_RESET_ROW_SCAN = 0x08,
	//	Index 09h -- Maximum Scan Line Register
	VGAREGIDXCRTC_MAXIMUM_SCAN_LINE = 0x09,
	//	Index 0Ah -- Cursor Start Register
	VGAREGIDXCRTC_CURSOR_START = 0x0A,
	//	Index 0Bh -- Cursor End Register
	VGAREGIDXCRTC_CURSOR_END = 0x0B,
	//	Index 0Ch -- Start Address High Register
	VGAREGIDXCRTC_START_ADDRESS_HIGH = 0x0C,
	//	Index 0Dh -- Start Address Low Register
	VGAREGIDXCRTC_START_ADDRESS_LOW = 0x0D,
	//	Index 0Eh -- Cursor Location High Register
	VGAREGIDXCRTC_CURSOR_LOCATION_HIGH = 0x0E,
	//	Index 0Fh -- Cursor Location Low Register
	VGAREGIDXCRTC_CURSOR_LOCATION_LOW = 0x0F,
	//	Index 10h -- Vertical Retrace Start Register
	VGAREGIDXCRTC_VERTICAL_RETRACE_START = 0x10,
	//	Index 11h -- Vertical Retrace End Register
	VGAREGIDXCRTC_VERTICAL_RETRACE_END = 0x11,
	//	Index 12h -- Vertical Display End Register
	VGAREGIDXCRTC_VERTICAL_DISPLAY_END = 0x12,
	//	Index 13h -- Offset Register
	VGAREGIDXCRTC_OFFSET = 0x13,
	//	Index 14h -- Underline Location Register
	VGAREGIDXCRTC_UNDERLINE_LOCATION = 0x14,
	//	Index 15h -- Start Vertical Blanking Register
	VGAREGIDXCRTC_VERTICAL_BLANKING_START = 0x15,
	//	Index 16h -- End Vertical Blanking
	VGAREGIDXCRTC_VERTICAL_BLANKING_END = 0x16,
	//	Index 17h -- CRTC Mode Control Register
	VGAREGIDXCRTC_MODE_CONTROL = 0x17,
	//	Index 18h -- Line Compare Register
	VGAREGIDXCRTC_LINE_COMPARE = 0x18
};

//static inline byte vga_register_read(word reg, byte regIdx)
//{
//	byte addr = inportb(reg);
//	outportb(reg, regIdx);
//	word addrData = inportw(reg);
//	outportb(reg, addr);
//	return byte_lo(addrData);
//}

static inline void vga_reg_write(word reg, byte regIdx, byte data)
{
	byte addr = inportb(reg);
	outportb(reg, regIdx);
	outportw(reg, word_make(data, regIdx));
	outportb(reg, addr);
}

//typedef char byte;
//typedef unsigned short word;
//typedef unsigned long dword;

#define SZ(x) (sizeof(x) / sizeof(x[0]))

#define R_COM  0x63 // "common" bits

#define R_W256 0x00
#define R_W320 0x00
#define R_W360 0x04
#define R_W376 0x04
#define R_W400 0x04

#define R_H200 0x00
#define R_H224 0x80
#define R_H240 0x80
#define R_H256 0x80
#define R_H270 0x80
#define R_H300 0x80
#define R_H360 0x00
#define R_H400 0x00
#define R_H480 0x80
#define R_H564 0x80
#define R_H600 0x80

static const byte hor_regs [] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x13 };

static const byte width_256[]  = { 0x5f, 0x3f, 0x40, 0x82, 0x4a, 0x9a, 0x20 };
static const byte width_320[]  = { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0x28 };
static const byte width_360[]  = { 0x6b, 0x59, 0x5a, 0x8e, 0x5e, 0x8a, 0x2d };
static const byte width_376[]  = { 0x6e, 0x5d, 0x5e, 0x91, 0x62, 0x8f, 0x2f };
static const byte width_400[]  = { 0x70, 0x63, 0x64, 0x92, 0x65, 0x82, 0x32 };

static const byte ver_regs[]   = { 0x06, 0x07, 0x09, 0x10, 0x11, 0x12, 0x15, 0x16 };

static const byte height_200[] = { 0xbf, 0x1f, 0x41, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
static const byte height_224[] = { 0x0b, 0x3e, 0x41, 0xda, 0x9c, 0xbf, 0xc7, 0x04 };
static const byte height_240[] = { 0x0d, 0x3e, 0x41, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
static const byte height_256[] = { 0x23, 0xb2, 0x61, 0x0a, 0xac, 0xff, 0x07, 0x1a };
static const byte height_270[] = { 0x30, 0xf0, 0x61, 0x20, 0xa9, 0x1b, 0x1f, 0x2f };
static const byte height_300[] = { 0x70, 0xf0, 0x61, 0x5b, 0x8c, 0x57, 0x58, 0x70 };
static const byte height_360[] = { 0xbf, 0x1f, 0x40, 0x88, 0x85, 0x67, 0x6d, 0xba };
static const byte height_400[] = { 0xbf, 0x1f, 0x40, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
static const byte height_480[] = { 0x0d, 0x3e, 0x40, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
static const byte height_564[] = { 0x62, 0xf0, 0x60, 0x37, 0x89, 0x33, 0x3c, 0x5c };
static const byte height_600[] = { 0x70, 0xf0, 0x60, 0x5b, 0x8c, 0x57, 0x58, 0x70 };

int vga_init(int width, int height, int chain4)
{
	const byte *w, *h;
	byte val;
	int a;
	
	switch (width)
	{
	case 256:
		w = width_256;
		val = R_COM + R_W256;
		break;
	case 320:
		w = width_320;
		val = R_COM + R_W320;
		break;
	case 360:
		w = width_360;
		val = R_COM + R_W360;
		break;
	case 376:
		w = width_376;
		val = R_COM + R_W376;
		break;
	case 400:
		w = width_400;
		val = R_COM + R_W400;
		break;
	default:
		return 0; // fail
	}
	
	switch (height)
	{
	case 200:
		h = height_200;
		val |= R_H200;
		break;
	case 224:
		h = height_224;
		val |= R_H224;
		break;
	case 240:
		h = height_240;
		val |= R_H240;
		break;
	case 256:
		h = height_256;
		val |= R_H256;
		break;
	case 270:
		h = height_270;
		val |= R_H270;
		break;
	case 300:
		h = height_300;
		val |= R_H300;
		break;
	case 360:
		h = height_360;
		val |= R_H360;
		break;
	case 400:
		h = height_400;
		val |= R_H400;
		break;
	case 480:
		h = height_480;
		val |= R_H480;
		break;
	case 564:
		h = height_564;
		val |= R_H564;
		break;
	case 600:
		h = height_600;
		val |= R_H600;
		break;
	default:
		return 0; // fail
	}
	
	// chain4 not available if mode takes over 64k

	if (chain4 && (long)width * (long)height > 65536L)
		return 0;
	
	// here goes the actual modeswitch

	outportb(VGAREG_MISC_OUTPUT_B, val);
	//outportw(VGAREG_CRTC_B_ADDR, 0x0e11); // enable regs 0-7
	vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_VERTICAL_RETRACE_END, 0x0E);

	for (a = 0; a < SZ(hor_regs); ++a)
		vga_reg_write(VGAREG_CRTC_B, hor_regs[a], w[a]);
	for (a = 0; a < SZ(ver_regs); ++a)
		vga_reg_write(VGAREG_CRTC_B, ver_regs[a], h[a]);
	
	//outportw(VGAREG_CRTC_B_ADDR, 0x0008); // vert.panning = 0
	vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_RESET_ROW_SCAN, 0x00);

	if (chain4)
	{
		//outportw(VGAREG_CRTC_B_ADDR, 0x4014);
		vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_UNDERLINE_LOCATION, 0x40);
		//outportw(VGAREG_CRTC_B_ADDR, 0xa317);
		vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_MODE_CONTROL, 0xa3);
		//outportw(VGAREG_SEQ_ADDR, 0x0e04);
		vga_reg_write(VGAREG_SEQ, VGAREGIDXCRTC_HORIZONTAL_RETRACE_START, 0x0E);
	}
	else
	{
		//outportw(VGAREG_CRTC_B_ADDR, 0x0014);
		vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_UNDERLINE_LOCATION, 0x00); 
		//outportw(VGAREG_CRTC_B_ADDR, 0xe317);
		vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_MODE_CONTROL, 0xE3);
		//outportw(VGAREG_SEQ_ADDR, 0x0604);
		vga_reg_write(VGAREG_CRTC_B, VGAREGIDXCRTC_HORIZONTAL_RETRACE_START, 0x06);
	}
	
	//outportw(VGAREG_SEQ_ADDR, 0x0101);
	vga_reg_write(VGAREG_SEQ_ADDR, 0x01, 0x01);
	
	// enable writing to all planes
	//outportw(VGAREG_SEQ_ADDR, 0x0f02);
	vga_reg_write(VGAREG_GRAPH, 0x02, 0x0f);
	// 256color mode
	vga_reg_write(VGAREG_GRAPH, 0x05, 0x40);
	// graph mode & A000-AFFF
	//outportw(VGAREG_GRAPH_ADDR, 0x0506);
	vga_reg_write(VGAREG_GRAPH, 0x06, 0x05);

	inportb(VGAREG_INPUT_STATUS_1_B);
	
	outportb(VGAREG_ATTR_ADDR_DATA, 0x30);
	outportb(VGAREG_ATTR_ADDR_DATA, 0x41);
	outportb(VGAREG_ATTR_ADDR_DATA, 0x33);
	outportb(VGAREG_ATTR_ADDR_DATA, 0x00);
	
	for (a = 0; a < 16; a++)
	{
		// ega pal
		outportb(VGAREG_ATTR_ADDR_DATA, (byte)a);
		outportb(VGAREG_ATTR_ADDR_DATA, (byte)a);
	}
	
	outportb(VGAREG_ATTR_ADDR_DATA, 0x20); // enable video

	return 1;
}
