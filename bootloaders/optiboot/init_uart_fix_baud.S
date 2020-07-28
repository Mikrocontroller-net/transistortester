  ; setup the correct BAUD_RATE divider for Clock-frequency F_CPU
  ; using of 2 Stop Bit make the transfer more stable,
  ; usually this effects only the transmitter TX

  #ifndef TWO_STOP_BITS 
   #if TEST_OUTPUT == 1
    #define TWO_STOP_BITS 0         /* Test output with 0101010101 */
   #else
    #define TWO_STOP_BITS 1
   #endif
  #endif
  #define BAUD_DIV (((F_CPU / 4 / BAUD_RATE) - 1) / 2)
  #if BAUD_DIV > 255
   #undef BAUD_DIV
   #define BAUD_DIV (((F_CPU / 8 / BAUD_RATE) - 1) / 2)
   #if BAUD_DIV > 4095
    #error "Unachievable baud rate (too slow) BAUD_RATE"
   #endif // baud rate slow check
   #define BAUD_ACTUAL (F_CPU/(16 * ((BAUD_DIV)+1)))
   #undef UART_MODE_2x
	ldi	r24, UART_SCALER16	; single speed mode (0<<U2X)
  #else
   #define BAUD_ACTUAL (F_CPU/(8 * ((BAUD_DIV)+1)))
   #define UART_MODE_2x
	ldi	r24, UART_SCALER8	; double speed mode (1<<U2X)
  #endif		/* BAUD_DIV > 255 */
	AOUT	UART_CNTL, r24		; set scaler to 8 or 16

  #if BAUD_ACTUAL <= BAUD_RATE
    #define BAUD_ERROR (( 100*(BAUD_RATE - BAUD_ACTUAL) ) / BAUD_RATE)
    #if BAUD_ERROR >= 5
      #error "BAUD_RATE error less than -5%"
    #elif BAUD_ERROR >= 2
      #if VerboseLev > 3
        #warning "BAUD_RATE error less than -2%"
      #endif
    #endif
  #else
    #define BAUD_ERROR (( 100*(BAUD_ACTUAL - BAUD_RATE) ) / BAUD_RATE)
    #if BAUD_ERROR >= 5
      #error "BAUD_RATE error greater than 5%"
    #elif BAUD_ERROR >= 2
      #if VerboseLev > 3
        #warning "BAUD_RATE error greater than 2%"
      #endif
    #endif
  #endif

  #if ((BAUD_DIV < 3) && (BAUD_ERROR != 0))  || (BAUD_DIV < 0)
    // permit high bitrates (ie 1Mbps@16MHz) if error is zero
    #error "Unachievable baud rate (too fast) BAUD_RATE"
  #endif // baud rate fastn check
  #include "report_baud_div.h"
  #if defined(UART_SRC) && defined(UART_SEL)
	; version for m8, m16, m32 for example (with double function UCSRC)
	ldi	r25, (1<<UART_SEL)|(TWO_STOP_BITS<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)	; config UART
	AOUT	UART_SRC, r25
  	; This processor family has no double register for the 12-bit scaler
	ldi	r25, lo8(BAUD_DIV)	; 0x10
	AOUT	UART_SRRL, r25
   #if (BAUD_DIV/256) != 0 
	ldi	r25, hi8(BAUD_DIV)	; 0x10
	AOUT	UART_SRC, r25		; without (1<<URSEL) we write the upper BAUD_DIV
   #endif
  #else
    /* no ATmega8_16_32 */
   #if !defined(__AVR_ATmega163__) && !defined(__AVR_ATtiny87__) && !defined(__AVR_ATtiny167__)
	ldi	r25, (TWO_STOP_BITS<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01)	;0x06
	AOUT	UART_SRC, r25
   #endif
   #if (BAUD_DIV/256) != 0
	ldi	r25, hi8(BAUD_DIV)	; 0x10
	AOUT	UART_SRRH, r25
   #endif
	ldi	r25, lo8(BAUD_DIV)	; 0x10
	AOUT	UART_SRRL, r25
  #endif	/* defined(UCSRC) && defined(URSEL) */
  #ifdef UART_ONE_WIRE
	ldi	r24, UART_ENABLE_RX
  #else
	ldi	r24, (UART_ENABLE_RX|UART_ENABLE_TX)
  #endif
	AOUT	UART_SRB, r24
  #ifdef LINDAT
 ; 	AOUT	LINDAT, r0	;Write first data to start, I had found no other way to set LRXOK in LINSIR	
  #endif
