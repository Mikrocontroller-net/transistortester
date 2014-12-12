// Interfacing a HD44780 compatible LCD with  4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//
// Pin selection can be changed in lcd-routines.h with define
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
//#include "tt_function.h"
//#include "config.h"
//#include "lcd-routines.h"
//#include "wait1000ms.h"
#include "Transistortester.h"

#ifdef STRIP_GRID_BOARD
 #warning "strip-grid-board layout selectet!"
#endif


 
#if (LCD_ST_TYPE == 7565)
uint8_t _page;
uint8_t _xpos;
#if FONT_HEIGHT > 8
extern const uint8_t PROGMEM font[256][FONT_WIDTH << 1];
#else
extern const uint8_t PROGMEM font[256][FONT_WIDTH];
#endif
#endif

// sends numeric character (Pin Number) to the LCD 
// from binary 0 we send ASCII 1 ....
void lcd_testpin(unsigned char temp) {
   lcd_data(temp + '1' + TP_OFFSET);
}

// send space character to LCD
void lcd_space(void) {
   lcd_data(' ');
}

//move to the beginning of the 1. row
void lcd_line1() {
#if (LCD_ST_TYPE == 7565)
   lcd_set_cursor(0,0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress));
#endif
}

//move to the beginning of the 2. row
void lcd_line2() {
#if (LCD_ST_TYPE == 7565)
   lcd_set_cursor(1,0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x40));
#endif
}

//move to the beginning of the 3. row
void lcd_line3() {
#if (LCD_ST_TYPE == 7565)
   lcd_set_cursor(2,0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x14));
#endif
}

//move to the beginning of the 4. row
void lcd_line4() {
#if (LCD_ST_TYPE == 7565)
   lcd_set_cursor(3,0);
#else
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x54));
#endif
}

void lcd_set_cursor(uint8_t y, uint8_t x) {
#if (LCD_ST_TYPE == 7565)
   //move to the specified position (depends on used font)
   _page = y;
    // The pixel memory is greater as the display window.
    // For example the SPL501 controller has 132x65 dot matrix memory
    // LCD_ST7565_H_OFFSET specifies the offset of the 128 pixel of the display window.
   _xpos = (x * FONT_WIDTH) + LCD_ST7565_H_OFFSET;
 #if FONT_HEIGHT > 8
   _page <<= 1;
 #endif
   lcd_command(CMD_SET_PAGE | (0x0f & _page));
   lcd_command(CMD_SET_COLUMN_UPPER | (0x0f & (_xpos >> 4)));
   lcd_command(CMD_SET_COLUMN_LOWER | (0x0f &  _xpos));
#else
   //move to the specified position
   lcd_command((uint8_t)(CMD_SetDDRAMAddress + (0x40*(y-1)) + x));
#endif
}

// sends a character to the LCD 
void lcd_data(unsigned char temp1) {
#if (LCD_ST_TYPE == 7565)
 uint8_t i, data;
 uint8_t *pfont;

 #if FONT_HEIGHT > 8
 lcd_command(CMD_RMW);
 pfont = (uint8_t *)font + ((FONT_WIDTH << 1) * temp1);
 for (i = 0; i < FONT_WIDTH; i++) {
   data = pgm_read_byte(pfont);
   lcd_write_data(data);
   pfont += 2;
 }
 lcd_command(CMD_RMW_CLEAR);

 lcd_command(CMD_SET_PAGE | (0x0f & (_page+1)));
 pfont = (uint8_t *)font + ((FONT_WIDTH << 1) * temp1) + 1;
 for (i = 0; i < FONT_WIDTH; i++) {
   data = pgm_read_byte(pfont);
   lcd_write_data(data);
   pfont += 2;
 }
 lcd_command(CMD_SET_PAGE | (0x0f & _page));
 #else
 pfont = (uint8_t *)font + (temp1*FONT_WIDTH);
 for (i = 0; i < FONT_WIDTH; i++) {
   data = pgm_read_byte(pfont);
   lcd_write_data(data);
   pfont++;
 }
 #endif
_xpos += FONT_WIDTH;
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
//    case LCD_CHAR_DEGREE:	// degree sign
//    	uart_putc(0xf8);	// codepage 437 or 850 has degree
//    	break;
    case LCD_CHAR_U:		//µ
//    	uart_putc(0xe6);	// codepage 437 or 850 has my
    	uart_putc('u');		// better use the ASCII u
    	break;
    case LCD_CHAR_OMEGA:	//Omega
//	uart_putc(0xea);	// only codepage 437 has Omega
        uart_putc('O');
        uart_putc('h');
        uart_putc('m');
    	break;
    default:
    	uart_putc(temp1);
 }
