#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
#include <avr/io.h>
#include <avr/common.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "config.h"
#include "part_defs.h"

 .GLOBAL GetESR

/* MAX_CNT is the maximum loop counter for repetition of mesurement */
#define MAX_CNT 255

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
// #define big_cap 1

//  unsigned long sumvolt0,sumvolt1,sumvolt2;	//  3 sums of ADC readings
#define sumvolt0 2 /* r14-r17 */
#define sumvolt1 6 /* SP + 6:9 */
#define sumvolt2 10 /* SP + 10:13 */

//  uint8_t LoADC;		// used to switch Lowpin directly to GND or VCC
#define LoADC 14	/* SP + 14 */

//  uint8_t HiADC;		// used to switch Highpin directly to GND or VCC
#define HiADC 15	/* SP + 15 */

//  unsigned int adcv[4];		// array for 4 ADC readings
// first part adcv0 r2/r3
// first part adcv1 Y+16/17
#define adcvv1 16
// first part adcv2 Y+18/19
#define adcvv2 18

// unsigned long cap_val_nF;	// capacity in nF
#define cap_val_nF 20	/* SP + 20:23 */

#define LowUpCount 24   /* SP + 24 */
#define HighUpCount 25  /* SP + 25 */
#define LowTooHigh 26  /* SP + 26 */
#define HighTooHigh 26  /* SP + 27 */

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

 .extern EE_ESR_ZEROtab

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
				// With wdt_reset the timing can be fine adjusted.
				// The middle of current pulse should be at the SH time of ADC.
				// SH time of next ADC cycle is 20 us after last ADC ready.
				// Charging of capacitor begins with negative voltage and
				// should be zero at SH time with a zero ESR capacitor.
 .macro DelayBigCap
 	call	wait20us;			// SH at 2.5 ADC clocks behind start = 20 us
#ifdef ADC_Sleep_Mode
 /* Wake up from sleep with Interrupt: 1+4+4T, jmp, rti, ldi, out takes 18 clock tics,  */
 /* 3+1 clock tics (sts,out) are needed for instructions before the current is switched off. */
 #if F_CPU == 8000000UL
         /* current begin after last ADCready:  2.25us (wakeup) + 5us + 5tics = 7.875us  */
	/* half puls length = (20us - 7.875us) = 12.125us (PL=24.25us) */
	call	wait3us; /* pulse length 23us + 2 clock tics (CALL instead of RCALL) = 23.25 us @ 8 MHz */ 
	wdr	;
 #endif
 #if F_CPU == 16000000UL
        /* current begin after last ADCready:  1.125us (wakeup) + 5us + 5tics = 6.4375us  */
	/* half puls length = (20us - 6.4375us) = 13.5625us (PL=27.125us) */
 	call	wait5us
	call	wait1us; /* pulse length 26us + 3 clock tics (CALL instead of RCALL) = 26.1875 us */
	push	r24;		/* 26.3125 us */
	pop	r24;            /* 26.4375 us */
	push	r24;		/* 26.5625 us */
	pop	r24;            /* 26.6875 us */
 #endif


#else
 /* Polling mode: lds,sbrc,sts and out Instructions are 7 clock tics */
 #if F_CPU == 8000000UL
        /* current begin after last ADCready:  0.875us (loop) + 5us + 5tics = 6.5us  */
	/* half puls length = (20us - 6.5us) = 13.5us  (PL=27us) */
 	call	wait5us
	call	wait1us;
         /* pulse length 26us + 3 clock tics (CALL instead of RCALL) = 26.375 us @ 8 MHz */
	push	r24;		/* 26.625 us */
	pop	r24;            /* 26.875 us */
	wdr	;
	wdr	;		/* 27.125 us */
 #endif
 #if F_CPU == 16000000UL
        /* current begin after last ADCready:  0.4375us (loop) + 5us + 5tics = 5.75us  */
	/* half puls length = (20us - 5.75us) = 14.25us (PL=28.5us) */
 	call	wait5us
 	call	wait3us;
        /* pulse length 28us + 3 clock tics (CALL instead of RCALL) = 28.1875 us */
	push	r24;		/* 28.3125 us */
	pop	r24;            /* 28.4375 us */
	wdr	;		/* 28.50   us */
 #endif
