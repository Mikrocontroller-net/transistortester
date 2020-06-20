#!/bin/bash

# VerboseLev  1 = minimal output to screen
#             2 = only the main avr-gcc call is shown
#             3 = the main tasks are shown
#             4 = nearly all tasks are shown
if (( 0${VerboseLev} == 0 )) ; then
  #define the default VerboseLev value in this script
  VerboseLev=2
fi

# wITH_COLORS=1, color + bold + reverse is use to format the screen output
# WITH_COLORS=0, unformatted output to the scrrenn
if (( 0${WITH_COLORS} == 0 )) ; then
  WITH_COLORS=1
fi
if (( 0${WITH_COLORS} > 1 )) ; then WITH_COLORS=0; fi
if (( WITH_COLORS == 1 )) ; then
 #define the used  tput extension
 #color red
 Vrot=`tput setaf 9`
 #color yellow
 Vgelb=`tput setaf 11`
 #color green
 Vgreen=`tput setaf 2`
 #color grey
 Vgrau=`tput setaf 245`
 # bold
 Vfett=`tput bold`
 #
 Vlila=`tput setaf 13`
 # reverse
 Vinv=`tput rev`
 #normal
 Vnormal=`tput sgr0`
else
 Vrot=""
 Vgelb=""
 Vgreen=""
 Vgrau=""
 Vfett=""
 Vlila=""
 Vinv=""
 Vnormal=""
fi


LIBS="-lc"

logfile="${PROGRAM}_${TARGET}.log"
AVR_MHZ="(`echo "scale=2;${AVR_FREQ} / 1000000" | bc` Mhz)"
FREQ_OPER="Optiboot for ${Vgelb}${AVR_FREQ} Hz ${AVR_MHZ} operation${Vnormal}"
EE_SUPPORT=" and ${Vinv}EEprom support${Vnormal} configured."
echo "###############################" > ${logfile}
echo "Build of ${FREQ_OPER}" >> ${logfile}

if (( ${BAUD_RATE} < 100 )) ; then
 if (( 0${SUPPORT_EEPROM} == 0 )) ; then
   echo "${FREQ_OPER} with ${Vgelb}Auto-Baudrate${Vnormal} configured."
 else
   echo "${FREQ_OPER} with ${Vgelb}Auto-Baudrate${Vnormal}${EE_SUPPORT}"
 fi
else
 if (( 0${SUPPORT_EEPROM} == 0 )) ; then
   echo "${FREQ_OPER} with ${Vgelb}Baudrate ${BAUD_RATE}${Vnormal} configured."
 else
   echo "${FREQ_OPER} with ${Vgelb}Baudrate ${BAUD_RATE}${Vnormal}${EE_SUPPORT}"
 fi
fi

echo "Start building for AVR ${MCU_TARGET}:"

source avr_family.sh  ; #build a name for AVR_FAMILY from MCU_TARGET
source auto_led_pin.sh ; # find a LED preset for AVR_FAMILY


# Build of the first object file .o
if (( 0${VIRTUAL_BOOT_PARTITION} == 1 )) ; then
 if (( 0${save_vect_num} == 0 )) ; then
c_params="${CFLAGS} ${COMMON_OPTIONS} -DVIRTUAL_BOOT_PARTITION -DLED=p${LED} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -DHFUSE=hex${HFUSE} -DLFUSE=hex${LFUSE} -DBOOT_PAGE_LEN=${BOOT_PAGE_LEN} -DVerboseLev=${VerboseLev} -c -o ${PROGRAM}.o ${PROGRAM}.${SOURCE_TYPE}"
 else
c_params="${CFLAGS} ${COMMON_OPTIONS} -DVIRTUAL_BOOT_PARTITION -Dsave_vect_num=${save_vect_num} -DLED=p${LED} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -DHFUSE=hex${HFUSE} -DLFUSE=hex${LFUSE} -DBOOT_PAGE_LEN=${BOOT_PAGE_LEN} -DVerboseLev=${VerboseLev} -c -o ${PROGRAM}.o ${PROGRAM}.${SOURCE_TYPE}"
 fi
else
c_params="${CFLAGS} ${COMMON_OPTIONS} -DLED=p${LED} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -DHFUSE=hex${HFUSE} -DLFUSE=hex${LFUSE} -DBOOT_PAGE_LEN=${BOOT_PAGE_LEN} -DVerboseLev=${VerboseLev} -c -o ${PROGRAM}.o ${PROGRAM}.${SOURCE_TYPE}"
fi

  if (( ${VerboseLev} > 1 )) ; then echo "${Vgreen}avr-gcc ${c_params}${Vnormal}"; fi
