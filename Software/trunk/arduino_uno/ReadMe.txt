You must setup a shield board for the Arduino UNO to use the transistortester program.
You can find a board example build with strip grid material in the Hardware/ArduinoUno_shield.
This implementation currently don't use any LCD display.
All the data output is written only to the serial port at 115200 baud.
Of course a stand alone version of the tester have some advantages.
But the Arduino Uno is a easy way to test the software because you need no programmer
and no external power supply.

On my Mint Linux-system I have used the Arduino sketch environment to select the board (Tools/Board -> Arduino Uno).Then I could check and select the right serial port (Tools/Serieller Port -> /dev/ttyACM0).
Please check the number of the port (ttyACM0, ttyACM1 ...).
Your transistortester Makefile in the directory trunk/arduino_uno should use the same
USB PORT (behind PROGRAMMER=arduino).
Please do not use the sketch to build a transistortester program for the Arduino.
You should use my Makefile with the avr-gcc and the avrdude to build and upload the program data.
I did use the "make flash" command at the linux command window to upload the program data to the ATmega328P.
If you use the "make upload" command, the program data are loaded correctly, but the EEprom data are 
not written correctly.
With my board the loading of EEprom data had destroyed some data in the flash memory and the program
was not useable after this.
This problem with the EEprom is the reason why you should not use the Makefile option USE_EEPROM and 
why you should run the calibration twice.

For the tests I did use two chinese clones of the Arduino Uno, which have worked not proper in the original state.
The reason for this was a hardware problem at the Arduino boards with a DIP version of the ATmega328.
The GND signal for pin 8 of the ATmega was not connected at both boards. You should better check your
board, if you find a DIP ATmega328 at a socket.
The resistance between pin 8 and pin 22 of the ATmega328P should be below 0.5 Ohm!!!
Both pins of the ATmega328P are internaly connected, but this connection has usually a resistance
of more than 1 Ohm. When you are not shure, if your board has the same problem, you can remove the ATmega328 (DIP)
and check the resistance between pin 8 and pin 22 at the socket again.
You should solder a wire jumper to the Arduino Uno board, if you detect the same problem.
You can find a report with pictures at https://www.mikrocontroller.net/topic/395590  !
I have also added a additional SMD capacitor (220nF) between pin 7 and pin 8. 


To display the data of your transistor tester, the serial port (TXD, PD1) is used.
This is connected to the USB-port of your Arduino Board.
To show the results, you should start the terminal window (Tools/Serial Monitor) at your Arduino sketch.
The terminal window starts with the previous selected serial port (Tools/Serieller Port).
You must select a baud rate of 115200 at your "Serial Monitor" window.
Please finish your "Serial Monitor" before you try to upload new software !
