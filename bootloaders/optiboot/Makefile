# Makefile for ATmegaBOOT
# E.Lins, 18.7.2005
# $Id$
#
# Instructions
#
# To make bootloader .hex file:
# make diecimila
# make lilypad
# make ng
# etc...
#
# To burn bootloader .hex file:
# make diecimila ISP=1
# make lilypad ISP=1
# make ng ISP=1
# etc...
#
# Edit History
# 20200619: kubi48: The Name is changes in Aoptiboot now.
#                   The terminal journal if build process can optionally
#                   formatted with colors.
#                   To prevent the warning message in the .h file,
#                   the automatic choose of a default LED pin for the 
#                   Life Signal of the bootloader is now set in the
#                   auto_led.sh script, called by the script build_hex.sh .
#                   For easier handling of the idifferent AVR controllers,
#                   the pin equal models are summarized to one family name.
# 20200615: kubi48: the Makefile has been reorganized, that the main task
#                   to build a new .hex file ifor the selected target with
#                   the choosed parameters is now done in the bash script
#                   file build_hex.sh . This will make the Makefile more
#                   readable and the script should run faster.
#                   The build_hex.sh script will also call the script
#                   program_target.sh for the handling with avrdude.
#                   So the Makefile.isp is no longer required.
#                   The build of all 79 sample configurations takes now
#                   135 seconds at my Windows10 laptop, a little faster
#                   as the previous version. But also the Linux system 
#                   runs faster (now 6s, before about 10s),
#                   the job runs faster in a Terminal window compared
#                   to a console run.
#                   Modification of the build process and improvement
#                   of the screen logging is now easier to do.
# 202006xx: kubi48: the Makefile has been tested at a Windows7 system to
#                   build the optiboot hex file. If you will use the makedefault
#                   or makeall batch files, you must rename them with the
#                   .bat extension or call them with bash (bash makedefault).
#                   At my laptop systems a Windows7 call for makedefault
#                   takes about 25 seconds. With Linux Mint 19.3 the same action
#                   takes only about 2 seconds. The makeall call with the build
#                   of 79 bootloaders takes about 280 seconds with Windows7
#                   and only 18 seconds with Linux at the same Laptop with the
#                   same SDD.
#                   Please note, that some cofiguration details are added at
#                   the end of the .lst file. you can read these with the
#                   tail command (for examplei: tail -12 optiboot_atmega328.lst).
# 201712xx: kubi48: all subjects with _isp extensions are removed,
#                   please use the make call with ISP=1 in the command line.
#                   Many additional processors are added to the makefiles.
#                   Some additional Makefile parts like Makefile.m645, Makefile.tiny
#                   and Makefile.at90 are added also.
#                   The Bootloader Start Address (BOOTRST) is now set matching to 
#                   the option VIRTUAL_BOOT_PARTITION .
# 201605xx: kubi48: Makefile is changed for the assembler version of optiboot.
#                   All sample boards now fit to a 512 byte boot segment.
#                   You can now use the SUPPORT_EEPROM option with all configurations.
#                   The makeall script is changed for this feature.
#		    The size of the generated bootloader code is determined
#		    and the location of the bootloader and fuse options are set 
#		    set automatically by the Makefile.
#		    The command line tools bc, cat, cut, echo and  grep are
#		    required to run this automatic function.
#		    You can still select the C-source of optiboot with option
#                   C-SOURCE=1 .
# 201406xx: WestfW: More Makefile restructuring.
#                   Split off Makefile.1284, Makefile.extras, Makefile.custom
#                   So that in theory, the main Makefile contains only the
#                   official platforms, and does not need to be modified to
#                   add "less supported" chips and boards.
# 201303xx: WestfW: Major Makefile restructuring.
#                   Allows options on Make command line "make xx LED=B3"
#                   (see also pin_defs.h)
#                   Divide into "chip" targets and "board" targets.
#                   Most boards are (recursive) board targets with options.
#                   Move isp target to separate makefile (fixes m8 EFUSE)
#                   Some (many) targets will now be rebuilt when not
#                     strictly necessary, so that options will be included.
#                     (any "make" with options will always compile.)
#                   Set many variables with ?= so they can be overridden
#                   Use arduinoISP settings as default for ISP targets
#
#
# * Copyright 2013-2015 by Bill Westfield.  Part of Optiboot.
# * This software is licensed under version 2 of the Gnu Public Licence.
# * See optiboot.c for details.

