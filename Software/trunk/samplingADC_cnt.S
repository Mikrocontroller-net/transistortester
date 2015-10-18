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
#define AUTO_CLOCK_DIV16 ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))	/* divide clock by 128 */
//#if CPU_MHZ == 8
#if 0
#define AUTO_CLOCK_DIV8 ((1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0))	/* divide clock by 64 */
#else
#define AUTO_CLOCK_DIV8 ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))	/* divide clock by 128 */
#endif

#define TICS_PER_ADC_CLOCK 128
#define SAMPLES_IN_ADC_PERIOD  (TICS_PER_ADC_CLOCK*13)

#define ADC_StartDelay 32	/* we cannot trigger the ADC before the Counter1 is started */
#define ADC_SHtime  (TICS_PER_ADC_CLOCK*2)	/* time to ADC S&H for triggered start */
#if PROCESSOR_TYP == 1280
  #define RESTART_DELAY 30
#else
  #define RESTART_DELAY 28
#endif

;===============================================================================================
;	This version uses counter1 for start of first ADC-cycle in a Signal sequence
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
	dec	R25
	andi	R25, 0x0f	; 0 - 15
;	cpi	R25, 1		; is dp_width > 2
;	brge	is_well
;	ldi	R25, 1		; limit dp_width to 2
is_well:
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
	ldi	r22, AUTO_CLOCK_DIV16
#if (AUTO_CLOCK_DIV8 != AUTO_CLOCK_DIV16)
	tst	r16		; is it L measurement?
	brne	no_L
	lsr	r26		; half of samples_per_adc_period
	ror	r27
	ldi	r22, AUTO_CLOCK_DIV8
no_L:
#endif
	movw	r2, r26			; save samples_per_adc_period in r2:3

 	AOUT	R_PORT, R14	;  Rport_0	set to start condition
 	AOUT	R_DDR, R12	;  Rddr_0

 	ldi	r26, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0); 
 	sts	ADCSRB, r26		; // trigger source COUNTER1 compare match B
 	ldi	r26, (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) ;
	or	r26, r22		; use actual clock div
 	sts	ADCSRA, r26		; start first ADC with ext trigger, but start immediately
wait_adc:
 	lds	r26, ADCSRA		;  while (ADCSRA & (1 << ADSC))
 	sbrc	r26, ADSC
 	rjmp	wait_adc 		; /* wait until first initial conversion is done */

; 	ldi	r26, (1<<ADEN) | (0<<ADSC) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) ;
;	or	r26, r22		; use actual clock div
 	sts	ADCSRA, r26		; clear flags (1<<ADIF)

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
	; r4:5 = Address of buffer
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
	; R24  = what, R25 = dp_width
 	sts	TCCR1B, r1		;  TCCR1B = 0;	// stop counter1

 	sts	TCCR1A, r1  		; TCCR1A = 0; // set counter1 to normal mode
  
 	sts	TCNT1H, r1		;  TCNT1 = 0;	// set initial counter to zero
 	sts	TCNT1L, r1

;  // set the ADC Start time, documentation mentions a 3 CPU clock delay, which is compensated here
 	movw	r26, r10		; start1Xspan
	add	r26, R25		; + dp_width
	adc	r27, r1
	adiw	r26, (ADC_StartDelay - 3)
 	sts	OCR1BH, r27		;  OCR1B = (ADC_StartDelay - 3 + start1Xspan);
 	sts	OCR1BL, r26		;  set compare B to start condition for this Pulse generation

#if (AUTO_CLOCK_DIV8 != AUTO_CLOCK_DIV16)
	tst	r16			; test of L?
        brne	no_L2
 	subi	r26, lo8(-((ADC_SHtime/2) + 16 + 3)) ; + time to S&H for double speed	
 	sbci	r27, hi8(-((ADC_SHtime/2) + 16 + 3)) ; 
	rjmp	set_ocr1a
no_L2:
#endif
 	subi	r26, lo8(-(ADC_SHtime + 16 + 3)) ; + time to S&H	
 	sbci	r27, hi8(-(ADC_SHtime + 16 + 3)) ; 
