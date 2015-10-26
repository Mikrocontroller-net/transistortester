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


#if AUTO_CLOCK_DIV == ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))
#define TICS_PER_ADC_CLOCK 128
#else
#define TICS_PER_ADC_CLOCK 64
#endif

#define ADC_StartDelay 32	/* we cannot trigger the ADC before the Counter1 is started */
#define ADC_SHtime  (TICS_PER_ADC_CLOCK*2)	/* time to ADC S&H for triggered start */

;===============================================================================================
;	This version uses counter1 for start of first ADC-cycle in a Signal sequence
;===============================================================================================

 .section .text
samplingADC:
;uint16_t samplingADC(R24:25 what, R22:23 array[], R20 nn, R18 Rport_1, R16 Rddr_1, R14 Rport_0, R12 Rddr_0) {}
; R24 = The length of the generated pulse in clock tics.
;	Supported values are 0-15 for pulse-length 1-16 .
;	Bit 0x10 is used for a direct pulse with length 1.
;	For direct pulse the ADC port is used as output without 680 Ohm resistors.
; R25 = Pulse-distance (span)
;	Possible values for pulse distance are 1,2,4,8,13,16,26,32,52,64,104,128 and 208 .
;	All other values match not with the ADC convertion time without remainder,
;	so that the sample time is not correct for the second and all following conversion periods.
; R22:23 The address of a array, which hold the data
	ldi	r26, 0		; no RAM space
	ldi	r27, 0
 	ldi	r30, lo8(gs(Retur1))	;0x6B	; 107
 	ldi	r31, hi8(gs(Retur1))	;0x32	; 50
 	jmp	__prologue_saves__
Retur1:
;	clr	r1
 	mov	R13, R18	; R13 = Rport_1 
	movw	R18,R24		; R18 = pulse-length+options, R19 = distance of samples
	and	R19, R19
	brne	no_zero
	ldi	R19, 1		; span must be 1
no_zero:
	andi	R18, 0x1f	; 0 - 15 for pulse length and direct-pulse option

	movw	R4, R22		; R4:5 = &array[0];
	ldi	R21, 1		; nn=256
	cpse	R20, r1
	ldi	R21, 0		; nn <256


	mov	r22, R20	; NN
	dec	r22
	mul	R19, r22	; (NN-1) * span
	movw	r22, r0		; r22:23 = last_sample = (NN-1) * span
	clr	r1		; restore zero register
	; R19 = span, r27=clock_div
	ldi	r26, lo8(TICS_PER_ADC_CLOCK*13)
	ldi	r27, hi8(TICS_PER_ADC_CLOCK*13)
	movw	r2, r26			; save tics per ADC cycle in r2:3

 	AOUT	R_PORT, R14	;  Rport_0	set to start condition
 	AOUT	R_DDR, R12	;  Rddr_0

 	ldi	r26, (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0); 
 	sts	ADCSRB, r26		; // trigger source COUNTER1 compare match B
 	ldi	r27, (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIF) | (0<<ADIE) | AUTO_CLOCK_DIV;
 	sts	ADCSRA, r27		; start first ADC with ext trigger, but start immediately
wait_adc:
 	lds	r26, ADCSRA		;  while (ADCSRA & (1 << ADSC))
 	sbrc	r26, ADSC
 	rjmp	wait_adc 		; /* wait until first initial conversion is done */

 	sts	ADCSRA, r26		; clear flags (1<<ADIF)

 	mov	r10, r1			; start_pos = 0;
 	mov	r11, r1
 	movw	r6, r10			; r6:7 Samples = 0;	// no ADC sample get yet

;    // The pulse generation is time critical
;    // we are just behind the previous cycle of the ADC
;    // time to next S&H is below 1.5 ADC clocks.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
;    // We switch back to the initial trigger source to stop the counter after completing this cycle.

//==============================================================================================
GeneratePulse:
 	; r2:3 = CPUtics of  a full ADC period (13 ADC-clocks)
	; r4:5 = Address of buffer
	; r6:7 = Samples,		the count of collected data
	; r8:9 = sample_pos		the tic position of actual sample
	; r10:11 = start_pos,		the tic position of the first sample in this signal period
	; R12 = Rddr_0
	; R13 = Rport_1
	; R14 = Rport_0
	; r15 = scratch
	; R16 = Rddr_1
	; r17 = scratch
	; R19 = span			each time shift step has span CPU-tics
	; R20:21 = nn			the number of requested data elements
	; r22:23 = last_sample		the position of last sample	
	; R24  = option, R18 = pulse_width, R19 = span
 	sts	TCCR1B, r1		;  TCCR1B = 0;	// stop counter1

 	sts	TCCR1A, r1  		; TCCR1A = 0; // set counter1 to normal mode
  
 	sts	TCNT1H, r1		;  TCNT1 = 0;	// set initial counter to zero
 	sts	TCNT1L, r1

