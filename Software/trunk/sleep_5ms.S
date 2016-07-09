#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include "config.h"
#include <stdlib.h>

#define RCALL rcall

 .GLOBAL sleep_5ms
 .func sleep_5ms
 .extern wait5ms

 .section .text

#define DELAY_10ms  (((F_CPU_HZ / 100) - RESTART_DELAY_TICS) / TICS_PER_T2_COUNT)

#define DELAY_5ms  (((F_CPU_HZ / 200) - RESTART_DELAY_TICS) / TICS_PER_T2_COUNT)


;/* set the processor to sleep state */
;/* wake up will be done with compare match interrupt of counter 2 */
; void sleep_5ms(uint8_t spause){
sleep_5ms:
 	ldi	r25, 0x00	;  pause = spause;
 	cpi	r24, 201
 	brcs	wloop		; if (spause > 200) 


;// spause = 202 = 2s
;// spause = 203 = 3s
;// spause = 204 = 4s
;// spause = 205 = 5s
 	subi	r24, 0xC8	; 200 pause = (spause-200) * 200;
 	ldi	r20, 0xC8	; 200
 	mul	r24, r20	; (spause-200) * 200
 	movw	r24, r0		; r24:25 = (spause-200) * 200
 	eor	r1, r1

wloop:
 	sbiw	r24, 0x00	; 0 while (pause > 0)
 	brne	check_remain 
; 	sts	TIMSK2, r1	; TIMSK2 = (0<<OCIE2B) | (0<<OCIE2A) | (0<<TOIE2); /* disable output compare match A interrupt */ 
;#endif
 	ret

check_remain:
#if (F_CPU_HZ / 400) > RESTART_DELAY_TICS
        lds	r18, TCCR1B	; if (TCCR1B & ((1<<CS12)|(1<<CS11)|(1<<CS10)) != 0) 
	andi	r18, ((1<<CS12)|(1<<CS11)|(1<<CS10))	;
	breq	do_sleep
;  timer 1 is running, don't use sleep
	RCALL	wait5ms		; wait5ms();
	sbiw	r24, 1		; pause -= 1;
	rjmp	wloop
do_sleep:
 	cpi	r24, 0x01	; 1
 	cpc	r25, r1
 	breq	only_one 		; if (pause > 1)
 	sbiw	r24, 0x02	; pause -= 2;
;     // Startup time is too long with 1MHz Clock!!!!
	ldi	r19, DELAY_10ms ; 	/* set to 10ms above the actual counter */
 	rjmp	set_cntr 

only_one:
	ldi	r19, DELAY_5ms	; 	/* set to 5ms above the actual counter */
 	ldi	r24, 0x00	;  pause = 0;
 	ldi	r25, 0x00	; 0
   
set_cntr:
 	lds	r18, TCNT2
 	add	r18, r19	; + t2_offset
 	sts	OCR2A, r18	; OCR2A = TCNT2 + t2_offset;	/* set the compare value */
 	ldi	r20, ((0<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2)); /* enable output compare match A interrupt */ 
 	sts	TIMSK2, r20	; TIMSK2 = (0<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2); /* enable output compare match A interrupt */ 

;;	ldi	r18, (1 << SM1) | (1 << SM0) | (0 << SE); set_sleep_mode(SLEEP_MODE_PWR_SAVE)
;;	out	_SFR_IO_ADDR(SMCR), r18;        /*  SMCR = (1 <<SM1) | (1 << SM0) | (0 << SE); */
	ldi	r18, (1 << SM1) | (1 << SM0) | (1 << SE);
	out	_SFR_IO_ADDR(SMCR), r18;        /*  SMCR = (1 <<SM1) | (1 << SM0) | (1 << SE); */
	sleep			;
; // wake up after output compare match interrupt
	ldi	r18, (1 << SM1) | (1 << SM0) | (0 << SE);
	out	_SFR_IO_ADDR(SMCR), r18;        /*  SMCR = (1 << SM0) | (0 << SE); */
 	sts	TIMSK2, r1	; TIMSK2 = (0<<OCIE2B) | (0<<OCIE2A) | (0<<TOIE2); /* disable output compare match A interrupt */ 
 	wdr			; wdt_reset();

#else
;    // restart delay ist too long, use normal delay of 5ms
	RCALL	wait5ms		; wait5ms();	// wait5ms includes wdt_reset()
	sbiw	r24, 1		; pause -= 1;
#endif
 	rjmp	wloop 
 .endfunc
