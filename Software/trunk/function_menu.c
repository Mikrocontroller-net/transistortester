// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

//=================================================================
// selection of different functions

#if PROCESSOR_TYP == 644
 // the ATmega644 has additional menu functions HFREQ, H_CRYSTALs and L_CRYSTAL
 #define MODE_TRANS 0		/* normal TransistorTester function */
 #define MODE_FREQ 1		/* frequency measurement without 16:1 divider */
 #define MODE_HFREQ 2		/* frequency measurement with the 16:1 divider */
 #define MODE_FGEN 3		/* frequency generator function */
 #define MODE_PWM 4		/* Pulse Width variation function */
 #define MODE_ESR 5		/* ESR measurement in circuit */
 #define MODE_RESIS 6		/* ResistorCheck at TP1:TP3 */
 #define MODE_CAP13 7		/* Capacitor check at TP1:TP3 */
 #define MODE_ROTARY 8		/* Test Rotary Switch */
 #define MODE_BIG_CAP_CORR 9	/* Correction for big caps */
 #define MODE_H_CRYSTAL 10	/* test of high frequency crystal */
 #define MODE_L_CRYSTAL 11	/* test of low frequency crystal */
 #define NNN 11
 #ifdef WITH_SELFTEST
  #define MODE_SELFTEST 12	/* full selftest function with calibration */
  #undef NNN
  #define NNN 12
 #endif
 #ifdef WITH_VEXT
  #define MODE_VEXT (NNN+1)	/* external voltage measurement and zener voltage */
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || defined(LCD_DOGM))
   #define MODE_CONTRAST (NNN+2)	/* select contrast */
   #define MODE_SHOW (NNN+3)	/* show data function */
  #else
   #ifdef LCD_CHANGE_COLOR
    #define MODE_SELECT_FG (NNN+2)
    #define MODE_SELECT_BG (NNN+3)
    #define MODE_SHOW (NNN+4)
   #else
    #define MODE_SHOW (NNN+2)	/* show data function */
   #endif
  #endif
 #else
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || defined(LCD_DOGM))
   #define MODE_CONTRAST (NNN+1)	/* select contrast */
   #define MODE_SHOW (NNN+2)	/* show data function */
  #else
   #ifdef LCD_CHANGE_COLOR
    #define MODE_SELECT_FG (NNN+1)
    #define MODE_SELECT_BG (NNN+2)
    #define MODE_SHOW (NNN+3)
   #else
    #define MODE_SHOW (NNN+1)	/* show data function */
   #endif
  #endif
  #define MODE_VEXT 66
 #endif		/* end #ifdef WITH_VEXT */
#else
 /* no PROCESSOR_TYP == 644 , 328 */
 #define MODE_TRANS 0		/* normal TransistorTester function */
 #define MODE_FREQ 1		/* frequency measurement */
 #define MODE_FGEN 2		/* frequency generator function */
 #define MODE_PWM 3		/* Pulse Width variation function */
 #define MODE_ESR 4		/* ESR measurement in circuit */
 #define MODE_RESIS 5		/* ResistorCheck at TP1:TP3 */
 #define MODE_CAP13 6		/* Capacitor check at TP1:TP3 */
 #define MODE_BIG_CAP_CORR 7	/* Correction for big caps */
 #define NNN 7
 #ifdef WITH_ROTARY_CHECK
  #define MODE_ROTARY 8		/* Test Rotary Switch */
  #undef NNN
  #define NNN 8
  #ifdef WITH_SELFTEST
   #define MODE_SELFTEST 9	/* full selftest function with calibration */
   #undef NNN
   #define NNN 9
  #endif
 #else
  #ifdef WITH_SELFTEST
   #define MODE_SELFTEST 8	/* full selftest function with calibration */
   #undef NNN
   #define NNN 8
  #endif
 #endif
 #ifdef WITH_VEXT
  #define MODE_VEXT (NNN+1)	/* external voltage measurement and zener voltage */
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   #define MODE_CONTRAST (NNN+2)	/* select contrast */
   #define MODE_SHOW (NNN+3)	/* show data function */
  #else
   #ifdef LCD_CHANGE_COLOR
    #define MODE_SELECT_FG (NNN+2)
    #define MODE_SELECT_BG (NNN+3)
    #define MODE_SHOW (NNN+4)
   #else
    #define MODE_SHOW (NNN+2)	/* show data function */
   #endif
  #endif
 #else
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   #define MODE_CONTRAST (NNN+1)	/* select contrast */
   #define MODE_SHOW (NNN+2)	/* show data function */
  #else
   #ifdef LCD_CHANGE_COLOR
    #define MODE_SELECT_FG (NNN+1)
    #define MODE_SELECT_BG (NNN+2)
    #define MODE_SHOW (NNN+3)
   #else
    #define MODE_SHOW (NNN+1)	/* show data function */
   #endif
  #endif
  #define MODE_VEXT 66
 #endif		/* end #ifdef WITH_VEXT */
 #define MODE_HFREQ 66
