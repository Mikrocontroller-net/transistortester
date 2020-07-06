; you must inculde this file with the GNU C-Preprocessor instruction #include
; The #include should be placed before the serial input or serial output functions.

; The following macros must be set:
; F_CPU  = operating frequency of the AVR
; BAUD_RATE = desired Baud rate of serial output and/or input.
; LOOP_TICS = the count of loop-tics without the rcall uartDelay for one bit.
;		The loop-tics should be the same for both bit conditions (0 or 1).
;
; With this #include are some MACROS set:
; DOUBLE_DELAY_ADDR is set to the function name which wast time to get a full time bit length.
; SINGLE_DELAY_ADDR is set to the function name which wast time to get a half time bit length.
;			both names should be used with the serial bit output loop
; BIT_CLOCKS_0  is set to the total count of tics for the first delay loop generated.
; BIT_CLOCKS_1  is set to the total count of tics for the second delay loop generated.
; BIT_CLOCKS_2  is set to the total count of tics for the third delay loop generated.
; BIT_CLOCKS_3  is set to the total count of tics for the fourth delay loop generated.
;               This file will only generate a new loop, if the count of tics is different.
;
; The inner count loop can only count up to 255. If a lower Baud rate is requested,
; the inner loop call is automatically called 2x, 4x, 8x, 16x, 32x or 64x.
;

// AVR305 equation: #define UART_B_VALUE (((F_CPU/BAUD_RATE)-23)/6)

#include "set_loop_tics.h"	/* set LOOP_TICS and RCALL_TICS */
// loop requires 9 or 10 tics, rcall 7 or 9 tics

; Now we know all clocktics needed for generating the bit sequence,
; let's take a look, how many clock tics must be wasted with a loop to get the BAUD time.
#undef CLOCKS_PER_BIT
#undef DelayMul
#undef UART_B_VALUE
#undef UART_R_VALUE
#define DelayMul 1

#if BAUD_RATE >= 100

#define CLOCKS_PER_BIT ((F_CPU-((((2*RCALL_TICS+LOOP_TICS)*2-1)*BAUD_RATE)/2))/BAUD_RATE)
; CLOCKS_PER_BIT is now the total clock tics required for matching the BAUD time.
#if CLOCKS_PER_BIT < 0
 ; single delay is too slow, can not be used
 #undef CLOCKS_PER_BIT_NO_RCALL
 #define CLOCKS_PER_BIT_NO_RCALL ((F_CPU-(((LOOP_TICS*2-1)*BAUD_RATE)/2))/BAUD_RATE)
 #if CLOCKS_PER_BIT_NO_RCALL < 0
  #error " No additional delay to Soft_UART loop time"
 #endif
 #undef DOUBLE_DELAY_CALL
 #undef DOUBLE_DELAY_CALL_X1
 #undef DOUBLE_DELAY_CALL_X2
 #undef DOUBLE_DELAY_CALL_X3
 #if CLOCKS_PER_BIT_NO_RCALL >= RCALL_TICS
  ; double_delay can use a rcall
  #define DOUBLE_DELAY_CALL      rcall direct_return0
  #undef REST_TICS_DD
  #define REST_TICS_DD (CLOCKS_PER_BIT_NO_RCALL-RCALL_TICS)
 #elif CLOCKS_PER_BIT_NO_RCALL >= 2
  #define DOUBLE_DELAY_CALL   rjmp	.
  #undef REST_TICS_DD
  #define REST_TICS_DD  (CLOCKS_PER_BIT_NO_RCALL -2)
 #elif CLOCKS_PER_BIT_NO_RCALL == 1
  #define DOUBLE_DELAY_CALL	nop
  #undef REST_TICS_DD
  #define REST_TICS_DD 0
 #elif CLOCKS_PER_BIT_NO_RCALL == 0
  #define DOUBLE_DELAY_CALL	; no delay
  #undef REST_TICS_DD
  #define REST_TICS_DD 0
 #endif
 ; we can handle up to 6 additional tics (3* rjmp) for rest time
 #if REST_TICS_DD == 0
  #define DOUBLE_DELAY_CALL_X1   ;
  #define DOUBLE_DELAY_CALL_X2   ;
  #define DOUBLE_DELAY_CALL_X3   ;
 #elif REST_TICS_DD == 1
  #define DOUBLE_DELAY_CALL_X1   nop
  #define DOUBLE_DELAY_CALL_X2   ;
  #define DOUBLE_DELAY_CALL_X3   ;
 #elif REST_TICS_DD == 2
  #define DOUBLE_DELAY_CALL_X1   rjmp	.
  #define DOUBLE_DELAY_CALL_X2   ;
  #define DOUBLE_DELAY_CALL_X3   ;
 #elif REST_TICS_DD == 3
  #define DOUBLE_DELAY_CALL_X1   rjmp	.
  #define DOUBLE_DELAY_CALL_X2   nop
  #define DOUBLE_DELAY_CALL_X3   ;
 #elif REST_TICS_DD == 4
  #define DOUBLE_DELAY_CALL_X1   rjmp	.
  #define DOUBLE_DELAY_CALL_X2   rjmp	.
  #define DOUBLE_DELAY_CALL_X3   ;
 #elif REST_TICS_DD == 5
  #define DOUBLE_DELAY_CALL_X1   rjmp	.
  #define DOUBLE_DELAY_CALL_X2   rjmp	.
  #define DOUBLE_DELAY_CALL_X3   nop
 #elif REST_TICS_DD == 6
  #define DOUBLE_DELAY_CALL_X1   rjmp	.
  #define DOUBLE_DELAY_CALL_X2   rjmp	.
  #define DOUBLE_DELAY_CALL_X3   rjmp	.
 #else
   #error " Baud rate not supported by Soft-UART"
 #endif


 #undef TOTAL_TICS_SD
 #define TOTAL_TICS_SD (((CLOCKS_PER_BIT_NO_RCALL + LOOP_TICS) / 2) - 4)
 #undef SINGLE_DELAY_CALL
 #undef SINGLE_DELAY_CALL_X1
 #undef SINGLE_DELAY_CALL_X2
 #undef SINGLE_DELAY_CALL_X3
 #if TOTAL_TICS_SD < 0
  #error " Baud rate Not supported by Soft-UART < 0"
 #endif
 ; we can handle between 0 and 8 additional tics (4* rjmp) for single delay
 ; the longest rcall/ret is mega2560 with 9 tics!!!
 #if TOTAL_TICS_SD == 0
