#if defined(LANG_GERMAN)		//deutsch
   const unsigned char TestRunning[] MEM_TEXT = "Testen.";
   const unsigned char BatWeak[] MEM_TEXT = "gering";
   const unsigned char BatEmpty[] MEM_TEXT = "leer!";
   const unsigned char TestFailed2[] MEM_TEXT = "defektes ";
   const unsigned char Bauteil[] MEM_TEXT = "Bauteil";
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyrist.";
   const unsigned char Unknown[] MEM_TEXT = " unbek.";
   const unsigned char TestFailed1[] MEM_TEXT = "Kein,unbek. oder";
   const unsigned char Detected[] MEM_TEXT = " detektiert";
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
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequenz";
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-Scaler";
 #endif
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Spannung";
   const unsigned char SHOW_str[] MEM2_TEXT = "Zeige Daten";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Schalte aus";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Impulsdrehgeber";
  #endif
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
 #endif  /* WITH MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Ker.Resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Quarz  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
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
'i','s','t',' ','a','u','f',' ',
'g','i','t','h','u','b','.','c','o','m','/',LCD_NO_SPACE,
'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r',LCD_NO_SPACE,
'-','n','e','t','/','t','r','a','n','s','i','s','t','o','r',LCD_NO_SPACE,
't','e','s','t','e','r',' '
  #endif   /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG GERMAN */

