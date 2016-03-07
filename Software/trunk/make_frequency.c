/* *************************************************** */
/* make frequency starts a frequency generator at TP2 with 680 Ohm resistor */
/* some predefined frequencies can be switched with the key */
/* a long key press returns to the selection menu */
/* *************************************************** */
void make_frequency() {
#undef KEYPRESS_LENGTH_10ms 
#define KEYPRESS_LENGTH_10ms 10		/* change frequency only with >100ms key press */
  uint8_t key_pressed;
  uint8_t significant;		// number of signigficant bits to show the frequency (+16 for option)
  unsigned long wish_freq;	// frequency (Hz), which you want to create
  union p_combi{		/* half period of the requested frequency */
   unsigned long dw;
   uint16_t w[2];
  } hperiod;
  uint16_t divider;		// divider for the counter clock 1,8,64,256,1024
  unsigned long  f_incre;	// frequency increment 1, 10, 100, 1000, 10000, 100000
  uint8_t f_digit;		// digit for f_incre, 0-10 or 0-20
  uint8_t Max_Digit;		// 10 or 20 , the last digit is for 
  uint8_t right;		// logical expression 

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
#if PROCESSOR_TYP == 1280
  DDRB  |= (1<<DDB6);	// set output enable for OC1B
#else
  DDRB  |= (1<<DDB2);	// set output enable for OC1B
#endif
  TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
//  wish_freq = 1;		// start with 1 Hz
  f_incre = 1;		// increment with 1 Hz
  f_digit = 1;			// start value
  Max_Digit = 10;	// normally 10, only for 100kHz steps 20
  significant = 16+1;	// only one significant frequency digit (Hz resolution)
#ifdef POWER_OFF
  uint8_t new_points;		// one point for every 30 seconds wait time
  uint8_t shown_points;		// one point for every 30 seconds wait time
  uint8_t times;		// total wait time
  shown_points = 0;
  for (times=0; times<240;) 
#else
  while (1)			/* wait endless without option POWER_OFF */
#endif
  {
#ifdef POWER_OFF
     new_points = (times+10) / 30;
     if (new_points != shown_points) {
        // count of points has changed, build LCD line1 new
        lcd_line1();	// position to line 1 
        lcd_MEM2_string(F_GEN_str);	// display f-Generator
        shown_points = new_points;
        for (new_points=0; new_points<shown_points ;new_points++) {
           lcd_data('.');		// show elapsed time, one point is 60 seconds
        }
        lcd_clear_line();	// clear remainder of line1
     }
#endif
     // Generate frequency
     if (significant > (16+1)) {
        // use lower significant digits and the frequency selection digit f_digit
        wish_freq = (wish_freq % f_incre) + ((f_digit%Max_Digit) * f_incre);
     } else {
	// with only one significant there are no lower significant digits
        wish_freq = ((f_digit%Max_Digit) * f_incre);
     }
     lcd_line2();
     right = ((f_digit == Max_Digit) && (significant > 17));
     uint8_t long_key_char;
     if (right) {
        long_key_char = '<';	// the frequency string will move to the left by long key press
     } else {
        if (significant < (16+6)) {
           long_key_char = '>';		// move the frequency string to the right with one leading zero
        } else {
           long_key_char = 'R';		// i100kHz is the last position, Reset to 1 by long key press
        }
     }
     lcd_data(long_key_char);		// show the selected character for long key action
#if (LCD_LINES > 2)
     lcd_space();
#endif
     Display_Hz(wish_freq, significant);	// show the selected frequuency
     hperiod.dw = (F_CPU + wish_freq) / (wish_freq * 2);  // total divider for selected frequenx
     divider = 1;
     if (hperiod.w[1] >= 256) {
        // divide by 1024
        TCCR1B = (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (1<<CS10); // divide clock by 1024
        divider = 1024;
     } else if (hperiod.w[1] >= 64) {
        // divide by 256
        TCCR1B = (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10); // divide clock by 256
        divider = 256;
     } else if (hperiod.w[1] >= 8) {
        // divide by 64
        TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10); // divide clock by 64
        divider = 64;
     } else if (hperiod.w[1] >= 1) {
        // divide by 8
        TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10); // divide clock by 8
        divider = 8;
     } else {
        // no divide
        TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10); // no clock divide
     }
     OCR1A = (hperiod.dw / divider) - 1;	// the remainder divider  match to 16-bit counter 
     if (OCR1A == 0) OCR1A = 1;
