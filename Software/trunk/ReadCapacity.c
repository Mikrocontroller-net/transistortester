// new code by K.-H. Kübbeler
// ReadCapacity tries to find the value of a capacitor by measuring the load time.
// first of all the capacitor is discharged.
// Then a series of up to 500 load pulses with 10ms duration each is done across the R_L (680Ohm)
// resistor.
// After each load pulse the voltage of the capacitor is measured without any load current.
// If voltage reaches a value of more than 300mV and is below 1.3V, the capacity can be
// computed from load time and voltage by a interpolating a build in table.
// If the voltage reaches a value of more than 1.3V with only one load pulse,
// another measurement methode is used:
// The build in 16bit counter can save the counter value at external events.
// One of these events can be the output change of a build in comparator.
// The comparator can compare the voltage of any of the ADC input pins with the voltage
// of the internal reference (1.3V or 1.1V).
// After setting up the comparator and counter properly, the load of capacitor is started 
// with connecting the positive pin with the R_H resistor (470kOhm) to VCC and immediately
// the counter is started. By counting the overflow Events of the 16bit counter and watching
// the counter event flag  the total load time of the capacitor until reaching the internal
// reference voltage can be measured.
// If any of the tries to measure the load time is successful,
// the following variables are set:
// cap.cval = value of the capacitor 
// cap.cval_uncorrected = value of the capacitor uncorrected
// cap.esr = serial resistance of capacitor,  0.01 Ohm units
// cap.cpre = units of cap.cval (-12==pF, -9=nF, -6=µF)
// ca   = Pin number (0-2) of the LowPin
// cb   = Pin number (0-2) of the HighPin

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
void ReadCapacity(uint8_t HighPin, uint8_t LowPin) {
  // check if capacitor and measure the capacity value
  unsigned int tmpint;
//  unsigned int adcv[4];
  int residual_voltage;
  int cap_voltage1;
  int cap_voltage2;
#ifdef INHIBIT_SLEEP_MODE
  unsigned int ovcnt16;
#endif
  uint8_t HiPinR_L, HiPinR_H;
  uint8_t LoPinR_L;
  uint8_t LoADC;
  uint8_t ii;

#if PROCESSOR_TYP == 666644  /* ########## not yet finshised ################ */
  if (HighPin == TestCapPin) {
     // special handling for build in calibration capacitor
     ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
     ADC_DDR = (1<<TestCapPin) | TXD_MSK;	// switch capacitor-Pin to output (GND)
     wait_about20ms();
     ADC_DDR = TXD_MSK;			// switch all ADC to input 
     residual_voltage = ReadADC(HighPin);		// voltage before any load 
  }
#endif

#ifdef AUTO_CAL
  pin_combination = ((HighPin - TP_MIN) * 3) + LowPin - TP_MIN - 1;	// coded Pin combination for capacity zero offset
#endif

  HiPinR_L = pgm_read_byte(&PinRLRHADCtab[HighPin - TP_MIN]);	//R_L mask for HighPin R_L load
  LoPinR_L = pgm_read_byte(&PinRLRHADCtab[LowPin - TP_MIN]);	//R_L mask for LowPin R_L load
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
  HiPinR_H = pgm_read_byte((&PinRLRHADCtab[3])+HighPin-TP_MIN);	//R_H mask for HighPin R_H load
  LoADC = pgm_read_byte((&PinRLRHADCtab[6])+LowPin-TP_MIN) | TXD_MSK;
#else
  HiPinR_H = HiPinR_L + HiPinR_L;	//double for HighPin R_H load
  LoADC = pgm_read_byte((&PinRLRHADCtab[3])+LowPin-TP_MIN) | TXD_MSK;
#endif

#if DebugOut == 10
  lcd_line3();
  lcd_clear_line();
  lcd_line3();
  lcd_testpin(LowPin);
  lcd_data('C');
  lcd_testpin(HighPin);
  lcd_space();
#endif
  if(PartFound == PART_RESISTOR) {
#if DebugOut == 10
     lcd_data('R');
     wait_about2s();	/* debug delay */
#endif
     return;	//We have found a resistor already 
  }
  for (ii=0;ii<NumOfDiodes;ii++) {
     if ((diodes.Cathode[ii] == LowPin) && (diodes.Anode[ii] == HighPin) && (diodes.Voltage[ii] < 1500)) {
#if DebugOut == 10
        lcd_data('D');	// debug
        wait_about2s();	/* debug delay */
#endif
        return;
     }
  }
  
#if FLASHEND > 0x1fff
  unsigned int vloss;	// lost voltage after load pulse in 0.1% 
  cap.esr = 0;				// set ESR of capacitor to zero
  vloss = 0;				// set lost voltage to zero
#endif
  cap.cpre = -15;			// mark for no cap
  cap.cval = 0;				// set capacity value to zero
  EntladePins();			// discharge capacitor
  ADC_PORT = TXD_VAL;			// switch ADC-Port to GND
// The polarity of residual voltage of the capacitor depends on the measurement
// history. For the normal test cycle the residual voltage is negative
// at the HighPin. Because the ADC can not measure a negative Voltage,
// the LowPin voltage is shifted to 139mV with the R_L resistor.
// The voltage of the capacitor is build as difference between HighPin and LowPin voltage.
  ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
  R_DDR = LoPinR_L;			// switch R_L Port of LoPin to VCC
  R_PORT = LoPinR_L;			// switch R_L Port of LoPin to VCC
  residual_voltage = ReadADC(HighPin) - ReadADC(LowPin); // voltage at HighPin before any load 
  R_DDR = 0;				// switch all R_L ports to input
  cap_voltage1 = 0;			// preset to prevent compiler warning
#define MAX_LOAD_TIME 500
#define MIN_VOLTAGE 300
  for (ovcnt16=0;ovcnt16<MAX_LOAD_TIME;ovcnt16++) {
     R_PORT = HiPinR_L;			//R_L to 1 (VCC) 
     R_DDR = HiPinR_L;			//switch Pin to output, across R to GND or VCC
     wait10ms();			// wait exactly 10ms, do not sleep
     R_DDR = 0;				// switch back to input
     R_PORT = 0;			// no Pull up
     wait500us();			//wait a little time
     wdt_reset();
     // read voltage without current, is already charged enough?
#ifdef big_cap_no_float
     R_PORT = HiPinR_H;			//R_H to 1 (VCC), to ensure that if nothing is connected, measured voltage will be Vcc, rather than float anywhere
     R_DDR = HiPinR_H;			// shouldn't influence the measurements significantly, since the R is almost 1000 times larger, and the time about 100 times shorter
     cap_voltage1 = ReadADC(HighPin);
     R_DDR = 0;				// switch back to input
     R_PORT = 0;			// no Pull up
     if (ovcnt16==0 && cap_voltage1 >=4096) {
        // apparently very small capacitance, if it charges so quickly
        goto messe_mit_rh;
     }
     cap_voltage1 -= residual_voltage; // voltage across capacitor
#else
     cap_voltage1 = ReadADC(HighPin) - residual_voltage; // voltage of capacitor
#endif
     if ((ovcnt16 > (MAX_LOAD_TIME/4)) && (cap_voltage1 < (MIN_VOLTAGE/4))) {
        // 300mV can not be reached well-timed 
        break;		// don't try to load any more
     }
     // probably 100mF can be charged well-timed 
     if (cap_voltage1 > MIN_VOLTAGE) {
        break;		// lowest voltage to get capacity from load time is reached
     }
  }  /* end for ovcnt16 */
  // wait 5ms and read voltage again, does the capacitor keep the voltage?
#if DebugOut == 10
  DisplayValue16(ovcnt16,0,' ',4);
  Display_mV(cap_voltage1,4);
#endif
  if (cap_voltage1 <= MIN_VOLTAGE) {
#if DebugOut == 10
     lcd_data('K');
     lcd_space();
     wait1s();		// debug delay
#endif
//     if (NumOfDiodes != 0) goto messe_mit_rh; /* ****************************** */
     goto keinC;		// was never charged enough, >100mF or shorted
  }
  //voltage is rised properly and keeps the voltage enough
  if ((ovcnt16 == 0 ) && (cap_voltage1 > 1300)) {
     goto messe_mit_rh;		// Voltage of more than 1300mV is reached in one pulse, too fast loaded
  }
  // Capacity is more than about 50µF
#ifdef NO_CAP_HOLD_TIME
  ChargePin10ms(HiPinR_H,0);		//switch HighPin with R_H 10ms auf GND, then currentless
  cap_voltage2 = ReadADC(HighPin) - residual_voltage;	// read voltage again, is discharged only a little bit ?
 #if DebugOut == 10
  lcd_data('U');
  lcd_data('3');
  lcd_data(':');
  u2lcd_space(cap_voltage2);
  wait_about2s();	/* debug delay */
 #endif
  if ((cap_voltage2 + cap_voltage2) < cap_voltage1) {
 #if DebugOut == 10
     lcd_data('H');
     lcd_space();
     wait_about1s();	/* debug delay */
 #endif
//     if (ovcnt16 == 0 )  {
//        goto messe_mit_rh;		// Voltage of more than 300mV is reached in one pulse, but not hold
//     }
     goto keinC; //implausible, not yet the half voltage
  }
  cap.cval_uncorrected.dw = ovcnt16 + 1;
  cap.cval_uncorrected.dw *= GetRLmultip(cap_voltage1);		// get factor to convert time to capacity from table
#else
  // wait the half the time which was required for loading
  cap_voltage2 = cap_voltage1;			// preset to prevent compiler warning
  for (tmpint=0;tmpint<=ovcnt16;tmpint++) {
     wait5ms();
     cap_voltage2 = ReadADC(HighPin) - residual_voltage;	// read voltage again, is discharged only a little bit ?
     wdt_reset();
  }
     cap_voltage2 = cap_voltage1 - cap_voltage2; 	// lost voltage during load time wait
#if FLASHEND > 0x1fff
  // compute equivalent parallel resistance from voltage drop
  if (cap_voltage2 > 0) {
     // there is any voltage drop !
     // cap_voltage1 is the loaded voltage.
     vloss = (unsigned long)(cap_voltage2 * 1000UL) / cap_voltage1;
  }
#endif
  if (cap_voltage2 > 200) {
     // more than 200mV is lost during load time
 #if DebugOut == 10
     lcd_data('L');
     lcd_space();
     wait_about1s();	/* debug delay */
 #endif
//     if (ovcnt16 == 0 )  {
//        goto messe_mit_rh;		// Voltage of more than 300mV is reached in one pulse, but not hold
//     }
     goto keinC;			// capacitor does not keep the voltage about 5ms
  }
  cap.cval_uncorrected.dw = ovcnt16 + 1;
  // compute factor with load voltage + lost voltage during the voltage load time
  cap.cval_uncorrected.dw *= GetRLmultip(cap_voltage1+cap_voltage2);	// get factor to convert time to capacity from table
#endif
   cap.cval = cap.cval_uncorrected.dw;	// set result to uncorrected
   cap.cpre = -9;		// switch units to nF 
   Scale_C_with_vcc();		// value is below 100000 after this call, cpre is changed
   // cap.cval for this type is at least 40000nF, so the last digit will be never shown
#if WITH_MENU
   cap.cval *= (1000 - (int8_t)eeprom_read_byte((uint8_t *)&big_cap_corr));	// correct with C_H_KORR with 0.1% resolution, but prevent overflow
#else
   cap.cval *= (1000 - C_H_KORR);	// correct with C_H_KORR with 0.1% resolution, but prevent overflow
#endif
   cap.cval /= 100;		// was multiplied with 1000, now divided by 100
#if DebugOut == 10
   lcd_line3();
   lcd_clear_line();
   lcd_line3();
   lcd_testpin(LowPin);
   lcd_data('C');
   lcd_testpin(HighPin);
   lcd_space();
   DisplayValue(cap.cval,cap.cpre,'F',4);
   lcd_space();
   u2lcd(ovcnt16);
   wait_about3s();	/* debug delay */
#endif
   goto checkDiodes;

//==================================================================================
// Measurement of little capacity values
messe_mit_rh:
  //little capacity value, about  < 50 µF
  EntladePins();			// discharge capacitor
  //measure with the R_H (470kOhm) resistor 
  R_PORT = 0;		// R_DDR ist HiPinR_L
  ADC_DDR = (1<<TP1) | (1<<TP2) | (1<<TP3) | TXD_MSK;	//switch all Pins to output
  ADC_PORT = TXD_VAL;		//switch all ADC Pins to GND
  R_DDR = HiPinR_H;   		// switch R_H resistor port for HighPin to output (GND)
// setup Analog Comparator
  ADC_COMP_CONTROL = (1<<ACME);			//enable Analog Comparator Multiplexer
  ACSR =  (1<<ACBG) | (1<<ACI)  | (1<<ACIC);	// enable, 1.3V, no Interrupt, Connect to Timer1 
  ADMUX = (1<<REFS0) | HighPin;			// switch Mux to High-Pin
  ADCSRA = (1<<ADIF) | AUTO_CLOCK_DIV; //disable ADC
  wait200us();			//wait for bandgap to start up

// setup Counter1
  ovcnt16 = 0;
  TCCR1A = 0;			// set Counter1 to normal Mode
  TCNT1 = 0;			//set Counter to 0
  TI1_INT_FLAGS = (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);	// clear interrupt flags
#ifndef INHIBIT_SLEEP_MODE
  TIMSK1 = (1<<TOIE1) | (1<<ICIE1);	// enable Timer overflow interrupt and input capture interrupt
  unfinished = 1;
#endif
  R_PORT = HiPinR_H;           	// switch R_H resistor port for HighPin to VCC
  if(PartFound == PART_FET) {
     // charge capacitor with R_H resistor
     TCCR1B = (0<<ICNC1) | (1<<CS10);	//Start counter 1MHz or 8MHz without Noise Canceler
     ADC_DDR = (((1<<TP1) | (1<<TP2) | (1<<TP3) | TXD_MSK) & ~(1<<HighPin));	// release only HighPin ADC port
  } else {
     TCCR1B =  (0<<ICNC1) | (1<<CS10);	//start counter 1MHz or 8MHz without Noise Canceler
     ADC_DDR = LoADC;		// stay LoADC Pin switched to GND, charge capacitor with R_H slowly
  }
//******************************
#ifdef INHIBIT_SLEEP_MODE
  while(1) {
     // Wait, until  Input Capture is set
     ii = TI1_INT_FLAGS;	//read Timer flags
     if (ii & (1<<ICF1))  {
        break;
     }
     if((ii & (1<<TOV1))) {	// counter overflow, 65.536 ms @ 1MHz, 8.192ms @ 8MHz
        TI1_INT_FLAGS = (1<<TOV1);	// Reset OV Flag
        wdt_reset();
        ovcnt16++;
        if(ovcnt16 == (F_CPU/5000)) {
           break; 		//Timeout for Charging, above 12 s
        }
     }
  }
  TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<CS10);  // stop counter
  TI1_INT_FLAGS = (1<<ICF1);		// Reset Input Capture
  tmpint = ICR1;		// get previous Input Capture Counter flag
