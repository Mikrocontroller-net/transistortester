#include "Transistortester.h"
/*  ShowData shows the Software version number and */
/*  the calibration data at the 2-line or 4-line LCD */
#ifdef WITH_MENU
void ShowData(void) {
#ifdef WITH_ROTARY_SWITCH
show_page_1:
#endif
  lcd_clear();
  lcd_MEM2_string(VERSION_str);	// "Version x.xxk"
  lcd_line2();
  lcd_MEM2_string(R0_str);	// "R0="
  DisplayValue(eeprom_read_byte(&EE_ESR_ZEROtab[2]),-2,' ',3);
  DisplayValue(eeprom_read_byte(&EE_ESR_ZEROtab[3]),-2,' ',3);
  DisplayValue(eeprom_read_byte(&EE_ESR_ZEROtab[1]),-2,LCD_CHAR_OMEGA,3);
#if (LCD_LINES > 3)
  lcd_line3();
#else
  wait_for_key_ms(MIDDLE_WAIT_TIME);
 #ifdef WITH_ROTARY_SWITCH
  if (rotary.incre > FAST_ROTATION) return;	// fast rotation ends the function
  if (rotary.count < 0) goto show_page_1;
show_page_2:
 #endif
  lcd_clear();
#endif
  /* output line 3 */
  lcd_MEM_string(RIHI); // "RiHi="
  DisplayValue(RRpinPL,-1,LCD_CHAR_OMEGA,3);
#if (LCD_LINES > 3)
  lcd_line4();
#else
  lcd_line2();
#endif
  /* output line 4 */
  lcd_MEM_string(RILO); // "RiLo="
  DisplayValue(RRpinMI,-1,LCD_CHAR_OMEGA,3);

  wait_for_key_ms(MIDDLE_WAIT_TIME);
#ifdef WITH_ROTARY_SWITCH
  if (rotary.incre > FAST_ROTATION) return;	// fast rotation ends the function
#if (LCD_LINES > 3)
  if (rotary.count < 0) goto show_page_1;
 #else
  if (rotary.count < -1) goto show_page_1;
  if (rotary.count < 0) goto show_page_2;
 #endif
show_page_3:
#endif
  lcd_clear();
  lcd_MEM_string(C0_str);                       //output "C0 "
  DisplayValue(eeprom_read_byte(&c_zero_tab[5]),0,' ',3);		//output cap0 1:3
  DisplayValue(eeprom_read_byte(&c_zero_tab[6]),0,' ',3);		//output cap0 2:3
  DisplayValue(eeprom_read_byte(&c_zero_tab[2]),-12,'F',3);		//output cap0 1:2
  lcd_line2();
  lcd_spaces(3);
  DisplayValue(eeprom_read_byte(&c_zero_tab[1]),0,' ',3);		//output cap0 3:1
  DisplayValue(eeprom_read_byte(&c_zero_tab[4]),0,' ',3);		//output cap0 3:2
  DisplayValue(eeprom_read_byte(&c_zero_tab[0]),-12,'F',3);		//output cap0 2:1
#if (LCD_LINES > 3)
  lcd_line3();
#else
  wait_for_key_ms(MIDDLE_WAIT_TIME);
 #ifdef WITH_ROTARY_SWITCH
  if (rotary.incre > FAST_ROTATION) return;	// fast rotation ends the function
  if (rotary.count < -2) goto show_page_1;
  if (rotary.count < -1) goto show_page_2;
  if (rotary.count < 0) goto show_page_3;
show_page_4:
 #endif
  lcd_clear();
#endif
  /* output line 7 */
  lcd_MEM2_string(REF_C_str);	// "REF_C="
  i2lcd((int16_t)eeprom_read_word((uint16_t *)(&ref_offset)));
#if (LCD_LINES > 3)
  lcd_line4();
#else
  lcd_line2();
#endif
  /* output line 8 */
  lcd_MEM2_string(REF_R_str);  // "REF_R="
  i2lcd((int8_t)eeprom_read_byte((uint8_t *)(&RefDiff)));
  wait_for_key_ms(MIDDLE_WAIT_TIME);
#ifdef WITH_ROTARY_SWITCH
  if (rotary.incre > FAST_ROTATION) return;	// fast rotation ends the function
 #if (LCD_LINES > 3)
  if (rotary.count < -1) goto show_page_1;
  if (rotary.count < 0) goto show_page_3;
 #else
  if (rotary.count < -3) goto show_page_1;
  if (rotary.count < -2) goto show_page_2;
  if (rotary.count < -1) goto show_page_3;
  if (rotary.count < 0) goto show_page_4;
 #endif
#endif
#ifdef WITH_GRAPHICS
 ShowIcons();		// show all Icons
#endif
}
#endif
