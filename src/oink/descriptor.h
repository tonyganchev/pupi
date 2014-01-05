#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "types.h"

enum DESCRIPTORACCESS
{
	DA_ACCESSED = 1,
	DA_WRITEABLE = 2,
	DA_EXPANDDOWN = 4,
	DA_EXECUTABLE = 8,
	DA_CODEDATA = 16,
	DA_RING0 = 0,
	DA_RING1 = 32,
	DA_RING2 = 64,
	DA_RING3 = 96,
	DA_PRESENT = 128,

	DA_GATE_INVALID = 0,
	DA_GATE_AVAIL286TSS = 1,
	DA_GATE_LDT = 2,
	DA_GATE_BUSY286TSS = 3,

	DA_GATE_286CALLGATE = 4,
	DA_GATE_TASKGATE = 5,
	DA_GATE_286INTRGATE = 6,
	DA_GATE_286TRAPGATE = 7,

	DA_GATE_INVALID2 = 8,
	DA_GATE_AVAIL386TSS = 9,
	DA_GATE_RESERVED = 10,
	DA_GATE_BUSY386TSS = 11,

	DA_GATE_386CALLGATE = 12,
	DA_GATE_RESERVED2 = 13,
	DA_GATE_386INTRGATE = 14,
	DA_GATE_386TRAPGATE = 15
};

enum DESCRIPTORFLAG
{
	DF_DEFSIZE = 4, DF_GRANULARITY = 8
};

/*

 Code / Data descriptor format
 ______________________________________________________________________________

 0B: Limit 0-7
 1B: Limit 8-15
 2B: Base 0-7
 3B: Base 8-15
 4B: Base 16-23
 5B: Access
 0b: Accessed
 1b: Writable
 2b: Expansion direction
 3b: Executable
 4b: Non-system
 5b: Privilege
 6b: -//-
 7b: Present
 6B: Flags, Limit 16-19
 4b: 0
 5b: 0
 6b: Default size
 7b: Granularity
 7B: Base 24-31
 ______________________________________________________________________________
 */

typedef struct __USERDESC
{
	word limit;
	word baseLo;
	byte baseHiLo;
	byte access;
	byte flags :4;
	byte limitHi :4;
	byte baseHi;
} USERDESC;

typedef struct __GATE
{
	word offsetLo;
	word selector;
	byte wordCount;
	byte access;
	word offsetHi;
} GATE;

typedef struct __RAWDESC
{
	dword lo, hi;
} RAWDESC;

typedef struct __TSS
{
	word backlink, __blh;
	dword esp0;
	word ss0, __ss0h;
	dword esp1;
	word ss1, __ss1h;
	dword esp2;
	word ss2, __ss2h;
	dword cr3;
	dword eip;
	dword eflags;
	dword eax, ecx, edx, ebx;
	dword esp, ebp, esi, edi;
	word es, __esh;
	word cs, __csh;
	word ss, __ssh;
	word ds, __dsh;
	word fs, __fsh;
	word gs, __gsh;
	word ldt, __ldth;
	word trace, bitmap;
} TSS;

typedef struct __INTERRUPT
{
	word offsetLo;
	word selector;
	byte wordCount;
	byte access;
	word offsetHi;
} INTERRUPT;

typedef union __DESCRIPTOR
{
	USERDESC desc;
	GATE gate;
	INTERRUPT intr;
	RAWDESC raw;
} DESCRIPTOR;

#define DA_KRNINTR (DA_PRESENT | DA_GATE_386INTRGATE)

#define GDT_ADDR (DESCRIPTOR *)0x10006
#define IDT_ADDR (DESCRIPTOR *)0x406

#define KRN_SEG_CODE 0x08
#define KRN_SEG_DATA 0x10 

typedef word DESCRIPTORINDEX;
typedef word SELECTOR;

void ltr(SELECTOR selector);
SELECTOR str(void);

void descriptor_set(const DESCRIPTOR *desc_table,
		const DESCRIPTORINDEX desc_idx, const DESCRIPTOR* desc);
void descriptor_get(const DESCRIPTOR *desc_table,
		const DESCRIPTORINDEX desc_idx, DESCRIPTOR* desc);

#endif /*DESCRIPTOR_H*/