# dummy instructions
  #define SINGLE_DELAY_CALL     ;
  #define SINGLE_DELAY_CALL_X1  ;
  #define SINGLE_DELAY_CALL_X2  ;
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 1
  #define SINGLE_DELAY_CALL     nop
  #define SINGLE_DELAY_CALL_X1  ;
  #define SINGLE_DELAY_CALL_X2  ;
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 2
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  ;
  #define SINGLE_DELAY_CALL_X2  ;
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 3
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  nop
  #define SINGLE_DELAY_CALL_X2  ;
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 4
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  rjmp	. 
  #define SINGLE_DELAY_CALL_X2  ;
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 5
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  rjmp	. 
  #define SINGLE_DELAY_CALL_X2  nop
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 6
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  rjmp	. 
  #define SINGLE_DELAY_CALL_X2  rjmp	. 
  #define SINGLE_DELAY_CALL_X3  ;
 #elif TOTAL_TICS_SD == 7
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  rjmp	. 
  #define SINGLE_DELAY_CALL_X2  rjmp	. 
  #define SINGLE_DELAY_CALL_X3  nop
 #elif TOTAL_TICS_SD == 8
  #define SINGLE_DELAY_CALL     rjmp	.
  #define SINGLE_DELAY_CALL_X1  rjmp	. 
  #define SINGLE_DELAY_CALL_X2  rjmp	. 
  #define SINGLE_DELAY_CALL_X3  rjmp  .
 #else
  #error " Baud rate Not supported by Soft-UART"
 #endif
 #define BASE_DELAY_CALL no_delay

