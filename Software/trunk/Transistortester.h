#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <math.h>
#include "config.h"	/* part_defs.h lcd_defines.h font.h autoconf.h */
#include "tt_function.h"
#include "tt_resistor.h"
#include "tt_globals.h"


#if defined (MAIN_C)
  /* The voltage at a capacitor grows with  Uc = VCC * (1 - e**(-t/T))
  The voltage 1.3V is reached at  t = -ln(3.7/5)*T  = 0.3011*T . 
  Time constant is  T = R * C ; also
  C = T / R
  for the resistor 470 kOhm  is C = t / (0.3011 * 470000)
  H_Fakt = 707/100 for a result in pF units.

  */

// Big Capacities (>50µF) are measured with up to 500 load-pulses with the 680 Ohm resistor.
// Each  of this load-puls has an length of 10ms. After every load-pulse the voltage of the
// capacitor is measured. If the voltage is more than 300mV, the capacity is computed by
// interpolating the corresponding values of the table RLtab and multiply that with the number
// of load pulses (*10).
// don't know why values are correct with a +40 0hm delta ?
const uint16_t RLtab[] MEM_TEXT = {
	/*  300 */ 161615107.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  325 */ 148790151.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  350 */ 137796673.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  375 */ 128268372.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  400 */ 119930523.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  425 */ 112573043.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  450 */ 106032531.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  475 */ 100179988.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  500 */  94912216.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  525 */  90145671.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  550 */  85812001.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  575 */  81854741.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  600 */  78226835.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  625 */  74888757.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  650 */  71807063.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  675 */  68953262.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  700 */  66302933.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  725 */  63835026.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  750 */  61531294.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  775 */  59375846.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  800 */  57354779.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  825 */  55455872.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  850 */  53668345.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  875 */  51982646.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  900 */  50390288.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  925 */  48883700.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  950 */  47456108.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/*  975 */  46101432.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1000 */  44814201.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1025 */  43589478.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1050 */  42422794.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1075 */  41310098.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1100 */  40247707.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1125 */  39232264.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1150 */  38260704.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1175 */  37330225.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1200 */  36438256.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1225 */  35582436.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1250 */  34760595.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1275 */  33970731.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1300 */  33210996.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1325 */  32479683.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1350 */  31775210.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1375 */  31096111.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5,
	/* 1400 */  30441023.0 / (PIN_RM + PIN_RP + R_L_VAL) + 0.5
};

//	#if R_L_VAL == 3300
//	// resistor   330 Ohm               300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
//	const uint16_t RLtab[] MEM_TEXT = {43680,40214,37242,34667,32414,30425,28657,27076,25652,24364,23192,22123,21142,20240,19407,18636,17920,17253,16630,16048,15501,14988,14505,14049,13619,13212,12826,12460,12112,11781,11466,11165,10878,10603,10341,10089, 9848, 9617, 9395, 9181, 8976, 8778, 8588, 8404, 8227};
//	#else
//	// resistor   680 Ohm               300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
//	const uint16_t RLtab[] MEM_TEXT = {22447,20665,19138,17815,16657,15635,14727,13914,13182,12520,11918,11369,10865,10401, 9973, 9577, 9209, 8866, 8546, 8247, 7966, 7702, 7454, 7220, 6999, 6789, 6591, 6403, 6224, 6054, 5892, 5738, 5590, 5449, 5314, 5185, 5061, 4942, 4828, 4718, 4613, 4511, 4413, 4319, 4228};
//	#endif

#if FLASHEND > 0x1fff
/* table for function -1000*log(1 - (permil/1000)) , distance is 20 permil */
//                                 {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50};
 const uint16_t LogTab[] PROGMEM = {0, 20, 41, 62, 83,105,128,151,174,198,223,248,274,301,329,357,386,416,446,478,511,545,580,616,654,693,734,777,821,868,916,968,1022,1079,1139,1204,1273,1347,1427,1514,1609,1715,1833,1966,2120,2303,2526,2813,3219,3912,7065 }; /* last value is replaced by  (-1000*ln(0.001)-3912)/19*20 + 3912 */

#endif


#ifdef AUTO_RH

/*
 #if R_H_VAL == 10000
// resistor  100000 Ohm             1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
 const uint16_t RHtab[] PROGMEM = {4483,4244,4026,3827,3645,3477,3322,3178,3045};
 #else
// resistor  470000 Ohm            1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
 const uint16_t RHtab[] PROGMEM = { 954, 903, 856, 814, 775, 740, 707, 676, 648};
 #endif
*/

