// new code by K.-H. Kübbeler
// CalibrateCap tries to find the value of a capacitor by measuring the load time.
// first of all the capacitor is discharged.
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
// cap.cpre = units of cap.cval (-12==pF, -9=nF, -6=µF)
// ca   = Pin number (0-2) of the LowPin
// cb   = Pin number (0-2) of the HighPin

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
#if (TPCAP >= 0)
void CalibrationCap() {
  // read voltage offsets with calibration capacitor
  unsigned int tmpint;
  unsigned int adcv0;
  unsigned int adcv1;
#ifdef INHIBIT_SLEEP_MODE
  unsigned int ovcnt16;
#endif
  uint8_t ii;

     // special handling for build in calibration capacitor
  for (ii=0; ii<20; ii++) {
     ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
     ADC_DDR = (1<<TPCAP) | TXD_MSK;	// switch capacitor-Pin to output (GND)
     wait_about20ms();
     ADC_DDR = TXD_MSK;			// switch all ADC to input 
     adcv0 = ReadADC(TPCAP);		// voltage before any load 
     if (adcv0 < CAP_EMPTY_LEVEL) break;
  }

#ifdef AUTO_CAL
  pin_combination =  1;	// use one of the calibrated values
#endif


//==================================================================================
// Measurement of little capacity values
  //little capacity value, about  < 50 µF
  //measure with the R_H (470kOhm) resistor 
  R_PORT = 0;		// R_DDR ist HiPinR_L
  ADC_DDR = (1<<TPCAP) | TXD_MSK;	//switch the Cap Pin to output
  ADC_PORT = TXD_VAL;		//switch the ADC Pins to GND
  TCAP_PORT &= ~(1<<TCAP_RH);		// switch 470k resistor to GND
  TCAP_DDR |= (1<<TCAP_RH);
// setup Analog Comparator
  ADC_COMP_CONTROL = (1<<ACME);			//enable Analog Comparator Multiplexer
  ACSR =  (1<<ACBG) | (1<<ACI)  | (1<<ACIC);	// enable, 1.3V, no Interrupt, Connect to Timer1 
  ADMUX = (1<<REFS0) | TPCAP;			// switch Mux to CapPin
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
  TCAP_PORT |= (1<<TCAP_RH);		// switch 470k resistor to VCC
  TCCR1B = (0<<ICNC1) | (1<<CS10);	//Start counter 1MHz or 8MHz without Noise Canceler
  ADC_DDR &= ~(1<<TPCAP);		// charge capacitor with R_H slowly
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


//############################################################
  ADCSRA = (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; //enable ADC
  TCAP_DDR &= ~(1<<TCAP_RH);
  TCAP_PORT &= ~(1<<TCAP_RH);		// switch 470k resistor off
  adcv1 = ReadADC(TPCAP);   // get loaded voltage
  load_diff = adcv1 + REF_C_KORR - ref_mv;	// build difference of capacitor voltage to Reference Voltage
//############################################################
  if (ovcnt16 >= (F_CPU/10000)) {
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
#ifdef AUTO_CAL
     // auto calibration mode, cap_null can be updated in selftest section
     tmpint = eeprom_read_byte(&c_zero_tab[pin_combination]);	// read zero offset
     if (cap.cval > tmpint) {
         cap.cval -= tmpint;		//subtract zero offset (pF)
     } else {
         cap.cval = 0;			//unsigned long may not reach negativ value
     }
#else
     if (cap.cval > C_NULL) {
         cap.cval -= C_NULL;		//subtract constant offset (pF)
     } else {
         cap.cval = 0;			//unsigned long may not reach negativ value
     }
#endif
  }

#if F_CPU < 2000001
   if(cap.cval < 50)
#else 
   if(cap.cval < 25)
#endif 
     {
     // cap.cval can only be so little in pF unit, cap.cpre must not be testet!
      goto keinC;	//capacity to low, < 50pF @1MHz (25pF @8MHz)
     }
   // end low capacity 
   PartFound = PART_CAPACITOR;	//capacitor is found
   if ((cap.cpre > cap.cpre_max) || ((cap.cpre == cap.cpre_max) && (cap.cval > cap.cval_max))) {
      // we have found a greater one
      cap.cval_max = cap.cval;
      cap.cpre_max = cap.cpre;
#if FLASHEND > 0x1fff
      cap.v_loss = 0;		// lost voltage in 0.01%
#endif
      cap.ca = 0;		// save LowPin
      cap.cb = TPCAP;		// save HighPin
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
 } // end CalibrationCap()
#endif  /* (TPCAP >= 0) */