#endif

#define MIN_SELECT_TIME 50	/* 50x10ms must be hold down to select function without a rotary switch */
#ifdef POWER_OFF
 #define MODE_OFF MODE_SHOW+1	/* add the power off function */
 #define MODE_LAST MODE_OFF
#else
 #define MODE_LAST MODE_SHOW	/* without POWER_OFF, the SHOW function is the last */
 #define MODE_OFF 66
#endif

#if LCD_LINES > 7
 #define MENU_LINES 6
#else
 #define MENU_LINES (LCD_LINES-1)
#endif
#define MENU_MIDDLE ((MENU_LINES-1)/2)

#ifdef WITH_MENU

void do_menu(uint8_t func_number) {

//    lcd_MEM2_string(DoMenu_str);	// "do menu "
//    u2lcd(func_number);
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
    if (func_number == MODE_RESIS) {
       show_Resis13();		// measure resistor at TP1 and TP3
    }
    if (func_number == MODE_CAP13) {
   lcd_clear();
       show_Cap13();		// measure capacitor at TP1 and TP3
    }
#ifdef WITH_ROTARY_CHECK
    if (func_number == MODE_ROTARY) {
       CheckRotaryEncoder();		// check rotary encoder
    }
#endif
    if (func_number == MODE_BIG_CAP_CORR) {
       set_big_cap_corr();
    }
#ifdef WITH_SELFTEST
    if (func_number == MODE_SELFTEST) AutoCheck(0x11);	// Full selftest with calibration
#endif
    if (func_number == MODE_VEXT) show_vext();
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
    if (func_number == MODE_CONTRAST) set_contrast();
#endif
#ifdef LCD_CHANGE_COLOR
    if (func_number == MODE_SELECT_FG) select_color(0);		// select foreground color
    if (func_number == MODE_SELECT_BG) select_color(1);		// select background color
#endif
    if (func_number == MODE_SHOW) {
       ShowData();			// Show Calibration Data
    }
    if (func_number == MODE_OFF) {
       switch_tester_off();		//switch off power
    }
}

/* ****************************************************************** */
/* ****************************************************************** */
#ifdef WITH_HARDWARE_SERIAL_XX

uint8_t last_func_number = 0;

