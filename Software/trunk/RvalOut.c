
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "Transistortester.h"

void RvalOut(uint8_t nrr) {	
   // output of resistor value
   uint8_t lopin, hipin;
#if FLASHEND > 0x1fff
   uint16_t rr;
   if ((ResistorVal[nrr] < 100) && (inductor_lpre == 0)) {
      lopin = TP1;
      hipin = TP3;
      if (nrr == 0) hipin = TP2;
      if (nrr == 2) lopin = TP2;
      rr = GetESR(hipin, lopin);
      DisplayValue(rr,-2,LCD_CHAR_OMEGA,3);
   } else {
      DisplayValue(ResistorVal[nrr],-1,LCD_CHAR_OMEGA,4);
   }
#else
   DisplayValue(ResistorVal[nrr],-1,LCD_CHAR_OMEGA,4);
#endif
   lcd_space();
 }

