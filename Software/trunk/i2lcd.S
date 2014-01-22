#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include "config.h"

.GLOBAL u2lcd
.GLOBAL i2lcd


i2lcd:			;	void i2lcd(int iw)

;;  	movw	r20, r24
 	sbrs	r25, 7
 	rjmp	to_lcd		; if (iw >= 0) {
;   // negativ value, output -  and invert iw
        push	r24
        push	r25
 	ldi	r24,'-'		; 45
 	ACALL	lcd_data	; lcd_data('-'); uses r22
        pop	r25		; old r25
        pop	r24		; old r24
        com	r25
	neg	r24
        sbci	r25,-1		; iw = - iw

u2lcd:			;	void i2lcd(uint16_t iw)
to_lcd:			;	void i2lcd(uint16_t iw)
 	ldi	r22, lo8(outval)	;0x0F
 	ldi	r23, hi8(outval)	;0x01
 	ldi	r20, 10
 	ldi	r21, 0x00	; 0
 	ACALL	utoa		; utoa(iw, outval, 10);	//output voltage to string
 	ACALL	lcd_string	;lcd_string(utoa(iw, outval, 10));	//output correction voltage

 	ret
