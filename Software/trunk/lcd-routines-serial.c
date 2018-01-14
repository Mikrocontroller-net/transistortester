// Interfacing a HD44780 compatible LCD with  4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//
// Pin selection can be changed in lcd-routines.h with define

#include "Transistortester.h"


uint8_t rawSerial = 0;

void outString(char *s) {
	while(*s) {
		uart_putc(*s);
		s++;
	}
}

uint8_t last_text_line;
uint8_t last_text_column;

/* ******************************************************************************* */
// sends numeric character (Pin Number) to the LCD 
// from binary 0 we send ASCII 1 ....
void lcd_testpin(unsigned char temp) {
	lcd_data(temp + '1' + TP_OFFSET - TP_MIN);
}

/* ******************************************************************************* */
// send space character to LCD
void lcd_space(void) {
   lcd_data(' ');	// output one space to LCD and UART
}

void lcd_spaces(int8_t nn) {
   while (nn > 0) {
     lcd_space();	// output one space to LCD and UART
     nn--;
   }
}
// send equal character to LCD
void lcd_equal(void) {
   lcd_data('=');
}

/* ******************************************************************************* */
//move to the beginning of the 1. row
void lcd_line1() {
   lcd_text_line = 0;
   _lcd_column = 0;
   uart_newline();
}

/* ******************************************************************************* */
//move to the beginning of the 2. row
// for the ST7920 you can select a accurate vertical pixel positioning of line with:
// or you can select a 8-line rounding of the positioning of the lines with: 
//#define FONT_V_SPACE (((FONT_HEIGHT + 7) / 8) * 8)
void lcd_line2() {
   lcd_text_line = 1;
   _lcd_column = 0;
   uart_newline();
}

/* ******************************************************************************* */
//move to the beginning of the 3. row
void lcd_line3() {
   lcd_text_line = 2;
   _lcd_column = 0;
   uart_newline();
}

/* ******************************************************************************* */
//move to the beginning of the 4. row
void lcd_line4() {
   lcd_text_line = 3;
   _lcd_column = 0;
   uart_newline();
}

/* ******************************************************************************* */
// position to next line to column xx
// Text_line number is incremented by 1, if possible (not yet at the last line).
// If already at the last line of the display, last_line_used is set to 1 .
void lcd_next_line(uint8_t xx) {
   lcd_clear_line();
   lcd_text_line ++;
   if (lcd_text_line > (LCD_LINES - 1))  {
      // Limit is reached
      lcd_text_line = (LCD_LINES - 1);
      last_line_used = 1;
   } else {
      last_line_used = 0;
   }
   _lcd_column = xx;
   uart_newline();
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
	lcd_text_line = y;
    _lcd_column = x;
    if (x == 0) uart_newline();
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
 lcd_set_cursor((uint8_t)last_text_line, (uint8_t)last_text_column);
}
#endif

/* ******************************************************************************* */
/* ******************************************************************************* */
// sends a character to the LCD 
void lcd_data(unsigned char temp1) {
/* -------------------------------------------------------------------------- */

 _lcd_column++;
 switch(temp1) {
    case LCD_CHAR_DIODE1:
    	uart_putc('>');
    	uart_putc('|');
        _lcd_column++;
   	break;
    case LCD_CHAR_DIODE2:
    	uart_putc('|');
    	uart_putc('<');
        _lcd_column++;
   	break;
    case LCD_CHAR_CAP:
    	uart_putc('|');
    	uart_putc('|');
        _lcd_column++;
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
    case LCD_CHAR_U:		// µ
    	uart_putc('u');
    	break;
    case LCD_CHAR_OMEGA:	//Omega
    	uart_putc('O');
    	uart_putc('h');
    	uart_putc('m');
        _lcd_column += 2;
    	break;
 #if LCD_CHAR_INDUCTOR1 != 'w'
    case LCD_CHAR_INDUCTOR1:
    case LCD_CHAR_INDUCTOR2:
    	uart_putc('L');
        break;
 #endif
    default:
    	uart_putc(temp1);
 }
} /* end lcd_data() */

void uart_newline(void) {
     uart_putc('\r');
     uart_putc('\n');
}
 
// sends a command to the LCD
 
void lcd_command(unsigned char temp1) {
}
 
/* ******************************************************************************* */
// Initialise: 
// Must be called first .
// every controller has its own initialize sequence!
 
/* -------------------------------------------------------------------------- */
void lcd_init(void) {
       // setup UART 
 #define BAUD_SET ((F_CPU / 8 / BAUDRATE ) - 1)
 #if (((F_CPU == 16000000UL) && (BAUDRATE == 57600)) | (BAUD_SET > 4095))
  // hardcoded exception for 57600 for compatibility with the bootloader
  // shipped with the Duemilanove and previous boards and the firmware
  // on the 8U2 on the Uno and Mega 2560. Also, The baud_setting cannot
  // be > 4095, so switch back to non-u2x mode if the baud rate is too
  // low.

        UCSR0A = 0;
        UBRR0H = (uint8_t) (((F_CPU / 16 / BAUDRATE) - 1) >> 8);
        UBRR0L = (uint8_t) (((F_CPU / 16 / BAUDRATE) - 1));
 #else
        UCSR0A = (1 << U2X0);
        UBRR0H = (uint8_t) (BAUD_SET >> 8);
        UBRR0L = (uint8_t) (BAUD_SET);
 #endif
//        UCSR0C = (1<<USBS0) | (1<<UCSZ00);	// 8 bit mode, 1 Stop bit, no parity
        UCSR0C = (1<<USBS0) | (3<<UCSZ00);	// 8 bit mode, 2 Stop bit, no parity
        UCSR0B = (1<<RXEN0) | (1<<TXEN0);	// enable TX and RX data transfer

}

/* ******************************************************************************* */
// send the command to clear the display 
 
void lcd_clear(void) {
        uart_newline();
	for(uint8_t x = 0; x < LCD_LINE_LENGTH; x++) {
                uart_putc(' ');
	}
	lcd_line1();
}
 
/* ******************************************************************************* */
// writes a string to the LCD 
 
void lcd_string(char *data) {
	outString(data);
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
void lcd_clear_line(void) {
 // writes spaces up to LCD_LINE_LENGTH to LCD-Display, Cursor can be positioned to any column
    lcd_spaces(LCD_LINE_LENGTH - _lcd_column);
}

#ifdef LCD_CLEAR
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
//	serialPrintf("lcd_set_pixels not implemented\n");
}
/* ******************************************************************************* */
void lcd_show_Cg(void) {
   lcd_MEM_string(GateCap_str);             //"Cg="
   if (cap.cpre < -3) {
	  DisplayValue(cap.cval,cap.cpre,'F',3);
   } else {
	  lcd_data('?');
   }
}
#ifdef SHOW_R_DS
void lcd_show_rds(uint8_t xpos)
{
       if ((PartMode&0x0f) == PART_MODE_MOS) {     /* kein JFET or IGBT */
          lcd_next_line_wait(xpos);
          lcd_MEM_string(RDS_str);		// "RDS=
          DisplayValue16(_trans->uBE,-1,LCD_CHAR_OMEGA,2);	// Drain-Source resistance
       }
}
#endif


