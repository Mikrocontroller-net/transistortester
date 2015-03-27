
	#include "Transistortester.h"

//	  Calibrate_UR();		// get Ref Voltages and Pin resistance
//        lcd_line1();			// Cursor to 1. row, column 1
	  
#ifdef BAT_CHECK
void Battery_check(void) {
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
	lcd_refresh();			// write the pixels to display, ST7920 only
	wait_about2s();
	PORTD = 0;			//switch power off
	wait_about2s();
	return;
     }
     lcd_MEM_string(BatWeak);		//Battery weak
  } else { // Battery-voltage OK
     lcd_MEM_string(OK_str); 		// "OK"
  }
};
#endif	/* BAT_CHECK */
