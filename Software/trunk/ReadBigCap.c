// new code by K.-H. Kübbeler
// ReadBigCap tries to find the value of a capacitor by measuring the load time.
// first of all the capacitor is discharged.
// Then a series of up to 1000 load pulses with 1ms duration each is done across the R_L (680Ohm)
// resistor.
// After each load pulse the voltage of the capacitor is measured without any load current.
// If voltage reaches a value of more than 300mV and is below 1.3V, the capacity can be
// computed from load time and voltage by a interpolating a build in table.
// If the voltage reaches a value of more than 1.3V with only one load pulse,
// the cap value is too low.
// If the measurement the load time is successful,
// the following variables are set:
// cap.cval = value of the capacitor 
// cap.cval_uncorrected = value of the capacitor uncorrected
// cap.esr = serial resistance of capacitor,  0.01 Ohm units
// cap.cpre = units of cap.cval ( -9=nF, -6=µF)
// ca   = Pin number (0-2) of the LowPin
// cb   = Pin number (0-2) of the HighPin

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


#ifdef WITH_MENU
//=================================================================
void ReadBigCap(uint8_t HighPin, uint8_t LowPin) {
  // check if capacitor and measure the capacity value
  unsigned int adcv[4];
#ifdef INHIBIT_SLEEP_MODE
  unsigned int ovcnt16;
#endif
  uint8_t HiPinR_L;
  uint8_t LoADC;

#ifdef AUTO_CAL
  pin_combination = (HighPin * 3) + LowPin - 1;	// coded Pin combination for capacity zero offset
#endif

#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
  LoADC = pgm_read_byte((&PinRLRHADCtab[6])+LowPin) | TXD_MSK;
#else
  LoADC = pgm_read_byte((&PinRLRHADCtab[3])+LowPin) | TXD_MSK;
#endif
  HiPinR_L = pgm_read_byte(&PinRLRHADCtab[HighPin]);	//R_L mask for HighPin R_L load

#if FLASHEND > 0x1fff
  cap.esr = 0;				// set ESR of capacitor to zero
#endif
  cap.cval = 0;				// set capacity value to zero
  cap.cpre = -9;			//default unit is nF
  EntladePins();			// discharge capacitor
  ADC_PORT = TXD_VAL;			// switch ADC-Port to GND
  R_PORT = 0;				// switch R-Port to GND
  ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
//  R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (GND)
  R_DDR = 0;				// set all R Ports to input (no current)
  adcv[0] = ReadADC(HighPin);		// voltage before any load 
  adcv[2] = adcv[0];			// preset to prevent compiler warning
  
  ovcnt16 = 0;
#define MAX_LOAD_TIME 12500
#define MIN_VOLTAGE 300
  while (ovcnt16 < MAX_LOAD_TIME) {
     R_PORT = HiPinR_L;			//R_L to 1 (VCC) 
     if ((ovcnt16 == 0) || ((MIN_VOLTAGE-adcv[2]) < (adcv[2]*10/ovcnt16))) {
        R_DDR = HiPinR_L;		//switch Pin to output, across R to VCC
        wait200us();			// wait exactly 0.2ms, do not sleep
        R_DDR = 0;			// switch back to input
        ovcnt16++;
     } else if ((ovcnt16 > 10) && ((MIN_VOLTAGE-adcv[2]) > ((adcv[2]*100)/ovcnt16))){
        R_DDR = HiPinR_L;		//switch Pin to output, across R to VCC
        wait20ms();			// wait exactly 20ms, do not sleep
        R_DDR = 0;			// switch back to input
        ovcnt16 += 100;
     } else {
        R_DDR = HiPinR_L;		//switch Pin to output, across R to VCC
        wait2ms();			// wait exactly 2ms, do not sleep
        R_DDR = 0;			// switch back to input
        ovcnt16 += 10;
     }
     R_PORT = 0;			// no Pull up
     wait50us();			//wait a little time
     wdt_reset();
     // read voltage without current, is already charged enough?
     adcv[2] = ReadADC(HighPin);
     if (adcv[2] > adcv[0]) {
        adcv[2] -= adcv[0];		//difference to beginning voltage
     } else {
        adcv[2] = 0;			// voltage is lower or same as beginning voltage
     }
     if ((ovcnt16 > (MAX_LOAD_TIME/8)) && (adcv[2] < (MIN_VOLTAGE/8))) {
        // 300mV can not be reached well-timed 
        break;		// don't try to load any more
     }
     // probably 50mF can be charged well-timed 
     if (adcv[2] > MIN_VOLTAGE) {
        break;		// lowest voltage to get capacity from load time is reached
     }
  }
  // wait 5ms and read voltage again, does the capacitor keep the voltage?
//  adcv[1] = W5msReadADC(HighPin) - adcv[0];
//  wdt_reset();
  if (adcv[2] <= MIN_VOLTAGE) {
     goto keinC;		// was never charged enough, >20mF or shorted
  }
  //voltage is rised properly and keeps the voltage enough
  if ((ovcnt16 == 1 ) && (adcv[2] > 1300)) {
     goto keinC;		// Voltage of more than 1300mV is reached in one pulse, too fast loaded
  }
  // Capacity is more than about 50µF
  cap.cval_uncorrected.dw = ovcnt16*2;
  // compute factor with load voltage + lost voltage during the voltage load time
  cap.cval_uncorrected.dw *= GetRLmultip(adcv[2]);	// get factor to convert time to capacity from table
   cap.cval = cap.cval_uncorrected.dw;	// set result to uncorrected
   Scale_C_with_vcc();
   // cap.cval for this type is at least 40000nF, so the last digit will be never shown
   cap.cval -= ((cap.cval * C_H_KORR) / 1000);	// correct with C_H_KORR with 0.1% resolution, but prevent overflow
   cap.cval /= 10;

//==================================================================================

   PartFound = PART_CAPACITOR;	//capacitor is found
      cap.cval_max = cap.cval;
      cap.cpre_max = cap.cpre;
#if FLASHEND > 0x1fff
      cap.v_loss = 0;			// set lost voltage to zero
#endif
      cap.ca = LowPin;		// save LowPin
      cap.cb = HighPin;		// save HighPin

keinC:
  // discharge capacitor again
//  EntladePins();		// discharge capacitors
  //ready
  // switch all ports to input
  ADC_DDR =  TXD_MSK;		// switch all ADC ports to input
  ADC_PORT = TXD_VAL;		// switch all ADC outputs to GND, no pull up
  R_DDR = 0;			// switch all resistor ports to input
  R_PORT = 0; 			// switch all resistor outputs to GND, no pull up
  return;
 } // end ReadBigCap()

#endif
