
/*########################################################################################
        Configuration
*/
#ifndef ADC_PORT
//#define DebugOut 3		// if set, output of voltages of resistor measurements in row 2,3,4
//#define DebugOut 4		// if set, output of voltages of Diode measurement in row 3+4
//#define DebugOut 5		// if set, output of Transistor checks in row 2+3
//#define DebugOut 10		// if set, output of capacity measurements (ReadCapacity) in row 3+4 
//#define DebugOut 11		// if set, output of load time (ReadCapacity) in row 3 for C at pins 1+3

// select the right Processor Typ
#if defined(__AVR_ATmega48__)
 #define PROCESSOR_TYP 168
#elif defined(__AVR_ATmega48P__)
 #define PROCESSOR_TYP 168
#elif defined(__AVR_ATmega88__)
 #define PROCESSOR_TYP 168
#elif defined(__AVR_ATmega88P__)
 #define PROCESSOR_TYP 168
#elif defined(__AVR_ATmega168__)
 #define PROCESSOR_TYP 168
#elif defined(__AVR_ATmega168P__)
 #define PROCESSOR_TYP 168
#elif defined(__AVR_ATmega328__)
 #define PROCESSOR_TYP 328
#elif defined(__AVR_ATmega328P__)
 #define PROCESSOR_TYP 328
#elif defined(__AVR_ATmega324__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega324P__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega644__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega644P__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega1284__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega1284P__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega640__)
 #define PROCESSOR_TYP 1280
#elif defined(__AVR_ATmega1280__)
 #define PROCESSOR_TYP 1280
#elif defined(__AVR_ATmega2560__)
 #define PROCESSOR_TYP 1280
#else
 #define PROCESSOR_TYP 8
#endif


/* Port , that is directly connected to the probes.
  This Port must have an ADC-Input  (ATmega8:  PORTC).
  The lower pins of this Port must be used for measurements.
  Please don't change the definitions of TP1, TP2 and TP3!
  The TPREF pin can be connected with a 2.5V precision voltage reference
  The TPext can be used with a 10:1 resistor divider as external voltage probe up to 50V
*/
#if PROCESSOR_TYP == 644
//################# for m324, m644, m1284 port A is the Analog input
 #define ADC_PORT PORTA
 #define ADC_DDR DDRA
 #define ADC_PIN PINA
 #define TP1 PA0
 #define TP2 PA1
 #define TP3 PA2
 // Port pin for external Voltage measurement (zener voltage extension)
 // The pin number gives the right MUX2:0 setting
 #define TPext PA3
 // Port pin for 2.5V precision reference used for VCC check (optional)
 #define TPREF PA4
 // Port pin for Battery voltage measuring
 #define TPBAT PA5
 // Port pin with >100nF capacitor for calibration 
 #define TPCAP PA7
#elif PROCESSOR_TYP == 1280
//################# for mega1280, mega2560 port F is the Analog input
 #define ADC_PORT PORTF
 #define ADC_DDR DDRF
 #define ADC_PIN PINF
 #define TP1 PF0
 #define TP2 PF1
 #define TP3 PF2
 // Port pin for external Voltage measurement (zener voltage extension)
 #define TPext PF3
 // Port pin for 2.5V precision reference used for VCC check (optional)
 #define TPREF PF4
 // Port pin for Battery voltage measuring
 #define TPBAT PF5
#else
//############### default for mega8, mega168 and mega328
 #define ADC_PORT PORTC
 #define ADC_DDR DDRC
 #define ADC_PIN PINC
 #define TP1 PC0
 #define TP2 PC1
 #define TP3 PC2
 // Port pin for external Voltage measurement (zener voltage extension) PC3
 #define TPext ((1<<MUX1) | (1<<MUX0))
 // Port pin for 2.5V precision reference used for VCC check (optional) PC4
 #define TPREF (1<<MUX2) 
 // Port pin for Battery voltage measuring PC5
 #define TPBAT ((1<<MUX2) | (1<<MUX0))
#endif

