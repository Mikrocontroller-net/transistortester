#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include "Transistortester.h"

/* set the processor to sleep state */
/* wake up will be done with compare match interrupt of counter 2 */
void sleep_5ms(uint8_t spause){
uint16_t pause;		// pause is the delay in 5ms units
uint8_t t2_offset;

pause = spause;
// spause = 202 = 2s
// spause = 203 = 3s
// spause = 204 = 4s
// spause = 205 = 5s
if (spause > 200) {
    pause = (spause-200) * 200;
}

while (pause > 0)
  {
 #if (F_CPU / 400) > RESTART_DELAY_TICS
   /* 2 ms are more tics than RESTART_DELAY_TICS */
   if ((TCCR1B & ((1<<CS12)|(1<<CS11)|(1<<CS10))) != 0) {
      // timer 1 is running, don't sleep 
      wait5ms();
      pause -= 1;
   } else {
   if (pause > 1)
     {
      t2_offset = ((F_CPU / 100) - RESTART_DELAY_TICS) / TICS_PER_T2_COUNT;
      pause -= 2;
     } else {
      t2_offset = ((F_CPU / 200) - RESTART_DELAY_TICS) / TICS_PER_T2_COUNT;
      pause = 0;
     }
   
   OCR2A = TCNT2 + t2_offset;	/* set the compare value */
   TIMSK2 = (0<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2); /* enable output compare match A interrupt */ 
// TIMSK2 |= (1<<OCIE2A); /* enable output compare match A interrupt */ 
   set_sleep_mode(SLEEP_MODE_PWR_SAVE);
   sleep_mode();
// wake up after output compare match interrupt
   TIMSK2 = (0<<OCIE2B) | (0<<OCIE2A) | (0<<TOIE2); /* disable output compare match A interrupt */ 
// TIMSK2 &= ~(1<<OCIE2A); /* disable output compare match A interrupt */ 
   wdt_reset();
  }
 #else
   // Startup time is too long with 1MHz Clock!!!!
   // restart delay ist too long, use normal delay of 5ms
   wait5ms();		// wait5ms includes wdt_reset
   pause -= 1;
 #endif
  }
}
