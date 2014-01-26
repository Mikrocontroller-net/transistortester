#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"
#include <stdlib.h>
#include "config.h"

#define Log_Tab_Distance 20

 .GLOBAL get_log

; // get_log interpolate a table with the function -log(1 - (permil/1000))
; uint16_t get_log(uint16_t permil) {
get_log:
 	push	r28
 	push	r29

 	ldi	r18, lo8(Log_Tab_Distance)	;0x14
 	ldi	r19, hi8(Log_Tab_Distance)	;0x00	
 	movw	r22, r18
 	call	__udivmodhi4		; tabind = permil / Log_Tab_Distance;	// index to table
 	movw	r26, r24
;  r26:27 = tabres = permil % Log_Tab_Distance;	// fraction of table distance
;  // interpolate the table of factors
;  y1 = pgm_read_word(&LogTab[tabind]);	// get the lower table value
#if 1
	ldi	r30, lo8(LogTab);
	ldi	r31, hi8(LogTab);
        add	r30, r22
        adc	r31, r23
        add	r30, r22
	adc	r31, r23	; &LogTab[tabind]
 	lpm	r28, Z+		; y1 = pgm_read_word(&LogTab[tabind]);	// get the lower table value
 	lpm	r29, Z+
 	lpm	r20, Z+		; y2 = pgm_read_word(&LogTab[tabind+1]); // get the higher table value
 	lpm	r21, Z+
#else
 	ldi	r23, 0x00	; 0
 	movw	r30, r22	; r22:23 = tabind
 	add	r30, r30
 	adc	r31, r31
 	subi	r30, 0x98	; 152
 	sbci	r31, 0xFF	; 255
 	lpm	r28, Z+		; y1 = pgm_read_word(&LogTab[tabind]);	// get the lower table value
 	lpm	r29, Z+
;  y2 = pgm_read_word(&LogTab[tabind+1]); // get the higher table value
 	movw	r30, r22
 	add	r30, r30
 	adc	r31, r31
 	subi	r30, 0x96	; 150
 	sbci	r31, 0xFF	; 255
 	lpm	r20, Z+		; y2 = pgm_read_word(&LogTab[tabind+1]); // get the higher table value
 	lpm	r21, Z+
#endif
;  result = ((y2 - y1) * tabres ) / Log_Tab_Distance + y1; // interpolate
 	sub	r20, r28	;  (y2 - y1) 
;;	sbc	r21, r29	;  hi8(y2 - y1) is allways zero
 	mul	r20, r26	; * tabres   (maximum 19)
 	movw	r24, r0		;  r24:25 =  ((y2 - y1) * tabres )
;;	mul	r20, r27	; hi8(tabres) allways zero
;;	add	r25, r0
;;	mul	r21, r26	; hi8(y2 - Y1) always zero
;;	add	r25, r0
 	eor	r1, r1
 	movw	r22, r18	; Log_Tab_Distance
 	call	__udivmodhi4	;   ((y2 - y1) * tabres ) / Log_Tab_Distance
 	add	r22, r28	;  result = ((y2 - y1) * tabres ) / Log_Tab_Distance + y1; // interpolate
 	adc	r23, r29
 	movw	r24, r22	; return(result);
 	pop	r29
 	pop	r28
 	ret
