// Interfacing a HD44780 compatible LCD with  4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//
// Pin selection can be changed in lcd-routines.h with define
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "Transistortester.h"

#ifdef STRIP_GRID_BOARD
 #warning "strip-grid-board layout selectet!"
#endif


 
uint8_t lcd_text_line;
uint8_t _lcd_column;
uint8_t last_text_line;
uint8_t last_text_column;

#if (LCD_GRAPHIC_TYPE != 0)
 #define NR_BYTE ((FONT_HEIGHT + 7) / 8)
uint8_t _page;		// y position of the next character 
uint8_t _xpos;		// x position of the next character
uint8_t icon_xx;	// x position of the last loaded big 24x32 icon
uint8_t icon_yy;	// y position of the last loaded big 24x32 icon
#endif

/* ******************************************************************************* */
// sends numeric character (Pin Number) to the LCD 
// from binary 0 we send ASCII 1 ....
void lcd_testpin(unsigned char temp) {
   lcd_data(temp + '1' + TP_OFFSET);
}

/* ******************************************************************************* */
// send space character to LCD
void lcd_space(void) {
   lcd_data(' ');
}
#if FLASHEND > 0x1fff
void lcd_spaces(uint8_t nn) {
   while (nn != 0) {
     lcd_space();
     nn--;
   }
}
#endif
// send equal character to LCD
void lcd_equal(void) {
   lcd_data('=');
}

/* ******************************************************************************* */
//move to the beginning of the 1. row
void lcd_line1() {
   lcd_text_line = 0;
#if (LCD_GRAPHIC_TYPE != 0)
   lcd_set_cursor(0 * ((FONT_HEIGHT + 7) / 8),0);
#elif (LCD_ST_TYPE == 7920)
   _page = 0;			// _page is the vertical pixel address for ST7920 controller
   _xpos = 0;			// LCD_ST7565_H_OFFSET is not used for ST7920 controller
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row1));
#endif
}

/* ******************************************************************************* */
//move to the beginning of the 2. row
// for the ST7920 you can select a accurate vertical pixel positioning of line with:
// #define FONT_V_SPACE FONT_HEIGHT
// or you can select a 8-line rounding of the positioning of the lines with: 
#define FONT_V_SPACE (((FONT_HEIGHT + 7) / 8) * 8)
void lcd_line2() {
   lcd_text_line = 1;
#if (LCD_GRAPHIC_TYPE != 0)
   lcd_set_cursor(1 * ((FONT_HEIGHT + 7) / 8),0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row2));
#endif
}

/* ******************************************************************************* */
//move to the beginning of the 3. row
void lcd_line3() {
   lcd_text_line = 2;
#if (LCD_GRAPHIC_TYPE != 0)
   lcd_set_cursor(2 * ((FONT_HEIGHT + 7) / 8),0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row3));
#endif
}

/* ******************************************************************************* */
//move to the beginning of the 4. row
void lcd_line4() {
   lcd_text_line = 3;
#if (LCD_GRAPHIC_TYPE != 0)
   lcd_set_cursor(3 * ((FONT_HEIGHT + 7) / 8),0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row4));
#endif
}

/* ******************************************************************************* */
// position to next line to column xx
// Text_line number is incremented by 1, if possible (not yet at the last line).
// If already at the last line of the display, last_line_used is set to 1 .
void lcd_next_line(uint8_t xx) {
lcd_text_line ++;
if (lcd_text_line > (LCD_LINES - 1))  {
   // Limit is reached
   lcd_text_line = (LCD_LINES - 1);
   last_line_used = 1;
} else {
   last_line_used = 0;
}
lcd_set_cursor((uint8_t)(lcd_text_line * PAGES_PER_LINE), xx);
}
/* ******************************************************************************* */
#ifdef WAIT_LINE2_CLEAR
void lcd_next_line_wait(uint8_t xx) {
 lcd_next_line(xx);
 wait_for_key_5s_line2();		// wait 5s and clear last line
}
#endif

/* ************************************************************************************** */
/* Set the character position to x,y , where x specifies the character number in a text line. */
/* The y position is the page address (8 line units).                                     */
/* For most controllers the y position must be increased by (Height + 7) / 8 for the next text line */
void lcd_set_cursor(uint8_t y, uint8_t x) {
 _lcd_column = x;
#if (LCD_GRAPHIC_TYPE != 0)
//  unsigned char xx;
   //move to the specified position (depends on used font)
   _page = y;
    // The pixel memory is greater as the display window.
    // For example the SPL501 controller has 132x65 dot matrix memory
    // LCD_ST7565_H_OFFSET specifies the offset of the 128 pixel of the display window.
   _xpos = (x * FONT_WIDTH);
#else
   // move to the specified position for character display
   if (y == 1) {
     lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row2 + x));
 #if (LCD_LINES > 2)
   } else if (y == 2) {
     lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row3 + x));
   } else if (y == 3) {
     lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row4 + x));
 #endif
   } else {
     lcd_command((uint8_t)(CMD_SetDDRAMAddress + LCD_Row1 + x));
   }
#endif
}

#if FLASHEND > 0x1fff
/* ************************************************************************************** */
uint8_t lcd_save_position(void) {
 last_text_column = _lcd_column;
 last_text_line = lcd_text_line;
 return(last_text_line);
}
/* ************************************************************************************** */
void lcd_restore_position(void) {
 lcd_set_cursor((uint8_t)last_text_line * PAGES_PER_LINE, (uint8_t)(last_text_column));
}
#endif

