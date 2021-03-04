/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Tranzistor Tester
 *  
 *  File:       langFRANCAIS.h
 *  Funktion:   language selection
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-12
 * ************************************************************************ */
#if defined(LANG_FRANCAIS)
   const unsigned char TestRunning[] MEM_TEXT = "Test en cours...";	
   const unsigned char BatWeak[] MEM_TEXT = "Pile faible"; 	
   const unsigned char BatEmpty[] MEM_TEXT = "Pas de pile"; 	
   const unsigned char TestFailed2[] MEM_TEXT = {'E','n','d','o','m','m','a','g',e_acute,' ',0}; 	
   const unsigned char Bauteil[] MEM_TEXT = "Elem."; 		
//   const unsigned char Diode[] MEM_TEXT = "Diodo: ";
   const unsigned char Triac[] MEM_TEXT = "Triac";
   const unsigned char Thyristor[] MEM_TEXT = "Thyristor";
   const unsigned char Unknown[] MEM_TEXT = "inconnu ?"; 	
   const unsigned char TestFailed1[] MEM_TEXT = {'C','o','m','p','o','s','.',' ','d',e_acute,'f','e','c','t','.',0}; 
   const unsigned char Detected[] MEM_TEXT = {' ','d',e_acute,'t',e_acute,'c','t','e',0};
   const unsigned char TestTimedOut[] MEM_TEXT = {'T','e','m','p','s',' ','d',e_acute,'p','a','s','s',e_acute,'!',0};
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Calibration...";
   //                                         1234567890123456
   const unsigned char RELPROBE[] MEM_TEXT = "Isoler la sonde!";
   const unsigned char ATE[] MEM_TEXT = "Fin de Cal.";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = {'S',e_acute,'l','e','c','t','i','o','n',':',0};
   const unsigned char TESTER_str[] MEM2_TEXT = "Transistor";
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = {'F','r',e_acute,'q','u','e','n','c','e',0};
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-Scaler";
 #endif
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = {'V','o','i','r',' ','d','o','n','n',e_acute,'e','s',0};	
   const unsigned char OFF_str[] MEM2_TEXT = {'A','r','r',e_circ,'t',0};
   const unsigned char F_GEN_str[] MEM2_TEXT = {'F','-','G',e_acute,'n',e_acute,'r','a','t','e','u','r',0};
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "encodeur rotatif";
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','C','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "tourner!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "Autotest...";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "Cou-circ. sondes";
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = {'F','r',e_acute,'q','u','e','n','c','e',' ','>',' ','2','M','H','z',0};
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF quartz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF quartz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Contraste";
  #endif
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = {'C',e_acute,'r','.',' ','r',e_acute,'s','o','n','a','t','e','u','r',' ',0};
   const unsigned char xtal_str[] MEM_TEXT = "Cristal  ";
 #endif
 #define LANG_SELECTED
 
 
 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','o','n',' ','c','a','l','i','b','r',e_acute,'!',' ',' ',' '
  #ifndef SHORT_UNCAL_MSG
 ,' ',
'P','o','u','r',LCD_CHAR_INSEP,
'c','a','l','i','b','r','a','t','i','o','n',' ',

's','t','r','a','p','p','e','r',' ','l','e','s',' ','p','i','n','s',LCD_CHAR_INSEP,'1','2','3',' ', 
//'c','l','a','m','p',' ','t','h','e',' ','3',LCD_CHAR_INSEP,'p','i','n','s',' ',
'e','n','s','e','m','b','l','e','s',' ','e','t',' ',
//'t','o','g','e','t','h','e','r',' ','a','n','d',' ',
'l','a','n','c','e','r',' ','a','v','e','c',' ',
//'s','t','a','r','t',' ','w','i','t','h',' ',
'l','a',' ','t','o','u','c','h','e','.',' ',
//'t','h','e',LCD_CHAR_INSEP ,'k','e','y','.',' ',
'A','c','q','u','i','t','t','e','r',' ','l','\'','a','u','t','o','t','e','s','t',' ','a','v','e','c',' ',
//'A','c','k','n','o','w','l','e','d','g','e',' ','t','h','e',' ',
//'s','e','l','f','-','t','e','s','t',' ','w','i','t','h',' ',
'l','a',' ','t','o','u','c','h','e',' ','d','a','n','s',' ', 
//'t','h','e',' ','k','e','y',' ',
'l','e','s',' ','2',' ','s','e','c','o','n','d','e','s','.',' ',
//'w','i','t','h','i','n',LCD_CHAR_INSEP,'2','s','.', ' ',
'D',e_acute,'c','o','n','n','e','c','t','e','r',' ','l','e','s',' ',
//'D','i','s','c','o','n','n','e','c','t',' ',
'b','r','o','c','h','e','s',' ','a','p','r',e_grave,'s',' ','l','\'','a','f','f','i','c','h','a','g','e',' ','d','u',' ','m','e','s','s','a','g','e',' ',
//'p','i','n','s',' ','a','f','t','e','r',' ',
'"','i','s','o','l','e','r',' ','l','e','s',' ','b','r','o','c','h','e','s','!','"',' ',
//'t','h','e',LCD_CHAR_INSEP,'m','e','s','s','a','g','e',' ',
//'"','i','s','o','l','a','t','e',LCD_CHAR_INSEP,'P','r','o','b','e','!','"',' ',

//'i','s',' ','s','h','o','w','n',
 #ifdef TPCAP
'.',' ',
 #else
' ','e','t',' ', 
//' ','a','n','d',' ',
'c','o','n','n','e','c','t','e','r',' ','u','n',' ','b','o','n',' ',
//'c','o','n','n','e','c','t',' ','a',' ','g','o','o','d',' ',
'c','o','n','d','e','n','s','a','t','e','u','r',' ','d','\'','a','u',' ','m','o','i','n','s',' ', 
//'c','a','p','a','c','i','t','o','r',' ','w','i','t','h',' ',
'1','0','0','n','F',' ',
//'a','t',' ','l','e','a','s','t',' ','1','0','0','n','F',' ',
'a','u','x',' ','p','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'e','t',LCD_CHAR_INSEP,'3',' ',
's','e','u','l','e','m','e','n','t',' ','a','p','r',e_grave,'s',' ', 
//'t','o',' ','p','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'a','n','d',LCD_CHAR_INSEP,'3',' ',
//'o','n','l','y',' ','a','f','t','e','r',' ',
'q','u','e',' ','l','e',' ','m','e','s','s','a','g','e',' ',
//'t','h','e',' ','m','e','s','s','a','g','e',' ',
'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',
's','o','i', 't',' ','a','f','f','i','c','h',e_acute,'.',' ',
//'i','s',LCD_CHAR_INSEP,'s','h','o','w','n','.',' ',
 #endif
//'S','o','f','t','w','a','r','e',' ','a','n','d',' ',
'S','o','f','t','w','a','r','e',' ','e','t',' ',
'd','o','c','u','m','e','n','t','a','t','i','o','n',' ',
//'i','s',' ','a','t',' ','s','v','n',':','/','/',' ',
's','o','n','t',' ','c','h','e','z',' ',
'g','i','t','h','u','b','.','c','o','m','/',LCD_NO_SPACE,
'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r',LCD_NO_SPACE,
'-','n','e','t','/','t','r','a','n','s','i','s','t','o','r',LCD_NO_SPACE,
't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */

#endif  /* LANG FRANCAIS */
/* ****************************** EOF ***************************************** */
