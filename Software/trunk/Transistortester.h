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
#include "config.h"
#include "tt_function.h"
#include "lcd-routines.h"
#include "wait1000ms.h"
#include "part_defs.h"
#include "bitmaps.h"


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

 #if R_H_VAL == 10000
// resistor  100000 Ohm             1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
 const uint16_t RHtab[] PROGMEM = {4483,4244,4026,3827,3645,3477,3322,3178,3045};
 #else
// resistor  470000 Ohm            1000 1050 1100 1150 1200 1250 1300 1350 1400  mV
 const uint16_t RHtab[] PROGMEM = { 954, 903, 856, 814, 775, 740, 707, 676, 648};
 #endif

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
   const unsigned char OFF_str[] MEM2_TEXT = "Schalte aus";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Impulsdrehgeber";
   const unsigned char TURN_str[] MEM2_TEXT = "drehen!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selbsttest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Verbinde Pins!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequenz > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
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
   const unsigned char OFF_str[] MEM2_TEXT = "Spegnere";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generatore";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encoder rotativo";
   const unsigned char TURN_str[] MEM2_TEXT = "turno!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autotest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Pin collegare!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequenza > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF quarzo";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF quarzo";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_POLISH)		//polnisch
   const unsigned char TestRunning[] MEM_TEXT = "Testowanie..."; //€€€";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "za slaba";
   const unsigned char TestFailed2[] MEM_TEXT = "lub uszkodz.";
   const unsigned char Bauteil[] MEM_TEXT = "Elemen"; //t€€€";
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyrystor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " nieznany";
   const unsigned char TestFailed1[] MEM_TEXT = "brak elementu"; //€€€";
   const unsigned char OrBroken[] MEM_TEXT = "lub uszkodz. ";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Tryb auto-test..";
   const unsigned char RELPROBE[] MEM_TEXT = "osobne pins!";
   const unsigned char ATE[] MEM_TEXT = "Testu End";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Selekcia:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzystor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Czestosc";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char OFF_str[] MEM2_TEXT = "wylaczyc";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "enkoder obrotowy";
   const unsigned char TURN_str[] MEM2_TEXT = "kolej!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "auto-test";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "nawiaz Pins!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Czestosc > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kwarc";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kwarc";
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
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnout";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotacni koder";
   const unsigned char TURN_str[] MEM2_TEXT = "otocte!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autotest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "propoj sondy!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvence > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
  #endif
 #endif
 #define LANG_SELECTED
#endif

#if defined(LANG_SLOVAK)		//slowakisch
   const unsigned char TestRunning[] MEM_TEXT = "Prebieha meranie";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "vybita!"		//"prazdna!";
   const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "suciastka!";
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " neznama"; //€";
   const unsigned char TestFailed1[] MEM_TEXT = "Ziadna, neznama"; //€";
   const unsigned char OrBroken[] MEM_TEXT = "alebo vadna "; //€";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Samotestovanie..";
   const unsigned char RELPROBE[] MEM_TEXT = "Izoluj sondy!";
   const unsigned char ATE[] MEM_TEXT = "Koniec testu";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Vyber:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvencia";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napetie";
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnut";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotacny koder";
   const unsigned char TURN_str[] MEM2_TEXT = "tocte!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Samotestovanie";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "prepoj sondy!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF krystal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF krystal";
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
   const unsigned char OFF_str[] MEM2_TEXT = "izklopi";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvenca > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kristal";
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
   const unsigned char OFF_str[] MEM2_TEXT = "uitschakelen";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotary encoder";
   const unsigned char TURN_str[] MEM2_TEXT = "rotaren!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Zelftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "verbind Pinnen!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequentie > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kristal";
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
   const unsigned char OFF_str[] MEM2_TEXT = "desconectar";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encoder rotativo";
   const unsigned char TURN_str[] MEM2_TEXT = "rodar!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autoteste";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "conecte Pontas!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF cristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF cristal";
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
   const unsigned char OFF_str[] MEM2_TEXT = "kikapcsol";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotary encoder";
   const unsigned char TURN_str[] MEM2_TEXT = "forgat!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "On-teszt mod";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";	// ???
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
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
   const unsigned char OFF_str[] MEM2_TEXT = "isjungti";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";      //???
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Testuoju";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";	// ???

  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Daznis > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF kvarcas";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF kvarcas";
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
   const unsigned char OFF_str[] MEM2_TEXT = "Switch off";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
   const unsigned char TURN_str[] MEM2_TEXT = "turn!";	
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequency > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF quartz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF quartz";
  #endif
 #endif
