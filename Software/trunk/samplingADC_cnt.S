#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "config.h"
#include "part_defs.h"

 .GLOBAL samplingADC
 .func	samplingADC

//#define COUNTER_DP		/* different signal length of Double pulse is respected with counter */

#warning samplingADC with counter under test

; currently use allways the 128 divider for ADC clock!
#undef AUTO_CLOCK_DIV
#define AUTO_CLOCK_DIV ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))	/* divide clock by 128 */

#if (AUTO_CLOCK_DIV == 7)
 #define TICS_PER_ADC_CLOCK 128
#elif (AUTO_CLOCK_DIV == 6)
 #define TICS_PER_ADC_CLOCK 64
#else
#endif
#define SAMPLES_IN_ADC_PERIOD  (TICS_PER_ADC_CLOCK*13)

#define ADC_StartDelay 32	/* we cannot trigger the ADC before the Counter1 is started */
#define ADC_SHtime  (TICS_PER_ADC_CLOCK*2)	/* time to ADC S&H for triggered start */
#if PROCESSOR_TYP == 1280
  #define RESTART_DELAY 30
#else
  #define RESTART_DELAY 28
#endif

 .extern wait20us
 .extern wait10us
 .extern wait4us
 .extern wait2us
;===============================================================================================
;	Version with using of counter1 for start of first ADC-cycle in a Signal sequence
;===============================================================================================

 .section .text
samplingADC:
;uint16_t samplingADC(R24:25 what, R22:23 array[], R20 nn, R18 Rport_1, R16 Rddr_1, R14 Rport_0, R12 Rddr_0) {}
	ldi	r26, 0		; no RAM space
	ldi	r27, 0
 	ldi	r30, lo8(gs(Retur1))	;0x6B	; 107
 	ldi	r31, hi8(gs(Retur1))	;0x32	; 50
 	jmp	__prologue_saves__
Retur1:
;	clr	r1
        sbrs	r24, smplADC_twopulses	; if ((what1 & (1<<smplADC_twopulses)) != 0) 
	ldi	r25, 6		; dp_width = 6;  // set dp_width for single pulse
	movw	R4, R22		; R4:5 = &array[0];
 	mov	R13, R18	; Rport_1 -> R13
	ldi	R21, 1		; nn=256
	cpse	R20,r1
	ldi	R21, 0		; nn <256

 	ldi	r18, 1				; span = 1;	// every CPU clock tic
 	ldi	r26, lo8(SAMPLES_IN_ADC_PERIOD)	; samples_per_adc_period = SAMPLES_IN_ADC_PERIOD;
	ldi	r27, hi8(SAMPLES_IN_ADC_PERIOD)
 	sbrs	r24, smplADC_slow4		; if ((what1 & (1<<smplADC_slow4)) != 0) 
	rjmp	check16;
 	ldi	r18, 4				; span = 4;	// every 4th CPU clock tic
 	ldi	r26, lo8(SAMPLES_IN_ADC_PERIOD/4)	; samples_per_adc_period = SAMPLES_IN_ADC_PERIOD/4; // without remainder
	ldi	r27, hi8(SAMPLES_IN_ADC_PERIOD/4)
check16:
 	sbrs	r24, smplADC_slow16		; if ((what1 & (1<<smplADC_slow16)) == 0) 
	rjmp	no_slow;
 	ldi	r18, 16				; span = 16;	// every 16th CPU clock tic
 	ldi	r26, lo8(SAMPLES_IN_ADC_PERIOD/16)	; samples_per_adc_period = SAMPLES_IN_ADC_PERIOD/16; // without remainder
	ldi	r27, hi8(SAMPLES_IN_ADC_PERIOD/16)
