Datum: 23.9.2012   Test version Transistortester (AVR) Version 1.00k


For all adventurous people here is my short intruction, how to get
the ATmega firmware for 1MHz or 8MHz clock operation. 
My software is currently at a testing state.
I can give you no warranty for proper operation.

Software for 8 MHz or 1MHz operation is allways the same.
The source will adapt to your Makefile option.
With Linux operating system with the GNU toolchain and
program avrdude installed you need only 4 steps to
get a operable ATmega microcontroller.


1.) Edit your Makefile (~./Sourcecode/default/Makefile) 
    Select OP_MHZ=1 or OP_MHZ=8 operation and AVR-type (PARTNO=).
    Set the programmer type (PROGRAMMER=) and your programmer port (PORT=)
    if you wish to use avrdude to load data to the ATmega.

2.) Change to directory ~./Sourcecode/default and call "make" !
    You need no "make clean" call.

3.) Connect your programmer to the ATmega and call "make upload" to load the
    compiled data to the ATmega.

4.) If you need to change the ATmega fuses, call "make fuses" or "make fuses-crystal"
    for the crystal operation.

For crystal operation you need a proper installed 8MHz crystal at pin 9 and pin 10 of the
ATmega.
For ATmega168 and ATmega328 is a clock operation of 1MHz possible, even if
a 8MHz crystal installed. For this case a clock divide by 8 is programmed with the
fuses.
A new make fuses-crystal-lp call is also possible with the ATmega168 line of controllers
to save power with the Low Power Crystal Oscillator mode. The fuses-crystal call
enables the Full Swing Crystal Oscillator mode.
The ATmega8 must have a 1MHz crystal installed, if you wish a 1MHz crystal operation.
For better resolution of capacity measurement the 8Mhz clock is recommended.
If you don't use the Makefile for setting the fuses, be careful, you can set the
fuses, that no further ISP programming of your ATmega is possible!!!

I have tested a ATmega168 vesion (PARTNO=m168) and a ATmega8 version (PARTNO=m8).
You can use all options at same time only with ATmega168 version.
The selftest and automatic calibration is only possible with mega168 of mega328!
You can change from ATmega8 to ATmega168 or ATmega328 without hardware changes.

In the directory Sourcecode/mega8_auto are compiled programming data for the
ATmega8 without the selftest function but with AUTOSCALE_ADC.

In the directory Sourcecode/mega8_selftest are compiled programming date for
the ATmega8 without selftest function and without the AUTOSCALE_ADC option.
Both versions are compiled for 8MHz operation, as you can see in the Makefile options.

All my tests are done with 8Mhz crystal operation. You can select also the
8MHz RC clock operation if you don't have the crystal installed.
You must change the fuses for 8MHz operation or alternatively compile the software for
1MHz operation and load this data for proper operation.
Usually the fuses are preset to 1Mhz operation.
See the 4 steps above!



Please tell me, if you have problems with my software.
You can send me a Email to kh_kuebbeler@web.de or answer to my www.mikrocontroller.net thread.

Good luck
Karl-Heinz Kübbeler
