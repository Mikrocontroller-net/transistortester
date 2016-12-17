// show_Resis_Cap.c , new code by K.-H. Kübbeler

#include <avr/io.h>
#include <stdlib.h>
#include "Transistortester.h"




typedef uint8_t byte;


// display formats for R, L, and resonance measurement:
// |....|....|....|   (16 characters line length)

// nothing found:
// 1-RR-3      [RL]
// ?

// only resistance:
// 1-RR-3      [RL]
// 12.34kO

// resistance and inductance
// 1-RR-LL-3   [RL]
// 12.34kO L=12.3uH
// 334.5kHz Q=12.3    <--- only  if resonance detected

// resistance and inductance measured through resonance, rather tight format to fit on 3 lines
// 1-RR-LL-3 12.34kO   (the ohm sign doesn't fit this way, but most likely, there's no 'k' sign since coils tend to not have that much resistance)
// 12.3uH if 22.0nF
// 334.5kHz Q=12.3

// same case but on bigger screens:  <---- not yet implemented!
// 1-RR-LL-3
// 12.34kO
// 12.3uH if 22.0nF
// 334.5kHz Q=12.3


#if FLASHEND > 0x1fff

 #if FLASHEND > 0x3fff
 // EXTRASPACES contains any extra spaces needed to fill out the line if wider than 16 characters, i.e., LCD_LINE_LENGTH-16 spaces
  #if LCD_LINE_LENGTH==16
   #define EXTRASPACES
  #elif LCD_LINE_LENGTH==17
   #define EXTRASPACES ' ',
  #elif LCD_LINE_LENGTH==18
   #define EXTRASPACES ' ',' ',
  #elif LCD_LINE_LENGTH==19
   #define EXTRASPACES ' ',' ',' ',
  #elif LCD_LINE_LENGTH==20
   #define EXTRASPACES ' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==21
   #define EXTRASPACES ' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==22
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==23
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==24
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==25
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==26
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==27
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==28
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==29
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==30
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==31
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==32
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', 
  #elif LCD_LINE_LENGTH==33
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==34
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==35
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==36
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==37
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==38
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #elif LCD_LINE_LENGTH==39
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',   
  #elif LCD_LINE_LENGTH==40
   #define EXTRASPACES ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  #else
   #warning Please add support for your LCD_LINE_LENGTH
   #define EXTRASPACES
  #endif
// strings to fill out the topline with "[RL]" or "[ R]"
// they include precisely enough extra space to start from the 7th position on the line
// if already k characters more have been written, just add k to the starting address
  #if RMETER_WITH_L
// strings to fill out the topline with "[RL]" or "[ R]"
// they include precisely enough extra space to start from the 7th position on the line
// if already k characters more have been written, just add k to the starting address
#ifdef RMETER_WITH_L
const unsigned char RL_METER_str[] MEM2_TEXT = {' ',' ',' ',' ',' ',EXTRASPACES ' ','[','R','L',']',0};
#else
const unsigned char RL_METER_str[] MEM2_TEXT = {' ',' ',' ',' ',' ',EXTRASPACES ' ',' ','[','R',']',0};
#endif
  #else
const unsigned char RESIS_str_R2[] MEM2_TEXT = {' ',' ',' ',' ',' ',EXTRASPACES ' ',' ','[','R',']',0};
  #endif
 #endif  /* FLASHEND > 0x3fff */

