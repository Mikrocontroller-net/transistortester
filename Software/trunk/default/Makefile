##############################################################################
# Makefile for the project TransistorTester
###############################################################################

## General Flags
PROJECT = TransistorTester
TARGET = $(PROJECT).elf
VPATH = ..:../bitmaps
CC = avr-gcc

CPP = avr-g++

CFLAGS = -Wall

# ********************** Änderbare Flags zur Configuration des Transistortesters
# ********************** config options for your Semiconductor tester
# Every changing of this Makefile will result in new compiling the whole
# programs, if you call make or make upload.

# Select your Part-No. for avrdude :
# atmega8  : m8
# atmega168: m168 or m168p
# atmega328: m328 or m328p
# atmega1280: m1280	// see config.h for different port setting
# atmega2560: m2560	// see config.h for different port setting
PARTNO = m168

# Select your language:
# Available languages are: LANG_BRASIL, LANG_CZECH, LANG_DUTCH, ENGLISH, LANG_GERMAN, LANG_HUNGARIAN, LANG_ITALIAN,
#                          LANG_LITHUANIAN, LANG_POLISH, LANG_RUSSIAN , LANG_SLOVAK, LANG_SLOVENE, LANG_UKRAINIAN
UI_LANGUAGE = LANG_ENGLISH

# The LCD_CYRILLIC option is necessary, if you have a display with cyrillic characterset.
# This lcd-display don't have a character for Ohm and for µ (micro).
# Russian language requires a LCD controller with russian characterset and option LCD_CYRILLIC!
#CFLAGS += -DLCD_CYRILLIC

# The LCD_DOGM option must be set for support of the DOG-M type of LCD modules with ST7036 controller.
# For this LCD type the contrast must be set with software command.
#CFLAGS += -DLCD_DOGM

# The LCD_ST7565 option must be set to support a 128x64 LCD graphics display
# with ST7565 controller. It is controlled with a 1-bit interface.
#WITH_LCD_ST7565 = 1

# If LCD_ST7565 option is used: Set the resitor ratio for the internal
# voltage regulator. Supported value range: 0..7.
# Good values are e.g. 4 or 7. If unsure just have a try.
LCD_ST7565_RESISTOR_RATIO = 4

# If LCD_ST7565 option is used: Flip the display both on horizontal and
# vertical direction.
WITH_LCD_ST7565_HV_FLIP = 1

# If option WITH_LCD_ST7565 is present one of the following fonts should be
# choosen. Currently only FONT_6X8 is completely supported.
CFLAGS += -DFONT_6X8
#CFLAGS += -DFONT_8X8
#CFLAGS += -DFONT_8X12
#CFLAGS += -DFONT_8X14
#CFLAGS += -DFONT_10X16

# Option STRIP_GRID_BOARD selects different board-layout, do not set for standard board!
# The connection of LCD is totally different for both versions.
#CFLAGS += -DSTRIP_GRID_BOARD

# The WITH_SELFTEST option enables selftest function (only for mega168 or mega328) including the calibration.
# Normally the mega168 uses selftest without T1 to T7 to enable both hFE measurements.
CFLAGS += -DWITH_SELFTEST
# You can enable the extended tests T1 to T7 for the atmega168 by selecting the  NO_COMMON_COLLECTOR_HFE  option.
# The T1 to T7 tests are usefull to find problems with your tester.
#CFLAGS += -DNO_COMMON_COLLECTOR_HFE
#CFLAGS += -DNO_COMMON_EMITTER_HFE
# You can also disable the extended tests T1 to T7 with the option NO_TEST_T1_T7 to accelerate the calibration
# for the atmega328 and atmega168.
#CFLAGS += -DNO_TEST_T1_T7

# AUTO_CAL will enable the autocalibration of zero offset of capacity measurement and
# also the port output resistance values will be find out in SELFTEST section.
# With a external capacitor a additionally correction of reference voltage is figured out for 
# low capacity measurement and also for the AUTOSCALE_ADC measurement.
# The AUTO_CAL option is only selectable for mega168 and mega328.
CFLAGS += -DAUTO_CAL

# FREQUENCY_50HZ enables a 50 Hz frequency generator for up to one minute at the end of selftests.
#CFLAGS += -DFREQUENCY_50HZ


