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

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7920) || (LCD_ST_TYPE == 7108))
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
void lcd_spaces(uint8_t nn) {
   while (nn != 0) {
     lcd_space();
     nn--;
   }
}

/* ******************************************************************************* */
//move to the beginning of the 1. row
void lcd_line1() {
   lcd_text_line = 0;
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
   lcd_set_cursor(0 * ((FONT_HEIGHT + 7) / 8),0);
#elif (LCD_ST_TYPE == 7920)
   _page = 0;			// _page is the vertical pixel address for ST7920 controller
   _xpos = 0;			// LCD_ST7565_H_OFFSET is not used for ST7920 controller
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress));
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
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
   lcd_set_cursor(1 * ((FONT_HEIGHT + 7) / 8),0);

#elif (LCD_ST_TYPE == 7920)
   _page = FONT_V_SPACE * 1;	// _page is the vertical pixel address for ST7920 controller
   _xpos = 0;			// LCD_ST7565_H_OFFSET is not used for ST7920 controller
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x40));
#endif
}

/* ******************************************************************************* */
//move to the beginning of the 3. row
void lcd_line3() {
   lcd_text_line = 2;
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
   lcd_set_cursor(2 * ((FONT_HEIGHT + 7) / 8),0);
#elif (LCD_ST_TYPE == 7920)
   _page = FONT_V_SPACE * 2;	// _page is the vertical pixel address for ST7920 controller
   _xpos = 0;			// LCD_ST7565_H_OFFSET is not used for ST7920 controller
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x14));
#endif
}

/* ******************************************************************************* */
//move to the beginning of the 4. row
void lcd_line4() {
   lcd_text_line = 3;
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
   lcd_set_cursor(3 * ((FONT_HEIGHT + 7) / 8),0);
#elif (LCD_ST_TYPE == 7920)
   _page = FONT_V_SPACE * 3;	// _page is the vertical pixel address for ST7920 controller
   _xpos = 0;			// LCD_ST7565_H_OFFSET is not used for ST7920 controller
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x54));
#endif
}

/* ******************************************************************************* */
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
/* ************************************************************************************** */
/* Set the character position to x,y , where x specifies the character number in a text line. */
/* The y position is the page address (8 line units).                                     */
/* For most controllers the y position must be increased by (Height + 7) / 8 for the next text line */
void lcd_set_cursor(uint8_t y, uint8_t x) {
 _lcd_column = x;
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
//  unsigned char xx;
   //move to the specified position (depends on used font)
   _page = y;
    // The pixel memory is greater as the display window.
    // For example the SPL501 controller has 132x65 dot matrix memory
    // LCD_ST7565_H_OFFSET specifies the offset of the 128 pixel of the display window.
   _xpos = (x * FONT_WIDTH);
   
#elif (LCD_ST_TYPE == 7920)
   _page *=  8; 		// _page is the vertical pixel address for ST7920 controller
#else
   // move to the specified position for character display
   if ((y & 0x01) != 0) {
     lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x40 + (20*(y/2)) + x));
   } else {
     lcd_command((uint8_t)(CMD_SetDDRAMAddress + (20*(y/2)) + x));
   }
#endif
}

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

/* ******************************************************************************* */
// send a 24x32 icon to one quarter of the screen
// the number temp1 is the icon-number  from 0 to 15
// with offset  0 the icon is written to the right lower quarter of the screen
// with offset 16, 0x10 the icon is written to the left lower quarter of the screen
// with offset 32, 0x20 the icon is written to the right upper quarter of the screen
// with offset 48, 0x30 the icon is written to the left upper quarter of the screen
// to the left of the quarter are at least 8 pixels free
// to the right of the quarter are 16 pixels free  (8+24+16 < (SCREEN_WIDTH/2)
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 7920))
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

#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108) || (LCD_ST_TYPE == 7920))
 uint8_t *pfont;
 pfont = (uint8_t *)font + (FONT_WIDTH * NR_BYTE * temp1);	// first byte of character data
#if (LCD_ST_TYPE == 7920)
 // for ST7920 the _page position is a pixel line.
 lcd_set_pixels( pfont, _xpos, _page, 0, (unsigned char)FONT_WIDTH, (unsigned char)(NR_BYTE*8));
