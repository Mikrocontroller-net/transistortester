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
     unsigned int hp1;
     unsigned int hp2;
     unsigned int hp3;
     unsigned int lp1;
     unsigned int lp2;
     unsigned int tp1;
     unsigned int tp2;
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
  uint8_t ii;			// Hilfsvariable
#if FLASHEND > 0x1fff
  int udiff;
#endif

#ifdef COMMON_COLLECTOR
  unsigned long c_hfe;		// amplification factor for common Collector (Emitter follower)
#endif

  struct resis_t *thisR;
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
  LoPinRL = MEM_read_byte(&PinRLtab[LowPin]);		// intruction for LowPin R_L
  LoPinRH = LoPinRL + LoPinRL;				// intruction for LowPin R_H
  TriPinRL = MEM_read_byte(&PinRLtab[TristatePin]);	// intruction for TristatePin R_L
  TriPinRH = TriPinRL + TriPinRL;			// intruction for TristatePin R_H
  HiPinRL = MEM_read_byte(&PinRLtab[HighPin]);		// intruction for HighPin R_L
  HiPinRH = HiPinRL + HiPinRL;				// intruction for HighPin R_H

  HiADCp = MEM_read_byte(&PinADCtab[HighPin]);		// intruction for ADC High-Pin 
  LoADCp = MEM_read_byte(&PinADCtab[LowPin]);		// intruction for ADC Low-Pin
  HiADCm = HiADCp | TXD_MSK;
  HiADCp |= TXD_VAL;
  LoADCm = LoADCp | TXD_MSK;
  LoADCp |= TXD_VAL;

  //setting of Pins 
  R_PORT = 0;				//resistor-Port outputs to 0
  R_DDR = LoPinRL;			//Low-Pin to output and across R_L to GND
  ADC_DDR = HiADCm;		//High-Pin to output
  ADC_PORT = HiADCp;		//High-Pin fix to Vcc
  //for some MOSFET the gate (TristatePin) must be discharged
  ChargePin10ms(TriPinRL,0);		//discharge for N-Kanal
  adc.lp_otr = W5msReadADC(LowPin);	//read voltage of Low-Pin 
  if(adc.lp_otr >= 977) {		//no current now? 
     ChargePin10ms(TriPinRL,1);	 	//else: discharge for P-channel (Gate to VCC)
     adc.lp_otr = ReadADC(LowPin);	//read voltage of Low-Pin again
  }

#if DebugOut == 5
  lcd_line2();
  lcd_clear_line();
  lcd_line2();