#endif

// Strings, which are not dependent of any language
 const unsigned char Bat_str[] MEM_TEXT = "Bat. ";
 const unsigned char OK_str[] MEM_TEXT = "OK";
 const unsigned char mosfet_str[] MEM_TEXT = "-MOS";
 const unsigned char jfet_str[] MEM_TEXT = "JFET";
 const unsigned char igbt_str[] MEM_TEXT = "-IGBT";
 const unsigned char GateCap_str[] MEM_TEXT = "C=";
#ifdef FOUR_LINE_LCD
 const unsigned char hfe_str[] MEM_TEXT ="hFE=";
#else
 const unsigned char hfe_str[] MEM_TEXT ="B=";
#endif
 const unsigned char NPN_str[] MEM_TEXT = "NPN";
 const unsigned char PNP_str[] MEM_TEXT = "PNP";

#ifndef EBC_STYLE
 // default is the 123= style
 const unsigned char N123_str[] MEM_TEXT = {' ','1'+TP_OFFSET,'2'+TP_OFFSET,'3'+TP_OFFSET,'=',0};
// const unsigned char N123_str[] MEM_TEXT = " Pin=";
#else
 #if EBC_STYLE == 321
  const unsigned char N123_str[] MEM_TEXT = {' ','3'+TP_OFFSET,'2'+TP_OFFSET,'1'+TP_OFFSET,'=',0};
 #endif
 #if EBC_STYLE == 123
  const unsigned char N123_str[] MEM_TEXT = {' ','1'+TP_OFFSET,'2'+TP_OFFSET,'3'+TP_OFFSET,'=',0};
 #endif
#endif

 const unsigned char Uf_str[] MEM_TEXT = "Uf=";
 const unsigned char vt_str[] MEM_TEXT = " Vt=";
 const unsigned char Vgs_str[] MEM_TEXT = "@Vg=";
 const unsigned char CapZeich[] MEM_TEXT = {'-',LCD_CHAR_CAP,'-',0};
 const unsigned char Cell_str[] MEM_TEXT = "Cell!";
 const unsigned char VCC_str[] MEM_TEXT = "VCC=";

#if FLASHEND > 0x1fff
 const unsigned char ESR_str[] MEM_TEXT = " ESR=";
 const unsigned char VLOSS_str[] MEM_TEXT = " Vloss=";
 const unsigned char Lis_str[] MEM_TEXT = "L=";
 const unsigned char Ir_str[] MEM_TEXT = "  Ir=";
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
 const unsigned char HelpCalibration_str[] PROGMEM = {'U','n','k','a','l','i','b','r','i','e','r','t','!',' ',
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

'"','1'+TP_OFFSET,'-',LCD_CHAR_CAP,'-','3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',
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

't','e','s','t','e','r',' ',0};
 #define HelpTXT
 #endif

 #if defined(LANG_CZECH)	//Tschechisch
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','e','z','k','a','l','i','b','r','o','v','a','n','o','!',' ',
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
'"','i','s','o','l','a','t','e',LCD_CHAR_INSEP,'P','r','o','b','e','!','"',' ',

'O','d','s','t','r','a','n',' ','z','k','r','a','t',' ',
'm','e','r','i','c','i','c','h',' ','b','o','d','u',' ',

'P','r','i',' ','z','o','b','r','a','z','e','n','i',' ',
'"','1'+TP_OFFSET,'-',LCD_CHAR_CAP,'-','3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

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
't','e','s','t','e','r',' ',0};
 #define HelpTXT
 #endif

