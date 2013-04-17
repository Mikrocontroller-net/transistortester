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
#include "tt_function.h"
#include "config.h"
#include "lcd-routines.h"
#include "wait1000ms.h"
#include "part_defs.h"


#if defined (MAIN_C)
 #define COMMON
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

// Widerstand 680 Ohm                300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
COMMON const uint16_t RLtab[] MEM_TEXT = {22447,20665,19138,17815,16657,15635,14727,13914,13182,12520,11918,11369,10865,10401, 9973, 9577, 9209, 8866, 8546, 8247, 7966, 7702, 7454, 7220, 6999, 6789, 6591, 6403, 6224, 6054, 5892, 5738, 5590, 5449, 5314, 5185, 5061, 4942, 4828, 4718, 4613, 4511, 4413, 4319, 4228};

#if FLASHEND > 0x1fff
//                                        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91 };
COMMON const uint16_t LogTab[] PROGMEM = {0, 20, 41, 62, 83, 105, 128, 151, 174, 198, 223, 248, 274, 301, 329, 357, 386, 416, 446, 478, 511, 545, 580, 616, 654, 693, 734, 777, 821, 868, 916, 968, 1022, 1079, 1139, 1204, 1273, 1347, 1427, 1514, 1609, 1715, 1833, 1966, 2120, 2303, 2526 };

#endif



#ifdef AUTO_RH

//resistor   470000 Ohm      1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
 const uint16_t RHtab[] PROGMEM = { 954, 903, 856, 814, 775, 740, 707, 676, 648};

#endif


// with integer factors the ADC-value will be changed to mV resolution in ReadADC !
// all if statements are corrected to the mV resolution.

/*########################################################################################
End of configuration 
*/

/*Strings in PROGMEM or in EEprom
*/

#if defined(LANG_GERMAN)		//deutsch
   const unsigned char TestRunning[] MEM_TEXT = "Testen..."; //€€€€€€€";
   const unsigned char BatWeak[] MEM_TEXT = "gering";
   const unsigned char BatEmpty[] MEM_TEXT = "leer!"; //€€€";
   const unsigned char TestFailed2[] MEM_TEXT = "defektes "; //€€€";
   const unsigned char Bauteil[] MEM_TEXT = "Bauteil"; //€€€";
   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyristor";
   const unsigned char Unknown[] MEM_TEXT = " unbek."; //€€";
   const unsigned char TestFailed1[] MEM_TEXT = "Kein,unbek. oder";
   const unsigned char OrBroken[] MEM_TEXT = "oder defekt "; //€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selbsttest ..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Test Ende";
 #endif
#endif

#if defined(LANG_ENGLISH)		//english
   const unsigned char TestRunning[] MEM_TEXT = "Testing..."; //€€€€€€";
   const unsigned char BatWeak[] MEM_TEXT = "weak"; //€€€";
   const unsigned char BatEmpty[] MEM_TEXT = "empty!"; //€€";
   const unsigned char TestFailed2[] MEM_TEXT = "damaged "; //€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "part"; //€€€€€€";
   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyristor";
   const unsigned char Unknown[] MEM_TEXT = " unknown"; //€";
   const unsigned char TestFailed1[] MEM_TEXT = "No, unknown, or"; //€";
   const unsigned char OrBroken[] MEM_TEXT = "or damaged "; //€€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
#endif

#if defined(LANG_POLISH)		//polnisch
   const unsigned char TestRunning[] MEM_TEXT = "Testowanie..."; //€€€";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "za slaba";
   const unsigned char TestFailed2[] MEM_TEXT = "lub uszkodz.";
   const unsigned char Bauteil[] MEM_TEXT = "Elemen"; //t€€€";
   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyrystor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " nieznany";
   const unsigned char TestFailed1[] MEM_TEXT = "brak elementu"; //€€€";
   const unsigned char OrBroken[] MEM_TEXT = "lub uszkodz. ";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
#endif

