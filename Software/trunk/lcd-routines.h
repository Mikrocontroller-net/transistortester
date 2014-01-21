#include "config.h"
#include <stdint.h>

// Interfacing of a HD44780 compatible LCD with 4-Bit-Interface mode
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
//

extern void _lcd_hw_write(uint8_t flags, uint8_t data);

#define lcd_write_cmd(cmd)                     _lcd_hw_write(0x00, cmd); wait50us();
#define lcd_write_data(data)                   _lcd_hw_write(0x01, data); wait50us();
#define lcd_write_init(data_length)            _lcd_hw_write(0x80, CMD_SetIFOptions | (data_length << 4))


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

//Software-UART
void uart_putc(uint8_t data);
void uart_newline(void);


//LCD-commands
#define CMD_SetEntryMode         0x04
#define CMD_SetDisplayAndCursor  0x08
#define CMD_SetIFOptions         0x20
#define CMD_SetCGRAMAddress      0x40    // for Custom character
#define CMD_SetDDRAMAddress      0x80    // set Cursor 
#define CMD1_SetBias             0x10	// set Bias (instruction table 1, DOGM)
#define CMD1_PowerControl        0x50	// Power Control, set Contrast C5:C4 (instruction table 1, DOGM)
#define CMD1_FollowerControl     0x60	// Follower Control, amplified ratio (instruction table 1, DOGM)
#define CMD1_SetContrast         0x70	// set Contrast C3:C0 (instruction table 1, DOGM)

//Makros for LCD
#define lcd_line1() lcd_command((uint8_t)(CMD_SetDDRAMAddress))		//move to the beginning of the 1. row
#define lcd_line2() lcd_command((uint8_t)(CMD_SetDDRAMAddress + 0x40))	//move to the beginning of the 2. row
#define lcd_line3() lcd_command(((uint8_t)CMD_SetDDRAMAddress + 0x14))	//move to the beginning of the 3. row
#define lcd_line4() lcd_command(((uint8_t)CMD_SetDDRAMAddress + 0x54))	//move to the beginning of the 4. row
#define lcd_aus() lcd_command(0x08)
#define lcd_ein() lcd_command(0x0c)
#define lcd_shift_right() lcd_command(0x1c)
#define lcd_shift_left() lcd_command(0x18)

#define lcdSetCursor(y, x) lcd_command((uint8_t)(CMD_SetDDRAMAddress + (0x40*(y-1)) + x)) //move to the specified position 

#define LCDLoadCustomChar(addr) lcd_command(CMD_SetCGRAMAddress | (addr<<3))	//load Custom-character

// LCD commands
 
#define CLEAR_DISPLAY 0x01

#define Cyr_B 0xa0
#define Cyr_b 0xb2
#define Cyr_v 0xb3
#define Cyr_G 0xa1
#define Cyr_g 0xb4
#define Cyr_D 0xe0
#define Cyr_d 0xe3
#define Cyr_Jo 0xa2
#define Cyr_jo 0xb5
#define Cyr_Zsch 0xa3
#define Cyr_zsch 0xb6
#define Cyr_Z 0xa4
#define Cyr_z 0xb7
#define Cyr_I 0xa5
#define Cyr_i 0xb8
#define Cyr_J 0xa6
#define Cyr_j 0xb9
#define Cyr_k 0xba
#define Cyr_L 0xa7
#define Cyr_l 0xbb
#define Cyr_m 0xbc
#define Cyr_n 0xbd
#define Cyr_P 0xa8
#define Cyr_p 0xbe
#define Cyr_U 0xa9
#define Cyr_t 0xbf
#define Cyr_F 0xaa
#define Cyr_f 0xe4
#define Cyr_C 0xe1
#define Cyr_c 0xe5
#define Cyr_Tsch 0xab
#define Cyr_tsch 0xc0
#define Cyr_Sch 0xac
#define Cyr_sch 0xc1
#define Cyr_Schtsch 0xe2
#define Cyr_schtsch 0xe6
#define Cyr_HH 0xad
#define Cyr_hh 0xc2
#define Cyr_Y 0xae
#define Cyr_y 0xc3
#define Cyr_ww 0xc4
#define Cyr_E 0xaf
#define Cyr_e 0xc5
#define Cyr_Ju 0xb0
#define Cyr_ju 0xc6
#define Cyr_Ja 0xb1
#define Cyr_ja 0xc7
