#include "config.h"


#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 7108))

// Options for lcd_pgm_bitmap option parameter:
#define OPT_HREVERSE    1 // Display bitmap reversed horizontally
#define OPT_VREVERSE    2 // Display bitmap reversed vertically
#define OPT_CINVERSE	4 // color is inverted (black to white ...) not used!

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64

 #ifdef FONT_8X16
  #define FONT_WIDTH    8
  #define FONT_HEIGHT   16
 #endif

 #ifdef FONT_8X16thin
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    8
   #define FONT_HEIGHT   16
  #endif
 #endif

 #ifdef FONT_8X15
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    8
   #define FONT_HEIGHT   15
  #endif
 #endif

 #ifdef FONT_8X14
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    8
   #define FONT_HEIGHT   14
  #endif
 #endif

 #ifdef FONT_8X12
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    8
   #define FONT_HEIGHT   12
  #endif
 #endif

 #ifdef FONT_8X8
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    8
   #define FONT_HEIGHT   8
  #endif
 #endif

 #ifdef FONT_6X8
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    6
   #define FONT_HEIGHT   8
  #endif
 #endif

 #ifdef FONT_10X16
  #ifndef FONT_WIDTH
   #define FONT_WIDTH    10
   #define FONT_HEIGHT   16
  #endif
 #endif
#endif


/* *********************************************************************************************************** */
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))

//LCD-commands
#define CMD_DISPLAY_OFF         0xAE
#define CMD_DISPLAY_ON          0xAF

#define CMD_SET_DISP_START_LINE 0x40
#define CMD_SET_PAGE            0xB0

#define CMD_SET_COLUMN_UPPER    0x10
#define CMD_SET_COLUMN_LOWER    0x00

#define CMD_SET_ADC_NORMAL      0xA0
#define CMD_SET_ADC_REVERSE     0xA1

#define CMD_SET_DISP_NORMAL     0xA6
#define CMD_SET_DISP_REVERSE    0xA7

#define CMD_SET_ALLPTS_NORMAL   0xA4
#define CMD_SET_ALLPTS_ON       0xA5
#define CMD_SET_BIAS_9          0xA2 
#define CMD_SET_BIAS_7          0xA3

#define CMD_RMW                 0xE0
#define CMD_RMW_CLEAR           0xEE
#define CMD_INTERNAL_RESET      0xE2
#define CMD_SET_COM_NORMAL      0xC0
#define CMD_SET_COM_REVERSE     0xC8
#define CMD_SET_POWER_CONTROL   0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST    0x81
 #if (LCD_ST_TYPE == 1306)
 #define VOLUME_VALUE   60	/* second byte of CMD_SET_VOLUME == volume value */
 #define CMD_SET_MEMORY_ADDR_MODE 0x20
 #define CMD_SET_ChargePump	0x8D
 #define CMD_SET_MUX_RATIO	0xA8
 #define CMD_SET_DISPLAY_OFFSET	0xD3
 #define CMD_SET_DIVIDE_RATIO	0xD5
 #define CMD_SET_PreCharge_PERIOD 0xD9
 #define CMD_SET_COM_Pins_CONFIG 0xDA
 #define CMD_SET_Vcomh_DESELECT_LEVEL 0xDB
 #else
 #define VOLUME_VALUE   40	/* second byte of CMD_SET_VOLUME == volume value */
 #endif
#define CMD_SET_STATIC_OFF      0xAC
#define CMD_SET_STATIC_ON       0xAD
#define CMD_SET_STATIC_REG      0x0
#define CMD_SET_BOOSTER_FIRST   0xF8
#define CMD_SET_BOOSTER_234     0
#define CMD_SET_BOOSTER_5       1
#define CMD_SET_BOOSTER_6       3
#define CMD_NOP                 0xE3
#define CMD_TEST                0xF0

//Makros for LCD
#define lcd_write_cmd(cmd)                     _lcd_hw_write(0x00, cmd);
#define lcd_write_data(data)                   _lcd_hw_write(0x01, data);
#define lcd_shift_right() // ignored
#define lcd_shift_left()  // ignored

#define LCDLoadCustomChar(addr) //load Custom-character (ignored)
#define lcd_cursor_on()  // ignored
#define lcd_cursor_off() // ignored

