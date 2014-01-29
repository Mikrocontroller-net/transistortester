#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"

#define	bitcnt 	r18
#define temp r21

#define	Txbyte r24

#define S_PORT _SFR_IO_ADDR(PORTC)
#define S_DDR _SFR_IO_ADDR(DDRC)
#define S_PIN_REG _SFR_IO_ADDR(PINC)

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

.global uart_putc
.func uart_putc
.extern wait100us
.extern wait3us

 .section .text
uart_putc:	
#ifdef WITH_UART
		push bitcnt
		in temp,_SFR_IO_ADDR(SREG)
    	push bitcnt
		ldi	bitcnt,10	;1+8+sb (sb is # of stop bits)
		com	Txbyte		;Inverte everything
		sec			;Start bit

putchar0:	brcc	putchar1	;If carry set
	#ifdef SWUART_INVERT
		sbi	S_PORT,TxD	;    send a '0'
	#else
		cbi	S_PORT,TxD	;    send a '0'
	#endif
		rjmp	putchar2	;else	

putchar1:	
	#ifdef SWUART_INVERT
		cbi	S_PORT,TxD	;    send a '1'
	#else
		sbi	S_PORT,TxD	;    send a '1'
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
		pop bitcnt
    	out _SFR_IO_ADDR(SREG),temp
    	pop bitcnt
#endif
		ret			;   return

.endfunc