#if defined(LANG_CZECH)		//Tschechisch
   const unsigned char TestRunning[] MEM_TEXT = "Probiha mereni..";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "prazdna!";
   const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "soucastka"; //€";
   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " neznama"; //€";
   const unsigned char TestFailed1[] MEM_TEXT = "Zadna, neznama"; //€€";
   const unsigned char OrBroken[] MEM_TEXT = "nebo vadna "; //€€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
#endif

#if defined(LANG_SLOVAK)		//slowakisch
   const unsigned char TestRunning[] MEM_TEXT = "PREBIEHA TEST"; //€€€";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "prazdna!";
   const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "suciastka!";
   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " neznama"; //€";
   const unsigned char TestFailed1[] MEM_TEXT = "Ziadna, neznama"; //€";
   const unsigned char OrBroken[] MEM_TEXT = "alebo vadna "; //€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
#endif

#if defined(LANG_SLOVENE)                    //slovenian
   const unsigned char TestRunning[] MEM_TEXT = "Testiranje..."; 
   const unsigned char BatWeak[] MEM_TEXT = "slaba!"; 
   const unsigned char BatEmpty[] MEM_TEXT = "prazna!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "pokvarjen "; 
   const unsigned char Bauteil[] MEM_TEXT = "del"; 
   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tiristor";
   const unsigned char Unknown[] MEM_TEXT = " neznan"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Ni, neznan, ali"; 
   const unsigned char OrBroken[] MEM_TEXT = "ali zanic "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
#endif
#if defined(LANG_DUTCH)                      //dutch
   const unsigned char TestRunning[] MEM_TEXT = "Testen..."; 
   const unsigned char BatWeak[] MEM_TEXT = "laag"; 
   const unsigned char BatEmpty[] MEM_TEXT = "leeg!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "defect "; 
   const unsigned char Bauteil[] MEM_TEXT = "component"; 
   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyristor";
   const unsigned char Unknown[] MEM_TEXT = " onbekend"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Geen, onbekend,"; 
   const unsigned char OrBroken[] MEM_TEXT = "of defect "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
#ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Zelftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probe";
   const unsigned char ATE[] MEM_TEXT = "Test Einde";
#endif
#endif

