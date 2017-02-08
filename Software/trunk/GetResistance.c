#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

//******************************************************************
void GetResistance(uint8_t HighPin, uint8_t LowPin)
  {
  /*
  Function for checking the resistance of a component with the following pin assignment 
  parameters:
  HighPin: Pin, which will be switched to VCC at the beginning
  LowPin: Pin, which will be switch to GND at the beginning
*/
  struct {
     unsigned int hp1;
     unsigned int hp2;
     unsigned int lp1;
     unsigned int lp2;
     unsigned int tp1;
     unsigned int tp2;
  }adc;

uint8_t LoPinRL;		// mask to switch the LowPin with R_L
uint8_t LoPinRH;		// mask to switch the LowPin with R_H
uint8_t HiPinRL;		// mask to switch the HighPin with R_L
uint8_t HiPinRH;		// mask to switch the HighPin with R_H
uint8_t LoADCp;			// mask to switch the ADC port LowPin
uint8_t HiADCp;			// mask to switch the ADC port HighPin

#if FLASHEND > 0x1fff
  uint8_t ii;			// temporary variable
  int udiff;
#endif


  unsigned long lrx1;
  unsigned long lirx1;
  unsigned long lirx2;
  const uint8_t	*addr;
  uint8_t resnum;
  /*
    switch HighPin directls to VCC 
    switch R_L port for LowPin to GND 
    TristatePin remains switched to input , no action required 
  */
  wdt_reset();
  addr = &PinRLRHADCtab[LowPin-TP_MIN];		// address of the combined RL / RH / ADC pin table
  LoPinRL = pgm_read_byte(addr);		// instruction for LowPin R_L
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
  addr += 3;			// address of PinRHtab[LowPin]
  LoPinRH = pgm_read_byte(addr);		// instruction for LowPin R_H
#else
  LoPinRH = (LoPinRL + LoPinRL);
#endif
  addr += 3;			// address of PinADCtab[LowPin]
  LoADCp = pgm_read_byte(addr);		// instruction for ADC Low-Pin, including | TXD_VAL

  addr = &PinRLRHADCtab[HighPin-TP_MIN];
  HiPinRL = pgm_read_byte(addr);		// instruction for HighPin R_L
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
  addr += 3;			// address of PinRLtab[HighPin]
  HiPinRH = pgm_read_byte(addr);		// instruction for HighPin R_H
#else
  HiPinRH = (HiPinRL + HiPinRL);
#endif
  addr += 3;			// address of PinADCtab[HighPin]
  HiADCp = pgm_read_byte(addr);		// instruction for ADC High-Pin, including | TXD_VAL

//##########################################################################################
// Search for resistors
//##########################################################################################
  if ((ptrans.count  + ntrans.count) > 0) {
     return;	// no resistors are searched, if transistors are detected
  }
  resnum = (LowPin - TP_MIN + HighPin - TP_MIN - 1);	// resistor-Number 0:1 = 0, 0:2 = 1, 1:2 = 2
  // resistor measurement
  wdt_reset();
// U_SCALE can be set to 4 for better resolution of ReadADC result
#if U_SCALE != 1
  ADCconfig.U_AVCC = (adc_vcc_reference * U_SCALE);	// scale to higher resolution, mV scale is not required
  ADCconfig.U_Bandgap = (adc_internal_reference * U_SCALE);
#endif
#if R_ANZ_MESS != ANZ_MESS
  ADCconfig.Samples = R_ANZ_MESS;	// switch to special number of repetitions
#endif
  #define MAX_REPEAT (700 / (5 + R_ANZ_MESS/8))
  ADC_PORT = TXD_VAL;
  ADC_DDR = LoADCp | TXD_MSK;		//switch Low-Pin to output (GND)
  R_DDR = HiPinRL;		//switch R_L port for High-Pin to output (VCC)
  R_PORT = HiPinRL;	
#if FLASHEND > 0x1fff
  adc.hp2 = 0;
  for (ii=1;ii<MAX_REPEAT;ii++) {
     // wait until voltage is stable
     adc.tp1 = W5msReadADC(LowPin);	// low-voltage at Rx with load
     adc.hp1 = ReadADC(HighPin);		// voltage at resistor Rx with R_L
     udiff = adc.hp1 - adc.hp2;
     if (udiff < 0) udiff = -udiff;
     if (udiff < 3) break;
     adc.hp2 = adc.hp1;
     wdt_reset();
  }
  if (ii == MAX_REPEAT) {
 #if (DEBUG_OUT == 1)
     lcd_data('a');
 #endif
     goto testend;
  }
#else
  adc.tp1 = W5msReadADC(LowPin);	// low-voltage at Rx with load
  adc.hp1 = ReadADC(HighPin);		// voltage at resistor Rx with R_L
#endif
  if (adc.tp1 > adc.hp1) {
     adc.tp1 = adc.hp1;
  }
  R_PORT = 0;
  R_DDR = HiPinRH;		//switch R_H port for High-Pin to output (GND)
  adc.hp2 = W5msReadADC(HighPin);	// read voltage, should be down
  if (adc.hp2 > (20*U_SCALE)) {
     // if resistor, voltage should be down
 #if (DEBUG_OUT == 1)
     lcd_data('b');
 #endif
     goto testend;
  }
  R_PORT = HiPinRH;		//switch R_H for High-Pin to VCC
  adc.hp2 = W5msReadADC(HighPin);	// voltage at resistor Rx with R_H

  ADC_DDR = HiADCp | TXD_MSK;		//switch High-Pin to output
  ADC_PORT = HiADCp;		//switch High-Pin to VCC
  R_PORT = 0;
  R_DDR = LoPinRL;			//switch R_L for Low-Pin to GND
#if FLASHEND > 0x1fff
  adc.lp2 = 0;
  for (ii=1;ii<MAX_REPEAT;ii++) {
     // wait until voltage is stable
     adc.tp2 = W5msReadADC(HighPin);	//high voltage with load
     adc.lp1 = ReadADC(LowPin);		//voltage at the other end of Rx
     udiff = adc.lp1 - adc.lp2;
     if (udiff < 0) udiff = -udiff;
     if (udiff < 3) break;
     adc.lp2 = adc.lp1;
     wdt_reset();
  }
  if (ii == MAX_REPEAT) {
 #if (DEBUG_OUT == 1)
     lcd_data('c');
 #endif
     goto testend;
  }
#else
  adc.tp2 = W5msReadADC(HighPin);	//high voltage with load
  adc.lp1 = ReadADC(LowPin);		//voltage at the other end of Rx
#endif
  if (adc.tp2 < adc.lp1) {
     adc.tp2 = adc.lp1;
  }
  R_DDR = LoPinRH;			//switch R_H for Low-Pin to GND
  adc.lp2 = W5msReadADC(LowPin);
		
  if((adc.hp1 < (4400*U_SCALE)) && (adc.hp2 > (97*U_SCALE))) {
     //voltage break down isn't insufficient 
 #if (DEBUG_OUT == 1)
     lcd_data('d');
 #endif
     goto testend; 
  }
//    if((adc.hp2 + (adc.hp2 / 61)) < adc.hp1)
  if (adc.hp2 < (4972*U_SCALE)) { 
     // voltage breaks down with low test current and it is not nearly shorted  => resistor
//     if (adc.lp1 < 120)  // take measurement with R_H 
     if (adc.lp1 < (169*U_SCALE)) { // take measurement with R_H 
        if (adc.lp2 < (38*U_SCALE)) {
           // measurement > 60MOhm too big resistance
 #if (DEBUG_OUT == 1)
     lcd_data('e');
 #endif
           goto testend;
        }
        // two measurements with R_H resistors (470k) are made:
        // lirx1 (measurement at HighPin)
        lirx1 = (unsigned long)((unsigned int)R_H_VAL) * (unsigned long)adc.hp2 / (ADCconfig.U_AVCC - adc.hp2);
        // lirx2 (measurement at LowPin)
        lirx2 = (unsigned long)((unsigned int)R_H_VAL) * (unsigned long)(ADCconfig.U_AVCC - adc.lp2) / adc.lp2;
#define U_INT_LIMIT (990*U_SCALE)		// 1V switch limit in ReadADC for atmega family
#ifdef __AVR_ATmega8__
#define FAKT_LOW 2		//resolution is about twice as good
#else
#define FAKT_LOW 4		//resolution is about four times better
#endif
#ifdef AUTOSCALE_ADC
        if (adc.hp2 < U_INT_LIMIT) {
           lrx1 = (lirx1*FAKT_LOW + lirx2) / (FAKT_LOW+1);	//weighted average of both R_H measurements
        } else if (adc.lp2 < U_INT_LIMIT){
           lrx1 = (lirx2*FAKT_LOW + lirx1) / (FAKT_LOW+1);	//weighted average of both R_H measurements
        } else 
#endif
        {
           lrx1 = (lirx1 + lirx2) / 2;		//average of both R_H measurements
        }
        lrx1 *= 100;
        lrx1 += RH_OFFSET;			// add constant for correction of systematic error
     } else {
        // two measurements with R_L resistors (680) are made:
        // lirx1 (measurement at HighPin)
        if (adc.tp1 > adc.hp1) {
           adc.hp1 = adc.tp1;		//diff negativ is illegal
        }
        lirx1 =(unsigned long)RR680PL * (unsigned long)(adc.hp1 - adc.tp1) / (ADCconfig.U_AVCC - adc.hp1);
        if (adc.tp2 < adc.lp1) {
           adc.lp1 = adc.tp2;		//diff negativ is illegal
        }
        // lirx2 (Measurement at LowPin)
        lirx2 =(unsigned long)RR680MI * (unsigned long)(adc.tp2 -adc.lp1) / adc.lp1;

#ifdef AUTOSCALE_ADC
        if (adc.hp1 < U_INT_LIMIT) {
           lrx1 = (lirx1*FAKT_LOW + lirx2) / (FAKT_LOW+1);	//weighted average of both R_L measurements
        } else if (adc.lp1 < U_INT_LIMIT) {
           lrx1 = (lirx2*FAKT_LOW + lirx1) / (FAKT_LOW+1);	//weighted average of both R_L measurements
        } else
#endif
        {
           lrx1 = (lirx1 + lirx2) / 2;		//average of both R_L measurements
        }
     }
     // measurement is finished, lrx1 is the resistance value of one direction
     if(PartFound < PART_TRANSISTOR) {
        if (ResistorChecked[resnum] != 0) {
           // must be measurement with inverse polarity 
           // resolution is 0.1 Ohm, 1 Ohm = 10 !
           lirx1 = (labs((long)lrx1 - (long)ResistorVal[resnum]) * 10) / (lrx1 + ResistorVal[resnum] + 100);
           if (lirx1  > 0) {
              // mismatch of the two measurements
//              ResistorsFound--;		// this one isn't a resistor
//              goto testend; // <10% mismatch
 #if (DEBUG_OUT == 1)
     lcd_line3();
     DisplayValue(lrx1,-1,' ',4);
     DisplayValue(ResistorVal[resnum],-1,' ',4);
 #endif
           } else {
              // resistor has the same value in both directions
              if (PartFound < PART_DIODE) {
                 PartFound = PART_RESISTOR;	// only mark as resistor, if no other part found
              }
              ResistorChecked[resnum] = 2;		// mark as checked in both direction
              ResistorList[ResistorsFound] = resnum;	// save number of this resistor
              ResistorsFound++;			// 1 more resistor found
           }
           goto testend;
        } else {		// resistor is never checked before
           // must be a new one with other pins
           ResistorVal[resnum] = lrx1;	// save register value
           ResistorChecked[resnum] = 1;		// is checked in one direction

        } // end  ResistorChecked[] != 0
     }	/* end if (PartFound < PART_TRANSISTOR) */
  }
  testend:			// end of resistor measurement
#if U_SCALE != 1
  ADCconfig.U_AVCC = adc_vcc_reference;	// scale to higher resolution, mV scale is not required
  ADCconfig.U_Bandgap = adc_internal_reference;	// set back to normal resolution
#endif
#if R_ANZ_MESS != ANZ_MESS
  ADCconfig.Samples = ANZ_MESS;		// switch back to standard number of repetition
#endif

  return;
} // end GetResistance()