// check actual counter, if an additional overflow must be added
  if((TCNT1 > tmpint) && (ii & (1<<TOV1))) {
     // this OV was not counted, but was before the Input Capture
     TI1_INT_FLAGS = (1<<TOV1);		// Reset OV Flag
     ovcnt16++;
  }
#else
  cli();		// disable interrupts to prevent wakeup Interrupts before sleeping
  set_sleep_mode(SLEEP_MODE_IDLE);
  while(unfinished) {
    sleep_enable();
    sei();		// enable interrupts after next instruction
    sleep_cpu();	// only enable interrupts during sleeping
    sleep_disable();
    cli();		// disable interrupts again
    wdt_reset();	// reset watch dog during waiting
    if(ovcnt16 == (F_CPU/5000)) {
       break; 		//Timeout for Charging, above 12 s
    }
  }
  sei();		// enable interrupts again
  TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<CS10);  // stop counter
  tmpint = ICR1;		// get previous Input Capture Counter flag
  TIMSK1 = (0<<TOIE1) | (0<<ICIE1);	// disable Timer overflow interrupt and input capture interrupt
#endif

#if DebugOut == 11
  // test output for checking error free load time measurement
  // select a capacitor, which gives a load time of about 65536 clock tics
  if ((LowPin == TP1) && (HighPin == TP3)) {
    lcd_line3();
    lcd_clear_line();
    lcd_line3();
    if (ovcnt16 != 0) {
      DisplayValue16(ovcnt16,0,',',4);
    }
    u2lcd(tmpint);
  }
