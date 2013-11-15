void AutoCheck(void) {
#ifdef WITH_SELFTEST
  uint8_t tt;		// number of running test
  uint8_t ww;		// counter for repeating the tests
  int  adcmv[7];
#ifdef EXTENDED_TESTS
  uint16_t u680;	// 3 * (Voltage at 680 Ohm)
#else
 #warning "Selftest without extended tests T1 to T7!"
#endif
// define the maximum count of repetitions MAX_REP
#define MAX_REP 4

 #ifdef AUTO_CAL
  uint8_t cap_found;	// counter for found capacitor
  #ifdef AUTOSCALE_ADC
  int8_t udiff;		// difference between ADC Voltage with VCC or Bandgap reference
  int8_t udiff2;
  #endif
 #endif
  #define RequireShortedProbes
  if (AllProbesShorted() != 3) return;
  lcd_clear();
  lcd_fix_string(SELFTEST);		// "Selftest mode.."
  lcd_line2();
  lcd_data('?');			// wait for key pressed
  for (tt=0;tt<50;tt++) {
     if ((ON_PIN_REG & (1<<RST_PIN)) != 0) break;	// key is unpressed
     wait_about10ms();			// key is still pressed, wait for release
    }
  if (tt == 50) goto no_selftest;		// key is more than 500ms pressed
  for (tt=0;tt<200;tt++) {
     if(!(ON_PIN_REG & (1<<RST_PIN))) goto begin_selftest;	// key is pressed again
     wait_about10ms();
    }
  // no key pressed for 2s
no_selftest:
  lcd_clear();
  lcd_fix2_string(VERSION_str);	//"Version ..."
  return;
begin_selftest:
  lcd_line2();
  lcd_fix2_string(R0_str);		// "R0="
  eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[2]), (int8_t)0);	// clear zero offset
  eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[3]), (int8_t)0);	// clear zero offset
  eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[1]), (int8_t)0);	// clear zero offset

  adcmv[0] = GetESR(TP3, TP1);
  adcmv[1] = GetESR(TP3, TP2);
  adcmv[2] = GetESR(TP2, TP1);
  DisplayValue(adcmv[0],-2,' ',3);
  DisplayValue(adcmv[1],-2,' ',3);
  DisplayValue(adcmv[2],-2,LCD_CHAR_OMEGA,3);
  if (adcmv[0] < 60) {
     eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[2]), (int8_t)adcmv[0]);	// fix zero offset
  }
  if (adcmv[1] < 60) {
     eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[3]), (int8_t)adcmv[1]);	// fix zero offset
  }
  if (adcmv[2] < 60) {
     eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[1]), (int8_t)adcmv[2]);	// fix zero offset
  }
  for(tt=0;tt<12;tt++) {
     wait_about500ms();
     if(!(ON_PIN_REG & (1<<RST_PIN))) {
        // if key is pressed, don't repeat
        break;
     }
  } /* end for tt */

