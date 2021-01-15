/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Transistor Tester
 *  
 *  File:       LANG_LITHUANIAN.h
 *  Funktion:   language selection
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-12
 * ************************************************************************ */
#if defined(LANG_LITHUANIAN) //Lithuanian
   const unsigned char TestRunning[] MEM_TEXT = "Tikrinu"; //"Testing...";
   const unsigned char BatWeak[] MEM_TEXT = "silpna"; //"weak";
   const unsigned char BatEmpty[] MEM_TEXT = "negera"; //"empty!"
   const unsigned char TestFailed2[] MEM_TEXT = "sugedusi "; //"damaged ";
   const unsigned char Bauteil[] MEM_TEXT = "dalis"; //"part";
// const unsigned char Diode[] MEM_TEXT = "Diodas: ";
   const unsigned char Triac[] MEM_TEXT = "Simistorius"; //"Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Tirist."; //"Thyristor";
   const unsigned char Unknown[] MEM_TEXT = " nezinoma"; //" unknown";
   const unsigned char TestFailed1[] MEM_TEXT = "Nezinoma arba"; //"Pajunkite detale" "No, unknown, or";
   const unsigned char Detected[] MEM_TEXT = " aptinka";
   const unsigned char TestTimedOut[] MEM_TEXT = "Viso gero!"; //"Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Testuoju.."; //"Selftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "Atjung laidus!"; //"isolate Probe!";
   const unsigned char ATE[] MEM_TEXT = "Testas baigtas"; //"Test End";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Pasirinkimas:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistorius";
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Daznis";
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-Scaler";
 #endif
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "Rodyti informacija";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "isjungti";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotary encoder";
  #endif
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Kvarcas  ";
 #endif
 #define LANG_SELECTED
#endif  /* LANG LITHUANIAN */
/* ****************************** EOF ***************************************** */
