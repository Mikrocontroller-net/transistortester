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
# 20200706: kubi48: Now you can use 2 bits, 4 bits and 9 bits are time source
#                   for the baud rate determination.
#                   You can select to measure the time source with a full CPU clock rate
#                   or with a factor 8 slower clock rate.
#                   For the Software UART you can use 10 different methods to
#                   compute and set delay loop parameters.
#                   So you can choose a total of 80 different settings for the
#                   Software UART solution.
# 20200625: kubi48: The default setting for LED pin, and also the
#                   TX and RX pin for processors without the Hardware UART,
#                   are now defined in the avr_pins/AVR_FAMILY.pins
#                   file at a unused column.
#                   The setting will be listed with the show_led_pin.sh,
#                   show_rx_pin.sh and show_tx_pin.sh scripts.
#                   Because the setting for Makefile targets is more simple
#                   now, the sub-Makefiles are removed and all entries
#                   are now in the common Makefile.
# 20200619: kubi48: The terminal journal of build process can optionally
#                   formatted with colors (WITH_COLORS=1).
#                   To prevent the warning message in the .h file,
#                   the automatic choose of a default LED pin for the 
#                   Life Signal of the bootloader is now set in the
#                   auto_led.sh script, called by the script build_hex.sh .
#                   For easier handling of the different AVR controllers,
#                   the pin equal models are summarized to one family name.
#                   The alternate Pin functions of the selected pin are
#                   shown at the terminal.
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


DEFS       = 

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
 ifndef AVR_FREQ
  AVR_FREQ := $(F_CPU)
 endif
endif


#

CC         = $(GCCROOT)avr-gcc
#

#---------------------------------------------------------------------------
# "Chip-level Platform" targets.
# A "Chip-level Platform" compiles for a particular chip, but probably does
# not have "standard" values for things like clock speed, LED pin, etc.
# Makes for chip-level platforms should usually explicitly define their
# options like: "make atmega1285 AVR_FREQ=16000000 LED=D0"
# * Copyright 2013-2015 by Bill Westfield.  Part of Optiboot.
# * reorganized 2017, 2020 by K.-H. Kuebbeler
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

# Diecimila, Duemilanove with m168, and NG use identical bootloaders
# Call it "atmega168" for generality and clarity, keep "diecimila" for
# backward compatibility of makefile
#
################################################
# generic atmega168p
atmega168: TARGET := atmega168
atmega168: MCU_TARGET := atmega168
atmega168: hex_file

################################################
#  atmega168p
atmega168p: TARGET := atmega168p
atmega168p: MCU_TARGET := atmega168p
atmega168p: hex_file

################################################
#  atmega168a
atmega168a: TARGET := atmega168a
atmega168a: MCU_TARGET := atmega168a
atmega168a: hex_file

################################################
# generic atmega328
atmega328: TARGET := atmega328
atmega328: MCU_TARGET := atmega328
atmega328: hex_file

################################################
# atmega328p
atmega328p: TARGET := atmega328p
atmega328p: MCU_TARGET := atmega328p
atmega328p: hex_file

#
################################################
# generic atmega88
atmega88: TARGET := atmega88
atmega88: MCU_TARGET := atmega88
atmega88: hex_file

#
################################################
# atmega88p
atmega88p: TARGET := atmega88p
atmega88p: MCU_TARGET := atmega88p
atmega88p: hex_file


#---------------------------------------------------------------------------
# "Board-level Platform" targets.
# A "Board-level Platform" implies a manufactured platform with a particular
# AVR_FREQ, LED, and so on.  Parameters are not particularly changable from
# the "make" command line.
# Most of the board-level platform builds should envoke make recursively
#  appropriate specific options
#---------------------------------------------------------------------------
# 20MHz clocked platforms
#
# These are capable of 230400 baud, or 115200 baud on PC (Arduino Avrdude issue)
#

pro20: TARGET := pro_20Mhz
pro20: MCU_TARGET := atmega168
pro20: LED_START_FLASHES := 3
# 4.3V brownout (for max speed!)
pro20: HFUSE := DC
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro20: LFUSE := F7
## 512 byte boot
pro20: EFUSE := FC
pro20: AVR_FREQ := 20000000
pro20: hex_file


# 16MHz clocked platforms
# These are capable of 230400 baud, or 115200 baud on PC (Arduino Avrdude issue)
#

pro16: TARGET := pro_16MHz
pro16: MCU_TARGET := atmega168
pro16: LED_START_FLASHES := 3
# 2.7V brownout
pro16: HFUSE := DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro16: LFUSE := F7
# 512 byte boot
pro16: EFUSE := FC
pro16: AVR_FREQ := 16000000
pro16: hex_file


