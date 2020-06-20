/**********************************************************************************/
/* Updated Version of Optiboot created by K.-H. Kuebbeler (kh_kuebbeler@web.de)   */
/* This version can support optional the handling of EEprom data and a automatic  */
/* baud rate selection for the HW-UART. If you select one of this features, the   */
/* optiboot code will not fit into 512 byte flash.                                */
/* The assembler version of the program selects the EEprom support by default.    */
/**********************************************************************************/

/**********************************************************/
/* Optiboot bootloader for Arduino                        */
/*                                                        */
/* http://optiboot.googlecode.com                         */
/*                                                        */
/* Arduino-maintained version : See README.TXT            */
/* http://code.google.com/p/arduino/                      */
/*  It is the intent that changes not relevant to the     */
/*  Arduino production envionment get moved from the      */
/*  optiboot project to the arduino project in "lumps."   */
/*                                                        */
/* Heavily optimised bootloader that is faster and        */
/* smaller than the Arduino standard bootloader           */
/*                                                        */
/* Enhancements:                                          */
/*   Fits in 512 bytes, saving 1.5K of code space         */
/*   Background page erasing speeds up programming        */
/*   Higher baud rate speeds up programming               */
/*   Written almost entirely in C                         */
/*   Customisable timeout with accurate timeconstant      */
/*   Optional virtual UART. No hardware UART required.    */
/*   Optional virtual boot partition for devices without. */
/*                                                        */
/* What you lose:                                         */
/*   Implements a skeleton STK500 protocol which is       */
/*     missing several features including EEPROM          */
/*     programming and non-page-aligned writes            */
/*   High baud rate breaks compatibility with standard    */
/*     Arduino flash settings                             */
/*                                                        */
/* Fully supported:                                       */
/*   ATmega168 based devices  (Diecimila etc)             */
/*   ATmega328P based devices (Duemilanove etc)           */
/*                                                        */
/* Beta test (believed working.)                          */
/*   ATmega8 based devices (Arduino legacy)               */
/*   ATmega328 non-picopower devices                      */
/*   ATmega644P based devices (Sanguino)                  */
/*   ATmega1284P based devices                            */
/*   ATmega1280 based devices (Arduino Mega)              */
/*                                                        */
/* Alpha test                                             */
/*   ATmega32                                             */
/*                                                        */
/* Work in progress:                                      */
/*   ATtiny84 based devices (Luminet)                     */
/*                                                        */
/* Does not support:                                      */
/*   USB based devices (eg. Teensy, Leonardo)             */
/*                                                        */
/* Assumptions:                                           */
/*   The code makes several assumptions that reduce the   */
/*   code size. They are all true after a hardware reset, */
/*   but may not be true if the bootloader is called by   */
/*   other means or on other hardware.                    */
/*     No interrupts can occur                            */
/*     UART and Timer 1 are set to their reset state      */
/*     SP points to RAMEND                                */
/*                                                        */
/* Code builds on code, libraries and optimisations from: */
/*   stk500boot.c          by Jason P. Kyle               */
/*   Arduino bootloader    http://arduino.cc              */
/*   Spiff's 1K bootloader http://spiffie.org/know/arduino_1k_bootloader/bootloader.shtml */
/*   avr-libc project      http://nongnu.org/avr-libc     */
/*   Adaboot               http://www.ladyada.net/library/arduino/bootloader.html */
/*   AVR305                Atmel Application Note         */
/*                                                        */

/* Copyright 2013-2015 by Bill Westfield.                 */
/* Copyright 2010 by Peter Knight.                        */
/*                                                        */
/* This program is free software; you can redistribute it */
/* and/or modify it under the terms of the GNU General    */
/* Public License as published by the Free Software       */
/* Foundation; either version 2 of the License, or        */
/* (at your option) any later version.                    */
/*                                                        */
/* This program is distributed in the hope that it will   */
/* be useful, but WITHOUT ANY WARRANTY; without even the  */
/* implied warranty of MERCHANTABILITY or FITNESS FOR A   */
/* PARTICULAR PURPOSE.  See the GNU General Public        */
/* License for more details.                              */
/*                                                        */
/* You should have received a copy of the GNU General     */
/* Public License along with this program; if not, write  */
/* to the Free Software Foundation, Inc.,                 */
/* 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA */
/*                                                        */
/* Licence can be viewed at                               */
/* http://www.fsf.org/licenses/gpl.txt                    */
/*                                                        */
/**********************************************************/


/**********************************************************/
/*                                                        */
/* Optional defines:                                      */
/*                                                        */
/**********************************************************/
/*                                                        */
/* BIG_BOOT:                                              */
/* Build a 1k bootloader, not 512 bytes. This turns on    */
/* extra functionality.                                   */
/*                                                        */
/* BAUD_RATE:                                             */
/* Set bootloader baud rate.                              */
/*                                                        */
/* LUDICROUS_SPEED:                                       */
/* 230400 baud :-)                                        */
/*                                                        */
/* SOFT_UART:                                             */
/* Use AVR305 soft-UART instead of hardware UART.         */
/*                                                        */
/* LED_START_FLASHES:                                     */
/* Number of LED flashes on bootup.                       */
/*                                                        */
/* LED_DATA_FLASH:                                        */
/* Flash LED when transferring data. For boards without   */
/* TX or RX LEDs, or for people who like blinky lights.   */
/*                                                        */
/* SUPPORT_EEPROM:                                        */
/* Support reading and writing from EEPROM. This is not   */
/* used by Arduino, so off by default.                    */
/*                                                        */
/* TIMEOUT_MS:                                            */
/* Bootloader timeout period, in milliseconds.            */
/* 500,1000,2000,4000,8000 supported.                     */
/*                                                        */
/* UART:                                                  */
/* UART number (0..n) for devices with more than          */
/* one hardware uart (644P, 1284P, etc)                   */
/*                                                        */
/* OSCCAL_CORR:                                           */
/* The specified value is subtracted from the OSCCAL Byte */
/* to correct the internal RC-oscillator frequency        */
/* This option is only effectual, when the internal       */
/* Frequency generation of the ATmega is used             */
/*                                                        */
/**********************************************************/

/**********************************************************/
/* Version Numbers!                                       */
/*                                                        */
/* Arduino Optiboot now includes this Version number in   */
/* the source and object code.                            */
/*                                                        */
/* Version 3 was released as zip from the optiboot        */
/*  repository and was distributed with Arduino 0022.     */
/* Version 4 starts with the arduino repository commit    */
/*  that brought the arduino repository up-to-date with   */
/*  the optiboot source tree changes since v3.            */
/* Version 5 was created at the time of the new Makefile  */
/*  structure (Mar, 2013), even though no binaries changed*/
/* It would be good if versions implemented outside the   */
/*  official repository used an out-of-seqeunce version   */
/*  number (like 104.6 if based on based on 4.5) to       */
/*  prevent collisions.                                   */
/*                                                        */
/**********************************************************/