#endif
 .endm

//=================================================================


//=================================================================
//void GetESR(uint8_t hipin, uint8_t lopin) {
.text
GetESR:
       	push	r2;
       	push	r3;
       	push	r4;
       	push	r5;
       	push	r6;
       	push	r7;
       	push	r8;
       	push	r9;
       	push	r10;
       	push	r11;
       	push	r12;
       	push	r13;
       	push	r14;
       	push	r15;
       	push	r16;
       	push	r17;
       	push	r29;
       	push	r28;
       	in	r28, _SFR_IO_ADDR(SPL);
       	in	r29, _SFR_IO_ADDR(SPH);
       	sbiw	r28, 0x1e;	/* 30 */
       	in	r0, _SFR_IO_ADDR(SREG);
       	cli
       	out	_SFR_IO_ADDR(SPH), r29;
       	out	_SFR_IO_ADDR(SREG), r0;
	out	_SFR_IO_ADDR(SPL), r28;


	mov	SelectLowPin, r22;
	mov	SelectHighPin, r24;
        add	r24, r22;
	std	Y+1, r24;

	lds	r18, PartFound;		/* if (PartFound == PART_CAPACITOR) { */
	cpi	r18, PART_CAPACITOR;	
 	brne	ad_35e4;
	lds	r18, cap+cval_max;      /* cap_val_nF = cap.cval_max; */
	lds	r19, cap+cval_max+1;
	lds	r20, cap+cval_max+2;
	lds	r21, cap+cval_max+3;
	lds	r17, cap+cpre_max;      /* prefix = cap.cpre_max; */
	rjmp	ad_35ba;

ad_35ac:
	movw	r24, r20;               /* cval /= 10;          // reduce value by factor ten */
	movw	r22, r18
	ldi	r18, 0x0A; 10
	mov	r19, zero_reg
	mov	r20, zero_reg
	mov	r21, zero_reg
	call	__udivmodsi4;           /* r18:21 = r22:25  / r18:21 */
	subi	r17, 0xFF;              /* prefix++;            // take next decimal prefix */

ad_35ba:
	cpi	r17, -9;                /* while (prefix < -9) { // set cval to nF unit */
	brlt	ad_35ac;                /*  } */
	std	Y+cap_val_nF, r18
	std	Y+cap_val_nF+1, r19
	std	Y+cap_val_nF+2, r20
	std	Y+cap_val_nF+3, r21


	cpi	r18, lo8(1800/10);       /* if (cap_val_nF < (1800/10)) return(0xffff);   //capacity lower than 0.18 uF */
	ldi	r22, hi8(1800/10)
	cpc	r19, r22
	cpc	r20, zero_reg
	cpc	r21, zero_reg
	brcc	ad_35e4
	ldi	r24, 0xff;
	ldi	r25, 0xff;
	rjmp	ad_exit; 
ad_35e4:				/* } */