atmega328-Mini: TARGET := atmega328-Mini
atmega328-Mini: MCU_TARGET := atmega328p
atmega328-Mini: LED_START_FLASHES := 0
# 512 byte boot, SPIEN
atmega328-Mini: HFUSE := DE
# Full swing xtal (16MHz) 16k CK/14CK+65ms
atmega328-Mini: LFUSE := F7
# Brwon Out 2.7V
atmega328-Mini: EFUSE := 05
atmega328-Mini: AVR_FREQ := 16000000
atmega328-Mini: hex_file


pro8: TARGET := pro_8MHz
pro8: MCU_TARGET := atmega168p
pro8: LED_START_FLASHES := 3
# 2.7V brownout
pro8: HFUSE := DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
pro8: LFUSE := C6
# 512 byte boot
pro8: EFUSE := FC
pro8: AVR_FREQ := 8000000
pro8: hex_file


diecimila: TARGET := diecimila
diecimila: MCU_TARGET := atmega168p
diecimila: LED_START_FLASHES := 3
# 2.7V brownout
diecimila: HFUSE := DD
# Low power xtal (16MHz) 16KCK/14CK+65ms
diecimila: LFUSE := F7
# 512 byte boot
diecimila: EFUSE := FC
diecimila: hex_file


# 8MHz clocked platforms
#
# These are capable of 115200 baud
# Note that "new" Arduinos with an AVR as USB/Serial converter will NOT work
# with an 8MHz target Arduino.  The bitrate errors are in opposite directions,
# and total too large a number.
#

lilypad: TARGET := lilypad
lilypad: MCU_TARGET := atmega168
lilypad: LED_START_FLASHES := 3
# 2.7V brownout
lilypad: HFUSE := DD
# Internal 8MHz osc (8MHz) Slow rising power
lilypad: LFUSE := E2
# 512 byte boot
lilypad: EFUSE := FC
lilypad: AVR_FREQ := 8000000
lilypad: hex_file


# lilypad_resonator is the same as a 8MHz lilypad, except for fuses.
lilypad_resonator: TARGET := lilypad_resonator
lilypad_resonator: MCU_TARGET := atmega168
lilypad_resonator: LED_START_FLASHES := 3
# 2.7V brownout
lilypad_resonator: HFUSE := DD
# Full swing xtal (20MHz) 258CK/14CK+4.1ms
lilypad_resonator: LFUSE := C6
# 512 byte boot
lilypad_resonator: EFUSE := FC
lilypad_resonator: AVR_FREQ := 8000000
lilypad_resonator: hex_file


atmega328_pro8: TARGET := atmega328_pro_8MHz
atmega328_pro8: MCU_TARGET := atmega328p
atmega328_pro8: LED_START_FLASHES := 3
# 512 byte boot, SPIEN
atmega328_pro8: HFUSE := DE
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega328_pro8: LFUSE := FF
# 2.7V brownout
atmega328_pro8: EFUSE := FD
atmega328_pro8: AVR_FREQ := 8000000
atmega328_pro8: hex_file



################################################
# 
# Support for the Atmel Xplained mini eval boards that are mostly
#   compatible with Arduino.  (168pb, 328p, and 328pb chips.)
#
# Currently these all masquerade as 168 or 328p, because the IDE
#  does not yet have compiler support for the -pb variants.
#
# These boards have an mEDBG debug chip that:
#   1) means that optiboot can only be programmed via Atmel Studio
#   2) prevents optiboot from working at 115200bps.
#   3) provides 16MHz (at 5V) via Xin on the chip.
#  
# 
#
xplained168pb: TARGET := xplained168pb
xplained168pb: MCU_TARGET := atmega168
xplained168pb: AVR_FREQ := 16000000
xplained168pb: BAUD_RATE := 57600
xplained168pb: hex_file

xplained328pb: TARGET := xplained328pb
xplained328pb: MCU_TARGET := atmega328
xplained328pb: AVR_FREQ := 16000000
xplained328pb: BAUD_RATE := 57600
xplained328pb: hex_file

xplained328p: TARGET := xplained328p
xplained328p: MCU_TARGET := atmega328
xplained328p: AVR_FREQ := 16000000
xplained328p: BAUD_RATE := 57600
xplained328p: hex_file

