#include <avr/pgmspace.h>
#include "24x32update_bitmaps.h"


//*****************************************************************************
//
// The data for the bitmap bmp_one
//
// Memory usage: 7 bytes
//*****************************************************************************
#if defined (MAIN_C)
 #ifdef INVERSE_TP
  #define TP_DATA_COUNT 7
 #else
  #define TP_DATA_COUNT 5
 #endif
 const unsigned char bmp_one_data[(TP_DATA_COUNT * 1) + 2] PROGMEM ={
	TP_DATA_COUNT, /* The width of the bitmap. */
	8, /* The height of the bitmap. */
 #ifdef INVERSE_TP
        0xff, 0xff, 0xf7, 0xfb, 0x81, 0xff, 0xff
 #else
  #ifdef BIG_TP
        0x00, 0x04, 0x02, 0xff, 0x00
  #else
	0x00, 0x08, 0x04, 0x7e, 0x00
  #endif
 #endif
};
#else
 extern const unsigned char bmp_one_data[] PROGMEM ;
#endif


//*****************************************************************************
//
// The data for the bitmap bmp_two
//
// Memory usage: 7 bytes
//*****************************************************************************
#if defined (MAIN_C)
 const unsigned char bmp_two_data[(TP_DATA_COUNT * 1) + 2] PROGMEM ={
	TP_DATA_COUNT, /* The width of the bitmap. */
	8, /* The height of the bitmap. */
 #ifdef INVERSE_TP
        0xff, 0xbb, 0x9d, 0xad, 0xad, 0xb3, 0xff
 #else
  #ifdef BIG_TP
        0xc2, 0xa1, 0x91, 0x89, 0x86
  #else
	0x44, 0x62, 0x52, 0x52, 0x4c
  #endif
 #endif
};
#else
 extern const unsigned char bmp_two_data[] PROGMEM ;
#endif


//*****************************************************************************
//
// The data for the bitmap bmp_three
//
// Memory usage: 7 bytes
//*****************************************************************************
#if defined (MAIN_C)
 const unsigned char bmp_three_data[(TP_DATA_COUNT * 1) + 2] PROGMEM ={
	TP_DATA_COUNT, /* The width of the bitmap. */
	8, /* The height of the bitmap. */
 #ifdef INVERSE_TP
        0xff, 0xdb, 0xbd, 0xb5, 0xb5, 0xcb, 0xff
 #else
  #ifdef BIG_TP
        0x42, 0x81, 0x89, 0x89, 0x76
  #else
	0x24, 0x42, 0x4a, 0x4a, 0x34
  #endif
 #endif
};
#else
 extern const unsigned char bmp_three_data[] PROGMEM ;
#endif



//*****************************************************************************
//
// The data for the bitmapbmp_vakdiode
//
// Memory usage: 34 bytes
//*****************************************************************************
#if defined (MAIN_C)
 const unsigned char bmp_vakdiode[(8 * 4) + 4] PROGMEM ={
        24,
        0,
	8, // The width of the bitmap.
	32, // The height of the bitmap.
 #if (ICON_TYPE == 1)
	0x04, 0x04, 0x04, 0x04, 0xFC, 0x00, 0x00, 0x00,
	0x00, 0x20, 0x60, 0xE0, 0xFF, 0xE0, 0x60, 0x20,
	0x00, 0x02, 0x02, 0x02, 0xFF, 0x02, 0x02, 0x02,
	0x20, 0x20, 0x20, 0x20, 0x3F, 0x00, 0x00, 0x00,

 #else
	0x06, 0x06, 0x06, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x20, 0x60, 0xE0, 0xFF, 0xFF, 0xE0, 0x60, 0x20,
	0x06, 0x06, 0x06, 0xFF, 0xFF, 0x06, 0x06, 0x06,
	0x60, 0x60, 0x60, 0x7F, 0x7F, 0x00, 0x00, 0x00, 
 #endif
};
#else
 extern const unsigned char bmp_vakdiode[] PROGMEM ;
#endif


//*****************************************************************************
//
// The data for the bitmap bmp_inductor
//
// Memory usage: 18 bytes
//*****************************************************************************
#if defined (MAIN_C)
 const unsigned char bmp_inductor_data[(16 * 1) + 2] PROGMEM ={
        16 , // The width of the bitmap.
         8, // The height of the bitmap.
	0x40, 0x7e, 0x01, 0x01,  0x3e, 0x01, 0x01, 0x3e, 
	0x01, 0x01, 0x3e, 0x01,  0x01, 0x7e, 0x40, 0x40, 
};
#else
 extern const unsigned char bmp_inductor_data[] PROGMEM ;
#endif

#if (LCD_ST_TYPE == 7920)
 #if defined (MAIN_C)
  const unsigned char set_msk[] PROGMEM = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
  const unsigned char clear_msk[] PROGMEM = { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
 #else
  extern const unsigned char set_msk[] PROGMEM ;
  extern const unsigned char clear_msk[] PROGMEM ;
 #endif
#endif

