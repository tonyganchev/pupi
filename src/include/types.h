/*
 +----------------------------------------------------------------------------+
 |                                                                            |
 |  types.h - basic types that normal programmers should use                  |
 |                                                                            |
 +----------------------------------------------------------------------------+
 */

#ifndef HDR_TYPES
#define HDR_TYPES

typedef unsigned char byte;
typedef unsigned short word; /* 2B */
typedef unsigned long dword; /* 4B */
//typedef int bool;

typedef enum
{
	false = 0,
	true = !false
} bool;

//typedef unsigned size_t;

//#define NULL ((void *) 0)

//#define ABS(x) ((x) < 0 ? -(x) : (x))


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

typedef struct __INTERRUPT
{
	word offsetLo;
	word selector;
	byte wordCount;
	byte access;
	word offsetHi;
} INTERRUPT;

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

typedef union __DESCRIPTOR
{
	USERDESC desc;
	GATE gate;
	INTERRUPT intr;
	RAWDESC raw;
} DESCRIPTOR;

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

typedef void (*INTRHANDLER)();

#endif
