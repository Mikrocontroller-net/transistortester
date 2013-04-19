// new code by K.-H. Kübbeler
// ca   = Pin number (0-2) of the LowPin
// cb   = Pin number (0-2) of the HighPin

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
void GetVloss() {
#if FLASHEND > 0x1fff
  // measure voltage drop after load pulse
  unsigned int tmpint;
  unsigned int adcv[4];
  union t_combi{
  unsigned long dw;     // capacity value  in 100nF units
  uint16_t w[2];
  } lval;
  uint8_t ii;
  uint8_t HiPinR_L;
  uint8_t LoADC;

  if (cap.v_loss > 0) return;		// Voltage loss is already known
  LoADC = pgm_read_byte(&PinADCtab[cap.ca]) | TXD_MSK;
  HiPinR_L = pgm_read_byte(&PinRLtab[cap.cb]);	//R_L mask for HighPin R_L load

  EntladePins();			// discharge capacitor
  ADC_PORT = TXD_VAL;			// switch ADC-Port to GND
  R_PORT = 0;				// switch R-Port to GND
  ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
  R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (GND)
  adcv[0] = ReadADC(cap.cb);		// voltage before any load 
// ******** should adcv[0] be measured without current???
  if (cap.cpre_max > -9) return;	// too much capacity
  lval.dw = cap.cval_max;
//  for (ii=cap.cpre_max+12;ii<5;ii++) {
  for (ii=cap.cpre_max+12;ii<4;ii++) {
     lval.dw = (lval.dw + 5) / 10;
  }
//  if ((lval.dw == 0) || (lval.dw > 500)) {
  if ((lval.dw == 0) || (lval.dw > 5000)) {
     /* capacity more than 50uF, Voltage loss is already measured  */
     return;
  }
  R_PORT = HiPinR_L;			//R_L to 1 (VCC) 
  R_DDR = HiPinR_L;			//switch Pin to output, across R to GND or VCC
  for (tmpint=0;tmpint<lval.w[0];tmpint+=2) {
//     wait50us();			// wait exactly 50us
     wait5us();			// wait exactly 5us
  }
  R_DDR = 0;				// switch back to input
  R_PORT = 0;			// no Pull up
 // wait10us();			//wait a little time
  wdt_reset();
  // read voltage without current
  ADCconfig.Samples = 5;		// set ADC to only 5 samples
  adcv[2] = ReadADC(cap.cb);
  if (adcv[2] > adcv[0]) {
     adcv[2] -= adcv[0];		//difference to beginning voltage
  } else {
     adcv[2] = 0;			// voltage is lower or same as beginning voltage
  }
  // wait 2x the time which was required for loading
  for (tmpint=0;tmpint<lval.w[0];tmpint++) {
//     wait50us();
     wait5us();
  }
  adcv[3] = ReadADC(cap.cb);	// read voltage again, is discharged only a little bit ?
  ADCconfig.Samples = ANZ_MESS;		// set ADC back to configured No. of samples
  wdt_reset();
  if (adcv[3] > adcv[0]) {
     adcv[3] -= adcv[0];		// difference to beginning voltage
  } else {
     adcv[3] = 0;			// voltage is lower or same as beginning voltage
  }
  if (adcv[2] > adcv[3]) {
     // build difference to load voltage
     adcv[1] = adcv[2] - adcv[3];	// lost voltage during load time wait
  } else {
     adcv[1] = 0;			// no lost voltage
  }
  // compute voltage drop as part from loaded voltage
  if (adcv[1] > 0) {
     // there is any voltage drop (adcv[1]) !
     // adcv[2] is the loaded voltage.
     cap.v_loss = (unsigned long)(adcv[1] * 500UL) / adcv[2];
  }
#if 0
  lcd_line3();
  DisplayValue(adcv[2],0,' ',4);
  DisplayValue(adcv[1],0,' ',4);
  lcd_line4();
  DisplayValue(lval.w[0],0,'x',4);
#endif

  // discharge capacitor again
  EntladePins();		// discharge capacitors
  //ready
  // switch all ports to input
  ADC_DDR =  TXD_MSK;		// switch all ADC ports to input
  ADC_PORT = TXD_VAL;		// switch all ADC outputs to GND, no pull up
  R_DDR = 0;			// switch all resistor ports to input
  R_PORT = 0; 			// switch all resistor outputs to GND, no pull up
#endif
  return;
 } // end GetVdrop()

