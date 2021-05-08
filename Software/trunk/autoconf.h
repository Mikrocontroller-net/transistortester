/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Transistor Tester
 *  
 *  File:       autofonfig.h
 *  Funktion:   Automatic Configuration
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-10  Karl      
 *
 * ************************************************************************ */

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 7735) || (LCD_ST_TYPE == 9163) || (LCD_ST_TYPE == 9341) || (LCD_ST_TYPE == 1327))
/* Define under which conditions a graphical display is supported. */
/* The graphical display should support at least 128x64 pixels. */
 #if ((SCREEN_HEIGHT > 128) && (SCREEN_WIDTH > 255))
  #define ONE_B 16
 #else
  #define ONE_B 8
 #endif
 #define WITH_GRAPHICS 1
 #undef PAGES_PER_LINE
 #define PAGES_PER_LINE ((FONT_HEIGHT + 7) / 8)
 #undef LCD_LINES
 #define LCD_LINES ((SCREEN_HEIGHT/ONE_B) / PAGES_PER_LINE)
 #undef LCD_LINE_LENGTH
 #define LCD_LINE_LENGTH (SCREEN_WIDTH / (ONE_B/8) / FONT_H_SPACE )
 #define TEXT_RIGHT_TO_ICON ((ICON_WIDTH + 16 + 5 + 7) / FONT_H_SPACE)
#elif (LCD_ST_TYPE == 8812)
 /* defines for PCF8812 */
 /* The space at the display is too small for the big icons for this type, some characters are missing */
 #define ONE_B 8
 #define WITH_GRAPHICS 1
 #undef PAGES_PER_LINE
 #define PAGES_PER_LINE ((FONT_HEIGHT + 7) / 8)
 #undef LCD_LINES
 #define LCD_LINES ((SCREEN_HEIGHT/8) / PAGES_PER_LINE)
 #undef LCD_LINE_LENGTH
 #define LCD_LINE_LENGTH (SCREEN_WIDTH / FONT_H_SPACE)
 #define TEXT_RIGHT_TO_ICON ((ICON_WIDTH + 16 + 5 + 7) / FONT_H_SPACE)
#elif (LCD_ST_TYPE == 8814)
 /* defines for PCF8814 */
 /* The space at the display is too small for the big icons for this type, some characters are missing */
 #define ONE_B 8
 #define WITH_GRAPHICS 1
 #undef PAGES_PER_LINE
 #define PAGES_PER_LINE ((FONT_HEIGHT + 7) / 8)
 #undef LCD_LINES
 #define LCD_LINES ((SCREEN_HEIGHT/8) / PAGES_PER_LINE)
 #undef LCD_LINE_LENGTH
 #define LCD_LINE_LENGTH (SCREEN_WIDTH / FONT_H_SPACE)
 #define TEXT_RIGHT_TO_ICON ((ICON_WIDTH + 16 + 5 + 7) / FONT_H_SPACE)
#else  /* no ST7565 or SSD1306 graphic controller */
// #ifdef FOUR_LINE_LCD
//  #if FOUR_LINE_LCD == 3
//   #define LCD_LINES 3
//   #define PAGES_PER_LINE 1
//   #ifndef LCD_LINE_LENGTH
//    #define LCD_LINE_LENGTH 16	/* usually a 16 character line */
//   #endif
//  #else
//   #define LCD_LINES 4
//   #define PAGES_PER_LINE 1
//   #ifndef LCD_LINE_LENGTH
//    #define LCD_LINE_LENGTH 20	/* usually a 20 character line */
//   #endif
//  #endif
// #else	/* no FOUR_LINE_LCD */
//   #define PAGES_PER_LINE 1
//   #define LCD_LINES 2
//   #define LCD_LINE_LENGTH 16
// #endif
 #define TEXT_RIGHT_TO_ICON 0
 #ifndef FONT_HEIGHT
  #define FONT_HEIGHT 8
 #endif
#endif	/* end LCD_ST_TYPE == */

#ifndef LCD_ST7565_H_OFFSET
 #define LCD_ST7565_H_OFFSET 0	/* default horizontal offset of the display window */
#endif

#ifndef LCD_ST7565_Y_START
 #define LCD_ST7565_Y_START  0
#endif
#if LCD_ST7565_Y_START >= SCREEN_HEIGHT
 #warning LCD_ST7565_Y_START is set higher than SCREEN_HEIGHT!
#endif

#ifndef LCD_LINE_LENGTH
 // if line length is not specified otherwise, use 16 characters
 #define LCD_LINE_LENGTH 16
#endif
//#if (LCD_LINE_LENGTH + 0) < 16
// #warning "Your screen can show less then 16 characters in a line! Probably a smaller font can help."
//#endif

#if (defined(WITH_GRAPHICS) && (((SCREEN_WIDTH-(TEXT_RIGHT_TO_ICON*FONT_H_SPACE))/FONT_H_SPACE) < 9))
 #define LOW_H_SPACE 1
#else
 #define LOW_H_SPACE 0
#endif