#endif

  if(adc.lp_otr > 92) {  //there is some current without TristatePin current 
#if DebugOut == 5
     lcd_testpin(LowPin);
     lcd_data('F');
     lcd_testpin(HighPin);
     lcd_space();
     wait_about1s();
#endif
     //Test if N-JFET or if self-conducting N-MOSFET
     R_DDR = LoPinRL | TriPinRH;	//switch R_H for Tristate-Pin (probably Gate) to GND
     adc.lp1 = W20msReadADC(LowPin);	//measure voltage at the assumed Source 
     adc.tp1 = ReadADC(TristatePin);	// measure Gate voltage
     R_PORT = TriPinRH;			//switch R_H for Tristate-Pin (probably Gate) to VCC
     adc.lp2 = W20msReadADC(LowPin);	//measure voltage at the assumed Source again
     //If it is a self-conducting MOSFET or JFET, then must be: adc.lp2 > adc.lp1 
     if(adc.lp2>(adc.lp1+488)) {
        //measure voltage at the  Gate, differ between MOSFET and JFET
        ADC_PORT = TXD_VAL;
        ADC_DDR = LoADCm;	//Low-Pin fix to GND
        R_DDR = TriPinRH | HiPinRL;	//High-Pin to output
        R_PORT = TriPinRH | HiPinRL;	//switch R_L for High-Pin to VCC
        adc.lp2 = W20msReadADC(TristatePin); //read voltage of assumed Gate 
        if(adc.lp2>3911) {  //MOSFET
           PartFound = PART_FET;	//N-Kanal-MOSFET
           PartMode = PART_MODE_N_D_MOS; //Depletion-MOSFET
        } else {  //JFET (pn-passage between Gate and Source is conducting )
           PartFound = PART_FET;	//N-Kanal-JFET
           PartMode = PART_MODE_N_JFET;
        }
//      if ((PartReady == 0) || (adc.lp1 > trans.uBE[0])) 
//      there is no way to find out the right Source / Drain
        trans.uBE[0] = adc.lp1;
        gthvoltage = adc.lp1 - adc.tp1;	//voltage GS (Source - Gate)
        trans.uBE[1] = (unsigned int)(((unsigned long)adc.lp1 * 1000) / RR680MI); // Id 0.01mA
        trans.b = TristatePin;		//save Pin numbers found for this FET
        trans.c = HighPin;
        trans.e = LowPin;
     }

     ADC_PORT = TXD_VAL;		// direct outputs to GND

     //Test, if P-JFET or if self-conducting P-MOSFET
     ADC_DDR = LoADCm;		//switch Low-Pin (assumed Drain) direct to GND,
				//R_H for Tristate-Pin (assumed Gate) is already switched to VCC
     R_DDR = TriPinRH | HiPinRL;	//High-Pin to output
     R_PORT = TriPinRH | HiPinRL;	//High-Pin across R_L to Vcc
     adc.hp1 = W20msReadADC(HighPin);	//measure voltage at assumed Source 
     adc.tp1 = ReadADC(TristatePin);	// measure Gate voltage
     R_PORT = HiPinRL;			//switch R_H for Tristate-Pin (assumed Gate) to GND
     adc.hp2 = W20msReadADC(HighPin);	//read voltage at assumed Source again
     //if it is a self-conducting P_MOSFET or P-JFET , then must be:  adc.hp1 > adc.hp2 
     if(adc.hp1>(adc.hp2+488)) {
        //read voltage at the Gate , to differ between MOSFET and JFET
        ADC_PORT = HiADCp;	//switch High-Pin directly to VCC
        ADC_DDR = HiADCm;	//switch High-Pin to output
        adc.tp2 = W20msReadADC(TristatePin); //read voltage at the assumed Gate 
        if(adc.tp2<977) { 		//MOSFET
           PartFound = PART_FET;	//P-Kanal-MOSFET
           PartMode = PART_MODE_P_D_MOS; //Depletion-MOSFET
        } else { 			//JFET (pn-passage between Gate and Source is conducting)
           PartFound = PART_FET;	//P-Kanal-JFET
           PartMode = PART_MODE_P_JFET;
        }
        gthvoltage = adc.tp1 - adc.hp1;		//voltage GS (Gate - Source)
        trans.uBE[1] = (unsigned int)(((unsigned long)(ADCconfig.U_AVCC - adc.hp1) * 1000) / RR680PL); // Id 0.01mA
        trans.b = TristatePin;		//save Pin numbers found for this FET
        trans.c = LowPin;
        trans.e = HighPin;
     }
  } // end component has current without TristatePin signal


#ifdef COMMON_COLLECTOR
  // Test circuit with common collector (Emitter follower) PNP
  ADC_PORT = TXD_VAL;
  ADC_DDR = LoADCm;			// Collector direct to GND
  R_PORT = HiPinRL;			// switch R_L port for HighPin (Emitter) to VCC
  R_DDR = TriPinRL | HiPinRL;		// Base resistor  R_L to GND
  adc.hp1 = ADCconfig.U_AVCC - W5msReadADC(HighPin);	// voltage at the Emitter resistor
  adc.tp1 = ReadADC(TristatePin);	// voltage at the base resistor

  if (adc.tp1 < 10) {
     R_DDR =  TriPinRH | HiPinRL;	// Tripin=RH-
     adc.hp1 = ADCconfig.U_AVCC - W5msReadADC(HighPin);
     adc.tp1 = ReadADC(TristatePin);	// voltage at base resistor 
#if DebugOut == 5
       lcd_line3();
       lcd_clear_line();
       lcd_line3();
       lcd_data('H');
       lcd_string(utoa(adc.hp1,outval,10));
       lcd_space();
       lcd_data('T');
       lcd_string(utoa(adc.tp1,outval,10));
       lcd_space();
#endif
 #ifdef LONG_HFE
     c_hfe = ((unsigned long)adc.hp1 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680PL)) / (unsigned int)adc.tp1;	
 #else
     c_hfe = ((adc.hp1 / ((RR680PL+500)/1000)) * (R_H_VAL/500)) / (adc.tp2/500);
 #endif
  } else {
     c_hfe = (unsigned long)((adc.hp1 - adc.tp1) / adc.tp1);
  }
