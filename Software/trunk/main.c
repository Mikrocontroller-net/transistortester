
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <math.h>

#define MAIN_C
#include "Transistortester.h"

#ifndef INHIBIT_SLEEP_MODE
  // prepare sleep mode
  EMPTY_INTERRUPT(TIMER2_COMPA_vect);
  EMPTY_INTERRUPT(ADC_vect);
#endif

//begin of transistortester program
int main(void) {
  uint8_t ii;
  unsigned int max_time;
#ifdef SEARCH_PARASITIC
  unsigned long n_cval;		// capacitor value of NPN B-E diode, for deselecting the parasitic Transistor
  int8_t n_cpre;		// capacitor prefix of NPN B-E diode
#endif
  char an_cat;			// diode is anode-cathode type
  //switch on
  ON_DDR = (1<<ON_PIN);			// switch to output
  ON_PORT = (1<<ON_PIN); 		// switch power on 
#ifndef PULLUP_DISABLE
  RST_PORT |= (1<<RST_PIN); 	// enable internal Pullup for Start-Pin
#endif
  uint8_t tmp;
  //ADC-Init
  ADCSRA = (1<<ADEN) | AUTO_CLOCK_DIV;	//prescaler=8 or 64 (if 8Mhz clock)
#ifdef __AVR_ATmega8__
// #define WDRF_HOME MCU_STATUS_REG
 #define WDRF_HOME MCUCSR
#else
 #define WDRF_HOME MCUSR
 #if FLASHEND > 0x3fff
  // probably was a bootloader active, disable the UART
  UCSR0B = 0;		// disable UART, if started with bootloader
 #endif
#endif
#if (PROCESSOR_TYP == 644) || (PROCESSOR_TYP == 1280)
 #define BAUD_RATE 9600
  UBRR0H = (F_CPU / 16 / BAUD_RATE - 1) >> 8;
  UBRR0L = (F_CPU / 16 / BAUD_RATE - 1) & 0xff;
  UCSR0B = (1<<TXEN0);
  UCSR0C = (1<<USBS0) | (3<<UCSZ00);	// 2 stop bits, 8-bit
  while (!(UCSR0A & (1<<UDRE0))) { };	// wait for send data port ready 
#endif
  tmp = (WDRF_HOME & (1<<WDRF));	// save Watch Dog Flag
  WDRF_HOME &= ~(1<<WDRF);	 	//reset Watch Dog flag
  wdt_disable();			// disable Watch Dog
#ifndef INHIBIT_SLEEP_MODE
  // switch off unused Parts
 #if PROCESSOR_TYP == 644
  PRR0 = (1<<PRTWI) |  (1<<PRSPI) | (1<<PRUSART1);
//  PRR1 =  (1<<PRTIM3) ;
 #elif PROCESSOR_TYP == 1280
  PRR0 = (1<<PRTWI) |  (1<<PRSPI) | (1<<PRUSART1);
  PRR1 = (1<<PRTIM5) | (1<<PRTIM4) | (1<<PRTIM3) | (1<<PRUSART3) | (1<<PRUSART2) | (1<<PRUSART3);
 #else
  PRR = (1<<PRTWI) | (1<<PRSPI) | (1<<PRUSART0);
 #endif
  DIDR0 = (1<<ADC5D) | (1<<ADC4D) | (1<<ADC3D);	
  TCCR2A = (0<<WGM21) | (0<<WGM20);		// Counter 2 normal mode
  TCCR2B = CNTR2_PRESCALER;	//prescaler set in autoconf
  sei();				// enable interrupts
#endif
  lcd_init();				//initialize LCD
	
//  ADC_PORT = TXD_VAL;
//  ADC_DDR = TXD_MSK;
  if(tmp) { 
     // check if  Watchdog-Event 
     // this happens, if the Watchdog is not reset for 2s
     // can happen, if any loop in the Program doen't finish.
     lcd_line1();
     lcd_MEM_string(TestTimedOut);	//Output Timeout
     wait_about3s();				//wait for 3 s
#if (LCD_ST_TYPE == 7565)
     lcd_powersave();			// set graphical display to power save mode
#endif
     ON_PORT &= ~(1<<ON_PIN);			//shut off!
//     ON_DDR = (1<<ON_PIN);		//switch to GND
     return 0;
  }
#if (LCD_ST_TYPE == 0)
  LCDLoadCustomChar(LCD_CHAR_DIODE1);	//Custom-Character Diode symbol
  lcd_fix_customchar(DiodeIcon1);	//load Character  >|
  LCDLoadCustomChar(LCD_CHAR_DIODE2);	//Custom-Character 
  lcd_fix_customchar(DiodeIcon2);	//load Character  |<
  LCDLoadCustomChar(LCD_CHAR_CAP);	//Custom-Character  Capacitor symbol
  lcd_fix_customchar(CapIcon);		//load Character  ||
  LCDLoadCustomChar(LCD_CHAR_RESIS1);	//Custom-Character Resistor symbol
  lcd_fix_customchar(ResIcon1);		//load Character  [
  LCDLoadCustomChar(LCD_CHAR_RESIS2);	//Custom-Character 
  lcd_fix_customchar(ResIcon2);		//load Character  ]
  
  //if kyrillish LCD-Characterset is defined, load  Omega- and µ-Character
 #if LCD_CHAR_OMEGA < 8
  LCDLoadCustomChar(LCD_CHAR_OMEGA);	//load omega as Custom-Character
  lcd_fix_customchar(CyrillicOmegaIcon);
 #endif
 #if LCD_CHAR_U < 8
  LCDLoadCustomChar(LCD_CHAR_U);	//load mu as Custom-Character
  lcd_fix_customchar(CyrillicMuIcon);
 #endif
 #if LCD_CHAR_RESIS3 != 'R'
  LCDLoadCustomChar(LCD_CHAR_RESIS3);	//load Resistor symbol as Custom-Character
  lcd_fix_customchar(ResIcon3);		// load character ||
 #endif
#endif /* LCD_ST_TYPE == 0 */
#ifdef PULLUP_DISABLE
 #ifdef __AVR_ATmega8__
  SFIOR = (1<<PUD);		// disable Pull-Up Resistors mega8
 #else
  MCUCR = (1<<PUD);		// disable Pull-Up Resistors mega168 family
 #endif
#endif

//  DIDR0 = 0x3f;			//disable all Input register of ADC

//#if POWER_OFF+0 > 1
  // tester display time selection
#ifndef USE_EEPROM
  EE_check_init();		// init EEprom, if unset
#endif
#ifdef WITH_ROTARY_SWITCH
//  rotary_switch_present = eeprom_read_byte(&EE_RotarySwitch);
  rotary.ind = ROT_MSK+1;		//initilize state history with next call of check_rotary()
#endif
#if 1
  for (ii=0; ii<60; ii++) {
     if (RST_PIN_REG & (1<<RST_PIN)) break;	// button is released
     wait_about10ms();
  }
#else
  ii = 0;
  if (!(RST_PIN_REG & (1<<RST_PIN))) {
     // key is still pressed
     ii = wait_for_key_ms(700);	
  }
#endif
  display_time = OFF_WAIT_TIME;		// LONG_WAIT_TIME for single mode, else SHORT_WAIT_TIME
  if (ii > 30) {
     display_time = LONG_WAIT_TIME;	// ... set long time display anyway
  }
//#else
//  #define display_time OFF_WAIT_TIME
//#endif

#if POWER_OFF+0 > 1
  empty_count = 0;
  mess_count = 0;
#endif
  ADCconfig.RefFlag = 0;
  Calibrate_UR();		// get Ref Voltages and Pin resistance
#ifdef WITH_MENU
  if (ii >= 60) {
     function_menu();		// selection of function
  }
#endif

//*****************************************************************
//Entry: if start key is pressed before shut down
start:
  PartFound = PART_NONE;	// no part found
  NumOfDiodes = 0;		// Number of diodes = 0
  ptrans.count = 0;		// Number of found P type transistors
  ntrans.count = 0;		// Number of found N type transistors
  PartMode = PART_MODE_NONE;
  WithReference = 0;		// no precision reference voltage
#if (LCD_ST_TYPE == 7565)
  lcd_command(CMD_DISPLAY_ON);
  lcd_command(CMD_SET_ALLPTS_NORMAL);		// 0xa4
#endif
  lcd_clear();			// clear the LCD
  ADC_DDR = TXD_MSK;		// activate Software-UART 
  ResistorsFound = 0;		// no resistors found
  cap.ca = 0;
  cap.cb = 0;
#if FLASHEND > 0x1fff
  inductor_lpre = 0;		// mark as zero
#endif
#ifdef WITH_UART
  uart_newline();		// start of new measurement
#endif
  Calibrate_UR();		// get Ref Voltages and Pin resistance
  lcd_line1();			// Cursor to 1. row, column 1
  
#ifdef BAT_CHECK
  // Battery check is selected
  ReadADC(TPBAT);	//Dummy-Readout
  ptrans.uBE = W5msReadADC(TPBAT); 	//with 5V reference
  lcd_MEM_string(Bat_str);		//output: "Bat. "
 #ifdef BAT_OUT
  // display Battery voltage
  // The divisor to get the voltage in 0.01V units is ((10*33)/133) witch is about 2.4812
  // A good result can be get with multiply by 4 and divide by 10 (about 0.75%).
//  cap.cval = (ptrans.uBE*4)/10+((BAT_OUT+5)/10); // usually output only 2 digits
//  DisplayValue(cap.cval,-2,'V',2);		// Display 2 Digits of this 10mV units
  #if BAT_NUMERATOR <= (0xffff/U_VCC)
  cap.cval = (ptrans.uBE*BAT_NUMERATOR)/BAT_DENOMINATOR + BAT_OUT;
  #else
   #if (BAT_NUMERATOR == 133) && (BAT_DENOMINATOR == 33)
  cap.cval = (ptrans.uBE*4)+BAT_OUT;		// usually output only 2 digits
   #else
  cap.cval = ((unsigned long)ptrans.uBE*BAT_NUMERATOR)/BAT_DENOMINATOR + BAT_OUT;
   #endif
  #endif
  DisplayValue(cap.cval,-3,'V',2);		// Display 2 Digits of this 10mV units
  lcd_space();
 #endif
 #if (BAT_POOR > 12000)
   #warning "Battery POOR level is set very high!"
 #endif
 #if (BAT_POOR < 2500)
   #warning "Battery POOR level is set very low!"
 #endif
 #if (BAT_POOR > 5300)
  // use .8 V difference to Warn-Level
  #define WARN_LEVEL (((unsigned long)(BAT_POOR+800)*(unsigned long)BAT_DENOMINATOR)/BAT_NUMERATOR)
 #elif (BAT_POOR > 3249)
  // less than 5.4 V only .4V difference to Warn-Level
  #define WARN_LEVEL (((unsigned long)(BAT_POOR+400)*(unsigned long)BAT_DENOMINATOR)/BAT_NUMERATOR)
 #elif (BAT_POOR > 1299)
  // less than 2.9 V only .2V difference to Warn-Level
  #define WARN_LEVEL (((unsigned long)(BAT_POOR+200)*(unsigned long)BAT_DENOMINATOR)/BAT_NUMERATOR)
 #else
  // less than 1.3 V only .1V difference to Warn-Level
  #define WARN_LEVEL (((unsigned long)(BAT_POOR+100)*(unsigned long)BAT_DENOMINATOR)/BAT_NUMERATOR)
 #endif
 #define POOR_LEVEL (((unsigned long)(BAT_POOR)*(unsigned long)BAT_DENOMINATOR)/BAT_NUMERATOR)

  // check the battery voltage
  if (ptrans.uBE <  WARN_LEVEL) {
     //Vcc < 7,3V; show Warning 
     if(ptrans.uBE < POOR_LEVEL) {	
        //Vcc <6,3V; no proper operation is possible
        lcd_MEM_string(BatEmpty);	//Battery empty!
        wait_about2s();
        PORTD = 0;			//switch power off
        return 0;
     }
     lcd_MEM_string(BatWeak);		//Battery weak
  } else { // Battery-voltage OK
     lcd_MEM_string(OK_str); 		// "OK"
  }
#else
  lcd_MEM2_string(VERSION_str);		// if no Battery check, Version .. in row 1
#endif	/* BAT_CHECK */
#ifdef WDT_enabled
  wdt_enable(WDTO_2S);		//Watchdog on
#endif

//  wait_about1s();			// add more time for reading batterie voltage
  // begin tests
#if FLASHEND > 0x1fff
  if (WithReference) {
     /* 2.5V precision reference is checked OK */
 #if POWER_OFF+0 > 1
     if ((mess_count == 0) && (empty_count == 0))
 #endif
     {
         /* display VCC= only first time */
         lcd_line2();
         lcd_MEM_string(VCC_str);		// VCC=
         DisplayValue(ADCconfig.U_AVCC,-3,'V',3);	// Display 3 Digits of this mV units
         wait_about1s();
     }
  }
#endif
#ifdef WITH_VEXT
  unsigned int Vext;
  // show the external voltage
  while (!(RST_PIN_REG & (1<<RST_PIN))) {
     lcd_line2();
     lcd_clear_line();
     lcd_line2();
     lcd_MEM_string(Vext_str);		// Vext=
     ADC_DDR = 0;		//deactivate Software-UART
     Vext = W5msReadADC(TPext);	// read external voltage 
//     ADC_DDR = TXD_MSK;		//activate Software-UART 
//#ifdef WITH_UART
//     uart_newline();		// start of new measurement
//#endif
 #if EXT_NUMERATOR <= (0xffff/U_VCC)
     DisplayValue(Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3);	// Display 3 Digits of this mV units
 #else
     DisplayValue((unsigned long)Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3);	// Display 3 Digits of this mV units
 #endif
     wait_about300ms();
  }
#endif /* WITH_VEXT */

#ifndef DebugOut
  lcd_line2();			//LCD position row 2, column 1
#endif
  EntladePins();		// discharge all capacitors!
  if(PartFound == PART_CELL) {
    lcd_clear();
    lcd_MEM_string(Cell_str);	// display "Cell!"
#if FLASHEND > 0x3fff
    lcd_line2();		// use LCD line 2
    DisplayValue(cell_mv[0],-3,'V',3);
    lcd_space();
    DisplayValue(cell_mv[1],-3,'V',3);
    lcd_space();
    DisplayValue(cell_mv[2],-3,'V',3);
#endif
#ifdef WITH_SELFTEST
    wait_about2s();
    AutoCheck(0x11);		// full Selftest with "Short probes" message
#endif
    goto end;
  }

#ifdef WITH_SELFTEST
 #ifdef AUTO_CAL
  lcd_cursor_off();
  UnCalibrated = (eeprom_read_byte(&c_zero_tab[3]) - eeprom_read_byte(&c_zero_tab[0]));
  if (UnCalibrated != 0) {
     // if calibrated, both c_zero_tab values are identical! c_zero_tab[3] is not used otherwise
     lcd_cursor_on();
  }
 #endif
 #ifdef WITH_MENU
  AutoCheck(0x00);			//check, if selftest should be done, only calibration
 #else
  AutoCheck(0x01);			//check, if selftest should be done, full selftest without MENU
 #endif
#endif
  lcd_line2();			//LCD position row2, column 1
  lcd_MEM_string(TestRunning);		//String: testing...
     
  // check all 6 combinations for the 3 pins 
//         High  Low  Tri
  CheckPins(TP1, TP2, TP3);
  CheckPins(TP2, TP1, TP3);

  CheckPins(TP1, TP3, TP2);
  CheckPins(TP3, TP1, TP2);

  CheckPins(TP2, TP3, TP1);
  CheckPins(TP3, TP2, TP1);

  if (ResistorsFound != 0) {
     if (resis[ResistorsFound-1].checked  == 0) {
        ResistorsFound--;	// last resistor is not checked in both directions
     }
  }
  
  // Capacity measurement is only possible correctly with two Pins connected.
  // A third connected pin will increase the capacity value!
//  if(((PartFound == PART_NONE) || (PartFound == PART_RESISTOR) || (PartFound == PART_DIODE)) ) {
  if(PartFound == PART_NONE) {
     // If no part is found yet, check separate if is is a capacitor
#if FLASHEND > 0x1fff
     lcd_data('C');
#endif
     EntladePins();		// discharge capacities
     //measurement of capacities in all 3 combinations
     cap.cval_max = 0;		// set max to zero
     cap.cpre_max = -12;	// set max to pF unit
     ReadCapacity(TP3, TP1);
#if DebugOut != 10
     ReadCapacity(TP3, TP2);
     ReadCapacity(TP2, TP1);
#endif
  }

  //All checks are done, output result to display

#ifdef DebugOut 
  // only clear two lines of LCD
  lcd_line2();
  lcd_clear_line();
  lcd_line2();
  lcd_line1();
  lcd_clear_line();
  lcd_line1();
#else
  lcd_clear();				// clear total display
#endif

  _trans = &ntrans;			// default transistor structure to show
  if (PartFound == PART_THYRISTOR) {
    lcd_MEM_string(Thyristor);		//"Thyristor"
    PinLayout(Cathode_char,'G','A'); 	// CGA= or 123=...
#ifdef WITH_GRAPHICS
    lcd_pgm_bitmap(bmp_thyristor_data, 102, 32, 0);
    lcd_draw_trans_pins(90, 48);
#endif
    goto TyUfAusgabe;
  }

  if (PartFound == PART_TRIAC) {
    lcd_MEM_string(Triac);		//"Triac"
    PinLayout('1','G','2'); 	// CGA= or 123=...
#ifdef WITH_GRAPHICS
//    lcd_pgm_bitmap(bmp_triac_data, 104, 32, 0);
    lcd_pgm_bitmap(bmp_triac_data, 94, 32, 0);
//    lcd_draw_trans_pins(104, 56, 97, 48, 106, 32);
    lcd_draw_trans_pins(88, 48);
#endif
    goto TyUfAusgabe;
  }

  if (PartFound == PART_CAPACITOR) {
//     lcd_MEM_string(Capacitor);
     lcd_testpin(cap.ca);		//Pin number 1
     lcd_MEM_string(CapZeich);		// capacitor sign
     lcd_testpin(cap.cb);		//Pin number 2
#if FLASHEND > 0x1fff
     GetVloss();			// get Voltage loss of capacitor
     if (cap.v_loss != 0) {
        lcd_MEM_string(VLOSS_str);	// "  Vloss="
        DisplayValue(cap.v_loss,-1,'%',2);
     }
#endif
     lcd_line2(); 			//2. row 
     DisplayValue(cap.cval_max,cap.cpre_max,'F',4);
#if FLASHEND > 0x1fff
     cap.esr = GetESR(cap.cb, cap.ca);		// get ESR of capacitor
     if ( cap.esr < 65530) {
        lcd_MEM_string(ESR_str);
        DisplayValue(cap.esr,-2,LCD_CHAR_OMEGA,2);
     }
#endif
     goto end;
  }

  if(PartFound == PART_DIODE) {
     if(NumOfDiodes == 1) {		//single Diode
//        lcd_MEM_string(Diode);		//"Diode: "
#if FLASHEND > 0x1fff
        // enough memory to sort the pins
 #if EBC_STYLE == 321
        // the higher test pin number is left side
        if (diodes.Anode[0] > diodes.Cathode[0]) {
           lcd_testpin(diodes.Anode[0]);
           lcd_MEM_string(AnKat);	//"->|-"
           lcd_testpin(diodes.Cathode[0]);
        } else {
           lcd_testpin(diodes.Cathode[0]);
           lcd_MEM_string(KatAn);	//"-|<-"
           lcd_testpin(diodes.Anode[0]);
        }
 #else
        // the higher test pin number is right side
        if (diodes.Anode[0] < diodes.Cathode[0]) {
           lcd_testpin(diodes.Anode[0]);
           lcd_MEM_string(AnKat);	//"->|-"
           lcd_testpin(diodes.Cathode[0]);
        } else {
           lcd_testpin(diodes.Cathode[0]);
           lcd_MEM_string(KatAn);	//"-|<-"
           lcd_testpin(diodes.Anode[0]);
        }
 #endif
#else
        // too less memory to sort the pins
        lcd_testpin(diodes.Anode[0]);
        lcd_MEM_string(AnKat);		//"->|-"
        lcd_testpin(diodes.Cathode[0]);
#endif
#if FLASHEND > 0x1fff
	GetIr(diodes.Cathode[0],diodes.Anode[0]);
#endif
        UfAusgabe(0x70);
        /* load current of capacity is (5V-1.1V)/(470000 Ohm) = 8298nA */
        lcd_MEM_string(GateCap_str);	//"C="
        ReadCapacity(diodes.Cathode[0],diodes.Anode[0]);	// Capacity opposite flow direction
        DisplayValue(cap.cval,cap.cpre,'F',3);
        goto end3;
     } else if(NumOfDiodes == 2) { // double diode
        lcd_data('2');
        lcd_MEM_string(Dioden);		//"diodes "
        if(diodes.Anode[0] == diodes.Anode[1]) { //Common Anode
           lcd_testpin(diodes.Cathode[0]);
           lcd_MEM_string(KatAn);	//"-|<-"
           lcd_testpin(diodes.Anode[0]);
           lcd_MEM_string(AnKat);	//"->|-"
           lcd_testpin(diodes.Cathode[1]);
           UfAusgabe(0x01);
           goto end3;
        } 
        if(diodes.Cathode[0] == diodes.Cathode[1]) { //Common Cathode
           lcd_testpin(diodes.Anode[0]);
           lcd_MEM_string(AnKat);	//"->|-"
	   lcd_testpin(diodes.Cathode[0]);
           lcd_MEM_string(KatAn);	//"-|<-"
           lcd_testpin(diodes.Anode[1]);
           UfAusgabe(0x01);
           goto end3;
//        else if ((diodes.Cathode[0] == diodes.Anode[1]) && (diodes.Cathode[1] == diodes.Anode[0])) 
        } 
        if (diodes.Cathode[0] == diodes.Anode[1]) {
           // normaly two serial diodes are detected as three diodes, but if the threshold is high
           // for both diodes, the third diode is not detected.
           // can also be Antiparallel
           diode_sequence = 0x01;	// 0 1
           SerienDiodenAusgabe();
           goto end3;
        } 
        if (diodes.Cathode[1] == diodes.Anode[0]) {
           diode_sequence = 0x10;	// 1 0
           SerienDiodenAusgabe();
           goto end3;
        }
     } else if(NumOfDiodes == 3) {
        //Serial of 2 Diodes; was detected as 3 Diodes 
        diode_sequence = 0x33;	// 3 3
        /* Check for any constallation of 2 serial diodes:
          Only once the pin No of anyone Cathode is identical of another anode.
          two diodes in series is additionally detected as third big diode.
        */
        if(diodes.Cathode[0] == diodes.Anode[1])
          {
           diode_sequence = 0x01;	// 0 1
          }
        if(diodes.Anode[0] == diodes.Cathode[1])
          {
           diode_sequence = 0x10;	// 1 0
          }
        if(diodes.Cathode[0] == diodes.Anode[2])
          {
           diode_sequence = 0x02;	// 0 2
          }
        if(diodes.Anode[0] == diodes.Cathode[2])
          {
           diode_sequence = 0x20;	// 2 0
          }
        if(diodes.Cathode[1] == diodes.Anode[2])
          {
           diode_sequence = 0x12;	// 1 2
          }
        if(diodes.Anode[1] == diodes.Cathode[2])
          {
           diode_sequence = 0x21;	// 2 1
          }
#if DebugOut == 4
	lcd_line3();
        lcd_testpin(diodes.Anode[0]);
        lcd_data(':');
        lcd_testpin(diodes.Cathode[0]);
        lcd_space();
        u2lcd(diodes.Voltage[0]);
        lcd_space();
        lcd_testpin(diodes.Anode[1]);
        lcd_data(':');
        lcd_testpin(diodes.Cathode[1]);
        lcd_space();
        u2lcd(diodes.Voltage[1]);
	lcd_line4();
        lcd_testpin(diodes.Anode[2]);
        lcd_data(':');
        lcd_testpin(diodes.Cathode[2]);
        lcd_space();
        u2lcd(diodes.Voltage[2]);
        lcd_line1();
#endif
//        if((ptrans.b<3) && (ptrans.c<3)) 
        if(diode_sequence < 0x22) {
           lcd_data('3');
           lcd_MEM_string(Dioden);	//"Diodes "
           SerienDiodenAusgabe();
           goto end3;
        }
     }  // end (NumOfDiodes == 3)
     lcd_MEM_string(Bauteil);		//"Bauteil"
     lcd_MEM_string(Unknown); 		//" unbek."
     lcd_line2(); //2. row 
     lcd_MEM_string(OrBroken); 		//"oder defekt "
     lcd_data(NumOfDiodes + '0');
     lcd_MEM_string(AnKat);		//"->|-"
     goto not_known;
     // end (PartFound == PART_DIODE)
  } else if (PartFound == PART_TRANSISTOR) {
#ifdef SEARCH_PARASITIC
    if ((ptrans.count != 0) && (ntrans.count !=0)) {
       // Special Handling of NPNp and PNPn Transistor.
       // If a protection diode is built on the same structur as the NPN-Transistor,
       // a parasitic PNP-Transistor will be detected. 
       ReadCapacity(ntrans.e, ntrans.b);	// read capacity of NPN base-emitter
       n_cval = cap.cval;			// save the found capacity value
       n_cpre  = cap.cpre;			// and dimension
       ReadCapacity(ptrans.b, ptrans.e);	// read capacity of PNP base-emitter
       if (((n_cpre == cap.cpre) && (cap.cval > n_cval)) || (cap.cpre > n_cpre)){
          // the capacity value or dimension of the PNP B-E is greater than the NPN B-E
          PartMode = PART_MODE_PNP;
       } else {
          PartMode = PART_MODE_NPN;
       }
    }
#endif
//#if FLASHEND > 0x1fff
    // not possible for mega8, change Pin sequence instead.
    if ((ptrans.count != 0) && (ntrans.count !=0) && (!(RST_PIN_REG & (1<<RST_PIN)))) {
       // if the Start key is still pressed, use the other Transistor
       if (PartMode == PART_MODE_NPN) {
          PartMode = PART_MODE_PNP;	// switch to parasitic transistor
       } else {
          PartMode = PART_MODE_NPN;	// switch to parasitic transistor
       }
    }
//#endif

    if(PartMode == PART_MODE_NPN) {
       lcd_MEM_string(NPN_str);		//"NPN "
       if (ptrans.count != 0) {
          lcd_data('p');		// mark for parasitic PNp
       }
//       _trans = &ntrans;  is allready selected a default
#ifdef WITH_GRAPHICS
       lcd_pgm_bitmap(bmp_npn_data, 90, 32, 0);
       lcd_draw_trans_pins(90-6, 40);
#endif
    } else {
       lcd_MEM_string(PNP_str);		//"PNP "
       if (ntrans.count != 0) {
          lcd_data('n');		// mark for parasitic NPn
       }
       _trans = &ptrans;		// change transistor structure
#ifdef WITH_GRAPHICS
       lcd_pgm_bitmap(bmp_npn_data, 90, 32, 0);
       lcd_pgm_bitmap(bmp_pnp_data, 90+14, 32+16, 0);	// update for PNP
       lcd_draw_trans_pins(90-6, 40);
#endif
    }
    lcd_space();
//    if( NumOfDiodes > 2) 	//Transistor with protection diode
    for (ii=0; ii<NumOfDiodes; ii++) {
       if ((diodes.Anode[ii] == _trans->b) || (diodes.Cathode[ii] == _trans->b)) continue;
#ifdef EBC_STYLE
 #if EBC_STYLE == 321
       // Layout with 321= style
       if (((PartMode == PART_MODE_NPN) && (ntrans.c < ntrans.e)) || ((PartMode != PART_MODE_NPN) && (ptrans.c > ptrans.e)))
 #else
       // Layout with EBC= style
       if(PartMode == PART_MODE_NPN)
 #endif
#else
       // Layout with 123= style
       if (((PartMode == PART_MODE_NPN) && (ntrans.c > ntrans.e)) || ((PartMode != PART_MODE_NPN) && (ptrans.c < ptrans.e)))
#endif
       {
          lcd_MEM_string(AnKat);	//"->|-"
       } else {
          lcd_MEM_string(KatAn);	//"-|<-"
       }
    }
    PinLayout('E','B','C'); 		//  EBC= or 123=...
    lcd_line2(); //2. row 
#ifndef FOUR_LINE_LCD
 #ifdef SHOW_ICE
    if (_trans->ice0 > 0) {
       lcd_MEM2_string(ICE0_str);		// "ICE0="
       DisplayValue(_trans->ice0,-5,'A',3);
       wait_for_key_5s_line2();		// wait 5s and clear line 2
    }
    if (_trans->ices > 0) {
       lcd_MEM2_string(ICEs_str);		// "ICEs="
       DisplayValue(_trans->ices,-5,'A',3);
       wait_for_key_5s_line2();		// wait 5s and clear line 2
    }
 #endif
#endif
    lcd_MEM_string(hfe_str);		//"B="  (hFE)
    DisplayValue(_trans->hfe,0,0,3);
    lcd_space();

    lcd_MEM_string(Uf_str);		//"Uf="
    DisplayValue(_trans->uBE,-3,'V',3);
#ifdef FOUR_LINE_LCD
 #ifdef SHOW_ICE
    if (_trans->ice0 > 0) {
       lcd_line3(); //3. row 
       lcd_MEM2_string(ICE0_str);		// "ICE0="
       DisplayValue(_trans->ice0,-5,'A',3);
    }
    if (_trans->ices > 0) {
       lcd_line4(); //4. row 
       lcd_MEM2_string(ICEs_str);		// "ICEs="
       DisplayValue(_trans->ices,-5,'A',3);
    }
 #endif
#endif
    goto end;
    // end (PartFound == PART_TRANSISTOR)
  } else if (PartFound == PART_FET) {	//JFET or MOSFET
    unsigned char fetidx = 0;
    if((PartMode&P_CHANNEL) == P_CHANNEL) {
       lcd_data('P');			//P-channel
       _trans = &ptrans;
       fetidx += 1;
    } else {
       lcd_data('N');			//N-channel
//       _trans = &ntrans;	is allready selected as default
    }
    lcd_data('-');

    tmp = PartMode&0x0f;
    if (tmp == PART_MODE_JFET) {
       lcd_MEM_string(jfet_str);	//"JFET"
#ifdef WITH_GRAPHICS
//     lcd_draw_jfet(fetidx, 96, 32);
       lcd_pgm_bitmap(bmp_n_jfet_data, 96, 32, 0);
       if (fetidx != 0) {
          // update the n_jfet bitmat at relative location 6, 16
          lcd_pgm_bitmap(bmp_p_jfet_data, 96+6, 32+16, 0);
       }
       lcd_draw_trans_pins(96-6, 48);
#endif
    } else {
       if ((PartMode&D_MODE) == D_MODE) {
          lcd_data('D');			// N-D or P-D
          fetidx += 2;
       } else {
          lcd_data('E');			// N-E or P-E
       }
       if (tmp == (PART_MODE_IGBT)) {
          lcd_MEM_string(igbt_str);	//"-IGBT"
#ifdef WITH_GRAPHICS
          lcd_draw_igbt(fetidx);
          lcd_draw_trans_pins(82, 48);
#endif
       } else {
          lcd_MEM_string(mosfet_str);	//"-MOS "
#ifdef WITH_GRAPHICS
          lcd_draw_mosfet(fetidx);
          lcd_draw_trans_pins(82, 40);
#endif
       }
    }
    if (tmp == PART_MODE_IGBT) {
       PinLayout('E','G','C'); 		//  SGD= or 123=...
    } else {
       PinLayout('S','G','D'); 		//  SGD= or 123=...
    }
//    if((NumOfDiodes == 1) && ((PartMode&D_MODE) != D_MODE)) 
    an_cat = 0;
    if(NumOfDiodes == 1) {
       //MOSFET with protection diode; only with enhancement-FETs
#ifdef EBC_STYLE
 #if EBC_STYLE == 321
       // layout with 321= style
       an_cat = (((PartMode&P_CHANNEL) && (ptrans.c > ptrans.e)) || ((!(PartMode&P_CHANNEL)) && (ntrans.c < ntrans.e)));
 #else
       // Layout with SGD= style
       an_cat = (PartMode&P_CHANNEL);	/* N or P MOS */
 #endif
#else
       // layout with 123= style
       an_cat = (((PartMode&P_CHANNEL) && (ptrans.c < ptrans.e)) || ((!(PartMode&P_CHANNEL)) && (ntrans.c > ntrans.e)));
#endif
       //  show diode symbol in right direction 
       if (an_cat) {
          lcd_data(LCD_CHAR_DIODE1);	//show Diode symbol >|
       } else {
          lcd_data(LCD_CHAR_DIODE2);	//show Diode symbol |<
       }
#ifndef WITH_GRAPHICS
 #ifndef FOUR_LINE_LCD
  #if FLASHEND > 0x1fff
       // there is enough space for long form of showing protection diode
       lcd_line2();			//2. Row
       if (an_cat) {
          lcd_testpin(diodes.Anode[0]);
          lcd_MEM_string(AnKat);	//"->|-"
          lcd_testpin(diodes.Cathode[0]);
       } else {
          lcd_testpin(diodes.Cathode[0]);
          lcd_MEM_string(KatAn);	//"-|<-"
          lcd_testpin(diodes.Anode[0]);
       }
       lcd_space();
       lcd_MEM_string(Uf_str);			//"Uf="
       mVAusgabe(0);
       wait_for_key_5s_line2();		// wait 5s and clear line 2
  #endif
 #endif
#endif
    } /* end if NumOfDiodes == 1 */
    lcd_line2();			//2. Row
    if((PartMode&D_MODE) != D_MODE) {	//enhancement-MOSFET
	//Gate capacity
       lcd_MEM_string(GateCap_str);		//"C="
       ReadCapacity(_trans->b,_trans->e);	//measure capacity
       DisplayValue(cap.cval,cap.cpre,'F',3);
       lcd_MEM_string(vt_str);		// "Vt="
    } else {
       lcd_data('I');
       lcd_data('=');
       DisplayValue(_trans->current,-5,'A',2);
       lcd_MEM_string(Vgs_str);		// " Vg="
    }
    //Gate-threshold voltage
    DisplayValue(_trans->gthvoltage,-3,'V',2);
#ifdef FOUR_LINE_LCD
 #if FLASHEND > 0x1fff
       // there is enough space for long form of showing protection diode
    if(NumOfDiodes == 1) {
  #ifdef WITH_GRAPHICS
       unsigned char options = 0;

       if (_trans->c != diodes.Anode[0])
          options |= OPT_VREVERSE;
       lcd_pgm_bitmap(bmp_vakdiode_data, 114, 32, options);
       lcd_line3();			//3. Row
  #else
       lcd_line3();			//3. Row
       if (an_cat) {
          lcd_testpin(diodes.Anode[0]);
          lcd_MEM_string(AnKat);	//"->|-"
          lcd_testpin(diodes.Cathode[0]);
       } else {
          lcd_testpin(diodes.Cathode[0]);
          lcd_MEM_string(KatAn);	//"-|<-"
          lcd_testpin(diodes.Anode[0]);
       }
       lcd_space();
  #endif
       lcd_MEM_string(Uf_str);			//"Uf="
       mVAusgabe(0);
    }
 #endif
#endif
    goto end;
    // end (PartFound == PART_FET)
  }
//   if(PartFound == PART_RESISTOR) 
resistor_out:
   if(ResistorsFound != 0) {
    ii = 0;
    if (ResistorsFound == 1) { // single resistor
       lcd_testpin(resis[0].rb);  	//Pin-number 1
       lcd_MEM_string(Resistor_str);
       lcd_testpin(resis[0].ra);		//Pin-number 2
    } else { // R-Max suchen
       if (resis[1].rx > resis[0].rx)
          ii = 1;
       if (ResistorsFound == 2) {
          ii = 2;
       } else {
          if (resis[2].rx > resis[ii].rx) {
             ii = 2;
          }
       }
       char x = '1';
       char y = '3';
       char z = '2';
   
       if (ii == 1) {
          // x = '1';
          y = '2';
          z = '3';
       }
       if (ii == 2) {
          x = '2';
          y = '1';
          z = '3';
       }
       lcd_data(x);
       lcd_MEM_string(Resistor_str);    // -[=]-
       lcd_data(y);
       lcd_MEM_string(Resistor_str);    // -[=]-
       lcd_data(z);
    }
#ifdef FOUR_LINE_LCD
    if(PartFound == PART_DIODE) {
       lcd_line4(); //4. row 
    } else {
       lcd_line2(); //2. row 
    }
#else
    lcd_line2(); //2. row 
#endif
    if (ResistorsFound == 1) {
#if FLASHEND > 0x1fff
       ReadInductance();		// measure inductance, possible only with single R<2.1k
       RvalOut(0);
       if (inductor_lpre != 0) {
	  // resistor have also Inductance
          lcd_MEM_string(Lis_str);	// "L="
          DisplayValue(inductor_lx,inductor_lpre,'H',3);	// output inductance
 #ifdef WITH_GRAPHICS
          lcd_pgm_bitmap(bmp_inductor_data, 103, 56, 0);
          lcd_draw_pin(resis[0].rb, 95, 56);
          lcd_draw_pin(resis[0].ra, 120, 56);
 #endif
       }
#else
       RvalOut(0);
#endif
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

  lcd_MEM_string(TestFailed1); 	//"Kein,unbek. oder"
  lcd_line2(); //2. row 
#if defined(LANG_ITALIAN)               //italiano
  lcd_MEM_string(Bauteil);		//"campione"
  lcd_space();
  lcd_MEM_string(TestFailed2); 		//"guasto "
#else
  lcd_MEM_string(TestFailed2); 		//"defektes "
  lcd_MEM_string(Bauteil);		//"Bauteil"
#endif
not_known:
#if POWER_OFF+0 > 1
  empty_count++;
  mess_count = 0;
#endif
  max_time = SHORT_WAIT_TIME;		// use allways the short wait time
  goto end2;


//gakAusgabe:
//  PinLayout(Cathode_char,'G','A'); 	// CGA= or 123=...
TyUfAusgabe:
#ifdef WITH_THYRISTOR_GATE_V
  lcd_line2(); //2. row 
  lcd_MEM_string(Uf_str);		// "Uf="
  DisplayValue(ntrans.uBE,-3,'V',2);
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 end:
#if POWER_OFF+0 > 1
  empty_count = 0;		// reset counter, if part is found
  mess_count++;			// count measurements
#endif
  max_time = display_time;	// full specified wait time

 end2:
  ADC_DDR = (1<<TPREF) | TXD_MSK; 	// switch pin with reference to GND, release relay
  while(!(RST_PIN_REG & (1<<RST_PIN)));	//wait ,until button is released
#ifdef WITH_ROTARY_SWITCH
wait_again:
#endif
  ii = wait_for_key_ms(max_time);
#if POWER_OFF+0 > 1
 #ifdef WITH_ROTARY_SWITCH
  if ((ii > 0) || (rotary.incre > 0))
 #else
  if (ii > 0)
 #endif
  {
     empty_count = 0;		// reset counter, if any switch activity
     mess_count = 0;
  }
#endif
#ifdef WITH_MENU
 #ifdef WITH_ROTARY_SWITCH
  if ((ii >=50) || (rotary.incre > 2))
 #else
  if (ii >= 50) 
 #endif
  {
     // menu selected by long key press or rotary switch
     function_menu();		// start the function menu
     goto start;
  }
#endif
  if (ii != 0 ) goto start;	// key is pressed again, repeat measurement
#ifdef WITH_ROTARY_SWITCH
  if (rotary.incre > 0) goto wait_again;
#endif
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
 #if FLASHEND > 0x3fff
  // look, if the tester is uncalibrated (C-source will be included directly)
  lcd_cursor_off();
  #include "HelpCalibration.c"
 #endif
//  MCUSR = 0;
 #if (LCD_ST_TYPE == 7565)
  lcd_powersave();			// set graphical display to power save mode
 #endif
  ON_PORT &= ~(1<<ON_PIN);		//switch off power
  wait_for_key_ms(0); //never ending loop 
#else
  // look, if the tester is uncalibrated (C-source will be included directly)
  lcd_cursor_off();
  #include "HelpCalibration.c"
#endif
  goto start;	// POWER_OFF not selected, repeat measurement
//  return 0;

end3:
  // the diode  is already shown on the LCD
  if (ResistorsFound == 0) goto end;
  ADC_DDR = (1<<TPREF) | TXD_MSK; 	// switch pin with reference to GND, release relay
  // there is one resistor or more detected
#ifdef FOUR_LINE_LCD
  ADC_DDR =  TXD_MSK; 	// switch pin with reference to input, activate relay
  lcd_line3();
#else
  wait_for_key_ms(display_time);
  ADC_DDR =  TXD_MSK; 	// switch pin with reference to input, activate relay
  lcd_clear();
//#if FLASHEND > 0x1fff
  lcd_data('0'+NumOfDiodes);
  lcd_MEM_string(Dioden);	//"Diodes "
//#endif
#endif
  goto resistor_out;

}   // end main


#ifdef WITH_SELFTEST
 #include "AutoCheck.c"
#endif
#ifdef AUTO_CAL
 #include "mark_as_uncalibrated.c"
#endif
#if FLASHEND > 0x1fff
 #include "GetIr.c"
#endif