#if (LCD_ST_TYPE != 7920)
#define lcd_refresh() 
#endif
#if (LCD_LINES < 2)
 #warning LCD_LINES is not correctly set!
 #if (SCREEN_HEIGHT < 32)
  #warning SCREEN_HEIGHT is not correctly set!
 #endif
 #if (FONT_HEIGHT < 8)
  #warning FONT_HEIGHT is not correctly set!
 #endif
#endif

#if (LCD_LINES < 7) && defined(WITH_6_SELECTION_MENU)
 #warning Display has not sufficient count of lines for WITH_6_SELECTION_MENU !
 #undef WITH_6_SELECTION_MENU
#endif

/* configure WITH_VEXT, TPext and TPex2  */
#if (PROCESSOR_TYP == 8) || (PROCESSOR_TYP == 168) || (PROCESSOR_TYP == 328)
 #if FLASHEND > 0x1fff	/* at least ATmega16 */
  #ifdef TQFP_ADC6
    #undef TPext
    #define TPext ((1<<MUX2) | (1<<MUX1))
    #ifdef TQFP_ADC7	/* both TQFP_ADC options defined */
      #define TPex2 ((1<<MUX2) | (1<<MUX1) | (1<<MUX0))
    #endif
  #else  /* TQFP_ADC6 */
    /* TQFP_ADC6 is undefined */
    #ifdef TQFP_ADC7
      #undef TPext
      #define TPext ((1<<MUX2) | (1<<MUX1) | (1<<MUX0))
    #else   /* no TQFP_ADC6 and no TQFP_ADC7 */
     #if (WITH_UART != 0) && (WITH_VEXT != 0)
      #warning "WITH_VEXT deselected, PC3 is used for UART"
      #undef WITH_VEXT	/* voltage measurement is not possible with UART output */
     #endif
    #endif  /* TQFP_ADC7 */
  #endif  /* TQFP_ADC6 */
 #else   /* FLASHEND <= 0x1fff */
  #ifndef BAT_CHECK
    #ifdef TQFP_ADC6
      #undef TPext
      #define TPext ((1<<MUX2) | (1<<MUX1))
    #endif		/* TQFP_ADC6 */
    #ifdef TQFP_ADC7
      #undef TPext
      #define TPext ((1<<MUX2) | (1<<MUX1) | (1<<MUX0))
    #endif		/* TQFP_ADC7 */
    #if (WITH_UART == 1) && (TQFP_ADC6 == 0) && (TQFP_ADC7 == 0) && (WITH_VEXT != 0)
      #warning "WITH_VEXT deselected, PC3 is used for UART"
      #undef WITH_VEXT	/* voltage measurement is not possible with UART output */
    #endif  /* WITH_UART */
  #endif   /* BAT_CHECK */
 #endif  /* FLASHEND > 0x1fff */
#endif	/* PROCESSOR_TYP == 8|168|328 */


/* check the BAT_NUMERATOR and BAT_DENOMINATOR setting */
#if BAT_NUMERATOR < BAT_DENOMINATOR
 #warning "Wrong BAT_NUMERATOR / BAT_DENOMINATOR setting!"
#endif

#if EXT_NUMERATOR < EXT_DENOMINATOR
 #warning "Wrong EXT_NUMERATOR / EXT_DENOMINATOR setting!"
#endif

/* check the R_L_VAL and R_H_VAL setting */
#ifndef R_L_VAL
  #define R_L_VAL 6800          // standard value 680 Ohm, multiplied by 10 for 0.1 Ohm resolution
#endif
#ifndef R_H_VAL
  #define R_H_VAL 47000         // standard value 470000 Ohm, multiplied by 10, divided by 100 
#endif

/* check the R_ANZ_MESS setting */
#if R_ANZ_MESS < ANZ_MESS
 #undef R_ANZ_MESS
 #define R_ANZ_MESS ANZ_MESS
#endif

/* check the U_SCALE setting, U_SCALE is temporary used to increase resolution of ReadADC */
#if U_SCALE < 1
 // limit U_SCALE
 #undef U_SCALE
 #define U_SCALE 1
#endif
#if U_SCALE > 4
 // limit U_SCALE
 #undef U_SCALE
 #define U_SCALE 4
#endif

#ifndef REF_L_KORR
 #define REF_L_KORR 50
#endif

// the following definitions specify where to load external data from: EEprom or flash
#ifdef USE_EEPROM
 #define MEM_TEXT EEMEM
 #define lcd_MEM_string(a)  lcd_fix_string(a)
 #if E2END > 0X1FF
  #define MEM2_TEXT EEMEM
  #define MEM2_read_byte(a)  eeprom_read_byte(a)
  #define MEM2_read_word(a)  eeprom_read_word(a)
  #define lcd_MEM2_string(a)  lcd_fix_string(a)
  #define use_lcd_pgm
 #else
  #define MEM2_TEXT PROGMEM
  #define MEM2_read_byte(a)  pgm_read_byte(a)
  #define MEM2_read_word(a)  pgm_read_word(a)
  #define lcd_MEM2_string(a)  lcd_pgm_string(a)
  #define use_lcd_pgm
 #endif
 #define MEM_EEPROM
 #define MEM_read_word(a)  eeprom_read_word(a)
 #define MEM_read_byte(a)  eeprom_read_byte(a)