void show_resis(byte pin1, byte pin2, byte how)
// can be invoked both from main() and from show_Resis13()
// pin1 and pin2 are resistor's pin numbers, but ResistorList[0] should also be correctly filled
// assumes resistance has already been measured, but will do inductance measurements as appropriate
// "how" flag tells how to show the results: if set [R] or [RL] will be shown in top right corner
{
 #ifdef RMETER_WITH_L
           lcd_testpin(pin1);
           lcd_MEM_string(Resistor_str);	// -[==]-
           lcd_refresh();
	   ReadInductance();	// measure inductance, possible only with R<2.1k

  #ifdef SamplingADC
           sampling_lc(pin1,pin2);    // measure inductance using resonance method

           // draw first line: the pin numbers, RR and possibly LL symbol, and possibly [R] or [RL]
           byte lclx0=(lc_lx==0);
           if (inductor_lpre < 0 || !lclx0) 
  #else 
           if (inductor_lpre < 0)
  #endif

           {
              lcd_MEM_string(Inductor_str+1);            // "LL-"
           }
           lcd_testpin(pin2);

           // second line: measured R value (but that goes on first line if lc_lx!=0), and measured inductance, if applicable

 #ifdef SamplingADC
           if (!lclx0) {  /* inductance measured by sampling method */
              lcd_space();
              RvalOut(ResistorList[0]);		// show Resistance, probably ESR, still on first line
           }
  #endif
 
  #if FLASHEND > 0x3fff
           if (how) {
              // still need to write "[RL]" or "[R]" at the end of first line, if it fits
              if (_lcd_column<=LCD_LINE_LENGTH-4) {
                 lcd_MEM_string(RL_METER_str+(_lcd_column-6));	// " [R]" or "[RL]"
              }
           }
  #else
           lcd_clear_line();
  #endif
//           lcd_line2();
           lcd_next_line_wait(0);
  #ifdef SamplingADC
           if (!lclx0) {  /* Frequency found */
//              lcd_next_line(0);
              DisplayValue(lc_lx,lc_lpre,'H',3);	// output inductance
              lcd_MEM2_string(iF_str);		// " if "
              uint16_t lc_cpar;    // value of parallel capacitor used for calculating inductance, in pF
              lc_cpar=eeprom_read_word((uint16_t *)&lc_cpar_ee);
   #if (LCD_LINES<3) && (LCD_LINE_LENGTH<17)
              DisplayValue16(lc_cpar,-12,'F',2);	        // on 2-line dispaly show parallel capacitance with only 2 digits to make room for the '+' sign at the end of the line
   #else
              DisplayValue16(lc_cpar,-12,'F',3);	        // show parallel capacitance
   #endif
           } else 
  #endif
           {
//              lcd_next_line_wait(0);
              RvalOut(ResistorList[0]);		// show Resistance, probably ESR

              if (inductor_lpre < -2) {
                 // resistor has also inductance
                 lcd_MEM_string(Lis_str);		// "L="
                 DisplayValue(inductor_lx,inductor_lpre,'H',3);        // output classic inductance
              }

           }
           // third line: measured resonance frequency and Q, if applicable

  #ifdef SamplingADC
           if (lc_fx) {
              lcd_next_line_wait(0);
              DisplayValue(lc_fx,lc_fpre,'H',4);
              lcd_MEM2_string(zQ_str);		// "z Q="
              DisplayValue16(lc_qx, lc_qpre,' ',3);
              lcd_clear_line();
           } else {
//  #if LCD_LINES>2
//              // make sure we clean the third line, but only if the display actually has a 3rd line
//              lcd_next_line(0);
//  #endif
              lcd_next_line(0);
	      if (last_line_used == 0) {
                 lcd_clear_line();
              }
           }
  #endif
 #else		/* without Inductance measurement, only show resistance */
           lcd_line2();
           inductor_lpre = -1;		// prevent ESR measurement because Inductance is not tested
           RvalOut(ResistorList[0]);	// show Resistance, no ESR
 #endif
}

#endif //   FLASHEND > 0x1fff


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
#ifndef RMETER_WITH_L
  lcd_set_cursor(0,6);
  lcd_MEM_string(RL_METER_str);	// " [R]" or "[RL]"
#endif
#ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<250;) 
 #else
  while (1)		/* wait endless without the POWER_OFF option */
 #endif
  {
        init_parts();		// set all parts to nothing found 
        GetResistance(TP3, TP1);
        GetResistance(TP1, TP3);
	lcd_line1();		// lcd_set_cursor(0,0);
        if (ResistorsFound != 0) {
           show_resis(TP1,TP3,1);
        } else {		/* no resistor found */
 #ifdef RMETER_WITH_L
           lcd_MEM_string(RESIS_13_str);
           lcd_MEM_string(RL_METER_str+4);	// " [R]" or "[RL]"
 #endif
           lcd_line2();
           lcd_data('?');		// too big
 #if LCD_LINES>2
           lcd_next_line(0);
 #endif
           lcd_clear_line();
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
 #if defined(POWER_OFF)
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
 #endif
  }  /* end for times */
  lcd_clear();
} /* end show_Resis13() */
#endif  /* FLASHEND > 0x3fff */
 