#ifdef EXTENDED_TESTS
 #define TEST_COUNT 8
 
  for(tt=1;tt<TEST_COUNT;tt++) {		// loop for all Tests
     for(ww=0;ww<MAX_REP;ww++) {	// repeat the test MAX_REP times
        lcd_line2();			//Cursor to column 1, row 2
        lcd_clear_line();		// clear total line
        lcd_line1();			//Cursor to column 1, row 1
        lcd_clear_line();		// clear total line
        lcd_line1();			//Cursor to column 1, row 1
        lcd_data('T');			//output the Testmode "T"
        lcd_string(utoa(tt, outval, 10));	//output Test number
        lcd_space();
                                        //############################################
        if (tt == 1) {   // output of reference voltage and factors for capacity measurement
           Calibrate_UR();		// get Reference voltage, Pin resistance
           lcd_fix2_string(URefT);	//"URef="
           DisplayValue(ref_mv,-3,'V',4);
           lcd_line2();			//Cursor to column 1, row 2
           lcd_fix2_string(RHfakt);	//"RHf="
           lcd_string(utoa(RHmultip, outval, 10));
           ADCconfig.Samples = 190;	// set number of ADC reads near to maximum
        }
                                        //############################################
        if (tt == 2) { // how equal are the RL resistors? 
           u680 = ((long)ADCconfig.U_AVCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + R_L_VAL + PIN_RP));
           R_PORT = 1<<(TP1*2);		//RL1 to VCC
           R_DDR = (1<<(TP1*2)) | (1<<(TP2*2));	//RL2 to -
           adcmv[0] = W20msReadADC(TP1);
           adcmv[0] -= u680;
           R_DDR = (1<<(TP1*2)) | (1<<(TP3*2));	//RL3 to -
           adcmv[1] = W20msReadADC(TP1);
           adcmv[1] -= u680;
           R_PORT = 1<<(TP2*2);		//RL2 to VCC
           R_DDR = (1<<(TP2*2)) | (1<<(TP3*2));	//RL3 to -
           adcmv[2] = W20msReadADC(TP2);
           adcmv[2] -= u680;
           lcd_fix_string(RLRL);	// "RLRL"
        }
                                        //############################################
        if (tt == 3) { // how equal are the RH resistors
           R_PORT = 2<<(TP1*2);		//RH1 to VCC
           R_DDR = (2<<(TP1*2)) | (2<<(TP2*2));	//RH2 to -
           adcmv[0] = W20msReadADC(TP1);
           adcmv[3] = ADCconfig.U_AVCC / 2;
           adcmv[0] -= adcmv[3];
           R_DDR = (2<<(TP1*2)) | (2<<(TP3*2));	//RH3 to -
           adcmv[1] = W20msReadADC(TP1);
           adcmv[1] -= adcmv[3];
           R_PORT = 2<<(TP2*2);		//RH2 to VCC
           R_DDR = (2<<(TP2*2)) | (2<<(TP3*2));	//RH3 to -
           adcmv[2] = W20msReadADC(TP2);
           adcmv[2] -= adcmv[3];
           lcd_fix_string(RHRH);	// "RHRH"
        }
                                        //############################################
        if (tt == 4) { // Text release probes
           lcd_fix_string(RELPROBE);	// "Release Probes"
           if (AllProbesShorted() != 0) ww = MAX_REP-2;
        }
                                        //############################################
        if (tt == 5) { // can we switch the ADC pins to GND across R_H resistor?
           R_PORT = 0;
           R_DDR = 2<<(TP1*2);		//Pin 1 over R_H to GND
           adcmv[0] = W20msReadADC(TP1);

           R_DDR = 2<<(TP2*2);		//Pin 2 over R_H to GND
           adcmv[1] = W20msReadADC(TP2);

           R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to GND
           adcmv[2] = W20msReadADC(TP3);
           lcd_fix_string(RH1L);	// "RH_Lo="
        }
                                        //############################################
        if (tt == 6) { // can we switch the ADC pins to VCC across the R_H resistor?
           R_DDR = 2<<(TP1*2);		//Pin 1 over R_H to VCC
           R_PORT = 2<<(TP1*2);
           adcmv[0] = W20msReadADC(TP1) - ADCconfig.U_AVCC;
           R_DDR = 2<<(TP2*2);		//Pin 2 over R_H to VCC
           R_PORT = 2<<(TP2*2);
           adcmv[1] = W20msReadADC(TP2) - ADCconfig.U_AVCC;
           R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to VCC
           R_PORT = 2<<(TP3*2);
           adcmv[2] = W20msReadADC(TP3) - ADCconfig.U_AVCC;
           lcd_fix_string(RH1H);	// "RH_Hi="
        }
        if (tt == 7) { // is the voltage of all R_H / R_L dividers correct?
           u680 = ((long)ADCconfig.U_AVCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + R_H_VAL*100));
           R_PORT = 2<<(TP1*2);		//RH1 to VCC
           R_DDR = (2<<(TP1*2)) | (1<<(TP1*2));	//RH1 to +, RL1 to -
           adcmv[0] = W20msReadADC(TP1);
           adcmv[0] -= u680;
           R_PORT = 2<<(TP2*2);		//RH2 to VCC
           R_DDR = (2<<(TP2*2)) | (1<<(TP2*2));	//RH2 to +, RL2 to -
           adcmv[1] = W20msReadADC(TP2);
           adcmv[1] -= u680;
           R_PORT = 2<<(TP3*2);		//RH3 to VCC
           R_DDR = (2<<(TP3*2)) | (1<<(TP3*2));	//RH3 to +, RL3 to -
           adcmv[2] = W20msReadADC(TP3);
           adcmv[2] -= u680;
           lcd_fix_string(RHRL);	// "RH/RL"
        }
                                        //############################################
        if (tt > 1) {	// output 3 voltages 
           lcd_line2();			//Cursor to column 1, row 2
           lcd_string(itoa(adcmv[0], outval, 10));	//output voltage 1
           lcd_space();
           lcd_string(itoa(adcmv[1], outval, 10));	//output voltage 2
           lcd_space();
           lcd_string(itoa(adcmv[2], outval, 10));	//output voltage 3
        }
        ADC_DDR =  TXD_MSK;		// all-Pins to Input
        ADC_PORT = TXD_VAL;		// all ADC-Ports to GND
        R_DDR = 0;			// all R-Ports to Input
        R_PORT = 0;
        if(!(ON_PIN_REG & (1<<RST_PIN))) {
	   // if key is pressed, don't repeat
           break;
        }
        wait_about500ms();
        if(!(ON_PIN_REG & (1<<RST_PIN))) {
	   // if key is pressed, don't repeat
           break;
        }
        wait_about500ms();
     } //end for ww
     wait_about1s();
  } //end for tt