/* ******************************************************************************* */
// send a 24x32 icon to one quarter of the screen
// the number temp1 is the icon-number  from 0 to 15
// with offset  0 the icon is written to the right lower quarter of the screen
// with offset 16, 0x10 the icon is written to the left lower quarter of the screen
// with offset 32, 0x20 the icon is written to the right upper quarter of the screen
// with offset 48, 0x30 the icon is written to the left upper quarter of the screen
// to the left of the quarter are at least 8 pixels free
// to the right of the quarter are 16 pixels free  (8+24+16 < (SCREEN_WIDTH/2)
#if (LCD_GRAPHIC_TYPE != 0) && (WITH_GRAPHICS != 0) 
void lcd_big_icon(unsigned char temp1) {
 uint8_t *pfont;	// pointer to the start of font data
 uint8_t pos_nr;
 #define TP_WIDTH 8	/* specifies the width of the Test Port number */
 #define DIODE_WIDTH 5    /* diode is 8 pixel width, but TP overlap the diode images with 3 pixels */
 pfont = (uint8_t *) bigfont[temp1 & 0x0f];	// first byte of character data
 pos_nr = temp1 & 0x30;	// filter the position Information
 icon_xx =  TP_WIDTH;		// left side 
 if ((pos_nr & 0x10) == 0) icon_xx += (SCREEN_WIDTH / 2); // right side
 icon_yy = 0;
 if (pos_nr < 0x20) {
   icon_yy = (SCREEN_HEIGHT / 2);	// lower half of display
   // shift lower icon position to the right
   icon_xx +=  (SCREEN_WIDTH / 2)  - ICON_WIDTH - TP_WIDTH - TP_WIDTH - DIODE_WIDTH;
 }

 lcd_set_pixels( pfont, icon_xx, icon_yy, 0, ICON_WIDTH, ICON_HEIGHT);	// write the pixel data of the Icon
}

/* ******************************************************************************* */
// update a icon 
// The left upper corner of the last loaded Icon is positioned to icon_xx, icon_yy
// You can change parts of this Icon with a lcd_update_icon call.
// The only parameter is a pointer to a array of bytes.
// The first two bytes specify the location of the change bitmap relative to the upper left corner
// (the start position) of the last loaded Icon ( call of lcd_big_icon(ICON_NUMBER);).
// the next two bytes specify the width and height of the correction bitfield,
// followed by the bitfield data.
void lcd_update_icon(const unsigned char *ubitmap) {
 lcd_update_icon_opt(ubitmap, 0);	// call with no special option
}
/* ******************************************************************************* */
void lcd_update_icon_opt(const unsigned char *ubitmap, unsigned char options) {
 const unsigned char *pfont;
 uint8_t xx, yy;
 uint8_t ww, hh;
 pfont =  &ubitmap[4];				// the bitfield data begin at position 4
 xx = icon_xx + pgm_read_byte(&ubitmap[0]);	// first byte = offset x to left (upper) corner of the Icon
 yy = icon_yy + pgm_read_byte(&ubitmap[1]);	// second byte = offset y to (left) upper corner of the Icon
 ww =  pgm_read_byte(&ubitmap[2]);		// the width of the bitfield
 hh =  pgm_read_byte(&ubitmap[3]);		// the height of the bitfield (must be multiple of 8 for ST7565)
 lcd_set_pixels( pfont, xx, yy, options, ww, hh);	// update the icon at the specified position with specified size
}
#endif

/* ******************************************************************************* */
/* ******************************************************************************* */
// sends a character to the LCD 
void lcd_data(unsigned char temp1) {
/* -------------------------------------------------------------------------- */

#if (LCD_GRAPHIC_TYPE != 0)
 uint8_t *pfont;
 pfont = (uint8_t *)font + (FONT_WIDTH * NR_BYTE * temp1);	// first byte of character data
 // for other controllers like ST7565 the _page specifies the page of the controller (8 lines)
 lcd_set_pixels( pfont, _xpos, _page*8, 0, (unsigned char)FONT_WIDTH, (unsigned char)(NR_BYTE*8));
_xpos += FONT_WIDTH;		// move pointer to the next character position
#else
 lcd_write_data(temp1);		// set RS to 1
#endif

#ifdef WITH_UART
 switch(temp1) {
    case LCD_CHAR_DIODE1:
    	uart_putc('>');
    	uart_putc('|');
    	break;
    case LCD_CHAR_DIODE2:
    	uart_putc('|');
    	uart_putc('<');
    	break;
    case LCD_CHAR_CAP:
    	uart_putc('|');
    	uart_putc('|');
    	break;
    case LCD_CHAR_RESIS1:
    case LCD_CHAR_RESIS2:
    	uart_putc('R');
    	break;
    case LCD_CHAR_LINE1:
        uart_putc('-');
        break;
//    case LCD_CHAR_DEGREE:	// degree sign
//    	uart_putc(0xf8);	// codepage 437 or 850 has degree
//    	break;
    case LCD_CHAR_U:		//µ
//    	uart_putc(0xe6);	// codepage 437 or 850 has my
    	uart_putc('u');		// better use the ASCII u
    	break;
    case LCD_CHAR_OMEGA:	//Omega
        uart_putc('O');
        uart_putc('h');
        uart_putc('m');
    	break;
    default:
    	uart_putc(temp1);
 }
#endif
} /* end lcd_data() */
 
// sends a command to the LCD
 
void lcd_command(unsigned char temp1) {
	_lcd_hw_write(0x00,temp1);
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814) || (LCD_ST_TYPE == 7735))
 ;
#else
	wait50us();
#endif
//	wait1ms();
#ifdef WITH_UART
	if((temp1 == 0x80) || (temp1 == 0xC0)) {
		uart_newline();
	}
#endif
}
 
/* ******************************************************************************* */
// Initialise: 
// Must be called first .
// every controller has its own initialize sequence!
 
