void AutoCheck(void) {
  uint8_t tt;		// number of running test
  uint8_t ww;		// counter for repeating the tests
  int8_t udiff;		// difference between ADC Voltage with VCC or Bandgap reference
  int  adcmv[7];
// define the maximum count of repetitions MAX_REP
#define MAX_REP 8

#ifdef AUTO_CAL
  uint16_t sum_c0;	// sum of empty probe C measurement
  uint8_t c0_count;	// counter for accumulated Cap measurements
  uint8_t err_flag=1;	//error flag
  int8_t udiff2;
  uint16_t sum_rm=0;	// sum of 3 Pin voltages with 680 Ohm load
  uint16_t u680;	// 3 * (Voltage at 680 Ohm)
  uint16_t pin_rp;
  uint16_t pin_rm;
#endif
  ADC_PORT = TXD_VAL;
  ADC_DDR = TXD_MSK;
  R_PORT = (1<<(TP1*2));		//Pin 1 over RL to +
  R_DDR = (1<<(TP1*2)) | (1<<(TP2*2));	//Pin 2 over RL to -
  adcmv[0] = W5msReadADC(PC0);		// voltage at Pin 1
  adcmv[1] = ReadADC(PC1);		// voltage at Pin 2
  adcmv[2] = ReadADC(PC2);		// voltage at Pin 3
  if (abs((int)(adcmv[1] - adcmv[0])) > 20) {
     return;				//difference to big, no selftest
  }
  if (abs((int)(adcmv[2] - adcmv[0])) > 20) {
     return;				//difference to big, no selftest
  }
  if (abs((int)(adcmv[0] - (U_VCC/2))) > 30) {
     return;				//difference to big, no selftest
  }
  R_DDR = (1<<(TP1*2)) | (1<<(TP3*2));	//Pin 3 over RL to - (Pin 1 over RL to +)
  adcmv[0] = W5msReadADC(PC0);		// voltage at Pin 1
  adcmv[1] = ReadADC(PC1);		// voltage at Pin 2
  adcmv[2] = ReadADC(PC2);		// voltage at Pin 3
  if (abs((int)(adcmv[1] - adcmv[0])) > 20) {
     return;				//difference to big, no selftest
  }
  if (abs((int)(adcmv[2] - adcmv[0])) > 20) {
     return;				//difference to big, no selftest
  }
  if (abs((int)(adcmv[0] - (U_VCC/2))) > 30) {
     return;				//difference to big, no selftest
  }
  lcd_clear();
  lcd_line1();
  lcd_fix2_string(SELFTEST);		// "Selftest mode.."
  wait1s();
#ifdef AUTO_CAL
 #define TEST_COUNT 12
#else
 #define TEST_COUNT 10
#endif
 
  for(tt=1;tt<TEST_COUNT;tt++) {		// loop for all Tests
#ifdef AUTO_CAL
     sum_c0 = 0;			//reset sum of capacity measurements
     c0_count = 0;			//counter for capacity measurement
#endif
     for(ww=0;ww<MAX_REP;ww++) {	// repeat the test MAX_REP times
        lcd_line2();			//Cursor to column 1, row 2
        lcd_clear_line();		// clear total line
        lcd_line1();			//Cursor to column 1, row 1
        lcd_clear_line();		// clear total line
        lcd_line1();			//Cursor to column 1, row 1
        lcd_data('T');			//output the Testmode "T"
        lcd_string(utoa(tt, outval, 10));	//output Test number
        lcd_data(' ');
                                        //############################################
        if (tt == 1) {   // output of reference voltage and factors for capacity measurement
           Config.Samples = 190;	// set number of ADC reads near to maximum
           (void) ReadADC(0x0e);	// read reference voltage 
           ref_mv = W5msReadADC(0x0e);	// read reference voltage 
           lcd_fix2_string(URefT);	//"URef="
           lcd_string(utoa(ref_mv, outval, 10));
           lcd_fix2_string(mVT);		//"mV "
           lcd_line2();			//Cursor to column 1, row 2
#ifdef WITH_AUTO_REF
           RefVoltage();		//compute RHmultip = f(reference voltage)
#endif
           lcd_fix2_string(RHfakt);	//"RHf="
           lcd_string(utoa(RHmultip, outval, 10));
        }
                                        //############################################
        if (tt == 2) { // how equal are the RL resistors? 
           R_PORT = 1<<(TP1*2);		//RL1 to VCC
           R_DDR = (1<<(TP1*2)) | (1<<(TP2*2));	//RL2 to -
           adcmv[0] = W20msReadADC(TP1);
           adcmv[0] -= ((long)U_VCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + R_L_VAL + PIN_RP));
           R_DDR = (1<<(TP1*2)) | (1<<(TP3*2));	//RL3 to -
           adcmv[1] = W20msReadADC(TP1);
           adcmv[1] -= ((long)U_VCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + R_L_VAL + PIN_RP));
           R_PORT = 1<<(TP2*2);		//RL2 to VCC
           R_DDR = (1<<(TP2*2)) | (1<<(TP3*2));	//RL3 to -
           adcmv[2] = W20msReadADC(TP2);
           adcmv[2] -= ((long)U_VCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + R_L_VAL + PIN_RP));
           lcd_fix2_string(RLRL);	// "RLRL"
        }
                                        //############################################
        if (tt == 3) { // how equal are the RH resistors
           R_PORT = 2<<(TP1*2);		//RH1 to VCC
           R_DDR = (2<<(TP1*2)) | (2<<(TP2*2));	//RH2 to -
           adcmv[0] = W20msReadADC(TP1);
           adcmv[0] -= (U_VCC/2);
           R_DDR = (2<<(TP1*2)) | (2<<(TP3*2));	//RH3 to -
           adcmv[1] = W20msReadADC(TP1);
           adcmv[1] -= (U_VCC/2);
           R_PORT = 2<<(TP2*2);		//RL2 to VCC
           R_DDR = (2<<(TP2*2)) | (2<<(TP3*2));	//RH3 to -
           adcmv[2] = W20msReadADC(TP2);
           adcmv[2] -= (U_VCC/2);
           lcd_fix2_string(RHRH);	// "RHRH"
        }
                                        //############################################
        if (tt == 4) { // Text release probes
           lcd_fix2_string(RELPROBE);	// "Release Probes"
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
           lcd_fix2_string(RH1L);	// "RH_Lo="
        }
                                        //############################################
        if (tt == 6) { // can we switch the ADC pins to VCC across the R_H resistor?
           R_DDR = 2<<(TP1*2);		//Pin 1 over R_H to VCC
           R_PORT = 2<<(TP1*2);
           adcmv[0] = W20msReadADC(TP1) - U_VCC;
           R_DDR = 2<<(TP2*2);		//Pin 2 over R_H to VCC
           R_PORT = 2<<(TP2*2);
           adcmv[1] = W20msReadADC(TP2) - U_VCC;
           R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to VCC
           R_PORT = 2<<(TP3*2);
           adcmv[2] = W20msReadADC(TP3) - U_VCC;
           lcd_fix2_string(RH1H);	// "RH_Hi="
        }
                                        //############################################
        if (tt == 7) { // measurement of internal resistance of the ADC port outputs switched to GND
           ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
           R_PORT = 1<<(TP1*2);		//R_L-PORT 1 to VCC
           R_DDR = 1<<(TP1*2);		//Pin 1 to output and over R_L to VCC
           adcmv[0] = W5msReadADC(TP1);

           ADC_DDR = 1<<TP2 | TXD_MSK;	//ADC-Pin 2 to output 0V
           R_PORT =  1<<(TP2*2);	//R_L-PORT 2 to VCC
           R_DDR = 1<<(TP2*2);		//Pin 2 to output and over R_L to VCC
           adcmv[1] = W5msReadADC(TP2);

           ADC_DDR = 1<<TP3 | TXD_MSK;	//ADC-Pin 3 to output 0V
           R_PORT =  1<<(TP3*2);	//R_L-PORT 3 to VCC
           R_DDR = 1<<(TP3*2);		//Pin 3 to output and over R_L to VCC
           adcmv[2] = W5msReadADC(TP3);
           lcd_fix2_string(RILO);	// "RiLo="
#ifdef AUTO_CAL
           err_flag = 1;		// set to not finished
           sum_c0 += adcmv[0] + adcmv[1] + adcmv[2]; //add all three values
           c0_count += 3;
           if (c0_count == (MAX_REP*3)) {
              //last repetition of measurement
              sum_c0 /= MAX_REP;
              sum_rm = sum_c0;		// sum of 3 Pin voltages switched to GND
              err_flag = 0;		// clear not finished flag
           }
#endif
        }
                                        //############################################
        if (tt == 8) { // measurement of internal resistance of the ADC port output switched to VCC
           R_PORT = 0;			// R-Ports to GND
           ADC_PORT = 1<<TP1 | TXD_VAL;	//ADC-Port 1 to VCC
           ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
           R_DDR = 1<<(TP1*2);		//Pin 1 to output and over R_L to GND
           adcmv[0] = U_VCC - W5msReadADC(TP1);
      
           ADC_PORT = 1<<TP2 | TXD_VAL;	//ADC-Port 2 to VCC
           ADC_DDR = 1<<TP2 | TXD_MSK;	//ADC-Pin  2 to output 0V
           R_DDR = 1<<(TP2*2);		//Pin 2 to output and over R_L to GND
           adcmv[1] = U_VCC - W5msReadADC(TP2);

           ADC_PORT = 1<<TP3 | TXD_VAL;	//ADC-Port 3 to VCC
           ADC_DDR = 1<<TP3 | TXD_MSK;	//ADC-Pin  3 to output 0V
           R_DDR = 1<<(TP3*2);		//Pin 3 to output and over R_L to GND
           adcmv[2] = U_VCC - W5msReadADC(TP3);

           lcd_fix2_string(RIHI);	// "RiHi="
#ifdef AUTO_CAL
           sum_c0 += adcmv[0] + adcmv[1] + adcmv[2];
           c0_count += 3;
           if ((err_flag == 0) && (c0_count == (MAX_REP*3))) {
              //last repetition of measurement
              sum_c0 /= MAX_REP;		// sum of 3 Pin voltages switched to VCC
              u680 = ((U_VCC * 3) - sum_rm - sum_c0);	//three times the voltage at the 680 Ohm
              pin_rm = (unsigned long)((unsigned long)sum_rm * (unsigned long)R_L_VAL) / (unsigned long)u680;
              adcmv[2] = pin_rm;	// for last output in row 2
              pin_rp = (unsigned long)((unsigned long)sum_c0 * (unsigned long)R_L_VAL) / (unsigned long)u680;
              if ((pin_rp < 280) && (pin_rm < 250)) {
                 // rp is below 28 Ohm and rm is below 25 Ohm
                 lcd_string(utoa(pin_rp, outval, 10));	//write value to LCD
                 (void) eeprom_write_word((uint16_t *)(&R680pl), pin_rp+R_L_VAL);  //hold VCC resistance value in EEprom
                 (void) eeprom_write_word((uint16_t *)(&R680mi), pin_rm+R_L_VAL);  //hold GND resistance value in EEprom
              }
           }
#endif
        }
