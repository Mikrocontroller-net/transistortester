#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include "config.h"

.GLOBAL u2lcd
.GLOBAL u2lcd_space

 .extern lcd_data
 .extern lcd_string
 .extern lcd_space

 .func i2lcd

; use allways rcall for nearby functions
#define RCALL rcall


 .section .text
.GLOBAL i2lcd

i2lcd:			;	void i2lcd(int iw)

#if FLASHEND > 0x1fff

;;  	movw	r20, r24
 	sbrs	r25, 7
 	rjmp	to_lcd		; if (iw >= 0) {
;   // negativ value, output -  and invert iw
        push	r24		; save r24:r25
        push	r25
 	ldi	r24,'-'		; 45
 	RCALL	lcd_data	; lcd_data('-'); uses r22
        pop	r25		; recall r25:r24
        pop	r24		; old r24
        com	r25
	neg	r24
        sbci	r25,-1		; iw = - iw
#endif

#if FLASHEND > 0x1fff
u2lcd:			;	void i2lcd(uint16_t iw)
to_lcd:			;	void i2lcd(uint16_t iw)
 	ldi	r22, lo8(outval)	;0x0F
 	ldi	r23, hi8(outval)	;0x01
 	ldi	r20, 10
 	ldi	r21, 0x00	; 0
 	ACALL	utoa		; utoa(iw, outval, 10);	//output voltage to string
 	RCALL	lcd_string	;lcd_string(utoa(iw, outval, 10));	//output correction voltage

 	ret
#endif

#if FLASHEND > 0x1fff
.GLOBAL i2lcd_space
i2lcd_space:
	RCALL	i2lcd
	rjmp	space_ret	; use return from u2lcd_space

u2lcd_space:
	RCALL	i2lcd
space_ret:
;	RCALL    lcd_space
;	ret
#endif
 .endfunc

; Output a ' ' to the LCD screen
.GLOBAL lcd_space
.func lcd_space
lcd_space:
	ldi	r24, ' '
	RJMP	lcd_data
.endfunc

; Output a '-' to the LCD screen
.GLOBAL lcd_minus
.func lcd_minus
lcd_minus:
	ldi	r24, '-'
	RJMP	lcd_data
.endfunc

; Output a '=' to the LCD screen
.GLOBAL lcd_equal
.func lcd_equal
lcd_equal:
	ldi	r24, '='
	RJMP	lcd_data
.endfunc