/* -------------------------------------------------------------------------- */
void lcd_init(void) {

#if (LCD_INTERFACE_MODE == MODE_SPI) || (LCD_INTERFACE_MODE == MODE_3LINE)
   HW_LCD_RES_PORT &= ~_BV(HW_LCD_RES_PIN); // RESET LCD-Display
   HW_LCD_EN_PORT  &= ~_BV(HW_LCD_EN_PIN);
   HW_LCD_B0_PORT  &= ~_BV(HW_LCD_B0_PIN);
   HW_LCD_CE_PORT  &= ~_BV(HW_LCD_CE_PIN);  // LCD CE to low

   HW_LCD_RES_DDR  |= _BV(HW_LCD_RES_PIN); // LCD RESET is Output
   HW_LCD_EN_DDR   |= _BV(HW_LCD_EN_PIN);  // LCD SCL is Output
   HW_LCD_B0_DDR   |= _BV(HW_LCD_B0_PIN);  // LCD SI is Output
   HW_LCD_CE_DDR   |= _BV(HW_LCD_CE_PIN);  // LCD CE is Output
   wait10us();
 #ifdef LCD_SPI_OPEN_COL
  #ifdef PULLUP_DISABLE
    MCUCR = (0<<PUD);             // enable Pull-Up Resistors mega168 family
  #endif
   HW_LCD_CE_DDR   &= ~_BV(HW_LCD_CE_PIN);  // LCD CS is Input
   HW_LCD_CE_PORT  |= _BV(HW_LCD_CE_PIN);   // enable Pullup  = 1
 #else
   HW_LCD_CE_PORT  |= _BV(HW_LCD_CE_PIN);   // set CS to 1, unselected
   HW_LCD_CE_DDR   |= _BV(HW_LCD_CE_PIN);  // LCD CS is Output
 #endif
 #if (LCD_INTERFACE_MODE == MODE_3LINE)
   // for 3LINE mode the RS is chip enable, RS is send as first serial bit
  #ifdef LCD_SPI_OPEN_COL
   HW_LCD_RS_DDR &= ~_BV(HW_LCD_RS_PIN);	// switch RS (SCE) to input
   HW_LCD_RS_PORT |= _BV(HW_LCD_RS_PIN);	// enable PullUp RS (SCE)  
  #else
   HW_LCD_RS_PORT |= _BV(HW_LCD_RS_PIN);	// switch RS (SCE) to VCC
  #endif  /* LCD_SPI_OPEN_COL */
 #else
   HW_LCD_RS_PORT  &= ~_BV(HW_LCD_RS_PIN);
   HW_LCD_RS_DDR   |= _BV(HW_LCD_RS_PIN);  // LCD RS (SCE) is Output
 #endif

   wait_about100ms(); // Set LCD for 100 ms into RESET
 #ifdef LCD_SPI_OPEN_COL
  #ifdef PULLUP_DISABLE
    MCUCR = (0<<PUD);             // enable Pull-Up Resistors mega168 family
  #endif
   HW_LCD_RES_DDR &= ~_BV(HW_LCD_RES_PIN);	// switch RES to input
   HW_LCD_RES_PORT |= _BV(HW_LCD_RES_PIN);	// enable PullUp RES 
 #else
   HW_LCD_RES_PORT |= _BV(HW_LCD_RES_PIN);	// switch RES to VCC
 #endif  /* LCD_SPI_OPEN_COL */
   wait_about100ms();  // Wait for 100 ms after RESET
#elif (LCD_INTERFACE_MODE == MODE_I2C)   /* ! MODE_SPI || MODE_3LINE */
   i2c_init();		// init the I2C interface
   wait_about100ms();	// Set LCD for 100 ms into RESET
#elif (LCD_INTERFACE_MODE == MODE_7920_SERIAL) /* ! MODE_SPI || MODE_3LINE || MODE_I2C */
   HW_LCD_B0_PORT  &= ~_BV(HW_LCD_B0_PIN); // LCD B0 = 0
   HW_LCD_B0_DDR   |= _BV(HW_LCD_B0_PIN);  // LCD SI is Output
   HW_LCD_EN_PORT  &= ~_BV(HW_LCD_EN_PIN); // LCD EN = 0
   HW_LCD_EN_DDR   |= _BV(HW_LCD_EN_PIN);  // LCD SCL is Output
//   HW_LCD_RESET_PORT &= ~_BV(HW_LCD_RESET_PIN); // set reset low
//   HW_LCD_RESET_DDR  |= _BV(HW_LCD_RESET_PIN); // LCD RESET is Output
//   wait1ms();
//   HW_LCD_RESET_PORT |= _BV(HW_LCD_RESET_PIN); // set reset high
   wait100ms();		// wait after power up reset
#elif (LCD_INTERFACE_MODE == MODE_PARALLEL) /* ! MODE_SPI || MODE_3LINE || MODE_I2C || MODE_7920_SERIAL */
 /* ! MODE_SPI || MODE_3LINE || MODE_I2C || MODE_7920_SERIAL */
 #ifdef LCD_ST_TYPE
   wait100ms();		// wait after power up reset
   lcd_command(0x22); // set to 4-bit mode for graphic display
   lcd_command(0x22 ); // set to 4-bit mode
 #endif
#endif

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
   lcd_command(CMD_DISPLAY_OFF);		// display off
   lcd_command(CMD_INTERNAL_RESET);		// 0xe2
   lcd_command(CMD_SET_BIAS_9);			// 0xa3
   lcd_command(CMD_SET_ADC_NORMAL);		// 0xa0
   lcd_command(CMD_SET_COM_NORMAL);		// 0xc0

 #if (LCD_ST_TYPE == 7565)
   //Booster on, Voltage regulator/follower circuit on
   lcd_command(CMD_SET_POWER_CONTROL | 7);	// 0x28 BOOSTER ON | V_REGULATOR ON | V_FOLLOWER ON
   lcd_command(CMD_SET_RESISTOR_RATIO | (7 & LCD_ST7565_RESISTOR_RATIO));	// 0x20

   lcd_command(CMD_SET_VOLUME_FIRST);		// 0x81 set  volume command
   lcd_command(eeprom_read_byte(&EE_Volume_Value) & 0x3f);	//set volume value of EEprom


//   lcd_command(CMD_SET_STATIC_OFF);		// 0xac , set Static indicator off
//   lcd_command(CMD_SET_STATIC_REG | 0);	// 0x00 , set Static indicator no flashing

//##   lcd_command(CMD_RMW);			// 0xe0 , set Read Modify Write mode

 #endif
 #if (LCD_ST_TYPE == 1306)

   lcd_command(CMD_SET_DIVIDE_RATIO);		// set display clock div (0xd5), 0x70 is default	
   lcd_command(0x80);		// suggested ratio

//   lcd_command(CMD_SET_MUX_RATIO);		// set multiplex (0xa8)
//   lcd_command(0x3f);

//   lcd_command(CMD_SET_DISPLAY_OFFSET);	// set display offset (0xd3)
//   lcd_command(0x00);		// no offset

   lcd_command(CMD_SET_ChargePump);		// charge pump setting (0x8d)
   lcd_command(0x14);

//   lcd_command(CMD_SET_MEMORY_ADDR_MODE);		// set memory addressing mode (0x20)
//   lcd_command(0x00);		// horizontal mode

//   lcd_command(CMD_SET_COM_Pins_CONFIG);		// sequential COM pin configuration (0xda)
//   lcd_command(0x12);		// disable COM Left/Right remap, alternative COM pin

   lcd_command(CMD_SET_VOLUME_FIRST);	// set contrast
   lcd_command(eeprom_read_byte(&EE_Volume_Value));	//set volume value of EEprom

//   lcd_command(CMD_SET_PreCharge_PERIOD);		// set precharge periodi (0xd9), 0x22 is reset value
//   lcd_command(0xf1);

//   lcd_command(CMD_SET_Vcomh_DESELECT_LEVEL);		// set Vcomh deselect level (0xdb), 0x20 is reset value
//   lcd_command(0x40);
//##   lcd_command(CMD_RMW);			// 0xe0 , set Read Modify Write mode
 #endif
   lcd_command(CMD_SET_DISP_START_LINE | (LCD_ST7565_Y_START & 0x3f));	// 0x40
   lcd_command(CMD_SET_ALLPTS_NORMAL);		// 0xa4
   lcd_command(CMD_SET_DISP_NORMAL);		// 0xa6 not reverse

   lcd_clear();

   lcd_command(CMD_DISPLAY_ON);			// 0xaf

   lcd_set_cursor(0,0);
#elif (LCD_ST_TYPE == 8812)    /*  !(LCD_ST_TYPE == 7565 || 1306) */
   wait_about100ms();
   lcd_command(CMD_SET_EXTENDED_INSTRUCTION);		// 0x21 switch power on, extended instruction
   lcd_command(ECMD_SET_HV_STAGES | (LCD_ST7565_RESISTOR_RATIO / 2)); // 0x08|0x03 set charge pump stages (3)
   lcd_command(ECMD_SET_BIAS | 4);			// 0x14 set bias system to 4
   lcd_command(ECMD_SET_TEMP_COEF | 1);			// 0x05 set temperature coefficient to 1
   lcd_command(ECMD_SET_CONTRAST | (eeprom_read_byte(&EE_Volume_Value) & 0x7f));	//set volume value of EEprom (0x4e=78) 
   lcd_command(CMD_SET_NORMAL_INSTRUCTION);		// switch power on, normal instruction
   lcd_clear();			// clear display
//   lcd_command(CMD_SET_DISPLAY_NORMAL);			// set display to normal mode
   lcd_command(CMD_SET_DISPLAY_INVERS);		// set display to inverse mode
   wait_about50ms();
#elif (LCD_ST_TYPE == 8814)
   uint8_t volume;
//   lcd_command(CMD_DISPLAY_OFF);		// display off
//   lcd_command(CMD_INTERNAL_RESET);		// 0xe2
   volume = eeprom_read_byte(&EE_Volume_Value);		// read Vop
   lcd_command(CMD_SET_VOP_UPPER | ((volume >> 5) & 0x07));	// set upper Vop
   lcd_command(CMD_SET_VOP_LOWER | (volume & 0x1f));	// set lower Vop
   lcd_command(CMD_SET_ALLPTS_NORMAL);		// 0xa4 set display to normal
   lcd_command(CMD_SET_POWER_CONTROL | 4);	// 0x28|4 Charge Pump ON
   lcd_command(CMD_SET_COM_REVERSE);		// 0xc8
   lcd_command(CMD_SET_COM_NORMAL);		// 0xc0 set normal Y orientation
   lcd_command(0xa1);				// set MX, X orientation, is pad selectet!
   lcd_command(0xaa);				// set horizontal mode
   lcd_command(0xac);				// set initial row
   lcd_command(0x08);				// initial row = 8  (0-7)
   lcd_clear();
   lcd_command(CMD_DISPLAY_ON);			// 0xaf Display on
   lcd_set_cursor(0,0);
/* -------------------------------------------------------------------------- */
#elif (LCD_ST_TYPE == 7920)  /* !(LCD_ST_TYPE == 7565 | 1306 | 8812 | 8814) */

   // reset the ST7920 controller
   lcd_command(CMD_CLEAR);			// clear display
   wait10ms();
   lcd_command(CMD_DISPLAY_CONTROL | DISPLAY_ON); // display on
   lcd_command(CMD_SET_ENTRY_MODE | MODE_RIGHT_MOVE); // increase address counter after read/write
   lcd_command(CMD_SET_FUNCTION | MODE_EXTENDED); // set to extended mode
   lcd_clear();
   lcd_command(CMD_SET_FUNCTION | MODE_EXTENDED | GRAPHIC_DISPLAY_ON);
   lcd_refresh();
   lcd_line1();
/* -------------------------------------------------------------------------- */
#elif (LCD_ST_TYPE == 7108)  /* !(LCD_ST_TYPE == 7565 | 1306 | 8812 | 8814 | 7920) */
   wait_about50ms();
   _lcd_hw_select(3);		// select both controllers
   lcd_command(CMD_SET_START_LINE | (LCD_ST7565_Y_START & 0x3f));	// set the start line 0
   lcd_clear();
   lcd_command(CMD_DISPLAY_ON);
   wait_about50ms();
/* -------------------------------------------------------------------------- */
#elif (LCD_ST_TYPE == 7735)  /* !(LCD_ST_TYPE == 7565 | 1306 | 8812 | 8814 | 7920 | 7108) */
   wait_about50ms();
   lcd_command(0x11);		// exit sleep
   wait_about20ms();
   lcd_command(0x26);		// set default Gamma
    lcd_data(0x04);		// Gamma 4
   lcd_command(0xb1);		// Frame Rate Control
    lcd_data(0x0e);		// DIVA=14, default 14(132*162) 17(128*128)
    lcd_data(0x10);		// VPA=16, default 20(132*162) 17(128*128)
   lcd_command(0xc0);		// Power_Control1
    lcd_data(0x0e);		// VRH=14, GVDD=4.1
    lcd_data(0x00);		// VC=0, VCI1=2.75
   lcd_command(0xc1);		// Power_Control2
    lcd_data(0x05);		// BT=5, AVDD=2x VCL=-1x VGH=6x VGL=-3x
   lcd_command(0xc5);		// VCOM_Control1
    lcd_data(0x38);		// VMH=56  VCOMH=3.900
    lcd_data(0x40);		// VML=64  VCOML=-0.900
   lcd_command(0x3a);		// set color format
    lcd_data(0x06);		// 18 bit/pixel
   lcd_command(0x36);		// Memory address control
    lcd_data(0x1c);	 	// ML=Vertical refresh, RGB=BGR color,MH=Refresh right to left
   lcd_command(0xb4);		// Display Inversion Control
    lcd_data(0x00);		// NLA=0, NLB=0, NLC=0
   // ? Gamma set

   lcd_clear();
   lcd_command(0x29);		// Display On
//   lcd_command(0x2c);		// RAMWR
   wait_about50ms();
/* -------------------------------------------------------------------------- */
#else    /* !(LCD_ST_TYPE == 7565 | 1306) | 7108 | 7920 | 7108 | 8812 | 8814 | 7735) */
/* must be a character display */
   wait_about100ms();
   // to initialise, send 3 times to be shure to be in 8 Bit mode

 #if LCD_INTERFACE_MODE == MODE_PARALLEL
   lcd_command(0x33);			// switch to 8-bit
   lcd_command(0x32);			// switch to 4-bit
 #else
   lcd_command(0x3a);			// switch to 8-bit for I2C or SPI
 #endif
   wait_about10ms();
 #ifdef LCD_DOGM
   uint8_t contrast;
   contrast = eeprom_read_byte(&EE_Volume_Value);
  #if LCD_ST7565_V_FLIP
   #define MODE_BDC 0x02
  #else
   #define MODE_BDC 0x00
  #endif
  #if LCD_ST7565_H_FLIP
   #define MODE_BDS 0x01
  #else
   #define MODE_BDS 0x00
  #endif
   lcd_command(CMD_SetIFOptions | MODE_8BIT | 0x0a);	// 8Bit / 2 rows /RE=1
  #ifdef FOUR_LINE_LCD
   lcd_command(0x09);				// 5-dot font, 4 line display NW=1
  #else
   lcd_command(0x08);				// 5-dot font, 2 line display NW=0
  #endif
   lcd_command(0x04 | MODE_BDC | MODE_BDS);	// BDC=1, bottom view
   lcd_command(CMD1_SetBias | 0x0e);		// 0x1e BS1:0=11, BS1=1

   lcd_command(CMD_SetIFOptions | MODE_8BIT | 0x09);	// 4Bit / 2 rows / 5x7 / Instr. table 1
//   lcd_command(CMD1_SetBias | 0x0b);		// 0x1b BS0=1 / F2:0 = 3, Bias = 1/6
   lcd_command(CMD1_SetBias | 0x0c);		// 0x1c 1/4 bias     (5V)

  #ifdef LCD_ST7565_RESISTOR_RATIO
   lcd_command(CMD1_FollowerControl | 0x08 | LCD_ST7565_RESISTOR_RATIO);	// 0x6e Follower on / Rab2:0 = RATIO
  #else
   lcd_command(CMD1_FollowerControl | 0x0e);	// 0x6e Follower on / Rab2:0 = 6
  #endif

//   lcd_command(CMD1_PowerControl | 0x02);	// 0x52 booster off / set contrast C5:C4 = 2
   lcd_command(CMD1_PowerControl | ((contrast>>4)&0x07));     // 0x5x booster on / set contrast C5:C4 
//   lcd_command(CMD1_SetContrast | 0x04);	// 0x74 set contrast C3:0 = 4
   lcd_command(CMD1_SetContrast | (contrast&0x0f));   // 0x7x set contrast C3:0 

   // old initialize without OLED display
   lcd_command(CMD_SetIFOptions | MODE_8BIT | 0x08);	// 4Bit / 2 rows / 5x7
   lcd_clear();
   lcd_command(CMD_DISPLAY_ON);			// 0x08 Display on, no Cursor, No blink
   lcd_command(CMD_SetEntryMode | 0x02);	// 0x06 increment / no Scroll    
 #else
   // initialize sequence with OLED display
   lcd_command(CMD_SetIFOptions | MODE_8BIT);		// Add for OLED
   lcd_command(CMD_SetIFOptions | MODE_8BIT);		// Add for OLED

   lcd_command(CMD_SetIFOptions | MODE_8BIT | 0x0A);	// 4Bit / 2 rows / 5x7 / table3 / Add for OLED
   lcd_command(CMD_SetDisplayAndCursor);	// Display off / no Blinking  / Add for OLED
   lcd_command(CMD_SetEntryMode | 0x02);	// increment / no Scroll
   lcd_command(CMD_CURSOR_HOME);		// Home Command  // Add for OLED
   lcd_clear();
   wait_about5ms();
   lcd_command(CMD_SetDisplayAndCursor | 0x04);	// Display on / Cursor off / no Blinking
 #endif

   LCDLoadCustomChar(LCD_CHAR_DIODE1);	//Custom-Character Diode symbol
   lcd_fix_customchar(DiodeIcon1);	//load Character  >|
   LCDLoadCustomChar(LCD_CHAR_DIODE2);	//Custom-Character 
   lcd_fix_customchar(DiodeIcon2);	//load Character  |<
   LCDLoadCustomChar(LCD_CHAR_CAP);	//Custom-Character  Capacitor symbol
   lcd_fix_customchar(CapIcon);		//load Character  ||
   LCDLoadCustomChar(LCD_CHAR_RESIS1);	//Custom-Character Resistor symbol
   lcd_fix_customchar(ResIcon1);		//load Character  [
   LCDLoadCustomChar(LCD_CHAR_RESIS2);	//Custom-Character 
   lcd_fix_customchar(ResIcon2);		//load Character  ]
	  
   //if kyrillish LCD-Characterset is defined, load  Omega- and µ-Character
 #if LCD_CHAR_OMEGA < 8
   LCDLoadCustomChar(LCD_CHAR_OMEGA);	//load omega as Custom-Character
   lcd_fix_customchar(CyrillicOmegaIcon);
 #endif
 #if LCD_CHAR_U < 8
   LCDLoadCustomChar(LCD_CHAR_U);	//load mu as Custom-Character
   lcd_fix_customchar(CyrillicMuIcon);
 #endif
 #if LCD_CHAR_RESIS3 != 'R'
   LCDLoadCustomChar(LCD_CHAR_RESIS3);	//load Resistor symbol as Custom-Character
   lcd_fix_customchar(ResIcon3);		// load character ||
 #endif

#endif
}	/* end lcd_init */
 
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
void lcd_powersave(void) {
     lcd_command(CMD_DISPLAY_OFF);
 #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
     lcd_command(CMD_SET_ALLPTS_ON); // Enter power save mode
 #endif
}
#endif