#ifdef ADC_Sleep_Mode
	ldi	r24, (1 << SM0) | (1 << SE);
	out	_SFR_IO_ADDR(SMCR), r24; 	/*  SMCR = (1 << SM0) | (1 << SE); */
     /* normal ADC-speed, ADC-Clock 8us */
	ldi	r25, (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */
	mov	StartADCmsk, r25;
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#else
	ldi	r18, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable and start ADC */
	mov	StartADCmsk, r18;
#endif
	rjmp	ad_3604;		/* } else { */
ad_35fe:
     /* fast ADC-speed, ADC-Clock 2us */
#ifdef ADC_Sleep_Mode
	ldi	r25, (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | FAST_CLOCK_DIV; /* enable ADC and Interrupt */
	mov	StartADCmsk, r25;
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#else
	ldi	r25, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; /* enable and start ADC */
	mov	StartADCmsk, r25;
#endif
					/*  }  */

ad_3604:
	ldi	r30, lo8(PinADCtab); 	/* LoADC = pgm_read_byte(&PinADCtab[cap.ca]) | TXD_MSK; */
	ldi	r31, hi8(PinADCtab);
	add	r30, SelectLowPin;
	adc	r31, zero_reg;
	lpm	r24, Z+;
	ori	r24, TXD_MSK;
	std	Y+LoADC, r24;
	ldi	r30, lo8(PinADCtab); 	/* HiADC = pgm_read_byte(&PinADCtab[cap.cb]) | TXD_MSK; */
	ldi	r31, hi8(PinADCtab);
	add	r30, SelectHighPin;
	adc	r31, zero_reg;
	lpm	r24, Z+;
	ori	r24, TXD_MSK;
	std	Y+HiADC, r24;
	ldi	r30, lo8(PinRLtab);	/* LoPinR_L = pgm_read_byte(&PinRLtab[cap.ca]);  //R_L mask for LowPin R_L load */
	ldi	r31, hi8(PinRLtab);
	add	r30, SelectLowPin;
	adc	r31, zero_reg;
	lpm	LoPinR_L, Z+;
	ldi	r30, lo8(PinRLtab);	/* HiPinR_L = pgm_read_byte(&PinRLtab[cap.cb]);	//R_L mask for HighPin R_L load */
	ldi	r31, hi8(PinRLtab);
	add	r30, SelectHighPin;
	adc	r31, zero_reg;
	lpm	HiPinR_L, Z+;


#if PROCESSOR_TYP == 1280
	    /* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
	//  SelectLowPin = (cap.ca | (1<<REFS1) | (0<<REFS0));	// switch ADC to LowPin, Internal Ref. 
	ldi	r25, (1<<REFS1)|(0<<REFS0);	0xC0
	or	SelectLowPin, r25;
	//  SelectHighPin = (cap.cb | (1<<REFS1) | (0<<REFS0));	// switch ADC to HighPin, Internal Ref. 
	or	SelectHighPin, r25;
#else
	//  SelectLowPin = (cap.ca | (1<<REFS1) | (1<<REFS0));	// switch ADC to LowPin, Internal Ref. 
	ldi	r25, (1<<REFS1)|(1<<REFS0);	0xC0
	or	SelectLowPin, r25;
	//  SelectHighPin = (cap.cb | (1<<REFS1) | (1<<REFS0));	// switch ADC to HighPin, Internal Ref. 
	or	SelectHighPin, r25;
#endif


	// Measurement of ESR of capacitors AC Mode
	ldi	r24, 0x01; 	/* sumvolt0 = 1;	// set sum of LowPin voltage to 1 to prevent divide by zero */
	mov	r14, r24;
	mov	r15, zero_reg;
	mov	r16, zero_reg;
	mov	r17, zero_reg;
	std	Y+sumvolt1, r24;	/* sumvolt1 = 1;	// clear sum of HighPin voltage with current */
	//                                // offset is about (x*10*200)/34000 in 0.01 Ohm units
	std	Y+sumvolt1+1, zero_reg;
	std	Y+sumvolt1+2, zero_reg;
	std	Y+sumvolt1+3, zero_reg;
	std	Y+sumvolt2, zero_reg;	/* sumvolt2 = 0;	// clear sum of HighPin voltage without current */
	std	Y+sumvolt2+1, zero_reg;
	std	Y+sumvolt2+2, zero_reg;
	std	Y+sumvolt2+3, zero_reg;
	std	Y+LowUpCount, zero_reg;
	std	Y+HighUpCount, zero_reg;
	call	EntladePins;	/* EntladePins();	// discharge capacitor */
	ldi	r24, TXD_VAL;
	out	_SFR_IO_ADDR(ADC_PORT), r24; /* ADC_PORT = TXD_VAL;	// switch ADC-Port to GND */
	sts	ADMUX, SelectLowPin;	/* ADMUX = SelectLowPin;	// set Mux input and Voltage Reference to internal 1.1V */
#ifdef NO_AREF_CAP
	call	wait100us;			/* time for voltage stabilization */
#else
	call	wait10ms;    		/* time for voltage stabilization with 100nF */
#endif
	/* start voltage should be negativ */
	ldd	r19, Y+HiADC;		/* ADC_DDR = HiADC;	// switch High Pin to GND */
	out	_SFR_IO_ADDR(ADC_DDR), r19; 
	out	_SFR_IO_ADDR(R_PORT), LoPinR_L;	/* R_PORT = LoPinR_L */
	out	_SFR_IO_ADDR(R_DDR), LoPinR_L;	/* R_DDR = LoPinR_L */
	call	wait10us;	/* Delay about half the current puls-length of measurement loop */
#ifdef ADC_Sleep_Mode
 #if F_CPU == 8000000UL
	call	wait1us;	/* shorter pulse-length */
 #else
	call	wait2us;
 #endif
#else
	call	wait2us;
#endif
	out	_SFR_IO_ADDR(R_PORT), zero_reg;	/* R_PORT = 0 */
	out	_SFR_IO_ADDR(R_DDR), zero_reg;	/* R_DDR = 0 */

	   // Measurement frequency is given by sum of ADC-Reads < 1116 Hz for normal ADC speed.
	   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after real start of conversion.
	   // Real ADC-conversion is started with the next ADC-Clock (125kHz) after setting the ADSC bit.
	eor	r13, r13;			/* for(ii=0;ii<MAX_CNT;ii++) { */
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
	ldi	r27, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV;	/* enable ADC and start with ADSC */
	mov	r9, r27
	//         adcv[0] = ADCW;		// Voltage LowPin with current
	//         ADMUX = SelectHighPin;
	ldi	r26, (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV;	/* enable ADC and start with ADSC */
	mov	r8, r26
	      /* ********* Forward direction, connect Low side with GND *********** */
ad_3692:
	ldd	r19, Y+LoADC;
	out	_SFR_IO_ADDR(ADC_DDR), r19;	/* ADC_DDR = LoADC;	// switch Low-Pin to output (GND) */
	out	_SFR_IO_ADDR(R_PORT), LoPinR_L;	/* R_PORT = LoPinR_L */
	out	_SFR_IO_ADDR(R_DDR), LoPinR_L;	/* R_DDR = LoPinR_L */
	sts	ADMUX, SelectLowPin;		/* ADMUX = SelectLowPin; */

	wdr	;				/* wdt_reset(); */
	StartADCwait			/* start ADC and wait */
	StartADCwait			/* start ADC and wait */
	lds	adcv0L, ADCW;			/* adcv[0] = ADCW;	// Voltage LowPin reference */
	lds	adcv0H, ADCW+1;
	sts	ADMUX, SelectHighPin;		/* ADMUX = SelectHighPin; */

	StartADCwait			/* start ADC and wait */
			// Start Conversion, real start is next rising edge of ADC clock
	sts	ADCSRA, r8;		/* ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start */
	call	wait5us;
	out	_SFR_IO_ADDR(R_PORT), HiPinR_L;	/* R_PORT = HiPinR_L;	// switch R-Port to VCC */
	out	_SFR_IO_ADDR(R_DDR), HiPinR_L;	/* R_DDR = HiPinR_L;	// switch R_L port for HighPin to output (VCC) */
	DelayBigCap;			/* wait the time defined by macro */
	out	_SFR_IO_ADDR(R_DDR), zero_reg;	/* R_DDR = 0; // switch current off,  SH is 1.5 ADC clock behind real start */
	out	_SFR_IO_ADDR(R_PORT), zero_reg;	/* R_PORT = 0; */
ad_370c:
	lds	r24, ADCSRA;		/* while (ADCSRA&(1<<ADSC));	// wait for conversion finished */
	sbrc	r24, ADSC;
	rjmp	ad_370c ;

	lds	r18, ADCW;		/* adcv[1] = ADCW;	// Voltage HighPin with current */
	lds	r19, ADCW+1;

#ifdef ADC_Sleep_Mode
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#endif

;=======

	//      sumvolt0 += adcv[0];	// add sum of both LowPin voltages with current
	//      adcv0 = r2/r3
	//      sumvolt1 += adcv[1];	// add  HighPin voltages with current
	//      adcv1 = r18/r19
	std	Y+adcvv1, r18;
	std	Y+adcvv1+1, r19;

	      /* ********* Reverse direction, connect High side with GND *********** */
	ldd	r19, Y+HiADC;		/* ADC_DDR = HiADC;	// switch High Pin to GND */
	out	_SFR_IO_ADDR(ADC_DDR), r19; 
	out	_SFR_IO_ADDR(R_PORT), HiPinR_L;	/* R_PORT = HiPinR_L;	// switch R-Port to VCC */
	out	_SFR_IO_ADDR(R_DDR), HiPinR_L;	/* R_DDR = HiPinR_L;	// switch R_L port for HighPin to output (VCC) */

	wdr	;			/* wdt_reset(); */
	sts	ADMUX, SelectHighPin;	/* ADMUX = SelectHighPin; */

	StartADCwait		/* start ADC and wait */
	StartADCwait		/* start ADC and wait */

	lds	r22, ADCW;         	/* adcv[2] = ADCW;	// Reverse Reference Voltage HighPin  */
	lds	r23, ADCW+1;
	sts	ADMUX, SelectLowPin;	/*  ADMUX = SelectLowPin; */
	// ****** Polling mode big cap
	StartADCwait		/* start ADC and wait */
	sts	ADCSRA, r8;	/* ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start with ADSC */
	call	wait5us;
	out	_SFR_IO_ADDR(R_PORT), LoPinR_L;	/* R_PORT = LoPinR_L; */
	out	_SFR_IO_ADDR(R_DDR), LoPinR_L;	/* R_DDR = LoPinR_L;	// switch LowPin with 680 Ohm to VCC */

	DelayBigCap;			/* wait the time defined by macro */
					/*  }  */
	out	_SFR_IO_ADDR(R_DDR), zero_reg; 	// switch current off, SH is 1.5 ADC clock ticks behind real start
	out	_SFR_IO_ADDR(R_PORT), zero_reg; 
ad_37f4:
	lds	r24, ADCSRA;		/* while (ADCSRA&(1<<ADSC));	// wait for conversion finished */
	sbrc	r24, ADSC;
	rjmp	ad_37f4 ;

	lds	r20, ADCW;		/* adcv[3] = ADCW;	//  Voltage LowPin with current */
	lds	r21, ADCW+1;
#ifdef ADC_Sleep_Mode
	sts	ADCSRA, StartADCmsk; 	/*  ADCSRA = StartADCmsk;	// enable ADC and Interrupt */
#endif

	out	_SFR_IO_ADDR(R_DDR), zero_reg; 	/* R_DDR = 0; // switch current off */

	movw	r24, r22;		/*  adcv[2] */
	add	r24, adcv0L;		/* adcv[0] + adcv[2] // add sum of both LowPin voltages with current */
	adc	r25, adcv0H;

	add	r14, r24;		/* r14:17 = sumvolt0 += (adcv[0] + adcv[2]); */
	adc	r15, r25;
	adc	r16, zero_reg;
	adc	r17, zero_reg;
	std	Y+sumvolt0, r14;
	std	Y+sumvolt0+1, r15;
	std	Y+sumvolt0+2, r16;
	std	Y+sumvolt0+3, r17;
	ldd	r24, Y+adcvv1;		/* add HighPin voltages with current */
	ldd	r25, Y+adcvv1+1; 
	add	r24, r20;		/* adcv[1] + adcv[3] */
	adc	r25, r21;
	ldd	r18, Y+sumvolt1;	/* sumvolt1 += (adcv[1] + adcv[3]); */
	ldd	r19, Y+sumvolt1+1;
	ldd	r22, Y+sumvolt1+2;
	ldd	r23, Y+sumvolt1+3;
	add	r18, r24;
	adc	r19, r25;
	adc	r22, zero_reg;
	adc	r23, zero_reg;
	std	Y+sumvolt1, r18;
	std	Y+sumvolt1+1, r19;
	std	Y+sumvolt1+2, r22;
	std	Y+sumvolt1+3, r23;

  	/* Y+adcvv1 is still the voltage of forward direction, r20:21 the voltage of reverse direction */
	ldi	r18, lo8(100);
	cp	r18, r20;
	cpc	zero_reg, r21;
	brcs	is_ok1;		/* r20:21 >= 100 */
	out	_SFR_IO_ADDR(R_PORT), LoPinR_L;	/* R_PORT = LoPinR_L; */
	out	_SFR_IO_ADDR(R_DDR), LoPinR_L;	/* R_DDR = LoPinR_L;	// switch LowPin with 680 Ohm to VCC */
	call	wait2us;			/* additional charge the capacitor */
	out	_SFR_IO_ADDR(R_DDR), zero_reg; 	// switch current off
	out	_SFR_IO_ADDR(R_PORT), zero_reg; 
	ldd	r24, Y+LowUpCount; 	/* count additional load pulses at Low side */
	inc	r24;
	std	Y+LowUpCount, r24;
	rjmp	is_ok1b;
is_ok1:
	cpi	r20, lo8(1000);
        ldi	r23, hi8(1000);
	cpc	r21, r23;
        brcs	is_ok1b;			/* voltage reverse direction < 1000 */
	ldd	r24, Y+LowTooHigh; 	/* count  pulses with too high voltage at Low side */
	inc	r24;
	std	Y+LowTooHigh, r24;
is_ok1b:
	ldd	r24, Y+adcvv1;
	ldd	r25, Y+adcvv1+1;
	cp	r18, r24;
	cpc	zero_reg, r25;	/* adcvv1 >= 100 */
	brcs	is_ok2;
	ldd	r19, Y+LoADC;
	out	_SFR_IO_ADDR(ADC_DDR), r19;	/* ADC_DDR = LoADC;	// switch Low-Pin to output (GND) */
	out	_SFR_IO_ADDR(R_PORT), HiPinR_L;	/* R_PORT = HiPinR_L;	// switch R-Port to VCC */
	out	_SFR_IO_ADDR(R_DDR), HiPinR_L;	/* R_DDR = HiPinR_L;	// switch R_L port for HighPin to output (VCC) */
	call	wait2us;			/* additional charge the capacitor */
	DelayBigCap;			/* wait the time defined by macro */
	out	_SFR_IO_ADDR(R_DDR), zero_reg;	/* R_DDR = 0; // switch current off,  SH is 1.5 ADC clock behind real start */
	out	_SFR_IO_ADDR(R_PORT), zero_reg;	/* R_PORT = 0; */
	ldd	r24, Y+HighUpCount;	/* count additional load pulses at High side */
	inc	r24;
	std	Y+HighUpCount, r24;
	rjmp	is_ok2b;
is_ok2:
	cpi	r24, lo8(1000);
        ldi	r23, hi8(1000);
	cpc	r25, r23;
        brcs	is_ok2b;			/* voltage forward direction < 1000 */
	ldd	r24, Y+HighTooHigh; 	/* count  pulses with too high voltage at High side */
	inc	r24;
	std	Y+HighTooHigh, r24;
is_ok2b:
	inc	r13;			/* for(    ;ii<MAX_CNT;ii++)  */
	mov	r21, r13;
	cpi	r21, MAX_CNT;
	breq	ad_38ac;
	rjmp	ad_3692;		/* } // end for */
ad_38ac:

#if RRpinMI == PIN_RM
	ldi	r18, lo8(RRpinMI*10);
	ldi	r19, hi8(RRpinMI*10);
#else
	lds	r4, RRpinMI;
	lds	r5, RRpinMI+1;
	add	r4, r4;		RRpinMI*2
	adc	r5, r5;
	movw	r18, r4;
	ldi	r30, 4;
ad_3924:
	add	r18, r4;		+ (2*RRpinMI)
	adc	r19, r5;
	dec	r30;
	brne	ad_3924;
#endif
	movw	r4, r18;		/* r4:5 = 10 * RRpinMI */
	movw	r10, r14;		/* r10:13 = r14:17 = sumvolt0 */
	movw	r12, r16;

	ldd	r6, Y+sumvolt1;
	ldd	r7, Y+sumvolt1+1;
	ldd	r8, Y+sumvolt1+2;
	ldd	r9, Y+sumvolt1+3;
/* ############################################################ */
	lds	r18, PartFound;		/* if (PartFound == PART_CAPACITOR) { */
	cpi	r18, PART_CAPACITOR;	
 	brne	no_sub;			/* it is not a capacitor */

/* First half of load pulse (13.5us) loads quicker than the second half of load pulse. */
/* Aproximation of 5000*(1 - exp(13.5e-6/(cap_val_nF*1.e-9*(0.1*(PIN_RM+PIN_RP+R_L_VAL)))) - 2500*(1 - exp(-27e-6/(cap_val_nF*1.e-9*(0.1*(PIN_RM+PIN_RP+R_L_VAL))))) */
/*  is done by ((9537620/(PIN_RM+PIN_RP+R_L_VAL))*(9537620/(PIN_RM+PIN_RP+R_L_VAL))) / (cap_val_nF * (cap_val_nF + (137180/(PIN_RM+PIN_RP+R_L_VAL)))) */
/*  is done by 4450.e5 / (cap_val_nF * (cap_val_nF + (137180/(PIN_RM+PIN_RP+R_L_VAL))) */
 #define FAKTOR_ESR (9537620/(PIN_RM+PIN_RP+R_L_VAL))

	ldd	r22, Y+cap_val_nF;	/* sumvolt1 -= (1745098UL*MAX_CNT) / (cap_val_nF * (cap_val_nF + 19)); */
	ldd	r23, Y+cap_val_nF+1;
	ldd	r24, Y+cap_val_nF+2;
	ldd	r25, Y+cap_val_nF+3;
	movw	r18, r22;		/* r18:21 = r22:25 = cap_val_nF; */
	movw	r20, r24;
       	subi	r22, lo8(-137180/(PIN_RM+PIN_RP+R_L_VAL)); 0xED; 237
       	sbci	r23, hi8(-137180/(PIN_RM+PIN_RP+R_L_VAL)); 0xFF; 255
       	sbci	r24, hlo8(-137180/(PIN_RM+PIN_RP+R_L_VAL)); 0xFF; 255
       	sbci	r25, hhi8(-137180/(PIN_RM+PIN_RP+R_L_VAL)); 0xFF; 255
	call	__mulsi3;		/* cap_val_nF * (cap_val_nF + 19) */
       	movw	r18, r22;
       	movw	r20, r24;
       	ldi	r22, lo8(FAKTOR_ESR*FAKTOR_ESR*MAX_CNT); 0x36; 54
       	ldi	r23, hi8(FAKTOR_ESR*FAKTOR_ESR*MAX_CNT); 0x29; 41
       	ldi	r24, hlo8(FAKTOR_ESR*FAKTOR_ESR*MAX_CNT); 0x86; 134
       	ldi	r25, hhi8(FAKTOR_ESR*FAKTOR_ESR*MAX_CNT); 0x1A; 26
	call	__udivmodsi4;
	sub	r6, r18
	sbc	r7, r19
	sbc	r8, r20
	sbc	r9, r21
no_sub:				/* } */
/* ############################################################ */

	cp	r10, r6;		/* if (sumvolt1 > sumvolt0) {  */
	cpc	r11, r7;
	cpc	r12, r8;
	cpc	r13, r9;
	brcc	ad_396c;
	sub	r6, r10;		/* sumvolt1 -= sumvolt0;	// difference HighPin - LowPin Voltage with current */
	sbc	r7, r11;
	sbc	r8, r12;
	sbc	r9, r13;
	rjmp	ad_3972;		/* } else { */
ad_396c:
	eor	r6, r6;			/* sumvolt1 = 0; */
	eor	r7, r7
	movw	r8, r6
ad_3972:
#ifdef ESR_DEBUG
	movw	r22, r6;		/* DisplayValue(sumvolt1,0,'d',4); */
	movw	r24, r8
	ldi	r20, 0;
	ldi	r18, 'd';
	ldi	r16, 4	;
	call	DisplayValue;
	ldi	r24, 0x94;		/* lcd_line3(); */
	call	lcd_command;
        ldd	r22, Y+LowUpCount;
	ldi	r23, 0;
	ldi	r24, 0;
	ldi	r25, 0;
	ldi	r20, 0;
	ldi	r18, '<';
	ldi	r16, 4	;
	call	DisplayValue;
        ldd	r22, Y+HighUpCount;
	ldi	r23, 0;
	ldi	r24, 0;
	ldi	r25, 0;
	ldi	r20, 0;
	ldi	r18, '>';
	ldi	r16, 4	;
	call	DisplayValue;
        ldd	r22, Y+LowTooHigh;
	ldi	r23, 0;
	ldi	r24, 0;
	ldi	r25, 0;
	ldi	r20, 0;
	ldi	r18, '+';
	ldi	r16, 4	;
	call	DisplayValue;
        ldd	r22, Y+HighTooHigh;
	ldi	r23, 0;
	ldi	r24, 0;
	ldi	r25, 0;
	ldi	r20, 0;
	ldi	r18, '#';
	ldi	r16, 4	;
	call	DisplayValue;
#endif
	movw	r22, r4
	ldi	r24, 0x00;
	ldi	r25, 0x00;	/*  r22:25 = 10 * (unsigned long)RRpinMI)  */

					/* jj = 0; */
	      // mean voltage at the capacitor is higher with current
	      // sumvolt0 is the sum of voltages at LowPin, caused by output resistance of Port
	      // RRpinMI is the port output resistance in 0.1 Ohm units.
	      // we scale up the difference voltage with 10 to get 0.01 Ohm units of ESR
		/* esrvalue = (sumvolt1 * 10 * (unsigned long)RRpinMI) / sumvolt0; */
	movw	r18, r6;		/* r18:21 = r6:9 = sumvolt1 */
	movw	r20, r8;
	call	__mulsi3;		/* r22:25 = r22:25 * r18:21 */
	movw	r18, r10;	/* r18:21 = r10:13 = sumvolt0 */
	movw	r20, r12;
	call	__udivmodsi4;		/* r18:21 = r22:25  / r18:21 */
	ldi	r24, lo8(EE_ESR_ZEROtab);	/* esr0 = (int8_t)eeprom_read_byte(&EE_ESR_ZEROtab[lopin+hipin]); */
	ldi	r25, hi8(EE_ESR_ZEROtab);
        ldd	r23, Y+1;
        add	r24, r23;
	adc	r25, zero_reg;
	call	eeprom_read_byte;
	mov	r6, r24;
	movw	r24,r18;	/* r24:25 = r18:19 = esrvalue */
	ldi	r22, 16;
	ldi	r23, 0;
	call	__udivmodhi4	/* r22:23 = r24:25 / r22:23 */
	add	r18, r22;	/* esrvalue += esrvalue / 16; */
	adc	r19, r23;
	movw	r24,r18;	/* esrvalue */
	cp	r6, r24;		/* if (esrvalue > esr0) esrvalue -= esr0; */
	cpc	zero_reg, r25;
	brcc	esr_too_less;
	sub	r24, r6;		/* - esr0 */ 
	sbc	r25, zero_reg;
	rjmp	ad_exit;
esr_too_less:
#ifdef AUTO_CAL
        subi	r24, lo8(-R_LIMIT_TO_UNCALIBRATED);		/* + 0.20 Ohm */
        sbci	r25, hi8(-R_LIMIT_TO_UNCALIBRATED);		/* esrvalue + 20 */
	cp	r24, r6;		/* if ((esrvalue+20) < esr0) ; */
	cpc	r25, zero_reg;
	brcc	esr_too_less2;
	call	mark_as_uncalibrated;
/*	ldi	r24,'<'; */
/*	call	lcd_data; */
esr_too_less2:
#endif
	mov	r24, zero_reg;
	mov	r25, zero_reg;
	
ad_exit:
#ifdef ADC_Sleep_Mode
	out	_SFR_IO_ADDR(SMCR), zero_reg; 	/*  SMCR = 0 */
#endif
	adiw	r28, 0x1e;   /* 30 */
	in	r0, _SFR_IO_ADDR(SREG); 63
	cli
	out	_SFR_IO_ADDR(SPH), r29; 62
	out	_SFR_IO_ADDR(SREG), r0; 63
	out	_SFR_IO_ADDR(SPL), r28; 61
	pop	r28;
	pop	r29;
	pop	r17;
	pop	r16;
	pop	r15;
	pop	r14;
	pop	r13;
	pop	r12;
	pop	r11;
	pop	r10;
	pop	r9;
	pop	r8;
	pop	r7;
	pop	r6;
	pop	r5;
	pop	r4;
	pop	r3;
	pop	r2;
	ret;
