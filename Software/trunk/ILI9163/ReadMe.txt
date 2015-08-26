The Makefile and .hex and .eep files in this directory
are configured for a 1.44" 128X128 SPI Color TFT LCD Module,
which is offered by some Chinese salesmen for a few dollars.
The module use a 8 pin SIP connector terminal with 100 mil pitch 
for power and SPI signals:
Pin  Signal
 1    VCC  (5V or 3.3V)
 2    GND
 3    CS   (Chip Select)
 4    RESET
 5    A0   (Command / Data)
 6    SDA  (Serial Data)
 7    SCK  (Serial Clock)
 8    LED  (Anode of the background LED)

Normaly the module use a ILI9163 controller.
The module include a 3.3V voltage regulator, so that you can connect
the VCC pin to 5V power directly.
But you should connect all the SPI signals to the ATmega outputs with
serial 10 kOhm resistors!
The CS input can be connected to GND or to the PD5 output of the ATmega.

The connection must be done in the following manner:

ATMEGA | Char-LCD | Color-LCD-Module
-------+----------+-----------
 PD0   | D4 (11)  |  RESET (4)
 PD1   | D5 (12)  |  A0    (5)
 PD2   | D6 (13)  |  SCK   (7)
 PD3   | D7 (14)  |  SDA   (6)
(PD5)  | (E (6))  |  CS    (3) 
 GND   | GND (1)  |  GND   (2)
 VCC   | VCC (2)  |  VCC   (1)
       | LED (15) |  LED   (8)

You should remember to use a 10k serial resistor for every connection
to a ATmega output. The only exceptions are the VCC and GND power,
which should be connected directly.
The LED pin must be connected to 5V with a
serial resistor (for example 39 Ohm) to accomplish a correct LED current.
Please check your board, that the pin 15 of the character LCD terminal is not directly
connected to VCC without any current limiting resistor.
