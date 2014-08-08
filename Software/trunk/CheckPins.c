#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"

//******************************************************************
void CheckPins(uint8_t HighPin, uint8_t LowPin, uint8_t TristatePin)
  {
  /*
  Function for checking the characteristic of a component with the following pin assignment 
  parameters:
  HighPin: Pin, which will be switched to VCC at the beginning
  LowPin: Pin, which will be switch to GND at the beginning
  TristatePin: Pin, which will be undefined at the beginning
  TristatePin will be switched to GND and VCC also .
*/
  struct {
     unsigned int lp_otr;
     unsigned int vCEs;
     unsigned int hp1;
     unsigned int hp2;
     unsigned int hp3;
     unsigned int hp4;
     unsigned int rhp;
     unsigned int lp1;
     unsigned int lp2;
     unsigned int tp1;
     unsigned int tp2;
     unsigned int rtp;
  }adc;
  uint8_t LoPinRL;		// mask to switch the LowPin with R_L
  uint8_t LoPinRH;		// mask to switch the LowPin with R_H
  uint8_t TriPinRL;		// mask to switch the TristatePin with R_L
  uint8_t TriPinRH;		// mask to switch the TristatePin with R_H
  uint8_t HiPinRL;		// mask to switch the HighPin with RL
  uint8_t HiPinRH;		// mask to switch the HighPin with R_H
  uint8_t HiADCp;		// mask to switch the ADC port High-Pin
  uint8_t LoADCp;		// mask to switch the ADC port Low-Pin
  uint8_t HiADCm;		// mask to switch the ADC DDR port High-Pin
  uint8_t LoADCm;		// mask to switch the ADC DDR port Low-Pin
  uint8_t PinMSK;
  uint8_t update_pins;		// flag for updating the trans.ebc pins, 1=update
  uint8_t ii;			// temporary variable
  unsigned int tmp16;		// temporary variable
#ifdef COMMON_EMITTER
  unsigned long e_hfe;		// current amplification factor with common emitter
#else
 #warning "hFE measurement without common emitter circuit"
#endif
#ifndef COMMON_COLLECTOR
 #warning "hFE measurement without common collector circuit"
#endif
#if FLASHEND > 0x1fff
  int udiff;
#endif

#ifdef COMMON_COLLECTOR
  unsigned long c_hfe;		// amplification factor for common Collector (Emitter follower)
#endif

  struct resis_t *thisR;
  unsigned int volt_dif;
  unsigned long lrx1;
  unsigned long lirx1;
  unsigned long lirx2;
  /*
    switch HighPin directls to VCC 
    switch R_L port for LowPin to GND 
    TristatePin remains switched to input , no action required 
  */
  wdt_reset();
//#ifdef AUTO_CAL
//  uint16_t resis680pl;
//  uint16_t resis680mi;
//  resis680pl = eeprom_read_word(&R680pl);
//  resis680mi = eeprom_read_word(&R680mi);
//  #define RR680PL resis680pl
//  #define RR680MI resis680mi
//#else
//  #define RR680PL (R_L_VAL + PIN_RP)
//  #define RR680MI (R_L_VAL + PIN_RM)
//#endif
  LoPinRL = pgm_read_byte(&PinRLtab[LowPin]);		// instruction for LowPin R_L
#if FLASHEND > 0x3fff
  LoPinRH = pgm_read_byte(&PinRHtab[LowPin]);		// instruction for LowPin R_H
#else
  LoPinRH = LoPinRL + LoPinRL;				// instruction for LowPin R_H
#endif
  TriPinRL = pgm_read_byte(&PinRLtab[TristatePin]);	// instruction for TristatePin R_L
#if FLASHEND > 0x3fff
  TriPinRH = pgm_read_byte(&PinRHtab[TristatePin]);	// instruction for TristatePin R_H
#else
  TriPinRH = TriPinRL + TriPinRL;			// instruction for TristatePin R_H
#endif
  HiPinRL = pgm_read_byte(&PinRLtab[HighPin]);		// instruction for HighPin R_L
#if FLASHEND > 0x3fff
  HiPinRH = pgm_read_byte(&PinRHtab[HighPin]);		// instruction for HighPin R_H
#else
  HiPinRH = HiPinRL + HiPinRL;				// instruction for HighPin R_H
#endif

  HiADCp = pgm_read_byte(&PinADCtab[HighPin]);		// instruction for ADC High-Pin, including | TXD_VAL 
  LoADCp = pgm_read_byte(&PinADCtab[LowPin]);		// instruction for ADC Low-Pin, including | TXD_VAL
  HiADCm = HiADCp | TXD_MSK;
  LoADCm = LoADCp | TXD_MSK;

  //setting of Pins 
#if (PROCESSOR_TYP != 8)
 #define EXACT_OTR
 // with option EXACT_OTR the vCE0 and vCEs is determined with common emitter circuit for
 // PNP and NPN transistors. Without this option set the circuit is common emitter for PNP and
 // common collector for NPN.
#endif
#ifndef EXACT_OTR
  R_PORT = 0;				//resistor-Port outputs to 0
  R_DDR = LoPinRL;			//Low-Pin to output and across R_L to GND
  ADC_DDR = HiADCm;		//High-Pin to output
  ADC_PORT = HiADCp;		//High-Pin fix to Vcc
  //for some MOSFET the gate (TristatePin) must be discharged
  R_DDR = LoPinRL | TriPinRL;
  adc.vCEs = W5msReadADC(LowPin);	// lp1 is the voltage at 680 Ohm with - Gate
  R_DDR = LoPinRL;	
  adc.lp_otr = W5msReadADC(LowPin);	//read voltage of Low-Pin  , without Gate current (-)
  R_DDR = 0;
  wait5ms();				// release all current (clear TRIAC and Thyristor)
  R_PORT = TriPinRL;
  R_DDR = LoPinRL | TriPinRL;		// start current again
  adc.lp2 = W5msReadADC(LowPin);	// lp2 is the voltage at 680 Ohm with + Gate
  R_DDR = LoPinRL;
  R_PORT = 0;
  if (adc.lp2 < adc.vCEs) {
     adc.lp_otr = W5msReadADC(LowPin);	//read voltage of Low-Pin again, without Gate current (+)
     if ((adc.lp2+288) > adc.vCEs) goto checkDiode;	// no significant change
     adc.vCEs = adc.lp2;
  } else {
     if ((adc.vCEs+288) > adc.lp2) goto checkDiode;	// no significant change
  }
#else
  R_PORT = TriPinRL;			//resistor-Port TriState to 1
  R_DDR = LoPinRL | TriPinRL;		// resistor-Port Low-Pin to 0
  ADC_DDR = HiADCm;		//High-Pin to output
  ADC_PORT = HiADCp;		//High-Pin fix to Vcc
  //for some MOSFET the gate (TristatePin) must be discharged
  adc.vCEs = W5msReadADC(LowPin);	// lp1 is the voltage at 680 Ohm with + Gate
  R_DDR = LoPinRL;	
  adc.lp_otr = W5msReadADC(LowPin);	//read voltage of Low-Pin  , without Gate current (+)
  R_DDR = 0;
  wait5ms();				// release all current (clear TRIAC and Thyristor)
  R_PORT = 0;
  R_DDR = LoPinRL | TriPinRL;		// start current again
  adc.lp2 = W5msReadADC(LowPin);	// lp2 is the voltage at 680 Ohm with - Gate
  if (adc.lp2 < adc.vCEs) {
     // current is lower with Gate switched to 0
     if ((adc.lp2+288) > adc.vCEs) goto checkDiode;	// no significant change
     // switch to common emitter for NPN or N-channel FET
     ADC_DDR = LoADCm;		//Low-Pin to output
     ADC_PORT = TXD_VAL;		//Low-Pin fix to GND
     R_PORT = HiPinRL;			//resistor-Port High-Pin to +, TriState to 0
     R_DDR = HiPinRL | TriPinRL;	// resistor-Port High-Pin to 1
     adc.vCEs = ADCconfig.U_AVCC - W5msReadADC(HighPin);	// voltage at 680 Ohm with - Gate
     R_DDR = HiPinRL;			// resistor-Port High-Pin to 1, TriState open
     adc.lp_otr = ADCconfig.U_AVCC - W5msReadADC(HighPin); // voltage at 680 Ohm with open Gate
  } else {
     if ((adc.vCEs+288) > adc.lp2) goto checkDiode;	// no significant change
  }
#endif

//  ChargePin10ms(TriPinRL,0);		//discharge for N-Kanal
//  adc.lp_otr = W5msReadADC(LowPin);	//read voltage of Low-Pin 
//  if(adc.lp_otr >= 977) {		//no current now? 
//     ChargePin10ms(TriPinRL,1);	 	//else: discharge for P-channel (Gate to VCC)
//     adc.lp_otr = ReadADC(LowPin);	//read voltage of Low-Pin again
//  }

#if DebugOut == 5
  lcd_line2();
  lcd_clear_line();
  lcd_line2();
#endif
  update_pins = 1;			// if a part is found, always update the pins
  // the tester will never find more that two transistors
  // a TRIAC is marked as two transistors at least (2 or 3)
  // both of NPN transistors (normal and inverse) are found, if ntrans.count == 2
  // both of PNP transistors (normal and inverse) are found, if ptrans.count == 2
  // If Transistor with protection diode is checked, all results are found,
  // if ntrans.count == 1 and ptrans.count == 1
  if ((ntrans.count + ptrans.count) > 1) {
     // all transistors found, no more search is needed
     // but TRIAC can be detected as NPNp with same pins as PNPn
     if (!((ntrans.count == 1) && (ntrans.b == ptrans.b))) {
       goto checkDiode;
     }
  }
//  if(adc.lp_otr > 92) {  //there is some current without TristatePin current 
  // Look for Source/Drain current without gate current.
  // Germanium transistors have also high collector current with higher temperature.
  // But this current is lower, if the base is hold at emitter level (adc.hp1).
  // Additional checking of adc.lp1 will prevent to detect a PNP as P-JFET.
//  if((PartMode <= PART_MODE_P_JFET) && (adc.lp_otr > 455)  && (adc.vCEs > 455))
  
//  if((adc.lp_otr > 455)  && (adc.vCEs > 455))
//  if((adc.vCEs > 115)  && ((adc.vCEs+100) > adc.lp_otr))
  if((adc.vCEs > 115)  && ((adc.vCEs+adc.vCEs+20) > adc.lp_otr))
     {  //there is more than 650uA current without TristatePin current 
     // can be JFET or D-FET
     //Test if N-JFET or if self-conducting N-MOSFET
#ifdef EXACT_OTR
     ADC_DDR = HiADCm;		//High-Pin to output
     ADC_PORT = HiADCp;		//High-Pin fix to Vcc
#endif
     R_DDR = LoPinRL | TriPinRH;	//switch R_H for Tristate-Pin (probably Gate) to GND
     adc.lp1 = W10msReadADC(LowPin);	//measure voltage at the assumed Source 
     adc.tp1 = ReadADC(TristatePin);	// measure Gate voltage
     R_PORT = TriPinRH;			//switch R_H for Tristate-Pin (probably Gate) to VCC
     adc.lp2 = W10msReadADC(LowPin);	//measure voltage at the assumed Source again
#if DebugOut == 5
     lcd_testpin(LowPin);
     lcd_data('f');
     lcd_testpin(HighPin);
     lcd_space();
     DisplayValue(adc.lp2,-3,' ',3);
     DisplayValue(adc.lp1,-3,' ',3);
#endif
     //If it is a self-conducting MOSFET or JFET, then must be: adc.lp2 > adc.lp1 
     if(adc.lp2>(adc.lp1+599)) {
        //measure voltage at the  Gate, differ between MOSFET and JFET
        ADC_PORT = TXD_VAL;
        ADC_DDR = LoADCm;	//Low-Pin fix to GND
        R_DDR = TriPinRH | HiPinRL;	//High-Pin to output
        R_PORT = TriPinRH | HiPinRL;	//switch R_L for High-Pin to VCC
        adc.lp2 = W10msReadADC(TristatePin); //read voltage of assumed Gate 
        if(adc.lp2>3911) {  //MOSFET
           PartFound = PART_FET;	//N-Kanal-MOSFET
           PartMode = PART_MODE_MOS|N_CHANNEL|D_MODE; //Depletion-MOSFET
        } else {  //JFET (pn-passage between Gate and Source is conducting )
           PartFound = PART_FET;	//N-Kanal-JFET
           PartMode = PART_MODE_JFET|N_CHANNEL|D_MODE;
        }
#if DebugOut == 5
        lcd_data('N');
        lcd_data('J');
#endif
//      if ((PartReady == 0) || (adc.lp1 > ntrans.uBE)) 
//      there is no way to find out the right Source / Drain
        ntrans.uBE = adc.lp1;
        if(adc.lp1 > adc.tp1) {
          ntrans.gthvoltage = adc.lp1 - adc.tp1;	//voltage GS (Source - Gate)
        } else {
          ntrans.gthvoltage = 0;	//voltage GS (Source - Gate)
        }
        ntrans.current = (unsigned int)(((unsigned long)adc.lp1 * 1000) / RR680MI); // Id 0.01mA
        ntrans.count++;			// count as two, the inverse is identical
        goto saveNresult;		// save Pin numbers and exit
     }

     ADC_PORT = TXD_VAL;		// direct outputs to GND

     //Test, if P-JFET or if self-conducting P-MOSFET
     ADC_DDR = LoADCm;		//switch Low-Pin (assumed Drain) direct to GND,
				//R_H for Tristate-Pin (assumed Gate) is already switched to VCC
     R_DDR = TriPinRH | HiPinRL;	//High-Pin to output
     R_PORT = TriPinRH | HiPinRL;	//High-Pin across R_L to Vcc
     adc.hp1 = W10msReadADC(HighPin);	//measure voltage at assumed Source 
     adc.tp1 = ReadADC(TristatePin);	// measure Gate voltage
     R_PORT = HiPinRL;			//switch R_H for Tristate-Pin (assumed Gate) to GND
     adc.hp2 = W10msReadADC(HighPin);	//read voltage at assumed Source again
     //if it is a self-conducting P_MOSFET or P-JFET , then must be:  adc.hp1 > adc.hp2 
     if(adc.hp1>(adc.hp2+599)) {
        //read voltage at the Gate , to differ between MOSFET and JFET
        ADC_PORT = HiADCp;	//switch High-Pin directly to VCC
        ADC_DDR = HiADCm;	//switch High-Pin to output
        adc.tp2 = W10msReadADC(TristatePin); //read voltage at the assumed Gate 
        if(adc.tp2<977) { 		//MOSFET
           PartFound = PART_FET;	//P-Kanal-MOSFET
           PartMode = PART_MODE_MOS|P_CHANNEL|D_MODE; //Depletion-MOSFET
        } else { 			//JFET (pn-passage between Gate and Source is conducting)
           PartFound = PART_FET;	//P-Kanal-JFET
           PartMode = PART_MODE_JFET|P_CHANNEL|D_MODE;
        }
#if DebugOut == 5
        lcd_data('P');
        lcd_data('J');
#endif
        if(adc.tp1 > adc.hp1) {
          ptrans.gthvoltage = adc.tp1 - adc.hp1;	//voltage GS (Gate - Source)
        } else {
          ptrans.gthvoltage = 0;
        }
        ptrans.current = (unsigned int)(((unsigned long)(ADCconfig.U_AVCC - adc.hp1) * 1000) / RR680PL); // Id 0.01mA
        ptrans.count++;			// count as two, the inverse is identical
        goto savePresult;		// save pin numbers and exit
     }
     // no JFET od D-MOS
     goto checkDiode;	
  } // end component has current without TristatePin signal


  //there is more than 650uA current without TristatePin current 
#ifdef COMMON_COLLECTOR
  // Test circuit with common collector (Emitter follower) PNP
  ADC_PORT = TXD_VAL;
  ADC_DDR = LoADCm;			// Collector direct to GND
  R_PORT = HiPinRL;			// switch R_L port for HighPin (Emitter) to VCC
  R_DDR = TriPinRL | HiPinRL;		// Base resistor  R_L to GND
  adc.hp1 = W5msReadADC(HighPin);	// voltage at the Emitter resistor
  adc.rhp = ADCconfig.U_AVCC - adc.hp1;	// voltage at the Emitter resistor
  adc.tp1 = ReadADC(TristatePin);	// voltage at the base resistor

  if (adc.tp1 < 10) {
     R_DDR = 0;
     wait_about5ms();		// clear TRIAC and Thyristor
     R_DDR =  TriPinRH | HiPinRL;	// Tripin=RH-
     adc.hp1 = W5msReadADC(HighPin);
     adc.rhp = ADCconfig.U_AVCC - adc.hp1;	// voltage at the Emitter resistor
     adc.tp1 = ReadADC(TristatePin);	// voltage at base resistor 
 #ifdef LONG_HFE
     c_hfe = ((unsigned long)adc.rhp * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680PL)) / (unsigned int)adc.tp1;	
 #else
     c_hfe = ((adc.rhp / ((RR680PL+500)/1000)) * (R_H_VAL/500)) / (adc.tp1/500);
 #endif
  } else {
     c_hfe = (unsigned long)((adc.rhp - adc.tp1) / adc.tp1);
  }
