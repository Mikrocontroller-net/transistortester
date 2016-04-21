#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"

#define	bitcnt 	r18
#define temp r21

#define	Txbyte r24

#define SER_PORT _SFR_IO_ADDR(SERIAL_PORT)
#define SER_DDR _SFR_IO_ADDR(SERIAL_DDR)

;* Software-UART nach Atmel AVR-Application-Note AVR305

;***************************************************************************
;*
;* "putchar"
;*
;* This subroutine transmits the byte stored in the "Txbyte" register
;* Low registers used	:None
;* High registers used	:2 (bitcnt,Txbyte)
;* Pointers used	:None
;*
;***************************************************************************

.func uart_putc
.global uart_putc
.extern wait100us
.extern wait3us

 .section .text
uart_putc:	
#if defined(WITH_UART) && (!defined(WITH_HARDWARE_SERIAL))
;	push bitcnt
;	in temp,_SFR_IO_ADDR(SREG)
		push	Txbyte		; save character 
	    	push bitcnt		; save register used for bit count
		ldi	bitcnt,10	;1+8+sb (sb is # of stop bits)
		com	Txbyte		;Inverte everything
		sbi	SER_DDR,SERIAL_BIT	; enable output of serial bit
		sec			;Start bit

putchar0:	brcc	putchar1	;If carry set
	#ifdef SWUART_INVERT
		sbi	SER_PORT,SERIAL_BIT	;    send a '0'
	#else
		cbi	SER_PORT,SERIAL_BIT	;    send a '0'
	#endif
		rjmp	putchar2	;else	

putchar1:	
	#ifdef SWUART_INVERT
		cbi	SER_PORT,SERIAL_BIT	;    send a '1'
	#else
		sbi	SER_PORT,SERIAL_BIT	;    send a '1'
	#endif
		nop

putchar2:
                rcall   wait100us	; about 9600 Baud
       	#if  F_CPU >= 8000000UL
                rcall	wait3us
	#endif
		lsr	Txbyte		;Get next bit
		dec	bitcnt		;If not all bit sent
		brne	putchar0	;   send next
					;else
		pop	bitcnt 		; restore register used for bit count
		pop	Txbyte		; restore character send
;   	out _SFR_IO_ADDR(SREG),temp
;   	pop bitcnt
#endif /* defined(WITH_UART) && (!defined(WITH_HARDWARE_SERIAL)) */
#ifdef WITH_HARDWARE_SERIAL
		; wait for empty transmit buffer
w3:
		lds	r25, UCSR0A
		sbrs	r25, UDRE0
		rjmp	w3		; wait
		AOUT    UDR0, r24	; put data to transmit buffer
#endif	/* def WITH_HARDWARE_SERIAL */
		ret			;   return

.endfunc


