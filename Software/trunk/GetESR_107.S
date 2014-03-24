#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include <avr/eeprom.h>
#include "config.h"
#include <stdlib.h>

 .GLOBAL GetESR
 .func GetESR

/* MAX_CNT is the maximum loop counter for repetition of mesurement */
#define MAX_CNT 128

/* ADC_Sleep_Mode enables the sleep state for reading ADC */
//#define ADC_Sleep_Mode

/* ESR_DEBUG enables additional Output on row 3 and row 4 */
//#define ESR_DEBUG

#ifdef INHIBIT_SLEEP_MODE
 /* Makefile option set to disable the sleep mode */
 #undef ADC_Sleep_Mode
#endif

#define zero_reg r1
//  uint8_t big_cap;
#define big_cap 1

//  unsigned long sumvolt0,sumvolt1,sumvolt2;	//  3 sums of ADC readings
#define sumvolt0 2 /* r14-r17 */
#define sumvolt1 6 /* SP + 6:9 */
#define sumvolt2 10 /* SP + 10:13 */

//  uint8_t LoADC;		// used to switch Lowpin directly to GND or VCC
#define LoADC 14	/* SP + 14 */

//  uint8_t HiADC;		// used to switch Highpin directly to GND or VCC
#define HiADC 15	/* SP + 15 */

//  unsigned int adcv[3];		// array for 3 ADC readings
// first part adcv0 r2/r3
// first part adcv1 Y+16/17
#define adcvv1 16
// first part adcv2 Y+18/19
#define adcvv2 18

// unsigned long cap_val_nF;	// capacity in nF
#define cap_val_nF 20	/* SP + 20:23 */

#define adcv0L r2
#define adcv0H r3
#define adcv2L r24
#define adcv2H r25

//  uint8_t HiPinR_L;		// used to switch 680 Ohm to HighPin
#define HiPinR_L r12

//  uint8_t LoPinR_L;		// used to switch 680 Ohm to LowPin
#define LoPinR_L r7

//  uint8_t ii,jj;		// tempory values

//  uint8_t StartADCmsk;		// Bit mask to start the ADC
#define StartADCmsk r10

//  uint8_t SelectLowPin;
#define SelectLowPin r6

//  uint8_t SelectHighPin;
#define SelectHighPin r11

//  int8_t esr0;			// used for ESR zero correction
// #define esr0 r2

// Structure cap:
 .extern cap
#define cval_max 4
#define esr 12
#define ca 16
#define cb 17
#define cpre_max 19

 .extern EE_ESR_ZERO

#ifdef ADC_Sleep_Mode
//  #define StartADCwait() ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */
//   set_sleep_mode(SLEEP_MODE_ADC);
//   sleep_mode()		/* Start ADC, return if ADC has finished */
    .macro StartADCwait
        ldi	r24, (1 << SM0) | (1 << SE);
       	out	_SFR_IO_ADDR(SMCR), r24; 	/*  SMCR = (1 << SM0) | (1 << SE); */
    sleep;			/* wait for ADC */
        ldi	r24, (1 << SM0) | (0 << SE);
       	out	_SFR_IO_ADDR(SMCR), r24; 	/*  SMCR = (1 << SM0) | (1 << SE); */
     .endm
#else
//  #define StartADCwait() ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable ADC and start */
    .macro StartADCwait
	sts	ADCSRA, StartADCmsk; 	/* ADCSRA = StartADCmsk = r10 */
 	lds	r24, ADCSRA; 	/* while (ADCSRA & (1 <<ADSC)) */
       	sbrc	r24, ADSC; 
       	rjmp	.-8 ;   /* wait until conversion is done */
     .endm
#endif

/* ************************************************************************************ */
/* Adjust the timing for switch off the load current for big capacitors                 */
/* ************************************************************************************ */
				// with wdt_reset the timing can be adjusted,
				// when time is too short, voltage is down before SH of ADC
				// when time is too long, capacitor will be overloaded.
				// That will cause too high voltage without current.
 .macro DelayBigCap
 	call	wait10us;			// SH at 2.5 ADC clocks behind start = 20 us
 	call	wait5us
#ifdef ADC_Sleep_Mode
 /* Wake up from sleep with Interrupt: 1+4+4T, jmp, rti, ldi, out takes 18 clock tics,  */
 /* 3+1 clock tics (sts,out) are needed for instructions before the current is switched off. */
 #if F_CPU == 8000000UL
	call	wait2us;	/* wait2us();    // with only 17 us delay the voltage goes down before SH */ 
	/* delay 17us + 3 clock tics (CALL instead of RCALL) = 17.375 us @ 8 MHz */ 
	/* + 22 clock tics delay from interrupt return, +2.75us = 20.125 + */ 
	/* 19.875 us - */ 	/* 20.0 us ? */
 #endif
 #if F_CPU == 16000000UL
	call	wait3us;	/* wait3us();    // with only 18 us delay the voltage goes down before SH */
            /* delay 18us + 3 clock tics (CALL instead of RCALL) = 18.1875 us */
            /* + 22 clock tics delay from interrupt return, +1.375us = 19.5625 us */
	call	wait500ns;	/* wait500ns();  // 20.125 us + */
	/*  19.9375  us - */	/* 20.0625  us ? */ 	/* 20.1875 us + */
 #endif


