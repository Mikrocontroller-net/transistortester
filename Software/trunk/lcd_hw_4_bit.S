#ifndef __ASSEMBLER__
 #define __ASSEMBLER__
#endif
;---------------------------------------------------------------------

#include <avr/io.h>
#include "config.h"

;#define FAST_SERIAL_OUT
#define FAST_SPI_OUTPUT
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
#define set_en_input           cbi             (_SFR_IO_ADDR(HW_LCD_EN_PORT) - 1), HW_LCD_EN_PIN

; Register select (0  = Command, 1 = Data)
#define set_rs_low             cbi             _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_high            sbi             _SFR_IO_ADDR(HW_LCD_RS_PORT), HW_LCD_RS_PIN
#define set_rs_output          sbi             (_SFR_IO_ADDR(HW_LCD_RS_PORT) - 1), HW_LCD_RS_PIN
#define set_rs_input           cbi             (_SFR_IO_ADDR(HW_LCD_RS_PORT) - 1), HW_LCD_RS_PIN

; for ST7565 controller: Serial data input (SI)
#define set_b0_low       cbi       _SFR_IO_ADDR(HW_LCD_B0_PORT), HW_LCD_B0_PIN
#define set_b0_high      sbi       _SFR_IO_ADDR(HW_LCD_B0_PORT), HW_LCD_B0_PIN
#define set_b0_output          sbi             (_SFR_IO_ADDR(HW_LCD_B0_PORT) - 1), HW_LCD_B0_PIN
#define set_b0_input           cbi             (_SFR_IO_ADDR(HW_LCD_B0_PORT) - 1), HW_LCD_B0_PIN
; for ST7565 controller: Chip Enable 
#define set_ce_low       cbi       _SFR_IO_ADDR(HW_LCD_CE_PORT), HW_LCD_CE_PIN
#define set_ce_high      sbi       _SFR_IO_ADDR(HW_LCD_CE_PORT), HW_LCD_CE_PIN
#define set_ce_output          sbi             (_SFR_IO_ADDR(HW_LCD_CE_PORT) - 1), HW_LCD_CE_PIN
#define set_ce_input           cbi             (_SFR_IO_ADDR(HW_LCD_CE_PORT) - 1), HW_LCD_CE_PIN

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

#define set_pclk_low             cbi             _SFR_IO_ADDR(HW_LCD_PCLK_PORT), HW_LCD_PCLK_PIN
#define set_pclk_high            sbi             _SFR_IO_ADDR(HW_LCD_PCLK_PORT), HW_LCD_PCLK_PIN
#define set_pclk_output          sbi             (_SFR_IO_ADDR(HW_LCD_PCLK_PORT) - 1), HW_LCD_PCLK_PIN

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

               

#if (LCD_INTERFACE_MODE == MODE_SPI) || (LCD_INTERFACE_MODE == MODE_3LINE)
;---------------------------------------------------------------------------------
; serial output for ST7565 controller, 4-Bit SPI
_lcd_hw_write:
 #ifdef LCD_SPI_OPEN_COL
	set_en_low
	set_en_output		// en/CLK to GND
  #ifdef PULLUP_DISABLE
	AOUT	MCUCR, r1		; MCUCR = 0;	//enable pull up resistors
  #endif
	set_ce_low
	set_ce_output		// enable chip
	; Set RS (0=Cmd, 1=Char)	
  #if (LCD_INTERFACE_MODE == MODE_3LINE)
	sbrc    preg_1, 0
	rjmp	set_rs
	set_b0_low
	set_b0_output
	rjmp	set_sce
set_rs:
	set_b0_input
	set_b0_high		// enable B0 pullup
set_sce:
	set_rs_low
	set_rs_output		// SCE to GND;
	rcall	wait1us
	set_en_input
	set_en_high		// enable en pullup
	rcall	wait1us
  #else
	sbrc    preg_1, 0
	rjmp	set_rs
	set_rs_low
	set_rs_output		// set B0 for RS to GND
        rjmp	fini_rs
set_rs:
	set_rs_input		// set B0 to input
	set_rs_high		// enable B0 pullup