#if (LCD_LINES > 2)
     lcd_clear_line();		// clear to end of line 2
     lcd_line3();
     lcd_data('f');
     lcd_space();

//     if (divider > 1) {
//        DisplayValue((((F_CPU/2) / divider) * 1000) / (OCR1A + 1),-3,'H',7);
//     } else {
//        DisplayValue((((F_CPU/2) / divider)) / (OCR1A + 1),0,'H',7);
//     }
//     lcd_data('z');
//     lcd_clear_line();
#endif
     long int diff_freq;
     diff_freq = (((F_CPU/2) / divider) * 100) / (OCR1A + 1) - (wish_freq * 100);
#if (LCD_LINES > 2)
     DisplayValue(diff_freq,-2,'H',32+5);
#else
     DisplayValue(diff_freq,-2,'H',32+2);
#endif
     lcd_data('z');
     lcd_clear_line();
     key_pressed = wait_for_key_ms(1500);	// check key and rotary encoder
#ifdef POWER_OFF
 #ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 0)) times = 0;	// reset counter, operator is active
 #else
     if (key_pressed != 0)  times = 0;	// reset counter, operator is active
 #endif
#endif
     if(key_pressed >= 150) break;	// more than 1.4 seconds
     if (key_pressed > 80) {	/* more than 0.8 seconds key press */
        // change of f_incre
        if (right) {
           // frequency string will move to the left, upper digit will move to the next lower digit
           Max_Digit = 10;		// f_incre will be 10kHz or less, allways 0-9
           significant--;		// diplay one digit less
           f_incre /= 10;		// 
           f_digit = (wish_freq / f_incre) % 10; // start with last digit
           continue;
        } else {
           if (significant >= (16+6)) {
              // reset to begin
//              wish_freq = 1;		// start with 1 Hz
              Max_Digit = 10;
              f_incre = 1;		// increment with 1 Hz
              f_digit = 1;		// start value = 1 Hz
              significant = 16+1;	// only one significant digit
              continue;
           }
           significant++;		// display one digit more
           if (significant == (16+6)) Max_Digit = 21;
           f_incre *= 10;		// change next digit to the left
           f_digit = 0;			// start new digit with zero
           continue;
        }
     } else {
        if (key_pressed > KEYPRESS_LENGTH_10ms) f_digit++; // longer key press select next frequency
     }
#ifdef WITH_ROTARY_SWITCH
     if (rotary.incre > FAST_ROTATION) break;		// fast rotation ends voltage measurement
     if (rotary.count >= 0) {
        f_digit += rotary.count;		// increase the frequency number by rotary.count
     } else {
        f_digit += (Max_Digit + 1 + rotary.count);	// decrease the frequency by rotary.count
     }
#endif
     f_digit = f_digit % (Max_Digit + 1);
#ifdef POWER_OFF
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
#endif
  } /* end for times */
  TCCR1B = 0;		// stop counter
  TCCR1A = 0;		// stop counter
  ADC_DDR =  TXD_MSK;	// disconnect TP1 
  R_DDR = 0;		// switch resistor ports to Input
#if PROCESSOR_TYP == 1280
  DDRB  &= ~(1<<DDB6);	// disable output 
#else
  DDRB  &= ~(1<<DDB2);	// disable output 
#endif

} /* end make frequency */

