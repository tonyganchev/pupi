#include "types.h"
#include "video.h"
#include "ports.h"
#include "intr.h"

#define SHIFT_PRESSED   0x01
#define CTRL_PRESSED    0x02
#define ALT_PRESSED     0x04
#define CAPSLOCK_ACTIVE 0x08

#define IS_MAKE_CODE(b)  (b <= 0x80)
#define IS_BREAK_CODE(b) (b > 0x80)
#define IS_CHAR_CODE(b)  (IS_MAKE_CODE(b) && ((b > 0 && b < 29) || (b > 29 && b < 54 && b != 42) || b == 57))
#define IS_UPPERCASE()   (((mode_keys & SHIFT_PRESSED) == SHIFT_PRESSED && (mode_keys & CAPSLOCK_ACTIVE) != CAPSLOCK_ACTIVE) || ((mode_keys & SHIFT_PRESSED) != SHIFT_PRESSED && (mode_keys & CAPSLOCK_ACTIVE) == CAPSLOCK_ACTIVE))

static byte kb_buffer = 0;
static byte mode_keys = 0;
static bool can_change_caps = true;

static const char normal_codes[] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13, 0, 'a',
	's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

static const char shift_codes[] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 8, 9,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 13, 0, 'A',
	'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '
};

static const char ctrl_codes[] = {
	0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', '=', 8, 9,
	17, 23, 5, 18, 20, 25, 21, 9, 15, 16, 27, 29, 10, 0, 1,
	19, 4, 6, 7, 8, 10, 11, 12, ';', '\'', '`', 0, 28,
	26, 24, 3, 22, 2, 14, 13, ',', '.', '/', 0, 0, 0, ' '
};

static const char ctrl_shift_codes[] = {
	0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 31, 0, 8, 9,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



void ih_keyboard()
{
	START_INTERRUPT();
	kb_buffer = inportb(0x60);
	switch (kb_buffer)
	{
	case 0x2A:
	case 0x36:
		mode_keys |= SHIFT_PRESSED;
		break;
	case 0xAA:
	case 0xB6:
		mode_keys ^= SHIFT_PRESSED;
		break;
	case 0x1D:
		mode_keys |= CTRL_PRESSED;
		break;
	case 0x9D:
		mode_keys ^= CTRL_PRESSED;
		break;
	case 0x38:
		mode_keys |= ALT_PRESSED;
		break;
	case 0xB8:
		mode_keys ^= ALT_PRESSED;
		break;
	case 0x3A:
		if (can_change_caps)
		{
			mode_keys ^= CAPSLOCK_ACTIVE;
			can_change_caps = 0;
		}
		break;
	case 0xBA:
		can_change_caps = 1;
		break;
	}
	END_INTERRUPT();
}



static byte get_last_code()
{
	byte b = 0;
	while (kb_buffer == 0);
	b = kb_buffer;
	kb_buffer = 0;
	return b;
}



char get_char()
{
	byte code = 0;
	while (!IS_CHAR_CODE(code))
		code = get_last_code();
	if (IS_UPPERCASE())
	{
		if ((mode_keys & CTRL_PRESSED) == CTRL_PRESSED)
			return ctrl_shift_codes[code];
		return shift_codes[code];
	}
	if ((mode_keys & CTRL_PRESSED) == CTRL_PRESSED)
		return ctrl_codes[code];
	return normal_codes[code];
}
