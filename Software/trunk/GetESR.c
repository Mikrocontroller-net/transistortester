#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

#define MAX_CNT 255

/* The sleep mode for ADC can be used. It is implemented for 8MHz and 16MHz operation */
/* But the ESR result is allways higher than the results with wait mode. */
/* The time of ESR measurement is higher with the sleep mode (checked with oszilloscope) */
/* The reason for the different time is unknown, the start of the next ADC measurement */
/* should be initiated before the next ADC-clock (8 us). One ADC takes 13 ADC clock + 1 clock setup. */
/* The setting to sleep mode takes 10 clock tics, the wakeup takes about 24 clock tics, but 8us are 64 clock tics. */
/* I have found no reason, why a reset of the ADC clock divider should occur during ESR measurement. */ 
//#define ADC_Sleep_Mode

#define ESR_DEBUG

#ifdef ADC_Sleep_Mode
//  #define StartADCwait() ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */
//  #define StartADCwait() set_sleep_mode(SLEEP_MODE_ADC);
//    sleep_mode()		/* Start ADC, return if ADC has finished */
    #define StartADCwait() sleep_cpu()
#else
//  #define StartADCwait() ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable ADC and start */
    #define StartADCwait() ADCSRA = StartADCmsk; /* Start conversion */\
    while (ADCSRA & (1 << ADSC))  /* wait until conversion is done */
#endif

/************************************************************************/
/* Predefine the wait time for switch off the load current for big caps */
/************************************************************************/
//         wdt_reset();		// with wdt_reset the timing can be adjusted,
				// when time is too short, voltage is down before SH of ADC
				// when time is too long, capacitor will be overloaded.
				// That will cause too high voltage without current.
#ifdef ADC_Sleep_Mode
            /* Interrupt mode, big cap */
 #if F_CPU == 8000000UL
    #define DelayBigCap() wait30us();	/* 2.5 ADC clocks = 20us */ \
           wait1us();		/*  */ \
           wait1us();	/* with only 17 us delay the voltage goes down before SH */ \
            /* delay 17us + 3 clock tics (CALL instead of RCALL) = 17.375 us @ 8 MHz */ \
            /* + 21 clock tics delay from interrupt return, +2.625us = 20.0 */	\
            wdt_reset();	/* 20.125 us  */ \
            wdt_reset()		/* 20.25  us  */
 #endif
 #if F_CPU == 16000000UL
    #define DelayBigCap() wait10us();	/* 2.5 ADC clocks = 20us */ \
           wait5us();		/*  */ \
           wait3us();	/* with only 18 us delay the voltage goes down before SH */ \
            /* delay 18us + 3 clock tics (CALL instead of RCALL) = 18.1875 us */ \
            /* + 21 clock tics delay from interrupt return, +1.3125us = 19.5 */ \
            wait500ns(); 	/* 20.0626us   */ \
            wdt_reset();	/* 20.125 us  */ \
            wdt_reset();	/* 20.1875us  */ \
            wdt_reset()		/* 20.25  us  */
 #endif
#else
            /* Polling mode, big cap */
 #if F_CPU == 8000000UL
    #define DelayBigCap() wait20us();	/* 2.5 ADC clocks = 20us */ \
           wait5us();		/*  */ \
           wait2us();	/* pulse length 27.375 us */ 
//	   wdt_reset(); 
            /* delay 19us + 3 clock tics (CALL instead of RCALL) = 19.375 us @ 8 MHz */ 
            /* + 7 clock tics delay from while loop, +0.875us  = 20.25 */ 
 //             wdt_reset()	/* 20.375 us + */
 #endif
 #if F_CPU == 16000000UL
    #define DelayBigCap() wait10us();	/* 2.5 ADC clocks = 20us */ \
           wait5us();		/*  */ \
           wait4us();	/* with only 18 us delay the voltage goes down before SH */ \
         /* delay 19us + 3 clock tics (CALL instead of RCALL) = 19.1875 us */ \
         /* + 7 clock tics delay from "while (ADCSRA&(1<<ADSC))" loop = 19.625 */ \
            wait500ns(); 	/* 20.1875us   */ 
