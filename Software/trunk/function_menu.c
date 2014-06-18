// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// selection of different functions

#if PROCESSOR_TYP == 644
 #define MODE_TRANS 0
 #define MODE_FREQ 1
 #define MODE_HFREQ 2
 #define MODE_FGEN 3
 #define MODE_PWM 4
 #define MODE_ESR 5
 #define MODE_ROTARY 6
 #define MODE_H_CRYSTAL 7
 #define MODE_L_CRYSTAL 8
 #define MODE_SELFTEST 9
 #ifdef WITH_VEXT
  #define MODE_VEXT 10
  #define MODE_OFF 11
 #else
  #define MODE_OFF 10
  #define MODE_VEXT 66
 #endif
#else
 #define MODE_TRANS 0
 #define MODE_FREQ 1
 #define MODE_FGEN 2
 #define MODE_PWM 3
 #define MODE_ESR 4
 #define MODE_ROTARY 5
 #define MODE_SELFTEST 6
 #ifdef WITH_VEXT
  #define MODE_VEXT 7
  #define MODE_OFF 8
 #else
  #define MODE_OFF 7
  #define MODE_VEXT 66
 #endif
 #define MODE_HFREQ 66
#endif
#define MIN_SELECT_TIME 50	/* 50x10ms must be hold down to select function without a rotary switch */

#define FAST_ROTATION 10		/* number of rotation steps to identify a fast rotation of rotary switch */

#ifdef WITH_MENU
/* ****************************************************************** */
/* ****************************************************************** */
void function_menu() {
  uint8_t ii;
  uint8_t func_number;

  func_number = 0;
 #ifdef POWER_OFF
  uint8_t ll;
  for (ll=0;ll<((MODE_OFF+1)*10);ll++) 
 #else
  while (1)		/* without end, if no power off specified */
 #endif
  {
     if (func_number > MODE_OFF) func_number -= (MODE_OFF + 1);
     message_key_released(SELECTION_str);
     lcd_line2();
     lcd_clear_line();				// clear line 2
     lcd_line2();				// reset cursor to begin of line 2
#ifdef FOUR_LINE_LCD
     lcd_space();				// put a blank to 1. row of line 2
     message2line(func_number + MODE_OFF);	// show lower (previous) function
     lcd_line3();
     lcd_clear_line();				// clear line 3
     lcd_line3();				// reset cursor to begin of line 3
     lcd_data('>');				// put a '>' marker to row 1 of line 3
     message2line(func_number);			// show selectable function
     lcd_line4();
     lcd_clear_line();				// clear line 4
     lcd_line4();				// reset cursor to begin of line 4
     lcd_space();				// put a blank to 1. row of line 4
     message2line(func_number + 1);		// show higher (next) function
#else
     message2line(func_number);
#endif
 #ifdef POWER_OFF
     ii = wait_for_key_ms(SHORT_WAIT_TIME);	// wait about 5 seconds
     if (ii > 0) ll = 0;			// reset timer, operator present
 #else
     ii = wait_for_key_ms(0);			// wait endless
 #endif
#ifdef WITH_ROTARY_SWITCH
     if ((ii >= MIN_SELECT_TIME) || ((rotary_switch_present != 0) && (ii > 0)))
#else
     if (ii >= MIN_SELECT_TIME)
#endif
     {
        // selection only with key-press
        if (func_number == MODE_TRANS) break;		// return to TransistorTester
        if (func_number == MODE_FREQ) GetFrequency(0);
 #if PROCESSOR_TYP == 644
        if (func_number == MODE_HFREQ) GetFrequency(1);	// measure high frequency with 16:1 divider
        if (func_number == MODE_H_CRYSTAL) GetFrequency(5); // HF crystal input + 16:1 divider
        if (func_number == MODE_L_CRYSTAL) GetFrequency(6); // LF crystal input, 1:1 divider
 #endif
        if (func_number == MODE_FGEN) {
           make_frequency();		// make some sample frequencies
        }
        if (func_number == MODE_PWM) {
           do_10bit_PWM();		// generate 10bit PWM
        }
        if (func_number == MODE_ESR) {
           show_C_ESR();		// measure capacity and ESR at TP1 and TP3
        }
        if (func_number == MODE_ROTARY) {
           CheckRotaryEncoder();		// check rotary encoder
        }
        if (func_number == MODE_SELFTEST) AutoCheck(0x11);	// Full selftest with calibration
        if (func_number == MODE_VEXT) show_vext();
        if (func_number == MODE_OFF) {
           ON_PORT &= ~(1<<ON_PIN);              //switch off power
           wait_for_key_ms(0); //never ending loop 
        }
        // don't increase function number for easier selection the same function
        ii = 0;			// function was executed before, do not increase func_number
#ifdef WITH_ROTARY_SWITCH
        rotary.incre = 0;	// reset all rotary information
        rotary.count = 0;
#endif
     } /* end if (ii >= MIN_SELECT_TIME) */
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre >= FAST_ROTATION) break; // to much rotation
 #ifdef POWER_OFF
     if (rotary.count != 0) ll = 0; 	// someone is working, reset timer
 #endif
     if (rotary.count >= 0) {
        func_number += rotary.count;	// function number is increased by rotary steps
     } else {
        func_number += (MODE_OFF + 1 + rotary.count);	// function is decreased by rotary steps
     }
#endif
     if (ii > 0) func_number++;	// increase the function number with key press
  } /* end for ll */
  return;
 } // end function_menu()

