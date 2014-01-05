#include "types.h"
#include "string.h"
#include "format.h"
#include "ports.h"

#include <stddef.h>

#define TEXT_COLUMNS	80
#define	TEXT_ROWS	25



typedef byte *VRAM;



static byte cursor_x_ = 0;
static byte cursor_y_ = 0;



static inline VRAM video_ram_address()
{
	return (VRAM)0xB8000;
}



void print_char(byte x, byte y, byte ch, byte attr)
{
	VRAM videoMemory = video_ram_address();
	videoMemory[(y * TEXT_COLUMNS + x) * 2] = ch;
	videoMemory[(y * TEXT_COLUMNS + x) * 2 + 1] = attr;
}



void print_string(byte x, byte y, const char *ch, byte attr)
{
	if (ch != NULL)
		while (*ch != '\0' && y < TEXT_ROWS)
		{
			VRAM videoMemory = video_ram_address();

			videoMemory[(y * TEXT_COLUMNS + x) * 2] = *ch++;
			videoMemory[(y * TEXT_COLUMNS + x++) * 2 + 1] = attr;
			if (x == TEXT_COLUMNS)
			{
				x = 0;
				y++;
			}
		}
}



void print_long(byte x, byte y, long l, byte attr)
{
	char buf[12];
	ltoa(l, buf);
	print_string(x, y, buf, attr);
}



void print_dword(byte x, byte y, dword u, byte attr)
{
	char buf[9];
	utox(u, buf, 8);
	print_string(x, y, buf, attr);
}



void print_word(byte x, byte y, word u, byte attr)
{
	char buf[5];
	utox(u, buf, 4);
	print_string(x, y, buf, attr);
}



void print_byte(byte x, byte y, byte u, byte attr)
{
	char buf[3];
	utox(u, buf, 2);
	print_string(x, y, buf, attr);
}



byte char_attr_at(byte x, byte y)
{
	VRAM video_memory = video_ram_address();
	return video_memory[(y * TEXT_COLUMNS + x) * 2 + 1];
}

extern void *memsetw(void *dest, word val, int count);

void scroll_up(byte attr)
{
	VRAM video_memory = video_ram_address();

	memcpy(video_memory, video_memory + 160, 3840);
	memsetw((word *)video_memory + 1920, attr << 8 | ' ', 80);
}



void scroll_down(byte attr)
{
	VRAM video_memory = video_ram_address();

	memcpy(video_memory + 160, video_memory, 3840);
	memsetw((word *)video_memory, attr << 8 | ' ', 80);
}



void clear_screen(byte attr)
{
	VRAM video_memory = video_ram_address();

	memsetw((word *)video_memory, attr << 8 | ' ', 2000);
}



void cursor_move(byte x, byte y)
{
	if (x < TEXT_COLUMNS && y < TEXT_ROWS)
	{
		word temp;

		temp = y * 80 + x;

		outportb(0x3D4, 14);
		outportb(0x3D5, temp >> 8);
		outportb(0x3D4, 15);
		outportb(0x3D5, temp);

		cursor_x_ = x;
		cursor_y_ = y;
	}
}



byte cursor_x()
{
	return cursor_x_;
}



byte cursor_y()
{
	return cursor_y_;
}
