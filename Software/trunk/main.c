
#include <avr/io.h>
#include "lcd-routines.h"
#include "wait1000ms.h"
#include "config.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <math.h>
//#define DebugOut 3		// if set, output of voltages of resistor measurements in row 2,3,4
//#define DebugOut 4		// if set, output of voltages of Diode measurement in row 3+4
//#define DebugOut 5		// if set, output of Transistor checks in row 2+3
//#define DebugOut 10		// if set, output of capacity measurements (ReadCapacity) in row 3+4 
#include "Transistortester.h"

//begin of transistortester program
int main(void) {
  //switch on
  ON_DDR = (1<<ON_PIN);
  ON_PORT = (1<<ON_PIN) | (1<<RST_PIN);	//power on and Pullup for the Reset-Pin
  uint8_t tmp;
  //ADC-Init
  ADCSRA = (1<<ADEN) | AUTO_CLOCK_DIV;	//prescaler=8 or 64 (if 8Mhz clock)
#ifdef __AVR_ATmega8__
// #define WDRF_HOME MCU_STATUS_REG
 #define WDRF_HOME MCUCSR
#else
 #define WDRF_HOME MCUSR
#endif
  tmp = (WDRF_HOME & (1<<WDRF));	// save Watch Dog Flag
  WDRF_HOME &= ~(1<<WDRF);	 	//reset Watch Dog flag
  wdt_disable();			// disable Watch Dog
  lcd_init();				//initialize LCD
	
//  ADC_PORT = TXD_VAL;
//  ADC_DDR = TXD_MSK;
  if(tmp) { 
     // check if  Watchdog-Event 
     // this happens, if the Watchdog is not reset for 2s
     // can happen, if any loop in the Program doen't finish.
     lcd_line1();
     lcd_fix_string(TestTimedOut);	//Output Timeout
     wait3s();				//wait for 3 s
     ON_PORT = 0;			//shut off!
     return 0;
  }
  LCDLoadCustomChar(LCD_CHAR_DIODE1);	//Custom-Character Diode symbol
  lcd_fix_customchar(DiodeIcon1);	//load Character  >|
  LCDLoadCustomChar(LCD_CHAR_DIODE2);	//Custom-Character 
  lcd_fix_customchar(DiodeIcon2);	//load Character  |<
#ifdef C_MESS
  LCDLoadCustomChar(LCD_CHAR_CAP);	//Custom-Character  Capacitor symbol
  lcd_fix_customchar(CapIcon);		//load Character  ||
#endif
#ifdef R_MESS
  LCDLoadCustomChar(LCD_CHAR_RESIS1);	//Custom-Character Resistor symbol
  lcd_fix_customchar(ResIcon1);		//load Character  [
  LCDLoadCustomChar(LCD_CHAR_RESIS2);	//Custom-Character 
  lcd_fix_customchar(ResIcon2);		//load Character  ]
#endif
  
#ifdef LCD_CYRILLIC
  //if kyrillish LCD-Characterset, load  Omega- and µ-Character
  LCDLoadCustomChar(LCD_CHAR_OMEGA);	//Custom-Character
  //load Omega-Character to LCD
  lcd_fix_customchar(CyrillicOmegaIcon);
  LCDLoadCustomChar(LCD_CHAR_U);	//Custom-Character
  //load µ-Character to LCD 
  lcd_fix_customchar(CyrillicMuIcon);
#endif
#ifdef PULLUP_DISABLE
 #ifdef __AVR_ATmega8__
  SFIOR = (1<<PUD);		// disable Pull-Up Resistors mega8
 #else
  MCUCR = (1<<PUD);		// disable Pull-Up Resistors mega168 family
 #endif
#endif

#if POWER_OFF+0 > 1
  // tester display time selection
  display_time = OFF_WAIT_TIME;		// LONG_WAIT_TIME for single mode, else SHORT_WAIT_TIME
  if (!(ON_PIN_REG & (1<<RST_PIN))) {
     // if power button is pressed ...
     wait300ms();			// wait to catch a long key press
     if (!(ON_PIN_REG & (1<<RST_PIN))) {
        // check if power button is still pressed
        display_time = LONG_WAIT_TIME;	// ... set long time display anyway
     }
  }
#else
  #define display_time OFF_WAIT_TIME
#endif

  empty_count = 0;
  mess_count = 0;


//*****************************************************************
//Entry: if start key is pressed before shut down
start:
  PartFound = PART_NONE;
  NumOfDiodes = 0;
  PartReady = 0;
  PartMode = 0;
  lcd_clear();
  ADC_DDR = TXD_MSK;		//activate Software-UART 
#ifdef R_MESS
  ResistorsFound = 0;
#endif
#ifdef C_MESS
  ca = 0;
  cb = 0;
#endif
#ifdef WITH_UART
  uart_newline();		// start of new measurement
#endif
  Config.RefFlag = 0;
  Config.Samples = 190;		// set number of ADC samples near to max
  Config.U_AVCC = U_VCC;	// set VCC Voltage
  (void) ReadADC(0x0e);		// read Reference-voltage 
  ref_mv = W20msReadADC(0x0e);	// read Reference-voltage
  Config.U_Bandgap = ADC_internal_reference;	// set internal reference voltage for ADC
  Config.Samples = ANZ_MESS;	// set to configured number of ADC samples

#ifdef BAT_CHECK
  // Battery check is selected
  ReadADC(5);	//Dummy-Readout
  trans.hfe[0] = W5msReadADC(5); 	//with 5V reference
  lcd_line1();	//1. row 
  lcd_fix_string(Bat);		//output: "Bat. "
 #ifdef BAT_OUT
  // display Battery voltage
  // The divisor to get the voltage in 0.01V units is ((10*33)/133) witch is about 2.4812
  // A good result can be get with multiply by 4 and divide by 10 (about 0.75%).
  cval = (trans.hfe[0]*4)/10+((BAT_OUT+5)/10); // usually output only 2 digits
  DisplayValue(cval,-2,'V',2);		// Display 2 Digits of this 10mV units
  lcd_data(' ');
 #endif
 #if (BAT_POOR > 52) && (BAT_POOR < 190)
 #define WARN_LEVEL (((unsigned long)(BAT_POOR*100+1000)*(unsigned long)33)/133)
 #define POOR_LEVEL (((unsigned long)(BAT_POOR*100)*(unsigned long)33)/133)
 #else
 #warning "Battery POOR level is set for 7805 regulator"
 #define WARN_LEVEL (((unsigned long)7300*(unsigned long)33)/133)
 #define POOR_LEVEL (((unsigned long)6300*(unsigned long)33)/133)
 #endif
  // check the battery voltage
  if (trans.hfe[0] <  WARN_LEVEL) {
     //Vcc < 7,3V; show Warning 
     if(trans.hfe[0] < POOR_LEVEL) {	
        //Vcc <6,3V; no proper operation is possible
        lcd_fix_string(BatEmpty);	//Battery empty!
        wait2s();
        PORTD = 0;			//switch power off
        return 0;
     }
     lcd_fix_string(BatWeak);		//Battery weak
  } else { // Battery-voltage OK
     lcd_fix_string(OK); 		// "OK"
  }
#else
  lcd_fix_string(VERSION);		// if no Battery check, Version .. in row 1
#endif
#ifdef WDT_enabled
  wdt_enable(WDTO_2S);		//Watchdog on
#endif

//  wait1s();			// add mor time for reading batterie voltage
  // begin tests
#ifdef WITH_AUTO_REF
  RefVoltage();			//compute RHmultip = f(reference voltage)
#endif
  lcd_line2();			//LCD position row2, column 1
  lcd_fix_string(TestRunning);		//String: testing...
//#ifndef __AVR_ATmega8__
#if 0
  // does not read temperature, looks like internal reference
  trans.hfe[0] = ReadADC((1<<REFS1)|(1<<REFS0)|8); 	//read temperature sensor
  lcd_data(' ');
  lcd_string(itoa((int)(trans.hfe[0] - 289), outval, 10));	//output temperature
  lcd_data(LCD_CHAR_DEGREE);
  lcd_data('C');
#endif
#ifndef DebugOut
  lcd_line2();			//LCD position row 2, column 1
#endif
#ifdef C_MESS
  #define AUSGABE_FUNKTION
  EntladePins();		// discharge all capacitors!
  if(PartFound == PART_CELL) {
    lcd_clear();
//    lcd_line1();
    lcd_data('C');
    lcd_data('e');
    lcd_data('l');
    lcd_data('l');
    lcd_data('!');
    goto end2;
  }
#endif
#ifdef WITH_SELFTEST
  AutoCheck();			//check, if selftest should be done
#endif
     
  // check all 6 combinations for the 3 pins 
  CheckPins(TP1, TP2, TP3);
  CheckPins(TP1, TP3, TP2);
  CheckPins(TP2, TP1, TP3);
  CheckPins(TP2, TP3, TP1);
  CheckPins(TP3, TP2, TP1);
  CheckPins(TP3, TP1, TP2);
  
#ifdef C_MESS
  //separate check if is is a capacitor
  if(((PartFound == PART_NONE) || (PartFound == PART_RESISTOR) || (PartFound == PART_DIODE)) ) {
     EntladePins();		// discharge capacities
     //measurement of capacities in all 3 combinations
     ReadCapacity(TP3, TP1);
     ReadCapacity(TP3, TP2);
     ReadCapacity(TP2, TP1);
  }
#endif
  //All checks are done, output result to display
  lcd_clear();
//  lcd_line1();
  if(PartFound == PART_DIODE) {
     if(NumOfDiodes == 1) {		//single Diode
        lcd_fix_string(Diode);		//"Diode: "
        lcd_testpin(diodes[0].Anode);
        lcd_fix_string(AnKat);		//"->|-"
        lcd_testpin(diodes[0].Cathode);
        UfAusgabe(0x70);
#ifdef C_MESS
        lcd_fix_string(GateCap);		//"C="
        ReadCapacity(diodes[0].Cathode,diodes[0].Anode);	// Capacity opposite flow direction
        DisplayValue(cval,cpre,'F',3);
#endif
        goto end;
     } else if(NumOfDiodes == 2) { // double diode
        lcd_data('2');
        lcd_fix_string(Dioden);		//"diodes "
        if(diodes[0].Anode == diodes[1].Anode) { //Common Anode
           lcd_testpin(diodes[0].Cathode);
           lcd_fix_string(KatAn);	//"-|<-"
           lcd_testpin(diodes[0].Anode);
           lcd_fix_string(AnKat);	//"->|-"
           lcd_testpin(diodes[1].Cathode);
           UfAusgabe(0x01);
           goto end;
        } else if(diodes[0].Cathode == diodes[1].Cathode) { //Common Cathode
           lcd_testpin(diodes[0].Anode);
           lcd_fix_string(AnKat);	//"->|-"
	   lcd_testpin(diodes[0].Cathode);
           lcd_fix_string(KatAn);	//"-|<-"
           lcd_testpin(diodes[1].Anode);
           UfAusgabe(0x01);
           goto end;
          }
        else if ((diodes[0].Cathode == diodes[1].Anode) && (diodes[1].Cathode == diodes[0].Anode)) {
           //Antiparallel
           lcd_testpin(diodes[0].Anode);
           lcd_fix_string(AnKat);	//"->|-"
           lcd_testpin(diodes[0].Cathode);
           lcd_fix_string(AnKat);	//"->|-"
           lcd_testpin(diodes[1].Cathode);
           UfAusgabe(0x01);
           goto end;
        }
     } else if(NumOfDiodes == 3) {
        //Serial of 2 Diodes; was detected as 3 Diodes 
        trans.b = 3;
        trans.c = 3;
        /* Check for any constallation of 2 serial diodes:
          Only once the pin No of anyone Cathode is identical of another anode.
          two diodes in series is additionally detected as third big diode.
        */
        if(diodes[0].Cathode == diodes[1].Anode)
          {
           trans.b = 0;
           trans.c = 1;
          }
        if(diodes[0].Anode == diodes[1].Cathode)
          {
           trans.b = 1;
           trans.c = 0;
          }
        if(diodes[0].Cathode == diodes[2].Anode)
          {
           trans.b = 0;
           trans.c = 2;
          }
        if(diodes[0].Anode == diodes[2].Cathode)
          {
           trans.b = 2;
           trans.c = 0;
          }
        if(diodes[1].Cathode == diodes[2].Anode)
          {
           trans.b = 1;
           trans.c = 2;
          }
        if(diodes[1].Anode == diodes[2].Cathode)
          {
           trans.b = 2;
           trans.c = 1;
          }
#if DebugOut == 4
	lcd_line3();
        lcd_testpin(diodes[0].Anode);
        lcd_data(':');
        lcd_testpin(diodes[0].Cathode);
        lcd_data(' ');
        lcd_string(utoa(diodes[0].Voltage, outval, 10));
        lcd_data(' ');
        lcd_testpin(diodes[1].Anode);
        lcd_data(':');
        lcd_testpin(diodes[1].Cathode);
        lcd_data(' ');
        lcd_string(utoa(diodes[1].Voltage, outval, 10));
	lcd_line4();
        lcd_testpin(diodes[2].Anode);
        lcd_data(':');
        lcd_testpin(diodes[2].Cathode);
        lcd_data(' ');
        lcd_string(utoa(diodes[2].Voltage, outval, 10));
        lcd_line1();
#endif
        if((trans.b<3) && (trans.c<3)) {
           lcd_data('3');
           lcd_fix_string(Dioden);	//"Diodes "
           lcd_testpin(diodes[trans.b].Anode);
           lcd_fix_string(AnKat);	//"->|-"
           lcd_testpin(diodes[trans.b].Cathode);
           lcd_fix_string(AnKat);	//"->|-"
           lcd_testpin(diodes[trans.c].Cathode);
           UfAusgabe( (trans.b<<4)|trans.c);
           goto end;
        }
     }
     // end (PartFound == PART_DIODE)
  } else if (PartFound == PART_TRANSISTOR) {
    if(PartReady != 0) {
       if((trans.hfe[0]>trans.hfe[1])) {
          //if the amplification factor was higher at first testr: swap C and E !
          tmp = trans.c;
          trans.c = trans.e;
          trans.e = tmp;
       } else {
          trans.hfe[0] = trans.hfe[1];
          trans.uBE[0] = trans.uBE[1];
       }
    }

    if(PartMode == PART_MODE_NPN) {
       lcd_fix_string(NPN);		//"NPN "
    } else {
       lcd_fix_string(PNP);		//"PNP "
    }
    if(NumOfDiodes > 2) {	//Transistor with protection diode
       if(PartMode == PART_MODE_NPN) {
          lcd_fix_string(AnKat);	//"->|-"
       } else {
          lcd_fix_string(KatAn);	//"-|<-"
       }
    }
    lcd_fix_string(ebcstr);		//" EBC="
    lcd_testpin(trans.e);
    lcd_testpin(trans.b);
    lcd_testpin(trans.c);
    lcd_line2(); //2. row 
    lcd_fix_string(hfestr);		//"B="  (hFE)
    lcd_string(utoa(trans.hfe[0], outval, 10));
    lcd_data(' ');

    lcd_fix_string(Uf);		//"Uf="
    diodes[2].Voltage = trans.uBE[0];
    mVAusgabe(2);		// output: xxxxmV
    goto end;
    // end (PartFound == PART_TRANSISTOR)
  } else if (PartFound == PART_FET) {	//JFET or MOSFET
    if(PartMode&1) {
       //N-Kanal
       lcd_data('P');			//P-channel
    } else {
       lcd_data('N');			//N-channel
    }
    lcd_data('-');

    tmp = PartMode/2;
    if (tmp == (PART_MODE_N_D_MOS/2)) {
       lcd_data('D');			// N-D
    }
    if (tmp == (PART_MODE_N_E_MOS/2)) {
       lcd_data('E');			// N-E
    }
    if (tmp == (PART_MODE_N_JFET/2)) {
       lcd_fix_string(jfet);		//"JFET"
    } else {
       lcd_fix_string(mosfet);		//"-MOS "
    }
    lcd_fix_string(gds);		//"GDS="
    lcd_testpin(trans.b);
    lcd_testpin(trans.c);
    lcd_testpin(trans.e);
    if((NumOfDiodes > 0) && (PartMode < PART_MODE_N_D_MOS)) {
       //MOSFET with protection diode; only with enhancement-FETs
       if (PartMode&1) {
          lcd_data(LCD_CHAR_DIODE1);	//show Diode symbol >|
       } else {
          lcd_data(LCD_CHAR_DIODE2);	//show Diode symbol |<
       }
    }
    lcd_line2();			//2. Row
    if(PartMode < PART_MODE_N_D_MOS) {	//enhancement-MOSFET
 #ifdef C_MESS	//Gate capacity
       lcd_fix_string(GateCap);		//"C="
       ReadCapacity(trans.b,trans.e);	//measure capacity
       DisplayValue(cval,cpre,'F',3);
 #endif
       lcd_fix_string(vt);		// "Vt="
    } else {
       lcd_data('I');
       lcd_data('=');
       utoa(trans.uBE[1]+100,outval,10);
       lcd_data(outval[1]);
       lcd_data('.');
       lcd_data(outval[2]);
       lcd_fix_string(Vgs_str);		// "mA Vgs="
    }
    //Gate-threshold voltage
    diodes[0].Voltage = gthvoltage;
    mVAusgabe(0);			//Output gthvoltage
    goto end;
    // end (PartFound == PART_FET)
  } else if (PartFound == PART_THYRISTOR) {
    lcd_fix_string(Thyristor);		//"Thyristor"
    goto gakAusgabe;
  } else if (PartFound == PART_TRIAC) {
    lcd_fix_string(Triac);		//"Triac"
    goto gakAusgabe;
  }
 #ifdef R_MESS	//resistor measurement is wanted
  else if(PartFound == PART_RESISTOR) {
    if (ResistorsFound == 1) { // single resistor
       lcd_testpin(resis[0].rb);  	//Pin-number 1
       lcd_fix_string(Resistor_str);
       lcd_testpin(resis[0].ra);		//Pin-number 2
    } else { // R-Max suchen
       ii = 0;
       if (resis[1].rx > resis[0].rx)
          ii = 1;
       if (ResistorsFound == 2) {
          ii = 2;
       } else {
          if (resis[2].rx > resis[ii].rx) {
             ii = 2;
          }
       }

       if (ii == 0) {
          lcd_data('1');
          lcd_fix_string(Resistor_str);	// -[=]-
          lcd_data('3');
          lcd_fix_string(Resistor_str);	// -[=]-
          lcd_data('2');
       }
       if (ii == 1) {
          lcd_data('1');
          lcd_fix_string(Resistor_str);	// -[=]-
          lcd_data('2');
          lcd_fix_string(Resistor_str);	// -[=]-
          lcd_data('3');
       }
       if (ii == 2) {
          lcd_data('2');
          lcd_fix_string(Resistor_str);	// -[=]-
          lcd_data('1');
          lcd_fix_string(Resistor_str);	// -[=]-
          lcd_data('3');
       }
    }
    lcd_line2(); //2. row 
    if (ResistorsFound == 1) {
       RvalOut(0);
    } else {
       // output resistor values in right order
       if (ii == 0) {
          RvalOut(1);
          RvalOut(2);
       }
       if (ii == 1) {
          RvalOut(0);
          RvalOut(2);
       }
       if (ii == 2) {
          RvalOut(0);
          RvalOut(1);
       }
    }
    goto end;

  } // end (PartFound == PART_RESISTOR)
#endif

#ifdef C_MESS
//capacity measurement is wanted
  else if(PartFound == PART_CAPACITOR) {
//     lcd_fix_string(Capacitor);
     lcd_testpin(ca);			//Pin number 1
     lcd_fix_string(CapZeich);		// capacitor sign
     lcd_testpin(cb);			//Pin number 2
     lcd_line2(); 				//2. row 
     DisplayValue(cval,cpre,'F',4);
     goto end;
  }
#endif
  if(NumOfDiodes == 0) { //no diodes are found
     lcd_fix_string(TestFailed1); 	//"Kein,unbek. oder"
     lcd_line2(); //2. row 
     lcd_fix_string(TestFailed2); 	//"defektes "
     lcd_fix_string(Bauteil);		//"Bauteil"
  } else {
     lcd_fix_string(Bauteil);		//"Bauteil"
     lcd_fix_string(Unknown); 		//" unbek."
     lcd_line2(); //2. row 
     lcd_fix_string(OrBroken); 		//"oder defekt "
     lcd_data(NumOfDiodes + '0');
     lcd_fix_string(AnKat);		//"->|-"
  }
  empty_count++;
  mess_count = 0;
  goto end2;


gakAusgabe:
  lcd_line2(); //2. row 
  lcd_fix_string(GAK);		//"GAK="
  lcd_testpin(trans.b);
  lcd_testpin(trans.c);
  lcd_testpin(trans.e);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 end:
  empty_count = 0;		// reset counter, if part is found
  mess_count++;			// count measurements

 end2:
  while(!(ON_PIN_REG & (1<<RST_PIN)));	//wait ,until button is released
  wait200ms();
// wait 10 seconds or 3 seconds (if repeat function)
  for(gthvoltage = 0;gthvoltage<display_time;gthvoltage++) {
     if(!(ON_PIN_REG & (1<<RST_PIN))) {
        // If the key is pressed again... 
        // goto start of measurement 
        goto start;
     }
     wdt_reset();
     wait1ms();
  }
#ifdef POWER_OFF
 #if POWER_OFF > 127
  #define POWER2_OFF 255
 #else
  #define POWER2_OFF POWER_OFF*2
 #endif
 #if POWER_OFF+0 > 1
  if ((empty_count < POWER_OFF) && (mess_count < POWER2_OFF)) {
     goto start;			// repeat measurement POWER_OFF times
  }
 #endif
  // only one Measurement requested, shut off
  wdt_disable();			//Watchdog off
  ON_PORT &= ~(1<<ON_PIN);		//switch off power
  //never ending loop 
  while(1) {
     if(!(ON_PIN_REG & (1<<RST_PIN))) {
        // The statement is only reached if no auto off equipment is installed
        goto start;
     }
  }
#else
  goto start;	// POWER_OFF not selected, repeat measurement
#endif
  return 0;
}   // end main


