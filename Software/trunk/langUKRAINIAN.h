/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Transistor Tester
 *  
 *  File:       langUKRAINIAN.h
 *  Funktion:   language selection
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-12
 * ************************************************************************ */
#if defined(LANG_UKRAINIAN)		//Ukrainian
   const unsigned char TestRunning[] MEM_TEXT = {'T','e','c',Cyr_t,'y',Cyr_v,'a',Cyr_n,Cyr_n,Cyr_ja,0};
   const unsigned char BatWeak[] MEM_TEXT = {'C',Cyr_l,'a',Cyr_b,Cyr_k,'a',0};
   const unsigned char BatEmpty[] MEM_TEXT = {Cyr_Z,'A','M','I','H','A',0};
   const unsigned char TestFailed2[] MEM_TEXT = {Cyr_v,'i',Cyr_d,'c','y',Cyr_t,Cyr_n,Cyr_ja,' ',0};
   const unsigned char Bauteil[] MEM_TEXT = {Cyr_d,'e',Cyr_t,'a',Cyr_l,Cyr_ww,0};
//   const unsigned char Diode[] MEM_TEXT = {Cyr_D,'i','o',Cyr_d,':',0};
   const unsigned char Triac[] MEM_TEXT = {'C',Cyr_i,Cyr_m,'i','c',Cyr_t,'o','p',0};
   const unsigned char Thyristor[] MEM_TEXT = {'T',Cyr_i,'p',Cyr_i,'c',Cyr_t,'o','p',0};
   const unsigned char Unknown[] MEM_TEXT = {' ',Cyr_n,'e',Cyr_v,'i',Cyr_d,'o',Cyr_m,'a',0};
   const unsigned char TestFailed1[] MEM_TEXT = {Cyr_P,'o',Cyr_sch,Cyr_k,'o',Cyr_d,Cyr_zsch,'e',Cyr_n,'a',' ',' ','a',Cyr_b,'o',0};