/* ****************************************************************** */
/* message2line writes the message corresponding to the number to LCD */
/* ****************************************************************** */
void message2line(uint8_t number) {
     if (number > MODE_OFF) number -= (MODE_OFF + 1);
     if (number == MODE_TRANS) lcd_MEM2_string(TESTER_str);
     if (number == MODE_FREQ) lcd_MEM2_string(FREQ_str);
 #if PROCESSOR_TYP == 644
     if (number == MODE_HFREQ) lcd_MEM2_string(HFREQ_str);
     if (number == MODE_H_CRYSTAL) lcd_MEM2_string(H_CRYSTAL_str);
     if (number == MODE_L_CRYSTAL) lcd_MEM2_string(L_CRYSTAL_str);
 #endif
     if (number == MODE_FGEN) lcd_MEM2_string(F_GEN_str);
     if (number == MODE_PWM) lcd_MEM2_string(PWM_10bit_str);
     if (number == MODE_ESR) lcd_MEM2_string(C_ESR_str);
     if (number == MODE_ROTARY) lcd_MEM2_string(RotaryEncoder_str);
     if (number == MODE_SELFTEST) lcd_MEM2_string(FULLCHECK_str);
 #ifdef WITH_VEXT
     if (number == MODE_VEXT) lcd_MEM_string(VOLTAGE_str); 
 #endif
     if (number == MODE_OFF) {
        lcd_MEM_string(OFF_str);
     }
}

/* ****************************************************************** */
/* show_C_ESR measures the capacity and ESR of a capacitor connected to TP1 and TP3 */
/* ****************************************************************** */
void show_C_ESR() {
  uint8_t key_pressed;
  message_key_released(C_ESR_str);
#ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<250;times++) 
#else
  while (1)		/* wait endless without the POWER_OFF option */
#endif
  {
        PartFound = PART_NONE;
        ReadBigCap(TP3,TP1);
        if (PartFound == PART_CAPACITOR) {
           lcd_line1();		// clear old capacity value 
           lcd_clear_line();
           lcd_line1();
           lcd_data('C');
           lcd_data('=');
           DisplayValue(cap.cval_max,cap.cpre_max,'F',3);
           cap.esr = GetESR(cap.cb,cap.ca);
           lcd_line2();		// clear old ESR value 
           lcd_clear_line();
           lcd_line2();
           lcd_MEM_string(&ESR_str[1]);
           if (cap.esr < 65530) {
              DisplayValue(cap.esr,-2,LCD_CHAR_OMEGA,2);
           } else {
              lcd_data('?');		// too big
           }
        } else {
           lcd_clear();
           lcd_MEM2_string(C_ESR_str);
        }
     key_pressed = wait_for_key_ms(1000);
#ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 3)) break;
#else
     if (key_pressed != 0) break;
#endif
  }  /* end for times */
} /* end show_C_ESR() */

