/*
 +----------------------------------------------------------------------------+
 |                                                                            |
 |  intr.h - routines to manage interrupts                                    |
 |                                                                            |
 +----------------------------------------------------------------------------+
 */

#ifndef INTR_H
#	define INTR_H

#include "types.h"
#include "bits.h"
#include "descriptor.h"
#include "intr_def.h"

typedef void (*INTRHANDLER)();

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

void intr_enable();
void intr_disable();
void intr_set(
		const INTERRUPTVECTOR intr_no,
		const INTRHANDLER func,
		const word selector,
		const word access);
#endif
