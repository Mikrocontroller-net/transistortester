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
// cval = value of the capacitor 
// cpre = units of cval (0==pF, 1=nF, 2=µF)
// ca   = Pin number (0-2) of the LowPin
// cb   = Pin number (0-2) of the HighPin

unsigned long CombineII2Long( unsigned int ovcnt16, unsigned int tmpcnt); //tricky function to build unsigned long from two unsigned int values


//=================================================================
void ReadCapacity(uint8_t HighPin, uint8_t LowPin) {
  // check if capacitor and measure the capacity value
  unsigned int tmpint;
  unsigned int adcv[4];
  unsigned int ovcnt16;
  uint8_t HiPinR_L, HiPinR_H;
  uint8_t LoADC;
  uint8_t ii;

  LoADC = MEM_read_byte(&PinADCtab[LowPin]) | TXD_MSK;
  HiPinR_L = MEM_read_byte(&PinRLtab[HighPin]);	//R_L mask for HighPin R_L load
  HiPinR_H = HiPinR_L + HiPinR_L;	//double for HighPin R_H load

  if(PartFound == PART_CAPACITOR) {
#if DebugOut == 10
     Line3();
     lcd_clear_line();
     Line3();
     lcd_ziff1(LowPin);
     lcd_data('C');
     lcd_ziff1(HighPin);
     lcd_data(' ');
     lcd_data('d');
     lcd_data('o');
     lcd_data('p');
     lcd_data(' ');
     lcd_string(ultoa(cval,outval,10));
     wait2s();
#endif
     return;	//We have found a capacitor already
  }
  if(PartFound == PART_RESISTOR) {
     return;	//We have found a resistor already 
  }
  for (ii=0;ii<NumOfDiodes;ii++) {
     if ((diodes[ii].Cathode == LowPin) && (diodes[ii].Anode == HighPin) && (diodes[ii].Voltage < 1500)) {
        return;
     }
  }
  
  EntladePins();			// discharge capacitor
  ADC_PORT = TXD_VAL;			// switch ADC-Port to GND
  R_PORT = 0;				// switch R-Port to GND
  ADC_DDR = LoADC;			// switch Low-Pin to output (GND)
  R_DDR = HiPinR_L;			// switch R_L port for HighPin to output (GND)
  adcv[0] = ReadADC(HighPin);		// voltage before any load 
// ******** should adcv[0] be measured without current???
  for (ovcnt16=0;ovcnt16<500;ovcnt16++) {
     ChargePin10ms(HiPinR_L,1);		//HighPin with R_L 10ms to VCC ,then currentless
     wait500us();			//wait a little time
     wdt_reset();
     // read voltage without current, is already charged enough?
//     adcv[2] = ReadADC(HighPin) - adcv[0] + C_H_KORR;
     adcv[2] = ReadADC(HighPin) - adcv[0];
     if ((ovcnt16 == 126) && (adcv[2] < 75)) {
        // 300mV can not be reached well-timed 
        break;		// don't try to load any more
     }
     if (adcv[2] > 300) {
        break;		// probably 100mF can be charged well-timed 
     }
  }
  // wait 5ms and read voltage again, does the capacitor keep the voltage?
  adcv[1] = W5msReadADC(HighPin) - adcv[0];
  wdt_reset();


#if DebugOut == 10
  Line3();
  lcd_clear_line();
  Line3();
  lcd_ziff1(LowPin);
  lcd_data('C');
  lcd_ziff1(HighPin);
  lcd_data(' ');
  lcd_data('U');
  lcd_data('0');
  lcd_data(':');
  lcd_string(utoa(adcv[0],outval,10));
  lcd_data(' ');
  lcd_data('U');
  lcd_data('2');
  lcd_data(':');
  lcd_string(utoa(adcv[2],outval,10));
  Line4();
  lcd_data('U');
  lcd_data('1');
  lcd_data(':');
  lcd_string(utoa(adcv[1],outval,10));
  lcd_data('*');
  lcd_string(utoa(ovcnt16,outval,10)); 
  lcd_data(' ');
  wait3s();
#endif
  cpre = 0;			//default unit is pF
  if (adcv[2] < 301) {
#if DebugOut == 10
     lcd_data('K');
     lcd_data(' ');
     wait1s();
#endif
     goto keinC;		// was never charged enough, >100mF or shorted
  }
  if (adcv[1] < 151) {
#if DebugOut == 10
     lcd_data('L');
     lcd_data(' ');
     wait1s();
#endif
     goto keinC;		// capacitor does not keep the voltage about 5ms
  }
  //voltage is rised properly and keeps the voltage enough
  if ((ovcnt16 == 0 ) && (adcv[2] > 1300)) {
     goto messe_mit_rh;		// Voltage of more than 1300mV is reached in one pulse, to fast loaded
  }
  // Capacity is more than about 50µF
  ChargePin10ms(HiPinR_H,0);		//switch HighPin with R_H 10ms auf GND, then currentless
  adcv[3] = ReadADC(HighPin) - adcv[0]; // read voltage again, is discharged only a little bit ?
#if DebugOut == 10
  lcd_data('U');
  lcd_data('3');
  lcd_data(':');
  lcd_string(utoa(adcv[3],outval,10));
  lcd_data(' ');
  wait2s();
#endif
  if ((adcv[3] + adcv[3]) < adcv[2]) {
#if DebugOut == 10
     lcd_data('H');
     lcd_data(' ');
     wait1s();
#endif
     goto keinC; //implausible, not yet the half voltage
  }
   cval = ovcnt16 + 1;

   cpre = 1;		// switch units to nF 
   cval *= getRLmultip(adcv[2]);	// get factor to convert time to capacity from table
   cval *= (400 - (C_H_KORR));
   cval /= 40;
#if DebugOut == 10
   Line3();
   lcd_clear_line();
   Line3();
   lcd_ziff1(LowPin);
   lcd_data('C');
   lcd_ziff1(HighPin);
   lcd_data(' ');
   lcd_string(ultoa(cval,outval,10));
   lcd_data('n');
   lcd_string(utoa(ovcnt16,outval,10));
   wait3s();
#endif
   goto checkDiodes;

//==================================================================================
// Measurement of little capacity values
messe_mit_rh:
  //little capacity value, about  < 50 µF
  EntladePins();			// discharge capacitor
  //measure with the R_H (470kOhm) resistor 
  R_PORT = 0;		// R_DDR ist HiPinR_L
  ADC_DDR = (1<<TP1) | (1<<TP2) | (1<<TP3) | (1<<TxD);	//switch all Pins to output
  ADC_PORT = TXD_VAL;		//switch all ADC Pins to GND
  R_DDR = HiPinR_H;   		// switch R_H resistor port for HighPin to output (GND)
// setup Analog Comparator
  ADC_COMP_CONTROL = (1<<ACME);			//enable Analog Comparator Multiplexer
  ACSR =  (1<<ACBG) | (1<<ACI)  | (1<<ACIC);	// enable, 1.3V, no Interrupt, Connect to Timer1 
  ADMUX = (1<<REFS0) | HighPin;			// switch Mux to High-Pin
  ADCSRA = (1<<ADIF) | AUTO_CLOCK_DIV; //disable ADC
  wait200us();

  ovcnt16 = 0;
// setup Counter1
  TCCR1A = 0;			// set Counter1 to normal Mode
  TCNT1 = 0;			//set Counter to 0
  TI1_INT_FLAGS = (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);	// mega88
  R_PORT = HiPinR_H;           	// switch R_H resistor port for HighPin to VCC
  if(PartFound == PART_FET) {
     // charge capacitor with R_H resistor
     TCCR1B = (1<<CS10);	//Start counter 1MHz or 8MHz
     ADC_DDR = (((1<<TP1) | (1<<TP2) | (1<<TP3) | TXD_MSK) & ~(1<<HighPin));	// release only HighPin ADC port
  } else {
     TCCR1B =  (1<<CS10);	//start counter 1MHz or 8MHz
     ADC_DDR = LoADC;		// stay LoADC Pin switched to GND, charge capacitor with R_H slowly
  }
//******************************
  while(1) {
     // Wait, until  Input Capture is set
     ii = TI1_INT_FLAGS;		//read Timer flags
     if (ii & (1<<ICF1))  {
        break;
     }
     if((ii & (1<<TOV1))) {		// counter overflow, 65.536 ms @ 1MHz, 8.192ms @ 8MHz
        TI1_INT_FLAGS = (1<<TOV1);	// Reset OV Flag
        wdt_reset();
        ovcnt16++;
        if(ovcnt16 == (F_CPU/5000)) {
           break; 	//Timeout for Charging, above 12 s
        }
     }
  }
  TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<CS10);  // stop counter
  TI1_INT_FLAGS = (1<<ICF1);			// Reset Input Capture
  tmpint = ICR1;		// get previous Input Capture Counter flag
