#include <avr/io.h>
#include "Transistortester.h"
#include "lcd-routines.h"

#define MAX_CS 150	/* maximum key hold time in 10ms units  */

/* *********************************************************** */
/* wait max_time or previous key press */
/* return value: !=0 == key is pressed for x*10ms, 0 == no key pressed, time has expired */
/* Additionally the movement of rotary encoder is monitored, if */
/* the constant WITH_ROTARY_SWITCH is defined.                  */
/* *********************************************************** */
uint8_t wait_for_key_ms(int max_time) {
  uint8_t key_cs;			// period of key hold down in 10ms units
  int8_t kk;
  uint8_t key_pressed;
  int count_time;

#if WITH_ROTARY_SWITCH != 4
 // normal rotary encoder operation, sampling with 200us
 #define WWend 25
 #define WaitRotary wait200us
#else
 // separate Up / Down key instead of rotary encoder, sample with 1ms
 #define WWend 5
 #define WaitRotary wait1ms
#endif

#if (LCD_ST_TYPE == 7920)
  lcd_refresh();
#endif

#ifdef WITH_ROTARY_SWITCH
  uint8_t wait_time;
  // update the state of rotary switch 
  ROTARY_A_DDR &= ~(1<<ROTARY_A_PIN);	// switch A to input
  ROTARY_B_DDR &= ~(1<<ROTARY_B_PIN);	// switch B to input
  wait1ms();
 #if WITH_ROTARY_SWITCH == 4
  rotary.ind = ROT_MSK+1;		//initilize with next call of check_rotary()
 #else
  rotary.count = 0;			// clear count, but don't clear the state history
  rotary.state[(rotary.ind+ROT_MSK-2)&ROT_MSK] = ' ' - '0';  // for debugging
 #endif
#endif

  kk = 100;
  while (kk >= 0)  { /* wait up to 500ms until key is released */
#ifdef WITH_ROTARY_SWITCH
     uint8_t ww;
     for (ww=0; ww < WWend; ww++) {
        check_rotary();		// check for change of switch state of the rotary encoder
        if ((RST_PIN_REG & (1<<RST_PIN)) != 0)  break; // key is released
        WaitRotary();		// wait the predefined time
     }
     if (ww < WWend) break;
#else
     if ((RST_PIN_REG & (1<<RST_PIN)) != 0)  break; // key is released
     wait_about5ms();		// wait 5 ms (sleep)
#endif
     kk--;
  }
// wait 28 seconds or 5 seconds (if repeat function)
  key_cs = 0;				// set key flag to not pressed
  key_pressed = 0x55;
  count_time = max_time;
#ifdef WITH_ROTARY_SWITCH
  wait_time = 0;
#endif
  while (count_time >= 0) {
#ifdef WITH_ROTARY_SWITCH
     // monitor rotary switch during wait
     uint8_t ww;
     for (ww=0; ww < (WWend*2) ;ww++) {
        check_rotary();		// check for change of switch state of the rotary encoder
        WaitRotary();		// wait the predefined time, total 10ms
     }
#else
     wait_about10ms();
#endif
     key_pressed += key_pressed;	// multiply with 2 is shift to left
     if ((RST_PIN_REG & (1<<RST_PIN)) == 0) key_pressed++; // append a 1
     key_pressed &= 0x3f;
     if(key_pressed == 0x3f) {
        // If the key is pressed again... 
        if (key_cs == 0) {
           // first time, when key down is detected
           key_cs = 4;	// must be 7 times for 10ms hold down
           count_time = max_time;	// begin new time period
        }
        key_cs++;			// count the time, the key is hold down
        if (key_cs >= MAX_CS) break;
     } else {
        // key_pressed is zero, if the key is not pressed for 80 ms
        if ((key_pressed == 0) && (key_cs != 0)) break;	// return, if key is released again
     }
     wdt_reset();
//     wait_time++;
     if (count_time > 0) {		// count only, if counter > 0
        count_time -= 10;		// 10 ms are done, count down
        if (count_time == 0) count_time = -1;	// never count to zero, zero is endless!
     }
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre != 0) {
        wait_time++;	// start time window of 200ms
        if (rotary_switch_present == 0) {
           rotary_switch_present = 1;		// set flag for rotary switch detected
//         eeprom_write_byte((uint8_t *)&EE_RotarySwitch, rotary_switch_present);  // make rotary switch permanent detected 
        }
     }
     if ((wait_time > 20) && (rotary.count != 0)) break;
     if (wait_time > 20) wait_time = 0;		// reset time window
#endif
  } /* end while (count_time >= 0) */
 return(key_cs);		// return period key is pressed within the specified time
}  /* end wait_for_key_5ms(  ) */

