
#if defined(LANG_ITALIAN)		//italiano
   const unsigned char TestRunning[] MEM_TEXT = "Analizzo"; 
   const unsigned char BatWeak[] MEM_TEXT = "debole";
   const unsigned char BatEmpty[] MEM_TEXT = "esaurita!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "guasto ";	//"danneggiato "; 
   const unsigned char Bauteil[] MEM_TEXT = "campione";		//"componente"; 
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "SCR";
   const unsigned char Unknown[] MEM_TEXT = " ignoto";		//" sconosciuto.";
   const unsigned char TestFailed1[] MEM_TEXT = "Test fallito, o";
   const unsigned char Detected[] MEM_TEXT = " detected";
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
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encoder rotativo";
  #endif
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Quarzo  ";
 #endif
 #define LANG_SELECTED
#endif  /* LANG ITALIAN */

