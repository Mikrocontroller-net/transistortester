
#include <avr/io.h>
#include "Transistortester.h"



void SerienDiodenAusgabe() {
   uint8_t first;
   uint8_t second;
   first = diode_sequence >> 4;
   second = diode_sequence & 3;
   lcd_testpin(diodes.Anode[first]);
   lcd_fix_string(AnKat);	//"->|-"
   lcd_testpin(diodes.Cathode[first]);
   lcd_fix_string(AnKat);	//"->|-"
   lcd_testpin(diodes.Cathode[second]);
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
   lcd_fix_string(Uf_str);			//"Uf="
   mVAusgabe(bcdnum >> 4);
   mVAusgabe(bcdnum & 0x0f);
}
void mVAusgabe(uint8_t nn) {
   if (nn < 3) {
      // Output in mV units
      DisplayValue(diodes.Voltage[nn],-3,'V',3);
      lcd_space();
   }
}

