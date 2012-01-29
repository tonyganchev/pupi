/*
 +----------------------------------------------------------------------------+
 |                                                                            |
 |  intr.h - routines to manage interrupts                                    |
 |                                                                            |
 +----------------------------------------------------------------------------+
 */

#ifndef INTR_H
#	define INTR_H

#include "bits.h"
#include "../oink/intr_def.h"

/*
 +----------------------------------------------------------------------------+
 | Defines                                                                    |
 +----------------------------------------------------------------------------+
 */

#define START_INTERRUPT() \
	__asm__ __volatile__( \
		"cli\n" \
		"pusha\n" \
		"push %gs\n" \
		"push %fs\n" \
		"push %es\n" \
		"push %ds\n"); \
	//BYTE emptyInterruptVariable;
#define END_INTERRUPT()	\
	__asm__ __volatile__( \
		"movb $0x20, %al\n" \
		"outb %al, $0x20\n" \
		"pop %ds\n" \
		"pop %es\n" \
		"pop %fs\n" \
		"pop %gs\n" \
		"popa\n" \
		"sti\n" \
		"leave\n" \
		"iret\n" \
	)

/*
 +----------------------------------------------------------------------------+
 | Prototypes                                                                 |
 +----------------------------------------------------------------------------+
 */

extern void intr_enable();
extern void intr_disable();
extern void intr_set_indir(INTERRUPTVECTOR intr_no, const INTERRUPT* desc);
extern void intr_get_indir(INTERRUPTVECTOR intr_no, INTERRUPT *desc);
extern void intr_set(
		const INTERRUPTVECTOR intr_no,
		const INTRHANDLER func,
		const word selector,
		const word access);
#endif
