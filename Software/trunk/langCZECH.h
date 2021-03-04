#if defined LANG_CZECH
   const unsigned char TestRunning[] MEM_TEXT = {'P','r','o','b',i_acute,'h',a_acute,' ','m',e_caron,r_caron,'e','n',i_acute,0};
   const unsigned char BatWeak[] MEM_TEXT = {'s','l','a','b',a_acute,0};
   const unsigned char BatEmpty[] MEM_TEXT = {'v','y','b','i','t',a_acute,0};
   const unsigned char TestFailed2[] MEM_TEXT = {'v','a','d','n',a_acute,' ',0};
   const unsigned char Bauteil[] MEM_TEXT = {'s','o','u',c_caron,a_acute,'s','t','k','a',0};
//const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor";
   const unsigned char Unknown[] MEM_TEXT = {' ','n','e','z','n',a_acute,'m',a_acute,0};
   const unsigned char TestFailed1[] MEM_TEXT = {Z_caron,a_acute,'d','n',a_acute,' ','n','e','z','n',a_acute,'m',a_acute,0};
   const unsigned char OrBroken[] MEM_TEXT = {'n','e','b','o',' ','v','a','d','n',a_acute,' ',0};
   const unsigned char Detected[] MEM_TEXT = {' ','d','e','t','e','k','o','v',a_acute,'n','a'};
   const unsigned char TestTimedOut[] MEM_TEXT = "Timeout!";
   #define Cathode_char 'K'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = "Autotest..";	//"Samotestovani..";
   const unsigned char RELPROBE[] MEM_TEXT = "Izoluj sondy!";
   const unsigned char ATE[] MEM_TEXT = "Konec testu";
 #endif
 #ifdef WITH_MENU
//   const unsigned char SELECTION_str[] MEM2_TEXT = "Vyber:";
  const unsigned char SELECTION_str[] MEM2_TEXT = {'V',y_acute,'b',e_caron,'r',':',0};
   const unsigned char TESTER_str[] MEM2_TEXT = "Tranzistor";
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvence";
 #ifdef WITH_FREQUENCY_DIVIDER
  const unsigned char FScaler_str[] MEM2_TEXT = {'f','-','p',r_caron,'e','d','d',e_caron,'l','i',c_caron,0};
 #endif
  #endif
   const unsigned char VOLTAGE_str[] MEM2_TEXT = {'N','a','p',e_caron,'t',i_acute,0};
   const unsigned char SHOW_str[] MEM2_TEXT = {'Z','o','b','r','a','z','i','t',' ',u_acute,'d','a','j','e',0};	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnout";
   const unsigned char F_GEN_str[] MEM2_TEXT = {'f','-','G','e','n','e','r',a_acute,'t','o','r',0};
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = {'R','o','t','a',c_caron,'n',i_acute,' ','k','o','d',e_acute,'r',0};
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','k','o','r','e','k','c','e',0};
   const unsigned char TURN_str[] MEM2_TEXT = {'o','t','o',c_caron,'!',0};
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
   const unsigned char cerres_str[] MEM_TEXT = {'K','e','r','.','r','e','z','o','n',a_acute,'t','o','r',' ',0};

   const unsigned char xtal_str[] MEM_TEXT = "Krystal  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','e','z','k','a','l','i','b','r','o','v',a_acute,'n','o','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'P','r','o',LCD_CHAR_INSEP, 'K','a','l','i','b','r','a','c','i',' ',

'z','k','r','a','t','u','j',' ','v',s_caron,'e','c','h','n','y',' ',
'3',' ','m',e_caron,r_caron,i_acute,'c',i_acute,' ','b','o','d','y',' ','a',' ',

's','p','u','s',t_caron,' ','s','t','i','s','k','e','m',' ',LCD_CHAR_INSEP,'t','l','a',c_caron,i_acute,'t','k','a','.',' ',

'K','a','l','i','b','r','a','c','i',' ','a','k','t','i','v','u','j',' ',
'd','a','l',s_caron,i_acute,'m',' ','s','t','i','s','k','e','m',' ',

't','l','a',c_caron,i_acute,'t','k','a',' ','v',' ','d','o','b',e_caron,' ',
'k','r','a','t',s_caron,i_acute,LCD_CHAR_INSEP,'n','e',z_caron,LCD_CHAR_INSEP,'2',LCD_CHAR_INSEP,'v','t','e',r_caron,'i','n','y','.',' ',

'P',r_caron,'i',LCD_CHAR_INSEP,'h','l',a_acute,s_caron,'c','e',LCD_CHAR_INSEP,
'"','i','z','o','l','u','j',LCD_CHAR_INSEP,'s','o','n','d','y','!','"',' ',

'o','d','s','t','r','a',n_caron,' ','z','k','r','a','t',' ',
'm',e_caron,r_caron,i_acute,'c',i_acute,'c','h',' ','b','o','d',u_acute,'.',' ',

'P',r_caron,'i',' ','z','o','b','r','a','z','e','n',i_acute,' ',
'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',',',' ',

'p',r_caron,'i','p','o','j',' ','k','v','a','l','i','t','n',i_acute,' ',
'k','o','n','d',i_acute,'k',',',LCD_CHAR_INSEP,'v',e_caron,'t',s_caron,i_acute,' ','n','e',z_caron,' ',

'1','0','0','n','F',',',LCD_CHAR_INSEP,'m','e','z','i',' ','b','o','d','y',' ',
'1',' ','a',' ','3',' ','A','V','R',' ','t','e','s','t','e','r','u','.',' ',

'V',LCD_CHAR_INSEP ,'p','o','s','l','e','d','n',i_acute,'m',' ','k','r','o','k','u',' ','k','a','l','i','b','r','a','c','e',' ',
'p',r_caron,'i','p','o','j',' ','k','o','n','d',i_acute,'k',' ','1','0','-','3','0','n','f',',',' ','k','t','e','r',y_acute,' ','l','z','e',' ',

'v',LCD_CHAR_INSEP,'b','u','d','o','u','c','n','o','s','t','i',' ','p','o','u',z_caron,i_acute,'t',' ','p','a','r','a','l','e','l','n',e_caron,' ','j','a','k','o',' ',
'r','e','z','o','n','a','n',c_caron,'n',i_acute,',',' ','p','r','o',' ','m',e_caron,r_caron,'e','n',i_acute,' ',
'v','e','l','k',y_acute,'c','h',' ','c',i_acute,'v','e','k','.',' ',

'D','a','l',s_caron,i_acute,' ','i','n','f','o','r','m','a','c','e',' ',
'o',' ','s','o','f','t','w','a','r','u',' ','a',' ',

'd','o','k','u','m','e','n','t','a','c','i',' ','n','a',' ',
'g','i','t','h','u','b','.','c','o','m','/',LCD_NO_SPACE,
,'m','i','k','r','o','c','o','n','t','r','o','l','l','e','r',LCD_NO_SPACE,
'-','n','e','t','/','t','r','a','n','s','i','s','t','o','r',LCD_NO_SPACE,
't','e','s','t','e','r',' '
  #endif   /* SHORT_UNCAL_MSG */
,0};
 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */
#endif  /* LANG CZECH */
