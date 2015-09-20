
	#include "Transistortester.h"

//	  Calibrate_UR();		// get Ref Voltages and Pin resistance
//        lcd_line1();			// Cursor to 1. row, column 1
	  
#ifdef BAT_CHECK
void Battery_check(void) {
 uint16_t bat_voltage;
 uint16_t bat_adc;
  // Battery check is selected
  ReadADC(TPBAT);	//Dummy-Readout
  bat_adc = W5msReadADC(TPBAT); 	//with 5V reference
 #ifdef BAT_OUT
  // display Battery voltage
  // The divisor to get the voltage in 0.01V units is ((10*33)/133) witch is about 2.4812
  // A good result can be get with multiply by 4 and divide by 10 (about 0.75%).
  #if BAT_NUMERATOR <= (0xffff/U_VCC)
	  bat_voltage = (bat_adc*BAT_NUMERATOR)/BAT_DENOMINATOR + BAT_OUT;
  #else
   #if (BAT_NUMERATOR == 133) && (BAT_DENOMINATOR == 33)
	  bat_voltage = (bat_adc*4)+BAT_OUT;		// usually output only 2 digits
   #else
	  bat_voltage = ((unsigned long)bat_adc*BAT_NUMERATOR)/BAT_DENOMINATOR + BAT_OUT;
   #endif
  #endif
  #if FLASHEND > 0x1fff
          DC_Pwr_mode = 0;
   #ifdef DC_PWR
          if ((bat_voltage < 900) || (bat_voltage > DC_PWR))
   #else
          if (bat_voltage < 900) 
   #endif
          {
             // no battery present, don't check,
	     lcd_MEM_string(DC_Pwr_Mode_str);	// "DC Pwr Mode"
             lcd_clear_line();			// clear to end of line
             DC_Pwr_mode = 1;
             return;
          }
  #endif
          lcd_MEM_string(Bat_str);		//output: "Bat. "
	  Display_mV(bat_voltage,2);		// Display 2 Digits of this 10mV units
          lcd_space();
 #else    /* without battery voltage output */
    lcd_MEM_string(Bat_str);		//output: "Bat. "
 #endif  /* BAT_OUT */
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
  if (bat_adc <  WARN_LEVEL) {
     //Vcc < 7,3V; show Warning 
     if(bat_adc < POOR_LEVEL) {	
	//Vcc <6,3V; no proper operation is possible
	lcd_MEM_string(BatEmpty);	//Battery empty!
        lcd_clear_line();			// clear to end of line
	lcd_refresh();			// write the pixels to display, ST7920 only
	wait_about5s();
        ON_PORT &= ~(1<<ON_PIN);	// switch power off
	wait_about2s();
	return;
     }
     lcd_MEM_string(BatWeak);		//Battery weak
  } else { // Battery-voltage OK
     lcd_MEM_string(OK_str); 		// "OK"
  }
  lcd_clear_line();			// clear to end of line
};
#endif	/* BAT_CHECK */
