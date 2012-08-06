#include <avr/io.h>
#include "config.h"
#include <stdlib.h>
#include "wait1000ms.h"
#include "lcd-routines.h"

extern uint8_t minmul;
extern uint8_t mindiv;

// ReadADC.c is replaces by functional identical ReadADC.S
//                          ==============================
// ANZ_MESS = 44 for best accuracy, Sum of 44 single ADC values 
// ANZ_MESS = 22 for middle accuracy, double of the sum of 22 single ADC values 
// ANZ_MESS = 11 for fastest result , four times the sum of 11 single ADC values 
// Per division by 9 we get the resolution of mV .

#ifndef ANZ_MESS
#define ANZ_MESS 44
#endif

unsigned int ReadADC(uint8_t mux) {
  //read value of specified ADC channel and give back result with mV resolution 
  unsigned int adcx;
  uint8_t jj;

  ADMUX = mux | (1<<REFS0);
#ifdef AUTOSCALE_ADC
  if ((mux&(1<<REFS1)) != 0) {
     goto lowADC;
  }
#endif
  adcx = (ANZ_MESS/11); 		// round up the result
  ADCSRA |= (1<<ADSC);		//start conversion
  while (ADCSRA&(1<<ADSC));	//wait for ADC finished
#ifdef NO_AREF_CAP
  wait50us();			// report AVR126 recommends a 70µs wait time for ADC Reading of band gap
#else
  wait300us();
#endif

  for (jj = 0; jj < ANZ_MESS; jj++) {
     //repeat ANZ_MESS measurements for oversampling 
     ADCSRA |= (1<<ADSC);		//start conversion
     while (ADCSRA & (1<<ADSC));	//wait for ADC finished
     adcx += ADCW;		// add  measurements
#define U_GRENZ 1024		//about 1V
#ifdef AUTOSCALE_ADC
     if ((jj == 4) && (adcx < U_GRENZ)){
       //switch ADC reference to internal ref instead of 5V
       goto lowADC;
     }
#endif
  }
#if ANZ_MESS == 22
  adcx *= 2;		//multiply sum by 2
#endif
  
#if ANZ_MESS == 11
  adcx *= 4;		//multiply sum by 4
#endif
  return adcx/9;	// return (sum / 9), gives a resolution in mV

//########################################################
#ifdef AUTOSCALE_ADC
lowADC:
#if 0
  lcd_line4();
  lcd_string(utoa(adcx, outval, 10));
  lcd_data('~');
#endif
  
  ADMUX = mux | (1<<REFS1) | (1<<REFS0); // Internal 1.1V reference
  adcx = 0;
  ADCSRA |= (1<<ADSC);		//start conversion
  while (ADCSRA&(1<<ADSC));	//wait for ADC finished
#ifdef NO_AREF_CAP
  wait300us();			// without or 1nF capacitor at the AREF pin
#else
  wait5ms();			// up to 100nF capacitor at the AREF pin
  wait1ms();
#endif
  for (jj=0; jj<minmul; jj++) {
     // add minmul measurements
     ADCSRA |= (1<<ADSC);		//start conversion
     while (ADCSRA&(1<<ADSC));		//wait for ADC finished
     adcx += ADCW;			//add measurement results 
  }
#if 0
  lcd_string(utoa(adcx, outval, 10));
  lcd_data('/');
  lcd_string(utoa(mindiv, outval, 10));
  lcd_data(' ');
  wait1s();
#endif
  return (adcx/mindiv);			//divide by mindiv gives a resolution in  mV
#endif
}

unsigned int W20msReadADC(uint8_t mux) {
   // wait 20ms before reading of ADC
   wait20ms();
   return (ReadADC(mux));
  }
  

unsigned int W5msReadADC(uint8_t mux) {
   // wait 5ms before reading of ADC
   wait5ms();
   return (ReadADC(mux));
  }