set_ocr1a:
 	sts	OCR1AH, r27		; OCR1A = (ADC_StartDelay + ADC_SHtime + 16 + start1Xspan );
 	sts	OCR1AL, r26		; update compare A interrupt to behind S&H
 	sts	TIMSK1, r1			; // disable counter1 compare A Interrupt

 	ldi	r26, (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);
 	out	_SFR_IO_ADDR(TIFR1), r26	; clear interrupt flags

 	cp	r6, R20			; if (Samples >= nn) 
	cpc	r7, R21
 	brcs	get_next_data 

     // all samples collected, finish
finish:
	clr	r1
 	sts	TCCR1B, r1			; TCCR1B = 0;	// stop counter1
;//	sts	TIMSK1, r1			; // disable counter1 compare A Interrupt
 	ldi	r26, (1<<ADIF) | (1<<ADIE)	; // stop ADC
 	sts	ADCSRA, r26
;##	in	r28, _SFR_IO_ADDR(SPL)
;##	in	r29, _SFR_IO_ADDR(SPH)
 	ldi	r30, 18			; restore full register list
 	jmp	__epilogue_restores__
;============== return ======================

get_next_data:
; 	ldi	r26, (1<<OCIE1A)		; TIMSK1 = (1<<OCIE1A);
; 	sts	TIMSK1, r26			; // enable counter1 compare A Interrupt
 	ldi	r26, (0<<ICNC1)|(1<<CS10)	; TCCR1B = (0<<ICNC1)|(1<<CS10);
 	sts	TCCR1B, r26			; // start counter at full speed
;============ Counter 1 is started =================================================
;  // We must count the CPU cycles used by the program to generate the signal just before S&H!
	; The counter starts ADC in ADC_StartDelay tics.
        ; The signal generation takes 29 tics.
	; So we must only delay the two ADC clock cycles  (ADC_SHtime).

#define SignalStartDelay (ADC_SHtime+ADC_StartDelay-(30-16)+1)
;			   256	+      32          -(30-16)+1 = 271
	ldi	r26, (SignalStartDelay / 3)
lop1:
	dec	r26
	brne	lop1
#if (SignalStartDelay % 3) > 1
	nop
#endif
#if (SignalStartDelay % 3) > 0
	nop
#endif

	ldi	r30, lo8(gs(Return2)) ;30
	ldi	r31, hi8(gs(Return2)) ;28
	ldi	r27, 15		;27
	sub	r27, R25	;26 (16-dp_width)
	add	r30, r27	;25  + (16-dp_width)
	adc	r31, r1		;24
	mov	r27, R14	;23  Rport_0
	or	r27, R13	;22 Rport_1
 	wdr			;21 wdt_reset();
	ijmp			;20 computed goto Return+(16-dp_width)
Return2:
	AOUT	R_PORT, R13	;18 R_PORT = Rport_1, dp_width = 15
	AOUT	R_PORT, R13	;17 R_PORT = Rport_1, dp_width = 14
	AOUT	R_PORT, R13	;16 R_PORT = Rport_1, dp_width = 13
	AOUT	R_PORT, R13	;15 R_PORT = Rport_1, dp_width = 12
	AOUT	R_PORT, R13	;14 R_PORT = Rport_1, dp_width = 11
	AOUT	R_PORT, R13	;13 R_PORT = Rport_1, dp_width = 10
	AOUT	R_PORT, R13	;12 R_PORT = Rport_1, dp_width = 9
	AOUT	R_PORT, R13	;11 R_PORT = Rport_1, dp_width = 8
	AOUT	R_PORT, R13	;10 R_PORT = Rport_1, dp_width = 7
	AOUT	R_PORT, R13	; 9 R_PORT = Rport_1, dp_width = 6
	AOUT	R_PORT, R13	; 8 R_PORT = Rport_1, dp_width = 5
	AOUT	R_PORT, R13	; 7 R_PORT = Rport_1, dp_width = 4
	AOUT	R_PORT, R13	; 6 R_PORT = Rport_1, dp_width = 3
	AOUT	R_PORT, R13	; 5 R_PORT = Rport_1, dp_width = 2
 	AOUT	R_PORT, R13	; 4 R_PORT = Rport_1, dp_width = 1	
 	AOUT	R_PORT, r27	; 2 R_PORT = Rport_1|Rport_0; // beginning of step, or end of (last) impulse
 	AOUT	R_DDR, R16	; 1 R_DDR = Rddr_1; // start of first measurement is aligned with this
;============ End of time critical part =================================================
 	AOUT	R_PORT, R14	; R_PORT = Rport_0; only switch of unused Rport_0

