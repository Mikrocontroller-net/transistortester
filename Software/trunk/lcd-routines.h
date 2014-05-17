#include "lcd_defines.h"
#include "bitmaps/tbitmap.h"
#include <stdint.h>

// Interfacing of a HD44780 compatible LCD with 4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//

extern void _lcd_hw_write(uint8_t flags, uint8_t data);

//LCD
void lcd_testpin(unsigned char temp);
void lcd_data(unsigned char temp1);
void lcd_space(void);
void lcd_command(unsigned char temp1);
void lcd_string(char *data);
void lcd_init(void);
void lcd_clear(void);
void lcd_fix_string(const unsigned char *data);
void lcd_pgm_string(const unsigned char *data);
void lcd_fix_customchar(const unsigned char *chardata);
void lcd_clear_line(void);		// only for DebugOut
void lcd_set_cursor(unsigned char y, unsigned char x);
void lcd_powersave(void);
void lcd_line1(void);
void lcd_line2(void);
void lcd_line3(void);
void lcd_line4(void);
#ifdef WITH_GRAPHICS
void lcd_pgm_bitmap(const struct tbitmap * const pbitmap,
                    unsigned char x,
                    unsigned char y,
                    unsigned char options);
#endif

//Software-UART
void uart_putc(uint8_t data);
void uart_newline(void);

