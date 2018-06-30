The software is tested with a board labled "1.5inch OLED Module".
This module can be configured with a I2C or with a SPI (4-wire) interface.
The default configuration is the SPI interface.
Please note, that the module require 3.3V signal level!
I have used a voltage divider build with 1k and 2.2k resistors for each
signal pin.
Pull-Up resistors of 10k to 3.3V are allready mounted at the Display Module.
The middle contact of the "BS" jumper field must be connected to the "0" side
(default position of the 0 Ohm resistor).
The tested modules have a connection between D1 (pin 17) and D2 (pin 18)
of the display connector. This signal is routed to the DIN pin of the module.
I have isolated the D2 (pin 18) connection for correct operation with the SPI interface.


Please note, that the horizontal and vertical orientation is interchanged
in the driver. One line of text is shown in Row (COM) direction.
The character height is 8 pixel of 16 pixel, which will use 4 or 8 column segments.
You can rotate the image by 180 degree by changing the LCD_ST7565_H_FLIP and
LCD_ST7565_V_FLIP option in the Makefile.
But there is currently no way to rotate the display content by 90 degree or 270 degree. 
Please mount the module in a correct position.