;  // set the ADC Start time, documentation mentions a 3 CPU clock delay, which is compensated here
 	movw	r26, r10		; start_pos
	movw	r8, r10			; sample_pos = start_pos
	add	r26, R18		; + pulse_width
	adc	r27, r1
	adiw	r26, (ADC_StartDelay - 3)
 	sts	OCR1BH, r27		;  OCR1B = (ADC_StartDelay - 3 + start_pos);
 	sts	OCR1BL, r26		;  set compare B to start condition for this Pulse generation

 	subi	r26, lo8(-(ADC_SHtime + 16 + 3)) ; + time to S&H	
 	sbci	r27, hi8(-(ADC_SHtime + 16 + 3)) ; 
 	sts	OCR1AH, r27		; OCR1A = (ADC_StartDelay + ADC_SHtime + 16 + start_pos );
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
 	ldi	r26, (1<<ADIF) | (1<<ADIE)	; // stop ADC
 	sts	ADCSRA, r26
;##	in	r28, _SFR_IO_ADDR(SPL)
;##	in	r29, _SFR_IO_ADDR(SPH)
 	ldi	r30, 18			; restore full register list
 	jmp	__epilogue_restores__
;============== return ======================

get_next_data:
 	ldi	r26, (0<<ICNC1)|(1<<CS10)	; TCCR1B = (0<<ICNC1)|(1<<CS10);
 	sts	TCCR1B, r26			; // start counter at full speed
;============ Counter 1 is started =================================================
;  // We must count the CPU cycles used by the program to generate the signal just before S&H!
	; The counter starts ADC in ADC_StartDelay tics.
        ; The signal generation takes 13 tics + (dp_width-1).
	; So we must only delay the two ADC clock cycles (ADC_SHtime) plus the ADC_StartDelay,
	; but minus the count of tics for the signal generation.
	; The time difference for the different signal types is compensated with counter delay.

#define SignalStartDelay (ADC_SHtime+ADC_StartDelay-11+1)
;			   256	+      32          -11 = 277
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

	ldi	r30, lo8(gs(Return2)) ;11
	ldi	r31, hi8(gs(Return2)) ;10
	sub	r30, R18	;9 -pulse_width
	sbc	r31, r1		;8
	mov	r27, R14	;7  Rport_0
	or	r27, R13	;6 Rport_1
 	wdr			;5 wdt_reset();
	ijmp			;4 computed goto Return+(16-dp_width)
	rjmp	direct_pulse	; special pulse without resistors, two additional tics
	AOUT	R_PORT, R13	;17 R_PORT = Rport_1, dp_width = (16-1)
	AOUT	R_PORT, R13	;16 R_PORT = Rport_1, dp_width = (15-1)
	AOUT	R_PORT, R13	;15 R_PORT = Rport_1, dp_width = (14-1)
	AOUT	R_PORT, R13	;14 R_PORT = Rport_1, dp_width = (13-1)
	AOUT	R_PORT, R13	;13 R_PORT = Rport_1, dp_width = (12-1)
	AOUT	R_PORT, R13	;12 R_PORT = Rport_1, dp_width = (11-1)
	AOUT	R_PORT, R13	;11 R_PORT = Rport_1, dp_width = (10-1)
	AOUT	R_PORT, R13	;10 R_PORT = Rport_1, dp_width = (9-1)
	AOUT	R_PORT, R13	; 9 R_PORT = Rport_1, dp_width = (8-1)
	AOUT	R_PORT, R13	; 8 R_PORT = Rport_1, dp_width = (7-1)
	AOUT	R_PORT, R13	; 7 R_PORT = Rport_1, dp_width = (6-1)
	AOUT	R_PORT, R13	; 6 R_PORT = Rport_1, dp_width = (5-1)
	AOUT	R_PORT, R13	; 5 R_PORT = Rport_1, dp_width = (4-1)
	AOUT	R_PORT, R13	; 4 R_PORT = Rport_1, dp_width = (3-1)
 	AOUT	R_PORT, R13	; 3 R_PORT = Rport_1, dp_width = (2-1)	
Return2:
 	AOUT	R_PORT, r27	; 2 R_PORT = Rport_1|Rport_0; // beginning of step, or end of (last) impulse
 	AOUT	R_DDR, R16	; 1 R_DDR = Rddr_1; // start of first measurement is aligned with this
;============ End of time critical part =================================================
 	AOUT	R_PORT, R14	; R_PORT = Rport_0; only switch of unused Rport_0
	rjmp	wait_cnt

direct_pulse:
	nop			;16	+ 2 tics for rjmp
#if MHZ_CPU != 16
	nop			;15
