/**********************************************************************************/
/* Assembler-Version of Optiboot created by K.-H. Kuebbeler (kh_kuebbeler@web.de) */
/* This Version can support additional EEprom read/write  with blinking LED       */
/* within 512 bytes. */
/* A automatic baud rate measurement can also be selected for the HW-UART or with */
/* the software UART.                                                             */
/* The automatic baud rate version can operate with the 512 byte space limit only */
/* if you use the HW-UART and without the LED-flashing.                           */
/* One Pin operation is possible with a additional hardware interface.            */
/* This Hardware interface must be connected between the host UART IO and         */
/* the one Pin RX/TX of the AVR. The hardware interface prevents a echo to the    */
/* host side RX, if the data are send from host's TX. The AVR TX/RX output will   */
/* pass to the Host's RX.  */
/* Prepared to support the old ATmega163     Dec. 17,2017                         */
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

#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>

#include "stk500.h"
#include "pin_defs.h"
#include "optiboot.h"
#include "led_defs.h"
#include "io_macros.h"

#include "check_fuses.h"


#define OPTIBOOT_MAJVER 6
#define OPTIBOOT_MINVER 3
/*
 * OPTIBOOT_CUSTOMVER should be defined (by the makefile) for custom edits
 * of optiboot.  That way you don't wind up with very different code that
 * matches the version number of a "released" optiboot.
 */

#if !defined(OPTIBOOT_CUSTOMVER)
 #define OPTIBOOT_CUSTOMVER 110
#endif

#if !defined(VIRTUAL_BOOT_PARTITION)
 #define VIRTUAL_BOOT_PARTITION 0
#endif

 /*
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

#ifndef SUPPORT_EEPROM
 #define SUPPORT_EEPROM 1	/* enable EEprom support by default */
#endif

#ifndef BIGBOOT
 #define BIGBOOT 0		/* set dummy size to 0 */
#endif

#ifndef TEST_OUTPUT
 #define TEST_OUTPUT 0
#endif

#if LED_START_FLASHES >= 0
 ; negative count of LED_START_FLASHES means a RX Pin monitoring!
 #define Check_RX 1
#else
 #warning "LED flash loop without RX Pin monitoring!"
#endif

#ifdef __AVR_ATmega163__
 #ifdef NRWWSTART
  #if NRWWSTART != 0
   #warning "NRWWSTART for ATmega163 must be unset or zero!"
   #undef NRWWSTART
   #define NRWWSTART 0
  #endif
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
#endif

 ; the reset cause is hold in GPIOR0 (or OCR2 for ATmega8/16/32)
#ifdef GPIOR0
 #define RESET_CAUSE GPIOR0
#else
 #ifdef OCR2
  #define RESET_CAUSE OCR2
 #else
  #define RESET_CAUSE ICR1L
 #endif
#endif

// #define SUPPORT_READ_FUSES 1

	.section	.version
optiboot_version:
	.byte	OPTIBOOT_MINVER
	.byte	OPTIBOOT_MAJVER+OPTIBOOT_CUSTOMVER
#ifdef RADIO_UART
 #error "RADIO_UART is not supported by optiboot!"
#endif

;***************************************************************
;***************************************************************
	.section .text
	.func optiboot
	.global optiboot

optiboot:
	cli
	eor	r1, r1
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
/* only the above list of processors require a initializing of Stack Pointer SP */
/* for ATtiny.. and ATmega324/644/1284/88/168/328 this has been done by reset! */
 #if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) 
  #define RAM_END_OFFSET 256	/* predefined RAMEND doesn't work */
 #else
  #define RAM_END_OFFSET 0
 #endif

	ldi	r24,hi8(RAMEND-RAM_END_OFFSET)
	AOUT	SPH, r24
	ldi	r24,lo8(RAMEND-RAM_END_OFFSET)
	AOUT	SPL, r24
#endif

  /*
   * With wireless flashing it's possible that this is a remote
   * board that's hard to reset manually.  In this case optiboot can
   * force the watchdog to run before jumping to userspace, so that if
   * a buggy program is uploaded, the board resets automatically.  We
   * still use the watchdog to reset the bootloader too.
   */

//#define marker (*(uint32_t *) (RAMEND - 16 - 3))

	AIN	r2, MCUCSR
;	clr	r1
	AOUT	MCUCSR, r1	; MCUCSR = 0
	AOUT	RESET_CAUSE, r2	; save reason of restart (MCUCSR) in IO register

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
	tst	r2
	breq	no_osc_upd
	AIN	r25, OSCCAL
	subi	r25, OSCCAL_CORR
	AOUT	OSCCAL, r25
no_osc_upd:
 #endif
#endif

#ifdef FORCE_WATCHDOG
 #ifdef GPIOR1
  #define MARKER1 GPIOR1
  #define MARKER2 GPIOR2
 #else
  #define MARKER1 OCR1BL
  #define MARKER2 OCR1BH
 #endif
 #define MARKER3 ICR1L
 #define MARKER4 ICR1H

//if ((ch & _BV(WDRF)) && marker == 0xdeadbeef) {
//    marker = 0;
//    appStart(reset_cause);
//  }
 #if TEST_OUTPUT == 0
	sbrc	r2, WDRF
	rjmp	to_app_start		; was WDT reset, start Application
 #endif

no_app_start:
	ldi	r20, 0xDE		; marker = 0xdeadbeef
	AOUT	MARKER1
	ldi	r20, 0xAD
	AOUT	MARKER2
	ldi	r20, 0xBE
	AOUT	MARKER3
	ldi	r20, 0xEF
	AOUT	MARKER4
	rjmp	try_loader

to_app_start:
 #if 0
	AIN	r20, MARKER1		; if MARKER1:4 == 0xDEADBEEF
	cpi	r20, 0xDE
	AIN	r20, MARKER2
	sbci	r20, 0xAD
	AIN	r20, MARKER3
	sbci	r20, 0xBE
	AIN	r20, MARKER4
	sbci	r20, 0xEF
	brne	no_app_start
 #endif
	AOUT	MARKER1, r1		; marker = 0
	AOUT	MARKER2, r1		;
	AOUT	MARKER3, r1		;
	AOUT	MARKER4, r1		;
;	rjmp	appStart
 #define WATCHDOG_TIME WATCHDOG_4S
