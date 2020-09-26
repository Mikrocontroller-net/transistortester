###############################################################################
# Makefile for the project TransistorTester
###############################################################################

## General Flags
MKFILES = Makefile
CC = avr-gcc

CPP = avr-g++

CFLAGS = -Wall

include Makefile.avr

# Select your programmer type, speed and port, if you wish to use avrdude.
# setting for DIAMEX ALL_AVR, Atmel AVRISP-mkII
#PROGRAMMER=avrisp2
#BitClock=5.0
#PORT=usb
# setting for USBasp
#PROGRAMMER=usbasp
#BitClock=20
#PORT=usb
# setting for ARDUINO MEGA, requires bootloader
#PROGRAMMER=wiring
#PORT = /dev/ttyACM0
#BitClock=5.0
#AVRDUDE_BAUD = -b 115200 -D
# setting for ARDUINO MEGA, requires bootloader
PROGRAMMER=arduino

ifndef PARTNO
PARTNO = m328p
endif

ifndef PORT
PORT = /dev/ttyACM0
endif


BitClock=5.0
AVRDUDE_BAUD = -b $(BAUD_RATE)
# ********************** end of selectable options

AVRDUDE_CONF = -D
ifdef ETC_CONF
 AVRDUDE_CONF += -C /etc/avrdude.conf
endif

ifdef TWO_STOP_BITS
 ifneq ($(TWO_STOP_BITS),0)
 AVRDUDE_BAUD += -S 
 endif
endif

CFLAGS += -mcall-prologues
# you can set the EFUSE_VAL to 0xfc or to 0x04, if your avrdude mask out the unused bits


CFLAGS += -DBAUD_RATE=$(BAUD_RATE)

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS += $(COMMON)
ifdef UART
 CFLAGS += -DUART=$(UART)
endif
CFLAGS += -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# CFLAGS += -maccumulate-args
CFLAGS += -MD -MP -MT $(*F).o  

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
#LDFLAGS +=  -Wl,--relax,-Map=$(PARTNO)_test.map
LDFLAGS +=  -Wl,-Map=$(PARTNO)_test.map

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature
#HEX_FLASH_FLAGS += -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

# let a F_CPU=  parameter pass as AVR_FREQ=
ifdef F_CPU
AVR_FREQ := $(F_CPU)
endif

ifndef BIGPROGRAM
 BIGPROGRAM = 0
endif

## Objects that must be built in order to link
OBJECTS = main.o wait1000ms.o soft_uarts.o wast_bytes.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 


## Build
#all: $(PARTNO)_test.elf $(PARTNO)_test.hex $(PARTNO)_test.eep $(PARTNO)_test.lst size

FORCE:

########### Compile only Assembler source available 

wait1000ms.o: wait1000ms.S $(MKFILES) FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -DF_CPU=$(AVR_FREQ) -c  -o $@ $<

soft_uarts.o: soft_uarts.S $(MKFILES) FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -DSOFT_UART=0$(SOFT_UART) -DF_CPU=$(AVR_FREQ) \
        -DUART=0$(UART) -DUART_RX=p$(UART_RX) -DUART_TX=p$(UART_TX) -c -o $@  $<

wast_bytes.o: wast_bytes.S $(MKFILES) FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -DBIGPROGRAM=$(BIGPROGRAM) -c  -o $@ $<


########### Compile C source only 
main.o: main.c $(MKFILES) FORCE
	$(CC) $(INCLUDES) $(CFLAGS) -DSOFT_UART=0$(SOFT_UART) -DF_CPU=$(AVR_FREQ) \
        -DUART=0$(UART) -DUART_RX=p$(UART_RX) -DUART_TX=p$(UART_TX) -c -o $@  $<


##Link
$(PARTNO)_test.elf: $(OBJECTS) FORCE
	 @$(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(PARTNO)_test.elf

$(PARTNO)_test.hex: $(PARTNO)_test.elf FORCE
	-avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $(PARTNO)_test.elf $(PARTNO)_test.hex

$(PARTNO)_test.eep: $(PARTNO)_test.elf FORCE
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $(PARTNO)_test.elf $(PARTNO)_test.eep || exit 0

$(PARTNO)_test.lst: $(PARTNO)_test.elf FORCE
	avr-objdump -h -S $(PARTNO)_test.elf > $(PARTNO)_test.lst

size: ${PARTNO}_test.elf FORCE
	@echo
	@echo $(AVR_FREQ) Hz operation with Baudrate $(BAUD_RATE) configured.
	@avr-size -C --mcu=${MCU} ${PARTNO}_test.elf

## Clean target
.PHONY: clean size hexfile
clean:
	-rm -rf $(OBJECTS) *.elf  *.hex *.eep *.lst *.map main.d soft_uart.d soft_uarts.d wait1000ms.d wast_bytes.d
hexfile: size


## Other dependencies


# device programming

# make upload  additionally calls make to compile the software for selected target
upload: $(PARTNO)_test.hex $(PARTNO)_test.eep $(PARTNO)_test.lst size
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -p $(PARTNO) -P $(PORT) -u \
	-U flash:w:./$(PARTNO)_test.hex:a \
	-U eeprom:w:./$(PARTNO)_test.eep:a
program: $(PARTNO)_test.hex
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -p $(PARTNO) -P $(PORT) -u \
	-U flash:w:./$(PARTNO)_test.hex:a \
	-U eeprom:w:./$(PARTNO)_test.eep:a
flash: $(PARTNO)_test.hex
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -p $(PARTNO) -P $(PORT) -u \
	-U flash:w:./$(PARTNO)_test.hex:a 
eeprom: $(PARTNO)_test.eep
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -p $(PARTNO) -P $(PORT) -u \
	-U eeprom:w:./$(PARTNO)_test.eep:a
eeread:
	avrdude -c $(PROGRAMMER) -B $(BitClock) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -p $(PARTNO) -P $(PORT) -u \
	-U eeprom:r:./Read.eep:i
read:
	avrdude -c $(PROGRAMMER) -B $(BitClock)  -p $(PARTNO) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -P $(PORT) -u \
	-U flash:r:./Read.hex:i
verify: $(PARTNO)_test.hex $(PARTNO)_test.eep
	avrdude -D -c $(PROGRAMMER) -B $(BitClock)  -p $(PARTNO) $(AVRDUDE_BAUD) $(AVRDUDE_CONF) -P $(PORT) -u \
	-U flash:v:./$(PARTNO)_test.hex:a \
	-U eeprom:v:./$(PARTNO)_test.eep:a -v
help:
	@echo "Please use short names like m328 for atmega328"
	@echo " "
	@echo "Options:"
	@echo "AVR_FREQ      Operating frequency of AVR target"
	@echo "BAUD_RATE     Baudrate for the serial interface"
	@echo "TWO_STOP_BITS use two stop bits for the serial interface"
	@echo "PORT          set the port for avrdude to something like /dev/ttyUSB0 or /dev/ttyACM0"
	@echo "BIGPROGRAM    Additional programming data for the flash memory"
	@echo "ISP           =1 for download the test program with avrdude"
