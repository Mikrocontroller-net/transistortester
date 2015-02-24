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
#define set_b0_output          sbi             (_SFR_IO_ADDR(HW_LCD_B0_PORT) - 1), HW_LCD_B0_PIN

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

; for ST7108 Controller: select CS1 and CS2
#define set_cs1_low             cbi             _SFR_IO_ADDR(HW_LCD_CS1_PORT), HW_LCD_CS1_PIN
#define set_cs1_high            sbi             _SFR_IO_ADDR(HW_LCD_CS1_PORT), HW_LCD_CS1_PIN
#define set_cs1_output          sbi             (_SFR_IO_ADDR(HW_LCD_CS1_PORT) - 1), HW_LCD_CS1_PIN

#define set_cs2_low             cbi             _SFR_IO_ADDR(HW_LCD_CS2_PORT), HW_LCD_CS2_PIN
#define set_cs2_high            sbi             _SFR_IO_ADDR(HW_LCD_CS2_PORT), HW_LCD_CS2_PIN
#define set_cs2_output          sbi             (_SFR_IO_ADDR(HW_LCD_CS2_PORT) - 1), HW_LCD_CS2_PIN

#define set_clk_low             cbi             _SFR_IO_ADDR(HW_LCD_CLK_PORT), HW_LCD_CLK_PIN
#define set_clk_high            sbi             _SFR_IO_ADDR(HW_LCD_CLK_PORT), HW_LCD_CLK_PIN
#define set_clk_output          sbi             (_SFR_IO_ADDR(HW_LCD_CLK_PORT) - 1), HW_LCD_CLK_PIN

#define RCALL rcall

/* For normal I2C mode use 5us wait time, but SSD1306 is faster, the cycle time is specified as 2.5us. */
/* So we use 2us, which results to a cycle time of >4us */
#define WAIT_I2C wait2us    
#define release_sda	cbi	(_SFR_IO_ADDR(HW_LCD_SDA_PORT) - 1), HW_LCD_SDA_PIN
#define set_low_sda	sbi	(_SFR_IO_ADDR(HW_LCD_SDA_PORT) - 1), HW_LCD_SDA_PIN
#define release_scl	cbi	(_SFR_IO_ADDR(HW_LCD_SCL_PORT) - 1), HW_LCD_SCL_PIN
#define set_low_scl	sbi	(_SFR_IO_ADDR(HW_LCD_SCL_PORT) - 1), HW_LCD_SCL_PIN
#define HW_LCD_SDA_OUT		_SFR_IO_ADDR(HW_LCD_SDA_PORT)
#define HW_LCD_SCL_OUT		_SFR_IO_ADDR(HW_LCD_SCL_PORT)
#define HW_LCD_SDA_IN		(_SFR_IO_ADDR(HW_LCD_SDA_PORT) - 2)
#define HW_LCD_SCL_IN		(_SFR_IO_ADDR(HW_LCD_SCL_PORT) - 2)
	/* SSD1306 controller defines 0x3C or 0x3D (SA0=1) as address LCD_I2C_ADDR */

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
       .extern wait30us		; used only for slow 4-bit interface (SLOW_LCD)
       .extern wait50us		; used for ST7920 controller

               

#if (LCD_INTERFACE_MODE == MODE_SPI)
; serial output for ST7565 controller, 4-Bit SPI
_lcd_hw_write:
               ; Set RS (0=Cmd, 1=Char)
               sbrc    preg_1, 0
                       set_rs_high
               sbrs    preg_1, 0
                       set_rs_low
               set_rs_output;		//init hardware
               set_b0_output		; wait for address setup, set B0 to output
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
	.endfunc

#elif (LCD_INTERFACE_MODE == MODE_7920_SERIAL)
_lcd_hw_write:
; 1-bit interface for ST7920 controller
	set_b0_high
	set_b0_output		; enable output mode
	set_en_low
	set_en_output		; enable output mode

	RCALL	toggle_en	; set en high and low

        RCALL	four_bits	; output four times 1  
	set_b0_low		; RW to write
	RCALL	toggle_en	; set en high and low

	sbrc    preg_1, 0
	set_b0_high		; data mode
	sbrs    preg_1, 0
	set_b0_low		; instruction mode
	RCALL	toggle_en	; set en high and low

	set_b0_low
	RCALL	toggle_en	; set en high and low
				; first 8 bit transfer finished
        sbrc    preg_2, 7
	 set_b0_high		; bit 7 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
        sbrc    preg_2, 6
	 set_b0_high		; bit 6 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
        sbrc    preg_2, 5
	 set_b0_high		; bit 5 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
        sbrc    preg_2, 4
	 set_b0_high		; bit 4 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
	RCALL	four_bits	; output 4 times 0
				; the upper 4-bit are followed by 4 x 0
	set_b0_low
        sbrc    preg_2, 3
	 set_b0_high		; bit 3 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
        sbrc    preg_2, 2
	 set_b0_high		; bit 2 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
        sbrc    preg_2, 1
	 set_b0_high		; bit 1 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
        sbrc    preg_2, 0
	 set_b0_high		; bit 0 == 1
	RCALL	toggle_en	; set en high and low

	set_b0_low
	RCALL	four_bits	; output 4 times 0
				; the lower 4-bit are followed by 4 x 0
	RCALL	wait50us
	RCALL	wait30us	; at least 72 us delay
	ret
	.endfunc

