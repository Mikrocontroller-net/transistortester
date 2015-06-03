// Selftest of the device and calibration 
#ifdef WITH_SELFTEST
void AutoCheck(uint8_t test_mode) {
  /* (test_mode & 0x0f) == 0 , only calibration without T1-T7 */
  /* (test_mode & 0x0f) == 1 , calibration and additional T1-T7 */
  /* (test_mode & 0xf0) == 0 , check for shorted probes, if unshorted, return */
  /* (test_mode & 0xf0) == 0x10 , ask for shorted probes */

 uint8_t ww;		// counter for repeating the tests
 int  adcmv[7];
 #ifdef EXTENDED_TESTS
  uint16_t u680;	// 3 * (Voltage at 680 Ohm)
  uint8_t taste;	// ist key pressed? 0 = no
 #else
  #ifndef NO_TEST_T1_T7
  #warning "Selftest without extended tests T1 to T7!"
  #endif
 #endif
 #if defined(EXTENDED_TESTS) || defined(WITH_MENU)
  uint8_t tt;		// number of running test
 #endif

 #ifdef FREQUENCY_50HZ
  uint8_t ff50;		// loop counter for 2s
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
PartFound = PART_NONE;		// no part found before
if ((test_mode & 0xf0) == 0) {
  // probed should be shorted already to begin selftest
  if (AllProbesShorted() != 3) return;
  lcd_clear();
  lcd_MEM_string(SELFTEST);		// "Selftest mode.."
  lcd_line2();
  lcd_data('?');			// wait for key pressed for confirmation
  if (wait_for_key_ms(2000) > 10) goto begin_selftest;	// key is pressed again
 #ifdef WITH_MENU
} else {
  // report to user, that probes should be shorted
  ww = 0;
  for (tt=0;tt<150;tt++) {	/* wait about 30 seconds for shorted probes */
    lcd_clear();
    lcd_MEM2_string(SHORT_PROBES_str);	// message "Short probes!" to LCD
    if (AllProbesShorted() == 3) {
       ww++;	// all probes now shorted
    } else {
       ww = 0;	// connection not stable, retry
    }
    if (ww > 3) break;	// connection seems to be stable
    lcd_refresh();		// write the pixels to display, ST7920 only
    wait_about200ms();			// wait 200ms and try again
  }  /* end for (tt...) */
  if (tt < 150) goto begin_selftest;		// is shorted before time limit
  goto no_zero_resistance;			// skip measuring of the zero resistance
 #endif
}
// no key pressed for 2s
lcd_clear();
lcd_MEM2_string(VERSION_str);	//"Version ..."
return;

begin_selftest:
lcd_line2();
lcd_MEM2_string(R0_str);		// "R0="
eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[2]), (uint8_t)0);	// clear zero offset
eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[3]), (uint8_t)0);	// clear zero offset
eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[1]), (uint8_t)0);	// clear zero offset

adcmv[0] = GetESR(TP3, TP1);
adcmv[1] = GetESR(TP3, TP2);
adcmv[2] = GetESR(TP2, TP1);
DisplayValue(adcmv[0],-2,' ',3);
DisplayValue(adcmv[1],-2,' ',3);
DisplayValue(adcmv[2],-2,LCD_CHAR_OMEGA,3);
if (adcmv[0] >= 90) {
  adcmv[0] = ESR_ZERO;	// set back to default value
}
eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[2]), (uint8_t)adcmv[0]);	// fix zero offset
if (adcmv[1] >= 90) {
  adcmv[1] = ESR_ZERO;	// set back to default value
}
eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[3]), (uint8_t)adcmv[1]);	// fix zero offset
if (adcmv[2] >= 90) {
  adcmv[2] = ESR_ZERO;	// set back to default value
}
eeprom_write_byte((uint8_t *)(&EE_ESR_ZEROtab[1]), (uint8_t)adcmv[2]);	// fix zero offset
last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2

 #ifdef WITH_MENU
no_zero_resistance:
 #endif
 #ifdef EXTENDED_TESTS