wait_cnt:
	sbis	_SFR_IO_ADDR(TIFR1), OCF1A 	;  while (TIFR1 & (1 << OCF1A) == 0)
 	rjmp	wait_cnt 		; /* wait until counter1 compare match is done */
;---------------XXXXXXXX-------------------------

;	// The first triggered ADC conversion takes 13.5 ADC clock cycles from Counter Reg B compare
 	sts	TCCR1B, r1	; TCCR1B = 0;  // stop counter, no longer required_
	mov	r8, r1			;  r8:9 m_shift = 0;
	mov	r9, r1			;  used for multiple ADC-conversions during one Signal period
 	ldi	r26, (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);
 	out	_SFR_IO_ADDR(TIFR1), r26	; clear interrupt flags
	

//==============================================================================================
CheckNextSample:
;    // The pulse generation is time critical.
;    // We are just behind the previous cycle of the ADC for repeated conversion.
;    // The time to next S&H is below 1.5 ADC clocks in this case.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
;    // Let us look, if the next ADC S&H is within the sampling period
 	movw	r26, r8			; m_shift

 	add	r26, r22		; + start1
 	adc	r27, r23

	add	r26, r2		; (m_shift + start1) + samples_per_adc_period
	adc	r27, r3
	adiw	r26, 1			; +1

 	cp	R20, r26	;R20:21 = nn, r26:27 = samples_per_adc_period+m_shifts+start1+1 
 	cpc	R21, r27

 	brcc	more_data	; if (((start1 + m_shift) + samples_per_adc_period + 1) > nn) 

;	--------------------------------------------------------------
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
        ldi	r26, ((TICS_PER_ADC_CLOCK*3)/6)		; 1.5 * ADC_CLOCK
#if (AUTO_CLOCK_DIV8 != AUTO_CLOCK_DIV16)
	tst	r16			; test of L?
        brne	lop3			; it is Cap mode
        ldi	r26, ((TICS_PER_ADC_CLOCK*3)/12)	; 1.5 * ADC_CLOCK, Fast mode
#endif
lop3:
	dec	r26
	brne	lop3
;       // Additionally we can wait the time, which the ISR_CNT1_A takes longer than the empty ISR_ADC 
;       // 17 = 28 cycles - 11 cycles (INT+JMP+RTI)
#if  MHZ_CPU == 8
	call	wait2us
#endif
#if  MHZ_CPU == 16
	call	wait1us
#endif
behind_SH:
	call	wait100us	; ############# for Test only #################################################
        ; -------------------------------------
	; toggle output back to the idle state 
 	AOUT	R_PORT, R14	; 5  Rport_0
 	AOUT	R_DDR, R12	; 4  Rddr_0

	rcall	store_data		; store new ADC data to array and count Samples
;    // This was the last ADC data of this Signal period, update the time shift registers
	ldi	r26, 1
 	add	r22, r26	; start1 ++;  set start time to next,  multiply by span to get CPUtics
	adc	r23, r1

 	add	r10, r18		; start1Xspan += span:
 	adc	r11, r1
 	rjmp	GeneratePulse 		; last data of this signal period is fetched

;	--------------------------------------------------------------
;    // there are more data to collect in this Signal period
;    // Now we try to switch the ADC to free running mode
more_data:
 	sts	ADCSRB, r1	;ADCSRB = 0;   // source ADC finish == 13 ADC clock cyclus free run

	rcall	store_data		; store new ADC data in array and count Samples

 	add	r8, r2			; m_shift += samples_per_adc_period;
 	adc	r9, r3			; + count of samples in previous periode (from S&H to S&H)
 	rjmp	CheckNextSample 	; check, if the next data is the last one in this signal period

; Store ADC data in caller's array
; Wait for ADC data ready with polling
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
wait_adc3:
 	lds	r17, ADCSRA		;  while (ADCSRA & (1 << ADIF) == 0)
 	sbrs	r17, ADIF
 	rjmp	wait_adc3 		; /* wait until conversion is done */
 	sts	ADCSRA, r17		; clear the interrupt ADIF

;    // next ADC data are ready
	lds	r17, ADCL		; neẃ ADC value
	lds	r15, ADCH
;##	sbrc	r24, smplADC_slow16		; if ((what1 & (1<<smplADC_slow16)) == 0) 
;##	lds	r17, ADCSRA

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
	adc	r7, r1			; add carry to r7
	ret				; return store_data

 .endfunc