#ifdef C_MESS
                                        //############################################
        if (tt == 9) {			//measure Zero offset for Capacity measurement
 #ifdef AUTO_CAL
           if (ww == 0) { //first step of loop
              // clear all sum
              for (c0_count=0;c0_count<7;c0_count++) adcmv[c0_count] = 0;
           }
           PartFound = PART_NONE;
           ReadCapacity(TP3, TP1);
           if (cval_uncorrected > 70) break;
           adcmv[5] += (unsigned char) cval_uncorrected;	//save capacity value of empty Pin 1:3
           ReadCapacity(TP3, TP2);
           if (cval_uncorrected > 70) break;
           adcmv[6] += (unsigned char) cval_uncorrected;	//save capacity value of empty Pin 2:3
           ReadCapacity(TP2, TP1);
           if (cval_uncorrected > 70) break;
           adcmv[2] += (unsigned char) cval_uncorrected;	//save capacity value of empty Pin 1:2
           ReadCapacity(TP1, TP3);
           if (cval_uncorrected > 70) break;
           adcmv[1] += (unsigned char) cval_uncorrected;	//save capacity value of empty Pin 3:1
           ReadCapacity(TP2, TP3);
           if (cval_uncorrected > 70) break;
           adcmv[4] += (unsigned char) cval_uncorrected;	//save capacity value of empty Pin 3:2
           ReadCapacity(TP1, TP2);
           if (cval_uncorrected > 70) break;
           adcmv[0] += (unsigned char) cval_uncorrected;	//save capacity value of empty Pin 2:1
           if (ww == (MAX_REP - 1)) {
              for (c0_count=0;c0_count<7;c0_count++) {
                 adcmv[c0_count] /= MAX_REP;
                 // write all zero offsets to the EEprom 
                 (void) eeprom_write_byte((uint8_t *)(&c_zero_tab[c0_count]),adcmv[c0_count]+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2)));
              }
              lcd_data('C');
              lcd_data('0');
           }
           adcmv[0] = adcmv[5];		// for output
           adcmv[1] = adcmv[6];		// for output
 #else
           // No AUTO_CAL mode, show only values
           ReadCapacity(TP3, TP1);
           adcmv[0] = (unsigned int) cval_uncorrected;	//save capacity value of empty Pin 1:3
           ReadCapacity(TP3, TP2);
           adcmv[1] = (unsigned int) cval_uncorrected;	//save capacity value of empty Pin 2:3
           ReadCapacity(TP2, TP1);
           adcmv[2] = (unsigned int) cval_uncorrected;	//save capacity value of empty Pin 1:2
 #endif
        }
 #ifdef AUTO_CAL
                                        //############################################
        if (tt == 10) {			// Message C > 100nF
           lcd_data('1');
           lcd_fix_string(CapZeich);	// "-||-"
           lcd_data('3');
           lcd_fix2_string(MinCap);	// " >100nF"
           PartFound = PART_NONE;
           err_flag = 1;		// C is not found
           ReadCapacity(TP3, TP1);	// look for capacitor > 100nF
           if (((cpre == 0) && (cval > 95000)) || ((cpre == 1) && (cval < 30000))) {
              // the right capacitor is detected, finish with next step
              ww = MAX_REP;
              err_flag = 0;		// Capacitor is found
           } else {
              wait2s();			// wait additional time
           }
           // next step will repeat measurement
        }
                                        //############################################
        if (tt == 11) {			//measure  offset Voltage of analog Comparator for Capacity measurement
           PartFound = PART_NONE;
           if (err_flag != 0) break;
           ReadCapacity(TP3, TP1);	// look for capacitor > 100nF
           if (((cpre == 0) && (cval > 95000)) || ((cpre == 1) && (cval < 30000))) {
              // value of capacitor is correct
              (void) eeprom_write_word((uint16_t *)(&ref_offset), load_diff);	// hold zero offset + slew rate dependend offset
              lcd_line2();		//Cursor to column 1, row 2
              lcd_fix2_string(REF_Cstr);	// "REF_C="
              lcd_string(itoa(load_diff, outval, 10));	//output REF_C_KORR
  #ifdef AUTOSCALE_ADC
              ADC_PORT =  TXD_VAL;	//ADC-Port 1 to GND
              ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
              R_DDR = 2<<(TP3*2);		//Pin 3 over R_H to GND
              do {
                adcmv[0] = ReadADC(TP3);
              } while (adcmv[0] > 980);
              R_DDR = 0;		//all Pins to input 
              Config.U_Bandgap = 0;	// do not use internal Ref
              adcmv[0] = ReadADC(TP3);  // get cap voltage with VCC reference
              Config.U_Bandgap = ADC_internal_reference;
              adcmv[1] = ReadADC(TP3);	// get cap voltage with internal reference
              Config.U_Bandgap = 0;	// do not use internal Ref
              adcmv[2] = ReadADC(TP3);  // get cap voltage with VCC reference
              Config.U_Bandgap = ADC_internal_reference;
              udiff = (int8_t)(((signed long)(adcmv[0] + adcmv[2] - adcmv[1] - adcmv[1])) * ADC_internal_reference / (2*adcmv[1]))+REF_R_KORR;
              lcd_data(' ');
              lcd_data('_');
              lcd_data('R');
              lcd_data('=');
              udiff2 = udiff + (int8_t)eeprom_read_byte((uint8_t *)(&RefDiff));
              if (udiff != 0) {
                 (void) eeprom_write_byte((uint8_t *)(&RefDiff), (uint8_t)udiff2);	// hold offset for true reference Voltage
              } else {
                 lcd_data('=');		// mark as identical
              }
              lcd_string(itoa(udiff2, outval, 10));	//output correction voltage
  #endif
              wait4s();
              break;
           }
        }
 #endif
