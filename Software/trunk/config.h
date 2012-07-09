
// the following definitions specify where to load external data from: EEprom or flash
#ifdef USE_EEPROM
#define MEM_TEXT EEMEM
#define MEM_read_word(a)  eeprom_read_word(a)
#define MEM_read_byte(a)  eeprom_read_byte(a)
#else
#define MEM_TEXT PROGMEM
#define MEM_read_word(a)  pgm_read_word(a)
#define MEM_read_byte(a)  pgm_read_byte(a)
#endif

// RH_OFFSET : systematic offset of resistor measurement with RH (470k) 
// resolution is 0.1 Ohm, 7000 defines a offset of 700 Ohm
#define RH_OFFSET 7000 

// automatic selection of option and parameters for different AVR s
//----------------========----------
#if defined(__AVR_ATmega48__)
//----------------========----------
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_RH_FAKT  856      // mega48 1101 mV
// WITH_SELFTEST enables the selftest fuction 
  #ifdef WITH_SELFTEST
  #warning "ATmega48 does NOT support SELFTEST!"
  #undef WITH_SELFTEST
  #endif
// R_MESS activates the resistor measurement 
  #ifdef R_MESS
  #warning "ATmega48 does NOT support Resistor measuring!"
  #undef R_MESS
  #endif
// C_MESS activates the capacitor measurement 
  #ifdef C_MESS
  #warning "ATmega48 does NOT support Capacity measuring!"
  #undef C_MESS
  #undef WITH_AUTO_REF
  #endif
// WITH_UART activates the output of data with software UART 
  #ifdef WITH_UART
  #warning "no UART support with ATmega48!"
  #undef WITH_UART
  #endif
  #ifdef BAT_CHECK
  #warning "no BAT_CHECK support with ATmega48!"
  #undef BAT_CHECK
  #endif

  #define C_NULL (((52 * F_CPU) / 10000000)+4)
  #define PIN_RM 190
  #define PIN_RP 220

//------------------========----------
#elif defined(__AVR_ATmega88__)
//------------------========----------
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_RH_FAKT  855      // mega88 1102 mV
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE
// COMMON_COLLECTOR activates measurement of current amplification factor also in common collector circuit  (Emitter follower)
  #define COMMON_COLLECTOR

  #define C_NULL (((52 * F_CPU) / 10000000)+4)
  #define PIN_RM 190
  #define PIN_RP 225

//------------------=========----------
#elif defined(__AVR_ATmega168__)
//------------------=========----------
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_RH_FAKT  855      // mega168 1102 mV
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE
// COMMON_COLLECTOR activates measurement of current amplification factor also in common collector circuit  (Emitter follower)
  #define COMMON_COLLECTOR

  #define C_NULL (((45 * F_CPU) / 10000000)+3)
  #define PIN_RM 196
  #define PIN_RP 225

//------------------=========----------
#elif defined(__AVR_ATmega328__)
//------------------=========----------
  #define MCU_STATUS_REG MCUCR
  #define ADC_COMP_CONTROL ADCSRB
  #define TI1_INT_FLAGS TIFR1
  #define DEFAULT_RH_FAKT  855      // mega328 1102 mV
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE
// COMMON_COLLECTOR activates measurement of current amplification factor also in common collector circuit  (Emitter follower)
  #define COMMON_COLLECTOR

  #define C_NULL (((52 * F_CPU) / 10000000)+4)
  #define PIN_RM 200
  #define PIN_RP 220

//------------------=========----------
#else
//                   ATmega8
//------------------=========----------
  #define MCU_STATUS_REG MCUCSR
  #define ADC_COMP_CONTROL SFIOR
  #define TI1_INT_FLAGS TIFR
  #define DEFAULT_RH_FAKT  708      // mega8 1298 mV
// LONG_HFE  activates computation of current amplification factor with long variables
  #define LONG_HFE
// COMMON_COLLECTOR activates measurement of current amplification factor also in common collector circuit  (Emitter follower)
  #define COMMON_COLLECTOR

  #define C_NULL (((37 * F_CPU) / 10000000)+3)
  #define PIN_RM 196
  #define PIN_RP 240
#endif

#ifndef REF_R_KORR
 #define REF_R_KORR 0
#endif
#ifndef REF_C_KORR
 #define REF_C_KORR 0
#endif

#ifdef POWER_OFF
// if POWER OFF function is selected, wait 10s
// if POWER_OFF with parameter > 2, wait only 3s before repeating
 #if (POWER_OFF+0) > 2
  #define OFF_WAIT_TIME 3000
 #else
  #define OFF_WAIT_TIME 10000
 #endif
#else
// if POWER OFF function is not selected, wait 3s before repeat measurement
 #define OFF_WAIT_TIME  3000
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

// defines for the WITH_UART option
/*
With define SWUART_INVERT you can specify, if the software-UART operates normal or invers.
In der normalen Betriebsart sendet der UART in der üblichen Logik (Low = 0; High = 1).
n the normal mode the UART sends with usual logik level (Low = 0; High = 1).
You can use this mode for direct connection to a µC, or a level converter like MAX232.

With invers mode the UART sends with invers logik (Low = 1, High = 0).
This is the level of a standard RS232 port of a PC.
In most cases the output of the software UART can so be connected to the RxD of a PC.
The specification say, that level -3V to 3V is unspecified, but in most cases it works.
Is a simple but unclean solution.

Is SWUART_INVERT defined, the UART works is inverse mode
*/
//#define SWUART_INVERT

#define TxD PC3	//TxD-Pin of Software-UART; must be at Port C !
#ifdef WITH_UART
 #define TXD_MSK (1<<TxD)
#else
 #define TXD_MSK 0
#endif

#ifdef SWUART_INVERT
  #define TXD_VAL 0
#else
  #define TXD_VAL TXD_MSK
#endif

