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
#if R_L_VAL == 3300
// resistor   330 Ohm                       300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
COMMON const uint16_t RLtab[] MEM_TEXT = {43680,40214,37242,34667,32414,30425,28657,27076,25652,24364,23192,22123,21142,20240,19407,18636,17920,17253,16630,16048,15501,14988,14505,14049,13619,13212,12826,12460,12112,11781,11466,11165,10878,10603,10341,10089, 9848, 9617, 9395, 9181, 8976, 8778, 8588, 8404, 8227};
#else

// resistor   680 Ohm                       300   325   350   375   400   425   450   475   500   525   550   575   600   625   650   675   700   725   750   775   800   825   850   875   900   925   950   975  1000  1025  1050  1075  1100  1125  1150  1175  1200  1225  1250  1275  1300  1325  1350  1375  1400  mV
COMMON const uint16_t RLtab[] MEM_TEXT = {22447,20665,19138,17815,16657,15635,14727,13914,13182,12520,11918,11369,10865,10401, 9973, 9577, 9209, 8866, 8546, 8247, 7966, 7702, 7454, 7220, 6999, 6789, 6591, 6403, 6224, 6054, 5892, 5738, 5590, 5449, 5314, 5185, 5061, 4942, 4828, 4718, 4613, 4511, 4413, 4319, 4228};
#endif

#if FLASHEND > 0x1fff
//                                        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91 };
COMMON const uint16_t LogTab[] PROGMEM = {0, 20, 41, 62, 83, 105, 128, 151, 174, 198, 223, 248, 274, 301, 329, 357, 386, 416, 446, 478, 511, 545, 580, 616, 654, 693, 734, 777, 821, 868, 916, 968, 1022, 1079, 1139, 1204, 1273, 1347, 1427, 1514, 1609, 1715, 1833, 1966, 2120, 2303, 2526 };

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
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyrist.";
   const unsigned char Unknown[] MEM_TEXT = " unbek."; //€€";
   const unsigned char TestFailed1[] MEM_TEXT = "Kein,unbek. oder";
   const unsigned char OrBroken[] MEM_TEXT = "oder defekt "; //€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selbsttest ..";
   const unsigned char RELPROBE[] MEM_TEXT = "Trenne Pins!";
   const unsigned char ATE[] MEM_TEXT = "Test Ende";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Auswahl:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequenz";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Spannung";
   const unsigned char SHOW_str[] MEM2_TEXT = "Zeige Daten";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Schalte aus";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Impulsdrehgeber";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','K','o','r','r','e','k','t','u','r',0};
   const unsigned char TURN_str[] MEM2_TEXT = "drehen!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selbsttest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Verbinde Pins!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequenz > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrast";
  #endif
 #endif
 #define LANG_SELECTED
