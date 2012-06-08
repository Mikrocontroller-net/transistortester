#include <avr/io.h>
#include "config.h"
#include <stdlib.h>

.global CombineBI2Long
.global CombineII2Long

// This tricky function replaces the long-winded way of gcc compiler
// to build  = high*65536 + low
// if there is any way to shorten the gcc implementation,
// this function can be omitted.

;//unsigned long CombineBI2Long(uint8_t high, unsigned int low)
; { 
CombineBI2Long:
// r24     = high input  (byte)
// r22,r23 = low  input
//  CombineToLong = (unsigned long)(((unsigned long)high * 65536) + low);	//compute total 
// CombineToLong = r22-r25
  clr	r25		//in case of high is byte, clear upper byte
//  ret			// because next function has nothing to do, use that return

;//unsigned long CombineII2Long(unsigned int high, unsigned int low)
; { 
CombineII2Long:
// r24,r25 = high input
// r22,r23 = low  input
//  CombineToLong = (unsigned long)(((unsigned long)high * 65536) + low);	//compute total 
// CombineToLong return value = r22-r25
// 				in case of high is unsigned int, nothing to do
  ret
