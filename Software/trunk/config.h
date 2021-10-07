/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Transistor Tester
 *  
 *  File:       fonfig.h
 *  Funktion:   Configuration of Port addresses ...
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-10  Karl      
 *              2021-01-10  Karl 
*
 * ************************************************************************ */

#ifndef ADC_PORT
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 7735) || (LCD_ST_TYPE == 9163) || (LCD_ST_TYPE == 9341) || (LCD_ST_TYPE == 1327))
  #define LCD_GRAPHIC_TYPE 1
#elif (LCD_ST_TYPE == 7920)
  #define LCD_GRAPHIC_TYPE 2
#else
  #define LCD_GRAPHIC_TYPE 0
#endif
#include "part_defs.h"
#include "lcd_defines.h"
//#define DebugOut 3		// if set, output of voltages of resistor measurements in row 2,3,4
//#define DebugOut 4		// if set, output of voltages of Diode measurement in row 3+4
//#define DebugOut 5		// if set, output of Transistor checks in row 2+3
//#define DebugOut 10		// if set, output of capacity measurements (ReadCapacity) in row 3+4 
//#define DebugOut 11		// if set, output of load time (ReadCapacity) in row 3 for C at pins 1+3

// defines for LCD_INTERFACE_MODE
#define MODE_PARALLEL 1		/* default 4-Bit parallel mode for character LCD */
				/* EN + RS + B4 + B5 + B6 + B7 */
#define MODE_I2C 2		/* I2C interface for SSD1306 */
				/* SCL + SDA */
#define MODE_I2C_CHAR 22	/* Special I2C converter with PCF8574 chip for character LCDs */
				/* default I2C address is 27 */
#define MODE_3LINE 3		/* special 3 line serial output with Data/Command as first data bit  PCF8814 */
				/* RES + EN + B0 (+ CE) */
#define MODE_SPI 4		/* 4 bit SPI interface for ST7565 or SSD1306 */
				/* RES + EN + RS + B0 (+ CE) */
#define MODE_7920_SERIAL 5		/* serial interface for ST7920 */
				/* EN + B0 (+ RESET) */
#define MODE_7108_SERIAL 6		/* serial interface for ST7108 with shift register 74HC164 */
				/* EN + [RS B0] + CS1 + CS2 + CLK (+ PCLK) */
#define MODE_1803_SERIAL 7		/* serial interface for SSD1803 */
				/* EN + B0 (+ RESET) */

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
#elif defined(__AVR_ATmega324PA__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega644__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega644P__)
 #define PROCESSOR_TYP 644
#elif defined(__AVR_ATmega644PA__)
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
#if (LCD_ST_TYPE == 0) && (LCD_INTERFACE_MODE == MODE_I2C)
 /* set the interface mode to the special PCF8574 I2C interface type */
 #undef LCD_INTERFACE_MODE
 #define LCD_INTERFACE_MODE  MODE_I2C_CHAR
#endif

/* ************************************************************************* */
/* Definition for the Port , that is directly connected to the probes.	     */
/*
  This Port must have an ADC-Input  (ATmega8:  PORTC).
  The lower pins of this Port must be used for measurements.
  Please don't change the definitions of TP1, TP2 and TP3!
  The TPREF pin can be connected with a 2.5V precision voltage reference
  The TPext can be used with a 10:1 resistor divider as external voltage probe up to 50V
*/
/* ************************************************************************* */
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
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
 #define TPext PA4		/* T7 */
 #else
 #define TPext PA3
 #endif

 // Port pin for 2.5V precision reference used for VCC check (optional)
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
 #define TPREF PA3		/* T7 */
 #define TPRELAY PA3		/* T7 */
 #else
 #define TPREF PA4
 #define TPRELAY PA4
 #endif
 // Port pin for Battery voltage measuring
 #define TPBAT PA5
 // Port pin with >100nF capacitor for calibration, -1 for none
 #define TPCAP PA7
 // you must set WITH_VEXT, if you wish external voltage reading
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
 #define TPRELAY PF4
 // Port pin for Battery voltage measuring
 #define TPBAT PF5
 #define TPCAP -1
#else	/* PROCESSOR_TYP */
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
 #define TPRELAY PC4
 // Port pin for Battery voltage measuring PC5
 #define TPBAT PC5		/* identical to ((1<<MUX2) | (1<<MUX0)) */
 // option WITH_VEXT can be set automatically, if WITH_UART is unset
 #define TPCAP -1
#endif	/* PROCESSOR_TYP */

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

/* ************************************************************************* */
/* Port for the Test resistors                                               */
/* ************************************************************************* */
/*
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
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
  #define R_DDR DDRC		/* T7 */
  #define R_PORT PORTC		/* T7 */
 
  #define PIN_RL1 PC0		/* T7 */
  #define PIN_RL2 PC2		/* T7 */
  #define PIN_RL3 PC4		/* T7 */
  #define PIN_RH1 PC1		/* T7 */
  #define PIN_RH2 PC3		/* T7 */
  #define PIN_RH3 PC5		/* T7 */
 #else
  #define R_DDR DDRD		/* m644 */
  #define R_PORT PORTD		/* m644 */
 
  #define PIN_RL1 PD2		/* m644 */
  #define PIN_RL2 PD4		/* m644 */
  #define PIN_RL3 PD6		/* m644 */
  #define PIN_RH1 PD3		/* m644 */
  #define PIN_RH2 PD5		/* m644 */
  #define PIN_RH3 PD7		/* m644 */
 #endif