#if defined(LANG_SLOVAK)	//Slowakisch

 const unsigned char HelpCalibration_str[] PROGMEM = {'>','N','e','k','a','l','i','b','r','o','v','a','n','y','!','<',' ',

'P','r','e',' ','k','a','l','i','b','r','o','v','a','n','i','e',' ',



's','p','o','j',' ','v','s','e','t','k','y',' ','3',' ',

'm','e','r','a','c','i','e',' ','s','o','n','d','y',' ','a',' ',



's','t','l','a','c',' ','t','l','a','c','i','t','k','o',' ',

't','e','s','t','.',' ','K','a','l','i','b','r','a','c','i','u',' ',



'p','o','t','v','r','d',' ','s','t','l','a','c','e','n','i','m',' ',

't','l','a','c','i','t','k','a',' ','t','e','s','t',' ',



'n','a',LCD_CHAR_INSEP,'2','s','.',' ','P','o',' ','v','y','z','v','e',' ',

'"','I','z','o','l','u','j',' ','s','o','n','d','y','!','"',' ',



'z','r','u','s',' ','p','r','e','p','o','j','e','n','i','e',' ',

'm','e','r','a','c','i','c','h',' ','s','o','n','d','.',' ',



'N','a',' ','d','a','l','s','i','u',' ','v','y','z','v','u',' ',

'"','1'+TP_OFFSET,'-',LCD_CHAR_CAP,'-','3'+TP_OFFSET,' ','>','1','0','0','n','F','"',' ',



'p','r','i','p','o','j',' ','m','e','d','z','i',' ',

's','o','n','d','y',' ','1',',','3',' ','d','o','b','r','y',' ',



'k','o','n','d','e','n','z','a','t','o','r',' ',

'v','a','c','s','i',' ','a','k','o',' ','1','0','0','n','F','.',' ',



'S','o','f','t','w','a','r','e',' ','a',' ',

'd','o','k','u','m','e','n','t','a','c','i','a',' ',



'j','e',LCD_CHAR_INSEP,'n','a',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',



'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' ',0};

 #define HelpTXT

 #endif

 #if defined(LANG_HUNGARIAN)      //Hungarian
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','o','t',' ','c','a','l','i','b','r','a','t','e','d','!',' ',
'F','o','r',LCD_CHAR_INSEP,
'c','a','l','i','b','r','a','t','i','o','n',' ',

'c','l','a','m','b',' ','t','h','e',' ','3',LCD_CHAR_INSEP,'p','i','n','s',' ',
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
'"','1','-',LCD_CHAR_CAP,'-','3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'i','s',LCD_CHAR_INSEP,'s','h','o','w','n','.',' ',
'S','o','f','t','w','a','r','e',' ','a','n','d',' ',

'd','o','c','u','m','e','n','t','a','t','i','o','n',' ',
'i','s',' ','a','t',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' ',0};
 #define HelpTXT
 #endif

 #if defined(LANG_RUSSIAN)		//russian

 const unsigned char HelpCalibration_str[] PROGMEM = {'H','e',LCD_CHAR_INSEP,'o',Cyr_t,Cyr_k,'a',Cyr_l,Cyr_i,Cyr_b,'p','o',Cyr_v,'a',Cyr_n,'!',

Cyr_D,Cyr_l,Cyr_ja,LCD_CHAR_INSEP,Cyr_z,'a',Cyr_p,'y','c',Cyr_k,'a',' ',

Cyr_k,'a',Cyr_l,Cyr_i,Cyr_b,'p','o',Cyr_v,Cyr_k,Cyr_i,' ',



Cyr_z,'a',Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e',LCD_CHAR_INSEP,Cyr_t,'p',Cyr_i,' ',

Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,'a',LCD_CHAR_INSEP,Cyr_i,' ',



Cyr_n,'a',Cyr_zsch,Cyr_m,Cyr_i,Cyr_t,'e',LCD_CHAR_INSEP,Cyr_k,Cyr_n,'o',Cyr_p,Cyr_k,'y',' ',

'"','T','E','S','T','"','.',' ',



Cyr_P,'o',Cyr_d,Cyr_t,Cyr_v,'e','p',Cyr_d,Cyr_i,Cyr_t,'e',' ',

'P','e',Cyr_zsch,Cyr_i,Cyr_m,LCD_CHAR_INSEP,'c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'a',' ',



Cyr_P,'o',Cyr_v,Cyr_t,'o','p',Cyr_n,'o',LCD_CHAR_INSEP,Cyr_n,'a',Cyr_zsch,'a',Cyr_v,' ',

'T','E','S','T',LCD_CHAR_INSEP,Cyr_v,LCD_CHAR_INSEP,Cyr_t,'e',Cyr_tsch,'e',Cyr_n,Cyr_i,Cyr_i,' ',



'2','c','.',LCD_CHAR_INSEP,'P','o',Cyr_z,Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e',' ',

Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_y,LCD_CHAR_INSEP,Cyr_k,'o',Cyr_g,Cyr_d,'a',' ',



'o',Cyr_t,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,'c',Cyr_ja,' ',

'"',Cyr_I,Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,Cyr_i,Cyr_ja,'!','"',' ', 



Cyr_P,'o',Cyr_d,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,Cyr_i,Cyr_t,'e',' ',

Cyr_k,'o',Cyr_n,Cyr_d,'e',Cyr_n,'c','a',Cyr_t,'o','p',' ',



'1','0','0','n','F',LCD_CHAR_INSEP,Cyr_i,Cyr_l,Cyr_i,LCD_CHAR_INSEP,Cyr_b,'o',Cyr_l,'e','e',' ',

Cyr_k,LCD_CHAR_INSEP,'"','T','P','1'+TP_OFFSET,'"',LCD_CHAR_INSEP,Cyr_i,LCD_CHAR_INSEP,'"','T','P','3'+TP_OFFSET,'"',' ',



Cyr_k,'o',Cyr_g,Cyr_d,'a',LCD_CHAR_INSEP,'c','o','o',Cyr_b,Cyr_schtsch,'e',Cyr_n,Cyr_i,'e',' ',

'"','1','-',LCD_CHAR_CAP,'-','3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',



'o',Cyr_t,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,'c',Cyr_ja,'.',' ',

Cyr_P,'p','o',Cyr_g,'p','a',Cyr_m,Cyr_m,'y',LCD_CHAR_INSEP,Cyr_i,' ',



Cyr_d,'o',Cyr_k,'y',Cyr_m,'e',Cyr_n,Cyr_t,'a',Cyr_c,Cyr_i,Cyr_ju,' ',

'c',Cyr_m,'.',LCD_CHAR_INSEP,Cyr_n,'a',' ','h','t','t','p',':','/','/',' ',



'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',



't','e','s','t','e','r',' ',0};

 #define HelpTXT

 #endif


 #if defined(LANG_UKRAINIAN)		//ukrainian

 const unsigned char HelpCalibration_str[] PROGMEM = {'B','i',Cyr_d,Cyr_k,'a',Cyr_l,'i',Cyr_b,'p','y',Cyr_j,Cyr_t,'e','!',' ',

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



'"','1','-',LCD_CHAR_CAP,'-','3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

Cyr_v,'i',Cyr_d,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,Cyr_ww,'c',Cyr_ja,'.',' ',



Cyr_P,'p','o',Cyr_g,'p','a',Cyr_m,'y',' ',Cyr_t,'a',' ',

Cyr_d,'o',Cyr_k,'y',Cyr_m,'e',Cyr_n,Cyr_t,'a',Cyr_c,'i',Cyr_ju,' ',



Cyr_d,Cyr_i,Cyr_v,'.',' ',Cyr_n,'a',' ','h','t','t','p',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',



'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' ',0};

 #define HelpTXT
 #endif

 #ifndef HelpTXT
 //default language english
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','o','t',' ','c','a','l','i','b','r','a','t','e','d','!',' ',
'F','o','r',LCD_CHAR_INSEP,
'c','a','l','i','b','r','a','t','i','o','n',' ',

'c','l','a','m','b',' ','t','h','e',' ','3',LCD_CHAR_INSEP,'p','i','n','s',' ',
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
'"','1'+TP_OFFSET,'-',LCD_CHAR_CAP,'-','3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'i','s',LCD_CHAR_INSEP,'s','h','o','w','n','.',' ',
 #endif
'S','o','f','t','w','a','r','e',' ','a','n','d',' ',

'd','o','c','u','m','e','n','t','a','t','i','o','n',' ',
'i','s',' ','a','t',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' ',0};
 #endif
