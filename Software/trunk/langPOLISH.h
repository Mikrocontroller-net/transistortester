#if defined(LANG_POLISH)		//polnisch
   const unsigned char TestRunning[] MEM_TEXT = "Testowanie"; //€€€";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "za slaba";
   const unsigned char TestFailed2[] MEM_TEXT = "lub uszkodzony";
   const unsigned char Bauteil[] MEM_TEXT = " "; //t€€€";                       // don't fit on display, besides i think word is unnecesary repeated in second row after tests
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyrystor"; //€";
   const unsigned char Unknown[] MEM_TEXT = " nieznany";
   const unsigned char TestFailed1[] MEM_TEXT = "Brak elementu"; //€€€";
   const unsigned char Detected[] MEM_TEXT = " wykrywa";
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
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Czestotliwosc";
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napiecie";
   const unsigned char SHOW_str[] MEM2_TEXT = "Pokaz dane";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Wylacz";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Enkoder obrotowy";
  #endif
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Kwarc  ";
 #endif
 #define LANG_SELECTED
#endif  /* LANG POLISH */