const uint16_t RHtab[] PROGMEM = { 
  44814201./R_H_VAL+0.5,   // 1000 mV
  42422794./R_H_VAL+0.5,   // 1050 mV
  40247707./R_H_VAL+0.5,   // 1100 mV
  38260704./R_H_VAL+0.5,   // 1150 mV
  36438256./R_H_VAL+0.5,   // 1200 mV
  34760595./R_H_VAL+0.5,   // 1250 mV
  33210996./R_H_VAL+0.5,   // 1300 mV
  31775210./R_H_VAL+0.5,   // 1350 mV
  30441023./R_H_VAL+0.5    // 1400 mV
  };
 // these values are 1e7/R_H_VAL/-ln(1-ref/5000)   (note that R_H_VAL is in multiples of 10 ohm)
 // the +0.5 is for rounding

#endif  // AUTO_RH


// with integer factors the ADC-value will be changed to mV resolution in ReadADC !
// all if statements are corrected to the mV resolution.

/*########################################################################################
End of configuration 
*/

/*Strings in PROGMEM or in EEprom
*/
#include "langFRANCAIS.h"
#include "langGERMAN.h"
#include "langITALIAN.h"
#include "langPOLISH.h"
#include "langCZECH.h"
#include "langSLOVAK.h"
#include "langSLOVENE.h"
#include "langDUTCH.h"
#include "langBRASIL.h"
#include "langSPANISH.h"
#include "langRUSSIAN.h"
#include "langUKRAINIAN.h"
#include "langHUNGARIAN.h"
#include "langLITHUANIAN.h"
#include "langDANISH.h"

// All languages must be placed before the english text.
// If none is actually selected, LANG_SELECTED is not set.
#ifndef LANG_SELECTED		//default language is english
   const unsigned char TestRunning[] MEM_TEXT = "Testing.";
   const unsigned char BatWeak[] MEM_TEXT = "weak";
   const unsigned char BatEmpty[] MEM_TEXT = "empty!";
   const unsigned char TestFailed2[] MEM_TEXT = "damaged ";
   const unsigned char Bauteil[] MEM_TEXT = "part";
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyrist.";
   const unsigned char Unknown[] MEM_TEXT = " unknown";
   const unsigned char TestFailed1[] MEM_TEXT = "No, unknown, or";
   const unsigned char Detected[] MEM_TEXT = " detected";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probes!"; // or "separate pins!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Selection:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "Show data";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Switch off";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
 const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','c','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";	
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";
#ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequency";
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-Scaler";
 #endif
#endif
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequency > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF quartz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF quartz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contrast";
  #endif
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Crystal  ";
 #endif
#endif  /* default LANG ENGLISH */


// Strings, which are not dependent of any language
 const unsigned char Bat_str[] MEM_TEXT = "Bat. ";
 const unsigned char OK_str[] MEM_TEXT = "OK";
#if FLASHEND > 0x1fff
 #ifndef DC_PWR_TEXT 
  #define DC_PWR_TEXT "DC Pwr Mode"
 #endif
 const unsigned char DC_Pwr_Mode_str[] MEM_TEXT = DC_PWR_TEXT;
#endif
 const unsigned char mosfet_str[] MEM_TEXT = "-MOS";
 const unsigned char jfet_str[] MEM_TEXT = "JFET";
 const unsigned char igbt_str[] MEM_TEXT = "-IGBT";
#if ((!defined(WITH_GRAPHICS) && (LCD_LINE_LENGTH > 17)) || (defined(WITH_GRAPHICS) && (LOW_H_SPACE != 1)))
 const unsigned char hfe_str[] MEM_TEXT ="hFE=";
 const unsigned char GateCap_str[] MEM_TEXT = "Cg=";
#else
 #ifdef LCD_CHAR_BETA
  const unsigned char hfe_str[] MEM_TEXT = {LCD_CHAR_BETA,'=',0};
 #else
  const unsigned char hfe_str[] MEM_TEXT ="B=";
 #endif
 const unsigned char GateCap_str[] MEM_TEXT = "C=";
#endif
 const unsigned char Cap_str[] MEM_TEXT = "C=";
#ifdef WITH_GRAPHICS
 const unsigned char NPN_str[] MEM_TEXT = "BJT-NPN";
 const unsigned char PNP_str[] MEM_TEXT = "BJT-PNP";
