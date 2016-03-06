#if defined(LANG_HUNGARIAN)     //HUNGARIAN
   const unsigned char TestRunning[] MEM_TEXT = "Meres..."; 
   const unsigned char BatWeak[] MEM_TEXT = "gyenge"; 
   const unsigned char BatEmpty[] MEM_TEXT = "ures!"; 
   const unsigned char TestFailed2[] MEM_TEXT = "hibas "; 
   const unsigned char Bauteil[] MEM_TEXT = "alkatresz"; 
//   const unsigned char Diode[] MEM_TEXT = "Diode: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tiriszt.";
   const unsigned char Unknown[] MEM_TEXT = " unknown"; // " ismeretlen"
   const unsigned char TestFailed1[] MEM_TEXT = "hibas vagy"; 
   const unsigned char OrBroken[] MEM_TEXT = "nincs alkatresz ";
   const unsigned char TestTimedOut[] MEM_TEXT = "Idotullepes!";

   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "On-teszt mod..";
   const unsigned char RELPROBE[] MEM_TEXT = "Rovidzar ki!"; // or "separate pins!";
   const unsigned char ATE[] MEM_TEXT = "Teszt vege";
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = "Valasztas:";
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzisztor";
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvencia";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = "Voltage";
   const unsigned char SHOW_str[] MEM2_TEXT = "adatokat mutat";	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "kikapcsol";
   const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = "Rotary encoder";
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','C','o','r','r','e','c','t','i','o','n',0};
   const unsigned char TURN_str[] MEM2_TEXT = "forgat!";
   const unsigned char FULLCHECK_str[] MEM2_TEXT = "On-teszt mod";
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = "short Probes!";	// ???
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = "Frekvencia > 2MHz";
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = "HF Quarz";
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = "LF Quarz";
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = "Kontraszt";
  #endif
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Cer.resonator  ";
   const unsigned char xtal_str[] MEM_TEXT = "Quarz  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','o','t',' ','c','a','l','i','b','r','a','t','e','d','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'F','o','r',LCD_CHAR_INSEP,
'c','a','l','i','b','r','a','t','i','o','n',' ',

'c','l','a','m','p',' ','t','h','e',' ','3',LCD_CHAR_INSEP,'p','i','n','s',' ',
't','o','g','e','t','h','e','r',' ','a','n','d',' ',

's','t','a','r','t',' ','w','i','t','h',' ',
't','h','e',LCD_CHAR_INSEP ,'k','e','y','.',' ',

'A','c','k','n','o','w','l','e','d','g','e',' ','t','h','e',' ',
's','e','l','f','-','t','e','s','t',' ','w','i','t','h',' ',

't','h','e',' ','k','e','y',' ',
'w','i','t','h','i','n',LCD_CHAR_INSEP,'2','s','.', ' ',

'D','i','s','c','o','n','n','e','c','t',' ',
'p','i','n','s',' ','a','f','t','e','r',' ',

't','h','e',LCD_CHAR_INSEP,'m','e','s','s','a','g','e',' ',
'"','i','s','o','l','a','t','e',LCD_CHAR_INSEP,'P','r','o','b','e','!','"',' ',

'i','s',' ','s','h','o','w','n',' ','a','n','d',' ',
'c','o','n','n','e','c','t',' ','a',' ','g','o','o','d',' ',

'c','a','p','a','c','i','t','o','r',' ','w','i','t','h',' ',
'a','t',' ','l','e','a','s','t',' ','1','0','0','n','F',' ',

't','o',' ','p','i','n',LCD_CHAR_INSEP,'1',LCD_CHAR_INSEP,'a','n','d',LCD_CHAR_INSEP,'3',' ',
'o','n','l','y',' ','a','f','t','e','r',' ',

't','h','e',' ','m','e','s','s','a','g','e',' ',
'"','1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'i','s',LCD_CHAR_INSEP,'s','h','o','w','n','.',' ',
'S','o','f','t','w','a','r','e',' ','a','n','d',' ',

'd','o','c','u','m','e','n','t','a','t','i','o','n',' ',
'i','s',' ','a','t',' ','s','v','n',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG HUNGARIAN */