#endif
#if defined(LANG_ITALIAN)		//italiano
   const unsigned char TestRunning[] MEM_TEXT = "Analizzo..."; 
   const unsigned char BatWeak[] MEM_TEXT = "debole";
   const unsigned char BatEmpty[] MEM_TEXT = "esaurita!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "guasto ";	//"danneggiato "; 
   const unsigned char Bauteil[] MEM_TEXT = "campione";		//"componente"; 
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "SCR";
   const unsigned char Unknown[] MEM_TEXT = " ignoto";		//" sconosciuto.";
   const unsigned char TestFailed1[] MEM_TEXT = "Test fallito, o";
   const unsigned char OrBroken[] MEM_TEXT = "o guasto ";	//"o danneggiato "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'	/* defines the character used for cathode */
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Autotest ..";
   const unsigned char RELPROBE[] MEM_TEXT = "Pin separati!";
   const unsigned char ATE[] MEM_TEXT = "Fine del test";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Selezione:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequenza";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltaggio";
   const unsigned char SHOW_str[] MEM2_TEXT = "Mostra dati";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Spegnere";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generatore";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encoder rotativo";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','C','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "turno!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autotest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Pin collegare!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequenza > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF quarzo";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF quarzo";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contrasto";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_POLISH)		//polnisch
   const unsigned char TestRunning[] MEM_TEXT = "Testowanie..."; //€€€";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "za slaba";
   const unsigned char TestFailed2[] MEM_TEXT = "lub uszkodzony";
   const unsigned char Bauteil[] MEM_TEXT = " "; //t€€€";                       // don't fit on display, besides i think word is unnecesary repeated in second row after tests
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyrystor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " nieznany";
   const unsigned char TestFailed1[] MEM_TEXT = "Brak elementu"; //€€€";
   const unsigned char OrBroken[] MEM_TEXT = "lub uszkodzony";
   const unsigned char TestTimedOut[] MEM_TEXT = "Przekr. czasu!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Tryb auto-test";
   const unsigned char RELPROBE[] MEM_TEXT = "Rozlacz piny!";
   const unsigned char ATE[] MEM_TEXT = "Koniec testu";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "MENU:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzystor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Czestotliwosc";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napiecie";
   const unsigned char SHOW_str[] MEM2_TEXT = "Pokaz dane";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Wylacz";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Enkoder obrotowy";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','c','o','r','e','c','t','a',0};
   const unsigned char TURN_str[] MEM2_TEXT = "Pokrec!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Auto-test";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Polacz Piny!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Czestotliwosc > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kwarc";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kwarc";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrast";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_CZECH)		//Tschechisch
   const unsigned char TestRunning[] MEM_TEXT = "Probiha mereni..";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "vybita";
   const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "soucastka"; //€";
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " neznama"; //€";
   const unsigned char TestFailed1[] MEM_TEXT = "Zadna, neznama"; //€€";
   const unsigned char OrBroken[] MEM_TEXT = "nebo vadna "; //€€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Autotest..";	//"Samotestovani..";
   const unsigned char RELPROBE[] MEM_TEXT = "Izoluj sondy!";
   const unsigned char ATE[] MEM_TEXT = "Konec testu";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Vyber:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvence";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napeti";
   const unsigned char SHOW_str[] MEM2_TEXT = "zobrazit udaje";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnout";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotacni koder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','k','o','r','e','c','t','u','r','a',0};
   const unsigned char TURN_str[] MEM2_TEXT = "otocte!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autotest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "propoj sondy!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvence > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrast";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_SLOVAK)		//slowakisch
   const unsigned char TestRunning[] MEM_TEXT = "Testujem ..."; //"Prebieha meranie";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "vybita";		//"prazdna!";
   const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "suciastka!";
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; 
   const unsigned char Unknown[] MEM_TEXT = " neznama"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Ziadna, neznama,"; 
   const unsigned char OrBroken[] MEM_TEXT = "alebo vadna "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Prekroceny cas!"; //"Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Kalibracia";	//"Samotestovanie..";
   const unsigned char RELPROBE[] MEM_TEXT = "Odpoj sondy!";
   const unsigned char ATE[] MEM_TEXT = "Koniec testu";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Vyber:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tester";	//"Tranzistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvencia";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napetie";
   const unsigned char SHOW_str[] MEM2_TEXT = "Informacie";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnut";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotacny koder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','K','o','r','e','k','c','i','a',0};
   const unsigned char TURN_str[] MEM2_TEXT = "Krut!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Plna kalibracia";	//"Samotestovanie";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Prepoj sondy!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF krystal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF krystal";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrast";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_SLOVENE)                    //slovenian
   const unsigned char TestRunning[] MEM_TEXT = "Testiranje..."; 
   const unsigned char BatWeak[] MEM_TEXT = "slaba!"; 
   const unsigned char BatEmpty[] MEM_TEXT = "prazna!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "pokvarjen "; 
   const unsigned char Bauteil[] MEM_TEXT = "del"; 
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tiristor";
   const unsigned char Unknown[] MEM_TEXT = " neznan"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Ni, neznan, ali"; 
   const unsigned char OrBroken[] MEM_TEXT = "ali zanic "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probes!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Izbor:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvenca";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "Prikazi podatke";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "izklopi";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','k','o','r','r','e','k','c','i','j','a',0};
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvenca > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kristal";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrast";
  #endif
 #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_DUTCH)                      //dutch
   const unsigned char TestRunning[] MEM_TEXT = "Testen..."; 
   const unsigned char BatWeak[] MEM_TEXT = "laag"; 
   const unsigned char BatEmpty[] MEM_TEXT = "leeg!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "defect "; 
   const unsigned char Bauteil[] MEM_TEXT = "component"; 
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyrist.";
   const unsigned char Unknown[] MEM_TEXT = " onbekend"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Geen, onbekend,"; 
   const unsigned char OrBroken[] MEM_TEXT = "of defect "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Zelftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "aparte Pinnen!";
   const unsigned char ATE[] MEM_TEXT = "Test Einde";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Selectie:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequentie";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Spanning";
   const unsigned char SHOW_str[] MEM2_TEXT = "toon gegevens";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "uitschakelen";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotary encoder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','c','o','r','r','e','c','t','i','e',0};
   const unsigned char TURN_str[] MEM2_TEXT = "rotaren!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Zelftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "verbind Pinnen!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequentie > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kristal";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contrast";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_BRASIL)					// Brazilian Portuguese
   const unsigned char TestRunning[] MEM_TEXT = "Testando...";	//€€€€€€";
   const unsigned char BatWeak[] MEM_TEXT = "fraca"; 		//€€€"; + 2
   const unsigned char BatEmpty[] MEM_TEXT = "acabou!"; 	//€€"; +2
   const unsigned char TestFailed2[] MEM_TEXT = "ou danificada..."; 	//€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "peca"; 		//€€€€€€";
