This version of Software is created for the chinese
WEI_M8_LGTST_V2.1 board.
This board carry a WEI_M8_LCD_LX02_V1.1 adapter board
with a graphical LCD module.
The controller of this LCD module is a ST7565 or compatible.
The 13 pin female header has the following layout:
Pin  Signal
 1    GND
 2    VCC
 3    GND  LCD-CS
 4    PD0  LCD-Reset 
 5    PD1  LCD-RS    
 6    PD2  LCD-SCLK  
 7    PD3  LCD-SI    
 8    VCC   ISP
 9    GND   ISP
10    MOSI  ISP
11    MISO  ISP
12    SCLK  ISP
13    Reset ISP
The tester has a micro USB connector at the left side,
which can be used to power the device and to charge
a AA LiIon Accu.
If the battery voltage is detected below 900mV, the tester
shows the message "DC Pwr Mode" instead of the battery voltage. 
If you plug in a charged LiIon Accu, you must also connect
the USB connector once for operating with the accu.
I recommend to charge the accu before first use.
You should not use the accu with a voltage below 2.8V.  

===========================================================
Please note, that another simular version labled VANVELL ELC
is also available!

One main difference is a 12-pin female header.
The layout is found by Janusz B. :
Pin  Signal
12    GND  LCD-CS
11    PD0  LCD-Reset
10    PD1  LCD-RS
 9    PD3  LCD-SI
 8    PD2  LCD-SCLK
 7    VCC   ISP + LCD
 6    GND   ISP + LCD
 5    150 Ohm to VCC   +LED 
 4    MOSI  ISP
 3    MISO  ISP
 2    SCLK  ISP
 1    Reset ISP

Please note, that the sequence of ISP-GND and ISP-VCC is different
for both versions. You can not use the same ISP adapter.
The signals of the LCD-Modul are also not in the same sequence!
