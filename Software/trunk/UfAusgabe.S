	#ifndef __ASSEMBLER__
	 #define __ASSEMBLER__
	#endif
	#include <avr/io.h>
	#include <avr/common.h>
	#include <avr/eeprom.h>
	#include "config.h"
	#include "part_defs.h"


	 .section .text


	#define zero_reg r1
	#define RCALL rcall

	/* #include <avr/io.h> */
	/* #include "Transistortester.h" */

	/* void mVAusgabe(uint8_t nn) {  */
	/*    if (nn < 3) {  */
	/*       // Output in mV units  */
	/*       DisplayValue(diodes.Voltage[nn],-3,'V',3);  */
	/*       lcd_space();  */
	/*    }  */
	/* }  */

	.GLOBAL mVAusgabe
        .extern DisplayValue
        .extern lcd_space

	 .func mVAusgabe
	mVAusgabe:
	;   if (nn < 3) {
		cpi	r24, 0x03	; 3
		brcc	ad1ca4;
	      // Output in mV units
		LDIZ	diodes+12;
		add	r30,r24
		adc	r31,zero_reg
		add	r30,r24
		adc	r31,zero_reg
		ld	r24, Z		; diodes.Voltage[nn]
		ldd	r25, Z+1	; 0x01
		ldi	r22, 0x03	; 3
		RCALL	Display_mV	; Display_mV(diodes.Voltage[nn],3);
		RCALL	lcd_space;	; lcd_space();
	ad1ca4:
		ret
	 .endfunc

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
/*    lcd_PGM_string(Uf_str);			//"Uf=" */
/*    mVAusgabe(bcdnum >> 4); */
/*    mVAusgabe(bcdnum & 0x0f); */
/* } */

	.GLOBAL UfAusgabe
 	.extern lcd_space
	.extern lcd_data
	.extern lcd_line2
	.extern mVAusgabe

	.func UfAusgabe

UfAusgabe:
 	push	r17
 	mov	r17, r24
 	lds	r24, ResistorsFound;	0x0168
 	and	r24, r24
 	breq	ad1cbe; 	   if (ResistorsFound > 0) { 
 	RCALL	lcd_space;	lcd_space();
 	ldi	r24, LCD_CHAR_RESIS3;	0
 	RCALL	lcd_data;      lcd_data(LCD_CHAR_RESIS3);	// special symbol or R
ad1cbe:
 	RCALL	lcd_line2;					//2. row
 	ldi	r24, lo8(Uf_str);	0xE9
 	ldi	r25, hi8(Uf_str);	0x01
#ifdef USE_EEPROM
 	RCALL	lcd_fix_string	; lcd_PGM_string(Uf_str);	//"Uf="
#else
 	RCALL	lcd_pgm_string	; lcd_PGM_string(Uf_str);	//"Uf="
#endif
 	mov	r24, r17
 	swap	r24
 	andi	r24, 0x0F
 	rcall	mVAusgabe	; mVAusgabe(bcdnum >> 4);
 	mov	r24, r17
 	andi	r24, 0x0F	; 15
 	rcall	mVAusgabe	; mVAusgabe(bcdnum & 0x0f);
 	pop	r17
 	ret
 .endfunc


/* void SerienDiodenAusgabe() { */
/*    uint8_t first; */
/*    uint8_t second; */
/*    first = diode_sequence >> 4; */
/*    second = diode_sequence & 3; */
/*    lcd_testpin(diodes.Anode[first]); */
/*    lcd_PGM_string(AnKat);	//"->|-" */
/*    lcd_testpin(diodes.Cathode[first]); */
/*    lcd_PGM_string(AnKat);	//"->|-" */
/*    lcd_testpin(diodes.Cathode[second]); */
/*    UfAusgabe(diode_sequence); */
/* } */
	.GLOBAL SerienDiodenAusgabe
	.extern lcd_testpin
	.extern UfAusgabe

	.extern AnKat

	.func  SerienDiodenAusgabe

SerienDiodenAusgabe:
 	push	r28
 	push	r29
 	lds	r24, diode_sequence;	0x0102
 	swap	r24
 	andi	r24, 0x0F	; first = diode_sequence >> 4;
 	ldi	r28, lo8(diodes)	;0x80
 	ldi	r29, hi8(diodes)	;0x01
 	add	r28, r24	; first
 	adc	r29, zero_reg
 	ld	r24, Y		;Anode
 	RCALL	lcd_testpin	; lcd_testpin(diodes.Anode[first]);
 	ldi	r24, lo8(AnKat)	;0xA3
 	ldi	r25, hi8(AnKat)	;0x03
#ifdef USE_EEPROM
	.extern lcd_fix_string
 	RCALL	lcd_fix_string	; lcd_PGM_string(AnKat);	//"->|-"
#else
	.extern lcd_pgm_string
 	RCALL	lcd_pgm_string	; lcd_PGM_string(AnKat);	//"->|-"
#endif
 	ldd	r24, Y+6	; Cathode
 	RCALL	lcd_testpin	; lcd_testpin(diodes.Cathode[first]);
 	ldi	r24, lo8(AnKat)	;0xA3
 	ldi	r25, hi8(AnKat)	;0x03
#ifdef USE_EEPROM
 	RCALL	lcd_fix_string	; lcd_PGM_string(AnKat);	//"->|-"
#else
 	RCALL	lcd_pgm_string	; lcd_PGM_string(AnKat);	//"->|-"
#endif
 	ldi	r28, lo8(diodes)	;0x80
 	ldi	r29, hi8(diodes)	;0x01
 	lds	r24, diode_sequence;	0x0102
 	andi	r24, 0x03	; second = diode_sequence & 3;
 	add	r28, r24
 	adc	r29, zero_reg
 	ldd	r24, Y+6	; Cathode
 	RCALL	lcd_testpin	; lcd_testpin(diodes.Cathode[second]);
 	lds	r24, diode_sequence;	x0102
 	rcall	UfAusgabe	; UfAusgabe(diode_sequence);
    	pop	r29
    	pop	r28
    	ret
 .endfunc
