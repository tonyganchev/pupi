/**
 * \file bits.h
 * \brief This header provides some often used bit operations
 */

#ifndef HDR_BITS
#	define HDR_BITS

#include "types.h"

extern byte byte_lo(word w);
extern byte byte_hi(word w);
extern word word_lo(dword dw);
extern word word_hi(dword dw);
extern word word_make(byte hi, byte lo);

#endif /*HDR_BITS*/