avr-gcc ${c_params}

if (( $? == 0 )) ; then
 echo " .o build : OK!" >> ${logfile}
else
 echo " .o build : FAILED!" >> ${logfile} ; exit 1
fi

#  we can determine the size of the loader with optiboot.o for Assembler source input,
#  but for C source we need to do some optimizing  with the binder call (.elf).
#  This call is done without the relolation of the .text section (instruction code).
#  After computing the required size (and the possible relocation address),
#  This x.elf is removed later.

c_paramsx="${CFLAGS} ${COMMON_OPTIONS} ${LDSECTIONS} ${LDFLAGS} -o ${PROGRAM}x.elf ${PROGRAM}.o ${LIBS}"

if (( ${VerboseLev} > 3 )) ; then echo "${Vgreen}avr-gcc ${c_paramsx}${Vnormal}" ; fi
avr-gcc ${c_paramsx}

if (( $? == 0 )) ; then
 echo " x.elf build : OK!" >> ${logfile}
else
 echo " x.elf build : FAILED!" >> ${logfile} ; exit 1
fi

# BootPages fetch the actual boot loader size from a interim ${PROGRAM}x.elf file, not from the
# ${PROGRAM}.o file. The ld program can do some optimizing for code generated from a C-source.
# For code generated with the assembler there is no size difference between .o and .elf !
# If 3 pages are required, number of pages is rounded to 4.
# If more than 4 pages are required, the number of pages is rounded to 8.
# Above 8 pages there is no round up.
# The ATtiny84 has no hardware feature for the bootloader like the ATmega family.
# Therefore the "round up" is not required for the ATtiny84, but the BOOT_PAGE_LEN is 64 for
# this processor, so the number of pages is more than 7 pages for the actual size of optiboot.

export prog_size=`avr-size -C ${PROGRAM}x.elf | grep "Program:" | cut -c 10-16`
export pg_anz=`echo "${prog_size}/${BOOT_PAGE_LEN}+1" | bc`

if (( 0${VIRTUAL_BOOT_PARTITION} == 1 )) ; then
 export BootPages=`echo "( ${prog_size}/${FLASH_PAGE_SIZE}/${FLASH_ERASE_CNT} +1) * ${FLASH_ERASE_CNT}" | bc`
else
 export BootPages=`echo "${pg_anz} + (${pg_anz}==3 ) + (${pg_anz}==5)*3 + (${pg_anz}==6)*2 + (${pg_anz} == 7)" | bc`
fi


# With the  BootPages and the ${BOOT_PAGE_LEN} or ${FLASH_PAGE_SIZE} we can compute the 
# Start Address BL_StartAdr of the bootloader depending on the ${FLASH_SIZE} 
if (( 0${VIRTUAL_BOOT_PARTITION} == 1 )) ; then
 export BL_StartAdr=`echo "obase=16;${FLASH_SIZE} - (${BootPages}*${FLASH_PAGE_SIZE})" | bc`
else
 export BL_StartAdr=`echo "obase=16;${FLASH_SIZE} - (${BootPages}*${BOOT_PAGE_LEN})" | bc` 
fi
rm ${PROGRAM}x.elf

echo " "


# the C preprocessor make the script file baudcheck.tmp from baudcheck.S with real Parameters
c_params="${CFLAGS} ${COMMON_OPTIONS} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -o baudcheck.tmp -E baudcheck.S"
  if (( ${VerboseLev} > 3 )) ; then echo "${Vgreen}avr-gcc ${c_params}${Vnormal}" ; fi
avr-gcc ${c_params}

if (( $? == 0 )) ; then
 echo " baudcheck.tmp.sh build : OK!" >> ${logfile}
else
 echo " baudcheck.tmp.sh build : FAILED!" >> ${logfile}
fi

# remove \r from baudcheck.tmp to make a bash script
  if (( ${VerboseLev} > 3 )) ; then echo "${Vgreen}cat baudcheck.tmp | tr -d \"\r\" > baudcheck.tmp.sh${Vnormal}" ; fi
cat baudcheck.tmp | tr -d "\r" > baudcheck.tmp.sh

  if (( ${VerboseLev} > 3 )) ; then echo "${Vgreen}source ./baudcheck.sh${Vnormal}" ; fi
source ./baudcheck.tmp.sh
rm -f ./baudcheck.tmp

