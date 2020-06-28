
;#include <avr/io.h>
;###############################################################################
; This File must be included by a Assembler program with #include "soft_uart.S" .
; Please set the following GNU C-Preprocessor Makros before you include this file:
;	SOFT_UART_NUMBER
;		If unset, the basic Software for UART IO is generated.
;		You can call two functions getch() and putch(char c) for
;		IO to the previously defined Port
;		If you set the SOFT_UART_NUMBER to 1 (up to 3 is possible),
;		the code for the function getch_1() and putch_1(char c) is generated.
;	F_CPU 		must be set to the clock frequency, which the AVR is running.
;	BAUD_RATE	must be set to the desired baud rate .
;	INVERSE_UART	ĉan be specified, if the output should be inverted (0=STOP level).
;	
;	NO_SOFT_UART_TX can be specified, if no code for serial data output is required.
;	NO_SOFT_UART_RX can be specified, if no code for serial data input is required.
;###############################################################################


;***************************************************************

#ifndef NO_SOFT_UART_RX
 ; setup some GNU C-proprocessor macros for the getch function
 ;------------------------------------------------------------
 #ifndef SOFT_UART_RX_PORT
  #error You must specify the Port of the RX bit for software UART with SOFT_UART_RX_PORT
 #endif
 #define SOFT_UART_RX_DDR (SOFT_UART_RX_PORT-1)
 #define SOFT_UART_RX_PIN (SOFT_UART_RX_PORT-2)
 #ifndef SOFT_UART_RX_BIT
  #error You must specify the RX-Bit number for software UART with SOFT_UART_RX_BIT
 #endif
 #undef GET_CH
 #undef WAIT_START
 #undef GET_NEXT_BIT
 #undef FIN_RX
 #undef G_BLIND_DELAY
 #if SOFT_UART_NUMBER == 1
  #define GET_CH getch_1
  #define WAIT_START wait_for_rx_1
  #define GET_NEXT_BIT getnextrx_1
  #define FIN_RX fini_rx1
  #define G_BLIND_DELAY direct_return1
 #elif	SOFT_UART_NUMBER == 2
  #define GET_CH getch_2
  #define WAIT_START wait_for_rx_2
  #define GET_NEXT_BIT getnextrx_2
  #define FIN_RX fini_rx2
  #define G_BLIND_DELAY direct_return2
 #elif	SOFT_UART_NUMBER == 3
  #define GET_CH getch_3
  #define WAIT_START wait_for_rx_3
  #define GET_NEXT_BIT getnextrx_3
  #define FIN_RX fini_rx3
  #define G_BLIND_DELAY direct_return3
 #else
  #define GET_CH getch
  #define WAIT_START wait_for_rx
  #define GET_NEXT_BIT getnextrx
  #define FIN_RX fini_rx
  #define G_BLIND_DELAY direct_return0
 #endif
 .global GET_CH
#endif		/* #ifndef NO_SOFT_UART_RX */

#ifndef NO_SOFT_UART_TX
 ; setup some GNU C-proprocessor macros for the putch function
 ;------------------------------------------------------------
 #ifndef SOFT_UART_TX_PORT
  #error You must specify the Port of the TX bit for software UART with SOFT_UART_TX_PORT
 #endif
 #define SOFT_UART_TX_DDR (SOFT_UART_TX_PORT-1)
 #define SOFT_UART_TX_PIN (SOFT_UART_TX_PORT-2)
 #ifndef SOFT_UART_TX_BIT
  #error You must specify the TX-Bit number for software UART with SOFT_UART_TX_BIT
 #endif
 #undef PUT_CH
 #undef PUT_LOP
 #undef TX_IS_SET
 #undef SET_BIT
 #if SOFT_UART_NUMBER == 1
  #define PUT_CH putch_1
  #define PUT_LOP puplop_1
  #define TX_IS_SET txisset_1
  #define SET_BIT set_tx_1
 #elif	SOFT_UART_NUMBER == 2
  #define PUT_CH putch_2
  #define PUT_LOP puplop_2
  #define TX_IS_SET txisset_2
  #define SET_BIT set_tx_2
 #elif	SOFT_UART_NUMBER == 3
  #define PUT_CH putch_3
  #define PUT_LOP puplop_3
  #define TX_IS_SET txisset_3
  #define SET_BIT set_tx_3
 #else
  #define PUT_CH putch
  #define PUT_LOP puplop
  #define TX_IS_SET txisset
  #define SET_BIT set_tx
 #endif
 .global PUT_CH