/* ****************************************************************** */
/* show cap display the capacity measurement results */
/* ****************************************************************** */
#if FLASHEND > 0x3fff
void show_cap(uint8_t how)
#else
void show_cap_simple(void)
#endif
{
//     lcd_MEM_string(Capacitor);
  lcd_line1();
  lcd_testpin(cap.ca);               //Pin number 1
  lcd_MEM_string(CapZeich);          // capacitor sign
#if FLASHEND > 0x1fff
  uint8_t present_res;	// true, if resistor symbol is shown in Row 1
  uint8_t present_esr;
  uint8_t present_vloss;
  GetVloss();              
  cap.esr = GetESR(cap.cb, cap.ca);          // get ESR of capacitor

  present_esr = (cap.esr < 65530);
  present_vloss = (cap.v_loss != 0);
 #if (LCD_LINES > 2)
  present_res = present_esr; // show every time a well ESR value is detected, Vloss extra line
 #else
  #if FLASHEND > 0x3fff
  present_res = (present_esr  && (!present_vloss || how));
  #else
  present_res = (present_esr  && (!present_vloss));
  #endif
  // show Vloss additionally in line 2 , or no Vloss
 #endif
  if (present_res)
  {
     lcd_MEM_string(Resistor_str+1);   // [=]-
  }
#endif   /* FLASHEND > 0x1fff */
  lcd_testpin(cap.cb);               //Pin number 2

#if FLASHEND > 0x1fff
 #if FLASHEND > 0x3fff
  if (how) {
     // Vloss is allways shown in separate line
     lcd_spaces(LCD_LINE_LENGTH - 3 - _lcd_column);
     lcd_MEM2_string(CMETER_13_str);       // "[C]" at the end of line 1
  } else {
 #endif  /* FLASHENF > 0x3fff */

 #if (LCD_LINES <= 2) /* Vloss in line 1 */
     if (cap.v_loss != 0) {
        lcd_MEM_string(VLOSS_str);      // " Vloss=" 
        DisplayValue16(cap.v_loss,-1,'%',2);
     }
 #endif
     lcd_clear_line();		// clear to end of line

 #if FLASHEND > 0x3fff
  }    /* end of if (how) */
 #endif
#else
  lcd_clear_line();		// clear to end of line
#endif  /* FLASHEND > 0x1fff */

// - - - - - - - - - - - - - - - - - - - - - -
  lcd_line2();                       //2. row 
  DisplayValue(cap.cval_max,cap.cpre_max,'F',4);

#if FLASHEND > 0x1fff
  if (present_esr) {
     lcd_MEM_string(ESR_str);        // " ESR="
     DisplayValue16(cap.esr,-2,LCD_CHAR_OMEGA,2);
  }
  lcd_clear_line();
// - - - - - - - - - - - - - - - - - - - - - -
 #if LCD_LINES > 2
     lcd_line3();
     if (present_vloss ) {
        lcd_MEM_string(&VLOSS_str[1]);      // "Vloss=" 
        DisplayValue16(cap.v_loss,-1,'%',2);
     }
     lcd_clear_line();
 #else  /* only two lines */
  #if FLASHEND > 0x3fff
  if (how && present_vloss)
  {
     lcd_next_line_wait(0);		// show vloss after waiting
     lcd_MEM_string(&VLOSS_str[1]);      // "Vloss=" 
     DisplayValue16(cap.v_loss,-1,'%',2);
  }
  lcd_clear_line();		// clear to end of line
  #endif
 #endif
#else
  lcd_clear_line();
#endif  /* FLASHEND > 0x1fff */
} /* end show_cap or show_cap_simple */

#if FLASHEND > 0x3fff
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

//  lcd_clear();
 #ifdef POWER_OFF
  uint8_t times;
  for (times=0;times<250;) 
 #else
  while (1)		/* wait endless without the POWER_OFF option */
 #endif
  {
     init_parts();		// set all parts to nothing found 
//     cap.v_loss = 0;		// clear vloss  for low capacity values (<25pF)!
     ReadCapacity(TP3, TP1);
     PartFound = PART_CAPACITOR;
 #ifdef SamplingADC
     if (cap.cpre==-12 && cap.cval<100) {
        // if below 100 pF, try the alternative measuring method for small capacitors
        cap.cval = sampling_cap(TP3,TP1,0);
        cap.cpre = sampling_cap_pre;
     }
 #endif
     if (cap.cpre > -15) {	/* Capacity below the detection limit */
       cap.cpre_max = cap.cpre;		// show_cap will display the cap.cval_max value
       cap.cval_max = cap.cval;
       show_cap(1);		// with [C] at the end of line
     } else { /* no cap detected */
       lcd_line1();
       lcd_MEM2_string(CAP_13_str);	// 1-||-3
       lcd_spaces(LCD_LINE_LENGTH - 3 - _lcd_column);
       lcd_MEM2_string(CMETER_13_str);       // "[C]" at the end of line 1
       lcd_line2();
       lcd_data('?');
       lcd_clear_line();		// clear to end of line 2
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
 #if defined(POWER_OFF)
     times = Pwr_mode_check(times);	// no time limit with DC_Pwr_mode
 #endif
  }  /* end for times */
  lcd_clear();		// clear to end of line
} /* end show_Cap13() */
#endif  /* FLASHEND > 0x3fff */

#if defined(POWER_OFF) && defined(BAT_CHECK)
// monitor Battery in line 4 or line2, if a two line display 
void Bat_update(uint8_t tt) {
  if((tt % 16) == 0) {
 #if (LCD_LINES > 3)
     lcd_line4();
     Battery_check();
 #else
     wait_about1s();	/* time delay before overwiting line2 with Bat= message */
  #if (LCD_LINES == 3)
     lcd_line3();	// use the last line for Bat=
  #else
     lcd_line2();	// use the last line for Bat=
  #endif
     Battery_check();
     wait_about1s();	/* time delay for reading the Bat= message */
 #endif
  }
};	/* end Bat_update() */
#endif
#if defined(POWER_OFF)
uint8_t Pwr_mode_check(uint8_t tt) {
 #if FLASHEND > 0x1fff
 if ((tt == 15) && (DC_Pwr_mode == 1)) return(0);  // when DC_Mode, next cycle start with 0
 #else
 if (tt == 15)  return(0);  // when DC_Mode, next cycle start with 0
 #endif
 return(tt + 1);	// otherwise increase
};
#endif