/**********************************************************/
/* Edit History:					  */
/*			   				  */
/* Aug 2014						  */
/* 6.2 WestfW: make size of length variables dependent    */
/*              on the SPM_PAGESIZE.  This saves space    */
/*              on the chips where it's most important.   */
/* 6.1 WestfW: Fix OPTIBOOT_CUSTOMVER (send it!)	  */
/*             Make no-wait mod less picky about	  */
/*               skipping the bootloader.		  */
/*             Remove some dead code			  */
/*							  */
/* Jun 2014						  */
/* 6.0 WestfW: Modularize memory read/write functions	  */
/*             Remove serial/flash overlap		  */
/*              (and all references to NRWWSTART/etc)	  */
/*             Correctly handle pagesize > 255bytes       */
/*             Add EEPROM support in BIGBOOT (1284)       */
/*             EEPROM write on small chips now causes err */
/*             Split Makefile into smaller pieces         */
/*             Add Wicked devices Wildfire		  */
/*             Move UART=n conditionals into pin_defs.h	  */
/*             Remove LUDICOUS_SPEED option		  */
/*             Replace inline assembler for .version	  */
/*              and add OPTIBOOT_CUSTOMVER for user code  */
/*             Fix LED value for Bobuino (Makefile)       */
/*             Make all functions explicitly inline or    */
/*              noinline, so we fit when using gcc4.8	  */
/*             Change optimization options for gcc4.8	  */
/*             Make ENV=arduino work in 1.5.x trees.      */
/* May 2014                                               */
/* 5.0 WestfW: Add support for 1Mbps UART                 */
/* Mar 2013                                               */
/* 5.0 WestfW: Major Makefile restructuring.              */
/*             See Makefile and pin_defs.h                */
/*             (no binary changes)                        */
/*                                                        */
/* 4.6 WestfW/Pito: Add ATmega32 support                  */
/* 4.6 WestfW/radoni: Don't set LED_PIN as an output if   */
/*                    not used. (LED_START_FLASHES = 0)   */
/* Jan 2013						  */
/* 4.6 WestfW/dkinzer: use autoincrement lpm for read     */
/* 4.6 WestfW/dkinzer: pass reset cause to app in R2      */
/* Mar 2012                                               */
/* 4.5 WestfW: add infrastructure for non-zero UARTS.     */
/* 4.5 WestfW: fix SIGNATURE_2 for m644 (bad in avr-libc) */
/* Jan 2012:                                              */
/* 4.5 WestfW: fix NRWW value for m1284.                  */
/* 4.4 WestfW: use attribute OS_main instead of naked for */
/*             main().  This allows optimizations that we */
/*             count on, which are prohibited in naked    */
/*             functions due to PR42240.  (keeps us less  */
/*             than 512 bytes when compiler is gcc4.5     */
/*             (code from 4.3.2 remains the same.)        */
/* 4.4 WestfW and Maniacbug:  Add m1284 support.  This    */
/*             does not change the 328 binary, so the     */
/*             version number didn't change either. (?)   */
/* June 2011:                                             */
/* 4.4 WestfW: remove automatic soft_uart detect (didn't  */
/*             know what it was doing or why.)  Added a   */
/*             check of the calculated BRG value instead. */
/*             Version stays 4.4; existing binaries are   */
/*             not changed.                               */
/* 4.4 WestfW: add initialization of address to keep      */
/*             the compiler happy.  Change SC'ed targets. */
/*             Return the SW version via READ PARAM       */
/* 4.3 WestfW: catch framing errors in getch(), so that   */
/*             AVRISP works without HW kludges.           */
/*  http://code.google.com/p/arduino/issues/detail?id=368n*/
/* 4.2 WestfW: reduce code size, fix timeouts, change     */
/*             verifySpace to use WDT instead of appstart */
/* 4.1 WestfW: put version number in binary.		  */
/**********************************************************/


#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

/*
 * Note that we use our own version of "boot.h"
 * <avr/boot.h> uses sts instructions, but this version uses out instructions
 * This saves cycles and program memory.  Sorry for the name overlap.
 */
#include "boot.h"


// We don't use <avr/wdt.h> as those routines have interrupt overhead we don't need.
#define wdt_reset() __asm__ __volatile__ ("wdr")

/*
 * pin_defs.h
 * This contains most of the rather ugly defines that implement our
 * ability to use UART=n and LED=D3, and some avr family bit name differences.
 */
#include "pin_defs.h"
#include "optiboot.h"
#include "led_defs.h"

/*
 * stk500.h contains the constant definitions for the stk500v1 comm protocol
 */
#include "stk500.h"

#include "check_fuses.h"


#define OPTIBOOT_MAJVER 6
#define OPTIBOOT_MINVER 2

/*
 * OPTIBOOT_CUSTOMVER should be defined (by the makefile) for custom edits
 * of optiboot.  That way you don't wind up with very different code that
 * matches the version number of a "released" optiboot.
 */

#if !defined(OPTIBOOT_CUSTOMVER)
 #define OPTIBOOT_CUSTOMVER 110
#endif

unsigned const int __attribute__((section(".version"))) 
optiboot_version = 256*(OPTIBOOT_MAJVER + OPTIBOOT_CUSTOMVER) + OPTIBOOT_MINVER;

#ifdef LUDICROUS_SPEED
 #define BAUD_RATE 230400L
#endif

/* set the UART baud rate defaults */
#ifndef BAUD_RATE
 #if F_CPU >= 8000000L
  #define BAUD_RATE   115200L // Highest rate Avrdude win32 will support
 #elif F_CPU >= 1000000L
  #define BAUD_RATE   9600L   // 19200 also supported, but with significant error
 #elif F_CPU >= 128000L
  #define BAUD_RATE   4800L   // Good for 128kHz internal RC
 #else
  #define BAUD_RATE 1200L     // Good even at 32768Hz
 #endif
#endif


/*
 * We can never load flash with more than 1 page at a time, so we can save
 * some code space on parts with smaller pagesize by using a smaller int.
 */
#if SPM_PAGESIZE > 255
 typedef uint16_t pagelen_t ;
 #define GETLENGTH(len) len = getch()<<8; len |= getch()
#else
 typedef uint8_t pagelen_t;
 #define GETLENGTH(len) (void) getch() /* skip high byte */; len = getch()
#endif


/* Function Prototypes
 * The main() function is in init9, which removes the interrupt vector table
 * we don't need. It is also 'OS_main', which means the compiler does not
 * generate any entry or exit code itself (but unlike 'naked', it doesn't
 * supress some compile-time options we want.)
 */

//int main(void) __attribute__ ((OS_main)) __attribute__ ((section (".init9"))) __attribute__ ((__noreturn__));
int main(void) __attribute__ ((OS_main)) __attribute__ ((section (".init9")));
void __attribute__((noinline)) putch(uint8_t);
uint8_t __attribute__((noinline)) getch(void);
void __attribute__((noinline)) verifySpace();
void __attribute__((noinline)) watchdogConfig(uint8_t x);
static inline void getNch(uint8_t);
uint8_t __attribute__((noinline)) t1_delay(void);

#if SOFT_UART > 0
 void uartDelay() __attribute__ ((naked));
#endif
//void wait_timeout(void) __attribute__ ((__noreturn__));
void wait_timeout(void);
//void appStart(uint8_t rstFlags) __attribute__ ((naked))  __attribute__ ((__noreturn__));
void appStart(uint8_t rstFlags) __attribute__ ((naked));

/*
 * NRWW memory
 * Addresses below NRWW (Non-Read-While-Write) can be programmed while
 * continuing to run code from flash, slightly speeding up programming
 * time.  Beware that Atmel data sheets specify this as a WORD address,
 * while optiboot will be comparing against a 16-bit byte address.  This
 * means that on a part with 128kB of memory, the upper part of the lower
 * 64k will get NRWW processing as well, even though it doesn't need it.
 * That's OK.  In fact, you can disable the overlapping processing for
 * a part entirely by setting NRWWSTART to zero.  This reduces code
 * space a bit, at the expense of being slightly slower, overall.
 *
 * RAMSTART should be self-explanatory.  It's bigger on parts with a
 * lot of peripheral registers.
 */
