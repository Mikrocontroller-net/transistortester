#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "Transistortester.h"

// check some EEprom values for correct values 
void EE_check_init(void) {
#ifndef USE_EEPROM
  uint8_t tt;		// read test value
  tt = (uint8_t)eeprom_read_byte(&EE_ESR_ZEROtab[0]);
  // this value will never be changed by calibration
  if (tt != ESR_ZERO) goto init_ee;
 #ifdef AUTO_CAL
  uint8_t tt0;		// value of first c_zero_tab
  uint8_t ww;		// loop counter
  tt0 = tt;			// init tt0 value
  for (ww=0;ww<7;ww++) {		//checking loop
      tt = (uint8_t)eeprom_read_byte(&c_zero_tab[ww]);
      if (ww == 0) tt0 = tt;		// save first value
      // for forth element, tt must be tt0 or tt0+1
      if ((ww == 3) && ((tt != tt0) && (tt != (tt0+1)) && (tt != (tt0+2)) )) goto init_ee;
      if ((tt > 190) || (tt < 10)) goto init_ee;	// value too low or too big
  }
 #endif
 return;

init_ee:
 // init all EEprom values
 lcd_line1();
 lcd_data('E');
 lcd_data('E');
 #ifdef AUTO_CAL
 // write the correction value for ADC internal 1.1V reference 
 (void) eeprom_write_byte((uint8_t *)(&RefDiff), (uint8_t)REF_R_KORR);	// offset for true reference Voltage
 // write the correction value for comparator 1.1V reference
 (void) eeprom_write_word((uint16_t *)(&ref_offset), REF_C_KORR);	// hold zero offset + slew rate dependend offset
 // write all 7 c_zero_tab values
 (void) eeprom_write_word((uint16_t *)(&c_zero_tab[0]),((C_NULL)*256) + (C_NULL));
 (void) eeprom_write_byte((uint8_t *)(&c_zero_tab[2]),(C_NULL+TP2_CAP_OFFSET));
 (void) eeprom_write_byte((uint8_t *)(&c_zero_tab[3]),(C_NULL+2));
 (void) eeprom_write_byte((uint8_t *)(&c_zero_tab[4]),(C_NULL+TP2_CAP_OFFSET));
 (void) eeprom_write_word((uint16_t *)(&c_zero_tab[5]),((C_NULL)*256) + (C_NULL));
  #endif
 // write 4 EE_ESR_ZEROtab values
 (void) eeprom_write_word((uint16_t *)(&EE_ESR_ZEROtab[0]),(ESR_ZERO*256)+ESR_ZERO);
 (void) eeprom_write_word((uint16_t *)(&EE_ESR_ZEROtab[2]),(ESR_ZERO*256)+ESR_ZERO);
#ifdef WITH_ROTARY_SWITCH
// (void) eeprom_write_byte(&EE_RotarySwitch,0); 		// no switch is detected
#endif
 wait_about1s();
#endif
}