#else
 const unsigned char NPN_str[] MEM_TEXT = "NPN";
 const unsigned char PNP_str[] MEM_TEXT = "PNP";
#endif

#ifdef WITH_GRAPHICS
 const unsigned char Pin_str[] MEM_TEXT = "Pin ";
#endif
#ifndef EBC_STYLE
 // default is the 123= style
 const unsigned char N123_str[] MEM_TEXT = {' ','1'+TP_OFFSET,'2'+TP_OFFSET,'3'+TP_OFFSET,'=',0};
// const unsigned char N123_str[] MEM_TEXT = " Pin=";
#else
 #if EBC_STYLE == 321
  const unsigned char N321_str[] MEM_TEXT = {' ','3'+TP_OFFSET,'2'+TP_OFFSET,'1'+TP_OFFSET,'=',0};
 #endif
 #if EBC_STYLE == 123
  const unsigned char N123_str[] MEM_TEXT = {' ','1'+TP_OFFSET,'2'+TP_OFFSET,'3'+TP_OFFSET,'=',0};
 #endif
#endif

 const unsigned char Ube_str[] MEM_TEXT = "Ube=";
 const unsigned char Uf_str[] MEM_TEXT = "Uf=";
 const unsigned char vt_str[] MEM_TEXT = " Vt=";
 const unsigned char Vgs_str[] MEM_TEXT = "@Vg=";
 const unsigned char CapZeich[] MEM_TEXT = {LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,0};
 const unsigned char Cell_str[] MEM_TEXT = "Cell!";
 const unsigned char VCC_str[] MEM_TEXT = "VCC=";

#if FLASHEND > 0x1fff
 const unsigned char ESR_str[] MEM_TEXT = " ESR=";
 const unsigned char VLOSS_str[] MEM_TEXT = " Vloss=";
 const unsigned char Lis_str[] MEM_TEXT = "L=";
 #if (LCD_LINES > 3)
 const unsigned char Ir_str[] MEM_TEXT = "Ir=";
 #else
 const unsigned char Ir_str[] MEM_TEXT = "  Ir=";
 #endif
#endif

#ifdef WITH_VEXT
 const unsigned char Vext_str[] MEM_TEXT = "Vext=";
#endif

#if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 #ifndef HelpTXT
 //default language english
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','o','t',' ','c','a','l','i','b','r','a','t','e','d','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'F','o','r',LCD_CHAR_INSEP,
'c','a','l','i','b','r','a','t','i','o','n',' ',

'c','l','a','m','p',' ','t','h','e',' ','3',LCD_CHAR_INSEP,'p','i','n','s',' ',
't','o','g','e','t','h','e','r',' ','a','n','d',' ',

's','t','a','r','t',' ','w','i','t','h',' ',
't','h','e',LCD_CHAR_INSEP ,'k','e','y','.',' ',

'A','c','k','n','o','w','l','e','d','g','e',' ','t','h','e',' ',
's','e','l','f','-','t','e','s','t',' ','w','i','t','h',' ',

't','h','e',' ','k','e','y',' ',
'w','i','t','h','i','n',LCD_CHAR_INSEP,'2','s','.', ' ',

'D','i','s','c','o','n','n','e','c','t',' ',
'p','i','n','s',' ','a','f','t','e','r',' ',

't','h','e',LCD_CHAR_INSEP,'m','e','s','s','a','g','e',' ',
'"','i','s','o','l','a','t','e',LCD_CHAR_INSEP,'P','r','o','b','e','!','"',' ',

'i','s',' ','s','h','o','w','n',
 #ifdef TPCAP
'.',' ',
 #else
' ','a','n','d',' ',
'c','o','n','n','e','c','t',' ','a',' ','g','o','o','d',' ',

'c','a','p','a','c','i','t','o','r',' ','w','i','t','h',' ',
'a','t',' ','l','e','a','s','t',' ','1','0','0','n','F',' ',

't','o',' ','p','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'a','n','d',LCD_CHAR_INSEP,'3',' ',
'o','n','l','y',' ','a','f','t','e','r',' ',

't','h','e',' ','m','e','s','s','a','g','e',' ',
'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'i','s',LCD_CHAR_INSEP,'s','h','o','w','n','.',' ',
 #endif
'S','o','f','t','w','a','r','e',' ','a','n','d',' ',

