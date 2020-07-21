#!/bin/bash
#
# K.-H. Kübbeler (kh_kuebbeler@web.de)  Jun 2020
# 
# This part of optiboot  is now called from the build_hex.sh script
# you have to set up many variables for using this script without
# the Makefile and the build_hex.sh script!
# Please use the optiboot Makefile like "make atmega328p ISP=1"
# to call this script!
#
#
# Set the parameters for the avrdude isp tool
#

if [ "${ISPTOOL}" = "" ]
then
  ISPTOOL="avrisp2"
fi

if [ "${ISPPORT}" = "" ]
then
  # no port specified
  ISPPORT="usb"
  # ISPPORT="COM7"
  # ISPPORT="/dev/ttyACM0"
fi
export ISPPORT
ISPSPEED="-b 115200"
#
#

#
#Test Output
# echo "abhaengig von ${PROGRAM}_${TARGET}.hex"
# echo "AVRDUDE_CONF = ${AVRDUDE_CONF}"
# echo "LFUSE = ${LFUSE}"
# echo "HFUSE = ${HFUSE}"
# echo "MCU_TARGET = ${MCU_TARGET}"
# echo "ISP_FUSES = ${ispfuses}"
# echo "TARGET = ${TARGET}"
# echo "MCU_TARGET = ${MCU_TARGET}"


if [ "${TARGET}" = "" ]
then
 if [ "${LANGUAGE}" == "de_DE" ] ; then
  echo "Fehler! Das Ziel TARGET ist nicht bekannt."
  echo "Bitte rufen Sie diesen Script nur über den build_hex.sh Script auf,"
  echo "der über die Makefile aufgerufen wird."
 else
  echo "Error! the actual TARGET name is not known"
  echo "Please call this script only inside the build_hex script, called from Makefile"
 fi
 exit 1
fi
if [ "${FLASH_SIZE}" = "" ]
then
 if [ "${LANGUAGE}" == "de_DE" ] ; then
  echo "Fehler! Dieser Script sollte nur vom build_hex.sh Script aufgerufen werden,"
  echo "welcher über die Makefile aufgerufen wird."
 else
  echo "Error! This script should only be called from inside the build_hex script, called from Makefile"
 fi
 exit 1
fi
if [ "${BOOT_PAGE_LEN}" = "" ]
then
 if [ "${LANGUAGE}" == "de_DE" ] ; then
  echo "Fehler! Dieser Script sollte nur vom build_hex.sh Script aufgerufen werden,"
  echo "welcher über die Makefile aufgerufen wird."
 else
  echo "Error! This script should only be called from inside the build_hex script, called from Makefile"
 fi
 exit 1
fi
#

#
# determine the bit clock (us) for avrdude
BIT_CLOCK=`echo "scale=2;8000000/${AVR_FREQ}+1" | bc`


# Check, if there are only some special request to do, not program the target!
if (( ${ISP} == 4 ))
then
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "##### Lies das komplette EEprom von ${TARGET} nach Read_${TARGET}.eep"
  else
   echo "##### read complete EEprom from ${TARGET} to Read_${TARGET}.eep"
  fi
  export DUDE_PARAMS=" ${AVRDUDE_CONF} -c ${ISPTOOL} -B ${BIT_CLOCK} -p ${MCU_TARGET} -P ${ISPPORT} ${ISPSPEED} -U eeprom:r:./Read_${TARGET}.eep:i"
  source ./only_avrdude.sh
  exit
fi
if (( ${ISP} == 3 ))
then
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "##### Lied den kompletten Flash Speicher von ${TARGET} nach  Read_${TARGET}.hex"
  else
   echo "##### read complete flash from ${TARGET} to Read_${TARGET}.hex"
  fi
  export DUDE_PARAMS="${AVRDUDE_CONF} -c ${ISPTOOL} -B ${BIT_CLOCK} -p ${MCU_TARGET} -P ${ISPPORT} ${ISPSPEED} -U flash:r:./Read_${TARGET}.hex:i"
  source ./only_avrdude.sh
  exit
fi
if (( ${ISP} == 2 ))
then
  # verify mode
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "##### Verify das optiboot Programm in ${TARGET}"
  else
   echo "##### verify the optiboot in ${TARGET}"
  fi
  export DUDE_PARAMS="${AVRDUDE_CONF} -c ${ISPTOOL} -B ${BIT_CLOCK} -p ${MCU_TARGET} -P ${ISPPORT} ${ISPSPEED} -U flash:v:${PROGRAM}_${TARGET}.hex:i"
  source ./only_avrdude.sh
  exit
