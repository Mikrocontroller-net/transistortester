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

#define        preg_1                  r24
#define        preg_2                  r22

; for ST7565 controller: Serial Clock Input (SCL)
#define set_en_low             cbi             _SFR_IO_ADDR(HW_LCD_EN_PORT), HW_LCD_EN_PIN
#define set_en_high            sbi             _SFR_IO_ADDR(HW_LCD_EN_PORT), HW_LCD_EN_PIN
#define set_en_output          sbi             (_SFR_IO_ADDR(HW_LCD_EN_PORT) - 1), HW_LCD_EN_PIN

; Register select (0  = Command, 1 = Data)
#define set_rs_low             cbi             _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_high            sbi             _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_output          sbi             (_SFR_IO_ADDR(HW_LCD_RS_PORT) - 1), HW_LCD_RS_PIN

; for ST7565 controller: Serial data input (SI)
#define set_b0_low       cbi       _SFR_IO_ADDR(HW_LCD_B0_PORT), HW_LCD_B0_PIN
#define set_b0_high      sbi       _SFR_IO_ADDR(HW_LCD_B0_PORT), HW_LCD_B0_PIN

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


#define RCALL rcall

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
       .extern wait1us

_lcd_hw_write:
               

#if (LCD_ST_TYPE == 7565)
; serial output for ST7565 controller
               ; Set RS (0=Cmd, 1=Char)
               sbrc    preg_1, 0
                       set_rs_high
               sbrs    preg_1, 0
                       set_rs_low
               set_rs_output;		//init hardware
               nop	;		//wait for address setup
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
               set_en_high              ; force data read from LCD controller

               ; Send bit-0
               set_en_low
               sbrc    preg_2, 0
                       set_b0_high
               sbrs    preg_2, 0
                       set_b0_low
               set_en_high              ; force data read from LCD controller
	ret

#elif (LCD_ST_TYPE == 7920)
; 1-bit interface for ST7920 controller
	set_b0_high
	set_b0_output		; enable output mode
	set_en_low
	set_en_output		; enable output mode
	set_en_high              ;1 force data read from LCD controller
	set_en_low
        RCALL	four_bits	; output four times 1  
	sbrc    preg_1, 0
	set_b0_high		; data mode
	sbrs    preg_1, 0
	set_b0_low		; instruction mode
	set_en_low
	set_en_high              ;11111x force data read from LCD controller
	set_b0_low
	set_en_low
	set_en_high              ;11111x0 force data read from LCD controller
	set_en_low
	set_en_high              ;11111x00 force data read from LCD controller
				; first 8 bit transfer finished
        sbrc    preg_2, 7
	 set_b0_high		; bit 7 == 1
	set_en_low
	set_en_high              ;7 force data read from LCD controller
	set_b0_low
        sbrc    preg_2, 6
	 set_b0_high		; bit 6 == 1
	set_en_low
	set_en_high              ;76 force data read from LCD controller
	set_b0_low
        sbrc    preg_2, 5
	 set_b0_high		; bit 5 == 1
	set_en_low
	set_en_high              ;765 force data read from LCD controller
	set_b0_low
        sbrc    preg_2, 4
	 set_b0_high		; bit 4 == 1
	set_en_low
	set_en_high              ;7654 force data read from LCD controller
	set_en_low
	set_b0_low
	RCALL	four_bits	; output 4 times 0
				; the upper 4-bit are followed by 4 x 0
	set_b0_low
        sbrc    preg_2, 3
	 set_b0_high		; bit 3 == 1
	set_en_low
	set_en_high              ;3 force data read from LCD controller
	set_b0_low
        sbrc    preg_2, 2
	 set_b0_high		; bit 2 == 1
	set_en_low
	set_en_high              ;32 force data read from LCD controller
	set_b0_low
        sbrc    preg_2, 1
	 set_b0_high		; bit 1 == 1
	set_en_low
	set_en_high              ;321 force data read from LCD controller
	set_b0_low
        sbrc    preg_2, 0
	 set_b0_high		; bit 0 == 1
	set_en_low
	set_en_high              ;3210 force data read from LCD controller
	set_b0_low
	RCALL	four_bits	; output 4 times 0
				; the lower 4-bit are followed by 4 x 0
	RCALL	wait50us
	RCALL	wait30us	; at least 72 us delay
	ret

/* output 4 times the same bit */
four_bits:
	set_en_high              ;force data read from LCD controller
	set_en_low
	set_en_high              ;force data read from LCD controller
	set_en_low
	set_en_high              ;force data read from LCD controller
	set_en_low
	set_en_high              ;force data read from LCD controller
	set_en_low
	ret
#else
; 4-bit interface for HD44780 compatible controller
               ; Set RS (0=Cmd, 1=Char)
               sbrc    preg_1, 0
                       set_rs_high
               sbrs    preg_1, 0
                       set_rs_low
               set_rs_output;		//init hardware
               nop	;		//wait for address setup
               set_en_high
               set_en_output;		//init hardware

               ; Send high nibble
               set_b4_low
               set_b5_low
               set_b6_low
               set_b7_low

               sbrc    preg_2, 4
                       set_b4_high
               set_b4_output;		//init hardware
               sbrc    preg_2, 5
                       set_b5_high
               set_b5_output;		//init hardware
               sbrc    preg_2, 6
                       set_b6_high
               set_b6_output;		//init hardware
               sbrc    preg_2, 7
                       set_b7_high
               set_b7_output;		//init hardware

               nop			; wait for data setup time
               set_en_low		; force data read from LCD controller
               RCALL    wait1us

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

               nop			; wait for data setup time
               set_en_low		; force data read from LCD controller
               RCALL    wait1us
_lcd_hw_write_exit:
               ret
#endif

       .endfunc

