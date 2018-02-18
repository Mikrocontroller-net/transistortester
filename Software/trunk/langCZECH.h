#if defined(LANG_CZECH)
//const unsigned char TestRunning[] MEM_TEXT = "Probíhá meøení..";
   const unsigned char TestRunning[] MEM_TEXT = {'P','r','o','b',Cz_i,'h',Cz_a,' ','m',Cz_ee,Cz_r,'e','n',Cz_i,0};
//const unsigned char BatWeak[] MEM_TEXT = "slabá"; //€€";
   const unsigned char BatWeak[] MEM_TEXT = {'s','l','a','b',Cz_a,0}; //slabá";
//const unsigned char BatEmpty[] MEM_TEXT = "vybitá";
   const unsigned char BatEmpty[] MEM_TEXT = {'v','y','b','i','t',Cz_a,0};
//const unsigned char TestFailed2[] MEM_TEXT = "vadná "; //€€€€€€";
   const unsigned char TestFailed2[] MEM_TEXT = {'v','a','d','n',Cz_a,' ',0}; //vadná";
//const unsigned char Bauteil[] MEM_TEXT = "souèástka"; //€";
   const unsigned char Bauteil[] MEM_TEXT = {'s','o','u',Cz_c,Cz_a,'s','t','k','a',0}; //souèástka";
//const unsigned char Diode[] MEM_TEXT = "Dioda: ";
   const unsigned char Triac[] MEM_TEXT = "Triak";
   const unsigned char Thyristor[] MEM_TEXT = "Tyristor"; //€";
//const unsigned char Unknown[] MEM_TEXT = " neznamá"; //€";
   const unsigned char Unknown[] MEM_TEXT = {' ','n','e','z','n',Cz_a,'m',Cz_a,0}; //€";
//const unsigned char TestFailed1[] MEM_TEXT = "Zadná, neznamá"; //€€";
   const unsigned char TestFailed1[] MEM_TEXT = {Cz_Z,Cz_a,'d','n',Cz_a,' ','n','e','z','n',Cz_a,'m',Cz_a,0}; //€€";
//   const unsigned char OrBroken[] MEM_TEXT = {'n','e','b','o',' ','v','a','d','n',Cz_a,' ',0}; //€€";
   const unsigned char Detected[] MEM_TEXT = {' ','d','e','t','e','k','o','v',Cz_a,'n','a'}; // " detekována"
//   const unsigned char Detected[] MEM_TEXT = " nalezena"; 
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
  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = "Frekvence";
 #ifdef WITH_FREQUENCY_DIVIDER
  const unsigned char FScaler_str[] MEM2_TEXT = {'f','-','p',Cz_r,'e','d','d',Cz_ee,'l','i',Cz_c,0};// " pøeddìliè"
 #endif
  #endif
//const unsigned char VOLTAGE_str[] MEM2_TEXT = "Napìtí";
   const unsigned char VOLTAGE_str[] MEM2_TEXT = {'N','a','p',Cz_ee,'t',Cz_i,0};
//const unsigned char SHOW_str[] MEM2_TEXT = "Zobrazit údaje";	// "Show data"
   const unsigned char SHOW_str[] MEM2_TEXT = {'Z','o','b','r','a','z','i','t',' ',Cz_u,'d','a','j','e',0};	// "Show data"
   const unsigned char OFF_str[] MEM2_TEXT = "Vypnout";
