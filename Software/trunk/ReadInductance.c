// new code by K.-H. Kübbeler
// The 680 Ohm resistor (R_L_VAL) at the Lowpin will be used as current sensor
// The current with a coil will with (1 - e**(-t*R/L)), where R is
// the sum of Pin_RM , R_L_VAL , Resistance of coil and Pin_RP.
// L in the inductance of the coil.

unsigned long CombineII2Long( unsigned int ovcnt16, unsigned int tmpcnt); //tricky function to build unsigned long from two unsigned int values


//=================================================================
void ReadInductance(uint8_t HighPin, uint8_t LowPin) {
  // check if inductor and measure the inductance value
  unsigned int tmpint;
  unsigned int ovcnt16;
  unsigned int umax;
  unsigned int total_r;
  uint8_t per_ref;
  uint8_t LoPinR_L, LoPinR_H;
  uint8_t HiADC;
  uint8_t ii;
  uint8_t count;
  uint8_t found;


  if(PartFound != PART_RESISTOR) {
     return;	//We have found no resistor  
  }
  for (found=0;found<ResistorsFound;found++) {
     if ((LowPin == resis[found].ra) && (HighPin == resis[found].rb) && resis[found].rx < 28000) goto mesinduk;
  }
  // no resistor found
  return;
 
mesinduk:
  LoPinR_L = MEM_read_byte(&PinRLtab[LowPin]);	//R_L mask for HighPin R_L load
  LoPinR_H = LoPinR_L + LoPinR_L;	//double for LowPin R_H load

//==================================================================================
// Measurement of Inductance values
// setup Analog Comparator
  ADC_COMP_CONTROL = (1<<ACME);			//enable Analog Comparator Multiplexer
  ACSR =  (1<<ACBG) | (1<<ACI)  | (1<<ACIC);	// enable, 1.3V, no Interrupt, Connect to Timer1 
  ADMUX = (1<<REFS0) | LowPin;			// switch Mux to High-Pin
  ADCSRA = (1<<ADIF) | AUTO_CLOCK_DIV; //disable ADC
  for (count=0;count<2;count++) {
     // Try two times
     HiADC = MEM_read_byte(&PinADCtab[HighPin]);
     R_PORT = 0;		// switch R_L to GND
     R_DDR = LoPinR_L;   		// switch R_L resistor port for LowPin to output (GND)
     ADC_PORT =   TXD_VAL;		// switch ADC-Port to GND
     ADC_DDR = HiADC | TXD_MSK;		// switch HiADC Pin to GND, 
     wait10ms();

   // setup Counter1
     ovcnt16 = 0;
     TCCR1A = 0;			// set Counter1 to normal Mode
     TCNT1 = 0;			//set Counter to 0
     TI1_INT_FLAGS = (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);	// mega88
     HiADC |= TXD_VAL;
     if (count == 0 ) {
        //first start counter, then start current
        TCCR1B =  (1<<ICNC1) | (0<<ICES1) | (1<<CS10);	//start counter 1MHz or 8MHz
        ADC_PORT = HiADC;		// switch ADC-Port to VCC
     } else {
        //first start current, then start counter with delay
        //parasitic capacity of coil can cause high current at the beginning
        ADC_PORT = HiADC;		// switch ADC-Port to VCC
        wdt_reset();			// delay
#if F_CPU >= 8000000UL
        wdt_reset();			// delay
        wdt_reset();			// delay
#endif
        TI1_INT_FLAGS = (1<<ICF1);	// Reset Input Capture
        TCCR1B =  (1<<ICNC1) | (0<<ICES1) | (1<<CS10);	//start counter 1MHz or 8MHz
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
           if(ovcnt16 == (F_CPU/500000)) {
              break; 	//Timeout for Charging, above 0.13 s
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
     if (count == 0) {
        cval_uncorrected = CombineII2Long(ovcnt16, tmpint);
     } else  {
        cval = CombineII2Long(ovcnt16, tmpint);
     }
  }  //end for
  ADCSRA = (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; //enable ADC
  ADC_PORT = TXD_VAL;		// switch ADC Port to GND
  wait10ms();
  if ((ovcnt16 >= (F_CPU/500000)) || (cval < cval_uncorrected)) {
     // no transition is detected at the second test or same transition
     cval = cval_uncorrected - 6;
  }
     
  lcd_line3();
  DisplayValue(cval,0,0,4);
  total_r = (RR680MI + resis[0].rx + RR680PL - R_L_VAL);
  umax = ((unsigned long)RR680MI * (unsigned long)ADCconfig.U_AVCC) / total_r;
  // Voltage of comparator in % of umax
  #ifdef AUTO_CAL
  per_ref = (unsigned long)(ref_mv + (int16_t)eeprom_read_word((uint16_t *)(&ref_offset))) * 100 / umax;
  #else
  per_ref = (unsigned long)(ref_mv + REF_C_KORR) * 100 / umax;
  #endif
  per_ref = (uint8_t)MEM2_read_byte(&LogTab[per_ref]);
  resis[found].lx = (cval * total_r ) / ((unsigned int)per_ref * (F_CPU/100000));


  //ready
  // switch all ports to input
  ADC_DDR =  TXD_MSK;		// switch all ADC ports to input
  R_DDR = 0;			// switch all resistor ports to input
  return;
 } // end ReadInductance()
