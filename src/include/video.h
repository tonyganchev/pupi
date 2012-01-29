/*
+----------------------------------------------------------------------------+
|                                                                            |
|  video.h - functions for Printing on the screen and stuff                  |
|                                                                            |
+----------------------------------------------------------------------------+
*/

#ifndef VIDEO_H
#  define VIDEO_H

extern void print_char(byte x, byte y, byte ch, byte attr);
extern void print_string(byte x, byte y, const char *ch, byte attr);
extern void print_long(byte x, byte y, long l, byte attr);
extern void print_dword(byte x, byte y, dword u, byte attr);
extern void print_word(byte x, byte y, word u, byte attr);
extern void print_byte(byte x, byte y, byte u, byte attr);

extern byte char_attr_at(byte x, byte y);

extern void scroll_up(byte attr);
extern void scroll_down(byte attr);
extern void clear_screen(byte attr);
extern void cursor_move(byte x, byte y);
extern byte cursor_x();
extern byte cursor_y();

#endif
