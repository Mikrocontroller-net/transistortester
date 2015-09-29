#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "config.h"
#include "part_defs.h"


/* #include <avr/io.h> */
/* #include <avr/eeprom.h> */
/* #include <avr/pgmspace.h> */

/* #include "Transistortester.h" */

/* void RvalOut(uint8_t nrr) {	 */
/* // output of resistor value */
/* #if FLASHEND > 0x1fff */
/*   uint16_t rr; */
/*   if ((resis[nrr].rx < 100) && (inductor_lpre == 0)) { */
/*      rr = GetESR(resis[nrr].ra,resis[nrr].rb); */
/*      DisplayValue(rr,-2,LCD_CHAR_OMEGA,3); */
/*   } else { */
/*      DisplayValue(resis[nrr].rx,-1,LCD_CHAR_OMEGA,4); */
/*   } */
/* #else */
/*    DisplayValue(resis[nrr].rx,-1,LCD_CHAR_OMEGA,4); */
/* #endif */
/*    lcd_space(); */
/*  } */

#define zero_reg r1
#define RCALL rcall

 .GLOBAL RvalOut
 .func RvalOut
 .extern DisplayValue
 .extern GetESR
 .extern lcd_space
 .extern ResistorVal

 .section .text

RvalOut:		; void RvalOut(uint8_t nrr) 	
 	push	r16

        mov	r16, r24
	LDIZ	ResistorVal
	add	r24, r24	; nrr*2
	add	r24, r24	; nrr*4
	add	r30, r24
	adc	r31, zero_reg

 	ld	r22, Z		; resis[rr].rx
 	ldd	r23, Z+1	; 0x01
 	ldd	r24, Z+2	; 0x02
 	ldd	r25, Z+3	; 0x03
#if FLASHEND > 0x1fff
 	cpi	r22, 0x64	; 100
 	cpc	r23, r1
 	cpc	r24, r1
 	cpc	r25, r1
 	brcc	ad1d8e 		; (ResistorVal[nrr] < 100)
 	lds	r18, inductor_lpre
	sbrc	r18, 7		; minus bit set?
 	rjmp	ad1d8e 		; (inductor_lpre >= 0)
	mov	r24,r16
	ACALL	Rnum2pins;	; pins = Rnum2pins(nrr)
        mov	r22, r25
 	ACALL	GetESR		; rr = GetESR(resis[nrr].ra,resis[nrr].rb);
; 	movw	r22, r24
;        ldi	r24, 0
;        ldi	r25, 0
; 	ldi	r20, -2		; 254
; 	ldi	r16, 0x03	; 3
; 	rjmp	ad1d94		; DisplayValue(rr,-2,LCD_CHAR_OMEGA,3);
	ldi	r22, -2
	ldi	r18, 3
	ldi	r20, LCD_CHAR_OMEGA
	RCALL	DisplayValue16	; DisplayValue16(rr,-2,LCD_OMEGA,3);
	rjmp	ret_with_space
ad1d8e:		; } else {
#endif
	; r22-r25 = ResistorVal[rr]
 	ldi	r20, -1		; 255
 	ldi	r16, 0x04	; DisplayValue(resis[nrr].rx,-1,LCD_CHAR_OMEGA,4);
;ad1d94:
 	ldi	r18, LCD_CHAR_OMEGA	; 244
 	RCALL	DisplayValue
ret_with_space:
 	RCALL	lcd_space	; lcd_space();
 	pop	r16
 	ret
 .endfunc
