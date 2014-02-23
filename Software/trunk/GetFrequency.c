// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// measure frequency at external pin T0 (PD4)

#ifdef WITH_MENU
void GetFrequency() {
  unsigned char taste;			// set if key is pressed during measurement
  uint8_t ii;
  uint8_t mm;

  lcd_clear();
  lcd_MEM2_string(FREQ_str);			// Frequency: in line 1
  while (!(RST_PIN_REG & (1<<RST_PIN))) { wdt_reset();};
  taste = 0;
  for (mm=0;mm<120;mm++) {
     //set up Counter 0
     // Counter 0 is used to count the external signal connected to T0 (PD4)
     DDRD &= ~(1<<PD4);			// switch PD4 to input
     wait1ms();				// let capacitor time to load to 2.4V input
     TCCR0A = 0; 			// normal operation, no output
     TCNT0 = 0;				// set counter to zero
     ext_freq.dw = 0;			// set external frequency to zero
     TIFR0 = (1<<TOV0);			// clear OV interrupt
     TIMSK0 = (1<<TOIE0);		// enable OV interrupt
     sei();				// set interrupt enable
     TCCR0B = (1<<CS02) | (1<<CS01);	// start the counter with external input T0

     // set up counter 1 to measure one second
     TCCR1A = 0;			// normal operation
     OCR1A = (F_CPU / 256UL) - 1;	// set to 1 second
     TCNT1 = 0;				// set counter to zero
     TIFR1 = (1<<OCF1B) | (1<<OCF1A);	// clear Output compare match
     TIMSK1 = (1<<OCIE1A);		// enable the Compare A match interrupt
     TCCR1B = (1<<CS12);		// divide CPU clock by 256
     GTCCR  |= (1<<PSRSYNC);		// reset clock precounter
//     GTCCR  = 0;		// free run clock precounter
     TCNT1 = 0;				// set counter to zero again
//     TCCR0B = (1<<CS02) | (1<<CS01) | (1<<CS00); // start the counter0 with external input T0
     // both counter are running now, wait for counter 1 reach OCR1A
     for (ii=0;ii<50;ii++) {
        wait20ms();
        wdt_reset();
        if (!(RST_PIN_REG & (1<<RST_PIN))) taste = 1;	// user request stop of operation
        if (TCCR0B == 0) break;		// timer is stopped by interrupt
     }
     // one second is counted
     TCCR0B = 0;		// stop timer 0
     ext_freq.b[0] = TCNT0;	// add lower 8 bit to get total counts
     lcd_clear();
     lcd_data('f');
     lcd_data('=');
     DisplayValue(ext_freq.dw,0,'H',5);
     lcd_data('z');
     if (TCCR1B != 0) {
       // Exact 1000ms period is only with end of period from timer1 interrupt.
       // When stopped with for loop, the time is too long because wait call does not
       // respect CPU time used for interrupts. Do not show Hz with for loop stop. 
       lcd_data('?');
     }
     TCCR1B = 0;		// stop timer 1
     TIMSK1 = 0;		// disable all timer 1 interrupts
     if ((ext_freq.dw < 10050) && (ext_freq.dw > 9)) {
        // also measure period
        if (ext_freq.dw < 100) {
           // 10 Hz is 100ms period, 25 periodes are 2.5 s
#if F_CPU == 8000000
           pinchange_max = 50;		// 25 periodes
#else
           pinchange_max = 25;		// 25 half periodes for 16 MHz
#endif
        } else {
           // 100 Hz is 10ms period, 125 periodes is 1.25 s
#if F_CPU == 8000000
           pinchange_max = 250;		// 125 periodes
#else
           pinchange_max = 125;		// 125 half periodes for 16 MHz
#endif
        }
        DDRD &= ~(1<<PD4);		// switch PD4 back to input
        wait1ms();			// let capacitor time to load to 2.4V input
        TCNT0 = 0;			// set counter to zero
        ext_freq.dw = 0;		// reset counter to zero
        TIFR0 = (1<<TOV0);		// clear OV interrupt
        TIMSK0 = (1<<TOIE0);		// enable OV interrupt
        // counter 0 ist started with first pin change interrupt
        pinchange_count = 0;
        PCIFR  = (1<<PCIF2);		// clear Pin Change Status
        PCICR  = (1<<PCIE2);		// enable pin change interrupt
        sei();
        PCMSK2 = (1<<PCINT20);		// monitor PD4 PCINT20 pin change
        for (ii=0;ii<250;ii++) {
           wait40ms();
           wdt_reset();
           if (!(RST_PIN_REG & (1<<RST_PIN))) taste = 1;	// user request stop of operation
           if ((PCMSK2 & (1<<PCINT20)) == 0) break;		// monitoring is disabled by interrupt
        }
        TCCR0B = 0;		// stop counter
        PCMSK2 = 0;		// stop monitor PD4 PCINT20 pin change
        ext_freq.b[0] = TCNT0;		// add lower 8 bit to get total counts
        // resolution is ns for 125 periodes of 8 MHz
        if (pinchange_max < 100) {
           // bring to ns resolution
           ext_freq.dw *= 5;
        }
        lcd_line2();
        lcd_clear_line();
        wait50ms();		// let LCD flicker to 
        lcd_line2();
        lcd_data('T');
        lcd_data('=');
        DisplayValue(ext_freq.dw,-9,'s',5);
        if (ii == 250) {
           lcd_data('?');
        } else {
           ext_period = ext_freq.dw / 250;	// period in 250ns resolution
           ext_freq.dw = 4000000000UL / ext_period; // frequency in mHz
           lcd_line1();
           lcd_data('f');
           lcd_data('=');
           DisplayValue(ext_freq.dw,-3,'H',5);  // display with mHz resolution
           lcd_data('z');
        }
     }  /* end if 10 < ext_freq < 10050 */
     TIMSK0 = 0;		// disable all timer 0 interrupts
     if (wait_for_key_ms(SHORT_WAIT_TIME/2) != 0) break;
     if (taste != 0) break;
  }  /* end for mm  */
 
  return;
 } // end GetFrequency()

// timer 0 count up to 0xff, then OV occur
ISR(TIMER0_OVF_vect, ISR_BLOCK) {
  ext_freq.dw += 256;
}

// interrupt after 1 second
ISR(TIMER1_COMPA_vect, ISR_BLOCK) {
  TCCR0B = 0;			// stop counter 0
  TCCR1B = 0;			// stop counter 1
}
// Pin change Interrupt
ISR(PCINT2_vect, ISR_BLOCK) {
  if (pinchange_count == 0) {
     // start the counter 0
     TCCR0B = (1<<CS00);	// start the counter with full CPU clock
  }
  if (pinchange_count >= pinchange_max) {
     // stop the counter 0, when maximum value has reached.
     TCCR0B = 0;		// stop counter
     PCMSK2 &= ~(1<<PCINT20);	// disable monitoring of PD4 PCINT20 pin change
//     PCICR &= ~(1<<PCIE2);	// disable the interrupt
  }
  pinchange_count++;
}
#endif