uint8_t read_menu() {
  uint8_t c;
  while ( !(UCSR0A & (1<<RXC0)) );	// Wait for received data
  c = UDR0;
//  u2lcd(c);
  if (c == '\n') { // '\n' alone == redo last operation
    return last_func_number;
  }
  last_func_number = c - '0'; // '0' -> 255 , '1' -> 0, ...
  // must be followed by \n
  do {
    while ( !(UCSR0A & (1<<RXC0)) );	// Wait for received data
    c = UDR0;
//  u2lcd(c);
  } while (c != '\n');
  return last_func_number;
}
void display_menu() {
  uint8_t ii;
  rawSerial = 1;
  lcd_data('\n');
  lcd_data('-');
  lcd_data('-');
  lcd_data('-');
  for (ii = 0; ii < MODE_LAST; ii++) {
      if (ii == last_func_number) lcd_data('>');
      else			lcd_space();
	  message2line(ii);
  }
  lcd_data('\n');
  lcd_data('-');
  lcd_data('-');
  lcd_data('-');
  rawSerial = 0;
}
uint8_t function_menu() {
  uint8_t func_number;

  display_menu();
  func_number = read_menu();
  if (func_number == 0) {
	  return 0;
  } else {
	  do_menu(func_number);
          lcd_MEM2_string(Done_str);	// "Done.\n"
	  lcd_refresh();
	  return 1;
  }
}

#else // !WITH_HARDWARE_SERIAL

uint8_t function_menu() {
  uint8_t ii;
  uint8_t func_number;

#ifdef PAGE_MODE
  uint8_t page_nr;
  uint8_t p_nr;
  uint8_t ff;
  page_nr = MODE_LAST;
 #ifdef WITH_ROTARY_SWITCH
  rotary.count = 0;
 #endif
#endif

  func_number = 0;
  message_key_released(SELECTION_str);
 #ifdef POWER_OFF
  uint8_t ll;
  for (ll=0;ll<((MODE_LAST+1)*10);ll++) 
 #else
  while (1)		/* without end, if no power off specified */
 #endif
  {
     if (func_number > MODE_LAST) func_number -= (MODE_LAST + 1);
#if (LCD_LINES > 3)
  uint8_t mm;
#ifdef WITH_HARDWARE_SERIAL
     uart_newline();          // start of new measurement
     for (mm=0;mm<LCD_LINE_LENGTH;mm++) uart_putc('=');
     message_key_released(SELECTION_str);	//write Line 1 with Selection:
#endif
 #ifdef PAGE_MODE
     ff = 0;
     mm = 0;
     do {
	p_nr = page_nr + mm;
        if (p_nr > MODE_LAST) p_nr -= (MODE_LAST + 1);
        if (func_number == p_nr) ff = 1;	// number is found
     } while (++mm < MENU_LINES);

     if (ff == 0) {
        // func_number is not in page list
  #ifdef WITH_ROTARY_SWITCH
        if (rotary.count >= 0) {
           page_nr = (func_number + MODE_LAST -1);  // page_nr = func_number - 2
        } else {
           page_nr = func_number;	// for backward, set page_nr to func_number
        }
       if (page_nr > MODE_LAST) page_nr -= (MODE_LAST + 1);
  #else
        page_nr = func_number;
  #endif
     }
     mm= 0;
     do {
        p_nr = page_nr + mm;
        if (p_nr > MODE_LAST) p_nr -= (MODE_LAST + 1);
        lcd_set_cursor((mm+1)*PAGES_PER_LINE,0);
        if (func_number == p_nr) {
           lcd_data('>');
        } else {
           lcd_space();				// put a blank to 1. row of line 2
        }
        message2line(p_nr);			// show  page function
     } while (++mm < MENU_LINES);

 #else	/* no PAGE_MODE */
     uint8_t f_nr;
     mm = 0;
     do {
        lcd_set_cursor((mm+1)*PAGES_PER_LINE,0);
        if (mm == MENU_MIDDLE) {
           lcd_data('>');				// put a '>' marker to row 1 of line 4
        } else {
           lcd_space();				// put a blank to 1. row of line 2
        } 
        f_nr = func_number + MODE_LAST + 1 - MENU_MIDDLE + mm;
        if (f_nr > MODE_LAST) f_nr -= (MODE_LAST +1);
        message2line(f_nr);	// show function for this line
     } while (++mm < MENU_LINES);

 #endif         /* PAGE_MODE */
#else	/* not LCD_LINES > 3 */
     lcd_line2();
     message2line(func_number);
#endif /* (LCD_LINES > 3) */
#ifdef POWER_OFF
     ii = wait_for_key_ms(SHORT_WAIT_TIME);	// wait about 5 seconds
     if (ii > 0) ll = 0;			// reset timer, operator present
     if (DC_Pwr_mode == 1) ll = 0;
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
        if (func_number == MODE_TRANS) return 0;		// return to TransistorTester
        do_menu(func_number);
        // don't increase function number for easier selection the same function
        ii = 0;			// function was executed before, do not increase func_number
#ifdef WITH_ROTARY_SWITCH
        rotary.incre = 0;	// reset all rotary information
        rotary.count = 0;
#endif
        message_key_released(SELECTION_str);	//write Line 1 with Selection:
     } /* end if (ii >= MIN_SELECT_TIME) */
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre >= FAST_ROTATION) break; // to much rotation
 #ifdef POWER_OFF
     if (rotary.count != 0) ll = 0; 	// someone is working, reset timer
 #endif
     if (rotary.count >= 0) {
        func_number += rotary.count;	// function number is increased by rotary steps
     } else {
        func_number += (MODE_LAST + 1 + rotary.count);	// function is decreased by rotary steps
     }