//   const unsigned char Diode[] MEM_TEXT = "Diodo: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Tiristor";
   const unsigned char Unknown[] MEM_TEXT = " O que? "; 	//€";
   const unsigned char TestFailed1[] MEM_TEXT = "Sem peca, ruim"; //€";
   const unsigned char OrBroken[] MEM_TEXT = "danificada "; 	//€€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Tempo Esgotado!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Autoteste...";
   const unsigned char RELPROBE[] MEM_TEXT = "Isole Pontas!";
   const unsigned char ATE[] MEM_TEXT = "Fim do Teste";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Selecao:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequencia";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltagem";
   const unsigned char SHOW_str[] MEM2_TEXT = "mostram dados";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "desconectar";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encoder rotativo";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','C','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "rodar!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autoteste";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "conecte Pontas!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF cristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF cristal";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contraste";
  #endif
 #endif
 #define LANG_SELECTED
#endif
#if defined(LANG_SPANISH)
   const unsigned char TestRunning[] MEM_TEXT = "Testeando...";	
   const unsigned char BatWeak[] MEM_TEXT = "Poca bateria"; 	
   const unsigned char BatEmpty[] MEM_TEXT = "Sin bateria"; 	
   const unsigned char TestFailed2[] MEM_TEXT = "Pieza "; 	
   const unsigned char Bauteil[] MEM_TEXT = "mal..."; 		
//   const unsigned char Diode[] MEM_TEXT = "Diodo: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Tiristor";
   const unsigned char Unknown[] MEM_TEXT = "No se ?"; 	
   const unsigned char TestFailed1[] MEM_TEXT = "Componente mal"; 
   const unsigned char OrBroken[] MEM_TEXT = " o roto "; 
   const unsigned char TestTimedOut[] MEM_TEXT = "Tiempo agotado!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Calibracion...";
   const unsigned char RELPROBE[] MEM_TEXT = "Aislar Puntas!";
   const unsigned char ATE[] MEM_TEXT = "Fin de Cal.";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Seleccion:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequencia";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "mostrar datos";	
   const unsigned char OFF_str[] MEM2_TEXT = "desconectar";
   const unsigned char F_GEN_str[] MEM2_TEXT = "F-Generador";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encoder rotativo";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','C','o','r','r','e','c','c','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "rotar!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Calibracion...";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Cortocircuite las puntas!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF cristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF cristal";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contraste";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#include "Ru-Ua-TransistorTester.txt"   /* Russian and Ukrainian language */ 

#if defined(LANG_HUNGARIAN)     //HUNGARIAN
   const unsigned char TestRunning[] MEM_TEXT = "Meres..."; 
   const unsigned char BatWeak[] MEM_TEXT = "gyenge"; 
   const unsigned char BatEmpty[] MEM_TEXT = "ures!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "hibas "; 
   const unsigned char Bauteil[] MEM_TEXT = "alkatresz"; 
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tiriszt.";
   const unsigned char Unknown[] MEM_TEXT = " unknown"; // " ismeretlen"
   const unsigned char TestFailed1[] MEM_TEXT = "hibas vagy"; 
   const unsigned char OrBroken[] MEM_TEXT = "nincs alkatresz ";
   const unsigned char TestTimedOut[] MEM_TEXT = "Idotullepes!";

   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "On-teszt mod..";
   const unsigned char RELPROBE[] MEM_TEXT = "Rovidzar ki!"; // or "separate pins!";
   const unsigned char ATE[] MEM_TEXT = "Teszt vege";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Valasztas:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzisztor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvencia";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "adatokat mutat";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "kikapcsol";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotary encoder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','C','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "forgat!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "On-teszt mod";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";	// ???
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontraszt";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_LITHUANIAN) //Lithuanian
   const unsigned char TestRunning[] MEM_TEXT = "Tikrinu..."; //"Testing...";
   const unsigned char BatWeak[] MEM_TEXT = "silpna"; //"weak";
   const unsigned char BatEmpty[] MEM_TEXT = "negera"; //"empty!"
   const unsigned char TestFailed2[] MEM_TEXT = "sugedusi "; //"damaged ";
   const unsigned char Bauteil[] MEM_TEXT = "dalis"; //"part";