#endif

  //set Pins again for circuit with common Emitter PNP
  R_DDR = 0;			//all Resistor Ports to Input
  R_PORT = 0;			//switch all resistor ports to GND
  ADC_PORT = HiADCp;		//switch High-Pin to VCC
  ADC_DDR = HiADCm;		//switch High-Pin to output
//  R_DDR = LoPinRL;		//switch R_L port for Low-Pin to output (GND)
  wait_about5ms();
  
  if(adc.lp_otr < 1977) {
     //if the component has no connection between  HighPin and LowPin
#if DebugOut == 5
     lcd_testpin(LowPin);
     lcd_data('E');
     lcd_testpin(HighPin);
     lcd_space();
#endif
     //Test to PNP
     R_DDR = LoPinRL | TriPinRL;	//switch R_L port for Tristate-Pin to output (GND), for Test of PNP
     adc.lp1 = W5msReadADC(LowPin);	//measure voltage at LowPin
     if(adc.lp1 > 3422) {
        //component has current => PNP-Transistor or equivalent
        R_DDR = 0;
        wait_about5ms();		// clear TRIAC and Thyristor
        //compute current amplification factor in both directions
        R_DDR = LoPinRL | TriPinRH;	//switch R_H port for Tristate-Pin (Base) to output (GND)

        adc.lp1 = W5msReadADC(LowPin);	//measure voltage at LowPin (assumed Collector)
        adc.tp2 = ReadADC(TristatePin);	//measure voltage at TristatePin (Base) 
        adc.hp2 = ReadADC(HighPin);	//measure voltage at HighPin (assumed Emitter)

        if(adc.tp2 > 2000) {
           //PNP-Transistor is found (Base voltage moves with Emitter to VCC)
           PartFound = PART_TRANSISTOR;
           PartMode = PART_MODE_PNP;
           update_pins = 0;		// only update pins, if hFE is higher or Thyristor 
#ifdef COMMON_EMITTER
           //compute current amplification factor for circuit with common Emitter
           //e_hFE = B = Collector current / Base current
           tmp16 = adc.lp1;
           if (tmp16 > adc.lp_otr) {
              tmp16 -= adc.lp_otr;
           }

 #ifdef LONG_HFE
           e_hfe = ((unsigned int)tmp16 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680MI)) / (unsigned int)adc.tp2;	
 #else
           e_hfe = ((tmp16 / ((RR680MI+500)/1000)) * (R_H_VAL/500)) / (adc.tp2/500);
 #endif
           // first hFE or e_hfe is greater than last hfe ?
           if ((ptrans.count == 0) || (e_hfe > ptrans.hfe)){
              ptrans.hfe = e_hfe;				// hFE with common emitter
              ptrans.uBE = adc.hp2 - adc.tp2;	// Base Emitter Voltage
              update_pins = 1;		// trans.ebc must be updated
           }
#endif
#ifdef COMMON_COLLECTOR
           //current amplification factor for common  Collector (Emitter follower)
           // c_hFE = (Emitter current - Base current) / Base current
 #ifdef COMMON_EMITTER
           // also with COMMON_EMITTER, is c_hfe greater than the last hFE?
           if (c_hfe > ptrans.hfe)  // trans.hfe is allready e_hfe or last c_hFE
 #else
           // without COMMON_EMITTER , c_hFE is first or greater than the old one?
           if ((ptrans.count == 0) || (c_hfe > ptrans.hfe)) 
 #endif
           {
              ptrans.hfe = c_hfe;		// c_hfe is the best
              ptrans.uBE = adc.hp1 - adc.tp1;	// Base Emitter Voltage common collector
              update_pins = 1;		// trans.ebc must be updated
           }
#endif
#if DebugOut == 5
           lcd_data('B');
#endif
#ifdef SHOW_ICE
           if (update_pins != 0) {
	      // update residual collector current without base current
              ptrans.ice0 = (unsigned int)(((unsigned long)adc.lp_otr * 1000) / RR680MI); // ICE0 0.01mA
              ptrans.ices = (unsigned int)(((unsigned long)adc.vCEs * 1000) / RR680MI); // ICEs 0.01mA
           }
#endif
           goto savePresult;		// marke P type, save Pins and exit
        }
        // is probably a P-E-MOS, check voltage
        if((adc.lp_otr < 97) && (adc.lp1 > 2000)) {
           //is flow voltage low enough in the closed  state?
           //(since D-Mode-FET would be by mistake detected as E-Mode )
           PartFound = PART_FET;		//P-Kanal-MOSFET is found (Basis/Gate moves not to VCC)
           PartMode = PART_MODE_MOS|P_CHANNEL|E_MODE;
           if (adc.hp2 > (adc.lp1+250)) {
              //Drain-Source Voltage to high, must be a IGBT
              PartMode = PART_MODE_IGBT|P_CHANNEL|E_MODE;
           }
       	   //measure the Gate threshold voltage
           //Switching of Drain is monitored with digital input
           // Low level is specified up to 0.3 * VCC
           // High level is specified above 0.6 * VCC
           PinMSK = LoADCm & 7;
       	   ADMUX = TristatePin | (1<<REFS0);	// switch to TristatePin, Ref. VCC
       	   tmp16 = 1;			// round up ((1*4)/9)
       	   for(ii=0;ii<11;ii++) {
       	      wdt_reset();
       	      ChargePin10ms(TriPinRL,1);
                R_DDR = LoPinRL | TriPinRH;		//switch R_H for Tristate-Pin (Basis) to GND
       	      while (!(ADC_PIN&PinMSK));		// Wait, until the MOSFET switches and Drain moves to VCC
                  			// 1 is detected with more than 2.5V (up to 2.57V) with tests of mega168 and mega328
       	      R_DDR = LoPinRL;
       	      ADCSRA |= (1<<ADSC);		// Start Conversion
       	      while (ADCSRA&(1<<ADSC));		// wait
      	      tmp16 += (1023 - ADCW);	// Add Tristatepin-Voltage
           }
           tmp16 *= 4;		// is equal to 44*ADCW
           ptrans.gthvoltage = tmp16 / 9;		// gives resolution in mV
           ptrans.count++;		// count FET as two for accelerate searching
#if DebugOut == 5
           lcd_data('F');
#endif
           goto savePresult;
        }
    } // end component has current => PNP

