#include <avr/io.h>
#include "config.h"
#include <stdlib.h>
#include "wait1000ms.h"
#include "lcd-routines.h"
#include "Transistortester.h"

/*
extern struct ADCconfig_t{
  uint8_t Samples;              // number of ADC samples to take
  uint8_t RefFlag;              // save Reference type VCC of IntRef
  uint16_t U_Bandgap;           // Reference Voltage in mV
  uint16_t U_AVCC;		// Voltage of AVCC
} ADCconfig;
*/


unsigned int ReadADC (uint8_t Probe) {
 unsigned int U; /* return value (mV) */
 uint8_t Samples; /* loop counter */
 unsigned long Value; /* ADC value */
 Probe |= (1 << REFS0); /* use internal reference anyway */
sample:
 ADMUX = Probe; /* set input channel and U reference */
#ifdef AUTOSCALE_ADC
 /* if voltage reference changed run a dummy conversion */
// Samples = Probe & (1 << REFS1); /* get REFS1 bit flag */
// if (Samples != ADCconfig.RefFlag) {
 if ((Probe & (1 << REFS1)) != 0) {
    // switch to 1.1V Reference
#ifdef NO_AREF_CAP
    wait100us(); /* time for voltage stabilization */
#else
    wait_about10ms(); /* time for voltage stabilization */
#endif
//    ADCconfig.RefFlag = Samples; /* update flag */
 }
#endif
#ifdef INHIBIT_SLEEP_MODE
// one dummy read of ADC, 112us
    ADCSRA |= (1 << ADSC); /* start conversion */
    while (ADCSRA & (1 << ADSC)); /* wait until conversion is done */
#else
    ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; //enable ADC and Interrupt
    set_sleep_mode(SLEEP_MODE_ADC);
    sleep_mode();	/* Start ADC, return, if ADC has finished */
#endif
 /* * sample ADC readings */
 Value = 0UL; /* reset sampling variable */
 Samples = 0; /* number of samples to take */
 while (Samples < ADCconfig.Samples) /* take samples */ {
#ifdef INHIBIT_SLEEP_MODE
    ADCSRA |= (1 << ADSC); /* start conversion */
    while (ADCSRA & (1 << ADSC)); /* wait until conversion is done */
#else
    ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADIE) | AUTO_CLOCK_DIV; //enable ADC and Interrupt
    set_sleep_mode(SLEEP_MODE_ADC);
    sleep_mode();	/* Start ADC, return, if ADC has finished */
#endif
    Value += ADCW; /* add ADC reading */
#ifdef AUTOSCALE_ADC
    /* auto-switch voltage reference for low readings */
    if ((Samples == 4) && (ADCconfig.U_Bandgap > 255) && ((uint16_t)Value < 1024) && !(Probe & (1 << REFS1))) {
       Probe |= (1 << REFS1); /* select internal bandgap reference */
#if PROCESSOR_TYP == 1280
       Probe &= ~(1 << REFS0);	/* ATmega640/1280/2560 1.1V Reference with REFS0=0 */
#endif
       goto sample; /* re-run sampling */
    }
#endif
    Samples++; /* one more done */
 }
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
