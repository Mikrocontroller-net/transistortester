#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "config.h"



#define zero_reg r1

/* unsigned int GetRLmultip(unsigned int cvolt) {  */

// interpolate table RLtab corresponding to voltage cvolt


/*  unsigned int uvolt; */
/*  unsigned int y1, y2; */
/*  uint8_t tabind; */
/*  uint8_t tabres; */
/*  if (cvolt >= RL_Tab_Beginn) { */
/*     uvolt = cvolt - RL_Tab_Beginn; */
/*  } else { */
/*     uvolt = 0;			// limit to begin of table */
/*  } */
/*  tabind = uvolt / RL_Tab_Abstand; */
/*  tabres = uvolt % RL_Tab_Abstand; */
/*  tabres = RL_Tab_Abstand - tabres; */
/*  if (tabind > ((RL_Tab_Length/RL_Tab_Abstand)-1)) { */
/*     tabind = (RL_Tab_Length/RL_Tab_Abstand)-1;	// limit to end of table */
/*     tabres = 0; */
/*  } */
/*  y1 = MEM_read_word(&RLtab[tabind]); */
/*  y2 = MEM_read_word(&RLtab[tabind+1]); */
/*  return ( ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand + y2); // interpolate table */
/*} */

 .GLOBAL GetRLmultip
 .func GetRLmultip

#define RL_Tab_Abstand 25	// displacement of table 25mV
#define RL_Tab_Beginn 300	// begin of table ist 300mV
#define RL_Tab_Length 1100	// length of table is 1400-300

 .section .text

; unsigned int GetRLmultip(unsigned int cvolt) 
GetRLmultip:
	push    r0
 	ldi	r18, hi8(RL_Tab_Beginn) 	; 1
 	cpi	r24, lo8(RL_Tab_Beginn)		; 44
 	cpc	r25, r18
 	brcc	is_bigger		;if (cvolt >= RL_Tab_Beginn) 
        ldi	r24, lo8(RL_Tab_Beginn)		; uvolt = 0 = RL_Tab_Begin - RL_Tab_Begin
        ldi	r25, hi8(RL_Tab_Beginn)		; limit to begin of table
is_bigger:
 	subi	r24, lo8(RL_Tab_Beginn)	;  uvolt = cvolt - RL_Tab_Beginn;
 	sbci	r25, hi8(RL_Tab_Beginn)	; 1

 	ldi	r22, lo8(RL_Tab_Abstand)	; 25
 	ldi	r23, hi8(RL_Tab_Abstand)	; 0
 	ACALL	__udivmodhi4 	;tabind = uvolt / RL_Tab_Abstand;
;  r24:25 tabres = uvolt % RL_Tab_Abstand;  // r25 allways zero
;  tabres = RL_Tab_Abstand - tabres;
 	ldi	r25, RL_Tab_Abstand	; 25
 	cpi	r22, ((RL_Tab_Length/RL_Tab_Abstand)-1)	; if (tabind > ((RL_Tab_Length/RL_Tab_Abstand)-1)) 
 	brcs	is_lower 
	mov	r25, r24		; tabres = 0 = (RL_Tab_Abstand==tabres) - tabres
 	ldi	r22, (RL_Tab_Length/RL_Tab_Abstand)-1; tabind = (RL_Tab_Length/RL_Tab_Abstand)-1;// limit to end of table
is_lower:
 	sub	r25, r24		; tabres = RL_Tab_Abstand - tabres;

	; r22 = tabind , r25 = tabres

	ldi	r30, lo8(RLtab)
	ldi	r31, hi8(RLtab)
	add	r30, r22		; + tabind
	adc	r31, zero_reg
	add	r30, r22		; + tabind (word access)
	adc	r31, zero_reg

#ifdef MEM_EEPROM
	movw	r24,r30
	ACALL	eeprom_read_byte	; y1 = MEM_read_word(&RLtab[tabind]);
	mov	r20, r24
	adiw	r30, 1			; address of high order byte
	movw	r24,r30
	ACALL	eeprom_read_byte	; y1 = MEM_read_word(&RLtab[tabind]);
	mov	r21, r24
	adiw	r30, 1			; tabind+1
	movw	r24,r30
	ACALL	eeprom_read_byte	; y2 = MEM_read_word(&RLtab[tabind+1]);
	mov	r18, r24
	adiw	r30, 1			; address of high order byte
	movw	r24,r30
	ACALL	eeprom_read_byte	; y2 = MEM_read_word(&RLtab[tabind+1]);
	mov	r19, r24
#else
 	lpm	r20, Z+		; y1 = MEM_read_word(&RLtab[tabind]);
 	lpm	r21, Z+
 	lpm	r18, Z+		; y2 = MEM_read_word(&RLtab[tabind+1]);
 	lpm	r19, Z+
#endif
;  return ( ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand + y2); // interpolate table
 	mov	r22, r25
;; 	ldi	r23, 0x00	; hi8(tabres) allways zero
 	sub	r20, r18	; y1 - y2
 	sbc	r21, r19 	; maximum of 3466 need two registers

 	mul	r22, r20	;lo8(tabres) * lo8(y1-y2)
 	movw	r24, r0		; r24:25 = *
 	mul	r22, r21	;lo8(tabres) * hi8(y1-y2)
 	add	r25, r0		; r25 + lo8(*)
;; 	mul	r23, r20	;hi8(tabres) * lo8(y1-y2) , allways zero
;; 	add	r25, r0		; r25 + lo8(*)
 	eor	r1, r1
 	adiw	r24, (RL_Tab_Abstand/2)		; 12
 	ldi	r22, lo8(RL_Tab_Abstand)	; 25
 	ldi	r23, hi8(RL_Tab_Abstand)	; 0
 	ACALL	__udivmodhi4	; ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand 
 	add	r22, r18			; + y2
 	adc	r23, r19
 	movw	r24, r22
	pop	r0
 	ret
 .endfunc
