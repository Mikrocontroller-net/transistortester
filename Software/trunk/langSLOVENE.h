/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Tranzistor Tester
 *  
 *  File:       langSLOVENE.h
 *  Funktion:   language selection
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-13  Bohu    ISO 8859-2 text add  
 * ************************************************************************ */
#if defined(LANG_SLOVENE)                    //slovenian
   const unsigned char TestRunning[] MEM_TEXT = "Testiranje"; 
   const unsigned char BatWeak[] MEM_TEXT = "slaba!"; 
   const unsigned char BatEmpty[] MEM_TEXT = "prazna!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "pokvarjen "; 
   const unsigned char Bauteil[] MEM_TEXT = "del"; 
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tiristor";
   const unsigned char Unknown[] MEM_TEXT = " neznan"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Ni, neznan, ali"; 
   const unsigned char Detected[] MEM_TEXT = " detektira";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "isolate Probes!";
   const unsigned char ATE[] MEM_TEXT = "Test End";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Izbor:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistor";
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvenca";
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-Scaler";
 #endif
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "Prikazi podatke";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "izklopi";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
  #endif
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Kristal  ";
 #endif
 #define LANG_SELECTED
#endif  /* LANG SLOVENE */
/* ****************************** EOF ***************************************** */