#ifdef COMMON_COLLECTOR
    // Low-Pin=RL- HighPin=VCC
    R_DDR = LoPinRL | TriPinRL;
    R_PORT = TriPinRL;			// TriPin=RL+  NPN with common Collector
    adc.lp1 = W5msReadADC(LowPin);	// voltage at Emitter resistor
    adc.tp1 = ReadADC(TristatePin);	// voltage at the Base 
    adc.rtp = ADCconfig.U_AVCC - adc.tp1;	// voltage at Base resistor
    if (adc.rtp < 10) { 
       R_DDR = 0;
       wait_about5ms();		// clear TRIAC and Thyristor
       R_DDR = LoPinRL | TriPinRH;
       R_PORT = TriPinRH;		// Tripin=RH+
       adc.lp1 = W5msReadADC(LowPin);
       adc.tp1 = ReadADC(TristatePin);	// voltage at the Base 
       adc.rtp = ADCconfig.U_AVCC - adc.tp1;	// voltage at Base resistor
 #ifdef LONG_HFE
       c_hfe = ((unsigned long)adc.lp1 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680MI)) / (unsigned int)adc.rtp;	
 #else
       c_hfe = ((adc.lp1 / ((RR680MI+500)/1000)) * (R_H_VAL/500)) / (adc.tp2/500);
 #endif
    } else {
       c_hfe = (adc.lp1 - adc.rtp) / adc.rtp;
    }
