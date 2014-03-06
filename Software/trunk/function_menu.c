// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// selection of different functions

#define MAX_FUNC 5
#ifdef WITH_MENU
void function_menu() {
  uint8_t ii;
  uint8_t ll;
  uint8_t func_number;

  func_number = 0xff;
  for (ll=0;ll<((MAX_FUNC+1)*20);ll++) {
     func_number++;
     if (func_number > MAX_FUNC) func_number = 0;
     message_key_released(SELECTION_str);
     lcd_line2();
     if (func_number == 2) {
 #ifdef WITH_VEXT
        lcd_MEM_string(VOLTAGE_str);
 #else
        continue;
 #endif
     }
     if (func_number == MAX_FUNC) {
 #ifdef POWER_OFF
        lcd_MEM_string(OFF_str);
 #else
        continue;
 #endif
     }
     if (func_number == 0) lcd_MEM2_string(TESTER_str);
     if (func_number == 1) lcd_MEM2_string(FREQ_str);
     if (func_number == 3) lcd_MEM2_string(F_GEN_str);
     if (func_number == 4) lcd_MEM2_string(PWM_10bit_str);
     ii = wait_for_key_ms(SHORT_WAIT_TIME);
     if (ii >= 30) {
        if (func_number == 0) break;		// return to TransistorTester
        if (func_number == 1) GetFrequency();
        if (func_number == 2) show_vext();
        if (func_number == 3) {
           make_frequency();		// make some sample frequencies
        }
        if (func_number == 4) {
           do_10bit_PWM();		// generate 10bit PWM
        }
        if (func_number == MAX_FUNC) {
           ON_PORT &= ~(1<<ON_PIN);              //switch off power
           wait_for_key_ms(0); //never ending loop 
        }
        // don't increase function number for easier selection the same function
        func_number--;	
     } /* end if (ii >= 30) */
  } /* end for ll */
  return;
 } // end function_menu()

/* *************************************************** */
/* show_vext() read one or two input voltages from     */
/* ADC input channel(s) TPext or (TPext and TPex2).    */
/* For both inputs a 10:1 voltage divider is required. */
/* *************************************************** */
void show_vext() {
 #ifdef WITH_VEXT
 
  uint8_t key_pressed;
  uint8_t times;
  unsigned int Vext;
  // show the external voltage
  message_key_released(VOLTAGE_str);
  for (times=0;times<250;times++) {
#ifdef TPex2
     lcd_line1();		// 2 Vext measurements 
     lcd_clear_line();
     lcd_line1();
#else
     lcd_line2();		// only one measurement use line 2
     lcd_clear_line();
     lcd_line2();
#endif	/* TPex2 */
#ifdef WITH_UART
     uart_newline();          // start of new measurement
     uart_newline();          // start of new measurement
#endif
     lcd_MEM_string(Vext_str);          // Vext=
     Vext = W5msReadADC(TPext); // read external voltage 
//     ADC_DDR = TXD_MSK;               //activate Software-UART 
  #if EXT_NUMERATOR <= (0xffff/U_VCC)
     DisplayValue(Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3); // Display 3 Digits of this mV units
  #else
     DisplayValue((unsigned long)Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3);  // Display 3 Digits of this mV units
  #endif

#ifdef TPex2
     lcd_line2();
     lcd_clear_line();
     lcd_line2();
#ifdef WITH_UART
     uart_newline();          // start of new measurement
#endif
     lcd_MEM_string(Vext_str);          // Vext=
     Vext = W5msReadADC(TPext); // read external voltage 
  #if EXT_NUMERATOR <= (0xffff/U_VCC)
     DisplayValue(Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3); // Display 3 Digits of this mV units
  #else
     DisplayValue((unsigned long)Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3);  // Display 3 Digits of this mV units
  #endif
#endif	/* TPex2 */

     key_pressed = wait_for_key_ms(1000);
     if (key_pressed != 0) break;
  }  /* end for times */
 #endif  /* WITH_VEXT */
} /* end show_vext() */