#else
 #define MEM_TEXT PROGMEM
 #define lcd_MEM_string(a)  lcd_pgm_string(a)
 #define MEM2_TEXT PROGMEM
 #define MEM_read_word(a)  pgm_read_word(a)
 #define MEM_read_byte(a)  pgm_read_byte(a)
 #define MEM2_read_byte(a)  pgm_read_byte(a)
 #define MEM2_read_word(a)  pgm_read_word(a)
 #define lcd_MEM2_string(a)  lcd_pgm_string(a)
 #define use_lcd_pgm
#endif

// automatic selection of right assembler call type
#if FLASHEND > 0X1FFF
 #define ACALL call
#else
 #define ACALL rcall
#endif

// automatic selection of option and parameters for different AVR s

//------------------=========----------
#if PROCESSOR_TYP == 168
//------------------=========----------
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_BAND_GAP 1070
 #if R_H_VAL == 10000
  #define DEFAULT_RH_FAKT 4022      // mega168 1101 mV, 100.0 kOhm
 #else
  #define DEFAULT_RH_FAKT  856      // mega168 1101 mV, 470.0 kOhm
 #endif
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE

// Pin resistor values of ATmega168
  #define PIN_RM 196
  #define PIN_RP 221
// CC0 defines the capacity of empty terminal pins 1 & 3 without cable
  #define CC0 36
// Slew rate correction  val += COMP_SLEW1 / (val + COMP_SLEW2)
  #define COMP_SLEW1 4000
  #define COMP_SLEW2 220
  #define C_NULL CC0+CABLE_CAP+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2))
  #define MUX_INT_REF 0x0e	/* channel number of internal 1.1 V */

//------------------=========----------
#elif PROCESSOR_TYP == 328
//------------------=========----------
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_BAND_GAP 1070
 #if R_H_VAL == 10000
  #define DEFAULT_RH_FAKT 4022      // mega328 1101 mV, 100.0 kOhm
 #else
  #define DEFAULT_RH_FAKT  856      // mega328 1101 mV, 470.0 kOhm
 #endif
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE

  #define PIN_RM 200
  #define PIN_RP 220
// CC0 defines the capacity of empty terminal pins 1 & 3 without cable
  #define CC0 36
// Slew rate correction  val += COMP_SLEW1 / (val + COMP_SLEW2)
  #define COMP_SLEW1 4000
  #define COMP_SLEW2 180
  #define C_NULL CC0+CABLE_CAP+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2))
  #define MUX_INT_REF 0x0e	/* channel number of internal 1.1 V */

//------------------=========----------
#elif PROCESSOR_TYP == 644
//------------------=========----------
 #ifndef WITH_ROTARY_CHECK
  #define WITH_ROTARY_CHECK		/* ROTARY Check is default for 644 */
 #endif
 #ifndef WITH_UJT
  #define WITH_UJT
 #endif
 #ifndef WITH_PUT
  #define WITH_PUT
 #endif
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_BAND_GAP 1070
 #if R_H_VAL == 10000
  #define DEFAULT_RH_FAKT 4022      // mega1280 1101 mV, 100.0 kOhm
 #else
  #define DEFAULT_RH_FAKT  856      // mega1280 1101 mV, 470.0 kOhm
 #endif
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE

  #define PIN_RM 200
  #define PIN_RP 220
// CC0 defines the capacity of empty terminal pins 1 & 3 without cable
  #define CC0 36
// Slew rate correction  val += COMP_SLEW1 / (val + COMP_SLEW2)
  #define COMP_SLEW1 4000
  #define COMP_SLEW2 180
  #define C_NULL CC0+CABLE_CAP+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2))
  #define MUX_INT_REF 0x1e	/* channel number of internal 1.1 V */


//------------------=========----------
#elif PROCESSOR_TYP == 1280
//------------------=========----------
 #ifndef WITH_ROTARY_CHECK
  #define WITH_ROTARY_CHECK		/* ROTARY Check is default for 1280 */
 #endif
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_BAND_GAP 1070
 #if R_H_VAL == 10000
  #define DEFAULT_RH_FAKT 4022      // mega1280 1101 mV, 100.0 kOhm
 #else
  #define DEFAULT_RH_FAKT  856      // mega1280 1101 mV, 470.0 kOhm
 #endif
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE

  #define PIN_RM 200
  #define PIN_RP 220
// CC0 defines the capacity of empty terminal pins 1 & 3 without cable
  #define CC0 36
// Slew rate correction  val += COMP_SLEW1 / (val + COMP_SLEW2)
  #define COMP_SLEW1 4000
  #define COMP_SLEW2 180
  #define C_NULL CC0+CABLE_CAP+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2))
  #define MUX_INT_REF 0x1e	/* channel number of internal 1.1 V */


