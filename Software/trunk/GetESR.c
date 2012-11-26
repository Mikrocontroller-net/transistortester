#include <avr/io.h>
#include <stdlib.h>
#include "config.h"
#include "wait1000ms.h"
#include "lcd-routines.h"
#include "Transistortester.h"

#define MAX_CNT 128

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


// Measurement of ESR of capacitors AC Mode
   sumvolt[0] = 1;		// set sum of LowPin voltage to 1 to prevent divide by zero
   sumvolt[1] = 1;		// clear sum of HighPin voltage with current
                                // offset is about (x*10*200)/34000 in 0.01 Ohm units
   sumvolt[2] = 0;		// clear sum of HighPin voltage without current
   EntladePins();	// discharge capacitor
   ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
// measurement frequency is given by sum of ADC-Reads < 680 Hz
   // ADC Sample and Hold (SH) is done 1.5 ADC clock number after starting 
   for(ii=0;ii<MAX_CNT;ii++) {
      ADC_DDR = LoADC;				// switch Low-Pin to output (GND)
       ADMUX = cap.ca | (1<<REFS1) | (1<<REFS0);	// switch ADC to LowPin, Internal Ref. 
       ADCSRA |= (1<<ADSC);				// Start Conversion
       while (ADCSRA&(1<<ADSC));	// wait dummy read , sync with ADC-clock
      while (1) {
         R_PORT = HiPinR_L;			// switch R-Port to VCC
         R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (VCC)
          ADMUX = cap.ca | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion
         while (ADCSRA&(1<<ADSC));		// wait
         adcv[0] = ADCW;			// Voltage LowPin with current
          ADMUX = cap.cb | (1<<REFS1) | (1<<REFS0);      // switch ADC to HighPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion, real start is rising edge of ADC clock
         wait10us();				// 2.5 ADC clocks = 20 us
         wait5us();			
         wait3us();				// with only 17 us delay the voltage goes down before SH
         wdt_reset();
         wdt_reset();
         wdt_reset();
         wdt_reset();
         wdt_reset();
         wdt_reset();
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
      ADC_DDR = HiADC;				// switch High Pin to GND
       ADCSRA |= (1<<ADSC);			// Start Conversion
       while (ADCSRA&(1<<ADSC));		// wait dummy read
      while (1) {
         R_PORT = LoPinR_L;
         R_DDR = LoPinR_L;			// switch LowPin with 680 Ohm to VCC
         wdt_reset();
          ADMUX = cap.cb | (1<<REFS1) | (1<<REFS0);      // switch ADC to HighPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion
         while (ADCSRA&(1<<ADSC));		// wait
         adcv[0] = ADCW;			// Voltage HighPin with current
          ADMUX = cap.ca | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
          ADCSRA |= (1<<ADSC);			// Start Conversion
          while (ADCSRA&(1<<ADSC));		// wait dummy read after switch
         ADCSRA |= (1<<ADSC);			// Start Conversion, real start is rising edge of ADC clock
         wait10us();				// 2.5 ADC clocks = 20 us
         wait5us();			
         wait3us();				// with only 17 us delay the voltage goes down before SH
         wdt_reset();
         wdt_reset();
         wdt_reset();
         wdt_reset();
         wdt_reset();
         wdt_reset();
//         wdt_reset();				// with wdt_reset the timing can be adjusted,
						// when time is too short, voltage is down before SH of ADC
						// when time is too long, capacitor will be overloaded.
						// That will cause too high voltage without current.
         R_DDR = 0;				// switch current off, SH is 1.5 ADC clock ticks behind start
         while (ADCSRA&(1<<ADSC));		// wait for conversion finished
         adcv[1] = ADCW;			//  Voltage LowPin with current
         R_PORT = 0;
          ADMUX = cap.ca | (1<<REFS1) | (1<<REFS0);      // switch ADC to LowPin, Internal Ref. 
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
   // we need to compensate the time delay between reading the LowPin Voltage and the
   // HighPin Voltage, which is usually 2 * 14 * 8 us = 224 us.
   // With the loading of the capacitor the current will sink, so we get a too high voltage at
   // the LowPin. The velocity of degration is inversely proportional to time constant (represented by capacity value).
   sumvolt[0] -= (sumvolt[0] * 150UL)  / cap.cval_max;
   esr0 = (int8_t)eeprom_read_byte(&EE_ESR_ZERO);
   sumvolt[0] += (((long)sumvolt[0] * esr0) / ((RR680MI - R_L_VAL) * 10)); // subtract 0.23 Ohm from ESR
   if (sumvolt[1] > sumvolt[0]) {
      sumvolt[1] -= sumvolt[0];		// difference HighPin - LowPin Voltage with current
   } else {
      sumvolt[1] = 0;
   }
   sumvolt[2] = (sumvolt[2] * 980) / 1000;	// scale down the voltage without current with 98.0%
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
