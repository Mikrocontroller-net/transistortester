#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"
#include <stdlib.h>
//#include "autoinout.h"

.GLOBAL ReadADC
.GLOBAL W20msReadADC
.GLOBAL W10msReadADC
.GLOBAL W5msReadADC
.func ReadADC
.extern wait100us
.extern wait5ms
.extern wait10ms

#define Samples 0
#define RefFlag 1
#define U_Bandgap 2
#define U_AVCC 4

;// assembler version of ReadADC.c
;// ACALL is defined in config.h as rcall for ATmega8 and as call for ATmega168

#define RCALL rcall

#ifdef INHIBIT_SLEEP_MODE
	.macro	StartADCwait
 #if ADCSRA < 32
	sbi	ADCSRA, ADSC;
 #else
	ldi	r24, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable and start ADC */
	sts	ADCSRA, r24;
 #endif
	lds     r24, ADCSRA;    /* while (ADCSRA & (1 <<ADSC)) */
	sbrc    r24, ADSC; 
	rjmp    .-8 ;   /* wait until conversion is done */
	.endm
#else
	.macro	StartADCwait
	ldi	r24, (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */
	sts	ADCSRA, r24;
	ldi	r24, (1 << SM0) | (1 << SE);
	out	_SFR_IO_ADDR(SMCR), r24;	/*  SMCR = (1 << SM0) | (1 << SE); */
	sleep;                  /* wait for ADC */
	ldi	r24, (1 << SM0) | (0 << SE);
	out	_SFR_IO_ADDR(SMCR), r24;	/*  SMCR = (1 << SM0) | (0 << SE); */
	.endm
#endif


 .section .text
;unsigned int W20msReadADC(uint8_t Probe) 
;unsigned int W5msReadADC(uint8_t Probe) 
#ifdef INHIBIT_SLEEP_MODE
W20msReadADC:
	ACALL wait10ms;
			;// runs to W10msReadADC
W10msReadADC:
	ACALL wait5ms;
			;// runs to W5msReadADC
W5msReadADC:
	ACALL wait5ms;
			;// runs directly to ReadADC, this will replace "ACALL ReadADC + ret"
#else
W20msReadADC:
	push	r24;
	ldi	r24, 4;		/* 4 * 5ms */
	RCALL	sleep_5ms;
	rjmp   to_read;
W10msReadADC:
	push	r24;
	ldi	r24, 2;		/* 2 * 5ms */
	RCALL	sleep_5ms;
	rjmp   to_read;
W5msReadADC:
	push	r24;
	ldi	r24, 1;		/* 1 * 5ms */
	RCALL	sleep_5ms;

to_read:
	pop	r24;
			; run directly to ReadADC
#endif

;unsigned int ReadADC(uint8_t Probe) 
ReadADC:
;  //returns result of ADC port Probe scaled to mV resolution (unsigned int)
;  unsigned long Value;
	push	r17;
; unsigned int U; /* return value (mV) */
; uint8_t Samples; /* loop counter */
; unsigned long Value; /* ADC value */
	mov	r17, r24;	Probe
	ori	r17, (1 << REFS0)	; Probe |= (1 << REFS0); /* use internal reference anyway */
get_sample:
        AOUT	ADMUX, r17		; ADMUX = Probe; /* set input channel and U reference */
#ifdef AUTOSCALE_ADC
 /* if voltage reference changed run a dummy conversion */
	mov	r30, r17;
       	andi	r30, (1 << REFS1)	; Samples = Probe & (1 << REFS1); /* get REFS1 bit flag */
	lds	r24, ADCconfig+RefFlag	;
       	cp	r30, r24;
       	breq	no_ref_change			; if (Samples != ADCconfig.RefFlag) 
	sts	ADCconfig+RefFlag, r30	; ADCconfig.RefFlag = Samples; /* update flag */
 #ifdef NO_AREF_CAP
	RCALL	wait100us		; wait100us(); /* time for voltage stabilization */
 #else
  #ifdef INHIBIT_SLEEP_MODE
	RCALL	wait10ms		; wait10ms(); /* long time for voltage stabilization */
  #else
	ldi	r24, 2			; /* 2 * 5ms */
	RCALL	sleep_5ms		; wait_about10ms()
  #endif
 #endif		/* end NO_AREF_CAP */
	StartADCwait                    ; // allways do one dummy read of ADC, 112us
#endif		/* end AUTOSCALE_ADC */

;unsigned int ReadADC (uint8_t Probe) {
no_ref_change:
 /* * sample ADC readings */
	ldi	r18, 0x00;	 Value = 0UL; /* reset sampling variable */
	ldi	r19, 0x00; 
	movw	r20, r18;
	ldi	r30, 0x00;	 Samples = 0; /* number of samples to take */
	rjmp	r2ae8 ;
; while (Samples < ADCconfig.Samples) /* take samples */ 
Loop:
	StartADCwait                    /* start ADC and wait */

	lds	r22, ADCL;     Value += ADCW; /* add ADC reading */
	lds	r23, ADCH;
	add	r18, r22;
	adc	r19, r23;
	adc	r20, r1;
	adc	r21, r1;
#ifdef AUTOSCALE_ADC
;    /* auto-switch voltage reference for low readings */
;    if ((Samples == 4) && (ADCconfig.U_Bandgap > 255) && ((uint16_t)Value < 1024) && !(Probe & (1 << REFS1))) {
	cpi	r30, 0x04;	Samples == 4
	brne	cnt_next		; if ((Samples == 4) && 
	lds	r24, ADCconfig+3;
        cpi	r24,0;
        breq	cnt_next		; if ( && (ADCconfig.U_Bandgap > 255) )
       	ldi	r24, hi8(1024)		; Value < 1024
	cpi	r18, lo8(1024)
	cpc	r19, r24;
	brcc	cnt_next		; if ( &&  && ((uint16_t)Value < 1024) )
	sbrc	r17, REFS1;
	rjmp	cnt_next		; if ( &&  &&  && !(Probe & (1 << REFS1))) 
	ori	r17, (1 << REFS1);   Probe |= (1 << REFS1); /* select internal bandgap reference */
 #if (PROCESSOR_TYP == 644) || (PROCESSOR_TYP == 1280)
	cbr	r17, (1<<REFS0); Probe &= ~(1 << REFS0);  /* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
 #endif
	rjmp	get_sample ;       goto get_sample; /* re-run sampling */
#endif		/* end AUTOSCALE_ADC */
cnt_next:
	subi	r30, 0xFF;	  Samples++; /* one more done */
r2ae8:
	lds	r24, ADCconfig+Samples;
	cp	r30, r24		; while (Samples < ADCconfig.Samples) /* take samples */ 
	brcs	Loop ;
	lds	r22, ADCconfig+U_AVCC		; U = ADCconfig.U_AVCC; /* Vcc reference */
       	lds	r23, ADCconfig+U_AVCC+1;
#ifdef AUTOSCALE_ADC
; /* * convert ADC reading to voltage * - single sample: U = ADC reading * U_ref / 1024 */
; /* get voltage of reference used */
	sbrs	r17, REFS1			; if (Probe & (1 << REFS1))
	rjmp	r2b02 ;
	lds	r22, ADCconfig+U_Bandgap	; U = ADCconfig.U_Bandgap; /* bandgap reference */
	lds	r23, ADCconfig+U_Bandgap+1;
#endif		/* end AUTOSCALE_ADC */
; /* convert to voltage; */
r2b02:
	ldi	r24, 0x00			; Value *= U; /* ADC readings * U_ref */
	ldi	r25, 0x00; 0
	ACALL	__mulsi3;			; sum(ADCreads) * ADC_reference
	ldi	r18, lo8(1023)			; Value /= 1023; /* / 1024 for 10bit ADC */
	ldi	r19, hi8(1023);
	ldi	r20, 0x00; 0
	ldi	r21, 0x00; 0
	ACALL	__udivmodsi4;			R22-25 / R18-21
	movw	r22, r18;
	movw	r24, r20;
; /* de-sample to get average voltage */
	lds	r18,ADCconfig+Samples			; Value /= ADCconfig.Samples;
	ldi	r19, 0x00; 0
	ldi	r20, 0x00; 0
	ldi	r21, 0x00; 0
	ACALL	__udivmodsi4;			R22-25 / R18-21
	movw	r24, r18			;;//   return ((unsigned int)(Value / (1023 * (unsigned long)ADCconfig.Samples)));
	pop	r17;
    	ret;

 .endfunc

.func abs_diff
.GLOBAL abs_diff
abs_diff:
	movw	r18, r22
        sub	r18, r24
	sbc	r19, r25
	brcs	is_pl		; return v1-v2
	movw	r24, r18
	ret			; return v2-v1
.endfunc

.func vcc_diff
.GLOBAL vcc_diff
; uint16_t vcc_diff(uint16_t v2)	// computes unsigned_diff(ADCconfig.U_AVCC, v2)
vcc_diff:
	movw	r22, r24
	lds	r24, ADCconfig+U_AVCC
	lds	r25, ADCconfig+U_AVCC+1
	; runs to function unsigned diff
 .endfunc

.func unsigned_diff
.GLOBAL unsigned_diff
; uint16_t unsigned_diff(uint16_t v1, uint16_t v2)  // computes v1-v2 if positive, otherwise returns 0

unsigned_diff:
	cp	r22, r24
	cpc	r23, r25
	brcc	no_pl
is_pl:
	sub	r24, r22
	sbc	r25, r23
	ret			; return v1-v2
no_pl:
	ldi	r24, 0
	ldi	r25, 0
	ret	;
 .endfunc