/* ******************************************************************************* */
// send the command to clear the display 
 
void lcd_clear(void) {
   
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
   unsigned char p;
   unsigned char count;

   for (p = 0; p < (SCREEN_HEIGHT / 8); p++) {
     lcd_command(CMD_SET_COLUMN_UPPER);		// set horizontal position to 0
     lcd_command(CMD_SET_COLUMN_LOWER);
     lcd_command(CMD_SET_PAGE | (0x0f & p));	// set page 0 to 7
     for (count = 0; count < 132; count++)
       lcd_write_data(0);
   }
#elif (LCD_ST_TYPE == 8812)
   unsigned char p;
   unsigned char count;

   for (p = 0; p < (SCREEN_HEIGHT / 8); p++) {
     lcd_command(CMD_SET_COLUMN);		// set horizontal position to 0
     lcd_command(CMD_SET_PAGE | (0x0f & p));	// set page 0 to 7
     for (count = 0; count < 102; count++)
       lcd_write_data(0);
   }
#elif (LCD_ST_TYPE == 8814)
   unsigned char p;
   unsigned char count;

   for (p = 0; p < (SCREEN_HEIGHT / 8); p++) {
     lcd_command(CMD_SET_COLUMN_UPPER);		// set horizontal position to 0
     lcd_command(CMD_SET_COLUMN_LOWER);
     lcd_command(CMD_SET_PAGE | (0x0f & p));	// set page 0 to 7
     for (count = 0; count < 96; count++)
       lcd_write_data(0);
   }
#elif (LCD_ST_TYPE == 7108)
   // clear both controllers at the same time
   unsigned char p;
   unsigned char count;

   _lcd_hw_select(3);				// select both controllers
   for (p = 0; p < (SCREEN_HEIGHT / 8); p++) {
     lcd_command(CMD_SET_COLUMN_ADDR);		// set horizontal position to 0
     lcd_command(CMD_SET_PAGE | (0x07 & p));	// set page 0 to 7, vertical position = p*8
     for (count = 0; count < 64; count++)
       lcd_write_data(0);			// clear 64 row in each controller (128 row total)
   }

#elif (LCD_ST_TYPE == 7920)
   unsigned char p;
   unsigned char count;

   for (count = 0; count < SCREEN_HEIGHT; count++) {
     for (p = 0; p < (SCREEN_WIDTH / 8); p++) {
       lcd_bit_mem[count][p] = 0;		// clear bits in image data
     }
   }
#elif (LCD_ST_TYPE == 7735)
   unsigned char p;
   unsigned char count;

   lcd_command(CMD_CASET);
    lcd_write_data(0);	// set start column
    lcd_write_data(0);	// set start column
    lcd_write_data(SCREEN_WIDTH>>8);	// set end column
    lcd_write_data(SCREEN_WIDTH && 0xff); // set end column
   lcd_command(CMD_RASET);
    lcd_write_data(0);	// set start row
    lcd_write_data(0);	// set start row
    lcd_write_data(SCREEN_HEIGHT>>8);	// set end row
    lcd_write_data(SCREEN_HEIGHT && 0xff); // set end row
   lcd_command(CMD_RAMWR);	// write data to RAM
   
   for (count = 0; count < SCREEN_HEIGHT; count++) {
     for (p = 0; p < SCREEN_WIDTH; p++) {
       lcd_write_data(0);	// clear red color
       lcd_write_data(0);	// clear green color
       lcd_write_data(0);	// clear blue color
     }
   }
#else
   lcd_command(CMD_CLEAR_DISPLAY);
   wait_about10ms();
#endif
#ifdef WITH_UART
   uart_newline();
#endif
   lcd_line1();			// set cursor to Line1 Column 1, (only for OLED-Display)
   lcd_text_line = 0;
}


