#include <avr/io.h>
//#include <config.h>
#include <stdlib.h>
//#include "autoinout.h"

#ifndef ANZ_MESS
#define ANZ_MESS 44
#endif

.GLOBAL ReadADC
.GLOBAL W20msReadADC
.GLOBAL W5msReadADC
// assembler version of ReadADC.c

;unsigned int W20msReadADC(uint8_t mux) {
W20msReadADC:
  rcall	wait10ms;
  rcall wait5ms;
// runs to W5msReadADC

;unsigned int W5msReadADC(uint8_t mux) {
W5msReadADC:
  rcall	wait5ms;
// runs directly to ReadADC, this will replace "rcall ReadADC + ret"

;unsigned int ReadADC(uint8_t mux) {
ReadADC:
  //returns result of ADC port mux scaled to mV resolution (unsigned int)
;  unsigned int adcx;
;  uint8_t jj;

;  ADMUX = mux | (1<<REFS0);
  mov   r22,r24
  ori	r24, 1<<REFS0		; make shure, that REFS0 is allways set
#if ADMUX < 32
  out   ADMUX, r24
#else
  sts	ADMUX, r24
#endif
#ifdef AUTOSCALE_ADC
  sbrc	r24,REFS1		; skip if not 1.1V reference
  rjmp  lowADC			; 1.1V is selected, go to special routine
#endif
; normal 5V reference
;  adcx = (ANZ_MESS/11); 	// round up the result (+4)
;  ADCSRA |= (1<<ADSC);		//start Conversion
#if ADCSRA < 32
  sbi	ADCSRA,ADSC
#else
  lds	r18, ADCSRA
  ori	r18, (1<<ADSC)
  sts	ADCSRA, r18
#endif

//  while (ADCSRA&(1<<ADSC));		// wait for ADC finish
w3f6:
#if ADCSRA < 32
  sbi	ADCSRA,ADSC
#else
  lds	r18, ADCSRA
  sbrc	r18, ADSC
#endif
  rjmp	w3f6 

#ifdef NO_AREF_CAP
; wait50us();		// AVR126 recommend a 70µs wait-time for band gap ADC
  rcall wait50us
#else
;  wait300us();
  rcall	wait300us
#endif

;  adcx = (ANZ_MESS/11); 	// round up the result (+4)
  ldi	r24, (ANZ_MESS/11)
  ldi	r25, 0

;  for (jj = 0; jj < ANZ_MESS; jj++) {
     //repeat ANZ_MESS measurements for oversampling
  ldi	r20, 0

forlop1:
;     ADCSRA |= (1<<ADSC);	//start conversion
#if ADCSRA < 32
  sbi	ADCSRA,ADSC
#else
  lds	r18, ADCSRA
  ori	r18, (1<<ADSC)
  sts	ADCSRA, r18
#endif

;     while (ADCSRA & (1<<ADSC));	//wait for ADC finished
w416:
#if ADCSRA < 32
  sbic	ADCSRA,ADSC
#else
  lds	r18, ADCSRA
  sbrc	r18, ADSC
#endif
  rjmp	w416 

;     adcx += ADCW;		//  add measurements 
#if ADCH < 32
  in	r18,ADCL
  in	r19,ADCH
#else
  lds	r18, ADCL
  lds	r19, ADCH
#endif
  add	r24, r18
  adc	r25, r19
#ifdef AUTOSCALE_ADC
;     if ((jj == 4) && (adcx < 1024)){
  cpi	r20, 0x04	; 4
  brne	r436 
  ldi	r21, 0x04	; 4 * 256
  cpi	r24, 0
  cpc	r25, r21
  brcs	lowADC 
#endif

;  for (jj = 0; jj < ANZ_MESS; jj++) {
r436:
  subi	r20, 0xFF	; 255
  cpi	r20, ANZ_MESS
  brne	forlop1 

#if ANZ_MESS == 22
;  adcx *= 2;		// multiply sum by 2
  add	r24,r24
  adc	r25,r25		;*2
#endif
  
#if ANZ_MESS == 11
;  adcx *= 4;		// multiply sum by 4
  add	r24,r24
  adc	r25,r25		;*2
  add	r24,r24
  adc	r25,r25		;*2
#endif

;  return adcx/9;
  ldi	r22, 0x09	; 9
  ldi	r23, 0

#ifdef AUTOSCALE_ADC
  rjmp	diviret 
  
lowADC:
;  adcx = 0;
;  ADMUX = mux | (1<<REFS1) | (1<<REFS0); // Internal 1.1V Referenz
  ori	r22, (1<<REFS1)|(1<<REFS0)
#if ADMUX < 32
  out	ADMUX, r22
#else
  sts	ADMUX, r22
#endif

;  ADCSRA |= (1<<ADSC);		//start conversion
#if ADCSRA < 32
  sbi	ADCSRA,ADSC
#else
  lds	r24, ADCSRA
  ori	r24, (1<<ADSC)
  sts	ADCSRA, r24
#endif

;  while (ADCSRA&(1<<ADSC));	//wait for ADC finished
w454:
#if ADCSRA < 32
  sbic	ADCSRA, ADSC
#else
  lds	r24, ADCSRA
  sbrc	r24, ADSC
#endif
  rjmp	w454 

#ifdef NO_AREF_CAP
;  wait300us();			//wait 300µs for no or 1nF AREF capacitor
  rcall wait300us
#else
;  wait5ms();			//wait 6ms for up to 100nF AREF capacitor
;  wait1ms();
  rcall	wait5ms
  rcall	wait1ms
#endif

;  adcx = 0;
  ldi	r24, 0x00	; 0
  ldi	r25, 0x00	; 0

;  for (jj=0; jj<minmul; jj++) {
  ldi	r20, 0x00	; 0
  rjmp	r48c

;     ADCSRA |= (1<<ADSC);	//start conversion
forlop2:
#if ADCSRA < 32
  sbi	ADCSRA, ADSC
#else
  lds	r18, ADCSRA
  ori	r18, (1<<ADSC)
  sts	ADCSRA, r18
#endif

;     while (ADCSRA&(1<<ADSC));		//wait for ADC finished
w476:
#if ADCSRA < 32
  sbic	ADCSRA, ADSC
#else
  lds	r18, ADCSRA
  sbrc	r18, ADSC
#endif
  rjmp	w476 

;     adcx += ADCW;			//add Measurement results
#if ADCSRA < 32
  in	r18, ADCL
  in	r19, ADCH
#else
  lds	r18, ADCL
  lds	r19, ADCH
#endif
  add	r24, r18
  adc	r25, r19

;  for (jj=0; jj<minmul; jj++) {
  subi	r20, 0xFF	; 255
r48c:
  lds	r18, minmul
  cp	r20, r18
  brcs	forlop2 

;  return (adcx/mindiv);		//divide by mindiv gives a resolution of mV
  lds	r22, mindiv
  ldi	r23, 0
#endif
diviret:
  rcall	__udivmodhi4
  movw	r24, r22
  clr	r0
  ret