#else	
 /* CLOCKS_PER_BIT with rcall >= 0 */
 #define UART_B_VALUE ((CLOCKS_PER_BIT)/6)	/* this is the required loop counter */
 #define UART_R_VALUE ((CLOCKS_PER_BIT/2) - (UART_B_VALUE*3))	/* the remainder tics not serviced */
 #if UART_B_VALUE > 255
  // try with double rcall BASE_DELAY_CALL
  #undef CLOCKS_PER_BIT
  #undef UART_B_VALUE
  #undef UART_R_VALUE
  #undef DelayMul
  #define DelayMul 2
  #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+9)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
  #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
  #define UART_R_VALUE ((CLOCKS_PER_BIT/(2*DelayMul)) - (UART_B_VALUE*3))
  #define DelayMul 2
  #if UART_B_VALUE > 255
   // try with double double rcall uartDelay_2
   #undef CLOCKS_PER_BIT
   #undef UART_B_VALUE
   #undef UART_R_VALUE
   #undef DelayMul
   #define DelayMul 4
   #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+9)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
   #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
   #define UART_R_VALUE ((CLOCKS_PER_BIT/(2*DelayMul)) - (UART_B_VALUE*3))
   #if UART_B_VALUE > 255
    // try with double double rcall uartDelay_4
    #undef CLOCKS_PER_BIT
    #undef UART_B_VALUE
    #undef UART_R_VALUE
    #undef DelayMul
    #define DelayMul 8
    #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+9)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
    #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
    #define UART_R_VALUE ((CLOCKS_PER_BIT/(2*DelayMul)) - (UART_B_VALUE*3))
    #if UART_B_VALUE > 255
     // try with double double rcall uartDelay_8
     #undef CLOCKS_PER_BIT
     #undef UART_B_VALUE
     #undef UART_R_VALUE
     #undef DelayMul
     #define DelayMul 16
     #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+9)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
     #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
     #define UART_R_VALUE ((CLOCKS_PER_BIT/(2*DelayMul)) - (UART_B_VALUE*3))
     #if UART_B_VALUE > 255
      // try with double double rcall uartDelay_16
      #undef CLOCKS_PER_BIT
      #undef UART_B_VALUE
      #undef UART_R_VALUE
      #undef DelayMul
      #define DelayMul 32
      #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+9)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
      #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
      #define UART_R_VALUE ((CLOCKS_PER_BIT/(6*DelayMul)) - (UART_B_VALUE*3))
      #if UART_B_VALUE > 255
       // try with double double rcall uartDelay_32
       #undef CLOCKS_PER_BIT
       #undef UART_B_VALUE
       #undef UART_R_VALUE
       #undef DelayMul
       #define DelayMul 64
       #define CLOCKS_PER_BIT ( (F_CPU-(((((2*DelayMul)*RCALL_TICS+9)*2-1)*BAUD_RATE)/2) )/BAUD_RATE)
       #define UART_B_VALUE ((CLOCKS_PER_BIT)/(6*DelayMul))
       #define UART_R_VALUE ((CLOCKS_PER_BIT/(6*DelayMul)) - (UART_B_VALUE*3))
       #if UART_B_VALUE > 255
        #error "Baud rate too slow for soft UART"
       #endif
      #endif	/* DelayMul = 64 */
     #endif	/* DelayMul = 32 */
    #endif	/* DelayMul = 16 */
   #endif	/* DelayMul = 8 */
  #endif		/* DelayMul = 4 */
 #endif		/* DelayMul = 2 */
 ; We don't know, if any delay routine has the same time,
 ; so we will check first with the C-Preprocessor.
 #undef DOUBLE_DELAY_ADDR
 #undef SINGLE_DELAY_ADDR
 #undef DOUBLE_DELAY_CALL
 #define DOUBLE_DELAY_CALL_X1  ; xtra delay not required with loop
 #define DOUBLE_DELAY_CALL_X2  ; xtra delay not required with loop
 #define DOUBLE_DELAY_CALL_X3  ; xtra delay not required with loop
 #undef SINGLE_DELAY_CALL
 #define SINGLE_DELAY_CALL_X1  ; xtra delay not required with loop
 #define SINGLE_DELAY_CALL_X2  ; xtra delay not required with loop
 #define SINGLE_DELAY_CALL_X3  ; xtra delay not required with loop
 #undef BASE_DELAY_CALL
 #define NEW_DELAY_CODE_REQUIRED
 #if (CLOCKS_PER_BIT * DelayMul) == BIT_CLOCKS_0
  #define DOUBLE_DELAY_ADDR double_uartDelay_0
  #define SINGLE_DELAY_ADDR single_uartDelay_0
  #define DOUBLE_DELAY_CALL rcall 	double_uartDelay_0
  #define SINGLE_DELAY_CALL rcall 	single_uartDelay_0
  #define BASE_DELAY_CALL half_uartDelay_0
  #undef NEW_DELAY_CODE_REQUIRED
 #endif
 #if (CLOCKS_PER_BIT * DelayMul) == BIT_CLOCKS_1
  #define DOUBLE_DELAY_ADDR double_uartDelay_1
  #define SINGLE_DELAY_ADDR single_uartDelay_1
  #define DOUBLE_DELAY_CALL rcall 	double_uartDelay_1
  #define SINGLE_DELAY_CALL rcall 	single_uartDelay_1
  #define BASE_DELAY_CALL half_uartDelay_1
  #undef NEW_DELAY_CODE_REQUIRED
 #endif
 #if (CLOCKS_PER_BIT * DelayMul) == BIT_CLOCKS_2
  #define DOUBLE_DELAY_ADDR double_uartDelay_2
  #define SINGLE_DELAY_ADDR single_uartDelay_2
  #define DOUBLE_DELAY_CALL rcall 	double_uartDelay_2
  #define SINGLE_DELAY_CALL rcall 	single_uartDelay_2
  #define BASE_DELAY_CALL half_uartDelay_2
  #undef NEW_DELAY_CODE_REQUIRED
 #endif
 #if (CLOCKS_PER_BIT * DelayMul) == BIT_CLOCKS_3
  #define DOUBLE_DELAY_ADDR double_uartDelay_3
  #define SINGLE_DELAY_ADDR single_uartDelay_3
  #define DOUBLE_DELAY_CALL rcall 	double_uartDelay_3
  #define SINGLE_DELAY_CALL rcall 	single_uartDelay_3
  #define BASE_DELAY_CALL half_uartDelay_3
  #undef NEW_DELAY_CODE_REQUIRED
 #endif
 ;
 #ifdef NEW_DELAY_CODE_REQUIRED
 ; let's take a look for a free code generating slot
  #if BIT_CLOCKS_0 == 0
   #define DOUBLE_DELAY_ADDR double_uartDelay_0
   #define SINGLE_DELAY_ADDR single_uartDelay_0
   #define BASE_DELAY_CALL half_uartDelay_0
   #define BIT_CLOCKS_0 (CLOCKS_PER_BIT * DelayMul)
  #elif BIT_CLOCKS_1 == 0
   #define DOUBLE_DELAY_ADDR double_uartDelay_1
   #define SINGLE_DELAY_ADDR single_uartDelay_1
   #define BASE_DELAY_CALL half_uartDelay_1
   #define BIT_CLOCKS_1 (CLOCKS_PER_BIT * DelayMul)
  #elif BIT_CLOCKS_2 == 0
   #define DOUBLE_DELAY_ADDR double_uartDelay_2
   #define SINGLE_DELAY_ADDR single_uartDelay_2
   #define BASE_DELAY_CALL half_uartDelay_2
   #define BIT_CLOCKS_2 (CLOCKS_PER_BIT * DelayMul)
  #elif BIT_CLOCKS_3 == 0
   #define DOUBLE_DELAY_ADDR double_uartDelay_3
   #define SINGLE_DELAY_ADDR single_uartDelay_3
   #define BASE_DELAY_CALL half_uartDelay_3
   #define BIT_CLOCKS_3 (CLOCKS_PER_BIT * DelayMul)
  #else
   #error "Too many different BAUD delays requested by this configuration!"
  #endif
 ;--------------------------------------------------------------------------------------
 ; the function DOUBLE_DELAY_ADDR calls the function uartDelay twice to get
 ; a total delay time of the requested BAUD time.
