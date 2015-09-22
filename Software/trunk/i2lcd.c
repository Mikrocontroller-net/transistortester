#include <avr/io.h>
#include <avr/common.h>
#include <stdlib.h>
#include "Transistortester.h"
#include "lcd-routines.h"

#if FLASHEND > 0x1fff
void i2lcd(int iw) {
  if (iw < 0) {
     lcd_data('-');
     iw = -iw;
  }
  lcd_string(utoa((unsigned int)iw, outval, 10));
}
#endif

void u2lcd(uint16_t uw) {
  lcd_string(utoa(uw, outval, 10));
}

void u2lcd_space(uint16_t uw) {
  u2lcd(uw);
  lcd_space();
}

#if FLASHEND > 0x1fff
void i2lcd_space(int16_t uw) {
  i2lcd(uw);
  lcd_space();
}
#endif