#ifdef WITH_ROTARY_SWITCH
/* *********************************************************** */
/* Check for state change of rotary encoder switches and save  */
/* the history. If right turn or left turn is detected,        */
/* increment or decrement the rotary.count                     */
/* The absolute value of rotary steps are hold in rotary.incre . */
/* *********************************************************** */
void check_rotary(void) {
 #if WITH_ROTARY_SWITCH != 4
  // normal rotary encoder with two switches
  uint8_t new_state;
  uint8_t old_ind;
  new_state = 0;		// reset to A and B switch at low level
  if ((ROTARY_A_REG & (1<<ROTARY_A_PIN)) != 0)  new_state = 1;	// switch A is high
  if ((ROTARY_B_REG & (1<<ROTARY_B_PIN)) != 0)  new_state |= 2;	// switch B is high
  if (rotary.ind > ROT_MSK) {	// index is out of range, initialize
//    rotary.state[0] = new_state;
    rotary.state[0] = '>' - '0';
    rotary.state[1] = new_state;
//    for (rotary.ind=2; rotary.ind<16;rotary.ind++) rotary.state[rotary.ind] = ' ' - '0';
    rotary.count = 0;	// reset counter to zero
    rotary.ind = 1;	// set initial index to 1
  }
  if (rotary.state[rotary.ind] != new_state) {
     // state of rotary encoder has changed
     // If the WITH_ROTARY_SWITCH is set to 5, all right state changes cause a count of rotary.count
     // This setting can be used for rotary switch without indexed positions (intends), to get
     // the highest resolution. It can also be used for rotary switches, which have four times
     // more indexed positions than switch cycles per turn (360 degree) for counting  every
     // indexed position.
     // If the WITH_ROTARY_SWITCH is set to 1, only 2 of 4 state changes are counted.
     // If the WITH_ROTARY_SWITCH is set to 2 or 3, only one of 4 state changes are counted.
     // Use setting of 2 or 3, if you operate a rotary switch type, which has the same count of 
     // indexed positions as switch cycles per turn to get one count for every indexed position.

     // There are three states to monitor, the new_state, the last rotary.state[rotary.ind] and
     // the state before rotary.state[old_ind] .
     old_ind = (rotary.ind + ROT_MSK) & ROT_MSK;	// the index of the state before rotary.ind
  #if WITH_ROTARY_SWITCH != 3
     // type 1, 2 and 5
     if ((new_state == 0) && (rotary.state[rotary.ind] == 2) && (rotary.state[old_ind] == 3)) {
        // 320 +
        rotary.count++;
     }
     if ((new_state == 0) && (rotary.state[rotary.ind] == 1) && (rotary.state[old_ind] == 3)) {
        // 310 -
        rotary.count--;
     }
  #endif
  #if WITH_ROTARY_SWITCH != 2
     // type 1, 3 and 5
     if ((new_state == 3) && (rotary.state[rotary.ind] == 2) && (rotary.state[old_ind] == 0)) {
        // 023 -
        rotary.count--;
     }
     if ((new_state == 3) && (rotary.state[rotary.ind] == 1) && (rotary.state[old_ind] == 0)) {
        // 013 +
        rotary.count++;
     }
  #endif
  #if WITH_ROTARY_SWITCH == 5
     if ((new_state == 1) && (rotary.state[rotary.ind] == 3) && (rotary.state[old_ind] == 2)) {
        // 231 -
        rotary.count--;
     }
     if ((new_state == 2) && (rotary.state[rotary.ind] == 3) && (rotary.state[old_ind] == 1)) {
        // 132 +
        rotary.count++;
     }
     if ((new_state == 1) && (rotary.state[rotary.ind] == 0) && (rotary.state[old_ind] == 2)) {
        // 201 +
        rotary.count++;
     }
     if ((new_state == 2) && (rotary.state[rotary.ind] == 0) && (rotary.state[old_ind] == 1)) {
        // 102 -
        rotary.count--;
     }
  #endif
    
     rotary.ind = (rotary.ind + 1) & ROT_MSK;	// update to next entry
     rotary.state[rotary.ind] = new_state;	// save the new state to the history
  }
 #else	/* WITH_ROTARY_SWITCH == 4 */
  // special handling with separate up and down switches instead of rotary switch
  if (rotary.ind > ROT_MSK) {	// index is out of range, initialize
     rotary.a_state = 0x55;
     rotary.b_state = 0x55;
     rotary.count = 0;	// reset counter to zero
     rotary.ind = 0;
      
  }
  rotary.a_state = (rotary.a_state + rotary.a_state); 	// *2 = shift left
  if ((ROTARY_A_REG & (1<<ROTARY_A_PIN)) != 0)  rotary.a_state += 1;	// switch A is high
  if (rotary.a_state == 0x80) {
     rotary.count = 1;		// set counter to 1
  }
  rotary.b_state = (rotary.b_state + rotary.b_state); 	// *2 = shift left
  if ((ROTARY_B_REG & (1<<ROTARY_B_PIN)) != 0)  rotary.b_state += 1;	// switch B is high
  if (rotary.b_state == 0x80) {
     rotary.count = -1;		// set counter to -1
  }
 #endif	/* WITH_ROTARY_SWITCH == 4 */
  if (rotary.count >= 0) {
     rotary.incre = rotary.count;	// absolute value of count
  } else {
     rotary.incre = -rotary.count;	// absolute value of count
  }
}  /* end check_rotary() */
#endif	/* ifdef WITH_ROTARY_SWITCH */

