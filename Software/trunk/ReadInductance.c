// new code by K.-H. Kübbeler
// The 680 Ohm resistor (R_L_VAL) at the Lowpin will be used as current sensor
// The current with a coil will with (1 - e**(-t*R/L)), where R is
// the sum of Pin_RM , R_L_VAL , Resistance of coil and Pin_RP.
// L in the inductance of the coil.
#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
void ReadInductance(void) {
#if FLASHEND > 0x1fff
  // check if inductor and measure the inductance value
  unsigned int tmpint;
  unsigned int umax;
  unsigned int total_r;		// total resistance of current loop
  unsigned int mess_r;		// value of resistor used for current measurement
  unsigned long inductance[4];	// four inductance values for different measurements
  union t_combi{
  unsigned long dw;     // time_constant
  uint16_t w[2];
  } timeconstant;
  uint16_t per_ref1,per_ref2;	// percentage
  uint8_t LoPinR_L;	// Mask for switching R_L resistor of low pin
  uint8_t HiADC;	// Mask for switching the high pin direct to VCC
  uint8_t ii;
  uint8_t count;	// counter for the different measurements
  uint8_t cnt_diff;     // resistance dependent offset
  uint8_t LowPin;	// number of pin with low voltage
  uint8_t HighPin;	// number of pin with high voltage 
  int8_t ukorr;		// correction of comparator voltage
  uint8_t nr_pol1;	// number of successfull inductance measurement with polarity 1
  uint8_t nr_pol2;	// number of successfull inductance measurement with polarity 2


  inductor_lpre = 0;	// H units, mark inductor as 0
  if(PartFound != PART_RESISTOR) {
     return;	//We have found no resistor  
  }
  if (ResistorsFound != 1) {
     return;	// do not search for inductance, more than 1 resistor
  }
     if (resis[0].rx > 21000) return;

     // we can check for Inductance, if resistance is below 2100 Ohm
     for (count=0;count<4;count++) {
        // Try four times (different direction and with delayed counter start)
        if (count < 2) {
           // first and second pass, direction 1
           LowPin = resis[0].ra;
           HighPin = resis[0].rb;
        } else {
           // third and fourth pass, direction 2
           LowPin = resis[0].rb;
           HighPin = resis[0].ra;
        }
        HiADC = pgm_read_byte(&PinADCtab[HighPin]);	// Table of ADC Pins including | TXD_VAL
        LoPinR_L = pgm_read_byte(&PinRLtab[LowPin]);	//R_L mask for HighPin R_L load
        //==================================================================================
        // Measurement of Inductance values
        R_PORT = 0;		// switch R port to GND
        ADC_PORT =   TXD_VAL;		// switch ADC-Port to GND
        if ((resis[0].rx < 240) && ((count & 0x01) == 0)) {
           // we can use PinR_L for measurement
           mess_r = RR680MI - R_L_VAL;			// use only pin output resistance
           ADC_DDR = HiADC | (1<<LowPin) | TXD_MSK;	// switch HiADC and Low Pin to GND, 
        } else {
           R_DDR = LoPinR_L;   		// switch R_L resistor port for LowPin to output (GND)
           ADC_DDR = HiADC | TXD_MSK;	// switch HiADC Pin to GND 
           mess_r = RR680MI;			// use 680 Ohm and PinR_L for current measurement
        }
        // Look, if we can detect any current
        for (ii=0;ii<20;ii++) {
            // wait for current is near zero
            umax = W10msReadADC(LowPin);
            total_r =  ReadADC(HighPin);
            if ((umax < 2) && (total_r < 2)) break;	// low current detected
        }
        // setup Analog Comparator
        ADC_COMP_CONTROL = (1<<ACME);			//enable Analog Comparator Multiplexer
        ACSR =  (1<<ACBG) | (1<<ACI)  | (1<<ACIC);	// enable, 1.3V, no Interrupt, Connect to Timer1 
        ADMUX = (1<<REFS0) | LowPin;			// switch Mux to Low-Pin
        ADCSRA = (1<<ADIF) | AUTO_CLOCK_DIV; //disable ADC
   
      // setup Counter1
        timeconstant.w[1] = 0;		// set ov counter to 0
        TCCR1A = 0;			// set Counter1 to normal Mode
        TCNT1 = 0;			//set Counter to 0
        TI1_INT_FLAGS = (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);	// reset TIFR or TIFR1
//        HiADC |= TXD_VAL;
        wait200us();			// wait for bandgap to start up
        if ((count & 0x01) == 0 ) {
           //first start counter, then start current
           TCCR1B =  (1<<ICNC1) | (0<<ICES1) | (1<<CS10);	//start counter 1MHz or 8MHz
           ADC_PORT = HiADC;		// switch ADC-Port to VCC
        } else {
           //first start current, then start counter with delay
           //parasitic capacity of coil can cause high current at the beginning
           ADC_PORT = HiADC;		// switch ADC-Port to VCC
      #if F_CPU >= 8000000UL
           wait3us();		// ignore current peak from capacity
      #else
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
              timeconstant.w[1]++;		// count one OV
              if(timeconstant.w[1] == (F_CPU/100000UL)) {
                 break; 	//Timeout for Charging, above 0.13 s
              }
           }
        }
        TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<CS10);  // stop counter
        TI1_INT_FLAGS = (1<<ICF1);			// Reset Input Capture
        timeconstant.w[0] = ICR1;		// get previous Input Capture Counter flag
      // check actual counter, if an additional overflow must be added
        if((TCNT1 > timeconstant.w[0]) && (ii & (1<<TOV1))) {
           // this OV was not counted, but was before the Input Capture
           TI1_INT_FLAGS = (1<<TOV1);		// Reset OV Flag
           timeconstant.w[1]++;			// count one additional OV
        }

        ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
        ADCSRA = (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; //enable ADC
        for (ii=0;ii<20;ii++) {
            // wait for current is near zero
            umax = W10msReadADC(LowPin);
            total_r =  ReadADC(HighPin);
            if ((umax < 2) && (total_r < 2)) break;	// low current detected
        }
  #define CNT_ZERO_42 6
  #define CNT_ZERO_720 7
//#if F_CPU == 16000000UL
//  #undef CNT_ZERO_42
//  #undef CNT_ZERO_720
//  #define CNT_ZERO_42 7
//  #define CNT_ZERO_720 10
//#endif
        total_r = (mess_r + resis[0].rx + RRpinMI);
//        cnt_diff = 0;
//        if (total_r > 7000) cnt_diff = 1;
//        if (total_r > 14000) cnt_diff = 2;
        cnt_diff = total_r / ((14000UL * 8) / (F_CPU/1000000UL));
#if 0
        // Voltage of comparator in % of umax
     #ifdef AUTO_CAL
        tmpint = (ref_mv + (int16_t)eeprom_read_word((uint16_t *)(&ref_offset))) ;
     #else
        tmpint = (ref_mv + REF_C_KORR);
     #endif
#endif
        tmpint = ref_mv_offs;
        if (mess_r < R_L_VAL) {
           // measurement without 680 Ohm
           cnt_diff = CNT_ZERO_42;
           if (timeconstant.dw < 225) {
              ukorr = (timeconstant.w[0] / 5) - 20;
           } else {
              ukorr = 25;
           }
           tmpint -= (((REF_L_KORR * 10) / 10) + ukorr);
        } else {
           // measurement with 680 Ohm resistor
           // if 680 Ohm resistor is used, use REF_L_KORR for correction
           cnt_diff += CNT_ZERO_720;
           tmpint += REF_L_KORR;
        }
        if (timeconstant.dw > cnt_diff) timeconstant.dw -= cnt_diff;
        else          timeconstant.dw = 0;
       
        if ((count&0x01) == 1) {
           // second pass with delayed counter start
           timeconstant.dw += (3 * (F_CPU/1000000UL))+10;
        }
        if (timeconstant.w[1] >= (F_CPU/100000UL)) timeconstant.dw = 0; // no transition found
        if (timeconstant.dw > 10) {
           timeconstant.dw -= 1;
        }
        // compute the maximum Voltage umax with the Resistor of the coil
        umax = ((unsigned long)mess_r * (unsigned long)ADCconfig.U_AVCC) / total_r;
        per_ref1 = ((unsigned long)tmpint * 1000) / umax;
//        per_ref2 = (uint8_t)MEM2_read_byte(&LogTab[per_ref1]);	// -log(1 - per_ref1/100)
        per_ref2 = get_log(per_ref1);		// -log(1 - per_ref1/1000)
/* ********************************************************* */
#if 0
          if (count == 0) {
             lcd_line3();
             DisplayValue(count,0,' ',4);
             DisplayValue(timeconstant.dw,0,'+',4);
             DisplayValue(cnt_diff,0,' ',4);
             DisplayValue(total_r,-1,'r',4);
             lcd_space();
             DisplayValue(per_ref1,-1,'%',4);
             lcd_line4();
             DisplayValue(tmpint,-3,'V',4);
             lcd_space();
             DisplayValue(umax,-3,'V',4);
             lcd_space();
             DisplayValue(per_ref2,-1,'%',4);
             wait_about4s();
             wait_about2s();
          }
#endif
/* ********************************************************* */
        // inductor_lx in 0.01mH units,  L = Tau * R
        per_ref1 = ((per_ref2 * (F_CPU/1000000UL)) + 5) / 10;
        inductance[count] = (timeconstant.dw * total_r ) / per_ref1;
        if (((count&0x01) == 0) && (timeconstant.dw > ((F_CPU/1000000UL)+3))) {
           // transition is found, measurement with delayed counter start is not necessary
           inductance[count+1] = inductance[count];	// set delayed measurement to same value
           count++;		// skip the delayed measurement
        }
        wdt_reset();
     }  //end for count
     ADC_PORT = TXD_VAL;		// switch ADC Port to GND
     wait_about20ms();
