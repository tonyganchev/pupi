/*
+----------------------------------------------------------------------------+
|                                                                            |
|  conio.h - functions for Printing on the console                           |
|                                                                            |
+----------------------------------------------------------------------------+
*/

#ifndef HDR_CONIO
	#define HDR_CONIO



//#include "types.h"



void char_attr_set(byte attr);
byte char_attr_get();

void clrscr();

void kputchar(char ch);
void kputs(const char *s);
void kgets(char *s);



#endif
