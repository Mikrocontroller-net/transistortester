#include <avr/io.h>
#include <avr/pgmspace.h> 

#ifdef LCD_ST7565 
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