#ifdef WITH_UART
void uart_newline(void) {
   uart_putc('\r');
   uart_putc('\n');
}
#endif
 
 
/* ******************************************************************************* */
// writes a string to the LCD 
 
void lcd_string(char *data) {
    while(*data) {
        lcd_data(*data);
        data++;
    }
}

#ifdef use_lcd_pgm
//Load string from PGM  and send to LCD 
void lcd_pgm_string(const unsigned char *data) {
   unsigned char cc;
   while(1) {
      cc = pgm_read_byte(data);
      if((cc==0) || (cc==128)) {
         return;
      }
      lcd_data(cc);
      data++;
   }
}
#endif

/* ******************************************************************************* */
#ifdef USE_EEPROM
//Load string from PGM or EEprom and send to LCD 
void lcd_fix_string(const unsigned char *data) {
   unsigned char cc;
   while(1) {
      cc = MEM_read_byte(data);
      if((cc==0) || (cc==128)) {
         return;
      }
      lcd_data(cc);
      data++;
   }
}
#endif

/* ******************************************************************************* */
// load custom character from PGM or EEprom and send to LCD
void lcd_fix_customchar(const unsigned char *chardata) {	
#if (LCD_ST_TYPE == 0)
    uint8_t ii;
    for (ii=0;ii<8;ii++) {
        lcd_data(MEM_read_byte(chardata++));
    }
#endif
}

