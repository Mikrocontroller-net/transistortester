#if defined(LANG_DUTCH)                      //dutch
   const unsigned char TestRunning[] MEM_TEXT = "Testen."; 
   const unsigned char BatWeak[] MEM_TEXT = "laag"; 
   const unsigned char BatEmpty[] MEM_TEXT = "leeg!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "defect "; 
   const unsigned char Bauteil[] MEM_TEXT = "component"; 
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyrist.";
   const unsigned char Unknown[] MEM_TEXT = " onbekend"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Geen, onbekend,"; 
   const unsigned char Detected[] MEM_TEXT = " detecteert";
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
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotary encoder";
  #endif
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Kristal  ";
 #endif
 #define LANG_SELECTED
#endif  /* LANG DUTCH */