# The WITH_AUTO_REF option enables reading of internal REF-voltage to get factors for the Capacity measuring.
CFLAGS += -DWITH_AUTO_REF
# REF_C_KORR corrects the reference Voltage for capacity measurement (<40uF) and has mV units.
# Greater values gives lower capacity results.
CFLAGS += -DREF_C_KORR=12
# REF_L_KORR corrects the reference Voltage for inductance measurement and has mV units.
CFLAGS += -DREF_L_KORR=40
# C_H_KORR defines a correction of 0.1% units for big capacitor measurement.
# Positive values will reduce measurement results.
CFLAGS += -DC_H_KORR=0

# The WITH_UART option enables the software UART  (TTL level output at Pin PC3, 26).
# If the option is deselected, PC3 can be used as external voltage input with a
# 10:1 resistor divider.
CFLAGS += -DWITH_UART

# With option TQFP_ADC6 or/and TQFP_ADC7 you can use the additional pins of the TQFP or
# the QFN package for external analog input. You should install a 10:1 voltage dividers
# on the selected pin(s).
# If both pins are defined, both voltages are measured with the voltage measure function.
# But for zener diode measurement the ADC6 pin is used, if both pins are defined.
#CFLAGS += -DTQFP_ADC6

# With option TQFP_ADC6 or/and TQFP_ADC7 you can use the additional pins of the TQFP or
# the QFN package for external analog input. You should install a 10:1 voltage dividers
# on the selected pin(s).
# If both pins are defined, both voltages are measured with the voltage measure function.
# But for zener diode measurement the ADC6 pin is used, if both pins are defined.
#CFLAGS += -DTQFP_ADC7

# The CAP_EMPTY_LEVEL  defines the empty voltage level for capacitors in mV.
# Choose a higher value, if your Tester reports "Cell!" by unloading capacitors.
CFLAGS += -DCAP_EMPTY_LEVEL=4

# The AUTOSCALE_ADC option enables the autoscale ADC (ADC use VCC and Bandgap Ref).
CFLAGS += -DAUTOSCALE_ADC
CFLAGS += -DREF_R_KORR=3

# The ESR_ZERO value define the zero value of ESR measurement (units = 0.01 Ohm).
#CFLAGS += -DESR_ZERO=29
CFLAGS += -DESR_ZERO=20

# NO_AREF_CAP tells your Software, that you have no Capacitor installed at pin AREF (21).
# This enables a shorter wait-time for AUTOSCALE_ADC function.
# A capacitor with 1nF can be used with the option NO_AREF_CAP set.
CFLAGS += -DNO_AREF_CAP

# The OP_MHZ option tells the software the Operating Frequency of your ATmega.
OP_MHZ = 8

# Restart from sleep mode will be delayed for 16384 clock tics with crystal mode.
# Operation with the internal RC-Generator or external clock will delay the restart by only 6 clock tics.
# You must specify this with "CFLAGS += -DRESTART_DELAY_TICS=6", if you don't use the crystal mode.
#CFLAGS += -DRESTART_DELAY_TICS=6

# The USE_EEPROM option specify where you wish to locate fix text and tables.
# If USE_EEPROM is unset, program memory (flash) is taken for fix text and tables.
CFLAGS += -DUSE_EEPROM

# Setting EBC_STYPE will select the old style to present the order of Transistor connection (EBC=...).
# Omitting the option will select the 123=... style.  Every point is replaced by a character identifying 
# type of connected transistor pin (B=Base, E=Emitter, C=Collector, G=Gate, S=Source, D=Drain).
# If you select EBC_STYLE=321 , the style will be 321=... , the inverted order to the 123=... style.
#CFLAGS += -DEBC_STYLE
#CFLAGS += -DEBC_STYLE=321

# Setting of NO_NANO avoids the use of n as prefix for Farad (nF), the mikro prefix is used insted (uF).
# CFLAGS += -DNO_NANO

# The PULLUP_DISABLE option disable the pull-up Resistors of IO-Ports.
# To use this option a external pull-up Resistor (10k to 30k)
# from Pin 13 to VCC must be installed!
CFLAGS += -DPULLUP_DISABLE

# The ANZ_MESS option specifies, how often an ADC value is read and accumulated.
	# Possible values of ANZ_MESS are 5 to 200 .
CFLAGS += -DANZ_MESS=25


