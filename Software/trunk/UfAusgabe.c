
#include <avr/io.h>
#include "Transistortester.h"


void DiodeSymbol_withPins(uint8_t nn) {
  uint8_t anode_nr;
  uint8_t cathode_nr;
  anode_nr = diodes.Anode[nn];
  cathode_nr = diodes.Cathode[nn];
#if FLASHEND > 0x1fff
 // enough memory (>8k) to sort the pins
 #if EBC_STYLE == 321
  // the higher test pin number is left side
  if (anode_nr > cathode_nr) {
     DiodeSymbol_ApinCpin(nn);		// 1->|-3
  } else {
     DiodeSymbol_CpinApin(nn);	 // 1-|<-3
  }
 #else
  // the higher test pin number is right side
  if (anode_nr < cathode_nr) {
     DiodeSymbol_ApinCpin(nn);		// 1->|-3
  } else {
     DiodeSymbol_CpinApin(nn);	 // 1-|<-3
  }
 #endif
#else
  // too less memory to sort the pins
  DiodeSymbol_ApinCpin(nn);		// 1->|-3
#endif
  lcd_space();
}
void DiodeSymbol_ApinCpin(uint8_t nn) {
  lcd_testpin(diodes.Anode[nn]);
  DiodeSymbol_ACpin(nn);
}
void DiodeSymbol_ACpin(uint8_t nn) {
  lcd_MEM_string(AnKat_str);	// "->|-"
  lcd_testpin(diodes.Cathode[nn]);
}
void DiodeSymbol_CpinApin(uint8_t nn) {
  lcd_testpin(diodes.Cathode[nn]);
  DiodeSymbol_CApin(nn);
}
void DiodeSymbol_CApin(uint8_t nn) {
  lcd_MEM_string(KatAn_str);	// "-|<-"
  lcd_testpin(diodes.Anode[nn]);
}

void SerienDiodenAusgabe() {
   uint8_t first;
   uint8_t second;
   first = diode_sequence >> 4;
   second = diode_sequence & 3;
   DiodeSymbol_ApinCpin(first);		// 1->|-2
   DiodeSymbol_ACpin(second);		//  ->|-3
   UfAusgabe(diode_sequence);
}


//******************************************************************
// output of flux voltage for 1-2 diodes in row 2
// bcdnum = Numbers of both Diodes:
// higher 4 Bit  number of first Diode
// lower 4 Bit  number of second Diode (Structure diodes[nn])
// if number >= 3  no output is done
void UfAusgabe(uint8_t bcdnum) {
   if (ResistorsFound > 0) { //also Resistor(s) found
      lcd_space();
      lcd_data(LCD_CHAR_RESIS3);	// special symbol or R
   }
   lcd_line2(); 				//2. row
   lcd_MEM_string(Uf_str);			//"Uf="
   mVAusgabe(bcdnum >> 4);
   mVAusgabe(bcdnum & 0x0f);
}
void mVAusgabe(uint8_t nn) {
   if (nn < 6) {
      // Output in mV units
      Display_mV(diodes.Voltage[nn],3);
      lcd_space();
   }
}