#define TEST_COUNT 8
if((test_mode & 0x0f) == 1) {  /* full test requested */

for(tt=1;tt<TEST_COUNT;tt++) {		// loop for all Tests
for(ww=0;ww<MAX_REP;ww++) {	// repeat the test MAX_REP times
   lcd_clear_line2();		// clear total line 2
   lcd_clear_line1();		// clear total line 1
   lcd_data('T');			//output the Testmode "T"
   u2lcd(tt);		//lcd_string(utoa(tt, outval, 10));	//output Test number
   lcd_space();
				//############################################
   if (tt == 1) {   // output of reference voltage and factors for capacity measurement
      Calibrate_UR();		// get Reference voltage, Pin resistance
      lcd_MEM2_string(URefT);	//"URef="
      Display_mV(ref_mv,4);
      lcd_line2();			//Cursor to column 1, row 2
      lcd_MEM2_string(RHfakt);	//"RHf="
      u2lcd(RHmultip);	//lcd_string(utoa(RHmultip, outval, 10));
      ADCconfig.Samples = R_ANZ_MESS;	// set number of ADC reads near to maximum
   }
				//############################################
   if (tt == 2) { // how equal are the RL resistors? 
      u680 = ((long)ADCconfig.U_AVCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + R_L_VAL + PIN_RP));
      R_PORT = 1<<PIN_RL1;		//RL1 to VCC
      R_DDR = (1<<PIN_RL1) | (1<<PIN_RL2);	//RL2 to -
      adcmv[0] = W20msReadADC(TP1);
      adcmv[0] -= u680;
      R_DDR = (1<<PIN_RL1) | (1<<PIN_RL3);	//RL3 to -
      adcmv[1] = W20msReadADC(TP1);
      adcmv[1] -= u680;
      R_PORT = 1<<PIN_RL2;		//RL2 to VCC
      R_DDR = (1<<PIN_RL2) | (1<<PIN_RL3);	//RL3 to -
      adcmv[2] = W20msReadADC(TP2);
      adcmv[2] -= u680;
      lcd_MEM_string(RLRL);	// "RLRL"
   }
				//############################################
   if (tt == 3) { // how equal are the RH resistors
      R_PORT = 1<<PIN_RH1;		//RH1 to VCC
      R_DDR = (1<<PIN_RH1) | (1<<PIN_RH2);	//RH2 to -
      adcmv[0] = W20msReadADC(TP1);
      adcmv[3] = ADCconfig.U_AVCC / 2;
      adcmv[0] -= adcmv[3];
      R_DDR = (1<<PIN_RH1) | (1<<PIN_RH3);	//RH3 to -
      adcmv[1] = W20msReadADC(TP1);
      adcmv[1] -= adcmv[3];
      R_PORT = 1<<PIN_RH2;		//RH2 to VCC
      R_DDR = (1<<PIN_RH2) | (1<<PIN_RH3);	//RH3 to -
      adcmv[2] = W20msReadADC(TP2);
      adcmv[2] -= adcmv[3];
      lcd_MEM_string(RHRH);	// "RHRH"
   }
				//############################################
   if (tt == 4) { // Text release probes
      lcd_MEM_string(RELPROBE);	// "Release Probes"
      if (AllProbesShorted() != 0) ww = MAX_REP-2;
   }
				//############################################
   if (tt == 5) { // can we switch the ADC pins to GND across R_H resistor?
      R_PORT = 0;
      R_DDR = 1<<PIN_RH1;		//Pin 1 over R_H to GND
      adcmv[0] = W20msReadADC(TP1);

      R_DDR = 1<<PIN_RH2;		//Pin 2 over R_H to GND
      adcmv[1] = W20msReadADC(TP2);

      R_DDR = 1<<PIN_RH3;		//Pin 3 over R_H to GND
      adcmv[2] = W20msReadADC(TP3);
      lcd_MEM_string(RH1L);	// "RH_Lo="
   }
				//############################################
   if (tt == 6) { // can we switch the ADC pins to VCC across the R_H resistor?
      R_DDR = 1<<PIN_RH1;		//Pin 1 over R_H to VCC
      R_PORT = 1<<PIN_RH1;
      adcmv[0] = W20msReadADC(TP1) - ADCconfig.U_AVCC;
      R_DDR = 1<<PIN_RH2;		//Pin 2 over R_H to VCC
      R_PORT = 1<<PIN_RH2;
      adcmv[1] = W20msReadADC(TP2) - ADCconfig.U_AVCC;
      R_DDR = 1<<PIN_RH3;		//Pin 3 over R_H to VCC
      R_PORT = 1<<PIN_RH3;
      adcmv[2] = W20msReadADC(TP3) - ADCconfig.U_AVCC;
      lcd_MEM_string(RH1H);	// "RH_Hi="
   }
   if (tt == 7) { // is the voltage of all R_H / R_L dividers correct?
      u680 = ((long)ADCconfig.U_AVCC * (PIN_RM + R_L_VAL) / (PIN_RM + R_L_VAL + (unsigned long)R_H_VAL*100));
      R_PORT = 1<<PIN_RH1;		//RH1 to VCC
      R_DDR = (1<<PIN_RH1) | (1<<PIN_RL1);	//RH1 to +, RL1 to -
      adcmv[0] = W20msReadADC(TP1);
      adcmv[0] -= u680;
      R_PORT = 1<<PIN_RH2;		//RH2 to VCC
      R_DDR = (1<<PIN_RH2) | (1<<PIN_RL2);	//RH2 to +, RL2 to -
      adcmv[1] = W20msReadADC(TP2);
      adcmv[1] -= u680;
      R_PORT = 1<<PIN_RH3;		//RH3 to VCC
      R_DDR = (1<<PIN_RH3) | (1<<PIN_RL3);	//RH3 to +, RL3 to -
      adcmv[2] = W20msReadADC(TP3);
      adcmv[2] -= u680;
      lcd_MEM_string(RHRL);	// "RH/RL"
   }
				//############################################
   if (tt > 1) {	// output 3 voltages 
      lcd_line2();			//Cursor to column 1, row 2
      i2lcd(adcmv[0]);		// lcd_string(itoa(adcmv[0], outval, 10));	//output voltage 1
      lcd_space();
      i2lcd(adcmv[1]);		// lcd_string(itoa(adcmv[1], outval, 10));	//output voltage 2
      lcd_space();
      i2lcd(adcmv[2]);		// lcd_string(itoa(adcmv[2], outval, 10));	//output voltage 3
   }
   ADC_DDR =  TXD_MSK;		// all-Pins to Input
   ADC_PORT = TXD_VAL;		// all ADC-Ports to GND
   R_DDR = 0;			// all R-Ports to Input
   R_PORT = 0;
   taste = wait_for_key_ms(1000);	// wait up to 1 second or key is pressed
   if ((tt != 4) && (taste > 10)) {
      // don't finish repetition  for T4 with pressed key
      break; // if key is pressed, don't repeat
   }
} //end for ww
//wait_about1s();
wait_for_key_ms(1000);	// wait up to 1 second or key is pressed
} //end for tt
  #if PROCESSOR_TYP == 1280