fi

if (( ${ISP} == 1 ))
then
  echo " "
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "####### Start von program_target.sh für ${MCU_TARGET} #############"
  else
   echo "####### Start of program_target.sh for ${MCU_TARGET} #############"
  fi
else
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "##### program_target.sh kann nur ISP Werte von 1 bis 4 bearbeiten ######"
   echo "ISP=1 installiert optiboot auf ${TARGET}"
   echo "ISP=2 prüft optiboot auf ${TARGET}"
   echo "ISP=3 liest den kompletten Flash Speicher von ${TARGET}"
   echo "ISP=4 liest den kompletten EEprom Speicher von ${TARGET}"
  else
   echo "##### program_target.sh can handle only ISP values from 1 to 4 ######"
   echo "ISP=1 for transfering optiboot to the ${TARGET}"
   echo "ISP=2 for verifying optiboot at the ${TARGET}"
   echo "ISP=3 to read the complete Flash memory of ${TARGET}"
   echo "ISP=4 to read the complete EEprom memory of ${TARGET}"
  fi
  exit 1
fi

# seems to be a normal request, check for correct Fuse setting!
# Not all chips have EFUSE.

if [ "${EFUSE}" = "" ]
then
EFUSE_CMD=""
else
EFUSE_CMD=" -U efuse:w:0x${EFUSE}:m"
fi

#
# avrdude commands to erase chip, unlock memory, and program fuses.
#
#  the lock bits can only be activated by writing a 0 bit, bits can be only set with chip erase!

# for non special handled processores, set USE_Edat to 0
USE_Edat=0
if [ "${MCU_TARGET}" = "attiny84" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny84a" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny44" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny44a" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny24" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny24a" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny85" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny45" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny25" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny88" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny48" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny861" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny861a" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny461" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny461a" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "attiny1634" ]
then
USE_Edat=2
fi

if [ "${MCU_TARGET}" = "atmega48" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "atmega48p" ]
then
USE_Edat=2
fi
if [ "${MCU_TARGET}" = "atmega88" ]
then
USE_Edat=1
fi
if [ "${MCU_TARGET}" = "atmega88p" ]
then
USE_Edat=1
fi
if [ "${MCU_TARGET}" = "atmega168" ]
then
USE_Edat=1
fi
if [ "${MCU_TARGET}" = "atmega168p" ]
then
USE_Edat=1
fi
if [ "${MCU_TARGET}" = "at90pwm2" ]
then
USE_Edat=1
fi
if [ "${MCU_TARGET}" = "at90pwm2b" ]
then
USE_Edat=1
fi
if [ "${MCU_TARGET}" = "at90pwm3" ]
then
USE_Edat=1
fi

#
# build the upper case form of hex values
UC_HFUSE=`echo "${HFUSE}" | tr a-z A-Z`
UC_EFUSE=`echo "${EFUSE}" | tr a-z A-Z`



if [ "${EFUSE}" = "" ]
then
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "   Die Fuses in program_target.sh sind gesetzt auf lfuse=0x${LFUSE}, hfuse=0x${HFUSE})"
  else
   echo "   The fuses in program_target.sh are set to lfuse=0x${LFUSE}, hfuse=0x${HFUSE})"
  fi
else
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "   Die Fuses in program_target.sh sind gesetzt auf lfuse=0x${LFUSE}, hfuse=0x${HFUSE}, efuse=0x${EFUSE}"
  else
   echo "   The fuses in program_target.sh are set to lfuse=0x${LFUSE}, hfuse=0x${HFUSE}, efuse=0x${EFUSE}"
  fi
fi