#endif
    //Tristate (can be Base) to VCC, Test if NPN
    ADC_DDR = LoADCm;		//Low-Pin to output 0V
    ADC_PORT = TXD_VAL;			//switch Low-Pin to GND
    R_DDR = TriPinRL | HiPinRL;		//RL port for High-Pin and Tristate-Pin to output
    // vCEs is already measured correctly with common emitter circuit
//#ifdef SHOW_ICE
//    R_PORT =  HiPinRL;	//RL port for High-Pin and Tristate-Pin to GND
//    adc.vCEs = ADCconfig.U_AVCC - W5msReadADC(HighPin); // measure voltage a High-Pin, Base low
//#endif
    R_PORT = TriPinRL | HiPinRL;	//RL port for High-Pin and Tristate-Pin to Vcc
    adc.hp1 = W5msReadADC(HighPin);	//measure voltage at High-Pin  (Collector)
#ifdef WITH_THYRISTOR_GATE_V
    adc.tp2 = ReadADC(TristatePin);	//voltage of gate
    adc.lp2 = ReadADC(LowPin);		//voltage of Cathode
#endif
    if(adc.hp1 < 1600) {
       //component has current => NPN-Transistor or somthing else

       //Test auf Thyristor:
       //Gate discharge
//      ChargePin10ms(TriPinRL,0);	//Tristate-Pin (Gate) across R_L 10ms to GND
       // TRIAC's can be triggered with gate and A1 (C) swapped. The current remains after triggering 
       // from gate to A2 (A) instead of A1 to A2. I have found that in this state the current will be lower,
       // if the Tristatepin (A1) is switched to GND.
       R_PORT = HiPinRL;
       adc.hp4 = W5msReadADC(HighPin);  //read voltage with switched back base
       R_DDR = HiPinRL;			// base to input
       adc.hp3 = W5msReadADC(HighPin);	//read voltage at High-Pin (probably Anode) again
					//current should still flow, if not,
					// no Thyristor or holding current to low 
		 	
       R_PORT = 0;			//switch R_L for High-Pin (probably Anode) to GND (turn off)
       wait_about5ms();
       R_PORT = HiPinRL;		//switch R_L for High-Pin (probably Anode) again to VCC
       adc.hp2 = W5msReadADC(HighPin);	//measure voltage at the High-Pin (probably Anode) again
       if((adc.hp3 < 1600) && (adc.hp2 > 4400)
           // additional check the voltage hp4 at A with gate hold at GND level
           && ((adc.hp1+150) > adc.hp4)
                                              ) {
          //if the holding current was switched off the thyristor must be switched off too. 
          //if Thyristor was still swiched on, if gate was switched off => Thyristor
          PartFound = PART_THYRISTOR;
#if DebugOut == 15
     lcd_line3();
     lcd_testpin(LowPin);
     lcd_data('Y');
     lcd_testpin(HighPin);
     lcd_space();
#endif
 
          ntrans.count++;		// mark as two N-type transistors
#ifdef WITH_THYRISTOR_GATE_V
          ntrans.uBE = adc.tp2 - adc.lp2;	// Gate - Cathode Voltage 
          ntrans.gthvoltage = adc.hp1 - adc.lp2;	// Anode-Cathode Voltage
#endif
          //Test if Triac
          R_DDR = 0;
          R_PORT = 0;
          ADC_PORT = LoADCp;	//Low-Pin fix to VCC
          wait_about5ms();
          R_DDR = HiPinRL;		//switch R_L port HighPin to output (GND)
          if(W5msReadADC(HighPin) > 244) {
#if DebugOut == 15
     lcd_data('H');
     lcd_data('2');
     lcd_space();
#endif
             goto saveNresult;		//measure voltage at the  High-Pin (probably A2); if too high:
                                	//component has current => kein Triac
          }
          R_DDR = HiPinRL | TriPinRL;	//switch R_L port for TristatePin (Gate) to output (GND) => Triac should be triggered 
          if(W5msReadADC(TristatePin) < 977) {
#if DebugOut == 15
     lcd_data('T');
     lcd_data('9');
     lcd_space();
#endif
             goto saveNresult; 		//measure voltage at the Tristate-Pin (probably Gate) ;
                              		// if to low, abort 
          }
          if(ReadADC(HighPin) < 733) {
#if DebugOut == 15
     lcd_data('H');
     lcd_data('7');
     lcd_space();
#endif
             goto saveNresult; 		//component has no current => no Triac => abort
          }
          R_DDR = HiPinRL;		//TristatePin (Gate) to input 
          if(W5msReadADC(HighPin) < 733) {
#if DebugOut == 15
     lcd_data('H');
     lcd_data('3');
     lcd_space();
#endif
             goto saveNresult; 		//component has no current without base current => no Triac => abort
          }
          R_PORT = HiPinRL;		//switch R_L port for HighPin to VCC => switch off holding current 
          wait_about5ms();
          R_PORT = 0;			//switch R_L port for HighPin again to GND; Triac should now switched off
          if(W5msReadADC(HighPin) > 244) {
#if DebugOut == 15
     lcd_data('H');
     lcd_data('4');
     lcd_space();
#endif
             goto saveNresult;		//measure voltage at the High-Pin (probably A2) ;
                                	//if to high, component is not switched off => no Triac, abort
          }
          PartFound = PART_TRIAC;
          goto saveNresult;
        }
      //Test if NPN Transistor or MOSFET
      // ADC_DDR = LoADCm;	//Low-Pin to output 0V
      R_DDR = HiPinRL | TriPinRH;	//R_H port of Tristate-Pin (Basis) to output
      R_PORT = HiPinRL | TriPinRH;	//R_H port of Tristate-Pin (Basis) to VCC
      adc.hp2 = W20msReadADC(HighPin);	//measure the voltage at the collector  
      adc.rhp = ADCconfig.U_AVCC - adc.hp2;	// voltage at the collector resistor
      adc.tp2 = ReadADC(TristatePin);	//measure the voltage at the base 
      adc.rtp = ADCconfig.U_AVCC - adc.tp2;	// voltage at the base resistor
      adc.lp2 = ReadADC(LowPin);

//      if((PartFound == PART_TRANSISTOR) || (PartFound == PART_FET)) {
//         PartReady = 1;	//check, if test is already done once
//      }

      if(adc.rtp > 2557) {		// Basis-voltage R_H is low enough
         PartFound = PART_TRANSISTOR;	//NPN-Transistor is found (Base is near GND)
         PartMode = PART_MODE_NPN;
         update_pins = 0;		// only update pins, if better hFE
 #ifdef COMMON_EMITTER

         //compute current amplification factor for common Emitter
         //hFE = B = Collector current / Base current
         tmp16 = adc.rhp;
         if (tmp16 > adc.lp_otr) {
            tmp16 -= adc.lp_otr;
         }

  #ifdef LONG_HFE
         e_hfe = ((unsigned int)tmp16 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680PL)) / (unsigned int)adc.rtp;	
  #else
         e_hfe = ((tmp16 / ((RR680PL+500)/1000)) * (R_H_VAL/500)) / (adc.rtp/500);
  #endif
         if ((ntrans.count == 0) || (e_hfe > ntrans.hfe)){
            ntrans.hfe = e_hfe;
            ntrans.uBE = adc.tp2 - adc.lp2;
            update_pins = 1;
         }
 #endif
#ifdef COMMON_COLLECTOR
          //compare current amplification factor for common Collector (Emitter follower)
          // hFE = (Emitterstrom - Basisstrom) / Basisstrom
 #ifdef COMMON_EMITTER
          if (c_hfe >  ntrans.hfe)
 #else
          if ((ntrans.count == 0) || (c_hfe >  ntrans.hfe))
 #endif
          {
            ntrans.hfe = c_hfe;
            ntrans.uBE = adc.tp1 - adc.lp1;
            update_pins = 1;
          }
#endif
#if DebugOut == 5
         lcd_data('B');
#endif
#ifdef SHOW_ICE
         if (update_pins != 0) {
	    // update residual collector (emitter) current without base current
            ntrans.ice0 = (unsigned int)(((unsigned long)adc.lp_otr * 1000) / RR680MI); // ICE0 0.01mA
            ntrans.ices = (unsigned int)(((unsigned long)adc.vCEs * 1000) / RR680PL); // ICEs 0.01mA
         }
#endif
         goto saveNresult;		// count the found N-Type and exit
      } 
      // Base has low current
      if((adc.lp_otr < 97) && (adc.rhp > 3400)) {
         //if flow voltage in switched off mode low enough?
         //(since D-Mode-FET will be detected in error as E-Mode )
         PartFound = PART_FET;	//N-Kanal-MOSFET is found (Basis/Gate will Not be pulled down)
         PartMode = PART_MODE_MOS|N_CHANNEL|E_MODE;
         if (adc.hp2 > (250+adc.lp2)) {
            // Drain-Source Voltage is too high for N_E_MOS
            PartMode = PART_MODE_IGBT|N_CHANNEL|E_MODE;
         }
#if DebugOut == 5
         lcd_data('F');
#endif
         //Switching of Drain is monitored with digital input
         // Low level is specified up to 0.3 * VCC
         // High level is specified above 0.6 * VCC
         PinMSK = HiADCm & 7;
         // measure Threshold voltage of Gate
         ADMUX = TristatePin | (1<<REFS0);	// measure TristatePin, Ref. VCC
         tmp16 = 1;			// round up ((1*4)/9)
         for(ii=0;ii<11;ii++) {
            wdt_reset();
            ChargePin10ms(TriPinRL,0);	// discharge Gate 10ms with RL 
            R_DDR = HiPinRL | TriPinRH;	// slowly charge Gate 
            R_PORT = HiPinRL | TriPinRH;
            while ((ADC_PIN&PinMSK));	// Wait, until the MOSFET switch and Drain moved to low 
             		// 0 is detected with input voltage of 2.12V to 2.24V (tested with mega168 & mega328)
            R_DDR = HiPinRL;		// switch off current
            ADCSRA |= (1<<ADSC);		// start ADC conversion
            while (ADCSRA&(1<<ADSC));	// wait until ADC finished
            tmp16 += ADCW;		// add result of ADC
         }
         tmp16 *= 4;	//is equal to 44 * ADCW
         ntrans.gthvoltage = tmp16/ 9;	//scale to mV
         ntrans.count++;		// count FET as two to accelerate  searching
         goto saveNresult;
      }
   } // end component conduct => npn
   ADC_DDR = TXD_MSK;		// switch all ADC-Ports to input
   ADC_PORT = TXD_VAL;		// switch all ADC-Ports to 0 (no Pull up)
   //ready
   //end of component has no connection between HighPin and LowPin
  }