lcd_clear();
lcd_testpin(TP1);
lcd_data('L');
lcd_equal();			// lcd_data('=');
ADC_PORT = TXD_VAL;
ADC_DDR = (1<<TP1) | TXD_MSK;
R_PORT = (1<<PIN_RL1);
R_DDR = (1<<PIN_RL1);
adcmv[0] = W5msReadADC(TP1);
ADCSRB = (1<<MUX5);		// switch to upper 8 MUX inputs
adcmv[1] = ReadADC(PIN_RL1);
ADCSRB = 0;			// switch back to lower 8 MUX inputs
ResistorVal[0] = (adcmv[0] * (unsigned long)R_L_VAL) / (adcmv[1] - adcmv[0]);
DisplayValue(ResistorVal[0],-1,LCD_CHAR_OMEGA,3);
lcd_space();
lcd_data('H');
lcd_equal();			// lcd_data('=');
ResistorVal[1] = ((ADCconfig.U_AVCC - adcmv[1]) * (unsigned long)R_L_VAL) / (adcmv[1] - adcmv[0]);
DisplayValue(ResistorVal[1],-1,LCD_CHAR_OMEGA,3);
lcd_line2();
lcd_testpin(TP1);
lcd_space();
lcd_data('H');
lcd_equal();			// lcd_data('=');
ADC_PORT = (1<<TP1) | TXD_VAL;
R_PORT = 0;
adcmv[0] = W5msReadADC(TP1);
ADCSRB = (1<<MUX5);		// switch to upper 8 MUX inputs
adcmv[1] = ReadADC(PIN_RL1);
ADCSRB = 0;			// switch back to lower 8 MUX inputs
ResistorVal[1] = ((ADCconfig.U_AVCC - adcmv[0]) * (unsigned long)R_L_VAL) / (adcmv[0] - adcmv[1]);
DisplayValue(ResistorVal[1],-1,LCD_CHAR_OMEGA,3);
lcd_space();
lcd_data('L');
lcd_equal();			// lcd_data('=');
ResistorVal[0] = (adcmv[1] * (unsigned long)R_L_VAL) / (adcmv[0] - adcmv[1]);
DisplayValue(ResistorVal[0],-1,LCD_CHAR_OMEGA,3);
wait_about1s();
last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2
// 
lcd_clear();
lcd_testpin(TP2);
lcd_data('L');
lcd_equal();			// lcd_data('=');
ADC_PORT = TXD_VAL;
ADC_DDR = (1<<TP2) | TXD_MSK;
R_PORT = (1<<PIN_RL2);
R_DDR = (1<<PIN_RL2);
adcmv[0] = W5msReadADC(TP2);
ADCSRB = (1<<MUX5);		// switch to upper 8 MUX inputs
adcmv[1] = ReadADC(PIN_RL2);
ADCSRB = 0;			// switch back to lower 8 MUX inputs
ResistorVal[0] = (adcmv[0] * (unsigned long)R_L_VAL) / (adcmv[1] - adcmv[0]);
DisplayValue(ResistorVal[0],-1,LCD_CHAR_OMEGA,3);
lcd_space();
lcd_data('H');
lcd_equal();			// lcd_data('=');
ResistorVal[1] = ((ADCconfig.U_AVCC - adcmv[1]) * (unsigned long)R_L_VAL) / (adcmv[1] - adcmv[0]);
DisplayValue(ResistorVal[1],-1,LCD_CHAR_OMEGA,3);
lcd_line2();
lcd_testpin(TP2);
lcd_data('H');
lcd_equal();			// lcd_data('=');
ADC_PORT = (1<<TP2) | TXD_VAL;
R_PORT = 0;
adcmv[0] = W5msReadADC(TP2);
ADCSRB = (1<<MUX5);		// switch to upper 8 MUX inputs
adcmv[1] = ReadADC(PIN_RL2);
ADCSRB = 0;			// switch back to lower 8 MUX inputs
ResistorVal[1] = ((ADCconfig.U_AVCC - adcmv[0]) * (unsigned long)R_L_VAL) / (adcmv[0] - adcmv[1]);
DisplayValue(ResistorVal[1],-1,LCD_CHAR_OMEGA,3);
lcd_space();
lcd_data('L');
lcd_equal();			// lcd_data('=');
ResistorVal[0] = (adcmv[1] * (unsigned long)R_L_VAL) / (adcmv[0] - adcmv[1]);
DisplayValue(ResistorVal[0],-1,LCD_CHAR_OMEGA,3);
wait_about1s();
last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2
//
lcd_clear();
lcd_testpin(TP3);
lcd_data('L');
lcd_equal();			// lcd_data('=');
ADC_DDR = (1<<TP3) | TXD_MSK;
R_PORT = (1<<PIN_RL3);
R_DDR = (1<<PIN_RL3);
adcmv[0] = W5msReadADC(TP3);
ADCSRB = (1<<MUX5);		// switch to upper 8 MUX inputs
adcmv[1] = ReadADC(PIN_RL3);
ADCSRB = 0;
ResistorVal[0] = (adcmv[0] * (unsigned long)R_L_VAL) / (adcmv[1] - adcmv[0]);
DisplayValue(ResistorVal[0],-1,LCD_CHAR_OMEGA,3);
lcd_space();
lcd_data('H');
lcd_equal();			// lcd_data('=');
ResistorVal[1] = ((ADCconfig.U_AVCC - adcmv[1]) * (unsigned long)R_L_VAL) / (adcmv[1] - adcmv[0]);
DisplayValue(ResistorVal[1],-1,LCD_CHAR_OMEGA,3);
lcd_line2();
lcd_testpin(TP3);
lcd_data('H');
lcd_equal();			// lcd_data('=');
ADC_PORT = (1<<TP3) | TXD_VAL;
R_PORT = 0;
adcmv[0] = W5msReadADC(TP3);
ADCSRB = (1<<MUX5);		// switch to upper 8 MUX inputs
adcmv[1] = ReadADC(PIN_RL3);
ADCSRB = 0;			// switch back to lower 8 MUX inputs
ResistorVal[1] = ((ADCconfig.U_AVCC - adcmv[0]) * (unsigned long)R_L_VAL) / (adcmv[0] - adcmv[1]);
DisplayValue(ResistorVal[1],-1,LCD_CHAR_OMEGA,3);
lcd_space();
lcd_data('L');
lcd_equal();			// lcd_data('=');
ResistorVal[0] = (adcmv[1] * (unsigned long)R_L_VAL) / (adcmv[0] - adcmv[1]);
DisplayValue(ResistorVal[0],-1,LCD_CHAR_OMEGA,3);
wait_about1s();
last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2
  #endif	/* PROCESSOR_TYP == 1280 */
}	/* end if((test_mode & 0x0f) == 1) */
 #endif		/* end EXTENDED_TESTS */

