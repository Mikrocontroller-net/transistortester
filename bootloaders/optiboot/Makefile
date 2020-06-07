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
# make lilypad ISP=
# make ng ISP=1
# etc...
#
# Edit History
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

# The default behavior is to build using tools that are in the users
# current path variables, but we can also build using an installed
# Arduino user IDE setup, or the Arduino source tree.
# Uncomment this next lines to build within the arduino environment,
# using the arduino-included avrgcc toolset (mac and pc)
# ENV ?= arduino
# ENV ?= arduinodev
# OS ?= macosx
# OS ?= windows

# export symbols to recursive makes (for ISP)

# no defaults


# Build environments
# Start of some ugly makefile-isms to allow optiboot to be built
# in several different environments.  See the README.TXT file for
# details.

# default
fixpath = $(1)
#SUB_MAKE = @$(MAKE) --no-print-directory -p
#SUB_MAKE = @$(MAKE) --no-print-directory --warn-undefined-variables
SUB_MAKE = @$(MAKE) --no-print-directory 

ifdef ENV
 ifeq ($(ENV), arduino)
# For Arduino, we assume that we're connected to the optiboot directory
# included with the arduino distribution, which means that the full set
# of avr-tools are "right up there" in standard places.
# (except that in 1.5.x, there's an additional level of "up")
TESTDIR := $(firstword $(wildcard ../../../tools/*))
  ifeq (,$(TESTDIR))
# Arduino 1.5.x tool location compared to optiboot dir
  TOOLROOT = ../../../../tools
  else
# Arduino 1.0 (and earlier) tool location
  TOOLROOT = ../../../tools
  endif
GCCROOT = $(TOOLROOT)/avr/bin/

  ifeq ($(OS), windows)
# On windows, SOME of the tool paths will need to have backslashes instead
# of forward slashes (because they use windows cmd.exe for execution instead
# of a unix/mingw shell?)  We also have to ensure that a consistent shell
# is used even if a unix shell is installed (ie as part of WINAVR)
fixpath = $(subst /,\,$1)
SHELL = cmd.exe
SH = sh
  endif

 else ifeq ($(ENV), arduinodev)
# Arduino IDE source code environment.  Use the unpacked compilers created
# by the build (you'll need to do "ant build" first.)
  ifeq ($(OS), macosx)
TOOLROOT = ../../../../build/macosx/work/Arduino.app/Contents/Resources/Java/hardware/tools
  endif
  ifeq ($(OS), windows)
TOOLROOT = ../../../../build/windows/work/hardware/tools
  endif

GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

 else
# no known config
GCCROOT =
AVRDUDE_CONF =
 endif
else
# no ENV set
GCCROOT =
AVRDUDE_CONF =
 ifdef ETC_CONF
  AVRDUDE_CONF += -C /etc/avrdude.conf
 endif
endif

STK500 = "C:\Program Files\Atmel\AVR Tools\STK500\Stk500.exe"
STK500-1 = $(STK500) -e -d$(MCU_TARGET) -pf -vf -if$(PROGRAM)_$(TARGET).hex \
           -lFF -LFF -f$(HFUSE)$(LFUSE) -EF8 -ms -q -cUSB -I200kHz -s -wt
STK500-2 = $(STK500) -d$(MCU_TARGET) -ms -q -lCF -LCF -cUSB -I200kHz -s -wt

ifeq ($(OS), windows)
# The real name for Windows7 is "Windows_NT" !!!
# But you should add the GnuWin32_dir path to the Path (environment variable)
GnuWin32_dir = C:\Program Files (x86)\GnuWin32\bin\
# bc, cat, cut, grep, tr, rm and bash is available for Windows with the GnuWin32 project
BC_CMD = bc -q
CAT_CMD = $(GnuWin32_dir)cat
CUT_CMD = $(GnuWin32_dir)cut
GREP_CMD = $(GnuWin32_dir)grep
TR_CMD = tr
RM_CMD = rm
BASH_CMD = $(GnuWin32_dir)bash
else
BC_CMD = bc -q
CAT_CMD = cat
CUT_CMD = cut
GREP_CMD = grep
TR_CMD = tr
RM_CMD = rm
BASH_CMD = bash
endif

# echo should be available for Linux and Windows
ECHO_CMD = echo
#
# End of build environment code.

OPTIMIZE = -Os -fno-split-wide-types -mrelax

DEFS       = 

ifndef BAUD_RATE
BAUD_RATE := 115200
endif

BAUD_RATE_CMD = -DBAUD_RATE=$(BAUD_RATE)

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
LED_START_FLASHES = 3
LED_START_FLASHES_CMD = -DLED_START_FLASHES=3
endif



# BIG_BOOT: Include extra features, up to 1K.
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
all: help
include Makefile.m328
include Makefile.m16
include Makefile.m169
include Makefile.m640
include Makefile.m644
include Makefile.m645
include Makefile.at90
include Makefile.tiny

#ifdef FLASH_SIZE
#if ("$(origin MCU_TARGET)" != "undefined") || ("$(origin MCU_TARGET)" != "file") || ("$(origin MCU_TARGET)" != "environment")
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


SIZE           = $(GCCROOT)avr-size

# We can not compute the number of boot pages, because the object file must be created first!
#BOOT_PAGES = $(shell $(ECHO_CMD) "pg_anz=(`$(SIZE) -C $(PROGRAM).o | $(GREP_CMD) "Program:" | $(CUT_CMD) -c 10-16`/$(BOOT_PAGE_LEN) +1); pg_anz + (pg_anz==3) + (pg_anz==5)*3 + (pg_anz==6)*2 + (pg_anz == 7)" | $(BC_CMD))
#define BOOT_PAGES =
# @$(ECHO_CMD) "pg_anz=(`$(SIZE) -C $(PROGRAM).o | $(GREP_CMD) "Program:" | $(CUT_CMD) -c 10-16`/$(BOOT_PAGE_LEN) +1); pg_anz + (pg_anz==3) + (pg_anz==5)*3 + (pg_anz==6)*2 + (pg_anz == 7)" | $(BC_CMD)
#endef

#define BOOT_SZ =
# @$(ECHO_CMD) "pg_anz=(`$(SIZE) -C $(PROGRAM).elf | $(GREP_CMD) "Program:" | $(CUT_CMD) -c 10-16`/$(BOOT_PAGE_LEN) +1); 0 + (pg_anz<5) + (pg_anz<3) + (pg_anz<2)" | $(BC_CMD)
#endef

LDSECTIONS  = -Wl,--section-start=.version=0x$(shell $(ECHO_CMD) "obase=16;$(FLASH_SIZE)-2" | $(BC_CMD))

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS) -fno-diagnostics-show-caret
override LDFLAGS       = -Wl,--relax -nostartfiles -nostdlib

OBJCOPY        = $(GCCROOT)avr-objcopy
OBJDUMP        = $(call fixpath,$(GCCROOT)avr-objdump)

AUTO_BAUD = $(shell $(ECHO_CMD) "$(BAUD_RATE) / 100" | $(BC_CMD))

#---------------------------------------------------------------------------
#
# Generic build instructions
#

FORCE:

export ECHO_CMD BC_CMD TR_CMD
.PHONY: baudck_prepare
baudcheck: baudck_prepare FORCE
	@$(BASH_CMD) ./baudcheck.tmp.sh
baudck_prepare: 
	@$(CC) $(CFLAGS) $(COMMON_OPTIONS) \
	 -DUART=0$(UART) -DSOFT_UART=0$(SOFT_UART) -DUART_RX=p$(UART_RX) -DUART_TX=p$(UART_TX) \
	 -DF_CPU=$(AVR_FREQ) -E -o baudcheck.tmp.sh baudcheck.S

$(PROGRAM).o: $(PROGRAM).$(SOURCE_TYPE) FORCE
ifeq ($(VIRTUAL_BOOT_PARTITION), 1)
 ifdef save_vect_num
	$(CC) $(CFLAGS) $(COMMON_OPTIONS) -DVIRTUAL_BOOT_PARTITION -Dsave_vect_num=$(save_vect_num) -DLED=p$(LED) \
        -DUART=0$(UART) -DSOFT_UART=0$(SOFT_UART) -DUART_RX=p$(UART_RX) -DUART_TX=p$(UART_TX) -DF_CPU=$(AVR_FREQ) \
        -DHFUSE=hex$(HFUSE) -DLFUSE=hex$(LFUSE) \
        -DBOOT_PAGE_LEN=$(BOOT_PAGE_LEN) -c -o $@ $<
 else
	$(CC) $(CFLAGS) $(COMMON_OPTIONS) -DVIRTUAL_BOOT_PARTITION -DLED=p$(LED) -DUART=0$(UART) -DSOFT_UART=0$(SOFT_UART) \
        -DUART_RX=p$(UART_RX) -DUART_TX=p$(UART_TX) -DF_CPU=$(AVR_FREQ) \
        -DHFUSE=hex$(HFUSE) -DLFUSE=hex$(LFUSE) \
        -DBOOT_PAGE_LEN=$(BOOT_PAGE_LEN) -c -o $@ $<
 endif 
else
	$(CC) $(CFLAGS) $(COMMON_OPTIONS) -DLED=p$(LED) -DUART=0$(UART) -DSOFT_UART=0$(SOFT_UART) \
        -DUART_RX=p$(UART_RX) -DUART_TX=p$(UART_TX) -DF_CPU=$(AVR_FREQ) \
        -DHFUSE=hex$(HFUSE) -DLFUSE=hex$(LFUSE) \
        -DBOOT_PAGE_LEN=$(BOOT_PAGE_LEN) -c -o $@ $<
endif

 export LFUSE HFUSE EFUSE TARGET BOOT_PAGE_LEN FLASH_SIZE VIRTUAL_BOOT_PARTITION MCU_TARGET PROGRAM CAT_CMD AVRDUDE_CONF GCCROOT AVR_FREQ
hex_file: hex_copy
ifdef ISP
	$(SUB_MAKE) --warn-undefined-variables -r -f Makefile.isp do_isp
else
	
	@$(ECHO_CMD) "Fuse Setting: LFUSE=0x$(LFUSE), HFUSE=0x$(HFUSE), EFUSE=0x$(EFUSE)"
endif

isp-stk500: $(PROGRAM).hex
	$(STK500-1)
	$(STK500-2)

#  we can determine the size of the loader with optiboot.o for Assembler source input,
#  but for C source we need to do some optimizing  with the binder call (.elf).
#  This call is done without the relolation of the .text section (instruction code).
#  After computing the required size (and the possible relocation address),
#  This x.elf is removed later.
$(PROGRAM)x.elf: $(PROGRAM).o
	$(CC) $(CFLAGS) $(COMMON_OPTIONS) $(LDSECTIONS) $(LDFLAGS)  -o $(PROGRAM)x.elf $< $(LIBS)

# BootPages.dat fetch the actual boot loader size from a interim $(PROGRAM)x.elf file, not from the
# $(PROGRAM).o file. The ld program can do some optimizing for code generated from a C-source.
# For code generated with the assembler there is no size difference between .o and .elf !
# If 3 pages are required, number of pages is rounded to 4.
# If more than 4 pages are required, the number of pages is rounded to 8.
# Above 8 pages there is no round up. 
# The ATtiny84 has no hardware feature for the bootloader like the ATmega family.
# Therefore the "round up" is not required for the ATtiny84, but the BOOT_PAGE_LEN is 64 for
# this processor, so the number of pages is more than 7 pages for the actual size of optiboot.
BootPages.dat: $(PROGRAM)x.elf
ifeq ($(VIRTUAL_BOOT_PARTITION), 1)
	@$(ECHO_CMD) "(($(shell $(SIZE) -C $(PROGRAM)x.elf | $(GREP_CMD) "Program:" | $(CUT_CMD) -c 10-16)/$(FLASH_PAGE_SIZE)/$(FLASH_ERASE_CNT) +1)*$(FLASH_ERASE_CNT))" | $(BC_CMD) > BootPages.dat
else
	@$(ECHO_CMD) "pg_anz=($(shell $(SIZE) -C $(PROGRAM)x.elf | $(GREP_CMD) "Program:" | $(CUT_CMD) -c 10-16)/$(BOOT_PAGE_LEN) +1); pg_anz + (pg_anz==3 ) + (pg_anz==5)*3 + (pg_anz==6)*2 + (pg_anz == 7)" | $(BC_CMD) > BootPages.dat
endif

# With the file BootPages.dat and the $(BOOT_PAGE_LEN) or $(FLASH_PAGE_SIZE) we can compute the 
# Start Address of the bootloader depending on the $(FLASH_SIZE) 
BL_StartAdr.dat:  BootPages.dat
ifeq ($(VIRTUAL_BOOT_PARTITION), 1)
	@$(ECHO_CMD) "obase=16;$(FLASH_SIZE) - ($(shell $(CAT_CMD) BootPages.dat)*$(FLASH_PAGE_SIZE))" | $(BC_CMD) > BL_StartAdr.dat
else
	@$(ECHO_CMD) "obase=16;$(FLASH_SIZE) - ($(shell $(CAT_CMD) BootPages.dat)*$(BOOT_PAGE_LEN))" | $(BC_CMD) > BL_StartAdr.dat
endif
	@$(RM_CMD) $(PROGRAM)x.elf

# Generate the final $(PROGRAM).elf file at the right Start Address,
# which is the base to generate the $(PROGRAM)_$(TARGET).hex and $(PROGRAM)_$(TARGET).lst files.
$(PROGRAM).elf:  $(PROGRAM).o baudcheck  BL_StartAdr.dat
	- @$(ECHO_CMD) "###########################################"
	- @$(ECHO_CMD) "Boot Loader start address: 0x$(shell $(CAT_CMD) BL_StartAdr.dat) = $(shell $(CAT_CMD) base_cmd BL_StartAdr.dat | $(BC_CMD))"
	- @$(ECHO_CMD) "###########################################"
	@$(CC) $(CFLAGS) $(COMMON_OPTIONS) $(LDSECTIONS) \
        -Wl,--section-start=.text=0x$(shell $(CAT_CMD) BL_StartAdr.dat) $(LDFLAGS) -o $(PROGRAM).elf $< $(LIBS)
	- @$(ECHO_CMD)
ifeq ($(AUTO_BAUD), 0)
 ifeq ((SUPPORT_EEPROM), 0)
	- @$(ECHO_CMD) $(AVR_FREQ) Hz operation with Auto-Baudrate configured.
 else
	- @$(ECHO_CMD) $(AVR_FREQ) Hz operation with Auto-Baudrate and EEprom support configured.
 endif
else
 ifeq ((SUPPORT_EEPROM), 0)
	- @$(ECHO_CMD) $(AVR_FREQ) Hz operation with Baudrate $(BAUD_RATE) configured.
 else
	- @$(ECHO_CMD) $(AVR_FREQ) Hz operation with Baudrate $(BAUD_RATE) and EEprom support configured.
 endif
endif
	- $(SIZE) $(PROGRAM).elf

# remove all generated files
.PHONY: clean steril hex_copy hex_file help help2
clean:
	@$(RM_CMD) -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *.tmp.sh *.dat
steril:
	@$(RM_CMD) -rf *.o *.elf *.map *.sym *.tmp.sh *.dat *.ppp $(PROGRAM).hex 

# With the Bootpages.dat file we can set the required BOOTSZ1 and BOOTSZ0 bits, which are
# combined to BOOTSZ (0 for eight pages, 1 for four pages, 2 for two pages and 3 for one page)
# This BOOTSZ.dat is taken by the Makefile.isp file to correct the HFUSE or EFUSE.
# With option VIRTUAL_BOOT_PARTITION BOOTSZ.dat is allways set to 3
BOOTSZ.dat: $(PROGRAM).elf BootPages.dat
ifeq ($(VIRTUAL_BOOT_PARTITION), 1)
	- @$(ECHO_CMD) "Requires $(shell $(CAT_CMD) BootPages.dat) Flash Pages, $(FLASH_PAGE_SIZE) Bytes each"
	- @$(ECHO_CMD) "3" > BOOTSZ.dat
else
	- @$(ECHO_CMD) "Requires $(shell $(CAT_CMD) BootPages.dat) Boot Pages, $(BOOT_PAGE_LEN) Bytes each"
	- @$(SIZE) -C $(PROGRAM).elf | $(GREP_CMD) "Program:" | $(CUT_CMD) -c 10-16 | $(TR_CMD) -d "\n\r " > opti_size.dat
	- @$(ECHO_CMD) /$(BOOT_PAGE_LEN)+1 >> opti_size.dat
	- @$(CAT_CMD) opti_size.dat | $(BC_CMD) > pg_anz.dat
	- @$(ECHO_CMD) "0+(`$(CAT_CMD) pg_anz.dat`<5)+(`$(CAT_CMD) pg_anz.dat`<3)+(`$(CAT_CMD) pg_anz.dat`<2)" | $(BC_CMD) > BOOTSZ.dat
	- @$(ECHO_CMD) "BOOTSZ=`$(CAT_CMD) BOOTSZ.dat`"
endif

# copy the  .lst and .hex files to files which identify the target
# add some options to the end of the .lst file as comment
hex_copy: $(PROGRAM).lst $(PROGRAM)_$(TARGET).hex
	- @$(ECHO_CMD) "; " >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; FORCE_WATCHDOG=$(FORCE_WATCHDOG)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; LED_START_FLASHES=$(LED_START_FLASHES)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; LED_DATA_FLASH=$(LED_DATA_FLASH)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; LED=$(LED)" >> $(PROGRAM)_$(TARGET).lst
ifdef SOFT_UART
	- @$(ECHO_CMD) "; SOFT_UART=$(SOFT_UART)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; UART_RX=$(UART_RX)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; UART_TX=$(UART_TX)" >> $(PROGRAM)_$(TARGET).lst
endif
	- @$(ECHO_CMD) "; UART=$(UART)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; SOURCE_TYPE=$(SOURCE_TYPE)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; SUPPORT_EEPROM=$(SUPPORT_EEPROM)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; MCU_TARGET = $(MCU_TARGET)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; AVR_FREQ= $(AVR_FREQ)" >> $(PROGRAM)_$(TARGET).lst
	- @$(ECHO_CMD) "; BAUD_RATE=$(BAUD_RATE)" >> $(PROGRAM)_$(TARGET).lst

help:
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
	@$(ECHO_CMD) "Type make help2 for additional options!"
help2:
	@$(ECHO_CMD) "Additional options for building a optiboot:"
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



# allways generate a $(PROGRAM)_$(TARGET).lst file, if a .hex file is generated from $(PROGRAM).elf
%.lst: $(PROGRAM).elf BOOTSZ.dat
	$(OBJDUMP) -h -S $< > $(PROGRAM)_$(TARGET).lst

%.hex: $(PROGRAM).elf BOOTSZ.dat
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $< $(PROGRAM)_$(TARGET).hex
# you can ignore the .version section with the following command
#	$(OBJCOPY) -j .text -j .data -R .version -O ihex $< $(PROGRAM).hex

%.srec: $(PROGRAM).elf BOOTSZ.dat
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O srec $< $@

%.bin: $(PROGRAM).elf BOOTSZ.dat
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O binary $< $@
