#include <avr/io.h>
#include "Transistortester.h"

#define MAX_CS 150	/* maximum key hold time in 10ms units  */

/* wait max_time or previous key press */
/* return value: !=0 == key is pressed for x*10ms, 0 == no key pressed, time has expired */
uint8_t wait_for_key_ms(int max_time) {
  uint8_t key_cs;			// period of key hold down in 10ms units
  uint8_t key_pressed;
  // if key is pressed, return 1
  // if max_time == 0 , do not count, wait endless
  wait_about200ms();
// wait 28 seconds or 5 seconds (if repeat function)
  key_cs = 0;				// set key flag to not pressed
  key_pressed = 0x55;
  while (max_time >= 0) {
     wait_about10ms();
     key_pressed += key_pressed;	// multiply with 2 is shift to left
     if ((RST_PIN_REG & (1<<RST_PIN)) == 0) key_pressed++; // append a 1
     key_pressed &= 0x3f;
     if(key_pressed == 0x3f) {
        // If the key is pressed again... 
        if (key_cs == 0) key_cs = 4;	// must be 7 times for 10ms hold down
        key_cs++;			// count the time, the key is hold down
        if (key_cs >= MAX_CS) break;
     } else {
        // key_pressed is zero, if the key is not pressed for 80 ms
        if ((key_pressed == 0) && (key_cs != 0)) break;	// return, if key is released again
     }
     wdt_reset();
     if (max_time > 0) {		// count only, if counter > 0
        max_time -= 10;			// 10 ms are done, count down
        if (max_time == 0) max_time = -1;	// never count to zero, zero is endless!
     }
  }
 return(key_cs);		// return period key is pressed within the specified time
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