/* *************************************************** */
/* show_vext() read one or two input voltages from     */
/* ADC input channel(s) TPext or (TPext and TPex2).    */
/* For both inputs a 10:1 voltage divider is required. */
/* *************************************************** */
void show_vext() {
 #ifdef WITH_VEXT
 
  uint8_t key_pressed;
  uint8_t key_long_pressed;
  unsigned int Vext;
  // show the external voltage
  message_key_released(VOLTAGE_str);
  key_long_pressed = 0;
#ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<240;times++) 
#else
  while (1)			/* wait endless without option POWER_OFF */
#endif
  {
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
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed > 0) || (rotary.incre > 0)) times = 0;	// reset the loop counter, operator is active
     if (rotary.incre > 5) break;		// fast rotation ends voltage measurement
 #else
     if (key_pressed > 0) times = 0;		//reset the loop counter, operator is active
 #endif
#endif
     if (key_pressed > ((1000/10)-6)) {
        key_long_pressed++;	// count the long key press
     }
     if (key_pressed == 0) key_long_pressed = 0; //reset the key long pressed counter
     if (key_long_pressed > 4) break;	// five seconds end the loop
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
  uint8_t key_pressed;
  uint8_t freq_nr;
  uint8_t old_freq;

  message_key_released(F_GEN_str);	// display f-Generator and wait for key released
  // OC1B is connected with 680 Ohm resistor to TP2 (middle test pin) 
  TCCR1A = (0<<COM1B1) | (1<<COM1B0) | (0<<WGM11) | (0<<WGM10); // CTC mode, count to OCR1A
  TIMSK1 = 0;		// no interrupt used
  OCR1A = 1;		// highest frequency
  OCR1B	= 0;		// toggle OC1B at this count
  TIFR1 = (1<<OCF1A) | (1<<OCF1A) | (1<<TOV1);	// reset interrupt flags
  TCCR1C = 0;
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10); // set counter mode 
  R_PORT = 0;		// set all resistor port outputs to GND
#if PROCESSOR_TYP == 644
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL2) | (1<<PIN_RL3);		// set TP1, DDD4(TP2) and TP3 to output
#else
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL3);		// set TP1 and TP3 to output
#endif
  ADC_PORT = TXD_VAL;
  ADC_DDR = (1<<TP1) | TXD_MSK;			//connect TP1 to GND
  DDRB  |= (1<<DDB2);	// set output enable
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
  old_freq = 0;
  freq_nr = MAX_FREQ_NR - 1;	// start with 1 MHz
#ifdef POWER_OFF
  uint8_t new_points;		// one point for every 30 seconds wait time
  uint8_t shown_points;		// one point for every 30 seconds wait time
  uint8_t times;		// total wait time
  shown_points = 0;
  for (times=0; times<240; times++) 
#else
  while (1)			/* wait endless without option POWER_OFF */
#endif
  {
#define KEYPRESS_LENGTH_10ms 0
#ifdef POWER_OFF
     new_points = (times+10) / 30;
     if (new_points != shown_points) {
        // count of points has changed, build LCD line1 new
        lcd_line1();
        lcd_clear_line();	// clear line 1 
        lcd_line1();
        lcd_MEM2_string(F_GEN_str);	// display f-Generator
        shown_points = new_points;
        for (new_points=0; new_points<shown_points ;new_points++) {
           lcd_data('.');		// show elapsed time, one point is 30 seconds
        }
     }
#undef KEYPRESS_LENGTH_10ms 
#define KEYPRESS_LENGTH_10ms 20		/* change frequency only with >200ms key press */
#endif
     if (old_freq != freq_nr) {
       // new frequency is selected
       if (freq_nr > MAX_FREQ_NR) freq_nr -= (MAX_FREQ_NR + 1);
       old_freq = freq_nr;	// update the last active frequency number
#ifdef FOUR_LINE_LCD
       lcd_line2();
       lcd_clear_line();	// clear line 2 for previous frequency
       lcd_line2();
       lcd_space();		// add a space to row 1 of line2
       switch_frequency(freq_nr + MAX_FREQ_NR);
       lcd_line4();
       lcd_clear_line();	// clear line 4 for next frequency
       lcd_line4();
       lcd_space();		// add a space to row 1 of line4
       switch_frequency(freq_nr + 1);
       lcd_line3();
       lcd_clear_line();	// clear line 3 for new frequency
       lcd_line3();
       lcd_data('>');
       switch_frequency(freq_nr);
#else
       lcd_line2();
       lcd_clear_line();	// clear line 2 for next frequency
       lcd_line2();
       switch_frequency(freq_nr);
#endif
     } /* end if (old_freq != freq_nr) */
     key_pressed = wait_for_key_ms(1000);
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 0)) times = 0;	// reset counter, operator is active
 #else
     if (key_pressed != 0)  times = 0;	// reset counter, operator is active
 #endif
