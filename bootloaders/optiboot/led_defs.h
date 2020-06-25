/*
 * pin_defs.h
 * optiboot helper defining the default pin assignments for LED 
 * for the various chips that are supported.  This also has some ugly macros
 * for selecting among various LED possibilities using command-line
 * defines like LED=B5"
 *
 * extracted from pin_defs.h  2018 by K.-H. Kuebbeler
 * Copyright 2013-2015 by Bill Westfield.
 * Copyright 2010 by Peter Knight.
 * This software is licensed under version 2 of the Gnu Public Licence.
 * See optiboot.c for details.
 */

#ifndef LED_START_FLASHES
 #define LED_START_FLASHES 0
#endif

#ifndef LED_DATA_FLASH
 #define LED_DATA_FLASH 0
#endif

//#include <avr/io.h>
//#include "short_port_codes.h"

/* First make undefined port equal to p */
#if !defined(LED)
 #define LED p
#endif

/*------------------------------------------------------------------------ */
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || \
    defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || \
    defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || \
    defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) || \
    defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) 
/*------------------------------------------------------------------------ */

 /* Onboard LED is connected to pin PB5 in Arduino NG, Diecimila, and Duemilanove */ 
 #if (LED == n) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B5"
  #endif
  #define LEDX           pB5	/* coded Port B Bit 5 */
 #else
  #define LEDX           LED
 #endif

#endif		/* __AVR_ATmega168__  ... */

/* Luminet support */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny24__) || \
    defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny24A__)
/*------------------------------------------------------------------------ */
 /* Red LED is connected to pin PA4 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default A4"
  #endif
  #define LEDX        pA4
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny44__ || __AVR_ATtiny84__ */

/*------------------------------------------------------------------------ */
/* support for attiny841 and attiny441 */
#if defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__) 
/*------------------------------------------------------------------------ */
 /* usually all AVR devices without a bootpage region has set
    BOOT_PAGE_LEN and SPM_PAGESIZE to the same value,
    but tiny441 and tiny841 not */
 /* Onboard LED can be connected to pin PB0 on Sanguino */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default A0"
  #endif
  #define LEDX         pA0
 #else
  #define LEDX         LED
 #endif

#endif		/* __AVR_ATiny841__  .. */

/*------------------------------------------------------------------------ */

/* Support for ATtiny48 and ATtiny88 */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny88__) || defined(__AVR_ATtiny48__)
/*------------------------------------------------------------------------ */
 /* Red LED is connected to pin PA4 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B5"
  #endif
   #define LEDX        pB5
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny88__ || __AVR_ATtiny48__ */

/* Support for the ATtiny87 family */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__) 
/*------------------------------------------------------------------------ */
 /*  LED is connected to pin PA2 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default A2"
  #endif
  #define LEDX        pA2
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny87__ || __AVR_ATtiny167__ */

/* Support for ATtiny85 family*/
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) 
/*------------------------------------------------------------------------ */
 /* Red LED is connected to pin PA4 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B2"
  #endif
   #define LEDX        pB2
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny85__  */

/* Support for ATtiny 26x family*/
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny261__) || \
    defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny461A__) || defined(__AVR_ATtiny261A__) 
/*------------------------------------------------------------------------ */
 /* Red LED is connected to pin PA4 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default A0"
  #endif
   #define LEDX        pA0
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny86x__  */

/* Support for ATtiny4313 */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny4313__) || defined(__AVR_ATtiny2313__) || \
    defined(__AVR_ATtiny4313A__) || defined(__AVR_ATtiny2313A__)
/*------------------------------------------------------------------------ */
 /* Red LED is connected to pin PA4 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B5"
  #endif
   #define LEDX        pB1
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny4313__ */

/* Support for ATtiny1634 */
/*------------------------------------------------------------------------ */
#if defined(__AVR_ATtiny1634__)
/*------------------------------------------------------------------------ */
 /* usually all AVR devices without a bootpage region has set
    BOOT_PAGE_LEN and SPM_PAGESIZE to the same value,
    but tiny1634 not */
  #undef NRWWSTART
  #define NRWWSTART 0
 /* Red LED is connected to pin PA4 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B1"
  #endif
   #define LEDX        pB1
 #else
  #define LEDX         LED
 #endif

#endif	/* __AVR_ATtiny1634__ */

