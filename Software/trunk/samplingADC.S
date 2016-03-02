
// June 2015 - Jan 2016, pa3fwm@amsat.org

#ifdef SamplingADC_CNT
#include "samplingADC_cnt.S"		/* take replacement with counter1 use */
#else
#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"
#include <stdlib.h>

.GLOBAL samplingADC_freqgen
.func samplingADC_freqgen
.endfunc

.GLOBAL samplingADC_freqgen_sck
.func samplingADC_freqgen_sck
.endfunc

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
samplingADC_freqgen:
samplingADC_freqgen_sck:
; prototypes for C declared in tt_function.h ; documentation is also there
;uint16_t samplingADC(R24:25 what, R22:23 array[], R20 nn, R18 Rport_1, R16 Rddr_1, R14 Rport_0, R12 Rddr_0, R10:11 freq, R8 shortcircuitduration ) {}
; the last two arguments are only used WITH_XTAL, and are optional (can be omitted if not needed; registers will not be clobbered)

#ifdef WITH_XTAL
	push r4				; r4:5 will be used for the high-pass filter
	push r5
	push r17
	mov r5,r1
	dec r5				; set bit 7 in r5 to mark it for initialization

	sbrs r24,smplADC_sck		; if we'll need to short-circuit the DUT later on, store the correct value of ADC_DDR in r9
	rjmp L30a
	push r9
	in r9, _SFR_IO_ADDR(ADC_DDR)	;
L30a:
	push r7
	lds r7,ADMUX
#endif

; r25 is span

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
#ifdef WITH_XTAL
	sbrs r24,smplADC_many		; if we have the long excitation
	rjmp L20a
	ldi	r26,3			; schedule start of ADC cycle at counter=3*256
	sts	OCR1BH, r26
	dec r26
	sts	TCNT1H, r26		; and make sure TCNT1 is initialized sucht that this is still essentially immediately
	ldi	r26,0xff
	sts	TCNT1L, r26
L20a:
#endif

#ifdef WITH_XTAL
 				; compute number of cycles for genfreq:
 	push r24			; we need registers r24 and r25 for calling __udivmodhi4
 	push r25
	ldi	r25,184/4		; total duration should be at most 255 cycles, but there's some 66 cycles of overhead (determined experimentally)
	sbrc r24,smplADC_many		; except in case of long set of pulses for crystal excitation, then
	ldi	r25,(184+3*256)/4	;   total duration should be at most 4*256 cycles
L20:					; note that we've divided r25:24 by 4 in the above, so it fits in 16 bits
	ldi	r24,0
	movw	r22,r10
	subi	r23,-8			; period is 8+r11+r10/256
	lsr	r23			; divide period by 4 since we've divided duration by 4 too
	ror	r22
	lsr	r23
	ror	r22
	call __udivmodhi4
	mov	r0,r22			; store number of pulses in r0
	inc	r0			; 1 more pulse than periods
	pop r25
	pop r24