// const unsigned char Diode[] MEM_TEXT = "Diodas: ";
   const unsigned char Triac[] MEM_TEXT = "Simistorius"; //"Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Tirist."; //"Thyristor";
   const unsigned char Unknown[] MEM_TEXT = " nezinoma"; //" unknown";
   const unsigned char TestFailed1[] MEM_TEXT = "Nezinoma arba"; //"Pajunkite detale" "No, unknown, or";
   const unsigned char OrBroken[] MEM_TEXT = "sugedusi "; //"or damaged";
   const unsigned char TestTimedOut[] MEM_TEXT = "Viso gero!"; //"Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Testuoju.."; //"Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "Atjung laidus!"; //"isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Testas baigtas"; //"Test End";
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Pasirinkimas:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistorius";
   const unsigned char FREQ_str[] MEM2_TEXT = "Daznis";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "Rodyti informacija";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "isjungti";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','k','o','r','e','k','c','i','j','a',0};
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";      //???
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Testuoju";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";	// ???

  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Daznis > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kvarcas";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kvarcas";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrastas";
  #endif
 #endif
 #endif
 #define LANG_SELECTED
#endif

// All languages must be placed before the english text.
// If none is actually selected, LANG_SELECTED is not set.
#ifndef LANG_SELECTED		//default language is english
   const unsigned char TestRunning[] MEM_TEXT = "Testing..."; //€€€€€€";
   const unsigned char BatWeak[] MEM_TEXT = "weak"; //€€€";
   const unsigned char BatEmpty[] MEM_TEXT = "empty!"; //€€";
   const unsigned char TestFailed2[] MEM_TEXT = "damaged "; //€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "part"; //€€€€€€";
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyrist.";
   const unsigned char Unknown[] MEM_TEXT = " unknown"; //€";
   const unsigned char TestFailed1[] MEM_TEXT = "No, unknown, or"; //€";
   const unsigned char OrBroken[] MEM_TEXT = "or damaged "; //€€";
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
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequency";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "Show data";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Switch off";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','c','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";	
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequency > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF quartz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF quartz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contrast";
  #endif
 #endif
#endif


// Strings, which are not dependent of any language
 const unsigned char Bat_str[] MEM_TEXT = "Bat. ";
 const unsigned char OK_str[] MEM_TEXT = "OK";
#if FLASHEND > 0x1fff
 const unsigned char DC_Pwr_Mode_str[] MEM_TEXT = "DC Pwr Mode";
#endif
 const unsigned char mosfet_str[] MEM_TEXT = "-MOS";
 const unsigned char jfet_str[] MEM_TEXT = "JFET";
 const unsigned char igbt_str[] MEM_TEXT = "-IGBT";
#if ((!defined(WITH_GRAPHICS) && (LCD_LINE_LENGTH > 17)) || (defined(WITH_GRAPHICS) && (LOW_H_SPACE != 1)))
 const unsigned char hfe_str[] MEM_TEXT ="hFE=";
 const unsigned char GateCap_str[] MEM_TEXT = "Cg=";