/* The calibration capacitor is connected to a 470k resistor to port TCAP */
 #define TCAP_DDR DDRC
 #define TCAP_PORT PORTC
 #define TCAP_RH PC6

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
#else		/* PROCESSOR_TYP */
//############### default for mega8, mega168 and mega328
 #define R_DDR DDRB
 #define R_PORT PORTB

 #define PIN_RL1 PB0
 #define PIN_RL2 PB2
 #define PIN_RL3 PB4
 #define PIN_RH1 PB1
 #define PIN_RH2 PB3
 #define PIN_RH3 PB5

#endif		/* PROCESSOR_TYP */


/* ************************************************************************* */
/* specify special pins:						     */
/* - input pin used for frequency measurement and corresponding interrupt    */
/* - two input pins used for incremental encoder (rotary switch)             */
/* ************************************************************************* */
#if PROCESSOR_TYP == 644
/* ------------------------------------------------------------------------- */
/* define the input pin for frequency measuring and also the pin change monitoring port for measuring the periode */
 #define FREQINP_DDR DDRB
 #define FREQINP_PIN PB0
 #define PCMSK_FREQ PCMSK1
 #define PCINT_FREQ PCINT8
/* define both input pins for rotaty encoder */
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
  /* default to PD5 and PD6, but still able to redefine */
  #ifndef ROTARY_1_DDR
   #define ROTARY_1_DDR DDRD	/* T7 */
   #define ROTARY_1_REG PIND	/* T7 */
  #endif
  #ifndef ROTARY_2_DDR
   #define ROTARY_2_DDR DDRD	/* T7 */
   #define ROTARY_2_REG PIND	/* T7 */
  #endif
  #ifndef ROTARY_1_PIN
   #define ROTARY_1_PIN PD6	/* T7 */
  #endif
  #ifndef ROTARY_2_PIN
   #define ROTARY_2_PIN PD5	/* T7 */
  #endif
 #else
  #ifndef ROTARY_1_DDR
   #define ROTARY_1_DDR DDRB
   #define ROTARY_1_REG PINB
  #endif
  #ifndef ROTARY_2_DDR
   #define ROTARY_2_DDR DDRB
   #define ROTARY_2_REG PINB
  #endif
  #ifndef ROTARY_1_PIN
   // default connection is PB7
   #define ROTARY_1_PIN PB7	/* m644 */
  #endif
  #ifndef ROTARY_2_PIN
   // PB5 is connected to character LCD D5 or ST7565 RS
   #define ROTARY_2_PIN PB5	/* m644 */
   // can be preset to PB6 for character display and PB3 for ST7565 graphic controller
  #endif
 #endif

/* FDIV_PIN specifies the output pin, which switch on a 16:1 frequency divider */
 #define FDIV_DDR DDRC
 #define FDIV_PORT PORTC
 #define FDIV_PIN PC0
/* the two bits P0 and P1 at FINP port control the input of frequency measurement */
/* P1:P0 = 00 external input, P1:P0 = 10 high frequency crystal, P1:P0 = 11 low frequency crystal */
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
 #define FINP_DDR DDRB		/* T7 */
 #define FINP_PORT PORTB	/* T7 */
 #else
 #define FINP_DDR DDRC
 #define FINP_PORT PORTC
 #endif
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
  #if ((STRIP_GRID_BOARD & 0x10) == 0x10)
  // 00 = external, 10 = HF Xtal, 11 = LF Xtal
  #define FINP_P0 PB2		/* T7, Y1 port of HC4052 is selected with 10 */
  #define FINP_P1 PB1		/* T7 */
  #else
  #define FINP_P0 PB1		/* T7, Y2 port of HC4052 is selected with 10 */
  #define FINP_P1 PB2		/* T7 */
  #endif
 #else
  #if ((STRIP_GRID_BOARD & 0x10) == 0x10)
  // 00 = external, 10 = HF Xtal, 11 = LF Xtal
  #define FINP_P0 PC2		/* Y1 port of HC4052 is selected with 10 */
  #define FINP_P1 PC1
  #else
  #define FINP_P0 PC1		/* Y2 port of HC4052 is selected with 10 */
  #define FINP_P1 PC2
  #endif
 #endif
#elif PROCESSOR_TYP == 1280
/* ------------------------------------------------------------------------- */
/* define the input pin for frequency measuring and also the pin change monitoring port for measuring the periode */
 // the PE6 pin is timer input of T3 and INT6 interrupt pin
 #define FREQINP_DDR DDRE
 #define FREQINP_PIN PE6
 #define PCMSK_FREQ EIMSK
 #define PCINT_FREQ INT6
/* define both input pins for rotaty encoder */
 #ifndef ROTARY_1_DDR
  #define ROTARY_1_DDR DDRA
  #define ROTARY_1_REG PINA
 #endif
 #ifndef ROTARY_2_DDR
  #define ROTARY_2_DDR DDRA
  #define ROTARY_2_REG PINA
 #endif
 #ifndef ROTARY_1_PIN
  // default connection is PA3
  #define ROTARY_1_PIN PA3
 #endif
 #ifndef ROTARY_2_PIN
  // PB5 is connected to character LCD D5 or ST7565 RS
  #define ROTARY_2_PIN PA1
  // can be preset to PB6 for character display and PB3 for ST7565 graphic controller
 #endif
#else		/* PROCESSOR_TYP, must be ATmega8|168|328 */
/* ------------------------------------------------------------------------- */
/* define the input pin for frequency measuring and also the pin change monitoring port for measuring the periode */
 #define FREQINP_DDR DDRD
 #define FREQINP_PIN PD4
 #define PCMSK_FREQ PCMSK2
 #define PCINT_FREQ PCINT20