#----------------------------------------------------------------------
#
# program name should not be changed...
override PROGRAM    = optiboot

# export symbols to recursive makes (for ISP)

# no defaults


# Build environments
# Start of some ugly makefile-isms to allow optiboot to be built
# in several different environments.  See the README.TXT file for
# details.

# default
fixpath = $(1)

# no ENV set
GCCROOT =
AVRDUDE_CONF =
ifdef ETC_CONF
  AVRDUDE_CONF += -C /etc/avrdude.conf
else 
  AVRDUDE_CONF += -C ./avrdude.conf
endif

BC_CMD = bc
CAT_CMD = cat
CUT_CMD = cut
GREP_CMD = grep
TR_CMD = tr
RM_CMD = rm
BASH_CMD = bash
ECHO_CMD = echo
MAKE = make
#
# End of build environment code.

OPTIMIZE = -Os -fno-split-wide-types -mrelax

DEFS       = 

ifndef BAUD_RATE
BAUD_RATE := 115200
endif

BAUD_RATE_CMD = -DBAUD_RATE=$(BAUD_RATE)

# Allow some shorter Names
ifndef SUPPORT_EEPROM
 ifdef SUPPORT_EE
  SUPPORT_EEPROM := $(SUPPORT_EE)
 endif
endif
ifndef LED_START_FLASHES
 ifdef LED_SF
  LED_START_FLASHES := $(LED_SF)
 endif
endif
ifndef LED_DATA_FLASH
 ifdef LED_DF
  LED_DATA_FLASH := $(LED_DF)
 endif
endif
ifndef VIRTUAL_BOOT_PARTITION
 ifdef VIRTUAL_BP
  VIRTUAL_BOOT_PARTITION := $(VIRTUAL_BP)
 endif
endif

COMMON_OPTIONS = $(BAUD_RATE_CMD) $(LED_START_FLASHES_CMD) $(BIGBOOT_CMD)
COMMON_OPTIONS += $(LED_DATA_FLASH_CMD)
ifdef TEST_OUTPUT
 COMMON_OPTIONS += -DTEST_OUTPUT=1
endif

ifdef INVERSE_UART
 COMMON_OPTIONS += -DINVERSE_UART=1
else
 ifdef INVERS_UART
  COMMON_OPTIONS += -DINVERSE_UART=1
 endif
endif

#
# Make command-line Options.
# Permit commands like "make atmega328 LED_START_FLASHES=10" to pass the
# appropriate parameters ("-DLED_START_FLASHES=10") to gcc
#
#default source type is Assembler (S).
# you can select a C source type with option "C_SOURCE=1"
SOURCE_TYPE = S
ifdef C_SOURCE
 ifneq ($(C_SOURCE),0)
  SOURCE_TYPE = c
 else
  ifndef SUPPORT_EEPROM
   SUPPORT_EEPROM := 1
  endif
 endif
else
 ifndef SUPPORT_EEPROM
  SUPPORT_EEPROM := 1
 endif
endif

# let a F_CPU=  parameter pass as AVR_FREQ=
ifdef F_CPU
AVR_FREQ := $(F_CPU)
endif

ifdef LED_START_FLASHES
LED_START_FLASHES_CMD = -DLED_START_FLASHES=$(LED_START_FLASHES)
else
# 3 flashes of LED are default
LED_START_FLASHES = 3
LED_START_FLASHES_CMD = -DLED_START_FLASHES=3
endif



# BIG_BOOT: Include extra flash space, up to 1K.
ifdef BIGBOOT
BIGBOOT_CMD = -DBIGBOOT=$(BIGBOOT)
endif


ifdef LED_DATA_FLASH
LED_DATA_FLASH_CMD = -DLED_DATA_FLASH=$(LED_DATA_FLASH)
endif

