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

typedef enum
{
	false = 0,
	true = !false
} bool;

//typedef unsigned size_t;

//#define NULL ((void *) 0)

//#define ABS(x) ((x) < 0 ? -(x) : (x))

#endif
