

#include <stdlib.h>
#include <string.h>

#include "Transistortester.h"

#if defined(WITH_XTAL) || defined(WITH_MENU)
#define LONG_OUTPUT      // to allow more digits for crystal frequency
#endif

/* ************************************************************************
 *   display of values and units
 * ************************************************************************ */

/*
 *  display value and unit
 *  - max. 4 (7) digits excluding "." and unit
 *
 *  requires:
 *  - value
 *  - exponent of factor related to base unit (value * 10^x)
 *    e.g: p = 10^-12 -> -12
 *  - unit character (0 = none)
 *  digits = 2 to 4 
 *  for option LONG_OUTPUT  digits can also be 5, 6 and 7
 *  Flag 0x10 for output with leading zeros
 *  Flag 0x20 for '+' Output, if not '-' 
 */

void DisplayValue(signed long signedValue, int8_t Exponent, unsigned char Unit, uint8_t xdigits)
{

  char OutBuffer[14];
#ifdef LONG_OUTPUT
  unsigned long     Limit;
#else
  uint16_t      Limit;
#endif
  unsigned char     Prefix;		/* prefix character */
  unsigned char     Offset;		/* exponent of offset to next 10^3 step */
  uint8_t           Index;		/* index ID */
  uint8_t           digits;		/* xdigits without option */
  uint8_t           Length;		/* string length */
  uint8_t	    Rshift;		/* right shift of significant bits with leading '0' */


  unsigned long Value=signedValue;
  digits = xdigits & 0x0f;
  for(Prefix=0;Prefix<6;Prefix++) OutBuffer[Prefix] = '0';

#if FLASHEND > 0x1fff
  if (signedValue<0) {
     lcd_data('-');
     Value=-signedValue;
  } else {
     if ((xdigits & 0x20) != 0) lcd_data('+');
  }
#endif

  Limit = 100;				/* scale value down to 2 digits */
  if (digits == 3) Limit = 1000;	/* scale value down to 3 digits */
  if (digits == 4) Limit = 10000;	/* scale value down to 4 digits */
#ifdef LONG_OUTPUT
  if (digits == 5) Limit = 100000;	/* scale value down to 5 digits */
  if (digits == 6) Limit = 1000000;	/* scale value down to 6 digits */
  if (digits == 7) Limit = 10000000;	/* scale value down to 7 digits */
#endif

  while (Value >= Limit)
  {
    Value += 5;				/* for automatic rounding */
    // perhaps TODO: the above is not really correct, can do double rounding (2000.45 -> 2000.5 -> 2001)
    Value = Value / 10;			/* scale down by 10^1 */
    Exponent++;				/* increase exponent by 1 */
  }


  /*
   *  determine prefix
   */

#if 0
  Length = Exponent + 12;
  if ((int8_t)Length <  0) Length = 0;		/* Limit to minimum prefix */
  if (Length > 18) Length = 18;		/* Limit to maximum prefix */
  Index = Length / 3;
  Offset = Length % 3;
  if (Offset > 0)
    {
      Index++;				/* adjust index for exponent offset, take next prefix */
      Offset = 3 - Offset;		/* reverse value (1 or 2) */
    }
#endif
  // the following is much more concise
  // also removes the limits checking, since limit errors should never happen, and if they do, they'd show _incorrect_ results;
  // showing a garbage prefix character seems preferable to something correct-looking but off by some power of 10
  Index = ((unsigned char)(Exponent+17))/3;
  Offset = ((unsigned char)(6-Exponent))%3;

  /* convert value into string */

#ifdef LONG_OUTPUT
  ultoa(Value, &OutBuffer[6], 10);	// sprintf(&OutBuffer[6],"%ld%c",Value,(char)0);
#else
  utoa((uint16_t)Value, &OutBuffer[6], 10);	// sprintf(&OutBuffer[6],"%d%c",Value,(char)0);
#endif

  Length = strlen(&OutBuffer[6]);

#ifdef LONG_OUTPUT
 #ifdef NO_NANO
  if ((Length > (4+Offset))  && (Index != 0))
 #else
  if (Length > (4+Offset)) 
 #endif
    {
      Index++;			/* take next prefix for better readable string */
      Offset += 3;		/* decimal point 3 to the left */
    }
#endif

#ifdef NO_NANO
  if (Index == 2)
    { /* use no nano, prefix 'f','p','n','u','m',' ','k','M' */
      Index++;				/* use mikro instead of nano */
      Offset += 3;			/* can be 3,4 or 5 */
    }
#endif

  Prefix = MEM_read_byte((unsigned char *)(&PrefixTab[Index]));   /* look up prefix in table */

  /*

   *  display value

   */


  /* position of dot */
  if (((xdigits & 0x10) != 0) && (digits > Length)) {
     Rshift = (digits - Length);	// use previous filled '0' characters
  } else {
     Rshift = 0;
  }

  Exponent = (Length + Rshift) - Offset;		/* calculate position */

  if (Exponent <= 0)			/* we have to prepend "0." */
  {
    /* 0: factor 10 / -1: factor 100 */
//    lcd_data('0');
    lcd_data('.');

#if 0
#ifdef NO_NANO
    while (Exponent < 0)
      {
       lcd_data('0');	/* extra 0 for factor 10 */
       Exponent++;
      }
#else
    if (Exponent < 0) lcd_data('0');	/* extra 0 for factor 100 */
#endif
#else
    Rshift = Offset - Length;
#endif
  }



  if (Offset == 0) Exponent = -8;	/* disable dot if not needed */



  /* adjust position to array or disable dot if set to 0 */

//  Exponent--;


  /* display value and add dot if requested */
  Index = 0;
  while (Index < (Length+Rshift))		/* loop through string */
  {
    lcd_data(OutBuffer[Index+6-Rshift]);		/* display char */
    Index++;				/* next one */
    if (Index == Exponent) {
      lcd_data('.');			/* display dot */
    }
  }



  /* display prefix and unit */

  if (Prefix != 0) {
     lcd_data(Prefix);
  }

  if (Unit != 0) {
      lcd_data(Unit);
  }
  return;
}

void DisplayValue16(uint16_t Value, int8_t Exponent, unsigned char Unit, uint8_t digits)
{
 DisplayValue((signed long)Value, Exponent, Unit, digits);
}

void Display_mV(uint16_t xx, uint8_t ll) {
 DisplayValue((long)xx,-3,'V',ll);
}
#if defined(WITH_MENU) || defined(WITH_XTAL)
void Display_Hz(unsigned long xx, uint8_t dd) {
 DisplayValue(xx,0,'H',dd);
 lcd_data('z');
}
#endif
