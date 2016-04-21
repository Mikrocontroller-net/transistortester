#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Transistortester.h"


// first discharge any charge of capacitors
void EntladePins() {
  uint8_t adc_gnd;		// Mask of ADC-outputs, which can be directly connected to GND
  unsigned int adcmv[3];	// voltages of 3 Pins in mV
  unsigned int clr_cnt;		// Clear Counter
  uint8_t lop_cnt;		// loop counter
// max. time of discharge in ms  (10000/20) == 10s
#define MAX_ENTLADE_ZEIT  (10000/20)
#if DebugOut == 99
  uart_putc('L');		//Debug
#endif
  for(lop_cnt=0;lop_cnt<10;lop_cnt++) {
     adc_gnd = TXD_MSK;		// put all ADC to Input
     ADC_DDR = adc_gnd;
     ADC_PORT = TXD_VAL;		// ADC-outputs auf 0
     R_PORT = 0;			// R-outputs auf 0
//     R_DDR = (1<<PIN_RH3) | (1<<PIN_RH2) | (1<<PIN_RH1); // R_H for all Pins to GND
     R_DDR = (1<<PIN_RH3) | (1<<PIN_RL3) | (1<<PIN_RH2) | (1<<PIN_RL2) | (1<<PIN_RH1) | (1<<PIN_RL1); // R_H and R_L for all Pins to GND
     adcmv[0] = W5msReadADC(TP1);	// which voltage has Pin 1?
     adcmv[1] = ReadADC(TP2);	// which voltage has Pin 2?
     adcmv[2] = ReadADC(TP3);	// which voltage has Pin 3?
#if DebugOut == 99
        lcd_line3();
        uart_putc('T');
        u2lcd_space(adcmv[0]);	// lcd_string(utoa(adcmv[0], outval, 10)); lcd_space();
        u2lcd_space(adcmv[1]);	// lcd_string(utoa(adcmv[1], outval, 10)); lcd_space();
        u2lcd(adcmv[2]);		// lcd_string(utoa(adcmv[2], outval, 10));
        wait2s();
#endif
     if ((PartFound == PART_CELL) || ((adcmv[0] < CAP_EMPTY_LEVEL) && (adcmv[1] < CAP_EMPTY_LEVEL) && (adcmv[2] < CAP_EMPTY_LEVEL))) {
        ADC_DDR = TXD_MSK;		// switch all ADC-Pins to input
        R_DDR = 0;			// switch all R_L Ports (and R_H) to input
#if FLASHEND > 0x3fff
        cell_mv[0] = adcmv[0];		// save the voltage of pin 1
        cell_mv[1] = adcmv[1];		// save the voltage of pin 2
        cell_mv[2] = adcmv[2];		// save the voltage of pin 3
#endif
#if DebugOut == 99
        uart_putc('l');		//Debug
#endif
        return;			// all is discharged
     }
     // all Pins with voltage lower than 1V can be connected directly to GND (ADC-Port)
     if (adcmv[0] < 1000) {
        adc_gnd |= (1<<TP1);	//Pin 1 directly to GND
     }
     if (adcmv[1] < 1000) {
        adc_gnd |= (1<<TP2);	//Pin 2 directly to GND
     }
     if (adcmv[2] < 1000) {
        adc_gnd |= (1<<TP3);	//Pin 3 directly to  GND
     }
     ADC_DDR = adc_gnd;		// switch all selected ADC-Ports at the same time

     // additionally switch the leaving Ports with R_L to GND.
     // since there is no disadvantage for the already directly switched pins, we can
     // simply switch all  R_L resistors to GND
//     R_DDR = (1<<PIN_RL3) | (1<<PIN_RL2) | (1<<PIN_RL1);	// Pins across R_L resistors to GND
     for(clr_cnt=0;clr_cnt<MAX_ENTLADE_ZEIT;clr_cnt++) {
        wdt_reset();
        adcmv[0] = W20msReadADC(TP1);	// which voltage has Pin 1?
        adcmv[1] = ReadADC(TP2);	// which voltage has Pin 2?
        adcmv[2] = ReadADC(TP3);	// which voltage has Pin 3?
        if (adcmv[0] < 1300) {
           ADC_DDR |= (1<<TP1);	// below 1.3V , switch directly with ADC-Port to GND
        }
        if (adcmv[1] < 1300) {
           ADC_DDR |= (1<<TP2);	// below 1.3V, switch directly with ADC-Port to GND
        }
        if (adcmv[2] < 1300) {
           ADC_DDR |= (1<<TP3);	// below 1.3V, switch directly with ADC-Port to GND
        }
        if ((adcmv[0] < (CAP_EMPTY_LEVEL+2)) && (adcmv[1] < (CAP_EMPTY_LEVEL+2)) && (adcmv[2] < (CAP_EMPTY_LEVEL+2))) {
           break;
        }
     }
     if (clr_cnt == MAX_ENTLADE_ZEIT) {
        PartFound = PART_CELL;	// mark as Battery
        // there is charge on capacitor, warn later!
     }
#if DebugOut == 99
        lcd_line4();
        u2lcd_space(adcmv[0]);	// lcd_string(utoa(adcmv[0], outval, 10)); lcd_space();
        u2lcd_space(adcmv[1]);	// lcd_string(utoa(adcmv[1], outval, 10)); lcd_space();
        u2lcd(adcmv[2]);		// lcd_string(utoa(adcmv[2], outval, 10));
#endif
     for(adcmv[0]=0;adcmv[0]<clr_cnt;adcmv[0]++) {
        // for safety, discharge 5% of discharge  time
        wait1ms();
     }
  } // end for lop_cnt
#if DebugOut == 99
  uart_putc('x');		//Debug
#endif
 }