#endif
        if ((tt > 1) && (tt <10)) {	// output 3 voltages 
           lcd_line2();			//Cursor to column 1, row 2
           lcd_string(itoa(adcmv[0], outval, 10));	//output voltage 1
           lcd_data(' ');
           lcd_string(itoa(adcmv[1], outval, 10));	//output voltage 2
           lcd_data(' ');
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
        wait500ms();
        if(!(ON_PIN_REG & (1<<RST_PIN))) {
	   // if key is pressed, don't repeat
           break;
        }
        wait500ms();
     } //end for ww
     wait1s();
  } //end for tt
  Config.Samples = ANZ_MESS;	// set to configured number of ADC samples
  lcd_clear();
//  lcd_line1();
  lcd_fix2_string(ATE);		//"Selftest End"
  lcd_line2();
  lcd_fix_string(VERSION);	//"Version ..."
 #ifdef FREQUENCY_50HZ
  lcd_line1();
  lcd_fix2_string(ATE);		//"Selftest End"
  lcd_data(' ');
  lcd_data('5');		// add text "50Hz"
  lcd_data('0');
  lcd_data('H');
  lcd_data('z');
  ADC_PORT = TXD_VAL;
  ADC_DDR = 1<<TP1 | TXD_MSK;	// Pin 1 to GND
  R_DDR = (1<<(TP3*2)) | (1<<(TP2*2));
  for(ww=0;ww<30;ww++) {	// repeat the signal up to 30 times (1 minute)
     for (ii=0;ii<100;ii++) {	// for 2 s generate 50 Hz
         R_PORT = (1<<(TP2*2));	// Pin 2 over R_L to VCC, Pin 3 over R_L to GND
         wait10ms();
         R_PORT = (1<<(TP3*2));	// Pin 3 over R_L to VCC, Pin 2 over R_L to GND
         wait10ms();
         wdt_reset();
     }
     if (!(ON_PIN_REG & (1<<RST_PIN))) {
        // if key is pressed, don't repeat
        break;
     }
  }
 #endif
 PartFound = PART_NONE;
 wait1s();			//wait 1 seconds
 } 
 