//const unsigned char F_GEN_str[] MEM2_TEXT = "f-Generator";
   const unsigned char F_GEN_str[] MEM2_TEXT = {'f','-','G','e','n','e','r',Cz_a,'t','o','r',0};
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
//const unsigned char RotaryEncoder_str[] MEM2_TEXT = "rotaèní kodér";
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = {'R','o','t','a',Cz_c,'n',Cz_i,' ','k','o','d',Cz_e,'r',0};
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-','k','o','r','e','k','c','e',0};
//const unsigned char TURN_str[] MEM2_TEXT = "otoète!";
   const unsigned char TURN_str[] MEM2_TEXT = {'o','t','o',Cz_c,'t','e','!',0};
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
//const unsigned char cerres_str[] MEM_TEXT = "Ker.rezonátor  ";
   const unsigned char cerres_str[] MEM_TEXT = {'K','e','r','.','r','e','z','o','n',Cz_a,'t','o','r',' ',0};

   const unsigned char xtal_str[] MEM_TEXT = "Krystal  ";
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!
 const unsigned char HelpCalibration_str[] PROGMEM = {'N','e','z','k','a','l','i','b','r','o','v',Cz_a,'n','o','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
'P','r','o',LCD_CHAR_INSEP, 'K','a','l','i','b','r','a','c','i',' ',

'z','k','r','a','t','u','j',' ','v',Cz_s,'e','c','h','n','y',' ',
'3',' ','m',Cz_ee,Cz_r,Cz_i,'c',Cz_i,' ','b','o','d','y',' ','a',' ',

's','p','u','s',Cz_t,' ','s','t','i','s','k','e','m',' ',
'T','E','S','T',LCD_CHAR_INSEP,'t','l','a',Cz_c,Cz_i,'t','k','a',' ',

'K','a','l','i','b','r','a','c','i',' ','a','k','t','i','v','u','j',' ',
'd','a','l',Cz_s,Cz_i,'m',' ','s','t','i','s','k','e','m',' ',

't','l','a',Cz_c,Cz_i,'t','k','a',' ','v',' ','d','o','b',Cz_ee,' ',
'k','r','a','t',Cz_s,Cz_i,' ','2',LCD_CHAR_INSEP,'s','e','k','u','n','d',' ',

'p',Cz_r,'i',LCD_CHAR_INSEP,'h','l',Cz_a,Cz_s,'c','e',' ',' ',
'"','i','z','o','l','u','j',LCD_CHAR_INSEP,'s','o','n','d','y','!','"',' ',

'O','d','s','t','r','a',Cz_n,' ','z','k','r','a','t',' ',
'm',Cz_ee,Cz_r,Cz_i,'c',Cz_i,'c','h',' ','b','o','d',Cz_uu,' ',

'P',Cz_r,'i',' ','z','o','b','r','a','z','e','n',Cz_i,' ',
'"','1'+TP_OFFSET,LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3'+TP_OFFSET,LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'v',LCD_CHAR_INSEP,'d','a','l',Cz_s,Cz_i,'m',' ',
'k','r','o','k','u',' ', 'k','a','l','i','b','r','a','c','e',' ',
'p',Cz_r,'i','p','o','j',' ','k','v','a','l','i','t','n',Cz_i,' ',
'k','o','n','d',Cz_i,'k',' ','v','e','t',Cz_s,Cz_i,' ','j','a','k',' ',

'1','0','0','n','F',LCD_CHAR_INSEP ,'m','e','z','i',' ','b','o','d','y',' ',
'1','a','3',' ',
'A','V','R',' ','t','e','s','t','e','r','u',' ',
'v',LCD_CHAR_INSEP ,'p','o','s','l','e','d','n',Cz_i,'m',' ',

'k','r','o','k','u',' ','k','a','l','i','b','r','a','c','e',' ',
'p',Cz_r,'i','p','o','j',' ','k','o','n','d',Cz_i,'k',' ','1','0','-','3','0','n','f',' ','p','a','k',' ',
'h','o',' ','p','o','u',Cz_z,'i','j',' ','p','a','r','a','l','e','l','n',Cz_ee,' ','j','a','k','o',' ',
'r','e','z','o','n','a','n',Cz_c,'n',Cz_i,' ','p','r','o',' ','m',Cz_ee,Cz_r,'e','n',Cz_i,' ',
'v','e','l','k',Cz_y,'c','h',' ','c',Cz_i,'v','e','k',' ',

'd','a','l',Cz_s,Cz_i,' ','i','n','f','o','r','m','a','c','e',' ',
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