/* define both input pins for rotaty encoder */
 #ifndef ROTARY_1_DDR
  #define ROTARY_1_DDR DDRD
  #define ROTARY_1_REG PIND
 #endif
 #ifndef ROTARY_2_DDR
  #define ROTARY_2_DDR DDRD
  #define ROTARY_2_REG PIND
 #endif
 #ifndef ROTARY_1_PIN
  // default connection is PD3
  #define ROTARY_1_PIN PD3
 #endif
 #ifndef ROTARY_2_PIN
  // PD1 is connected to character LCD D5/D7 or ST7565 RS
  #define ROTARY_2_PIN PD1
  // can be preset to PD2 for character display and PD5 for ST7565 graphic controller
 #endif
#endif		/* PROCESSOR_TYP */

#if CHANGE_ROTARY_DIRECTION
 #define ROTARY_B_PIN ROTARY_1_PIN	/* is connected to LCD-D7/D5 or ST7565-B0 */
 #define ROTARY_B_DDR ROTARY_1_DDR
 #define ROTARY_B_REG ROTARY_1_REG
 #define ROTARY_A_PIN ROTARY_2_PIN
 #define ROTARY_A_DDR ROTARY_2_DDR
 #define ROTARY_A_REG ROTARY_2_REG
#else		/* CHANGE_ROTARY_DIRECTION */
 #define ROTARY_A_PIN ROTARY_1_PIN	/* is connected to LCD-D7/D5 or ST7565-B0 */
 #define ROTARY_A_DDR ROTARY_1_DDR
 #define ROTARY_A_REG ROTARY_1_REG
 #define ROTARY_B_PIN ROTARY_2_PIN
 #define ROTARY_B_DDR ROTARY_2_DDR
 #define ROTARY_B_REG ROTARY_2_REG
#endif		/* CHANGE_ROTARY_DIRECTION */


/* ************************************************************************* */
/* define the output pin for switch the power on/off                         */
/* ************************************************************************* */
#if PROCESSOR_TYP == 644
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
 #define ON_DDR DDRD		/* T7 */
 #define ON_PORT PORTD		/* T7 */
 #define ON_PIN PD2      // T7, This Pin is switched to high to switch power on
 #else
 #define ON_DDR DDRB
 #define ON_PORT PORTB
 #define ON_PIN PB1      // This Pin is switched to high to switch power on
 #endif
#elif PROCESSOR_TYP == 1280
 #define ON_DDR DDRA
 #define ON_PORT PORTA
 #define ON_PIN PA6      // This Pin is switched to high to switch power on
#else		/* PROCESSOR_TYP, must be ATmega8|168|328 */
 #if STRIP_GRID_BOARD == 3
  // special layout for Arduino Nano, dummy entry
  #define ON_DDR DDRB
  #define ON_PORT PORTB
  #define ON_PIN 7     // non existing pin
 #else
  #define ON_DDR DDRD
  #define ON_PORT PORTD
  #define ON_PIN PD6      // This Pin is switched to high to switch power on
 #endif
#endif		/* PROCESSOR_TYP */

/* ************************************************************************* */
/* define the pin for push button  (low value, if pressed)                   */
/* ************************************************************************* */
#if PROCESSOR_TYP == 644
 #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
  #define RST_PORT PORTD		/* T7 */
  #define RST_PIN_REG PIND		/* T7 */
  #define RST_PIN PD1     // T7, Pin, is switched to low, if push button is pressed
 #else
  #define RST_PORT PORTC
  #define RST_PIN_REG PINC
  #define RST_PIN PC7     // Pin, is switched to low, if push button is pressed
 #endif
#elif PROCESSOR_TYP == 1280
 #if STRIP_GRID_BOARD == 1
  #define RST_PORT PORTA
  #define RST_PIN_REG PINA
  #define RST_PIN PA0     // Pin, is switched to low, if push button is pressed
 #else
  #define RST_PORT PORTA
  #define RST_PIN_REG PINA
  #define RST_PIN PA7     // Pin, is switched to low, if push button is pressed
 #endif
#else		/* PROCESSOR_TYP, must be ATmega8|168|328 */
  /* Processor mega8/168/328 , two board layouts defined */
 #if STRIP_GRID_BOARD == 1
 // Strip Grid board version
  #define RST_PORT PORTD
  #define RST_PIN_REG PIND
  #if (LCD_GRAPHIC_TYPE != 0)
   #define RST_PIN PD7     //Pin, is switched to low, if push button is pressed
   // option STRIP_GRID is used to specify other connection of the LCD (chinese version),
   // the RST_PIN remain at PD7.
  #else
   #define RST_PIN PD0     //Pin, is switched to low, if push button is pressed
  #endif
 #elif STRIP_GRID_BOARD == 3
 // special version for a Arduino Nano Layout
  #define RST_PORT PORTC
  #define RST_PIN_REG PINC
  #define RST_PIN PC3     //Pin, is switched to low, if push button is pressed
 #else		/* no STRIP_GRID_BOARD */
 // normal layout version
  #define RST_PORT PORTD
  #define RST_PIN_REG PIND
  #define RST_PIN PD7     //Pin, is switched to low, if push button is pressed
 #endif		/* STRIP_GRID_BOARD */
#endif		/* PROCESSOR_TYP */