#else		/* no FORCE_WATCHDOG */
  // save the reset flags in the designated register
  //  This can be saved in a main program by putting code in .init0 (which
  //  executes before normal c init code) to save R2 to a global variable.
 #if TEST_OUTPUT == 0
	ldi	r24, (1<<WDRF)|(1<<PORF)|(1<<BORF)  ;0x0D
	and	r24, r2		; r2 hold the reset reason , MCUCSR
		; none of the WatchDog, PowerOn or BrownOut reason ?
	breq	try_loader	; interrupt must be caused by a external reset
 #else
	rjmp	try_loader	; never start application with option TEST_OUTPUT
 #endif
 #define WATCHDOG_TIME WATCHDOG_OFF
#endif		/* FORCE_WATCHDOG */
;***************************************************************
appStart:

#if (defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny1634__)) && (WATCHDOG_TIME == WATCHDOG_OFF)
	ldi	r21, 0xD8	; special signature to change protected I/O register
	AOUT	CCP, r21
	AOUT	WDTCSR, r1	; clear watchdog timer
#else
	ldi	r20, WATCHDOG_TIME	; _OFF or _4S depending on FORCE_WATCHDOG
	rcall	watchdogConfig		; WATCHDOG_OFF (or _4S if FORCE_WATCHDOG)
#endif
#if 0
;##############################################################
	; talk to the application 
	ldi	ZL,hi8(1000)
	AOUT	GPIOR2, ZH
	AOUT	ICR1H, ZH
	ldi	ZL,lo8(1000)
	AOUT	GPIOR1, ZH
	AOUT	ICR1L, ZH
;##############################################################
#endif
;	AIN	r2, RESET_CAUSE			; R2  is compatible  to older optiboot version
	; R2 is never used and should still hold the content of last MCUCSR read
;	mov	r24,r2			; I believe, that r24 must not be set 
					; The reset cause  is in R2 and IO-Reg RESET_CAUSE
	; application program should look to the RESET_CAUSE register directly instead

#if	VIRTUAL_BOOT_PARTITION > 0
	ldi	ZL, save_vect_addr/2		; // Jump to WDT vector (jmp or rjmp table)
	eor	ZH, ZH
#else
       	eor	ZL, ZL	; // Jump to RST vector	 0x0000
      	eor	ZH, ZH
#endif
       	ijmp

;***************************************************************
try_loader:

/* If the LED_DDR bit would be switched, no prepare is needed */
/* because the LED_PORT bit is set to 0 by reset */
/* But the LED Anode must be connected to the VCC side */
#if (LED_START_FLASHES != 0) || (LED_DATA_FLASH > 0)
	ASBI	LED_DDR, LEDbit			; set LED Port bit to output mode
 #if ( (LED_DATA_FLASH & 0x04) > 0)
   /* LED_DATA_FLASH=4 will put the light steady on */
	ASBI	LED_PORT, LEDbit
 #endif
#endif

#ifdef TIMEOUT_MS
 #if TIMEOUT_MS > 6000
	ldi	r20, WATCHDOG_8S	;0x20
 #elif TIMEOUT_MS > 3000
	ldi	r20, WATCHDOG_4S	;0x28
 #elif TIMEOUT_MS > 1500
	ldi	r20, WATCHDOG_2S	;0x0F
 #elif TIMEOUT_MS > 750
	ldi	r20, WATCHDOG_1S	;0x0E
 #else
	ldi	r20, WATCHDOG_500MS	;
 #endif
#else
	ldi	r20, WATCHDOG_1S	;0x0E	(default value)
#endif
	rcall	watchdogConfig

; The check for locked PLL is disabled (1 == 0), because the PLL was 
; allways locked during the tests.
#if (1 == 0) && defined(PLL_MODE) && defined(PLLCSR)
 ; if a PLL is present and enabled, wait for the lock signal
;	if PLL_MODE is set, PLLE should be enabled allways in PLLCSR
;	ASBIS	PLLCSR, PLLE		; skip, if PLL is enabled
;	rjmp	no_pll
wait_locked:
	ASBIS	PLLCSR, PLOCK		; wait until PLL is locked
	rjmp	wait_locked
no_pll:
#endif

#if FLASHEND > 0x1ffff
	clr	r3			; reset mode
#endif
	ldi	r18, lo8(RAMSTART)	; r18:r19 = RAMSTART
	ldi	r19, hi8(RAMSTART)	; 

#if SOFT_UART == 0
 #if INVERSE_UART > 0
  #error "Inverse UART data are not supported with Hardware UART!"
 #endif
 /* I don't know the reason for disabling the Pull-Up resistors!
  * The port D is only correct for ATmega8/88/168/328.
  * Other processors must use other Ports, probably depending
  * on the selected UART number!
  * Therefore this code is disabled!  K.-H. Kuebbeler
 #if defined(PORTD) && defined(PD1)
	ACBI	PORTD, PD0	; RXD, disable PullUp
	ACBI	PORTD, PD1	; TX0   ????
 #endif
   ; usually the PORT Bits are initialized to zero allready.
	ACBI	UART_RX_PORT, UART_RX_BIT	; disable Pullup Resistor for RX
	ACBI	UART_TX_PORT, UART_TX_BIT	; disable Pullup Resistor for TX
  */

 #if BAUD_RATE < 100
  ; try with Auto-baud

  #include "flash_led_opti.S"	/* flash the led before init of UART */
  #include "init_uart_auto_baud.S"

 #else

  #include "init_uart_fix_baud.S"

 #endif		/* BAUD_RATE < 100 */
 
#endif		/* no SOFT_UART */

#if SOFT_UART > 0
 #if (UART_TX_DDR == UART_RX_DDR) && (UART_TX_BIT == UART_RX_BIT)
  #if INVERSE_UART > 1
   #warning "One Pin UART can not be used with INVERSE_UART !"
   #undef INVERSE_UART
   #define INVERSE_UART 0
  #endif
  #warning "One Wire mode serial IO selected!"
	ACBI	UART_TX_PORT, UART_TX_BIT	; set TX bit to low
	ACBI	UART_TX_DDR, UART_TX_BIT	; set TX DDR bit as input, High level with Pull-Up
 #else
	; initialize the TX output, normal mode
  #if INVERSE_UART > 1
	ACBI	UART_TX_PORT, UART_TX_BIT	; set TX bit to low
  #else
	ASBI	UART_TX_PORT, UART_TX_BIT	; set TX bit to high
  #endif
	ASBI	UART_TX_DDR, UART_TX_BIT	; set TX DDR bit as output
 #endif

 #if BAUD_RATE < 100
  ; try with Auto-baud

  #include "flash_led_opti.S"	/* flash the led before init of UART */
  #include "init_uart_auto_baud.S"

 #endif
