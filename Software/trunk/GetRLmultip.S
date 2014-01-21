#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif

#include <avr/io.h>
#include <stdlib.h>
#include "config.h"



#define zero_reg r1

/* unsigned int getRLmultip(unsigned int cvolt) {  */

// interpolate table RLtab corresponding to voltage cvolt

#define RL_Tab_Abstand 25       // displacement of table 25mV
#define RL_Tab_Beginn 300       // begin of table ist 300mV
#define RL_Tab_Length 1100      // length of table is 1400-300

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

.GLOBAL getRLmultip

getRLmultip:
 	ldi	r18, hi8(RL_Tab_Beginn) 	; 1
 	cpi	r24, lo8(RL_Tab_Beginn)		; 44
 	cpc	r25, r18
 	brcs	ad221c		;if (cvolt >= RL_Tab_Beginn) 
 	subi	r24, lo8(RL_Tab_Beginn)	;  uvolt = cvolt - RL_Tab_Beginn;
 	sbci	r25, hi8(RL_Tab_Beginn)	; 1
 	rjmp	ad2220 
ad221c:
 	ldi	r24, 0x00	;  uvolt = 0;	// limit to begin of table
 	ldi	r25, 0x00	; 0
ad2220:
 	ldi	r22, lo8(RL_Tab_Abstand)	; 25
 	ldi	r23, hi8(RL_Tab_Abstand)	; 0
 	ACALL	__udivmodhi4 	;tabind = uvolt / RL_Tab_Abstand;
;  tabres = uvolt % RL_Tab_Abstand;
;  tabres = RL_Tab_Abstand - tabres;
 	cpi	r22, ((RL_Tab_Length/RL_Tab_Abstand)-1)	; if (tabind > ((RL_Tab_Length/RL_Tab_Abstand)-1)) {
 	brcc	ad2232 
 	ldi	r25, RL_Tab_Abstand	; 25
 	sub	r25, r24		; tabres = RL_Tab_Abstand - tabres;
 	rjmp	ad2236 
ad2232:
 	ldi	r25, 0x00	; tabres = 0;
 	ldi	r22, (RL_Tab_Length/RL_Tab_Abstand)-1; tabind = (RL_Tab_Length/RL_Tab_Abstand)-1;// limit to end of table
ad2236:
        ; r22 = tabind , r25 = tabres

;; 	ldi	r23, 0x00	; 0
;; 	movw	r30, r22
;; 	add	r30, r30
;; 	adc	r31, r31
;; 	subi	r30, 0x98	; 152
;; 	sbci	r31, 0xFF	; 255
        ldi	r30, lo8(RLtab)
        ldi	r31, hi8(RLtab)
        add	r30, r22
	adc	r31, zero_reg
        add	r30, r22
	adc	r31, zero_reg

 	lpm	r20, Z+		; y1 = MEM_read_word(&RLtab[tabind]);
 	lpm	r21, Z+
;; 	movw	r30, r22
;; 	add	r30, r30
;; 	adc	r31, r31
;; 	subi	r30, 0x96	; 150
;; 	sbci	r31, 0xFF	; 255
 	lpm	r18, Z+		; y2 = MEM_read_word(&RLtab[tabind+1]);
 	lpm	r19, Z+
;  return ( ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand + y2); // interpolate table
 	mov	r22, r25
;; 	ldi	r23, 0x00	; tabres.L
 	sub	r20, r18	; y1 - y2
 	sbc	r21, r19

 	mul	r22, r20	;lo8(tabres) * lo8(y1-y2)
 	movw	r24, r0		; r24:25 =
 	mul	r22, r21	;lo8(tabres) * hi8(y1-y2)
 	add	r25, r0		; r25 + lo8(*)
;; 	mul	r23, r20	;hi8(tabres) * lo8(y1-y2)
;; 	add	r25, r0		; r25 + lo8(*)
 	eor	r1, r1
 	adiw	r24, (RL_Tab_Abstand/2)		; 12
 	ldi	r22, lo8(RL_Tab_Abstand)	; 25
 	ldi	r23, hi8(RL_Tab_Abstand)	; 0
 	ACALL	__udivmodhi4	; ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand 
 	add	r22, r18			; + y2
 	adc	r23, r19
 	movw	r24, r22
 	ret
