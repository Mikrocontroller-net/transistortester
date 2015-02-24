#include <avr/io.h>
#include <avr/pgmspace.h> 

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 7108))

 #ifdef FONT_8X16 
   #define FONT_SELECTED
   #include "fonts/8x16_vertikal_LSB_1.h"
 #endif

 #ifdef FONT_8X16thin 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/8x16_vertikal_LSB_1thin.h"
  #endif
 #endif

 #ifdef FONT_8X15 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/8x15_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X14 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/8x14_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X12 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/8x12_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X8 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/8x8_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_6X8 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/6x8_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_10X16 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #include "fonts/10x16_vertikal_LSB_1.h"
  #endif
 #endif

 #ifndef FONT_SELECTED
  #error No Font specified. Check Makefile!
 #endif
#endif