# Generate the final ${PROGRAM}.elf file at the right Start Address,
# which is the base to generate the ${PROGRAM}_${TARGET}.hex and ${PROGRAM}_${TARGET}.lst files.
echo "${Vgrau}###########################################"
echo "${Vnormal}Boot Loader start address: 0x${BL_StartAdr}${Vgrau} = `echo "ibase=16;${BL_StartAdr}" | bc`"
echo "###########################################${Vnormal}"
c_paramf="${CFLAGS} ${COMMON_OPTIONS} ${LDSECTIONS} -Wl,--section-start=.text=0x${BL_StartAdr} ${LDFLAGS} -o ${PROGRAM}.elf ${PROGRAM}.o ${LIBS}"
  if (( ${VerboseLev} > 2 )) ; then echo "${Vgreen}avr-gcc ${c_paramf}${Vnormal}" ; fi
avr-gcc ${c_paramf}
echo " "

# With the ${Bootpages} file we can set the required BOOTSZ1 and BOOTSZ0 bits, which are
# combined to BOOTSZ (0 for eight pages, 1 for four pages, 2 for two pages and 3 for one page)
# This ${BOOTSZ} is taken by the Makefile.isp file to correct the HFUSE or EFUSE.
# With option VIRTUAL_BOOT_PARTITION ${BOOTSZ} is allways set to 3
#
  if (( ${VerboseLev} > 2 )) ; then echo "${Vgreen}avr-size ${PROGRAM}.elf${Vnormal}" ; fi
avr-size ${PROGRAM}.elf

if (( $? == 0 )) ; then
 echo " avr-size run : OK!" >> ${logfile}
else
 echo " avr-size run : FAILED!" >> ${logfile}
fi

if (( 0${VIRTUAL_BOOT_PARTITION} == 1 )) ; then
  RelVal=`echo "scale=1;${BootPages}*${FLASH_PAGE_SIZE}*100/${FLASH_SIZE}" | bc`
  RelMsg=`echo ", which is ${RelVal}% of Flash Memory"`
  size2know=`echo "${BootPages} * ${FLASH_PAGE_SIZE}" | bc`
  if (( ${BootPages} > 1 )) ; then
   echo -n "Requires ${BootPages} Flash Pages, ${FLASH_PAGE_SIZE} Bytes each${RelMsg}"
  else
   echo -n "Requires ${BootPages} Flash Page of ${FLASH_PAGE_SIZE} Bytes${RelMsg}"
  fi
 if (( 0${FLASH_ERASE_CNT} > 1 )) ; then
    echo ", Cluster of ${FLASH_ERASE_CNT} Pages erasable"
 else
    echo " "
 fi
 if (( ${BOOT_PAGE_LEN} < 129)) ; then
   echo "No Boot Pages present!"
 else
   echo "Boot Pages present, but No Boot Pages used!"
 fi
 BOOTSZ=3 
else
# no virtual boot page
 BOOTSZ=`echo "0+(${pg_anz}<5)+(${pg_anz}<3)+(${pg_anz}<2)" | bc`

 RelVal=`echo "scale=1;${pg_anz}*${BOOT_PAGE_LEN}*100/${FLASH_SIZE}" | bc`
 RelMsg=`echo ", which is ${RelVal}% of Flash Memory"`
 size2know=`echo "${pg_anz} * ${BOOT_PAGE_LEN}" | bc`
 if (( ${pg_anz} > 1 )) ; then
  echo "Requires ${pg_anz} Boot Pages, ${BOOT_PAGE_LEN} Bytes each${RelMsg}"
 else
  echo "Requires ${pg_anz} Boot Page of ${BOOT_PAGE_LEN} Bytes${RelMsg}"
 fi
 echo "${Vinv}BOOTSZ=${BOOTSZ}${Vnormal}, which means ${Vinv}${BootPages} Boot Pages${Vnormal}"
fi

# generate a new .hex and .lst file from the right .elf
ocp_args="-j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex ${PROGRAM}.elf ${PROGRAM}_${TARGET}.hex"
  if (( ${VerboseLev} > 3 )) ; then echo "${Vgreen}avr-objcopy ${ocp_args}${Vnormal}" ; fi
avr-objcopy -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex ${PROGRAM}.elf ${PROGRAM}_${TARGET}.hex

  if (( ${VerboseLev} > 3 )) ; then echo "${Vgreen}avr-objdump -h -S ${PROGRAM}.elf > ${PROGRAM}_${TARGET}.lst${Vnormal}" ; fi
 lstfile="${PROGRAM}_${TARGET}.lst"