// check actual counter, if an additional overflow must be added
  if((TCNT1 > tmpint) && (ii & (1<<TOV1))) {
     // this OV was not counted, but was before the Input Capture
     TI1_INT_FLAGS = (1<<TOV1);		// Reset OV Flag
     ovcnt16++;
  }
  R_PORT = 0;			//discharge the capacitor
  if (ovcnt16 >= (F_CPU/10000)) {
     goto keinC;	// no normal end
  }
  cval = CombineII2Long(ovcnt16, tmpint);

  cpre = 0;			// cval unit is pF 
  if (ovcnt16 > 65) {
     // cval > 4259840
     cval /= 1000;		// switch from pF to nF unit
     cpre = 1;			// set unit, prevent overflow
  }
  cval *= RHmultip;		// 708
  cval /= (F_CPU / 10000);	// divide by 100 (@ 1MHz clock), 800 (@ 8MHz clock)
#if 0
  Line4();
//  lcd_clear_line();
//  Line4();
  lcd_ziff1(LowPin);
  lcd_data('c');
  lcd_ziff1(HighPin);
  lcd_data(' ');
  lcd_string(ultoa(cval,outval,10));
  lcd_data(' ');
  wait3s();
#endif
  if (cpre == 0) {
#if COMP_SLEW1 > COMP_SLEW2
     if (cval < COMP_SLEW1) {
        // add slew rate dependent offset
        cval += (COMP_SLEW1 / (cval+COMP_SLEW2 ));
     }
#endif
     if (HighPin == TP2) cval += 2;	// measurements with TP2 have 2pF less capacity
//     if ((HighPin == TP3) && (LowPin == TP2)) cval -= 1; // this combination has 1pF to much
//     if ((HighPin == TP1) && (LowPin == TP3)) cval += 1; // this combinations has 1pF to less
     if (cval > C_NULL) {
         cval -= C_NULL;		//subtract constant offset (pF)
     } else {
         cval = 0;			//unsigned long may not reach negativ value
     }
  }

