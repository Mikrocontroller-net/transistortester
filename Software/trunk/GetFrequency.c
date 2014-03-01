// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// measure frequency at external pin T0 (PD4)

#ifdef WITH_MENU
void GetFrequency() {
  unsigned char taste;			// set if key is pressed during measurement
  unsigned long ext_period;
  unsigned long freq_from_per;
  uint8_t ii;
  uint8_t mm;

  message_key_released(FREQ_str);	// Frequency: in line 1
  taste = 0;				// reset flag for key pressed
  for (mm=0;mm<240;mm++) {
     //set up Counter 0
     // Counter 0 is used to count the external signal connected to T0 (PD4)
     DDRD &= ~(1<<PD4);			// switch PD4 to input
     wait1ms();				// let capacitor time to load to 2.4V input
     TCCR0A = 0; 			// normal operation, no output
     TCNT0 = 0;				// set counter to zero
     ext_freq.dw = 0;			// set external frequency to zero
     TIFR0 = (1<<TOV0);			// clear OV interrupt of timer 0
     TIMSK0 = (1<<TOIE0);		// enable OV interrupt of timer 0
     // start counter after starting second counter timer 1

     // set up counter 1 to measure one second
     TCCR1A = 0;			// normal operation
     OCR1A = (F_CPU / 256UL)+1;		// set to 1 second  (counter 0 is started with 1)
     OCR1B = 1;				// start counter 0 with first count
     TCNT1 = 0;				// set counter to zero
     GTCCR  |= (1<<PSRSYNC);		// reset clock precounter
     TIFR1 = (1<<OCF1B) | (1<<OCF1A);	// clear Output compare match
     TIMSK1 = (1<<OCIE1B) | (1<<OCIE1A);	// enable the Compare A match and Compare A match interrupt
     sei();				// set interrupt enable
     TCCR1B = (1<<CS12);		// divide CPU clock by 256, start counter
     // both counter are running now, wait for counter 1 reach OCR1A
     for (ii=0;ii<50;ii++) {
        wait20ms();			// first count of counter 1 (<32us) has started the counter 0
        wdt_reset();
        if (!(RST_PIN_REG & (1<<RST_PIN))) taste = 1;	// user request stop of operation
        if (TCCR0B == 0) break;		// timer is stopped by interrupt
     }
     // one second is counted
     TCCR0B = 0;		// stop timer 0, if not stopped by timer 1 compare interrupt
     ext_freq.b[0] = TCNT0;	// add lower 8 bit to get total counts
     lcd_clear();		// clear total display
     lcd_data('f');
     lcd_data('=');
     DisplayValue(ext_freq.dw,0,'H',7);
     lcd_data('z');
     DDRD &= ~(1<<PD4);			// switch PD4 to input
     if (TCCR1B != 0) {
       // Exact 1000ms period is only with "end of period" from timer1 interrupt.
       // When stopped with the for loop, the time is too long because wait call does not
       // respect CPU time used for interrupts and loop itself.
       // For this case show ? behind the Hz. 
       lcd_data('?');
     }
     TCCR1B = 0;		// stop timer 1
     TIMSK1 = 0;		// disable all timer 1 interrupts
     if ((ext_freq.dw < 10050) && (ext_freq.dw > 4)) {
        // also measure period
        if (ext_freq.dw < 100) {
           // 10 Hz is 100ms period, 25 periodes are 2.5 s
#if F_CPU == 8000000
           pinchange_max = 50;		// 25 periodes
#else
           pinchange_max = 25;		// 25 half periodes for 16 MHz
#endif
        } else if (ext_freq.dw < 1000) {
	   // 125 periods is 250 pin changes for 8 MHz, which give a period resolution of ns.
           // But for 16 MHz the ns resolution can be get with 125 pin changes, which are 62.5 periods.
	   // Therefore only 200 pinchanges (100 periods) are used. 
           // 100 Hz is 10ms period, 100 periodes is 1 s
#if F_CPU == 8000000
           pinchange_max = 250;		// 125 periodes
#else
           pinchange_max = 125;		// 125 half periodes for 16 MHz
#endif
        } else {
           // more than 1000 Hz
#if F_CPU == 8000000
           pinchange_max = 2500;		// 1250 periodes
#else
           pinchange_max = 1250;		// 625 half periodes for 16 MHz
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
//        lcd_line2();
//        lcd_clear_line();
//        wait50ms();		// let LCD flicker to 
        lcd_line2();
        lcd_data('T');
        lcd_data('=');
        // resolution is ns for 125 periodes of 8 MHz
        if (pinchange_max < 100) {
           //  100Hz = .01s , 25 periodes for 8MHz = 0.25s = 2000000 clock tics,
	   //         one period has 10000000 ns
           ext_period = ext_freq.dw * 50;		// counted clocks gives already 5ns resolution
        } else if (pinchange_max < 1000) {
           ext_period = ext_freq.dw * 10;	//has already  1ns resolution
        } else {
           ext_period = ext_freq.dw;		//has already  100ps resolution
        }
        DisplayValue(ext_period,-10,'s',7);	// show period converted to 1ns units
        if (ii == 250) {
           lcd_data('?');		// wait loop has regular finished
        } else {
           if (ext_period > 50000) {
              freq_from_per = (long long)(100000000000000) / ext_period; // frequency in 0.0001Hz (1e9*10000)/ns
              lcd_line1();
              lcd_data('f');
              lcd_data('=');
              DisplayValue(freq_from_per,-4,'H',7);  // display with  0.0001 Hz resolution
              lcd_data('z');
              DDRD &= ~(1<<PD4);			// switch PD4 to input
           }
        }
     taste += wait_for_key_ms(SHORT_WAIT_TIME/2);
     if (taste != 0) break;
     }  /* end if 10 < ext_freq < 10050 */
     TIMSK0 = 0;		// disable all timer 0 interrupts
     if (taste != 0) break;
  }  /* end for mm  */
 
  return;
 } // end GetFrequency()

/* ************************************************************ */
/* timer 0 Overflow interrupt                                   */
/* timer 1 count up to 0xff, then OV occur                      */
/* ************************************************************ */
ISR(TIMER0_OVF_vect, ISR_BLOCK) {
  sei();		// set interrupt enable
  ext_freq.dw += 256;	// add 256 clock tics to the total time
}

/* ************************************************************ */
/* Timer 1 Compare B interrupts with count 1 to start counter 0 */
/* ************************************************************ */
ISR(TIMER1_COMPB_vect, ISR_BLOCK) {
  TCCR0B = (1<<CS02) | (1<<CS01);	// now start the counter 0 with external input T0
}

/* ************************************************************ */
/* Timer 1 Compare A interrupt after 1 second to stop counter 0 */
/* ************************************************************ */
ISR(TIMER1_COMPA_vect, ISR_BLOCK) {
  // The TIMER1_COMPB_vect uses one push and one ldi more than this interrupt.
  // Therefore we stop timer 1 first and ajust to same time with wdt_reset();
  wdt_reset();			// for adjusting to same time as TIMER1_COMPB_vect
  TCCR1B = 0;			// stop counter 1
  TCCR0B = 0;			// stop counter 0
}

/* ************************************************************ */
/* Pin change Interrupt                                         */
/* this interrupt is used to measure the period of input signal */
/* the counter 0 is started with pin change interrupt and       */
/* stopped after pin_changed_max changes                        */
/* The pin_changed_max value should be equal to measure         */
/* full periods  (no half period)				*/
/* ************************************************************ */
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
