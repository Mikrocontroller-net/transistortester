
// June-Sept 2015, pa3fwm@amsat.org

#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include "config.h"
#include <stdlib.h>

.GLOBAL samplingADC
.func samplingADC




// note: we use the maximum clock divider, regardless of the CPU clock
// thus, a conversion always takes 1664 cycles
// we _could_ do this differently, and run a conversion in 832 cycles at 8 MHz
// but then lots of timing stuff in this routine will have to change, and possibly also in its callers

#define AUTO_CLOCK_DIV (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)


 .section .text

samplingADC:
// prototype for C declared in tt_function.h ; documentation is also there

	push	r17;

	mov	r30,r22			; r31:r30 := ptr
	mov	r31,r23

	neg r20

	sbrs r24,3			; if not two-impulses, set inter-impulse-timer to a reasonable value
	ldi	r25,6

	ldi	r17, 0			; enable ADC in free-running mode
	sts	ADCSRB, r17		; with interrupts enabled for the synchronization procedure
	ldi	r17, (1<<ADSC) | (1<<ADATE) | (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV;
	sts	ADCSRA, r17;
        ldi     r17, (1 << SM0) | (1 << SE);
        out     _SFR_IO_ADDR(SMCR), r17
        sleep 				; sleep until interrupt wakes us up; C-code has installed a
					; dummy interrupt handler consisting of just reti
        sleep 				; once more, just in case the previous one was started at an unfortunate time w.r.t. the ADC's cycle; this one definitely is started between two ADC completions
					; so now we're really synchronized
        ldi     r17, (1 << SM0) | (0 << SE)
        out     _SFR_IO_ADDR(SMCR), r17
					; don't forget to disable ADC interrupts though:
	ldi	r17, (1<<ADSC) | (1<<ADATE) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV
	sts	ADCSRA, r17
	
					; where are we now, time-wise?
					; interrupt handling from sleep took 12 cycles (datasheet section 7.7.1)
					; since then, we've done the above ldi, out, ldi, sts, total 5 cycles
					; S&H happens 1.5*128=192 clockcycles after interrupt
					; so we still need to way 192-12-5=175 cycles until sample is taken
					; the code applying the impulse or step signal takes 30 more cycles
					; so need to waste 145 cycles, in theory
					; in practice 139 cycles are needed (perhaps the interrupt timing is a bit different than I thought?)
	ldi r19,46
	nop
L03:	dec	r19
	brne L03


	; main measurement loop starts here
L05:
				; 6 cycles
	mov r23, r25			; prepare r21 and r23 for the right timing around the extra impulse (if used)
	lsr r23				; in the end, r21+r23=7
	dec r23
	ldi r21,7
	sub r21,r23
	AOUT	R_PORT, R14		; Rport_0	; does nothing for step response, but prepares levels for impulse(s); we assume that for impulse, PORT data is same for on and off, so pulse is created purely via DDR

				; 1+2*r21 cycles for 1<=r21<=6  (but 12 cycles for r21>6)
	dec r21
	breq L98
	dec r21
	breq L98
	dec r21
	breq L98
	dec r21
	breq L98
	dec r21
	breq L98
	dec r21
	breq L98
L98:
				; 5 cycles
	sbrc r24,3			; skip this impulse if not two pulses desired (r24 = control byte)
	AOUT	R_DDR, R12		; start of impulse
	dec r23
	sbrc r24,3			; skip this impulse if not two pulses desired
	AOUT	R_DDR, R16		; end of impulse
				; 2*r23 cycles for 1<=r23<=6
	breq L99			; note that the flag is set by the dec between the above to OUTs
	dec r23
	breq L99
	dec r23
	breq L99
	dec r23
	breq L99
	dec r23
	breq L99
	dec r23
	breq L99
L99:

				; 4 cycles
	AOUT	R_DDR, R12		; Rddr_0	; for step response, this effectively does nothing; for impulse response, it starts the (last) impulse
	nop
	AOUT	R_PORT, R18		; Rport_1	; beginning of step, or end of (last) impulse
	AOUT	R_DDR, R16		; Rddr_1	; start of first measurement is aligned with this

	ldi	r19,251		; 1004 cycles (i.e., almost 8 ADC cycles)
L04a:	dec	r19
	nop
	brne L04a

	sbrs r24,4		; 3 cycles in normal case, 2 cycles if slow16
	rjmp skipextradelay1

				; for slow16, we insert two extra delays around the reading of the conversion result
				; these extra delays sum up to 3*1664-15 cycles:
				; 3 entire conversion cycles of the ADC, minus 15 cycles so instead of moving the sample point by 1 cycle, we move it by 16 cycles
				; the extra delays vary with the sample number, such that the readout moment stays (approx.) fixed w.r.t. the ADC's conversion cycle, rather than with the main loop
				; note that slow16 could be implemented way faster: we now throw away 3 out of 4 conversion results,
				; while in principle those could all be used to need 4 times fewer ADC conversions; however, the timing and bookkeeping would be quite a headache
				; for non-slow16 the situation is much simpler, because then the entire response fits within one ADC conversion time

	mov r19,r20		; extra delay for slow16: this delay is 774+16*(i&0xfe) cycles, where i=number of sample 0..255, in r20
	lsr r19			; left shift r19, since the delay routine expects multiple of 32 cycles
	subi r19,-24		; r19=24+(i>>1) ; note that this is guaranteed to be within 1..255
	rcall r19del32		; delay =32*r19+6 = 32*24+6+16*(i&0xfe) ; the above calculations take another 4 cycles

skipextradelay1:

					; read the conversion result
					; note: this is a "safe" moment for doing so
					; this moment should not get close to the point where the conversion result becomes ready, lest errors occur and even a cycle is skipped/doubled
				; 16 cycles
	lds	r17, ADCL
	lds	r21, ADCH
	ld	r19,z
	sbrc 	r24,2			; check bit 2 in the control byte: if set do addition
	add	r17,r19
	st	z+,r17
	ld	r19,z
	sbrc 	r24,2			; check bit 2 in the control byte: if set do addition
	adc	r21,r19
	st	z+,r21


	sbrs r24,4		; 3 cycles in normal case, 2 cycles if slow16
	rjmp skipextradelay2

        mov r19,r20		; extra delay for slow16, 4203-16*(i&0xfe) cycles
        lsr r19			; r19=(i>>1)
	neg r19
        subi r19,-131		; r19=131-(i>>1) ; note that this is guaranteed to be within 1..255
	rcall r19del32		; delay = 32*r19+6 = 32*131+6-16*(i&0xfe); above calculations take another 5 cycles
skipextradelay2:

				; 4 cycles
					; toggle output back to its "idle" state in case of step response
	sbrc r24,0
	AOUT	R_DDR, R12		; Rddr_0
	sbrc r24,0
	AOUT	R_PORT, R14		; Rport_0


	ldi	r19,199			; 599 cycles make total loop take 1663 cycles = 13*128-1 = 1 less than period of ADC
	sbrc 	r24,1			; check bit 1 in the control byte: if set, then delay for 3 clockcycles fewer, effectively increasing the time between samples by a factor of 4
	dec	r19
L04c:	dec	r19
	brne L04c

				; 4 cycles (if not end of loop)
	inc r20				; n
	breq done
	rjmp L05

done:
	pop	r17
    	ret;


r19del32:			; delay 32*r19-1 + 4(ret) + 3(rcall) = 6+32*r19 cycles
	push r19	;2
	ldi r19,8	;3*8
	nop
l08:	dec r19	
	brne l08	
	pop r19		;2
	dec	r19
	brne r19del32
	ret