# The POWER_OFF option enables the power off function, otherwise loop measurements infinitely
# until power is disconnected with a ON/OFF switch (CFLAGS += -DPOWER_OFF).
# If you have the tester without the power off transistors, you can deselect POWER_OFF .
# If you have NOT selected the POWER_OFF option with the transistors installed,
# you can stop measuring by holding the key several seconds after a result is
# displayed. After releasing the key, the tester will be shut off by timeout.
# Otherwise you can also specify, after how many measurements without found part
# the tester will shut down (CFLAGS += -DPOWER_OFF=5).
# The tester will also shut down with found part,
# but successfull measurements are allowed double of the specified number.
#  You can specify up to 255 empty measurements (CFLAGS += -DPOWER_OFF=255).
#CFLAGS += -DPOWER_OFF=5
CFLAGS += -DPOWER_OFF

# Option BAT_CHECK enables the Battery Voltage Check, otherwise the SW Version is displayed instead of Bat.
# BAT_CHECK should be set for battery powered tester version.
CFLAGS += -DBAT_CHECK

# The BAT_OUT option enables Battery Voltage Output on LCD (if BAT_CHECK is selected).
# If your 9V supply has a diode installed, use the BAT_OUT=600 form to specify the
# threshold voltage of your diode to adjust the output value.
# This threshold level is added to LCD-output and does not affect the voltage checking levels.
CFLAGS += -DBAT_OUT=150

# To adjust the warning-level and poor-level of battery check to the capability of a
# low drop voltage regulator, you can specify the Option BAT_POOR=5400 .
# The unit for this option value is 1mV , 5400 means a poor level of 5.4V.
# The warning level is 0.8V higher than the specified poor level (>5.3V).
# The warning level is 0.4V higher than the specified poor level (>2.9V, <=5.3V).
# The warning level is 0.2V higher than the specified poor level (>1.3V, <=2.9V).
# The warning level is 0.1V higher than the specified poor level (<=1.3V).
# Setting the poor level to low values is not recommended for rechargeable Batteries,
# because this increase the danger for deep discharge!!
CFLAGS += -DBAT_POOR=6400

# Voltage divider for battery voltage measurement  10k / 3.3k = 133/33
#CFLAGS += -DBAT_NUMERATOR=133
#CFLAGS += -DBAT_DENOMINATOR=33
# Voltage divider for the external zener voltage measurement 180k / 20k = 10/1
#CFLAGS += -DEXT_NUMERATOR=10
#CFLAGS += -DEXT_DENOMINATOR=1

# The sleep mode of the ATmega168 or ATmega328 is normally used by the software to save current.
# You can inhibit this with the option INHIBIT_SLEEP_MODE .
INHIBIT_SLEEP_MODE = 0


# Select your programmer type, speed and port, if you wish to use avrdude.
# setting for DIAMEX ALL_AVR, Atmel AVRISP-mkII
PROGRAMMER=avrisp2
BitClock=5.0
PORT=usb
# setting for USBasp
#PROGRAMMER=usbasp
#BitClock=20
#PORT=usb
# setting for ARDUINO MEGA, requires bootloader
#PROGRAMMER=wiring
#PORT = /dev/ttyACM0
#BitClock=5.0
#AVRDUDE_BAUD = -b 115200 -D
# ********************** end of selectable options

include ../setup.mk

########### Compile only Assembler source available 
lcd_hw_4_bit.o: ../lcd_hw_4_bit.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wait1000ms.o: ../wait1000ms.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

swuart.o: ../swuart.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

########### Compile  Assembler source only, is time critical 
GetESR.o: ../GetESR.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

########### Compile C or Assembler , Assembler saves more than 400 bytes flash
ifeq ($(WITH_LCD_ST7565),1)
lcd-routines.o: ../lcd-routines.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
else
lcd-routines.o: ../lcd-routines.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<
endif

GetRLmultip.o: ../GetRLmultip.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

UfAusgabe.o: ../UfAusgabe.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

RvalOut.o: ../RvalOut.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PinLayout.o: ../PinLayout.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

RefVoltage.o: ../RefVoltage.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2lcd.o: ../i2lcd.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadADC.o: ../ReadADC.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sleep_5ms.o: ../sleep_5ms.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $< 

wait_for_key_ms.o: ../wait_for_key_ms.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

get_log.o: ../get_log.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

include ../finish.mk

