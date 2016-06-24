#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>




.func wait1000ms
.global wait5s		; wait 5 seconds
.global wait4s		; wait 4 seconds
.global wait3s		; wait 3 seconds
.global wait2s		; wait 2 seconds
.global wait1s		; wait 1 seconds
.global wait1000ms	; wait 1 second and wait 1000ms are identical
.global wait500ms	; wait 500ms
.global wait400ms	; wait 400ms
.global wait300ms	; wait 300ms
.global wait200ms	; wait 200ms
.global wait100ms	; wait 100ms
.global wait50ms	; wait 50ms
.global wait40ms	; wait 40ms
.global wait30ms	; wait 30ms
.global wait20ms	; wait 20ms
.global wait10ms	; wait 10ms
.global wait5ms		; wait 5ms
.global wait4ms		; wait 4ms
.global wait3ms		; wait 3ms
.global wait2ms		; wait 2ms
.global wait1ms		; wait 1ms
.global wait500us	; wait 500µs
.global wait400us	; wait 400µs
.global wait300us	; wait 300µs
.global wait200us	; wait 200µs
.global wait100us	; wait 100µs
.global wait50us	; wait 50µs
.global wait40us	; wait 40µs
.global wait30us	; wait 30µs
.global wait20us	; wait 20µs
.global wait10us	; wait 10µs
/* A delay of 5us require a clock frequency of at least 1.4 MHz */
.global wait5us		; wait 5µs
.global wait4us		; wait 4µs
.global wait3us		; wait 3µs
.global wait2us		; wait 2µs
/* A delay of 1us require a clock frequency of at least 7 MHz */
.global wait1us		; wait 1µs

;wait loops for ATmega at a clock above 1MHz
; use of flash memory is: 76 bytes (16MHz operation, including 2 Byte Watch Dog reset at wait100ms)
; Every wait call needs only one instruction (rcall) for every of the 36 different delays (500ns - 5s).
; No registers are used. (only stack pointer)
; A maximum of 28 bytes of space for return addresses is used in RAM 
; I see no way to implement this function with C-language (too tricky)


wait5s:
 rcall wait1s		;	12+x return-adresses
wait4s:
 rcall wait1s		;	12+x return-adresses
wait3s:
 rcall wait1s		;	12+x return-adresses
wait2s:
 rcall wait1s		;1s     12+x Return-Adresses

wait1s:
wait1000ms:	
 rcall wait500ms	;500ms	11+x Return-Adresses

wait500ms:
 rcall wait100ms	;100ms	10+x Return-Adresses
wait400ms:
 rcall wait100ms	;100ms	10+x Return-Adresses
wait300ms:
 rcall wait100ms	;100ms	10+x Return-Adresses
wait200ms:
 rcall wait100ms	;100ms	10+x Return-Adresses


wait100ms:
 rcall wait50ms	    	; 50ms	9+x Return-Adresses

wait50ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses 
wait40ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses  
wait30ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses  
wait20ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses

wait10ms:
 rcall wait5ms	   	;5ms	7+x Return-Adresses
wait5ms:
 wdr				; every 5ms one Watchdog reset!
 rcall wait1ms	   	;1ms	6+x Return-Adresses
wait4ms:
 rcall wait1ms	   	;1ms	6+x Return-Adresses
wait3ms:
 rcall wait1ms	   	;1ms	6+x Return-Adresses
wait2ms:
 rcall wait1ms	   	;1ms	6+x Return-Adresses

wait1ms:
 rcall wait500us    	;500us	5+x Return-Adresses
wait500us:
 rcall wait100us    	;100us	4+x Return-Adresses
wait400us:
 rcall wait100us    	;100us	4+x Return-Adresses
wait300us:
 rcall wait100us    	;100us	4+x Return-Adresses
wait200us:
 rcall wait100us    	;100us	4+x Return-Adresses


# RCALL_TICS specify the number of tics used for rcall and ret instruction
#if FLASH_END > 0x1ffff
 /* special 3 byte return address takes two tics more */
 #define RCALL_TICS 9
#else
 #define RCALL_TICS 7
#endif
/* PS25_PER_TIC specify the number of 25ps units for one clock tic */
/* 40000000000 need more than 32 bit, so F_CPU is divided by 10 to match the 32-bit */
/* Note, that there is a little error of time delay, if 1/F_CPU does not match a multiple */
/* of a 25ps unit */
/* Normally the higher delay values are build with multiple of the base delay, */
/* so wait100us is done with 100 times of a wait1us call for example. */
/* But for F_CPU values, that does not match the 1us delay, corrections are done */
/* for the wait calls up to the wait100us call. The higher wait calls are not corrected. */
#define PS25_PER_TIC  (4000000000 / ((F_CPU + 5) / 10))
#define US100_TICS (4000000 / PS25_PER_TIC)
#define US50_TICS (2000000 / PS25_PER_TIC)
#define US40_TICS (1600000 / PS25_PER_TIC)
#define US30_TICS (1200000 / PS25_PER_TIC)
#define US20_TICS (800000 / PS25_PER_TIC)
#define US10_TICS (400000 / PS25_PER_TIC)
#define US5_TICS (200000 / PS25_PER_TIC)
#define US4_TICS (160000 / PS25_PER_TIC)
#define US3_TICS (120000 / PS25_PER_TIC)
#define US2_TICS (80000 / PS25_PER_TIC)
#define US1_TICS (40000 / PS25_PER_TIC)
#define NS500_TICS (20000 / PS25_PER_TIC)