//******************************************************************
// output of flux voltage for 1-2 diodes in row 2
// bcdnum = Numbers of both Diodes:
// higher 4 Bit  number of first Diode
// lower 4 Bit  number of second Diode (Structure diodes[nn])
// if number >= 3  no output is done
void UfAusgabe(uint8_t bcdnum) {

   lcd_line2(); 				//2. row
   lcd_fix_string(Uf);			//"Uf="
   mVAusgabe(bcdnum >> 4);
   mVAusgabe(bcdnum & 0x0f);
}
void mVAusgabe(uint8_t nn) {
   if (nn < 3) {
      // Output in mV units
      DisplayValue(diodes[nn].Voltage,-3,'V',3);
      lcd_data(' ');
   }
}

#ifdef R_MESS
  #define AUSGABE_FUNKTION
void RvalOut(uint8_t ii) {	
   // output of resistor value

   DisplayValue(resis[ii].rx,-1,LCD_CHAR_OMEGA,4);
   lcd_data(' ');
 }
#endif

//******************************************************************
#include "CheckPins.c"

void ChargePin10ms(uint8_t PinToCharge, uint8_t ChargeDirection) {
   //Load the specified pin to the specified direction with 680 Ohm for 10ms.
   //Will be used by discharge of MOSFET Gates or to load big capacities.
   //Parameters:
   //PinToCharge: specifies the pin as mask for R-Port
   //ChargeDirection: 0 = switch to GND (N-Kanal-FET), 1= switch to VCC(P-Kanal-FET)

   if(ChargeDirection&1) {
      R_PORT |= PinToCharge;	//R_PORT to 1 (VCC) 
   } else {
      R_PORT &= ~PinToCharge; // or 0 (GND)
   }
   R_DDR |= PinToCharge;		//switch Pin to output, across R to GND or VCC
   wait10ms();
   // switch back Input, no current
   R_DDR &= ~PinToCharge;	// switch back to input
   R_PORT &= ~PinToCharge;	// no Pull up
}