#endif

#if BAUD_RATE >= 100
 #include "flash_led_opti.S"	/* flash the led after init of UART */
#endif

#if TEST_OUTPUT != 0
 #warning "Test output to serial TXD and OC2"
 ; this output should only be used for checking the baud-rate with scope
 ; the bootloader sequence is disturbed with this output.
 #warning "##### endless loop out of 'U' at TX, don't use a bootloader"
;	ASBI	DDRB, DDD1
;	ldi	r24, (1<<COM20) | (1<<CS20)	; start with full speed and OC2
;	AOUT	TCCR2, r24
test_out:
	ldi	r24, 'U'		; binary 01010101
	rcall	putch			; putch can use r22, r23,(r24), r25
	rjmp	test_out		; endless loop
#endif



;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  /* Forever loop */
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
get_nextp:
	rcall	getch
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	cpi	r24, STK_GET_PARAMETER		; 'A' 0x41
	brne	fin_get_par
// handle get parameter instruction
	rcall	getch		; get parameter byte
	mov	r21, r24	; move parameter to r21
	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)

 	ldi	r24, OPTIBOOT_MINVER	; 	
	cpi	r21, STK_SW_MINOR	; Parm_STK_SW_MINOR (0x82)
	breq	to_putch	; rcall putch, rjmp put_ok
	ldi	r24, 0x03	; answer generic 0x03
	cpi	r21, STK_SW_MAJOR	; Parm_STK_SW_MAJOR (0x81)
	brne	to_putch	; rcall putch, rjmp put_ok
	ldi	r24, OPTIBOOT_MAJVER+OPTIBOOT_CUSTOMVER
to_putch:
       	rcall	putch		; answer MINVER or MAJVER+CUSTOMVER or 0x03
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
fin_get_par:
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#if defined(SUPPORT_READ_FUSES) && defined(__AVR_ATtiny84__)
	cpi	r24, STK_READ_FUSE		; 0x72
	brne	no_rd_fuse
	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)
	rcall	transfer_low_high_fuse
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
no_rd_fuse:
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	cpi	r24, STK_READ_FUSE_EXT		; 0x77
	brne	no_rd_fuse_ext
	rcall	transfer_low_high_fuse
	ldi	ZL, 2		; address of extended FUSE
	rcall	transfer_fuse
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
no_rd_fuse_ext:
 #if 0
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	cpi	r24, STK_READ_LOCK		; 0x73
	brne	no_rd_lock
	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)
	ldi	ZL, 1
	rcall	transfer_fuse
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
no_read_lock:
 #endif
#endif	/* SUPPORT_READ_FUSES */
ck_SET_DEV:
	ldi	r20, 20
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	cpi	r24, STK_SET_DEVICE		; 'B'
       	breq	to_getNch	; STK set device is ignored
	ldi	r20, 5
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   	cpi	r24, STK_SET_DEVICE_EXT		; 'E'
	brne	ck_LA
; STK set device or STK set device ext is ignored
to_getNch:
	rcall	getNch		; ignore r20 count (20 or 5) parameters
to_put_ok:
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_LA:
	cpi	r24, STK_LOAD_ADDRESS		; 'U'
   	brne	ck_UNI
; **** STK load address
	rcall	getch		; lower address bits
	mov	r4, r24
	rcall	getch		; upper address bits
	mov	r5, r24		; r4:5 is load address
#if defined(EEprom_ByteAddress)
	movw	ZL, r4		; save original address in r30:r31
#endif
	add	r4, r4		; newAddress << 1
	adc	r5, r5		; make word address to byte address
#if !defined(EEprom_ByteAddress)
	movw	ZL, r4		; save original address in r30:r31
#endif
#ifdef RAMPZ
 #if FLASHEND > 0x1ffff
	AIN	r24, RAMPZ
	andi	r24, 0xfe
        adc	r24, r1		; add carry
	AOUT	RAMPZ, r24
no_update:
 #else
	adc	r1, r1
	AOUT	RAMPZ, r1
	eor	r1, r1		; restore default zero value for r1
 #endif
#endif
	rjmp	ver_put 	; rcall verifySpace; rjmp put_ok
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_UNI:
       	cpi	r24, STK_UNIVERSAL 	; 'V'
       	brne	ck_PP
#if 0
	; The following code is not tested, the RAMPZ is updated with the
	; Address Overflow instead. If the Begin of the Update-region is
	; below 0x1ff80, this method will work well and requires less code.
        ; 'V', 'M', 0x00, 0x0R, 0x00, ' '  is the sequence for RAMPZ setting to R*2
	rcall	getch
	cpi	r24, 0x4d		; 'M' = Map Flash memory
	breq	isMap
	ldi	r20, 3
	rcall	getNch
	ldi	r24, 0
	rjmp	to_putch	; rcall putch, rjmp put_ok
isMap:
	rcall 	getch		; MSB for Program Memory Page is ignored
	rcall	getch		; LSB for Program Memory Page
	add	r24, r24	; * 2
	AOUT	RAMPZ, r24
	rcall	getch		; 0x00 is ignored
	rcall	verifySpace
	ldi	r24,0
	rcall	putch
	rjmp	rjmp put_ok
#else
	ldi	r20, 4		; getNch(4)
	rcall	getNch
 	ldi	r24, 0
	rjmp	to_putch	; rcall putch, rjmp put_ok
#endif
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_PP:
  	cpi	r24, STK_PROG_PAGE 	; 'd'
	breq	is_PROG
	rjmp	ck_READP
is_PROG:
#if FLASHEND > 0x1ffff
	rcall	check_mode_change