#
# ATmega168p [QFN32]
#
atmega168p_lp: TARGET := atmega168p_lp
atmega168p_lp: MCU_TARGET := atmega168p
# 2.7V brownout
atmega168p_lp: HFUSE := DD
# Low power xtal (16MHz) 16KCK/14CK+65ms
atmega168p_lp: LFUSE := FF
# 512 byte boot
atmega168p_lp: EFUSE := FC
atmega168p_lp: VIRTUAL_BOOT_PARTITION := 1
atmega168p_lp: hex_file


#
# Test for virtual boot feature, which is not recommended for the ATmega328
# for virtual boot partition we can use the Flash page size as Boot page length
# no boot section, SPIEN
virboot328: TARGET := virboot328
virboot328: MCU_TARGET := atmega328p
virboot328: HFUSE := DF
virboot328: VIRTUAL_BOOT_PARTITION := 1
virboot328: hex_file

#
#  AVR chips, ATmega8, ATmega16, ATmega32 and ATmega64
# Some older chips are probably also supported ATmega162, ATmega163, ATmega323
#
# * Copyright 2013-2015 by Bill Westfield.  Part of Optiboot.
# * reorganized 2017, 2020 by K.-H. Kuebbeler
# * This software is licensed under version 2 of the Gnu Public Licence.
# * See optiboot.c for details.

#---------------------------------------------------------------------------

################################################
# generic atmega8
atmega8: TARGET := atmega8
atmega8: MCU_TARGET := atmega8
atmega8: hex_file

################################################
# generic atmega16
atmega16: TARGET := atmega16
atmega16: MCU_TARGET := atmega16
atmega16: hex_file

################################################
# generic atmega32
atmega32: TARGET := atmega32
atmega32: MCU_TARGET := atmega32
atmega32: hex_file

################################################
# generic atmega64
atmega64: TARGET := atmega64
atmega64: MCU_TARGET := atmega64
atmega64: hex_file

################################################
# generic atmega128
atmega128: TARGET := atmega128
atmega128: MCU_TARGET := atmega128
atmega128: hex_file

################################################
# generic atmega162
atmega162: TARGET:= atmega162
atmega162: override MCU_TARGET := atmega162
atmega162: hex_file

################################################
# generic atmega163
atmega163: TARGET:= atmega163
atmega163: MCU_TARGET := atmega163
atmega163: hex_file

#
# ATmega323
################################################
# generic atmega323
atmega323: TARGET:= atmega323
atmega323: MCU_TARGET := atmega323
atmega323: hex_file

################################################
# generic atmega8515
atmega8515: TARGET:= atmega8515
atmega8515: MCU_TARGET := atmega8515
atmega8515: hex_file

################################################
# generic atmega8535
atmega8535: TARGET:= atmega8535
atmega8535: MCU_TARGET := atmega8535
atmega8535: hex_file

################################################
# generic atmega8u2
atmega8u2: TARGET:= atmega8u2
atmega8u2: MCU_TARGET := atmega8u2
atmega8u2: hex_file

################################################
# generic atmega16u2
atmega16u2: TARGET:= atmega16u2
atmega16u2: MCU_TARGET := atmega16u2
atmega16u2: hex_file

################################################
# generic atmega32u2
atmega32u2: TARGET:= atmega32u2
atmega32u2: MCU_TARGET := atmega32u2
atmega32u2: hex_file

################################################
# generic atmega16u4
atmega16u4: TARGET:= atmega16u4
atmega16u4: MCU_TARGET := atmega16u4
atmega16u4: hex_file

################################################
# generic atmega32u4
atmega32u4: TARGET:= atmega32u4
atmega32u4: MCU_TARGET := atmega32u4
atmega32u4: hex_file

################################################
# Test platforms
################################################
# Virtual boot block test
virboot8: TARGET := virboot8
virboot8: VIRTUAL_BOOT_PARTITION := 1
virboot8: save_vect_num := EE_RDY_vect_num
virboot8: MCU_TARGET := atmega8
virboot8: HFUSE := C9
virboot8: hex_file

#
# AVR chips, ATmega169, ATmega329, ATmega3290, ATmega649 and ATmega6490
#

################################################
# generic atmega169
atmega169: TARGET:= atmega169
atmega169: MCU_TARGET := atmega169
atmega169: hex_file

################################################
# generic atmega169p
atmega169p: TARGET:= atmega169p
atmega169p: MCU_TARGET := atmega169p
atmega169p: hex_file

##############################################
# generic atmega329
atmega329: TARGET:= atmega329
atmega329: MCU_TARGET := atmega329
atmega329: hex_file

##############################################
# generic atmega329p
atmega329p: TARGET:= atmega329p
atmega329p: MCU_TARGET := atmega329p
atmega329p: hex_file