// first discharge any charge of capacitors
void EntladePins() {
  uint8_t adc_gnd;		// Mask of ADC-outputs, which can be directly connected to GND
  unsigned int adcmv[3];	// voltages of 3 Pins in mV
  unsigned int clr_cnt;		// Clear Counter
  uint8_t lop_cnt;		// loop counter
// time of discharge in ms  (10000/20)
#define MAX_ENTLADE_ZEIT  (10000/20)

  for(lop_cnt=0;lop_cnt<10;lop_cnt++) {
     adc_gnd = TXD_MSK;		// put all ADC to Input
     ADC_DDR = adc_gnd;
     ADC_PORT = TXD_VAL;		// ADC-outputs auf 0
     R_PORT = 0;			// R-outputs auf 0
     R_DDR = (2<<(PC2*2)) | (2<<(PC1*2)) | (2<<(PC0*2)); // R_H for all Pins to GND
     adcmv[0] = W5msReadADC(PC0);	// which voltage has Pin 1?
     adcmv[1] = ReadADC(PC1);	// which voltage has Pin 2?
     adcmv[2] = ReadADC(PC2);	// which voltage has Pin 3?
     if ((PartFound == PART_CELL) || (adcmv[0] < CAP_EMPTY_LEVEL) & (adcmv[1] < CAP_EMPTY_LEVEL) & (adcmv[2] < CAP_EMPTY_LEVEL)) {
        ADC_DDR = TXD_MSK;		// switch all ADC-Pins to input
        R_DDR = 0;			// switch all R_L Ports (and R_H) to input
        return;
     }
     // all Pins with voltage lower than 1V can be connected directly to GND (ADC-Port)
     if (adcmv[0] < 1000) {
        adc_gnd |= (1<<PC0);	//Pin 1 directly to GND
     }
     if (adcmv[1] < 1000) {
        adc_gnd |= (1<<PC1);	//Pin 2 directly to GND
     }
     if (adcmv[2] < 1000) {
        adc_gnd |= (1<<PC2);	//Pin 3 directly to  GND
     }
     ADC_DDR = adc_gnd;		// switch all selected ADC-Ports at the same time

     // additionally switch the leaving Ports with R_L to GND.
     // since there is no disadvantage for the already directly switched pins, we can
     // simply switch all  R_L resistors to GND
     R_DDR = (1<<(PC2*2)) | (1<<(PC1*2)) | (1<<(PC0*2));	// Pins across R_L resistors to GND
     for(clr_cnt=0;clr_cnt<MAX_ENTLADE_ZEIT;clr_cnt++) {
        wdt_reset();
        adcmv[0] = W20msReadADC(PC0);	// which voltage has Pin 1?
        adcmv[1] = ReadADC(PC1);	// which voltage has Pin 2?
        adcmv[2] = ReadADC(PC2);	// which voltage has Pin 3?
        if (adcmv[0] < 1300) {
           ADC_DDR |= (1<<PC0);	// below 1.3V , switch directly with ADC-Port to GND
        }
        if (adcmv[1] < 1300) {
           ADC_DDR |= (1<<PC1);	// below 1.3V, switch directly with ADC-Port to GND
        }
        if (adcmv[2] < 1300) {
           ADC_DDR |= (1<<PC2);	// below 1.3V, switch directly with ADC-Port to GND
        }
        if ((adcmv[0] < (CAP_EMPTY_LEVEL+2)) && (adcmv[1] < (CAP_EMPTY_LEVEL+2)) && (adcmv[2] < (CAP_EMPTY_LEVEL+2))) {
           break;
        }
     }
     if (clr_cnt == MAX_ENTLADE_ZEIT) {
        PartFound = PART_CELL;	// mark as Battery
        // there is charge on capacitor, warn later!
//        lcd_line3();
//        lcd_data('E');
//        lcd_string(utoa(adcmv[0], outval, 10));
//        lcd_data(' ');
//        lcd_string(utoa(adcmv[1], outval, 10));
//        lcd_data(' ');
//        lcd_string(utoa(adcmv[2], outval, 10));
     }
     for(adcmv[0]=0;adcmv[0]<clr_cnt;adcmv[0]++) {
        // for safety, discharge 5% of discharge  time
        wait1ms();
     }
  } // end while
 }




