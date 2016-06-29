#if defined(LANG_RUSSIAN)		//Russian
   const unsigned char TestRunning[] MEM_TEXT = {'T','e','c',Cyr_t,Cyr_i,'p','o',Cyr_v,'a',Cyr_n,Cyr_i,'e','.','.','.',0}; //Тестирование...";
   const unsigned char BatWeak[] MEM_TEXT = {'C',Cyr_l,'a',Cyr_b,'a',Cyr_ja,0}; //слабая";
   const unsigned char BatEmpty[] MEM_TEXT = {Cyr_Z,'A','M','E','H','A',0}; //ЗАМЕНА";
   const unsigned char TestFailed2[] MEM_TEXT = {Cyr_p,'o',Cyr_v,'p','e',Cyr_zsch,Cyr_d,'.',' ',0}; //поврежд. ";
   const unsigned char Bauteil[] MEM_TEXT = {Cyr_d,'e',Cyr_t,'a',Cyr_l,Cyr_ww,0}; //деталь";
//   const unsigned char Diode[] MEM_TEXT = {Cyr_D,Cyr_i,'o',Cyr_d,':',0}; //диод: ";
   const unsigned char Triac[] MEM_TEXT = {'C',Cyr_i,Cyr_m,Cyr_i,'c',Cyr_t,'o','p',0}; //Симистор
   const unsigned char Thyristor[] MEM_TEXT = {'T',Cyr_i,'p',Cyr_i,'c',Cyr_t,'o','p',0}; //Тиристор
   const unsigned char Unknown[] MEM_TEXT = {' ',Cyr_n,'e',Cyr_i,Cyr_z,Cyr_v,'e','c',Cyr_t,'.',0}; // неизвест.";
   const unsigned char TestFailed1[] MEM_TEXT = {'O',Cyr_t,'c','y',Cyr_t,'c',Cyr_t,Cyr_v,'y','e',Cyr_t,' ',Cyr_i,Cyr_l,Cyr_i,0}; //Отсутствует или";
   const unsigned char OrBroken[] MEM_TEXT = {Cyr_p,'o',Cyr_v,'p','e',Cyr_zsch,Cyr_d,'e',Cyr_n,'a',' ',0}; //повреждена ";
   const unsigned char TestTimedOut[] MEM_TEXT = {'T','a',Cyr_j,Cyr_m,'a','y',Cyr_t,0}; //Таймаут
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = {'P','e',Cyr_zsch,Cyr_i,Cyr_m,' ','c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'a',0}; //Режим самотеста
   const unsigned char RELPROBE[] MEM_TEXT = {Cyr_I,Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,Cyr_i,Cyr_ja,'!',0}; //Изоляция! 
   const unsigned char ATE[] MEM_TEXT = {'T','e','c',Cyr_t,' ',Cyr_z,'a',Cyr_k,'o',Cyr_n,Cyr_tsch,'e',Cyr_n,0}; //Тест закончен
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = {'B',Cyr_y,Cyr_b,'e','p',Cyr_i,Cyr_t,'e',':',0}; //"Selection:"

   const unsigned char TESTER_str[] MEM2_TEXT = {'T','p','a',Cyr_n,Cyr_z,Cyr_i,'c',Cyr_t,'o','p',' ','T','e','c',Cyr_t,0}; //"TransistorT"

   const unsigned char FREQ_str[] MEM2_TEXT = {Cyr_Tsch,'a','c',Cyr_t,'o',Cyr_t,'o',Cyr_m,'e','p',0};

   const unsigned char VOLTAGE_str[] MEM2_TEXT = {'B','o',Cyr_l,Cyr_ww,Cyr_t,Cyr_m,'e',Cyr_t,'p',0}; //"Voltage"
   const unsigned char SHOW_str[] MEM2_TEXT = {Cyr_I,Cyr_n,Cyr_f,'o','p',Cyr_m,'a',Cyr_c,Cyr_i,Cyr_ja,' ','o',' ','T','T',0}; //"Show data"

   const unsigned char OFF_str[] MEM2_TEXT = {'B',Cyr_y,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,Cyr_i,Cyr_t,Cyr_ww,0}; //"Switch off"

   const unsigned char F_GEN_str[] MEM2_TEXT = {'f','-',Cyr_G,'e',Cyr_n,'e','p','a',Cyr_t,'o','p',0}; //"f
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = {'T','e','c',Cyr_t,' ',Cyr_e,Cyr_n,Cyr_k,'o',Cyr_d,'e','p','a',0};	// Тест энкодера
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-',Cyr_k,'o','p','p','e',Cyr_k,Cyr_c,Cyr_i,Cyr_ja,0}; // C(uF) коррекция
   const unsigned char TURN_str[] MEM2_TEXT = {'K','p','y',Cyr_t,Cyr_i,Cyr_t,'e','!',0}; // Крутите!
   const unsigned char FULLCHECK_str[] MEM2_TEXT = {'P','e',Cyr_zsch,Cyr_i,Cyr_m,' ','c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'a',0}; //Режим самотеста
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = {Cyr_Z,'a',Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e','!',0}; // Закоротите!
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = {Cyr_Tsch,'a','c',Cyr_t,'o',Cyr_t,'o',Cyr_m,'e','p',' ','>',' ','2','M','H','z',0};
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = {'H','F',' ',Cyr_k,Cyr_v,'a','p',Cyr_c,0}; // HF кварц
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = {'L','F',' ',Cyr_k,Cyr_v,'a','p',Cyr_c,0}; // LF кварц
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = {'K','o',Cyr_n,Cyr_t,'p','a','c',Cyr_t,Cyr_n,'o','c',Cyr_t,Cyr_ww,0}; //Kontrast
  #endif
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Ker.rezonator  ";
   const unsigned char xtal_str[] MEM_TEXT = {Cyr_k,Cyr_v,'a','p',Cyr_c,0}; // LF кварц
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!

 const unsigned char HelpCalibration_str[] PROGMEM = {'H','e',' ','o',Cyr_t,Cyr_k,'a',Cyr_l,Cyr_i,Cyr_b,'p','o',Cyr_v,'a',Cyr_n,'!'
  #ifndef SHORT_UNCAL_MSG
,Cyr_D,Cyr_l,Cyr_ja,LCD_CHAR_INSEP,Cyr_z,'a',Cyr_p,'y','c',Cyr_k,'a',' ',
Cyr_k,'a',Cyr_l,Cyr_i,Cyr_b,'p','o',Cyr_v,Cyr_k,Cyr_i,' ',

Cyr_z,'a',Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e',LCD_CHAR_INSEP,Cyr_t,'p',Cyr_i,' ',
Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,'a',LCD_CHAR_INSEP,Cyr_i,' ',

Cyr_n,'a',Cyr_zsch,Cyr_m,Cyr_i,Cyr_t,'e',LCD_CHAR_INSEP,Cyr_k,Cyr_n,'o',Cyr_p,Cyr_k,'y',' ',
'"','T','E','S','T','"','.',' ',

Cyr_P,'o',Cyr_d,Cyr_t,Cyr_v,'e','p',Cyr_d,Cyr_i,Cyr_t,'e',' ',
'P','e',Cyr_zsch,Cyr_i,Cyr_m,LCD_CHAR_INSEP,'c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'a',' ',

Cyr_P,'o',Cyr_v,Cyr_t,'o','p',Cyr_n,'o',LCD_CHAR_INSEP,Cyr_n,'a',Cyr_zsch,'a',Cyr_v,' ',
'T','E','S','T',LCD_CHAR_INSEP,Cyr_v,LCD_CHAR_INSEP,Cyr_t,'e',Cyr_tsch,'e',Cyr_n,Cyr_i,Cyr_i,' ',

'2','c','.',LCD_CHAR_INSEP,'P','a','c',Cyr_k,'o','p','o',Cyr_t,Cyr_i,Cyr_t,'e',' ',
Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_y,LCD_CHAR_INSEP,Cyr_k,'o',Cyr_g,Cyr_d,'a',' ',

'o',Cyr_t,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,'c',Cyr_ja,' ',
'"',Cyr_I,Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,Cyr_i,Cyr_ja,'!','"',' ', 

Cyr_P,'o',Cyr_d,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,Cyr_i,Cyr_t,'e',' ',
Cyr_k,'o',Cyr_n,Cyr_d,'e',Cyr_n,'c','a',Cyr_t,'o','p',' ',

'1','0','0','n','F',LCD_CHAR_INSEP,Cyr_i,Cyr_l,Cyr_i,LCD_CHAR_INSEP,Cyr_b,'o',Cyr_l,'e','e',' ',
Cyr_k,LCD_CHAR_INSEP,'"','T','P','1'+TP_OFFSET,'"',LCD_CHAR_INSEP,Cyr_i,LCD_CHAR_INSEP,'"','T','P','3'+TP_OFFSET,'"',' ',

Cyr_k,'o',Cyr_g,Cyr_d,'a',LCD_CHAR_INSEP,'c','o','o',Cyr_b,Cyr_schtsch,'e',Cyr_n,Cyr_i,'e',' ',
'"','1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',

'o',Cyr_t,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,'c',Cyr_ja,'.',' ',
Cyr_P,'p','o',Cyr_g,'p','a',Cyr_m,Cyr_m,'y',LCD_CHAR_INSEP,Cyr_i,' ',

Cyr_d,'o',Cyr_k,'y',Cyr_m,'e',Cyr_n,Cyr_t,'a',Cyr_c,Cyr_i,Cyr_ju,' ',
'c',Cyr_m,'.',LCD_CHAR_INSEP,Cyr_n,'a',' ','h','t','t','p',':','/','/',' ',

'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',
'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',

't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};

 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */
#endif   /* LANG RUSSIAN */
