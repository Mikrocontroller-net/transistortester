#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

/*
extern struct ADCconfig_t{
  uint8_t Samples;              // number of ADC samples to take
  uint8_t RefFlag;              // save Reference type VCC of IntRef
  uint16_t U_Bandgap;           // Reference Voltage in mV
  uint16_t U_AVCC;		// Voltage of AVCC
} ADCconfig;
*/


#ifdef INHIBIT_SLEEP_MODE
//  #define StartADCwait() ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; /* enable ADC and start */
    #define StartADCwait() ADCSRA = (1<<ADSC)|(1<<ADEN)|(1<<ADIF)|AUTO_CLOCK_DIV; /* Start conversion */\
    while (ADCSRA & (1 << ADSC));  /* wait until conversion is done */
#else
    #define StartADCwait() ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; /*enable ADC and Interrupt */\
    set_sleep_mode(SLEEP_MODE_ADC);\
    sleep_mode();	/* Start ADC, return, if ADC has finished */
#endif

/***********************************************************************************/
/* ReadADC takes ADCconfig.Samples ADC reads and build a sum of the data           */
/* One ADC read take a time period about 112us.                                    */
/* So 25 ADC reads take more than 2.8ms time.                                      */
/* for resistor and other measurements 190 ADC reads are done with more than 21ms  */


unsigned int ReadADC (uint8_t Probe) {
 unsigned int U; /* return value (mV) */
 uint8_t samples; /* loop counter */
 unsigned long Value; /* ADC value */

 Probe |= (1 << REFS0); /* use internal reference anyway */

#ifdef AUTOSCALE_ADC
get_sample:
#endif  /* AUTOSCALE_ADC */

#if (PROCESSOR_TYP == 644) || (PROCESSOR_TYP == 1280)
 if (Probe & (1 << REFS1)) Probe &= ~(1<<REFS0);	/* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
#endif

 ADMUX = Probe; /* set input channel and U reference */

#ifdef AUTOSCALE_ADC
 /* if voltage reference changes, wait for voltage stabilization */
 if ((Probe & (1<<REFS1)) != ADCconfig.RefFlag) {
    //  Reference is switched, delay depends on NO_AREF_CAP option
    ADCconfig.RefFlag = (Probe & (1<<REFS1));	// save new reference state
 #ifdef NO_AREF_CAP
    wait100us(); /* time for voltage stabilization */
 #else
    wait_about10ms(); /* time for voltage stabilization */
 #endif
// allways do one dummy read of ADC, 112us
    StartADCwait();		/* start ADC and wait */
 }
#endif  /* AUTOSCALE_ADC */

 /* * sample ADC readings */
 Value = 0UL; /* reset sampling variable */
 samples = 0; /* number of samples to take */
 while (samples < ADCconfig.Samples) /* take samples */ {
    StartADCwait();		/* start ADC and wait */
    Value += ADCW; /* add ADC reading */
#ifdef AUTOSCALE_ADC
    /* auto-switch voltage reference for low readings */
    if ((samples == 4) && (ADCconfig.U_Bandgap > 255) && ((uint16_t)Value < 1024) && !(Probe & (1<<REFS1))) {
       Probe |= (1 << REFS1); /* select internal bandgap reference */
       goto get_sample; /* re-run sampling */
    }
#endif
    samples++; /* one more done */
 }  /* end while */
#ifdef AUTOSCALE_ADC
 /* * convert ADC reading to voltage * - single sample: U = ADC reading * U_ref / 1024 */
 /* get voltage of reference used */
 if (Probe & (1 << REFS1)) U = ADCconfig.U_Bandgap; /* bandgap reference */
 else U = ADCconfig.U_AVCC; /* Vcc reference */
#else
 U = ADCconfig.U_AVCC; /* Vcc reference */
#endif

 /* convert to voltage; */
 Value *= U; /* ADC readings * U_ref */
 Value /= 1023; /* / 1024 for 10bit ADC */
 /* de-sample to get average voltage */
 Value /= ADCconfig.Samples;
 U = (unsigned int)Value;
 return U;
//   return ((unsigned int)(Value / (1023 * (unsigned long)ADCconfig.Samples)));
}
unsigned int W5msReadADC (uint8_t Probe) {
  wait_about5ms();
  return (ReadADC(Probe));
}
unsigned int W10msReadADC (uint8_t Probe) {
  wait_about10ms();
  return (ReadADC(Probe));
}
unsigned int W20msReadADC (uint8_t Probe) {
  wait_about20ms();
  return (ReadADC(Probe));
}

uint16_t unsigned_diff(uint16_t v1, uint16_t v2)
{
  if (v1 > v2) return (v1 - v2);
  else         return (0);
}
uint16_t abs_diff(uint16_t v1, uint16_t v2)
{
  if (v1 > v2) return (v1 - v2);
  else         return (v2 - v1);
}

uint16_t vcc_diff(uint16_t v2)
{
  return unsigned_diff(ADCconfig.U_AVCC, v2);
//  if (ADCconfig.U_AVCC > v2) return (ADCconfig.U_AVCC - v2);
//  else         return (0);
}