################################################
# generic atmega3290
atmega3290: TARGET:= atmega3290
atmega3290: MCU_TARGET := atmega3290
atmega3290: hex_file

################################################
# generic atmega3290p
atmega3290p: TARGET:= atmega3290p
atmega3290p: MCU_TARGET := atmega3290p
atmega3290p: hex_file

################################################
# generic atmega649
# let board specific change the frequency and fuses
atmega649: TARGET:= atmega649
atmega649: MCU_TARGET := atmega649
atmega649: hex_file

################################################
# generic atmega649p
atmega649p: TARGET:= atmega649p
atmega649p: MCU_TARGET := atmega649p
atmega649p: hex_file

################################################
# generic atmega6490
atmega6490: TARGET:= atmega6490
atmega6490: MCU_TARGET := atmega6490
atmega6490: hex_file

################################################
# generic atmega6490p
atmega6490p: TARGET:= atmega6490p
atmega6490p: MCU_TARGET := atmega6490p
atmega6490p: hex_file


#
# AVR chips, ATmega640 and ATmega1280
#
################################################
# generic atmega1280
atmega1280: TARGET := atmega1280
atmega1280: MCU_TARGET := atmega1280
atmega1280: hex_file 

################################################
# generic atmega2560
atmega2560: TARGET := atmega2560
atmega2560: MCU_TARGET := atmega2560
atmega2560: hex_file 

################################################
# generic atmega1281
atmega1281: TARGET := atmega1281
atmega1281: MCU_TARGET := atmega1281
#ifndef EFUSE
## 2.7V brownout; wants F5 for some reason...
#atmega1281: EFUSE := F5
#endif
atmega1281: hex_file 

################################################
# generic atmega2561
atmega2561: TARGET := atmega2561
atmega2561: MCU_TARGET := atmega2561
atmega2561: hex_file 

################################################
# generic atmega640
atmega640: TARGET := atmega640
atmega640: MCU_TARGET := atmega640
atmega640: hex_file 



# MEGA1280 Board (this is different from the atmega1280 chip platform)
# Mega has a minimum boot size of 1024 bytes, so enable extra functions
#mega: TARGET = atmega1280
mega1280: TARGET := mega1280
mega1280: MCU_TARGET := atmega1280
mega1280: hex_file


#  AVR Chips ATmega164, ATmega324, ATmega644 and ATmega1284

# Chip level targets
################################################
# generic atmega164a
atmega164a: TARGET := atmega164a
atmega164a: MCU_TARGET := atmega164a
atmega164a: hex_file

################################################
# generic atmega164p
atmega164p: TARGET := atmega164p
atmega164p: MCU_TARGET := atmega164p
atmega164p: hex_file

################################################
# generic atmega324a
atmega324a: TARGET := atmega324a
atmega324a: MCU_TARGET := atmega324a
atmega324a: hex_file

################################################
# generic atmega324p
atmega324p: TARGET := atmega324p
atmega324p: MCU_TARGET := atmega324p
atmega324p: hex_file

################################################
# generic atmega324pa
atmega324pa: TARGET := atmega324pa
atmega324pa: MCU_TARGET := atmega324pa
atmega324pa: hex_file

################################################
# generic atmega644
atmega644: TARGET := atmega644
atmega644: MCU_TARGET := atmega644
atmega644: hex_file

################################################
# generic atmega644p
atmega644p: TARGET := atmega644p
atmega644p: MCU_TARGET := atmega644p
atmega644p: hex_file

################################################
# generic atmega644a
atmega644a: TARGET := atmega644a
atmega644a: MCU_TARGET := atmega644a
atmega644a: hex_file

################################################
# generic atmega644pa
atmega644pa: TARGET := atmega644pa
atmega644pa: MCU_TARGET := atmega644pa
atmega644pa: hex_file

################################################
# generic atmega1284p
atmega1284p: TARGET := atmega1284p
atmega1284p: MCU_TARGET := atmega1284p
atmega1284p: hex_file


#
# Board-level targets
#

# Sanguino has a minimum boot size of 1024 bytes, so enable extra functions
#
sanguino: TARGET := sanguino
sanguino: MCU_TARGET := atmega644p
sanguino: LED := B0
sanguino: HFUSE := DE
sanguino: LFUSE := F7
sanguino: EFUSE := FD
sanguino: AVR_FREQ := 16000000
sanguino: hex_file