#ifndef RAMSTART
 #if RAMEND > 0x2000
  #warning "RAMSTART was unset, is now 0x200"
  #define RAMSTART (0x200)
 #else
  #warning "RAMSTART was unset, is now 0x100"
  #define RAMSTART (0x100)
 #endif
#endif


/* C zero initialises all global variables. However, that requires */
/* These definitions are NOT zero initialised, but that doesn't matter */
/* This allows us to drop the zero init code, saving us memory */
#define buff    ((uint8_t*)(RAMSTART))


#ifndef SUPPORT_EEPROM
 #define SUPPORT_EEPROM 0
#endif

#ifdef BIGBOOT
 #undef SUPPORT_EEPROM		/* prevent compiler error, if previously set */
 #define SUPPORT_EEPROM 1	/* allways set the EEprom support */
#else
 #define BIGBOOT 0
#endif

#ifndef TEST_OUTPUT
 #define TEST_OUTPUT 0
#endif

#if LED_START_FLASHES < 0
 // negative count of LED_START_FLASHES means a RX Pin monitoring!
 #define Check_RX 1
 #warning "LED flash loop with RX Pin monitoring!"
#endif

#ifdef __AVR_ATmega163__
 #ifdef NRWWSTART
   #warning "NRWWSTART for ATmega163 must be unset or zero!"
   #undef NRWWSTART
   #define NRWWSTART 0
 #else
   #define NRWWSTART 0
 #endif
#endif

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || \
    defined(__AVR_ATmega8535__) || defined(__AVR_ATmega8515__) || \
    defined (__AVR_ATmega64__) || defined (__AVR_ATmega64A__) || \
    defined(__AVR_ATmega128__) || \
    defined(__AVR_ATmega163__)
 /* avrdude set a byte address for this processors (tested with ATmega8) */
 /* for the Program Page command. */
 /* For other processors like ATmega88/168/328 avrdude use a word address, */
 /* which is the same as with the Flash memory. */
 #define EEprom_ByteAddress
#else
 #define e_address f_address
#endif

// the reset cause is hold in GPIOR0 (or OCR2 for ATmega8/16/32)
#ifdef GPIOR0
 #define RESET_CAUSE GPIOR0
#else
 #ifdef OCR2
  #define RESET_CAUSE OCR2
 #else
  #define RESET_CAUSE ICR1L
 #endif
#endif
#if defined(UART_SRC) && defined(UART_SEL)
 // without the Option bit UART_SEL (URSEL) the UART_SRC (UCSRC) register
 // hold the upper SRR bits.
 #define UART_SRRH UART_SRC
#endif


/* main program starts here */
int main(void) {
  uint8_t ch;

  /*
   * Making these local and in registers prevents the need for initializing
   * them, and also saves space because code no longer stores to memory.
   * (initializing address to 0 keeps the compiler happy, but isn't really
   *  necessary, and uses 4 bytes of flash.)
   * A "address = address" solve the problem because no code is generated.
   */
  register uint16_t f_address = f_address;
#ifdef EEprom_ByteAddress
  register uint8_t e_address = e_address;
#endif
  register pagelen_t  length;

  // After the zero init loop, this is the first code to run.
  //
  // This code makes the following assumptions:
  //  No interrupts will execute
  //  SP points to RAMEND
  //  r1 contains zero
  //
  // If not, uncomment the following instructions:
  // cli();
  asm volatile ("clr __zero_reg__");
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || \
    defined (__AVR_ATmega16__) || defined (__AVR_ATmega16A__) || \
    defined (__AVR_ATmega32__) || defined (__AVR_ATmega32A__) || \
    defined (__AVR_ATmega64__) || defined (__AVR_ATmega64A__) || \
    defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__) || \
    defined(__AVR_AT90PWM2__) || defined(__AVR_AT90PWM3__) || \
    defined(__AVR_AT90PWM2B__) || defined(__AVR_AT90PWM3B__) || \
  defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || defined(__AVR_AT90CAN128__) || \
    defined(__AVR_ATmega162__) || defined(__AVR_ATmega163__) || \
    defined(__AVR_ATmega323__) || \
    defined(__AVR_ATmega169__) || defined(__AVR_ATmega169P__) || \
    defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__) || \
    defined(__AVR_ATmega329__) || defined(__AVR_ATmega329P__) || \
    defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || \
    defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290P__) || \
    defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || \
    defined(__AVR_ATmega649__) || defined(__AVR_ATmega649P__) || \
    defined(__AVR_ATmega649A__) || \
    defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490P__) || \
    defined(__AVR_ATmega6490A__)  || \
    defined(__AVR_ATmega165__) || defined(__AVR_ATmega165P__) || \
    defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165PA__) || \
    defined(__AVR_ATmega325__) || defined(__AVR_ATmega325P__) || \
    defined(__AVR_ATmega325A__) || defined(__AVR_ATmega325PA__) || \
    defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3250P__) || \
    defined(__AVR_ATmega3250A__) || defined(__AVR_ATmega3250PA__) || \
    defined(__AVR_ATmega645__) || defined(__AVR_ATmega645P__) || \
    defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645PA__) || \
    defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6450P__) || \
    defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450PA__)
 #if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
  SP = RAMEND - 256;
 #else
  SP = RAMEND;  // This is done by hardware reset
 #endif
#endif

  /*
   * modified Adaboot no-wait mod.
   * Pass the reset reason to app.  Also, it appears that an Uno poweron
   * can leave multiple reset flags set; we only want the bootloader to
   * run on an 'external reset only' status
   */
  // Adaboot no-wait mod
  ch = MCUCSR;
  MCUCSR = 0;

#if !defined(OSCCAL)
 // OSCCAL0 can be Bit 0 of OSCCAL, if OSCCAL is defined
 #if defined(OSCCAL0)
  // OSCCAL0 is the name if the first OSCCAL register
  #define OSCCAL OSCCAL0
 #endif
#endif

#if defined(OSCCAL_CORR) && defined(OSCCAL)
 #if (OSCCAL_CORR < -20) || (OSCCAL_CORR > 20)
  #warning "OSCCAL_CORR is too big, should be greater -20 and less than 20"
 #endif
 #if (OSCCAL_CORR != 0)
  if(ch != 0) OSCCAL -= OSCCAL_CORR;
 #endif
#endif

  if (ch & (_BV(WDRF) | _BV(PORF) | _BV(BORF)))
    appStart(ch);

  /*
   * Disable pullups that may have been enabled by a user program.
   * Somehow a pullup on RXD screws up everything unless RXD is externally
   * driven high.
   */
#if ((UART_TXX & 0xff00) == (UART_RXX & 0xff00))
  UART_TX_PORT &= ~((1<<UART_TX_BIT) || (1<<UART_RX_BIT));
#else	/* RX and TX use not the same port */
  UART_TX_PORT &= ~(1<<UART_TX_BIT);
  UART_RX_PORT &= ~(1<<UART_RX_BIT);
#endif

#if SOFT_UART > 0
  /* Set TX pin as output */
 #define BAUD_SETTING ( (F_CPU + BAUD_RATE) / (BAUD_RATE * 2))
 #define BAUD_ACTUAL (F_CPU / (BAUD_SETTING * 2))
  UART_TX_DDR |= _BV(UART_TX_BIT);