for (ww=0;ww<120;ww++) {
  // wait up to 1 minute for releasing the probes
  if (AllProbesShorted() == 0) break;
  lcd_clear_line2();		// clear total line2
  lcd_MEM_string(RELPROBE);	// "Release Probes"
  lcd_refresh();		// write the pixels to display, ST7920 only
  wait_about500ms();
}



lcd_clear();
lcd_MEM_string(RIHI);	// "RiHi="
DisplayValue(RRpinPL,-1,LCD_CHAR_OMEGA,3);
lcd_line2();
lcd_MEM_string(RILO);	// "RiLo="
DisplayValue(RRpinMI,-1,LCD_CHAR_OMEGA,3);
last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2

//measure Zero offset for Capacity measurement
PartFound = PART_NONE;
lcd_clear();
lcd_MEM_string(C0_str);			//output "C0 "
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
 #ifdef WITH_MENU
if (((test_mode & 0x0f) == 1) || (UnCalibrated == 2))
 #else
if (UnCalibrated == 2)
 #endif
{
  adcmv[3] = adcmv[0] + 2;		// mark as uncalibrated until Cap > 100nF has success
} else {
  adcmv[3] = adcmv[0];			// mark as calibrated, short calibration is finished
  UnCalibrated = 0;			// clear the UnCalibrated Flag
  lcd_cursor_off();			// switch cursor off
}
DisplayValue(adcmv[5],0,' ',3);		//output cap0 1:3
DisplayValue(adcmv[6],0,' ',3);		//output cap0 2:3
DisplayValue(adcmv[2],-12,'F',3);		//output cap0 1:2
 #ifdef AUTO_CAL
