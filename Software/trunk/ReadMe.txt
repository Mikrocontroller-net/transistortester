This directory contains the program source for all TransistorTester versions.
To build the programming files for your ATmega microcontroller
please change to one of the following subdirectories:
mega8, default, mega328, mega168_strip_grid or mega328_strip_grid

The mega8 subdirectory contains the Makefile to build the programming data for a ATmega8 microcontroller.
The default subdirectory contains the Makefile to build the programming data for a ATmega168 controller.
The mega328 subdirectory contains the Makefile to build the programming data for a ATmega328 controller.
The subdirectories with the _strip_grid ending contains the Makefiles for the different connection
of the LCD used for the strip grid board and can not be used with the normal connection of the LCD.

Additionally all subdirectories hold the compiled data TransistorTester.hex and TransistorTester.eep!
The .hex data must be loaded to the flash memory and the .eep data must be loaded to the EEprom memory.
With the Linux operating system or with WinAVR the data can be loaded with the program avrdude to the
memory of the ATmega, controled with the Makefile. The connected ISP programmer must be supported by the program avrdude.
The data can also be loaded with any other program to the ATmega without control of the Makefile.

You can also create a own subdirectory and copy any of the Makefiles of the other directories to this
own subdirectory. Now you can set other options in this Makefile copy in your directory and try to build
your own TransistorTester programming data in this directory. 

Please tell me, if you have problems with my software or if you have found errors.
You can send me a Email to kh_kuebbeler@web.de or answer to my www.mikrocontroller.net thread.

Good luck
Karl-Heinz Kübbeler