#endif

 const unsigned char AnKat[] MEM_TEXT = {'-', LCD_CHAR_DIODE1, '-',0};
 const unsigned char KatAn[] MEM_TEXT = {'-', LCD_CHAR_DIODE2, '-',0};
 const unsigned char Dioden[] MEM_TEXT = {'*',LCD_CHAR_DIODE1, ' ', ' ',0};
 const unsigned char Resistor_str[] MEM_TEXT = {'-', LCD_CHAR_RESIS1, LCD_CHAR_RESIS2,'-',0};
 const unsigned char VERSION_str[] MEM2_TEXT = "Version 1.11k";
#ifdef SHOW_ICE
 const unsigned char ICE0_str[] MEM2_TEXT = "ICE0=";
 const unsigned char ICEs_str[] MEM2_TEXT = "ICEs=";
#endif


#ifdef WITH_SELFTEST
 #ifdef EXTENDED_TESTS
 const unsigned char URefT[] MEM2_TEXT = "Ref=";
 const unsigned char RHfakt[] MEM2_TEXT = "RHf=";
 const unsigned char RH1L[] MEM_TEXT = "RH-";
 const unsigned char RH1H[] MEM_TEXT = "RH+";
 const unsigned char RLRL[] MEM_TEXT = "+RL- 12 13 23";
 const unsigned char RHRH[] MEM_TEXT = "+RH- 12 13 23";
 const unsigned char RHRL[] MEM_TEXT = "RH/RL";
 #endif
 const unsigned char R0_str[] MEM2_TEXT = "R0=";
 const unsigned char RIHI[] MEM_TEXT = "Ri_Hi=";
 const unsigned char RILO[] MEM_TEXT = "Ri_Lo=";
 const unsigned char C0_str[] MEM_TEXT = "C0 ";
 const unsigned char T50HZ[] MEM_TEXT = " 50Hz";