/*------------------------------------------------------------------------ */
/* Sanguino support (and other 40pin DIP cpus) */
#if defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) || \
    defined(__AVR_ATmega164PA__) || \
    defined(__AVR_ATmega324__) || defined(__AVR_ATmega324A__) || \
    defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || \
    defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || \
    defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || \
    defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || \
    defined(__AVR_ATmega1284A__) || \
    defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || \
    defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__) || \
    defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__)
/*------------------------------------------------------------------------ */
 /* Onboard LED is connected to pin PB0 on Sanguino */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B0"
  #endif
  #define LEDX         pB0
 #else
  #define LEDX         LED
 #endif

#endif		/* __AVR_ATmega644P__  .. */

/*------------------------------------------------------------------------ */
/* Support for AT90CAN32_64_128 */
#if defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__) 
/*------------------------------------------------------------------------ */
 /* Onboard LED is connected to pin PB0  */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B5"
  #endif
  #define LEDX         pB5
 #else
  #define LEDX         LED
 #endif

#endif		/* __AVR_AT90CAN... */

/*------------------------------------------------------------------------ */
#if defined(__AVR_AT90PWM2__) || defined(__AVR_AT90PWM2B__)
/*------------------------------------------------------------------------ */

 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default D2"
  #endif
  #define LEDX           pD2	/* coded Port D Bit 2 */
 #else
  #define LEDX           LED
 #endif

#endif		/* __AVR_AT90PWM2... */

/*------------------------------------------------------------------------ */
#if defined(__AVR_AT90PWM3__) || defined(__AVR_AT90PWM3B__)
/*------------------------------------------------------------------------ */

 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default C1"
  #endif
  #define LEDX           pC1	/* coded Port C Bit 1 */
 #else
  #define LEDX           LED
 #endif

#endif		/* __AVR_AT90PWM3... */

/*------------------------------------------------------------------------ */
#if defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__)
/*------------------------------------------------------------------------ */

 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B5"
  #endif
  #define LEDX           pB5	/* coded Port B Bit 5 */
 #else
  #define LEDX           LED
 #endif

#endif		/* __AVR_ATmega32U2__ ... */

/*------------------------------------------------------------------------ */
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
/*------------------------------------------------------------------------ */

 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default C6"
  #endif
  #define LEDX           pC6	/* coded Port C Bit 6 */
 #else
  #define LEDX           LED
 #endif

#endif		/* __AVR_ATmega32U4__ ... */

/*------------------------------------------------------------------------ */
/* Support for ATmega64 */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega64A__) || \
    defined(__AVR_ATmega128__)
/*------------------------------------------------------------------------ */
 /* Onboard LED is connected to pin PB0  */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B7"
  #endif
  #define LEDX         pB7
 #else
  #define LEDX         LED
 #endif

#endif		/* __AVR_ATmega64__  .. */

#if defined(__AVR_ATmega162__) || defined(__AVR_ATmega163__) || defined(__AVR_ATmega323__)
 /* Onboard LED is connected to pin PB5 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B0"
  #endif
  #define LEDX         pB0
 #else
  #define LEDX         LED
 #endif
#endif

#if defined(__AVR_ATmega169__) || defined(__AVR_ATmega169P__) || \
    defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__)
 /* Onboard LED is connected to pin PB5 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B5"
  #endif
  #define LEDX         pB5
 #else
  #define LEDX         LED
 #endif
#endif


#if defined(__AVR_ATmega165__) || defined(__AVR_ATmega165P__) || \
    defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165PA__) || \
    defined(__AVR_ATmega325__) || defined(__AVR_ATmega325P__) || \
    defined(__AVR_ATmega325A__) || defined(__AVR_ATmega325PA__) || \
    defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3250P__) || \
    defined(__AVR_ATmega3250A__) || defined(__AVR_ATmega3250PA__) || \
    defined(__AVR_ATmega645__) || defined(__AVR_ATmega645P__) || \
    defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645PA__) || \
    defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6450P__) || \
    defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450PA__) || \
    defined(__AVR_ATmega329__) || defined(__AVR_ATmega329P__) || \
    defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || \
    defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290P__) || \
    defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || \
    defined(__AVR_ATmega649__) || defined(__AVR_ATmega649P__) || \
    defined(__AVR_ATmega649A__) || \
    defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490P__) || \
    defined(__AVR_ATmega6490A__) 
 
 /* Onboard LED can be connected to pin PG0 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B7"
  #endif
  #define LEDX         pB7
 #else
  #define LEDX         LED
 #endif
#endif

/*------------------------------------------------------------------------ */
/* Mega support */
#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
    defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