#endif
    // Check for Mode change. If other mode before, clear RAMPZ
    /* Write memory, length is big endian and is in bytes */
	rcall	get_length	; r16:r17  and r26:r27 is length, r6=r24= type-'E'

    // PROGRAM PAGE - we support flash and optional EEPROM programming
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//#if (SUPPORT_EEPROM > 0) && (NRWWSTART != 0)
#if (NRWWSTART != 0)
    	breq	no_pg_erase	; if (type)
#endif
#if NRWWSTART != 0
  ; if NRWWSTART is zero, no RWW section is present. Never erase the page
  ; at this early state.
	ldi	r24, hi8(NRWWSTART) ; 0x70
	cp	r5, r24		; lo8(NRWWSTART) is allways zero
	brsh	no_pg_erase	; if (address < NRWWSTART)
 // If we are in RWW section, immediately start page erase
	rcall	boot_page_erase
 // While that is going on, read in page contents
#endif

no_pg_erase:
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	movw	YL, r18		; buf16Ptr (r28:r29) = RAMSTART
fill_buf:
	rcall	getch		; call next data from serial
	st	Y+, r24		; *bufPtr++ = getch()
	sbiw	r26, 1		; length = length - 1
	brne	fill_buf
	movw	r26, r16	; set length back to start value
	movw	YL, r18		; buf16Ptr (r28:r29) = RAMSTART

	; SRAM of ATmega is filled with data
	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)
#if SUPPORT_EEPROM > 0
	cpse	r6, r1		; r6 == 0, is EEprom write
	rjmp	write_flash	; if (!type)

; is EEprom write
; the address is allŕeady set in Z
ee_wrlop:
     	wdr		; watchdogReset();
	ld	r24, Y+			; *bufPtr++
 #if VIRTUAL_BOOT_PARTITION > 0
	rcall	wr_eeprom
 #else
 /* eeprom write is done only here, therefore without a rcall to save flash */
	rcall	set_eeprom_adr	; Z+
	AOUT	EEDR, r24	; data to EEprom controller
	ASBI	EECR, EEMPE
	ASBI	EECR, EEPE	; /* Start eeprom write by setting EEPE */
 #endif 	/* VIRTUAL_BOOT_PARTITION */

	sbiw	r26, 1		; length = length - 1
	brne	ee_wrlop
	; eeprom write is finished
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
#else
	cpse	r6, r1		; r6 = 0, is EEprom write
	rjmp	write_flash	; if (!type)
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp ; simulate OK
#endif		/* SUPPORT_EEPROM */

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
write_flash:
#if NRWWSTART != 0
  ; If NRWWSTART is zero, boot_page_erase must be called every time here!
	ldi	r24, hi8(NRWWSTART) ; 0x70
	cp	r5, r24			; hi8(NRWWSTART) lo8(NRWWSTART) is allways zero
	brlo	no_erase1
        // If we are in NRWW section, page erase has to be delayed until now.
        // Todo: Take RAMPZ into account (not doing so just means that we will
        //  treat the top of both "pages" of flash as NRWW, for a slight speed
        //  decrease, so fixing this is not urgent.)
#endif

	rcall	boot_page_erase
no_erase1:
        // If only a partial page is to be programmed, the erase might not be complete.
        // So check that here
	rcall	wait_flash_ready

#if VIRTUAL_BOOT_PARTITION > 0
 #if FLASHEND > 0x1ffff
  #error "Interrupt Vector Table can NOT be patched with more than 131072 byte flash!"
 #endif
 #if VerboseLev > 2
 #warning "RESET vector will be patched!"
 #endif
 #ifdef RAMPZ
	eor	r3, r3		; clear r3 == no patch
	AIN	r2, RAMPZ	; upper address bits in RAMPZ must be 0
	cp	r1, r2
	; RAMPZ is zero, when carry is unset
	cpc	r1, r4
 #else
	cp	r1, r4
 #endif
	cpc	r1, r5
	brcs	no_patch1	; address not zero, no patch
	inc	r3		; set r3 to 1, patch is done
	// address is zero!
        // This is the reset vector page. We need to live-patch the code so the
        // bootloader runs.
        //
        // Move RESET vector to WDT vector
;	movw	YL, r18		; buf16Ptr (r28:r29) = RAMSTART
 #if FLASHEND > 8192
 /*
  * We assume, that the vector table is filled with jmp instructions.
  * But this is not checked!! If the table is "optimized" with rjmp
  * instructions (Option -relax to the binder),
  * the two bytes loaded here, are  usually filled with
  * a nop instruction behind the rjmp.
  * Probably a patch program at the host computer, which send the 
  * programming data, is the better solution.
  * A patch program at the host can analyse the content of the hex data file
  * and create a new "patched" file. The patch program must know the
  * address of the bootloader and the interrupt vector number to save
  * the original reset vector content. 
  * The bootloader solution save the vector content for simulating
  + a correct verify pass after flashing.
  * But you can not repeat a separate verify pass without errors!!!
  */
	ldd	r24, Y+2	; rstVect = buff[2] | (buff[3]<<8)
	ldd	r25, Y+3
	ldi	ZL, lo8(save_vect_addr)	; load Z with save vector address
	ldi	ZH, hi8(save_vect_addr)
	add	ZL, r28
	adc	ZH, r29		;Z =  RAMSTART + save_vect_addr
	ldd	r8, Z+2		; r8:9 is the original vector address of save_vect_num
	ldd	r9, Z+3		; 
	std	Z+2, r24	; new WDT vector address  = old address of reset vector
	std	Z+3, r25
	ldi	r24, lo8(gs(optiboot))		; absolute address of optiboot start
	std	Y+2, r24
	ldi	r24, hi8(gs(optiboot))		; to reset vector address
	std	Y+3, r24
 #else
  /* two byte Vector table can only hold rjmp instructions */
	ld	r24, Y			; rstVect = buff[0] | (buff[1]<<8)
	ldd	r25, Y+1
          // Add jump to bootloader at RESET vector
	ldd	r8, Y+save_vect_addr	; wdtVect = buff[8] | (buff[9]<<8)
	ldd	r9, Y+save_vect_addr+1	; wdtVect = buff[8] | (buff[9]<<8)
	sbiw	r24, save_vect_num		; recalculate relative jmp, word address!
	std	Y+save_vect_addr, r24	; new WDT vector
	std	Y+save_vect_addr+1, r25
	ldi	r24, lo8(gs(optiboot-2))	; 0x7f
	st	Y, r24
	ldi	r24, hi8(gs(0x18000+optiboot-2))	; rjmp optiboot instruction (0xce)
	std	Y+1, r24
 #endif		/* FLASHEND > 8192 */ 

