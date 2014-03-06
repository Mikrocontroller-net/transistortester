

#include <string.h>



#include "Transistortester.h"





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
 *  for option WITH_MENU digits can also be 5, 6 and 7

 */

void DisplayValue(unsigned long Value, int8_t Exponent, unsigned char Unit, unsigned char digits)

{

//  char OutBuffer[15];

#ifdef WITH_MENU
  unsigned long     Limit;
#else
  unsigned int      Limit;
#endif

  unsigned char     Prefix;		/* prefix character */

  uint8_t           Offset;		/* exponent of offset to next 10^3 step */

  uint8_t           Index;		/* index ID */

  uint8_t           Length;		/* string length */





  Limit = 100;				/* scale value down to 2 digits */

  if (digits == 3) Limit = 1000;	/* scale value down to 3 digits */

  if (digits == 4) Limit = 10000;	/* scale value down to 4 digits */
#ifdef WITH_MENU
  if (digits == 5) Limit = 100000;	/* scale value down to 5 digits */
  if (digits == 6) Limit = 1000000;	/* scale value down to 6 digits */
  if (digits == 7) Limit = 10000000;	/* scale value down to 7 digits */
#endif

  while (Value >= Limit)

  {

    Value += 5;				/* for automatic rounding */

    Value = Value / 10;			/* scale down by 10^1 */

    Exponent++;				/* increase exponent by 1 */

  }





  /*

   *  determine prefix

   */

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

  /* convert value into string */

#ifdef WITH_MENU
  ultoa(Value, OutBuffer, 10);
#else
  utoa((unsigned int)Value, OutBuffer, 10);
#endif

  Length = strlen(OutBuffer);

#ifdef WITH_MENU
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

  if (Index == 1)

    { /* use no nano */

      Index++;				/* use mikro instead of nano */

      Offset += 3;			/* can be 3,4 or 5 */

    }

#endif

  Prefix = MEM_read_byte((uint8_t *)(&PrefixTab[Index]));   /* look up prefix in table */

  /*

   *  display value

   */




  /* position of dot */

  Exponent = Length - Offset;		/* calculate position */



  if (Exponent <= 0)			/* we have to prepend "0." */

  {

    /* 0: factor 10 / -1: factor 100 */

//    lcd_data('0');

    lcd_data('.');

#ifdef NO_NANO

    while (Exponent < 0)

      {

       lcd_data('0');	/* extra 0 for factor 10 */

       Exponent++;

      }

#else

    if (Exponent < 0) lcd_data('0');	/* extra 0 for factor 100 */

#endif

  }



  if (Offset == 0) Exponent = -1;	/* disable dot if not needed */



  /* adjust position to array or disable dot if set to 0 */

//  Exponent--;



  /* display value and add dot if requested */

  Index = 0;

  while (Index < Length)		/* loop through string */

  {

    lcd_data(OutBuffer[Index]);		/* display char */

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