#endif

				; prepare timing and registers for main loop
					; r27:26 will contain the TOP value for the counter, i.e., 1 less than the pulse-generation period
	ldi 	r27,hi8(TICS_PER_ADC_CYCLE-1)
	ldi 	r26,lo8(TICS_PER_ADC_CYCLE-1)
	sub 	r26,r25			; this defaults to 1663-span, but may need to be incremented if measurement will take longer due to large span
	ldi	r21,1			; r21 will contain number of ADC readings per pulse
 	mov	r19,r25			; load span into r19; total measurement will cover r19*nn tics, which is upperbounded by 256*r19
 					; one ADC cycle is 13*128 (or 13*64) tics, but we need some time for generating the pulse, so can fit say at most 10*128 = 5*256 tics (or 8*64 = 2*256; 10*64 isn't a multiple of 256, so would require more code)
 					; NEW: leave a bit more space so we can also do the long train of pulses for crystals
L14:
 	cpi	r19,(1+10*TICS_PER_ADC_CLOCK/256)	; if "remaining" span <=5 (or <=2), we're fine, don't need to increase pulse generation period
#ifdef WITH_XTAL
	sbrc r24,smplADC_many
 	cpi	r19,(2+2*TICS_PER_ADC_CLOCK/256)	; in case of long pulse train: if "remaining" span <=3 (or <=2), we're fine, don't need to increase pulse generation period
#endif
 	brmi L13
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
; each time it overflows, we'll start our signal for "exciting" the DUT
; first sample will be taken 2*128 clockcycles after triggering
; so this whole signal generation procedure should take 256 clockcycles
; note: there's some uncertainty due to the unknown time the interrupt handler takes, which may be compiler-dependent
; at 8 MHz, the first sample is taken after only 128 clockcycles; this is taken into account via the Lessdelay macro

; we have the following excitation options, governed by the "what" parameter:
; - 0 (default):              step (Rddr_0 -> Rddr_1) and optionally single pulse (Rport_1)
; - smplADC_freq:             bunch of pulses via Rport_1
; - ... |smplADC_many:        4 times longer bunch of pulses via Rport_1
; - ... |smplADC_sck:         short-circuit the DUT for a while after applying pulses
; - smplADC_direct:           single pulse via the ADC pins, i.e. with no series resistance ("direct" pulse)

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
#define Delay_pulse (201-Lessdelay)

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


#ifdef WITH_XTAL
genfreq:
	; generate signal with frequency CPUclock / ( 8 + R11 + R10/256 )
	; generate r19 periods
	; to do this, we have a loop where we jump back to r27:31 in "normal" case, or (r27:31)-1 in case of "carry" of the fractional bits
	mov	r19,r0
        ldi	r27, lo8(gs(genfreq_jmpbase))
        sub	r27,r11
	ldi	r26,0      ; accumulated backlog
	push	r30
	push	r31
	mov	r30,r27
        ldi     r31, hi8(gs(genfreq_jmpbase))
;
; NOTE: we ASSUME that lo8(gs(genfreq_jmpbase)) > 8 so r31 never needs to be updated; there is NOTHING which guarantees this!
;
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	mov	r30,r27
genfreq_jmpbase:
	AOUT	R_PORT, R18	;  = Rport_1
	AOUT	R_PORT, R14	;  = Rport_0
	dec	r19
	breq	genfreq_end1
	add	r26, r10
	sbc	r30, r1
        ijmp

genfreq_end1:

 	AOUT	R_DDR, R16	;  R_DDR = Rddr_1	; switch to "active" state (relatively high impedance in case of crystal measurement) immediately after the last impulse, to preserve a positive DC component
	pop	r31
	pop	r30
	sbrs 	r24,smplADC_sck		; should we short-circuit the DUT (crystal)?
	rjmp genfreq_end
	in 	r26, _SFR_IO_ADDR(ADC_DDR)
	ori 	r26,(1<<TP1)|(1<<TP2)|(1<<TP3)
	AOUT 	ADC_DDR,r26
	rjmp genfreq_end
#endif


backtosleep2:
	jmp backtosleep

stepresponse:
#ifdef WITH_XTAL
	sbrc 	r24,smplADC_freq
	rjmp genfreq
#endif
stepresponse2:
 	; wait a bit less than 256 ticks; precise value determined experimentally
#ifdef WITH_XTAL
 #define Delay_step (211-Lessdelay)
#else
 #define Delay_step (214-Lessdelay)
#endif
 	ldi	r26,(Delay_step/3)
#if Delay_step%3>0
	nop
#endif
#if Delay_step%3>1
	nop
#endif
L10:	dec r26
	brne L10

	AOUT	R_PORT, R18	;  = Rport_1
	AOUT	R_PORT, R14	;  = Rport_0
	nop
				; generate start of step signal
				; this is (should be) aligned with the first sample
 	AOUT	R_DDR, R16	;  R_DDR = Rddr_1




genfreq_end:



waitevent:			; waiting loop: we wait until either counter1 is almost going to be reset, or the AD converter has a result
	lds	r26, TCNT1L	; need to read TCNT1L to latch TCNT1H
	lds	r26, TCNT1H
	lds	r27, OCR1AH
	inc	r26
	cp	r26,r27		; check if TCNT1H is getting near OCR1AH; if so, go to sleep to be sure not to miss the interrupt
	brcc backtosleep2

#ifdef WITH_XTAL
	sbrs r24,smplADC_sck	; are we doing a measurement involving short-circuiting the DUT (crystal)?
	rjmp L26
	cp	r26, r8		; then check whether it's getting time to end the short-circuit
	brne L26		; r8 determines how many times 256 clockcycles the short-circuit should last
L27:				; we enter this point up to 256 clockcycles early
	lds	r26, TCNT1L	; (need to read TCNT1L to latch TCNT1H)
	lds	r26, TCNT1H
	cp	r26, r8		; busyloop until r26 exceeds r8; due to this busyloop there's a few cycles of uncertainty, but that doesn't matter for this application
	breq L27
	AOUT 	ADC_DDR,r9	; set ADC_DDR back to its original non-shortcircuiting value
L26:
#endif

 	lds	r26, ADCSRA	; otherwise:
 	sbrs	r26, ADIF	; check if conversion done (interrupt flag is raised)
 	rjmp	waitevent	; if not, go back to checking counter

 				; the ADC gives a result
 	ldi	r27, (1<<ADEN) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27	; reset ADC interrupt flag

#ifdef WITH_XTAL
	sbrs r24,smplADC_mux	; do we need to toggle MUX?
	rjmp L23
	ldi 	r23,3
	cpi	r21,2		; if span so large that we have to discard one or more ADC readings per cycle:
	brcs L23
	brne L24		;   if precisely one ADC reading per cycle to be discarded, r23=1, otherwise =3
	ldi r23,1		;   now r23 is the value of r21 (counter for ADC discarding) at which we have to set the ADMUX to its proper setting
L24:
	mov r19, r7
	cp r22,r23
	breq L22
	andi r19,0xf8		;   set ADMUX to input 0 if we're going to discard the value
L22:
	sts ADMUX,r19
L23:
#endif
 				; now need to check whether we should read or discard it
	dec	r22		; r22 counts AD conversions within one pulse cycle
brnewaitevent:
	brne	waitevent	; if r22 not yet zero, it's not yet our turn


	lds	r22, ADCL	; read ADC
	lds	r23, ADCH

#ifdef WITH_XTAL
	sbrs	r5,7		; bit 7 of r5 is used as marker that we still need to initialize the DC level
	rjmp L25
	movw	r4,r22
	lsl	r4
	rol	r5
	lsl	r4
	rol	r5
;lsl	r4
;rol	r5
L25:
	mov	r17,r4		; r4:5 contains "DC-level"<<2
	mov	r19,r5
	lsr	r19		; shift >>2
	ror	r17
	lsr	r19
	ror	r17
;lsr	r19
;ror	r17
	sbrc 	r24,smplADC_hpf
	sub 	r22,r17		; subtract DC from sample
	sbrc 	r24,smplADC_hpf
	sbc 	r23,r19
	add	r4,r22		; update DC<<2, namely  dc := (sample-dc)/8 ;
	adc	r5,r23
#endif

	ld	r19,z		; and store, accumulating if that bit in r24 is set
	sbrc 	r24,smplADC_cumul
	add	r22,r19
	st	z+,r22
	ld	r19,z
	sbrc 	r24,smplADC_cumul
	adc	r23,r19
	sbrs    r24,smplADC_8bit
	st	z+,r23

	mov	r22,r21		; reinitialize r22

	dec r20			; decrement counter of remaining samples
;	brne waitevent
	brne brnewaitevent

end:
 	sts	TIMSK1, r1	; disable counter1 interrupts
 	sts	TCCR1B, r1	; stop counter1
 	ldi	r27, AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27	; disable ADC


#ifdef WITH_XTAL
	sts ADMUX,r7
	pop r7
	sbrs r24,smplADC_sck	; if possibly short-circuiting the DUT, restore ADC_DDR
	rjmp L30c
	AOUT ADC_DDR,r9
	pop r9
L30c:
	pop r17
	pop r5
	pop r4
#endif
	ret




#endif  /* SamplingADC_CNT */