#else	/* no SOFT_UART */
 #if INVERSE_UART > 0
  #error "Inverse UART data are not supported with Hardware UART!"
 #endif

 /* Prepare the hardware UART */
 #define UART_MODE_2X
 #if BAUD_RATE >= 100
  #define BAUD_DIV (((F_CPU / 4L / BAUD_RATE) - 1) / 2)
  #define BAUD_ACTUAL (F_CPU / 8L / ((BAUD_DIV)+1))
  #if (BAUD_DIV > 255) && (BAUD_DIV < 2046)
   // try with single speed
   #undef UART_MODE_2X
   #undef BAUD_DIV
   #define BAUD_DIV ((F_CPU + BAUD_RATE * 8L) / (BAUD_RATE * 16L) - 1)
   #define BAUD_DIV (((F_CPU / 8L / BAUD_RATE) - 1) / 2)
   #define BAUD_ACTUAL (F_CPU / 16L / ((BAUD_DIV)+1))
   #if BAUD_DIV > 4095
    #error "Unachievable baud rate (too slow) BAUD_RATE"
   #endif
  #endif	/* (BAUD_DIV > 255) && (BAUD_DIV < 2046) */
 #endif		/* BAUD_RATE >= 100 */

 #ifdef UART_MODE_2X
   UART_CNTL = UART_SCALER8;	// Double speed mode USART
 #else
   UART_CNTL = UART_SCALER16;		// Single speed mode USART
 #endif
 //#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__) || defined(__AVR_ATmega16__)
 #if defined(UART_SRC) && defined(UART_SEL)
  UART_SRC = _BV(UART_SEL) | _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);  // config USART; 8N2
 #else		/* no ATmega8 ... */
  #if defined(UART_SRC)
  UART_SRC = _BV(USBS0) | _BV(UCSZ00) | _BV(UCSZ01);
  #endif
 #endif		/* defined(ATmega8) ... */
 #if BAUD_RATE >= 100
  UART_SRRL = (uint8_t)( BAUD_DIV );
  #if (BAUD_DIV/256) != 0
   UART_SRRH = (uint8_t)( BAUD_DIV/256 );
  #endif
  #ifdef UART_ONE_WIRE
  UART_SRB = UART_ENABLE_RX;		// enable only UART input
  #else
  UART_SRB = (UART_ENABLE_RX|UART_ENABLE_TX);	// enable UART input and output
  #endif
  #ifdef LINDAT
  LINDAT = 0;		// init TX ready flag
  #endif
 #endif
#endif		/* SOFT_UART > 0 */

#if BAUD_RATE >= 100
 // check the ACTUAL Baud Rate for soft and hard
 #if BAUD_ACTUAL <= BAUD_RATE
   #define BAUD_ERROR (( 100*(BAUD_RATE - BAUD_ACTUAL) ) / BAUD_RATE)
   #if BAUD_ERROR >= 5
     #error "BAUD_RATE error greater than -5%"
   #elif BAUD_ERROR >= 2
     #warning "BAUD_RATE error greater than -2%"
   #endif
 #else
   #define BAUD_ERROR (( 100*(BAUD_ACTUAL - BAUD_RATE) ) / BAUD_RATE)
   #if BAUD_ERROR >= 5
     #error "BAUD_RATE error greater than 5%"
   #elif BAUD_ERROR >= 2
     #warning "BAUD_RATE error greater than 2%"
   #endif
 #endif

 #if (BAUD_DIV < 3)
  #if BAUD_ERROR != 0 // permit high bitrates (ie 1Mbps@16MHz) if error is zero
   #error "Unachievable baud rate (too fast) BAUD_RATE"
  #endif
 #endif // baud rate fastn check
 #include "report_baud_div.h"
#endif		/* BAUD_RATE >= 100 */

#if (LED_START_FLASHES != 0) || (LED_DATA_FLASH > 0)
  /* Set LED pin as output */
  LED_DDR |= _BV(LEDbit);
 #if ( (LED_DATA_FLASH & 0x04) > 0)
   /* LED_DATA_FLASH=4 will put the light steady on */
  LED_PORT |= _BV(LEDbit);
 #endif
#endif

  // Set up watchdog to trigger after 500ms
#ifdef TIMEOUT_MS
 #if TIMEOUT_MS > 6000
  watchdogConfig(WATCHDOG_8S);		/* try to set watchdog timer to 8s */
 #elif TIMEOUT_MS > 3000
  watchdogConfig(WATCHDOG_4S);		/* try to set watchdog timer to 4s */
 #elif TIMEOUT_MS > 1500
  watchdogConfig(WATCHDOG_2S);		/* set the watchdog timer to 2s */
 #elif TIMEOUT_MS > 750
  watchdogConfig(WATCHDOG_1S);		/* set the watchdog timer to 1s */
 #else
  watchdogConfig(WATCHDOG_500MS);	/* set the watchdog timer to 500ms */
 #endif
#else
  watchdogConfig(WATCHDOG_1S);		/* set the watchdog timer to 1s (default) */
#endif

// The check for locked PLL is disabled (1 == 0), because the PLL was 
// allways locked during the tests.
#if (1 == 0) && defined(PLL_MODE) && defined(PLLCSR)
 
  // if a PLL is present and enabled, wait for the lock signal
  // if PLL_MODE is set, PLLE should be enabled allways in PLLCSR
  if ((PLLCSR & _BV(PLLE)) != 0) while((PLLCSR & _BV(PLOCK)) == 0);
#endif

#if FLASHEND > 0x1ffff
      uint8_t operation_mode;
      operation_mode = 0;	// preset operation mode, is changed by STK_PROG_PAGE / STK_READ_PAGE
#endif

#if LED_START_FLASHES != 0
  /* Flash onboard LED to signal entering of bootloader */
 #if (LED_START_FLASHES > 1) 
  uint8_t count = LED_START_FLASHES;
  do {
    LED_PORT |= _BV(LEDbit);
    if (t1_delay()) goto RX_was_high;
    LED_PORT &= ~(_BV(LEDbit));
    if (t1_delay()) goto RX_was_high;
  } while (--count);
 #elif (LED_START_FLASHES < -1)
  uint8_t count = LED_START_FLASHES;
  do {
    LED_PORT |= _BV(LEDbit);
    if (t1_delay()) goto RX_was_high;
    LED_PORT &= ~(_BV(LEDbit));
    if (t1_delay()) goto RX_was_high;
  } while (++count);
 #else
    LED_PORT |= _BV(LEDbit);
    if (t1_delay()) goto RX_was_high;
    LED_PORT &= ~(_BV(LEDbit));
    if (t1_delay()) goto RX_was_high;
 #endif
#endif	/* LED_START_FLASHES != 0 */

//-------------------------------------------------------------------------------------------------
#if BAUD_RATE < 100
 #if SOFT_UART != 0
  #error "Auto-Baud Function (with measurement) together with SOFT_UART is only supported by Assembler version!"
 #endif
 // this is the Auto Baud section
 // Time of RX bit change is measured with a 16-Bit counter
 #if BAUD_RATE >= 50
next_try:
 #endif
  wdt_reset();
 #if INVERSE_UART > 0
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, INVERSE
 #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0
 #endif
 #if LED_START_FLASHES != 0