#else
  for (ww=0;ww<120;ww++) {
     // wait 1 minute for releasing the probes
     lcd_line2();		//Cursor to column 1, row 2
     lcd_clear_line();		// clear total line
     lcd_line2();		//Cursor to column 1, row 2
     lcd_fix_string(RELPROBE);	// "Release Probes"
     wait_about500ms();
     if (AllProbesShorted() == 0) break;
  }
#endif

  lcd_clear();
  lcd_fix_string(RIHI);	// "RiHi="
  DisplayValue(RRpinPL,-1,LCD_CHAR_OMEGA,3);
  lcd_line2();
  lcd_fix_string(RILO);	// "RiLo="
  DisplayValue(RRpinMI,-1,LCD_CHAR_OMEGA,3);
  wait_about2s();			//wait 2 seconds

  //measure Zero offset for Capacity measurement
  adcmv[3] = 0;
  PartFound = PART_NONE;
  ReadCapacity(TP3, TP1);
  adcmv[5] = (unsigned int) cap.cval_uncorrected.dw;	//save capacity value of empty Pin 1:3
  ReadCapacity(TP3, TP2);
  adcmv[6] = (unsigned int) cap.cval_uncorrected.dw;	//save capacity value of empty Pin 2:3
  ReadCapacity(TP2, TP1);
  adcmv[2] = (unsigned int) cap.cval_uncorrected.dw;	//save capacity value of empty Pin 1:2
  ReadCapacity(TP1, TP3);
  adcmv[1] = (unsigned int) cap.cval_uncorrected.dw;	//save capacity value of empty Pin 3:1
  ReadCapacity(TP2, TP3);
  adcmv[4] = (unsigned int) cap.cval_uncorrected.dw;	//save capacity value of empty Pin 3:2
  ReadCapacity(TP1, TP2);
  adcmv[0] = (unsigned int) cap.cval_uncorrected.dw;	//save capacity value of empty Pin 2:1
  lcd_clear();
  lcd_fix_string(C0_str);			//output "C0 "
  DisplayValue(adcmv[5],0,' ',3);		//output cap0 1:3
  DisplayValue(adcmv[6],0,' ',3);		//output cap0 2:3
  DisplayValue(adcmv[2],-12,'F',3);		//output cap0 1:2
 #ifdef AUTO_CAL
  for (ww=0;ww<7;ww++) {
      if ((adcmv[ww] > 70) || (adcmv[ww] <10)) goto no_c0save;
  }
  for (ww=0;ww<7;ww++) {
      // write all zero offsets to the EEprom 
      (void) eeprom_write_byte((uint8_t *)(&c_zero_tab[ww]),adcmv[ww]+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2)));
  }
  lcd_line2();
  lcd_fix_string(OK_str);		// output "OK"
