
/*########################################################################################
        Configuration
*/
#ifndef ADC_PORT
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 7735))
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
#define MODE_I2C 2		/* I2C interface for SSD1306 */
#define MODE_3LINE 3		/* special 3 line serial output with Data/Command as first data bit  PCF8814 */
#define MODE_SPI 4		/* 4 bit SPI interface for ST7565 or SSD1306 */
#define MODE_7920_SERIAL 5		/* serial interface for ST7920 */
#define MODE_7108_SERIAL 6		/* serial interface for ST7108 with shift register 74HC164 */
#define MODE_1308_SERIAL 7		/* serial interface for SSD1308 */

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
 #define TPext PA3
 // Port pin for 2.5V precision reference used for VCC check (optional)
 #define TPREF PA4
 #define TPRELAY PA4
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
 #define TPBAT ((1<<MUX2) | (1<<MUX0))
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
 #define ROTARY_A_DDR DDRB
 #define ROTARY_B_DDR DDRB
 #ifndef ROTARY_1_PIN
  // default connection is PB7
  #define ROTARY_1_PIN PB7
 #endif
 #ifndef ROTARY_2_PIN
  // PB5 is connected to character LCD D5 or ST7565 RS
  #define ROTARY_2_PIN PB5
  // can be preset to PB6 for character display and PB3 for ST7565 graphic controller
 #endif
 #define ROTARY_A_REG PINB
 #define ROTARY_B_REG PINB
/* FDIV_PIN specifies the output pin, which switch on a 16:1 frequency divider */
 #define FDIV_DDR DDRC
 #define FDIV_PORT PORTC
 #define FDIV_PIN PC0
/* the two bits P0 and P1 at FINP port control the input of frequency measurement */
/* P1:P0 = 00 external input, P1:P0 = 10 high frequency crystal, P1:P0 = 11 low frequency crystal */
 #define FINP_DDR DDRC
 #define FINP_PORT PORTC
 #define FINP_P0 PC1
 #define FINP_P1 PC2
#elif PROCESSOR_TYP == 1280
/* ------------------------------------------------------------------------- */
/* define the input pin for frequency measuring and also the pin change monitoring port for measuring the periode */
 // the PE6 pin is timer input of T3 and INT6 interrupt pin
 #define FREQINP_DDR DDRE
 #define FREQINP_PIN PE6
 #define PCMSK_FREQ EIMSK
 #define PCINT_FREQ INT6
/* define both input pins for rotaty encoder */
 #define ROTARY_A_DDR DDRA
 #define ROTARY_B_DDR DDRA
 #define ROTARY_A_REG PINA
 #define ROTARY_B_REG PINA
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
 #define ROTARY_A_DDR DDRD
 #define ROTARY_B_DDR DDRD
 #ifndef ROTARY_1_PIN
  // default connection is PD3
  #define ROTARY_1_PIN PD3
 #endif
 #ifndef ROTARY_2_PIN
  // PD1 is connected to character LCD D5/D7 or ST7565 RS
  #define ROTARY_2_PIN PD1
  // can be preset to PD2 for character display and PD5 for ST7565 graphic controller
 #endif
 #define ROTARY_A_REG PIND
 #define ROTARY_B_REG PIND
#endif		/* PROCESSOR_TYP */

#if CHANGE_ROTARY_DIRECTION
 #define ROTARY_B_PIN ROTARY_1_PIN	/* is connected to LCD-D7/D5 or ST7565-B0 */
 #define ROTARY_A_PIN ROTARY_2_PIN
#else		/* CHANGE_ROTARY_DIRECTION */
 #define ROTARY_A_PIN ROTARY_1_PIN	/* is connected to LCD-D7/D5 or ST7565-B0 */
 #define ROTARY_B_PIN ROTARY_2_PIN
#endif		/* CHANGE_ROTARY_DIRECTION */


/* ************************************************************************* */
/* define the output pin for switch the power on/off                         */
/* ************************************************************************* */
#if PROCESSOR_TYP == 644
 #define ON_DDR DDRB
 #define ON_PORT PORTB
 #define ON_PIN PB1      // This Pin is switched to high to switch power on
#elif PROCESSOR_TYP == 1280
 #define ON_DDR DDRA
 #define ON_PORT PORTA
 #define ON_PIN PA6      // This Pin is switched to high to switch power on