mighty1284: TARGET := mighty1284
mighty1284: MCU_TARGET := atmega1284p
mighty1284: LED := B7
mighty1284: HFUSE := DE
mighty1284: LFUSE := F7
mighty1284: EFUSE := FD
mighty1284: AVR_FREQ := 16000000
mighty1284: hex_file


bobuino: TARGET := bobuino
bobuino: MCU_TARGET := atmega1284p
bobuino: LED := B7
# 1024 byte boot
bobuino: HFUSE := DE
# Full swing xtal (16MHz) 16KCK/14CK+65ms
bobuino: LFUSE := F7
# 2.7V brownout
#bobuino: EFUSE := 05
bobuino: EFUSE := FD
bobuino: AVR_FREQ := 16000000
bobuino: hex_file


#
# Wicked Devices "Wildfire" boards (1284 with wireless!)
#

wildfirev2: TARGET := wildfirev2
wildfirev2: MCU_TARGET := atmega1284p
wildfirev2: LED := B7
wildfirev2: BAUD_RATE := 1000000
wildfirev2: HFUSE := DE
wildfirev2: LFUSE := F7
wildfirev2: EFUSE := FD
wildfirev2: AVR_FREQ := 16000000
wildfirev2: hex_file


wildfirev3: TARGET := wildfirev3
wildfirev3: MCU_TARGET := atmega1284p
wildfirev3: LED := B5
wildfirev3: HFUSE := DE
wildfirev3: LFUSE := F7
wildfirev3: EFUSE := FD
wildfirev3: AVR_FREQ := 16000000
wildfirev3: hex_file


wildfire: TARGET := wildfire
wildfire: MCU_TARGET := atmega1284p
wildfire: LED := B5
wildfire: HFUSE := DE
wildfire: LFUSE := F7
wildfire: EFUSE := FD
wildfire: AVR_FREQ := 16000000
wildfire: hex_file


#
# for 44-pin or 100-pin AVR chips, ATmega165, ATmega165p, ATmega325, ATmega325p,
#                       ATmega645, ATmega645p,  ATmega6450 and ATmega6450p
#

# Chip level targets
################################################
# generic atmega165
atmega165: TARGET := atmega165
atmega165: MCU_TARGET := atmega165
atmega165: hex_file

################################################
# generic atmega165p
atmega165p: TARGET := atmega165p
atmega165p: MCU_TARGET := atmega165p
atmega165p: hex_file

################################################
# generic atmega165a
atmega165a: TARGET := atmega165a
atmega165a: MCU_TARGET := atmega165a
atmega165a: hex_file

################################################
# generic atmega325
atmega325: TARGET := atmega325
atmega325: MCU_TARGET := atmega325
atmega325: hex_file

################################################
# generic atmega325p
atmega325p: TARGET := atmega325p
atmega325p: MCU_TARGET := atmega325p
atmega325p: hex_file

################################################
# generic atmega325a
atmega325a: TARGET := atmega325a
atmega325a: MCU_TARGET := atmega325a
atmega325a: hex_file

################################################
# generic atmega3250p
atmega3250p: TARGET := atmega3250p
atmega3250p: MCU_TARGET := atmega3250p
atmega3250p: hex_file

################################################
# generic atmega3250
atmega3250: TARGET := atmega3250
atmega3250: MCU_TARGET := atmega3250
atmega3250: hex_file

################################################
# generic atmega3250a
atmega3250a: TARGET := atmega3250a
atmega3250a: MCU_TARGET := atmega3250a
atmega3250a: hex_file

################################################
# generic atmega3250pa
atmega3250pa: TARGET := atmega3250pa
atmega3250pa: MCU_TARGET := atmega3250pa
atmega3250pa: hex_file

################################################
# generic atmega645
atmega645: TARGET := atmega645
atmega645: MCU_TARGET := atmega645
atmega645: hex_file

################################################
# generic atmega645p
atmega645p: TARGET := atmega645p
atmega645p: MCU_TARGET := atmega645p
atmega645p: hex_file

################################################
# generic atmega645a
atmega645a: TARGET := atmega645a
atmega645a: MCU_TARGET := atmega645a
atmega645a: hex_file

################################################
# generic atmega6450p
atmega6450p: TARGET := atmega6450p
atmega6450p: MCU_TARGET := atmega6450p
atmega6450p: hex_file

################################################
# generic atmega6450
atmega6450: TARGET := atmega6450
atmega6450: MCU_TARGET := atmega6450
atmega6450: hex_file

################################################
# generic atmega6450a
atmega6450a: TARGET := atmega6450a
atmega6450a: MCU_TARGET := atmega6450a
atmega6450a: hex_file

################################################