// setting for voltage devider of Batterie voltage measurement 10K and 3.3k
#ifndef BAT_NUMERATOR
 // factor corresponding to the sum of both resistors (10k + 3.3k)
 #define BAT_NUMERATOR 133
#endif
#ifndef BAT_DENOMINATOR
 // divider corresponding to the resistor at the GND side (3.3k)
 #define BAT_DENOMINATOR 33
#endif

// setting for Voltage divider of the external Voltage measurement 180k and 20k
#ifndef EXT_NUMERATOR
 // smallest factor correcponding to the sum of both resistors (180k + 20k)
 #define EXT_NUMERATOR 10
#endif
#ifndef EXT_DENOMINATOR
 // smallest divider corresponding to the resistor at the GND side (20k)
 #define EXT_DENOMINATOR 1
#endif


/* Port for the Test resistors
  The Resistors must be connected to the lower 6 Pins of the Port in following sequence:
  RLx = 680R-resistor for Test-Pin x
  RHx = 470k-resistor for Test-Pin x
  For ATmega328 you can define any pin number for every resistor in any order.
  For other processors the RHx pin must one number higher than the RLx.
  The default layout is:

  RL1 an Pin 0
  RH1 an Pin 1
  RL2 an Pin 2
  RH2 an Pin 3
  RL3 an Pin 4
  RH3 an Pin 5
*/
#if PROCESSOR_TYP == 644
//################# for m324, m664, m1284 use port D 
 #define R_DDR DDRD
 #define R_PORT PORTD

 #define PIN_RL1 PD2
 #define PIN_RL2 PD4
 #define PIN_RL3 PD6
 #define PIN_RH1 PD3
 #define PIN_RH2 PD5
 #define PIN_RH3 PD7
/* The calibration capacitor is connected to a 470k resistor to port TCAP */
 #define TCAP_DDR DDRC
 #define TCAP_PORT PORTC
 #define TCAP_RH PC6
/* FDIV_PIN specifies the pin, which switch on a 16:1 frequency divider */
 #define FDIV_DDR DDRC
 #define FDIV_PORT PORTC
 #define FDIV_PIN PC0
/* the two bits P0 and P1 at FINP port control the input of frequency measurement */
/* P1:P0 = 00 external input, P1:P0 = 10 high frequency crystal, P1:P0 = 11 low frequency crystal */
 #define FINP_DDR DDRC
 #define FINP_PORT PORTC
 #define FINP_P0 PC1
 #define FINP_P1 PC2

/* define the input pin for frequency measuring and also the pin change monitoring port for measuring the periode */
 #define FREQINP_DDR DDRB
 #define FREQINP_PIN PB0
 #define PCMSK_FREQ PCMSK1
 #define PCINT_FREQ PCINT8
#elif PROCESSOR_TYP == 1280
//################# for mega1280, mega2560 use port K 
 #define R_DDR DDRK
 #define R_PORT PORTK

 #define PIN_RL1 PK0
 #define PIN_RL2 PK2
 #define PIN_RL3 PK4
 #define PIN_RH1 PK1
 #define PIN_RH2 PK3
 #define PIN_RH3 PK5
#else
//############### default for mega8, mega168 and mega328
 #define R_DDR DDRB
 #define R_PORT PORTB

 #define PIN_RL1 PB0
 #define PIN_RL2 PB2
 #define PIN_RL3 PB4
 #define PIN_RH1 PB1
 #define PIN_RH2 PB3
 #define PIN_RH3 PB5

/* define the input pin for frequency measuring and also the pin change monitoring port for measuring the periode */
 #define FREQINP_DDR DDRD
 #define FREQINP_PIN PD4
 #define PCMSK_FREQ PCMSK2
 #define PCINT_FREQ PCINT20
#endif

#if PROCESSOR_TYP == 644
 #define ON_DDR DDRB
 #define ON_PORT PORTB
 #define ON_PIN PD1      // This Pin is switched to high to switch power on
#elif PROCESSOR_TYP == 1280
 #define ON_DDR DDRA
 #define ON_PORT PORTA
 #define ON_PIN PA6      // This Pin is switched to high to switch power on
#else
 #define ON_DDR DDRD
 #define ON_PORT PORTD
 #define ON_PIN PD6      // This Pin is switched to high to switch power on