no_slow:
	; r18 = span, r22:23 = samples in ADC period
	movw	r2, r26			; save samples_per_adc_period

 	ldi	r26, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0); 
 	sts	ADCSRB, r26		; // trigger source COUNTER1 compare match B
 	ldi	r26, (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r26		; start first ADC with ext trigger, but start immediately

wait_adc:
 	lds	r26, ADCSRA		;  while (ADCSRA & (1 << ADSC))
 	sbrc	r26, ADSC
 	rjmp	wait_adc 		; /* wait until first initial conversion is done */

 	ldi	r26, (1<<ADEN) | (0<<ADSC) | (1<<ADATE) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r26		; external trigger with interrupt enable


 	mov	r10, r1			; start1Xspan = 0;
 	mov	r11, r1
 	movw	r6, r10			; r6:7 Samples = 0;	// no ADC sample get yet
	movw	r22, r10 		; r22:23 start1 = 0;

;    // The pulse generation is time critical
;    // we are just behind the previous cycle of the ADC
;    // time to next S&H is below 1.5 ADC clocks.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
;    // We switch back to the initial trigger source to stop the counter after completing this cycle.

//==============================================================================================
GeneratePulse:
 	; r2:3 = samples in a full ADC period (13 ADC-clocks)
	; r6:7 = Samples,		the count of collected data
	; r8:9 = m_shift,		used to find the right position of data for multiple ADC
	; r10:11 = start1*span,		the time shift of this signal period
	; R12 = Rddr_0
	; R13 = Rport_1
	; R14 = Rport_0
	; r15 = scratch
	; R16 = Rddr_1
	; r17 = scratch
	; r18 = span			each time shift step has span CPU-tics
	; R20:21 = nn			the number of requested data elements
	; r22:23 = start1			the count of time shift values, span CPU tics each
	; r24  = what, r25 = dp_width
// 	sts	TCCR1B, r1		;  TCCR1B = 0;	// stop counter1
 	sts	TCCR1A, r1  		; TCCR1A = 0; // set counter1 to normal mode
  
 	sts	TCNT1H, r1		;  TCNT1 = 0;	// set initial counter to zero
 	sts	TCNT1L, r1

;  // set the ADC Start time, documentation mentions a 3 CPU clock delay, which is compensated here
 	movw	r26, r10		; start1Xspan
#ifndef COUNTER_DP
 	adiw	r26, (ADC_StartDelay - 3)	; 29
#else
	add	r26, r25		; + dp_width
	adc	r27, r1
	adiw	r26, (ADC_StartDelay - 3 - 4)
#endif
 	sts	OCR1BH, r27		;  OCR1B = (ADC_StartDelay - 3 + start1Xspan);
 	sts	OCR1BL, r26		;  set compare B to start condition for this Pulse generation

 	subi	r26, lo8(-(ADC_SHtime + 16 + 3)) ; 0xD0	
 	sbci	r27, hi8(-(ADC_SHtime + 16 + 3)) ; 0xFE
 	sts	OCR1AH, r27		; OCR1A = (ADC_StartDelay + ADC_SHtime + 16 + start1Xspan );
 	sts	OCR1AL, r26		; update compare A interrupt to behind S&H

 	ldi	r26, (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);
 	out	_SFR_IO_ADDR(TIFR1), r26	; clear interrupt flags

 	cp	r6, R20			; if (Samples >= nn) 
	cpc	r7, R21
 	brcs	get_next_data 

     // all samples collected, finish
finish:
;// 	sts	TCCR1B, r1			; TCCR1B = 0;	// stop counter1
 	sts	TIMSK1, r1			; // disable counter1 compare A Interrupt
 	ldi	r26, (1<<ADIF) | (1<<ADIE)	; // stop ADC
 	sts	ADCSRA, r26

; 	adiw	r28, 0
 	ldi	r30, 18
 	jmp	__epilogue_restores__
;============== return ======================

get_next_data:
 	wdr					; wdt_reset();
 	ldi	r26, (1<<OCIE1A)		; TIMSK1 = (1<<OCIE1A);
 	sts	TIMSK1, r26			; // enable counter1 compare A Interrupt
 	cli					; cli();
 	ldi	r26, (0<<ICNC1)|(1<<CS10)	; TCCR1B = (0<<ICNC1)|(1<<CS10);
 	sts	TCCR1B, r26			; // start counter at full speed
;============ Counter 1 is started =================================================
;  // We must count the CPU cycles used by the program to generate the signal just before S&H!
	; The counter starts ADC in ADC_StartDelay tics.
        ; The signal generation takes 29 tics.
	; So we must only delay the two ADC clock cycles  (ADC_SHtime).

#ifndef COUNTER_DP
#define SignalStartDelay (ADC_SHtime+ADC_StartDelay-30+1)
;			   256	+      32          -30+1 = 259
#else
#define SignalStartDelay (ADC_SHtime+ADC_StartDelay-15+1)
;			   256	+      32          -15+1 = 274
#endif
	ldi	r26, (SignalStartDelay / 4)
lop1:
	dec	r26
	nop
	brne	lop1
#if (SignalStartDelay % 4) > 2
	nop
#endif
#if (SignalStartDelay % 4) > 1
	nop
#endif
#if (SignalStartDelay % 4) > 0
	nop
#endif

#ifndef COUNTER_DP
	mov	r27, r25	; 1 dp_width
	lsr	r27		; 2
	dec	r27		; 3 count2 = (dp_width/2) - 1;
	ldi	r26, 7		; 4 count1 = 7 - count2;
	sub	r26, r27	; 5
 	AOUT	R_PORT, R14	; 6 R_PORT = Rport_0;	// does nothing for step response,

					; 1+(2*r26) for 1<=r26<=6
	dec	r26		; 7 count1--;
	breq	L98		; 9 , dp_width = 14
	dec	r26		; count1--;
	breq	L98		; 11  dp_width = 12
	dec	r26		; count1--;
	breq	L98		; 13  dp_width = 10
	dec	r26		; count1--;
	breq	L98		; 15  dp_width = 8
	dec	r26		; count1--;
	breq	L98		; 17  dp_width = 6
	dec	r26		; count1--;
	breq	L98		; 19  dp_width = 4
L98:
#else
 	AOUT	R_PORT, R14	; 1 R_PORT = Rport_0;	// does nothing for step response,
	mov	r27, r25	; 2 dp_width
	lsr	r27		; 3
	dec	r27		; 4 count2 = (dp_width/2) - 1;
#endif
        sbrc	r24, smplADC_twopulses	; 11
 	AOUT	R_DDR, R12	; if ((what1 & (1<<smplADC_twopulses)) != 0) R_DDR = Rddr_0;
	dec	r27		; 12 count2--;
        sbrc	r24, smplADC_twopulses	; 14
 	AOUT	R_DDR, R16	; if ((what1 & (1<<smplADC_twopulses)) != 0) R_DDR = Rddr_1;

	breq	L99		; 16 , dp_width = 4
	dec	r27		;  count2--;
	breq	L99		; 18 , dp_width = 6
	dec	r27		;  count2--;
	breq	L99		; 20 , dp_width = 8
	dec	r27		;  count2--;
	breq	L99		; 22 , dp_width = 10
	dec	r27		;  count2--;
	breq	L99		; 24 , dp_width = 12
	dec	r27		;  count2--;
	breq	L99		; 26 , dp_width = 14
L99:
 	AOUT	R_DDR, R12	; 27 R_DDR = Rddr_0; // for step response, this effectly does noting;
				// for impulse response, it starts the (last) impulse
 	wdr			; 28 wdt_reset();
 	AOUT	R_PORT, R13	; 29 R_PORT = Rport_1; // beginning of step, or end of (last) impulse
 	AOUT	R_DDR, R16	; 30 R_DDR = Rddr_1; // start of first measurement is aligned with this
;============ End of time critical part =================================================

;  // The signal is generated, we go to sleep now.
;  // the interrupt was disabled until now, so the Compare A Interrupt will wake us up anyway,
;  // even if the signal was generated in the past.
 	ldi	r26, (1<<SE)|(0<<SM2)|(0<<SM1)|(0<<SM0); set Sleep Enable with SLEEP_MODE_IDLE
 	out	_SFR_IO_ADDR(SMCR), r26	; 
 	sei		; sei();	// enable interrupts just before sleep to prevent lost INT's
 	sleep		; sleep_cpu(); 	// sleep until interrupt wake us up; 
;---------------XXXXXXXX-------------------------

;	// First wake up should be caused by the counter1 compare match A interrupt.
;	// The interrupt flag should be cleared by the ISR
;	// This is just behind 2 ADC_clock_tics + ADC_StartDelay + 16 + ISR CPUcycles. 
;	// The first triggered ADC conversion takes 13.5 ADC clock cycles from Counter Reg B compare
;	// Let us look, if the next ADC S&H in within the sampling period
;== 	sts	TCCR1B, r1	; TCCR1B = 0;  // stop counter, no longer required, is stopped by ISR_
	mov	r8, r1			;  r8:9 m_shift = 0;
	mov	r9, r1			;  used for multiple ADC-conversions during one Signal period
;	movw	r8, r0			;  r8:9 m_shift = 0;
;	// The first triggered ADC conversion takes 13.5 ADC clock cycles from Counter Reg B compare
;i	// Let us look, if the next ADC S&H in within the sampling period
	

//==============================================================================================
CheckNextSample:
;    // The pulse generation is time critical.
;    // We are just behind the previous cycle of the ADC for repeated conversion.
;    // The time to next S&H is below 1.5 ADC clocks in this case.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
 	movw	r26, r8			; m_shift

 	add	r26, r22		; + start1
 	adc	r27, r23

	add	r26, r2		; (m_shift + start1) + samples_per_adc_period
	adc	r27, r3
	adiw	r26, 1			; +1

 	cp	R20, r26	;R20:21 = nn, r26:27 = samples_per_adc_period+m_shifts+start1+1 
 	cpc	R21, r27

 	brcc	more_data	; if (((start1 + m_shift) + samples_per_adc_period + 1) > nn) 

;	// The running ADC-cycle is the last one in this Signal period
;	// We switch back to the initial trigger source to stop the counter after completing this cycle.
 	ldi	r26, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0);  
 	sts	ADCSRB, r26  	; trigger source = COUNTER1 compare match B, STOP after ADC-cycle ends