/* *************************************************** */
/* make frequency starts a frequency generator at TP2 with 680 Ohm resistor */
/* some predefined frequencies can be switched with the key */
/* a long key press returns to the selection menu */
/* *************************************************** */
void make_frequency() {
#define MAX_FREQ_NR 19
  uint8_t times;
  uint8_t key_pressed;
  uint8_t freq_nr;

  message_key_released(F_GEN_str);	// display f-Generator and wait for key released
  // OC1B is connected with 680 Ohm resistor to TP2 (middle test pin) 
  TCCR1A = (0<<COM1B1) | (1<<COM1B0) | (0<<WGM11) | (0<<WGM10); // CTC mode, count to OCR1A
  TIMSK1 = 0;		// no interrupt used
  OCR1A = 1;		// highest frequency
  OCR1B	= 0;		// toggle OC1B at this count
  TIFR1 = (1<<OCF1A) | (1<<OCF1A) | (1<<TOV1);	// reset interrupt flags
  TCCR1C = 0;
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10); // set counter mode to 9 PWM
  R_PORT = 0;		// set all resistor port outputs to GND
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL3);		// set TP1 and TP3 to output
  ADC_PORT = TXD_VAL;
  ADC_DDR = (1<<TP1) | TXD_MSK;			//connect TP1 to GND
  DDRB  |= (1<<DDB2);	// set output enable
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
  key_pressed = 1;
  freq_nr = MAX_FREQ_NR;
  for (times=0; times<240; times++) {
//     if(key_pressed >= 80) break;
//     if(key_pressed >= 30) {
//       if (freq_nr >= 2) {
//         freq_nr -= 2;		// two back and one foreward
//       } else {
//         freq_nr += (MAX_FREQ_NR - 2);
//       }
//     }
     if(key_pressed >= 50) break;	// more than 0.5 seconds
     if (key_pressed != 0) {
       times = 0;	// reset counter
       freq_nr++;
       if (freq_nr > MAX_FREQ_NR) freq_nr = 0;
       lcd_line2();
       lcd_clear_line();	// clear line 2 for next frequency
       lcd_line2();
       if (freq_nr == 0) {
         // 2 MHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #define DIVIDER  ((F_TIM1+2000000) / (2*2000000UL))
          TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
       }
       // 1333.333kHz
       if (freq_nr == 1) {
         // 1 MHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+1000000) / (2*1000000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
       }
       // 800kHz, 666.666kHz
       if (freq_nr == 2) {
          // 500 kHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+500000) / (2*500000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
       }
       // 444.444kHz, 400kHz, 362.636kHz, 333.333kHz, 307.692kHz 285.714kHz
       if (freq_nr == 3) {
         // 250 kHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+250000) / (2*250000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
       }
       // 235.294kHz, 222.222kHz, 210.526kHz, 200kHz, 190.476kHz, 181.818kHz, 173.913kHz, 166.666kHz
       // 160kHz, 153.846kHz, 148.148kHz, 142.857kHz, 137.931kHz, 133.333kHz, 129.032kHz, 125kHz,
       // (33) 121.212kHz, 117.647kHz, 114.285kHz, 111.111kHz, 108.108kHz, 105.263kHz , 102.564kHz 
       if (freq_nr == 4) {
         // 153.6 kHz  (Baud rate clock for 9600 baud)
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+153600) / (2*153600UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
       }
       if (freq_nr == 5) {
          // 100 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+100000) / (2*100000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
       }
       if (freq_nr == 6) {
          // 50 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+50000) / (2*50000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 5000UL) + (DIVIDER / 2)) / DIVIDER,-4,'H',7);
       }
       if (freq_nr == 7) {
          // 25 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+25000) / (2*25000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 5000UL) + (DIVIDER / 2)) / DIVIDER,-4,'H',7);
       }
       if (freq_nr == 8) {
          // 10 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+10000) / (2*10000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 5000UL) + (DIVIDER / 2)) / DIVIDER,-4,'H',7);
       }
       if (freq_nr == 9) {
          // 5 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+5000) / (2*5000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
       if (freq_nr == 10) {
          // 2500 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+2500) / (2*2500UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
       if (freq_nr == 11) {
          // 1000 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+1000) / (2*1000UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
       if (freq_nr == 12) {
          // 443 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+443) / (2*443UL))
          OCR1A = (DIVIDER - 1);
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
       if (freq_nr == 13) {
          // 442 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+442) / (2*442UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
       if (freq_nr == 14) {
          // 440 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+440) / (2*440UL))
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
       if (freq_nr == 15) {
          // 250 Hz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+250) / (2*250UL))
          TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
       }
// please use clock divider to build frequencies lower than 250 Hz (DIVIDER=64000 with 16MHz clock)
       if (freq_nr == 16) {
          // 100 Hz
 #undef F_TIM1
 #define F_TIM1 (F_CPU/64)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+100) / (2*100UL))
          TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
       }
       if (freq_nr == 17) {
          // 50 Hz
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+50) / (2*50UL))
//          TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
       }
       if (freq_nr == 18) {
          // 10 Hz
 #undef F_TIM1
 #define F_TIM1 (F_CPU/64)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+10) / (2*10UL))
          TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
       }
       if (freq_nr == 19) {
          // 1 Hz
 #undef F_TIM1
 #define F_TIM1 (F_CPU/256)
 #undef DIVIDER
 #define DIVIDER (F_TIM1 / (2*1UL))
          TCCR1B = (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10); // divide clock by 256
          OCR1A = DIVIDER - 1;
          DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
       }
       lcd_data('z');		// append the z to get Hz unit
     } /* end if key_pressed != 0 */
     key_pressed = wait_for_key_ms(1000);
  } /* end for times */
  TCCR1B = 0;		// stop counter
  ADC_DDR =  TXD_MSK;	// disconnect TP1 
  R_DDR = 0;		// switch resistor ports to Input
  DDRB  &= ~(1<<DDB2);	// disable output 

} /* end make frequency */

/* *************************************************** */
/* do_10bit PWM                                        */
/* a short key press increase the duty cycle with 1%   */
/* a longer key press incrrase with 10%                */
/* a very long key press returns to menue              */
/* *************************************************** */
void do_10bit_PWM() {
  uint8_t key_pressed;
  uint8_t times;		// time limit
  uint8_t percent;		// requestet duty-cycle in %
  unsigned int pwm_flip;	// value for counter to flip the state
  message_key_released(PWM_10bit_str);	// display PWM-Generator and wait for key released
  // OC1B is connected with 680 Ohm resistor to TP2 (middle test pin) 
  TCCR1A = (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (1<<WGM10); // fast PWM mode, count to 10 bit
  TIMSK1 = 0;		// no interrupt used
  OCR1A = 1;		// highest frequency
  OCR1B	= 0xff;		// toggle OC1B at this count
  TIFR1 = (1<<OCF1A) | (1<<OCF1A) | (1<<TOV1);	// reset interrupt flags
  TCCR1C = 0;

  R_PORT = 0;		// set all resistor port outputs to GND
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL3);		// set TP1 and TP3 to output
  ADC_PORT = TXD_VAL;
  ADC_DDR = (1<<TP1) | TXD_MSK;			//connect TP1 to GND
  DDRB  |= (1<<DDB2);	// set output enable
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
  key_pressed = 1;
  percent = 9;
  for (times=0; times<240; times++) {
     if(key_pressed >= 80) break;	// more than 0.8 seconds
     if (key_pressed > 35) percent += 9; // will be increased to 10
     if (key_pressed != 0) {
        times = 0;	// Reset the time limit to zero, user is present
        percent++;	// use next percent value, if long key press use 10% more
        if (percent >= 100) {
           percent -= 100;		//reset to 0 percent
        }
        pwm_flip = (((unsigned long)0x3ff * percent) + 50) / 100;
        OCR1B = pwm_flip;		// new percentage
        lcd_line2();		// only one value use line 2
        lcd_clear_line();
        lcd_line2();
        DisplayValue((((unsigned long)pwm_flip * 1000) + 0x1ff) / 0x3ff,-1,'%',5);
        if (key_pressed > 35) {
           wait_about300ms();	// wait some time to release the button
        }
     } /* end if key_pressed != 0 */
     key_pressed = wait_for_key_ms(1000);
  } /* end for times */

  ADC_DDR =  TXD_MSK;	// disconnect TP1 
  TCCR1B = 0;		// stop counter
  R_DDR = 0;		// switch resistor ports to Input
  DDRB  &= ~(1<<DDB2);	// disable output 
} /* end do_10bit_PWM */
#endif  /* WITH_MENU */