no_patch1:
#endif		/* VIRTUAL_BOOT_PARTITION */

;     Fill the programming buffer
;     ---------------------------
	movw	ZL, r4		; addrPtr = address
#if FLASHEND > 0x1ffff
	AIN	r21, RAMPZ	; save old RAMPZ
#endif
;	buf16Ptr (r28:r29) = RAMSTART
wr_lop1:
    	ld	r0, Y+		; *buf16Ptr++
	ld	r1, Y+
	ldi	r20, (1<<SELFPRGEN)
	; r0:r1 is data, Z r30:r31 is address, probably RAMPZ is also set
	rcall	do_spm		; AOUT SPMCSR, r20 ; spm
	eor	r1, r1
	adiw	ZL, 2		; increment address
#if FLASHEND > 0x1ffff
	adc	r21, r1		; increment the RAMPZ content
#endif
	sbiw	r26, 2		; length = length - 2
	brne	wr_lop1

	movw	ZL, r4		; addrPtr = address
#ifdef __AVR_ATmega163__
	andi	ZL, 0x80	; make shure a zero Z6:1 for ATmega163, 128 Byte Flash
#endif
#if defined(__AVR_ATtiny48__) || defined(__AVR_ATtiny88__)
	andi	ZL, 0xc0	; make shure a zero Z5:1 for ATtiny48/88, 64 Byte Flash
#endif
        // Write from programming buffer
        // -----------------------------
	ldi	r20, (1<<PGWRT)|(1<<SELFPRGEN)	; 0x05	
	rcall	do_spm		; AOUT SPMCSR, r20 ; spm
	rcall	wait_flash_ready
#if defined(RWWSRE)
       // Reenable read access to flash
	ldi	r20, (1<<RWWSRE)|(1<<SELFPRGEN)	; 0x11	
	rcall	do_spm		; AOUT SPMCSR, r20 ; spm
#endif
#if FLASHEND > 0x1ffff
	AOUT	RAMPZ, r21	; save (probably)  updated RAMPZ
#endif
#if VIRTUAL_BOOT_PARTITION > 0
	; now we save the original content of the save_vect (r8:r9)
	cp	r3, r1
	breq	put_ok			; no patch was done
	ldi	ZL, lo8(E2END-1)	; use last two bytes of EEprom memory
	ldi	ZH, hi8(E2END-1)	; 
	mov	r24, r8
	rcall	wr_eeprom		; save content of save_vect
	mov	r24, r9
	rcall	wr_eeprom
        ; Now we have saved the original save_vector address to the last two EEproms.
	; The save_vector has now the original address from the reset vector and
	; the reset vector now jmp to the bootloader.
#endif
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_READP:
      	cpi	r24, STK_READ_PAGE	; 't'
	brne   	ck_READS
#if FLASHEND > 0x1ffff
	rcall	check_mode_change
#endif
    /* Read memory block mode, length is big endian.  */
	rcall	get_length	; r16:r17  and r26:r27 is length, r6=r24= type-'E'
     // READ PAGE - we only read flash and optional EEPROM
	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)
;	TODO: putNch()

#if SUPPORT_EEPROM > 0
	tst	r6		; check if type was 'E'
	brne	flash_read
;     must be EEprom read
; read EEprom, the Address is allready set in Z
ee_rd_lop2:
	sbiw	r26, 1		; length-1
	brcs	next_adr_put_ok	;
 #if VIRTUAL_BOOT_PARTITION > 0
	rcall	rd_eeprom	; Z+
 #else
  /* EEprom read is only required here without the VIRTUAL_BOOT_PARTITION */
  /* Therefore is is used without a rcall directly                        */
	rcall 	set_eeprom_adr	; Z+
	ASBI	EECR, EERE
	AIN	r24, EEDR		; read data from EEprom
 #endif
	rcall	putch
	rjmp	ee_rd_lop2
#endif

; - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;	read flash
flash_read:
	movw	ZL, r4		; Z = addrPtr = address

flash_rd_lop:
#ifdef RAMPZ
          // Since RAMPZ should already be set, we need to use EPLM directly.
          // Also, we can use the autoincrement version of lpm to update "address"
          //      do putch(pgm_read_byte_near(address++));
          //      while (--length);
          // read a Flash and increment the address (may increment RAMPZ)
	elpm	r24, Z+
#else
	lpm	r24, Z+
#endif
#define LenDecrement 1

#if	VIRTUAL_BOOT_PARTITION > 0
	// Undo vector patch in bottom page so verify passes
 #undef LenDecrement 
 #define LenDecrement 2
	eor	r1, r1		; make shure a zero r1
 #ifdef RAMPZ
	elpm	r25, Z+
	AIN	r14, RAMPZ	; upper address bits in RAMPZ must be 0
	cp	r1, r14
	; RAMPZ is zero, when carry is unset
	cpc	r1, ZH
 #else
	lpm	r25, Z+
	cp	r1, ZH
 #endif
	brcs	nopatch			; upper bits of address are not zero
 #if FLASHEND > 8192
	; interrupt table with JMP	(4 Bytes for each entry)
	cpi	ZL, 2+2			; Z was increased by 2
	brne	ck8
	; move vector address back to the reset vector address for read flash
	subi	ZL, 2-save_vect_addr
	lpm	r24, Z+ 		; get reset vector address from save_vect
	lpm	r25, Z
	ldi	ZL, 2+2			; restore Z for loop
ck8:
	cpi	ZL, save_vect_addr+2+2	; Z was increased by 2
	brne	nopatch
	movw	r8, ZL			; save Z, which is still used by the loop
	ldi	ZL, lo8(E2END-1)		; last two bytes of EEprom memory
	ldi	ZH, hi8(E2END-1)		; 
	rcall	rd_eeprom		; get the original save_vect content
	mov	r14, r24
	rcall	rd_eeprom
	movw	ZL, r8			; restore Z for loop
	mov	r25, r24
	mov	r24, r14 		; old wdtVect	from EEprom save
 #else
	; interrupt table with RJMP	(2 Bytes for each entry)
	cpi	ZL, 2
	brne	ck8
 
	; move vector address back to the reset vector address for read flash
	subi	ZL, 2-save_vect_addr
	lpm	r24, Z+ 		; get reset vector address from save_vect
	lpm	r25, Z
	adiw	r24, save_vect_num	; recalculate relative jmp, word address!
	ldi	ZL, 2			; restore Z for loop