for (ww=0;ww<7;ww++) {			//checking loop
if ((adcmv[ww] > 190) || (adcmv[ww] < 10)) goto no_c0save;
}
for (ww=0;ww<7;ww++) {
  // write all zero offsets to the EEprom 
  (void) eeprom_write_byte((uint8_t *)(&c_zero_tab[ww]),adcmv[ww]+(COMP_SLEW1 / (CC0 + CABLE_CAP + COMP_SLEW2)));
}
lcd_line2();
lcd_MEM_string(OK_str);		// output "OK"
no_c0save:
 #endif
last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2

 #ifdef AUTO_CAL
  #ifdef WITH_MENU
if (((test_mode & 0x0f) == 1) || (UnCalibrated == 2))
  #endif
// without menu function the capacitor is requested every time,
// because there is no way to request recaltbration again
// With menu function the capacitor is only requested for first time 
// calibration (UnCalibrated = 2) or for the full selftest call (test_mode = 1) 
// of the menu function, not with the automatically call (test_mode = 1).
{
// for full test or first time calibration, use external capacitor
// Message C > 100nF at TP1 and TP3
cap_found = 0;
for (ww=0;ww<64;ww++) {
  #if (TPCAP >= 0)
  PartFound = PART_NONE;
  CalibrationCap();	// measure with internal calibration capacitor
  #else
  lcd_clear();
  lcd_testpin(TP1);
  lcd_MEM_string(CapZeich);	// "-||-"
  lcd_testpin(TP3);
  lcd_MEM2_string(MinCap_str); // " >100nF!"
  PartFound = PART_NONE;
  //measure  offset Voltage of analog Comparator for Capacity measurement
  ReadCapacity(TP3, TP1);	// look for capacitor > 100nF
  #endif
  while (cap.cpre < -9) {
   cap.cpre++;
   cap.cval /= 10;
  }
  if ((cap.cpre == -9) && (cap.cval > 95) && (cap.cval < 22000) &&
    (load_diff > -150) && (load_diff < 150)) {
   cap_found++;
  } else {
   cap_found = 0;		// wait for stable connection
  }
  if (cap_found > 4) {
     // value of capacitor is correct
     (void) eeprom_write_word((uint16_t *)(&ref_offset), load_diff);	// hold zero offset + slew rate dependend offset
     lcd_clear();
     lcd_MEM2_string(REF_C_str);	// "REF_C="
     i2lcd(load_diff);		// lcd_string(itoa(load_diff, outval, 10));	//output REF_C_KORR
     RefVoltage();			// new ref_mv_offs and RHmultip
  #if 0
//#######################################
   // Test for switching level of the digital input of port TP3
   for (tt=0;tt<8;tt++) {
     ADC_PORT =  TXD_VAL;	//ADC-Port 1 to GND
     ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
     R_PORT = 1<<PIN_RH3;		//Pin 3 over R_H to VCC
     R_DDR = 1<<PIN_RH3;		//Pin 3 over R_H to VCC
     while (1) {
        wdt_reset();
        if ((ADC_PIN&(1<<TP3)) == (1<<TP3)) break;
     }
     R_DDR = 0;		//Pin 3 without current
     R_PORT = 0;
     adcmv[0] = ReadADC(TP3);
     lcd_line3();
     Display_mV(adcmv[0],4);
     R_DDR = 1<<PIN_RH3;		//Pin 3 over R_H to GND
     while (1) {
        wdt_reset();
        if ((ADC_PIN&(1<<TP3)) != (1<<TP3)) break;
     }
     R_DDR = 0;		//Pin 3 without current
     lcd_line4();
     adcmv[0] = ReadADC(TP3);
     Display_mV(adcmv[0],4);
     last_line_used = 2;
     wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2
   }
//#######################################
  #endif
  #ifdef AUTOSCALE_ADC
   #if (TPCAP >= 0)
    #define CAP_ADC TPCAP	/* Cap >100nF is integrated at TPCAP */
   TCAP_PORT &= ~(1<<TCAP_RH);	// 470k resistor to GND
   TCAP_DDR |= (1<<TCAP_RH);	// enable output
   #else
    #define CAP_ADC TP3		/* Cap >100nF at TP3 */
   ADC_PORT =  TXD_VAL;	//ADC-Port 1 to GND
   ADC_DDR = 1<<TP1 | TXD_MSK;	//ADC-Pin  1 to output 0V
   R_DDR = 1<<PIN_RH3;		//Pin 3 over R_H to GND
   #endif
   do {
      adcmv[0] = ReadADC(CAP_ADC);
   } while (adcmv[0] > 980);
   #if (TPCAP >= 0)
   TCAP_DDR &= ~(1<<TCAP_RH);	// 470k resistor port to input mode
   #else
   R_DDR = 0;		//all Pins to input 
   #endif
   ADCconfig.U_Bandgap = 0;	// do not use internal Ref
   adcmv[0] = ReadADC(CAP_ADC);  // get cap voltage with VCC reference
   ADCconfig.U_Bandgap = adc_internal_reference;
   adcmv[1] = ReadADC(CAP_ADC);	// get cap voltage with internal reference
   adcmv[1] += adcmv[1];		// double the value
   ADCconfig.U_Bandgap = 0;	// do not use internal Ref
   adcmv[2] = ReadADC(CAP_ADC);  // get cap voltage with VCC reference
   ADCconfig.U_Bandgap = adc_internal_reference;
   udiff = (int8_t)(((signed long)(adcmv[0] + adcmv[2] - adcmv[1])) * adc_internal_reference / adcmv[1])+REF_R_KORR;
   lcd_line2();
   lcd_MEM2_string(REF_R_str);	// "REF_R="
   udiff2 = udiff + (int8_t)eeprom_read_byte((uint8_t *)(&RefDiff));
   (void) eeprom_write_byte((uint8_t *)(&RefDiff), (uint8_t)udiff2);	// hold offset for true reference Voltage
   i2lcd(udiff2);		// output correction voltage
   RefVoltage();			// set new ADCconfig.U_Bandgap
  #endif	/* end AUTOSCALE_ADC */
   last_line_used = 2;
   wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2
   UnCalibrated = 0;		// clear the UnCalibrated Flag
   lcd_cursor_off();		// switch cursor off
   cap_found = eeprom_read_byte((uint8_t *)&c_zero_tab[0]);	// read first capacity zero offset
   eeprom_write_byte((uint8_t *)&c_zero_tab[3], cap_found);	// mark as calibrated permanent
   break;			// leave the ww for loop
  }  /* end if (cap_found > 4) */
  lcd_line2();
  DisplayValue(cap.cval,cap.cpre,'F',4);
  lcd_refresh();		// write the pixels to display, ST7920 only
  wait_about200ms();			// wait additional time
} // end for ww
}	/* end if((test_mode & 0x0f) == 1) */
 #endif  /* end AUTO_CAL */