#endif
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends voltage measurement
     if (rotary.count >= 0) {
        freq_nr += rotary.count;		// increase the frequency number by rotary.count
     } else {
        freq_nr += (MAX_FREQ_NR + 1 + rotary.count);	// decrease the frequency by rotary.count
     }
#endif
     if (key_pressed > KEYPRESS_LENGTH_10ms) freq_nr++; // longer key press select next frequency
     if(key_pressed >= 80) break;	// more than 0.8 seconds
  } /* end for times */
  TCCR1B = 0;		// stop counter
  TCCR1A = 0;		// stop counter
  ADC_DDR =  TXD_MSK;	// disconnect TP1 
  R_DDR = 0;		// switch resistor ports to Input
  DDRB  &= ~(1<<DDB2);	// disable output 

} /* end make frequency */

/* *************************************************** */
/* switch to a new frequency number and write actual frequency to LCD */
/* *************************************************** */
void switch_frequency(uint8_t freq_num) {
  if (freq_num > MAX_FREQ_NR) freq_num -= (MAX_FREQ_NR + 1);
  if (freq_num == 19) {
         // 2 MHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #define DIVIDER  ((F_TIM1+2000000) / (2*2000000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
  }
       // 1333.333kHz
  if (freq_num == 18) {
         // 1 MHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+1000000) / (2*1000000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
  }
       // 800kHz, 666.666kHz
  if (freq_num == 17) {
          // 500 kHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+500000) / (2*500000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
  }
       // 444.444kHz, 400kHz, 362.636kHz, 333.333kHz, 307.692kHz 285.714kHz
  if (freq_num == 16) {
         // 250 kHz
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+250000) / (2*250000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
  }
       // 235.294kHz, 222.222kHz, 210.526kHz, 200kHz, 190.476kHz, 181.818kHz, 173.913kHz, 166.666kHz
       // 160kHz, 153.846kHz, 148.148kHz, 142.857kHz, 137.931kHz, 133.333kHz, 129.032kHz, 125kHz,
       // (33) 121.212kHz, 117.647kHz, 114.285kHz, 111.111kHz, 108.108kHz, 105.263kHz , 102.564kHz 
  if (freq_num == 15) {
         // 153.6 kHz  (Baud rate clock for 9600 baud)
 #undef F_TIM1
 #define F_TIM1 (F_CPU)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+153600) / (2*153600UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
  }
  if (freq_num == 14) {
          // 100 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+100000) / (2*100000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500UL) + (DIVIDER / 2)) / DIVIDER,-3,'H',7);
  }
  if (freq_num == 13) {
          // 50 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+50000) / (2*50000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 5000UL) + (DIVIDER / 2)) / DIVIDER,-4,'H',7);
  }
  if (freq_num == 12) {
          // 25 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+25000) / (2*25000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 5000UL) + (DIVIDER / 2)) / DIVIDER,-4,'H',7);
  }
  if (freq_num == 11) {
          // 10 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+10000) / (2*10000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 5000UL) + (DIVIDER / 2)) / DIVIDER,-4,'H',7);
  }
  if (freq_num == 10) {
          // 5 kHz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+5000) / (2*5000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
  }
  if (freq_num == 9) {
          // 2500 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+2500) / (2*2500UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
  }
  if (freq_num == 8) {
          // 1000 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+1000) / (2*1000UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
  }
  if (freq_num == 7) {
          // 443 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+443) / (2*443UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = (DIVIDER - 1);
     DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
  }
  if (freq_num == 6) {
          // 442 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+442) / (2*442UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
  }
  if (freq_num == 5) {
          // 440 Hz
 #undef DIVIDER
 #define DIVIDER  ((F_TIM1+440) / (2*440UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 50000UL) + (DIVIDER / 2)) / DIVIDER,-5,'H',7);
  }
  if (freq_num == 4) {
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
  if (freq_num == 3) {
          // 100 Hz
 #undef F_TIM1
 #define F_TIM1 (F_CPU/64)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+100) / (2*100UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
  }
  if (freq_num == 2) {
          // 50 Hz
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+50) / (2*50UL))
//          TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
  }
  if (freq_num == 1) {
          // 10 Hz
 #undef F_TIM1
 #define F_TIM1 (F_CPU/64)
 #undef DIVIDER
 #define DIVIDER ((F_TIM1+10) / (2*10UL))
     TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
     OCR1A = DIVIDER - 1;
     DisplayValue((((unsigned long long)F_TIM1 * 500000UL) + (DIVIDER / 2)) / DIVIDER,-6,'H',7);
  }
  if (freq_num == 0) {
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
}	/* end switch_frequency */

