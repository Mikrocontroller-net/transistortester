#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
;---------------------------------------------------------------------

#include <avr/io.h>
#include "config.h"

       .section .text

;----------------------------------------------------------------------
; Global Definitions
;----------------------------------------------------------------------

#define preg_1           r24
#define preg_2           r22

; Serial Clock Input (SCL)
#define set_en_low       cbi       _SFR_IO_ADDR(HW_LCD_EN_PORT), HW_LCD_EN_PIN
#define set_en_high      sbi       _SFR_IO_ADDR(HW_LCD_EN_PORT), HW_LCD_EN_PIN

; Register select (0 = Command, 1 = Data)
#define set_rs_low       cbi       _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_high      sbi       _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN

; Serial data input (SI)
#define set_b0_low       cbi       _SFR_IO_ADDR(HW_LCD_B0_PORT), HW_LCD_B0_PIN
#define set_b0_high      sbi       _SFR_IO_ADDR(HW_LCD_B0_PORT), HW_LCD_B0_PIN

;----------------------------------------------------------------------
;
; "_lcd_hw_write"
;
;      preg_1 (r24) = flags
;      preg_2 (r22) = data
;
; Write one byte (Cmd or Data) to ST7565 controller. In assembler for
; Performance reasons.
;----------------------------------------------------------------------
       .global _lcd_hw_write
       .func _lcd_hw_write
       .extern wait1us

_lcd_hw_write:
               ; Set RS (0=Cmd, 1=Data)
               sbrc    preg_1, 0
                       set_rs_high
               sbrs    preg_1, 0
                       set_rs_low

_bit_loop:
               ; Send bit-7
               set_en_low
               sbrc    preg_2, 7
                       set_b0_high
               sbrs    preg_2, 7
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-6
               set_en_low
               sbrc    preg_2, 6
                       set_b0_high
               sbrs    preg_2, 6
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-5
               set_en_low
               sbrc    preg_2, 5
                       set_b0_high
               sbrs    preg_2, 5
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-4
               set_en_low
               sbrc    preg_2, 4
                       set_b0_high
               sbrs    preg_2, 4
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-3
               set_en_low
               sbrc    preg_2, 3
                       set_b0_high
               sbrs    preg_2, 3
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-2
               set_en_low
               sbrc    preg_2, 2
                       set_b0_high
               sbrs    preg_2, 2
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-1
               set_en_low
               sbrc    preg_2, 1
                       set_b0_high
               sbrs    preg_2, 1
                       set_b0_low
               set_en_high		; force data read from LCD controller

               ; Send bit-0
               set_en_low
               sbrc    preg_2, 0
                       set_b0_high
               sbrs    preg_2, 0
                       set_b0_low
               set_en_high		; force data read from LCD controller

_lcd_hw_write_exit:
               ret
       .endfunc

