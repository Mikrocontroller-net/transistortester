#include <stdint.h>
#include "lcd_defines.h"

// Interfacing of a HD44780 compatible LCD with 4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//

extern void _lcd_hw_write(uint8_t flags, uint8_t data);
extern void _lcd_hw_select(uint8_t flags);

//LCD
void lcd_testpin(unsigned char temp);
void lcd_data(unsigned char temp1);
void lcd_big_icon(unsigned char temp1);
void lcd_update_icon(const unsigned char * ubitmap);
void lcd_update_icon_opt(const unsigned char * ubitmap, unsigned char options);
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
unsigned char reverse_byte(unsigned char);
#ifdef WITH_GRAPHICS
// Graphics display only: Some drawing functions for graphics LCD
void lcd_pgm_bitmap(const unsigned char * pbitmap,
                    unsigned char x, unsigned char y,
                    unsigned char options);
void lcd_set_pixels(const unsigned char * pdata,
                    unsigned char x, unsigned char y,
                    unsigned char options,
                    unsigned char width, unsigned char height);
void ShowIcons(void);		/* show the different Icons */
void lcd_draw_pin(unsigned char index, char dx, char dy);
void lcd_draw_trans_pins( char dxb, char dyb);
#endif
#if (LCD_ST_TYPE == 7920)
void lcd_refresh(void);		/* for ST7920 controller only*/
#endif

//Software-UART
void uart_putc(uint8_t data);
void uart_newline(void);