#else
 /* Polling mode: lds,sbrc,sts and out Instructions are 7 clock tics */
 #if F_CPU == 8000000UL
 	call	wait4us;			// with only 18 us delay the voltage goes down before SH
         /* delay 19us + 3 clock tics (CALL instead of RCALL) = 19.375 us @ 8 MHz */
         /* + 7 clock tics delay from while loop, +0.875us  = 20.25 us + */
	/* 19.875us - */ 	/* 20.0  us ? */ 	/* 20.125us + */
 #endif
 #if F_CPU == 16000000UL
 	call	wait4us;			// with only 18 us delay the voltage goes down before SH
        /* delay 19us + 3 clock tics (CALL instead of RCALL) = 19.1875 us */
        /* + 7 clock tics delay from while loop, +0.4375us = 19.625 us */
	push	r24;	/* 19.75  us */
	pop	r24;	/* 19.875 us */
	wdr	;		/* wdt_reset();  // 19.9375us  */
	wdr	;		/* wdt_reset();  // 20.0   us - */
	wdr	;		/* wdt_reset();  // 20.0625us ? */
	wdr	;		/* wdt_reset();  // 20.125 us + */
 #endif
#endif
 .endm

/* ************************************************************************************ */
/* Adjust the timing for switch off the load current for small capacitors               */
/* ************************************************************************************ */
 .macro DelaySmallCap
#ifdef ADC_Sleep_Mode
 /* Wake up from sleep with Interrupt: 1+4+4T, jmp, rti, ldi, out takes 18 clock tics,  */
 /* 5+1 clock tics (sts,rjmp,out) are needed for instructions before the current is switched off. */
 #if F_CPU == 8000000UL
	/* Restart from sleep needs more than 2us, that is more than one ADC-clock tic in fast mode. */
	/* More than one clock delay for the restart of ADC is required, 3.5 instead of 2.5 ADC clock delay */
	call	wait4us;	/* wait4us();  // with only 3 us delay the voltage goes down before SH */
            /* delay 4us + 1 clock tics (CALL instead of RCALL) = 4.125 us @ 8 MHz */
            /* + 24 clock tics delay from interrupt return, +3.0us = 7.125 us + */
	/* 6.75us - */ 	/* 6.875us ? */ 	/* 7.0us + */
 #endif
 #if F_CPU == 16000000UL
	call	wait3us;	/* wait3us();  // with only 3 us delay the voltage goes down before SH */
            /* delay 3us + 1 clock tics (CALL instead of RCALL) = 3.0625 us */
            /* + 24 clock tics delay from interrupt return, +1.5us = 4.5625 us */
	call	wait500ns;	/* wait500ns();  // 5.125 us + */
	/*  4.9375us - */ 	/* 5.0625us ? */	/* 5.1875 us + */
 #endif
#else
 /* Polling mode, lds,sbrc,sts,rjmp and out Instructions are 9 clock tics */
 #if F_CPU == 8000000UL
 	call	wait4us;			// with only 3 us delay the voltage goes down before SH
            /* delay 4us + 1 clock tic (CALL instead of RCALL) = 4.125 us @ 8 MHz */
            /* + 9 clock tics delay from while loop, +1.125us  = 5.25  us + */
	/* 4.875us - */ 	/* 5.0  us ? */		/* 5.125us + */
 #endif
 #if F_CPU == 16000000UL
 	call	wait4us;			// with only 18 us delay the voltage goes down before SH
            /* delay 4us + 1 clock tics (CALL instead of RCALL) = 4.0625 us */
            /* + 9 clock tics delay from while loop, +0.5625us = 4.625 us */
	push	r24;	/* 4.8125 us */
	pop	r24;	/* 4.9375 us */
	wdr	;		/* wdt_reset(); // 5.0   us - */
	wdr	;		/* wdt_reset(); // 5.0625us ? */
	wdr	;		/* wdt_reset(); // 5.125 us + */
 #endif
#endif
 .endm

//=================================================================


//=================================================================
//void GetESR() {
 .section .text
GetESR:
       	push	r2
       	push	r3
       	push	r4
       	push	r5
       	push	r6
       	push	r7
       	push	r8
       	push	r9
       	push	r10
       	push	r11
       	push	r12
       	push	r13
       	push	r14
       	push	r15
       	push	r16
       	push	r17
       	push	r29
       	push	r28
       	in	r28, _SFR_IO_ADDR(SPL);
       	in	r29, _SFR_IO_ADDR(SPH);
       	sbiw	r28, 0x1a;
       	in	r0, _SFR_IO_ADDR(SREG);
       	cli
       	out	_SFR_IO_ADDR(SPH), r29;
       	out	_SFR_IO_ADDR(SREG), r0;
       	out	_SFR_IO_ADDR(SPL), r28;


 	lds	r18, cap+cval_max;	/* cap_val_nF = cap.cval_max; */
 	lds	r19, cap+cval_max+1;
 	lds	r20, cap+cval_max+2;
 	lds	r21, cap+cval_max+3;
 	lds	r17, cap+cpre_max;	/* prefix = cap.cpre_max; */

        rjmp	ad_35ba;

