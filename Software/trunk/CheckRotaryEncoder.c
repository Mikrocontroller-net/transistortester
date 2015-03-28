// new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"


//=================================================================
// CheckRotaryEndoder

#ifdef WITH_MENU
void CheckRotaryEncoder(void) {
  unsigned int TP13tab[4];	//count of state of the bits TP1 and TP3
  unsigned int TP23tab[4];	//count of state of the bits TP2 and TP3
  unsigned int TP12tab[4];	//count of state of the bits TP1 and TP2
  uint8_t index;	// Table offset, 0=nobit set, 1=lower, 2=upper, 3=both bit set
  uint8_t start_index23=0;	// start index of TP2:TP3
  uint8_t ii;
  uint8_t iim;
  uint8_t taste;
  uint8_t open_state;	// flag is set, if many times a open state is found fpr both switches
  uint8_t close_state;	// flag is set, if many times a closed state is found for both switches
  uint8_t switch1;	// pin number of first switch
  uint8_t switch2;	// pin number of second switch
  uint8_t switchm;	// common pin number of both switches
  unsigned int kk;	// loop counter for polling the pins

  message_key_released(RotaryEncoder_str);	// RotaryEncoder in line 1
  lcd_line2();
  lcd_MEM_string(TURN_str);			// "turn!" at line 2 of LCD
  lcd_refresh();		// write the pixels to display, ST7920 only
  
  for (ii=0;ii<4;ii++) {
     TP13tab[ii] = 0;		//reset count of bit TP1:TP3 to zero
     TP23tab[ii] = 0;		//reset count of bit TP2:TP3 to zero
     TP12tab[ii] = 0;		//reset count of bit TP1:TP2 to zero
  }  
  R_PORT = (1<<PIN_RL1) | (1<<PIN_RL2) | (1<<PIN_RL3); // all ports with 470k to VCC
  R_DDR = (1<<PIN_RL1) | (1<<PIN_RL2) | (1<<PIN_RL3); // all ports with 470k to VCC
  switch1 = TP1;		// no middle pin found, set all pins equal
  switch2 = TP1;
  switchm = TP1;
  open_state = 0;		// open state for both switches is not often found
  close_state = 0;		// close state for both switches is not often found
  for (kk=0; kk<63001; kk++) {
     ADC_PORT = TXD_VAL;		// all ADC output are preset to GND
     ADC_DDR = (1<<TP1) | TXD_MSK;	// switch TP1 to GND level
     wait2us();
     // look, what the other two levels are (TP2 and TP3)
     index = 0;			// index for both switches closed
     ii = ADC_PIN;		// Read port
     if ((ii & (1<<TP2)) != 0) index += 1;	// index for first switch open is 1 or 3
     if ((ii & (1<<TP3)) != 0) index += 2;	// index for second switch open is 2 or 3
     if (kk == 0) {
        start_index23 = index;		// save the start state
     } else {
        if (index != start_index23) {
           TP23tab[index]++;		// add one to the found state of TP2:TP3
        } else {
           start_index23 = 4;		// now every state will be counted
        }
     }

     ADC_DDR = (1<<TP2) | TXD_MSK;	// switch TP2 to GND level
     wait2us();
     // look, what the other two levels are (TP1 and TP3)
     index = 0;			// index for both switches closed
     ii = ADC_PIN;		// Read port
     if ((ii & (1<<TP1)) != 0) index += 1;	// index for first switch open is 1 or 3
     if ((ii & (1<<TP3)) != 0) index += 2;	// index for second switch open is 2 or 3
     if (start_index23 == 4) {
        TP13tab[index]++;		//add one to the found state of TP1:TP3
     }

     ADC_DDR = (1<<TP3) | TXD_MSK;	// switch TP3 to GND level
     wait2us();
     // look, what the other two levels are (TP1 and TP3)
     index = 0;			// index for both switches closed
     ii = ADC_PIN;		// Read port
     if ((ii & (1<<TP1)) != 0) index += 1;	// index for first switch open is 1 or 3
     if ((ii & (1<<TP2)) != 0) index += 2;	// index for second switch open is 2 or 3
     if (start_index23 == 4) {
        TP12tab[index]++;		//add one to the found state of TP1:TP2
     }
     wdt_reset();
     wait1ms();			// wait a little for next sample
     if ((RST_PIN_REG & (1<<RST_PIN)) == 0) goto end_rotary;
     if ((TP13tab[0] > 20) &&(TP13tab[1] > 20) && (TP13tab[2] > 20) && (TP13tab[3] > 20)) {
        switch1 = TP1;
        switch2 = TP3;
        switchm = TP2;
        if ((TP13tab[1] + TP13tab[2]) < TP13tab[0]) {
           close_state = 1;	// both switches are often monitored in close state
        }
        if ((TP13tab[1] + TP13tab[2]) < TP13tab[3]) {
           open_state = 1; 	// both switches are often monitored in open state
        }
        break;
     } 
     if ((TP23tab[0] > 20) &&(TP23tab[1] > 20) && (TP23tab[2] > 20) && (TP23tab[3] > 20)) {
        switch1 = TP2;
        switch2 = TP3;
        switchm = TP1;
        if ((TP23tab[1] + TP23tab[2]) < TP23tab[0]) {
           close_state = 1;	// both switches are often monitored in close state
        }
        if ((TP23tab[1] + TP23tab[2]) < TP23tab[3]) {
           open_state = 1; 	// both switches are often monitored in open state
        }
        break;
     } 
     if ((TP12tab[0] > 20) &&(TP12tab[1] > 20) && (TP12tab[2] > 20) && (TP12tab[3] > 20)) {
        switch1 = TP1;
        switch2 = TP2;
        switchm = TP3;
        if ((TP12tab[1] + TP12tab[2]) < TP12tab[0]) {
           close_state = 1;	// both switches are often monitored in close state
        }
        if ((TP12tab[1] + TP12tab[2]) < TP12tab[3]) {
           open_state = 1; 	// both switches are often monitored in open state
        }
        break;
     } 
  }
  if (switch1 == switch2) return;	// no rotary encoder found
  
  lcd_clear_line2();		// clear line 2 of LCD
  for (ii=0;ii<120;ii++) {
     lcd_line2();
     lcd_testpin(switch1);
     lcd_data('-');
     if ((ii & 0x01) == 0) {
        lcd_data('-');		// close state of switch
     } else {
        lcd_data('/');		// open state of switch
     }
     lcd_data('-');
     lcd_testpin(switchm);		// show the found pin number of the common pin
     lcd_data('-');
     if ((ii & 0x02) == 0) {
        lcd_data('-');		// close state of switch
     } else {
        lcd_data('/');		// open state of switch
     }
     lcd_data('-');
     lcd_testpin(switch2);
     lcd_space();
     iim = (ii & 0x03);
     if (((iim == 0) && (close_state != 0)) || ((iim  == 3) && (open_state != 0))) {
        if (iim == 0) lcd_data('C');
        if (iim == 3) lcd_data('o');
        taste = wait_for_key_ms(2000);	// this state is monitored for a long time
     } else {
        lcd_space();
        taste = wait_for_key_ms(400);	// this state is monitored only for a short time
     }
     if (taste != 0) break;
  } /* end for ii */
end_rotary:
  R_PORT = 0; // all resistor ports  to GND
  R_DDR = 0;	// all resistor ports to input
  ADC_DDR = TXD_MSK;	// switch ADC port to input
}
#endif