#endif		/* #ifndef NO_SOFT_UART_TX */
;
; the two functions getch and putch can serve a half duplex
; serial communication with the baud rate specified with BAUD_RATE.
; The clock frequency F_CPU and features of the selected port 
; are respected to get the nearest possible real baud rate.
;
; The Option QuickStopBit let the function putch return
; after output of the last data.
; The serial output is set to 1 for the begin of the stop bit,
; but the Baud-time of the stop bit is not monitored correctly.
; So the program has more time to watch the serial input.
; The next output of a start bit is delayed for one Baud-time,
; so that sequential serial output is timed correctly.

#ifndef QuickStopBit
 #define QuickStopBit 1		/* set the option by default */
; #define QuickStopBit 0		/* reset the option by default */
#endif


#if (BAUD_RATE*8) > F_CPU
 #error Baud Rate too high for this CPU clock
#endif
#if (BAUD_RATE != 600) && (BAUD_RATE != 1200) && (BAUD_RATE != 2400) && (BAUD_RATE != 4800) \
 && (BAUD_RATE != 9600) && (BAUD_RATE != 19200) && (BAUD_RATE != 38400) && (BAUD_RATE != 57600) \
 && (BAUD_RATE != 115200) && (BAUD_RATE != 230400) && (BAUD_RATE != 500000) && (BAUD_RATE != 1000000) \
 && (BAUD_RATE >= 100)
 #warning Selected BAUD Rate has no usual value like 600,1200,2400.4800,9600,19200,38400,57600,115200,230400,500000 or 1000000!
#endif

; check with C preprocessor for One Wire mode
#if !defined(NO_SOFT_UART_RX) && !defined(NO_SOFT_UART_TX)
 ; we can only check, if both RX and TX are enabled
 #if (SOFT_UART_TX_DDR == SOFT_UART_RX_DDR) && (SOFT_UART_TX_BIT == SOFT_UART_RX_BIT)
  #undef UART_ONE_WIRE
  #define UART_ONE_WIRE 
  #if INVERSE_UART > 0
   #warning UART RX and TX to the same Port Bit not possible with INVERSE_UART! Option is canceled!
   #undef INVERSE_UART
   #define INVERSE_UART 0
  #endif
 #endif
#endif

#if defined(FORCE_ONE_WIRE_MODE) && defined(NO_SOFT_UART_RX)
  ; If requested, force the one wire output mode without serial input
  #define UART_ONE_WIRE 
#endif

;**********************************************************
#ifndef NO_SOFT_UART_TX
;**********************************************************
#define SOFT_UART_PORT UART_TX_PORT	/* SOFT_UART_PORT is only used for Address range check in uart_delay.S */
#include "uart_delay.S"		/* generate delay loop for SINGLE_DELAY_ADDR and DOUBLE_DELAY_ADDR */

PUT_CH :	/*  void putch(char ch) */
 #if ((LED_DATA_FLASH & 0x02) > 0) && defined(LED_PORT) && defined(LEDbit) && (SOFT_UART_NUMBER == 0)
	/* Use of The data direction bit DDR has advantage to the PORT bit because */
        /* no prepare is needed (PORT bit is cleared by reset), but LED circuit must */
	/* switch the LED on with a 0 signal level */
	ASBI	LED_PORT, LEDbit
 #endif
	; r24 hold the data for serial output
	; use r22, r23, r25
	DOUBLE_DELAY_CALL
	DOUBLE_DELAY_CALL_X1	; trick for delay without loop
	DOUBLE_DELAY_CALL_X2	; trick for delay without loop
	DOUBLE_DELAY_CALL_X3	; trick for delay without loop

	DOUBLE_DELAY_CALL
	DOUBLE_DELAY_CALL_X1	; trick for delay without loop
	DOUBLE_DELAY_CALL_X2	; trick for delay without loop
	DOUBLE_DELAY_CALL_X3	; trick for delay without loop
