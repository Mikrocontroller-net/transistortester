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
 	ldi	r26, 0		; R2-R17
 	ldi	r27, 0
 	ldi	r30, lo8(gs(Retur1))	;0x6B	; 107
 	ldi	r31, hi8(gs(Retur1))	;0x32	; 50
 	jmp	__prologue_saves__
Retur1:
	clr	r1
 	movw	r6, r24		; R6:7 = what, R6 = what1
	movw	r4, r22		; R4:5 = &array[0];
 	mov	r13, r18	; Rport_1
 	eor	r25, r25

#define SAMPLES_IN_ADC_PERIOD  (TICS_PER_ADC_CLOCK*13)
 	sbrc	r6, samplingADC_slow4		; if ((what1 & (1<<samplingADC_slow4)) != 0) 
 	rjmp	slow4_is_set

 	sbrs	r6, samplingADC_slow16		; if ((what1 & (1<<samplingADC_slow16)) == 0) 
 	rjmp	no_slow 

#if (SAMPLES_IN_ADC_PERIOD/16) > 250
	ldi	r19, 254			; samples_per_adc_period = 254;
#else
 	ldi	r19, SAMPLES_IN_ADC_PERIOD/16	; samples_per_adc_period = SAMPLES_IN_ADC_PERIOD/16; // without remainder
#endif
 	ldi	r18, 16				; span = 16;	// every 16th CPU clock tic
 	rjmp	L_64f8 

slow4_is_set:
#if (SAMPLES_IN_ADC_PERIOD/4) > 250
 	ldi	r19, 254			; samples_per_adc_period = 254;
#else
 	ldi	r19, SAMPLES_IN_ADC_PERIOD/4	; samples_per_adc_period = SAMPLES_IN_ADC_PERIOD/4; // without remainder
#endif
 	ldi	r18, 4				; span = 4;	// every 4th CPU clock tic
 	rjmp	L_64f8 

no_slow:
#if SAMPLES_IN_ADC_PERIOD > 250
 	ldi	r19, 254			; samples_per_adc_period = 254;
#else
 	ldi	r19, SAMPLES_IN_ADC_PERIOD	; samples_per_adc_period = SAMPLES_IN_ADC_PERIOD;
#endif
 	ldi	r18, 1				; span = 1;	// every CPU clock tic