#else
 // for other controllers like ST7565 the _page specifies the page of the controller (8 lines)
 lcd_set_pixels( pfont, _xpos, _page*8, 0, (unsigned char)FONT_WIDTH, (unsigned char)(NR_BYTE*8));
#endif
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
        lcd_write_cmd(temp1);		// set RS to 0
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
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306))
void lcd_init(void) {

 #if (LCD_INTERFACE_MODE == MODE_SPI)
   HW_LCD_RES_PORT &= ~_BV(HW_LCD_RES_PIN); // RESET LCD-Display
   HW_LCD_EN_PORT  &= ~_BV(HW_LCD_EN_PIN);
   HW_LCD_RS_PORT  &= ~_BV(HW_LCD_RS_PIN);
   HW_LCD_B0_PORT  &= ~_BV(HW_LCD_B0_PIN);

   HW_LCD_RES_DDR  |= _BV(HW_LCD_RES_PIN); // LCD RESET is Output
   HW_LCD_EN_DDR   |= _BV(HW_LCD_EN_PIN);  // LCD SCL is Output
   HW_LCD_RS_DDR   |= _BV(HW_LCD_RS_PIN);  // LCD A0 is Output
   HW_LCD_B0_DDR   |= _BV(HW_LCD_B0_PIN);  // LCD SI is Output

   wait_about100ms(); // Set LCD for 100 ms into RESET
   HW_LCD_RES_PORT |= _BV(HW_LCD_RES_PIN);
   wait_about30ms();  // Wait for 30 ms after RESET
 #endif
 #if (LCD_INTERFACE_MODE == MODE_I2C)
   i2c_init();		// init the I2C interface
   wait_about30ms();	// Set LCD for 100 ms into RESET
 #endif

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

   //Center the two lines
   //lcd_command(CMD_SET_DISP_START_LINE | (0x3f & 40));
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
   lcd_command(CMD_SET_DISP_START_LINE | 0);	// 0x40
   lcd_command(CMD_SET_ALLPTS_NORMAL);		// 0xa4
   lcd_command(CMD_SET_DISP_NORMAL);		// 0xa6 not reverse

   lcd_clear();

   lcd_command(CMD_DISPLAY_ON);			// 0xaf

   lcd_set_cursor(0,0);
}
/* -------------------------------------------------------------------------- */
#elif (LCD_ST_TYPE == 7920)  /* !(LCD_ST_TYPE == 7565 | 1306) */