#if defined(LANG_RUSSIAN)                    //Russian
   const unsigned char TestRunning[] MEM_TEXT = {'T','e','c',Cyr_t,Cyr_i,'p','o',Cyr_v,'a',Cyr_n,Cyr_i,'e','.','.','.',0}; 
   const unsigned char BatWeak[] MEM_TEXT = {'c',Cyr_l,'a',Cyr_b,Cyr_y,Cyr_j,0}; 
   const unsigned char BatEmpty[] MEM_TEXT = {Cyr_p,Cyr_U,'c',Cyr_t,'o',Cyr_j,'!',0};
   const unsigned char TestFailed2[] MEM_TEXT = {Cyr_p,'o',Cyr_v,'p','e',Cyr_zsch,Cyr_d,'e',Cyr_n,' ',0};
   const unsigned char Bauteil[] MEM_TEXT = {Cyr_tsch,'a','c',Cyr_t,Cyr_hh,0}; 
   const unsigned char Diode[] MEM_TEXT = {Cyr_D,Cyr_i,'o',Cyr_d,':',0};
   const unsigned char Triac[] MEM_TEXT = {'C',Cyr_i,Cyr_m,Cyr_i,'c',Cyr_t,'o','p',0};
   const unsigned char Thyristor[] MEM_TEXT = {Cyr_t,Cyr_i,'p',Cyr_i,'c',Cyr_t,'o','p',0};
   const unsigned char Unknown[] MEM_TEXT = {' ',Cyr_n,'e',Cyr_i,Cyr_z,Cyr_v,'e','c',Cyr_t,Cyr_n,Cyr_y,Cyr_j,0}; 
   const unsigned char TestFailed1[] MEM_TEXT = {'o',Cyr_t,'c',Cyr_U,Cyr_t,'c',Cyr_t,Cyr_v,Cyr_U,'e',Cyr_t,0};
   const unsigned char OrBroken[] MEM_TEXT = {Cyr_i,Cyr_l,Cyr_i,' ',Cyr_p,'o',Cyr_v,'p','e',Cyr_zsch,Cyr_d,'e',Cyr_n,0};
   const unsigned char TestTimedOut[] MEM_TEXT = {Cyr_v,'p','e',Cyr_m,Cyr_ja,' ',Cyr_t,'e','c',Cyr_t,Cyr_i,'p','o',Cyr_v,'a',Cyr_n,Cyr_i,Cyr_ja,Cyr_v,Cyr_y,Cyr_sch,Cyr_l,'o','!',0};
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = {'o',Cyr_t,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,Cyr_i,Cyr_t,'b',' ',Cyr_z,'o',Cyr_n,Cyr_d,0};
   const unsigned char ATE[] MEM_TEXT = {Cyr_k,'o',Cyr_n,'e',Cyr_c,' ','c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,Cyr_i,'p','o',Cyr_v,'a',Cyr_n,Cyr_i,Cyr_ja,0};
 #endif
#endif

// Strings, which are not dependent of any language
 const unsigned char Bat_str[] MEM_TEXT = "Bat. ";
 const unsigned char OK_str[] MEM_TEXT = "OK";
 const unsigned char mosfet_str[] MEM_TEXT = "-MOS";
 const unsigned char jfet_str[] MEM_TEXT = "JFET";
 const unsigned char GateCap_str[] MEM_TEXT = "C=";
 const unsigned char hfe_str[] MEM_TEXT ="B=";
 const unsigned char NPN_str[] MEM_TEXT = "NPN ";
 const unsigned char PNP_str[] MEM_TEXT = "PNP ";
#ifndef EBC_STYLE
 const unsigned char N123_str[] MEM_TEXT = " 123=";
// const unsigned char N123_str[] MEM_TEXT = " Pin=";
 #else
 #if EBC_STYLE == 321
  const unsigned char N321_str[] MEM_TEXT = " 321=";
 #endif
#endif
 const unsigned char Uf_str[] MEM_TEXT = "Uf=";
 const unsigned char vt_str[] MEM_TEXT = " Vt=";
 const unsigned char Vgs_str[] MEM_TEXT = "@Vgs=";
 const unsigned char CapZeich[] MEM_TEXT = {'-',LCD_CHAR_CAP,'-',0};
 const unsigned char Cell_str[] MEM_TEXT = "Cell!";
 const unsigned char VCC_str[] MEM_TEXT = "VCC=";
#if FLASHEND > 0x1fff
 const unsigned char ESR_str[] MEM_TEXT = " ESR=";
 const unsigned char VLOSS_str[] MEM_TEXT = " Vloss=";
 const unsigned char Lis_str[] MEM_TEXT = "L=";
 #ifndef WITH_UART
  #define WITH_VEXT
 #endif
#else
 #ifndef BAT_CHECK
  #ifndef WITH_UART
   #define WITH_VEXT
  #endif
 #endif
#endif
#ifdef WITH_VEXT
 const unsigned char Vext_str[] MEM_TEXT = "Vext=";
 #define LCD_CLEAR
#endif
 const unsigned char AnKat[] MEM_TEXT = {'-', LCD_CHAR_DIODE1, '-',0};
 const unsigned char KatAn[] MEM_TEXT = {'-', LCD_CHAR_DIODE2, '-',0};
 const unsigned char Dioden[] MEM_TEXT = {'*',LCD_CHAR_DIODE1, ' ', ' ',0};
 const unsigned char Resistor_str[] MEM_TEXT = {'-', LCD_CHAR_RESIS1, LCD_CHAR_RESIS2,'-',0};
 const unsigned char VERSION_str[] MEM2_TEXT = "Version 1.07k";


#ifdef WITH_SELFTEST
 const unsigned char URefT[] MEM2_TEXT = "Ref=";
 const unsigned char RHfakt[] MEM2_TEXT = "RHf=";
 const unsigned char RH1L[] MEM_TEXT = "RH-";
 const unsigned char RH1H[] MEM_TEXT = "RH+";
 const unsigned char RLRL[] MEM_TEXT = "+RL- 12 13 23";
 const unsigned char RHRH[] MEM_TEXT = "+RH- 12 13 23";
 const unsigned char RHRL[] MEM_TEXT = "RH/RL";
 const unsigned char R0_str[] MEM2_TEXT = "R0=";
 #define LCD_CLEAR
#endif
 
#ifdef CHECK_CALL
 const unsigned char RIHI[] MEM_TEXT = "Ri_Hi=";
 const unsigned char RILO[] MEM_TEXT = "Ri_Lo=";
 const unsigned char C0_str[] MEM_TEXT = "C0 ";
 const unsigned char T50HZ[] MEM_TEXT = " 50Hz";
#endif
#ifdef AUTO_CAL
 const unsigned char MinCap_str[] MEM2_TEXT = " >100nF";
 const unsigned char REF_C_str[] MEM2_TEXT = "REF_C=";
 const unsigned char REF_R_str[] MEM2_TEXT = "REF_R=";
#endif
#ifdef DebugOut
 #define LCD_CLEAR
#endif

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
 const unsigned char CapIcon[] MEM_TEXT = { 0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x00};	// Capacitor Icon
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

 const unsigned char PinRLtab[] PROGMEM = { (1<<(TP1*2)),
				     (1<<(TP2*2)),
				     (1<<(TP3*2))};	// Table of commands to switch the  R-L resistors Pin 0,1,2

 const unsigned char PinADCtab[] PROGMEM = { (1<<TP1),
				     (1<<TP2),
				     (1<<TP3)};	// Table of commands to switch the ADC-Pins 0,1,2

#ifdef LCD_CYRILLIC	//generate Omega- and µ-character as Custom-character, because these characters are not inclluded with the cyrillic character set
   const unsigned char CyrillicOmegaIcon[] MEM_TEXT = {0,0,14,17,17,10,27,0};	//Omega
   const unsigned char CyrillicMuIcon[] MEM_TEXT = {0,17,17,17,19,29,16,16};	//µ
#endif

#ifdef AUTO_CAL
//   const uint16_t R680pl EEMEM = R_L_VAL+PIN_RP;	// total resistor to VCC
//   const uint16_t R680mi EEMEM = R_L_VAL+PIN_RM;	// total resistor to GND
   const int8_t RefDiff EEMEM = REF_R_KORR;	// correction of internal Reference Voltage
#endif
  const uint8_t PrefixTab[] MEM_TEXT = { 'p','n',LCD_CHAR_U,'m',0,'k','M'}; // p,n,u,m,-,k,M
#ifdef AUTO_CAL
//  const uint16_t cap_null EEMEM = C_NULL;	// Zero offset of capacity measurement 
   const int16_t ref_offset EEMEM = REF_C_KORR;	// default correction of internal reference voltage for capacity measurement
  // LoPin:HiPin                        2:1    3:1    1:2                    :     3:2                   1:3    2:3
   const uint8_t c_zero_tab[] EEMEM = { C_NULL,C_NULL,C_NULL+TP2_CAP_OFFSET,C_NULL,C_NULL+TP2_CAP_OFFSET,C_NULL,C_NULL }; //table of zero offsets
#endif
  const uint8_t EE_ESR_ZEROtab[] EEMEM = {ESR_ZERO, ESR_ZERO, ESR_ZERO, ESR_ZERO};	// zero offset of ESR measurement
//End of EEPROM-Strings
// Multiplier for capacity measurement with R_H (470KOhm)
  unsigned int RHmultip=DEFAULT_RH_FAKT;
#else
 // no MAIN_C
 #define COMMON extern
 #ifdef WITH_SELFTEST
  extern const unsigned char SELFTEST[] MEM_TEXT;
  extern const unsigned char RELPROBE[] MEM_TEXT;
  extern const unsigned char ATE[] MEM_TEXT;
 #endif
 #ifdef AUTO_CAL
//  extern uint16_t R680pl;
//  extern uint16_t R680mi;
  extern int8_t RefDiff;
  extern uint16_t ref_offset;
  extern uint8_t c_zero_tab[];
 #endif
  extern const uint8_t EE_ESR_ZEROtab[] EEMEM;	// zero offset of ESR measurement
  extern  const uint16_t RLtab[];

 #if FLASHEND > 0x1fff
  extern uint16_t LogTab[];
  extern const unsigned char ESR_str[];
 #endif



 #ifdef AUTO_RH
  extern const uint16_t RHtab[];
 #endif
  extern const unsigned char PinRLtab[];
  extern const unsigned char PinADCtab[];
  extern unsigned int RHmultip;
#endif


struct Diode_t {
  uint8_t Anode;
  uint8_t Cathode;
  unsigned int Voltage;
};

COMMON struct Diode_t diodes[6];
COMMON uint8_t NumOfDiodes;

COMMON struct {
  unsigned long hfe[2];		//current amplification factor 
  unsigned int uBE[2];		//B-E-voltage of the Transistor
  uint8_t b,c,e;		//pins of the Transistor
}trans;
COMMON unsigned int gthvoltage;	//Gate-threshold voltage 

COMMON uint8_t PartReady;		//part detection is finished 
COMMON uint8_t PartMode;
COMMON uint8_t tmpval, tmpval2;
COMMON unsigned int ref_mv;            //Reference-voltage  in mV units

COMMON struct resis_t{
   unsigned long rx;		// value of resistor RX  
#if FLASHEND > 0x1fff
   unsigned long lx;		// inductance 10uH or 100uH
   int8_t lpre;			// prefix for inductance
#endif
   uint8_t ra,rb;		// Pins of RX
   uint8_t rt;			// Tristate-Pin (inactive)
} resis[3];
COMMON  uint8_t ResistorsFound;	//Number of found resistors



COMMON uint8_t ii;			// multipurpose counter
COMMON struct cap_t {
  unsigned long cval;		// capacitor value 
  unsigned long cval_max;	//capacitor with maximum value
  union t_combi{
  unsigned long dw;	// capacity value without corrections
  uint16_t w[2];
  } cval_uncorrected;
#if FLASHEND > 0x1fff
  unsigned int esr;		// serial resistance of C in 0.01 Ohm
  unsigned int v_loss;		// voltage loss 0.1%
#endif
  uint8_t ca, cb;		//pins of capacitor
  int8_t cpre;			//Prefix for capacitor value  -12=p, -9=n, -6=µ, -3=m
  int8_t cpre_max;		//Prefix of the biggest capacitor
} cap;
#ifndef INHIBIT_SLEEP_MODE
 /* with sleep mode we need a global ovcnt16 */
COMMON volatile uint16_t ovcnt16;
COMMON volatile uint8_t unfinished;
#endif
COMMON int16_t load_diff;		// difference voltage of loaded capacitor and internal reference

COMMON uint8_t WithReference;		// Marker for found precision voltage reference = 1
COMMON uint8_t PartFound;	 	// the found part 
COMMON char outval[12];		// String for ASCII-outpu
COMMON uint8_t empty_count;		// counter for max count of empty measurements
COMMON uint8_t mess_count;		// counter for max count of nonempty measurements

COMMON struct ADCconfig_t {
  uint8_t Samples;		// number of ADC samples to take
  uint8_t RefFlag;		// save Reference type VCC of IntRef
  uint16_t U_Bandgap;		// Reference Voltage in mV
  uint16_t U_AVCC;		// Voltage of AVCC
} ADCconfig;

#ifdef AUTO_CAL
COMMON uint8_t pin_combination;		// coded Pin-combination  2:1,3:1,1:2,x:x,3:2,1:3,2:3
COMMON uint16_t resis680pl;	// port output resistance + 680
COMMON uint16_t resis680mi;	// port output resistance + 680
COMMON uint16_t pin_rmi;	// port output resistance to GND side, 0.1 Ohm units
COMMON uint16_t pin_rpl;	// port output resistance to VCC side, 0.1 Ohm units
#endif


#if POWER_OFF+0 > 1
COMMON unsigned int display_time;	// display time of measurement in ms units
#endif