'd','o','c','u','m','e','n','t','a','t','i','o','n',' ',
'i','s',' ','a','t',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};
 #endif  /* ndef HelpTXT */
#endif  /* FLASHEND > 0x3fff */

 const unsigned char AnKat_str[] MEM_TEXT = {LCD_CHAR_LINE1, LCD_CHAR_DIODE1, LCD_CHAR_LINE1,0};
 const unsigned char KatAn_str[] MEM_TEXT = {LCD_CHAR_LINE1, LCD_CHAR_DIODE2, LCD_CHAR_LINE1,0};
#ifdef SamplingADC
 const unsigned char AT05volt[] MEM_TEXT = " @0-5V";
 const unsigned char iF_str[] MEM2_TEXT = " if ";
 const unsigned char zQ_str[] MEM2_TEXT = "z Q=";
#endif
 const unsigned char Dioden[] MEM_TEXT = {'*',LCD_CHAR_DIODE1, ' ', ' ',0};
 const unsigned char Resistor_str[] MEM_TEXT = {LCD_CHAR_LINE1, LCD_CHAR_RESIS1, LCD_CHAR_RESIS2,LCD_CHAR_LINE1,0};
#if FLASHEND > 0x1fff
 const unsigned char Inductor_str[] MEM_TEXT = {LCD_CHAR_LINE1, LCD_CHAR_INDUCTOR1, LCD_CHAR_INDUCTOR2,LCD_CHAR_LINE1,0};
#endif
#if defined (WITH_SELFTEST) || !defined (BAT_CHECK)
 const unsigned char VERSION_str[] MEM_TEXT = "Version 1.13k";
#endif
#ifdef SHOW_ICE
 const unsigned char ICE0_str[] MEM2_TEXT = "ICE0=";
 const unsigned char ICEs_str[] MEM2_TEXT = "ICEs=";
#endif
#ifdef LCD_CHANGE_COLOR
 const unsigned char FrontColor_str[] MEM2_TEXT = "FrontColor";
 const unsigned char BackColor_str[] MEM2_TEXT = "BackColor";
#endif


#ifdef WITH_SELFTEST
 #ifdef EXTENDED_TESTS
 const unsigned char URef_str[] MEM2_TEXT = "Ref=";
 const unsigned char RHfakt_str[] MEM2_TEXT = "RHf=";
 const unsigned char RH1L_str[] MEM_TEXT = "RH-";
 const unsigned char RH1H_str[] MEM_TEXT = "RH+";
 const unsigned char RLRL_str[] MEM_TEXT = "+RL- 12 13 23";
 const unsigned char RHRH_str[] MEM_TEXT = "+RH- 12 13 23";
 const unsigned char RHRL_str[] MEM_TEXT = "RH/RL";
 #endif
 const unsigned char R0_str[] MEM2_TEXT = "R0=";
 const unsigned char RIHI_str[] MEM_TEXT = "Ri_Hi=";
 const unsigned char RILO_str[] MEM_TEXT = "Ri_Lo=";
 const unsigned char C0_str[] MEM_TEXT = "C0 ";
 #ifdef SamplingADC
 const unsigned char C0samp_str[] MEM2_TEXT = "C0samp ";
 #endif
 const unsigned char T50HZ[] MEM_TEXT = " 50Hz";
#endif

#ifdef WITH_MENU
 const unsigned char FatTP2_str[] MEM2_TEXT = "f@TP2";
 const unsigned char C_ESR_str[] MEM2_TEXT = "C+ESR@TP1:3";
#endif
#ifdef WITH_HARDWARE_SERIAL
 const unsigned char DoMenu_str[] MEM2_TEXT = "do menu ";
 const unsigned char Done_str[] MEM2_TEXT = "Done.\n";
#endif

#if FLASHEND > 0x3fff
 #ifdef RMETER_WITH_L
const unsigned char RESIS_13_str[] MEM2_TEXT = {'1'+TP1,LCD_CHAR_LINE1, LCD_CHAR_RESIS1, LCD_CHAR_RESIS2,LCD_CHAR_LINE1, LCD_CHAR_INDUCTOR1, LCD_CHAR_INDUCTOR2, LCD_CHAR_LINE1, '1'+TP3,' ',0};
 #else
