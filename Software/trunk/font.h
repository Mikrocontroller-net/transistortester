/* ************************************************************************
 *  (c) by Karl-Heinz Kuebbeler, Projekt Transistor Tester
 *  
 *  File:       font.h
 *  Funktion:   font file selection from FONT define in Makefile
 * 
 * History:     Date        Sign    Kommentar 
 *              2021-01-02  Bohu    font*.LSB_2.h add  
 *              2021-01-09  Karl    FONT_SELECTED add
 *              2021-01-24  Karl    WITH_CAPITAL_SPECIALS depends on Flash End
 *
 * ************************************************************************ */

#if (LCD_GRAPHIC_TYPE != 0)

#if FLASHEND > 0x7fff
 #define WITH_CAPITAL_SPECIALS 1
#else
 #undef WITH_CAPITAL_SPECIALS
#endif

#if defined(FONT_5X8) || defined(FONT_5x8)
   /* The 5x8 Font is created without horizontal space. */
   /* Therefore the FONT_H_SPACE is defined with one Pixel more. */
   #define FONT_WIDTH 5
   #define FONT_H_SPACE (FONT_WIDTH + 1)  /* 6 Pixels */
   #define FONT_HEIGHT 8
   #include "fonts/5x8_vertikal_LSB_1.h"   //******Font_5x8 (create by mike0815 *** 23-09-2015 )
 #define FONT_SELECTED
 #endif

#if defined(FONT_6X8_2) || defined(FONT_6x8_2)
   #define FONT_WIDTH 6
   #define FONT_HEIGHT 8
   #include "fonts/6x8_vertikal_LSB_2.h"   //***Font_6x8 ISO-8859-2 (02-01-2021)***
 #define FONT_SELECTED
 #endif

 #if defined(FONT_6X8) || defined (FONT_6x8)
  #define FONT_WIDTH 6
  #define FONT_HEIGHT 8
  #include "fonts/6x8_vertikal_LSB_1.h"
  #define FONT_SELECTED
 #endif

 #if defined(FONT_7X8) || defined(FONT_7x8)
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_WIDTH 7
   #define FONT_HEIGHT 8
   #include "fonts/7x8_vertikal_LSB_1.h" 
   #define FONT_SELECTED
  #endif
 #endif

 #if defined(FONT_8X8_2) || defined(FONT_8x8_2)
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_WIDTH 8
   #define FONT_HEIGHT 8
   #include "fonts/8x8_vertikal_LSB_2.h" //***Font_8x8 ISO-8859-2 (08-01-2021)***
   #define FONT_SELECTED
  #endif
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
   #define FONT_HEIGHT 15  /* use 8x14 font with extra space */
   #include "fonts/8x14_vertikal_LSB_1.h"
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
   #include "fonts/7x12_vertikal_LSB_1.h"
  #endif
 #endif

 #if defined(FONT_7X12x) || defined(FONT_7x12x)
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 7
   #define SFONT_WIDTH 8
   #define FONT_H_SPACE SFONT_WIDTH  /* 8 Pixels */
   #define FONT_HEIGHT 12
   #include "fonts/7x12_vertikal_LSB_1.h"
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

 #if defined(FONT_10X16) || defined(FONT_10x16)
  #ifdef FONT_SELECTED
   #warning Multiple fonts selected, please select only one!
  #else
   #define FONT_SELECTED
   #define FONT_WIDTH 10
   #define FONT_HEIGHT 16
   #include "fonts/10x16_vertikal_LSB_1.h"
  #endif
 #endif

 #ifndef FONT_SELECTED
   #warning No Font specified. Check Makefile!
   #define FONT_WIDTH 6
   #define FONT_HEIGHT 8
   #include "fonts/6x8_vertikal_LSB_1.h"
   #define FONT_SELECTED
 #endif
 
 #ifndef FONT_H_SPACE
  #define FONT_H_SPACE FONT_WIDTH  /* usually the horizontal space is same as FONT_WIDTH */
 #endif
 #ifndef SFONT_WIDTH
  #define SFONT_WIDTH FONT_WIDTH
 #endif

#endif /* LCD_GRAPHIC_TYPE != 0 */

/* ****************************** EOF ***************************************** */

