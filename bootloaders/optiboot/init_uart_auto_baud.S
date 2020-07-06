 ; init_uart_auto_baud is part of optiboot 
 ; designed by K.-H. Kuebbeler
 ; based on an idea and code from A. Watterott
 ;
 ; This part of assembler code tries to detect the
 ; STK_GET_SYNC (0x30 = 0b00110000) at the RX PIN,
 ; which should be the first character received
 ; use register r10:r11, r12:r13, r22:r23, r24:r25, r26:r27, r28:r29

; Settings for dignostic
//#define SHOW_BAUD 11		/* output of LSB Baud at PORTA if 1, PORTB if 2 PORTC if 3 */
;				/* 1,2,3 show the LSB UBRR setting for hardware UART */
;				/*  and the LSB bits for software UART delay loop */
;				/* 11,12,13 show the LSB bits of the time measurement */
;				/*  source (2 or 4 bits) for the baud setting */
#define SHOW_BAUD_MSK 0xff	/* with this mask you can exclude bits for output */

#ifdef SHOW_BAUD
 #warning "Debug output of the lower bits of baud rate measurement to port "
#endif

#define TWO_STOP_BITS 1		/* 1 = output with two Stop bits to Tx */
 ; The divide operation is done with shift, is possible (/2 /4 /8 ...)
 ; Other divides  (/6, /9, /10 are done with subtracts in a loop
 ; This give a small Program code, but the operation is slow, so that the
 ; computation isn't finished in the stop bit time. So the next character
 ; is lost for this type of auto baud calculation.
 ; A workaround is to subtract the double value in a loop and increase the
 ; div counter by 2 instead of 1. The lowest bit of the div counter
 ; can be get by analysing the remainder of the subtracts.
 ; You can force this type of computation by setting the last digit if the baud rate
 ; not to 0 or 5. Any other digit will use the faster subtract loop or shift.
 ; Even if the FastDiv=1 can spare some bytes of code, you should remember,
 ; that FastDiv=1 is only possible with 2 Stop Bits from the sender of the serial data (avrdude).

#include "set_loop_tics.h"	/* required for SOFT_UART */

#if defined(UART_SRC) && defined(UART_SEL)
 ; version for m8, m16, m32 for example,
 ; which have double function for register UART_SRC (UCSRC).
 ; This processor family has no double register for the 12-bit scaler.
 ; Without the bit UART_SEL (URSEL) set in the UART_SRC (UCSRC) register
 ; we can write the upper BAUD_DIV to register UART_SRC
 #undef UART_SRRH
 #define UART_SRRH UART_SRC
#endif
#if SOFT_UART == 100	/* disable early setting of TX pin to Stop Level */
   ; ###### TEST to initiate the TX output before the UART is enabled
  #if INVERSE_UART > 0
	ACBI	UART_TX_PORT, UART_TX_BIT	; set TX bit to low
  #else
	ASBI	UART_TX_PORT, UART_TX_BIT	; set TX bit to high
  #endif
	ASBI	UART_TX_DDR, UART_TX_BIT	; set TX DDR bit as output
#endif

next_try:
#if ((LED_DATA_FLASH & 0x01) > 0) && defined(LED_PORT) && defined(LEDbit)
	ASBI	LED_PORT, LEDbit
#endif
	wdr				; watchdog timer reset
wt_rx0low:
#if INVERSE_UART > 0
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
#else
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
#endif
	rjmp	wt_rx0low			; wait for at least one 1 read
RX_was_high:

#if ((BAUD_RATE - ((BAUD_RATE/10)*10)) < 5)
 #define START_COUNTER_COMMAND  (1<<CS11)	/* run counter at F_CPU/8 speed */
 #define CounterClockDiv 8
#else
 ; Full speed measurement, selected by BAUD_RATE or by SOFT_UART
 #define START_COUNTER_COMMAND  (1<<CS10)	/* run counter at full F_CPU speed */
 #define CounterClockDiv 1
#endif

#if (BAUD_RATE < 80) && (BAUD_RATE > 19)
 #if ((BAUD_RATE - ((BAUD_RATE/20)*20)) > 9)
  #define Bits2Measure  4
 #else
  #define Bits2Measure  2
 #endif
#else	/* BAUD_RATE >= 80  || BAUD_RATE < 20 */
 #undef Bits2Measure
 #define Bits2Measure 9
#endif

#if Bits2Measure == 2
 #if CounterClockDiv == 1
  #warning "The use of 4-Bit time measurement is recommended!"
 #else
  #warning "Measurement of 2-Bit-time with /8 counter is not reliable, you should select 4-Bit"
 #endif
#endif

#define Remainder5 (BAUD_RATE - ((BAUD_RATE/5)*5))
#if Remainder5 == 0
 #define FastDiv 1	/* Use the single subtract for slow divide loop */
#else
 #define FastDiv 2	/* Use the double subtract for divide loop */
#endif

 ; the macro ASBIW use a sbiw instruction, if possible, otherwise two instructions subi,sbic are used
 .macro ASBIW reg, incre
  .if	\incre < 64
	sbiw	\reg, \incre
  .else
	subi	\reg, lo8(\incre)
        sbci	(\reg+1), hi8(\incre)
  .endif
 .endm
 .macro REMCHK	resreg, divid, divreg
    #if FastDiv != 1
	; analyse the remainder of the subtract loop
	subi	\resreg, (-\divid)	; + TicsPer N LoopPass
	brcs	nokorr
     #if ((BAUD_RATE - ((BAUD_RATE/2)*2)) != 0)
	adiw	\divreg, 1
     #else
	subi	\divreg, -1
     #endif
nokorr:
    #else
	; nothing to do with remainder
    #endif
 .endm

#if SOFT_UART > 0
 #if ((BAUD_RATE - ((BAUD_RATE/2)*2)) != 0)
  #if (Remainder5 > 2)
   // xtra slow 15 bit loop, 8 tics per count
   #define TICS_PER_LOOP_PASS 8
  #else
   // slow 15 bit loop, 5 tics per count
   #define TICS_PER_LOOP_PASS 5
  #endif
  #define XTRA_TIC 1
  ; the macros LDDIV, ADDIV, SBDIV use two registers for 15-Bit loop counter setup!
  .macro LDDIV reg, start
	ldi	\reg, lo8(\start)
	ldi	(\reg+1), hi8(\start)
  .endm
  .macro ADDIV reg, incre
	adiw	\reg, \incre
  .endm
  .macro SBDIV reg, incre
	sbiw	\reg, \incre
  .endm
 #else
  // fast 8 bit loop, 3 tics per count
  #define XTRA_TIC 0
  #if ((Bits2Measure == 9) && (CounterClockDiv == 8)) || (Remainder5 > 2)
   #define TICS_PER_LOOP_PASS 4		/* special case 9 Bit measurement and clk/8 Counter */
  #else
   #define TICS_PER_LOOP_PASS 3
  #endif
   ; the macros LDDIV, ADDIV, SBDIV use a 8-Bit register, if a 8-Bit loop counter is used!
  .macro LDDIV reg, start
        ldi     \reg, lo8(\start)
  .endm
  .macro ADDIV reg, incre
	subi	\reg, -\incre
  .endm
  .macro SBDIV reg, incre
	subi	\reg, \incre
  .endm

 #endif		/* BAUD_RATE/2*2 */
#else    /* use Hardware UART */
  ; the macros LDDIV, ADDIV, SBDIV use two registers for 12-Bit UART setup!
  .macro LDDIV reg, start
	ldi	\reg, lo8(\start)
	ldi	(\reg+1), hi8(\start)
  .endm
  .macro ADDIV reg, incre
	adiw	\reg, \incre
  .endm
  .macro SBDIV reg, incre
	sbiw	\reg, \incre
  .endm
#endif		/* SOFT_UART */

#ifdef TCNT1H
 #define CNT16_CNTH  TCNT1H
 #define CNT16_CNTL  TCNT1L
 #define CNT16_CCRB  TCCR1B
 #define TOVFLAG TOV1

#elif defined(TCNT0H)
 #define CNT16_CNTH  TCNT0H
 #define CNT16_CNTL  TCNT0L
 #define CNT16_CCRB  TCCR0B
 #define TOVFLAG TOV0
	ldi	r24, (1<<TCW0)		; switch counter 0 to 16-Bit (tiny861)
	AOUT	TCCR0A, r24

#else 
   #error "AutoBaud Function is only possible with 16-Bit counter 1!"
#endif

#if !defined(TIFR1) && defined(TIFR)
 #define TIFR1 TIFR
#endif


#if SOFT_UART == 0
	ldi	r24, UART_SCALER8	; double speed mode
	AOUT	UART_CNTL, r24		; set UART scaler to 8, double speed
   ; prepare the UART
 #if defined(UART_SRC) && defined(UART_SEL)
	; version for m8, m16, m32 for example (with double function UCSRC)
	ldi	r23, (1<<UART_SEL)|(TWO_STOP_BITS<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)	;config UART U8N2
	AOUT	UART_SRC, r23
 #else
    /* no ATmega8_16_32 */
  #if !defined(__AVR_ATmega163__) && !defined(__AVR_ATtiny87__) && !defined(__AVR_ATtiny167__)
	ldi	r23, (TWO_STOP_BITS<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01)	;config UART U8N2
	AOUT	UART_SRC, r23
  #endif
 #endif	/* defined(UART_SRC) && defined(UART_SEL) */
#endif

  ; use the 16-Bit counter to measure Baud time
  ; prepare the counter 1
	AOUT	CNT16_CCRB, r1		; Stop counter 1 
	AOUT	CNT16_CNTH, r1		; set initial counter to 0
	AOUT	CNT16_CNTL, r1

#if BAUD_RATE < 60
  ; simple detection of the baud-rate
; wait, if the UART-RX-Bit is low
	ldi	r24, START_COUNTER_COMMAND	; start command for counter with /8 scaler or full speed
wt_rx0s:
 #if INVERSE_UART > 0
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low, INVERSE
 #else
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #endif
	rjmp	wt_rx0s
	; Probably start bit is found, at least a 1 to 0 transition.
	; STK500 protocol send a '3'=0x30 followed by ' '=0x20.
	; P is a pause condition (STOP bit = 1) and S is a START bit (0), 
	; the 0 and 1 represent the expected data bits.
	; the full bit sequence send is PS00001100PS00000100PPP
	;                                ^      ^  ^      ^
	;                                1      2  3      4
	; Every of the marked postion is a 1 to 0 transition!
	; If we begin now with the time measurement, we get the following result
	; for the next two bit changes:
	; Position 1:  5B, 7B (,9B)
	; Position 2:  2B, 3B+d (,9B+d)
	; Position 3:  6B, 7B (,9B)
	; Position 4:  2B, 3B+Delay
	; B is the unknown Baud time and d can be a short delay of the next Start-bit.
	; The Delay is a long time delay to the next transmission retry,
	; if the sender get no answer.
	; If we begin the time measurement at the next transition, we
	; get only the following time results:
	; Position 1: 2B (,4B)
	; Position 2: 1B+d (,6B+d)
	; Position 3: 1B  (,3B)
	; Position 4: 1B+Delay
	; I see no easy way to prevent the wrong baud-rate detection for each
	; of the possible start conditions. So we check here only a upper time limit
	; for the position 4 for saving flash memory (BAUD_RATE > 39).
	; A full plausibility check is done for a Boot-Page size of more than 512 Byte.

	; begin of Start bit is found 
 #if (Bits2Measure == 9) 
	AOUT	CNT16_CCRB, r24		; Start counter 1 with or without /8 scaler for 9-Bit measure
 #endif
wt_rx1s:
 #if INVERSE_UART > 0
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #else
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #endif
	rjmp	wt_rx1s
	; begin of first "1" data bit is found after S0000, if it was a STK_GET_SYNC, 0x30
 #if (Bits2Measure == 2) || (Bits2Measure == 4)
	AOUT	CNT16_CCRB, r24		; Start counter 1 with or without /8 scaler for 2 or 4 Bit measure
 #endif

 #if BAUD_RATE > 39
	; the optional time-limit check is only done, if the BAUD_RATE is set > 39 
  #if TIFR1 < (0x20 + __SFR_OFFSET)
	sbi	_SFR_IO_ADDR(TIFR1), TOVFLAG	; Clear Timer OV Flag
  #else
	ldi	r26, (1<<TOVFLAG)
	AOUT	TIFR1, r26		; clear OV-Flag
  #endif
 #endif	/* BAUD_RATE > 39 */

wt_rx0bs:
 #if BAUD_RATE > 39
	ASBIC	TIFR1, TOVFLAG	; Look if time limit is reached, OV Flag
	rjmp	next_try		; upper limit for 2x Baud-time is reached
 #endif
 #if INVERSE_UART > 0
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #else
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #endif
	rjmp	wt_rx0bs

 #if Bits2Measure == 2
	AIN	r24, CNT16_CNTL		; read counter after second '1' bit is finished, 2-Bit time
	AIN	r25, CNT16_CNTH
 #endif

 ; Transmission of byte is not finished, we must wait for last two "0" Bits
wt_rx1bs:
 #if INVERSE_UART > 0
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #else
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #endif
	rjmp	wt_rx1bs

 #if (Bits2Measure == 4) || (Bits2Measure == 9)
	AIN	r24, CNT16_CNTL		; read counter after second '0' bit is finished, 4-Bit or 9-Bit time
	AIN	r25, CNT16_CNTH
 #endif

 ; simple measurement is finished, we have the time of 2, 4 or 9 bits
 #if SHOW_BAUD == 11
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r24		; show the lower bits, if requested
 #endif
 #if SHOW_BAUD == 12
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r24		; show the lower bits, if requested
 #endif
 #if SHOW_BAUD == 13
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r24		; show the lower bits, if requested
 #endif

 #if SOFT_UART > 0		/* determine baud rate after simple check */

  #if (Bits2Measure == 2) || (Bits2Measure == 4)
   #if Bits2Measure == 2
    #if CounterClockDiv != 1
     #warning "optiboot can get the baud rate better with full speed counter or 4-bit mode for Software-UART!"
    #endif
    #if (CounterClockDiv != 1) && ((TICS_PER_LOOP_PASS - ((TICS_PER_LOOP_PASS/4)*4)) != 0)
	; simple measurement, we have measured the count of cpu-tics for two RX bits
	add	r24,r24
	adc	r25,r25		; we double the time for better calculation

     #define TicsPerNLoopPass ((TICS_PER_LOOP_PASS*2*4)/CounterClockDiv)
     #define MinTics4Bit ((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*2*2)/CounterClockDiv-(TicsPerNLoopPass/2)
    #else     /* CounterClockDiv == 1 */
	; simple measurement, we have measured the count of cpu-tics for two RX bits
     #define TicsPerNLoopPass ((TICS_PER_LOOP_PASS*2*2)/CounterClockDiv)
     #define MinTics4Bit (((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*2)/CounterClockDiv-(TicsPerNLoopPass/2))
    #endif       /* CounterClockDiv */
   #elif Bits2Measure == 4
	; simple measurement, we have measured the count of cpu-tics for four RX bits
    #define TicsPerNLoopPass (TICS_PER_LOOP_PASS*2*4/CounterClockDiv)
    #define MinTics4Bit ((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*4)/CounterClockDiv-(TicsPerNLoopPass/2)
   #endif
        ASBIW	24, MinTics4Bit
   #if (TicsPerNLoopPass == 2)
	movw	r10, r24
	lsr	r10
	ror	r11
   #elif (TicsPerNLoopPass == 4)
	movw	r10, r24
	lsr	r10
	ror	r11
	lsr	r10
	ror	r11
   #elif (TicsPerNLoopPass == 8)
	movw	r10, r24
	ldi	r27, 3		; divide by 8 can be done with 3 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #elif (TicsPerNLoopPass == 16)
	movw	r10, r24
	ldi	r27, 4		; divide by 16 can be done with 4 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #elif (TicsPerNLoopPass == 32)
	movw	r10, r24
	ldi	r27, 5		; divide by 32 can be done with 5 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #else
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop2:
	ADDIV	r28, FastDiv		; 15 bit delay loop will be used
	ASBIW	24, (FastDiv*TicsPerNLoopPass)	; 2 loop calls for every Tx bit, /4 for simulated 4 bit measured
	brsh	divlop2
	REMCHK   r24,TicsPerNLoopPass,r28
	movw	r10, r28
   #endif	/* TicsPerNLoopPass */
  #endif	/* Bits2Measure == 2  || Bits2Measure == 4 */

  #if Bits2Measure == 9
	; full check, we use a nine bit time (S00001100) for baud-rate calculation.
	; simple check, we have measured the count of cpu-tics for nine RX bits
   #define TicsPer9LoopPass (((TICS_PER_LOOP_PASS*2*9)+(CounterClockDiv/2))/CounterClockDiv) /* 2 loop calls for every Tx bit, /9 for 9 bit time */
   #define MinTics9Bit ((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*9)/CounterClockDiv-(TicsPer9LoopPass/2)

 	ASBIW	24, MinTics9Bit
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	; speed up the subtract loop for div by nearly factor 2 by double subtract
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop9:
	ADDIV	r28, FastDiv
	ASBIW	24, (FastDiv*TicsPer9LoopPass)	; 2 loop calls for every Tx bit, /9 for 9 bit time
	brsh	divlop9
	REMCHK   r24,TicsPer9LoopPass,r28
	movw	r10, r28		; r10:11 hold the delay loop counter
   ; result of 1 will make the single delay return after one pass (TICS_PER_LOOP_PASS), 0=long and wrong delay
  #endif	/* Bits2Measure == 9 */


  #if SHOW_BAUD == 1
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r10		; show the lower bits, if requested
  #endif
  #if SHOW_BAUD == 2
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r10		; show the lower bits, if requested
  #endif
  #if SHOW_BAUD == 3
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r10		; show the lower bits, if requested
  #endif

 #else		/* for Hardware UART */

  #if Bits2Measure == 2
	; because we have measured the time of two bits,
	; we must divide by 2 and subtract 1 to get the final UBRR
	; ((two_bit_time + 1) / 2) - 1  is equal to
	; (two_bit_time - 1) / 2  
   #if CounterClockDiv == 1
    #if (BAUD_RATE - ((BAUD_RATE/20)*20) < 10)
	sbiw	r24, 12		; (two_bit_time - 12) / 16, faster selection than correct round up
    #else
	sbiw	r24, 15		; (two_bit_time - 15) / 16, faster selection than correct round up
    #endif
	ldi	r28, 4		; divide by 16 can be done with 4 shifts in a loop
div2lop1:
	lsr	r25
	ror	r24
	dec	r28
	brne	div2lop1
   #else	/* measured with /8 counter clock */
	sbiw	r24, 1		; (two_bit_time - 1) / 2 , divide by two need only one shift
	lsr	r25
	ror	r24		; rotate through carry
   #endif	/* CounterClockDiv */
	; now we can start the UART
	AOUT	UART_SRRH, r25
	AOUT	UART_SRRL, r24		; set the UART divider
  #endif	/* Bits2Measure == 2 */

  #if Bits2Measure == 4
	; because we have measured the time of FOUR bits,
	; we must divide by 4 and subtract 1 to get the final UBRR
	; ((two_bit_time + 2) / 4) - 1  is equal to
	; (two_bit_time - 2) / 4  
   #if CounterClockDiv == 1
;	sbiw	r24, 16		; (four_bit_time - 16) / 32
    #if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
	sbiw	r24, 28		; (four_bit_time - 28) / 32, faster selection than correct round up
    #elif defined(__AVR_ATmega162__)
	sbiw	r24, 19		; (four_bit_time - 19) / 32, faster selection than correct round up
    #else
	sbiw	r24, 24		; (four_bit_time - 24) / 32, faster selection than correct round up
    #endif
	ldi	r28, 5		; divide by 32 is done with 5 shifts in a loop
div4lop1:
	lsr	r25
	ror	r24
	dec	r28
	brne	div4lop1
   #else	/* measured with /8 counter clock */
;	sbiw	r24, 2		; (two_bit_time - 2) / 4
	sbiw	r24, 3		; (two_bit_time - 3) / 4, faster selection than with correct round up
				; divide by 4 can be done with two shifts without a loop
	lsr	r25
	ror	r24		; rotate through carry
	lsr	r25
	ror	r24		; rotate through carry
   #endif	/* CounterClockDiv */
	; now we can start the UART
	AOUT	UART_SRRH, r25
	AOUT	UART_SRRL, r24		; set the UART divider
  #endif	/* Bits2Measure == 4 */

  #if Bits2Measure == 9

	; full check, we have allready the time of 9 Bit (S0001100) in r24:25
	; because we have measured the time of four bits,
	; we must divide by 9 to get the final UBRR
	; ((nine_bit_time + 5) / 9) - 1  is equal to
	; (nine_bit_time - 4) / 9 
    #define MinTics9Bit (9*8/CounterClockDiv)
	sbiw	r24, (4*8/CounterClockDiv)	; (nine_bit_time - (4*8)) / (8*9), faster selection than correct round up
	; speed up the subtract loop for div with double value subtracts
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop9f:
	ADDIV	r28, FastDiv
	ASBIW	24, (FastDiv*MinTics9Bit)		; divide by 9 by subtract
	brsh	divlop9f
	REMCHK	r24,MinTics9Bit,r28
	; now we can start the UART
	AOUT	UART_SRRH, r29
	AOUT	UART_SRRL, r28		; set the UART divider
   #if (SHOW_BAUD > 0) && (SHOW_BAUD <= 3)
	movw	r24, r28
   #endif
  #endif	/* Bits2Measure == 9 */


  #if SHOW_BAUD == 1
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r24		; show the lower bits, if requested
  #endif
  #if SHOW_BAUD == 2
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r24		; show the lower bits, if requested
  #endif
  #if SHOW_BAUD == 3
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r24		; show the lower bits, if requested
  #endif

 #endif		/* SW / HW UART */
 #undef DELAY_NEXT_REQUEST
#else		/* BAUD_RATE >= 60 */

  ;------------------------------------------------------------------------------------
  ; you assume enough space for exact analye for BAUD_RATE >= 60
	ldi	r24, START_COUNTER_COMMAND	; start command for counter with /8 scaler or full speed
wt_rx0l:
 #if INVERSE_UART > 0
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #else
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #endif
	rjmp	wt_rx0l
	; Probably start bit is found, at least a 1 to 0 transition.
	; STK500 protocol send a '3'=0x30 followed by ' '=0x20.
	; P is a pause condition (STOP bit = 1) and S is a START bit (0). 
	; the full bit sequence send is PS00001100PS00000100PPP
	;                                ^      ^  ^      ^
	;                                1      2  3      4
	; Every of the marked postion is a 1 to 0 transition!
	; We start the counter to measure the time of the next 3 bit changes.
	; This result to 5B, 7B and 9B time measurements for the marked "1" transition,
	; where B is the baud time.
	; For the "2" transition you get 2B+d, 3B+d and 9B+d for the time measurement,
	; where d is a possible delay of the next start bit.
	; For the "3" transition you get 6B, 7B and 9B as time measurement result.
	; For the "4" transition you get 2B and a unassigned result for the following
	; measurements, because the Transmitter wait for a reply.
	; The best way to solve this problem is to monitor the counter time for
	; exceed of a 4*(2B) limit.

	AOUT	CNT16_CCRB, r24		; Start counter 1 with /8 scaler, begin measurement
wt_rx1l:
 #if INVERSE_UART > 0
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #else
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #endif
	rjmp	wt_rx1l
	; now we should have start-bit and 4 data bits 0
	AIN	r10, CNT16_CNTL			; read counter after 'S0000'
	AIN	r11, CNT16_CNTH
 #if TIFR1 < (0x20 + __SFR_OFFSET)
	sbi	_SFR_IO_ADDR(TIFR1), TOVFLAG	; reset OV Flag
 #else
	ldi	r26, (1<<TOVFLAG)		; reset OV Flag
	AOUT	TIFR1, r26
 #endif

wt_rx0bl:
	ASBIC	TIFR1, TOVFLAG	; Look if time limit is reached
	rjmp	next_try		; 4x first time must be greater than actual time
 #if INVERSE_UART > 0
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #else
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #endif
	rjmp	wt_rx0bl
	; now we should have start-bit + 4 data 0 bits + 2 date 1 bits
	AIN	r12, CNT16_CNTL			; read counter after 'S000011'
	AIN	r13, CNT16_CNTH

 ; Transmission of byte is not finished, we must wait for last two "0" Bits
wt_rx1bs:
 #if INVERSE_UART > 0
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #else
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #endif
	rjmp	wt_rx1bs
	AIN	r14, CNT16_CNTL		; read counter after 'S00001100', full 8 Bit transmission with Start bit
	AIN	r15, CNT16_CNTH

;	All time measurements are done for full check
;       =============================================
	movw	r24, r14
	sub	r24, r10		; S00001100-S0000 = 1100 time
	sbc	r25, r11		; r24:r25 used for 4-Bit time

	cp	r24, r10		; 1100 < S0000  ?
	cpc	r25, r11
	brcc	delay_next_try		; No, 1100 > S0000, get Next Data
	movw	r26, r14		; S00001100
	sub	r26, r12		; build time diff  S00001100 - S000011 = 00
	sbc	r27, r13		; r26:27 used for 2-Bit time

	movw	r28, r12		; S000011
	adiw	r28, 32/CounterClockDiv			; 
	sub	r28, r10		; build time diff  S000011 - S0000 = 11
	sbc	r29, r11
	cp	r26, r28		; 00 < (11 + d) ?
	cpc	r27, r29
	brcc	to_next_try		; No, 00 > (11 + d), get Next Data

 #if SOFT_UART > 0
  #if Bits2Measure == 4
	; full check, we use a four bit time (1100) for baud-rate calculation.

   #if SHOW_BAUD == 11
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRA, r28
	AOUT    PORTA, r24
   #endif
   #if SHOW_BAUD == 12
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRB, r28
	AOUT    PORTB, r24
   #endif
   #if SHOW_BAUD == 13
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRC, r28
	AOUT    PORTC, r24
    #endif

	; full check, we have measured the count of cpu-tics for four RX bits
    #define TicsPerNBitLoopPass (TICS_PER_LOOP_PASS*2*4/CounterClockDiv)
    #define MinTics4Bit (((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*4)/CounterClockDiv)-(TicsPerNBitLoopPass/2)
  	subi	r24, lo8(MinTics4Bit)
  	sbci	r25, hi8(MinTics4Bit)
   #if (TicsPerNLoopPass == 4)
	movw	r10, r24
	lsr	r10
	ror	r11
	lsr	r10
	ror	r11
   #elif (TicsPerNLoopPass == 8)
	movw	r10, r24
	ldi	r27, 3		; divide by 8 can be done with 3 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #elif (TicsPerNLoopPass == 16)
	movw	r10, r24
	ldi	r27, 4		; divide by 16 can be done with 4 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #elif (TicsPerNLoopPass == 32)
	movw	r10, r24
	ldi	r27, 5		; divide by 32 can be done with 5 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #else
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop2:
	ADDIV	r28, FastDiv
	ASBIW	24, (FastDiv*TicsPerNBitLoopPass)	; 2 loop calls for every Tx bit, /4 for 4 bit time
	brsh	divlop2
	REMCHK   r24,TicsPerNBitLoopPass,r28
	movw	r10, r28
   #endif	/* TicsPerNLoopPass */

   ; result of 1 will make the single delay return after one pass (TICS_PER_LOOP_PASS), 0=long and wrong delay

  #endif		/* Bits2Measure == 4 */

  #if Bits2Measure == 2
	; 60-69, we have measured the count of cpu-tics for two RX bits

   #if SHOW_BAUD == 11
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRA, r28
	AOUT    PORTA, r26
   #endif
   #if SHOW_BAUD == 12
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRB, r28
	AOUT    PORTB, r26
   #endif
   #if SHOW_BAUD == 13
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRC, r28
	AOUT    PORTC, r26
   #endif

   #if CounterClockDiv != 1
    #warning "optiboot can get the baud rate better with full speed counter or 4-bit mode for Software-UART!"
   #endif
   #if (CounterClockDiv != 1) && ((TICS_PER_LOOP_PASS - ((TICS_PER_LOOP_PASS/4)*4)) != 0)
	; simple measurement, we have measured the count of cpu-tics for two RX bits
	add	r26,r26
	adc	r27,r27		; we double the time for better calculation

    #define TicsPerNLoopPass ((TICS_PER_LOOP_PASS*2*4)/CounterClockDiv)
    #define MinTics4Bit ((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*2*2)/CounterClockDiv-(TicsPerNLoopPass/2)
   #else     /* CounterClockDiv == 1 */
	; simple measurement, we have measured the count of cpu-tics for two RX bits
    #define TicsPerNLoopPass ((TICS_PER_LOOP_PASS*2*2)/CounterClockDiv)
    #define MinTics4Bit (((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*2)/CounterClockDiv-(TicsPerNLoopPass/2))
   #endif       /* CounterClockDiv */

	sbiw	r26, MinTics4Bit
   #if (TicsPerNLoopPass == 4)
	movw	r10, r26
	lsr	r10
	ror	r11
	lsr	r10
	ror	r11
   #elif (TicsPerNLoopPass == 8)
	movw	r10, r26
	ldi	r27, 3		; divide by 8 can be done with 3 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #elif (TicsPerNLoopPass == 16)
	movw	r10, r26
	ldi	r27, 4		; divide by 16 can be done with 4 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #elif (TicsPerNLoopPass == 32)
	movw	r10, r26
	ldi	r27, 5		; divide by 32 can be done with 5 shifts in a loop
div2lop1:
	lsr	r10
	ror	r11
	dec	r27
	brne	div2lop1
   #else
	; TicsPerNLoopPass is no power of 2 use subtract loop
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop3:
	ADDIV	r28, FastDiv
	sbiw	r26, (FastDiv*TicsPerNLoopPass)	; 2 loop calls for every Tx bit, /2 for 2 bit measured
	brsh	divlop3
	REMCHK   r26,TicsPerNLoopPass,r28
	movw	r10, r28
   #endif	/* TicsPerNLoopPass */

   #if SHOW_BAUD == 1
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r10
   #endif
   #if SHOW_BAUD == 2
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r10
   #endif
   #if SHOW_BAUD == 3
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r10
   #endif
  #endif	/* Bits2Measuse == 2 */

  #if Bits2Measure == 9
	; full check, we use a four bit time (S00001100) for baud-rate calculation.
	movw	r24, r14		; S00001100

   #if SHOW_BAUD == 11
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRA, r28
	AOUT    PORTA, r24
   #endif
   #if SHOW_BAUD == 12
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRB, r28
	AOUT    PORTB, r24
   #endif
   #if SHOW_BAUD == 13
	ldi     r28, SHOW_BAUD_MSK
	AOUT    DDRC, r28
	AOUT    PORTC, r24
   #endif
	; full check, we have measured the count of cpu-tics for nine RX bits
   #define TicsPer9LoopPass (((TICS_PER_LOOP_PASS*2*9)+(CounterClockDiv/2))/CounterClockDiv) /* 2 loop calls for every Tx bit, /9 for 9 bit time */
   #define MinTics9Bit ((LOOP_TICS+(RCALL_TICS+XTRA_TIC)*2)*9)/CounterClockDiv-TicsPer9LoopPass/2
 	ASBIW	24, MinTics9Bit
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	; speed up the subtract loop for div by nearly factor 2 with double subtracts
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop9s:
	ADDIV	r28, FastDiv
	ASBIW	24, (FastDiv*TicsPer9LoopPass)	; 2 loop calls for every Tx bit, /9 for 9 bit time
	brsh	divlop9s
	REMCHK   r24,TicsPer9LoopPass,r28

	movw	r10, r28
   ; result of 1 will make the single delay return after one pass (TICS_PER_LOOP_PASS), 0=long and wrong delay

   #if SHOW_BAUD == 1
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r10
   #endif
   #if SHOW_BAUD == 2
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r10
   #endif
   #if SHOW_BAUD == 3
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r10
   #endif

  #endif		/* Bits2Measure == 9 */

 #else		/* HW-UART */

  #if Bits2Measure == 4
	; full check, we use a four bit time (1100) for baud-rate calculation.

   #if SHOW_BAUD == 11
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r24
   #endif
   #if SHOW_BAUD == 12
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r24
   #endif
   #if SHOW_BAUD == 13
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r24
   #endif

	; full check, we have allready the sum of the 11 and 00 time in r24:25
	; because we have measured the time of four bits,
	; we must divide by 4 to get the final UBRR
	; ((four_bit_time + 2) / 4) - 1  is equal to
	; (four_bit_time - 2) / 4 
   #if CounterClockDiv == 1
    #if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
	sbiw	r24, 28		; (four_bit_time - 28) / 32, faster selection than correct round up
    #elif defined(__AVR_ATmega162__)
	sbiw	r24, 19		; (four_bit_time - 19) / 32, faster selection than correct round up
    #else
;	sbiw	r24, 24		; (four_bit_time - 24) / 32, faster selection than correct round up
	sbiw	r24, 16		; (four_bit_time - 24) / 32, faster selection than correct round up
    #endif
	ldi	r28, 5		; divide by 32
div4lop2:
	lsr	r25		; /2
	ror	r24
	dec	r28
	brne	div4lop2
   #else 	/* /8 counter clock */
	sbiw	r24, 3		; (four_bit_time - 3) / 4, faster selection than correct round up
	lsr	r25		; /2
	ror	r24		; rotate through carry
	lsr	r25		; /2
	ror	r24		; rotate through carry
   #endif	/* CounterClockDiv */
	; now we can start the UART
	AOUT	UART_SRRH, r25
	AOUT	UART_SRRL, r24

   #if SHOW_BAUD == 1
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r24
   #endif
   #if SHOW_BAUD == 2
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r24
   #endif
   #if SHOW_BAUD == 3
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r24
   #endif

  #endif		/* Bits2Measure == 4 */

  #if Bits2Measure == 2

   #if SHOW_BAUD == 11
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r26
   #endif
   #if SHOW_BAUD == 12
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r26
   #endif
   #if SHOW_BAUD == 13
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r26
   #endif

	; 60-79, We have the time of the 00 bits in r26:r27
	; because we have measured the time of two bits,
	; we must divide by 2 to get the final UBRR
	; ((two_bit_time + 1) / 2) - 1  is equal to
	; (two_bit_time - 1) / 2 
   #if CounterClockDiv == 1
	; 60-79, measurement of two bits is done with full CPU clock
;	sbiw	r26, 8		; (two_bit_time - 8) / 16
	sbiw	r26, 15		; (two_bit_time - 15) / 16, faster selection than correct round up
	ldi	r28, 4		; divide by 16
div2lop2:
	lsr	r27
	ror	r26
	dec	r28
	brne	div2lop2
   #else	/* /8 counter clock */
	; 60-79, measurement of two bits is done with clock/8
	sbiw	r26, 1		; (two_bit_time - 1) / 2
	lsr	r27		; /2
	ror	r26		; rotate through carry
   #endif
	; now we can start the UART
	AOUT	UART_SRRH, r27
	AOUT	UART_SRRL, r26

   #if SHOW_BAUD == 1
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r26
   #endif
   #if SHOW_BAUD == 2
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r26
   #endif
   #if SHOW_BAUD == 3
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r26
   #endif

  #endif	/* Bits2Measuse == 2 */

  #if Bits2Measure == 9
	; full check, we use a four bit time (S00001100) for baud-rate calculation.
	movw	r24, r14		; S00001100

   #if SHOW_BAUD == 11
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRA, r28
	AOUT	PORTA, r24
   #endif
   #if SHOW_BAUD == 12
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRB, r28
	AOUT	PORTB, r24
   #endif
   #if SHOW_BAUD == 13
	ldi	r28, SHOW_BAUD_MSK
	AOUT	DDRC, r28
	AOUT	PORTC, r24
   #endif

	; full check, we have allready the time of 9 Bit (S0001100) in r24:25
	; because we have measured the time of four bits,
	; we must divide by 9 to get the final UBRR
	; ((nine_bit_time + 5) / 9) - 1  is equal to
	; (nine_bit_time - 4) / 9 
	sbiw	r24, (4*8/CounterClockDiv)	; (nine_bit_time - (4*8)) / (8*9), faster selection than correct round up
	; we will nearly double the speed of subtract loop by double suctract
    #if FastDiv == 1
     #warning "Division operation by successive subtraction is probably too slow to finish before the Stop bit ends"
    #endif
	LDDIV	28, (-FastDiv)			; set divide loop counter start
divlop9f:
	ADDIV	r28, FastDiv
	ASBIW	24, (FastDiv*9*8/CounterClockDiv)		; divide by 9 by subtract
	brsh	divlop9f
	REMCHK   r24,(9*8/CounterClockDiv),r28
	; now we can start the UART
	AOUT	UART_SRRH, r29
	AOUT	UART_SRRL, r28

   #if SHOW_BAUD == 1
	ldi	r24, SHOW_BAUD_MSK
	AOUT	DDRA, r24
	AOUT	PORTA, r28
   #endif
   #if SHOW_BAUD == 2
	ldi	r24, SHOW_BAUD_MSK
	AOUT	DDRB, r24
	AOUT	PORTB, r28
   #endif
   #if SHOW_BAUD == 3
	ldi	r24, SHOW_BAUD_MSK
	AOUT	DDRC, r24
	AOUT	PORTC, r28
   #endif

  #endif		/* Bits2Measure == 9 */

 #endif		/* SOFT_UART | HW_UART */


 #define DELAY_NEXT_REQUEST
#endif		/* BAUD_RATE < 60 */

#if SOFT_UART == 0 
 #ifdef UART_ONE_WIRE
	ldi	r24, UART_ENABLE_RX
 #else
	ldi	r24, UART_ENABLE_RX|UART_ENABLE_TX
 #endif
	AOUT	UART_SRB, r24
 #ifdef LINDAT
;	AOUT	LINDAT, r1	; output 0 to TX Port to initialize LTXOK in LINSIR
 #endif
#endif

#if ((LED_DATA_FLASH & 0x01) > 0) && defined(LED_PORT) && defined(LEDbit)
	ACBI	LED_PORT, LEDbit
#endif

#if TEST_OUTPUT == 1
	rjmp	test_out	; output 'U'= S10101010P for speed measuring
#else
	rjmp	ver_put		;  rcall verifySpace; rjmp put_ok
#endif

#ifdef DELAY_NEXT_REQUEST
delay_next_try:
 #if INVERSE_UART > 0
	ASBIS	UART_RX_PIN, UART_RX_BIT	; Look if RX is still low
 #else
	ASBIC	UART_RX_PIN, UART_RX_BIT	; Look if RX is still high
 #endif
	rjmp	delay_next_try
	; RX=0 can be a data 0 bit or a start bit. We hope,that is is a data bit.
to_next_try:
	rjmp	next_try			; wait for next stop bit
#endif 