/* ******************************************************************************* */
#ifdef LCD_CLEAR
void lcd_clear_line(void) {
 // writes 20 spaces to LCD-Display, Cursor must be positioned to first column
    lcd_spaces(20);
}
void lcd_clear_line1(void) {
    lcd_line1();
    lcd_clear_line();
    lcd_line1();
}
void lcd_clear_line2(void) {
    lcd_line2();
    lcd_clear_line();
    lcd_line2();
}
#endif

/* ******************************************************************************* */
#if (LCD_GRAPHIC_TYPE != 0)
unsigned char reverse_byte(unsigned char b)
{
   unsigned char result = 0;
   unsigned char bitin;
   unsigned char bitout;

   for (bitin = 1, bitout = 0x80; bitin != 0; bitin <<= 1, bitout >>= 1) 
      if (b & bitin)
         result |= bitout;

   return result;
}
#endif

/* ******************************************************************************* */
/* Function lcd_pgm_bitmap                                                         */
/* The first parameter pbitmap is a pointer to a array of bytes in the program memory */
/* the first two bytes in pbitmap specifies the size (width + height) of the bitfield */
/* the following data are written to the display to position x,y with options */
/* ******************************************************************************* */
#ifdef WITH_GRAPHICS
void lcd_pgm_bitmap(const unsigned char * pbitmap,
                    unsigned char x,
                    unsigned char y,
                    unsigned char options)
{
 uint8_t dx;	// width
 uint8_t dy;	// height
 dx = (unsigned char)pgm_read_byte(&pbitmap[0]);	// width of the bitmap
 dy = (unsigned char)pgm_read_byte(&pbitmap[1]);	// height of the bitmap
 lcd_set_pixels( (const unsigned char *)(pbitmap+2), x, y, options, dx, dy);
}
#endif

