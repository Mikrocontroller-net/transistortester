#if defined(LANG_CZECH)
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
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotacni koder";
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','k','o','r','e','k','c','e',0};
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Ker.rezonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Krystal  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
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
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG CZECH */
