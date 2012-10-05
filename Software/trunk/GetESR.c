#include <avr/io.h>
#include <stdlib.h>
#include "config.h"
#include "wait1000ms.h"
#include "lcd-routines.h"
#include "Transistortester.h"

#define ESR_AC_METHOD		// use the AC measurement method for ESR measurement

//=================================================================
void GetESR(uint8_t HighPin, uint8_t LowPin) {
#if FLASHEND > 0x1fff
  //  measure the ESR value of capacitor
  unsigned int adcv[3];
  unsigned long sumvolt[3];
  uint8_t HiPinR_L;
 #ifdef ESR_AC_METHOD
  uint8_t HiADC, LoPinR_L;
 #endif
  uint8_t LoADC;
  uint8_t ii;

  LoADC = MEM_read_byte(&PinADCtab[LowPin]) | TXD_MSK;
 #ifdef ESR_AC_METHOD
  HiADC = MEM_read_byte(&PinADCtab[HighPin]) | TXD_MSK;
  LoPinR_L = MEM_read_byte(&PinRLtab[LowPin]);  //R_L mask for LowPin R_L load
 #endif
  HiPinR_L = MEM_read_byte(&PinRLtab[HighPin]);	//R_L mask for HighPin R_L load

 #ifdef ESR_AC_METHOD

// Measurement of ESR of capacitors AC Mode
   sumvolt[0] = 1;		// set sum of LowPin voltage to 1 to prevent divide by zero
   sumvolt[1] = 128;		// clear sum of HighPin voltage with current
   sumvolt[2] = 0;		// clear sum of HighPin voltage without current
   EntladePins();	// discharge capacitor
   ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
// measurement frequency is given by sum of ADC-Reads < 680 Hz
   ADMUX = LowPin | (1<<REFS1) | (1<<REFS0);	// switch ADC to LowPin, Internal Ref. 
   ADCSRA |= (1<<ADSC);				// Start Conversion
   while (ADCSRA&(1<<ADSC));	// wait dummy read , sync with ADC-clock
   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after starting 
   for(ii=0;ii<128;ii++) {
      ADC_DDR = LoADC;				// switch Low-Pin to output (GND)
      while (1) {
         R_PORT = HiPinR_L;			// switch R-Port to VCC
         R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
          ADMUX = LowPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion
         while (ADCSRA&(1<<ADSC));		// wait
         adcv[0] = ADCW;			// Voltage LowPin with current
          ADMUX = HighPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to HighPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion, real start is rising edge of ADC clock
         wait10us();				// 2.5 ADC clocks = 20 us
         wait5us();			
         wait3us();				// with only 17 us delay the voltage goes down before SH
//         wdt_reset();				// with wdt_reset the timing can be adjusted,
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
         R_DDR = 0;				// switch current off,  SH is 1.5 ADC clock behind start
         while (ADCSRA&(1<<ADSC));		// wait for conversion finished
         adcv[1] = ADCW;			// Voltage HighPin with current
         R_PORT = 0;
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read
         ADCSRA |= (1<<ADSC);			// Start Conversion
         while (ADCSRA&(1<<ADSC));		// wait
         adcv[2] = ADCW;			// Voltage HighPin without current
         if (adcv[2] > 2) break;
         wdt_reset();
      }
      sumvolt[0] += adcv[0];			// add sum of both LowPin voltages with current
      sumvolt[1] += adcv[1];			// add  HighPin voltages with current
      sumvolt[2] += adcv[2]; 			// capacitor voltage without current
       ADCSRA |= (1<<ADSC);			// Start Conversion
       while (ADCSRA&(1<<ADSC));		// wait dummy read
      ADC_DDR = HiADC;				// switch High Pin to GND
      while (1) {
         R_PORT = LoPinR_L;
         R_DDR = LoPinR_L;			// switch LowPin with 680 Ohm to VCC
         wdt_reset();
          ADMUX = HighPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to HighPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion
         while (ADCSRA&(1<<ADSC));		// wait
         adcv[0] = ADCW;			// Voltage HighPin with current
          ADMUX = LowPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion, real start is rising edge of ADC clock
         wait10us();				// 2.5 ADC clocks = 20 us
         wait5us();			
         wait3us();				// with only 17 us delay the voltage goes down before SH
//         wdt_reset();				// with wdt_reset the timing can be adjusted,
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
         R_DDR = 0;				// switch current off, SH is 1.5 ADC clock ticks behind start
         while (ADCSRA&(1<<ADSC));		// wait for conversion finished
         adcv[1] = ADCW;			//  Voltage LowPin with current
         R_PORT = 0;
          ADMUX = LowPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);		// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read
         ADCSRA |= (1<<ADSC);			// Start Conversion
         while (ADCSRA&(1<<ADSC));		// wait
         adcv[2] = ADCW;			// Voltage LowPin without current
         if (adcv[2] > 2) break;
         wdt_reset();
      }
      R_DDR = 0;				// switch current off
      sumvolt[0] += adcv[0];			// add  LowPin voltages with current
      sumvolt[1] += adcv[1];			// add  HighPin voltages with current
      sumvolt[2] += adcv[2];			// add  HighPin voltages without current
   } // end for
   if (sumvolt[1] > sumvolt[0]) {
      sumvolt[1] -= sumvolt[0];
   } else {
      sumvolt[1] = 0;
   }
   sumvolt[2] = (sumvolt[2] * 960) / 1000;	// scale down the voltage without current with 96.0%
 #else
   // ESR measurement of capacitors, unipolar methode
   sumvolt[0] = 1;		// set sum of LowPin voltage to 1 to prevent divide by zero
   sumvolt[1] = 25;		// clear sum of HighPin voltage with current
   sumvolt[2] = 0;		// clear sum of HighPin voltage without current
   adcv[1] = 1023;
   // ADC Sample and Hold is done 1.5 ADC clock number after starting 
   for(ii=0;ii<250;ii++) {
      // on cycle of loop reads 7 times ADC, about 784 us total
      if (adcv[1] > 500) {
         EntladePins();	// discharge capacitor
      }
      ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
      ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
       ADMUX = LowPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
       ADCSRA |= (1<<ADSC);		// Start Conversion
       while (ADCSRA&(1<<ADSC));	// wait dummy read , sync with ADC-clock
      R_PORT = HiPinR_L;		// switch R-Port to VCC
      R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
       ADCSRA |= (1<<ADSC);		// Start Conversion
       while (ADCSRA&(1<<ADSC));	// wait dummy read
      ADCSRA |= (1<<ADSC);		// Start Conversion
      while (ADCSRA&(1<<ADSC));		// wait
      adcv[0] = ADCW;			// Voltage LowPin with current
       ADMUX = HighPin | (1<<REFS1) | (1<<REFS0);      // switch ADC to HighPin, Internal Ref. 
       ADCSRA |= (1<<ADSC);		// Start Conversion
       while (ADCSRA&(1<<ADSC));	// wait dummy read
      ADCSRA |= (1<<ADSC);		// Start Conversion, real start is rising edge of ADC clock
      wait10us();			// 2.5 ADC clocks = 20 us
      wait5us();			
      wait3us();			// with only 17 us delay the voltage goes down before SH
      R_DDR = 0;			// switch current off,  1.5 ADC clock behind start conversion
      while (ADCSRA&(1<<ADSC));		// wait for conversion finished
      adcv[1] = ADCW;			// Voltage HighPin with current
#
       ADCSRA |= (1<<ADSC);		// Start Conversion
       while (ADCSRA&(1<<ADSC));	// wait dummy read
      ADCSRA |= (1<<ADSC);		// Start Conversion
      while (ADCSRA&(1<<ADSC));		// wait
      adcv[2] = ADCW;			// Voltage HighPin without current
#
      sumvolt[0] += adcv[0];		// add sum of both LowPin voltages with current
      sumvolt[1] += adcv[1];		// add  HighPin voltages with current
      sumvolt[2] += adcv[2]; 		// capacitor voltage without current
   } // end for
   if (sumvolt[1] > sumvolt[0]) {
      // sum of HighPin voltages is higher than the sum of LowPin voltages
      sumvolt[1] -= sumvolt[0];		// difference is the mean voltage at C with current
   } else {
      sumvolt[1] = 0;			// no mean voltage at the capacitor!
   }
   sumvolt[2] = (sumvolt[2] * 1998) / 2000;
 #endif
   if (sumvolt[1] > sumvolt[2]) {
      // mean voltage at the capacitor is higher with current
      // sumvolt[0] is the sum of voltages at LowPin, caused by output resistance of Port
      // (RR680MI - R_L_VAL) is the port output resistance in 0.1 Ohm units.
      // we scale up the difference voltage with 10 to get 0.01 Ohm units of ESR
      cap.esr = ((sumvolt[1] - sumvolt[2]) * 10 * (unsigned long)(RR680MI - R_L_VAL)) / sumvolt[0];
   }
//   lcd_line3();
//   DisplayValue(sumvolt[0],0,' ',4);	// Voltage at LowPin with current
//   lcd_line4();
//   DisplayValue(sumvolt[1],0,' ',4);	// Difference Voltage HighPin - LowPin
//   DisplayValue(sumvolt[2],0,' ',4);	// Voltage at HighPin without current
//   wait3s();
#endif
  return;
}