/* ******************************************************************************* */
/* Function lcd_set pixels                                                         */
/* pdata is a pointer to the byte array in the program memory holding the pixel data. */
/* x,y specifies the target position of this data at the display.                  */
/* The parameter options can specify a inverted position in x or/and y direction.  */
/* width and height specifies the dimension of the pixel field.                    */
/* The different controller types uses different ways of loading the pixels.       */
/* For the ST7920 controller the changes are only made in a memory field (RAM).    */
/* ******************************************************************************* */
void lcd_set_pixels(const unsigned char *pdata, unsigned char x, unsigned char y,
unsigned char options, unsigned char width, unsigned char height) {
/* ------------------------------------------------------------------------------- */
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 8812) || (LCD_ST_TYPE == 8814))
   if (x >= (SCREEN_WIDTH - 6) || (y >= (SCREEN_HEIGHT - 8)))
      return;

   unsigned char offset;
   unsigned char page;
   unsigned char pagemax;
   unsigned char xx;

   page = y >> 3;		// page of screen has 8 pixel lines each
   pagemax = (y + height - 1) >> 3;

   if ((options & OPT_VREVERSE) == OPT_VREVERSE)
      pdata += (pagemax - page) * width;

   if (pagemax >= (SCREEN_HEIGHT >> 3))
      pagemax = ((SCREEN_HEIGHT - 1) >> 3);	// limit to last page of screen
   for (; page <= pagemax; page++)
   { 
  #if (LCD_ST7565_H_FLIP == 1)
      xx = (SCREEN_WIDTH  - x - width) + LCD_ST7565_H_OFFSET;
  #else
      xx = x + LCD_ST7565_H_OFFSET;
  #endif
  #if (LCD_ST_TYPE == 7108)
      _lcd_hw_select(3);		// select both controllers
  #endif
  #if (LCD_ST7565_V_FLIP == 1)
      lcd_command(CMD_SET_PAGE | (((SCREEN_HEIGHT / 8) - 1) - page));
  #else
      lcd_command(CMD_SET_PAGE | page);
  #endif
  #if (LCD_ST_TYPE == 7108)
      if (xx < 64) {
        _lcd_hw_select(1);		// select controller 1
      } else {
        _lcd_hw_select(2);		// select controller 2
      }
      lcd_command(CMD_SET_COLUMN_ADDR | (0x3f & xx ));
  #elif (LCD_ST_TYPE == 8812)
      lcd_command(CMD_SET_COLUMN | (0x7f & xx));
  #else
      lcd_command(CMD_SET_COLUMN_UPPER | (0x0f & (xx >> 4)));
      lcd_command(CMD_SET_COLUMN_LOWER | (0x0f & xx));
  #endif
      for (offset = 0; offset < width; offset++)
      {
         unsigned char byte;
  #if (LCD_ST_TYPE == 7108)
         if (xx == 64) {
           // select the other controller
           _lcd_hw_select(2);		// select controller 2
           lcd_command(CMD_SET_COLUMN_ADDR | (0x3f & xx ));
         }
  #endif
  #if (LCD_ST7565_H_FLIP == 1)
         if (!((options & OPT_HREVERSE) == OPT_HREVERSE))
  #else
         if ((options & OPT_HREVERSE) == OPT_HREVERSE)
  #endif
            { byte = pgm_read_byte(pdata + width - offset - 1);
         } else {
            byte = pgm_read_byte(pdata + offset);
         }
  #if (LCD_ST_TYPE == 8812)
   #if (LCD_ST7565_V_FLIP == 1)
         if (((options & OPT_VREVERSE) == OPT_VREVERSE))
   #else
         if (!(options & OPT_VREVERSE) == OPT_VREVERSE)
   #endif
  #else
   #if (LCD_ST7565_V_FLIP == 1)
         if (!((options & OPT_VREVERSE) == OPT_VREVERSE))
   #else
         if ((options & OPT_VREVERSE) == OPT_VREVERSE)
   #endif
  #endif
            { byte = reverse_byte(byte);
         }
//         if ((options & OPT_CINVERSE) == OPT_CINVERSE) {
//            byte = ~byte;
//         }
         lcd_write_data(byte);
         xx++;
      } /* end for offset */
      if ((options & OPT_VREVERSE) == OPT_VREVERSE) {
         pdata -= width;
      } else {
         pdata += width;
      }
   } /* end for page */
   // Restore previous cursor position
  #if (LCD_ST7565_V_FLIP == 1)
   lcd_command(CMD_SET_PAGE | (((SCREEN_HEIGHT / 8) - 1) - _page));
  #else
   lcd_command(CMD_SET_PAGE | (0x0f & _page));
  #endif
/* ------------------------------------------------------------------------------- */
#elif (LCD_ST_TYPE == 7920)
  // lcd_set_pixels(const uchar *pdata, uchar x, uchar y, uchar options, uchar width, uchar height) 
   unsigned char ii,hh,hmsk;
   unsigned char jj;
   unsigned char xx,xxbit;
//   unsigned char width;
//   unsigned char height;
   unsigned char ymem;
//   const unsigned char *pdata;
   if ((x > (SCREEN_WIDTH - 6)) || (y > (SCREEN_HEIGHT-8))) return;
   for (ii=0; ii<height; ii++) {
     if (options & OPT_VREVERSE) {
       hh = (height - 1 - ii) / 8;
       hmsk = pgm_read_byte(set_msk+((height - 1 - ii) % 8));
     } else {
       hh = ii / 8;
       hmsk = pgm_read_byte(set_msk + (ii % 8));
     }
     for (jj=0; jj<width; jj++) {
       if (options & OPT_HREVERSE) {
         xx = (width - 1 + x - jj) / 8;
         xxbit = (width - 1 + x - jj) % 8;
       } else {
         xx = (x + jj) / 8;
         xxbit = (x + jj) % 8;
       }
 #if (LCD_ST7565_V_FLIP > 0)
       ymem = (SCREEN_HEIGHT - 1) - y - ii;
 #else
       ymem = y + ii;
 #endif
       if ((pgm_read_byte(pdata + jj + (hh*width)) & hmsk) == 0) {
         // clear bit in the pixel image
 #if (LCD_ST7565_H_FLIP > 0)
         lcd_bit_mem[ymem][((SCREEN_WIDTH / 8) - 1)-xx] &= pgm_read_byte(clear_msk+xxbit);
 #else
         lcd_bit_mem[ymem][xx] &= pgm_read_byte(clear_msk+(7-xxbit));
 #endif
       } else {
	// set bit in the pixel image
 #if (LCD_ST7565_H_FLIP > 0)
         lcd_bit_mem[ymem][((SCREEN_WIDTH / 8) - 1)-xx] |= pgm_read_byte(set_msk+xxbit);
 #else
         lcd_bit_mem[ymem][xx] |= pgm_read_byte(set_msk+(7-xxbit));
 #endif
       }
     } /* end for jj */
   } /* end for ii */