const unsigned char RESIS_13_str[] MEM2_TEXT = {'1'+TP1,LCD_CHAR_LINE1, LCD_CHAR_RESIS1, LCD_CHAR_RESIS2, LCD_CHAR_LINE1, '1'+TP3,' ',0};
 #endif

 const unsigned char CAP_13_str[] MEM2_TEXT = {'1'+TP1,LCD_CHAR_LINE1, LCD_CHAR_CAP, LCD_CHAR_LINE1,'1'+TP3,' ',0};
 const unsigned char CMETER_13_str[] MEM2_TEXT = {"[C]"};
#endif  /* FLASHEND > 0x3fff */

#ifdef SHOW_R_DS
 const unsigned char RDS_str[] MEM_TEXT = {"RDS="};
#endif
 

 const unsigned char CapIcon[] MEM_TEXT = { 0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x00};	// Capacitor Icon
 const unsigned char DiodeIcon1[] MEM_TEXT = { 0x11,
				     0x19,
				     0x1d,
				     0x1f,
				     0x1d,
				     0x19,
				     0x11,
				     0x00};	// Diode-Icon Anode left

 const unsigned char DiodeIcon2[] MEM_TEXT = { 0x11,
				     0x13,
				     0x17,
				     0x1f,
				     0x17,
				     0x13,
				     0x11,
				     0x00};	// Diode-Icon Anode right
 const unsigned char ResIcon1[] MEM_TEXT = { 0x00,
				     0x0f,
				     0x08,
				     0x18,
				     0x08,
				     0x0f,
				     0x00,
				     0x00};	// Resistor Icon1
 const unsigned char ResIcon2[] MEM_TEXT = { 0x00,
				     0x1e,
				     0x02,
				     0x03,
				     0x02,
				     0x1e,
				     0x00,
				     0x00};	// Resistor Icon2
#if LCD_CHAR_RESIS3 !=  'R'
 const unsigned char ResIcon3[] MEM_TEXT = { 0x04,
				     0x1f,
				     0x11,
				     0x11,
				     0x11,
				     0x11,
				     0x1f,
				     0x04};	// Resistor Icon2
#endif

//generate Omega- and µ-character as Custom-character, if these characters has a number of loadable type
#if LCD_CHAR_OMEGA < 8
   const unsigned char CyrillicOmegaIcon[] MEM_TEXT = {0,0,14,17,17,10,27,0};	//Omega
#endif
#if LCD_CHAR_U < 8
   const unsigned char CyrillicMuIcon[] MEM_TEXT = {0,17,17,17,19,29,16,16};	//µ
#endif

// The combined Table for switching RL / RH and ADC pins, index is the TP number
 const unsigned char PinRLRHADCtab[] PROGMEM = { (1<<PIN_RL1),
				     (1<<PIN_RL2),
				     (1<<PIN_RL3),	// Table of commands to switch the  R-L resistors Pin 0,1,2
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
 // Processors with little memory must use one Pin number higher than correspondig Low Resistor
/* PinRHtab */			     (1<<PIN_RH1),
				     (1<<PIN_RH2),
				     (1<<PIN_RH3),	// Table of commands to switch the  R-L resistors Pin 0,1,2
#endif
		// Table include the predefined value TXD_VAL of other output port(s) of port C.
		// Every pin, that should be switched permanent to VCC level, should be set to 1 in every tab position.
		// The predefined value TXD_MSK defines the pin (all pins), that must be switched permanent to output.
/* PinADCtab */			     (1<<TP1)|TXD_VAL,
				     (1<<TP2)|TXD_VAL,
				     (1<<TP3)|TXD_VAL};	// Table of commands to switch the ADC-Pins 0,1,2 to output


 const uint8_t PrefixTab[] MEM_TEXT = { 'f','p','n',LCD_CHAR_U,'m',0,'k','M'}; // f,p,n,u,m,-,k,M


#ifdef AUTO_CAL
  const unsigned char MinCap_str[] MEM2_TEXT = " >100nF";
  const unsigned char REF_C_str[] MEM2_TEXT = "REF_C=";
  const unsigned char REF_R_str[] MEM2_TEXT = "REF_R=";
#endif

#ifdef SamplingADC
 const unsigned char cap_for_l_meas_str[] MEM2_TEXT = {'1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',' ','4','-','3','0','n','F','(','L',')',0};
#endif