#endif
     if (ii > 0) func_number++;	// increase the function number with key press
  } /* end for ll */

  return 0;
 } // end function_menu()

#endif // !WITH_HARDWARE_SERIAL

/* ****************************************************************** */
/* message2line writes the message corresponding to the number to LCD */
/* ****************************************************************** */
void message2line(uint8_t number) { 
     if (number > MODE_LAST) number -= (MODE_LAST + 1);
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
     if (number == MODE_RESIS) lcd_MEM_string(RESIS_13_str);
     if (number == MODE_CAP13) lcd_MEM_string(CAP_13_str);
 #ifdef WITH_ROTARY_CHECK
     if (number == MODE_ROTARY) lcd_MEM2_string(RotaryEncoder_str);
 #endif
     if (number == MODE_BIG_CAP_CORR) lcd_MEM2_string(SetCapCorr_str);
 #ifdef WITH_SELFTEST
     if (number == MODE_SELFTEST) lcd_MEM2_string(FULLCHECK_str);
 #endif
 #ifdef WITH_VEXT
     if (number == MODE_VEXT) lcd_MEM_string(VOLTAGE_str); 
 #endif
 #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 1306) || defined(LCD_DOGM))

     if (number == MODE_CONTRAST) lcd_MEM_string(CONTRAST_str); 
 #endif
 #ifdef LCD_CHANGE_COLOR
     if (number == MODE_SELECT_FG) lcd_MEM_string(FrontColor_str); 
     if (number == MODE_SELECT_BG) lcd_MEM_string(BackColor_str); 
 #endif
     if (number == MODE_SHOW) {
        lcd_MEM2_string(SHOW_str);
     }
     if (number == MODE_OFF) {
        lcd_MEM2_string(OFF_str);
     }
 lcd_clear_line();
 uart_newline();                   // MAURO
} /* end message2line() */