//------------------=========----------
#else
//                   ATmega8
//------------------=========----------
  #define MCU_STATUS_REG MCUCSR
  #define ADC_COMP_CONTROL SFIOR
  #define TI1_INT_FLAGS TIFR
  #define DEFAULT_BAND_GAP 1298		//mega8 1298 mV
 #if R_H_VAL == 10000
  #define DEFAULT_RH_FAKT 3328      // mega8 1298 mV, 100.0 kOhm
 #else
  #define DEFAULT_RH_FAKT  740      // mega8 1250 mV, 470.0 kOhm
 //#define DEFAULT_RH_FAKT  708      // mega8 1298 mV, 470.0 kOhm
 #endif
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE

  #define PIN_RM 196
  #define PIN_RP 240
// CC0 defines the capacity of empty terminal pins 1 & 3 without cable
  #define CC0 27
// Slew rate correction  val += COMP_SLEW1 / (val + COMP_SLEW2)
  #define COMP_SLEW1 0
  #define COMP_SLEW2 33
  #define C_NULL CC0+CABLE_CAP+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2))
  #define MUX_INT_REF 0x0e	/* channel number of internal 1.1 V */
 #ifndef INHIBIT_SLEEP_MODE
  #define INHIBIT_SLEEP_MODE	/* do not use the sleep mode of ATmega8 */
  #warning "SLEEP_MODE not possible with mega8"
 #endif
#endif

#ifndef REF_R_KORR
 #define REF_R_KORR 0
#endif
#ifndef REF_C_KORR
 #define REF_C_KORR 0
#endif


#ifdef POWER_OFF
// if POWER OFF function is selected, wait 28s
// if POWER_OFF with parameter > 2, wait only 5s before repeating
 #if (POWER_OFF+0) > 2
  #define OFF_WAIT_TIME SHORT_WAIT_TIME
 #else
  #define OFF_WAIT_TIME (LONG_WAIT_TIME - 1)
 #endif
#else
// if POWER OFF function is not selected, wait 28s before repeat measurement
 #define OFF_WAIT_TIME  (LONG_WAIT_TIME - 1)
#endif

//**********************************************************
// defines for the selection of a correctly  ADC-Clock 
// will match for 1MHz, 2MHz, 4MHz, 8MHz and 16MHz
// ADC-Clock can be 125000 or 250000 
// 250 kHz is out of the full accuracy specification!
// clock divider is 4, when CPU_Clock==1MHz and ADC_Clock==250kHz
// clock divider is 128, when CPU_Clock==16MHz and ADC_Clock==125kHz
#define F_ADC 200000
//#define F_ADC 250000
#if F_CPU/F_ADC <= 2
 #define TICS_PER_ADC_CLOCK 2
 #define AUTO_CLOCK_DIV ((0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0))
#elif F_CPU/F_ADC <= 4
 #define TICS_PER_ADC_CLOCK 4
 #define AUTO_CLOCK_DIV ((0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0))
#elif F_CPU/F_ADC <= 8
 #define TICS_PER_ADC_CLOCK 8
 #define AUTO_CLOCK_DIV ((0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))
#elif F_CPU/F_ADC <= 16
 #define TICS_PER_ADC_CLOCK 16
 #define AUTO_CLOCK_DIV ((1<<ADPS2) | (0<<ADPS1) | (0<<ADPS0))
#elif F_CPU/F_ADC <= 32
 #define TICS_PER_ADC_CLOCK 32
 #define AUTO_CLOCK_DIV ((1<<ADPS2) | (0<<ADPS1) | (1<<ADPS0))
#elif F_CPU/F_ADC <= 64
 #define TICS_PER_ADC_CLOCK 64
 #define AUTO_CLOCK_DIV ((1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0))
#else
 #define TICS_PER_ADC_CLOCK 128
 #define AUTO_CLOCK_DIV ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))
#endif
//**********************************************************

#ifndef PIN_RP
  #define PIN_RP  220           //estimated internal resistance PORT to VCC
                                //will only be used, if not set before in config.h
#endif
#ifndef PIN_RM
  #define PIN_RM  190           //estimated internal resistance PORT to GND
                                //will only be used, if not set before in config.h
#endif

#ifdef INHIBIT_SLEEP_MODE
 // save memory, do not use the sleep mode
 #define wait_about5ms() wait5ms()
 #define wait_about10ms() wait10ms()
 #define wait_about20ms() wait20ms()
 #define wait_about30ms() wait30ms()
 #define wait_about50ms() wait50ms()
 #define wait_about100ms() wait100ms()
 #define wait_about200ms() wait200ms()
 #define wait_about300ms() wait300ms()
 #define wait_about400ms() wait400ms()
 #define wait_about500ms() wait500ms()
 #define wait_about1s() wait1s()
 #define wait_about2s() wait2s()
 #define wait_about3s() wait3s()
 #define wait_about4s() wait4s()
 #define wait_about5s() wait5s()