//##########################################################################################
// Search for diodes
//##########################################################################################
checkDiode:
  R_DDR = 0;			//switch off resistor current
  R_PORT = 0;
  ADC_DDR = TXD_MSK;		// switch ADC ports to input
//  if (adc.lp_otr < 977) 
  if (adc.lp_otr < 455) {
#if DebugOut == 4
  lcd_line3();
  lcd_clear_line();
  lcd_line3();
  lcd_testpin(HighPin);
  lcd_data('D');
  lcd_testpin(LowPin);
  lcd_space();
  DisplayValue(adc.lp_otr,-3,' ',3);
  lcd_data('<');
#endif
     goto widmes;
  }
  // component has current
  //Test if Diode
  ADC_PORT = TXD_VAL;
  for (ii=0;ii<200;ii++) {
     ADC_DDR = LoADCm | HiADCm; // discharge by short of Low and High side
     wait_about5ms();		// Low and Highpin to GND for discharge
     ADC_DDR = LoADCm;		// switch only Low-Pin fix to GND
     adc.hp1 = ReadADC(HighPin); // read voltage at High-Pin
     if (adc.hp1 < (150/8)) break;
  }
    /*It is possible, that wrong Parts are detected without discharging, because
      the gate of a MOSFET can be charged.
      The additional measurement with the big resistor R_H is made, to differ antiparallel diodes
      from resistors.
      A diode has a voltage, that is nearly independent from the current.
      The voltage of a resistor is proportional to the current.
    */