/* ****************************************************************** */
/* show_C_ESR measures the capacity and ESR of a capacitor connected to TP1 and TP3 */
/* ****************************************************************** */
void show_C_ESR() {
  uint8_t key_pressed;
  message_key_released(C_ESR_str);
#ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<250;) 
#else
  while (1)		/* wait endless without the POWER_OFF option */
#endif
  {
        PartFound = PART_NONE;
        ReadBigCap(TP3,TP1);
        if (PartFound == PART_CAPACITOR) {
#if LCD_LINES > 2
           lcd_line2(); 	// set to line2 
#else
           lcd_line1(); 	// set to line1 
#endif
           lcd_data('C');
           lcd_equal();		// lcd_data('=');
           DisplayValue(cap.cval_max,cap.cpre_max,'F',3);
           lcd_clear_line();	// clear to end of line 1
           cap.esr = GetESR(cap.cb,cap.ca);
#if LCD_LINES > 2
	   lcd_line3();		// use line 3 
#else
           lcd_line2();		// use line 2 
#endif
           lcd_MEM_string(&ESR_str[1]);
           if (cap.esr < 65530) {
              DisplayValue16(cap.esr,-2,LCD_CHAR_OMEGA,2);
           } else {
              lcd_data('?');		// too big
           }
           lcd_clear_line();		// clear to end of line
        } else { // no cap found
#if LCD_LINES > 2
           lcd_clear_line2(); 	// clear C value 
           lcd_line3();
	   lcd_clear_line();	// clear old ESR value
#else
           lcd_line1();	//  
           lcd_MEM2_string(C_ESR_str);
           lcd_clear_line();
           lcd_clear_line2(); 	// clear old ESR value 
#endif
        }
#if defined(POWER_OFF) && defined(BAT_CHECK)
     Bat_update(times);
#endif
     key_pressed = wait_for_key_ms(1000);
#ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 3)) break;
#else
     if (key_pressed != 0) break;
#endif
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
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
  for (times=0;times<240;) 
#else
  while (1)			/* wait endless without option POWER_OFF */
#endif
  {
#ifdef TPex2
     lcd_clear_line1(); 	// 2 Vext measurements 
#else
     lcd_clear_line2();		// only one measurement use line 2
#endif	/* TPex2 */
     uart_newline();          // start of new measurement
     uart_newline();          // start of new measurement
     lcd_MEM_string(Vext_str);          // Vext=
     Vext = W5msReadADC(TPext); // read external voltage 
//     ADC_DDR = TXD_MSK;               //activate Software-UART 
  #if EXT_NUMERATOR <= (0xffff/U_VCC)
     Display_mV(Vext*EXT_NUMERATOR/EXT_DENOMINATOR,3); // Display 3 Digits of this mV units
  #else
     Display_mV((unsigned long)Vext*EXT_NUMERATOR/EXT_DENOMINATOR,3);  // Display 3 Digits of this mV units
  #endif

#ifdef TPex2
     lcd_clear_line2();
     uart_newline();          // start of new measurement
     lcd_MEM_string(Vext_str);          // Vext=
     Vext = W5msReadADC(TPex2); // read external voltage 2
  #if EXT_NUMERATOR <= (0xffff/U_VCC)
     Display_mV(Vext*EXT_NUMERATOR/EXT_DENOMINATOR,3); // Display 3 Digits of this mV units
  #else
     Display_mV((unsigned long)Vext*EXT_NUMERATOR/EXT_DENOMINATOR,3);  // Display 3 Digits of this mV units
  #endif
#endif	/* TPex2 */
#if defined(POWER_OFF) && defined(BAT_CHECK)
     Bat_update(times);
#endif

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
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
#endif
  }  /* end for times */
 #endif  /* WITH_VEXT */
} /* end show_vext() */

#include "make_frequency.c"

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
  TCCR1A = (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (1<<WGM10); // fast PWM mode, mode 7: count to 10 bit
  TIMSK1 = 0;		// no interrupt used
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
#if PROCESSOR_TYP == 1280
  DDRB  |= (1<<DDB6);	// set output enable for OC1B
#else
  DDRB  |= (1<<DDB2);	// set output enable
#endif
#ifdef PWM_SERVO
  TCCR1B = (1<<WGM13) | (1<<WGM12) | SERVO_START; // mode 15, clock divide by 8 or 64
  OCR1A = PWM_MAX_COUNT - 1;	// clock tics for 20 ms
#else
  OCR1A = 1;		// highest frequency
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // mode 7, no clock divide
#endif
  key_pressed = 0;
  old_perc = 0;
  percent = (SERVO_MAX + SERVO_MIN) / 2;	// set to middle
#ifdef POWER_OFF
  uint8_t times;		// time limit
  for (times=0; times<240; ) 
#else
  while (1)			/* wait endless without option POWER_OFF */
#endif
  {
     if (percent != old_perc) {
        // new duty cycle is requested
        if (percent >= SERVO_MAX) {
	   percent -= (SERVO_MAX - SERVO_MIN);		// reset near to mininum value
        }
#ifdef PWM_SERVO
        pwm_flip = (((unsigned long)PWM_MAX_COUNT * percent) + 500) / 1000;
#else
        pwm_flip = (((unsigned long)PWM_MAX_COUNT * percent) + 50) / 100;
#endif
        OCR1B = pwm_flip;		// new percentage
        lcd_line2();		// goto line 2
#ifdef PWM_SERVO
        DisplayValue(((unsigned long)pwm_flip * SERVO_DIV)/MHZ_CPU ,-6,'s',3);
	lcd_space();
	lcd_data('/');
	lcd_space();
	DisplayValue16(((unsigned long)PWM_MAX_COUNT * SERVO_DIV)/MHZ_CPU, -6,'s',3);
#else
        DisplayValue16((((unsigned long)pwm_flip * 1000) + (PWM_MAX_COUNT/2)) / PWM_MAX_COUNT,-1,'%',5);
#endif
        lcd_clear_line();
        old_perc = percent;	// update the old duty cycle
        if (key_pressed > 40) {
           wait_about300ms();	// wait some time to release the button
        }
     } /* end if percent != old_perc */
     key_pressed = wait_for_key_ms(1600);
     if(key_pressed > 130) break;	// more than 1.3 seconds
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends voltage measurement
     if (rotary.count >= 0) {
        percent += rotary.count;		// increase the duty cycle by rotary.count
     } else {
        percent += ((SERVO_MAX-SERVO_MIN) + rotary.count);	// decrease the duty cycle by rotary.count
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
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
#endif
  } /* end for times */

  ADC_DDR =  TXD_MSK;	// disconnect TP1 
  TCCR1B = 0;		// stop counter
  TCCR1A = 0;		// stop counter
  R_DDR = 0;		// switch resistor ports to Input
#if PROCESSOR_TYP == 1280
  DDRB  &= ~(1<<DDB6);	// disable output 
#else
  DDRB  &= ~(1<<DDB2);	// disable output 
#endif
} /* end do_10bit_PWM */

 #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
/* *************************************************** */
/* set the contrast value of the ST7565 display */
/* *************************************************** */
 #if (LCD_ST_TYPE == 1306)
  #define MAX_CONTRAST 0xff
 #elif (LCD_ST_TYPE == 8812)
  #define MAX_CONTRAST 0x7f
 #elif (LCD_ST_TYPE == 8814)
  #define MAX_CONTRAST 0xff
 #else
  /* for DOG-M the upper bit of contrast value is BOOSTER 0x40 */
  #define MAX_CONTRAST 0x7f
 #endif
void set_contrast(void) {
uint8_t key_pressed;
uint8_t contrast;
  // set the contrast value
  message_key_released(CONTRAST_str);	// display Contrast and wait for key released
  contrast = eeprom_read_byte(&EE_Volume_Value);
  #ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<240;)
  #else
  while (1)                     /* wait endless without option POWER_OFF */
  #endif
  {
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
     lcd_command(CMD_SET_VOLUME_FIRST);		// 0x81 set  volume command
     lcd_command(contrast);			// value from 1 to 63 (0x3f) */
  #elif (LCD_ST_TYPE == 8812)	/* PCF8812 controller */
     lcd_command(CMD_SET_EXTENDED_INSTRUCTION);		// set extended instruction mode
     lcd_command(ECMD_SET_CONTRAST | (contrast & 0x7f));	// set the contrast value
     lcd_command(CMD_SET_NORMAL_INSTRUCTION);		// return to normal instruction mode
  #elif (LCD_ST_TYPE == 8814)	/* PCF8814 controller */
   lcd_command(CMD_SET_VOP_UPPER | ((contrast >> 5) & 0x07));      // set upper Vop
   lcd_command(CMD_SET_VOP_LOWER | (contrast & 0x1f));    // set lower Vop
  #else		/* DOGM display */
     lcd_command(CMD_SetIFOptions | MODE_8BIT | 0x09);	// 2-line / IS=1
     lcd_command(CMD1_PowerControl | ((contrast>>4)&0x07));	// booster on,off / set contrast C5:C4 
     lcd_command(CMD1_SetContrast | (contrast&0x0f));	// set contrast C3:0 
     lcd_command(CMD_SetIFOptions | MODE_8BIT | 0x08);	// 2-line / IS=0
  #endif
     lcd_line2();
     DisplayValue16(contrast,0,' ',4);
     lcd_clear_line();		// clear to end of line
     key_pressed = wait_for_key_ms(1600);
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 0)) times = 0;	// reset counter, operator is active
 #else
     if (key_pressed != 0)  times = 0;	// reset counter, operator is active
 #endif
#endif
     if(key_pressed >= 130) break;	// more than 1.3 seconds
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends setting of contrast
     if (rotary.count >= 0) {
        contrast += rotary.count;		// increase the contrast by rotary.count
     } else {
        contrast += (MAX_CONTRAST + 1 + rotary.count);	// decrease the contrast by rotary.count
     }
#endif
     if (key_pressed > 0) {
        if (key_pressed > 40) {
           contrast++; // longer key press select higher contrast value
        } else {
           contrast += MAX_CONTRAST;	// decrease the contrast 
        }
     }
     contrast &= MAX_CONTRAST;
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
#endif
  } /* end for times */

  eeprom_write_byte((uint8_t *)(&EE_Volume_Value), (int8_t)contrast);	// save contrast value
}
 #endif /* LCD_ST_TYPE == 7565 */

 #ifdef LCD_CHANGE_COLOR
