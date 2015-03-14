#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include "config.h"
#include "part_defs.h"



;// show the Pin Layout of the device 
;  void PinLayout(char pin1, char pin2, char pin3) 
;// pin1-3 is EBC or SGD or CGA
; typedef struct {
;   unsigned long hfe;		//0 current amplification factor 
;   unsigned int uBE;		//4 B-E-voltage of the Transistor
;   unsigned int current;	//6 current of Drain in 0.01mA
;   				//  for bipolar current is ICE0
;   unsigned int gthvoltage;	//8 Gate-threshold voltage 
;   				// for bipolar gthvoltage is ICEs in 0.01mA
;   uint8_t b,c,e;		//10,11,12 pins of the Transistor
;   uint8_t count;		//13
; }trans_t;

#define RCALL rcall

 .GLOBAL PinLayout
 .func PinLayout
 .extern lcd_data
 .extern lcd_space
 .extern lcd_testpin
 .extern _trans
#define OFFS_b 12	/* offset to trans.b */
#define OFFS_c 13	/* offset to trans.c */
#define OFFS_e 14	/* offset to trans.e */
 .extern N123_str
 .extern N321_str

PinLayout:

#ifndef EBC_STYLE 
;   // Layout with 123= style
 	push	r14
 	push	r15
 	push	r16
 	push	r17

 	mov	r17, r24		; Pin1
 	mov	r16, r22		; Pin2
 	mov	r15, r20		; Pin3
 	ldi	r24, lo8(N123_str)	; 0x0B
 	ldi	r25, hi8(N123_str)	; 0x01	
#ifdef USE_EEPROM
 	RCALL	lcd_fix_string	; lcd_MEM_string(N123_str);		//" 123="
#else
 	RCALL	lcd_pgm_string	; lcd_MEM_string(N123_str);		//" 123="
#endif
 	eor	r14, r14	; for (ipp=0;
loop1:
 	lds	r30, _trans
 	lds	r31, _trans+1
 	ldd	r24, Z+OFFS_e	; _trans->e
 	cp	r14, r24
 	brne	checkb		; if (ipp == _trans->e) 
 	mov	r24, r17	; pin1
        rjmp	data_ipp  	; lcd_data(pin1);	// Output Character in right order
checkb:
 	ldd	r24, Z+OFFS_b	; _trans->b
 	cp	r14, r24	; if (ipp == _trans->b) 
 	brne	checkc
 	mov	r24, r16
        rjmp	data_ipp	;  lcd_data(pin2);
checkc:
 	ldd	r24, Z+OFFS_c	; _trans->c
 	cp	r14, r24
 	brne	next_ipp		; if (ipp == _trans->c) 
 	mov	r24, r15
data_ipp:
 	RCALL	lcd_data	;  lcd_data(pin3);
next_ipp:
 	inc	r14
 	mov	r24, r14
 	cpi	r24, 0x03	;  for (     ;ipp<3;ipp++) {
 	brne	loop1 

 	pop	r17
 	pop	r16
 	pop	r15
 	pop	r14
 	ret
#else
 #if EBC_STYLE == 321 
; // Layout with 321= style
 	push	r14
 	push	r15
 	push	r16
 	push	r17
 	mov	r17, r24		; Pin1
 	mov	r16, r22		; Pin2
 	mov	r15, r20		; Pin3
   
 	ldi	r24, lo8(N321_str)	; 0x0B
 	ldi	r25, hi8(N321_str)	; 0x01	
#ifdef USE_EEPROM
 	RCALL	lcd_fix_string		; lcd_MEM_string(N321_str);	//" 321="
#else
 	RCALL	lcd_pgm_string		; lcd_MEM_string(N321_str);	//" 321="
#endif
 	ldi	r24, 0x03	; 3
 	mov	r14, r24	; ipp = 3;
loop2:
 	dec	r14		; ipp--;
 	lds	r30, _trans	;0x0142
 	lds	r31, _trans+1	;0x0143
 	ldd	r24, Z+OFFS_e	; _trans->e
 	cp	r14, r24
 	brne	checkb		; if (ipp == _trans->e) 
 	mov	r24, r17 	; lcd_data(pin1);	// Output Character in right order
	rjmp	data_ipp2
checkb:
 	lds	r30, _trans	;0x0142
 	lds	r31, _trans+1	;0x0143
 	ldd	r24, Z+OFFS_b	; _trans->b
 	cp	r14, r24
 	brne	checkc		; if (ipp == _trans->b) 
 	mov	r24, r16	;  lcd_data(pin2);
	rjmp	data_ipp2
checkc:
 	ldd	r24, Z+OFFS_c	; _trans->c
 	cp	r14, r24
 	brne	next_ipp2	; if (ipp == _trans->c) 
 	mov	r24, r15 	; lcd_data(pin3);
data_ipp2:
 	RCALL	lcd_data	; lcd_data(pinx);
next_ipp2:
 	and	r14, r14	; while (ipp != 0) 
 	brne	 loop2 

 	pop	r17
 	pop	r16
 	pop	r15
 	pop	r14
 	ret

 #else 
; // Layout with EBC= style 
 	push	r15
 	push	r16
 	push	r17
 	mov	r17, r24		; Pin1
 	mov	r16, r22		; Pin2
 	mov	r15, r20		; Pin3
 	RCALL	lcd_space	; lcd_space();
	mov	r24, r17	; Pin1
 	RCALL	lcd_data	; lcd_data(pin1);
	mov	r24, r16	; Pin2
 	RCALL	lcd_data	; lcd_data(pin2);
	mov	r24, r15	; Pin3
 	RCALL	lcd_data	; lcd_data(pin3);
 	ldi	r24, '='	; 0x3D
 	RCALL	lcd_data	; lcd_data('=');
 	lds	r30, _trans	;0x0142
 	lds	r31, _trans+1	;0x0143
 	ldd	r24, Z+OFFS_e	; _trans->e
 	RCALL	lcd_testpin	; lcd_testpin(_trans->e);
 	lds	r30, _trans	;0x0142
 	lds	r31, _trans+1	;0x0143
 	ldd	r24, Z+OFFS_b	; _trans->b
 	RCALL	lcd_testpin	; lcd_testpin(_trans->b);
 	lds	r30, _trans	;0x0142
 	lds	r31, _trans+1	;0x0143
 	ldd	r24, Z+OFFS_c	; _trans->c
 	RCALL	lcd_testpin	; lcd_testpin(_trans->c);

 	pop	r17
 	pop	r16
 	pop	r15
 	ret
 #endif
#endif

 .endfunc