#endif

  //set Pins again for circuit with common Emitter PNP
  R_DDR = LoPinRL;		//switch R_L port for Low-Pin to output (GND)
  R_PORT = 0;			//switch all resistor ports to GND
  ADC_DDR = HiADCm;		//switch High-Pin to output
  ADC_PORT = HiADCp;		//switch High-Pin to VCC
  wait_about5ms();
  
  if(adc.lp_otr < 977) {
     //if the component has no connection between  HighPin and LowPin
#if DebugOut == 5
     lcd_testpin(LowPin);
     lcd_data('P');
     lcd_testpin(HighPin);
     lcd_space();
     wait_about1s();
#endif
     //Test to PNP
     R_DDR = LoPinRL | TriPinRL;	//switch R_L port for Tristate-Pin to output (GND), for Test of PNP
     adc.lp1 = W5msReadADC(LowPin);	//measure voltage at LowPin
     if(adc.lp1 > 3422) {
        //component has current => PNP-Transistor or equivalent
        //compute current amplification factor in both directions
        R_DDR = LoPinRL | TriPinRH;	//switch R_H port for Tristate-Pin (Base) to output (GND)

        adc.lp1 = W5msReadADC(LowPin);	//measure voltage at LowPin (assumed Collector)
        adc.tp2 = ReadADC(TristatePin);	//measure voltage at TristatePin (Base) 
        //check, if Test is done before 
        if((PartFound == PART_TRANSISTOR) || (PartFound == PART_FET)) {
           PartReady = 1;
        }
        trans.uBE[PartReady] = ReadADC(HighPin) - adc.tp2;	// Base Emitter Voltage

        //compute current amplification factor for circuit with common Emitter
        //hFE = B = Collector current / Base current
        if(adc.tp2 < 53) {
#if DebugOut == 5
           lcd_data('<');
           lcd_data('5');
           lcd_data('3');
#endif
           adc.tp2 = 53;
        }

 #ifdef LONG_HFE
        trans.hfe[PartReady] = ((unsigned int)adc.lp1 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
                 (unsigned int)RR680MI)) / (unsigned int)adc.tp2;	
 #else
        trans.hfe[PartReady] = ((adc.lp1 / ((RR680MI+500)/1000)) * (R_H_VAL/500)) / (adc.tp2/500);
 #endif
#ifdef COMMON_COLLECTOR
        //current amplification factor for common  Collector (Emitter follower)
        // c_hFE = (Emitter current - Base current) / Base current
        if (c_hfe > trans.hfe[PartReady]) {
           trans.hfe[PartReady] = c_hfe;
           trans.uBE[PartReady] = ADCconfig.U_AVCC - adc.hp1 - adc.tp1;	// Base Emitter Voltage common collector
        }
#endif

 
        if(PartFound != PART_THYRISTOR) {
           if(adc.tp2 > 977) {
              //PNP-Transistor is found (Base voltage moves to VCC)
              PartFound = PART_TRANSISTOR;
              PartMode = PART_MODE_PNP;
           } else {
              if((adc.lp_otr < 97) && (adc.lp1 > 2000)) {
                 //is flow voltage low enough in the closed  state?
                 //(since D-Mode-FET would be by mistake detected as E-Mode )
        	 PartFound = PART_FET;		//P-Kanal-MOSFET is found (Basis/Gate moves not to VCC)
        	 PartMode = PART_MODE_P_E_MOS;
        	 //measure the Gate threshold voltage
                 //Switching of Drain is monitored with digital input
                 // Low level is specified up to 0.3 * VCC
                 // High level is specified above 0.6 * VCC
                 PinMSK = LoADCm & 7;
        	 ADMUX = TristatePin | (1<<REFS0);	// switch to TristatePin, Ref. VCC
        	 gthvoltage = 1;			// round up ((1*4)/9)
        	 for(ii=0;ii<11;ii++) {
        	    wdt_reset();
        	    ChargePin10ms(TriPinRL,1);
                    R_DDR = LoPinRL | TriPinRH;		//switch R_H for Tristate-Pin (Basis) to GND
        	    while (!(ADC_PIN&PinMSK));		// Wait, until the MOSFET switches and Drain moves to VCC
                    			// 1 is detected with more than 2.5V (up to 2.57V) with tests of mega168 and mega328
        	    R_DDR = LoPinRL;
        	    ADCSRA |= (1<<ADSC);		// Start Conversion
        	    while (ADCSRA&(1<<ADSC));		// wait
      		    gthvoltage += (1023 - ADCW);	// Add Tristatepin-Voltage
                 }
                 gthvoltage *= 4;		// is equal to 44*ADCW
                 gthvoltage /= 9;		// gives resolution in mV
              }
           }
           trans.b = TristatePin;
           trans.c = LowPin;
           trans.e = HighPin;
        }  // end if PartFound != PART_THYRISTOR
    } // end component has current => PNP