/* ************************************************************************* */
/* Port(s) / Pins for LCD						     */
/* ************************************************************************* */

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 7735) || (LCD_ST_TYPE == 9163) || (LCD_ST_TYPE == 9341) || (LCD_ST_TYPE == 1327))
 /* set the default INTERFACE_MODE for the ST7565 controller */
 #ifndef LCD_INTERFACE_MODE
  #define LCD_INTERFACE_MODE MODE_SPI
 #endif
#elif (LCD_ST_TYPE == 7920)
 #ifndef LCD_INTERFACE_MODE
//  #define LCD_INTERFACE_MODE MODE_7920_SERIAL
  #define LCD_INTERFACE_MODE MODE_PARALLEL
 #endif
  #define SLOW_LCD
#elif (LCD_ST_TYPE == 7108)
 #ifndef LCD_INTERFACE_MODE
  #define LCD_INTERFACE_MODE MODE_7108_SERIAL
 #endif
#else
 // default interface  with character LCD
 #ifndef LCD_INTERFACE_MODE
  #define LCD_INTERFACE_MODE MODE_PARALLEL
 #endif
#endif

/* ----------------------------------------------------------------------------- */
#if (LCD_INTERFACE_MODE == MODE_SPI) || (LCD_INTERFACE_MODE == MODE_3LINE)
 // SPI-mode is used for the 128x64 pixel graphics LCD with ST7565 controller
 // LCD-P/S = low, LCD-CS1 = low, LCD-CS2 = high
 // LCD_B0_xxx=SI, LCD_EN_xxx=SCL, LCD_RS_xxx=A0, LCD_RES_xxx=RST, (CS-GND)
  /* additional Chip Enable CE is defined */
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* mega324/644/1284 with SPI */
   /* the ST7565 Reset signal */
   #define HW_LCD_RES_DDR         DDRB
   #define HW_LCD_RES_PORT        PORTB
   #define HW_LCD_RES_PIN         4

   /* EN is the serial clock signal SCL */
   #define HW_LCD_EN_DDR          DDRB
   #define HW_LCD_EN_PORT         PORTB
  #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
   #define HW_LCD_EN_PIN          7		/* T7 */
  #else
   #define HW_LCD_EN_PIN          6		/* m644 */
  #endif

   /* the data/instruction signal RS */
   #define HW_LCD_RS_DDR          DDRB
   #define HW_LCD_RS_PORT         PORTB
   #define HW_LCD_RS_PIN          5

   /* the data signal SI */
   #define HW_LCD_B0_DDR          DDRB
   #define HW_LCD_B0_PORT         PORTB
  #if (STRIP_GRID_BOARD & 0x0f) == 7    /* m644 */
   #define HW_LCD_B0_PIN          6		/* T7 */
  #else
   #define HW_LCD_B0_PIN          7		/* m644 */
  #endif

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRB
   #define HW_LCD_CE_PORT         PORTB
   #define HW_LCD_CE_PIN          3
				/* --------------------------------------------- */
 #elif PROCESSOR_TYP == 1280	/* mega1280/2560 with SPI */
  #ifdef STRIP_GRID_BOARD
   /* the ST7565 Reset signal */
   #define HW_LCD_RES_DDR         DDRA
   #define HW_LCD_RES_PORT        PORTA
   #define HW_LCD_RES_PIN         4

   /* EN is the serial clock signal SCL */
   #define HW_LCD_EN_DDR          DDRA
   #define HW_LCD_EN_PORT         PORTA
   #define HW_LCD_EN_PIN          2

   /* the data/instruction signal RS */
   #define HW_LCD_RS_DDR          DDRA
   #define HW_LCD_RS_PORT         PORTA
   #define HW_LCD_RS_PIN          3

   /* the data signal SI */
   #define HW_LCD_B0_DDR          DDRA
   #define HW_LCD_B0_PORT         PORTA
   #define HW_LCD_B0_PIN          1

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRA
   #define HW_LCD_CE_PORT         PORTA
   #define HW_LCD_CE_PIN          5
  #else		/* no STRIP_GRID_BOARD */
   /* the ST7565 Reset signal */
   #define HW_LCD_RES_DDR         DDRA
   #define HW_LCD_RES_PORT        PORTA
   #define HW_LCD_RES_PIN         0

   /* EN is the serial clock signal SCL */
   #define HW_LCD_EN_DDR          DDRA
   #define HW_LCD_EN_PORT         PORTA
   #define HW_LCD_EN_PIN          2

   /* the data/instruction signal RS */
   #define HW_LCD_RS_DDR          DDRA
   #define HW_LCD_RS_PORT         PORTA
   #define HW_LCD_RS_PIN          1

   /* the data signal SI */
   #define HW_LCD_B0_DDR          DDRA
   #define HW_LCD_B0_PORT         PORTA
   #define HW_LCD_B0_PIN          3

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRA
   #define HW_LCD_CE_PORT         PORTA
   #define HW_LCD_CE_PIN          4
  #endif
				/* --------------------------------------------- */
 #else				/* mega8/168/328 with SPI  */
  /* The SPI interface uses four signals  RES, EN, RS and B0 */
  /* additional Chip Enable CE is defined */
  #ifdef STRIP_GRID_BOARD
   #if STRIP_GRID_BOARD == 2
   // alternative connection of graphical LCD for the chinese T4 board with PD0 <--> LCD-REST patch
   /* the Reset Pin, 0 = Reset */
   #define HW_LCD_RES_DDR         DDRD
   #define HW_LCD_RES_PORT        PORTD
   #define HW_LCD_RES_PIN         0

   /* serial clock input  (SCL) */
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_PIN          2

   /* command / data switch  0=command 1=data */
   #define HW_LCD_RS_DDR          DDRD
   #define HW_LCD_RS_PORT         PORTD
   #define HW_LCD_RS_PIN          3

   /* serial data input SI | SDA */
   #define HW_LCD_B0_DDR          DDRD
   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_PIN          1

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRD
   #define HW_LCD_CE_PORT         PORTD
   #define HW_LCD_CE_PIN          5

   #elif STRIP_GRID_BOARD == 4
   // alternative connection of graphical LCD for the chinese KKmoon board
   /* the Reset Pin, 0 = Reset */
   #define HW_LCD_RES_DDR         DDRD
   #define HW_LCD_RES_PORT        PORTD
   #define HW_LCD_RES_PIN         3

   /* serial clock input  (SCL) */
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_PIN          0

   /* command / data switch  0=command 1=data */
   #define HW_LCD_RS_DDR          DDRD
   #define HW_LCD_RS_PORT         PORTD
   #define HW_LCD_RS_PIN          2

   /* serial data input SI | SDA */
   #define HW_LCD_B0_DDR          DDRD
   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_PIN          1

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRD
   #define HW_LCD_CE_PORT         PORTD
   #define HW_LCD_CE_PIN          5

   #elif STRIP_GRID_BOARD == 5
   // alternative connection of graphical LCD for the chinese T5 board
   /* the Reset Pin, 0 = Reset */
   #define HW_LCD_RES_DDR         DDRD
   #define HW_LCD_RES_PORT        PORTD
   #define HW_LCD_RES_PIN         2

   /* serial clock input  (SCL) */
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_PIN          3

   /* command / data switch  0=command 1=data */
   #define HW_LCD_RS_DDR          DDRD
   #define HW_LCD_RS_PORT         PORTD
   #define HW_LCD_RS_PIN          1

   /* serial data input SI | SDA */
   #define HW_LCD_B0_DDR          DDRD
   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_PIN          4

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRD
   #define HW_LCD_CE_PORT         PORTD
   #define HW_LCD_CE_PIN          5
   #else
   // alternative connection of graphical LCD 
   /* the Reset Pin, 0 = Reset */
   #define HW_LCD_RES_DDR         DDRD
   #define HW_LCD_RES_PORT        PORTD
   #define HW_LCD_RES_PIN         4

   /* serial clock input  (SCL) */
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_PIN          2

   /* command / data switch  0=command 1=data */
   #define HW_LCD_RS_DDR          DDRD
   #define HW_LCD_RS_PORT         PORTD
   #define HW_LCD_RS_PIN          3

   /* serial data input SI | SDA */
   #define HW_LCD_B0_DDR          DDRD
   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_PIN          1

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRD
   #define HW_LCD_CE_PORT         PORTD
   #define HW_LCD_CE_PIN          5
   #endif
  #else		/* no STRIP_GRID_BOARD */
   // the default connection of LCD for chinese version from Fish8840, weiweitm
   /* the Reset Pin, 0 = Reset */
   #define HW_LCD_RES_DDR         DDRD
   #define HW_LCD_RES_PORT        PORTD
   #define HW_LCD_RES_PIN         0

   /* serial clock input  (SCL) */
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_PIN          2

   /* command / data switch  0=command 1=data */
   #define HW_LCD_RS_DDR          DDRD
   #define HW_LCD_RS_PORT         PORTD
   #define HW_LCD_RS_PIN          1

   /* serial data input SI | SDA */
   #define HW_LCD_B0_DDR          DDRD
   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_PIN          3

   /* Chip Enable input */
   #define HW_LCD_CE_DDR          DDRD
   #define HW_LCD_CE_PORT         PORTD
   #define HW_LCD_CE_PIN          5
  #endif
 #endif	/* PROCESSOR_TYP for SPI Interface*/
 