DOUBLE_DELAY_ADDR :
	rcall	SINGLE_DELAY_ADDR	; after first call runs to uartDelay and returns to DOUBLE_DELAY_CALL 
 ;--------------------------------------------------------------------------------------
 ; The function uartDelay make a total delay time of the half BAUD time.
  #if DelayMul > 1
	; set the single delay call to the top
SINGLE_DELAY_ADDR :
  #endif
  #if DelayMul >= 64
 ; The time limit for uartDelay_32 has been reached,
 ; so  the function uartDelay_32 is called twice to get the requested delay.
	rcall	.+0		; double the time of 32x again
  #endif
  #if DelayMul >= 32
 ; The time limit for uartDelay_16 has been reached,
 ; so  the function uartDelay_16 is called twice to get the requested delay.
	rcall	.+0		; double the time of 16x again
  #endif
  #if DelayMul >= 16
 ; The time limit for uartDelay_8 has been reached,
 ; so  the function uartDelay_8 is called twice to get the requested delay.
	rcall	.+0		; double the time of 8x again
  #endif
  #if DelayMul >= 8
 ; The time limit for uartDelay_4 has been reached,
 ; so  the function uartDelay_4 is called twice to get the requested delay.
	rcall	.+0		; double the time of 4x again
  #endif
  #if DelayMul >= 4
 ; The time limit for BASE_DELAY_CALL  and SINGLE_DELAY_ADDR has been reached,
 ; so  the function uartDelay_2 is called twice to get the requested delay.
	rcall	.+0		; double the time  of 2x again
  #endif
  #if DelayMul >= 2
 ; The time limit for BASE_DELAY_CALL has been reached,
 ; so  the function BASE_DELAY_ADDR is called twice to get the requested delay.
	rcall	BASE_DELAY_CALL
  #endif
 ; rcall BASE_DELAY_CALL take RCALL_TICS + UART_B_VALUE*3 + UART_R_VALUE  clock tics
 ; use r25 for counting
  #if ((CLOCKS_PER_BIT/2) * 2) < CLOCKS_PER_BIT
	nop
  #endif
  #if DelayMul == 1
	; set the single delay call to the basic half delay behind the nop
