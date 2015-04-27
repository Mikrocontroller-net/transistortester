This configuration is made for the chinese printed board
fish8840 with a graphical display.
The graphical display is controlled with a ST7565 controller or compatible.
The LCD printed board is equipped with a 3.3V voltage regulator, but
the 5V data signal level is routed to the controller input directly.
The controller data sheet does not allow 5V signal level to any
data input.
So the operating voltage of the ST7565 controller can be increased
by the signal level from ATmega outputs with internal protection diodes
of the ST7565 controller.
You should install serial resistors of about 2700 Ohm to all four
data lines of the ATmega (PD0-PD3) to prevent this irregular voltage enhancement!
Usually only 6 pins (11-16) of the 16 pin female header of the LCD is installed,
but also pin 1-6 is connected at the main board.

So you can also use a character display (16x2) with this printed board,
if you install a additional female header (1-6) and update the processor
with a program version for character display. But you should notice,
that there is no current limiting resistor at pin 15 of the femal header
for the anode of the background LED. If your display module is equipped
with a background LED, you should check that pin 15 of your display module
is not directly connected to the anode ot the LED.  

Some details are described in the PDF documentation about this board.
You should check, if the battery voltage measurement must be modified
to prevent a irregular battery consumption.