#else		/* PROCESSOR_TYP, must be ATmega8|168|328 */
 #define ON_DDR DDRD
 #define ON_PORT PORTD
 #define ON_PIN PD6      // This Pin is switched to high to switch power on
#endif		/* PROCESSOR_TYP */

/* ************************************************************************* */
/* define the pin for push button  (low value, if pressed)                   */
/* ************************************************************************* */
#if PROCESSOR_TYP == 644
  // currently no special strip grid layout defined
  #define RST_PORT PORTC
  #define RST_PIN_REG PINC
  #define RST_PIN PC7     //Pin, is switched to low, if push button is pressed
#elif PROCESSOR_TYP == 1280
 #if STRIP_GRID_BOARD == 1
  #define RST_PORT PORTA
  #define RST_PIN_REG PINA
  #define RST_PIN PA0     //Pin, is switched to low, if push button is pressed
 #else
  #define RST_PORT PORTA
  #define RST_PIN_REG PINA
  #define RST_PIN PA7     //Pin, is switched to low, if push button is pressed
 #endif
#else		/* PROCESSOR_TYP, must be ATmega8|168|328 */
  /* Processor mega8/168/328 , two board layouts defined */
 #if STRIP_GRID_BOARD == 1
 // Strip Grid board version
  #define RST_PORT PORTD
  #define RST_PIN_REG PIND
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
   #define RST_PIN PD7     //Pin, is switched to low, if push button is pressed
   // option STRIP_GRID is used to specify other connection of the LCD (chinese version),
   // the RST_PIN remain at PD7.
  #else
   #define RST_PIN PD0     //Pin, is switched to low, if push button is pressed
  #endif
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

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814))
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
  // currently no difference between normal and strip grid board
   /* the ST7565 Reset signal */
   #define HW_LCD_RES_DDR         DDRB
   #define HW_LCD_RES_PORT        PORTB
   #define HW_LCD_RES_PIN         4

   /* EN is the serial clock signal SCL */
   #define HW_LCD_EN_DDR          DDRB
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_PIN          6

   /* the data/instruction signal RS */
   #define HW_LCD_RS_DDR          DDRB
   #define HW_LCD_RS_PORT         PORTB
   #define HW_LCD_RS_PIN          5

   /* the data signal SI */
   #define HW_LCD_B0_DDR          DDRB
   #define HW_LCD_B0_PORT         PORTB
   #define HW_LCD_B0_PIN          7

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
   #if STRIP_GRID_BOARD == 5
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
#elif (LCD_INTERFACE_MODE == MODE_I2C)
 /* only two wires are required for the I2C interface: SCL and SDA */
 #ifndef LCD_I2C_ADDR
  #define LCD_I2C_ADDR 0x3C		/* SSD1306 controller defines 0x3c or 0x3d (SA0=1) as address */
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
#elif (LCD_INTERFACE_MODE == MODE_7920_SERIAL) || (LCD_INTERFACE_MODE == MODE_1308_SERIAL)
 /* only two wires are required for the serial interface of the ST7920: RW and E */
 /* PSB must be connected to GND and RS(CS) and RESET must be connected to 1. */
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* mega324/644/1284 with serial ST7920 interface */
   #define HW_LCD_EN_PORT         PORTB
   #define HW_LCD_EN_DDR          DDRB
   #define HW_LCD_EN_PIN          3

   #define HW_LCD_B0_PORT         PORTB
   #define HW_LCD_B0_DDR          DDRB
   #define HW_LCD_B0_PIN          4

   #define HW_LCD_RESET_PORT      PORTB
   #define HW_LCD_RESET_DDR       DDRB
   #define HW_LCD_RESET_PIN       2
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
#else /* (LCD_INTERFACE_MODE != MODE_SPI || MODE_I2C || MODE_ST7920_SERIAL || MODE_ST7108_SERIAL) */
 /* The 4-bit parallel Interface is usually used for the character display */
				/* --------------------------------------------- */
 #if PROCESSOR_TYP == 644	/* connection for 4-bit parallel interface and mega324/644/1284 */
  #ifdef STRIP_GRID_BOARD
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
You can use this mode for direct connection to a µC, or a level converter like MAX232.

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
 #define SERIAL_BIT PD1		/* ATmega644 use port D bit 1 for serial out */
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
// #undef WITH_VEXT   /* disable the external voltage measurement */
#include "font.h"
#include "autoconf.h"
#endif /* not defined ADC_PORT */