ifdef SUPPORT_EEPROM
COMMON_OPTIONS += -DSUPPORT_EEPROM=$(SUPPORT_EEPROM)
endif

ifdef FORCE_WATCHDOG
COMMON_OPTIONS += -DFORCE_WATCHDOG
endif

ifdef FORCE_RSTDISBL
 ifneq ($(FORCE_RSTDISBL), 0)
  COMMON_OPTIONS += -DFORCE_RSTDISBL
 endif
endif

ifdef TIMEOUT_MS
COMMON_OPTIONS += -DTIMEOUT_MS=$(TIMEOUT_MS)
endif

ifdef OSCCAL_CORR
COMMON_OPTIONS += -DOSCCAL_CORR=$(OSCCAL_CORR)
endif
#

#
# platforms support EEPROM and large bootloaders need the eeprom functions that
# are defined in libc, even though we explicity remove it with -nostdlib because
# of the space-savings.
LIBS       =  -lc

CC         = $(GCCROOT)avr-gcc
#

#---------------------------------------------------------------------------
# "Chip-level Platform" targets.
# A "Chip-level Platform" compiles for a particular chip, but probably does
# not have "standard" values for things like clock speed, LED pin, etc.
# Makes for chip-level platforms should usually explicitly define their
# options like: "make atmega1285 AVR_FREQ=16000000 LED=D0"
#---------------------------------------------------------------------------
#
# Note about fuses:
# the efuse should really be 0xf8; since, however, only the lower
# three bits of that byte are used on the atmega168, avrdude gets
# confused if you specify 1's for the higher bits, see:
# http://tinker.it/now/2007/02/24/the-tale-of-avrdude-atmega168-and-extended-bits-fuses/
#
# similarly, the lock bits should be 0xff instead of 0x3f (to
# unlock the bootloader section) and 0xcf instead of 0x2f (to
# lock it), but since the high two bits of the lock byte are
# unused, avrdude would get confused.
#---------------------------------------------------------------------------
#


#
# Include Makefile parts for different platforms, atmega168 is the first target in Makefile.m328
all: help help2
include Makefile.m328
include Makefile.m16
include Makefile.m169
include Makefile.m640
include Makefile.m644
include Makefile.m645
include Makefile.at90
include Makefile.tiny

ifneq "$(origin MCU_TARGET)" "undefined" 
 ifneq "$(origin MCU_TARGET)" "file" 
  ifneq "$(origin MCU_TARGET)" "environment"
   $(error The origin of MCU_TARGET is '$(origin MCU_TARGET)' instead of 'file' or 'environment')
  endif
 endif
endif
ifneq "$(origin FLASH_SIZE)" "undefined" 
 ifneq "$(origin FLASH_SIZE)" "override"
  ifneq "$(origin FLASH_SIZE)" "environment"
   $(error The origin of FLASH_SIZE is '$(origin FLASH_SIZE)' instead of 'override' or 'environment')
  endif
 endif
endif
ifneq "$(origin FLASH_PAGE_SIZE)" "undefined"
 ifneq "$(origin FLASH_PAGE_SIZE)" "override"
  ifneq "$(origin FLASH_PAGE_SIZE)" "environment"
   $(error The origin of FLASH_PAGE_SIZE is '$(origin FLASH_PAGE_SIZE)' instead of 'override' or 'environment')
  endif
 endif
endif
ifneq "$(origin BOOT_PAGE_LEN)" "undefined"
 ifneq "$(origin BOOT_PAGE_LEN)" "override"
  ifneq "$(origin BOOT_PAGE_LEN)" "environment"
   $(error The origin of BOOT_PAGE_LEN is '$(origin BOOT_PAGE_LEN)' instead of 'override' or 'environment')
  endif
 endif
endif

ifndef FLASH_ERASE_CNT
 FLASH_ERASE_CNT = 1
endif

LDSECTIONS  = -Wl,--section-start=.version=0x$(shell $(ECHO_CMD) "obase=16;$(FLASH_SIZE)-2" | $(BC_CMD))

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -fno-diagnostics-show-caret
override LDFLAGS       = -Wl,--relax -nostartfiles -nostdlib