if (( ${USE_Edat} == 1 ))
then
  # correct the Bootloadersize in EFUSE
  if (( 0${VIRTUAL_BOOT_PARTITION} > 0 ))
  then
    PEFUSE=`echo "obase=16;ibase=16;(0${UC_EFUSE} - (0${UC_EFUSE} % 8) ) + (${BOOTSZ}*2) + 1" | bc`
  else
    PEFUSE=`echo "obase=16;ibase=16;(0${UC_EFUSE} - (0${UC_EFUSE} % 8) ) + (${BOOTSZ}*2)" | bc`
  fi
  if [ "${PEFUSE}" = "${EFUSE}" ]
  then
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "Bootloader EFUSE wird auf 0x${PEFUSE} gesetzt, OK"
    else
     echo "Bootloader EFUSE will be set to 0x${PEFUSE}, OK"
    fi
  else
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "Bootloader EFUSE wird auf 0x${PEFUSE} gesetzt anstelle von 0x${EFUSE}!!!"
    else
     echo "Bootloader EFUSE will be changed to 0x${PEFUSE} instead of 0x${EFUSE}!!!"
    fi
  fi
  if (( (${PEFUSE}&1) == 0 ))
  then
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "BootLoader Start ist gesetzt auf 0x${BL_StartAdr}, `echo "ibase=16;${BL_StartAdr}"| bc`"
    else
     echo "BootLoader Start is enabled to 0x${BL_StartAdr}, `echo "ibase=16;${BL_StartAdr}"| bc`"
    fi
  else
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "Bootloader Startadresse wird nicht benutzt, Start auf Adresse 0!"
    else
     echo "BootLoader Start is NOT enabled, start at address 0!"
    fi
  fi
fi

if (( ${USE_Edat} == 0 ))
then
#
# PHFUSE is only referenced by isp:
# The echo is used to produce the arithmetic formula for the bc function.
# Input and output of the bc function is set to hex format (base 16).
# Because the bc has no arithmetic and function to clear the BOOTSZ bits,
# a ((${HFUSE} && 2)*2) like subtraction is used to clear both bits.
# The ${BOOTSZ} is created from the real .text length of the bootloader
# and the ${BOOT_PAGE_LEN} variable of the Makefile.
# ${BOOTSZ}  = 00 for one boot-page, 01 for two boot-pages,
#              10 for four boot-pages and 11 for eight boot-pages.
# The boot-page length is usually 256, 512 or 1024 bytes. 
# The atmega48/88/168 use the extended fuse for setting the boot length.
# Therefore both values (${PHFUSE} and ${PEFUSE}) are estimated, but
# the right one is selected by the ${ISPFUSE}.

  if (( 0${VIRTUAL_BOOT_PARTITION} > 0 ))
  then
    PHFUSE=`echo "obase=16;ibase=16;(${UC_HFUSE} - (${UC_HFUSE} % 8) ) + (${BOOTSZ}*2) + 1" | bc`
  else
    PHFUSE=`echo "obase=16;ibase=16;(${UC_HFUSE} - (${UC_HFUSE} % 8) ) + (${BOOTSZ}*2) " | bc`
  fi
  if [ "${PHFUSE}" = "${HFUSE}" ]
  then
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "Bootloader HFUSE wird auf 0x${PHFUSE} gesetzt, OK!"
    else
     echo "Bootloader HFUSE will be set to 0x${PHFUSE}, OK!"
    fi
  else
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "Bootloader HFUSE wird auf 0x${PHFUSE} anstelle von 0x${HFUSE} gesetzt!!!"
    else
     echo "Bootloader HFUSE will be changed to 0x${PHFUSE} instead of 0x${HFUSE}!!!"
    fi
  fi
  Hlobit=`echo "ibase=16;${PHFUSE} % 2" | bc`
  if (( ${Hlobit} == 0 ))
  then
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "BootLoader Startadresse ist gesetzt auf 0x${BL_StartAdr}, `echo "ibase=16;${BL_StartAdr}"| bc`"
    else
     echo "BootLoader Start is enabled to 0x${BL_StartAdr}, `echo "ibase=16;${BL_StartAdr}"| bc`"
    fi
  else
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "Bootloader Startadresse wird nicht benutzt, Start auf Adresse 0!"
    else
     echo "BootLoader Start is NOT enabled, start at 0!"
    fi
  fi
fi

if (( ${USE_Edat} == 2 ))
then
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "BootLoader Startadresse ist 0x${BL_StartAdr}, Ende Adresse des Anwenderprogramms muß darunter bleiben! "
  else
   echo "BootLoader Start address is 0x${BL_StartAdr}, End address of application programs must remain below! "
  fi
fi


#
# avrdude commands to program the new bootloader, and protect the bootloader
# space from accidental SPM writes.  Note: "ef" allows boot section to be read
# by the application, which is different than the arduino default.
#
if (( ${USE_Edat} == 0 ))
then
  # most processors have the Bootloader bits BOOTSZ in the HFUSE.
  ISPFUSES="-e -u ${EFUSE_CMD} -U hfuse:w:0x${PHFUSE}:m -U lfuse:w:0x${LFUSE}:m"
  if ((0${VIRTUAL_BOOT_PARTITION} > 0))
  then
   ISPFLASH="-U flash:w:${PROGRAM}_${TARGET}.hex:i "
  else