/* *********************************************************************************************************** */
#elif (LCD_ST_TYPE == 7108) 	/* not ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306)) */
#define CMD_DISPLAY_OFF	0x3e
#define CMD_DISPLAY_ON	0x3f
#define CMD_SET_COLUMN_ADDR	0x40
#define CMD_SET_PAGE		0xb8
#define CMD_SET_START_LINE	0xC0		/* set the start line for ST7108, Z address */

//Makros for LCD
#define lcd_write_cmd(cmd)                     _lcd_hw_write(0x00, cmd);
#define lcd_write_data(data)                   _lcd_hw_write(0x01, data);
#define lcd_cursor_on()  // ignored
#define lcd_cursor_off() // ignored

/* *********************************************************************************************************** */
#elif (LCD_ST_TYPE == 7920) 	/* not ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108)) */
#define lcd_write_init(data_length)            _lcd_hw_write(0x80, CMD_SetIFOptions | (data_length << 4))
#define lcd_cursor_on()  // ignored
#define lcd_cursor_off() // ignored
#define CMD_CLEAR	0x01		/* clear display, basic Instruction */
#define CMD_CGRAM_SELECT 0x02		/* SR=0, enable CGRAM address */
#define CMD_SET_ENTRY_MODE 0x04		/* set cursor posistion and shift for read/write */
 #define MODE_RIGHT_MOVE 0x02
#define CMD_DISPLAY_CONTROL	0x08	/* display control, basic Instruction */
 #define DISPLAY_ON 0x4
 #define CURSOR_ON 0x02
 #define BLINK_ON 0x01
#define CMD_DISPLAY_ON	(CMD_DISPLAY_CONTROL | DISPLAY_ON)
#define CMD_DISPLAY_OFF	CMD_DISPLAY_CONTROL
#define CMD_CURSOR_DISPLAY_CONTROL 0x10
#define CMD_SET_FUNCTION 0x20		/* basic + extended Instruction */
 #define MODE_8_BIT 0x10		/* Bit 4 is 8 bit mode */
 #define MODE_EXTENDED 0x04		/* Bit 2 is used for extended Intruction */
 #define GRAPHIC_DISPLAY_ON 0x02	/* Bit 1 is used for graphic display on, only extended Instruction */
#define CMD_SET_GDRAM_ADDRESS 0x80	/* set graphic display RAM address ,extended Instruction */
					/* 2x, first vertical address 5:0, second horizontal address 3:0 */
					/* followed by pairs of horizontal data (16-bit) */
//Makros for LCD
#define lcd_write_cmd(cmd)                     _lcd_hw_write(0x00, cmd);
#define lcd_write_data(data)                   _lcd_hw_write(0x01, data);
#define lcd_cursor_on()  // ignored
#define lcd_cursor_off() // ignored

/* *********************************************************************************************************** */
#else /* not ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 7108)) */
/* must be a character display! */
#define lcd_write_cmd(cmd)                     _lcd_hw_write(0x00, cmd); wait50us();
#define lcd_write_data(data)                   _lcd_hw_write(0x01, data); wait50us();
#define lcd_write_init(data_length)            _lcd_hw_write(0x80, CMD_SetIFOptions | (data_length << 4))



//LCD-commands
#define CLEAR_DISPLAY 0x01
#define CMD_SetEntryMode         0x04
#define CMD_SetDisplayAndCursor  0x08
#define CMD_SetIFOptions         0x20
#define CMD_SetCGRAMAddress      0x40    // for Custom character
#define CMD_SetDDRAMAddress      0x80    // set Cursor 
#define CMD1_SetBias             0x10	// set Bias (instruction table 1, DOGM)
#define CMD1_PowerControl        0x50	// Power Control, set Contrast C5:C4 (instruction table 1, DOGM)
#define CMD1_FollowerControl     0x60	// Follower Control, amplified ratio (instruction table 1, DOGM)
#define CMD1_SetContrast         0x70	// set Contrast C3:C0 (instruction table 1, DOGM)
#define CMD_DISPLAY_OFF          0x08
#define CMD_DISPLAY_ON           0x0c

//Makros for LCD
#define lcd_shift_right() lcd_command(0x1c)
#define lcd_shift_left() lcd_command(0x18)

#define LCDLoadCustomChar(addr) lcd_command(CMD_SetCGRAMAddress | (addr<<3))	//load Custom-character
#define lcd_cursor_on()  lcd_command(CMD_SetDisplayAndCursor | 0x06)
#define lcd_cursor_off() lcd_command(CMD_SetDisplayAndCursor | 0x04)

#endif

