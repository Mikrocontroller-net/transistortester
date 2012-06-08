#include "config.h"

// Interfacing of a HD44780 compatible LCD with 4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//


//LCD
void lcd_ziff1(unsigned char temp);
void lcd_data(unsigned char temp1);
void lcd_command(unsigned char temp1);
void lcd_send(unsigned char data);
void lcd_string(char *data);
void lcd_enable(void);
void lcd_init(void);
void lcd_clear(void);
void lcd_fix_string(const unsigned char *data);
void lcd_fix_customchar(const unsigned char *chardata);
void lcd_clear_line(void);		// only for DebugOut

//Software-UART
void uart_putc(uint8_t data);
void uart_newline(void);


//LCD-commands
#define CMD_SetEntryMode         0x04
#define CMD_SetDisplayAndCursor  0x08
#define CMD_SetIFOptions         0x20
#define CMD_SetCGRAMAddress      0x40    // für Custom-Zeichen
#define CMD_SetDDRAMAddress      0x80    // zum Cursor setzen

//Makros for LCD
#define Line1() lcd_command((uint8_t)(CMD_SetDDRAMAddress))		//move to the beginning of the 1. row
#define Line2() lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x40))	//move to the beginning of the 2. row
#define Line3() lcd_command(((uint8_t)CMD_SetDDRAMAddress + 0x14))	//move to the beginning of the 3. row
#define Line4() lcd_command(((uint8_t)CMD_SetDDRAMAddress + 0x54))	//move to the beginning of the 4. row
#define lcd_aus() lcd_command(0x08)
#define lcd_ein() lcd_command(0x0c)
#define lcd_shift_right() lcd_command(0x1c)
#define lcd_shift_left() lcd_command(0x18)

#define SetCursor(y, x) lcd_command((uint8_t)(CMD_SetDDRAMAddress + (0x40*(y-1)) + x)) //move to the specified position 

#define LCDLoadCustomChar(addr) lcd_command(CMD_SetCGRAMAddress | (addr<<3))	//load Custom-character

//self build characters 
#define LCD_CHAR_DIODE1  1	//Diode-Icon; will be generated as custom character
#define LCD_CHAR_DIODE2  2	//Diode-Icon;  will be generated as custom character
#define LCD_CHAR_CAP 3		//Capacitor-Icon;  will be generated as custom character
#define LCD_CHAR_RESIS1 6       // Resistor left part will be generated as custom character
#define LCD_CHAR_RESIS2 7       // Resistor right part will be generated as custom character

#ifdef LCD_CYRILLIC
	#define LCD_CHAR_OMEGA  4	//Omega-character 
	#define LCD_CHAR_U  5		//µ-character 
#else
	#define LCD_CHAR_OMEGA  244	//Omega-character
	#define LCD_CHAR_U  228		//µ-character
#endif
  
#define LCD_CHAR_DEGREE	0xdf		// Character for degree
// LCD commands
 
#define CLEAR_DISPLAY 0x01
 
// specifies which pins are used for the LCD, adapt to your hardware
 
#define LCD_PORT      PORTD
#define LCD_DDR       DDRD
#define LCD_RS        PD4
#define LCD_EN1       PD5