;	SINGLE_DELAY_CALL	; work simular to a second half stop bit
				; this can help to prevent framing error, if the
				; baudrate has a error of 2% or more
	ldi	r22, 10		; counter for the serial bit output
;	ldi	r19, 0
 #ifdef UART_ONE_WIRE
  #if	SOFT_UART_TX_DDR >= (0x20 + __SFR_OFFSET) 
  ; we must load the SOFT_UART_TX_DDR content to a register (r23)
   #if	SOFT_UART_TX_DDR < (0x40 + __SFR_OFFSET)
	in	r23, _SFR_IO_ADDR(SOFT_UART_TX_DDR)
   #else
	lds	r23, _SFR_MEM_ADDR(SOFT_UART_TX_DDR)
   #endif
  #endif
 #else
  #if	SOFT_UART_TX_PORT >= (0x20 + __SFR_OFFSET)
  ; we must load the SOFT_UART_TX_PORT content to a register (r23)
   #if	SOFT_UART_TX_PORT < (0x40 + __SFR_OFFSET)
	in	r23, _SFR_IO_ADDR(SOFT_UART_TX_PORT)
   #else
	lds	r23, _SFR_MEM_ADDR(SOFT_UART_TX_PORT)
   #endif
  #endif
 #endif

 #if INVERSE_UART > 0
	clc			; clear carry for start bit
 #else
	com	r24
	sec			; set carry for start bit
 #endif
PUT_LOP :
 #if QuickStopBit > 0
	DOUBLE_DELAY_CALL 	; takes 2*(7 + UART_B_VALUE*3 + UART_R_VALUE clock tics)
	DOUBLE_DELAY_CALL_X1	; trick for delay without loop
	DOUBLE_DELAY_CALL_X2	; trick for delay without loop
	DOUBLE_DELAY_CALL_X3	; trick for delay without loop
 #endif
	brcc	SET_BIT		; 1/2 clock tics
;############### clear the TX bit #######################
 #ifdef UART_ONE_WIRE
 ; special output, set DDR Bit to 1, which enable a 0 output
  #if	SOFT_UART_TX_DDR < (0x20 + __SFR_OFFSET)
	sbi	_SFR_IO_ADDR(SOFT_UART_TX_DDR), SOFT_UART_TX_BIT	; 1+2 #### set DDR Bit to 1
	rjmp	TX_IS_SET		; 1+2+2 = 5
  #else
	sbr	r23, SOFT_UART_TX_BIT	; 1+1
	sts	_SFR_MEM_ADDR(SOFT_UART_TX_DDR), r23	; 1+1+2 #### set DDR Bit to 1
	rjmp	TX_IS_SET		; 1+1+2+2 = 6!!!!
  #endif
 #else
  ; normal output, set Port Bit to 0
  #if	SOFT_UART_TX_PORT < (0x20 + __SFR_OFFSET)
	cbi	_SFR_IO_ADDR(SOFT_UART_TX_PORT), SOFT_UART_TX_BIT	; 1+2 #### set Port Bit to 0
	rjmp	TX_IS_SET		; 1+2+2 = 5
  #else
	cbr	r23, SOFT_UART_TX_BIT	; 1+1
	sts	_SFR_MEM_ADDR(SOFT_UART_TX_PORT), r23	; 1+1+2 #### set Port Bit to 0
	rjmp	TX_IS_SET		; 1+1+2+2 = 6!!!!
  #endif
 #endif
