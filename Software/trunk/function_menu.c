// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// selection of different functions

#define MAX_FUNC 3
#ifdef WITH_MENU
void function_menu() {
  uint8_t ii;
  uint8_t ll;
  uint8_t func_number;

  func_number = 0xff;
  for (ll=0;ll<((MAX_FUNC+1)*20);ll++) {
     func_number++;
     if (func_number > MAX_FUNC) func_number = 0;
     message_key_released(SELECTION_str);
     lcd_line2();
     if (func_number == 2) {
 #ifdef WITH_VEXT
        lcd_MEM_string(VOLTAGE_str);
 #else
        continue;
 #endif
     }
     if (func_number == 3) {
 #ifdef POWER_OFF
        lcd_MEM_string(OFF_str);
 #else
        continue;
 #endif
     }
     if (func_number == 0) lcd_MEM_string(TESTER_str);
     if (func_number == 1) lcd_MEM_string(FREQ_str);
     ii = wait_for_key_ms(SHORT_WAIT_TIME);
     if (ii >= 30) {
        if (func_number == 0) break;		// return to TransistorTester
        if (func_number == 1) GetFrequency();
        if (func_number == 2) show_vext();
        if (func_number == 3) {
           ON_PORT &= ~(1<<ON_PIN);              //switch off power
           wait_for_key_ms(0); //never ending loop 
        }
        // don't increase function number for easier selection the same function
        func_number--;	
     } /* end if (ii >= 30) */
  } /* end for ll */
  return;
 } // end function_menu()

void show_vext() {
 #ifdef WITH_VEXT
 
  uint8_t key_pressed;
  uint8_t times;
  unsigned int Vext;
  // show the external voltage
  message_key_released(VOLTAGE_str);
  for (times=0;times<250;times++) {
     lcd_line2();
     lcd_clear_line();
     lcd_line2();
     lcd_MEM_string(Vext_str);          // Vext=
     ADC_DDR = 0;               //deactivate Software-UART
     Vext = W5msReadADC(TPext); // read external voltage 
//     ADC_DDR = TXD_MSK;               //activate Software-UART 
//#ifdef WITH_UART
//     uart_newline();          // start of new measurement
//#endif
  #if EXT_NUMERATOR <= (0xffff/U_VCC)
     DisplayValue(Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3); // Display 3 Digits of this mV units
  #else
     DisplayValue((unsigned long)Vext*EXT_NUMERATOR/EXT_DENOMINATOR,-3,'V',3);  // Display 3 Digits of this mV units
  #endif
     key_pressed = wait_for_key_ms(800);
     if (key_pressed != 0) break;
  }  /* end for times */
 #endif  /* WITH_VEXT */
} /* end show_vext() */

#endif  /* WITH_MENU */
