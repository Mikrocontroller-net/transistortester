// new code by K.-H. Kübbeler
// The 680 Ohm resistor (R_L_VAL) at the Lowpin will be used as current sensor
// The current with a coil will with (1 - e**(-t*R/L)), where R is
// the sum of Pin_RM , R_L_VAL , Resistance of coil and Pin_RP.
// L in the inductance of the coil.
#include <avr/io.h>
#include <stdlib.h>
#include "config.h"
#include "Transistortester.h"

unsigned long CombineII2Long( unsigned int ovcnt16, unsigned int tmpcnt); //tricky function to build unsigned long from two unsigned int values


//=================================================================
void ReadInductance(void) {
#if FLASHEND > 0x1fff
  // check if inductor and measure the inductance value
  unsigned int tmpint;
  unsigned int ovcnt16;	// overrun counter for 16 bit counter
  unsigned int umax;
  unsigned int total_r;		// total resistance of current loop
  unsigned int mess_r;		// value of resistor used for current measurement
  unsigned long inductance[4];	// four inductance values for different measurements
  uint8_t per_ref;	// percentage
  uint8_t LoPinR_L;	// Mask for switching R_L resistor of low pin
  uint8_t HiADC;	// Mask for switching the high pin direct to VCC
  uint8_t ii;
  uint8_t count;	// counter for the different measurements
  uint8_t found;	// variable used for searching resistors 
  uint8_t LowPin;	// number of pin with low voltage
  uint8_t HighPin;	// number of pin with high voltage 


  if(PartFound != PART_RESISTOR) {
     return;	//We have found no resistor  
  }
  for (found=0;found<ResistorsFound;found++) {
     if (resis[found].rx > 21000) continue;
     // we can check for Inductance, if resistance is below 2800 Ohm
     for (count=0;count<4;count++) {
        // Try four times (different direction and with delayed counter start)
        if (count < 2) {
           // first and second pass, direction 1
           LowPin = resis[found].ra;
           HighPin = resis[found].rb;
        } else {
           // third and fourth pass, direction 2
           LowPin = resis[found].rb;
           HighPin = resis[found].ra;
        }
        HiADC = MEM_read_byte(&PinADCtab[HighPin]);
        LoPinR_L = MEM_read_byte(&PinRLtab[LowPin]);	//R_L mask for HighPin R_L load
        //==================================================================================
        // Measurement of Inductance values
        R_PORT = 0;		// switch R port to GND
        ADC_PORT =   TXD_VAL;		// switch ADC-Port to GND
        if ((resis[found].rx < 80) && ((count & 0x01) == 0)) {
           // we can use PinR_L for measurement
           mess_r = RR680MI - R_L_VAL;
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
        ovcnt16 = 0;
        TCCR1A = 0;			// set Counter1 to normal Mode
        TCNT1 = 0;			//set Counter to 0
        TI1_INT_FLAGS = (1<<ICF1) | (1<<OCF1B) | (1<<OCF1A) | (1<<TOV1);	// mega88
        HiADC |= TXD_VAL;
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
              ovcnt16++;
              if(ovcnt16 == (F_CPU/100000)) {
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
        ADC_PORT = TXD_VAL;		// switch ADC-Port to GND
        ADCSRA = (1<<ADEN) | (1<<ADIF) | AUTO_CLOCK_DIV; //enable ADC
        for (ii=0;ii<20;ii++) {
            // wait for current is near zero
            umax = W10msReadADC(LowPin);
            total_r =  ReadADC(HighPin);
            if ((umax < 2) && (total_r < 2)) break;	// low current detected
        }
        cap.cval = CombineII2Long(ovcnt16, tmpint);
  #define CNT_ZERO_42 6
  #define CNT_ZERO_720 8
#if F_CPU == 16000000UL
  #undef CNT_ZERO_42
  #undef CNT_ZERO_720
  #define CNT_ZERO_42 4
  #define CNT_ZERO_720 10
#endif
        if (mess_r < R_L_VAL) {
           if (cap.cval > CNT_ZERO_42) cap.cval -= CNT_ZERO_42;
           else          cap.cval = 0;
        } else {
           if (cap.cval > CNT_ZERO_720) cap.cval -= CNT_ZERO_720;
           else          cap.cval = 0;
        }
        if ((count&0x01) == 1) {
           // second pass with delayed counter start
           cap.cval += (3 * (F_CPU/1000000))+10;
        }
        if (ovcnt16 >= (F_CPU/100000)) cap.cval = 0; // no transition found
        total_r = (mess_r + resis[0].rx + RR680PL - R_L_VAL);
        // compute the maximum Voltage umax with the Resistor of the coil
        umax = ((unsigned long)mess_r * (unsigned long)ADCconfig.U_AVCC) / total_r;
        // Voltage of comparator in % of umax
        #ifdef AUTO_CAL
        per_ref = ((unsigned long)(ref_mv + (int16_t)eeprom_read_word((uint16_t *)(&ref_offset))) * 100) / umax;
        #else
        per_ref = ((unsigned long)(ref_mv + REF_C_KORR) * 100) / umax;
        #endif
        per_ref = (uint8_t)MEM2_read_byte(&LogTab[per_ref]);	// -log(1 - per_ref/100)
        // lx in 0.01mH units,  L = Tau * R
        inductance[count] = (cap.cval * total_r ) / ((unsigned int)per_ref * (F_CPU/1000000));
        if (((count&0x01) == 0) && (inductance[count] > (F_CPU/1000000))) {
           // transition is found, measurement with delayed counter start is not necessary
           inductance[count+1] = inductance[count];	// set delayed measurement to same value
           count++;		// skip the delayed measurement
        }
        wdt_reset();
     }  //end for count
     ADC_PORT = TXD_VAL;		// switch ADC Port to GND
     wait20ms();
     if (inductance[1] > inductance[0]) {
        resis[found].lx = inductance[1];	// use value found with delayed counter start
     } else {
        resis[found].lx = inductance[0];
     }
     if (inductance[3] > inductance[2]) inductance[2] = inductance[3];
     if (inductance[2] < resis[found].lx) resis[found].lx = inductance[2];	// use the other polarity
  } // end loop for all resistors

  // switch all ports to input
  ADC_DDR =  TXD_MSK;		// switch all ADC ports to input
  R_DDR = 0;			// switch all resistor ports to input
#endif
  return;
 } // end ReadInductance()