;############### set the TX bit #######################
 SET_BIT:
 #ifdef UART_ONE_WIRE
 ; special output, set the DDR bit to 0 (input mode. pull-up resistor required to make 1)
  #if	SOFT_UART_TX_DDR < (0x20 + __SFR_OFFSET)
	cbi	_SFR_IO_ADDR(SOFT_UART_TX_DDR), SOFT_UART_TX_BIT	;2+2 #### set DDR Bit to 0
	nop			; 2+2+1 = 5
  #else
	cbr	r23, SOFT_UART_TX_BIT	; 2+1
	sts	_SFR_MEM_ADDR(SOFT_UART_TX_DDR), r23	; 2+1+2 = 5 #### set DDR Bit to 0
	nop				; 2+1+2+1 = 6
  #endif
 #else
 ; normal output, set the output to 1
  #if	SOFT_UART_TX_PORT < (0x20 + __SFR_OFFSET)
	sbi	_SFR_IO_ADDR(SOFT_UART_TX_PORT), SOFT_UART_TX_BIT	;2+2 #### set Port Bit to 1
	nop			; 2+2+1 = 5
  #else
	sbr	r23, SOFT_UART_TX_BIT	; 2+1
	sts	_SFR_MEM_ADDR(SOFT_UART_TX_PORT), r23	; 2+1+2 = 5 #### set Port Bit to 1
	nop				; 2+1+2+1 = 6
  #endif
 #endif
TX_IS_SET :
 #if QuickStopBit == 0
	DOUBLE_DELAY_CALL 	; takes 2*(7 + UART_B_VALUE*3 + UART_R_VALUE clock tics)
	DOUBLE_DELAY_CALL_X1	; trick for delay without loop
	DOUBLE_DELAY_CALL_X2	; trick for delay without loop
	DOUBLE_DELAY_CALL_X3	; trick for delay without loop
 #endif
 #if INVERSE_UART > 0
	sec			; 5+1 set carry for stop bit
	ror	r24		; 5+1+1 shift right with carry
 #else
	lsr	r24		; 5+1
 #endif
	dec	r22		; 5+1+1 decrement don't touch the carry
	brne	PUT_LOP 	; 5+1+1+2 = 9, total 9 +(2*7) = 23 Clock tics
     	wdr		; watchdogReset();
 #if ((LED_DATA_FLASH & 0x02) > 0) && defined(LED_PORT) && defined(LEDbit) && (SOFT_UART_NUMBER == 0)
	ACBI	LED_PORT, LEDbit
 #endif
P_BLIND_DELAY:
      	ret
 #undef SOFT_UART_TX_PORT
 #undef SOFT_UART_TX_DDR
 #undef SOFT_UART_TX_PIN
 #undef SOFT_UART_TX_BIT
 #undef SOFT_UART_PORT
#else
 #undef NO_SOFT_UART_TX
#endif   /* end #ifndef NO_SOFT_UART_TX */


;**********************************************************
#ifndef NO_SOFT_UART_RX
;**********************************************************
 #define SOFT_UART_PORT UART_RX_PORT	/* SOFT_UART_PORT is only used for Address range check in uart_delay.S */
 #include "uart_delay.S"		/* generate delay loop for SINGLE_DELAY_ADDR and DOUBLE_DELAY_ADDR */
	/* call getch fetch a character from serial interface */
	; return value: r24 = new character read
	; use r22, r25 (soft)
GET_CH :
 #if ((LED_DATA_FLASH & 0x01) > 0) && defined(LED_PORT) && defined(LEDbit) && (SOFT_UART_NUMBER == 0)
	/* Use of The data direction bit DDR has advantage to the PORT bit because */
        /* no prepare is needed (PORT bit is cleared by reset), but LED circuit must */
	/* switch the LED on with a 0 signal level */
	ASBI	LED_PORT, LEDbit
 #endif
 #if FLASHEND > 0x1ffff
	ldi	r22, 9		; load the loopcounter before the RX_pin observation
 #endif
WAIT_START :
 #if SOFT_UART_RX_PIN < (0x20 + __SFR_OFFSET)
  #if INVERSE_UART > 0
	sbis	_SFR_IO_ADDR(SOFT_UART_RX_PIN), SOFT_UART_RX_BIT
  #else
        sbic    _SFR_IO_ADDR(SOFT_UART_RX_PIN), SOFT_UART_RX_BIT
  #endif
 #else
  #if UART_RX_PIN < (0x40 + __SFR_OFFSET)
        in      r25, _SFR_IO_ADDR(SOFT_UART_RX_PIN)
  #else
        lds     r25, _SFR_MEM_ADDR(SOFT_UART_RX_PIN)
  #endif
  #if INVERSE_UART > 0
        sbrs    r25, SOFT_UART_RX_BIT
  #else
        sbrc    r25, SOFT_UART_RX_BIT
  #endif
 #endif
	rjmp	WAIT_START	; RX bit still set, wait
 #if FLASHEND <= 0x1ffff
	ldi	r22, 9		; load the loopcounter behind the RX_pin observation
 #endif
	SINGLE_DELAY_CALL	; wait half BAUD time
	SINGLE_DELAY_CALL_X1	; trick for delay without loop
	SINGLE_DELAY_CALL_X2	; trick for delay without loop
	SINGLE_DELAY_CALL_X3	; trick for delay without loop