ad_35ac:
       	movw	r24, r20;		/* cval /= 10;		// reduce value by factor ten */
       	movw	r22, r18
       	ldi	r18, 0x0A; 10
        mov	r19, zero_reg
        mov	r20, zero_reg
        mov	r21, zero_reg
 	call	__udivmodsi4;		/* r18:21 = r22:25  / r18:21 */
      	subi	r17, 0xFF;		/* prefix++;		// take next decimal prefix */

ad_35ba:
       	cpi	r17, -9;		/* while (prefix < -9) { // set cval to nF unit */
       	brlt	ad_35ac;		/*  } */
	std	Y+cap_val_nF, r18
	std	Y+cap_val_nF+1, r19
	std	Y+cap_val_nF+2, r20
	std	Y+cap_val_nF+3, r21


       	cpi	r18, lo8(1800/4);	/* if (cap_val_nF < (1800/4)) return;	//capacity lower than 1.8 uF */
       	ldi	r22, hi8(1800/4)
       	cpc	r19, r22
       	cpc	r20, zero_reg
       	cpc	r21, zero_reg
       	brcc	ad_35e4 
       	rjmp	ad_exit; 
ad_35e4:
#ifdef ADC_Sleep_Mode
        ldi	r24, (1 << SM0) | (1 << SE);
       	out	_SFR_IO_ADDR(SMCR), r24; 	/*  SMCR = (1 << SM0) | (1 << SE); */
#endif
       	cpi	r18, lo8((1800*2)+1);	/* if (cap_val_nF > (1800*2)) { */
       	ldi	r23, hi8((1800*2)+1);
       	cpc	r19, r23
       	cpc	r20, zero_reg
       	cpc	r21, zero_reg
       	brcs	ad_35fe 

       	ldi	r24, 0x01; 1
       	std	Y+big_cap, r24;		/* big_cap = 1; */
       	cpi	r18, lo8(50000);	/* if (cap_val_nF > (50000)) { */
       	ldi	r23, hi8(50000);
       	cpc	r19, r23
       	cpc	r20, zero_reg
       	cpc	r21, zero_reg
       	brcs	not_very_big
       	ldi	r24, 0x02; 2
       	std	Y+big_cap, r24;		/* big_cap = 2; // very big capacitor */
not_very_big:
     /* normal ADC-speed, ADC-Clock 8us */