#
#  for AT90 series of AVRs.
#
################################################
# generic at90can32
at90can32: TARGET:= at90can32
at90can32: override MCU_TARGET := at90can32
at90can32: hex_file

################################################
# generic at90can64
at90can64: TARGET:= at90can64
at90can64: override MCU_TARGET := at90can64
at90can64: hex_file
#
################################################
# generic at90can128
at90can128: TARGET:= at90can128
at90can128: override MCU_TARGET := at90can128
at90can128: hex_file

################################################
# generic at90pwm2b
at90pwm2b: TARGET:= at90pwm2b
at90pwm2b: MCU_TARGET := at90pwm2b
at90pwm2b: hex_file

################################################
# at90pwm2
at90pwm2: TARGET:= at90pwm2
at90pwm2: MCU_TARGET := at90pwm2b
# MCU_Target at90pwm2 has error in FLASHEND setting in GNU lib,
# so we use the at90pwm2b instead.
at90pwm2: hex_file

################################################
# generic at90pwm3b
at90pwm3b: TARGET:= at90pwm3b
at90pwm3b: MCU_TARGET := at90pwm3b
at90pwm3b: hex_file

################################################
# at90pwm3
at90pwm3: TARGET:= at90pwm3
at90pwm3: MCU_TARGET := at90pwm3
# MCU_Target at90pwm3 has error in FLASHEND setting in GNU lib,
# so we use the at90pwm3b instead.
at90pwm3: hex_file


#
# for tiny AVR chips
#
################################################
# generic attiny84
attiny84: TARGET := attiny84
attiny84: MCU_TARGET := attiny84
attiny84: hex_file
#
################################################
attiny44: TARGET := attiny44
attiny44: MCU_TARGET := attiny44
attiny44: hex_file
#
################################################
attiny24: TARGET := attiny24
attiny24: MCU_TARGET := attiny24
attiny24: hex_file
#
################################################
# generic attiny841
attiny841: TARGET := attiny841
attiny841: MCU_TARGET := attiny841
attiny841: hex_file
#
# ATtiny441
################################################
# generic attiny441
# let board specific change the frequency and fuses
attiny441: TARGET := attiny441
attiny441: MCU_TARGET := attiny441
# use the flash page size of tiny841 to prevent unaligned Start-Address
# but tiny441 with a 16 byte page size, can only erase 4 pages (64 byte)
attiny441: hex_file

#
# ATtiny85
################################################
# generic attiny85
attiny85: TARGET := attiny85
attiny85: MCU_TARGET := attiny85
attiny85: hex_file
#
# ATtiny45
################################################
attiny45: TARGET := attiny45
attiny45: MCU_TARGET := attiny45
attiny45: hex_file
#
# ATtiny25
################################################
# let board specific change the frequency and fuses
attiny25: TARGET := attiny25
attiny25: MCU_TARGET := attiny25
attiny25: hex_file

################################################
# generic attiny88
attiny88: TARGET := attiny88
attiny88: MCU_TARGET := attiny88
attiny88: hex_file

#
# ATtiny48
################################################
attiny48: TARGET := attiny48
attiny48: MCU_TARGET := attiny48
attiny48: hex_file

#
# ATtiny87
################################################
attiny87: TARGET := attiny87
attiny87: MCU_TARGET := attiny87
attiny87: hex_file

#
# ATtiny167
################################################
attiny167: TARGET := attiny167
attiny167: MCU_TARGET := attiny167
attiny167: hex_file

# generic atmega48   has no Bootloader Page
atmega48: TARGET := atmega48
atmega48: MCU_TARGET := atmega48
atmega48: hex_file

# generic atmega48p   has no Bootloader Page
atmega48p: TARGET := atmega48p
atmega48p: MCU_TARGET := atmega48p
atmega48p: hex_file

#
################################################
# generic attiny861
attiny861: TARGET := attiny861
attiny861: MCU_TARGET := attiny861
attiny861: hex_file

#
################################################
# generic attiny461
attiny461: TARGET := attiny461
attiny461: MCU_TARGET := attiny461
attiny461: hex_file

#
################################################
# generic attiny261
attiny261: TARGET := attiny261
attiny261: MCU_TARGET := attiny261
attiny261: hex_file


################################################
# generic attiny1634
# use the flash page size of tiny1634 to prevent unaligned Start-Address
# but tiny1634 with a 32 byte page size, can only erase 4 pages (64 byte)
attiny1634: TARGET := attiny1634
attiny1634: MCU_TARGET := attiny1634
attiny1634: hex_file