ck8:
	cpi	ZL, save_vect_addr+2
	brne	nopatch
	movw	r8, ZL			; save Z, which is still used by the loop
	ldi	ZL, lo8(E2END-1)		; last two bytes of EEprom memory
	ldi	ZH, hi8(E2END-1)		; 
	rcall	rd_eeprom		; get the original save_vect content
	mov	r14, r24
	rcall	rd_eeprom
	movw	ZL, r8			; restore Z for loop
	mov	r25, r24
	mov	r24, r14 		; old wdtVect	from EEprom save
 #endif		/* FLASHEND > 8192 */
nopatch:
	mov	r21, r25	; save second byte
	rcall	putch
	mov	r24, r21
#endif		/* VIRTUAL_BOOT_PARTITION */

	rcall	putch
	sbiw	r26, LenDecrement	; length - 1 or 2 bytes for VIRTUAL_BOOT_PARTITION
	brne	flash_rd_lop
next_adr_put_ok:
	movw	r4, ZL		; new address  ???
put_ok:
     	ldi	r24, STK_OK	; 0x10
       	rcall	putch
to_get_nextp:
       	rjmp	get_nextp

;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_READS:
     	cpi	r24, STK_READ_SIGN	; 'u'
	brne	ck_EOP
	brne	ck_LEAVE
;	READ SIGN - return what Avrdude wants to hear
	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)
	ldi	r24, SIGNATURE_0
	rcall	putch
	ldi	r24, SIGNATURE_1
	rcall	putch
	ldi	r24, SIGNATURE_2
	rjmp	to_putch	; rcall putch, rjmp put_ok
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_EOP:
			; avrdude send a STK_GET_SYNC followed by CRC_EOP
			; if the STK_GET_SYNC is loose out, the CRC_EOP is detected as last character
	cpi	r24, CRC_EOP			; ' ' 0x20
	breq	to_get_nextp			; wait for next STK_GET_SYNC
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ck_LEAVE:
      	cpi	r24, STK_LEAVE_PROGMODE	; 'Q'
	brne	ver_put
;  Adaboot no wait mod
	ldi	r20, WATCHDOG_16MS	; 0x08
	rcall	watchdogConfig
ver_put:
     	rcall	verifySpace	; check Sync_CRC_EOP, putch(STK_INSYNC)
	rjmp	put_ok		; putch(STK_OK); rjmp get_nextp

#if SOFT_UART > 0
;
; Here is a good location to place the delay loop for putch
; Usually the uart delay is placed before the getch or putch function automatically.
; But we wish to save a RJMP later at no_to
 #define SOFT_UART_PORT UART_TX_PORT
 #include "uart_delay.S"
#endif

;**********************************************************
; read r20 count character from serial input and look if space follow
getNch:		; call  getNch, r20 = count
	; repeat count times "call getch"
repeat_g:
     	rcall	getch		; do getch(); while (--count);
       	subi	r20, 1		; count - 1
     	brne	repeat_g
;** 	rjmp	verifySpace	; verifySpace(); let verifySpace return to netNch call

;**********************************************************
; look if a space character can be read from serial input and answer
verifySpace:		/* void verifySpace(void) { */
; use of r24,
     	rcall	getch		; if (getch() != CRC_EOP)
       	cpi	r24, CRC_EOP	; 0x20
       	breq	no_to
     	rcall	wait_timeout		;wait_timeout();
no_to:
       	ldi	r24, STK_INSYNC
;    	rjmp	putch		; let putch return to verifySpace call


;**********************************************************
#if SOFT_UART > 0
	; soft putch and getch uses registers r22,r23 and r25
 #define SOFT_UART_TX_PORT UART_TX_PORT
 #define SOFT_UART_TX_BIT UART_TX_BIT
 #define SOFT_UART_RX_PORT UART_RX_PORT
 #define SOFT_UART_RX_BIT UART_RX_BIT
 #include "soft_uart.S"		/* This will generate the putch and getch function */
				/* The uart_delay function is generated at a lower space. */
				/* Therefore the putch function is the first one and */
				/* can be reached from no_to without a rjmp  */
#else
 ; the Hardware UART is used
 #if ((F_CPU + (BAUD_RATE * 4)) / (BAUD_RATE * 8) - 1) < 3
  #if BAUD_ERROR != 0 // permit high bitrates (ie 1Mbps@16MHz) if error is zero
   #error "Unachievable baud rate (too fast) BAUD_RATE"
  #endif
 #endif // baud rate fastn check
putch:	/* void putch(char r24) */
 #if ((LED_DATA_FLASH & 0x02) > 0) && defined(LED_PORT) && defined(LEDbit)
	ASBI	LED_PORT, LEDbit
 #endif
 #ifdef UART_ONE_WIRE
	/* We can not use the advantage of hardware UART because the */
        /* TX output is enables only during the transmission time. */
	/* Therefore we must wait to finish of transmission to reenable */
        /* the RX input again */
	ldi	r26, UART_ENABLE_TX
	AOUT	UART_SRB, r25		; enable UART output, disable input
	AOUT	UART_UDR, r24	; UART_UDR = ch;
ptch2:
	AIN	r25, UART_STATUS	; while (!(UART_SRA & _BV(TXC0)));
      	sbrs	r25, TXC0
      	rjmp	ptch2		; wait, UART transmit not completed
	ldi	r25, UART_ENABLE_RX	
	AOUT	UART_SRB, r25		; enable UART input
 #else
	/* For normal serial transfer we wait until the buffer UDR is free. */
	/* After loading the buffer we return immediately to caller */
