#include <avr/io.h>
#include "lcd-routines.h"
#include "wait1000ms.h"
#include "config.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <math.h>

/*########################################################################################
	Configuration
*/

/* Port , that is directly connected to the probes.
  This Port must have an ADC-Input  (ATmega8:  PORTC).
  The lower pins of this Port must be used for measurements.
  Please don't change the definitions of TP1, TP2 and TP3!
*/

#define ADC_PORT PORTC
#define ADC_DDR DDRC
#define ADC_PIN PINC
#define TP1 PC0
#define TP2 PC1
#define TP3 PC2


/*
  exact values of used resistors (Ohm).
  The standard value for R_L is 680 Ohm, for R_H 470kOhm.
  
  To calibrate your tester the resistor-values can be adjusted:
*/
  #define R_L_VAL 6800		// standard value 680 Ohm, multiplied by 10 for 0.1 Ohm resolution
//  #define R_L_VAL 6690          // this will be define a 669 Ohm
  #define R_H_VAL 47000		// standard value 470000 Ohm, multiplied by 10, divided by 100 
//  #define R_H_VAL 47900		// this will be define a 479000 Ohm, divided by 100 

#ifndef PIN_RP
  #define PIN_RP  220		//estimated internal resistance PORT to VCC
				//will only be used, if not set before in config.h
#endif
#ifndef PIN_RM
  #define PIN_RM  190		//estimated internal resistance PORT to GND
				//will only be used, if not set before in config.h
#endif


  /* The voltage at a capacitor grows with  Uc = VCC * (1 - e**(-t/T))
  The voltage 1.3V is reached at  t = -ln(3.7/5)*T  = 0.3011*T . 
  Time constant is  T = R * C ; also
  C = T / R
  for the resistor 470 kOhm  is C = t / (0.3011 * 470000)
  H_Fakt = 707/100 for a result in pF units.

  */
//#define H_CAPACITY_FACTOR  724    //replaced with RHmultip

// Big Capacities (>50µF) are measured with up to 500 load-pulses with the 680 Ohm resistor.
// Each  of this load-puls has an length of 10ms. After every load-pulse the voltage of the
// capacitor is measured. If the voltage is more than 300mV, the capacity is computed by
// interpolating the corresponding values of the table RLtab and multiply that with the number
// of load pulses (*10).

// Widerstand 680 Ohm                300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
const uint16_t RLtab[] MEM_TEXT = {22447,20665,19138,17815,16657,15635,14727,13914,13182,12520,11918,11369,10865,10401, 9973, 9577, 9209, 8866, 8546, 8247, 7966, 7702, 7454, 7220, 6999, 6789, 6591, 6403, 6224, 6054, 5892, 5738, 5590, 5449, 5314, 5185, 5061, 4942, 4828, 4718, 4613, 4511, 4413, 4319, 4228};


#ifdef WITH_AUTO_REF

//resistor   470000 Ohm      1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
const uint16_t RHtab[] MEM_TEXT = { 954, 903, 856, 814, 775, 740, 707, 676, 648};

#endif


// with integer factors the ADC-value will be changed to mV resolution in ReadADC !
// all if statements are corrected to the mV resolution.
#define U_VCC 5001		// maximum of the ReadADC routine 


/*########################################################################################
End of configuration 
*/

/*Strings in PROGMEM or in EEprom
*/

#if defined(LANG_GERMAN)		//deutsch
  const unsigned char TestRunning[] MEM_TEXT = "Testen..."; //€€€€€€€";
  const unsigned char BatWeak[] MEM_TEXT = "schwach";
  const unsigned char BatEmpty[] MEM_TEXT = "leer!"; //€€€";
  const unsigned char TestFailed2[] MEM_TEXT = "defektes "; //€€€";
  const unsigned char Bauteil[] MEM_TEXT = "Bauteil"; //€€€";
  const unsigned char Diode[] MEM_TEXT = "Diode: ";
  const unsigned char GAK[] MEM_TEXT = "GAK=";
  const unsigned char Triac[] MEM_TEXT = "Triac";
  const unsigned char Thyristor[] MEM_TEXT = "Thyristor";
  const unsigned char Unknown[] MEM_TEXT = " unbek."; //€€";
  const unsigned char TestFailed1[] MEM_TEXT = "Kein,unbek. oder";
  const unsigned char OrBroken[] MEM_TEXT = "oder defekt "; //€";
#endif

