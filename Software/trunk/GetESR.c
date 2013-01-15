#include <avr/io.h>
#include <stdlib.h>
#include "config.h"
#include "wait1000ms.h"
#include "lcd-routines.h"
#include "Transistortester.h"

#define MAX_CNT 128

/* The sleep mode for ADC can be used. It is implemented for 8MHz and 16MHz operation */
/* But the ESR result is allways higher than the results with wait mode. */
/* The time of ESR measurement is higher with the sleep mode (checked with oszilloscope) */
/* The reason for the different time is unknown, the start of the next ADC measurement */
/* should be initiated before the next ADC-clock (8 us). One ADC takes 13 ADC clock + 1 clock setup. */
/* The setting to sleep mode takes 10 clock tics, the wakeup takes about 24 clock tics, but 8us are 64 clock tics. */
/* I have found no reason, why a reset of the ADC clock divider should occur during ESR measurement. */ 
//#define ADC_Sleep_Mode


#ifdef ADC_Sleep_Mode
  #define StartADCwait() ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /* enable ADC and Interrupt */\
    set_sleep_mode(SLEEP_MODE_ADC);\
    sleep_mode()		/* Start ADC, return if ADC has finished */
#else
//  #define StartADCwait() ADCSRA |= (1 << ADSC); /* Start conversion */
  #define StartADCwait() ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable ADC and start */\
    while (ADCSRA & (1 << ADSC))  /* wait until conversion is done */
#endif