#---------------------------------------------------------------------------
#
# Generic build instructions
#
.PHONY: clean steril hex_file help help2 
FORCE:

 export LFUSE HFUSE EFUSE TARGET BOOT_PAGE_LEN FLASH_SIZE FLASH_PAGE_SIZE
 export VIRTUAL_BOOT_PARTITION BIGBOOT ISP
 export LED LED_START_FLASHES LED_DATA_FLASH
 export AVR_FREQ BAUD_RATE UART_RX UART_TX OSCCAL_CORR SOFT_UART
 export MCU_TARGET PROGRAM AVRDUDE_CONF FORCE_RSTDISBL TIMEOUT_MS

hex_file: 
	@bash build_hex.sh


# remove all generated files
clean:
	@$(RM_CMD) -rf *.o *.elf *.lst *.log *.map *.sym *.lss *.eep *.srec *.bin *.hex *.ppp *.tmp.sh *.dat
steril:
	@$(RM_CMD) -rf *.o *.elf *.map *.sym *.tmp.sh *.dat *.ppp $(PROGRAM).hex 

help:
	@$(ECHO_CMD) "This is the build in help text for the optiboot Makefile (make help)"
	@$(ECHO_CMD) "Important options to build optiboot:"
	@$(ECHO_CMD) "AVR_FREQ       specifies the operating frequency (Hz) of the target AVR."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "BAUD_RATE      specifies the baud rate of the serial interface."
	@$(ECHO_CMD) "               A value less than 100 select the baud rate measurement,"
	@$(ECHO_CMD) "               less than 60 with a simpler method and less than 40"
	@$(ECHO_CMD) "               additionally without a time limit."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART           specifies the hardware UART number (0-3 if existing)."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "LED_START_FLASHES specifies the number of times to flicker the LED"
	@$(ECHO_CMD) "               before the serial communication starts. 0 means no flicker."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "LED_DATA_FLASH let the LED light during serial character input wait."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "LED            specifies with two characters the port and bit number."
	@$(ECHO_CMD) "               LED=B5 will select bit 5 of port B."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "SUPPORT_EEPROM select the EEprom support, if set to nonzero value"
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "ISP            will start avrdude for download of optiboot if set to 1."
	@$(ECHO_CMD) "               2 will start a verify with avrdude,"
	@$(ECHO_CMD) "               3 will read the complete flash to Read_<AVR-ID>.hex,"
	@$(ECHO_CMD) "               4 will read the complete EEprom to Read_<AVR-ID>.eep."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "You must allways specify a target AVR processor (AVR-ID) to build"
	@$(ECHO_CMD) "a proper optiboot bootloader with any of the available options."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "Example:"
	@$(ECHO_CMD) "make atmega328p LED_START_FLASHES=0 LED_DATA_FLASH=1"
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "would deselect the binking LED at the bootloader start and"
	@$(ECHO_CMD) "would use a bright LED when waiting for Rx data."
	@$(ECHO_CMD) "Type make help2 for additional options!"
	@$(ECHO_CMD) " "
help2:
	@$(ECHO_CMD) "Additional options for building a optiboot (make help2):"
	@$(ECHO_CMD) "SOFT_UART      select a software UART solution instead of the hardware UART."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART_RX        specifies the port for software UART input."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART_TX        specifies the port for software UART output."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "C_SOURCE       set to 1 for C source instead of assembler version."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "BIGBOOT        select additional memory for the bootloader to test"
	@$(ECHO_CMD) "               the automatic selection of start address and the number"
	@$(ECHO_CMD) "               of boot pages."
	@$(ECHO_CMD) "VIRTUAL_BOOT_PARTITION changes two interrupt vector jmp addresses"
	@$(ECHO_CMD) "               for starting the bootloader with the reset vector."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "TIMEOUT_MS     select a time limit for receiving boot data in ms units."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "OSCCAL_CORR    specifies a correction value for the OSCCAL Byte for the"
	@$(ECHO_CMD) "               internal RC clock generator."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "FORCE_RSTDISBL enables setting of the RSTDISBL bit to 0."
	@$(ECHO_CMD) "               Otherwise compiling is aborted."
pl:
	@bash ./list_processors.sh