/* ----------------------------------------------------------------------------- */
#elif (LCD_INTERFACE_MODE == MODE_I2C) || (LCD_INTERFACE_MODE == MODE_I2C_CHAR)
 /* only two wires are required for the I2C interface: SCL and SDA */
 #ifndef LCD_I2C_ADDR
  #if LCD_INTERFACE_MODE == MODE_I2C_CHAR
   #define LCD_I2C_ADDR 0x27	/* PCF8574 controller defines 0x27 as default address */
  #else
   #define LCD_I2C_ADDR 0x3C	/* SSD1306 controller defines 0x3c or 0x3d (SA0=1) as address */
  #endif
 #endif
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* mega324/644/1284 with I2C interface */
   #define HW_LCD_SCL_PORT         PORTB
   #define HW_LCD_SCL_PIN          3

   #define HW_LCD_SDA_PORT         PORTB
   #define HW_LCD_SDA_PIN          4
				/* --------------------------------------------- */
 #elif PROCESSOR_TYP == 1280	/* mega1280/2560 with I2C interface */
   #define HW_LCD_SCL_PORT         PORTA
   #define HW_LCD_SCL_PIN          5

   #define HW_LCD_SDA_PORT         PORTA
   #define HW_LCD_SDA_PIN          4
				/* --------------------------------------------- */
 #else				/* mega8/168/328 with I2C interface  */
   #define HW_LCD_SCL_PORT         PORTD
   #define HW_LCD_SCL_PIN          5

   #define HW_LCD_SDA_PORT         PORTD
   #define HW_LCD_SDA_PIN          2
 #endif	/* PROCESSOR_TYP for the I2C Interface */