#if DebugOut == 10
  R_DDR = 0;			// switch all resistor ports to input
  Line4();
  lcd_clear_line();
  Line4();
  lcd_ziff1(LowPin);
  lcd_data('c');
  lcd_ziff1(HighPin);
  lcd_data(' ');
  lcd_string(ultoa(cval,outval,10));
  lcd_data('p');
  wait3s();
#endif
  R_DDR = HiPinR_L; 		//switch R_L for High-Pin to GND
#if F_CPU < 2000001
   if(cval < 50)
#else 
   if(cval < 25)
#endif 
     {
     // cval can only be so little in pF unit, cpre must not be testet!
#if DebugOut == 10
     lcd_data('<');
     lcd_data(' ');
     wait1s();
#endif
      goto keinC;	//capacity to low, < 70pF @1MHz (35pF @8MHz)
   }
   // end low capacity 
checkDiodes:
   if((NumOfDiodes > 0)  && (PartFound != PART_FET)) {
#if DebugOut == 10
      lcd_data('D');
      lcd_data(' ');
      wait1s();
#endif
      //höchstwahrscheinlich eine (oder mehrere) Diode(n) in Sperrrichtung,
      //die sonst fälschlicherweise als Kondensator erkannt wird
   } else {

   PartFound = PART_CAPACITOR;	//capacitor is found
   ca = LowPin;			// save LowPin
   cb = HighPin;		// save HighPin
   }

keinC:
  // discharge capacitor again
  ADCSRA = (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; //enable ADC
  EntladePins();		// discharge capacitors
  //ready
  // switch all ports to input
  ADC_DDR =  TXD_MSK;		// switch all ADC ports to input
  ADC_PORT = TXD_VAL;		// switch all ADC outputs to GNG, no pull up
  R_DDR = 0;			// switch all resistor ports to input
  R_PORT = 0; 			// switch all resistor outputs to GND, no pull up
  return;
 } // end ReadCapacity()