#
################################################
# generic attiny4313
attiny4313: TARGET := attiny4313
attiny4313: MCU_TARGET := attiny4313
attiny4313: hex_file

#
################################################
# generic attiny2313
attiny2313: TARGET := attiny2313
attiny2313: MCU_TARGET := attiny2313
attiny2313: LED_START_FLASHES := 0
attiny2313: hex_file


# 1MHz clocked platforms/boards
#
# These are capable of 9600 baud
#

luminet: TARGET := luminet
luminet: MCU_TARGET := attiny84
luminet: AVR_FREQ := 1000000
# 1MHz internal oscillator, slowly rising power, requires AVR_FREQ set to 1000000
luminet: LFUSE := 62
luminet: BAUD_RATE := 9600
luminet: LED_START_FLASHES := 0
luminet: hex_file

digispark_t167: TARGET := digispark_t167
digispark_t167: MCU_TARGET := attiny167
digispark_t167: AVR_FREQ := 16000000
digispark_t167: LFUSE := DF
digispark_t167: LED := B1
digispark_t167: BAUD_RATE := 52
digispark_t167: hex_file


ifneq "$(origin MCU_TARGET)" "undefined" 
 ifneq "$(origin MCU_TARGET)" "file" 
  ifneq "$(origin MCU_TARGET)" "environment"
   $(error The origin of MCU_TARGET is '$(origin MCU_TARGET)' instead of 'file' or 'environment')
  endif
 endif
endif

ifndef FLASH_ERASE_CNT
 FLASH_ERASE_CNT = 1
endif


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
 export SOURCE_TYPE TWO_STOP_BITS
 export WRITE_PROTECT_PIN

hex_file: 
	@bash build_hex.sh


# remove all generated files
clean:
	@$(RM_CMD) -rf *.o *.elf *.lst *.log *.map *.sym *.lss *.eep *.srec *.bin *.hex *.ppp *.tmp.sh *.dat
steril:
	@$(RM_CMD) -rf *.o *.elf *.map *.sym *.tmp.sh *.dat *.ppp $(PROGRAM).hex 

help:
ifeq ("$(LANGUAGE)", "de_DE")
	@$(ECHO_CMD) "Das ist der Hilfetext für die optiboot Makefile (make help)"
	@$(ECHO_CMD) "Wichtige Optionen für die Erzeugung eines optiboot bootloaders:"
	@$(ECHO_CMD) "AVR_FREQ       gibt die Arbeitsfrequenz (Hz) für den AVR an."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "BAUD_RATE      gibt die gewünschte Baudrate für die serielle Schnittstelle an."
	@$(ECHO_CMD) "               Ein Wert unter 100 wählt eine automatische Baudraten-Anpassung."
	@$(ECHO_CMD) "               Dabei wird bei Werten von 40-59 nur eine Zeitüberwachung"
	@$(ECHO_CMD) "               durchgeführt, bei Werten unter 40 wird nichts kontrolliert."
	@$(ECHO_CMD) "               Bei Werten von 60-99 wird das empfangene Zeichen aufwendiger"
	@$(ECHO_CMD) "               kontrolliert."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART           gibt die Nummer des Hardware UART (0-3) an, wenn vorhanden."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "LED_START_FLASHES gibt die Anzahl des Blinkens einer LED an."
	@$(ECHO_CMD) "               Eine 0 wählt die Funktion ab, bei 1 wird nur einmal geblinkt."
	@$(ECHO_CMD) "               Bei negative Zahlen wird das Blinken NICHT vorzeitig verlassen,"
	@$(ECHO_CMD) "               wenn ein ankommendes Zeichen auf der seriellen Schnittstelle"
	@$(ECHO_CMD) "               erkannt wird."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "LED_DATA_FLASH =1 lässt die LED aufleuchten, wenn auf serielle Daten gewartet wird. "
	@$(ECHO_CMD) "               =4 Schaltet die LED nur beim Start ein."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "LED            spezifiziert den Port und das Bit, wo die LED angeschlossen ist."
	@$(ECHO_CMD) "               LED=B5 würde das Bit 5 von Port B wählen."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "SUPPORT_EEPROM ermöglicht das Beschreiben des EEproms mit dem bootloader,"
	@$(ECHO_CMD) "               wenn ungleich 0 gesetzt wird. In der Assemblerversion ist"
	@$(ECHO_CMD) "               auch ohne Angabe der Option das EEprom Beschreiben ermöglicht."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "ISP            mit dieser Option wird für die ISP Schnittstelle avrdude gestartet"
	@$(ECHO_CMD) "               ISP=1 startet das Beschreiben des AVR mit dem optiboot Bootloader."
	@$(ECHO_CMD) "               ISP=2 statet eine extra Kontrolle des vorhandenen Bootloaders (verify)."
	@$(ECHO_CMD) "               ISP=3 liest den kompletten Flash Speicher des AVR (Read_<AVR-ID>.hex)."
	@$(ECHO_CMD) "               ISP=4 liest das komplette EEprom des AVR (Read_<AVR_ID>.eep."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "Bei jedem make Aufruf muß der Ziel-AVR-Prozessor <AVR_ID> angegeben werden,"
	@$(ECHO_CMD) "um einen passenden optiboot bootloader mit den zusätzlich gewählten Optionen"
	@$(ECHO_CMD) "zu generieren."
	@$(ECHO_CMD) "Beispiel:" 
	@$(ECHO_CMD) "make atmega328p LED_START_FLASHES=0 LED_DATA_FLASH=1"
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "würde einen optiboot generieren ohne das LED-Blinken mit aufleuchtender LED,"
	@$(ECHO_CMD) "wenn auf RX Daten gewartet wird."
	@$(ECHO_CMD) "Mit make help2 werden weitere Optionen aufgelistet."
	@$(ECHO_CMD) "make pl   listet die unterstützten AVR-Prozessoren."
	@$(ECHO_CMD) " "

