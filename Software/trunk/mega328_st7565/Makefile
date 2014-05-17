###############################################################################
# Makefile for the project TransistorTester
###############################################################################

## General Flags
PROJECT = TransistorTester
TARGET = $(PROJECT).elf
VPATH = ..:../bitmaps
CC = avr-gcc

CPP = avr-g++

CFLAGS = -Wall
#CFLAGS += -DWITH_MENU

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
PARTNO = m328p

# The WITH_MENU option enables a dialog to choose some additional functions.
# Currently a frequency measurement at ATmega Pin PD4, a frequency generator at TP2 ,
# tan the external voltage measurement can be selected. Of course you can also return to
# the normal transistor tester function and switch off the tester.
#CFLAGS += -DWITH_MENU

# Select your language:
# Available languages are: LANG_ENGLISH, LANG_GERMAN, LANG_POLISH, LANG_CZECH, LANG_SLOVAK, LANG_SLOVENE,
#                          LANG_DUTCH, LANG_BRASIL, LANG_RUSSIAN, LANG_UKRAINIAN, LANG_LITHUANIAN, LANG_HUNGARIAN
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
WITH_LCD_ST7565 = 1

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
#CFLAGS += -DWITH_UART

# With option TQFP_ADC6 or/and TQFP_ADC7 you can use the additional pins of the TQFP or
# the QFN package for external analog input. You should install a 10:1 voltage dividers
# on the selected pin(s).
# If both pins are defined, both voltages are measured with the voltage measure function.
# But for zener diode measurement the ADC6 pin is used, if both pins are defined.
#CFLAGS += -DTQFP_ADC6

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
#CFLAGS += -DUSE_EEPROM

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
CFLAGS += -DBAT_NUMERATOR=66
CFLAGS += -DBAT_DENOMINATOR=33
# Voltage divider for the external zener voltage measurement 180k / 20k = 10/1
#CFLAGS += -DEXT_NUMERATOR=10
#CFLAGS += -DEXT_DENOMINATOR=1

# The sleep mode of the ATmega168 or ATmega328 is normally used by the software to save current.
# You can inhibit this with the option INHIBIT_SLEEP_MODE .
#CFLAGS += -DINHIBIT_SLEEP_MODE


# Select your programmer type, speed and port, if you wish to use avrdude.
# setting for DIAMEX ALL_AVR, Atmel AVRISP-mkII
PROGRAMMER=avrispmkII
BitClock=1.0
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

#Add some Defines if 128x64 controller ST7565 is used
ifeq ($(WITH_LCD_ST7565),1)
CFLAGS += -DLCD_ST7565
CFLAGS += -DFOUR_LINE_LCD
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
ifeq ($(WITH_LCD_ST7565_HV_FLIP),1)
CFLAGS += -DLCD_ST7565_HV_FLIP
endif
endif
# Define under which conditions a graphical display is supported.
# The graphical display should at least support 128x64 pixels.
ifeq ($(WITH_LCD_ST7565),1)
CFLAGS += -DWITH_GRAPHICS
endif

ifeq ($(PARTNO),m8)
MCU = atmega8
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x2000
ifeq ($(OP_MHZ),1)
# BODLEV 4V, BODEN enabled, SUT 65ms, CLK 1MHz
FUSES_INT = -U lfuse:w:0x21:m -U hfuse:w:0xd9:m
# operation with 1MHz crystal!!!
FUSES_CRY = -U lfuse:w:0x3d:m -U hfuse:w:0xc9:m
FUSES_CRY_L = -U lfuse:w:0x3d:m -U hfuse:w:0xd9:m
else
# BODLEV 4V, BODEN enabled, SUT 65ms, CLK 8MHz
FUSES_INT = -U lfuse:w:0x24:m -U hfuse:w:0xd9:m
# Operation with 8MHz crystal 
FUSES_CRY = -U lfuse:w:0x3f:m -U hfuse:w:0xc9:m
FUSES_CRY_L = -U lfuse:w:0x3f:m -U hfuse:w:0xd9:m
endif
endif

ifeq ($(PARTNO),m88)
MCU = atmega88
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x2000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
endif
endif

ifeq ($(PARTNO),m88p)
MCU = atmega88p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x2000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
endif
endif

ifeq ($(PARTNO),m168)
MCU = atmega168
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x4000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
endif
endif

ifeq ($(PARTNO),m168p)
MCU = atmega168p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x4000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xdc:m
# -U efuse:w:0xf9:m
endif
endif

ifeq ($(PARTNO),m328)
MCU = atmega328
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x8000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
endif
endif

ifeq ($(PARTNO),m328p)
MCU = atmega328p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x8000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:0xfc:m
endif
endif

CFLAGS += -DF_CPU=$(OP_MHZ)000000UL
CFLAGS += -DMHZ_CPU=$(OP_MHZ)

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS += $(COMMON)
CFLAGS += -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
#HEX_FLASH_FLAGS += -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings



