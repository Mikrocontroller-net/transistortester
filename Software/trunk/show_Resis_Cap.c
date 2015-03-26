// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


#if FLASHEND > 0x3fff
//=================================================================
// selection of different functions

/* ****************************************************************** */
/* show_Resis13 measures the resistance of a part connected to TP1 and TP3 */
/* if RMETER_WITH_L is configured, inductance is also measured */
/* ****************************************************************** */
void show_Resis13(void) {
  uint8_t key_pressed;
  message_key_released(RESIS_13_str);	// "1-|=|-3 .."
#ifdef RMETER_WITH_L
  lcd_set_cursor(0,LCD_LINE_LENGTH-4);
  lcd_MEM2_string(RLMETER_13_str);	// "[RL]" at the end of line 1
#else
  lcd_set_cursor(0,LCD_LINE_LENGTH-3);
  lcd_MEM2_string(RMETER_13_str);	// "[R]" at the end of line 1
#endif
#ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<250;times++) 
#else
  while (1)		/* wait endless without the POWER_OFF option */
#endif
  {
        PartFound = PART_NONE;
        ResistorsFound = 0;
        ResistorChecked[1] = 0;
        GetResistance(TP3, TP1);
        GetResistance(TP1, TP3);
        lcd_line2();		// clear old Resistance value 
        if (ResistorsFound != 0) {
#ifdef RMETER_WITH_L
	   ReadInductance();	// measure inductance, possible only with R<2.1k
           RvalOut(1);			// show Resistance, probably ESR
           if (inductor_lpre != 0) {
	      // resistor has also inductance
              lcd_MEM_string(Lis_str);		// "L="
              DisplayValue(inductor_lx,inductor_lpre,'H',3);        // output inductance
              lcd_set_cursor(0,5);
              lcd_MEM_string(Inductor_str);	// -ww-
              lcd_testpin(TP3);
           } else {
              lcd_spaces(14);		// clear old L=
              lcd_set_cursor(0,5);
              lcd_testpin(TP3);
              lcd_spaces(4);		// clear ww-3
           }
#else		/* without Inductance measurement, only show resistance */
           inductor_lpre = -1;		// prevent ESR measurement because Inductance is not tested
           RvalOut(1);			// show Resistance, no ESR
#endif
        } else {		/* no resistor found */
           lcd_data('?');		// too big
           lcd_spaces(19);
#ifdef RMETER_WITH_L
           lcd_set_cursor(0,5);
           lcd_testpin(TP3);
           lcd_spaces(4);		// clear ww-3
#endif
        }
#if defined(POWER_OFF) && defined(BAT_CHECK)
     Bat_update(times);
#endif
     key_pressed = wait_for_key_ms(1000);
#ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 3)) break;
#else
     if (key_pressed != 0) break;
#endif
  }  /* end for times */
  lcd_clear();
} /* end show_Resis13() */

/* ****************************************************************** */
/* show_Cap13 measures the capacity of a part connected to TP1 and TP3 */
/* ****************************************************************** */
#if (LCD_LINES > 2)
 #define SCREEN_TIME 1000
#else
 #define SCREEN_TIME 2000	/* line 2 is multi use, wait longer to read */
#endif
void show_Cap13(void) {
  uint8_t key_pressed;
  message_key_released(CAP_13_str);	// 1-||-3 at the beginning of line 1
  lcd_set_cursor(0,LCD_LINE_LENGTH-3);
  lcd_MEM2_string(CMETER_13_str);	// "[C]" at the end of line 1
#ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<250;times++) 
#else
  while (1)		/* wait endless without the POWER_OFF option */
#endif
  {
     PartFound = PART_NONE;
     NumOfDiodes = 0;
     cap.cval_max = 0;		// clear cval_max for update of vloss
     cap.cpre_max = -12;	// set to pF unit
     cap.v_loss = 0;		// clear vloss  for low capacity values (<25pF)!
     ReadCapacity(TP3, TP1);
     lcd_line2();		// overwrite old Capacity value 
     if (cap.cpre < 0) {
        // a cap is detected
        DisplayValue(cap.cval,cap.cpre,'F',4);	// display capacity
        lcd_spaces(2);
        if (cap.cpre == -12) lcd_spaces(2);  // no decimal point
        PartFound = PART_CAPACITOR;	// GetESR should check the Capacity value
        cap.esr = GetESR(TP3,TP1);
        if ( cap.esr < 65530) {
           // ESR is measured
           lcd_set_cursor(1 * PAGES_PER_LINE, 8);	// position behind the capacity
           lcd_MEM_string(&ESR_str[1]);		// show also "ESR="
           DisplayValue(cap.esr,-2,LCD_CHAR_OMEGA,2); // and ESR value
           lcd_set_cursor(0,4);
           lcd_MEM2_string(Resistor_str);   // "-[=]- .."
           lcd_testpin(TP3);		// add the TP3
        } else {		// no ESR known
           lcd_set_cursor(1 * PAGES_PER_LINE, 8);	// position behind the capacity
           lcd_spaces(10);		// clear ESR text and value
           lcd_set_cursor(0,4);		// clear ESR resistor
           lcd_testpin(TP3);		// write the TP3
           lcd_spaces(5);			// overwrite ESR resistor symbol
        }
        GetVloss();                        // get Voltage loss of capacitor
 #if (LCD_LINES > 2)
        lcd_line3();
        if (cap.v_loss != 0) {
           lcd_MEM_string(&VLOSS_str[1]);  // "Vloss="
           DisplayValue(cap.v_loss,-1,'%',2);
           lcd_spaces(4);
        } else {
           lcd_clear_line();
        }
 #else
        if (cap.v_loss != 0) {
           key_pressed = wait_for_key_ms(SCREEN_TIME);
  #ifdef WITH_ROTARY_SWITCH
//           if ((key_pressed != 0) || (rotary.incre > 3)) break;
  #else
//           if (key_pressed != 0) break;
  #endif
	   lcd_line2();
           lcd_clear_line();
	   lcd_line2();
           lcd_MEM_string(&VLOSS_str[1]);  // "Vloss="
           DisplayValue(cap.v_loss,-1,'%',2);
        }
 #endif
     } else { /* no cap detected */
       lcd_data('?');
       lcd_spaces(18);		// clear rest of line 2
 #if (LCD_LINES > 2)
       lcd_line3();	
       lcd_clear_line();	// clear old Vloss= message
 #endif
     }
#if defined(POWER_OFF) && defined(BAT_CHECK)
     Bat_update(times);
#endif
     key_pressed = wait_for_key_ms(SCREEN_TIME);
#ifdef WITH_ROTARY_SWITCH
     if ((key_pressed != 0) || (rotary.incre > 3)) break;
#else
     if (key_pressed != 0) break;
#endif
  }  /* end for times */
  lcd_clear();
} /* end show_Cap13() */
#endif

#if defined(POWER_OFF) && defined(BAT_CHECK)
// monitor Battery in line 4 or line2, if a two line display 
void Bat_update(uint8_t tt) {
  if((tt % 16) == 0) {
 #if (LCD_LINES > 3)
     lcd_line4();
     Battery_check();
 #else
     wait_about1s();
     lcd_line2();
     lcd_clear_line();
     lcd_line2();
     Battery_check();
     wait_about2s();
 #endif
  }
};	/* end Bat_update() */
#endif
