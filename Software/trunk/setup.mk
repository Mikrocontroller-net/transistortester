# MKFILES == list of all Makefile parts
MKFILES = Makefile ../setup.mk ../finish.mk
CFLAGS += -mcall-prologues
# you can set the EFUSE_VAL to 0xfc or to 0x04, if your avrdude mask out the unused bits
# this value enable the brown out voltage of 4.3V
EFUSE_VAL = 0xfc
ifeq ($(INHIBIT_SLEEP_MODE),1)
# set the option for gcc 
CFLAGS += -DINHIBIT_SLEEP_MODE
endif

# Include current directory
CFLAGS  += -I.
# set the Language
CFLAGS += -D$(UI_LANGUAGE)

# Path to generated files
OBJDIR := ../Obj/$(PROJECT)
DEPDIR := ../Obj/$(PROJECT)/dep

TARGET = $(OBJDIR)/$(PROJECT).elf


#Add some Defines if 128x64 controller ST7565 is used
ifeq ($(WITH_LCD_ST7565),1)
CFLAGS += -DLCD_ST_TYPE=7565
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),7565)
CFLAGS += -DLCD_ST_TYPE=7565
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),1306)
CFLAGS += -DLCD_ST_TYPE=1306
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),7108)
CFLAGS += -DLCD_ST_TYPE=7108
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),7735)
CFLAGS += -DLCD_ST_TYPE=7735
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),9163)
CFLAGS += -DLCD_ST_TYPE=9163
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),9341)
CFLAGS += -DLCD_ST_TYPE=9341
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),7920)
CFLAGS += -DLCD_ST_TYPE=7920
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),8812)
CFLAGS += -DLCD_ST_TYPE=8812
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),8814)
CFLAGS += -DLCD_ST_TYPE=8814
CFLAGS += -DLCD_ST7565_RESISTOR_RATIO=$(LCD_ST7565_RESISTOR_RATIO)
WITH_lcd-draw = 1
endif
ifeq ($(WITH_LCD_ST7565),1327)
CFLAGS += -DLCD_ST_TYPE=1327
WITH_lcd-draw = 1
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
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m328p)
MCU = atmega328p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x8000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x7f:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m644)
MCU = atmega644
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x10000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m644p)
MCU = atmega644p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x10000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m644pa)
MCU = atmega644pa
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x10000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m1284)
MCU = atmega1284
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x20000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m1284p)
MCU = atmega1284p
HEX_FLASH_FLAGS = --change-section-lma .eeprom=0x20000
ifeq ($(OP_MHZ),1)
# RC operation ,CLK 1MHz
FUSES_INT = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /8 divider , low power
FUSES_CRY_L = -U lfuse:w:0x77:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
else
# RC operation ,CLK 8MHz
FUSES_INT = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , full swing crystal
FUSES_CRY = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
# Operation with 8MHz crystal and /1 divider , low power
FUSES_CRY_L = -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:$(EFUSE_VAL):m
endif
endif

ifeq ($(PARTNO),m2560)
MCU = atmega2560
endif

CFLAGS += -DF_CPU=$(OP_MHZ)000000UL
CFLAGS += -DF_CPU_HZ=$(OP_MHZ)000000
CFLAGS += -DMHZ_CPU=$(OP_MHZ)

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS += $(COMMON)
CFLAGS += -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# CFLAGS += -maccumulate-args
CFLAGS += -MD -MP -MT $(*F).o -MF $(DEPDIR)/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,--relax,-Map=$(PROJECT).map

# remove unused code
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
#HEX_FLASH_FLAGS += -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
# Please note, that the sequence of objects will change the 
# resulting program length, because call Instructions will be
# automatically replaced by the shorter rcall Instruction by optimizing.
# This replacement is only possible, if the target is near enough.
# You should not try to find another sequence of object files
# without to check and compare the results of different configurations.
# You can find a better sequence for one configuration,
# but the result of another configuration is a bigger program!
OBJECTS = 

ifneq ($(PARTNO),m8)
# I have not found any target, that will result to shorter program,
# if it is places here
endif

OBJECTS += lcd_hw_4_bit.o
OBJECTS += lcd-routines.o
OBJECTS += i2lcd.o
OBJECTS += PinLayout.o
OBJECTS += RvalOut.o
OBJECTS += UfAusgabe.o
OBJECTS += DisplayValue.o

ifeq ($(WITH_lcd-draw),1)
OBJECTS += lcd-draw.o
endif

OBJECTS += swuart.o
OBJECTS += wait1000ms.o 

ifneq ($(INHIBIT_SLEEP_MODE),1)
OBJECTS += sleep_5ms.o
endif

OBJECTS += ReadADC.o
OBJECTS += wait_for_key_ms.o
OBJECTS += RefVoltage.o

OBJECTS += main.o
OBJECTS += Battery_check.o
OBJECTS += CheckPins.o
OBJECTS += GetResistance.o
OBJECTS += ChargePin10ms.o
OBJECTS += EntladePins.o
OBJECTS += ReadCapacity.o
OBJECTS += GetRLmultip.o
OBJECTS += Calibrate_UR.o
OBJECTS += show_Resis_Cap.o

ifneq ($(PARTNO),m8)
OBJECTS += get_log.o
OBJECTS += ReadInductance.o
OBJECTS += GetESR.o
OBJECTS += GetVloss.o
OBJECTS += function_menu.o
OBJECTS += message_key_released.o
OBJECTS += GetFrequency.o
OBJECTS += ReadBigCap.o
OBJECTS += CheckRotaryEncoder.o
OBJECTS += CalibrationCap.o
OBJECTS += CheckUJT.o
OBJECTS += ShowData.o
endif

ifndef USE_EEPROM
OBJECTS +=  EE_check_init.o
endif

ifeq ($(WITH_SamplingADC),1)
CFLAGS += -DSamplingADC
OBJECTS += samplingADC.o sampling_cap.o sampling_lc.o sampling_xtal.o
endif

## Objects explicitly added by the user
LINKONLYOBJECTS = 


## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size
