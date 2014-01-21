#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include <avr/eeprom.h>
#include "config.h"
#include "part_defs.h"

.GLOBAL mVAusgabe
.GLOBAL UfAusgabe
.GLOBAL SerienDiodenAusgabe


#define zero_reg r1

/* #include <avr/io.h> */
/* #include "Transistortester.h" */

/* void mVAusgabe(uint8_t nn) {  */
/*    if (nn < 3) {  */
/*       // Output in mV units  */
/*       DisplayValue(diodes.Voltage[nn],-3,'V',3);  */
/*       lcd_space();  */
/*    }  */
/* }  */

mVAusgabe:
 	push	r16
;   if (nn < 3) {
 	cpi	r24, 0x03	; 3
 	brcc	ad1ca4;
      // Output in mV units
	ldi	r30,lo8(diodes+12);
	ldi	r31,hi8(diodes+12);
	add	r30,r24
	adc	r31,zero_reg
	add	r30,r24
	adc	r31,zero_reg
;; 	mov	r30, r24
;;        mov	r31, zero_reg
;; 	add	r30, r30
;; 	adc	r31, r31
;; 	subi	r30, 0x74	; 116
;; 	sbci	r31, 0xFE	; 254
 	ld	r22, Z
 	ldd	r23, Z+1	; 0x01
        mov	r24, zero_reg
        mov	r25, zero_reg
 	ldi	r20, -3;	0xFD
 	ldi	r18, 'V';	0x56
 	ldi	r16, 0x03	; 3
 	ACALL	DisplayValue	; DisplayValue(diodes.Voltage[nn],-3,'V',3);
 	ACALL	lcd_space;	; lcd_space();
ad1ca4:
 	pop	r16
 	ret

/* // ****************************************************************** */
/* // output of flux voltage for 1-2 diodes in row 2 */
/* // bcdnum = Numbers of both Diodes: */
/* // higher 4 Bit  number of first Diode */
/* // lower 4 Bit  number of second Diode (Structure diodes[nn]) */
/* // if number >= 3  no output is done */
/* void UfAusgabe(uint8_t bcdnum) { */
/*    if (ResistorsFound > 0) { //also Resistor(s) found */
/*       lcd_space(); */
/*       lcd_data(LCD_CHAR_RESIS3);	// special symbol or R */
/*    } */
/*    lcd_line2(); 				//2. row */
/*    lcd_fix_string(Uf_str);			//"Uf=" */
/*    mVAusgabe(bcdnum >> 4); */
/*    mVAusgabe(bcdnum & 0x0f); */
/* } */

UfAusgabe:
 	push	r17
 	mov	r17, r24
 	lds	r24, ResistorsFound;	0x0168
 	and	r24, r24
 	breq	ad1cbe; 	   if (ResistorsFound > 0) { 
 	ACALL	lcd_space;	lcd_space();
 	ldi	r24, LCD_CHAR_RESIS3;	0
 	ACALL	lcd_data;      lcd_data(LCD_CHAR_RESIS3);	// special symbol or R
ad1cbe:
 	ldi	r24, 0xC0	; 192
 	ACALL	lcd_command	;lcd_line2(); 				//2. row
 	ldi	r24, lo8(Uf_str);	0xE9
 	ldi	r25, hi8(Uf_str);	0x01
 	ACALL	lcd_fix_string	; lcd_fix_string(Uf_str);	//"Uf="
 	mov	r24, r17
 	swap	r24
 	andi	r24, 0x0F
 	rcall	mVAusgabe	; mVAusgabe(bcdnum >> 4);
 	mov	r24, r17
 	andi	r24, 0x0F	; 15
 	rcall	mVAusgabe	; mVAusgabe(bcdnum & 0x0f);
 	pop	r17
 	ret


/* void SerienDiodenAusgabe() { */
/*    uint8_t first; */
/*    uint8_t second; */
/*    first = diode_sequence >> 4; */
/*    second = diode_sequence & 3; */
/*    lcd_testpin(diodes.Anode[first]); */
/*    lcd_fix_string(AnKat);	//"->|-" */
/*    lcd_testpin(diodes.Cathode[first]); */
/*    lcd_fix_string(AnKat);	//"->|-" */
/*    lcd_testpin(diodes.Cathode[second]); */
/*    UfAusgabe(diode_sequence); */
/* } */
SerienDiodenAusgabe:
 	push	r13
 	push	r14
 	push	r15
 	push	r16
 	push	r17
 	push	r28
 	push	r29
 	lds	r13, diode_sequence;	0x0102
 	mov	r24, r13
 	swap	r24
 	andi	r24, 0x0F	; first = diode_sequence >> 4;
 	ldi	r28, lo8(diodes)	;0x80
 	ldi	r29, hi8(diodes)	;0x01
 	movw	r14, r28
 	add	r14, r24	; first
 	adc	r15, zero_reg
 	movw	r30, r14
 	ld	r24, Z		;Anode
 	ACALL	lcd_testpin	; lcd_testpin(diodes.Anode[first]);
 	ldi	r16, lo8(AnKat)	;0xA3
 	ldi	r17, hi8(AnKat)	;0x03
 	movw	r24, r16
 	ACALL	lcd_fix_string	; lcd_fix_string(AnKat);	//"->|-"
 	movw	r30, r14
 	ldd	r24, Z+6	; Cathode
 	ACALL	lcd_testpin	; lcd_testpin(diodes.Cathode[first]);
 	movw	r24, r16
 	ACALL	lcd_fix_string	; lcd_fix_string(AnKat);	//"->|-"

 	mov	r24, r13
 	andi	r24, 0x03	; second = diode_sequence & 3;
 	add	r28, r24
 	adc	r29, zero_reg
 	ldd	r24, Y+6	; Cathode
 	ACALL	lcd_testpin	; lcd_testpin(diodes.Cathode[second]);
 	lds	r24, diode_sequence;	x0102
 	rcall	UfAusgabe	; UfAusgabe(diode_sequence);
    	pop	r29
    	pop	r28
    	pop	r17
    	pop	r16
    	pop	r15
    	pop	r14
    	pop	r13
    	ret