no_c0save:
 #endif
 wait_about2s();		//wait 2 seconds
 
 #ifdef AUTO_CAL
 // Message C > 100nF
 cap_found = 0;
 for (ww=0;ww<64;ww++) {
     lcd_clear();
     lcd_data('1');
     lcd_fix_string(CapZeich);	// "-||-"
     lcd_data('3');
     lcd_fix2_string(MinCap_str); // " >100nF!"
     PartFound = PART_NONE;
     //measure  offset Voltage of analog Comparator for Capacity measurement
     ReadCapacity(TP3, TP1);	// look for capacitor > 100nF
     while (cap.cpre < -9) {
        cap.cpre++;
        cap.cval /= 10;
     }
     if ((cap.cpre == -9) && (cap.cval > 95) && (cap.cval < 22000)) {
        cap_found++;
     } else {
        cap_found = 0;		// wait for stable connection
     }
     if (cap_found > 1) {
        // value of capacitor is correct
        (void) eeprom_write_word((uint16_t *)(&ref_offset), load_diff);	// hold zero offset + slew rate dependend offset
        lcd_clear();
        lcd_fix2_string(REF_C_str);	// "REF_C="
        lcd_string(itoa(load_diff, outval, 10));	//output REF_C_KORR
#if 0
//#######################################
        // Test for switching level of the digital input of port TP3
        for (ii=0;ii<8;ii++) {
        ADC_PORT =  TXD_VAL;	//ADC-Port 1 to GND
        ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
        R_PORT = 2<<(TP3*2);		//Pin 3 over R_H to VCC
        R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to VCC
        while (1) {
           wdt_reset();
           if ((ADC_PIN&(1<<TP3)) == (1<<TP3)) break;
        }
        R_DDR = 0;		//Pin 3 without current
        R_PORT = 0;
        adcmv[0] = ReadADC(TP3);
        lcd_line3();
        DisplayValue(adcmv[0],-3,'V',4);
        R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to GND
        while (1) {
           wdt_reset();
           if ((ADC_PIN&(1<<TP3)) != (1<<TP3)) break;
        }
        R_DDR = 0;		//Pin 3 without current
        lcd_line4();
        adcmv[0] = ReadADC(TP3);
        DisplayValue(adcmv[0],-3,'V',4);
        wait_about1s();
        }
//#######################################
#endif
  #ifdef AUTOSCALE_ADC
        ADC_PORT =  TXD_VAL;	//ADC-Port 1 to GND
        ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
        R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to GND
        do {
           adcmv[0] = ReadADC(TP3);
        } while (adcmv[0] > 980);
        R_DDR = 0;		//all Pins to input 
        ADCconfig.U_Bandgap = 0;	// do not use internal Ref
        adcmv[0] = ReadADC(TP3);  // get cap voltage with VCC reference
        ADCconfig.U_Bandgap = ADC_internal_reference;
        adcmv[1] = ReadADC(TP3);	// get cap voltage with internal reference
        ADCconfig.U_Bandgap = 0;	// do not use internal Ref
        adcmv[2] = ReadADC(TP3);  // get cap voltage with VCC reference
        ADCconfig.U_Bandgap = ADC_internal_reference;
        udiff = (int8_t)(((signed long)(adcmv[0] + adcmv[2] - adcmv[1] - adcmv[1])) * ADC_internal_reference / (2*adcmv[1]))+REF_R_KORR;
        lcd_line2();
        lcd_fix2_string(REF_R_str);	// "REF_R="
        udiff2 = udiff + (int8_t)eeprom_read_byte((uint8_t *)(&RefDiff));
        (void) eeprom_write_byte((uint8_t *)(&RefDiff), (uint8_t)udiff2);	// hold offset for true reference Voltage
        lcd_string(itoa(udiff2, outval, 10));	//output correction voltage
  #endif
        wait_about4s();
        break;
     }
     lcd_line2();
     DisplayValue(cap.cval,cap.cpre,'F',4);
     wait_about200ms();			// wait additional time
  } // end for ww
 #endif


  ADCconfig.Samples = ANZ_MESS;	// set to configured number of ADC samples
  lcd_clear();
  lcd_fix2_string(VERSION_str);	//"Version ..."
  lcd_line2();
  lcd_fix_string(ATE);		//"Selftest End"

 #ifdef FREQUENCY_50HZ
