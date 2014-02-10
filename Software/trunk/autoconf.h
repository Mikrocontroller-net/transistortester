
/*########################################################################################
       Automatic Configuration
*/
#if BAT_NUMERATOR < BAT_DENOMINATOR
 #warning "Wrong BAT_NUMERATOR / BAT_DENOMINATOR setting!"
#endif

#if EXT_NUMERATOR < EXT_DENOMINATOR
 #warning "Wrong EXT_NUMERATOR / EXT_DENOMINATOR setting!"
#endif

#ifndef R_L_VAL
  #define R_L_VAL 6800          // standard value 680 Ohm, multiplied by 10 for 0.1 Ohm resolution
#else
 #if R_L_VAL != 6800
  // please check the table RLtab in Transistortester.h for non standard RL value
  #warning "no standard RL !=680"
 #endif
#endif
#ifndef R_H_VAL
  #define R_H_VAL 47000         // standard value 470000 Ohm, multiplied by 10, divided by 100 
#else
 #if R_H_VAL != 47000
  // please check the table RHtab in Transistortester.h for non standard RH value
  #warning "no standard RH !=470k"
 #endif
#endif

#if R_ANZ_MESS < ANZ_MESS
 #undef R_ANZ_MESS
 #define R_ANZ_MESS ANZ_MESS
#endif

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
#elif PROCESSOR_TYP == 1280
//------------------=========----------
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
  #define OFF_WAIT_TIME LONG_WAIT_TIME
 #endif
#else
// if POWER OFF function is not selected, wait 28s before repeat measurement
 #define OFF_WAIT_TIME  LONG_WAIT_TIME
#endif

//**********************************************************
// defines for the selection of a correctly  ADC-Clock 
// will match for 1MHz, 2MHz, 4MHz, 8MHz and 16MHz
// ADC-Clock can be 125000 or 250000 
// 250 kHz is out of the full accuracy specification!
// clock divider is 4, when CPU_Clock==1MHz and ADC_Clock==250kHz
// clock divider is 128, when CPU_Clock==16MHz and ADC_Clock==125kHz
#define F_ADC 125000
//#define F_ADC 250000
#if F_CPU/F_ADC == 2
 #define AUTO_CLOCK_DIV (1<<ADPS0) 
#endif
#if F_CPU/F_ADC == 4
 #define AUTO_CLOCK_DIV (1<<ADPS1) 
#endif
#if F_CPU/F_ADC == 8
 #define AUTO_CLOCK_DIV (1<<ADPS1) | (1<<ADPS0)
#endif
#if F_CPU/F_ADC == 16
 #define AUTO_CLOCK_DIV (1<<ADPS2)
#endif
#if F_CPU/F_ADC == 32
 #define AUTO_CLOCK_DIV (1<<ADPS2) | (1<<ADPS0)
#endif
#if F_CPU/F_ADC == 64
 #define AUTO_CLOCK_DIV (1<<ADPS2) | (1<<ADPS1)
#endif
#if F_CPU/F_ADC == 128
 #define AUTO_CLOCK_DIV (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
#endif
//**********************************************************
#define F_ADC_F 500000
#if F_CPU/F_ADC_F == 2
 #define FAST_CLOCK_DIV (1<<ADPS0) 
#endif
#if F_CPU/F_ADC_F == 4
 #define FAST_CLOCK_DIV (1<<ADPS1) 
#endif
#if F_CPU/F_ADC_F == 8
 #define FAST_CLOCK_DIV (1<<ADPS1) | (1<<ADPS0)
#endif
#if F_CPU/F_ADC_F == 16
 #define FAST_CLOCK_DIV (1<<ADPS2)
#endif
#if F_CPU/F_ADC_F == 32
 #define FAST_CLOCK_DIV (1<<ADPS2) | (1<<ADPS0)
#endif
#if F_CPU/F_ADC_F == 64
 #define FAST_CLOCK_DIV (1<<ADPS2) | (1<<ADPS1)
#endif
#if F_CPU/F_ADC_F == 128
 #define FAST_CLOCK_DIV (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