ptch1:
	AIN	r25, UART_STATUS	; while (!(UART_SRA & _BV(UDRE0)));
  #if UART_STATUS == LINSIR
	andi	r25, (1<<LBUSY)|(1<<LTXOK)
	cpi	r25, (1<<LBUSY)	
	breq	ptch1		; wait if LBUSY and !LTXOK
  #else
      	sbrs	r25, UDRE0
      	rjmp	ptch1		; wait, UART out not ready
  #endif
 #endif
	AOUT	UART_UDR, r24	; UART_UDR = ch;
 #if ((LED_DATA_FLASH & 0x02) > 0) && defined(LED_PORT) && defined(LEDbit)
	ACBI	LED_PORT, LEDbit
 #endif
	wdr		; watchdogReset(); /* prevent watch dog timeout during slow serial output */
      	ret
;**********************************************************
	/* call getch fetch a character from serial interface */
	; return value: r24 = new character read
	; use r22, r25 (soft)
getch: 
 #if ((LED_DATA_FLASH & 0x01) > 0) && defined(LED_PORT) && defined(LEDbit)
	ASBI	LED_PORT, LEDbit
 #endif
gtch1:
	AIN	r24, UART_STATUS	; if (UART_SRA & _BV(RXC0)) {
     	sbrs	r24, RXC0
       	rjmp	gtch1			; no data received, wait
#if UART_STATUS != UART_ERRSTAT
	AIN	r24, UART_ERRSTAT
#endif
      	sbrs	r24, FE0		; if (!(UART_SRA & _BV(FE0))) {
     	wdr		; watchdogReset();
       /*
         * A Framing Error indicates (probably) that something is talking
         * to us at the wrong bit rate.  Assume that this is because it
         * expects to be talking to the application, and DON'T reset the
         * watchdog.  This should cause the bootloader to abort and run
         * the application "soon", if it keeps happening.  (Note that we
         * don't care that an invalid char is returned...)
         */

 	AIN	r24, UART_UDR	; ch = UART_UDR; return ch;
 #if ((LED_DATA_FLASH & 0x01) > 0) && defined(LED_PORT) && defined(LEDbit)
	ACBI	LED_PORT, LEDbit
 #endif
       	ret
#endif	/* SOFT_UART */

;**********************************************************
#if VIRTUAL_BOOT_PARTITION > 0
/* Create function wr_eeprom */
/* EEprom address is in register Z (r30,r31), data in r24 */
/* Z is increased by 1 for every write cycle  */
wr_eeprom:
	rcall	set_eeprom_adr
	AOUT	EEDR, r24
 #if EECR < (0x20 + __SFR_OFFSET)
	sbi	_SFR_IO_ADDR(EECR), EEMPE
	sbi	_SFR_IO_ADDR(EECR), EEPE	/* Start EEprom write by setting EEPE */
 #else
	AIN	r24, EECR
	sbr	r24, EEMPE
	AOUT	EECR, r24
	sbr	r24, EEPE		/* Start EEprom write by setting EEPE */
	AOUT	EECR, r24
 #endif
	ret			; return wr_eeprom function

;**********************************************************
/* Function rd_eeprom read one byte from the EEprom address Z to r24 */
/* Z is increased by 1 for every read */
rd_eeprom:
	rcall	set_eeprom_adr
	ASBI	EECR, EERE
	AIN	r24, EEDR		; read data from EEprom
	ret			; return rd_eeprom function
#endif		/* VIRTUAL_BOOT_PARTITION */

;**********************************************************
#if (VIRTUAL_BOOT_PARTITION > 0) || defined(SUPPORT_EEPROM)
/* Z hold the eeprom address, which is loaded to EEAR and afterwards increased by 1 */
/* ASBIC can destroy content of register r0 */
set_eeprom_adr:
  	ASBIC	EECR, EEPE	; while (!eeprom_is_ready())
   	rjmp	set_eeprom_adr	; wait
;	rcall	wait_flash_ready

 #ifdef EEARH
	AOUT	EEARH, ZH	; EEAR = addrPtr++
 #endif
	AOUT	EEARL, ZL
	adiw	ZL, 1
	ret			; set_eeprom_adr
#endif		/* VIRTUAL_BOOT_PARTITION || SUPPORT_EEPROM */

#if defined(SUPPORT_READ_FUSES) && defined(__AVR_ATtiny84__)
;**********************************************************
;     transfer fuses will transfer the low and high fuse
transfer_low_high_fuse:	
	ldi	ZL, 0		; address of low fuse
	rcall	transfer_fuse
	ldi	ZL, 3		; address of high fuse
	; run to transfer_fuse, which will transfer the high fuse and return to transfer_low_high_fuse

;   transfer_fuse has number of fuse set to register r30 (ZL) !
;   the specified fuse is read from flash and send to the host.
transfer_fuse:
	ldi	ZH, 0		; upper bits are allways zero
;	ldi	r24, (1<<BLBSET) | (1<<SELFPRGEN)
	ldi	r24, (1<<RFLB) | (1<<SPMEN)
	AOUT	SPMCSR, r24
	lpm
	mov	r24, r0
	rjmp	putch		; putch will return to transfer_fuse
#endif

;**********************************************************
;	rcall get_length get two bytes from serial input
;	result is r26:r27 = r16:r17  last:first byte
;	use r22, r25 (soft)
;	additional result is r6 = r24 = type  - 'E' 
;	zero-Flag is set, if type is 'E' (with SUPPORT_EEPROM)
get_length:
	rcall	getch
	mov	r17, r24	; r17 = upper bits of length
	rcall	getch
	mov	r16, r24	; r16 = lower bits of length
	movw	r26, r16	; can be changed by sbiw or adiw
	rcall	getch		; read in type
//#if SUPPORT_EEPROM > 0
	subi	r24, 'E'	; type = getch() - 'E'
	mov	r6, r24
//#endif
	ret

;**********************************************************
;	call wait_flash_ready wait for a idle Flash controller
;	use r0
wait_flash_ready:
       	AIN	r0, SPMCSR 
	sbrc	r0, SELFPRGEN
	rjmp	wait_flash_ready
	ret

;**********************************************************
;	rcall boot_page_erase let the flash controller erase a page
;	r4:r5 must contain the byte address of the flash page to be erased
;	Z (r30:r31) is destroyed  (copy from r4:r5)
;	r20 is used to setup the spm instruction
;	probably RAMPZ must be set before to extend the r4:r5 address
;	return is immediately, the flash controller is probably still busy
boot_page_erase:
;	rcall	wait_flash_ready
    	movw	ZL, r4		; __boot_page_erase_short((uint16_t)(void*)address)
;	second entry	***********************************
;	rcall do_spm
;	r20 must contain the spm command
;	no register is destroyed
 #if defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny1634__) 
 ;	this processors do a 4 page erase, so only every fourth page
 ;	must do a page erage (page size is 16 bytes only)
  #if defined(__AVR_ATtiny1634__)
	ldi	r20, 0x60	; the lower two bits of the page address t1634
   #if VerboseLev > 2
    #warning "4 page erase for t1634"
   #endif
  #else
	ldi	r20, 0x30	; the lower two bits of the page address t841/t441.
   #if VerboseLev > 2
    #warning "4 page erase for t841/441"
   #endif
  #endif
	and	r20, r4
	brne	no_erase4	; not a fourth page
 #endif
	ldi	r20, (1<<PGERS)|(1<<SELFPRGEN)	; 0x03
do_spm:
	AOUT	SPMCSR, r20	; (1<<PGERS)|(SELFPRGEN)
	spm
#if defined(__AVR_ATmega161__) || defined(__AVR_AT163__) || defined(__AVR_ATmega323__)
;	special handling of spm for ATmega163 and ATmega323
	.word	0xffff
	nop
#endif
no_erase4:
	ret

;**********************************************************
;	reset the RAMPZ address extension, if mode change
;	r24 = new mode
;	r3  = old mode
;	the upper bits of RAMPZ are cleared with mode change!
#if FLASHEND > 0x1ffff
check_mode_change:
	cp	r24, r3
	breq	nochange
	mov	r3, r24		; update mode
	AIN 	r25, RAMPZ
	andi	r25, 0x01	; leave lowest bit, probably set by STK_LOAD_ADDRESS
	AOUT	RAMPZ, r25
nochange:
	ret
#endif

;**********************************************************
;	rcall wait_timeout  set the watch dog timer to 16ms and wait for reset
wait_timeout:
     	ldi	r20, WATCHDOG_16MS
	rcall	watchdogConfig	;  watchdogConfig(WATCHDOG_16MS) 
lop77:
    	rjmp	lop77		; endless loop, watch Dog will reset!


;**********************************************************
;	rcall watchdogConfig set the watch dog timer to the time specified by r20
;	use r21
watchdogConfig: 
#ifndef WDTCSR
 #define WDTCSR  WDTCR
#endif
#if defined(WDCE)
    	ldi	r21, (1<<WDCE) | (1<<WDE)	; 0x18
	AOUT	WDTCSR, r21	; (1<<WDCE) | (1<<WDE) ; watchdogConfig(x);
#else
 #if defined(WDTOE)
   	ldi	r21, (1<<WDTOE) | (1<<WDE)	; 0x18
	AOUT	WDTCSR, r21	; (1<<WDCE) | (1<<WDE) ; watchdogConfig(x);
 #else
    	ldi	r21, (1<<WDE)	; 0x08
	AOUT	WDTCSR, r21	; (1<<WDE) ; watchdogConfig(x);
 #endif
#endif
       	AOUT	WDTCSR, r20		; WDTCSR = x;
       	ret


;**********************************************************
#if LED_START_FLASHES != 0
; rcall wait_T1ov setup the timer1 to overflow in r24:r25 clock tics
; use  r24 and probably r0
; if no timer1 (16-Bit) is present, use counter 0 with additional register count
wait_T1ov:
 #ifdef TCNT1H
; use 16-Bit counter
	ldi	r24, hi8(0xffff-(F_CPU/(1024*20)))
	AOUT	TCNT1H, r24		; set new counter value to TCNT1
	ldi	r24, lo8(0xffff-(F_CPU/(1024*20)))
	AOUT	TCNT1L, r24
	ldi	r24, (1<<CS12)|(1<<CS10)	; internal clock, divide by 1024, mode 0
;	sts	_SFR_MEM_ADDR(TCCR1B), r24	
	AOUT	TCCR1B, r24

; wait until timer 1 OV occur (optional soft serial input can break the loop )
wt_ov1:
  #if Check_RX != 0		/* Check for RX Start bit is detected */
	; leave the wait for OV event loop, if data at the RX port are present
	sec			; set carry for break
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
       	ret			; any character present at serial rx port
  #endif		/* Check_RX */
	AIN	r24, TIFR1	; get Timer 1 Status bits
	sbrs	r24, TOV1	; Test OV bit
	rjmp	wt_ov1		; no OverFlow, wait
	AOUT	TIFR1, r24	; clear the flags
 #else
; no 16-Bit counter, use 8-Bit and counter register r25
	ldi	r25, hi8(0xffff-(F_CPU/(1024*20)))
	ldi	r24, lo8(0xffff-(F_CPU/(1024*20)))
  #ifdef TCNT0H
	; counter 0 is 16-Bit counter
	AOUT	TCNT0H, r25
	AOUT	TCNT0L, r24
  #else
	AOUT	TCNT0, r24
  #endif
	ldi	r24, (1<<CS12)|(1<<CS10)	; internal clock, divide by 1024, mode 0
	AOUT	TCCR0B, r24
; wait until timer 0 OV occur (optional soft serial input can break the loop )
wt_ov0:
  #if Check_RX != 0		/* Check for RX Start bit is canceled */
	; leave the wait for OV event loop, if data at the RX port are present
	sec			; set carry for break
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
       	ret			; any character present at serial rx port
  #endif		/* Check_RX */
	AIN	r24, TIFR	; get Timer 0 Status bits
	sbrs	r24, TOV0	; Test OV bit
	rjmp	wt_ov0		; no OverFlow, wait
	AOUT	TIFR, r24	; clear the flags
  #ifndef TCNT0H
	; counter 0 is not a 16-Bit counter, higher bits simulated with r25
	inc	r25
	brne	wt_ov0		; wait to next OV
  #endif
 #endif
 #if Check_RX != 0		/* Check for RX Start bit is detected */
	clc		; clear carry
 #endif
     	wdr		; watchdogReset();  /* prevent watch dog timeout during flashing the LED */
	ret
#endif		/* LED_FLASHES */

;-------------------------------------------------------------------------------------
#if (BIGBOOT > 0)
; only wast flash space for test of automatic size handling
	.rept	BIGBOOT/2
	nop
	.endr
#endif


;**********************************************************
        .endfunc