fini_rs:
	rcall	wait1us
   #endif

	; Send bit-7
        ROL	preg_2		// shift B7 to carry
        rcall	shift_out
	; Send bit-6
        ROL	preg_2		// shift B6 to carry
        rcall	shift_out
	; Send bit-5
        ROL	preg_2		// shift B5 to carry
        rcall	shift_out
	; Send bit-4
        ROL	preg_2		// shift B4 to carry
        rcall	shift_out
	; Send bit-3
        ROL	preg_2		// shift B3 to carry
        rcall	shift_out
	; Send bit-2
        ROL	preg_2		// shift B2 to carry
        rcall	shift_out
	; Send bit-1
        ROL	preg_2		// shift B1 to carry
        rcall	shift_out
	; Send bit-0
        ROL	preg_2		// shift B0 to carry
        rcall	shift_out
	rcall	wait1us
	set_en_low
	set_en_output		// set en/clk to GND
  #if (LCD_INTERFACE_MODE == MODE_3LINE)
//	rcall	wait1us
	set_rs_input		// SCE to  high
	set_rs_high		// enable pullup
  #endif
	set_ce_input
        set_ce_high		// disable chip

  #ifdef PULLUP_DISABLE
	ldi	r25, (1<<PUD)		;
	AOUT	MCUCR, r25		; MCUCR = (1<<PUD);	//disable pull up resistors
  #endif
	set_en_low
	set_en_output		// en/CLK to GND
	ret		// return _lcd_hw_write

// sub-function shift_out: send 1, if carry is set, send 0, if carry is reset
shift_out:
	set_en_low
	set_en_output		// set en/clk to GND
        brcc	clr_bit
	set_b0_input		// set B0 to input
	set_b0_high		// enable B0 pullup = high
	rjmp	fini_bit
clr_bit:
	set_b0_low
	set_b0_output		// set B0 for Bx to GND
fini_bit:
	set_en_input
	set_en_high		// enable en/clk pullup
	rcall	wait1us
	ret
 #else /* no LCD_SPI_OPEN_COL */
  #ifdef FAST_SPI_OUTPUT
               ; Set RS (0=Cmd, 1=Char)
	set_ce_low
	set_ce_output		// enable chip
   #if (LCD_INTERFACE_MODE == MODE_3LINE)
               set_en_low
               sbrc    preg_1, 0
                       set_b0_high
               sbrs    preg_1, 0
                       set_b0_low
               set_b0_output		; set B0 to output
               set_rs_low		; SCE to GND
               set_rs_output		//init hardware
               set_en_high		; force data read from LCD controller
   #else
               sbrc    preg_1, 0
                       set_rs_high
               sbrs    preg_1, 0
                       set_rs_low
               set_rs_output;		//init hardware
               set_b0_output		; wait for address setup, set B0 to output
   #endif
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
   #if (LCD_INTERFACE_MODE == MODE_3LINE)
               set_rs_high		; SCE to VCC
   #endif
        set_ce_high		// disable chip
	set_en_low
	ret		// return _lcd_hw_write
  #else /* no FAST_SPI_OUTPUT */
               ; Set RS (0=Cmd, 1=Char)
        set_ce_low		// enable chip
	set_ce_output
   #if (LCD_INTERFACE_MODE == MODE_3LINE)
               set_en_low
               set_rs_low
               set_rs_output		//init hardware
               sbrc    preg_1, 0
                       set_b0_high	// set to data
               sbrs    preg_1, 0
                       set_b0_low	// set to command
               set_b0_output		; set B0 to output
               set_rs_low		; SCE to GND
               set_en_high		; force data read from LCD controller
   #else
               sbrc    preg_1, 0
                       set_rs_high
               sbrs    preg_1, 0
                       set_rs_low
               set_rs_output;		//init hardware
               set_b0_output		; wait for address setup, set B0 to output
   #endif
	; Send bit-7
        ROL	preg_2		// shift B7 to carry
        rcall	shift_out2
	; Send bit-6
        ROL	preg_2		// shift B6 to carry
        rcall	shift_out2
	; Send bit-5
        ROL	preg_2		// shift B5 to carry
        rcall	shift_out2
	; Send bit-4
        ROL	preg_2		// shift B4 to carry
        rcall	shift_out2
	; Send bit-3
        ROL	preg_2		// shift B3 to carry
        rcall	shift_out2
	; Send bit-2
        ROL	preg_2		// shift B2 to carry
        rcall	shift_out2
	; Send bit-1
        ROL	preg_2		// shift B1 to carry
        rcall	shift_out2
	; Send bit-0
        ROL	preg_2		// shift B0 to carry
        rcall	shift_out2
   #if (LCD_INTERFACE_MODE == MODE_3LINE)
        set_rs_high		; SCE to VCC
   #endif
        set_ce_high		// disable chip
	set_en_low
	ret		// return _lcd_hw_write

