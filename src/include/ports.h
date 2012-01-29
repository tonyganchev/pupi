/*
+----------------------------------------------------------------------------+
|                                                                            |
|  types.h - basic types that normal programmers should use                  |
|                                                                            |
+----------------------------------------------------------------------------+
*/

#ifndef HDR_PORTS
	#define HDR_PORTS

#include "types.h"

extern byte inportb (word _port);
extern void outportb (word _port, byte _data);
extern word inportw (word _port);
extern void outportw (word _port, word _data);

#endif