RX_was_high:		/* entry for detected Start bit during flashing */
 #endif
 #ifdef TCNT1H
  #define CNT16_CNTH TCNT1H
  #define CNT16_CNTL TCNT1L
  #define CNT16_CCRB TCCR1B
  #define CNT16_OCREGH OCR1AH
  #define CNT16_OCREGL OCR1AL
  #define OCFLAG OCF1A
 #elif defined(TCNT0H)
  #define CNT16_CNTH TCNT0H
  #define CNT16_CNTL TCNT0L
  TCCR0A = _BV(TCW0);		// switch counter 0 to 16-Bit (tiny861)
  #define CNT16_CCRB TCCR0B
  #define CNT16_OCREGL OCR0A
  #define CNT16_OCREGH OCR0B
  #define OCFLAG OCF0A
 #else
  #error "Auto Baud mode requires a 16-Bit counter !"
 #endif
 #if !defined(TIFR1) && defined(TIFR)
  #define TIFR1 TIFR
 #endif

  CNT16_CCRB = 0;		// stop 16-bit counter 

 typedef union {
  uint8_t b[2];
  uint16_t w;
 } cnt16_t;

 #if BAUD_RATE < 60
  CNT16_CNTH = -1;
  CNT16_CNTL = -1;
 #else
  CNT16_CNTH = 0;
  CNT16_CNTL = 0;
 #endif

 #if BAUD_RATE < 60
  // short version of the Auto-Baud time measurement, only one time is measured.
  // this version will get wrong baud rate, if a wrong start bit is detected.
  cnt16_t baud_t1;
 #if INVERSE_UART > 0
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);		// wait until RX is not 0, Start Bit, INVERSE
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0 );	// wait until RX is not 1, S0000 sequence, INVERSE
 #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0 );	// wait until RX is not 1, Start Bit
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);		// wait until RX is not 0, S0000 sequence
 #endif

  CNT16_CCRB = _BV(CS11);		// start counter with /8 scaler
  #if BAUD_RATE > 49
  CNT16_OCREGH = (4864/256);
  CNT16_OCREGL = 0;
  TIFR1 |= (1<<OCFLAG);
   #if INVERSE_UART > 0
  do {
    if ((TIFR1 & (1<<OCFLAG)) != 0) goto next_try;
  } while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, S000011 sequence finished, INVERSE
   #else
  do {
    if ((TIFR1 & (1<<OCFLAG)) != 0) goto next_try;
  } while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, S000011 sequence finished
   #endif
  #else
   #if INVERSE_UART > 0
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, S000011 sequence finished, INVERSE
   #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, S000011 sequence finished
   #endif
  #endif
  baud_t1.b[0] = CNT16_CNTL;
  baud_t1.b[1] = CNT16_CNTH;
  baud_t1.w = baud_t1.w / 2;
  // transmission of the byte is not finished, we must wait for the last two "0" bits
   #if INVERSE_UART > 0
  while(UART_RX_PIN & _BV(UART_RX_BIT) != 0);	// wait until RX is not 1, S00001100 sequence finished, INVERSE
   #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, S00001100 sequence finished
   #endif
   UART_SRRL = baud_t1.b[0];
   UART_SRRH = baud_t1.b[1];


 #else		/* BAUD_RATE >= 60 */

  // This is a version with a more complex check of the received bit change sequence.
  // Three time values are measured for the received bit changes.
  // STK500 protocol send a '3'=0x30 followed by ' '=0x20.
  // P is a pause condition (STOP bit = 1) and S is a START bit (0),
  // the 0 and 1 represent the expected data bits.
  // the full bit sequence send is PS00001100PS00000100PPP
  //                                ^      ^  ^      ^
  //                                1      2  3      4
  // Every of the marked Position is a 1 to 0 transition!
  cnt16_t baud_t1;
  cnt16_t baud_t2;
  cnt16_t baud_t3;
   #if INVERSE_UART > 0
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, Start Bit, INVERSE
   #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, Start Bit
   #endif
  // first 1/0 RX bit change detected
  // If we begin now with the time measurement, we get the following result
  // for the next three bit changes:
  // Position 1: 5B,   7B  , 9B
  // Position 2: 2B+d, 3B+d, 9B+d
  // Position 3: 6B  , 7B  , 9B
  // Position 4: 2B  , 5B+Delay	, 7B+Delay
  // B means the time of one bit (Baud-time), d means a possible short delay between two characters
  // Delay means a long time delay, because the sender of the RX data will wait for a reply!
  // The best way to solve the Delay problem with Position 4 is to monitor
  // the counter time for exceed of a 4*(2B) limit.
  CNT16_CCRB = _BV(CS11);		// start counter with /8 scaler
   #if INVERSE_UART > 0
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, S0000 sequence, INVERSE
   #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, S0000 sequence
   #endif
  baud_t1.b[0] = CNT16_CNTL;	// time of S0000
  baud_t1.b[1] = CNT16_CNTH;

  baud_t2.w = baud_t1.w * 4;
  CNT16_OCREGH = baud_t2.b[1];
  CNT16_OCREGL = baud_t2.b[0];
  TIFR1 |= (1<<OCFLAG);
   #if INVERSE_UART > 0
  do {
    if ((TIFR1 & (1<<OCFLAG)) != 0) goto next_try;
  } while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, S000011 sequence finished, INVERSE
   #else
  do {
    if ((TIFR1 & (1<<OCFLAG)) != 0) goto next_try;
  } while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, S000011 sequence finished
   #endif
  baud_t2.b[0] = CNT16_CNTL;	// time of S000011
  baud_t2.b[1] = CNT16_CNTH;

   #if INVERSE_UART > 0
  while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1, S00001100 sequence, INVERSE
   #else
  while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, S00001100 sequence finished
   #endif
  baud_t3.b[0] = CNT16_CNTL;	// time of S00001100
  baud_t3.b[1] = CNT16_CNTH;

  // check the measured time sequence
  if (((baud_t3.w - baud_t1.w) > baud_t1.w) || ((baud_t3.w - baud_t2.w) > (baud_t2.w - baud_t1.w + 4))) {
  //  S00001100   - S0000                        S00001100 - S000011         S000011 - S0000 +
  //            1100              S0000                   00                         11+
    // wrong sync, data bit was detected as Start bit.
   #if INVERSE_UART > 0
    while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 0, INVERSE
   #else
    while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1
   #endif
    goto next_try;
  }
  #if BAUD_RATE >= 80 
  baud_t3.w = (baud_t3.w - baud_t1.w - 2) / 4;	// (S00001100-S0000-2)/4 = (1100 - 2) / 4
  #else   /* no FOUR_BIT_MODE */
  if ((baud_t3.w - baud_t1.w) > baud_t1.w) {
   #if INVERSE_UART > 0
    while((UART_RX_PIN & _BV(UART_RX_BIT)) == 0);	// wait until RX is not 1, INVERSE
   #else
    while((UART_RX_PIN & _BV(UART_RX_BIT)) != 0);	// wait until RX is not 1
   #endif
    goto next_try;
  }
  baud_t3.w = (baud_t3.w - baud_t2.w - 1) / 2;	// (S00001100-S000011-1)/2 = (00 -2) / 2
  #endif   /* BAUD_RATE >= 80 */

  UART_SRRL = baud_t3.b[0];
  UART_SRRH = baud_t3.b[1];
 #endif		/* BOOT_PAGE_LEN < 1024 */
 #ifdef LINCR
  #ifdef UART_ONE_WIRE
  LINCR = (1<<LENA)|(1<<LCMD2)|(1<<LCMD1);	// enable Rx
  #else
  LINCR = (1<<LENA)|(1<<LCMD2)|(1<<LCMD1)|(1<<LCMD0);	// enable Rx + Tx
  #endif
 #else
  #ifdef UART_ONE_WIRE
  UART_SRB = _BV(RXEN0);		// enable only UART input
  #else
  UART_SRB = _BV(RXEN0) | _BV(TXEN0);	// enable UART input and output
  #endif
  #ifdef LINDAT
  LINDAT = 0;		// init TX ready flag
  #endif
 #endif
 #if TEST_OUTPUT == 0