#ifdef ADC_Sleep_Mode
       	ldi	r25, (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */
       	mov	StartADCmsk, r25
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#else
       	ldi	r18, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable and start ADC */
       	mov	StartADCmsk, r18
#endif
       	rjmp	ad_3604;		/* } else { */
ad_35fe:
     /* fast ADC-speed, ADC-Clock 2us */
#ifdef ADC_Sleep_Mode
       	ldi	r25, (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | FAST_CLOCK_DIV; /* enable ADC and Interrupt */
       	mov	StartADCmsk, r25
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#else
       	ldi	r25, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; /* enable and start ADC */
       	mov	StartADCmsk, r25
#endif
       	std	Y+big_cap, zero_reg;	/* big_cap = 0 */
					/*  }  */

ad_3604:
       	ldi	r24, lo8(ESR_str);
       	ldi	r25, hi8(ESR_str);
#ifdef USE_EEPROM
 	call	lcd_fix_string;		/* lcd_MEM_string(ESR_str);	// " ESR=" */
#else
 	call	lcd_mem_string;		/* lcd_MEM_string(ESR_str);	// " ESR=" */
#endif
 	lds	r14, cap+ca;		/* LoADC = pgm_read_byte(&PinADCtab[cap.ca]) | TXD_MSK; */
        mov	SelectLowPin,r14
        LDIZ	PinADCtab;
        add	r30, r14
	adc	r31, zero_reg
        lpm	r24, Z+;
       	ori	r24, TXD_MSK;
       	std	Y+LoADC, r24;
 	lds	r15, cap+cb;		/* HiADC = pgm_read_byte(&PinADCtab[cap.cb]) | TXD_MSK; */
        mov	SelectHighPin,r15
        LDIZ	PinADCtab;
        add	r30, r15
	adc	r31, zero_reg
        lpm	r24, Z+;
       	ori	r24, TXD_MSK;
       	std	Y+HiADC, r24;
        LDIZ	PinRLtab;	/* LoPinR_L = pgm_read_byte(&PinRLtab[cap.ca]);  //R_L mask for LowPin R_L load */
        add	r30, r14
	adc	r31, zero_reg
        lpm	LoPinR_L, Z+;
        LDIZ	PinRLtab;	/* HiPinR_L = pgm_read_byte(&PinRLtab[cap.cb]);	//R_L mask for HighPin R_L load */
        add	r30, r15
	adc	r31, zero_reg
        lpm	HiPinR_L, Z+;


#if (PROCESSOR_TYP == 644) || (PROCESSOR_TYP == 1280)
    /* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
//  SelectLowPin = (cap.ca | (1<<REFS1) | (0<<REFS0));	// switch ADC to LowPin, Internal Ref. 
       	ldi	r25, (1<<REFS1)|(0<<REFS0);	0xC0
       	or	SelectLowPin, r25
//  SelectHighPin = (cap.cb | (1<<REFS1) | (0<<REFS0));	// switch ADC to HighPin, Internal Ref. 
       	or	SelectHighPin, r25
#else
//  SelectLowPin = (cap.ca | (1<<REFS1) | (1<<REFS0));	// switch ADC to LowPin, Internal Ref. 
       	ldi	r25, (1<<REFS1)|(1<<REFS0);	0xC0
       	or	SelectLowPin, r25
//  SelectHighPin = (cap.cb | (1<<REFS1) | (1<<REFS0));	// switch ADC to HighPin, Internal Ref. 
       	or	SelectHighPin, r25
#endif


// Measurement of ESR of capacitors AC Mode
       	ldi	r24, 0x01; 	/* sumvolt0 = 1;	// set sum of LowPin voltage to 1 to prevent divide by zero */
       	mov	r14, r24
       	mov	r15, zero_reg
       	mov	r16, zero_reg
       	mov	r17, zero_reg
       	std	Y+sumvolt1, r24;	/* sumvolt1 = 1;	// clear sum of HighPin voltage with current */
//                                // offset is about (x*10*200)/34000 in 0.01 Ohm units
       	std	Y+sumvolt1+1, zero_reg;
       	std	Y+sumvolt1+2, zero_reg;
       	std	Y+sumvolt1+3, zero_reg;
       	std	Y+sumvolt2, zero_reg;	/* sumvolt2 = 0;	// clear sum of HighPin voltage without current */
       	std	Y+sumvolt2+1, zero_reg;
       	std	Y+sumvolt2+2, zero_reg;
       	std	Y+sumvolt2+3, zero_reg;
 	call	EntladePins;	/* EntladePins();	// discharge capacitor */
       	ldi	r24, TXD_VAL;
       	out	_SFR_IO_ADDR(ADC_PORT), r24; /* ADC_PORT = TXD_VAL;	// switch ADC-Port to GND */
 	sts	ADMUX, SelectLowPin;	/* ADMUX = SelectLowPin;	// set Mux input and Voltage Reference to internal 1.1V */
#ifdef NO_AREF_CAP
 	call	wait100us;			/* time for voltage stabilization */
#else
 	call	wait10ms;    		/* time for voltage stabilization with 100nF */
#endif

   // Measurement frequency is given by sum of ADC-Reads < 680 Hz for normal ADC speed.
   // For fast ADC mode the frequency is below 2720 Hz (used for capacity value below 3.6 uF).
   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after real start of conversion.
   // Real ADC-conversion is started with the next ADC-Clock (125kHz) after setting the ADSC bit.
       	eor	r13, r13;			/* for(ii=0;ii<MAX_CNT;ii++) { */
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
       	ldi	r27, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV;	/* enable ADC and start with ADSC */
       	mov	r9, r27
//         adcv[0] = ADCW;			// Voltage LowPin with current
//         ADMUX = SelectHighPin;
       	ldi	r26, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV;	/* enable ADC and start with ADSC */
       	mov	r8, r26
      /* ********* Forward direction, connect Low side with GND *********** */
ad_3692:
       	ldd	r19, Y+LoADC;
       	out	_SFR_IO_ADDR(ADC_DDR), r19;	/* ADC_DDR = LoADC;	// switch Low-Pin to output (GND) */
 	sts	ADMUX, SelectLowPin;		/* ADMUX = SelectLowPin; */
      StartADCwait				/* start ADC and wait */

;=======					/* while (1) { */
while_lop1:
       	wdr	;				/* wdt_reset(); */
 	sts	ADMUX, SelectLowPin;		/* ADMUX = SelectLowPin; */
       	out	_SFR_IO_ADDR(R_PORT), HiPinR_L;	/* R_PORT = HiPinR_L;	// switch R-Port to VCC */
       	out	_SFR_IO_ADDR(R_DDR), HiPinR_L;	/* R_DDR = HiPinR_L;	// switch R_L port for HighPin to output (VCC) */
      StartADCwait				/* start ADC and wait */
      StartADCwait				/* start ADC and wait */
 	lds	adcv0L, ADCW;			/* adcv[0] = ADCW;	// Voltage LowPin with current */
 	lds	adcv0H, ADCW+1
	sts	ADMUX, SelectHighPin;		/* ADMUX = SelectHighPin; */

       	ldd	r20, Y+big_cap;			/* if (!big_cap) { */
       	and	r20, r20
       	brne	ad_big1 
        /* **** Polling mode, small cap **** */
      StartADCwait				/* start ADC and wait */
 	sts	ADCSRA, r9;		/* ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; // enable ADC and start */
        DelaySmallCap;		/* wait the time defined by macro */
       	rjmp	ad_swoff1;			/* } else { */
ad_big1:
	/* **** Polling mode, big cap **** */
      StartADCwait				/* start ADC and wait */
			// Start Conversion, real start is next rising edge of ADC clock
 	sts	ADCSRA, r8;		/* ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start */
	DelayBigCap;			/* wait the time defined by macro */
						/* } */
ad_swoff1:
       	out	_SFR_IO_ADDR(R_DDR), zero_reg;	/* R_DDR = 0; // switch current off,  SH is 1.5 ADC clock behind real start */
       	out	_SFR_IO_ADDR(R_PORT), zero_reg;	/* R_PORT = 0; */
ad_370c:
 	lds	r24, ADCSRA;		/* while (ADCSRA&(1<<ADSC));	// wait for conversion finished */
       	sbrc	r24, ADSC
       	rjmp	ad_370c 

 	lds	r18, ADCW;		/* adcv[1] = ADCW;	// Voltage HighPin with current */
 	lds	r19, ADCW+1;

#ifdef ADC_Sleep_Mode
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#endif
      StartADCwait			/* start ADC and wait */
      StartADCwait			/* start ADC and wait */

 	lds	r24, ADCW;		/* adcv[2] = ADCW;	// Voltage HighPin without current */
 	lds	r25, ADCW+1;

       	cpi	r24, 0x03;		/* if (adcv[2] > 2) break;	// at least more than two digits required */
       	cpc	r25, zero_reg
       	brcc	end_while1;
	rjmp	while_lop1;		/* } // end while (1) */
;=======

end_while1:
//      sumvolt0 += adcv[0];		// add sum of both LowPin voltages with current
//      adcv0 = r2/r3
//      sumvolt1 += adcv[1];		// add  HighPin voltages with current
//      adcv1 = r18/r19
       	std	Y+adcvv1, r18;
       	std	Y+adcvv1+1, r19;
//      sumvolt2 += adcv[2]; 		// capacitor voltage without current
//      adcv2 = R24/r25
       	std	Y+adcvv2, r24;
       	std	Y+adcvv2+1, r25;

      /* ********* Reverse direction, connect High side with GND *********** */
       	ldd	r19, Y+HiADC;		/* ADC_DDR = HiADC;	// switch High Pin to GND */
       	out	_SFR_IO_ADDR(ADC_DDR), r19; 

      StartADCwait			/* start ADC and wait */

;=======				/*  while (1) { */
while_lop2:
       	wdr	;			/* wdt_reset(); */
 	sts	ADMUX, SelectHighPin;	/* ADMUX = SelectHighPin; */
       	out	_SFR_IO_ADDR(R_PORT), LoPinR_L;	/* R_PORT = LoPinR_L; */
       	out	_SFR_IO_ADDR(R_DDR), LoPinR_L;	/* R_DDR = LoPinR_L;	// switch LowPin with 680 Ohm to VCC */

      StartADCwait			/* start ADC and wait */
      StartADCwait			/* start ADC and wait */

 	lds	r22, ADCW;         	/* adcv[0] = ADCW;	// Voltage HighPin with current */
 	lds	r23, ADCW+1;
 	sts	ADMUX, SelectLowPin;	/*  ADMUX = SelectLowPin; */
       	ldd	r20, Y+big_cap	;  	/*  if (!big_cap) { */
       	and	r20, r20
       	brne	ad_big2 
    // ****** Polling mode small cap
      StartADCwait				/* start ADC and wait */
 	sts	ADCSRA, r9;		/* ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; // enable ADC and start */
        DelaySmallCap;		/* wait the time defined by macro */
       	rjmp	ad_swoff2;		/* } else {  */
ad_big2:
        // ****** Polling mode big cap
      StartADCwait			/* start ADC and wait */

 	sts	ADCSRA, r8;	/* ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start with ADSC */
	DelayBigCap;			/* wait the time defined by macro */
					/*  }  */
ad_swoff2:
       	out	_SFR_IO_ADDR(R_DDR), zero_reg; 	// switch current off, SH is 1.5 ADC clock ticks behind real start
       	out	_SFR_IO_ADDR(R_PORT), zero_reg; 
ad_37f4:
 	lds	r24, ADCSRA;		/* while (ADCSRA&(1<<ADSC));	// wait for conversion finished */
       	sbrc	r24, ADSC
       	rjmp	ad_37f4 

 	lds	r20, ADCW;		/* adcv[1] = ADCW;	//  Voltage LowPin with current */
 	lds	r21, ADCW+1;
#ifdef ADC_Sleep_Mode
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#endif
      StartADCwait			/* start ADC and wait */
      StartADCwait			/* start ADC and wait */

 	lds	r18, ADCW;		/* adcv[2] = ADCW;	// Voltage LowPin without current */
 	lds	r19, ADCW+1;

       	cpi	r18, 0x03;		/* if (adcv[2] > 2) break;	// at least more than two digits required */
       	cpc	r19, zero_reg
       	brcc	end_while2;
	rjmp	while_lop2;		/*  } // end while (1) */
;=======
end_while2:
       	out	_SFR_IO_ADDR(R_DDR), zero_reg; 	/* R_DDR = 0; // switch current off */

       	movw	r24, r22;		/* sumvolt0 += adcv[0];	// add LowPin voltages with current */
       	add	r24, adcv0L;		/* 	// add sum of both LowPin voltages with current */
       	adc	r25, adcv0H

       	add	r14, r24
       	adc	r15, r25
       	adc	r16, zero_reg
       	adc	r17, zero_reg
	std	Y+sumvolt0, r14
	std	Y+sumvolt0+1, r15
	std	Y+sumvolt0+2, r16
	std	Y+sumvolt0+3, r17
       	ldd	r24, Y+adcvv1;		/* sumvolt1 += adcv[1];	// add HighPin voltages with current */
       	ldd	r25, Y+adcvv1+1; 
       	add	r24, r20
       	adc	r25, r21
       	ldd	r20, Y+sumvolt1;	/* sumvolt1 += adcv[1];	// add  HighPin voltages with current */
       	ldd	r21, Y+sumvolt1+1;
       	ldd	r22, Y+sumvolt1+2;
       	ldd	r23, Y+sumvolt1+3;
       	add	r20, r24
       	adc	r21, r25
       	adc	r22, zero_reg
       	adc	r23, zero_reg
       	std	Y+sumvolt1, r20;
       	std	Y+sumvolt1+1, r21;
       	std	Y+sumvolt1+2, r22;
       	std	Y+sumvolt1+3, r23;
       	ldd	r24, Y+adcvv2;		/* // add HighPin voltages without current */
       	ldd	r25, Y+adcvv2+1; 0x11
       	add	r24, r18
       	adc	r25, r19
       	ldd	r20, Y+sumvolt2;	/* sumvolt2 += adcv[2];	// add  HighPin voltages without current */
       	ldd	r21, Y+sumvolt2+1;
       	ldd	r22, Y+sumvolt2+2;
       	ldd	r23, Y+sumvolt2+3;
       	add	r20, r24
       	adc	r21, r25
       	adc	r22, zero_reg
       	adc	r23, zero_reg
       	std	Y+sumvolt2, r20	;
       	std	Y+sumvolt2+1, r21;
       	std	Y+sumvolt2+2, r22;
       	std	Y+sumvolt2+3, r23;

   // Measurement frequency is given by sum of ADC-Reads < 680 Hz for normal ADC speed.
   // For fast ADC mode the frequency is below 2720 Hz (used for capacity value below 3.6 uF).
   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after real start of conversion.
   // Real ADC-conversion is started with the next ADC-Clock (125kHz) after setting the ADSC bit.

       	inc	r13;			/* for(    ;ii<MAX_CNT;ii++)  */
       	mov	r21, r13
       	cpi	r21, MAX_CNT;
       	breq	ad_38ac
       	rjmp	ad_3692;		/* } // end for */
ad_38ac:
#ifdef ESR_DEBUG
      	movw	r22, r14;		/* DisplayValue(sumvolt0,0,'L',4); */
      	movw	r24, r16; 
      	ldi	r20, 0;
      	ldi	r18, 'L';
      	ldi	r16, 4	;
	call	DisplayValue
#endif
 	ldd	r10, Y+cap_val_nF;
 	ldd	r11, Y+cap_val_nF+1;
 	ldd	r12, Y+cap_val_nF+2;
 	ldd	r13, Y+cap_val_nF+3;

       	ldd	r22, Y+big_cap;		/* if (big_cap) { */
       	and	r22, r22
       	breq	is_small 
   // HighPin Voltage, which is usually 2 * 14 * 8 us = 224 us.
   // With the loading of the capacitor the current will sink, so we get a too high voltage at
   // the LowPin. The velocity of degration is inversely proportional to time constant (represented by capacity value).
   // Time constant for 1uF & 720 Ohm is 720us
// //   sumvolt0 -= (sumvolt0 * 150UL)  / cap_val_nF;	// Version 1.04k
       	ldi	r18, lo8(310);		/* sumvolt0 -= (sumvolt0 * 345UL)  / cap_val_nF; */
       	ldi	r19, hi8(310);
       	rjmp	ad_38dc 		/* } else { */
is_small:
       	ldi	r18, lo8(105);		/* sumvolt0 -= (sumvolt0 * 105UL)  / cap_val_nF; */
       	ldi	r19, hi8(105);
ad_38dc:
       	ldi	r20, 0x00;
       	ldi	r21, 0x00;
        ldd	r22, Y+sumvolt0
        ldd	r23, Y+sumvolt0+1
        ldd	r24, Y+sumvolt0+2
        ldd	r25, Y+sumvolt0+3
 	call	__mulsi3;		/* r22:25 = r22:25 * r18:21 */
       	movw	r18, r10;		/* cap_val_nF */
       	movw	r20, r12
 	call	__udivmodsi4;		/* r18:21 = r22:25  / r18:21 */
      	ldd	r10, Y+sumvolt0;
      	ldd	r11, Y+sumvolt0+1;
      	ldd	r12, Y+sumvolt0+2;
      	ldd	r13, Y+sumvolt0+3;
       	sub	r10, r18;		/* r10:13 == sumvolt0 -= */
       	sbc	r11, r19
       	sbc	r12, r20
       	sbc	r13, r21		/* } */
#ifdef ESR_DEBUG
	ldi	r24, 0x94;		/* lcd_line3(); */
	call	lcd_command;
      	ldd	r22, Y+sumvolt1;	/* DisplayValue(sumvolt1,0,'H',4); */
      	ldd	r23, Y+sumvolt1+1; 
      	ldd	r24, Y+sumvolt1+2; 0
      	ldi	r25, 0x00; 0
      	ldi	r20, 0;
      	ldi	r18, 'H';
      	ldi	r16, 4	;
	call	DisplayValue
        ldi	r24, ' '
        call	lcd_data
#endif

       	ldi	r24, lo8(EE_ESR_ZERO);	/* esr0 = (int8_t)eeprom_read_byte(&EE_ESR_ZERO); */
       	ldi	r25, hi8(EE_ESR_ZERO);
 	call	eeprom_read_byte;
       	mov	r2, r24
// //   sumvolt0 += (((long)sumvolt0 * esr0) / (RRpinMI * 10)); // subtract 0.23 Ohm from ESR, Vers. 1.04k
//    sumvolt2 += (((long)sumvolt0 * esr0) / (RRpinMI * 10)); // subtract 0.23 Ohm from ESR
       	mov	r22, r24
       	eor	r23, r23
       	sbrc	r22, 7
       	com	r23
       	mov	r24, r23
       	mov	r25, r23
       	movw	r18, r10;	/* sumvolt0 */
       	movw	r20, r12;
 	call	__mulsi3;		/* r22:25 = r22:25 * r18:21 */
#if RRpinMI == PIN_RM
	ldi	r18, lo8(RRpinMI*10)
	ldi	r19, hi8(RRpinMI*10)
#else
 	lds	r4, RRpinMI
 	lds	r5, RRpinMI+1
	add	r4,r4;		RRpinMI*2
	adc	r5,r5
	movw	r18,r4
	ldi	r30,4
ad_3924:
	add	r18,r4;		+ (2*RRpinMI)
	adc	r19,r5
	dec	r30
	brne	ad_3924
#endif
        movw	r4,r18;		/* r4:5 = 10 * RRpinMI */
       	ldi	r20, 0x00;
       	ldi	r21, 0x00;
 	call	__divmodsi4;		/* r18:21 = r22:25  / r18:21 */
       	ldd	r24, Y+sumvolt2	;
       	ldd	r25, Y+sumvolt2+1;
       	ldd	r26, Y+sumvolt2+2;
       	ldd	r27, Y+sumvolt2+3;
       	add	r18, r24		/* r18 == sumvolt2 += */
       	adc	r19, r25
       	adc	r20, r26
       	adc	r21, r27
       	std	Y+sumvolt2, r18;
       	std	Y+sumvolt2+1, r19;
       	std	Y+sumvolt2+2, r20;
       	std	Y+sumvolt2+3, r21;
#ifdef ESR_DEBUG
      	movw	r22, r10;		/* DisplayValue(sumvolt0,0,'C',4); */
      	movw	r24, r12; 
      	ldi	r20, 0;
      	ldi	r18, 'C';
      	ldi	r16, 4	;
	call	DisplayValue
#endif

       	ldd	r6, Y+sumvolt1;	/* if (sumvolt1 > sumvolt0) {  */
       	ldd	r7, Y+sumvolt1+1;
       	ldd	r8, Y+sumvolt1+2;
       	ldd	r9, Y+sumvolt1+3;
       	cp	r10, r6
       	cpc	r11, r7
       	cpc	r12, r8
       	cpc	r13, r9
       	brcc	ad_396c 
       	sub	r6, r10;		/* sumvolt1 -= sumvolt0;	// difference HighPin - LowPin Voltage with current */
       	sbc	r7, r11
       	sbc	r8, r12
       	sbc	r9, r13
       	rjmp	ad_3972;		/* } else { */
ad_396c:
       	eor	r6, r6;			/* sumvolt1 = 0; */
       	eor	r7, r7
       	movw	r8, r6
ad_3972:
#ifdef ESR_DEBUG
	ldi	r24, 0xd4;		/* lcd_line4(); */
	call	lcd_command;
      	movw	r22, r6;		/* DisplayValue(sumvolt1,0,'d',4); */
	movw	r24, r8
      	ldi	r20, 0;
      	ldi	r18, 'd';
      	ldi	r16, 4	;
	call	DisplayValue
        ldi	r24, ' '
        call	lcd_data
      	ldd	r22, Y+sumvolt2;		/* DisplayValue(sumvolt2,0,' ',4); */
      	ldd	r23, Y+sumvolt2+1;
      	ldd	r24, Y+sumvolt2+2;
      	ldd	r25, Y+sumvolt2+3;
      	ldi	r20, 0;
      	ldi	r18, ' ';
      	ldi	r16, 4	;
	call	DisplayValue
#endif
       	movw	r22, r4
       	ldi	r24, 0x00;
       	ldi	r25, 0x00;	/*  r22:25 = 10 * (unsigned long)RRpinMI)  */

					/* jj = 0; */
      	ldd	r14, Y+sumvolt2;	/* if (sumvolt1 >= sumvolt2) { */
      	ldd	r15, Y+sumvolt2+1;
      	ldd	r16, Y+sumvolt2+2;
      	ldd	r17, Y+sumvolt2+3;
       	cp	r6, r14;	/* r6:9 = sumvolt1 */
       	cpc	r7, r15
       	cpc	r8, r16
       	cpc	r9, r17
      	brcs	ad_39c0 
      // mean voltage at the capacitor is higher with current
      // sumvolt0 is the sum of voltages at LowPin, caused by output resistance of Port
      // RRpinMI is the port output resistance in 0.1 Ohm units.
      // we scale up the difference voltage with 10 to get 0.01 Ohm units of ESR
 		/* cap.esr = ((sumvolt1 - sumvolt2) * 10 * (unsigned long)RRpinMI) / sumvolt0; */
       	movw	r18, r6
       	movw	r20, r8
       	sub	r18, r14;	/* sumvolt1 - sumvolt2 */
       	sbc	r19, r15
       	sbc	r20, r16
       	sbc	r21, r17
 	call	__mulsi3;		/* r22:25 = r22:25 * r18:21 */
       	movw	r18, r10;	/* r10:13 = sumvolt0 */
       	movw	r20, r12
 	call	__udivmodsi4;		/* r18:21 = r22:25  / r18:21 */
 	sts	cap+esr, r18;
 	sts	cap+esr+1, r19;
       	movw	r22, r18;		/* DisplayValue(cap.esr,-2,LCD_CHAR_OMEGA,2); */
       	ldi	r24, 0x00; 0
       	ldi	r25, 0x00; 0
       	ldi	r20, -2	; 254
       	ldi	r18, LCD_CHAR_OMEGA;
       	ldi	r16, 2	;
 	call	DisplayValue
      	rjmp	ad_exit; 		/*   } else { */
ad_39c0:
 		/* jj = ((sumvolt2 - sumvolt1) * 10 * (unsigned long)RRpinMI) / sumvolt0; */
       	movw	r18, r14
       	movw	r20, r16
       	sub	r18, r6
       	sbc	r19, r7
       	sbc	r20, r8
       	sbc	r21, r9
 	call	__mulsi3;		/* r22:25 = r22:25 * r18:21 */
       	movw	r18, r10
       	movw	r20, r12
 	call	__udivmodsi4;		/* r18:21 = r22:25  / r18:21 */
       	mov	r17, r18
       	ldi	r24,'0';		/* lcd_data('0'); */
 	call	lcd_data
       	mov	r24, r17;		/* if ((jj < 100) && (jj > 0)) { */
      	subi	r24, 0x01; 1
       	cpi	r24, 0x63; 99
       	brcc	ad_exit; 
       	ldd	r19, Y+big_cap;		/* if (big_cap != 2) { */
       	cpi	r19, 0x02;	/*  (cap_val_nF > (50000)) */
       	brne	ad_3a0e 
       	ldi	r24,'?';		/* lcd_data('?');	// mark ESR zero correction */
 	call	lcd_data
       	mov	r22, r2;		/* esr0 -= jj;		// correct ESR_ZERO by negative resistance */
       	sub	r22, r17
       	ldi	r24, lo8(EE_ESR_ZERO);	/* eeprom_write_byte((uint8_t *)(&EE_ESR_ZERO), (int8_t)esr0); */
       	ldi	r25, hi8(EE_ESR_ZERO);	/* // fix new zero offset */
        call	eeprom_write_byte
       	rjmp	ad_exit;		/* } else { */
ad_3a0e:
      	ldi	r24,'!';		/* lcd_data('!');	// mark ESR zero without correction */
 	call	lcd_data
//  return;				/* } } } */
ad_exit:
#ifdef ADC_Sleep_Mode
       	out	_SFR_IO_ADDR(SMCR), zero_reg; 	/*  SMCR = 0 */
#endif
      	adiw	r28, 0x1a; 26
       	in	r0, _SFR_IO_ADDR(SREG); 63
       	cli
       	out	_SFR_IO_ADDR(SPH), r29; 62
       	out	_SFR_IO_ADDR(SREG), r0; 63
       	out	_SFR_IO_ADDR(SPL), r28; 61
       	pop	r28
       	pop	r29
       	pop	r17
       	pop	r16
       	pop	r15
       	pop	r14
       	pop	r13
       	pop	r12
       	pop	r11
       	pop	r10
       	pop	r9
       	pop	r8
       	pop	r7
       	pop	r6
       	pop	r5
       	pop	r4
       	pop	r3
       	pop	r2
       	ret
 .endfunc