ADCconfig.Samples = ANZ_MESS;	// set to configured number of ADC samples

 #ifdef FREQUENCY_50HZ
//#define TEST_SLEEP_MODE	/* only select for checking the sleep delay */
 lcd_clear();
 lcd_MEM_string(T50HZ);	//" 50Hz"
 lcd_refresh();		// write the pixels to display, ST7920 only
 ADC_PORT = TXD_VAL;
 ADC_DDR = 1<<TP1 | TXD_MSK;	// Pin 1 to GND
 R_DDR = (1<<PIN_RL3) | (1<<PIN_RL2);
 for(ww=0;ww<30;ww++) {	// repeat the signal up to 30 times (1 minute)
   for (ff50=0;ff50<100;ff50++) {	// for 2 s generate 50 Hz
      R_PORT = (1<<PIN_RL2);	// Pin 2 over R_L to VCC, Pin 3 over R_L to GND
  #ifdef TEST_SLEEP_MODE
      sleep_5ms(2); 		// test of timing of sleep mode call 
  #else
      wait10ms();		// normal delay
  #endif
      R_PORT = (1<<PIN_RL3);	// Pin 3 over R_L to VCC, Pin 2 over R_L to GND
  #ifdef TEST_SLEEP_MODE
      sleep_5ms(2); 		// test of timing of sleep mode call 
  #else
      wait10ms();		// normal delay
  #endif
      wdt_reset();
   } /* end for ff50 */
   if (!(RST_PIN_REG & (1<<RST_PIN))) {
      // if key is pressed, don't repeat
      break;
   }
 } /* end for ww */
 #endif		/* end FREQUENCY_50HZ */
