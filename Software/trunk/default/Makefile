###############################################################################
# Makefile for the project TransistorTester
###############################################################################

## General Flags
PROJECT = TransistorTester
TARGET = TransistorTester.elf
CC = avr-gcc

CPP = avr-g++

CFLAGS = -Wall

# ********************** Änderbare Flags zur Configuration des Transistortesters
# ********************** config options for your Semiconductor tester
# NOTE: Software will deselect impossible Options for atmega48 automatically!!
#every changing of this Makefile will result in new compiling the whole
#programs, if you call make or make upload

#select your Part-No. for avrdude :
# atmega8  : m8
# atmega168: m168 or m168p
# atmega328: m328 or m328p
PARTNO = m168

#select your language
# available languages are: LANG_ENGLISH, LANG_GERMAN, LANG_POLISH, LANG_CZECH, LANG_SLOVAK, LANG_SLOVENE,
#                          LANG_RUSSIAN
UI_LANGUAGE = LANG_ENGLISH

# the LCD_CYRILLIC option is necessary, if you have a display with cyrillic characterset
# this lcd-display don't have a character for Ohm and for µ (micro)
# Russian language requires a LCD controller with russian characterset and option LCD_CYRILLIC!
#CFLAGS += -DLCD_CYRILLIC

# Option STRIP_GRID_BOARD selects different board-layout, do not set for standard board!
#CFLAGS += -DSTRIP_GRID_BOARD

# the WITH_SELFTEST option enables selftest function 
CFLAGS += -DWITH_SELFTEST
# AUTO_CAL will enable the autocalibration of zero offset of capacity measurement in selftest section
# Additionally the port output resistance and correction for low capacity measurement can be done
# with ATmega168
CFLAGS += -DAUTO_CAL
# FREQUENCY_50HZ enables a 50 Hz frequency generator for up to one minute at the end of selftests
#CFLAGS += -DFREQUENCY_50HZ


# the WITH_AUTO_REF option enables reading of internal REF-voltage to get factors for the Capacity measuring
CFLAGS += -DWITH_AUTO_REF
# REF_C_KORR has mV units, greater values gives lower capacity results
CFLAGS += -DREF_C_KORR=12
CFLAGS += -DC_H_KORR=0
CFLAGS += -DREF_L_KORR=50

# the WITH_UART option enables the software UART
CFLAGS += -DWITH_UART

# this level defines the empty voltage level for capacitors in mV
CFLAGS += -DCAP_EMPTY_LEVEL=4

# the AUTOSCALE_ADC option enables the autoscale ADC (VCC and Bandgap Ref)
CFLAGS += -DAUTOSCALE_ADC
CFLAGS += -DREF_R_KORR=3

# the ESR_ZERO value define the zero value of ESR measurement (units = 0.01 Ohm)
CFLAGS += -DESR_ZERO=29

# NO_AREF_CAP tells your Software, that you have no Capacitor installed at pin AREF (21)
# this enables a shorter wait-time for AUTOSCALE_ADC function
CFLAGS += -DNO_AREF_CAP

# the OP_MHZ option tells the software the Operating Frequency
OP_MHZ = 8
#OP_MHZ = 1

# the USE_EEPROM option specify where you wish to locate fix text and tables
CFLAGS += -DUSE_EEPROM


# the PULLUP_DISABLE option disable the pull-up Resistors of IO-Ports
# to use this option a external pull-up Resistor (10k to 30k)
# from Pin 13 to VCC must be installed!
CFLAGS += -DPULLUP_DISABLE

# the ANZ_MESS option specifies, how often an ADC value is read and accumulated
# possible values of ANZ_MESS are 5 to 200
CFLAGS += -DANZ_MESS=25


# The POWER_OFF option enables the power off function, otherwise loop measurements infinitely
# until power is disconnected with a ON/OFF switch (CFLAGS += -DPOWER_OFF).
# If you have the tester without the power off transistors, you can deselect POWER_OFF
# If you have NOT selected the POWER_OFF option with the transistors installed,
# you can stop measuring by holding the key several seconds after a result is
# displayed. After releasing the key, the tester will be shut off by timeout.
# Otherwise you can also specify, after how many measurements without found part
# the tester will shut down (CFLAGS += -DPOWER_OFF=5).
# The tester will also shut down with found part,
# but measurements are allowed double of the specified number.
#  You can specify up to 255 empty measurements (CFLAGS += -DPOWER_OFF=255).
CFLAGS += -DPOWER_OFF=5
# option BAT_CHECK enables the Battery Voltage Check, otherwise output SW Version
# is only needed for battery powered tester version
CFLAGS += -DBAT_CHECK
# The BAT_OUT option enables Battery Voltage Output on LCD (if BAT_CHECK is selected).
# If your 9V supply has a diode installed, use the BAT_OUT=600 form to specify the
# threshold voltage of your diode to adjust the output value.
# This threshold level is added to LCD-output and does not affect the voltage checking levels.
CFLAGS += -DBAT_OUT=150

