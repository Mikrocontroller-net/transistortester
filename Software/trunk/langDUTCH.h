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
   const unsigned char Detected[] MEM_TEXT = " gedetecteerd";
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Zelftest mode..";
   const unsigned char RELPROBE[] MEM_TEXT = "Scheid pinnen!";
   const unsigned char ATE[] MEM_TEXT = "Einde test";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Selectie:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frequentie";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Spanning";
   const unsigned char SHOW_str[] MEM2_TEXT = "Toon gegevens";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Uitschakelen";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Pulsgever";
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','c','o','r','r','e','c','t','i','e',0};
   const unsigned char TURN_str[] MEM2_TEXT = "draaien!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Zelftest";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Verbind pinnen!";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frequentie > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF-kristal";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF-kristal";
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

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 const unsigned char HelpCalibration_str[] PROGMEM = {'O','n','g','e','c','a','l','i','b','r','e','e','r','d','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'V','o','o','r',LCD_CHAR_INSEP,
'c','a','l','i','b','r','a','t','i','e',' ',

'v','e','r','b','i','n','d',' ','3',LCD_CHAR_INSEP,'p','i','n','n','e','n',' ',
'o','n','d','e','r','l','i','n','g',' ','e','n',' ',

's','t','a','r','t',' ','m','e','t',' ','d','e',' ',
't','o','e','t','s','.',' ',

'B','e','v','e','s','t','i','g',' ','d','e',' ',
'z','e','l','f','t','e','s','t',' ','m','e','t',' ',

'd','e',' ','t','o','e','t','s',' ','i','n',' ','2','s','.',' ',
'S','c','h','e','i','d',' ','d','e',' ','p','i','n','n','e','n',
   #ifdef TPCAP
'.',' ',
   #else
' ','p','a','s',' ','n','a',' ','d','e',' ',
'm','e','l','d','i','n','g',' ',

'"','S','c','h','e','i','d',LCD_CHAR_INSEP,'P','i','n','n','e','n','!','"',' ',
'e','n',' ','s','l','u','i','t',

'p','a','s',' ', 'n','a',' ','d','e',' ',
'm','e','l','d','i','n','g',' ',

'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',
'e','e','n',' ','g','o','e','d','e',' ',

'c','o','n','d','e','n','s','a','t','o','r',' ',
'v','a','n',' ','m','e','e','r',' ','a','l','s',' ','1','0','0','n','F',' ',

'a','a','n',' ','p','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'+',LCD_CHAR_INSEP,'3',' ','a','a','n','.',' ',
   #endif
'S','o','f','t','w','a','r','e',' ','e','n',' ',

'd','o','c','u','m','e','n','t','a','t','i','e',' ',
's','t','a','a','t',' ','o','p',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',
'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' '
  #endif   /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG DUTCH */
