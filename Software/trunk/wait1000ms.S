#include <avr/io.h>
#include "config.h"




.func wait1000ms
.global wait5s		; wait 5 seconds
.global wait4s		; wait 4 seconds
.global wait3s		; wait 3 seconds
.global wait2s		; wait 2 seconds
.global wait1s		; wait 1 seconds
.global wait1000ms	; wait 2 second and wait 1000ms are identical
.global wait500ms	; wait 500ms
.global wait400ms	; wait 400ms
.global wait300ms	; wait 300ms
.global wait200ms	; wait 200ms
.global wait100ms	; wait 100ms
.global wait50ms	; wait 50ms
.global wait40ms	; wait 40ms
.global wait30ms	; wait 30ms
.global wait20ms	; wait 20ms
.global wait10ms	; wait 10ms
.global wait5ms		; wait 5ms
.global wait4ms		; wait 4ms
.global wait3ms		; wait 3ms
.global wait2ms		; wait 2ms
.global wait1ms		; wait 1ms
.global wait500us	; wait 500µs
.global wait400us	; wait 400µs
.global wait300us	; wait 300µs
.global wait200us	; wait 200µs
.global wait100us	; wait 100µs
.global wait50us	; wait 50µs
.global wait40us	; wait 40µs
.global wait30us	; wait 30µs
.global wait20us	; wait 20µs
.global wait10us	; wait 10µs
.global wait5us		; wait 5µs
.global wait4us		; wait 4µs
.global wait3us		; wait 3µs
.global wait2us		; wait 2µs
.global wait1us		; wait 1µs

;wait loops for ATmega8 at a clock of  1MHz / 2MHz / 4MHz und 8MHz
; use of flash memory is: (include 2 Byte Watch Dog reset at wait100ms)
; 68 Byte at 1MHz
; 70 Byte at 2MHz
; 76 Byte at 4MHz
; 74 Byte at 8Mhz
; No registers are used. (only stack pointer)
; A maximum of 28 bytes of space for return addresses is used in RAM 
; Every wait call needs only one instruction (rcall).
; I see no way to implement this function with C-language (too tricky)


wait5s:
 rcall wait1s		;	12+x return-adresses
wait4s:
 rcall wait1s		;	12+x return-adresses
; run to wait3s

wait3s:
 rcall wait1s		;1s	12+x Return-Adresses
; run to wait2s		;3s

wait2s:
 rcall wait1s		;1s     12+x Return-Adresses
; run to wait1s		,2s

wait1s:
wait1000ms:	
 rcall wait500ms	;500ms	11+x Return-Adresses
;laueft in wait500ms	;1s	

wait500ms:
 rcall wait100ms	;100ms	10+x Return-Adresses
wait400ms:
 rcall wait100ms	;200ms	10+x Return-Adresses

wait300ms:
 rcall wait100ms        ;300ms	10+x Return-Adresses
; laeuft in wait200ms	;500ms	10+x Return-Adresses


wait200ms:
 rcall wait100ms	;100ms	10+x Return-Adresses
; laeuft in wait100ms 	;200ms


wait100ms:
 wdr				; alle 100ms ein Watchdog reset!
 rcall wait50ms	    	; 50ms	9+x Return-Adresses
;laeuft in wait50us 	;100ms

wait50ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses 
wait40ms:
 rcall wait10ms	    	;20ms	8+x Return-Adresses  
;laeuft in wait30us 	;50ms

wait30ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses 
;laeuft in wait20us 	;30ms

wait20ms:
 rcall wait10ms	    	;10ms	8+x Return-Adresses
; laeuft in wait10ms	;20ms

wait10ms:
 rcall wait5ms	   	;5ms	7+x Return-Adresses
; laueft in wait5ms	;10ms

wait5ms:
 rcall wait1ms	   	;1ms	6+x Return-Adresses
wait4ms:
 rcall wait1ms	   	;2ms	6+x Return-Adresses
; laueft in wait3ms	;5ms	

wait3ms:
 rcall wait1ms	   	;1ms	6+x Return-Adresses
; laeuft in wait2ms	;3ms


wait2ms:
 rcall wait1ms	   	;1ms	6+x Return-Adresses
; laeuft in wait1ms	;2ms

wait1ms:
 rcall wait500us    	;500us	5+x Return-Adresses
;laeuft in wait500us	;1ms

wait500us:
 rcall wait100us    	;100us	4+x Return-Adresses
wait400us:
 rcall wait100us    	;200us	4+x Return-Adresses
;laeuft in wait300us	;500us

wait300us:
 rcall wait100us    	;100us	4+x Return-Adresses
;laeuft in wait200us	;300us

wait200us:
 rcall wait100us    	;100us	4+x Return-Adresses
; laeuft in wait100us	;200us	


wait100us:
 rcall wait50us	    	; 50us	3+x Return-Adresses
; laeuft in wait50us	;100us	

wait50us:
 rcall wait10us     	;10us	2+x Return-Adresses
wait40us:
 rcall wait10us     	;20us
; laeuft in wait30us	;50us

wait30us:
 rcall wait10us     	;10us	2+x Return-Adresses
; laeuft in wait20us	;20us

wait20us:
 rcall wait10us		;10us	2+x Return-Adresses
; laeuft weiter in wait10us	1+x Return-Adresses


wait10us:		;	1+x Return-Adresses

#if F_CPU == 1000000
; 1MHz Version			x = 0
; rcall needs at ATmega8L 3 clock cycles
 nop   ;if call need 4 clock cycles, delete on nop!
 nop
 nop
; shortest time possible for wait5us, wait4us, wait3us, wait2us and wait1us
wait5us:
wait4us:
wait3us:
wait2us:
wait1us:
; ret needs 4 clock cycles (1 µs each)
#endif

#if F_CPU == 2000000
; 2MHz version			x = 1
 rcall wait5us		;5us
; runs to wait5us	;total 10µs

wait5us:
 nop
 nop
wait4us:
 nop
; shortest time possible for  wait3us, wait2us and wait1us
wait3us:
wait2us:
wait1us:
#endif

#if F_CPU == 4000000
; 4MHz Version			x = 2
 rcall wait5us		;5us
wait5us:
 nop
 nop
 nop
 nop		;1us
wait4us:
rcall wait2us	;3us
; runs to  wait2us  ; total of 10µs

wait2us:
 nop
; shortest time possible for wait1us
wait1us:
#endif

#if F_CPU == 8000000
; 8MHz Version			x = 2
 rcall wait5us		;5us
; run directly to wait5us ;10us

wait5us:
 rcall wait1us		;1us
wait4us:
 rcall wait1us		;2us

wait3us:
 rcall wait1us		;3us
wait2us:
 rcall wait1us
; run directly to wait1us

wait1us:
 nop
#endif

#if F_CPU == 16000000
; 16MHz Version			x = 2
 rcall wait5us		;5us
; run directly to wait5us ;10us

wait5us:
 rcall wait1us		;1us
wait4us:
 rcall wait1us		;2us

wait3us:
 rcall wait1us		;3us
wait2us:
 rcall wait1us
; run directly to wait1us

wait1us:
 rcall wait500ns
.global wait500ns		; wait 500ns
wait500ns:
 nop
#endif

 ret
