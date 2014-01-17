#include <avr/io.h>
#include "Transistortester.h"


/* wait max_time or previous key press */
/* return value: 1 == key is pressed, 0 == time expired */
uint8_t wait_for_key_ms(int max_time) {
  // if key is pressed, return 1
  // if max_time == 0 , do not count, wait endless
  wait_about200ms();
// wait 28 seconds or 5 seconds (if repeat function)
  while (max_time >= 0) {
     wait_about10ms();
     if(!(ON_PIN_REG & (1<<RST_PIN))) {
        // If the key is pressed again... 
        // goto start of measurement 
        return(1);    // key is pressed
     }
     wdt_reset();
     if (max_time > 0) {		// count only, if counter > 0
        max_time -= 10;			// 10 ms are done, count down
        if (max_time == 0) max_time = -1;	// never count to zero, zero is endless!
     }
  }
 return(0);		// no key pressed within the specified time
}

#ifdef WAIT_LINE2_CLEAR
/* wait 5 seconds or previous key press, then clear line 2 of LCD and */
/* set the cursor to the beginning of line 2 */
void wait_for_key_5s_line2(void) {
  wait_for_key_ms(SHORT_WAIT_TIME);
  lcd_line2(); //2. row 
  lcd_clear_line();		// clear the whole line
  lcd_line2(); //2. row 
}
#endif