void lcd_init(void) {
   // reset the ST7920 controller
 #if (LCD_INTERFACE_MODE == MODE_7920_SERIAL)
   HW_LCD_B0_PORT  &= ~_BV(HW_LCD_B0_PIN); // LCD B0 = 0
   HW_LCD_B0_DDR   |= _BV(HW_LCD_B0_PIN);  // LCD SI is Output
   HW_LCD_EN_PORT  &= ~_BV(HW_LCD_EN_PIN); // LCD EN = 0
   HW_LCD_EN_DDR   |= _BV(HW_LCD_EN_PIN);  // LCD SCL is Output
 #endif
   wait50ms();		// wait after power up reset
 #if (LCD_INTERFACE_MODE == MODE_7920_SERIAL)
//   HW_LCD_RESET_PORT &= ~_BV(HW_LCD_RESET_PIN); // set reset low
//   HW_LCD_RESET_DDR  |= _BV(HW_LCD_RESET_PIN); // LCD RESET is Output
//   wait1ms();
//   HW_LCD_RESET_PORT |= _BV(HW_LCD_RESET_PIN); // set reset high
 #endif

 #if (LCD_INTERFACE_MODE == MODE_PARALLEL)
   lcd_command(0x22); // set to 4-bit mode
   lcd_command(0x22 ); // set to 4-bit mode
 #endif
   lcd_command(CMD_CLEAR);			// clear display
   wait10ms();
   lcd_command(CMD_DISPLAY_CONTROL | DISPLAY_ON); // display on
   lcd_command(CMD_SET_ENTRY_MODE | MODE_RIGHT_MOVE); // increase address counter after read/write
   lcd_command(CMD_SET_FUNCTION | MODE_EXTENDED); // set to extended mode
   lcd_clear();
   lcd_command(CMD_SET_FUNCTION | MODE_EXTENDED | GRAPHIC_DISPLAY_ON);
   lcd_refresh();
   lcd_line1();
}
/* -------------------------------------------------------------------------- */
#elif (LCD_ST_TYPE == 7108)  /* !(LCD_ST_TYPE == 7565 | 1306 | 7920) */
void lcd_init(void) {
   wait_about50ms();
   _lcd_hw_select(3);		// select both controllers
   lcd_command(CMD_SET_START_LINE);	// set the start line 0
   lcd_clear();
   lcd_command(CMD_DISPLAY_ON);
   wait_about50ms();
}
/* -------------------------------------------------------------------------- */
#else    /* !(LCD_ST_TYPE == 7565 | 1306) | 7108 | 7920) */
/* must be a character display */
void lcd_init(void) {
   wait_about30ms();
   // to initialise, send 3 times to be shure to be in 8 Bit mode
   lcd_write_init(1);
   wait_about5ms();
   
   lcd_write_init(1);
   wait1ms();

   lcd_write_init(1);
   wait1ms();

   lcd_write_init(0);		// switch to 4 Bit mode
   wait_about10ms();
 #ifdef LCD_DOGM
   lcd_command(CMD_SetIFOptions | 0x09);	// 4Bit / 2 rows / 5x7 / Instr. table 1
   lcd_command(CMD1_SetBias | 0x0c);		// 1/4 bias     (5V)
   lcd_command(CMD1_PowerControl | 0x02);	// booster off / set contrast C5:C4 = 2
   lcd_command(CMD1_FollowerControl | 0x09);	// Follower on / Rab2:0 = 1
   lcd_command(CMD1_SetContrast | 0x04);	// set contrast C3:0 = 4
   // old initialize without OLED display
   lcd_command(CMD_SetIFOptions | 0x08);	// 4Bit / 2 rows / 5x7

   lcd_command(CMD_SetDisplayAndCursor | 0x04); // Display on / Cursor off / no Blinking

   lcd_command(CMD_SetEntryMode | 0x02);	// increment / no Scroll    
   lcd_clear();
 #else
   // initialize sequence with OLED display
   lcd_command(CMD_SetIFOptions);		// Add for OLED
   lcd_command(CMD_SetIFOptions);		// Add for OLED

   lcd_command(CMD_SetIFOptions| 0x0A);		// 4Bit / 2 rows / 5x7 / table3 / Add for OLED
   lcd_command(CMD_SetDisplayAndCursor);	// Display off / no Blinking  / Add for OLED
   lcd_clear();
   lcd_command(CMD_SetEntryMode | 0x02);	// increment / no Scroll
   lcd_command(2);				// Home Command  // Add for OLED
   wait_about5ms();
   lcd_command(CMD_SetDisplayAndCursor | 0x04);	// Display on / Cursor off / no Blinking
 #endif
}
#endif
 
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
#else
   lcd_command(CLEAR_DISPLAY);
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
#endif

/* ******************************************************************************* */
#if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
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
 #if ((LCD_ST_TYPE == 7565) || (LCD_ST_TYPE == 1306) || (LCD_ST_TYPE == 7108))
   if (x >= SCREEN_WIDTH || (y >= SCREEN_HEIGHT))
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
   unsigned char ii,hh,hmsk;
   unsigned char jj;
   unsigned char xx,xxbit;
//   unsigned char width;
//   unsigned char height;
   unsigned char ymem;
//   const unsigned char *pdata;
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
 #endif
}
#endif
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
  unsigned char yy;
  for (yy=0; yy<SCREEN_HEIGHT; yy++) {
    if (yy < 32) {
      lcd_command(CMD_SET_GDRAM_ADDRESS|yy);	/* set vertical start address */
      lcd_command(CMD_SET_GDRAM_ADDRESS|0);	/* horizontal address starts with 0 */
    } else {
      // the second half of display is located at GDRAM address 128..255 ((8..15) x 16 pixel)
      lcd_command(CMD_SET_GDRAM_ADDRESS|(yy-32));	/* set vertical start address */
      lcd_command(CMD_SET_GDRAM_ADDRESS|(SCREEN_WIDTH / 16));	/* horizontal address starts with 0 (128)*/
    }
    for (xx=0; xx<(SCREEN_WIDTH / 8); xx++) {
      lcd_write_data(lcd_bit_mem[yy][xx]);	// write 16 x 8 bits to GDRAM (= 128 pixel)
    }
  } /* end for yy */
  wdt_reset();
}
#endif