//End of EEPROM-Strings
#else
 // no MAIN_C
 #ifdef WITH_SELFTEST
  extern const unsigned char SELFTEST[] MEM_TEXT;
  extern const unsigned char RELPROBE[] MEM_TEXT;
  extern const unsigned char ATE[] MEM_TEXT;
  extern unsigned char NPN_str[];
  extern unsigned char PNP_str[];
  extern unsigned char mosfet_str[];
  extern unsigned char jfet_str[];
  extern unsigned char igbt_str[];
 #endif
 #ifdef WITH_MENU
  extern const unsigned char SELECTION_str[] MEM2_TEXT ;
  extern const unsigned char TESTER_str[] MEM2_TEXT ;
  extern const unsigned char FREQ_str[] MEM2_TEXT;
 #ifdef WITH_FREQUENCY_DIVIDER
   extern const unsigned char FScaler_str[] MEM2_TEXT;
 #endif
  extern const unsigned char VOLTAGE_str[] MEM2_TEXT ;
  extern const unsigned char SHOW_str[] MEM2_TEXT ;
  extern const unsigned char OFF_str[] MEM2_TEXT ;
  extern const unsigned char F_GEN_str[] MEM2_TEXT ;
  extern const unsigned char PWM_10bit_str[] MEM2_TEXT ;
  #ifdef WITH_ROTARY_CHECK
  extern const unsigned char RotaryEncoder_str[] MEM2_TEXT ;
  #endif
  extern const unsigned char SetCapCorr_str[] MEM2_TEXT ;
  extern const unsigned char TURN_str[] MEM2_TEXT ;	
  extern const unsigned char FULLCHECK_str[] MEM2_TEXT ;
  extern const unsigned char SHORT_PROBES_str[] MEM2_TEXT ;
  #if PROCESSOR_TYP == 644
  extern const unsigned char HFREQ_str[] MEM2_TEXT ;
  extern const unsigned char H_CRYSTAL_str[] MEM2_TEXT ;
  extern const unsigned char L_CRYSTAL_str[] MEM2_TEXT ;
  #endif
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
  extern const unsigned char cerres_str[] MEM_TEXT ;
  extern const unsigned char xtal_str[] MEM_TEXT ;
 #endif

 #ifdef WITH_VEXT
  extern const unsigned char Vext_str[] MEM_TEXT ;
 #endif

 #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
  extern const unsigned char CONTRAST_str[] MEM2_TEXT;
 #endif
  extern const unsigned char Bat_str[];
  extern const unsigned char BatWeak[];
  extern const unsigned char BatEmpty[]; 
  extern const unsigned char OK_str[];

 #if FLASHEND > 0x1fff
  extern const unsigned char DC_Pwr_Mode_str[];
 #endif

 #ifdef AUTO_CAL
  extern int8_t RefDiff;
  extern uint16_t ref_offset;
  extern uint8_t c_zero_tab[];
 #endif
 #ifdef WITH_GRAPHICS
  extern const unsigned char Pin_str[] MEM_TEXT;
 #endif
 #ifndef EBC_STYLE
 // default is the 123= style
  extern const unsigned char N123_str[] MEM_TEXT;
 #else
  #if EBC_STYLE == 321
  extern const unsigned char N321_str[] MEM_TEXT;
  #endif
  #if EBC_STYLE == 123
  extern const unsigned char N123_str[] MEM_TEXT;
  #endif
 #endif
  extern const unsigned char Uf_str[] MEM_TEXT;
 #ifdef WITH_GRAPHICS
  extern const unsigned char Ube_str[] MEM_TEXT;
 #endif
  extern  const uint16_t RLtab[];

 #if FLASHEND > 0x1fff
  extern uint16_t LogTab[];
  extern const unsigned char ESR_str[] MEM_TEXT;
  extern const unsigned char VLOSS_str[] MEM_TEXT ;
  extern const unsigned char Inductor_str[] MEM_TEXT;
  extern const unsigned char Lis_str[] MEM_TEXT;
 #endif

  extern const unsigned char AnKat_str[] MEM_TEXT ;
  extern const unsigned char KatAn_str[] MEM_TEXT ;
 #ifdef SamplingADC
  extern const unsigned char cap_for_l_meas_str[] MEM2_TEXT;
  extern const unsigned char AT05volt[] MEM_TEXT;
  extern const unsigned char iF_str[] MEM2_TEXT ;
  extern const unsigned char zQ_str[] MEM2_TEXT ;
 #endif
  extern const unsigned char Resistor_str[] MEM_TEXT ;
  extern const unsigned char CapZeich[] MEM_TEXT ;
  extern const unsigned char GateCap_str[] MEM_TEXT;


 #ifdef AUTO_RH
  extern const uint16_t RHtab[];
 #endif
  extern const unsigned char PinRLRHADCtab[] PROGMEM;
  extern const uint8_t PrefixTab[]; // f,p,n,u,m,-,k,M
 #if FLASHEND > 0x3fff
  extern const unsigned char PinRHtab[];
 #endif
