// Interfacing a HD44780 compatible LCD with  4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//
// Pin selection can be changed in lcd-routines.h with define
 
#include <avr/io.h>
#include "config.h"
#include "lcd-routines.h"
#include "wait1000ms.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

 

// sends numeric character (Pin Number) to the LCD 
// from binary 0 we send ASCII 1 ....
void lcd_testpin(unsigned char temp) {
   lcd_data(temp + '1');
}

// send space character to LCD
void lcd_space(void) {
   lcd_data(' ');
}

// sends data byte to the LCD 
void lcd_data(unsigned char temp1) {
 LCD_PORT |= (1<<LCD_RS);        // set RS to 1 
 lcd_send(temp1);
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
    case 225:	//ä
    	uart_putc('ä');
    	break;
    case 228:	//µ
    	uart_putc('µ');
    	break;
    case 244:	//Omega
    	break;
    default:
    	uart_putc(temp1);
 }
#endif
}
 
// sends a command to the LCD
 
void lcd_command(unsigned char temp1) {
	LCD_PORT &= ~(1<<LCD_RS);        // set RS to 0 
	lcd_send(temp1);
#ifdef WITH_UART
	if((temp1 == 0x80) || (temp1 == 0xC0)) {
		uart_newline();
	}
#endif
}

//output to  LCD; 4-Bit-Modus
void lcd_send(unsigned char data) {
   // set upper Nibble
  LCD_PORT = (LCD_PORT & 0xF0) | ((data >> 4) & 0x0F);
#if F_CPU < 2000000
  _delay_us(5);
#else
  wait5us();
#endif
  lcd_enable();
   // set lower Nibble 
  LCD_PORT = (LCD_PORT & 0xF0) | (data & 0x0F);
#if F_CPU < 2000000
  _delay_us(5);
#else
  wait5us();
#endif
  lcd_enable();
  wait50us();
  LCD_PORT &= 0xF0;
}

// genertes the Enable-Puls
void lcd_enable(void) {
   LCD_PORT |= (1<<LCD_EN1);
   wait10us();			// short break 
   // if problems occure, set the wait time to the value of datasheet of your display
   // http://www.mikrocontroller.net/topic/80900
   LCD_PORT &= ~(1<<LCD_EN1);
}
 
// Initialise: 
// Mustt be called first .
 
void lcd_init(void) {
   LCD_DDR = LCD_DDR | 0x0F | (1<<LCD_RS) | (1<<LCD_EN1);   // switch Port to output
   // to initialise, send 3 times
          wait30ms();
   LCD_PORT = (LCD_PORT & 0xF0 & ~(1<<LCD_RS)) | 0x03;
   lcd_enable();

   wait5ms();
   lcd_enable();

   wait1ms();
   lcd_enable();
   wait1ms();
   LCD_PORT = (LCD_PORT & 0xF0 & ~(1<<LCD_RS)) | 0x02;
   wait1ms();
   lcd_enable();
   wait1ms();

   // 4Bit / 2 rows / 5x7
   lcd_command(CMD_SetIFOptions | 0x08);

   // Display on / Cursor off / no Blinking
   lcd_command(CMD_SetDisplayAndCursor | 0x04);

   // increment / no Scroll    
   lcd_command(CMD_SetEntryMode | 0x02);	
   lcd_clear();
}
 
// send the command to clear the display 
 
void lcd_clear(void) {
   lcd_command(CLEAR_DISPLAY);
   wait5ms();
#ifdef WITH_UART
   uart_newline();
#endif
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

#ifdef AUTO_CAL
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

// load custom character from PGM or EEprom and send to LCD
void lcd_fix_customchar(const unsigned char *chardata) {	
    for(uint8_t i=0;i<8;i++) {
        lcd_data(MEM_read_byte(chardata));
        chardata++;
    }
}
