#if defined(LANG_DANISH)		//dansk
   const unsigned char TestRunning[] MEM_TEXT = "Tester."; //ÄÄÄÄÄÄÄ";
   const unsigned char BatWeak[] MEM_TEXT = "svag";
   const unsigned char BatEmpty[] MEM_TEXT = "lav!"; //ÄÄÄ";
   const unsigned char TestFailed2[] MEM_TEXT = "defekt "; //ÄÄÄ";
   const unsigned char Bauteil[] MEM_TEXT = "Komponent"; //ÄÄÄ";
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Tyrist.";
   const unsigned char Unknown[] MEM_TEXT = " ukendt"; //ÄÄ";
   const unsigned char TestFailed1[] MEM_TEXT = "ingen,ukendt eller";
   const unsigned char Detected[] MEM_TEXT = " detekteret";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Selvtest ..";
   const unsigned char RELPROBE[] MEM_TEXT = "Adskil pinde!";
   const unsigned char ATE[] MEM_TEXT = "Test slut";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Udvalg:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvens";
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-scaler";
 #endif
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Spænding";
   const unsigned char SHOW_str[] MEM2_TEXT = "Vis data";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "slukker";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Drejegiver";
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','j','u','s','t','e','r','i','n','g',0};
   const unsigned char TURN_str[] MEM2_TEXT = "drej!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Selvtest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Forbind pinde!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvens > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF-krystal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF-krystal";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontrast";
  #endif
 #endif  /* WITH MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Ker.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "krystal  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 const unsigned char HelpCalibration_str[] PROGMEM = {'U','k','a','l','i','b','r','e','r','e','t','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'T','i','l',LCD_CHAR_INSEP,
'k','a','l','i','b','r','e','r','i','n','g',' ',

'k','l','e','m',' ','d','e',' ','3',LCD_CHAR_INSEP,'p','i','n','d','e',' ',
's','a','m','m','e','n',' ','o','g',' ',

's','t','a','r','t',' ','m','e','d',' ',
't','a','s','t','e','t','r','y','k','.',' ',

'B','e','k','r','æ','f','t',' ',
's','e','l','v','t','e','s','t','e','n',' ','m','e','d',' ',

't','a','s','t','e','t','r','y','k',' ','i',' ','l','ø','b','e','t',' ','a','f',' ','2','s','.',' ',
'A','d','s','k','i','l',' ','f','ø','r','s','t',' ','p','i','n','d','e','n','e',
   #ifdef TPCAP
'.',' ',
   #else
' ','e','f','t','e','r',' ',
'b','e','k','r','æ','f','t','e','l','s','e','n',' ',

'"','A','d','s','i','l',LCD_CHAR_INSEP,'p','i','n','d','e','n','e','!','"',' ',
'o','g',' ','t','i','l','s','l','u','t',' ',

'f','ø','r','s','t',' ', 'e','f','t','e','r',' ',
'b','e','k','r','æ','f','t','e','l','s','e','n',' ',

'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',
'T','i','l','s','l','u','t',' ','e','n',' ','g','o','d',' ',

'k','o','n','d','e','n','s','a','t','o','r',' ',
'p','å',' ','m','e','r','e',' ','e','n','d',' ','1','0','0','n','F',' ',

'p','å',' ','b','e','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'+',LCD_CHAR_INSEP,'3','.',' ',
   #endif
'S','o','f','t','w','a','r','e',' ','o','g',' ',

'd','o','k','u','m','e','n','t','a','t','i','o','n',' ',
'e','r',' ','p','å',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',
'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' '
  #endif   /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG DANISH */

