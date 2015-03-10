#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 7108))

 #if defined(FONT_6X8) || defined (FONT_6x8)
  #define FONT_WIDTH 6
  #define FONT_HEIGHT 8
  #include "fonts/6x8_vertikal_LSB_1.h"
  #define FONT_SELECTED
 #endif

 #ifdef FONT_8X16 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 16
   #include "fonts/8x16_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X16thin 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 16
   #include "fonts/8x16_vertikal_LSB_1thin.h"
  #endif
 #endif

 #ifdef FONT_8X15 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 15
   #include "fonts/8x15_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X14 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 14
   #include "fonts/8x14_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X12 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 12
   #include "fonts/8x12_vertikal_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X12thin
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 12
   #include "fonts/8x12_vertikal_LSB_1thin.h"
  #endif
 #endif

 #ifdef FONT_7X12 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 7
   #define FONT_HEIGHT 12
   #include "fonts/7x12_vertical_LSB_1.h"
  #endif
 #endif

 #ifdef FONT_8X8 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 8
   #include "fonts/8x8_vertikal_LSB_1.h"
  #endif
 #endif

 #ifndef FONT_SELECTED
   #warning No Font specified. Check Makefile!
   #define FONT_WIDTH 6
   #define FONT_HEIGHT 8
   #include "fonts/6x8_vertikal_LSB_1.h"
   #define FONT_SELECTED
 #endif

 #include "fonts/24x32_vertical_LSB.h"

#endif /* LCD_ST_TYPE... */