#endif
#ifdef WITH_MENU
 const unsigned char FatTP2_str[] MEM2_TEXT = "f@TP2";
 const unsigned char C_ESR_str[] MEM2_TEXT = "C+ESR@TP1:3";
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

 const unsigned char PinRLtab[] PROGMEM = { (1<<PIN_RL1),
				     (1<<PIN_RL2),
				     (1<<PIN_RL3)};	// Table of commands to switch the  R-L resistors Pin 0,1,2
#if FLASHEND > 0x3fff
 // Processors with little memory must use one Pin number higher than correspondig Low Resistor
 const unsigned char PinRHtab[] PROGMEM = { (1<<PIN_RH1),
				     (1<<PIN_RH2),
				     (1<<PIN_RH3)};	// Table of commands to switch the  R-L resistors Pin 0,1,2
#endif

		// Table include the predefined value TXD_VAL of other output port(s) of port C.
		// Every pin, that should be switched permanent to VCC level, should be set to 1 in every tab position.
		// The predefined value TXD_MSK defines the pin (all pins), that must be switched permanent to output.
 const unsigned char PinADCtab[] PROGMEM = { (1<<TP1)|TXD_VAL,
				     (1<<TP2)|TXD_VAL,
				     (1<<TP3)|TXD_VAL};	// Table of commands to switch the ADC-Pins 0,1,2 to output

 const uint8_t PrefixTab[] MEM_TEXT = { 'p','n',LCD_CHAR_U,'m',0,'k','M'}; // p,n,u,m,-,k,M

//generate Omega- and µ-character as Custom-character, if these characters has a number of loadable type
#if LCD_CHAR_OMEGA < 8
   const unsigned char CyrillicOmegaIcon[] MEM_TEXT = {0,0,14,17,17,10,27,0};	//Omega
#endif
#if LCD_CHAR_U < 8
   const unsigned char CyrillicMuIcon[] MEM_TEXT = {0,17,17,17,19,29,16,16};	//µ
#endif