#ifdef COMMON_COLLECTOR
    // Low-Pin=RL- HighPin=VCC
    R_DDR = LoPinRL | TriPinRL;
    R_PORT = TriPinRL;			// TriPin=RL+  NPN with common Collector
    adc.lp1 = W5msReadADC(LowPin);	// voltage at Emitter resistor
    adc.tp1 = ADCconfig.U_AVCC - ReadADC(TristatePin);	// voltage at Base resistor
    if (adc.tp1 < 10) {
       R_DDR = LoPinRL | TriPinRH;
       R_PORT = TriPinRH;		// Tripin=RH+
       adc.lp1 = W5msReadADC(LowPin);
       adc.tp1 = ADCconfig.U_AVCC - ReadADC(TristatePin);	// voltage at Base resistor
 #ifdef LONG_HFE
       c_hfe = ((unsigned long)adc.lp1 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680MI)) / (unsigned int)adc.tp1;	
 #else
       c_hfe = ((adc.lp1 / ((RR680MI+500)/1000)) * (R_H_VAL/500)) / (adc.tp2/500);
 #endif
    } else {
       c_hfe = (adc.lp1 - adc.tp1) / adc.tp1;
    }
#if DebugOut == 5
       lcd_line4();
       lcd_clear_line();
       lcd_line4();
       lcd_data('L');
       lcd_data('P');
       lcd_string(utoa(adc.lp1,outval,10));
       lcd_space();
       lcd_data('T');
       lcd_data('P');
       lcd_string(utoa(adc.tp1,outval,10));
       wait_about1s();