#else
 const unsigned char hfe_str[] MEM_TEXT ="B=";
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
 #if defined(LANG_GERMAN)		//deutsch
 const unsigned char HelpCalibration_str[] PROGMEM = {'U','n','k','a','l','i','b','r','i','e','r','t','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'Z','u','m',LCD_CHAR_INSEP,
'K','a','l','i','b','r','i','e','r','e','n',' ',

'k','l','e','m','m','e',' ','3',LCD_CHAR_INSEP,'P','i','n','n','e',' ',
'z','u','s','a','m','m','e','n',' ','u','n','d',' ',

's','t','a','r','t','e',' ','m','i','t',' ','d','e','r',' ',
'T','a','s','t','e','.',' ',

'B','e','s','t','a','e','t','i','g','e',' ','d','e','n',' ',
'S','e','l','b','s','t','t','e','s','t',' ','m','i','t',' ',

'd','e','r',' ','T','a','s','t','e',' ','i','n',' ','2','s','.',' ',
'T','r','e','n','n','e',' ','d','i','e',' ','P','i','n','n','e',
   #ifdef TPCAP
'.',' ',
   #else
' ','e','r','s','t',' ','n','a','c','h',' ','d','e','r',' ',
'M','e','l','d','u','n','g',' ',

'"','T','r','e','n','n','e',LCD_CHAR_INSEP,'P','i','n','s','!','"',' ',
'u','n','d',' ','s','c','h','l','i','e','s','s','e',' ',

'e','r','s','t',' ', 'n','a','c','h',' ','d','e','r',' ',
'M','e','l','d','u','n','g',' ',

'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',
'e','i','n','e','n',' ','g','u','t','e','n',' ',

'K','o','n','d','e','n','s','a','t','o','r',' ',
'm','i','t',' ','m','e','h','r',' ','a','l','s',' ','1','0','0','n','F',' ',

'a','n',' ','P','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'+',LCD_CHAR_INSEP,'3',' ','a','n','.',' ',
   #endif
'S','o','f','t','w','a','r','e',' ','u','n','d',' ',

'D','o','k','u','m','e','n','t','a','t','i','o','n',' ',
'i','s','t',' ','a','u','f',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',
'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' '
  #endif   /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif

 #if defined(LANG_CZECH)	//Tschechisch
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','e','z','k','a','l','i','b','r','o','v','a','n','o','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'P','r','o',LCD_CHAR_INSEP, 'K','a','l','i','b','r','a','c','i',' ',

'z','k','r','a','t','u','j',' ','v','s','e','c','h','n','y',' ',
'3',' ','m','e','r','i','c','i',' ','b','o','d','y',' ','a',' ',

's','p','u','s','t',' ','s','t','i','s','k','e','m',' ',
'T','E','S','T',LCD_CHAR_INSEP,'t','l','a','c','i','t','k','a',' ',

'K','a','l','i','b','r','a','c','i',' ','p','o','t','v','r','d',' ',
'd','a','l','s','i','m',' ','s','t','i','s','k','e','m',' ',

't','l','a','c','i','t','k','a',' ','v',' ','d','o','b','e',' ',
'k','r','a','t','s','i',' ','2',LCD_CHAR_INSEP,'s','e','k','u','n','d',' ',

'p','r','i',LCD_CHAR_INSEP,'h','l','a','s','c','e',' ',' ',
'"','i','z','o','l','u','j',LCD_CHAR_INSEP,'s','o','n','d','y','!','"',' ',

'O','d','s','t','r','a','n',' ','z','k','r','a','t',' ',
'm','e','r','i','c','i','c','h',' ','b','o','d','u',' ',

'P','r','i',' ','z','o','b','r','a','z','e','n','i',' ',
'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'v',LCD_CHAR_INSEP,'p','o','s','l','e','d','n','i','m',' ',
'k','r','o','k','u',' ', 'k','a','l','i','b','r','a','c','e',' ',

'p','r','i','p','o','j',' ','k','v','a','l','i','t','n','i',' ',
'k','o','n','d','i','k',' ','v','e','t','s','i',' ','n','e','z',' ',

'1','0','0','n','F',' ','m','e','z','i',' ','b','o','d','y',' ',
'1','a','3',' ',
'A','V','R',' ','t','e','s','t','e','r','u',' ',

'd','a','l','s','i',' ','i','n','f','o','r','m','a','c','e',' ',
'o',' ','s','o','f','t','w','a','r','e',' ','a',' ',

'd','o','k','u','m','e','n','t','a','c','e',' ',
'n','a',' ',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' '
  #endif   /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif

#if defined(LANG_SLOVAK)	//Slowakisch
 const unsigned char HelpCalibration_str[] PROGMEM = {'>','N','e','k','a','l','i','b','r','o','v','a','n','y','!','<'
  #ifndef SHORT_UNCAL_MSG
,' ',
'P','r','e',' ','k','a','l','i','b','r','o','v','a','n','i','e',' ',

's','p','o','j',' ','v','s','e','t','k','y',' ','3',' ',
'm','e','r','a','c','i','e',' ','s','o','n','d','y',' ','a',' ',

's','t','l','a','c',' ','t','l','a','c','i','t','k','o',' ',
'T','E','S','T','.',' ','K','a','l','i','b','r','a','c','i','u',' ',

'p','o','t','v','r','d',' ','s','t','l','a','c','e','n','i','m',' ',
't','l','a','c','i','t','k','a',' ','T','E','S','T',' ',

'n','a',LCD_CHAR_INSEP,'2','s','.',' ','N','a',' ','v','y','z','v','u',' ',
'"','O','d','p','o','j',' ','s','o','n','d','y','!','"',' ',

'z','r','u','s',' ','p','r','e','p','o','j','e','n','i','e',' ',
'm','e','r','a','c','i','c','h',' ','s','o','n','d','.',' ',

'N','a',' ','d','a','l','s','i','u',' ','v','y','z','v','u',' ',
'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,' ','>','1','0','0','n','F','"',' ',

'p','r','i','p','o','j',' ','m','e','d','z','i',' ',
's','o','n','d','y',' ','1',',','3',' ','d','o','b','r','y',' ',

'k','o','n','d','e','n','z','a','t','o','r',' ',
'v','a','c','s','i',' ','a','k','o',' ','1','0','0','n','F','.',' ',

'S','o','f','t','w','a','r','e',' ','a',' ',
'd','o','k','u','m','e','n','t','a','c','i','a',' ',

'j','e',LCD_CHAR_INSEP,'n','a',' ','s','v','n',':','/','/',' ',
'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif

 #if defined(LANG_HUNGARIAN)      //Hungarian
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

'i','s',' ','s','h','o','w','n',' ','a','n','d',' ',
'c','o','n','n','e','c','t',' ','a',' ','g','o','o','d',' ',

'c','a','p','a','c','i','t','o','r',' ','w','i','t','h',' ',
'a','t',' ','l','e','a','s','t',' ','1','0','0','n','F',' ',

't','o',' ','p','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'a','n','d',LCD_CHAR_INSEP,'3',' ',
'o','n','l','y',' ','a','f','t','e','r',' ',

't','h','e',' ','m','e','s','s','a','g','e',' ',
'"','1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'i','s',LCD_CHAR_INSEP,'s','h','o','w','n','.',' ',
'S','o','f','t','w','a','r','e',' ','a','n','d',' ',

'd','o','c','u','m','e','n','t','a','t','i','o','n',' ',
'i','s',' ','a','t',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif

 #if defined(LANG_RUSSIAN)		//russian

 const unsigned char HelpCalibration_str[] PROGMEM = {'H','e',LCD_CHAR_INSEP,'o',Cyr_t,Cyr_k,'a',Cyr_l,Cyr_i,Cyr_b,'p','o',Cyr_v,'a',Cyr_n,'!'
  #ifndef SHORT_UNCAL_MSG
,Cyr_D,Cyr_l,Cyr_ja,LCD_CHAR_INSEP,Cyr_z,'a',Cyr_p,'y','c',Cyr_k,'a',' ',

Cyr_k,'a',Cyr_l,Cyr_i,Cyr_b,'p','o',Cyr_v,Cyr_k,Cyr_i,' ',



Cyr_z,'a',Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e',LCD_CHAR_INSEP,Cyr_t,'p',Cyr_i,' ',

Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,'a',LCD_CHAR_INSEP,Cyr_i,' ',



Cyr_n,'a',Cyr_zsch,Cyr_m,Cyr_i,Cyr_t,'e',LCD_CHAR_INSEP,Cyr_k,Cyr_n,'o',Cyr_p,Cyr_k,'y',' ',

'"','T','E','S','T','"','.',' ',



Cyr_P,'o',Cyr_d,Cyr_t,Cyr_v,'e','p',Cyr_d,Cyr_i,Cyr_t,'e',' ',

'P','e',Cyr_zsch,Cyr_i,Cyr_m,LCD_CHAR_INSEP,'c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'a',' ',



Cyr_P,'o',Cyr_v,Cyr_t,'o','p',Cyr_n,'o',LCD_CHAR_INSEP,Cyr_n,'a',Cyr_zsch,'a',Cyr_v,' ',

'T','E','S','T',LCD_CHAR_INSEP,Cyr_v,LCD_CHAR_INSEP,Cyr_t,'e',Cyr_tsch,'e',Cyr_n,Cyr_i,Cyr_i,' ',



'2','c','.',LCD_CHAR_INSEP,'P','a','c',Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e',' ',

Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_y,LCD_CHAR_INSEP,Cyr_k,'o',Cyr_g,Cyr_d,'a',' ',



'o',Cyr_t,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,'c',Cyr_ja,' ',

'"',Cyr_I,Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,Cyr_i,Cyr_ja,'!','"',' ', 



Cyr_P,'o',Cyr_d,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,Cyr_i,Cyr_t,'e',' ',

Cyr_k,'o',Cyr_n,Cyr_d,'e',Cyr_n,'c','a',Cyr_t,'o','p',' ',



'1','0','0','n','F',LCD_CHAR_INSEP,Cyr_i,Cyr_l,Cyr_i,LCD_CHAR_INSEP,Cyr_b,'o',Cyr_l,'e','e',' ',

Cyr_k,LCD_CHAR_INSEP,'"','T','P','1'+TP_OFFSET,'"',LCD_CHAR_INSEP,Cyr_i,LCD_CHAR_INSEP,'"','T','P','3'+TP_OFFSET,'"',' ',



Cyr_k,'o',Cyr_g,Cyr_d,'a',LCD_CHAR_INSEP,'c','o','o',Cyr_b,Cyr_schtsch,'e',Cyr_n,Cyr_i,'e',' ',

'"','1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',



'o',Cyr_t,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,'c',Cyr_ja,'.',' ',

Cyr_P,'p','o',Cyr_g,'p','a',Cyr_m,Cyr_m,'y',LCD_CHAR_INSEP,Cyr_i,' ',



Cyr_d,'o',Cyr_k,'y',Cyr_m,'e',Cyr_n,Cyr_t,'a',Cyr_c,Cyr_i,Cyr_ju,' ',

'c',Cyr_m,'.',LCD_CHAR_INSEP,Cyr_n,'a',' ','h','t','t','p',':','/','/',' ',



'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',



't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};

 #define HelpTXT

 #endif


 #if defined(LANG_UKRAINIAN)		//ukrainian

 const unsigned char HelpCalibration_str[] PROGMEM = {'B','i',Cyr_d,Cyr_k,'a',Cyr_l,'i',Cyr_b,'p','y',Cyr_j,Cyr_t,'e','!'
  #ifndef SHORT_UNCAL_MSG
,' ',

Cyr_Schtsch,'o',Cyr_b,Cyr_i,LCD_CHAR_INSEP,Cyr_z,'a',Cyr_p,'y','c',Cyr_t,Cyr_i,Cyr_t,Cyr_i,' ', 



Cyr_k,'a',Cyr_l,'i',Cyr_b,'p','y',Cyr_v,'a',Cyr_n,Cyr_n,Cyr_ja,' ',

Cyr_z,'a',Cyr_m,Cyr_k,Cyr_n,'i',Cyr_t,Cyr_ww,LCD_CHAR_INSEP,Cyr_t,'p',Cyr_i,' ', 



Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_i,LCD_CHAR_INSEP,'T','P','-','1','2','3',' ',

Cyr_t,'a',LCD_CHAR_INSEP,Cyr_n,'a',Cyr_t,Cyr_i,'c',Cyr_n,'i',Cyr_t,Cyr_ww,' ',



Cyr_k,Cyr_n,'o',Cyr_p,Cyr_k,'y',LCD_CHAR_INSEP,'"','T','E','S','T','"','.',' ',

Cyr_P,'i',Cyr_d,Cyr_t,Cyr_v,'e','p',Cyr_d,'i',Cyr_t,Cyr_ww,' ',



'P','e',Cyr_zsch,Cyr_i,Cyr_m,LCD_CHAR_INSEP,'c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'y',' ',

Cyr_schtsch,'e',LCD_CHAR_INSEP,'p','a',Cyr_z,LCD_CHAR_INSEP,Cyr_n,'a',Cyr_zsch,Cyr_m,'i',Cyr_t,Cyr_ww,' ',



'"','T','E','S','T','"',LCD_CHAR_INSEP,Cyr_v,Cyr_p,'p','o',Cyr_d,'o',Cyr_v,Cyr_zsch,' ',

'2','c','.',LCD_CHAR_INSEP,'P','o',Cyr_z,'i',Cyr_m,Cyr_k,Cyr_n,'i',Cyr_t,Cyr_ww,' ',



Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_i,LCD_CHAR_INSEP,Cyr_p,'i','c',Cyr_l,Cyr_ja,' ',

Cyr_p,'o',Cyr_v,'i',Cyr_d,'o',Cyr_m,Cyr_l,'e',Cyr_n,Cyr_n,Cyr_ja,' ', 



'"','I',Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,'i',Cyr_ja,'!','"',' ',

Cyr_P,'i',Cyr_d,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,'i',Cyr_t,Cyr_ww,LCD_CHAR_INSEP,Cyr_ja,Cyr_k,'i','c','-',



Cyr_n,Cyr_i,Cyr_j,LCD_CHAR_INSEP,Cyr_k,'o',Cyr_n,Cyr_d,'e',Cyr_n,'c','a',Cyr_t,'o','p',' ',

Cyr_b,'i',Cyr_l,Cyr_ww,Cyr_sch,'e',' ','1','0','0','n','F',' ',Cyr_d,'o',' ',



'T','P','1'+TP_OFFSET,' ',Cyr_t,'a',' ','T','P','3'+TP_OFFSET,' ',Cyr_k,'o',Cyr_l,Cyr_i,' ',

Cyr_p,'o',Cyr_v,'i',Cyr_d,'o',Cyr_m,Cyr_l,'e',Cyr_n,Cyr_n,Cyr_ja,':',' ',



'"','1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

Cyr_v,'i',Cyr_d,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,Cyr_ww,'c',Cyr_ja,'.',' ',



Cyr_P,'p','o',Cyr_g,'p','a',Cyr_m,'y',' ',Cyr_t,'a',' ',

Cyr_d,'o',Cyr_k,'y',Cyr_m,'e',Cyr_n,Cyr_t,'a',Cyr_c,'i',Cyr_ju,' ',



Cyr_d,Cyr_i,Cyr_v,'.',' ',Cyr_n,'a',' ','h','t','t','p',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',



'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};

 #define HelpTXT
 #endif

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
 #endif
#endif

 const unsigned char AnKat_str[] MEM_TEXT = {LCD_CHAR_LINE1, LCD_CHAR_DIODE1, LCD_CHAR_LINE1,0};
 const unsigned char KatAn_str[] MEM_TEXT = {LCD_CHAR_LINE1, LCD_CHAR_DIODE2, LCD_CHAR_LINE1,0};
#ifdef SamplingADC
 const unsigned char str_cap_for_l_meas[] MEM2_TEXT = "Cap for L meas?";    // for now placed here, should be translated at some point
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
 const unsigned char VERSION_str[] MEM_TEXT = "Version 1.12k";
#endif
#ifdef SHOW_ICE
 const unsigned char ICE0_str[] MEM2_TEXT = "ICE0=";
 const unsigned char ICEs_str[] MEM2_TEXT = "ICEs=";
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


 const uint8_t PrefixTab[] MEM_TEXT = { 'p','n',LCD_CHAR_U,'m',0,'k','M'}; // p,n,u,m,-,k,M


#ifdef AUTO_CAL
  const unsigned char MinCap_str[] MEM2_TEXT = " >100nF";
  const unsigned char REF_C_str[] MEM2_TEXT = "REF_C=";
  const unsigned char REF_R_str[] MEM2_TEXT = "REF_R=";
#endif

//End of EEPROM-Strings
#else
 // no MAIN_C
 #define COMMON extern
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
  extern const unsigned char VOLTAGE_str[] MEM2_TEXT ;
  extern const unsigned char SHOW_str[] MEM2_TEXT ;
  extern const unsigned char OFF_str[] MEM2_TEXT ;
  extern const unsigned char F_GEN_str[] MEM2_TEXT ;
  extern const unsigned char PWM_10bit_str[] MEM2_TEXT ;
  extern const unsigned char RotaryEncoder_str[] MEM2_TEXT ;
  extern const unsigned char SetCapCorr_str[] MEM2_TEXT ;
  extern const unsigned char TURN_str[] MEM2_TEXT ;	
  extern const unsigned char FULLCHECK_str[] MEM2_TEXT ;
  extern const unsigned char SHORT_PROBES_str[] MEM2_TEXT ;
  #if PROCESSOR_TYP == 644
  extern const unsigned char HFREQ_str[] MEM2_TEXT ;
  extern const unsigned char H_CRYSTAL_str[] MEM2_TEXT ;
  extern const unsigned char L_CRYSTAL_str[] MEM2_TEXT ;
  #endif
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
//  extern uint16_t R680pl;
//  extern uint16_t R680mi;
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
 extern const unsigned char str_cap_for_l_meas[] MEM2_TEXT;
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
  extern const uint8_t PrefixTab[]; // p,n,u,m,-,k,M
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

//generate Omega- and µ-character as Custom-character, if these characters has a number of loadable type
#if LCD_CHAR_OMEGA < 8
extern const unsigned char CyrillicOmegaIcon[] MEM_TEXT;	//Omega
#endif
#if LCD_CHAR_U < 8
extern const unsigned char CyrillicMuIcon[] MEM_TEXT;	//µ
#endif


