
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "Transistortester.h"

void RvalOut(uint8_t nrr) {	
   // output of resistor value
  union {
  uint16_t pw;          // return value from Rnum2pins()
  uint8_t pb[2];        // the pin numbers LowPin and HighPin
  } rpins;      // resistor pin structure to prevent two return parameters

#if FLASHEND > 0x1fff
   uint16_t rr;
   if ((ResistorVal[nrr] < 100) && (inductor_lpre == 0)) {
      rpins.pw = Rnum2pins(nrr);	// compute the pin numbers for resistor nrr
      rr = GetESR(rpins.pb[1], rpins.pb[0]);
      DisplayValue16(rr,-2,LCD_CHAR_OMEGA,3);
   } else {
      DisplayValue(ResistorVal[nrr],-1,LCD_CHAR_OMEGA,4);
   }
#else
   DisplayValue(ResistorVal[nrr],-1,LCD_CHAR_OMEGA,4);
#endif
   lcd_space();
 }