#if defined(LANG_ENGLISH)		//english
  const unsigned char TestRunning[] MEM_TEXT = "Testing..."; //€€€€€€";
  const unsigned char BatWeak[] MEM_TEXT = "weak"; //€€€";
  const unsigned char BatEmpty[] MEM_TEXT = "empty!"; //€€";
  const unsigned char TestFailed2[] MEM_TEXT = "damaged "; //€€€€";
  const unsigned char Bauteil[] MEM_TEXT = "part"; //€€€€€€";
  const unsigned char Diode[] MEM_TEXT = "Diode: ";
  const unsigned char GAK[] MEM_TEXT = "GAC=";
  const unsigned char Triac[] MEM_TEXT = "Triac";
  const unsigned char Thyristor[] MEM_TEXT = "Thyristor";
  const unsigned char Unknown[] MEM_TEXT = " unknown"; //€";
  const unsigned char TestFailed1[] MEM_TEXT = "No, unknown, or"; //€";
  const unsigned char OrBroken[] MEM_TEXT = "or damaged "; //€€";
#endif

#if defined(LANG_POLISH)		//polnisch
  const unsigned char TestRunning[] MEM_TEXT = "Testowanie..."; //€€€";
  const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
  const unsigned char BatEmpty[] MEM_TEXT = "za slaba";
  const unsigned char TestFailed2[] MEM_TEXT = "lub uszkodz.";
  const unsigned char Bauteil[] MEM_TEXT = "Elemen"; //t€€€";
  const unsigned char Diode[] MEM_TEXT = "Dioda: ";
  const unsigned char GAK[] MEM_TEXT = "GAK=";
  const unsigned char Triac[] MEM_TEXT = "Triak";
  const unsigned char Thyristor[] MEM_TEXT = "Tyrystor"; //€";
  const unsigned char Unknown[] MEM_TEXT = " nieznany";
  const unsigned char TestFailed1[] MEM_TEXT = "brak elementu"; //€€€";
  const unsigned char OrBroken[] MEM_TEXT = "lub uszkodz. ";
#endif

#if defined(LANG_CZECH)		//Tschechisch
  const unsigned char TestRunning[] MEM_TEXT = "Probiha mereni..";
  const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
  const unsigned char BatEmpty[] MEM_TEXT = "prazdna!";
  const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
  const unsigned char Bauteil[] MEM_TEXT = "soucastka"; //€";
  const unsigned char Diode[] MEM_TEXT = "Dioda: ";
  const unsigned char GAK[] MEM_TEXT = "GAK=";
  const unsigned char Triac[] MEM_TEXT = "Triak";
  const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
  const unsigned char Unknown[] MEM_TEXT = " neznama"; //€";
  const unsigned char TestFailed1[] MEM_TEXT = "Zadna, neznama"; //€€";
  const unsigned char OrBroken[] MEM_TEXT = "nebo vadna "; //€€";
#endif

#if defined(LANG_SLOVAK)		//slowakisch
  const unsigned char TestRunning[] MEM_TEXT = "PREBIEHA TEST"; //€€€";
  const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
  const unsigned char BatEmpty[] MEM_TEXT = "prazdna!";
  const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
  const unsigned char Bauteil[] MEM_TEXT = "suciastka!";
  const unsigned char Diode[] MEM_TEXT = "Dioda: ";
  const unsigned char GAK[] MEM_TEXT = "GAK=";
  const unsigned char Triac[] MEM_TEXT = "Triak";
  const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
  const unsigned char Unknown[] MEM_TEXT = " neznama"; //€";
  const unsigned char TestFailed1[] MEM_TEXT = "Ziadna, neznama"; //€";
  const unsigned char OrBroken[] MEM_TEXT = "alebo vadna "; //€";
#endif

#if defined(LANG_SLOVENE)                    //slovenian
  const unsigned char TestRunning[] MEM_TEXT = "Testiranje..."; 
  const unsigned char BatWeak[] MEM_TEXT = "slaba!"; 
  const unsigned char BatEmpty[] MEM_TEXT = "prazna!"; 
  const unsigned char TestFailed2[] MEM_TEXT = "pokvarjen "; 
  const unsigned char Bauteil[] MEM_TEXT = "del"; 
  const unsigned char Diode[] MEM_TEXT = "Dioda: ";
  const unsigned char GAK[] MEM_TEXT = "GAC=";
  const unsigned char Triac[] MEM_TEXT = "Triak";
  const unsigned char Thyristor[] MEM_TEXT = "Tiristor";
  const unsigned char Unknown[] MEM_TEXT = " neznan"; 
  const unsigned char TestFailed1[] MEM_TEXT = "Ni, neznan, ali"; 
  const unsigned char OrBroken[] MEM_TEXT = "ali zanic "; 
#endif


