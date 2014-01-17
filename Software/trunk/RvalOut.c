
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "Transistortester.h"

void RvalOut(uint8_t nrr) {	
   // output of resistor value
#if FLASHEND > 0x1fff
   uint16_t rr;
   if ((resis[nrr].rx < 100) && (inductor_lpre == 0)) {
      rr = GetESR(resis[nrr].ra,resis[nrr].rb);
      DisplayValue(rr,-2,LCD_CHAR_OMEGA,3);
   } else {
      DisplayValue(resis[nrr].rx,-1,LCD_CHAR_OMEGA,4);
   }
#else
   DisplayValue(resis[nrr].rx,-1,LCD_CHAR_OMEGA,4);
#endif
   lcd_space();
 }