//  extern const unsigned char PinADCtab[];
#endif /* if defined (MAIN_C) */

#ifdef WITH_GRAPHICS
 #include "bitmaps.h"
#endif

#ifdef WITH_MENU
  extern const unsigned char FatTP2_str[] MEM2_TEXT;
  extern const unsigned char VERSION_str[] MEM_TEXT;
  extern const unsigned char C_ESR_str[] MEM2_TEXT;
  extern const unsigned char REF_C_str[] MEM2_TEXT;
  extern const unsigned char REF_R_str[] MEM2_TEXT;
  extern const unsigned char R0_str[] MEM2_TEXT;
  extern const unsigned char RIHI_str[] MEM_TEXT;
  extern const unsigned char RILO_str[] MEM_TEXT;
#endif
  extern const unsigned char C0_str[] MEM_TEXT;
  extern const unsigned char C0samp_str[] MEM2_TEXT;
#if FLASHEND > 0x1fff
  extern const unsigned char CAP_13_str[] MEM2_TEXT;
  extern const unsigned char RDS_str[] MEM_TEXT;
  extern const unsigned char CMETER_13_str[] MEM2_TEXT;
  extern const unsigned char RESIS_13_str[] MEM2_TEXT;
  extern const unsigned char RL_METER_str[] MEM2_TEXT;
#endif

  extern const unsigned char CapIcon[] MEM_TEXT;	// Capacitor Icon
  extern const unsigned char DiodeIcon1[] MEM_TEXT;	// Diode-Icon Anode left
  
  extern const unsigned char DiodeIcon2[] MEM_TEXT;	// Diode-Icon Anode right
  extern const unsigned char ResIcon1[] MEM_TEXT;	// Resistor Icon1
  extern const unsigned char ResIcon2[] MEM_TEXT;	// Resistor Icon2
#if LCD_CHAR_RESIS3 !=  'R'
  extern const unsigned char ResIcon3[] MEM_TEXT;	// Resistor Icon2
#endif
#ifdef LCD_CHANGE_COLOR
  extern const unsigned char FrontColor_str[];
  extern const unsigned char BackColor_str[];
#endif

//generate Omega- and µ-character as Custom-character, if these characters has a number of loadable type
#if LCD_CHAR_OMEGA < 8
  extern const unsigned char CyrillicOmegaIcon[] MEM_TEXT;	//Omega
#endif
#if LCD_CHAR_U < 8
  extern const unsigned char CyrillicMuIcon[] MEM_TEXT;	//µ
#endif


// macros for easily acquiring the bitmasks for pins
 #define pinmaskRL(pin) pgm_read_byte(PinRLRHADCtab+(pin))
#if (((PIN_RL1 + 1) != PIN_RH1) || ((PIN_RL2 + 1) != PIN_RH2) || ((PIN_RL3 + 1) != PIN_RH3))
 #define pinmaskRH(pin) pgm_read_byte(PinRLRHADCtab+(pin)+3)
 #define pinmaskADC(pin) (pgm_read_byte(PinRLRHADCtab+(pin)+6)) | TXD_MSK
#else
 #define pinmaskRH(pin) (pinmaskRL(pin)<<1)
 #define pinmaskADC(pin) (pgm_read_byte(PinRLRHADCtab+(pin)+3)) | TXD_MSK
#endif


// for processor-dependent selection of reference:
#if (PROCESSOR_TYP == 644) || (PROCESSOR_TYP == 1280)
 #define ADref1V1 ((0<<REFS0)|(1<<REFS1))   // use built-in reference, about 1.1 V;
#else
 #define ADref1V1 ((1<<REFS0)|(1<<REFS1))   // use built-in reference, about 1.1 V;
#endif

// macro for appropriate delay for voltage stabilization:
#ifdef NO_AREF_CAP
 #define wait_aref_stabilize    wait100us
#else
 #define wait_aref_stabilize    wait10ms
#endif