## Objects that must be built in order to link
OBJECTS = lcd-routines.o i2lcd.o PinLayout.o RvalOut.o UfAusgabe.o DisplayValue.o
ifeq ($(WITH_LCD_ST7565),1)
OBJECTS += lcd_hw_1_bit.o font.o lcd-draw.o
OBJECTS += bmp_one.o bmp_two.o bmp_three.o
OBJECTS += bmp_npn.o bmp_pnp.o 
OBJECTS += bmp_n_e_mos.o bmp_n_d_mos.o bmp_p_e_mos.o bmp_p_d_mos.o
OBJECTS += bmp_n_e_igbt.o bmp_n_d_igbt.o bmp_p_e_igbt.o bmp_p_d_igbt.o
OBJECTS += bmp_vakdiode.o
OBJECTS += bmp_n_jfet.o bmp_p_jfet.o bmp_triac.o bmp_thyristor.o
OBJECTS += bmp_inductor.o
else
OBJECTS += lcd_hw_4_bit.o
endif
OBJECTS += swuart.o wait1000ms.o 
ifndef INHIBIT_SLEEP_MODE
OBJECTS += sleep_5ms.o
endif
OBJECTS += ReadADC.o wait_for_key_ms.o RefVoltage.o
ifneq ($(PARTNO),m8)
OBJECTS += get_log.o
endif
OBJECTS += main.o CheckPins.o ChargePin10ms.o EntladePins.o
OBJECTS +=  ReadCapacity.o GetRLmultip.o Calibrate_UR.o
ifneq ($(PARTNO),m8)
OBJECTS +=  ReadInductance.o GetESR.o GetVloss.o GetFrequency.o function_menu.o message_key_released.o
endif
ifndef USE_EEPROM
OBJECTS +=  EE_check_init.o
endif

## Objects explicitly added by the user
LINKONLYOBJECTS = 


## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size

########### Compile only Assembler source available 
lcd_hw_1_bit.o: ../lcd_hw_1_bit.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

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

wait_for_key_ms.o: ../wait_for_key_ms.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

get_log.o: ../get_log.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

########### Compile C source only 
font.o: ../font.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd-draw.o: ../lcd-draw.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

DisplayValue.o: ../DisplayValue.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

function_menu.o: ../function_menu.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

message_key_released.o: ../message_key_released.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadCapacity.o: ../ReadCapacity.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadInductance.o: ../ReadInductance.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Calibrate_UR.o: ../Calibrate_UR.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CheckPins.o: ../CheckPins.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetVloss.o: ../GetVloss.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetFrequency.o: ../GetFrequency.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -D$(UI_LANGUAGE) -c  $<

ChargePin10ms.o: ../ChargePin10ms.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EntladePins.o: ../EntladePins.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

EE_check_init.o: ../EE_check_init.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_one.o: bmp_one.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_two.o: bmp_two.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_three.o: bmp_three.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_npn.o: bmp_npn.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_pnp.o: bmp_pnp.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_e_mos.o: bmp_p_e_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_d_mos.o: bmp_p_d_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_e_mos.o: bmp_n_e_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_d_mos.o: bmp_n_d_mos.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_e_igbt.o: bmp_p_e_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_d_igbt.o: bmp_p_d_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_e_igbt.o: bmp_n_e_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_d_igbt.o: bmp_n_d_igbt.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_vakdiode.o: bmp_vakdiode.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_n_jfet.o: bmp_n_jfet.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_p_jfet.o: bmp_p_jfet.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_triac.o: bmp_triac.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_thyristor.o: bmp_thyristor.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

bmp_inductor.o: bmp_inductor.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@echo $(OP_MHZ) MHz operation configured.
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map
steril:
	$(MAKE)
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).lss $(PROJECT).map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


# device programming
# make fuses  call if you don't have installed a crystal
fuses:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT)  $(FUSES_INT)

# fuses-crytal can only used if a 8MHz Crytal is installed (between Pin 9 and 10)
fuses-crystal:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT)  $(FUSES_CRY)
fuses-crystal-lp:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT)  $(FUSES_CRY_L)

# erases the FLASH ROM, EEPROM and clears all lock bits 
erase:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -e

# make upload  additionally calls make to compile the software for selected target
upload:
	$(MAKE)
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a \
	-U eeprom:w:./$(PROJECT).eep:a
program:
	$(MAKE)
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:w:./$(PROJECT).hex:a \
	-U eeprom:w:./$(PROJECT).eep:a
eeprom:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) \
	-U eeprom:w:./$(PROJECT).eep:a
verify:
	avrdude -D -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:v:./$(PROJECT).hex:a \
	-U eeprom:v:./$(PROJECT).eep:a -v
upload_orig:
	avrdude -c $(PROGRAMMER) -B $(BitClock) -p $(PARTNO) -P $(PORT) -U flash:w:./TransistorTestorig.hex:a \
	-U eeprom:w:./TransistorTestorig.eep:a