#ifdef C_MESS	//measurement of capacity is wanted
#include "ReadCapacity.c"

unsigned int getRLmultip(unsigned int cvolt) {

// interpolate table RLtab corresponding to voltage cvolt
// Widerstand 680 Ohm          300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
//uint16_t RLtab[] MEM_TEXT = {22447,20665,19138,17815,16657,15635,14727,13914,13182,12520,11918,11369,10865,10401, 9973, 9577, 9209, 8866, 8546, 8247, 7966, 7702, 7454, 7220, 6999, 6789, 6591, 6403, 6224, 6054, 5892, 5738, 5590, 5449, 5314, 5185, 5061, 4942, 4828, 4718, 4613, 4511, 4413, 4319, 4228};

#define RL_Tab_Abstand 25	// displacement of table 25mV
#define RL_Tab_Beginn 300	// begin of table ist 300mV
#define RL_Tab_Length 1100	// length of table is 1400-300

  unsigned int uvolt;
  unsigned int y1, y2;
  uint8_t tabind;
  uint8_t tabres;
  if (cvolt >= RL_Tab_Beginn) {
     uvolt = cvolt - RL_Tab_Beginn;
  } else {
     uvolt = 0;		// limit to begin of table
  }
  tabind = uvolt / RL_Tab_Abstand;
  tabres = uvolt % RL_Tab_Abstand;
  tabres = RL_Tab_Abstand - tabres;
  if (tabind > (RL_Tab_Length/RL_Tab_Abstand)) {
     tabind = (RL_Tab_Length/RL_Tab_Abstand);	// limit to end of table
  }
  y1 = MEM_read_word(&RLtab[tabind]);
  y2 = MEM_read_word(&RLtab[tabind+1]);
  return ( ((y1 - y2) * tabres + (RL_Tab_Abstand/2)) / RL_Tab_Abstand + y2); // interpolate table
}


 #ifdef WITH_AUTO_REF