# To adjust the warning-level and poor-level of battery check to the capability of a
# low drop voltage regulator, you can specify the Option BAT_POOR=54 .
# The unit for this option value is 100mV (0.1 Volt), 54 means a poor level of 5.4V.
# The warning level is always 1V higher than the specified poor level.
# Setting the poor level to low values is not recommended for rechargeable 9V Batteries,
# because this increase the danger for deep discharge!!
CFLAGS += -DBAT_POOR=64


# select your programmer type and port if you will use avrdude
PROGRAMMER=avrisp2
PORT=usb
# ********************** end of selectable options

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
ifeq ($(PARTNO),m48)
MCU = atmega48
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x1000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
endif
endif

ifeq ($(PARTNO),m48p)
MCU = atmega48p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x1000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xdc:m
# -U efuse:w:0xff:m
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
LDFLAGS +=  -Wl,-Map=TransistorTester.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
#HEX_FLASH_FLAGS += -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings



## Objects that must be built in order to link
OBJECTS = lcd-routines.o lcd_hw_4_bit.o main.o swuart.o wait1000ms.o CombineToLong.o ReadCapacity.o ReadInductance.o ReadADC.o GetESR.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 


## Build
all: $(TARGET) TransistorTester.hex TransistorTester.eep TransistorTester.lss size

## Compile
lcd-routines.o: ../lcd-routines.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_hw_4_bit.o: ../lcd_hw_4_bit.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadCapacity.o: ../ReadCapacity.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadInductance.o: ../ReadInductance.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ReadADC.o: ../ReadADC.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

GetESR.o: ../GetESR.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -D$(UI_LANGUAGE) -c  $<

swuart.o: ../swuart.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wait1000ms.o: ../wait1000ms.S Makefile
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CombineToLong.o: ../CombineToLong.S Makefile
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
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) TransistorTester.elf dep/* TransistorTester.hex TransistorTester.eep TransistorTester.lss TransistorTester.map
steril:
	make
	-rm -rf $(OBJECTS) TransistorTester.elf dep/* TransistorTester.lss TransistorTester.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


# device programming
# make fuses  call if you don't have installed a crystal
fuses:
	avrdude -c $(PROGRAMMER) -B 10.0 -p $(PARTNO) -P $(PORT)  $(FUSES_INT)

# fuses-crytal can only used if a 8MHz Crytal is installed (between Pin 9 and 10)
fuses-crystal:
	avrdude -c $(PROGRAMMER) -B 10.0 -p $(PARTNO) -P $(PORT)  $(FUSES_CRY)
fuses-crystal-lp:
	avrdude -c $(PROGRAMMER) -B 10.0 -p $(PARTNO) -P $(PORT)  $(FUSES_CRY_L)

# make upload  additionally calls make to compile the software for selected target
upload:
program:
	make
	avrdude -c $(PROGRAMMER) -B 5.0 -p $(PARTNO) -P $(PORT) -U flash:w:./TransistorTester.hex:a \
	-U eeprom:w:./TransistorTester.eep:a
eeprom:
	avrdude -c $(PROGRAMMER) -B 5.0 -p $(PARTNO) -P $(PORT) \
	-U eeprom:w:./TransistorTester.eep:a
verify:
	avrdude -D -c $(PROGRAMMER) -B 5.0 -p $(PARTNO) -P $(PORT) -U flash:v:./TransistorTester.hex:a \
	-U eeprom:v:./TransistorTester.eep:a -v
upload_orig:
	avrdude -c $(PROGRAMMER) -B 5.0 -p $(PARTNO) -P $(PORT) -U flash:w:./TransistorTestorig.hex:a \
	-U eeprom:w:./TransistorTestorig.eep:a
