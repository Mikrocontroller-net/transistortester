
// June-Sept 2015, pa3fwm@amsat.org

#ifdef SamplingADC_CNT
#include "samplingADC_cnt.S"		/* take replacement with counter1 use */
#else
#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"
#include <stdlib.h>

.GLOBAL samplingADC
.func samplingADC


#if AUTO_CLOCK_DIV == ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))
 #define TICS_PER_ADC_CLOCK 128
#elif AUTO_CLOCK_DIV == ((1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0))
 #define TICS_PER_ADC_CLOCK 64
#else
#error Unsupported clock frequency
#endif

#define TICS_PER_ADC_CYCLE 13*TICS_PER_ADC_CLOCK
#define Lessdelay (256-2*TICS_PER_ADC_CLOCK)


.section .text

samplingADC:
; prototype for C declared in tt_function.h ; documentation is also there
;uint16_t samplingADC(R24:25 what, R22:23 array[], R20 nn, R18 Rport_1, R16 Rddr_1, R14 Rport_0, R12 Rddr_0) {}

; r25 is span
;ldi r25,16

	mov	r30,r22			; r31:r30 := ptr
	mov	r31,r23

	cpi	r25,0		; set nonsensical span of 0 to 1
	brne L16
	inc r25
L16:

 	AOUT	R_PORT, R14	;   Rport_0
 	AOUT	R_DDR, R12	;   Rddr_0

 	ldi	r27, (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27		; start first ADC with ext trigger, but start immediately also, to get done that first conversion which takes extra long
 	ldi	r26, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0); 
 	sts	ADCSRB, r26		; trigger source is COUNTER1 compare match B

        ldi	r26,(1<<WGM12)
 	sts	TCCR1B, r26		; TCCR1B = 0;  stop counter1
 	sts	TCCR1A, r1  		; TCCR1A = 0;  set counter1 to Clear Timer on Compare Match mode
	ldi	r26,0xff
 	sts	TCNT1H, r26		; set initial counter to -1
 	sts	TCNT1L, r26
 	ldi r26,(1<<OCF1B)|(1<<OCF1A)
 	sts	TIFR1, r26		; reset both counter compare interrupts, otherwise ADC might be started prematurely
 	sts	OCR1BH, r1		; schedule start of ADC cycle at counter=0, i.e., essentially immediately
 	sts	OCR1BL, r1		; 

					; prepare timing and registers for main loop
					; r27:26 will contain the TOP value for the counter, i.e., 1 less than the pulse-generation period 
	ldi 	r27,hi8(TICS_PER_ADC_CYCLE-1)
	ldi 	r26,lo8(TICS_PER_ADC_CYCLE-1)
	sub 	r26,r25			; this defaults to 1663-span, but may need to be incremented if measurement will take longer due to large span
	ldi	r21,1			; r21 will contain number of ADC readings per pulse
 	mov	r19,r25			; load span into r19; total measurement will cover r19*nn tics, which is upperbounded by 256*r19
 					; one ADC cycle is 13*128 (or 13*64) tics, but we need some time for generating the pulse, so can fit say at most 8*128 = 4*256 tics (or 8*64 = 2*256)
L14:
 	cpi	r19,(1+TICS_PER_ADC_CLOCK/32)	; if "remaining" span <=4 (or <=2), we're fine, don't need to increase pulse generation period
 	brcs L13
 	subi	r26,lo8(-TICS_PER_ADC_CYCLE)	; otherwise, extend pulse generation period by 1664
 	sbci	r27,hi8(-TICS_PER_ADC_CYCLE)		
 	inc	r21			; increment number of ADC readings per pulse
 	subi	r19,3*(TICS_PER_ADC_CLOCK/64)		; each extra ADC cycle included in pulse generation period makes space for 13*128 (or 13*64) tics; for simplicity we (safely) calculate as if it were 12*128 = 6*256 (or 12*64 = 3*256)
 	rjmp L14			; check whether this was enough
L13:
 	sts	OCR1AH, r27		; store calculated TOP value for counter 1
 	sts	OCR1AL, r26		; 


wait_adc:
 	lds	r26, ADCSRA		
 	sbrs	r26, ADIF
 	rjmp	wait_adc 		; wait until the initial conversion finishes
 	ldi	r27, (1<<ADEN) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27		; reset ADC interrupt flag

 	ldi r26,(1<<OCIE1A)
 	sts	TIMSK1, r26		; disable counter1 compare B Interrupt (used to trigger ADC), enable counter1 compare A Interrupt (used to exit sleep to generate next pulse)

 	ldi	r26, (1<<CS10)|(1<<WGM12)
 	sts	TCCR1B, r26		; start counter1 at full speed

	ldi	r22,2			; skip first ADC result since it is nonsense (it predates the start of the pulse or step)

	ldi	r26, 0			; switch ADC to free-running mode (we can do that here because can be sure that by now it has been triggered)
	sts	ADCSRB, r26		; 