#if 0
  /* first check with higher current (R_L=680) */
  /* A diode is found better with a parallel mounted capacitor, */
  /* but some capacitors can be detected a a diode. */
  R_DDR = HiPinRL;              //switch R_L port for High-Pin to output (VCC)
  R_PORT = HiPinRL;
  ChargePin10ms(TriPinRL,1);    //discharge of P-Kanal-MOSFET gate
  adc.lp_otr = W5msReadADC(HighPin) - ReadADC(LowPin);
  R_DDR = HiPinRH;              //switch R_H port for High-Pin output (VCC)
  R_PORT = HiPinRH;
  adc.hp2 = W5msReadADC(HighPin);               // M--|<--HP--R_H--VCC

  R_DDR = HiPinRL;              //switch R_L port for High-Pin to output (VCC)
  R_PORT = HiPinRL;
  ChargePin10ms(TriPinRL,0);    //discharge for N-Kanal-MOSFET gate
  adc.hp1 = W5msReadADC(HighPin) - W5msReadADC(LowPin);
  R_DDR = HiPinRH;              //switch R_H port for High-Pin to output (VCC)
  R_PORT = HiPinRH;
  adc.hp3 = W5msReadADC(HighPin);               // M--|<--HP--R_H--VCC
  if(adc.lp_otr > adc.hp1) {
      adc.hp1 = adc.lp_otr;	//the higher value wins
      adc.hp3 = adc.hp2;
  }
