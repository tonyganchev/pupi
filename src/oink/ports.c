#include "types.h"

byte inportb(word port)
{
	byte rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}

void outportb(word port, byte data)
{
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

word inportw(word port)
{
	word rv;
	__asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}

void outportw(word port, word data)
{
	__asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data));
}
