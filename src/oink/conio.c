#include <types.h>
#include <video.h>
#include <string.h>
#include <keyb.h>
#include <conio.h>



#define TEXT_COLUMNS	80
#define	TEXT_ROWS	25



static byte current_attribute_ = 0x0F;



void char_attr_set(byte attr)
{
	current_attribute_ = attr;
}



byte char_attr_get()
{
	return current_attribute_;
}



void clrscr()
{
	clear_screen(current_attribute_);
}



void kputchar(char ch)
{
	if (ch == 8)
	{
		if (!cursor_x() && cursor_y())
			cursor_move(0, cursor_y() - 1);
		else
			cursor_move(cursor_x() - 1, cursor_y());
		print_char(cursor_x(), cursor_y(), ' ', current_attribute_);
	}
	else if (ch == 10)
	{
		if (cursor_y() == TEXT_ROWS - 1)
		{
			scroll_up(current_attribute_);
			cursor_move(cursor_x(), cursor_y());
		}
		else
			cursor_move(cursor_x(), cursor_y() + 1);
	}
	else if (ch == 13)
		cursor_move(0, cursor_y());
	else
	{
		print_char(cursor_x(), cursor_y(), ch, current_attribute_);
		if (cursor_x() == TEXT_COLUMNS - 1)
		{
			if (cursor_y() == TEXT_ROWS - 1)
			{
				scroll_up(current_attribute_);
				cursor_move(0, cursor_y());
			}
			else
				cursor_move(0, cursor_y() + 1);
			
		}
		else
			cursor_move(cursor_x() + 1, cursor_y());
	}
}



void kputs(const char *s)
{
	if (s != NULL)
		while (*s != '\0')
			kputchar(*s++);
}



void kgets(char *s)
{
	long l = 0;
	if (s != NULL)
	{
		char c;
		do
		{
			c = get_char();
			if (c == 8 && l != 0)
			{
				l--;
				kputchar(c);
			}
			else if (c != 8 && c != 13)
				kputchar(s[l++] = c);
		}
		while (c != 13);
		s[l] = '\0';
	}
	cputs("\n\r");
}