L_64f8:
	; r18 = span, r19 = samples in ADC period
        mov	r21, r7		; dp_width = (what >> samplingADC_interpulse_width);
        sbrs	r6, samplingADC_twopulses	; if ((what1 & (1<<samplingADC_twopulses)) != 0) 
	ldi	r21, 6		; dp_width = 6;  // set dp_width for single pulse

 	ldi	r25, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0); 
 	sts	ADCSRB, r25		; // trigger source COUNTER1 compare match B
 	ldi	r25, (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r25		; start ADC with ext trigger, but start immediately

wait_adc:
 	lds	r25, ADCSRA		;  while (ADCSRA & (1 << ADSC))
 	sbrc	r25, ADSC
 	rjmp	wait_adc 		; /* wait until first initial conversion is done */

 	ldi	r25, (1<<ADEN) | (0<<ADSC) | (1<<ADATE) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r25		; external trigger with interrupt enable

 	sts	TCCR1A, r1  		; TCCR1A = 0; // set counter1 to normal mode
 	in	r25, _SFR_IO_ADDR(SMCR)	; set_sleep_mode(SLEEP_MODE_IDLE);
 	andi	r25, 0xF1	; 
 	out	_SFR_IO_ADDR(SMCR), r25	;

 	mov	r15, r1			; Samples = 0;	// no ADC sample get yet
 	mov	r10, r1			; start1Xspan = 0;
 	mov	r11, r1
 	ldi	r17, 0x00		; start1 = 0;

;    // The pulse generation is time critical
;    // we are just behind the previous cycle of the ADC
;    // time to next S&H is below 1.5 ADC clocks.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
;    // We switch back to the initial trigger source to stop the counter after completing this cycle.

//==============================================================================================
GeneratePulse:
	; r6  = what, r7 = dp_width_orig
	; r8:9 = m_shift,		used to find the right position of data for multiple ADC
	; r10:11 = start1*span,		the time shift of this signal period
	; r12 = Rddr_0
	; r13 = Rport_1
	; r14 = Rport_0
	; r15 = Samples			the count of collected data
	; r16 = Rddr_1
	; r17 = start1			the count of time shift values, span CPU tics each
	; r18 = span			each time shift step has span CPU-tics
 	; r19 = samples in a full ADC period (13 ADC-clocks)
	; r20 = nn			the number of requested data elements
	; r21 = dp_width		width of the double pulse
 	sts	TCCR1B, r1		;  TCCR1B = 0;	// stop counter1
  
 	sts	TCNT1H, r1		;  TCNT1 = 0;	// set initial counter to zero
 	sts	TCNT1L, r1

;  // set the ADC Start time, documentation mentions a 3 CPU clock delay, which is compensated here
 	movw	r24, r10		; start1Xspan
 	adiw	r24, (ADC_StartDelay - 3)	; 29
 	sts	OCR1BH, r25		;  OCR1B = (ADC_StartDelay - 3 + start1Xspan);
 	sts	OCR1BL, r24		;  set compare B to start condition for this Pulse generation

 	movw	r24, r10		; start1Xspan
 	subi	r24, lo8(-(ADC_StartDelay + ADC_SHtime + 16)) ; 0xD0	
 	sbci	r25, hi8(-(ADC_StartDelay + ADC_SHtime + 16)) ; 0xFE
 	sts	OCR1AH, r25		; OCR1A = (ADC_StartDelay + ADC_SHtime + 16 + start1Xspan );
 	sts	OCR1AL, r24		; update compare A interrupt to behind S&H

 	ldi	r25, (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);
 	out	_SFR_IO_ADDR(TIFR1), r25	; clear interrupt flags
 	ldi	r25, (1<<OCIE1A)		; TIMSK1 = (1<<OCIE1A);
 	sts	TIMSK1, r25			; // enable counter1 compare A Interrupt

 	cp	r15, r20			; if (Samples >= nn) 
 	brcs	get_next_data 

     // all samples collected, finish
finish:
 	sts	TCCR1B, r1			; TCCR1B = 0;	// stop counter1
 	ldi	r24, (1<<ADIF) | (1<<ADIE)	; // stop ADC
 	sts	ADCSRA, r24

 	movw	r24, r6				;  return(what);
; 	adiw	r28, 0
 	ldi	r30, 18
 	jmp	__epilogue_restores__
;============== exit ======================

get_next_data:
 	wdr					; wdt_reset();
 	cli					; cli();
 	ldi	r26, (0<<ICNC1)|(1<<CS10)	; TCCR1B = (0<<ICNC1)|(1<<CS10);
 	sts	TCCR1B, r26			; // start counter at full speed
;============ Counter 1 is started =================================================
;  // We must count the CPU cycles used by the program to generate the signal just before S&H!
	; The counter starts ADC in ADC_StartDelay tics.
        ; The signal generation takes 29 tics.
	; So we must only delay the two ADC clock cycles  (ADC_SHtime).

#define SignalStartDelay (ADC_SHtime)
	ldi	r23, (SignalStartDelay / 4)
lop1:
	dec	r23
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

	mov	r23, r21	; 1 dp_width
	lsr	r23		; 2
	dec	r23		; 3 count2 = (dp_width/2) - 1;
	ldi	r24, 7		; 4 count1 = 7 - count2;
	sub	r24, r23	; 5
 	AOUT	R_PORT, r14	; 6 R_PORT = Rport_0;	// does nothing for step response,

					; 1+(2*r24) for 1<=r24<=6
	dec	r24		; 7 count1--;
	breq	L98
	dec	r24		; count1--;
	breq	L98
	dec	r24		; count1--;
	breq	L98
	dec	r24		; count1--;
	breq	L98
	dec	r24		; count1--;
	breq	L98
	dec	r24		; count1--;
	breq	L98

L98:
        sbrc	r6, samplingADC_twopulses	; 9
 	AOUT	R_DDR, r12	; if ((what1 & (1<<samplingADC_twopulses)) != 0) R_DDR = Rddr_0;
	dec	r23		; 11 count2--;
        sbrc	r6, samplingADC_twopulses
 	AOUT	R_DDR, r16	; 13 if ((what1 & (1<<samplingADC_twopulses)) != 0) R_DDR = Rddr_1;

	breq	L99		; 15
	dec	r23		;  count2--;
	breq	L99		; 17
	dec	r23		;  count2--;
	breq	L99		; 19
	dec	r23		;  count2--;
	breq	L99		; 21
	dec	r23		;  count2--;
	breq	L99		; 23
	dec	r23		;  count2--;
	breq	L99		; 24 +1
L99:
 	AOUT	R_DDR, r12	; 26 R_DDR = Rddr_0; // for step response, this effectly does noting;
				// for impulse response, it starts the (last) impulse
 	wdr			; 27 wdt_reset();
 	AOUT	R_PORT, r13	; 28 R_PORT = Rport_1; // beginning of step, or end of (last) impulse
 	AOUT	R_DDR, r16	; 29 R_DDR = Rddr_1; // start of first measurement is aligned with this

;  // The signal is generated, we go to sleep now.
;  // the interrupt was disabled until now, so the Compare A Interrupt will wake us up anyway,
;  // even if the signal was generated in the past.
 	in	r24, _SFR_IO_ADDR(SMCR)	; sleep_enable();
 	ori	r24, (1<<SE)	; 1
 	out	_SFR_IO_ADDR(SMCR), r24	; 51
 	sei			; sei();	// enable interrupts just before sleep to prevent lost INT's
 	sleep			; sleep_cpu(); 	// sleep until interrupt wake us up; 
;---------------XXXXXXXX-------------------------

;// First wake up should be caused by the counter1 compare match A interrupt.
;// The interrupt flag should be cleared by the ISR
;// This is just behind 2 ADC_clock_tics + ADC_StartDelay + 16 + ISR CPUcycles. 
;// The first triggered ADC conversion takes 13.5 ADC clock cycles from Counter Reg B compare
;// Let us look, if the next ADC S&H in within the sampling period
// 	sts	TCCR1B, r1	; TCCR1B = 0;	// stop counter, no longer required
	mov	r8, r1			;  m_shift = 0;
 	mov	r9, r1			;  used for multiple ADC-conversions during one Signal period
;// The first triggered ADC conversion takes 13.5 ADC clock cycles from Counter Reg B compare
;// Let us look, if the next ADC S&H in within the sampling period
	

//==============================================================================================
CheckNextSample:
;    // The pulse generation is time critical.
;    // We are just behind the previous cycle of the ADC for repeated conversion.
;    // The time to next S&H is below 1.5 ADC clocks in this case.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
 	movw	r26, r8			; m_shift

 	add	r26, r17		; + start1
 	adc	r27, r1

	add	r26, r19		; (m_shift + start1) + samples_per_adc_period
	adc	r27, r1
	adiw	r26, 1			; +1


 	cp	r20, r26		;r20 = nn
 	cpc	r1, r27

 	brcc	more_data		; if (((start1 + m_shift) + samples_per_adc_period + 1) > nn) 

;    The running ADC-cycle is the last one in this Signal period
;    We switch back to the initial trigger source to stop the counter after completing this cycle.
 	ldi	r21, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0);  
 	sts	ADCSRB, r21  		; trigger source = COUNTER1 compare match B, STOP after ADC-cycle ends