#ifdef AUTO_CAL
  const unsigned char MinCap_str[] MEM2_TEXT = " >100nF";
  const unsigned char REF_C_str[] MEM2_TEXT = "REF_C=";
  const unsigned char REF_R_str[] MEM2_TEXT = "REF_R=";
  const int8_t RefDiff EEMEM = REF_R_KORR;	// correction of internal Reference Voltage

//  const uint16_t cap_null EEMEM = C_NULL;	// Zero offset of capacity measurement 
   const int16_t ref_offset EEMEM = REF_C_KORR;	// default correction of internal reference voltage for capacity measurement
   // the zero offset for capacity measurement for all pin combinations
  // LoPin:HiPin                        2:1    3:1    1:2                    marker  3:2                   1:3    2:3
   const uint8_t c_zero_tab[] EEMEM = { C_NULL,C_NULL,C_NULL+TP2_CAP_OFFSET,C_NULL+2,C_NULL+TP2_CAP_OFFSET,C_NULL,C_NULL }; //table of zero offsets
   // if the marker position of c_zero_tab is not equal the first position, the calibration has not run before
#endif

  const uint8_t EE_ESR_ZEROtab[] EEMEM = {ESR_ZERO, ESR_ZERO, ESR_ZERO, ESR_ZERO};	// zero offset of ESR measurement
#ifdef WITH_ROTARY_SWITCH
//  const uint8_t EE_RotarySwitch EEMEM = 0;	// rotation switch is not detected
#endif
//End of EEPROM-Strings
#else
 // no MAIN_C
 #define COMMON extern
 #ifdef WITH_SELFTEST
  extern const unsigned char SELFTEST[] MEM_TEXT;
  extern const unsigned char RELPROBE[] MEM_TEXT;
  extern const unsigned char ATE[] MEM_TEXT;
 #endif
 #ifdef WITH_MENU
  extern const unsigned char SELECTION_str[] MEM2_TEXT ;
  extern const unsigned char TESTER_str[] MEM2_TEXT ;
  extern const unsigned char FREQ_str[] MEM2_TEXT;
  extern const unsigned char VOLTAGE_str[] MEM2_TEXT ;
  extern const unsigned char OFF_str[] MEM2_TEXT ;
  extern const unsigned char F_GEN_str[] MEM2_TEXT ;
  extern const unsigned char PWM_10bit_str[] MEM2_TEXT ;
  extern const unsigned char RotaryEncoder_str[] MEM2_TEXT ;
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
 #ifdef AUTO_CAL
//  extern uint16_t R680pl;
//  extern uint16_t R680mi;
  extern int8_t RefDiff;
  extern uint16_t ref_offset;
  extern uint8_t c_zero_tab[];
 #endif
#ifndef EBC_STYLE
 // default is the 123= style
 extern const unsigned char N123_str[] MEM_TEXT;
// const unsigned char N123_str[] MEM_TEXT = " Pin=";
#else
 #if EBC_STYLE == 321
  extern const unsigned char N321_str[] MEM_TEXT;
 #endif
 #if EBC_STYLE == 123
  extern const unsigned char N123_str[] MEM_TEXT;
 #endif
#endif
  extern const unsigned char Uf_str[] MEM_TEXT;
  extern const uint8_t EE_ESR_ZEROtab[] EEMEM;	// zero offset of ESR measurement
  extern  const uint16_t RLtab[];

 #if FLASHEND > 0x1fff
  extern uint16_t LogTab[];
  extern const unsigned char ESR_str[];
 #endif

 extern const unsigned char AnKat[] MEM_TEXT ;


 #ifdef AUTO_RH
  extern const uint16_t RHtab[];
 #endif
  extern const unsigned char PinRLtab[];
  extern const uint8_t PrefixTab[]; // p,n,u,m,-,k,M
 #if FLASHEND > 0x3fff
  extern const unsigned char PinRHtab[];
 #endif
  extern const unsigned char PinADCtab[];
#endif
#ifdef WITH_MENU
COMMON const unsigned char FatTP2_str[];
COMMON const unsigned char C_ESR_str[];
#endif