/* ----------------------------------------------------------------------------- */
#elif (LCD_INTERFACE_MODE == MODE_7920_SERIAL) || (LCD_INTERFACE_MODE == MODE_1803_SERIAL)
 /* only two wires are required for the serial interface of the ST7920: RW and E */
 /* PSB must be connected to GND and RS(CS) and RESET must be connected to 1. */
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* mega324/644/1284 with serial ST7920 interface */
  #if defined(STRIP_GRID_BOARD) && ((STRIP_GRID_BOARD & 0xef) != 0)
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_DDR          DDRB
   #define HW_LCD_EN_PIN          6

   #define HW_LCD_B0_PORT         PORTB
   #define HW_LCD_B0_DDR          DDRB
   #define HW_LCD_B0_PIN          7

   #define HW_LCD_RESET_PORT      PORTB
   #define HW_LCD_RESET_DDR       DDRB
   #define HW_LCD_RESET_PIN       4
  #else
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_DDR          DDRB
   #define HW_LCD_EN_PIN          3

   #define HW_LCD_B0_PORT         PORTB
   #define HW_LCD_B0_DDR          DDRB
   #define HW_LCD_B0_PIN          4

   #define HW_LCD_RESET_PORT      PORTB
   #define HW_LCD_RESET_DDR       DDRB
   #define HW_LCD_RESET_PIN       2
  #endif
				/* --------------------------------------------- */
 #elif PROCESSOR_TYP == 1280	/* mega1280/2560 with serial ST7920 interface */
   #define HW_LCD_EN_PORT         PORTA
   #define HW_LCD_EN_DDR          DDRA
   #define HW_LCD_EN_PIN          5

   #define HW_LCD_B0_PORT         PORTA
   #define HW_LCD_B0_DDR          DDRA
   #define HW_LCD_B0_PIN          4

   #define HW_LCD_RESET_PORT      PORTA
   #define HW_LCD_RESET_DDR       DDRA
   #define HW_LCD_RESET_PIN       0
				/* --------------------------------------------- */
 #else				/* mega8/168/328 with serial ST7920 interface  */
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PIN          5

   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_DDR          DDRD
   #define HW_LCD_B0_PIN          2

   #define HW_LCD_RESET_PORT      PORTD
   #define HW_LCD_RESET_DDR       DDRD
   #define HW_LCD_RESET_PIN       0
 #endif	/* PROCESSOR_TYP for the serial ST7920 Interface */
/* ----------------------------------------------------------------------------- */
#elif (LCD_INTERFACE_MODE == MODE_7108_SERIAL)
 /* EN_PIN is connected to LCD-E  */
 /* RS_PIN is connected to LCD-RS and serial data input of the '164 shift register */
 /* CLK_PIN is connected to the clock input of the '164 shift register */
 /* CS1_PIN is connected to the LCD-CS1  (chip select of controller 1, row 0-63) */
 /* CS2_PIN is connected to the LCD-CS2  (chip select of controller 2, row 64-127) */
 /* PCK_PIN can be connected to the 74HCT595 parallel clock */
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* mega324/644/1284 with serial ST7108 interface */
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_DDR          DDRB
   #define HW_LCD_EN_PIN          3

   #define HW_LCD_RS_PORT         PORTB
   #define HW_LCD_RS_PIN          2

   #define HW_LCD_B0_PORT         PORTB
   #define HW_LCD_B0_PIN          2
 
   #define HW_LCD_CS1_PORT         PORTB
   #define HW_LCD_CS1_DDR          DDRB
   #define HW_LCD_CS1_PIN          7

   #define HW_LCD_CS2_PORT         PORTB
   #define HW_LCD_CS2_DDR          DDRB
   #define HW_LCD_CS2_PIN          5

   #define HW_LCD_CLK_PORT         PORTB
   #define HW_LCD_CLK_DDR          DDRB
   #define HW_LCD_CLK_PIN          6

   #define HW_LCD_PCLK_PORT        PORTB
   #define HW_LCD_PCLK_DDR         DDRB
   #define HW_LCD_PCLK_PIN         4

				/* --------------------------------------------- */
 #elif PROCESSOR_TYP == 1280	/* mega1280/2560 with serial ST7108 interface */
   #define HW_LCD_EN_PORT         PORTA
   #define HW_LCD_EN_DDR          DDRA
   #define HW_LCD_EN_PIN          5

   #define HW_LCD_RS_PORT         PORTA
   #define HW_LCD_RS_PIN          4

   #define HW_LCD_B0_PORT         PORTA
   #define HW_LCD_B0_PIN          4
 
   #define HW_LCD_CS1_PORT         PORTA
   #define HW_LCD_CS1_DDR          DDRA
   #define HW_LCD_CS1_PIN          3

   #define HW_LCD_CS2_PORT         PORTA
   #define HW_LCD_CS2_DDR          DDRA
   #define HW_LCD_CS2_PIN          1

   #define HW_LCD_CLK_PORT         PORTA
   #define HW_LCD_CLK_DDR          DDRA
   #define HW_LCD_CLK_PIN          2

   #define HW_LCD_PCLK_PORT        PORTA
   #define HW_LCD_PCLK_DDR         DDRA
   #define HW_LCD_PCLK_PIN         0

				/* --------------------------------------------- */
 #else				/* mega8/168/328 with serial ST7108 interface  */
   #define HW_LCD_EN_PORT         PORTD
   #define HW_LCD_EN_DDR          DDRD
   #define HW_LCD_EN_PIN          5

   #define HW_LCD_RS_PORT         PORTD
   #define HW_LCD_RS_PIN          4

   #define HW_LCD_B0_PORT         PORTD
   #define HW_LCD_B0_PIN          4
 
   #define HW_LCD_CS1_PORT         PORTD
   #define HW_LCD_CS1_DDR          DDRD
   #define HW_LCD_CS1_PIN          3

   #define HW_LCD_CS2_PORT         PORTD
   #define HW_LCD_CS2_DDR          DDRD
   #define HW_LCD_CS2_PIN          1

   #define HW_LCD_CLK_PORT         PORTD
   #define HW_LCD_CLK_DDR          DDRD
   #define HW_LCD_CLK_PIN          2

   #define HW_LCD_PCLK_PORT        PORTD
   #define HW_LCD_PCLK_DDR         DDRD
   #define HW_LCD_PCLK_PIN         0

 #endif	/* PROCESSOR_TYP for the serial ST7108 Interface */