;    // We must differ between the first and repeated ADC cycle condition.
;    // If it is the first cycle, we are already behind the S&H time (Counter1 Compare match A).
;    // The other situation is the repeated ADC. In this case we are just behind
;    // the end of ADC cycle, so we must wait for the next S&H time.
;    // The next S&H is at 1.5*ADCclock. 
	cp	r1, r8
	cpc	r1, r9
 	brcc	behind_SH 		; if (m_shift > 0) 
       // This is not the first ADC-cycle in this Signal-generation cycle.
       // Let us wait for next SH time.
#if ((TICS_PER_ADC_CLOCK * 3) / (MHZ_CPU *2)) == 12
	call	wait20us		; wait10us();
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
 	sbrc	r6, samplingADC_step	; if (what1 & (1<<samplingADC_step)) R_DDR = Rddr_0;
 	AOUT	R_DDR, r12	; 4
	sbrc	r6, samplingADC_step	; if (what1 & (1<<samplingADC_step)) R_PORT = Rport_0;
 	AOUT	R_PORT, r14	; 5

	rcall	store_data		; store new ADC data to array and count Samples
;    // This was the last ADC data of this Signal period 
 	subi	r17, 0xFF		; start1 ++; // set start time to next, must be multiplied by span to get CPUtics
 	add	r10, r18		; start1Xspan += span:
 	adc	r11, r1
 	rjmp	GeneratePulse 

;    // there are more data to collect in this Signal period
;    // Now we try to switch the ADC to free running mode
more_data:
 	sts	ADCSRB, r1		;ADCSRB = 0;   // source ADC finish == 13 ADC clock cyclus
 	add	r8, r19			; m_shift += samples_per_adc_period;
 	adc	r9, r1			; + count of samples in previous periode (from S&H to S&H)

	rcall	store_data		; store new ADC data in array and count Samples
 	rjmp	CheckNextSample 	;

; store_data wait for ADC data ready with sleep command
; Function use register r22:23 to get new ADC data and
; register r26:27 to read the old data at the array place.
; every call increments the Samples counter r15
store_data:

 	movw	r30, r8			; m_shift

 	add	r30, r17		; + start1
 	adc	r31, r1

 	add	r30, r30		; (start1 + m_shift); // *2, int16
 	adc	r31, r31

 	add	r30, r4			; &array[start1 + m_shift]
 	adc	r31, r5

; r30:31 = Z = number of a 16-Bit element.
 	in	r26, _SFR_IO_ADDR(SMCR)	; sleep_enable();
 	ori	r26, (1<<SE)		; 
 	out	_SFR_IO_ADDR(SMCR), r26	; 
 	sleep				; sleep_cpu(); 	// sleep until ADC interrupt wakes us up; 
;---------------XXXXXXXX-------------------------

;    // next ADC data are ready
	lds	r22, ADCL		; neẃ ADC value
	lds	r23, ADCH
 	ld	r26, Z			; lo8(array[start1 + m_shift])
 	ldd	r27, Z+1		; hi8(array[start1 + m_shift])
	sbrc	r6, samplingADC_cumul
	add	r22, r26		; + lo8(array[start1 + m_shift])
 	st	Z+, r22
	sbrc	r6, samplingADC_cumul
	adc	r23, r27		; + hi8(array[start1 + m_shift])
 	st	Z, r23			;
 	inc	r15			; Samples++
	ret

 .endfunc