/* *********************************************************** */
/* set the color values for the ST7735/ILI9163/ILI9341 display */
/* *********************************************************** */
void select_color(uint8_t xcol) {
/* xcol = 0  for foreground */
/* xcol != 0 for backgreound */
uint8_t key_pressed;
uint8_t color[3];	// color red, green, blue
uint8_t tmp1, tmp2;
uint8_t c_num;
uint8_t cc;
uint8_t max_value;
  // set the contrast value
  if (xcol == 0) {
    message_key_released(FrontColor_str);	// display FrontColor and wait for key released
    tmp1 = eeprom_read_byte(&EE_FG_COLOR1);	// lower bits of foreground color
    tmp2 = eeprom_read_byte(&EE_FG_COLOR2);	// upper bits of foreground color
  } else {
    message_key_released(BackColor_str);  	// display BackColor and wait for key released
    tmp1 = eeprom_read_byte(&EE_BG_COLOR1);
    tmp2 = eeprom_read_byte(&EE_BG_COLOR2);
  }
  color[0] = tmp2 >> 3;
  color[2] = tmp1 & 0x1f;
  color[1] = ((tmp2 & 0x07) << 3) | (tmp1 >> 5);
  c_num = 0;
  #ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<240;)
  #else
  while (1)                     /* wait endless without option POWER_OFF */
  #endif
  {
     for (cc=0; cc<3; cc++) {
        lcd_set_cursor((1+cc)*PAGES_PER_LINE,0);
        if (c_num == cc) lcd_data('>');
        else             lcd_space();
  #if LCD_CHANGE_COLOR == 1
        if (cc == 0) lcd_data('R');
  #else
        if (cc == 0) lcd_data('B');
  #endif
        if (cc == 1) lcd_data('G');
  #if LCD_CHANGE_COLOR == 1
        if (cc == 2) lcd_data('B');
  #else
        if (cc == 2) lcd_data('R');
  #endif
        lcd_data('=');
        DisplayValue16(color[cc],0,' ',4);
        lcd_clear_line();		// clear to end of line
     }
     key_pressed = wait_for_key_ms(1600);
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 0)) times = 0;	// reset counter, operator is active
 #else
     if (key_pressed != 0)  times = 0;	// reset counter, operator is active
 #endif