lcd_clear();
lcd_MEM2_string(VERSION_str);	//"Version ..."
lcd_line2();
lcd_MEM_string(ATE);		//"Selftest End"
PartFound = PART_NONE;
     last_line_used = 2;
wait_for_key_5s_line2();		// wait up to 5 seconds and clear line 2
} /* end AutoCheck */ 

 
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
  const uint8_t *addr;
  uint8_t pp;
  /*
   *  Set up a voltage divider between the two probes:
   *  - Probe1: Rl pull-up
   *  - Probe2: Rl pull-down
   */

  ADC_DDR =  TXD_MSK;		// all-Pins to Input
  ADC_PORT = TXD_VAL;		// all ADC-Ports to GND
  addr = &PinRLRHADCtab[Probe1];
  pp = pgm_read_byte(addr);
  R_PORT = pp;
  addr += (int8_t)(Probe2-Probe1);
  R_DDR =  pp | pgm_read_byte(addr);	// pgm_read_byte(PinRHtab[Probe1]) | pgm_read_byte(PinRLtab[Probe2]);

  /* read voltages */
  U1 = ReadADC(Probe1);
  U2 = ReadADC(Probe2);

  /*
   *  We expect both probe voltages to be about the same and
   *  to be half of Vcc (allowed difference +/- 20mV).
   */
 #ifndef MAX_UH_DIFF
  #define MAX_UH_DIFF 30
 #endif

  URH = ADCconfig.U_AVCC / 2;
  URH -= ((long)U_VCC * (long)(PIN_RP-PIN_RM)) / (4*(unsigned long)(R_L_VAL+PIN_RM));			// differenz of Pin resistance high (22) and low (20)
  if (((U1 + MAX_UH_DIFF) > URH ) && (U1 < (URH + MAX_UH_DIFF)))
  {
    if (((U2 + MAX_UH_DIFF) > URH) && (U2 < (URH + MAX_UH_DIFF)))
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
#endif /* end #ifdef WITH_SELFTEST */