#else
  /* check first with low current (R_H=470k) */
  /* With this method the diode can be better differed from a capacitor, */
  /* but a parallel to a capacitor mounted diode can not be found. */
 #if FLASHEND > 0x1fff
  /* It is difficult to detect the protection diode of D-mode MOSFET . */
  /* We have to generate a negative gate voltage to isolate the diode. */
  /* For P-mode the resistors must reside on the VCC side. */
  /* For N-mode the resistors must be moved to the GND side. */
  R_DDR = HiPinRH;		//switch R_H port for High-Pin output (VCC)
  R_PORT = HiPinRH;
  ChargePin10ms(TriPinRL,1);	//discharge of P-Kanal-MOSFET gate
  adc.hp2 = W5msReadADC(HighPin); 		// GND--|<--HP--R_H--VCC
  // now the resistor is moved to the Low side
  R_DDR = LoPinRH;
  R_PORT = 0;
  ADC_DDR = HiADCm;		// switch High-Pin fix to VCC
  ADC_PORT = HiADCp;
  ChargePin10ms(TriPinRL,0);	//discharge for N-Kanal-MOSFET gate
  adc.hp3 = ADCconfig.U_AVCC - W5msReadADC(LowPin); // GND--R_H--LP--|<--VCC
  /* check with higher current (R_L=680) */
  R_DDR = LoPinRL;
  adc.hp1 = W5msReadADC(HighPin) - ReadADC(LowPin); // GND--R_L--LP--|<--VCC
  //  the resistor is moved back to the High side
  ADC_PORT = TXD_VAL;
  ADC_DDR = LoADCm;		// switch only Low-Pin fix to GND
  R_DDR = HiPinRL;		//switch R_L port for High-Pin output (VCC)
  R_PORT = HiPinRL;
  ChargePin10ms(TriPinRL,1);	//discharge for P-Kanal-MOSFET gate
  adc.lp_otr = W5msReadADC(HighPin) - ReadADC(LowPin); // GND--|<--HP--R_L--VCC
  if(adc.lp_otr > adc.hp1) {
      adc.hp1 = adc.lp_otr;	//the higher value wins
      adc.hp3 = adc.hp2;
      R_DDR = HiPinRH;		//switch R_H port for High-Pin output (VCC)
      R_PORT = HiPinRH;
      adc.hp2 = W5msReadADC(HighPin); 		// GND--|<--HP--R_H--VCC
  } else {
      R_DDR = LoPinRH;
      R_PORT = 0;
      ADC_DDR = HiADCm;		// switch High-Pin fix to VCC
      ADC_PORT = HiADCp;
      ChargePin10ms(TriPinRL,0);	//discharge for N-Kanal-MOSFET gate
      adc.hp2 = ADCconfig.U_AVCC - W5msReadADC(LowPin); // GND--R_H--LP--|<--VCC
  }
  // move the resistor to the Low side again
 #else
  /* There is not enough space to detect the protection diode for N-D-MOS correctly. */
  R_DDR = HiPinRH;		//switch R_H port for High-Pin output (VCC)
  R_PORT = HiPinRH;
  ChargePin10ms(TriPinRL,1);	//discharge of P-Kanal-MOSFET gate
  adc.hp2 = W5msReadADC(HighPin); 		// M--|<--HP--R_H--VCC
  ChargePin10ms(TriPinRL,0);	//discharge for N-Kanal-MOSFET gate
  adc.hp3 = W5msReadADC(HighPin);		// M--|<--HP--R_H--VCC

  /* check with higher current (R_L=680) */
  R_DDR = HiPinRL;		//switch R_L port for High-Pin to output (VCC)
  R_PORT = HiPinRL;
  adc.hp1 = W5msReadADC(HighPin) - ReadADC(LowPin);
  ChargePin10ms(TriPinRL,1);	//discharge for P-Kanal-MOSFET gate
  adc.lp_otr = W5msReadADC(HighPin) - ReadADC(LowPin);

  R_DDR = HiPinRH;		//switch R_H port for High-Pin output (VCC)
  R_PORT = HiPinRH;
  if(adc.lp_otr > adc.hp1) {
      adc.hp1 = adc.lp_otr;	//the higher value wins
      adc.hp3 = adc.hp2;
  } else {
      ChargePin10ms(TriPinRL,0);	//discharge for N-Kanal-MOSFET gate
  }
  adc.hp2 = W5msReadADC(HighPin); 		// M--|<--HP--R_H--VCC
 #endif
#endif
#if DebugOut == 4
  lcd_line3();
  lcd_clear_line();
  lcd_line3();
  lcd_testpin(HighPin);
  lcd_data('D');
  lcd_testpin(LowPin);
  lcd_space();
  lcd_data('h');
  DisplayValue(adc.hp3,-3,' ',2);
  lcd_data('L');
  DisplayValue(adc.hp1,-3,' ',2);
  lcd_data('H');
  DisplayValue(adc.hp2,-3,' ',2);
#endif
  volt_dif = adc.hp3/8;
  if (volt_dif > 200) volt_dif = 200;

//  if((adc.hp1 > 150) && (adc.hp1 < 4640) && (adc.hp1 > (adc.hp3+(adc.hp3/8))) && (adc.hp3*8 > adc.hp1)) {
  if((adc.hp1 > 150) && (adc.hp1 < 4640) && (adc.hp2 < adc.hp1) && (adc.hp1 > (adc.hp3+volt_dif)) && (adc.hp3 > adc.hp1/16)) {
     //voltage is above 0,15V and below 4,64V => Ok
     // hp2 >= hp1 is only possible with capacitor, not with a diode, hp2 is measured with 470k
//     if((PartFound == PART_NONE) || (PartFound == PART_RESISTOR)) {
     if(PartFound < PART_DIODE) {
        PartFound = PART_DIODE;	//mark for diode only, if no other component is found
				//since there is a problem with Transistors with a protection diode
#if DebugOut == 4
        lcd_data('D');
#endif
     }
     diodes.Anode[NumOfDiodes] = HighPin;
     diodes.Cathode[NumOfDiodes] = LowPin;
     diodes.Voltage[NumOfDiodes] = adc.hp1;	// voltage in Millivolt 
     NumOfDiodes++;
  } //end voltage is above 0,15V and below 4,64V 
#if DebugOut == 4
  lcd_data(NumOfDiodes+'0');
#endif

//##########################################################################################
// Search for resistors
//##########################################################################################
widmes:
//  if ((NumOfDiodes + ptrans.count  + ntrans.count) > 0) {
//     goto clean_ports;	// no resistors are searched, if diodes are detected
  if ((ptrans.count  + ntrans.count) > 0) {
     goto clean_ports;	// no resistors are searched, if transistors are detected
  }
  // resistor measurement
  wdt_reset();
// U_SCALE can be set to 4 for better resolution of ReadADC result
#if U_SCALE != 1
  ADCconfig.U_AVCC *= U_SCALE;	// scale to higher resolution, mV scale is not required
  ADCconfig.U_Bandgap *= U_SCALE;
#endif
#if R_ANZ_MESS != ANZ_MESS
  ADCconfig.Samples = R_ANZ_MESS;	// switch to special number of repetitions
#endif
  #define MAX_REPEAT (700 / (5 + R_ANZ_MESS/8))
  ADC_PORT = TXD_VAL;
  ADC_DDR = LoADCm;		//switch Low-Pin to output (GND)
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
  if (ii == MAX_REPEAT) goto testend;
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
#if DebugOut == 3
     lcd_line3();
     lcd_clear_line();
     lcd_line3();
     lcd_testpin(LowPin);
     lcd_data('U');
     lcd_testpin(HighPin);
     lcd_data('A');
     u2lcd(adc.hp1);
     lcd_data('B');
     u2lcd(adc.hp2);
     lcd_space();