; we'll have counter1 counting up from 0 to about 1663 (or multiples of that)
; each time it overflows, we'll start our signal for triggering the response
; first sample will be taken 2*128 clockcycles after triggering
; so this whole signal generation procedure should take 256 clockcycles
; note: there's some uncertainty due to the unknown time the interrupt handler takes, which may be compiler-dependent
; at 8 MHz, the first sample is taken after only 128 clockcycles; this is taken into account via the Lessdelay macro

backtosleep:
	ldi r26,(1<<SE)
	sts SMCR,r26
	sleep

	; toggle output (back) to the idle state 
 	AOUT	R_PORT, R14	;   Rport_0
 	AOUT	R_DDR, R12	;   Rddr_0

	sbrs 	r24,smplADC_direct
	rjmp stepresponse

 	; wait a bit less than 256 ticks; precise value determined experimentally, by looking at the sampled data and aligning start of response with first sample
//#define Delay_pulse (207-Lessdelay)
#define Delay_pulse (201-Lessdelay)	/* -6 additional clock tics (push, ldi, out, pop) */

 	ldi	r26,(Delay_pulse/3)
#if Delay_pulse%3>0
	nop
#endif
#if Delay_pulse%3>1
	nop
#endif
L11:	dec r26
	brne L11
	; do the "direct" pulse, i.e., apply the pulse via de ADC pins, with no series resistance
	push 	r30
	push	r31
	in	r30, _SFR_IO_ADDR(ADC_DDR)	;
	in	r31, _SFR_IO_ADDR(ADC_PORT)	;
	ldi	r26, (1<<TP3)	;
	sbrc	R12, PIN_RL2	; is the bit for TP2 resistor set?
	ldi	r26, (1<<TP2)	;
	sbrc	R12, PIN_RL1	; ist the bit for TP3 resistor set?
	ldi	r26, (1<<TP1)	;
	; r26 now hold the bit for the direct ADC port
	mov	r27, r31	; ADC_PORT state
	or	r27, r26	; r27 is the for ADC port with HiPin set to 1
	or	r26, r30	; r26 enables the HiPin and LoPin output,  ADC_DDR
	AOUT	ADC_PORT, r27	; set Hipin to high
	AOUT	R_DDR, R16	; R_DDR = Rddr1 open all resistor ports
	AOUT	ADC_DDR, r26	; one clock tic high without resistor at HiPin, current about 5V/(42 Ohm)=119mA !!!
	AOUT	ADC_DDR, r30	; disable the HiPin output
	AOUT	ADC_PORT, r31	; reset Hipin to low
	pop	r31
	pop	r30
	rjmp	waitevent


stepresponse:
 	; wait a bit less than 256 ticks; precise value determined experimentally
// #define Delay_step (222-Lessdelay)
#define Delay_step (216-Lessdelay)	/* -6 additional clock tics (push, ldi, out, pop) */

 	ldi	r26,(Delay_step/3)
#if Delay_step%3>0
	nop
#endif
#if Delay_step%3>1
	nop
#endif
L10:	dec r26
	brne L10
	; generate start of step signal
	; this is (should be) aligned with the first sample
 	AOUT	R_DDR, R16	;  R_DDR = Rddr_1

waitevent:			; waiting loop: we wait until either counter1 is almost going to be reset, or the AD converter has a result
	lds	r26, TCNT1L	; need to read TCNT1L to latch TCNT1H
	lds	r26, TCNT1H
	lds	r27, OCR1AH
	inc	r26
	cp	r26,r27		; check if TCNT1H is getting near OCR1AH; if so, go to sleep to be sure not to miss the interrupt
	brcc backtosleep

 	lds	r26, ADCSRA	; otherwise:
 	sbrs	r26, ADIF	; check if conversion done (interrupt flag is raised)
 	rjmp	waitevent	; if not, go back to checking counter

 				; the ADC gives a result
 	ldi	r27, (1<<ADEN) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27	; reset ADC interrupt flag

 				; now need to check whether we should read or discard it
	dec	r22		; r22 counts AD conversions within one pulse cycle
	brne	waitevent	; if r22 not yet zero, it's not yet our turn

	lds	r22, ADCL	; read ADC
	lds	r23, ADCH

;lds	r25, TCNT1L
;lds	r23, TCNT1H
;mov r22,r25
	ld	r19,z		; and store, accumulating if that bit in r24 is set
	sbrc 	r24,smplADC_cumul
	add	r22,r19
	st	z+,r22
	ld	r19,z
	sbrc 	r24,smplADC_cumul
	adc	r23,r19
	st	z+,r23

	mov	r22,r21		; reinitialize r22

	dec r20			; decrement counter of remaining samples
	brne waitevent

end:
 	sts	TIMSK1, r1	; disable counter1 interrupts
 	sts	TCCR1B, r1	; stop counter1
 	ldi	r27, AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27	; disable ADC
	ret




#endif  /* SamplingADC_CNT */