;	// We must differ between the first and repeated ADC cycle condition.
;	// If it is the first cycle, we are already behind the S&H time (Counter1 Compare match A).
;	// The other situation is the repeated ADC. In this case we are just behind
;	// the end of ADC cycle, so we must wait for the next S&H time.
;	// The next S&H is at 1.5*ADCclock. 
	cp	r1, r8			; r8:9 = m_shift
	cpc	r1, r9
 	brcc	behind_SH 		; if (m_shift > 0) 
       // This is not the first ADC-cycle in this Signal-generation cycle.
       // Let us wait for next SH time.
#if ((TICS_PER_ADC_CLOCK * 3) / (MHZ_CPU *2)) == 12
	call	wait10us		; wait10us();
	call	wait2us			;  wait2us();
#elif ((TICS_PER_ADC_CLOCK * 3) / (MHZ_CPU *2)) == 24
 	call	wait20us		;  wait20us();
 	call	wait4us			;  wait4us();
#else
  #warning delay to S&H time cannot be build
#endif
;       // Additionally we can wait the time, which the ISR_CNT1_A takes longer than the empty ISR_ADC 
;       // 17 = 28 cycles - 11 cycles (INT+JMP+RTI)
#if  MHZ_CPU == 8
	call	wait2us
#endif
#if  MHZ_CPU == 16
	call	wait1us