//   verifySpace();
//   putch(STK_OK);
   goto ver_put;
 #endif
#else
 #if LED_START_FLASHES != 0
RX_was_high:		/* entry for detected Start bit during flashing */
 #endif
#endif		/* BAUD_RATE < 100 */
//-------------------------------------------------------------------------------------------------

#if TEST_OUTPUT == 1
 #warning "optiboot with test output only!"
    /* only a test output for baud rate check, the bootloader will not work with this */
   for (;;) {
    putch('U');         // produce a 01010101 pattern for test
   } ;
#endif


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  /* Forever loop */
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  for (;;) {
    /* get character from UART */
    ch = getch();

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if(ch == STK_GET_PARAMETER) {
      unsigned char which = getch();
      verifySpace();
      if (which == 0x82) {
	/*
	 * Send optiboot version as "SW version"
         * Note that the references to memory are optimized away.
	 */
	putch(OPTIBOOT_MINVER);
      } else if (which == 0x81) {
	  putch(OPTIBOOT_MAJVER+OPTIBOOT_CUSTOMVER);
      } else {
	/*
	 * GET PARAMETER returns a generic 0x03 reply for
         * other parameters - enough to keep Avrdude happy
	 */
	putch(0x03);
      }
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_SET_DEVICE) {
      // SET DEVICE is ignored
      getNch(20);
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_SET_DEVICE_EXT) {
      // SET DEVICE EXT is ignored
      getNch(5);
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_LOAD_ADDRESS) {
      // LOAD ADDRESS
      union {
        // this contruct will help the compiler to generate effective code
        uint16_t w;		// word access, 16 bits
        uint8_t b[2];		// char access to the same item, 2x8 bits
      } newAddress;
      newAddress.b[0] = getch();	// first byte to lower bits
      newAddress.b[1] = getch();	// next byte to upper bits
#ifdef RAMPZ
      // Transfer top bit to RAMPZ
 #if FLASHEND > 0x1ffff
      // set the lowest bit of RAMPZ from newAddress
      uint8_t temp8 = RAMPZ & 0xfe;
      if ((newAddress.b[1] & 0x80) != 0) temp8 |= 1;
      RAMPZ = temp8;
 #else
      RAMPZ = (newAddress.b[1] & 0x80) ? 1 : 0;
 #endif
#endif
#if (SUPPORT_EEPROM > 0) && defined(EEprom_ByteAddress)
      e_address = newAddress.w;
#endif
      newAddress.w += newAddress.w; // Convert from word address to byte address
      f_address = newAddress.w;
      // if EEprom_ByteAddress is not set, e_address is replaced by f_address
      verifySpace();
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_UNIVERSAL) {
      // UNIVERSAL command is ignored
      getNch(4);
      putch(0x00);
    }
    /* Write memory, length is big endian and is in bytes */
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_PROG_PAGE) {
#if FLASHEND > 0x1ffff
      if (operation_mode != ch) {
         operation_mode = ch;	// update operation mode
         RAMPZ &= 0x01;		// leave lowest bit, probably set by STK_LOAD_ADDRESS
      }
#endif
      // PROGRAM PAGE - we support flash and optional EEPROM programming
      uint8_t *bufPtr;
      uint16_t addrPtr;

      GETLENGTH(length);
      pagelen_t savelength;
      savelength = length;

#if SUPPORT_EEPROM > 0
      uint8_t desttype = getch() - 'E';	/* desttype = 0, if EEprom */
 #if NRWWSTART != 0
        // If we are in RWW section, immediately start page erase
      if ((desttype != 0) && (f_address < NRWWSTART)) {
         boot_page_erase((uint16_t)(void*)f_address);	// early page erase
      }
 #endif
#else	/* no EEprom Support */
      getch();			/* dummy type */
 #if NRWWSTART != 0
        // If we are in RWW section, immediately start page erase
      if (f_address < NRWWSTART) {
         boot_page_erase((uint16_t)(void*)f_address);	// early page erase
      }
 #endif
#endif 

      // While that is going on, read in page contents
      bufPtr = buff;
      do *bufPtr++ = getch();
      while (--length);

      // Read command terminator, start reply
      verifySpace();

#if SUPPORT_EEPROM > 0
      if (!desttype) {	/* EEPROM */

        length = savelength;
        addrPtr = e_address;
        bufPtr = buff;
        while (length--) {
          wdt_reset();
          while (!eeprom_is_ready());

          EEAR = addrPtr++;
          EEDR = *bufPtr++;
          EECR |= 1 << EEMPE;	/* Write logical one to EEMPE */
          EECR |= 1 << EEPE;	/* Start eeprom write by setting EEPE */
        }
      } else {	/* Flash */
#endif
        // If we are in NRWW section, page erase has to be delayed until now.
        // Todo: Take RAMPZ into account (not doing so just means that we will
        //  treat the top of both "pages" of flash as NRWW, for a slight speed
        //  decrease, so fixing this is not urgent.)
#if NRWWSTART == 0
 #if defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny1634__) 
   //     this processors do a 4 page erase, so only every fourth page
   //     must do a page erage (page size is 16 bytes only)
	uint8_t check_fourth;
  #if defined(__AVR_ATtiny1634__)
	check_fourth = f_address & 0x60;	// the lower two bits of the page address t1634
   #warning "4 page erase for t1634"
  #else
	check_fourth = f_address & 0x30;	// the lower two bits of the page address t841/t441.
   #warning "4 page erase for t841/441"
  #endif
	if (check_fourth == 0) boot_page_erase((uint16_t)(void*)f_address);
 #else	/* erase every page */
	boot_page_erase((uint16_t)(void*)f_address);
 #endif
#else
        if ((f_address) >= NRWWSTART) {
           boot_page_erase((uint16_t)(void*)f_address);
        }
#endif

        // If only a partial page is to be programmed, the erase might not be complete.
        // So check that here
        boot_spm_busy_wait();

#ifdef VIRTUAL_BOOT_PARTITION
 #if FLASHEND > 8192
/*
 * AVR with 4-byte ISR Vectors and "jmp"
 * WARNING: this works only up to 128KB flash!
 */
      if (f_address == 0) {
        // This is the reset vector page. We need to live-patch the
        // code so the bootloader runs first.
        //
        // Save jmp targets (for "Verify")
	// Both interrupt vectors (rstVect and saveVect) must hold a JMP instruction!!
        rstVect0_sav = buff[rstVect0];
        rstVect1_sav = buff[rstVect1];
        saveVect0_sav = buff[saveVect0];
        saveVect1_sav = buff[saveVect1];

        // Move RESET jmp target to 'save' vector
        buff[saveVect0] = rstVect0_sav;
        buff[saveVect1] = rstVect1_sav;

        // Add jump to bootloader at RESET vector
        // WARNING: this works as long as 'main' is in first section
	// we should check, that the reset vector holds a JMP, not a RJMP!!!
        buff[rstVect0] = ((uint16_t)main) & 0xFF;
        buff[rstVect1] = ((uint16_t)main) >> 8;
      }

 #else
/*
 * AVR with 2-byte ISR Vectors and rjmp
 */
      if ((uint16_t)(void*)f_address == rstVect0) {
        // This is the reset vector page. We need to live-patch
        // the code so the bootloader runs first.
        //
        // Move RESET vector to 'save' vector
        // Save jmp targets (for "Verify")
        rstVect0_sav = buff[rstVect0];
        rstVect1_sav = buff[rstVect1];
        saveVect0_sav = buff[saveVect0];
        saveVect1_sav = buff[saveVect1];

        // Instruction is a relative jump (rjmp), so recalculate.
        uint16_t vect=(rstVect0_sav & 0xff) | ((rstVect1_sav & 0x0f)<<8); //calculate 12b displacement
        vect = (vect-save_vect_num) & 0x0fff; //substract 'save' interrupt position and wrap around 4096
        // Move RESET jmp target to 'save' vector
        buff[saveVect0] = vect & 0xff;
        buff[saveVect1] = (vect >> 8) | 0xc0; //

        // Add rjump to bootloader at RESET vector
        vect = ((uint16_t)main) &0x0fff; //WARNIG: this works as long as 'main' is in first section
	// where is the offset for relative jmp !!!!!!
        buff[rstVect0] = vect & 0xFF; // rjmp 0x1c00 instruction
        buff[rstVect1] = (vect >> 8) | 0xC0;
      }
 #endif // FLASHEND
#endif // VBP


        // Copy RAM-buffer into programming buffer
        uint16_t *buf16Ptr;
        buf16Ptr = (uint16_t *)buff;
        addrPtr = (uint16_t)(void*)f_address;
//        ch = SPM_PAGESIZE / 2;
        ch = savelength/2;
        do {
	  boot_page_fill((uint16_t)(void *)addrPtr,*buf16Ptr++);
          addrPtr += 2;
        } while (--ch);

        // Write from programming buffer to flash
        boot_page_write((uint16_t)(void*)f_address);
        boot_spm_busy_wait();

#if defined(RWWSRE)
        // Reenable read access to flash
        boot_rww_enable();
#endif
#if FLASHEND > 0x1ffff
	if (addrPtr == 0) RAMPZ += 1;		// increment RAMPZ, Address Overflow
#endif
#if SUPPORT_EEPROM > 0
      }  /* end if (!desttype) */
#endif
    }  /* end else if(ch == STK_PROG_PAGE) */

    /* Read memory block mode, length is big endian.  */
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_READ_PAGE) {
      // READ PAGE - we only read flash and EEPROM
#if FLASHEND > 0x1ffff
      if (operation_mode != ch) {
         operation_mode = ch;	// update operation mode
         RAMPZ &= 0x01;		// leave lowest bit, probably set by STK_LOAD_ADDRESS
      }
#endif
      GETLENGTH(length);
#if SUPPORT_EEPROM > 0
      uint8_t desttype = getch() - 'E';     /* 0 if EEprom */

      verifySpace();
      /* TODO: putNch */
      if (desttype)
#else
      getch(); 		/* dummy desttype */

      verifySpace();
      /* TODO: putNch */
#endif
        do {
#ifdef VIRTUAL_BOOT_PARTITION
        // Undo vector patch in bottom page so verify passes
            if (f_address == rstVect0) ch = rstVect0_sav;
            else if (f_address == rstVect1) ch = rstVect1_sav;
            else if (f_address == saveVect0) ch = saveVect0_sav;
            else if (f_address == saveVect1) ch = saveVect1_sav;
            else ch = pgm_read_byte_near(f_address);
            f_address++;
#elif defined(RAMPZ)
          // Since RAMPZ should already be set, we need to use EPLM directly.
          // Also, we can use the autoincrement version of lpm to update "f_address"
          //      do putch(pgm_read_byte_near(f_address++));
          //      while (--length);
          // read a Flash and increment the f_address (may increment RAMPZ)
          __asm__ ("elpm %0,Z+\n" : "=r" (ch), "=z" (f_address): "1" (f_address));
#else
          // read a Flash byte and increment the f_address
          __asm__ ("lpm %0,Z+\n" : "=r" (ch), "=z" (f_address): "1" (f_address));
#endif
          putch(ch);
        } while (--length);
#if SUPPORT_EEPROM > 0
      else 
        while (length--) {
          while (!eeprom_is_ready());

          EEAR = e_address++;
          EECR |= 1 << EERE;	/* Start eeprom read by writing EERE */

          putch( EEDR );
        }
#endif
    }

    /* Get device signature bytes  */
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if(ch == STK_READ_SIGN) {
      // READ SIGN - return what Avrdude wants to hear
      verifySpace();
      putch(SIGNATURE_0);
      putch(SIGNATURE_1);
      putch(SIGNATURE_2);
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if (ch == CRC_EOP) { /* ' ' */
// avrdude send a STK_GET_SYNC followed by CRC_EOP.
// If the STK_GET_SYNC is loose out, the CRC_EOP is detected as last character.
// Ignore this CRC_EOP and wait for next STK_GET_SYNC
      continue;
    }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    else if (ch == STK_LEAVE_PROGMODE) { /* 'Q' */
      // Adaboot no-wait mod
      watchdogConfig(WATCHDOG_16MS);
      verifySpace();
    }
    else {
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
      // This covers the response to commands like STK_ENTER_PROGMODE
#if BAUD_RATE < 100
ver_put:
#endif
      verifySpace();
    }
    putch(STK_OK);
  }  /* end for */
} /* end main */