/*------------------------------------------------------------------------ */
 /* Onboard LED is connected to pin PB7 on Arduino Mega1280 */ 
 #if (LED == p) && ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
  #ifndef NO_WARNING
   #warning "LED bit is set to default B7"
  #endif
  #define LEDX	pB7
 #else
  #define LEDX   LED
 #endif

#endif		/* __AVR_ATmega640__ || __AVR_ATmega1280__ ... */

/*------------------------------------------------------------------------ */
/* Settings for all processors */
/*------------------------------------------------------------------------ */


// The Preprocessor should have replaced LEDX with a constant now
#if ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
 #if (LEDX & 0xff) > 7
  #error "Unrecognized LED name. Should be like 'B5' "
  #error "probably the bit number is wrong!"
 #endif
 #if ((LEDX >> 8) > 12) || ((LEDX & 0xff00) == 0)
  #error "Unrecognized LED name.  Should be like 'B5' "
  #error "probably the port is wrong!"
 #endif

// LEDbit must be set to the bit number of the selected Port.
// This is allways given by the lower Byte of the previous definition
 #define LEDbit (LEDX & 0x07)
#endif


// setup the Port definitions for LED
#if ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0))
 #if (LEDX & 0xff00) == pB0
  #define LED_DDR     DDRB
  #define LED_PORT    PORTB
  #define LED_PIN     PINB
 #elif (LEDX & 0xff00) == pC0
  #define LED_DDR     DDRC
  #define LED_PORT    PORTC
  #define LED_PIN     PINC
 #elif (LEDX & 0xff00) == pD0
  #define LED_DDR     DDRD
  #define LED_PORT    PORTD
  #define LED_PIN     PIND
 #elif (LEDX & 0xff00) == pE0
  #define LED_DDR     DDRE
  #define LED_PORT    PORTE
  #define LED_PIN     PINE
 #elif (LEDX & 0xff00) == pF0
  #define LED_DDR     DDRF
  #define LED_PORT    PORTF
  #define LED_PIN     PINF
 #elif (LEDX & 0xff00) == pG0
  #define LED_DDR     DDRG
  #define LED_PORT    PORTG
  #define LED_PIN     PING
 #elif (LEDX & 0xff00) == pH0
  #define LED_DDR     DDRH
  #define LED_PORT    PORTH
  #define LED_PIN     PINH
 #elif (LEDX & 0xff00) == pJ0
  #define LED_DDR     DDRJ
  #define LED_PORT    PORTJ
  #define LED_PIN     PINJ
 #elif (LEDX & 0xff00) == pK0
  #define LED_DDR     DDRK
  #define LED_PORT    PORTK
  #define LED_PIN     PINK
 #elif (LEDX & 0xff00) == pL0
  #define LED_DDR     DDRL
  #define LED_PORT    PORTL
  #define LED_PIN     PINL
 #elif (LEDX & 0xff00) == pA0
  #define LED_DDR     DDRA
  #define LED_PORT    PORTA
  #define LED_PIN     PINA
 
 #else
  #error "--------------------------------------------"
  #error "Unrecognized LED name.  Should be like 'B4' "
  #error "--------------------------------------------"
 #endif
#endif		/*# ((LED_DATA_FLASH > 0) || (LED_START_FLASHES != 0)) */