#endif
     if(key_pressed >= 130) break;	// more than 1.3 seconds
     max_value = 31;
     if (c_num == 1) max_value = 63;
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends setting of contrast
     if (rotary.count >= 0) {
        color[c_num] += rotary.count;		// increase the contrast by rotary.count
     } else {
        color[c_num] += (max_value + 1 + rotary.count);	// decrease the contrast by rotary.count
     }
     color[c_num] &= max_value;
     if (xcol == 0) {
        // foreground color
        lcd_fg_color.b[1] = (color[0] << 3) | (color[1] >> 3);
        lcd_fg_color.b[0] = ((color[1] & 7) << 5) | (color[2] & 0x1f);
     } else {
        lcd_bg_color.b[1] = (color[0] << 3) | (color[1] >> 3);
        lcd_bg_color.b[0] = ((color[1] & 7) << 5) | (color[2] & 0x1f);
     }
#endif
     if (key_pressed > 0) {
        if (key_pressed > 40) {
           c_num += 2;	// decrease the color number 
        } else {
           c_num++;		// increase the color number
        }
     }
     if (c_num > 2) c_num -= 3;
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
#endif
  } /* end for times */

//  eeprom_write_byte((uint8_t *)(&EE_Volume_Value), (int8_t)contrast);	// save contrast value
  if (xcol == 0) {
     eeprom_write_byte((uint8_t *)(&EE_FG_COLOR1), (int8_t)lcd_fg_color.b[0]);
     eeprom_write_byte((uint8_t *)(&EE_FG_COLOR2), (int8_t)lcd_fg_color.b[1]);
  } else {
     eeprom_write_byte((uint8_t *)(&EE_BG_COLOR1), (int8_t)lcd_bg_color.b[0]);
     eeprom_write_byte((uint8_t *)(&EE_BG_COLOR2), (int8_t)lcd_bg_color.b[1]);
  }
}
 #endif