#else
 // use sleep mode to save current for user interface
 #define wait_about5ms() sleep_5ms(1)
 #define wait_about10ms() sleep_10ms()
 #define wait_about20ms() sleep_5ms(4)
 #define wait_about30ms() sleep_5ms(6)
 #define wait_about50ms() sleep_5ms(10)
 #define wait_about100ms() sleep_5ms(20)
 #define wait_about200ms() sleep_5ms(40)
 #define wait_about300ms() sleep_5ms(60)
 #define wait_about400ms() sleep_5ms(80)
 #define wait_about500ms() sleep_5ms(100)
 #define wait_about1s() sleep_5ms(200)
 #define wait_about2s() sleep_5ms(202)
 #define wait_about3s() sleep_5ms(203)
 #define wait_about4s() sleep_5ms(204)
 #define wait_about5s() sleep_5ms(205)
#endif

#ifndef WITH_SELFTEST
 // no calibration without selftest
 #ifdef AUTO_CAL
  #undef AUTO_CAL
  #warning "No AUTO_CAL without WITH_SELFTEST!"
 #endif
#endif

#undef AUTO_RH
#ifdef WITH_AUTO_REF
 // variable reference voltage needs table RHtab
 #define AUTO_RH
#else
 #ifdef AUTO_CAL
  // calibration needs table RHtab
  #define AUTO_RH
 #endif
#endif

#ifdef AUTO_CAL
  // AutoCheck Function is needed
  #define RR680PL resis680pl
  #define RR680MI resis680mi
  #define RRpinPL pin_rpl
  #define RRpinMI pin_rmi
#else
  #define RR680PL (R_L_VAL + PIN_RP)
  #define RR680MI (R_L_VAL + PIN_RM)
  #define RRpinPL (PIN_RP)
  #define RRpinMI (PIN_RM)
#endif

#ifndef ESR_ZERO
 //define a default zero value for ESR measurement (0.01 Ohm units)
 #define ESR_ZERO 20
#endif

#ifndef RESTART_DELAY_TICS
 // define the processor restart delay for crystal oscillator 16K
 // only set, if no preset (Makefile) exists.
 #define RESTART_DELAY_TICS 16384
 // for ceramic oscillator 258 or 1024 Clock tics can be selected with fuses
 // for external oscillator or RC-oscillator is only a delay of 6 clock tics.
#endif

// with EBC_STYLE you can select the Pin-description in EBC= style instead of 123=??? style
//#define EBC_STYLE
#if EBC_STYLE == 123
 // unset the option for the 123 selection, since this style is default.
 #undef EBC_STYLE
#endif

//self build characters 
#define LCD_CHAR_DIODE1  1      //Diode-Icon; will be generated as custom character
#define LCD_CHAR_DIODE2  2      //Diode-Icon;  will be generated as custom character
#define LCD_CHAR_CAP 3          //Capacitor-Icon;  will be generated as custom character
        // numbers of RESIS1 and RESIS2 are swapped for OLED display, which shows a corrupt RESIS1 character otherwise ???
#define LCD_CHAR_RESIS1 4       // Resistor left part will be generated as custom character
#define LCD_CHAR_RESIS2 5       // Resistor right part will be generated as custom character


#ifdef LCD_CYRILLIC
        #define LCD_CHAR_OMEGA  3       //Omega-character 
        #define LCD_CHAR_U  5           //mu-character
#else
        #define LCD_CHAR_OMEGA  244     //Omega-character
        #define LCD_CHAR_U  228         //mu-character
#endif

#ifdef LCD_DOGM
	#undef LCD_CHAR_OMEGA
	#define LCD_CHAR_OMEGA 0x1e	//Omega-character for DOGM module
        #undef LCD_CHAR_U
        #define LCD_CHAR_U  6           //mu-character for DOGM module loadable
#endif


#define LCD_CHAR_DEGREE 0xdf            // Character for degree
#define LCD_CHAR_INSEP 0xff		// used as space character without separating text
#define LCD_NO_SPACE 0xfe               // used to separate text without space
#define LCD_CHAR_LINE1 '-'		// usually the minus sign is used for line symbol
#define LCD_CHAR_INDUCTOR1 'w'		// use ww for inductor symbol
#define LCD_CHAR_INDUCTOR2 'w'		// use ww for inductor symbol

#if (defined(WITH_GRAPHICS) || defined(LCD_USE_OWN_FONT))
// redefine the special symbols for software character set used with graphical display
        #undef LCD_CHAR_DEGREE
        #undef LCD_CHAR_OMEGA
        #undef LCD_CHAR_U
        #undef LCD_CHAR_LINE1
        #define LCD_CHAR_LINE1  6	// line is specified as long -
        #undef LCD_CHAR_INDUCTOR1
        #define LCD_CHAR_INDUCTOR1  7		//begin of coil
        #undef LCD_CHAR_INDUCTOR2
        #define LCD_CHAR_INDUCTOR2  8		//end of coil
 #if (defined WITH_GRAPHICS)
	#define LCD_CHAR_DEGREE 0x1f
	#define LCD_CHAR_U	0x1e
	#define LCD_CHAR_OMEGA	0x1d
	#define LCD_CHAR_BETA	0x1c
 #else
        #define LCD_CHAR_DEGREE 0xf8	// Character for degree
        #define LCD_CHAR_OMEGA  13       //Omega-character 
        #define LCD_CHAR_U  14		//\B5-character
	#define LCD_CHAR_BETA 12
 #endif