struct Diode_t {
  uint8_t Anode[6];
  uint8_t Cathode[6];
  unsigned int Voltage[6];
};
COMMON struct Diode_t diodes;

COMMON uint8_t NumOfDiodes;
COMMON uint8_t diode_sequence;

typedef struct {
  unsigned long hfe;		//current amplification factor 
  unsigned int uBE;		//B-E-voltage of the Transistor
  unsigned int current;		// current of Drain in 0.01mA
  				// for bipolar current is ICE0
 #define ice0 current
  unsigned int gthvoltage;	//Gate-threshold voltage 
  				// for bipolar gthvoltage is ICEs in 0.01mA
 #define ices gthvoltage
  uint8_t b,c,e;		//pins of the Transistor
  uint8_t count;
}trans_t;

COMMON trans_t ptrans;		// parameters of P type transistor
COMMON trans_t ntrans;		// parameters of N type transistor
COMMON trans_t *_trans;		// pointer to trans_t structure

COMMON uint8_t tmpval, tmpval2;
COMMON unsigned int ref_mv;            //Reference-voltage  in mV units (as read with ADC)
COMMON unsigned int ref_mv_offs;       //Reference-voltage  in mV units with eeprom offset for C
COMMON unsigned int adc_internal_reference;  //internal reference voltage of ADC in mV units
COMMON  unsigned int RHmultip;	// Multiplier for capacity measurement with R_H (470KOhm)

COMMON struct resis_t{
   unsigned long rx;		// value of resistor RX  
   uint8_t ra,rb;		// Pins of RX
   uint8_t rt;			// Tristate-Pin (inactive)
   uint8_t checked;		// marked as 1, if same value in both directions
} resis[3];
COMMON  uint8_t ResistorsFound;	//Number of found resistors

#if FLASHEND > 0x1fff
   unsigned long inductor_lx;	// inductance 10uH or 100uH
   int8_t inductor_lpre;	// prefix for inductance
#endif
#ifdef WITH_MENU
COMMON union t_frq{
  unsigned long dw;
  uint8_t b[4];
} ext_freq;	// external frequency
//COMMON unsigned long ext_period; 
COMMON unsigned int pinchange_count;
COMMON unsigned int pinchange_max;
#endif

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
  unsigned int cell_mv[3];	//remaining load voltages after discharge cycle

#ifndef INHIBIT_SLEEP_MODE
 /* with sleep mode we need a global ovcnt16 */
COMMON volatile uint16_t ovcnt16;
COMMON volatile uint8_t unfinished;
#endif

COMMON int16_t load_diff;		// difference voltage of loaded capacitor and internal reference

COMMON uint8_t WithReference;		// Marker for found precision voltage reference = 1
COMMON uint8_t PartFound;	 	// type of the found part 
COMMON uint8_t PartMode;		// description of the found part
COMMON char outval[10];			// String for ASCII-output i2lcd, u2lcd
COMMON char OutBuffer[10];		// String for ASCII-output DisplayValue
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
COMMON uint8_t UnCalibrated;	// 0, if the tester is calibrated
#endif

#ifdef WITH_ROTARY_SWITCH
 #define ROT_MSK 0x03		/* must be power of two - 1: 3,7,15 */
struct Rotary_t {
 uint8_t state[(ROT_MSK+1)];	// coded state history of the rotatry switch, bit 0 == state of A-switch, bit 1 = state of B-switch
 uint8_t ind;		// index to the last entry of the state history (rotary switch)
 int8_t count;		// count of right steps, negative if left steps
 uint8_t incre;		// absolute value of step count
 #if WITH_ROTARY_SWITCH == 4
 // no rotary switch connected, UP and DOWN  key is present
 uint8_t a_state;	// history of switch A state for single UP switch
 uint8_t b_state;	// history of switch B state for single DOWN switch
 #endif
};
COMMON struct Rotary_t rotary;
COMMON uint8_t rotary_switch_present;	// is set to 1, if rotary switch movement is detected
// COMMON  const uint8_t EE_RotarySwitch; 	// rotation switch is detected
#endif


//#if POWER_OFF+0 > 1
COMMON unsigned int display_time;	// display time of measurement in ms units
//#endif
