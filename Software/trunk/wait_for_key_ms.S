#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <stdlib.h>
#include "config.h"
#include "lcd_defines.h"

#define RCALL rcall

#define MAX_CS 65

.GLOBAL wait_for_key_ms
#if INHIBIT_SLEEP_MODE
 .extern wait200ms
 .extern wait10ms
#else
 .extern sleep_5ms
#endif
.func wait_for_key_ms

.section .text
;/* wait max_time or previous key press */
;/* max_time zero wait without time limit */
;/* return value: !=0 == key is pressed for xx*10ms, 0 == key is not pressed, time expired */
;uint8_t wait_for_key_ms(int max_time) 
wait_for_key_ms:
        push	r17
 	push	r28		; save registers r28:29
 	push	r29
 	movw	r28, r24	; r28:29 = max_time
;  // if key is pressed, return 1
;  // if max_time == 0 , do not count, wait endless
#if INHIBIT_SLEEP_MODE
	RCALL	wait200ms	; wait200ms();
#else
 	ldi	r24,  40
 	RCALL	sleep_5ms	; wait_about200ms();
#endif
	ldi	r17, 0		; key_cs = 0
 	rjmp	no_cnt 		; wait max_time milliseconds or endless, if zero
wloop:
#if INHIBIT_SLEEP_MODE
	RCALL	wait10ms	; wait10ms();
#else
 	ldi	r24, 0x02	; 2
 	RCALL	sleep_5ms	; wait_about10ms();
#endif
 	sbic	_SFR_IO_ADDR(RST_PIN_REG), RST_PIN		; if(!(RST_PIN_REG & (1<<RST_PIN))) {
 	rjmp	not_pressed	; // If the key is pressed again... return(1)
	subi	r17, 0xff	; key_cs++;
        cpi	r17, MAX_CS	; if (key_cs >= MAX_CS)
	brcs	cnt_loop	;
        rjmp	to_ret		;  break;
not_pressed:
	cpse	r17, r1		; if ( key_cs != 0)
        rjmp	to_ret		; break;

cnt_loop:
 	wdr			; wdt_reset();
 	sbiw	r28, 0x00	; if (max_time > 0) 	// count only, if counter > 0
 	breq	no_cnt 		; special case zero, don't count
 	sbiw	r28, 0x0a	; max_time -= 10;			// 10 ms are done, count down
 	brne	no_cnt		; if (max_time == 0) max_time = -1;	// never count to zero, zero is endless!
 	ldi	r28, 0xFF	; max_time = -1
 	ldi	r29, 0xFF	; 
no_cnt:
 	sbrs	r29, 7		; while (max_time >= 0) 
 	rjmp	wloop 
to_ret:
        mov	r24, r17	; return(key_cs)
 	pop	r29		; restore registers r29:28
 	pop	r28
	pop	r17
 	ret
.endfunc


#ifdef WAIT_LINE2_CLEAR

 .GLOBAL wait_for_key_5s_line2
 .extern wait_for_key_ms
 .extern lcd_command
 .extern lcd_clear_line

 .func wait_for_key_5s_line2

; /* wait 5 seconds or previous key press, then clear line 2 of LCD and */
; /* set the cursor to the beginning of line 2 */
; void wait_for_key_5s_line2(void) 
wait_for_key_5s_line2:
 	ldi	r24, lo8(SHORT_WAIT_TIME)	; 0x88
 	ldi	r25, hi8(SHORT_WAIT_TIME)	; 0x13
 	RCALL	wait_for_key_ms		;wait_for_key_ms(SHORT_WAIT_TIME);
 	ldi	r24, (CMD_SetDDRAMAddress | 0x40) ; 0xC0
 	RCALL	lcd_command	; lcd_line2(); //2. row 
 	RCALL	lcd_clear_line	; lcd_clear_line();	// clear the whole line
 	ldi	r24, (CMD_SetDDRAMAddress | 0x40) ; 0xC0
 	RCALL	lcd_command	; lcd_line2(); //2. row 
 	ret
 .endfunc
#endif