#endif
}
 
// sends a command to the LCD
 
void lcd_command(unsigned char temp1) {
        lcd_write_cmd(temp1);		// set RS to 0
#ifdef WITH_UART
	if((temp1 == 0x80) || (temp1 == 0xC0)) {
		uart_newline();
	}
#endif
}
 
// Initialise: 
// Must be called first .
 
#if (LCD_ST_TYPE == 7565)
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
 #endif
 #if (LCD_INTERFACE_MODE == MODE_I2C)
   i2c_init();		// init the I2C interface
 #endif
   wait_about30ms();  // Wait for 30 ms after RESET

   lcd_command(CMD_INTERNAL_RESET);		// 0xe2
   lcd_command(CMD_SET_BIAS_9);			// 0xa3
 #if (LCD_ST7565_H_FLIP > 0)
   lcd_command(CMD_SET_ADC_REVERSE);		// 0xa1
 #else
   lcd_command(CMD_SET_ADC_NORMAL);		// 0xa0
 #endif
 #if (LCD_ST7565_V_FLIP > 0)
   lcd_command(CMD_SET_COM_REVERSE);		// 0xc8
 #else
   lcd_command(CMD_SET_COM_NORMAL);		// 0xc0
 #endif

   //Booster on, Voltage regulator/follower circuit on
   lcd_command(CMD_SET_POWER_CONTROL | 7);	// 0x28 BOOSTER ON | V_REGULATOR ON | V_FOLLOWER ON
   lcd_command(CMD_SET_RESISTOR_RATIO | (7 & LCD_ST7565_RESISTOR_RATIO));	// 0x20

   lcd_command(CMD_SET_VOLUME_FIRST);		// 0x81 set  volume command
   lcd_command(eeprom_read_byte(&EE_Volume_Value) & 0x3f);	//set volume value of EEprom

   lcd_command(CMD_SET_DISP_START_LINE | 0);	// 0x40
   lcd_command(CMD_SET_ALLPTS_NORMAL);		// 0xa4
   lcd_command(CMD_SET_DISP_NORMAL);		// 0xa6 not reverse

//   lcd_command(CMD_SET_STATIC_OFF);		// 0xac , set Static indicator off
//   lcd_command(CMD_SET_STATIC_REG | 0);	// 0x00 , set Static indicator no flashing

   lcd_command(CMD_RMW);			// 0xe0 , set Read Modify Write mode
   lcd_command(CMD_DISPLAY_ON);			// 0xaf

   //Center the two lines
   //lcd_command(CMD_SET_DISP_START_LINE | (0x3f & 40));

   lcd_clear();

   lcd_set_cursor(0,0);
}
#elif (LCD_ST_TYPE == 7920)
void lcd_init(void) {
   // reset the ST7920 controller
   HW_LCD_B0_PORT  &= ~_BV(HW_LCD_B0_PIN); // LCD B0 = 0
   HW_LCD_B0_DDR   |= _BV(HW_LCD_B0_PIN);  // LCD SI is Output
   HW_LCD_EN_PORT  &= ~_BV(HW_LCD_EN_PIN); // LCD EN = 0
   HW_LCD_EN_DDR   |= _BV(HW_LCD_EN_PIN);  // LCD SCL is Output
   HW_LCD_RES_PORT &= ~_BV(HW_LCD_RES_PIN); // set reset low
   HW_LCD_RES_DDR  |= _BV(HW_LCD_RES_PIN); // LCD RESET is Output
   wait1ms();
   HW_LCD_RES_PORT |= _BV(HW_LCD_RES_PIN); // set reset high
   wait10ms();
   lcd_command(0b00110000); // 8 bit data, basic instructions
   lcd_command(0b00110000); // 8 bit data, basic instructions
   lcd_command(0b00001100); // display on
   lcd_clear();
   lcd_command(0b00000110); // increment, no shift 
}
#else 
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
 
