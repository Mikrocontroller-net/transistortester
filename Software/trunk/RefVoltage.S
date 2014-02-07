#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/eeprom.h>
#include "config.h"
#include "part_defs.h"

#define zero_reg r1
 

#define Ref_Tab_Abstand 50
	; displacement of table is 50mV 
#define Ref_Tab_Beginn 1000
	;	// begin of table is 1000mV 

 .GLOBAL RefVoltage
 .func RefVoltage
 .extern eeprom_read_byte
 .extern RHtab
 .section .text
 

RefVoltage:

#ifdef AUTO_CAL
 	ldi	r24, lo8(ref_offset)	; 1
 	ldi	r25, hi8(ref_offset)	; 0
;; 	ACALL	eeprom_read_word	; eeprom_read_word((uint16_t *)(&ref_offset));
;; 	lds	r18, ref_mv
;; 	lds	r19, ref_mv+1
;; 	add	r18, r24		; referenz =  ref_mv +
;; 	adc	r19, r25
	ACALL	eeprom_read_byte	; eeprom_read_word((uint16_t *)(&ref_offset)); done as two read_byte
	mov	r19, r24
	ldi	r24, lo8(ref_offset+1)
	ldi	r25, hi8(ref_offset+1)
	ACALL	eeprom_read_byte
 	lds	r18, ref_mv
	add	r18, r19
 	lds	r19, ref_mv+1
	adc	r19, r24
#else
	lds	r18, ref_mv
	lds	r19, ref_mv+1
	subi	r18, -REF_C_KORR		; referenz = ref_mv + REF_C_KORR;
	adc	r19, zero_reg
#endif
        sts	ref_mv_offs, r18
        sts	ref_mv_offs+1, r19

#ifdef AUTO_RH 
 	ldi	r24, hi8(Ref_Tab_Beginn)	; 3
 	cpi	r18, lo8(Ref_Tab_Beginn)	; 232
 	cpc	r19, r24
 	brcs	ad210e 				; if (referenz >= Ref_Tab_Beginn) 
 	movw	r24, r18
 	subi	r24, lo8(Ref_Tab_Beginn)	; 232 referenz -= Ref_Tab_Beginn;
 	sbci	r25, hi8(Ref_Tab_Beginn)	; 3
 	rjmp	ad2112 
ad210e:
 	ldi	r24, 0x00	; referenz = 0;		// limit to begin of table
 	ldi	r25, 0x00	; 0
ad2112:
 	ldi	r22, lo8(Ref_Tab_Abstand)	; 50 tabind = referenz / Ref_Tab_Abstand;
 	ldi	r23, hi8(Ref_Tab_Abstand)	; 0
 	ACALL	__udivmodhi4
;  r22 = tabind = referenz / Ref_Tab_Abstand;
;  r24 =  tabres = referenz % Ref_Tab_Abstand;
 	cpi	r22, 0x08	; if (tabind > 7) 
 	brcs	ad2120 
 	ldi	r22, 0x07	; tabind = 7;		// limit to end of table
ad2120:
; // interpolate the table of factors

	LDIZ	RHtab
	add	r30, r22
	adc	r31, zero_reg
	add	r30, r22
	adc	r31, zero_reg

 	lpm	r20, Z+		; y1 = pgm_read_word(&RHtab[tabind]);
 	lpm	r21, Z+
 	lpm	r18, Z+		; y2 = pgm_read_word(&RHtab[tabind+1]);
 	lpm	r19, Z+
 	ldi	r22, Ref_Tab_Abstand	; 50
 	sub	r22, r24		; tabres = Ref_Tab_Abstand-tabres;
;  // interpolate the table of factors
;  // RHmultip is the interpolated factor to compute capacity from load time with 470k
;; 	ldi	r23, 0x00	; 0
 	sub	r20, r18	; y1 - y2
 #if FLASHEND > 0x1fff
	sbc	r21, r19	; hi8(y1 - y2) is usually allway zero
 #endif
 	mul	r22, r20	; lo8(tabres) * lo8(y1-y2)
 	movw	r24, r0		; r24:25 = *
 #if FLASHEND > 0x1fff
 	mul	r22, r21	; lo8(tabres) * hi8(y1-y2)
 	add	r25, r0		; r25 + lo8(*)
 #endif
;; 	mul	r23, r20	; hi8(tabres) * lo8(y1*y2) , allways zero
;; 	add	r25, r0		; r25 + lo8(*)

 	eor	r1, r1
 	adiw	r24, (Ref_Tab_Abstand/2)	; 25
 	ldi	r22, lo8(Ref_Tab_Abstand)	; 50
 	ldi	r23, hi8(Ref_Tab_Abstand)	; 0
 	ACALL	__udivmodhi4	; ((y1 - y2) * tabres + (Ref_Tab_Abstand/2)) / Ref_Tab_Abstand 
 	add	r22, r18	; + y2
 	adc	r23, r19
 	sts	RHmultip+1, r23
 	sts	RHmultip, r22
#else
	ldi	r22, lo8(DEFAULT_RH_FAKT)
	ldi	r23, hi8(DEFAULT_RH_FAKT)
 	sts	RHmultip, r22
 	sts	RHmultip+1, r23
#endif 
#ifdef AUTO_CAL
	ldi	r24, lo8(RefDiff)
	ldi	r25, hi8(RefDiff)
	ACALL	eeprom_read_byte	; (int8_t)eeprom_read_byte((uint8_t *)&RefDiff));
	eor	r25, r25		; set zero for sign extend
        sbrc	r24, 7			; minus?
	com	r25			; yes, set to 0xff
        lds	r22, ref_mv		; ADCconfig.U_Bandgap = (ref_mv + (int8_t)eeprom_read_byte((uint8_t *)&RefDiff));
        lds	r23, ref_mv+1
	add	r24, r22
	adc	r25, r23
#else
        ldi	r22, lo8(REF_R_KORR)
        ldi	r23, hi8(REF_R_KORR)
        lds	r24, ref_mv		; ADCconfig.U_Bandgap = (ref_mv + REF_R_KORR);
        lds	r25, ref_mv+1
	add	r24, r22
        adc	r25, r23
#endif
#define U_Bandgap 2
        sts	ADCconfig+U_Bandgap+1, r25
        sts	ADCconfig+U_Bandgap, r24
	sts	adc_internal_reference+1, r25		; adc_internal_reference = ADCconfig.U_Bandgap;
	sts	adc_internal_reference, r24


 	ret

 .endfunc