#endif
behind_SH:
        ; -----------------------------------------------------
	; toggle output back to the idle state in case of _step
 	sbrc	r24, smplADC_step	; if (what1 & (1<<smplADC_step)) R_DDR = Rddr_0;
 	AOUT	R_DDR, R12	; 4  Rddr_0
	sbrc	r24, smplADC_step	; if (what1 & (1<<smplADC_step)) R_PORT = Rport_0;
 	AOUT	R_PORT, R14	; 5  Rport_0

	rcall	store_data		; store new ADC data to array and count Samples
;    // This was the last ADC data of this Signal period 
	ldi	r26, 1
 	add	r22, r26	; start1 ++;  set start time to next,  multiply by span to get CPUtics
	adc	r23, r1

 	add	r10, r18		; start1Xspan += span:
 	adc	r11, r1
 	rjmp	GeneratePulse 

;    // there are more data to collect in this Signal period
;    // Now we try to switch the ADC to free running mode
more_data:
 	sts	ADCSRB, r1	;ADCSRB = 0;   // source ADC finish == 13 ADC clock cyclus free run

	rcall	store_data		; store new ADC data in array and count Samples

 	add	r8, r2			; m_shift += samples_per_adc_period;
 	adc	r9, r3			; + count of samples in previous periode (from S&H to S&H)
 	rjmp	CheckNextSample 	;

; Store ADC data in caller's array
; Wait for ADC data ready with sleep command
; The position of array cell is array[start1 + m_shift]
; r8:9 = m_shift, r22:23 = start1, R4:5 = beginn of array
; Function use register r17:15 to get new ADC data and
; register r26:27 to read (and accumulate) the old data at the array place.
; every call increments the Samples counter r6:7 .
store_data:
 	movw	r30, r8			; m_shift

 	add	r30, r22		; + start1
 	adc	r31, r23

 	add	r30, r30		; (start1 + m_shift); // *2, int16
 	adc	r31, r31

 	add	r30, R4			; &array[start1 + m_shift]
 	adc	r31, R5

 	ld	r26, Z			; lo8(array[start1 + m_shift])
 	ldd	r27, Z+1		; hi8(array[start1 + m_shift])

; r30:31 = Z = number of a 16-Bit element.
 	ldi	r17, (1<<SE)|(0<<SM2)|(0<<SM1)|(0<<SM0); set Sleep Enable with SLEEP_MODE_IDLE
 	out	_SFR_IO_ADDR(SMCR), r17	; 
 	sleep				; sleep_cpu(); 	// sleep until ADC interrupt wakes us up; 
;---------------XXXXXXXX-------------------------

;    // next ADC data are ready
	lds	r17, ADCL		; neẃ ADC value
	lds	r15, ADCH

;==	mov	r17, r22		;******************************************
;==	mov	r15, r23		;********* TEST ***************************
;** 	lds	r17, ADCSRA		;  ADCSRA 
;**	mov	r17, R12
;**	mov	r15, r1
;##	lds	r17, TCNT1L		;  TCNT
;##	lds	r15, TCNT1H

	sbrc	r24, smplADC_cumul	; skip next instruction, if no acummulate
	add	r17, r26		; + lo8(array[start1 + m_shift])
 	st	Z+, r17			; store lower part
	sbrc	r24, smplADC_cumul	; skip next instruction, if no acummulate
	adc	r15, r27		; + hi8(array[start1 + m_shift])
 	st	Z, r15			; store upper part

	ldi	r17, 1
	add	r6, r17			; Samples++
	adc	r7, r1			; add carry to r3
	ret				; return store_data

 .endfunc
