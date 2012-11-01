;----------------------------------------------------------------------

#include <avr/io.h>
#include "config.h"

       .section .text

;----------------------------------------------------------------------
; Global Definitions
;----------------------------------------------------------------------

#define        preg_1                  r24
#define        preg_2                  r22

#define set_en_low             cbi             _SFR_IO_ADDR(HW_LCD_EN_PORT), HW_LCD_EN_PIN
#define set_en_high            sbi             _SFR_IO_ADDR(HW_LCD_EN_PORT), HW_LCD_EN_PIN
#define set_en_output          sbi             (_SFR_IO_ADDR(HW_LCD_EN_PORT) - 1), HW_LCD_EN_PIN

#define set_rs_low             cbi             _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_high            sbi             _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_output          sbi             (_SFR_IO_ADDR(HW_LCD_RS_PORT) - 1), HW_LCD_RS_PIN

#define set_b4_low             cbi             _SFR_IO_ADDR(HW_LCD_B4_PORT), HW_LCD_B4_PIN
#define set_b4_high            sbi             _SFR_IO_ADDR(HW_LCD_B4_PORT), HW_LCD_B4_PIN
#define set_b4_output          sbi             (_SFR_IO_ADDR(HW_LCD_B4_PORT) - 1), HW_LCD_B4_PIN

#define set_b5_low             cbi             _SFR_IO_ADDR(HW_LCD_B5_PORT), HW_LCD_B5_PIN
#define set_b5_high            sbi             _SFR_IO_ADDR(HW_LCD_B5_PORT), HW_LCD_B5_PIN
#define set_b5_output          sbi             (_SFR_IO_ADDR(HW_LCD_B5_PORT) - 1), HW_LCD_B5_PIN

#define set_b6_low             cbi             _SFR_IO_ADDR(HW_LCD_B6_PORT), HW_LCD_B6_PIN
#define set_b6_high            sbi             _SFR_IO_ADDR(HW_LCD_B6_PORT), HW_LCD_B6_PIN
#define set_b6_output          sbi             (_SFR_IO_ADDR(HW_LCD_B6_PORT) - 1), HW_LCD_B6_PIN

#define set_b7_low             cbi             _SFR_IO_ADDR(HW_LCD_B7_PORT), HW_LCD_B7_PIN
#define set_b7_high            sbi             _SFR_IO_ADDR(HW_LCD_B7_PORT), HW_LCD_B7_PIN
#define set_b7_output          sbi             (_SFR_IO_ADDR(HW_LCD_B7_PORT) - 1), HW_LCD_B7_PIN

;----------------------------------------------------------------------
;
; "_lcd_hw_init"
;
;----------------------------------------------------------------------
       .global _lcd_hw_init
       .func _lcd_hw_init
_lcd_hw_init:

               set_b4_output
               set_b5_output
               set_b6_output
               set_b7_output

               set_rs_output
               set_en_output

               set_rs_low
               set_en_low
                                                
               ret
       .endfunc

;----------------------------------------------------------------------
;
; "_lcd_hw_write"
;
;      preg_1 (r24) = flags
;      preg_2 (r22) = data
;
;----------------------------------------------------------------------
       .global _lcd_hw_write
       .func _lcd_hw_write
_lcd_hw_write:
               
               ; Set RS (0=Cmd, 1=Char)
               set_rs_low
               sbrc    preg_1, 0
                       set_rs_high

               ; Send high nibble
               set_en_high
               
               set_b4_low
               set_b5_low
               set_b6_low
               set_b7_low

               sbrc    preg_2, 4
                       set_b4_high
               sbrc    preg_2, 5
                       set_b5_high
               sbrc    preg_2, 6
                       set_b6_high
               sbrc    preg_2, 7
                       set_b7_high

               set_en_low
               ACALL    wait1us

               ; skip sending low nibble for init commands
               sbrc    preg_1, 7
                       rjmp _lcd_hw_write_exit

               ; Send low nibble
               set_en_high
               
               set_b4_low
               set_b5_low
               set_b6_low
               set_b7_low
               
               sbrc    preg_2, 0
                       set_b4_high
               sbrc    preg_2, 1
                       set_b5_high
               sbrc    preg_2, 2
                       set_b6_high
               sbrc    preg_2, 3
                       set_b7_high

               set_en_low
               ACALL    wait1us

_lcd_hw_write_exit:
               ret
       .endfunc