avr-objdump -h -S ${PROGRAM}.elf > ${lstfile}

if (( $? == 0 )) ; then
 echo " avr-objdump run : OK!" >> ${logfile}
else
 echo " avr-objdump run : FAILED!" >> ${logfile}
fi

# copy the  .lst and .hex files to files which identify the target
# add some options to the end of the .lst file as comment
echo "; " >> ${lstfile}
echo "; FORCE_WATCHDOG=${FORCE_WATCHDOG}" >> ${lstfile}
echo "; LED_START_FLASHES=${LED_START_FLASHES}" >> ${lstfile}
echo "; LED_DATA_FLASH=${LED_DATA_FLASH}" >> ${lstfile}
echo "; LED=${LED}" >> ${lstfile}
if (( 0${SOFT_UART} > 0 )) ; then
  echo "; SOFT_UART=${SOFT_UART}" >> ${lstfile}
  echo "; UART_RX=${UART_RX}" >> ${lstfile}
  echo "; UART_TX=${UART_TX}" >> ${lstfile}
fi
echo "; UART=${UART}" >> ${lstfile}
echo "; SOURCE_TYPE=${SOURCE_TYPE}" >> ${lstfile}
echo "; SUPPORT_EEPROM=${SUPPORT_EEPROM}" >> ${lstfile}
echo "; MCU_TARGET = ${MCU_TARGET}" >> ${lstfile}
echo "; AVR_FREQ= ${AVR_FREQ}" >> ${lstfile}
echo "; BAUD_RATE=${BAUD_RATE}" >> ${lstfile}

echo "${PROGRAM} for ${TARGET} with AVR ${MCU_TARGET}" >> ${logfile}
echo "Parameter Settings:" >> ${logfile}
echo "AVR_FREQ=${AVRFREQ}" >> ${logfile}
echo "BAUD_RATE=${BAUD_RATE}" >> ${logfile}
echo "UART=${UART}" >> ${logfile}
echo "LED_START_FLASHES=${LED_START_FLASHES}" >> ${logfile}
echo "LED_DATA_FLASH=${LED_DATA_FLASH}" >> ${logfile}
echo "LED=${LED}" >> ${logfile}
echo "SUPPORT_EEPROM=${SUPPORT_EEPROM}" >> ${logfile}
echo "ISP=${ISP}" >> ${logfile}
echo "SOFT_UART=${SOFT_UART}" >> ${logfile}
echo "UART_RX=${UART_RX}" >> ${logfile}
echo "UART_TX=${UART_TX}" >> ${logfile}
echo "C_SOURCE=${C_SOURCE} ,SOURCE_TYPE=${SOURCE_TYPE}" >> ${logfile}
echo "BIGBOOT=${BIGBOOT}" >> ${logfile}
echo "VIRTUAL_BOOT_PARTITION=${VIRTUAL_BOOT_PARTITION}" >> ${logfile}
echo "TIMEOUT_MS=${TIMEOUT_MS}" >> ${logfile}
echo "OSCCAL_CORR=${OSCCAL_CORR}" >> ${logfile}
echo "FORCE_RSTDISBL=${FORCE_RSTDISBL}" >> ${logfile}
echo "save_vect_num=${save_vect_num}" >> ${logfile}
echo " " >> ${logfile}
echo "Bootloader use ${size2know} Bytes of Flash," >> ${logfile}
echo "so the Application must use less than 0x${BL_StartAdr} Bytes of Flash " >> ${logfile}

export TARGET FLASH_SIZE BOOT_PAGE_LEN BOOTSZ BL_StartAdr
export EFUSE HFUSE LFUSE
export PROGRAM TARGET MCU_TARGET AVR_FREQ
if (( 0${ISP} > 0 )) ; then
 source  ./program_target.sh
# make --no-print-directory --warn-undefined-variables -r -f Makefile.isp do_isp
else
 if (( ${VerboseLev} > 3 )) ; then 
   if [ "${EFUSE}" = "" ] ; then
    echo "${Vgelb}Fuse Setting:${Vnormal} LFUSE=0x${LFUSE}, HFUSE=0x${HFUSE}"
   else
    echo "${Vgelb}Fuse Setting:${Vnormal} LFUSE=0x${LFUSE}, HFUSE=0x${HFUSE}, EFUSE=0x${EFUSE}"
   fi
 fi
fi
echo " "
exit 0
