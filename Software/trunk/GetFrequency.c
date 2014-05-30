// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// measure frequency at external pin T0 (PD4 or PB0)
#define FMAX_PERIOD 25050
#define FMAX_INPUT 2004000
#define FREQ_DIV 16

#if PROCESSOR_TYP == 644
 /* Pin change interrupt is PCINT8 (PB0) */
 #define PCI_ENABLE_BIT PCIE1
 #define PCI_CLEAR_BIT PCIF1
 #define PCINTx_vect PCINT1_vect
#else
 /* Pin change interrupt is PCINT20 (PD4) */
 #define PCI_ENABLE_BIT PCIE2
 #define PCI_CLEAR_BIT PCIF2
 #define PCINTx_vect PCINT2_vect
#endif

#ifdef WITH_MENU
void GetFrequency(uint8_t range) {
  unsigned char taste;			// set if key is pressed during measurement
  unsigned long freq_count;		// the counted pulses in 1 second
  unsigned long long ext_period;
  unsigned long freq_from_per;
  uint8_t ii;
  uint8_t mm;
  /* range has set the lowest bit to use the 16:1 frequency divider permanently. */
  /* The upper bits of range specifies the input selection. */
  /* 0 = external input, 2 = channel 2, 4 = HF Quartz, 6 = LF Quartz */
  
 #if PROCESSOR_TYP == 644
  FDIV_DDR |= (1<<FDIV_PIN);		//switch to output
  if ((range & 0x01) == 0) {
     FDIV_PORT &= ~(1<<FDIV_PIN);	// switch off the 16:1 divider
  } else {
     FDIV_PORT |= (1<<FDIV_PIN);	// use frequency divider for next measurement
  }
  FINP_DDR |= (1<<FINP_P0) | (1<<FINP_P1);	// switch both pins to output

 FINP_PORT &= ~(1<<FINP_P0);		// clear lower bit of input selection
 FINP_PORT &= ~(1<<FINP_P1);		// clear higher bit of input selection
 if (range == 0) {  
    message_key_released(FREQ_str);	// Frequency: in line 1
 } else if (range == 1) { 
    message_key_released(HFREQ_str);	// High Frequency: in line 1
 } else if (range < 4) { /* 2+3 */
    FINP_PORT |= (1<<FINP_P0);		// set lower bit of input selection
    FINP_PORT &= ~(1<<FINP_P1);		// clear higher bit of input selection
 } else if (range < 6) { /* 4+5 */
    FINP_PORT &= ~(1<<FINP_P0);		// clear lower bit of input selection
    FINP_PORT |= (1<<FINP_P1);		// set higher bit of input selection
    message_key_released(H_CRYSTAL_str);	// HF Quarz: in line 1
 } else {  /* 6+7 */
    FINP_PORT |= (1<<FINP_P0);		// set lower bit of input selection
    FINP_PORT |= (1<<FINP_P1);		// set higher bit of input selection
    message_key_released(L_CRYSTAL_str);	// LF Quarz: in line 1
 }

 #else
  message_key_released(FREQ_str);	// Frequency: in line 1
 #endif
  taste = 0;				// reset flag for key pressed
  for (mm=0;mm<240;mm++) {
     // *************************************************************************
     // *********** straight frequency measurement by counting 1 second *********
     // *************************************************************************
     //set up Counter 0
     // Counter 0 is used to count the external signal connected to T0 (PD4 or PB0)
     FREQINP_DDR &= ~(1<<FREQINP_PIN);	// switch frequency pin to input
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
     freq_count = ext_freq.dw;	// save the frequency counter
     lcd_clear();		// clear total display
     lcd_data('f');
     lcd_data('=');
 #if PROCESSOR_TYP == 644
     if ((FDIV_PORT&(1<<FDIV_PIN)) == 0) {
        DisplayValue(ext_freq.dw,0,'H',7);
     } else {
        // frequency divider is activ
        DisplayValue(ext_freq.dw*FREQ_DIV,0,'H',7);
     }
 #else
     DisplayValue(ext_freq.dw,0,'H',7);
 #endif
     lcd_data('z');
 #if PROCESSOR_TYP == 644
     lcd_space();
     if ((FDIV_PORT&(1<<FDIV_PIN)) != 0) {
        lcd_data('/');		// Frequency divider is activ
     } else {
        lcd_space();		// Frequency divider is not activ
     }
 #endif
     FREQINP_DDR &= ~(1<<FREQINP_PIN);	// switch frequency pin to input
     if (TCCR1B != 0) {
       // Exact 1000ms period is only with "end of period" from timer1 interrupt.
       // When stopped with the for loop, the time is too long because wait call does not
       // respect CPU time used for interrupts and loop itself.
       // For this case show ? behind the Hz. 
       lcd_data('?');
     }
     TCCR1B = 0;		// stop timer 1
     TIMSK1 = 0;		// disable all timer 1 interrupts
     if ((ext_freq.dw < FMAX_PERIOD) && (ext_freq.dw > 0)) {
     // *************************************************************************
     // ******** Period measurement by counting some periods ******************** 
     // *************************************************************************
        pinchange_max = ((10 * (unsigned long)ext_freq.dw) + MHZ_CPU) / MHZ_CPU;	// about 10000000 clock tics
        pinchange_max += pinchange_max;	// * 2 for up and down change
        FREQINP_DDR &= ~(1<<FREQINP_PIN);	// switch frequency pin to input
        wait1ms();			// let capacitor time to load to 2.4V input
        TCNT0 = 0;			// set counter to zero
        ext_freq.dw = 0;		// reset counter to zero
        TIFR0 = (1<<TOV0);		// clear OV interrupt
        TIMSK0 = (1<<TOIE0);		// enable OV interrupt
        // counter 0 ist started with first pin change interrupt
        pinchange_count = 0;
        PCIFR  = (1<<PCI_CLEAR_BIT);		// clear Pin Change Status
        PCICR  |= (1<<PCI_ENABLE_BIT);		// enable pin change interrupt
        sei();
        PCMSK_FREQ |= (1<<PCINT_FREQ);	// monitor PD4 PCINT20 or PB0 PCINT8 pin change
        for (ii=0;ii<250;ii++) {
           wait20ms();
           wdt_reset();
           if (!(RST_PIN_REG & (1<<RST_PIN))) taste = 1;	// user request stop of operation
           if ((PCMSK_FREQ & (1<<PCINT_FREQ)) == 0) break;		// monitoring is disabled by interrupt
        } /* end for ii */
        TCCR0B = 0;		// stop counter
        PCMSK_FREQ &= ~(1<<PCINT_FREQ);		// stop monitor PD4 PCINT20 or PB0 PCINT8 pin change
        PCICR &= ~(1<<PCI_ENABLE_BIT);	// disable the interrupt
        ext_freq.b[0] = TCNT0;		// add lower 8 bit to get total counts
//        lcd_line2();
//        lcd_clear_line();
//        wait50ms();		// let LCD flicker to 
        lcd_line2();
        lcd_data('T');
        lcd_data('=');
        ext_period = ((unsigned long long)ext_freq.dw * (125*200)) / pinchange_max;
 #if PROCESSOR_TYP == 644
        if ((FDIV_PORT&(1<<FDIV_PIN)) != 0) {
           // frequency divider is activ, period is measured too long
           ext_period = ext_period / FREQ_DIV;
        }
 #endif
        if (pinchange_max > 127) {
           DisplayValue(ext_period,-11,'s',7);	// show period converted to 0.01ns units
        } else {
           //prevent overflow of 32-Bit
           DisplayValue((unsigned long)(ext_period/100),-9,'s',7);	// show period converted to 1ns units
        }
        if (ii == 250) {
           lcd_data('?');		// wait loop has regular finished
        } else {
           if (ext_period > 249500) {
              lcd_line1();
              lcd_data('f');
              lcd_data('=');
              if (ext_period > 1000000000) {
                 // frequency in 0.000001Hz (1e11*1e6)/(0.01ns count)
                 freq_from_per = (unsigned long long)(100000000000000000) / ext_period;
                 DisplayValue(freq_from_per,-6,'H',7);  // display with  0.000001 Hz resolution
              } else {
                 // prevent unsigned long overflow, scale to 0.0001 Hz
                 // frequency in 0.0001Hz (1e11*1e4)/(0.01ns count)
                 freq_from_per = (unsigned long long)(1000000000000000) / ext_period;
                 DisplayValue(freq_from_per,-4,'H',7);  // display with  0.0001 Hz resolution
              }
              lcd_data('z');
              FREQINP_DDR &= ~(1<<FREQINP_PIN);	// switch frequency pin to input
           }
        }
     }  /* end if 1 < ext_freq < FMAX_PERIOD */
 #if PROCESSOR_TYP == 644
     if ((FDIV_PORT & (1<<FDIV_PIN)) == 0) {
        // frequency divider is not activ
        if ( ((freq_count >= FMAX_PERIOD) && (freq_count < ((unsigned long)FMAX_PERIOD*FREQ_DIV))) ||
            (freq_count > FMAX_INPUT) ){
           FDIV_PORT |= (1<<FDIV_PIN);			// use frequency divider for next measurement
        }
     } else {
        // frequency divider is activ
        if ((freq_count < (FMAX_PERIOD/FREQ_DIV)) && ((range & 0x01) == 0)) {
           FDIV_PORT &= ~(1<<FDIV_PIN);			// switch off the 16:1 divider
        }
     }
     wait_about2s();
 #endif
//     taste += wait_for_key_ms(SHORT_WAIT_TIME/2);
     taste += wait_for_key_ms(2000);
     if (taste != 0) break;
     TIMSK0 = 0;		// disable all timer 0 interrupts
     if (taste != 0) break;
  }  /* end for mm  */
 
  return;
 } // end GetFrequency()

/* ************************************************************ */
/* timer 0 Overflow interrupt                                   */
/* timer 0 count up to 0xff, then OV occur. update upper part   */
/* to build the total counts within one second or               */
/* to build the total counts within the specified pin changes.  */
/* ************************************************************ */
ISR(TIMER0_OVF_vect, ISR_BLOCK) {
  sei();		// set interrupt enable
  ext_freq.dw += 256;	// add 256 clock tics to the total time
}

/* ************************************************************ */
/* Timer 1 Compare B interrupts with count 1 to start counter 0 */
/* This is defined as start of the measurement second.          */
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


ISR(PCINTx_vect, ISR_BLOCK)
{
  if (pinchange_count == 0) {
     // start the counter 0
     TCCR0B = (1<<CS00);	// start the counter with full CPU clock
  }
  if (pinchange_count >= pinchange_max) {
     // stop the counter 0, when maximum value has reached.
     TCCR0B = 0;		// stop counter
     PCMSK_FREQ &= ~(1<<PCINT_FREQ);	// disable monitoring of PD4 PCINT20 or PB0 PCINT8 pin change
//     PCICR &= ~(1<<PCI_ENABLE_BIT);	// disable the interrupt
  }
  pinchange_count++;
}
#endif