//            wdt_reset();	/* 20.250 us + */ 
//            wdt_reset()		/* 20.3125us + */
 #endif
#endif

/************************************************************************/
/* Predefine the wait time for switch off the load current for small caps */
/************************************************************************/
      	/* SH at 2.5 ADC clocks behind start = 5 us */
#ifdef ADC_Sleep_Mode
            /* Interrupt mode, small cap */
 #if F_CPU == 8000000UL
    #define DelaySmallCap() wait2us();	/* with only 4 us delay the voltage goes down before SH */ \
            /* delay 2us + 1 clock tics (CALL instead of RCALL) = 2.125 us @ 8 MHz */ \
            /* + 21 clock tics delay from interrupt return, +2.625us = 4.75 */ \
            wdt_reset();	/* 4.8750 us  */ \
            wdt_reset();	/* 5.0    us  */ \
            wdt_reset()		/* 5.125  us  */
 #endif
 #if F_CPU == 16000000UL
    #define DelaySmallCap() wait3us();	/* with only 18 us delay the voltage goes down before SH */ \
            /* delay 3us + 1 clock tics (CALL instead of RCALL) = 3.0625 us */ \
            /* + 21 clock tics delay from interrupt return, +1.3125us = 4.375 */ \
            wdt_reset();	/* 4.4375 us  */ \
            wait500ns(); 	/* 5.0    us  */ \
            wdt_reset();	/* 5.0625 us  */ \
            wdt_reset()		/* 5.1250 us  */ 
 #endif
#else
            /* Polling mode, small cap */
 #if F_CPU == 8000000UL
    #define DelaySmallCap() wait5us();	/* with only 4 us delay the voltage goes down before SH */ \
            /* delay 4us + 1 clock tics (CALL instead of RCALL) = 4.125 us @ 8 MHz */ \
            /* + 7 clock tics delay from while loop, +0.875us  = 5.000 */ 
//            wdt_reset()		/* 5.125 us   */
 #endif
 #if F_CPU == 16000000UL
    #define DelaySmallCap() wait4us();	/* with only 4 us delay the voltage goes down before SH */ \
            /* delay 4us + 1 clock tics (CALL instead of RCALL) = 4.0625 us */ \
            /* + 7 clock tics delay from "while (ADCSRA&(1<<ADSC))" loop, +0.4375 = 4.5000 */ \
            wait500ns(); 	/* 5.0625 us  */ \
            wdt_reset()		/* 5.1250 us  */ 
 #endif
#endif