#endif

//############################################################
  ADCSRA = (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; //enable ADC
  R_DDR = 0;			// switch R_H resistor port for input
  R_PORT = 0;			// switch R_H resistor port pull up for HighPin off
  load_diff = ReadADC(HighPin) + REF_C_KORR - ref_mv;	// build difference of capacitor voltage to Reference Voltage
//############################################################
  if (ovcnt16 >= (F_CPU/10000)) {
#if DebugOut == 10
     lcd_data('k');
     wait_about1s();	/* debug delay */
#endif
     goto keinC;	// no normal end
  }
//  cap.cval_uncorrected = CombineII2Long(ovcnt16, tmpint);
  cap.cval_uncorrected.w[1] = ovcnt16;
  cap.cval_uncorrected.w[0] = tmpint;

  cap.cpre = -12;			// cap.cval unit is pF 
  if (ovcnt16 > 65) {
     cap.cval_uncorrected.dw /= 100;	// switch to next unit
     cap.cpre += 2;			// set unit, prevent overflow
  }
  cap.cval_uncorrected.dw *= RHmultip;		// 708
  cap.cval_uncorrected.dw /= (F_CPU / 10000);	// divide by 100 (@ 1MHz clock), 800 (@ 8MHz clock)
  cap.cval = cap.cval_uncorrected.dw;		// set the corrected cap.cval
  Scale_C_with_vcc();
  if (cap.cpre == -12) {
#if COMP_SLEW1 > COMP_SLEW2
     if (cap.cval < COMP_SLEW1) {
        // add slew rate dependent offset
        cap.cval += (COMP_SLEW1 / (cap.cval+COMP_SLEW2 ));
     }
#endif
#ifdef SamplingADC
     // store as reference for inductance measurement
     // note that we store this before subtracting cap_null, since for inductance measurement that cap. also contributes
     if ( cap.cval < 65536) lc_cpartmp=cap.cval; /* prevent wrong value */
     else                   lc_cpartmp = 1;	/* set to 1pF, if too big */
#endif /* SamplingADC */
#ifdef AUTO_CAL
     // auto calibration mode, cap_null can be updated in selftest section
     tmpint = eeprom_read_byte(&c_zero_tab[pin_combination]);	// read zero offset
     if (cap.cval > tmpint) {
         cap.cval -= tmpint;		//subtract zero offset (pF)
     } else {
 #if FLASHEND > 0x3fff
       if ((cap.cval+C_LIMIT_TO_UNCALIBRATED) < tmpint) {
         mark_as_uncalibrated();	// set in EEprom to uncalibrated
  #if DebugOut == 11
         lcd_line3();
         lcd_testpin(LowPin);
         lcd_data('y');
         lcd_testpin(HighPin);
         lcd_space();
  #endif
       }
 #endif
         cap.cval = 0;			//unsigned long may not reach negativ value
     }
#else  /* no AUTO_CAL */
     if (HighPin == TP2) cap.cval += TP2_CAP_OFFSET;	// measurements with TP2 have 2pF less capacity
     if (cap.cval > C_NULL) {
         cap.cval -= C_NULL;		//subtract constant offset (pF)
     } else {
         cap.cval = 0;			//unsigned long may not reach negativ value
     }
#endif
  } /* end if (cap.cpre == -12) */

#if DebugOut == 10
  R_DDR = 0;			// switch all resistor ports to input
  lcd_line4();
  lcd_clear_line();
  lcd_line4();
  lcd_testpin(LowPin);
  lcd_data('c');
  lcd_testpin(HighPin);
  lcd_space();
  DisplayValue(cap.cval,cap.cpre,'F',4);
  wait_about3s();	/* debug delay */
#endif
  R_DDR = HiPinR_L; 		//switch R_L for High-Pin to GND
#if F_CPU < 2000001
   if(cap.cval < 50)
#else 
   if(cap.cval < 25)
#endif 
     {
     // cap.cval can only be so little in pF unit, cap.cpre must not be testet!
#if DebugOut == 10
     lcd_data('<');
     lcd_space();
     wait_about1s();	/* debug delay */
#endif
      goto keinC;	//capacity to low, < 50pF @1MHz (25pF @8MHz)
   }
   // end low capacity 
checkDiodes:
   if((NumOfDiodes > 0)  && (PartFound != PART_FET)) {
#if DebugOut == 10
      lcd_data('D');	// debug
      lcd_space();
      wait_about1s();	/* debug delay */
#endif
      // nearly sure, that there is one or more diodes in reverse direction,
      // which would be wrongly detected as capacitor 
   } else {
      PartFound = PART_CAPACITOR;	//capacitor is found
      if ((cap.cpre > cap.cpre_max) || ((cap.cpre == cap.cpre_max) && (cap.cval > cap.cval_max))) {
         // we have found a greater one, overwrite the old values
         cap.cval_max = cap.cval;
         cap.cpre_max = cap.cpre;
#if FLASHEND > 0x1fff
         cap.v_loss = vloss;		// lost voltage in 0.01%
#endif
         cap.ca = LowPin;		// save LowPin
         cap.cb = HighPin;		// save HighPin
      }
   }

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
 } // end ReadCapacity()