;  Last inp should be (LOOP_TICS-2) before without additional delay.
;  Because of the wait-loop time the RX change can be between 0 and 3 clock tics before the inp.
;  For (LOOP_TICS == 9) we have to get a 7 clocks delay without the half baud time delay.
;  But the middle delay is only 4.5 (3.5 for FLASHEND>0x1ffff) CLOCK tics.
;  Unfortunately the SINGLE_DELAY_CALL is without the LOOP_TICS/2 (4.5 tics for a 9 tic LOOP)
;  So a total of 7 (8) clock tics must be wasted.
;	rcall	direct_return0		; some clock ticks are missing to the half delay time
GET_NEXT_BIT :
	DOUBLE_DELAY_CALL 	; takes 2*(7 + UART_B_VALUE*3 + UART_R_VALUE clock tics)
	DOUBLE_DELAY_CALL_X1	; trick for delay without loop
	DOUBLE_DELAY_CALL_X2	; trick for delay without loop
	DOUBLE_DELAY_CALL_X3	; trick for delay without loop
  #if INVERSE_UART > 0
	sec			; 1 set carry
  #else
	clc			; 1 clear carry  
  #endif
 #if UART_RX_PIN < (0x20 + __SFR_OFFSET)
        nop						; 1+1 (make loop same as putch)
        sbic    _SFR_IO_ADDR(SOFT_UART_RX_PIN), UART_RX_BIT  ; 1+1+1
 #else
  #if UART_RX_PIN < (0x40 + __SFR_OFFSET)
e       in      r25, _SFR_IO_ADDR(SOFT_UART_RX_PIN)	; 1+1
  #else
        lds     r25, _SFR_MEM_ADDR(SOFT_UART_RX_PIN)	; 1+2
  #endif
        sbrc    r25, SOFT_UART_RX_BIT                ; 1+1+1+1 or 1+2+1 are same
 #endif
  #if INVERSE_UART > 0
	clc			; 3+1 or 4+1 clear carry, bit was set
  #else
	sec			; 3+1 or 4+1 set carry, bit was set
  #endif
	dec	r22		; 3+1+1 or 4+1+1
	breq	FIN_RX		; 3+1+1+1 or 4+1+1+1
	ror	r24		; 6 or 7
  #if INVERSE_UART > 0
	nop			; 7 or 8 for invers mode
  #endif
	rjmp	GET_NEXT_BIT	; 7+2 + 2*7 clock tics (22 + UART_B_VALUE*6 + UART_R_VALUE*2)
FIN_RX :
#if 0
  #if INVERSE_UART > 0
	brcs	.+2	; skip wdr instruction, if carry is set, framing error, no STOP
  #else
	brcc	.+2	; skip wdr instruction, if no carry set, framing error, no STOP
  #endif
#endif
     	wdr		; watchdogReset();
 #if ((LED_DATA_FLASH & 0x01) > 0) && defined(LED_PORT) && defined(LEDbit) && (SOFT_UART_NUMBER == 0)
	ACBI	LED_PORT, LEDbit
 #endif
G_BLIND_DELAY:
       	ret

 #undef SOFT_UART_RX_PORT
 #undef SOFT_UART_RX_DDR
 #undef SOFT_UART_RX_PIN
 #undef SOFT_UART_RX_BIT
 #undef SOFT_UART_PORT
#else
 #undef NO_SOFT_UART_RX
#endif   /* #ifndef NO_SOFT_UART_RX */


;**********************************************************