/* ----------------------------------------------------------------------------- */
#else /* (LCD_INTERFACE_MODE != MODE_SPI || MODE_I2C || MODE_I2C_CHAR || MODE_ST7920_SERIAL || MODE_ST7108_SERIAL) */
 /* The 4-bit parallel Interface is usually used for the character display */
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* connection for 4-bit parallel interface and mega324/644/1284 */
  #if defined(STRIP_GRID_BOARD) && ((STRIP_GRID_BOARD & 0xef) != 0)
 // special Layout for strip grid board with the 4-bit parallel interface and ATmega324/644/1284
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_PIN          2
 
   #define HW_LCD_RS_PORT         PORTB
   #define HW_LCD_RS_PIN          3

  #else
 // normal connection for the 4-bit parallel interface and ATmega324/644/1284
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_PIN          3
 
   #define HW_LCD_RS_PORT         PORTB
   #define HW_LCD_RS_PIN          2
  #endif

   #define HW_LCD_B4_PORT         PORTB
   #define HW_LCD_B4_PIN          4
   #define HW_LCD_B5_PORT         PORTB
   #define HW_LCD_B5_PIN          5
   #define HW_LCD_B6_PORT         PORTB
   #define HW_LCD_B6_PIN          6
   #define HW_LCD_B7_PORT         PORTB
   #define HW_LCD_B7_PIN          7
				/* --------------------------------------------- */
 #elif PROCESSOR_TYP == 1280	/* normal layout with 4-bit parallel interface and mega1280/2560 */
  #ifdef STRIP_GRID_BOARD
   // special connection for strip grid board with 4-bit parallel interface and ATmega1280
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
  #else 	/* no STRIP_GRID_BOARD  with 4-bit parallel interface and ATmega1280 */
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
  #endif	/* STRIP_GRID_BOARD */
				/* --------------------------------------------- */
 #else				/* PROCESSOR_TYP  ATmega8/168/328 with 4-bit parallel interface*/
  #ifdef STRIP_GRID_BOARD
   #if STRIP_GRID_BOARD == 3
  		/* special layout for Arduino Nano with 4-bit parallel interface mega328 */
    #define HW_LCD_EN_PORT         PORTD
    #define HW_LCD_EN_PIN          6
 
    #define HW_LCD_RS_PORT         PORTD
    #define HW_LCD_RS_PIN          7
 
    #define HW_LCD_B4_PORT         PORTD
    #define HW_LCD_B4_PIN          5
    #define HW_LCD_B5_PORT         PORTD
    #define HW_LCD_B5_PIN          4
    #define HW_LCD_B6_PORT         PORTD
    #define HW_LCD_B6_PIN          3
    #define HW_LCD_B7_PORT         PORTD
    #define HW_LCD_B7_PIN          2
   #else
  		/* strip grid layout with 4-bit parallel interface and mega8/168/328 */
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
  #else		/* no STRIP_GRID_BOARD */
		/* normal layout with 4-bit parallel interface and  mega8/168/328 */
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
  #endif 		/* STRIP_GRID_BOARD */
 #endif		/* PROCESSOR_TYP for 4-bit parallel interface*/
#endif		/* INTERFACE_MODE */




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

#ifndef WITH_HARDWARE_SERIAL
//Watchdog
#define WDT_enabled
/* If you remove the "#define WDT_enabled" , the Watchdog will not be activated.
  This is only for Test or debugging usefull.
 For normal operation please activate the Watchdog !
*/
#endif


// RH_OFFSET : systematic offset of resistor measurement with RH (470k) 
// resolution is 0.1 Ohm, 3500 defines a offset of 350 Ohm
#define RH_OFFSET 3500 

// TP2_CAP_OFFSET is a additionally offset for TP2 capacity measurements in pF units
#define TP2_CAP_OFFSET 2

// CABLE_CAP defines the capacity (pF) of 12cm cable with clip at the terminal pins
#define CABLE_CAP 3


// LONG_WAIT_TIME is the time for displaying the successfull test result
#ifndef LONG_WAIT_TIME
#define LONG_WAIT_TIME 28000
#endif

// MIDDLE_WAIT_TIME is used for display the next parameter for Show data menu function
#define MIDDLE_WAIT_TIME 15000

// SHORT_WAIT_TIME is the time for displaying a unknown component message
#ifndef SHORT_WAIT_TIME
#define SHORT_WAIT_TIME 8000
#endif

//**********************************************************

// defines for the WITH_UART option
/*
With define SWUART_INVERT you can specify, if the software-UART operates normal or invers.
in the normal mode the UART sends with usual logik level (Low = 0; High = 1).
You can use this mode for direct connection to a uC, or a level converter like MAX232.

With invers mode the UART sends with invers logik (Low = 1, High = 0).
This is the level of a standard RS232 port of a PC.
In most cases the output of the software UART can so be connected to the RxD of a PC.
The specification say, that level -3V to 3V is unspecified, but in most cases it works.
Is a simple but unclean solution.

Is SWUART_INVERT defined, the UART works is inverse mode
*/
//#define SWUART_INVERT