//#define TEST_SLEEP_MODE	/* only select for checking the sleep delay */
  lcd_fix_string(T50HZ);	//" 50Hz"
  ADC_PORT = TXD_VAL;
  ADC_DDR = 1<<TP1 | TXD_MSK;	// Pin 1 to GND
  R_DDR = (1<<(TP3*2)) | (1<<(TP2*2));
  for(ww=0;ww<30;ww++) {	// repeat the signal up to 30 times (1 minute)
     for (ii=0;ii<100;ii++) {	// for 2 s generate 50 Hz
         R_PORT = (1<<(TP2*2));	// Pin 2 over R_L to VCC, Pin 3 over R_L to GND
  #ifdef TEST_SLEEP_MODE
         sleep_5ms(2); 		// test of timing of sleep mode call 
  #else
         wait10ms();		// normal delay
  #endif
         R_PORT = (1<<(TP3*2));	// Pin 3 over R_L to VCC, Pin 2 over R_L to GND
  #ifdef TEST_SLEEP_MODE
         sleep_5ms(2); 		// test of timing of sleep mode call 
  #else
         wait10ms();		// normal delay
  #endif
         wdt_reset();
     }
     if (!(ON_PIN_REG & (1<<RST_PIN))) {
        // if key is pressed, don't repeat
        break;
     }
  }
 #endif
 PartFound = PART_NONE;
 wait_about1s();			//wait 1 seconds
 lcd_line2();
 lcd_clear_line();			// clear total line
#endif
 } 
 
#ifdef RequireShortedProbes
/*
 *  check for a short circuit between two probes
 *  from Markus R.
 *
 *  requires:
 *  - ID of first probe (0-2)
 *  - ID of second probe (0-2)
 *
 *  returns:
 *  - 0 if not shorted
 *  - 1 if shorted
 */

uint8_t ShortedProbes(uint8_t Probe1, uint8_t Probe2)
{
  uint8_t           Flag1 = 0;      /* return value */
  unsigned int      U1;            /* voltage at probe #1 in mV */
  unsigned int      U2;            /* voltage at probe #2 in mV */
  unsigned int      URH;	   /* half of reference voltage */
  /*
   *  Set up a voltage divider between the two probes:
   *  - Probe1: Rl pull-up
   *  - Probe2: Rl pull-down
   */

  ADC_DDR =  TXD_MSK;		// all-Pins to Input
  ADC_PORT = TXD_VAL;		// all ADC-Ports to GND
  R_PORT = pgm_read_byte(&PinRLtab[Probe1]);
  R_DDR = pgm_read_byte(&PinRLtab[Probe1]) | pgm_read_byte(&PinRLtab[Probe2]);

  /* read voltages */
  U1 = ReadADC(Probe1);
  U2 = ReadADC(Probe2);

  /*
   *  We expect both probe voltages to be about the same and
   *  to be half of Vcc (allowed difference +/- 20mV).
   */
  #define MAX_UH_DIFF 20

  URH = ADCconfig.U_AVCC / 2;
  if ((U1 > (URH - MAX_UH_DIFF)) && (U1 < (URH + MAX_UH_DIFF)))
  {
    if ((U2 > (URH - MAX_UH_DIFF)) && (U2 < (URH + MAX_UH_DIFF)))
    {
      Flag1 = 1;
    }
  }

  /* reset port */
  R_DDR = 0;

  return Flag1;
}
/*
 *  check for a short circuit between all probes
 *  from Markus R.
 *
 *  returns:
 *  - 0 if no probes are short-circuited
 *  - number of probe pairs short-circuited (3 = all)
 */

uint8_t AllProbesShorted(void)
{
  uint8_t           Flag2;      /* return value */

  /* check all possible combinations */
  Flag2 = ShortedProbes(TP1, TP2);
  Flag2 += ShortedProbes(TP1, TP3);
  Flag2 += ShortedProbes(TP2, TP3);

  return Flag2;
}
#endif