#endif 


#if FLASHEND > 0x3fff
 #define LCD_CHAR_RESIS3 0
#else
 #define LCD_CHAR_RESIS3 'R'
#endif


// SEARCH_PARASITIC let the Tester search for greater Base-Emitter capacity, if two transistors are detected.
// The one with the lower capacity value is the parasitic, the other is shown by default
// This is enabled for all processors (deselecting save about 120 bytes flash)
// Otherwise the shown type (NPNp or PNPn) depends on the selected pin sequence of the tester!
#if FLASHEND > 0x1fff
 #define SEARCH_PARASITIC
#endif

//#if FLASHEND > 0x1fff
// you can save about 14 bytes of Flash, if you deselect Thyristor gate voltage
 #define WITH_THYRISTOR_GATE_V
//#endif

#if FLASHEND > 0x1fff
 // You can save about 328 bytes of Flash, if you don't show the ICE0 and ICEs Collector cutoff current.
 // Only enabled for mega328, but you can also enable it for mega168, if you deselect other functions.
 // You can save about 250 bytes flash, if you deselect the WITH_UART option.
 #define SHOW_R_DS
 #if defined(NO_COMMON_EMITTER_HFE) && !defined(NO_TEST_T1_T7) && (FLASHEND < 0x3fff)
  #warning  Uf parameter of VAKDiode and ICE can not be shown with full selftest
 #else
  #define SHOW_VAKDIODE
  #define SHOW_ICE
 #endif
#endif

// COMMON_COLLECTOR activates measurement of current amplification factor in common collector circuit  (Emitter follower)
#ifndef NO_COMMON_COLLECTOR_HFE
 #define COMMON_COLLECTOR
#endif

/* the hFE (B) can also be determined with  common emitter circuit (not for atmega8 ) */
#if FLASHEND > 0x1fff
 #ifndef NO_COMMON_EMITTER_HFE
   // only define the common emitter, if the extended tests are not explicitly requested
   #define COMMON_EMITTER
 #endif
#endif

// automatic check, if the extended tests are possible!
#ifdef COMMON_EMITTER
 #ifdef COMMON_COLLECTOR
  // both hFE measurement methodes
  #define BOTH_HFE
  #if FLASHEND > 0x3fff
    // extended tests are only possible with enough memory!
    #define EXTENDED_TESTS
  #endif
 #else
    // no COMMON_COLLECTOR hFE measurement methode, the extended tests are always possible!
    #define EXTENDED_TESTS
 #endif
#else
 // no COMMON_EMITTER  hFE measurement methode, the extended tests are always possible!
 #define EXTENDED_TESTS
#endif

#ifdef COMMON_COLLECTOR
 #ifndef COMMON_EMITTER
    // Only by selecting one hFE measurement methode, the extended tests are always possible!
    #define EXTENDED_TESTS
 #endif
#else
 #ifndef COMMON_EMITTER
    // both measurement methodes are unselected, use COMMON_COLLECTOR
    #warning "No hFE measurement type selected, common collector circuit choosed!"
    #define COMMON_COLLECTOR
    // Only by selecting one hFE measurement methode, the extended tests are always possible!
    #define EXTENDED_TESTS
 #endif
#endif

//#define EXTENDED_TESTS  /* extended tests are possible for ATmega168, if the gcc option -mcall-prologues is set */

#ifdef NO_TEST_T1_T7
 #undef EXTENDED_TESTS	/* extended tests are deselected! */
#endif

#ifdef EXTENDED_TESTS
 #if FLASHEND <= 0x3fff
  // we have to save some memory for the mega168 to enable the extended tests!
  #ifdef WITH_UART
   /* WITH_UART require about 146 byte of memory */
   #warning "Serial Output is deselected to save memory!"
   #undef WITH_UART
  #endif
  #ifdef BOTH_HFE
   /* SEARCH_PARASITIC require abaut 112 byte of memory */
   #undef SEARCH_PARASITIC
   #warning "Search of parasitic transistor not possible. NPNp or PNPn result depends on the selected pin sequence!"
  #endif	/* BOTH_HFE */
  #ifdef SHOW_ICE
   #ifdef BOTH_HFE
    /* SHOW_ICE require about 290 byte of memory */
    #undef SHOW_ICE
    #warning "Display of ICE is disabled to save memory!"
   #endif	/* BOTH_HFE */
  #endif
 #endif
#endif	/* EXTENDED_TESTS */

// the following Options needs WAIT_LINE2_CLEAR
#ifdef WITH_SELFTEST
 #define WAIT_LINE2_CLEAR
#endif
#ifdef SHOW_ICE
 #define WAIT_LINE2_CLEAR
