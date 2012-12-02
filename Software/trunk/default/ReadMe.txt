Date: 01.12.2012   Test version Transistortester (AVR) Version 1.04k


For all adventurous people here is my short intruction, how to get
the ATmega firmware for 1MHz or 8MHz clock operation. 
I can give you no warranty for proper operation.

Software for 8 MHz or 1MHz operation is allways the same.
The source will adapt to your Makefile option.
The 8 Mhz operation is recommended, because last tests are
always done in this mode.

With Linux operating system with the GNU toolchain and
program avrdude installed you need only 4 steps to
get a operable ATmega microcontroller.


1.) Edit your Makefile (~./trunk/default/Makefile) 
    Select OP_MHZ=1 or OP_MHZ=8 operation and AVR-type (PARTNO=).
    Set the programmer type (PROGRAMMER=) and your programmer port (PORT=)
    if you wish to use avrdude to load data to the ATmega.

2.) Change to directory ~./trunk/default and call "make" !
    You need no "make clean" call.

3.) Connect your programmer to the ATmega and call "make upload" to load the
    compiled data to the ATmega.

4.) If you need to change the ATmega fuses, call "make fuses" or "make fuses-crystal"
    for the crystal operation. You should disconnect the ISP plug now.

For crystal operation you need a proper installed 8MHz crystal at pin 9 and pin 10 of the
ATmega.
For ATmega168 and ATmega328 is a clock operation of 1MHz possible, even if
a 8MHz crystal installed. For this case a clock divide by 8 is programmed with the
fuses. 
A new make fuses-crystal-lp call is also possible with the ATmega168 line of controllers
to save power with the Low Power Crystal Oscillator mode. The fuses-crystal call
enables the Full Swing Crystal Oscillator mode.
The ATmega8 must have a 1MHz crystal installed, if you wish a 1MHz crystal operation.
For better resolution of capacity and inductivity measurement the 8Mhz clock is recommended.
If you don't use the Makefile for setting the fuses, be careful, you can set the
fuses, that no further ISP programming of your ATmega is possible!!!

I have tested some ATmega168 versions (PARTNO=m168) as well as some ATmega328 (PARTNO=m328)
and some ATmega8 version (PARTNO=m8).
You can use all options only with ATmega168 or ATmega328 versions.
The selftest and automatic calibration is only possible with mega168 or mega328!
You can change from ATmega8 to ATmega168 or ATmega328 without hardware changes.
I strongly recommend to change from ATmega8 to ATmega168 or
better ATmega328 microcontroller.

In the directory trunk/mega8 are compiled programming date for
the ATmega8 without selftest function and without the AUTOSCALE_ADC option.
This versions is compiled for 8MHz operation, as you can see in the Makefile options.

The directory trunk/mega168_strip_grid contains compiled data for ATmega168 for
the strip grid printed board. You can not use the standard version with this
modified layout!

All my tests are done with 8Mhz crystal operation. You can select also the
8MHz RC clock operation if you don't have the crystal installed.
The fuses of ATmega are usually preset to 1 MHz operation.
You must change the fuses for 8MHz operation (RC or crystal mode) or alternatively 
you can compile the software for 1MHz operation and load this data for proper operation.
See the 4 steps above!

The precompiled data for the ATmega168 can also be used for a ATmega328!

Usage of mega168 or mega328 versions:
=====================================
You should run the selftest with autocalibration of your tester to get better measurement results for
capacity measurement and resistor measurement, especially with the AUTOSCALE_ADC option set.
For autocalibration you need a high quality external capacitor with at least 100nF and
less than 20uF. 
This capacitor must be connected to pin 1 and pin 3 of the tester after measuring the
zero offset of capacity measurement and the text "1--||--3 >100nF" is shown.
The zero offset of the ESR (Equivalent Series Resistor) of a capacitor would be adjusted, if
you connect a capacitor with a high capacity value and a very low ESR. The tester assumes, that
negative ESR is not possible, and will reduce the zero offset so that result will be zero.
Because this measurement is not very stable, you should repeat this calibration some times.
This correction behavior is shown with the output of "ESR=0?".
The new zero offset take effect for the next measurements until the next selftest will reset
the zero offset to the initial value.
Therfore you must repeat this calibration step after every selftest!
Measurement of ESR is very critical, since the resolution of resistance measurement is 0.01 Ohm.
Unstable connection, the printed board, the ATmega exemplar or cable resistance have influences
to the ESR measurement result!


Please tell me, if you have problems with my software or if you have found errors.
You can send me a Email to kh_kuebbeler@web.de or answer to my www.mikrocontroller.net thread.

Good luck
Karl-Heinz Kübbeler
