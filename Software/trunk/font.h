#if (LCD_GRAPHIC_TYPE != 0)


#if defined(FONT_5X8) || defined(FONT_5x8) 
   #define FONT_WIDTH 6
   #define FONT_HEIGHT 8
   #include "fonts/5x8_vertikal_LSB_1.h"   //******Font_5x8 (create by mike0815 *** 23-09-2015 )
 #define FONT_SELECTED
 #endif
 

 #if defined(FONT_6X8) || defined (FONT_6x8)
  #define FONT_WIDTH 6
  #define FONT_HEIGHT 8
  #include "fonts/6x8_vertikal_LSB_1.h"
  #define FONT_SELECTED
 #endif

 #if defined(FONT_8X16) || defined(FONT_8x16) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 16
   #include "fonts/8x16_vertikal_LSB_1.h"
  #endif
 #endif

 #if defined(FONT_8X16thin) || defined(FONT_8x16thin) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 16
   #include "fonts/8x16_vertikal_LSB_1thin.h"
  #endif
 #endif

 #if defined(FONT_8X15) || defined(FONT_8x15) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 15
   #include "fonts/8x15_vertikal_LSB_1.h"
  #endif
 #endif

 #if defined(FONT_8X14) || defined(FONT_8x14) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 14
   #include "fonts/8x14_vertikal_LSB_1.h"
  #endif
 #endif

 #if defined(FONT_8X12) || defined(FONT_8x12) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 12
   #include "fonts/8x12_vertikal_LSB_1.h"
  #endif
 #endif

 #if defined(FONT_8X12thin) || defined(FONT_8x12thin) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 12
   #include "fonts/8x12_vertikal_LSB_1thin.h"
  #endif
 #endif

 #if defined(FONT_7X12) || defined(FONT_7x12) 
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 7
   #define FONT_HEIGHT 12
   #include "fonts/7x12_vertical_LSB_1.h"
  #endif
 #endif

 #if defined(FONT_8X8) || defined(FONT_8x8) 
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

#endif /* LCD_GRAPHIC_TYPE != 0 */