#elif (LCD_ST_TYPE == 7735)
/* ------------------------------------------------------------------------------- */
  // support for color display
  // lcd_set_pixels(const uchar *pdata, uchar x, uchar y, uchar options, uchar width, uchar height) 
   unsigned int hh;
   if ((x > (SCREEN_WIDTH - 6)) || (y > (SCREEN_HEIGHT-8))) return;

   unsigned char offset;
   unsigned char page;
   unsigned char pagemax;
   unsigned char xx;

   page = y >> 3;		// page of screen has 8 pixel lines each
   pagemax = (y + height - 1) >> 3;

   if ((options & OPT_VREVERSE) == OPT_VREVERSE)
      pdata += (pagemax - page) * width;

   if (pagemax >= (SCREEN_HEIGHT >> 3))
      pagemax = ((SCREEN_HEIGHT - 1) >> 3);	// limit to last page of screen
   for (; page <= pagemax; page++)
   { 
  #if (LCD_ST7565_H_FLIP == 1)
      xx = (SCREEN_WIDTH  - x - width) + LCD_ST7565_H_OFFSET;
  #else
      xx = x + LCD_ST7565_H_OFFSET;
  #endif
      lcd_command(CMD_RASET);		// set row range
  #if (LCD_ST7565_V_FLIP == 1)
      lcd_data(0);			// upper row bits allways zero
      lcd_data(((SCREEN_HEIGHT / 8) - 1) - (page << 3));
      lcd_data(0);			// upper row bits allways zero
      lcd_data(((SCREEN_HEIGHT / 8) - 1) - (page << 3) + 7);
  #else
      lcd_data(0);			// upper row bits allways zero
      lcd_data(page);
      lcd_data(0);			// upper row bits allways zero
      lcd_data(page + 7);
  #endif
      lcd_command(CMD_CASET);		// set column range
      lcd_data(0);			// upper column bits allways zero
      lcd_data(xx);
      lcd_data(0);			// upper column bits allways zero
      lcd_data(SCREEN_WIDTH - 1);
      lcd_command(CMD_RAMWR);		// write pixel data to RAM
      for (offset = 0; offset < width; offset++)
      {
         unsigned char byte;
  #if (LCD_ST7565_H_FLIP == 1)
         if (!((options & OPT_HREVERSE) == OPT_HREVERSE))
  #else
         if ((options & OPT_HREVERSE) == OPT_HREVERSE)
  #endif
            { byte = pgm_read_byte(pdata + width - offset - 1);
         } else {
            byte = pgm_read_byte(pdata + offset);
         }
  #if (LCD_ST7565_V_FLIP == 1)
         if (!((options & OPT_VREVERSE) == OPT_VREVERSE))
  #else
         if ((options & OPT_VREVERSE) == OPT_VREVERSE)
  #endif
            { byte = reverse_byte(byte);
         }
//         if ((options & OPT_CINVERSE) == OPT_CINVERSE) {
//            byte = ~byte;
//         }
         unsigned char bb;
         for (bb=0; bb<8; bb++)
         {
           if ((byte & 0x80) != 0) {
	      // set pixel to foreground color
              lcd_data(0xfc);	// red color
              lcd_data(0xfc);	// green color
              lcd_data(0xfc);	// blue color
           } else {
	      // set pixel to background color
              lcd_data(0x00);	// red color
              lcd_data(0x00);	// green color
              lcd_data(0x00);	// blue color
           }
           byte *= 2;		// next bit to 2**7
         } /* end for bb */
      } /* end for offset */
      if ((options & OPT_VREVERSE) == OPT_VREVERSE) {
         pdata -= width;
      } else {
         pdata += width;
      }
   } /* end for page */
/* ------------------------------------------------------------------------------- */
#endif /* (LCD_ST_TYPE == 7565 || 1306 || 7108 || 7920 || 8812 || 8814 */
}
/* ******************************************************************************* */
void lcd_show_Cg(void) {
       lcd_MEM_string(GateCap_str);             //"Cg="
#if FLASHEND > 0x1fff
       if (cap.cpre < -3) {
          DisplayValue(cap.cval,cap.cpre,'F',3);
       } else {
          lcd_data('?');
       }
#else
       DisplayValue(cap.cval,cap.cpre,'F',3);
#endif
}

/* ******************************************************************************* */
#if (LCD_ST_TYPE == 7920)
/* lcd_refresh writes the pixel image data to the ST7920 controller GDRAM */
/* usually the refresh is done after the display data are finished before waiting */
void lcd_refresh(void) {
  unsigned char xx;
  unsigned char yy, yyo;
  for (yy=0; yy<SCREEN_HEIGHT; yy++) {
    yyo = (yy + LCD_ST7565_Y_START) & 0x3f;	/* the y-address with offset */
    if (yyo < 32) {
      lcd_command(CMD_SET_GDRAM_ADDRESS|yyo);	/* set vertical start address */
      lcd_command(CMD_SET_GDRAM_ADDRESS|0);	/* horizontal address starts with 0 */
    } else {
      // the second half of display is located at GDRAM address 128..255 ((8..15) x 16 pixel)
      lcd_command(CMD_SET_GDRAM_ADDRESS|(yyo-32));	/* set vertical start address */
      lcd_command(CMD_SET_GDRAM_ADDRESS|(SCREEN_WIDTH / 16));	/* horizontal address starts with 0 (128)*/
    }
    for (xx=0; xx<(SCREEN_WIDTH / 8); xx++) {
      lcd_write_data(lcd_bit_mem[yy][xx]);	// write 16 x 8 bits to GDRAM (= 128 pixel)
    }
  } /* end for yy */
  wdt_reset();
}
#endif