#endif
     goto testend;
  }
  R_PORT = HiPinRH;		//switch R_H for High-Pin to VCC
  adc.hp2 = W5msReadADC(HighPin);	// voltage at resistor Rx with R_H

  ADC_DDR = HiADCm;		//switch High-Pin to output
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
  if (ii == MAX_REPEAT) goto testend;
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
#if DebugOut == 3
     lcd_data('F');
#endif
     goto testend; 
  }
//    if((adc.hp2 + (adc.hp2 / 61)) < adc.hp1)
  if (adc.hp2 < (4972*U_SCALE)) { 
     // voltage breaks down with low test current and it is not nearly shorted  => resistor
//     if (adc.lp1 < 120)  // take measurement with R_H 
     if (adc.lp1 < (169*U_SCALE)) { // take measurement with R_H 
        ii = 'H';
        if (adc.lp2 < (38*U_SCALE)) {
           // measurement > 60MOhm to big resistance
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
        ii = 'L';
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
//     lrx1 =(unsigned long)R_L_VAL * (unsigned long)adc.hp1 / (adc.hp3 - adc.hp1);

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
//   lrx1  is tempory result
#if 0
     /* The zero resistance is in 0.01 Ohm units and usually so little, that correction for resistors above 10 Ohm */
     /* is not necassary.  */
     ii = eeprom_read_byte(&EE_ESR_ZEROtab[LowPin+HighPin]) / 10; /* Resistance offset in 0,1 Ohm units */
     if (ii < lrx1) {
       lrx1 -= ii;
     } else {
       lrx1 = 0;
     }
#endif
#if DebugOut == 3
  lcd_line3();
  lcd_clear_line();
  lcd_line3();
  lcd_testpin(LowPin);
  lcd_data(ii);
  lcd_testpin(HighPin);
  lcd_space();
  if (ii == 'H') {
     lcd_data('X');
     DisplayValue(lirx1,1,LCD_CHAR_OMEGA,4);
     lcd_space();
     lcd_data('Y');
     DisplayValue(lirx2,1,LCD_CHAR_OMEGA,4);
     lcd_space();
  } else {
     lcd_data('x');
     DisplayValue(lirx1,-1,LCD_CHAR_OMEGA,4);
     lcd_space();
     lcd_data('y');
     DisplayValue(lirx2,-1,LCD_CHAR_OMEGA,4);
  }
  lcd_space();
  lcd_line4();
  lcd_clear_line();
  lcd_line4();
  DisplayValue(lirx2,-1,LCD_CHAR_OMEGA,4);
  lcd_space();
  lcd_line2();
#endif
//     if((PartFound == PART_DIODE) || (PartFound == PART_NONE) || (PartFound == PART_RESISTOR)) {
     if(PartFound < PART_TRANSISTOR) {
//        for (ii=0; ii<ResistorsFound; ii++) 
        if (ResistorsFound != 0) {
           // probably measurement with inverse polarity 
           thisR = &resis[ResistorsFound-1];
           if (thisR->rt == TristatePin) { 
              // must be measurement with inverse polarity 
              // resolution is 0.1 Ohm, 1 Ohm = 10 !
              lirx1 = (labs((long)lrx1 - (long)thisR->rx) * 10) / (lrx1 + thisR->rx + 100);
              if (lirx1  > 0) {
#if DebugOut == 3
                 lcd_data('R');
                 lcd_data('!');
                 lcd_data('=');
                 DisplayValue(thisR->rx,-1,LCD_CHAR_OMEGA,3);
                 lcd_space();
                 DisplayValue(lirx1,-1,LCD_CHAR_OMEGA,3);
                 lcd_space();
#endif
//                 ResistorsFound--;		// this one isn't a resistor
//                 goto testend; // <10% mismatch
              } else {
                 // resistor has the same value in both directions
                 if (PartFound < PART_DIODE) {
                    PartFound = PART_RESISTOR;	// only mark as resistor, if no other part found
                 }
                 thisR->checked = 1;		// mark as checked in both direction
              }
              goto testend;
           } // end  thisR->rt == TristatePin
           // must be a new one with other pins
           if (thisR->checked == 0) {
              // the last resistor has not the same value in both directions
              ResistorsFound--;	//overwrite with new one
           }
        }
        // no same resistor with the same Tristate-Pin found, new one
        thisR = &resis[ResistorsFound];		// pointer to a free resistor structure
        thisR->rx = lrx1;		// save resistor value
#if FLASHEND > 0x1fff
//        thisR->lx = 0;			// no inductance
#endif
        thisR->ra = LowPin;		// save Pin numbers
        thisR->rb = HighPin;
        thisR->rt = TristatePin;	// Tristate is saved for easier search of inverse measurement
        thisR->checked = 0;		// only one direction 
        ResistorsFound++;			// 1 more resistor found
#if DebugOut == 3
        lcd_data(ResistorsFound+'0');
        lcd_data('R');
#endif
     }
  }
  testend:			// end of resistor measurement
#if U_SCALE != 1
  ADCconfig.U_AVCC /= U_SCALE;		// scale back to mV resolution
  ADCconfig.U_Bandgap = adc_internal_reference;	// set back to normal resolution
#endif
#if R_ANZ_MESS != ANZ_MESS
  ADCconfig.Samples = ANZ_MESS;		// switch back to standard number of repetition
#endif

//---------------------------------------------------------------------------
// reset the ports and exit
//---------------------------------------------------------------------------
 clean_ports:
#ifdef DebugOut
 #if DebugOut < 10
  wait_for_key_5s_line2();
 #endif
#endif
  ADC_DDR = TXD_MSK;		// all ADC-Pins Input
  ADC_PORT = TXD_VAL;		// all ADC outputs to Ground, keine Pull up
  R_DDR = 0;			// all resistor-outputs to Input
  R_PORT = 0;			// all resistor-outputs to Ground, no Pull up
  return;

//---------------------------------------------------------------------------
// save Pins of P type transistor
//---------------------------------------------------------------------------
savePresult:
#if DebugOut == 5
 lcd_data('P');
#endif
 ptrans.count++;
 if (update_pins != 0) {
    ptrans.b = TristatePin;	// save Pin-constellation
    ptrans.c = LowPin;
    ptrans.e = HighPin;
 }
 goto clean_ports;

//---------------------------------------------------------------------------
// save Pins of N type transistor
//---------------------------------------------------------------------------
saveNresult:
#if DebugOut == 5
 lcd_data('N');
#endif
 ntrans.count++;
 if (update_pins != 0) {
    ntrans.b = TristatePin;	// save Pin-constellation
    ntrans.c = HighPin;
    ntrans.e = LowPin;
 }
 goto clean_ports;

} // end CheckPins()