shift_out2:
	set_en_low;
        brcs	set_bit
	set_b0_low
	rjmp	fini_bit
set_bit:
	set_b0_high		// enable B0 pullup
fini_bit:
	set_b0_output		// set B0 to output mode
	set_en_high		// set en up
	ret
  #endif	/* FAST_SPI_OUTPUT */
 #endif  /* LCD_SPI_OPEN_COL */
	.endfunc

#elif (LCD_INTERFACE_MODE == MODE_7920_SERIAL)
;---------------------------------------------------------------------------------
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
	ret		// return _lcd_hw_write
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
;---------------------------------------------------------------------------------
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
	ret		// return _lcd_hw_write
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
;---------------------------------------------------------------------------------
_lcd_hw_write:
; serial interface for ST7108 controller
	set_clk_low
	set_clk_output

	set_pclk_low
	set_pclk_output

        set_en_low
        set_en_output

	set_b0_low
        set_b0_output
        sbrc    preg_2, 7
	 set_b0_high		; bit 7 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 6
	 set_b0_high		; bit 6 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 5
	 set_b0_high		; bit 5 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 4
	 set_b0_high		; bit 4 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 3
	 set_b0_high		; bit 3 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 2
	 set_b0_high		; bit 2 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 1
	 set_b0_high		; bit 1 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif

        sbrc    preg_2, 0
	 set_b0_high		; bit 0 == 1
 #ifdef FAST_SERIAL_OUT
	set_clk_high		; set clk high and low
	set_clk_low
	set_b0_low
 #else
	RCALL	toggle_clk	; set clk high and low
 #endif
        ; all 8 bit are loaded to the 74HC164 output
	set_pclk_high		; set parallel clk high and low
	set_pclk_low

	set_rs_low		; instruction mode
        set_rs_output		; if RS is set to same as B0, RS is allready output
	sbrc    preg_1, 0
	set_rs_high		; data mode
	RCALL	wait1us		; hold the setup time of RS

	set_en_high
	RCALL	wait1us
	set_en_low
;	RCALL	wait30us	; at least 30 us delay
	RCALL	wait10us	; at least 10 us delay
	ret
	.endfunc

 #ifndef FAST_SERIAL_OUT
toggle_clk:
	set_clk_high
	set_clk_high
	set_clk_low
	set_b0_low
	ret
 #endif

#else	/* !(LCD_INTERFACE_MODE == (MODE_SPI | MODE_7920_SERIAL | MODE_I2C | MODE_7108_SERIAL)) */
;---------------------------------------------------------------------------------
_lcd_hw_write:
; must be a 4-bit parallel interface for HD44780 compatible controller or simular
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
#endif /* LCD_INTERFACE_MODE */
;----------------------------------------------------------------------
#if (LCD_ST_TYPE == 7108)
       .global _lcd_hw_select
       .func _lcd_hw_select
; select one of the two controllers or both
;
;      preg_1 (r24) = bit 0 for CS1 and bit 1 for CS2
;
_lcd_hw_select:
#ifdef ST_CS_LOW   /* inverted CS level, 0 = enable */
	sbrc    preg_1, 0
	  set_cs1_low		; enable controller 1
	sbrs    preg_1, 0
	  set_cs1_high		; disable controller 1
	sbrc    preg_1, 1
	  set_cs2_low		; enable controller 2
	sbrs    preg_1, 1
	  set_cs2_high		; disable controller 2
#else	/* not inverted CS level, 1 = enable */
	sbrc    preg_1, 0
	  set_cs1_high		; enable controller 1
	sbrs    preg_1, 0
	  set_cs1_low		; disable controller 1
	sbrc    preg_1, 1
	  set_cs2_high		; enable controller 2
	sbrs    preg_1, 1
	  set_cs2_low		; disable controller 2
#endif
	set_cs1_output		; enable output CS1
	set_cs2_output		; enable output CS2
	ret
        .endfunc
#endif