#endif

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
 #define wait_about10ms() sleep_5ms(2)
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
#define LCD_CHAR_RESIS1 7       // Resistor left part will be generated as custom character
#define LCD_CHAR_RESIS2 6       // Resistor right part will be generated as custom character

#ifdef LCD_CYRILLIC
        #define LCD_CHAR_OMEGA  4       //Omega-character 
        #define LCD_CHAR_U  5           //µ-character 
#else
        #define LCD_CHAR_OMEGA  244     //Omega-character
        #define LCD_CHAR_U  228         //µ-character
#endif

#ifdef LCD_DOGM
	#undef LCD_CHAR_OMEGA
	#define LCD_CHAR_OMEGA 0x1e	//Omega-character for DOGM module
        #undef LCD_CHAR_U
        #define LCD_CHAR_U  5           //µ-character for DOGM module loadable
#endif

#define LCD_CHAR_DEGREE 0xdf            // Character for degree
#define LCD_CHAR_INSEP 0xff		// used as space character without separating text

#if FLASHEND > 0x3fff
 #define LCD_CHAR_RESIS3 0
#else
 #define LCD_CHAR_RESIS3 'R'
#endif


// SEARCH_PARASITIC let the Tester search for greater Base-Emitter capacity, if two transistors are detected.
// The one with the lower capacity value is the parasitic, the other is shown by default
// This is enabled for all processors (deselecting save about 120 bytes flash)
// Otherwise the shown type (NPNp or PNPn) depends on the selected pin sequence of the tester!
#define SEARCH_PARASITIC

#if FLASHEND > 0x1fff
// you can save about 14 bytes of Flash, if you deselect Thyristor gate voltage
 #define WITH_THYRISTOR_GATE_V
#endif

#if FLASHEND > 0x1fff
 // You can save about 328 bytes of Flash, if you don't show the ICE0 and ICEs Collector cutoff current.
 // Only enabled for mega328, but you can also enable it for mega168, if you deselect other functions.
 // You can save about 250 bytes flash, if you deselect the WITH_UART option.
 #define SHOW_ICE
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

#ifdef NO_TEST_T1_T7
 #undef EXTENDED_TESTS
#endif

#ifdef EXTENDED_TESTS
 #if FLASHEND <= 0x3fff
  // we have to save some memory for the mega168 to enable the extended tests!
  #ifdef WITH_UART
   #warning "Serial Output is deselected to save memory!"
   #undef WITH_UART
  #endif
  #undef SEARCH_PARASITIC
  #warning "Search of parasitic transistor not possible. NPNp or PNPn result depends on the selected pin sequence!"
  #ifdef SHOW_ICE
   #undef SHOW_ICE
   #warning "Display of ICE is disabled to save memory!"
  #endif
 #endif
#endif

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

#if F_CPU <= 1000000UL
  #define CNTR2_PRESCALER (1<<CS22) | (0<<CS21) | (1<<CS20)   /* prescaler 128, 128us @ 1MHz */
  #define T2_PERIOD 128
#endif
#if F_CPU == 2000000UL
  #define CNTR2_PRESCALER (1<<CS22) | (1<<CS21) | (0<<CS20)   /* prescaler 256, 128us @ 2MHz */
  #define T2_PERIOD 128
#endif
#if F_CPU == 4000000UL
  #define CNTR2_PRESCALER (1<<CS22) | (1<<CS21) | (0<<CS20)   /* prescaler 256, 64us @ 2MHz */
  #define T2_PERIOD 64
#endif
#if F_CPU >= 8000000UL
  #define CNTR2_PRESCALER (1<<CS22) | (1<<CS21) | (1<<CS20)   /* prescaler 1024, 128us @ 8MHz, 64us @ 16MHz */
// #define T2_PERIOD (1024/(F_CPU/1000000UL))
  #define T2_PERIOD (1024 / MHZ_CPU)
       /* set to 128 or 64 us */
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
 
/* LDIZ load the address adr to the Z register R30:R31 */
     .macro LDIZ adr
	ldi	ZL, lo8(\adr)
	ldi	ZH, hi8(\adr)
     .endm

#endif