/* *************************************************** */
/* do_10bit PWM                                        */
/* a short key press increase the duty cycle with 1%   */
/* a longer key press incrrase with 10%                */
/* a very long key press returns to menue              */
/* *************************************************** */
void do_10bit_PWM() {
  uint8_t key_pressed;
  uint8_t percent;		// requestet duty-cycle in %
  uint8_t old_perc;		// old duty-cycle in %
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
#if PROCESSOR_TYP == 644
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL2) | (1<<PIN_RL3);		// set TP1, DDD4(TP2) and TP3 to output
#else
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL3);		// set TP1 and TP3 to output
#endif
  ADC_PORT = TXD_VAL;
  ADC_DDR = (1<<TP1) | TXD_MSK;			//connect TP1 to GND
  DDRB  |= (1<<DDB2);	// set output enable
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
  key_pressed = 0;
  old_perc = 0;
  percent = 10;
#ifdef POWER_OFF
  uint8_t times;		// time limit
  for (times=0; times<240; times++) 
#else
  while (1)			/* wait endless without option POWER_OFF */
#endif
  {
     if (percent != old_perc) {
        // new duty cycle is requested
        if (percent >= 100) {
           percent -= 100;		//reset to 0 percent or higher
        }
        pwm_flip = (((unsigned long)0x3ff * percent) + 50) / 100;
        OCR1B = pwm_flip;		// new percentage
        lcd_line2();		// set cursor to begin of line 2
        lcd_clear_line();	// clear line 2
        lcd_line2();		// set cursor to row 1 of line 2
        DisplayValue((((unsigned long)pwm_flip * 1000) + 0x1ff) / 0x3ff,-1,'%',5);
#if 0
        lcd_space();
        if (rotary.count >= 0) {	// actual count for debugging
           lcd_data('+');
           lcd_data('0'+rotary.count);
        } else {
           lcd_data('-');
           lcd_data('0'-rotary.count);
        }
        lcd_line3();
        uint8_t kk;
        kk = (rotary.ind + 1) & ROT_MSK;
        do {
           lcd_data('0'+rotary.state[kk]);	// debugging output of rotary state
           kk = (kk + 1) & ROT_MSK;
        } while (kk != rotary.ind);
#endif
        old_perc = percent;	// update the old duty cycle
        if (key_pressed > 40) {
           wait_about300ms();	// wait some time to release the button
        }
     } /* end if percent != old_perc */
     key_pressed = wait_for_key_ms(2000);
     if(key_pressed > 130) break;	// more than 1.3 seconds
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends voltage measurement
     if (rotary.count >= 0) {
        percent += rotary.count;		// increase the duty cycle by rotary.count
     } else {
        percent += (100 + rotary.count);	// decrease the duty cycle by rotary.count
     }
#endif
     if (key_pressed > 50) {
        percent += 10;		// duty cycle will be increased with 10
     } else {
        if (key_pressed > 0) percent += 1;	// duty cycle will be increased with 1
     }
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed > 0) || (rotary.incre > 0)) times = 0;	// reset the loop counter, operator is active
 #else
     if (key_pressed > 0) times = 0;		//reset the loop counter, operator is active
 #endif
#endif
  } /* end for times */

  ADC_DDR =  TXD_MSK;	// disconnect TP1 
  TCCR1B = 0;		// stop counter
  TCCR1A = 0;		// stop counter
  R_DDR = 0;		// switch resistor ports to Input
  DDRB  &= ~(1<<DDB2);	// disable output 
} /* end do_10bit_PWM */
#endif  /* WITH_MENU */