//   const unsigned char OrBroken[] MEM_TEXT = {Cyr_n,'e','c',Cyr_p,'p','a',Cyr_v,Cyr_n,'a',' ',' ',0};
   const unsigned char Detected[] MEM_TEXT = " detected";
   const unsigned char TestTimedOut[] MEM_TEXT = {'T','a',Cyr_j,Cyr_m,'a','y',Cyr_t,0};
   #define Cathode_char 'C'
 #ifdef WITH_SELFTEST
   const unsigned char SELFTEST[] MEM_TEXT = {'P','e',Cyr_zsch,Cyr_i,Cyr_m,' ','c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'y',0};
   const unsigned char RELPROBE[] MEM_TEXT = {'I',Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,'i',Cyr_ja,'!',0};
   const unsigned char ATE[] MEM_TEXT = {'T','e','c',Cyr_t,' ',Cyr_z,'a',Cyr_k,'i',Cyr_n,Cyr_tsch,'e',Cyr_n,'o',0};
 #endif
 #ifdef WITH_MENU
   const unsigned char SELECTION_str[] MEM2_TEXT = {'B',Cyr_i,Cyr_b,'e','p','i',Cyr_t,Cyr_ww,':',0}; //"Selection:"

   const unsigned char TESTER_str[] MEM2_TEXT = {'T','p','a',Cyr_n,Cyr_z,Cyr_i,'c',Cyr_t,'o','p',' ','T','e','c',Cyr_t,0}; //"TransistorT"

  #ifndef NO_FREQ_COUNTER
   const unsigned char FREQ_str[] MEM2_TEXT = {Cyr_Tsch,'a','c',Cyr_t,'o',Cyr_t,'o',Cyr_m,'i','p',0};
 #ifdef WITH_FREQUENCY_DIVIDER
   const unsigned char FScaler_str[] MEM2_TEXT = "F-Scaler";
 #endif
  #endif

   const unsigned char VOLTAGE_str[] MEM2_TEXT = {'B','o',Cyr_l,Cyr_ww,Cyr_t,Cyr_m,'e',Cyr_t,'p',0}; //"Voltage"
   const unsigned char SHOW_str[] MEM2_TEXT = {Cyr_P,'o',Cyr_k,'a',Cyr_z,'a',Cyr_t,Cyr_i,' ',Cyr_d,'a',Cyr_n,'i',0}; //"Show data";

   const unsigned char OFF_str[] MEM2_TEXT = {'B',Cyr_i,Cyr_m,Cyr_k,Cyr_n,'y',Cyr_t,Cyr_i,0}; //"Switch off"

   const unsigned char F_GEN_str[] MEM2_TEXT = {'f','-',Cyr_G,'e',Cyr_n,'e','p','a',Cyr_t,'o','p',0}; //"f-Generator"
  #ifdef PWM_SERVO
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "Servo PWM";
  #else
   const unsigned char PWM_10bit_str[] MEM2_TEXT = "10-bit PWM";
  #endif
  #ifdef WITH_ROTARY_CHECK
   const unsigned char RotaryEncoder_str[] MEM2_TEXT = {'T','e','c',Cyr_t,' ','e',Cyr_n,Cyr_k,'o',Cyr_d,'e','p','a',0};
  #endif
   const unsigned char SetCapCorr_str[] MEM2_TEXT = {'C','(',LCD_CHAR_U,'F',')','-',Cyr_k,'o','p','e',Cyr_k,Cyr_c,'i',Cyr_ja,0};
   const unsigned char TURN_str[] MEM2_TEXT = {'K','p','y',Cyr_t,'i',Cyr_t,Cyr_ww,'!',0};
   const unsigned char FULLCHECK_str[] MEM2_TEXT = {'P','e',Cyr_zsch,Cyr_i,Cyr_m,' ','c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'y',0};
   const unsigned char SHORT_PROBES_str[] MEM2_TEXT = {Cyr_Z,'a',Cyr_k,'o','p','o',Cyr_t,'i',Cyr_t,Cyr_ww,'!',0};
  #if PROCESSOR_TYP == 644
   const unsigned char HFREQ_str[] MEM2_TEXT = {Cyr_Tsch,'a','c',Cyr_t,'o',Cyr_t,'o',Cyr_m,'i','p',' ','>',' ','2','M','H','z',0};
   const unsigned char H_CRYSTAL_str[] MEM2_TEXT = {'H','F',' ',Cyr_k,Cyr_v,'a','p',Cyr_c,0};
   const unsigned char L_CRYSTAL_str[] MEM2_TEXT = {'L','F',' ',Cyr_k,Cyr_v,'a','p',Cyr_c,0};
  #endif
  #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || defined(LCD_DOGM))
   const unsigned char CONTRAST_str[] MEM2_TEXT = {'K','o',Cyr_n,Cyr_t,'p','a','c',Cyr_t,Cyr_n,'i','c',Cyr_t,Cyr_ww,0}; //Kontrast
  #endif
 #endif  /* WITH_MENU */
 #ifdef WITH_XTAL
   const unsigned char cerres_str[] MEM_TEXT = "Ker.rezonator  ";
   const unsigned char xtal_str[] MEM_TEXT = {Cyr_k,Cyr_v,'a','p',Cyr_c,0};
 #endif
 #define LANG_SELECTED

 #if FLASHEND > 0x3fff
// Define a help text for calibration for ATmega328
// Complete words are output to the LCD rows.
// Words are separated by ' '. If you don't allow to separate two words
// to different rows, you can hide the ' ' with LCD_CHAR_INSEP.
// Notice, that the LCD has only 16 character in each line!

 const unsigned char HelpCalibration_str[] PROGMEM = {'B','i',Cyr_d,Cyr_k,'a',Cyr_l,'i',Cyr_b,'p','y',Cyr_j,Cyr_t,'e','!'
  #ifndef SHORT_UNCAL_MSG
,' ',
Cyr_Schtsch,'o',Cyr_b,Cyr_i,LCD_CHAR_INSEP,Cyr_z,'a',Cyr_p,'y','c',Cyr_t,Cyr_i,Cyr_t,Cyr_i,' ',

Cyr_k,'a',Cyr_l,'i',Cyr_b,'p','y',Cyr_v,'a',Cyr_n,Cyr_n,Cyr_ja,' ',
Cyr_z,'a',Cyr_m,Cyr_k,Cyr_n,'i',Cyr_t,Cyr_ww,LCD_CHAR_INSEP,Cyr_t,'p',Cyr_i,' ',

Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_i,LCD_CHAR_INSEP,'T','P','-','1','2','3',' ',
Cyr_t,'a',LCD_CHAR_INSEP,Cyr_n,'a',Cyr_t,Cyr_i,'c',Cyr_n,'i',Cyr_t,Cyr_ww,' ',

Cyr_k,Cyr_n,'o',Cyr_p,Cyr_k,'y',LCD_CHAR_INSEP,'"','T','E','S','T','"','.',' ',
Cyr_P,'i',Cyr_d,Cyr_t,Cyr_v,'e','p',Cyr_d,'i',Cyr_t,Cyr_ww,' ',

'P','e',Cyr_zsch,Cyr_i,Cyr_m,LCD_CHAR_INSEP,'c','a',Cyr_m,'o',Cyr_t,'e','c',Cyr_t,'y',' ',
Cyr_schtsch,'e',LCD_CHAR_INSEP,'p','a',Cyr_z,LCD_CHAR_INSEP,Cyr_n,'a',Cyr_zsch,Cyr_m,'i',Cyr_t,Cyr_ww,' ',

'"','T','E','S','T','"',LCD_CHAR_INSEP,Cyr_v,Cyr_p,'p','o',Cyr_d,'o',Cyr_v,Cyr_zsch,' ',
'2','c','.',LCD_CHAR_INSEP,'P','o',Cyr_z,'i',Cyr_m,Cyr_k,Cyr_n,'i',Cyr_t,Cyr_ww,' ',

Cyr_k,'o',Cyr_n,Cyr_t,'a',Cyr_k,Cyr_t,Cyr_i,LCD_CHAR_INSEP,Cyr_p,'i','c',Cyr_l,Cyr_ja,' ',
Cyr_p,'o',Cyr_v,'i',Cyr_d,'o',Cyr_m,Cyr_l,'e',Cyr_n,Cyr_n,Cyr_ja,' ',

'"','I',Cyr_z,'o',Cyr_l,Cyr_ja,Cyr_c,'i',Cyr_ja,'!','"',' ',
Cyr_P,'i',Cyr_d,Cyr_k,Cyr_l,Cyr_ju,Cyr_tsch,'i',Cyr_t,Cyr_ww,LCD_CHAR_INSEP,Cyr_ja,Cyr_k,'i','c','-',

Cyr_n,Cyr_i,Cyr_j,LCD_CHAR_INSEP,Cyr_k,'o',Cyr_n,Cyr_d,'e',Cyr_n,'c','a',Cyr_t,'o','p',' ',
Cyr_b,'i',Cyr_l,Cyr_ww,Cyr_sch,'e',' ','1','0','0','n','F',' ',Cyr_d,'o',' ',

'T','P','1'+TP_OFFSET,' ',Cyr_t,'a',' ','T','P','3'+TP_OFFSET,' ',Cyr_k,'o',Cyr_l,Cyr_i,' ',
Cyr_p,'o',Cyr_v,'i',Cyr_d,'o',Cyr_m,Cyr_l,'e',Cyr_n,Cyr_n,Cyr_ja,':',' ',

'"','1',LCD_CHAR_LINE1,LCD_CHAR_CAP,LCD_CHAR_LINE1,'3',LCD_CHAR_INSEP,'>','1','0','0','n','F','"',' ',
Cyr_v,'i',Cyr_d,'o',Cyr_b,'p','a',Cyr_zsch,'a','e',Cyr_t,Cyr_ww,'c',Cyr_ja,'.',' ',

Cyr_P,'p','o',Cyr_g,'p','a',Cyr_m,'y',' ',Cyr_t,'a',' ',
Cyr_d,'o',Cyr_k,'y',Cyr_m,'e',Cyr_n,Cyr_t,'a',Cyr_c,'i',Cyr_ju,' ',

Cyr_d,Cyr_i,Cyr_v,'.',' ',Cyr_n,'a',' ','h','t','t','p',':','/','/',' ',
'm','i','k','r','o','c','o','n','t','r','o','l','l','e','r','.',' ',

'n','e','t','/','t','r','a','n','s','i','s','t','o','r',' ',
't','e','s','t','e','r',' '
  #endif  /* SHORT_UNCAL_MSG */
,0};

 #define HelpTXT
 #endif  /* FLASHEND > 0x3fff */

#endif   /* LANG UKRAINIAN */
/* ****************************** EOF ***************************************** */