void Scale_C_with_vcc(void) {
   while (cap.cval > 100000) {
      cap.cval /= 10;
      cap.cpre ++;			// prevent overflow
   }
   cap.cval *= ADCconfig.U_AVCC;	// scale with measured voltage
   cap.cval /= U_VCC;			// Factors are computed for U_VCC
}
#ifndef INHIBIT_SLEEP_MODE
/* Interrupt Service Routine for timer1 Overflow */
 ISR(TIMER1_OVF_vect, ISR_BLOCK)
{
 if ((!(TI1_INT_FLAGS & (1<<ICF1)) && (unfinished !=0)) || ((TI1_INT_FLAGS & (1<<ICF1)) && (ICR1 < 250))) {
    // Capture Event not pending or (Capture Event pending and ICR1 < 250
    // the ICR1H is buffered and can not examined alone, we must read the ICR1L first (with ICR1 access) !
   ovcnt16++;				// count Overflow
 }
}
/* Interrupt Service Routine for timer1 capture event (Comparator) */
 ISR(TIMER1_CAPT_vect, ISR_BLOCK)
{
 unfinished = 0;			// clear unfinished flag
 // With unfinished set to 0, we prevent further counting the Overflow.s
 // Is already a Overflow detected?
 if((TI1_INT_FLAGS & (1<<TOV1))) {	// counter overflow, 65.536 ms @ 1MHz, 8.192ms @ 8MHz
   //there is already a Overflow detected, before or after the capture event?
   // ICR1H is buffered and can not examined alone, we must read the ICR1L first (with ICR1 access) !
   if (ICR1 < 250) {
     //  Yes, Input Capture Counter is low, Overflow has occured before the capture event
     ovcnt16++;				// count Overflow
   }
 }
}
#endif