/* output 4 times the same bit */
four_bits:
	RCALL toggle_en
	RCALL toggle_en
	RCALL toggle_en
	RCALL toggle_en
	ret

toggle_en:
	set_en_high              ;force data read from LCD controller
        set_en_high		; hold en high to meet the specification (300ns)
	set_en_low		; set SCLK back to low
	ret
#elif (LCD_INTERFACE_MODE == MODE_I2C)
;===================================================
_lcd_hw_write:
	; use I2C as master
	push 	preg_2
        push	preg_1		; save data/command
	release_scl
	rcall	WAIT_I2C
	set_low_sda		; set START bit
	rcall	WAIT_I2C
	ldi	preg_1, (LCD_I2C_ADDR*2)
	rcall	i2c_send	; write I2C address
	pop	preg_2
	ldi	preg_1,0x80	; send command type
	sbrc	preg_2,0	; skip if bit 0 is unset
	ldi	preg_1,0x40	; send data type
	rcall	i2c_send	; send command/data
	pop	preg_1		;restore data from parameter
	rcall	i2c_send	; write the data
	set_low_sda		; set the sda signal to low STOP
	rcall	WAIT_I2C
	release_scl		; pullup move the scl signal to high
	rcall	WAIT_I2C
	release_sda		; pullup move the sda signal to high, STOP
	rcall	WAIT_I2C
	ret
;
;===================================================
i2c_send:
	sec			;set carry
	rol	preg_1		; shift carry to r24 bit 0 and bit 7 of r24 to carry
i2c_wf:
	set_low_scl		; scl signal to low, data change
	brcc	wr0
			; carry was set
	release_sda		; pullup move the sda signal to high
	rjmp	wr1
wr0:
	set_low_sda		; set the sda signal to low
wr1:
	rcall	WAIT_I2C	; wait defined time
	release_scl		; pullup move the scl signal to high
	rcall	WAIT_I2C	; wait defined time
	lsl	preg_1
	brne	i2c_wf
; 8 bit are transfered
	set_low_scl		; scl signal to low, data change
	release_sda		; give sda free
	rcall	WAIT_I2C	; wait defined time
	release_scl		; pullup move the scl signal to high, ack cycle
loop:
	sbis	HW_LCD_SCL_IN, HW_LCD_SCL_PIN
	rjmp	loop		; wait for releasing SCL
	; r24 is zero, return 0
	sbic	HW_LCD_SDA_IN, HW_LCD_SDA_PIN
	ldi	preg_1,1		; if SDA is returned high, answer 1
	rcall	WAIT_I2C	; wait defined time
	set_low_scl
	rcall	WAIT_I2C	; wait defined time
	ret
	.endfunc

       .global	i2c_init
       .func	i2c_init
       .extern	wait5us
i2c_init:
	release_sda
	release_scl
	cbi	HW_LCD_SDA_OUT, HW_LCD_SDA_PIN	; set output to 0, no pull up
	cbi	HW_LCD_SCL_OUT, HW_LCD_SCL_PIN	; set output to 0, no pull up

	ret
	.endfunc
#elif (LCD_INTERFACE_MODE == MODE_7108_SERIAL)
_lcd_hw_write:
; serial interface for ST7108 controller
	set_clk_low
	set_clk_output

        set_en_low
        set_en_output

	set_b0_low
        set_b0_output
        sbrc    preg_2, 7
	 set_b0_high		; bit 7 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 6
	 set_b0_high		; bit 6 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 5
	 set_b0_high		; bit 5 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 4
	 set_b0_high		; bit 4 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 3
	 set_b0_high		; bit 3 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 2
	 set_b0_high		; bit 2 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 1
	 set_b0_high		; bit 1 == 1
	RCALL	toggle_clk	; set clk high and low

        sbrc    preg_2, 0
	 set_b0_high		; bit 0 == 1
	RCALL	toggle_clk	; set clk high and low
        ; all 8 bit are loaded to the 74HC164 output

	set_rs_low		; instruction mode
        set_rs_output		; if RS is set to same as B0, RS is allready output
	sbrc    preg_1, 0
	set_rs_high		; data mode

	set_en_high
	RCALL	wait1us
	set_en_low
	RCALL	wait30us	; at least 30 us delay
	ret
	.endfunc

toggle_clk:
	set_clk_high
	set_clk_high
	set_clk_low
	set_b0_low
	ret

#else
_lcd_hw_write:
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
 #if (LCD_ST_TYPE == 7920)
               RCALL    wait50us
 #endif
 #ifdef SLOW_LCD
               RCALL    wait30us
 #else
               RCALL    wait1us
 #endif
_lcd_hw_write_exit:
               ret
       .endfunc
#endif
;----------------------------------------------------------------------
#if (LCD_ST_TYPE == 7108)
       .global _lcd_hw_select
       .func _lcd_hw_select
; select one of the two controllers or both
;
;      preg_1 (r24) = bit 0 for CS1 and bit 1 for CS2
;
_lcd_hw_select:
	sbrc    preg_1, 0
	  set_cs1_high		; enable controller 1
	sbrs    preg_1, 0
	  set_cs1_low		; disable controller 1
	sbrc    preg_1, 1
	  set_cs2_high		; enable controller 2
	sbrs    preg_1, 1
	  set_cs2_low		; disable controller 2
	set_cs1_output		; enable output CS1
	set_cs2_output		; enable output CS2
	ret
        .endfunc
#endif