else
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
	@$(ECHO_CMD) "make pl   list the names of the supported AVR processors."
	@$(ECHO_CMD) " "
endif
help2:
ifeq ("$(LANGUAGE)", "de_DE")
	@$(ECHO_CMD) "Weitere Optionen zum Erzeugen eines optiboot Bootloaders (make help2):"
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "SOFT_UART       wählt ein Software Programm anstelle des Hardware UART."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART_RX         Wählt einen Pin und Port für den Software UART Eingang."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART_TX         Wählt einen Pin und Port für den Software UART Ausgang."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "C_SOURCE        =1 wählt die C-Quellen anstelle der Assemblerversion."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "BIGBOOT         wählt die angegebene Menge zusätzlichen Speicher für optiboot."
	@$(ECHO_CMD) "                Die Option wird nur für Tests der Längenanpassung sinnvoll."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "VIRTUAL_BOOT_PARTITION ändert zwei Interrupt Vektoren des Anwenderprogramms,"
	@$(ECHO_CMD) "                um einen Bootloader auch ohne die Bootloader Unterstützung des"
	@$(ECHO_CMD) "                AVR Prozessor (ATtiny) zu ermöglichen."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "TIMEOUT_MS      gibt eine Zeitobergrenze in ms für das Warten auf serielle"
	@$(ECHO_CMD) "                Daten an. Nach dieser Zeit wird das Anwenderprogramm gestartet."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "OSCCAL_CORR     gibt einen Korrekturwert für das OSCCAL-Byte des internen"
	@$(ECHO_CMD) "                RC-Oszillators an."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "FORCE_RSTDISBL  läßt das Setzen des RSTDISBL Fuse-Bits zu, wenn auf 1 gesetzt."
	@$(ECHO_CMD) "                Ohne diese Option wird die Erzeugung des Bootloaders abgebrochen."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "NO_EARLY_PAGE_ERASE verhindert das Löschen der Flash-Speicherseite während"
	@$(ECHO_CMD) "                des Dateneempfang. Das nach dem Datenempfang durchgeführte"
	@$(ECHO_CMD) "                Löschen macht den Programmirvorgang etwa 30% langsamer,"
	@$(ECHO_CMD) "                spart aber 14 Byte Platz für den optiboot."

else
	@$(ECHO_CMD) "Additional options for building a optiboot (make help2):"
	@$(ECHO_CMD) "SOFT_UART      select a software UART solution instead of the hardware UART."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART_RX        specifies the port and pin for software UART input."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "UART_TX        specifies the port and pin for software UART output."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "C_SOURCE       set to 1 for C source instead of assembler version."
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "INVERSE_UART   set to 1 for inverting the level of RX and TX signals, only"
	@$(ECHO_CMD) "               with SOFT_UART possible."
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
	@$(ECHO_CMD) " "
	@$(ECHO_CMD) "NO_EARLY_PAGE_ERASE will disable the early page erase before data fetch."
	@$(ECHO_CMD) "              So the programming of flash is about 30% slower,"
	@$(ECHO_CMD) "              but this option will save 14 bytes optiboot space."
	@$(ECHO_CMD) " "
endif
pl:
	@bash ./list_processors.sh