void putch(uint8_t ch) {
#if SOFT_UART == 0
 // use the specified hardware UART
 #ifdef UART_ONE_WIRE
  // The main advantage of Hardware UART can not be used with ONE_WIRE mode.
  // We enable the TX transfer only for transmission time, therefore we must
  // wait until the transfer is done
  UART_SRB = UART_ENABLE_TX;	// enable UART output, disable input
  UART_UDR = ch;		// load data to transfer
  while (!(UART_STATUS & _BV(TXC0)));	// wait for TX transfer complete
  UART_SRB = UART_ENABLE_RX;	// enable UART input, disable output
 #else
  // For normal Serial communication we wait until the buffer can be loaded again
  // and return to caller after loading the new data.
  #ifdef LINSIR
  while((LINSIR & _BV(LTXOK)) == 0);	// wait for last character transmitted
  #else
  while (!(UART_STATUS & _BV(UDRE0)));	// wait for buffer empty
  #endif
  UART_UDR = ch;
 #endif
#else
  // Transfer serial data with software loop
  __asm__ __volatile__ (
    "   com %[ch]\n" // ones complement, carry set
    "   sec\n"
    "1: brcc 2f\n"
    "   cbi %[uartPort],%[uartBit]\n"
    "   rjmp 3f\n"
    "2: sbi %[uartPort],%[uartBit]\n"
    "   nop\n"
    "3: rcall uartDelay\n"
    "   rcall uartDelay\n"
    "   lsr %[ch]\n"
    "   dec %[bitcnt]\n"
    "   brne 1b\n"
    :
    :
      [bitcnt] "d" (10),
      [ch] "r" (ch),
 #ifdef UART_ONE_WIRE
      [uartPort] "I" (_SFR_IO_ADDR(UART_TX_DDR)),
 #else
      [uartPort] "I" (_SFR_IO_ADDR(UART_TX_PORT)),
 #endif
      [uartBit] "I" (UART_TX_BIT)
    :
      "r25"
  );
#endif
 wdt_reset();		/* prevent wdt time-out during slow TX transmission */
}

