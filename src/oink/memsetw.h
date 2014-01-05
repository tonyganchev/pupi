/*
 +----------------------------------------------------------------------------+
 |                                                                            |
 |  types.h - basic types that normal programmers should use                  |
 |                                                                            |
 +----------------------------------------------------------------------------+
 */

#ifndef HDR_MEMSETW
#	define HDR_MEMSETW

/* Conversion routines */
//extern void ltoa(long l, char *dest);
//extern void utox(dword u, char *dest, byte digits);
//
//extern void *memcpy(void *dest, const void *src, int count);
//extern void *memset(void *dest, byte val, int count);
extern void *memsetw(void *dest, word val, int count);
//extern int strlen(char *str);
//extern void strcpy(char *dest, const char *src);
//extern int strcmp(const char *s1, const char *s2);

#endif