#if 0
     if (inductance[1] > inductance[0]) {
        inductor_lx = inductance[1];	// use value found with delayed counter start
     } else {
        inductor_.lx = inductance[0];
     }
     if (inductance[3] > inductance[2]) inductance[2] = inductance[3];		// other polarity, delayed start
     if (inductance[2] < inductor_lx) inductor_lx = inductance[2];	// use the other polarity
#else
     nr_pol1 = 0;
     if (inductance[1] > inductance[0]) { nr_pol1 = 1; } 
     nr_pol2 = 2;
     if (inductance[3] > inductance[2]) { nr_pol2 = 3; } 
     if (inductance[nr_pol2] < inductance[nr_pol1]) nr_pol1 = nr_pol2;
     inductor_lx = inductance[nr_pol1];
     inductor_lpre = -5;	// 10 uH units
     if (((nr_pol1 & 1) == 1) || (resis[0].rx >= 240)) {
        // with 680 Ohm resistor total_r is more than 7460
        inductor_lpre = -4;	// 100 uH units
        inductor_lx = (inductor_lx + 5) / 10;
     } 
     if (inductor_lx == 0) inductor_lpre = 0;	//mark as zero
#endif

  // switch all ports to input
  ADC_DDR =  TXD_MSK;		// switch all ADC ports to input
  R_DDR = 0;			// switch all resistor ports to input