void RefVoltage(void) {
// RefVoltage interpolates table RHtab corresponding to voltage ref_mv .
// RHtab contain the factors to get capacity from load time with 470k for
// different Band gab reference voltages.
// for remember:
//resistor     470000 Ohm      1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
//uint16_t RHTAB[] MEM_TEXT = { 954, 903, 856, 814, 775, 740, 707, 676, 648};

#define Ref_Tab_Abstand 50		// displacement of table is 50mV
#define Ref_Tab_Beginn 1000		// begin of table is 1000mV

  unsigned int referenz;
  unsigned int y1, y2;
  uint8_t tabind;
  uint8_t tabres;

  #ifdef AUTO_CAL
  referenz = ref_mv + (int16_t)eeprom_read_word((uint16_t *)(&ref_offset));
  #else
  referenz = ref_mv + REF_C_KORR;
  #endif
  if (referenz >= Ref_Tab_Beginn) {
     referenz -= Ref_Tab_Beginn;
  } else  {
     referenz = 0;		// limit to begin of table
  }
  tabind = referenz / Ref_Tab_Abstand;
  tabres = referenz % Ref_Tab_Abstand;
  tabres = Ref_Tab_Abstand-tabres;
  if (tabind > 7) {
     tabind = 7;		// limit to end of table
  }
  // interpolate the table of factors
  y1 = MEM_read_word(&RHtab[tabind]);
  y2 = MEM_read_word(&RHtab[tabind+1]);
  // RHmultip is the interpolated factor to compute capacity from load time with 470k
  RHmultip = ((y1 - y2) * tabres + (Ref_Tab_Abstand/2)) / Ref_Tab_Abstand + y2;
 }
 #endif