#endif
	in	r30, _SFR_IO_ADDR(ADC_DDR)	;14
	in	r31, _SFR_IO_ADDR(ADC_PORT)	;13
	ldi	r26, (1<<TP3)	;12
	sbrc	R12, PIN_RL2	;11 is the bit for TP2 resistor set?
	ldi	r26, (1<<TP2)	;10
	sbrc	R12, PIN_RL1	;9 ist the bit for TP3 resistor set?
	ldi	r26, (1<<TP1)	;8
	; r26 now hold the bit for the direct ADC port
	mov	r27, r31	;7 ADC_PORT state
	or	r27, r26	;6 r27 is the for ADC port with HiPin set to 1
	or	r26, r30	;5 r26 enables the HiPin and LoPin output,  ADC_DDR
	AOUT	ADC_PORT, r27	;4 set Hipin to high
	AOUT	R_DDR, R16	;3 R_DDR = Rddr1 open all resistor ports
	AOUT	ADC_DDR, r26	;2 one clock tic high without resistor at HiPin, current about 5V/(42 Ohm)=119mA !!!
#if MHZ_CPU == 16
	AOUT	ADC_DDR, r26	;2 one clock tic high without resistor at HiPin, current about 5V/(42 Ohm)=119mA !!!
#endif
	AOUT	ADC_DDR, r30	;1 disable the HiPin output
;============ End of time critical part =================================================
	AOUT	ADC_PORT, r31	; reset Hipin to low
	rjmp	wait_cnt

wait_cnt:
	sbis	_SFR_IO_ADDR(TIFR1), OCF1A 	;  while (TIFR1 & (1 << OCF1A) == 0)
 	rjmp	wait_cnt 		; /* wait until counter1 compare match is done */
;---------------XXXXXXXX-------------------------

;	// The first triggered ADC conversion takes 13.5 ADC clock cycles from Counter Reg B compare
 	sts	TCCR1B, r1	; TCCR1B = 0;  // stop counter, no longer required_
 	ldi	r26, (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);
 	out	_SFR_IO_ADDR(TIFR1), r26	; clear interrupt flags
	

//==============================================================================================
CheckNextSample:
;    // The pulse generation is time critical.
;    // We are just behind the previous cycle of the ADC for repeated conversion.
;    // The time to next S&H is below 1.5 ADC clocks in this case.
;    // If required, the next Signal period begins in about 13 ADC-clocks.
;    // Let us look, if the next ADC S&H is within the sampling period
 	movw	r26, r8			; sample_pos

	add	r26, r2		; sample_pos + adc_period
	adc	r27, r3

 	cp	r22, r26	;R22:23 = position of last sample, r26:27 = sample_pos + adc_period+1 
 	cpc	r23, r27

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
	cp	r10, r8			; start_delay <= sample_pos
	cpc	r11, r9
 	brcc	behind_SH 		; if (m_shift > 0) 
       // This is not the first ADC-cycle in this Signal-generation cycle.
       // Let us wait for next SH time.
        ldi	r26, ((TICS_PER_ADC_CLOCK*3)/6)		; 1.5 * ADC_CLOCK
lop3:
	dec	r26
	brne	lop3

behind_SH:
        ; -------------------------------------
	; toggle output back to the idle state 
 	AOUT	R_PORT, R14	; 5  Rport_0
 	AOUT	R_DDR, R12	; 4  Rddr_0

	rcall	store_data		; store new ADC data to array and count Samples
;    // This was the last ADC data of this Signal period, update the time shift registers

 	add	r10, R19		; start_pos += span:
 	adc	r11, r1
	call	wait200us	; ############# for Test only #################################################
 	rjmp	GeneratePulse 		; last data of this signal period is fetched

;	--------------------------------------------------------------
;    // there are more data to collect in this Signal period
;    // Now we try to switch the ADC to free running mode
more_data:
 	sts	ADCSRB, r1	;ADCSRB = 0;   // source ADC finish == 13 ADC clock cyclus free run

	rcall	store_data		; store new ADC data in array and count Samples

 	add	r8, r2			; sample_pos += adc_period;
 	adc	r9, r3			; 
 	rjmp	CheckNextSample 	; check, if the next data is the last one in this signal period

; Store ADC data in caller's array
; Wait for ADC data ready with polling
; The position of array cell is array[start1 + m_shift]
; r8:9 = position of sample, R4:5 = beginn of array
; Function use register r17:15 to get new ADC data and
; register r26:27 to read (and accumulate) the old data at the array place.
; every call increments the Samples counter r6:7 .
store_data:
 	movw	r26, r8			; sample_pos
	movw	r30, R4			; &array
sub_loop:
	sub	r26, R19		; - span
	sbc	r27, r1
	brcs	is_found
	adiw	r30, 2			; increment array address
	rjmp	sub_loop
is_found:
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