#endif

#ifdef STRIP_GRID_BOARD
 // Strip Grid board version
 #if PROCESSOR_TYP == 644
  #define RST_PORT PORTC
  #define RST_PIN_REG PINC
  #define RST_PIN PC7     //Pin, is switched to low, if push button is pressed
 #elif PROCESSOR_TYP == 1280
  #define RST_PORT PORTA
  #define RST_PIN_REG PINA
  #define RST_PIN PA0     //Pin, is switched to low, if push button is pressed
 #else
  #define RST_PORT PORTD
  #define RST_PIN_REG PIND
  #define RST_PIN PD0     //Pin, is switched to low, if push button is pressed
 #endif
#else
 // normal layout version
 #if PROCESSOR_TYP == 644
  #define RST_PORT PORTC
  #define RST_PIN_REG PINC
  #define RST_PIN PC7     //Pin, is switched to low, if push button is pressed
 #elif PROCESSOR_TYP == 1280
  #define RST_PORT PORTA
  #define RST_PIN_REG PINA
  #define RST_PIN PA7     //Pin, is switched to low, if push button is pressed
 #else
  #define RST_PORT PORTD
  #define RST_PIN_REG PIND
  #define RST_PIN PD7     //Pin, is switched to low, if push button is pressed
 #endif
#endif


/*
       Port(s) / Pins for LCD
*/
#ifdef STRIP_GRID_BOARD
 // special Layout for strip grid board
 #if PROCESSOR_TYP == 644
  #define HW_LCD_EN_PORT         PORTB
  #define HW_LCD_EN_PIN          2
 
  #define HW_LCD_RS_PORT         PORTB
  #define HW_LCD_RS_PIN          3

  #define HW_LCD_B4_PORT         PORTB
  #define HW_LCD_B4_PIN          4
  #define HW_LCD_B5_PORT         PORTB
  #define HW_LCD_B5_PIN          5
  #define HW_LCD_B6_PORT         PORTB
  #define HW_LCD_B6_PIN          6
  #define HW_LCD_B7_PORT         PORTB
  #define HW_LCD_B7_PIN          7
 #elif PROCESSOR_TYP == 1280
  #define HW_LCD_EN_PORT         PORTA
  #define HW_LCD_EN_PIN          5
 
  #define HW_LCD_RS_PORT         PORTA
  #define HW_LCD_RS_PIN          7

  #define HW_LCD_B4_PORT         PORTA
  #define HW_LCD_B4_PIN          4
  #define HW_LCD_B5_PORT         PORTA
  #define HW_LCD_B5_PIN          3
  #define HW_LCD_B6_PORT         PORTA
  #define HW_LCD_B6_PIN          2
  #define HW_LCD_B7_PORT         PORTA
  #define HW_LCD_B7_PIN          1
 #else
  #define HW_LCD_EN_PORT         PORTD
  #define HW_LCD_EN_PIN          5
 
  #define HW_LCD_RS_PORT         PORTD
  #define HW_LCD_RS_PIN          7

  #define HW_LCD_B4_PORT         PORTD
  #define HW_LCD_B4_PIN          4
  #define HW_LCD_B5_PORT         PORTD
  #define HW_LCD_B5_PIN          3
  #define HW_LCD_B6_PORT         PORTD
  #define HW_LCD_B6_PIN          2
  #define HW_LCD_B7_PORT         PORTD
  #define HW_LCD_B7_PIN          1
 #endif