#endif
  return;
 } // end ReadInductance()


#if 0
#if FLASHEND > 0x1fff
// get_log interpolate a table with the function -log(1 - (permil/1000))
uint16_t get_log(uint16_t permil) {
// for remember:
// uint16_t LogTab[] PROGMEM = {0, 20, 41, 62, 83, 105, 128, 151, 174, 198, 223, 248, 274, 301, 329, 357, 386, 416, 446, 478, 511, 545, 580, 616, 654, 693, 734, 777, 821, 868, 916, 968, 1022, 1079, 1139, 1204, 1273, 1347, 1427, 1514, 1609, 1715, 1833, 1966, 2120, 2303, 2526 };


#define Log_Tab_Distance 20              // displacement of table is 20 mil

  uint16_t y1, y2;			// table values
  uint16_t result;			// result of interpolation
  uint8_t tabind;			// index to table value
  uint8_t tabres;			// distance to lower table value, fraction of Log_Tab_Distance

  tabind = permil / Log_Tab_Distance;	// index to table
  tabres = permil % Log_Tab_Distance;	// fraction of table distance
  // interpolate the table of factors
  y1 = pgm_read_word(&LogTab[tabind]);	// get the lower table value
  y2 = pgm_read_word(&LogTab[tabind+1]); // get the higher table value
  result = ((y2 - y1) * tabres ) / Log_Tab_Distance + y1; // interpolate
  return(result);
 }
#endif
#endif