#ifdef WAIT_LINE2_CLEAR
/* *********************************************************** */
/* wait 5 seconds or previous key press, then clear last line  of LCD and */
/* set the cursor to the beginning of last line */
/* *********************************************************** */
void wait_for_key_5s_line2(void) {
  uint8_t current_line;
  current_line = lcd_save_position();
  if (last_line_used != 0) {
  if ((last_line_used == 1) && (current_line == (LCD_LINES-1))) {
     // add a + sign at the last location of screen
     lcd_set_cursor(((LCD_LINES - 1) * PAGES_PER_LINE), (LCD_LINE_LENGTH - 1));
     lcd_data('+');
     lcd_set_cursor(((LCD_LINES - 1) * PAGES_PER_LINE), (LCD_LINE_LENGTH - 1));
  };
 #ifdef WITH_ROTARY_SWITCH
  do {
     if (wait_for_key_ms(SHORT_WAIT_TIME) > 0) break;
     // continue waiting, if the key is not pressed, but rotary switch is rotated
  } while (rotary.incre > 0);
 #else
  wait_for_key_ms(SHORT_WAIT_TIME);	// wait until time is elapsed or key is pressed
 #endif
  if ((current_line == (LCD_LINES - 1)) && (last_line_used == 1)) {
     lcd_set_cursor((LCD_LINES-1) * PAGES_PER_LINE,0);
     lcd_clear_line();		// clear the whole line
  }
  lcd_restore_position();
  }  /* end if last_line_used */
}  /* end wait_for_key_5s_line2() */
#endif