wait100us:
#if US100_TICS > (2 * US50_TICS)
 nop
#endif
 rcall wait50us	    	; 50us delay
wait50us:
#if US50_TICS > (US40_TICS + US10_TICS)
 nop
#endif
 rcall wait10us     	;10us delay
wait40us:
#if US40_TICS > (US30_TICS + US10_TICS)
 nop
#endif
 rcall wait10us     	;10us delay
wait30us:
#if US30_TICS > (US20_TICS + US10_TICS)
 nop
#endif
 rcall wait10us     	;10us delay
wait20us:
#if US20_TICS > (2 * US10_TICS)
 nop
#endif
 rcall wait10us		;10us delay
wait10us:		;

#if US5_TICS >= RCALL_TICS
 #if US10_TICS > (2 * US5_TICS)
  nop
 #endif
 rcall wait5us
 ; at least 5us delay is possible
 #if US1_TICS >= RCALL_TICS
  ; a 1us delay is also possible
  #if NS500_TICS >= RCALL_TICS
wait5us:
   #if US5_TICS > (US4_TICS + US1_TICS)
    nop
   #endif
   rcall wait1us
wait4us:
   #if US4_TICS > (US3_TICS + US1_TICS)
    nop
   #endif
   rcall wait1us
wait3us:
   #if US3_TICS > (US2_TICS + US1_TICS)
    nop
   #endif
   rcall wait1us
wait2us:
   #if US2_TICS > (2 * US1_TICS)
    nop
   #endif
   rcall wait1us
wait1us:
   ; a 500ns delay is also possible with call
   #if US1_TICS > (2 * NS500_TICS)
    nop
   #endif
   rcall wait500ns
   .global wait500ns
wait500ns:
   #define INNER_TICS NS500_TICS
  #else
   #define INNER_TICS US1_TICS
wait5us:
   #if US5_TICS > (US4_TICS + US1_TICS)
    nop
   #endif
   rcall wait1us
wait4us:
   #if US4_TICS > (US3_TICS + US1_TICS)
    nop
   #endif
   rcall wait1us
wait3us:
   #if US3_TICS > (US2_TICS + US1_TICS)
    nop
   #endif
   rcall wait1us
wait2us:
   #if US2_TICS > (2 * US1_TICS)
    nop
   #endif
   rcall wait1us
wait1us:
  #endif
 #else
  ; wait5us is the inner body, 1us not possible
  #define INNER_TICS US5_TICS
wait5us:
wait4us:
wait3us:
  #if INNER_TICS < (RCALL_TICS * 2)
wait2us:
wait1us:
  #else
   #define LATE_2US
  #endif
 #endif
#else
; wait10us is the inner body
 #define INNER_TICS US10_TICS
wait5us:
wait4us:
wait3us:
wait2us:
wait1us:
#endif
; Now we have to build the inner loop with INNER_TICS clock tics.
; The rcall and ret take RCALL_TICS clock tics (7 or 9).
#if INNER_TICS < RCALL_TICS
 #error wait100ms can not build a function with 10us, F_CPU frequency too low!
#endif
;-----------------------------------------------------------------------------
#if INNER_TICS >= (RCALL_TICS * 4)
 #define WAST_TICS3 ((INNER_TICS / 4) - RCALL_TICS)
 ; Build two nested loops waitinner1 and waitinner2
wait5us:
 #if INNER_TICS > ((INNER_TICS/2) * 2) 
  nop
 #endif
 rcall waitinner2
waitinner2:
 #ifdef LATE_2US
wait2us:
 #endif
 #if ((INNER_TICS / 2) - (2*(RCALL_TICS+WAST_TICS3)))  > 0
  nop
 #endif
  rcall waitinner1
waitinner1:
 #ifdef LATE_2US
wait1us:
 #endif
 #if WAST_TICS3 > 1
  nop
 #endif
 #if WAST_TICS3 > 0
  nop
 #endif
#else
 ; ##################################  INNER_TICS < (RCALL_TICS * 4)
 #if INNER_TICS >= (RCALL_TICS * 2)
  #define WAST_TICS2 ((INNER_TICS - (2*RCALL_TICS)) / 2)
wait5us:
  #if (INNER_TICS - (2*WAST_TICS2)) > (2*RCALL_TICS)
    nop
  #endif
    rcall waitinner1
  #ifdef LATE_2US
wait2us:
wait1us:
  #endif
waitinner1:
 #else
  ; #################################  INNER_TICS < (RCALL_TICS * 2)
  #define WAST_TICS2 (INNER_TICS - RCALL_TICS)
 #endif
#endif
 #if WAST_TICS2 > 7
 #warning wait1000ms: WAST_TICS2 > 7, delay times may be wrong!
 #endif

 #if WAST_TICS2 >= 4
    push r1
    pop  r1
  #define WAST_TICS3 (WAST_TICS2 - 4)
 #else
  #define WAST_TICS3 WAST_TICS2
 #endif
 #if WAST_TICS3 > 2
   nop	
 #endif
 #if WAST_TICS3 > 1
    nop	
 #endif
 #if WAST_TICS3 > 0
    nop	
 #endif

 ret