#endif

// the following Options need LCD_CLEAR   ( lcd_clear_line() )
#ifdef WAIT_LINE2_CLEAR
 // include SHOW_ICE and WITH_SELFTEST
 #define LCD_CLEAR
#endif
#ifdef WITH_VEXT
 #define LCD_CLEAR
#endif
#ifdef DEBUG_OUT
 #define LCD_CLEAR
#endif

// check for required T2 clock divider for 5ms / 10ms delay (max 255)
#if ((F_CPU/100) / 32) < 256
 #define CNTR2_PRESCALER (0<<CS22) | (1<<CS21) | (1<<CS20)	/* prescaler 32 */
 #define TICS_PER_T2_COUNT 32
#elif ((F_CPU/100) / 64) < 256
 #define CNTR2_PRESCALER (1<<CS22) | (0<<CS21) | (0<<CS20)	/* prescaler 64 */
 #define TICS_PER_T2_COUNT 64
#elif ((F_CPU/100) / 128) < 256
 #define CNTR2_PRESCALER (1<<CS22) | (0<<CS21) | (1<<CS20)	/* prescaler 128 */
 #define TICS_PER_T2_COUNT 128
#elif ((F_CPU/100) / 256) < 256
 #define CNTR2_PRESCALER (1<<CS22) | (1<<CS21) | (0<<CS20)	/* prescaler 256 */
 #define TICS_PER_T2_COUNT 256
#else
 #define CNTR2_PRESCALER (1<<CS22) | (1<<CS21) | (1<<CS20)	/* prescaler 1024 */
 #define TICS_PER_T2_COUNT 1024
#endif

#if FLASHEND <= 0x3fff
 #ifdef WITH_MENU
  #undef WITH_MENU
  #warning "no WITH_MENU possible with ATmega8 or ATmega168"
 #endif
#endif
#ifdef __ASSEMBLER__
/* AOUT output the content of reg to the specified IO-adr */
     .macro  AOUT adr, reg
     .if  _SFR_IO_REG_P(\adr)
        out     _SFR_IO_ADDR(\adr), \reg
     .else
        sts     \adr, \reg
     .endif
     .endm

/* AIN input the specified IO-adr to reg */
     .macro  AIN reg, adr
     .if  _SFR_IO_REG_P(\adr)
        in     \reg, _SFR_IO_ADDR(\adr)
     .else
        lds     \reg, \adr
     .endif
     .endm
 
/* LDIZ load the address adr to the Z register R30:R31 */
     .macro LDIZ adr
	ldi	ZL, lo8(\adr)
	ldi	ZH, hi8(\adr)
     .endm

#endif
#ifndef TP_OFFSET
 #define TP_OFFSET 0
#endif
#if  !defined(LCD_INTERFACE_MODE)  || (LCD_INTERFACE_MODE == MODE_PARALLEL)
 #define MODE_8BIT 0x00         /* 4-bit Mode */
#else
 #define MODE_8BIT 0x10
#endif

#if defined(WITH_GRAPHICS) && !defined(NO_ICONS_DEMO)
  #undef SHOW_ICONS		/* if allready defined */
  #define SHOW_ICONS
#endif
#ifdef PWM_SERVO
 #ifndef SERVO_MIN
  #define SERVO_MIN 40	/* Pulswidth 0.8ms, Period 20ms : 4 percent */
 #endif
 #ifndef SERVO_MAX
  #define SERVO_MAX 111	/* Pulswidth 2.2ms, Period 20ms : 11 percent */
 #endif
 #ifndef SERVO_PERIOD
  #define SERVO_PERIOD 20000UL	/* Period 20ms in us units */
 #endif
 #define SERVO_DIV 1
 #define SERVO_START ((0<<CS12) | (0<<CS11) | (1<<CS10))
 #if ((SERVO_PERIOD*MHZ_CPU)  > 0xffff)
  #undef SERVO_DIV
  #define SERVO_DIV 8
  #undef SERVO_START
  #define SERVO_START ((0<<CS12) | (1<<CS11) | (0<<CS10))
  #if (((SERVO_PERIOD * MHZ_CPU) / SERVO_DIV) > 0xffff)
   #undef SERVO_DIV
   #define SERVO_DIV 64
   #undef SERVO_START
   #define SERVO_START ((0<<CS12) | (1<<CS11) | (1<<CS10))
  #endif
 #endif
 #if SERVO_MAX > 251
  #warning  The maximum pulse width SERVO_MAX must set below 25.2% !
  #undef SERVO_MAX
  #define SERVO_MAX 251
 #endif
 #if SERVO_MIN >= SERVO_MAX
  #warning  The minimum pulse width SERVO_MIN must set below SERVO_MAX !
  #undef SERVO_MIN
  #define SERVO_MIN (SERVO_MAX/2)
 #endif
 #define PWM_MAX_COUNT ((SERVO_PERIOD * MHZ_CPU) / SERVO_DIV)
 #if PWM_MAX_COUNT < 0x3ff
  #warning PWM_MAX_COUNT
 #endif
