#include <types.h>
#include <bits.h>

#include "intr_def.h"

/*
 +----------------------------------------------------------------------------+
 | Implementation                                                             |
 +----------------------------------------------------------------------------+
 */

void intr_enable()
{
	__asm__ __volatile__ ("sti");
}

void intr_disable()
{
	__asm__ __volatile__ ("cli");
}

void intr_set_indir(const INTERRUPTVECTOR intrNo, const INTERRUPT desc)
{
	*((INTERRUPT *) (idt_start + intrNo * sizeof(INTERRUPT))) = desc;
}

void intr_get_indir(const INTERRUPTVECTOR intrNo, INTERRUPT *desc)
{
	*desc = *((INTERRUPT *) (idt_start + intrNo * sizeof(INTERRUPT)));
}

void intr_set(const INTERRUPTVECTOR intrNo, const INTRHANDLER func,
		const word selector, const word access)
{
	INTERRUPT intr;

	intr.offsetLo = word_lo((dword) func); //(WORD)(DWORD)func;
	intr.selector = selector;
	intr.wordCount = 0;
	intr.access = access;
	intr.offsetHi = word_hi((dword) func); //(WORD)((DWORD)func >> 16);

	intr_set_indir(intrNo, intr);
}