SINGLE_DELAY_ADDR :
  #endif
BASE_DELAY_CALL :
  #if UART_B_VALUE > 0
	ldi	r25, UART_B_VALUE

	dec	r25		; important: dec don't change the carry!
	brne	.-4
  #endif
 ; With UART_B_VALUE we can match the half bit period with multiples of 3 clock tics.
 ; We can add one or two clock tics for better match the required time
  #if UART_R_VALUE == 2
	rjmp	.	; one additional instruction with 2 clock tics and no function
  #endif
  #if UART_R_VALUE == 1
	nop		; one additional instruction with 1 clock tic
  #endif
	ret
 #endif		/* NEW_DELAY_CODE_REQUIRED  */
#endif		/* else CLOCKS_PER_BIT < 0) */

#else		/* BAUD_RATE < 100 */
 ;
 ; with the Auto-Baud function we use allways the same delay function
  #if !defined(BIT_CLOCKS_A)
   #undef DOUBLE_DELAY_CALL
   #undef DOUBLE_DELAY_CALL_X1
   #undef DOUBLE_DELAY_CALL_X2
   #undef DOUBLE_DELAY_CALL_X3
   #undef SINGLE_DELAY_CALL
   #undef SINGLE_DELAY_CALL_X1
   #undef SINGLE_DELAY_CALL_X2
   #undef SINGLE_DELAY_CALL_X3
   #define DOUBLE_DELAY_CALL rcall	double_uartDelay_a
   #define DOUBLE_DELAY_CALL_X1   ;
   #define DOUBLE_DELAY_CALL_X2   ;
   #define DOUBLE_DELAY_CALL_X3   ;
   #define SINGLE_DELAY_CALL rcall	single_uartDelay_a
   #define SINGLE_DELAY_CALL_X1  ;
   #define SINGLE_DELAY_CALL_X2  ;
   #define SINGLE_DELAY_CALL_X3  ;
   #define BIT_CLOCKS_A 1
double_uartDelay_a:
	rcall	single_uartDelay_a	; after first call runs to uartDelay and returns to DOUBLE_DELAY_CALL 
   #if ((BAUD_RATE - ((BAUD_RATE/2)*2)) != 0)
   // slow 15 bit loop, 5 tics per count
single_uartDelay_a:
	mov	r25, r10	; use the count from auto-baud in r10:11
	mov	r0, r11		; use r0 for upper bits
    #if (TICS_PER_LOOP_PASS == 8) || (TICS_PER_LOOP_PASS == 6)
count_lower:
     #if TICS_PER_LOOP_PASS == 8
	rjmp	.		; +2 =2
     #endif
	nop			; +1 =3
	dec	r25		; +1 =4
	brne	next_lower	; +1,+2 =5 / 6
	dec	r0		; +1,  = 6 higher bits
	brpl	count_lower	; +1,+2 = 8
	ret			; lower bits are counted to 0 and higher bits are -1
next_lower:
	rjmp	count_lower	; +2 =8
    #else
        ; normal 15 bit loop with TICS_PER_LOOP_PASS 5
count_lower:
	dec	r25		; +1  =1
	brne	next_lower	; +1,+2 = 2 / 3
	dec	r0		; +1, = 3 higher bits
	brpl	count_lower	; +1,+2  =5
	ret			; lower bits are counted to 0 and higher bits are -1
next_lower:
	rjmp	count_lower	; +2 = 5
    #endif
   #else
   // fast 8 bit loop, 3 tics per count
single_uartDelay_a:
	mov	r25, r10	; use the count from auto-baud in r10
    #if TICS_PER_LOOP_PASS == 4		/* special case 9 Bit measurement and clk/8 Counter */
        nop
	dec	r25		; important: dec don't change the carry!
	brne	.-6
    #else
	dec	r25		; important: dec don't change the carry!
	brne	.-4
    #endif
	ret
   #endif
  #endif
#endif		/* BAUD_RATE < 100 */