# program bootloader and set lock bits
   ISPFLASH="-U flash:w:${PROGRAM}_${TARGET}.hex:i -U lock:w:0xef:m"
  fi
fi

if (( ${USE_Edat} == 1 ))
then
  ISPFUSES=" -e -u -U efuse:w:0x${PEFUSE}:m -U hfuse:w:0x${HFUSE}:m -U lfuse:w:0x${LFUSE}:m"
  if ((0${VIRTUAL_BOOT_PARTITION} > 0))
  then
    ISPFLASH="-U flash:w:${PROGRAM}_${TARGET}.hex:i "
  else
   # program bootloader and set lock bits
    ISPFLASH="-U flash:w:${PROGRAM}_${TARGET}.hex:i -U lock:w:0xef:m"
  fi
fi

if (( ${USE_Edat} == 2 ))
then
  # No fuse correction, no Bootloader feature present.
  # But we can set the SELFPRGEN Bit in the EFUSE
  ISPFUSES="-e -u -U efuse:w:0x`echo "obase=16;ibase=16;(0${UC_EFUSE} - (0${UC_EFUSE} % 2))" | bc`:m -U hfuse:w:0x${HFUSE}:m -U lfuse:w:0x${LFUSE}:m" 
  ISPFLASH="-U flash:w:${PROGRAM}_${TARGET}.hex:i"
  if ((0${VIRTUAL_BOOT_PARTITION} == 0))
  then
   if [ "${LANGUAGE}" == "de_DE" ] ; then
    echo ">>>>> Prozessor ${MCU_TARGET} braucht einen Bootloader mit der Option VIRTUAL_BOOT_PARTITION ! <<<<<"
   else
    echo ">>>>> Processor ${MCU_TARGET} requires a Bootloader with option VIRTUAL_BOOT_PARTITION ! <<<<<"
   fi
   exit 1
  fi
fi


# There are certain complicated caused by the fact that the default state
# of a fuse is a "1" rather than a "0", especially with respect to fuse bits
# that have not been implemented.  Those would normally not be included, but
# unimplemented fuses still default to being "1"
#
# the efuse should really be 0xf8; since, however, only the lower
# three bits of that byte are used on the atmega168, avrdude gets
# confused if you specify 1's for the higher bits, see:
# http://tinker.it/now/2007/02/24/the-tale-of-avrdude-atmega168-and-extended-bits-fuses/
#
# similarly, the lock bits should be 0xff instead of 0x3f (to
# unlock the bootloader section) and 0xcf instead of 0xef (to
# lock it), but since the high two bits of the lock byte are
# unused, avrdude would get confused.
if (( ${ISP} == 1 ))
then
  echo "\n; ISPFUSES = ${ISPFUSES}" >> ${PROGRAM}_${TARGET}.lst
  # normal ISP programming mode. ISPFLASH set the Lock-Bits, if required.
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "##### Lösche den ${TARGET} und setze die Fuses"
  else
   echo "##### erase the ${TARGET} and set fuses"
  fi
  export DUDE_PARAMS="${AVRDUDE_CONF} -c ${ISPTOOL} -B 200 -p ${MCU_TARGET} -P ${ISPPORT} ${ISPSPEED} -q -q ${ISPFUSES}"
  source ./only_avrdude.sh
  DUDE_FAIL=$?
  if (( ${DUDE_FAIL} != 0 ))
  then
    # Don't try the second Step, if avrdude fail!
    return ${DUDE_FAIL}
  fi
  if (( 0${VIRTUAL_BOOT_PARTITION} > 0 ))
  then
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "##### Schreibe den optiboot Bootloader auf ${TARGET}"
    else
     echo "##### write the optiboot bootloader to ${TARGET}"
    fi
  else
    if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "##### Schreibe den optiboot Bootloader auf ${TARGET} und setze die Lock Bits"
    else
     echo "##### write the optiboot bootloader to ${TARGET} and set the lock bits"
    fi
  fi
  export DUDE_PARAMS="${AVRDUDE_CONF} -c ${ISPTOOL} -B ${BIT_CLOCK} -p ${MCU_TARGET} -P ${ISPPORT} ${ISPSPEED} -D ${ISPFLASH}"
  source ./only_avrdude.sh
fi