#endif

#ifdef LCD_CLEAR
void lcd_clear_line(void) {
 // writes 20 spaces to LCD-Display, Cursor must be positioned to first column
 unsigned char ll;
 for (ll=0;ll<20;ll++) {
    lcd_data(' ');
 }
}
#endif

#ifdef AUSGABE_FUNKTION
/* ************************************************************************
 *   display of values and units
 * ************************************************************************ */


/*
 *  display value and unit
 *  - max. 4 digits excluding "." and unit
 *
 *  requires:
 *  - value
 *  - exponent of factor related to base unit (value * 10^x)
 *    e.g: p = 10^-12 -> -12
 *  - unit character (0 = none)
 *  digits = 2, 3 or 4
 */
void DisplayValue(unsigned long Value, int8_t Exponent, unsigned char Unit, unsigned char digits)
{
  char OutBuffer[15];
  unsigned int      Limit;
  unsigned char     Prefix;		/* prefix character */
  uint8_t           Offset;		/* exponent of offset to next 10^3 step */
  uint8_t           Index;		/* index ID */
  uint8_t           Length;		/* string length */


  Limit = 100;				/* scale value down to 2 digits */
  if (digits == 3) Limit = 1000;
  if (digits == 4) Limit = 10000;
  while (Value >= Limit)
  {
    Value += 5;				/* for automagic rounding */
    Value = Value / 10;			/* scale down by 10^1 */
    Exponent++;				/* increase exponent by 1 */
  }


  /*
   *  determine prefix
   */
  Length = Exponent + 12;
  if (Length <  0) Length = 0;		/* Limit to minimum prefix */
  if (Length > 18) Length = 18;		/* Limit to maximum prefix */
  Index = Length / 3;
  Offset = Length % 3;
    if (Offset > 0)
    {
      Index++;				/* adjust index for exponent offset, take next prefix */
      Offset = 3 - Offset;		/* reverse value (1 or 2) */
    }
  Prefix = MEM_read_byte((uint8_t *)(&PrefixTab[Index]));   /* look up prefix in table */
  /*
   *  display value
   */

  /* convert value into string */
  utoa((unsigned int)Value, OutBuffer, 10);
  Length = strlen(OutBuffer);

  /* position of dot */
  Exponent = Length - Offset;		/* calculate position */

  if (Exponent <= 0)			/* we have to prepend "0." */
  {
    /* 0: factor 10 / -1: factor 100 */
//    lcd_data('0');
    lcd_data('.');
    if (Exponent < 0) lcd_data('0');	/* extra 0 for factor 100 */
  }

  if (Offset == 0) Exponent = -1;	/* disable dot if not needed */

  /* adjust position to array or disable dot if set to 0 */
//  Exponent--;

  /* display value and add dot if requested */
  Index = 0;
  while (Index < Length)		/* loop through string */
  {
    lcd_data(OutBuffer[Index]);		/* display char */
    Index++;				/* next one */
    if (Index == Exponent) {
      lcd_data('.');			/* display dot */
    }
  }

  /* display prefix and unit */
  if (Prefix != 0) lcd_data(Prefix);
  if (Unit) lcd_data(Unit);
}

#endif

#ifdef WITH_SELFTEST
 #include "AutoCheck.c"
#endif
