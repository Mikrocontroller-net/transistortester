#if defined(LANG_SLOVAK)		//slowakisch
   const unsigned char TestRunning[] MEM_TEXT = "Testujem"; //"Prebieha meranie";
   const unsigned char BatWeak[] MEM_TEXT = "slaba"; //€€";
   const unsigned char BatEmpty[] MEM_TEXT = "vybita";		//"prazdna!";
   const unsigned char TestFailed2[] MEM_TEXT = "vadna "; //€€€€€€";
   const unsigned char Bauteil[] MEM_TEXT = "suciastka!";
//   const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; 
   const unsigned char Unknown[] MEM_TEXT = " neznama"; 
   const unsigned char TestFailed1[] MEM_TEXT = "Ziadna, neznama,"; 
   const unsigned char Detected[] MEM_TEXT = " detekuje";
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
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvencia";
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napetie";
   const unsigned char SHOW_str[] MEM2_TEXT = "Informacie";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnut";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotacny koder";
  #endif
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
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "krystal  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
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
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG SLOVAK */