#if (LCD_ST_TYPE == 7565)
void lcd_powersave(void) {
     lcd_command(CMD_DISPLAY_OFF);
     lcd_command(CMD_SET_ALLPTS_ON); // Enter power save mode
}
#endif

// send the command to clear the display 
 
void lcd_clear(void) {
#if (LCD_ST_TYPE == 7565)
   unsigned char p;
   unsigned char count;

   lcd_command(CMD_RMW); // Start Read-Modify-Write

   for (p = 0; p < 8; p++) {
     lcd_command(CMD_SET_PAGE | (0x0f & p));
     lcd_command(CMD_SET_COLUMN_UPPER);
     lcd_command(CMD_SET_COLUMN_LOWER);
     for (count = 0; count < 132; count++)
       lcd_write_data(0);
   }

   lcd_command(CMD_RMW_CLEAR); // Clear Read-Modify-Write ???
#else
   lcd_command(CLEAR_DISPLAY);
   wait_about10ms();
#endif
#ifdef WITH_UART
   uart_newline();
#endif
   lcd_line1();			// set cursor to Line1 Column 1, (only for OLED-Display)
}


#ifdef WITH_UART
void uart_newline(void) {
   uart_putc('\r');
   uart_putc('\n');
}
#endif
 
 
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

// load custom character from PGM or EEprom and send to LCD
void lcd_fix_customchar(const unsigned char *chardata) {	
#if (LCD_ST_TYPE == 0)
    for(uint8_t i=0;i<8;i++) {
        lcd_data(MEM_read_byte(chardata));
        chardata++;
    }
#endif
}

#ifdef LCD_CLEAR
void lcd_clear_line(void) {
 // writes 20 spaces to LCD-Display, Cursor must be positioned to first column
 unsigned char ll;
 for (ll=0;ll<20;ll++) {
    lcd_space();
 }
}
#endif

#if (LCD_ST_TYPE == 7565)
static unsigned char reverse(unsigned char b)
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

#ifdef WITH_GRAPHICS
void lcd_pgm_bitmap(const unsigned char * pbitmap,
                    unsigned char x,
                    unsigned char y,
                    unsigned char options)
{
#if (LCD_ST_TYPE == 7565)
   if (x >= SCREEN_WIDTH || (y >= SCREEN_HEIGHT))
      return;

   unsigned char offset;
   unsigned char width;
   unsigned char page;
   unsigned char pagemax;
   const unsigned char *pdata;

//   pdata = (const unsigned char *)pgm_read_word(&pbitmap->data);
   pdata = pbitmap + 2;
//   width = pgm_read_byte(&pbitmap->width);
   width = (unsigned char)pgm_read_byte(&pbitmap[0]);
   page = y >> 3;
//   pagemax = (y + pgm_read_byte(&pbitmap->height) - 1) >> 3;
   pagemax = (y + (unsigned char)pgm_read_byte(&pbitmap[1]) - 1) >> 3;
   if (options & OPT_VREVERSE)
      pdata += (pagemax - page) * width;
   if (pagemax >= (SCREEN_HEIGHT >> 3))
      pagemax = ((SCREEN_HEIGHT - 1) >> 3);
   for (; page <= pagemax; page++)
   { 
      lcd_command(CMD_SET_PAGE | page);
      lcd_command(CMD_SET_COLUMN_UPPER | (0x0f & (x >> 4)));
      lcd_command(CMD_SET_COLUMN_LOWER | (0x0f & x));
      for (offset = 0; offset < width; offset++)
      {
         unsigned char byte;

         if (options & OPT_HREVERSE)
            byte = pgm_read_byte(pdata + width - offset - 1);
         else
            byte = pgm_read_byte(pdata + offset);
         if (options & OPT_VREVERSE)
            byte = reverse(byte);
         lcd_write_data(byte);
      }
      if (options & OPT_VREVERSE)
         pdata -= width;
      else
         pdata += width;
   }
   // Restore previous cursor position
   lcd_command(CMD_SET_PAGE | (0x0f & _page));
   lcd_command(CMD_SET_COLUMN_UPPER | (0x0f & (_xpos >> 4)));
   lcd_command(CMD_SET_COLUMN_LOWER | (0x0f &  _xpos));
#endif
}
#endif

