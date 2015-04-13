#include <avr/io.h>
#include <avr/common.h>
#include <stdlib.h>
#include "Transistortester.h"
#include "lcd-routines.h"

void i2lcd(int iw) {
  if (iw < 0) {
     lcd_data('-');
     iw = -iw;
  }
  lcd_string(utoa((unsigned int)iw, outval, 10));
}
void u2lcd(uint16_t uw) {
  lcd_string(utoa(uw, outval, 10));
}