// Strings, which are not dependent of any language
const unsigned char Bat[] MEM_TEXT = "Bat. ";
const unsigned char OK[] MEM_TEXT = "OK";
const unsigned char mosfet[] MEM_TEXT = "-MOS ";
const unsigned char jfet[] MEM_TEXT = "JFET";
const unsigned char GateCap[] MEM_TEXT = "C=";
const unsigned char hfestr[] MEM_TEXT ="B=";
const unsigned char NPN[] MEM_TEXT = "NPN ";
const unsigned char PNP[] MEM_TEXT = "PNP ";
const unsigned char ebcstr[] MEM_TEXT = " EBC=";
const unsigned char gds[] MEM_TEXT = "GDS=";
const unsigned char Uf[] MEM_TEXT = "Uf=";
const unsigned char vt[] MEM_TEXT = "Vt=";
#ifdef C_MESS
const unsigned char CapZeich[] MEM_TEXT = {'-',LCD_CHAR_CAP,'-',0};
#endif
const unsigned char AnKat[] MEM_TEXT = {'-', LCD_CHAR_DIODE1, '-',0};
const unsigned char KatAn[] MEM_TEXT = {'-', LCD_CHAR_DIODE2, '-',0};
const unsigned char Dioden[] MEM_TEXT = {'*',LCD_CHAR_DIODE1, ' ', ' ',0};
#ifdef R_MESS
const unsigned char Resistor_str[] MEM_TEXT = {'-', LCD_CHAR_RESIS1, LCD_CHAR_RESIS2,'-',0};
#endif
const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
const unsigned char VERSION[] MEM_TEXT = "Version 0.99k";

#ifdef WITH_SELFTEST
const unsigned char mVT[] MEM_TEXT = "mV ";
const unsigned char URefT[] MEM_TEXT = "Ref=";
const unsigned char RHfakt[] MEM_TEXT = "RHf=";
const unsigned char RLfakt[] MEM_TEXT = " RLf=";
const unsigned char ATE[] MEM_TEXT = "Selftest End";
const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
const unsigned char RH1L[] MEM_TEXT = "RH-";
const unsigned char RH1H[] MEM_TEXT = "RH+";
const unsigned char RILO[] MEM_TEXT = "Ri_Lo= (mV)";
 #ifdef AUTO_CAL
const unsigned char RIHI[] MEM_TEXT = "Ri_Hi=";
 #else
const unsigned char RIHI[] MEM_TEXT = "Ri_Hi= (mV)";
 #endif
const unsigned char RLRL[] MEM_TEXT = "+RL- 12 13 23";
const unsigned char RHRH[] MEM_TEXT = "+RH- 12 13 23";
const unsigned char RELPROBE[] MEM_TEXT = "isolate probe";
 #define MULTIP
 #define LCD_CLEAR
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
#ifdef C_MESS
const unsigned char CapIcon[] MEM_TEXT = { 0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x1b,
				     0x00};	// Capacitor Icon
#endif
#ifdef R_MESS
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
#endif

const unsigned char PinRLtab[] MEM_TEXT = { (1<<(TP1*2)),
				     (1<<(TP2*2)),
				     (1<<(TP3*2))};	// Table of commands to switch the  R-L resistors Pin 0,1,2

const unsigned char PinADCtab[] MEM_TEXT = { (1<<TP1),
				     (1<<TP2),
				     (1<<TP3)};	// Table of commands to switch the ADC-Pins 0,1,2

#ifdef LCD_CYRILLIC	//generate Omega- and µ-character as Custom-character, because these characters are not inclluded with the cyrillic character set
  const unsigned char CyrillicOmegaIcon[] MEM_TEXT = {0,0,14,17,17,10,27,0};	//Omega
  const unsigned char CyrillicMuIcon[] MEM_TEXT = {0,17,17,17,19,29,16,16};	//µ
#endif

#ifdef AUTO_CAL
  const uint16_t R680pl EEMEM = R_L_VAL+PIN_RP;	// total resistor to VCC
  const uint16_t R680mi EEMEM = R_L_VAL+PIN_RM;	// total resistor to GND
#endif
//End of EEPROM-Strings

//Watchdog
#define WDT_enabled
/* If you remove the "#define WDT_enabled" , the Watchdog will not be activated.
  This is only for Test or debugging usefull.
 For normal operation please activate the Watchdog !
*/


struct Diode {
  uint8_t Anode;
  uint8_t Cathode;
  unsigned int Voltage;
};