#if PROCESSOR_TYP == 644
 // ATmega324/644/1284
 #define SERIAL_PORT PORTD
 #define SERIAL_DDR DDRD
 #if STRIP_GRID_BOARD == 7    /* m644 */
 #define SERIAL_BIT PD4		/* T7 use port D bit 1 for serial out */
 #else
 #define SERIAL_BIT PD1		/* ATmega644 use port D bit 1 for serial out */
 #endif
 #define TXD_VAL 0	/* no output for ADC port */
 #define TXD_MSK 0	/* no output for ADC port */
#elif PROCESSOR_TYP == 1280
 // ATmega1280/2560
 #define SERIAL_PORT PORTD
 #define SERIAL_DDR DDRD
 #define SERIAL_BIT PD3		/* ATmega1280 use port D bit 3 for serial out */
 #define TXD_VAL 0	/* no output for ADC port */
 #define TXD_MSK 0	/* no output for ADC port */
#else
 // ATmega8/168/328
 #if STRIP_GRID_BOARD == 3
  // special layout for Arduino Nano, use TXD(PD1) for serial output
  #define SERIAL_PORT PORTD
  #define SERIAL_DDR DDRD
  #define SERIAL_BIT PD1 	//TxD-Pin of Software-UART
  // If you use any pin of port C for output, you should define all used pins in TXD_MSK.
  // With TXD_VAL you can specify the default level of output pins for all port C output pins.
  #define TXD_MSK 0
  #define TXD_VAL 0
 #else
  #define SERIAL_PORT PORTC
  #define SERIAL_DDR DDRC
  #define SERIAL_BIT PC3 	//TxD-Pin of Software-UART; must be at Port C !
  #ifdef WITH_UART
   #define TXD_MSK (1<<SERIAL_BIT)
   #ifdef SWUART_INVERT
    #define TXD_VAL 0
   #else
    #define TXD_VAL TXD_MSK
   #endif
  #else	/* no WITH_UART */
  // If you use any pin of port C for output, you should define all used pins in TXD_MSK.
  // With TXD_VAL you can specify the default level of output pins for all port C output pins.
   #define TXD_MSK 0
   #define TXD_VAL 0
  #endif	/* WITH_UART */
 #endif /* STRIP_GRID_BOARD */
#endif	/* PROCESSOR_TYP */

 //define a default zero value for ESR measurement (0.01 Ohm units)
// #define ESR_ZERO 20

// all capacity measurement results does not go C_LIMIT_TO_UNCALIBRATED pF below the calibrated zero capacity.
#define C_LIMIT_TO_UNCALIBRATED 20

// all ESR measurements results does not go R_LIMIT_TO_UNCALIBRATED 0.01 Ohm units below the calibrated zero resistance.
#define R_LIMIT_TO_UNCALIBRATED 20   

// number of rotation steps to identify a fast rotation of rotary switch
#define FAST_ROTATION 10

/*########################################################################################
End of configuration 
*/


// defines for bits in samplingADC function, parameter 1 (what)
// smplADC_* are bit numbers or offsets
// samplingADC_* are corresponding bit values
// note: _hpf, _freq, _many, _mux and _sck are only valid WITH_XTAL

 #define smplADC_freq 7		/* generate a bunch of pulses (only without -DSamplingADC_CNT ) */
 #define smplADC_cumul 6	/* don't overwrite, but add the samples to the array */
 #define smplADC_many 5		/* with _freq, generate 4 times as many pulses */
 #define smplADC_direct 4	/* direct pulse without resistor */
 #define smplADC_mux 3		/* with large span, switch MUX to input 0 for ADC measurements that are to be discarded */
 #define smplADC_sck 1		/* short-circuit the crystal after excitation, duration given by extra parameter */
 #define smplADC_8bit 0         /* treat the array as an array of 8-bit samples */
 #define smplADC_hpf 2          /* treat the data with a high-pass filter, i.e., remove DC; only to be used with _8bit, in which case the 8bit samples are signed */
 // 0x0f				/* pulse-length 1-16, only for -DSamplingADC_CNT */


 #define smplADC_span 8		/* distance between two samples, value in upper 8 bits */

#define samplingADC_direct (1<<smplADC_direct)
#define samplingADC_cumul (1<<smplADC_cumul)
#define samplingADC_freq (1<<smplADC_freq)
#define samplingADC_many (1<<smplADC_many)
#define samplingADC_sck (1<<smplADC_sck)
#define samplingADC_mux (1<<smplADC_mux)
#define samplingADC_8bit (1<<smplADC_8bit)
#define samplingADC_hpf (1<<smplADC_hpf)



// #undef WITH_VEXT   /* disable the external voltage measurement */

#ifndef LCD_CYRILLIC
 #if (defined(LANG_RUSSIAN) || defined(LANG_UKRAINIAN))
  #warning LCD_CYRILLIC automatically set!
  #define LCD_CYRILLIC    // LCD_CYRILLIC is required by font.h
 #endif
#endif
#ifndef NO_FREQUENCY_SWITCH
#define NO_FREQUENCY_SWITCH 0
#endif
#include "font.h"
#include "autoconf.h"
#ifdef WITH_GRAPHICS
 #include "fonts/24x32_vertical_LSB.h"
#endif
// #define AUTO_LC_CAP 1  /* reread parallel capacity value for [RL] measurement */
#endif /* not defined ADC_PORT */

/* ****************************** EOF ***************************************** */