uint8_t getch(void) {
  uint8_t ch;

#if LED_DATA_FLASH > 0
  LED_PORT |= _BV(LEDbit);			// switch on LED
#endif

#if SOFT_UART > 0
  __asm__ __volatile__ (
    "1: sbic  %[uartPin],%[uartBit]\n"  // Wait for start edge
    "   rjmp  1b\n"
    "   rcall uartDelay\n"          // Get to middle of start bit
    "2: rcall uartDelay\n"              // Wait 1 bit period
    "   rcall uartDelay\n"              // Wait 1 bit period
    "   clc\n"
    "   sbic  %[uartPin],%[uartBit]\n"
    "   sec\n"
    "   dec   %[bitCnt]\n"
    "   breq  3f\n"
    "   ror   %[ch]\n"
    "   rjmp  2b\n"
    "3:\n"
    :
      [ch] "=r" (ch)
    :
      [bitCnt] "d" (9),
      [uartPin] "I" (_SFR_IO_ADDR(UART_RX_PIN)),
      [uartBit] "I" (UART_RX_BIT)
    :
      "r25"
);
#else
  while(!(UART_STATUS & _BV(RXC0)))
    ;
  if (!(UART_ERRSTAT & _BV(FE0))) {
      /*
       * A Framing Error indicates (probably) that something is talking
       * to us at the wrong bit rate.  Assume that this is because it
       * expects to be talking to the application, and DON'T reset the
       * watchdog.  This should cause the bootloader to abort and run
       * the application "soon", if it keeps happening.  (Note that we
       * don't care that an invalid char is returned...)
       */
    wdt_reset();
  }

  ch = UART_UDR;
#endif

#if LED_DATA_FLASH > 0
  LED_PORT &= ~(_BV(LEDbit));		// switch off LED
#endif

  return ch;
}  /* end getch() */

#if LED_START_FLASHES != 0
uint8_t t1_delay(void) {
 #ifdef TCNT1H
  // Set up Timer 1 for timeout counter
  TCNT1 = -(F_CPU/(1024*20));
  TCCR1B = _BV(CS12) | _BV(CS10); // div 1024
  TIFR1 = _BV(TOV1);
  #ifdef Check_RX
  do {
   #if INVERSE_UART > 0
    if ((UART_RX_PIN & _BV(UART_RX_BIT)) != 0) return(1);
   #else
    if ((UART_RX_PIN & _BV(UART_RX_BIT)) == 0) return(1);
   #endif
  } while( !(TIFR1 & _BV(TOV1)) );
  #else
  while(!(TIFR1 & _BV(TOV1)));
  #endif
 #elif defined(TCNT0H)
  // counter 0 is a 16-Bit counter
  #warning "16-Bit counter 0 is used instead of counter1"
  TCNT0H = (-(F_CPU/(1024*20))) / 256;
  TCNT0L = (-(F_CPU/(1024*20))) & 0xff;
  TCCR0B = _BV(CS02) | _BV(CS00); // div 1024
  TIFR = _BV(TOV0);
  #ifdef Check_RX
  do {
   #if INVERSE_UART > 0
     if ((UART_RX_PIN & _BV(UART_RX_BIT)) != 0) return(1);
   #else
     if ((UART_RX_PIN & _BV(UART_RX_BIT)) == 0) return(1);
   #endif
  } while( !(TIFR1 & _BV(TOV1)) );
  #else
  while(!(TIFR & _BV(TOV0)));
  #endif
 #else
  // no 16-Bit counter, use 8-Bit and external counter
  uint8_t ecnt;
  #warning "no 16-Bit counter, no 16-Bit counter1"
  ecnt = (-(F_CPU/(1024*20)))/256;
  TCNT0 = (-(F_CPU/(1024*20))) & 0xff;
  TCCR0B = _BV(CS02) | _BV(CS00); // div 1024
  for(;ecnt!=0;ecnt++) {
    TIFR = _BV(TOV0);
    #ifdef Check_RX
     do {
     #if INVERSE_UART > 0
       if ((UART_RX_PIN & _BV(UART_RX_BIT)) != 0) return(1);
     #else
       if ((UART_RX_PIN & _BV(UART_RX_BIT)) == 0) return(1);
     #endif
     } while( !(TIFR1 & _BV(TOV1)) );
    #else
    while(!(TIFR & _BV(TOV0)));
    #endif
   return(0);
  }
 #endif
  wdt_reset();		/* prevent wdt time-out during LED flashing */
}	/* end t1_delay() */
#endif		/* LED_START_FLASHES != 0 */

#if SOFT_UART > 0
// AVR305 equation: #define UART_B_VALUE (((F_CPU/BAUD_RATE)-23)/6)
// Adding 3 to numerator simulates nearest rounding for more accurate baud rates
#define RCALL_TICS 7
#define LOOP_TICS 9
#define WAST_TICS (((F_CPU/BAUD_RATE)- ((2*RCALL_TICS)+LOOP_TICS))+1)
#define UART_B_VALUE (WAST_TICS / 6)
 #if UART_B_VALUE > 255
  #error "Baud rate too slow for soft UART"
 #endif
#define UART_R_VALUE (WAST_TICS - (UART_B_VALUE * 6))/2
// with UART_R_VALUE the remainder of the uartDelay loop
// is added as dummy instruction at the end of the counter loop.

void uartDelay() {
  __asm__ __volatile__ (
#if UART_B_VALUE > 0
    "ldi r25,%[count]\n"
    "1:dec r25\n"
    "brne 1b\n"
#endif
#if UART_R_VALUE == 2
    "rjmp  .\n"
#endif
#if UART_R_VALUE == 1
    "nop   \n"
#endif
    "ret\n"
    ::[count] "M" (UART_B_VALUE)
  );
}
#endif

void getNch(uint8_t count) {
  do getch(); while (--count);
  verifySpace();
}

void verifySpace(void) {
  if (getch() != CRC_EOP)
    wait_timeout();
  putch(STK_INSYNC);
}

void wait_timeout(void) {
  watchdogConfig(WATCHDOG_16MS);      // shorten WD timeout
  while (1);			      // and busy-loop so that WD causes
    				      //  a reset and app start.
}

void watchdogConfig(uint8_t x) {
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = x;
}

void appStart(uint8_t rstFlags) {
#ifdef FORCE_WATCHDOG
  watchdogConfig(WATCHDOG_4S);
#else
  watchdogConfig(WATCHDOG_OFF);
#endif
 RESET_CAUSE = rstFlags;

  // save the reset flags in the designated register
  //  This can be saved in a main program by putting code in .init0 (which
  //  executes before normal c init code) to save R2 to a global variable.
  __asm__ __volatile__ ("mov r2, %0\n" :: "r" (rstFlags));

  __asm__ __volatile__ (
    // Jump to WDT or RST vector
    "ldi r30,%[rstvec]\n"
    "clr r31\n"
    "ijmp\n"::[rstvec] "M"(appstart_vec)
  );
#if BIGBOOT >= 128
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*2)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*3)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*4)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*5)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*6)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*7)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*8)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*9)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*10)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*11)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*12)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*13)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*14)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*15)
 #include "wast_128bytes.h"
#endif
#if BIGBOOT >= (128*16)
 #include "wast_128bytes.h"
#endif
}