void CheckPins(uint8_t HighPin, uint8_t LowPin, uint8_t TristatePin);
void ChargePin10ms(uint8_t PinToCharge, uint8_t ChargeDirection);
unsigned int ReadADC(uint8_t mux);		// read Routine for ADC
unsigned int W5msReadADC(uint8_t mux);		// wait 5ms and read than the ADC
unsigned int W20msReadADC(uint8_t mux);		// wait 5ms and read then ADC
void lcd_show_format_cap(void);
void ReadCapacity(uint8_t HighPin, uint8_t LowPin);	//capacity measurement
void UfAusgabe(uint8_t bcdchar);	// Output of the threshold voltage(s) Uf
void mVAusgabe(uint8_t nn);		// Output of the theshold voltage for Diode nn 
void RvalOut(uint8_t ii);		// Output of the resistore value(s)
void EntladePins();			// discharge capacitors
void RefVoltage();			// compensate the reference voltage for comparator 
void AutoCheck();			// check if self-test should be done 
unsigned int getRLmultip(unsigned int cvolt);  // get C-Multiplikator for voltage cvolt
void scale_intref_adc();		// get scale factors for ReadADC with internal reference
uint8_t value_out(unsigned long vval,uint8_t pp);    // output 4 digits with (pp-1) digits after point
unsigned int compute_hfe(unsigned int lpx, unsigned int tpy);

#define R_DDR DDRB
#define R_PORT PORTB

/* Port for the Test resistors
  The Resistors must be connected to the lower 6 Pins of the Port in following sequence:
  RLx = 680R-resistor for Test-Pin x
  RHx = 470k-resistor for Test-Pin x

  RL1 an Pin 0
  RH1 an Pin 1
  RL2 an Pin 2
  RH2 an Pin 3
  RL3 an Pin 4
  RH3 an Pin 5

*/



#define ON_DDR DDRD
#define ON_PORT PORTD
#define ON_PIN_REG PIND
#define ON_PIN PD6	//Pin, must be switched to high to swtch power on
#define RST_PIN PD7	//Pin, is swiched to low, if push button is pressed

//definitions of parts
#define PART_NONE 0
#define PART_DIODE 1
#define PART_TRANSISTOR 2
#define PART_FET 3
#define PART_TRIAC 4
#define PART_THYRISTOR 5
#define PART_RESISTOR 6
#define PART_CAPACITOR 7
#define PART_CELL 8

//End (parts)
//special definition for different parts 
//FETs
#define PART_MODE_N_E_MOS 1
#define PART_MODE_P_E_MOS 2
#define PART_MODE_N_D_MOS 3
#define PART_MODE_P_D_MOS 4
#define PART_MODE_N_JFET 5
#define PART_MODE_P_JFET 6

//Bipolar
#define PART_MODE_NPN 1
#define PART_MODE_PNP 2


struct Diode diodes[6];
uint8_t NumOfDiodes;

struct {
  unsigned int hfe[2];		//current amplification factor 
  //unsigned int uBE[2];	//B-E-voltage of the Transistor
  uint8_t b,c,e;		//pins of the Transistor
}trans;
unsigned int gthvoltage;	//Gate-threshold voltage 

uint8_t PartReady;		//part detection is finished 
uint8_t PartMode;
uint8_t tmpval, tmpval2;
unsigned int ref_mv;		//Reference-voltage  in mV units

#ifdef R_MESS
struct resis_t{
   unsigned long rx;		// value of resistor RX  
   uint8_t ra,rb;		// Pins of RX
   uint8_t rt;			// Tristate-Pin (inactive)
} resis[3];
 unsigned int rxv, rtst;
 unsigned int rvmax;
 uint8_t ResistorsFound;	//Number of found resistors
#endif


#ifdef C_MESS 
  const unsigned char C_Prefix_tab[] PROGMEM = { 'p','n',LCD_CHAR_U,'m'}; // pF,nF,µF,mF
 #ifdef AUTO_CAL
  const uint16_t cap_null EEMEM = C_NULL;	// Zero offset of capacity measurement 
  const int16_t ref_offset EEMEM = REF_C_KORR;	// default correction of internal reference voltage for capacity measurement
 #endif
  #define MULTIP
#endif
#ifdef MULTIP
// Multiplier for capacity measurement with R_H (470KOhm)
  unsigned int RHmultip=DEFAULT_RH_FAKT;
#endif

#ifdef AUTOSCALE_ADC
uint8_t minmul=1,mindiv=1;
#endif

uint8_t ii;			// multipurpose counter
unsigned long cval;		// capacitor value 
unsigned long cval_uncorrected;	// capacity value without corrections
int16_t load_diff;		// difference voltage of loaded capacitor and internal reference
uint8_t cpre;			//Prefix for capacitor value  0=p, 1=n, 2=µ, 3=m
uint8_t ca, cb;			//pins of capacitor

uint8_t PartFound;	 	// the found part 
char outval[12];		// String for ASCII-outpu
uint8_t empty_count;		// counter for max count of empty measurements
uint8_t mess_count;		// counter for max count of nonempty measurements
#if POWER_OFF+0 > 1
unsigned int display_time;	// display time of measurement in ms units
#endif