#endif
#endif
    //Tristate (can be Base) to VCC, Test if NPN
    ADC_DDR = LoADCm;		//Low-Pin to output 0V
    ADC_PORT = TXD_VAL;			//switch Low-Pin to GND
    R_DDR = TriPinRL | HiPinRL;		//RL port for High-Pin and Tristate-Pin to output
    R_PORT = TriPinRL | HiPinRL;	//RL port for High-Pin and Tristate-Pin to Vcc
    adc.hp1 = W5msReadADC(HighPin);	//measure voltage at High-Pin  (Collector)
    if(adc.hp1 < 1600) {
       //component has current => NPN-Transistor or somthing else
#if DebugOut == 5
       lcd_testpin(LowPin);
       lcd_data('N');
       lcd_testpin(HighPin);
       lcd_space();
       wait_about1s();
#endif
       if(PartReady==1) {
          goto widmes;
       }

       //Test auf Thyristor:
       //Gate discharge
       ChargePin10ms(TriPinRL,0);	//Tristate-Pin (Gate) across R_L 10ms to GND
       adc.hp3 = W5msReadADC(HighPin);	//read voltage at High-Pin (probably Anode) again
					//current should still flow, if not,
					// no Thyristor or holding current to low 
		 	
       R_PORT = 0;			//switch R_L for High-Pin (probably Anode) to GND (turn off)
       wait_about5ms();
       R_PORT = HiPinRL;		//switch R_L for High-Pin (probably Anode) again to VCC
       adc.hp2 = W5msReadADC(HighPin);	//measure voltage at the High-Pin (probably Anode) again
       if((adc.hp3 < 1600) && (adc.hp2 > 4400)) {
          //if the holding current was switched off the thyristor must be switched off too. 
          //if Thyristor was still swiched on, if gate was switched off => Thyristor
          PartFound = PART_THYRISTOR;
          //Test if Triac
          R_DDR = 0;
          R_PORT = 0;
          ADC_PORT = LoADCp;	//Low-Pin fix to VCC
          wait_about5ms();
          R_DDR = HiPinRL;		//switch R_L port HighPin to output (GND)
          if(W5msReadADC(HighPin) > 244) {
             goto savenresult;		//measure voltage at the  High-Pin (probably A2); if too high:
                                	//component has current => kein Triac
          }
          R_DDR = HiPinRL | TriPinRL;	//switch R_L port for TristatePin (Gate) to output (GND) => Triac should be triggered 
          if(W5msReadADC(TristatePin) < 977) {
             goto savenresult; 		//measure voltage at the Tristate-Pin (probably Gate) ;
                              		// if to low, abort 
          }
          if(ReadADC(HighPin) < 733) {
             goto savenresult; 		//component has no current => no Triac => abort
          }
          R_DDR = HiPinRL;		//TristatePin (Gate) to input 
          if(W5msReadADC(HighPin) < 733) {
             goto savenresult; 		//component has no current without base current => no Triac => abort
          }
          R_PORT = HiPinRL;		//switch R_L port for HighPin to VCC => switch off holding current 
          wait_about5ms();
          R_PORT = 0;			//switch R_L port for HighPin again to GND; Triac should now switched off
          if(W5msReadADC(HighPin) > 244) {
             goto savenresult;		//measure voltage at the High-Pin (probably A2) ;
                                	//if to high, component is not switched off => no Triac, abort
         }
         PartFound = PART_TRIAC;
         PartReady = 1;
         goto savenresult;
        }
      //Test if NPN Transistor or MOSFET
      // ADC_DDR = LoADCm;	//Low-Pin to output 0V
      R_DDR = HiPinRL | TriPinRH;	//R_H port of Tristate-Pin (Basis) to output
      R_PORT = HiPinRL | TriPinRH;	//R_H port of Tristate-Pin (Basis) to VCC
      wait_about50ms();
      adc.hp2 = ADCconfig.U_AVCC - ReadADC(HighPin);	//measure the voltage at the collector resistor 
      adc.tp2 = ADCconfig.U_AVCC - ReadADC(TristatePin);	//measure the voltage at the base resistor 

#if DebugOut == 5
       lcd_line3();
       lcd_clear_line();
       lcd_line3();
       lcd_data('H');
       lcd_data('P');
       lcd_string(utoa(adc.hp2,outval,10));
       lcd_space();
       lcd_data('T');
       lcd_data('P');
       lcd_string(utoa(adc.tp2,outval,10));
#endif
      if((PartFound == PART_TRANSISTOR) || (PartFound == PART_FET)) {
         PartReady = 1;	//check, if test is already done once
      }
      trans.uBE[PartReady] = ADCconfig.U_AVCC - adc.tp2 - ReadADC(LowPin);

      //compute current amplification factor for common Emitter
      //hFE = B = Collector current / Base current
      if(adc.tp2 < 53) {
#if DebugOut == 5
         lcd_data('<');
         lcd_data('5');
         lcd_data('3');
#endif
         adc.tp2 = 53;
      }

#ifdef LONG_HFE
      trans.hfe[PartReady] = ((unsigned int)adc.hp2 * (unsigned long)(((unsigned long)R_H_VAL * 100) / 
              (unsigned int)RR680PL)) / (unsigned int)adc.tp2;	
#else
      trans.hfe[PartReady] = ((adc.hp2 / ((RR680PL+500)/1000)) * (R_H_VAL/500)) / (adc.tp2/500);
#endif
#ifdef COMMON_COLLECTOR
       //compare current amplification factor for common Collector (Emitter follower)
       // hFE = (Emitterstrom - Basisstrom) / Basisstrom
       if (c_hfe >  trans.hfe[PartReady]) {
          trans.hfe[PartReady] = c_hfe;
          trans.uBE[PartReady] = ADCconfig.U_AVCC - adc.lp1 - adc.tp1;
       }
#endif

      if(adc.tp2 > 2557) {		// Basis-voltage R_H is low enough
         PartFound = PART_TRANSISTOR;	//NPN-Transistor is found (Base is near GND)
         PartMode = PART_MODE_NPN;
      } else { // Basis has low current
         if((adc.lp_otr < 97) && (adc.hp2 > 3400)) {
            //if flow voltage in switched off mode low enough?
            //(since D-Mode-FET will be detected in error as E-Mode )
            PartFound = PART_FET;	//N-Kanal-MOSFET is found (Basis/Gate will Not be pulled down)
            PartMode = PART_MODE_N_E_MOS;
#if DebugOut == 5
            lcd_line3();
            lcd_clear_line();
            lcd_line3();
            lcd_data('N');
            lcd_data('F');
            wait_about1s();
#endif
            //Switching of Drain is monitored with digital input
            // Low level is specified up to 0.3 * VCC
            // High level is specified above 0.6 * VCC
            PinMSK = HiADCm & 7;
            // measure Threshold voltage of Gate
            ADMUX = TristatePin | (1<<REFS0);	// measure TristatePin, Ref. VCC
            gthvoltage = 1;			// round up ((1*4)/9)
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
            	gthvoltage += ADCW;		// add result of ADC
            }
            gthvoltage *= 4;	//is equal to 44 * ADCW
            gthvoltage /= 9;	//scale to mV
         }
      }
