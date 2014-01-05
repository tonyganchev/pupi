#include "types.h"
#include "string.h"

//void ltoa(long l, char *dest);
//void utox(DWORD u, char *dest, BYTE digits);

//void strcpy(char *dest, const char *src)
//{
//	while (*src != '\0')
//		*dest++ = *src++;
//	*dest = '\0';
//}
//
//int strcmp(const char *left, const char *right)
//{
//	while (*left != '\0' && *right != '\0')
//	{
//		int diff = *left++ - *right++;
//		if (diff != 0)
//			return diff;
//	}
//	return *left++ - *right++;
//}

void ltoa(long l, char *dest)
{
	char str[12];
	byte i = 10, neg = 0;

	if (l < 0)
	{
		l = -l;
		neg = 1;
	}

	if (!l)
	{
		strcpy(dest, "0\0");
		return;
	}

	str[11] = '\0';

	while (l != 0)
	{
		str[i--] = l % 10 + '0';
		l /= 10;
	}
	if (neg)
		str[i] = '-';
	else
		i++;
	strcpy(dest, str + i);
}

void utox(dword u, char *dest, byte digits)
{
	short i;
	dest[digits] = '\0';
	for (i = digits - 1; i >= 0; i--)
	{
		dest[i] = u % 16 > 9 ? 55 + u % 16 : 48 + u % 16;
		u /= 16;
	}
}

//void *memcpy(void *dest, const void *src, size_t count)
//{
//	if (dest != NULL && src != NULL)
//		for (; count > 0; count--)
//			*(byte *) dest++ = *(byte *) src++;
//	return dest;
//}
//
//void *memset(void *dest, int val, size_t count)
//{
//	if (dest != NULL)
//		for (; count > 0; count--)
//			*(unsigned char *) dest++ = (unsigned char) val;
//	return dest;
//}

void *memsetw(void *dest, word val, int count)
{
	if (dest != NULL)
		for (; count > 0; count -= sizeof(word))
			*(word *) dest++ = val;
	return dest;
}

//int strlen(const char *str)
//{
//	const char *str_end = str;
//	for (; *str_end != '\0'; ++str_end)
//		;
//	return str_end - str;
//}