#else
 
 #ifndef SERVO_MIN
  #define SERVO_MIN 0
 #endif
 #ifndef SERVO_MAX
  #define SERVO_MAX 100
 #endif
 #define PWM_MAX_COUNT 0x3ff
#endif

#if (TP1 < TP2) && (TP1 < TP3)
 #define TP_MIN TP1
#endif
#if (TP2 < TP1) && (TP2 < TP3)
 #define TP_MIN TP2
#endif
#if (TP3 < TP1) && (TP3 < TP2)
 #define TP_MIN TP3
#endif

#ifndef WITH_UART
 #define uart_newline()	// ignored
#endif

#if defined(LCD_SPI_OPEN_COL) && defined(WITH_ROTARY_SWITCH)
// #if ((ROTARY_A_DDR == HW_LCD_RES_DDR) && (ROTARY_A_PIN == HW_LCD_RES_PIN)) || ((ROTARY_A_DDR == HW_LCD_EN_DDR) && (ROTARY_A_PIN == HW_LCD_EN_PIN)) || ((ROTARY_A_DDR == HW_LCD_RS_DDR) && (ROTARY_A_PIN == HW_LCD_RS_PIN)) || ((ROTARY_A_DDR == HW_LCD_B0_DDR) && (ROTARY_A_PIN == HW_LCD_B0_PIN)) || ((ROTARY_A_DDR == HW_LCD_CE_DDR) && (ROTARY_A_PIN == HW_LCD_CE_PIN)) 
//  #warning Option WITH_ROTARY_SWITCH can not be used together with option LCD_SPI_OPEN_COL, if the ROTARY_A switch signals use the same port signals as the LCD.
// #endif
// #if ((ROTARY_B_DDR == HW_LCD_RES_DDR) && (ROTARY_B_PIN == HW_LCD_RES_PIN)) || ((ROTARY_B_DDR == HW_LCD_EN_DDR) && (ROTARY_B_PIN == HW_LCD_EN_PIN)) || ((ROTARY_B_DDR == HW_LCD_RS_DDR) && (ROTARY_B_PIN == HW_LCD_RS_PIN)) || ((ROTARY_B_DDR == HW_LCD_B0_DDR) && (ROTARY_B_PIN == HW_LCD_B0_PIN)) || ((ROTARY_B_DDR == HW_LCD_CE_DDR) && (ROTARY_B_PIN == HW_LCD_CE_PIN)) 
//  #warning Option WITH_ROTARY_SWITCH can not be used together with option LCD_SPI_OPEN_COL, if the ROTARY_B switch signals use the same port signals as the LCD.
// #endif
  #pragma message "Option WITH_ROTARY_SWITCH can not be used together with option LCD_SPI_OPEN_COL, if the ROTARY switch signals use the same port signals as the LCD."
#endif

/* ------------------------------------------------------------------- */
/* Set the USE_ defines for building the function-menu items           */

#define USE_ESR 1

#define USE_RESIS 1

#define USE_CAP13 1

#ifdef NO_FREQ_COUNTER
 #define USE_FREQ 0
 #define USE_HFREQ 0
 #define USE_H_CRYSTAL 0
 #define USE_L_CRYSTAL 0
#else   /* with frequency counter */
 #if PROCESSOR_TYP == 644
  #define USE_FREQ 1
  #define USE_HFREQ 1
  #define USE_H_CRYSTAL 1
  #define USE_L_CRYSTAL 1
 #else
  #define USE_FREQ 1
  #define USE_HFREQ 0
  #define USE_H_CRYSTAL 0
  #define USE_L_CRYSTAL 0
 #endif
#endif

#if (defined WITH_FREQUENCY_DIVIDER) && (USE_FREQ == 1)
 #define USE_FSCALER 1
#else
 #define USE_FSCALER 0
#endif

#define USE_FGEN 1

#define USE_PWM 1


#define USE_BIG_CAP_CORR 1

#ifdef WITH_ROTARY_CHECK
 #define USE_ROTARY 1
#else
 #define USE_ROTARY 0
#endif

#ifdef WITH_VEXT
 #define USE_VEXT 1
#else
 #define USE_VEXT 0
#endif

#ifdef WITH_SELFTEST
 #define USE_SELFTEST 1
#else
 #define USE_SELFTEST 0
#endif

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || defined(LCD_DOGM))
 #define USE_CONTRAST 1
#else
 #define USE_CONTRAST 0
#endif

#ifdef LCD_CHANGE_COLOR
 #define USE_SELECT_FG 1
 #define USE_SELECT_BG 1
#else
 #define USE_SELECT_FG 0
 #define USE_SELECT_BG 0
#endif

#define USE_SHOW 1

#ifdef POWER_OFF
 #define USE_OFF 1
#else
 #define USE_OFF 0
#endif

#define MODE_TRANS 0      /* Return to normal tester function is always default as number 0 */

#if 1
 #include "function_sequence1.h"
#else
 #include "function_sequence2.h"
#endif

/* ****************************** EOF ***************************************** */