/* *************************************************** */
/* set the correction value for big capacitor measurement */
/* *************************************************** */
#define MIN_KORR (-20)
#define MAX_KORR 80
void set_big_cap_corr(void) {
uint8_t key_pressed;
int8_t korr;
  // set the contrast value
  message_key_released(SetCapCorr_str);	// display Capacity correction and wait for key released
  korr = eeprom_read_byte((uint8_t *)&big_cap_corr);
  #ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<240;)
  #else
  while (1)                     /* wait endless without option POWER_OFF */
  #endif
  {
     lcd_line2();
     if (korr < 0) {
       lcd_data('-');
       DisplayValue16(-korr,-1,'%',3);
     } else {
       DisplayValue16(korr,-1,'%',3);
     }
     lcd_clear_line();		// clear to end of line
     key_pressed = wait_for_key_ms(1600);
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 0)) times = 0;	// reset counter, operator is active
 #else
     if (key_pressed != 0)  times = 0;	// reset counter, operator is active
 #endif
#endif
     if(key_pressed >= 130) break;	// more than 1.3 seconds
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends setting of korr
     korr += rotary.count;		// increase or decrease the korr by rotary.count
#endif
     if (key_pressed > 0) {
        if (key_pressed > 40) {
           korr++; // longer key press select higher korr value
        } else {
           korr--;	// decrease the korr 
        }
     }
     if (korr > MAX_KORR) korr -= (MAX_KORR - MIN_KORR + 1);
     if (korr < MIN_KORR) korr += (MAX_KORR - MIN_KORR + 1);
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
#endif
  } /* end for times */

  eeprom_write_byte((uint8_t *)(&big_cap_corr), (int8_t)korr);	// save korr value
}
#endif  /* WITH_MENU */
