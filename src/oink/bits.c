#include "types.h"

byte byte_lo(word w) {
	return (byte) w;
}

byte byte_hi(word w) {
	return (byte) (w >> 8);
}

word word_lo(dword dw) {
	return (word) dw;
}

word word_hi(dword dw) {
	return (word) (dw >> 16);
}

word word_make(byte hi, byte lo) {
	return ((word) hi << 8) | (word) lo;
}
