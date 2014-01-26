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


;unsigned int W20msReadADC(uint8_t Probe) 
;unsigned int W5msReadADC(uint8_t Probe) 
#ifdef INHIBIT_SLEEP_MODE
W20msReadADC:
  ACALL wait10ms;
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

;unsigned int ReadADC(uint8_t Probe) {
ReadADC:
;  //returns result of ADC port Probe scaled to mV resolution (unsigned int)
;  unsigned long Value;
	push	r17;
; unsigned int U; /* return value (mV) */
; uint8_t Samples; /* loop counter */
; unsigned long Value; /* ADC value */
; Probe |= (1 << REFS0); /* use internal reference anyway */
	mov	r17, r24;	Probe
	ori	r17, (1 << REFS0);
get_sample:
; ADMUX = Probe; /* set input channel and U reference */
#if ADMUX < 32
        out	ADMUX, r17;
#else
	sts	ADMUX, r17;
#endif
#ifdef AUTOSCALE_ADC
 /* if voltage reference changed run a dummy conversion */
; Samples = Probe & (1 << REFS1); /* get REFS1 bit flag */
	mov	r30, r17;
       	andi	r30, (1 << REFS1);
; if (Samples != ADCconfig.RefFlag) {
	lds	r24, ADCconfig+1;
       	cp	r30, r24;
       	breq	r2aa2 ;
#ifdef NO_AREF_CAP
;    wait100us(); /* time for voltage stabilization */
	RCALL	wait100us;
#else
	RCALL	wait10ms;
#endif
	StartADCwait                    /* start ADC and wait */
	sts	ADCconfig+1, r30;	 ADCconfig.RefFlag = Samples; /* update flag */

#endif

;unsigned int ReadADC (uint8_t Probe) {
r2aa2:
 /* * sample ADC readings */
	ldi	r18, 0x00;	 Value = 0UL; /* reset sampling variable */
	ldi	r19, 0x00; 
	movw	r20, r18;
	ldi	r30, 0x00;	 Samples = 0; /* number of samples to take */
	rjmp	r2ae8 ;
; while (Samples < ADCconfig.Samples) /* take samples */ {
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
	brne	r2ae6;
	lds	r24, ADCconfig+3;
        cpi	r24,0;
        breq	r2ae6;
       	ldi	r24, (1024 >> 8);	Value < 1024
	cpi	r18, (1024 & 0xff); 0
	cpc	r19, r24;
	brcc	r2ae6 ;
	sbrc	r17, REFS1;
	rjmp	r2ae6 ;
	ori	r17, (1 << REFS1);   Probe |= (1 << REFS1); /* select internal bandgap reference */
 #if PROCESSOR_TYP == 1280
	cbr	r17, REFS0; Probe &= ~(1 << REFS0);  /* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
 #endif
	rjmp	get_sample ;       goto get_sample; /* re-run sampling */
;    }
#endif
r2ae6:
	subi	r30, 0xFF;	  Samples++; /* one more done */
;    ADCconfig.RefFlag = Samples; /* update flag */
; }
; while (Samples < ADCconfig.Samples) /* take samples */ {
r2ae8:
	lds	r24, ADCconfig+0;
	cp	r30, r24;
	brcs	Loop ;
; }
#ifdef AUTOSCALE_ADC
; /* * convert ADC reading to voltage * - single sample: U = ADC reading * U_ref / 1024 */
; /* get voltage of reference used */
; if (Probe & (1 << REFS1)) U = ADCconfig.U_Bandgap; /* bandgap reference */
	sbrs	r17, REFS1;
	rjmp	r2afe ;
	lds	r22, ADCconfig+2;   ADCconfig.U_Bandgap
	lds	r23, ADCconfig+3;
       	rjmp	r2b02 ;
#endif
; else U = U_VCC; /* Vcc reference */
r2afe:
	lds	r22, ADCconfig+4;	ADCconfig.U_AVCC
       	lds	r23, ADCconfig+5;
; /* convert to voltage; */
; Value *= U; /* ADC readings * U_ref */
r2b02:
	ldi	r24, 0x00; 0
	ldi	r25, 0x00; 0
	ACALL	__mulsi3;
; Value /= 1023; /* / 1024 for 10bit ADC */
	ldi	r18, (1023 & 0xff);
	ldi	r19, (1023 >> 8);
	ldi	r20, 0x00; 0
	ldi	r21, 0x00; 0
	ACALL	__udivmodsi4;			R22-25 / R18-21
	movw	r22, r18;
	movw	r24, r20;
; /* de-sample to get average voltage */
; Value /= ADCconfig.Samples;
	lds	r18,ADCconfig+0;
	ldi	r19, 0x00; 0
	ldi	r20, 0x00; 0
	ldi	r21, 0x00; 0
	ACALL	__udivmodsi4;			R22-25 / R18-21
; U = (unsigned int)Value;
; return U;
;//   return ((unsigned int)(Value / (1023 * (unsigned long)ADCconfig.Samples)));
;}
	movw	r24, r18;
	pop	r17;
    	ret;