savenresult:
      trans.b = TristatePin;	// save Pin-constellation
      trans.c = HighPin;
      trans.e = LowPin;
   } // end component conduct => npn
   ADC_DDR = TXD_MSK;		// switch all ADC-Ports to input
   ADC_PORT = TXD_VAL;		// switch all ADC-Ports to 0 (no Pull up)
   //Fertig
   //end das component has no connection between HighPin and LowPin
   goto widmes;
  }
  // component has current
  //Test if Diode
  ADC_PORT = TXD_VAL;
  ADC_DDR = LoADCm;	//Low-Pin fix to GND
  R_DDR = HiPinRL;		//switch R_L port for High-Pin to output (VCC)
  R_PORT = HiPinRL;
  ChargePin10ms(TriPinRL,1);	//discharge of P-Kanal-MOSFET gate
  adc.lp_otr = W5msReadADC(HighPin) - ReadADC(LowPin);
  R_DDR = HiPinRH;		//switch R_H port for High-Pin output (VCC)
  R_PORT = HiPinRH;
  adc.hp2 = W5msReadADC(HighPin); 		// M--|<--HP--R_H--VCC

  R_DDR = HiPinRL;		//switch R_L port for High-Pin to output (VCC)
  R_PORT = HiPinRL;
  ChargePin10ms(TriPinRL,0);	//discharge for N-Kanal-MOSFET gate
  adc.hp1 = W5msReadADC(HighPin) - W5msReadADC(LowPin);
  R_DDR = HiPinRH;		//switch R_H port for High-Pin to output (VCC)
  R_PORT = HiPinRH;
  adc.hp3 = W5msReadADC(HighPin);		// M--|<--HP--R_H--VCC

    /*It is possible, that wrong Parts are detected without discharging, because
      the gate of a MOSFET can be charged.
      The additional measurement with the big resistor R_H is made, to differ antiparallel diodes
      from resistors.
      A diode has a voltage, that is nearly independent from the current.
      The voltage of a resistor is proportional to the current.
    */
  if(adc.lp_otr > adc.hp1) {
      adc.hp1 = adc.lp_otr;	//the higher value wins
      adc.hp3 = adc.hp2;
  }
#if DebugOut == 4
  lcd_line3();
  lcd_clear_line();
  lcd_line3();
  lcd_testpin(HighPin);
  lcd_data('D');
  lcd_testpin(LowPin);
  lcd_space();
  lcd_data('L');
  lcd_string(utoa(adc.hp1,outval,10));
  lcd_space();
  lcd_data('H');
  lcd_string(utoa(adc.hp3,outval,10));
  lcd_space();
  wait_about1s();