//=================================================================
uint16_t GetESR(uint8_t hipin, uint8_t lopin) {
#if FLASHEND > 0x1fff
  //  measure the ESR value of capacitor
  unsigned int adcv[4];		// array for 4 ADC readings
  unsigned long sumvolt[4];	// array for 3 sums of ADC readings
  unsigned long cap_val_nF;
  uint16_t esrvalue;
  uint8_t HiPinR_L;		// used to switch 680 Ohm to HighPin
  uint8_t HiADC;		// used to switch Highpin directly to GND or VCC
  uint8_t LoPinR_L;		// used to switch 680 Ohm to LowPin
  uint8_t LoADC;		// used to switch Lowpin directly to GND or VCC
  uint8_t ii,jj;		// tempory values
  uint8_t StartADCmsk;		// Bit mask to start the ADC
  uint8_t SelectLowPin,SelectHighPin;
  uint8_t big_cap;
  int8_t esr0;			// used for ESR zero correction

  big_cap = 1;
  if (PartFound == PART_CAPACITOR) {
     ii = cap.cpre_max;
     cap_val_nF = cap.cval_max;
     while (ii < -9) { // set cval to nF unit
         cap_val_nF /= 10;		// reduce value by factor ten
         ii++;		// take next decimal prefix
     }
     if (cap_val_nF < (1800/18)) return(0xffff);			//capacity lower than 1.8 uF
//     if (cap_val_nF > (1800/18)) {
        /* normal ADC-speed, ADC-Clock 8us */
#ifdef ADC_Sleep_Mode
        StartADCmsk = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */
        ADCSRA = StartADCmsk;		/* enable ADC and Interrupt */
#else
        StartADCmsk =  (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable and start ADC */
#endif

//     } else {
        /* fast ADC-speed, ADC-Clock 2us */
#ifdef ADC_Sleep_Mode
//        StartADCmsk = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | FAST_CLOCK_DIV; /* enable ADC and Interrupt */
//        ADCSRA = StartADCmsk;		/* enable ADC and Interrupt */
//        SMCR = (1 << SM0) | (1 <<SE);	/* set ADC Noise Reduction and Sleep Enable */
#else
//        StartADCmsk =  (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; /* enable and start ADC */
#endif
//        big_cap = 0;
//     }
  }
  LoADC = pgm_read_byte(&PinADCtab[lopin]) | TXD_MSK;
  HiADC = pgm_read_byte(&PinADCtab[hipin]) | TXD_MSK;
  LoPinR_L = pgm_read_byte(&PinRLtab[lopin]);  //R_L mask for LowPin R_L load
  HiPinR_L = pgm_read_byte(&PinRLtab[hipin]);	//R_L mask for HighPin R_L load

#if PROCESSOR_TYP == 1280
    /* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
  SelectLowPin = (lopin | (1<<REFS1) | (0<<REFS0));	// switch ADC to LowPin, Internal Ref. 
  SelectHighPin = (hipin | (1<<REFS1) | (0<<REFS0));	// switch ADC to HighPin, Internal Ref. 
#else
  SelectLowPin = (lopin | (1<<REFS1) | (1<<REFS0));	// switch ADC to LowPin, Internal Ref. 
  SelectHighPin = (hipin | (1<<REFS1) | (1<<REFS0));	// switch ADC to HighPin, Internal Ref. 
#endif


// Measurement of ESR of capacitors AC Mode
   sumvolt[0] = 1;		// set sum of LowPin voltage to 1 to prevent divide by zero
   sumvolt[2] = 1;		// clear sum of HighPin voltage with current
                                // offset is about (x*10*200)/34000 in 0.01 Ohm units
   sumvolt[1] = 0;		// clear sum of HighPin voltage without current
   sumvolt[3] = 0;		// clear sum of HighPin voltage without current
   EntladePins();		// discharge capacitor
   ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
   ADMUX = SelectLowPin;	// set Mux input and Voltage Reference to internal 1.1V
#ifdef NO_AREF_CAP
   wait100us();			/* time for voltage stabilization */
#else
   wait_about10ms();		/* time for voltage stabilization with 100nF */
#endif
   /* start voltage must be negativ */
   ADC_DDR = HiADC;				// switch High Pin to GND
   R_PORT = LoPinR_L;			// switch R-Port to VCC
   R_DDR = LoPinR_L;			// switch R_L port for HighPin to output (VCC)
   wait10us();
   wait2us();
   R_DDR = 0;				// switch off current
   R_PORT = 0;
   StartADCwait();				// set ADCSRA Interrupt Mode, sleep

   // Measurement frequency is given by sum of ADC-Reads < 680 Hz for normal ADC speed.
   // For fast ADC mode the frequency is below 2720 Hz (used for capacity value below 3.6 uF).
   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after real start of conversion.
   // Real ADC-conversion is started with the next ADC-Clock (125kHz) after setting the ADSC bit.
   for(ii=0;ii<MAX_CNT;ii++) {
      ADC_DDR = LoADC;				// switch Low-Pin to output (GND)
      R_PORT = LoPinR_L;			// switch R-Port to VCC
      R_DDR = LoPinR_L;			// switch R_L port for LowPin to output (VCC)
      ADMUX = SelectLowPin;
      StartADCwait();			// set ADCSRA Interrupt Mode, sleep
      StartADCwait();			// set ADCSRA Interrupt Mode, sleep
      adcv[0] = ADCW;			// Voltage LowPin with current
      ADMUX = SelectHighPin;
//      if (big_cap != 0) {
         StartADCwait();			// ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV;	
         ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start with ADSC
         wait4us();
         R_PORT = HiPinR_L;			// switch R-Port to VCC
         R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
         DelayBigCap();		// wait predefined time
//      } else {
//         StartADCwait();			// ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV;	
//         R_PORT = HiPinR_L;			// switch R-Port to VCC
//         R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
//         ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; // enable ADC and start with ADSC
//      		// SH at 2.5 ADC clocks behind start = 5 us
//         DelaySmallCap();		// wait predefined time
//      }
      R_DDR = 0;				// switch current off,  SH is 1.5 ADC clock behind real start
      R_PORT = 0;
      while (ADCSRA&(1<<ADSC));		// wait for conversion finished
      adcv[1] = ADCW;			// Voltage HighPin with current
#ifdef ADC_Sleep_Mode
      ADCSRA = StartADCmsk;		/* enable ADC and Interrupt */
#endif
      wdt_reset();
      /* ********* Reverse direction, connect High side with GND *********** */
      ADC_DDR = HiADC;				// switch High Pin to GND
      R_PORT = HiPinR_L;			// switch R-Port to VCC
      R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
      wdt_reset();
      ADMUX = SelectHighPin;
      StartADCwait();			// set ADCSRA Interrupt Mode, sleep
      StartADCwait();			// set ADCSRA Interrupt Mode, sleep
      adcv[2] = ADCW;			// Voltage HighPin with current
      ADMUX = SelectLowPin;
//      if (big_cap != 0) {
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start with ADSC
         wait4us();
         R_PORT = LoPinR_L;
         R_DDR = LoPinR_L;			// switch LowPin with 680 Ohm to VCC
         DelayBigCap();		// wait predefined time
//      } else {
//         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
//         R_PORT = LoPinR_L;
//         R_DDR = LoPinR_L;			// switch LowPin with 680 Ohm to VCC
//         ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | FAST_CLOCK_DIV; // enable ADC and start with ADSC
//      			// 2.5 ADC clocks = 5 us
//         DelaySmallCap();		// wait predefined time
//      }
      R_DDR = 0;				// switch current off
      R_PORT = 0;
      while (ADCSRA&(1<<ADSC));		// wait for conversion finished
      adcv[3] = ADCW;			//  Voltage LowPin with current
#ifdef ADC_Sleep_Mode
      ADCSRA = StartADCmsk;		/* enable ADC and Interrupt */
#endif
      sumvolt[0] += adcv[0];			// add sum of both LowPin voltages with current
      sumvolt[1] += adcv[1];			// add  HighPin voltages with current
      sumvolt[2] += adcv[2];			// add  LowPin voltages with current
      sumvolt[3] += adcv[3];			// add  HighPin voltages with current
   } // end for


   sumvolt[0] += sumvolt[2];
#ifdef ESR_DEBUG
   lcd_testpin(hipin);
   lcd_testpin(lopin);
   lcd_data(' ');
   DisplayValue(sumvolt[0],0,'L',4);	// LowPin 1
   lcd_line3();
   DisplayValue(sumvolt[1],0,'h',4);	// HighPin 1
   lcd_data(' ');
   DisplayValue(sumvolt[3],0,'H',4);	// LowPin 2
   lcd_line4();
#endif


   if ((sumvolt[1] + sumvolt[3]) > sumvolt[0]) {
      sumvolt[2] = (sumvolt[1] + sumvolt[3]) - sumvolt[0];	// difference HighPin - LowPin Voltage with current
   } else {
      sumvolt[2] = 0;
   }
  if (PartFound == PART_CAPACITOR) {
      sumvolt[2] -= (1745098UL*MAX_CNT) / (cap_val_nF * (cap_val_nF + 19));
  }

#ifdef ESR_DEBUG
   DisplayValue(sumvolt[2],0,'d',4);	// HighPin - LowPin
   lcd_data(' ');
#endif
   esrvalue = (sumvolt[2] * 10 * (unsigned long)RRpinMI) / (sumvolt[0]+sumvolt[2]);
   esrvalue += esrvalue / 14;		/* esrvalue + 7% */
   esr0 = (int8_t)eeprom_read_byte(&EE_ESR_ZEROtab[hipin+lopin]);
   if (esrvalue > esr0) {
      esrvalue -= esr0;
   } else {
      esrvalue = 0;
   }


 #ifdef ADC_Sleep_Mode
     SMCR = (0 << SM0) | (0 << SE);	/* clear ADC Noise Reduction and Sleep Enable */
 #endif
  return (esrvalue);
#else
  return (0);
#endif
}