#else
 // normal Layout
 #if PROCESSOR_TYP == 644
  #define HW_LCD_EN_PORT         PORTB
  #define HW_LCD_EN_PIN          2
 
  #define HW_LCD_RS_PORT         PORTB
  #define HW_LCD_RS_PIN          3

  #define HW_LCD_B4_PORT         PORTB
  #define HW_LCD_B4_PIN          4
  #define HW_LCD_B5_PORT         PORTB
  #define HW_LCD_B5_PIN          5
  #define HW_LCD_B6_PORT         PORTB
  #define HW_LCD_B6_PIN          6
  #define HW_LCD_B7_PORT         PORTB
  #define HW_LCD_B7_PIN          7
 #elif PROCESSOR_TYP == 1280
  #define HW_LCD_EN_PORT         PORTA
  #define HW_LCD_EN_PIN          5

  #define HW_LCD_RS_PORT         PORTA
  #define HW_LCD_RS_PIN          4

  #define HW_LCD_B4_PORT         PORTA
  #define HW_LCD_B4_PIN          0
  #define HW_LCD_B5_PORT         PORTA
  #define HW_LCD_B5_PIN          1
  #define HW_LCD_B6_PORT         PORTA
  #define HW_LCD_B6_PIN          2
  #define HW_LCD_B7_PORT         PORTA
  #define HW_LCD_B7_PIN          3
 #else
  #define HW_LCD_EN_PORT         PORTD
  #define HW_LCD_EN_PIN          5

  #define HW_LCD_RS_PORT         PORTD
  #define HW_LCD_RS_PIN          4

  #define HW_LCD_B4_PORT         PORTD
  #define HW_LCD_B4_PIN          0
  #define HW_LCD_B5_PORT         PORTD
  #define HW_LCD_B5_PIN          1
  #define HW_LCD_B6_PORT         PORTD
  #define HW_LCD_B6_PIN          2
  #define HW_LCD_B7_PORT         PORTD
  #define HW_LCD_B7_PIN          3
 #endif
#endif



// U_VCC defines the VCC Voltage of the ATmega in mV units

#define U_VCC 5000
// integer factors are used to change the ADC-value to mV resolution in ReadADC !

// With the option NO_CAP_HOLD_TIME you specify, that capacitor loaded with 680 Ohm resistor will not
// be tested to hold the voltage same time as load time.
// Otherwise (without this option) the voltage drop during load time is compensated to avoid displaying
// too much capacity for capacitors with internal parallel resistance.
// #define NO_CAP_HOLD_TIME


// U_SCALE can be set to 4 for better resolution of ReadADC function for resistor measurement
#define U_SCALE 4

// R_ANZ_MESS can be set to a higher number of measurements (up to 200) for resistor measurement
#define R_ANZ_MESS 190

//Watchdog
#define WDT_enabled
/* If you remove the "#define WDT_enabled" , the Watchdog will not be activated.
  This is only for Test or debugging usefull.
 For normal operation please activate the Watchdog !
*/



// RH_OFFSET : systematic offset of resistor measurement with RH (470k) 
// resolution is 0.1 Ohm, 3500 defines a offset of 350 Ohm
#define RH_OFFSET 3500 

// TP2_CAP_OFFSET is a additionally offset for TP2 capacity measurements in pF units
#define TP2_CAP_OFFSET 2

// CABLE_CAP defines the capacity (pF) of 12cm cable with clip at the terminal pins
#define CABLE_CAP 3


#define LONG_WAIT_TIME 28000
#define SHORT_WAIT_TIME 5000

//**********************************************************

// defines for the WITH_UART option
/*
With define SWUART_INVERT you can specify, if the software-UART operates normal or invers.
in the normal mode the UART sends with usual logik level (Low = 0; High = 1).
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
 #ifdef SWUART_INVERT
  #define TXD_VAL 0
 #else
  #define TXD_VAL TXD_MSK
 #endif
#else
 // without UART
 // If you use any pin of port C for output, you should define all used pins in TXD_MSK.
 // With TXD_VAL you can specify the default level of output pins for all port C output pins.
 #define TXD_MSK 0
 #define TXD_VAL 0
#endif

 //define a default zero value for ESR measurement (0.01 Ohm units)
// #define ESR_ZERO 20

// all capacity measurement results does not go C_LIMIT_TO_UNCALIBRATED pF below the calibrated zero capacity.
#define C_LIMIT_TO_UNCALIBRATED 20

// all ESR measurements results does not go R_LIMIT_TO_UNCALIBRATED 0.01 Ohm units below the calibrated zero resistance.
#define R_LIMIT_TO_UNCALIBRATED 20   

/*########################################################################################
End of configuration 
*/
#include "autoconf.h"
#endif
