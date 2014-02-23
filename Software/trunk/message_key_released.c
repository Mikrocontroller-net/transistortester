#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// Clear display, show Message XX_str and wait for key release

#ifdef WITH_MENU
void message_key_released(const unsigned char XX_str[]) {
 
  uint8_t times;
  // show the external voltage
  lcd_clear();
  lcd_MEM2_string(XX_str);		// display MEM2_str in row 1
  for (times=0;times<100;times++) {
     wait_about10ms();
    if((RST_PIN_REG & (1<<RST_PIN))) break;	// key is released
  }
  lcd_line2();
  return;
} /* end message_key_released() */

#endif