//defines for the cyrillic character set
#ifdef WITH_GRAPHICS
 #define GR_OFFSET1 0x20	/* shift 0xa0 to 0x80, direct behind ASCII table, to save place in font table */
 #define GR_OFFSET2 0xd0	/* shift 0xe0 to 0x10, before the ASCII table */  
#else
 #define GR_OFFSET1 0
 #define GR_OFFSET2 0
#endif

#define Cyr_B	(0xa0 - GR_OFFSET1)
#define Cyr_G	(0xa1 - GR_OFFSET1)
#define Cyr_Jo	(0xa2 - GR_OFFSET1)
#define Cyr_Zsch (0xa3 - GR_OFFSET1)
#define Cyr_Z	(0xa4 - GR_OFFSET1)
#define Cyr_I	(0xa5 - GR_OFFSET1)
#define Cyr_J	(0xa6 - GR_OFFSET1)
#define Cyr_L	(0xa7 - GR_OFFSET1)
#define Cyr_P	(0xa8 - GR_OFFSET1)
#define Cyr_U	(0xa9 - GR_OFFSET1)
#define Cyr_F	(0xaa - GR_OFFSET1)
#define Cyr_Tsch (0xab - GR_OFFSET1)
#define Cyr_Sch (0xac - GR_OFFSET1)
#define Cyr_HH	(0xad - GR_OFFSET1)
#define Cyr_Y	(0xae - GR_OFFSET1)
#define Cyr_E	(0xaf - GR_OFFSET1)

#define Cyr_Ju	(0xb0 - GR_OFFSET1)
#define Cyr_Ja	(0xb1 - GR_OFFSET1)
#define Cyr_b	(0xb2 - GR_OFFSET1)
#define Cyr_v	(0xb3 - GR_OFFSET1)
#define Cyr_g	(0xb4 - GR_OFFSET1)
#define Cyr_jo	(0xb5 - GR_OFFSET1)
#define Cyr_zsch (0xb6 - GR_OFFSET1)
#define Cyr_z	(0xb7 - GR_OFFSET1)
#define Cyr_i	(0xb8 - GR_OFFSET1)
#define Cyr_j	(0xb9 - GR_OFFSET1)
#define Cyr_k	(0xba - GR_OFFSET1)
#define Cyr_l	(0xbb - GR_OFFSET1)
#define Cyr_m	(0xbc - GR_OFFSET1)
#define Cyr_n	(0xbd - GR_OFFSET1)
#define Cyr_p	(0xbe - GR_OFFSET1)
#define Cyr_t	(0xbf - GR_OFFSET1)

#define Cyr_tsch (0xc0 - GR_OFFSET1)
#define Cyr_sch (0xc1 - GR_OFFSET1)
#define Cyr_hh	(0xc2 - GR_OFFSET1)
#define Cyr_y	(0xc3 - GR_OFFSET1)
#define Cyr_ww	(0xc4 - GR_OFFSET1)
#define Cyr_e	(0xc5 - GR_OFFSET1)
#define Cyr_ju	(0xc6 - GR_OFFSET1)
#define Cyr_ja	(0xc7 - GR_OFFSET1)

#define Cyr_D	(0xe0 - GR_OFFSET2)
#define Cyr_C	(0xe1 - GR_OFFSET2)
#define Cyr_Schtsch (0xe2 - GR_OFFSET2)
#define Cyr_d	(0xe3 - GR_OFFSET2)
#define Cyr_f	(0xe4 - GR_OFFSET2)
#define Cyr_c	(0xe5 - GR_OFFSET2)
#define Cyr_schtsch (0xe6 - GR_OFFSET2)

//defines for the big special "character" set, the graphical symbols
#define ICON_WIDTH 24
#define ICON_HEIGHT 32
#define BJT_NPN 0
#define N_E_IGBT 1
#define N_E_MOS 2
#define N_JFET 3
#define TRIAC 4
#define THYRISTOR 5
#define INDUCTOR 6
#define RESISTOR 7
#define CAPACITOR 8
#define DIODE_C_A 9
#define DIODES_C_A_C_A 10
#define DIODES_A_C_C_A 11
#define DIODES_C_A_A_C 12
#define RESISTORS 13
// define the positioning bits for lcd_big_icon
#define LCD_UPPER_LEFT 0x30
#define LCD_UPPER_RIGHT 0x20
#define LCD_LOWER_LEFT 0x10
#define LCD_LOWER_RIGHT 0x00