#endif

  if((adc.hp1 > 150) && (adc.hp1 < 4640) && (adc.hp1 > (adc.hp3+(adc.hp3/8))) && (adc.hp3*8 > adc.hp1)) {
     //voltage is above 0,15V and below 4,64V => Ok
     if((PartFound == PART_NONE) || (PartFound == PART_RESISTOR)) {
        PartFound = PART_DIODE;	//mark for diode only, if no other component is found
				//since there is a problem with Transistors with a protection diode
#if DebugOut == 4
        lcd_data('D');
#endif
     }
     diodes[NumOfDiodes].Anode = HighPin;
     diodes[NumOfDiodes].Cathode = LowPin;
     diodes[NumOfDiodes].Voltage = adc.hp1;	// voltage in Millivolt 
     NumOfDiodes++;
  } //end voltage is above 0,15V and below 4,64V 
#if DebugOut == 4
  lcd_data(NumOfDiodes+'0');
#endif

widmes:
  if (NumOfDiodes > 0) goto clean_ports;
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
     lcd_string(utoa(adc.hp1, outval, 10));
     lcd_data('B');
     lcd_string(utoa(adc.hp2, outval, 10));
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
//     if (adc.lp1 < 120) { // take measurement with R_H 
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
     DisplayValue(lirx1,1,LCD_CHAR_OMEGA,4)
     lcd_space();
     lcd_data('Y');
     DisplayValue(lirx2,1,LCD_CHAR_OMEGA,4)
     lcd_space();
  } else {
     lcd_data('x');
     DisplayValue(lirx1,-1,LCD_CHAR_OMEGA,4)
     lcd_space();
     lcd_data('y');
     DisplayValue(lirx2,-1,LCD_CHAR_OMEGA,4)
  }
  lcd_space();
  lcd_line4();
  lcd_clear_line();
  lcd_line4();
  DisplayValue(lirx2,-1,LCD_CHAR_OMEGA,4)
  lcd_space();
  lcd_line2();
#endif
     if((PartFound == PART_DIODE) || (PartFound == PART_NONE) || (PartFound == PART_RESISTOR)) {
        for (ii=0; ii<ResistorsFound; ii++) {
           // search measurements with inverse polarity 
           thisR = &resis[ii];
           if (thisR->rt != TristatePin) {
              continue;
           }
           // must be measurement with inverse polarity 
           // resolution is 0.1 Ohm, 1 Ohm = 10 !
           lirx1 = (labs((long)lrx1 - (long)thisR->rx) * 10) / (lrx1 + thisR->rx + 100);
           if (lirx1  > 0) {
#if DebugOut == 3
              lcd_data('R');
              lcd_data('!');
              lcd_data('=');
              DisplayValue(thisR->rx,-1,LCD_CHAR_OMEGA,3)
              lcd_space();
              DisplayValue(lirx1,-1,LCD_CHAR_OMEGA,3)
              lcd_space();
#endif
              goto testend; // <10% mismatch
           }
           PartFound = PART_RESISTOR;
           goto testend;
        } // end for
        // no same resistor with the same Tristate-Pin found, new one
        thisR = &resis[ResistorsFound];		// pointer to a free resistor structure
        thisR->rx = lrx1;		// save resistor value
#if FLASHEND > 0x1fff
        thisR->lx = 0;			// no inductance
#endif
        thisR->ra = LowPin;		// save Pin numbers
        thisR->rb = HighPin;
        thisR->rt = TristatePin;	// Tristate is saved for easier search of inverse measurement
        ResistorsFound++;			// 1 more resistor found
#if DebugOut == 3
        lcd_data(ResistorsFound+'0');
        lcd_data('R');
#endif
     }
  }
  testend:
#if U_SCALE != 1
  ADCconfig.U_AVCC /= U_SCALE;		// scale back to mV resolution
  ADCconfig.U_Bandgap /= U_SCALE;
#endif
#if R_ANZ_MESS != ANZ_MESS
  ADCconfig.Samples = ANZ_MESS;		// switch back to standard number of repetition
#endif
#ifdef DebugOut
#if DebugOut < 10
  wait_about2s();
#endif
#endif
 clean_ports:
  ADC_DDR = TXD_MSK;		// all ADC-Pins Input
  ADC_PORT = TXD_VAL;		// all ADC outputs to Ground, keine Pull up
  R_DDR = 0;			// all resistor-outputs to Input
  R_PORT = 0;			// all resistor-outputs to Ground, no Pull up
} // end CheckPins()