//=================================================================
void GetESR() {
#if FLASHEND > 0x1fff
  //  measure the ESR value of capacitor
  unsigned int adcv[3];		// array for 3 ADC readings
  unsigned long sumvolt[3];	// array for 3 sums of ADC readings
  uint8_t HiPinR_L;		// used to switch 680 Ohm to HighPin
  uint8_t HiADC;		// used to switch Highpin directly to GND or VCC
  uint8_t LoPinR_L;		// used to switch 680 Ohm to LowPin
  uint8_t LoADC;		// used to switch Lowpin directly to GND or VCC
  uint8_t ii,jj;		// tempory values
  uint8_t SelectLowPin,SelectHighPin;
  int8_t esr0;			// used for ESR zero correction

  while (cap.cpre_max < -9) { // set cval to nF unit
      cap.cval_max /= 10;		// reduce value by factor ten
      cap.cpre_max++;		// take next decimal prefix
  }
  if (cap.cval_max < 1800) return;			//capacity lower than 1.8 uF
  lcd_fix_string(ESR_str);		// " ESR="
  LoADC = MEM_read_byte(&PinADCtab[cap.ca]) | TXD_MSK;
  HiADC = MEM_read_byte(&PinADCtab[cap.cb]) | TXD_MSK;
  LoPinR_L = MEM_read_byte(&PinRLtab[cap.ca]);  //R_L mask for LowPin R_L load
  HiPinR_L = MEM_read_byte(&PinRLtab[cap.cb]);	//R_L mask for HighPin R_L load

#if PROCESSOR_TYP == 1280
    /* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
  SelectLowPin = (cap.ca | (1<<REFS1) | (0<<REFS0));	// switch ADC to LowPin, Internal Ref. 
  SelectHighPin = (cap.cb | (1<<REFS1) | (0<<REFS0));	// switch ADC to HighPin, Internal Ref. 
#else
  SelectLowPin = (cap.ca | (1<<REFS1) | (1<<REFS0));	// switch ADC to LowPin, Internal Ref. 
  SelectHighPin = (cap.cb | (1<<REFS1) | (1<<REFS0));	// switch ADC to HighPin, Internal Ref. 
#endif


// Measurement of ESR of capacitors AC Mode
   sumvolt[0] = 1;		// set sum of LowPin voltage to 1 to prevent divide by zero
   sumvolt[1] = 1;		// clear sum of HighPin voltage with current
                                // offset is about (x*10*200)/34000 in 0.01 Ohm units
   sumvolt[2] = 0;		// clear sum of HighPin voltage without current
   EntladePins();		// discharge capacitor
   ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
   ADMUX = SelectLowPin;	// set Mux input and Voltage Reference to internal 1.1V
#ifdef NO_AREF_CAP
   wait100us();			/* time for voltage stabilization */
#else
   wait_about10ms();		/* time for voltage stabilization with 100nF */
#endif

// measurement frequency is given by sum of ADC-Reads < 680 Hz
   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after real start of conversion.
   // Real ADC-conversion is started with the next ADC-Clock (125kHz) after setting the ADSC bit.
   for(ii=0;ii<MAX_CNT;ii++) {
      ADC_DDR = LoADC;				// switch Low-Pin to output (GND)
      ADMUX = SelectLowPin;
      StartADCwait();				// set ADCSRA Interrupt Mode, sleep
      while (1) {
         R_PORT = HiPinR_L;			// switch R-Port to VCC
         R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
         ADMUX = SelectLowPin;
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         adcv[0] = ADCW;			// Voltage LowPin with current
         ADMUX = SelectHighPin;
         StartADCwait();			// ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV;	
//         ADCSRA |= (1<<ADSC);			// Start Conversion, real start is next rising edge of ADC clock
         ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start with ADSC
         wait10us();				// SH at 2.5 ADC clocks behind start = 20 us
         wait5us();			
#ifdef ADC_Sleep_Mode
 #if F_CPU == 8000000UL
         wait1us();				// with only 17 us delay the voltage goes down before SH
         // delay 16us + 3 clock tics (CALL instead of RCALL) = 16.375 us @ 8 MHz
         // + 24 clock tics delay from interrupt return, +3.00us = 19.375
         wdt_reset();	// 19.5   us -
         wdt_reset();	// 19.625 us -
         wdt_reset();	// 19.75  us -
         wdt_reset();	// 19.875 us -
         wdt_reset();	// 20.0   us -
         wdt_reset();	// 20.125 us +
 #endif
 #if F_CPU == 16000000UL
         wait3us();				// with only 18 us delay the voltage goes down before SH
         // delay 18us + 3 clock tics (CALL instead of RCALL) = 18.1875 us
         // + 24 clock tics delay from interrupt return, +1.50us = 19.6875
         wdt_reset();	// 19.75  us -
         wdt_reset();	// 19.8125us -
         wdt_reset();	// 19.875 us -
         wdt_reset();	// 19.9375us -
         wdt_reset();	// 20.00  us -  
         wdt_reset();	// 20.0625us +  
 #endif
#else
 #if F_CPU == 8000000UL
         wait4us();				// with only 18 us delay the voltage goes down before SH
         // delay 18us + 3 clock tics (CALL instead of RCALL) = 18.375 us @ 8 MHz
         // + 7 clock tics delay from while loop, +0.875us  = 19.25
//         wdt_reset();	/* 19.375 us - */
//         wdt_reset();	/* 19.5   us - */
//         wdt_reset();	/* 19.625 us - */
//         wdt_reset();	/* 19.75  us - */
//         wdt_reset();	/* 19.875 us - */
//         wdt_reset();	/* 20.0   us - */
//         wdt_reset();	/* 20.125 us + (1.1 , 0.63) */
//         wdt_reset();	/* 20.25  us + (1.0 , 0.60) */
//         wdt_reset();	/* 20.75  us + (0.86 , 0.52) */
 #endif
 #if F_CPU == 16000000UL
         wait4us();				// with only 18 us delay the voltage goes down before SH
         // delay 19us + 3 clock tics (CALL instead of RCALL) = 19.1875 us
         // + 7 clock tics delay from "while (ADCSRA&(1<<ADSC))" loop = 19.625
         wdt_reset();	// 19.6875us -
         wdt_reset();	// 19.75  us -
         wdt_reset();	// 19.8125us -
         wdt_reset();	// 19.875 us -
         wdt_reset();	// 19.9375us -
         wdt_reset();	// 20.00  us -  
         wdt_reset();	// 20.0625us +  
         wdt_reset();	// 20.125 us +  
         wdt_reset();	// 20.1875us +  
 #endif
#endif
//         wdt_reset();				// with wdt_reset the timing can be adjusted,
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
         R_DDR = 0;				// switch current off,  SH is 1.5 ADC clock behind real start
         R_PORT = 0;
         while (ADCSRA&(1<<ADSC));		// wait for conversion finished
         adcv[1] = ADCW;			// Voltage HighPin with current
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         adcv[2] = ADCW;			// Voltage HighPin without current
         if (adcv[2] > 2) break;		// at least more than two digits required
         wdt_reset();
      } // end while (1)
      sumvolt[0] += adcv[0];			// add sum of both LowPin voltages with current
      sumvolt[1] += adcv[1];			// add  HighPin voltages with current
      sumvolt[2] += adcv[2]; 			// capacitor voltage without current
      /* ********* Reverse direction, connect High side with GND *********** */
      ADC_DDR = HiADC;				// switch High Pin to GND
      StartADCwait();				// set ADCSRA Interrupt Mode, sleep
      while (1) {
         R_PORT = LoPinR_L;
         R_DDR = LoPinR_L;			// switch LowPin with 680 Ohm to VCC
         wdt_reset();
         ADMUX = SelectHighPin;
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         adcv[0] = ADCW;			// Voltage HighPin with current
         ADMUX = SelectLowPin;
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
//         ADCSRA |= (1<<ADSC);			// Start Conversion, real start is next rising edge of ADC clock
         ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; // enable ADC and start with ADSC
         wait10us();				// 2.5 ADC clocks = 20 us
         wait5us();			
#ifdef ADC_Sleep_Mode
 #if F_CPU == 8000000UL
         wait1us();				// with only 16 us delay the voltage goes down before SH
         // delay 16us + 3 clock tics (CALL instead of RCALL) = 16.375 us @ 8 MHz
         // + 24 clock tics delay from interrupt return, +3.00us = 19.375
         wdt_reset();	// 19.5   us -
         wdt_reset();	// 19.625 us -
         wdt_reset();	// 19.75  us -
         wdt_reset();	// 19.875 us -
         wdt_reset();	// 20.0   us -
         wdt_reset();	// 20.125 us +
 #endif
 #if F_CPU == 16000000UL
         wait3us();				// with only 18 us delay the voltage goes down before SH
         // delay 18us + 3 clock tics (CALL instead of RCALL) = 18.1875 us
         // + 24 clock tics delay from interrupt return, +1.50us = 19.6875
         wdt_reset();	// 19.75  us -
         wdt_reset();	// 19.8125us -
         wdt_reset();	// 19.875 us -
         wdt_reset();	// 19.9375us -
         wdt_reset();	// 20.00  us -  
         wdt_reset();	// 20.0625us +  
 #endif
#else
 #if F_CPU == 8000000UL
         wait4us();				// with only 18 us delay the voltage goes down before SH
         // delay 18us + 3 clock tics (CALL instead of RCALL) = 18.375 us @ 8 MHz
         // + 7 clock tics delay from while loop, +0.875us  = 19.25
//         wdt_reset();	/* 19.375 us - */
//         wdt_reset();	/* 19.5   us - */
//         wdt_reset();	/* 19.625 us - */
//         wdt_reset();	/* 19.75  us - */
//         wdt_reset();	/* 19.875 us - */
//         wdt_reset();	/* 20.0   us - */
//         wdt_reset();	/* 20.125 us + */
//         wdt_reset();	/* 20.25  us + */
 #endif
 #if F_CPU == 16000000UL
         wait4us();				// with only 18 us delay the voltage goes down before SH
         // delay 19us + 3 clock tics (CALL instead of RCALL) = 19.1875 us
         // + 7 clock tics delay from "while (ADCSRA&(1<<ADSC))" loop = 19.625
         wdt_reset();	// 19.6875us -
         wdt_reset();	// 19.75  us -
         wdt_reset();	// 19.8125us -
         wdt_reset();	// 19.875 us -
         wdt_reset();	// 19.9375us -
         wdt_reset();	// 20.00  us -  
         wdt_reset();	// 20.0625us +  
         wdt_reset();	// 20.125 us +  
         wdt_reset();	// 20.1875us +  
 #endif
#endif
//         wdt_reset();				// with wdt_reset the timing can be adjusted,
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
         R_DDR = 0;				// switch current off, SH is 1.5 ADC clock ticks behind real start
         R_PORT = 0;
         while (ADCSRA&(1<<ADSC));		// wait for conversion finished
         adcv[1] = ADCW;			//  Voltage LowPin with current
//          ADMUX = SelectLowPin;
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         StartADCwait();			// set ADCSRA Interrupt Mode, sleep
         adcv[2] = ADCW;			// Voltage LowPin without current
         if (adcv[2] > 2) break;		// at least more than two digits required
         wdt_reset();
      } // end while (1)
      R_DDR = 0;				// switch current off
      sumvolt[0] += adcv[0];			// add  LowPin voltages with current
      sumvolt[1] += adcv[1];			// add  HighPin voltages with current
      sumvolt[2] += adcv[2];			// add  HighPin voltages without current
   } // end for

//#define ESR_DEBUG

#ifdef ESR_DEBUG
   DisplayValue(sumvolt[0],0,'L',4);	// LowPin
#endif
   // we need to compensate the time delay between reading the LowPin Voltage and the
   // HighPin Voltage, which is usually 2 * 14 * 8 us = 224 us.
   // With the loading of the capacitor the current will sink, so we get a too high voltage at
   // the LowPin. The velocity of degration is inversely proportional to time constant (represented by capacity value).
   // Time constant for 1uF & 720 Ohm is 720us
//   sumvolt[0] -= (sumvolt[0] * 150UL)  / cap.cval_max;	// Version 1.04k
   sumvolt[0] -= (sumvolt[0] * 360UL)  / cap.cval_max;
#ifdef ESR_DEBUG
   lcd_line3();
   DisplayValue(sumvolt[1],0,'H',4);	// HighPin
   lcd_data(' ');
#endif

   esr0 = (int8_t)eeprom_read_byte(&EE_ESR_ZERO);
//   sumvolt[0] += (((long)sumvolt[0] * esr0) / ((RR680MI - R_L_VAL) * 10)); // subtract 0.23 Ohm from ESR, Vers. 1.04k
   sumvolt[2] += (((long)sumvolt[0] * esr0) / ((RR680MI - R_L_VAL) * 10)); // subtract 0.23 Ohm from ESR

#ifdef ESR_DEBUG
   DisplayValue(sumvolt[0],0,'C',4);	// Lowpin corrected
#endif

   if (sumvolt[1] > sumvolt[0]) {
      sumvolt[1] -= sumvolt[0];		// difference HighPin - LowPin Voltage with current
   } else {
      sumvolt[1] = 0;
   }

#ifdef ESR_DEBUG
   lcd_line4();
   DisplayValue(sumvolt[1],0,'d',4);	// HighPin - LowPin
   lcd_data(' ');
#endif

//   sumvolt[2] = (sumvolt[2] * 980) / 1000;	// scale down the voltage without current with 98.0% 1.04k

#ifdef ESR_DEBUG
   DisplayValue(sumvolt[2],0,' ',4);	// HighPin without current
#endif

   jj = 0;
   if (sumvolt[1] >= sumvolt[2]) {
      // mean voltage at the capacitor is higher with current
      // sumvolt[0] is the sum of voltages at LowPin, caused by output resistance of Port
      // (RR680MI - R_L_VAL) is the port output resistance in 0.1 Ohm units.
      // we scale up the difference voltage with 10 to get 0.01 Ohm units of ESR
      cap.esr = ((sumvolt[1] - sumvolt[2]) * 10 * (unsigned long)(RR680MI - R_L_VAL)) / sumvolt[0];
      DisplayValue(cap.esr,-2,LCD_CHAR_OMEGA,2);
   } else {
      jj = ((sumvolt[2] - sumvolt[1]) * 10 * (unsigned long)(RR680MI - R_L_VAL)) / sumvolt[0];
      lcd_data('0');
      if ((jj < 100) && (jj > 0)) {
         lcd_data('?');			// mark ESR zero correction
         esr0 -= jj;			// correct ESR_ZERO by negative resistance
         eeprom_write_byte((uint8_t *)(&EE_ESR_ZERO), (int8_t)esr0);       // fix new zero offset 
      }
   }
//   lcd_line3();
//   DisplayValue(jj,0,' ',4);	// correction for ESR_ZERO
//   DisplayValue(1000+esr0,0,' ',4);	// new ESR_ZERO
#endif
  return;
}
