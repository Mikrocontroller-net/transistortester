#include <avr/io.h>
#include <avr/pgmspace.h> 

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7920))
 #ifdef FONT_6X8 
#include "fonts/6x8_vertikal_LSB_1.h"
 #else
  #ifdef FONT_8X8 
#include "fonts/8x8_vertikal_LSB_1.h"
  #else
   #ifdef FONT_8X12 
#include "fonts/8x12_vertikal_LSB_1.h"
   #else
    #ifdef FONT_8X14
#include "fonts/8x14_vertikal_LSB_1.h"
    #else
     #ifdef FONT_8X15
#include "fonts/8x15_vertikal_LSB_1.h"
     #else
      #ifdef FONT_8X16 
#include "fonts/8x16_vertikal_LSB_1.h"
      #else
       #ifdef FONT_10X16 
#include "fonts/10x16_vertikal_LSB_1.h"
       #else
#error No Font specified. Check Makefile!
       #endif
      #endif
     #endif
    #endif
   #endif
  #endif
 #endif
#endif

