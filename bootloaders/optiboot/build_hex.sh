#!/bin/bash

LIBS="-lc"
export AUTO_BAUD=`echo "(${BAUD_RATE} > 99)" | bc`
echo "baud rate=${BAUD_RATE}, ${AUTO_BAUD}"

# Build of the first object file .o
if (( 0${VIRTUAL_BOOT_PARTITION} == 1 ))
then
 if (( 0${save_vect_num} == 0 ))
 then
c_params="${CFLAGS} ${COMMON_OPTIONS} -DVIRTUAL_BOOT_PARTITION -DLED=p${LED} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -DHFUSE=hex${HFUSE} -DLFUSE=hex${LFUSE} -DBOOT_PAGE_LEN=${BOOT_PAGE_LEN} -c -o ${PROGRAM}.o ${PROGRAM}.${SOURCE_TYPE}"
 else
c_params="${CFLAGS} ${COMMON_OPTIONS} -DVIRTUAL_BOOT_PARTITION -Dsave_vect_num=${save_vect_num} -DLED=p${LED} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -DHFUSE=hex${HFUSE} -DLFUSE=hex${LFUSE} -DBOOT_PAGE_LEN=${BOOT_PAGE_LEN} -c -o ${PROGRAM}.o ${PROGRAM}.${SOURCE_TYPE}"
 fi
else
c_params="${CFLAGS} ${COMMON_OPTIONS} -DLED=p${LED} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -DHFUSE=hex${HFUSE} -DLFUSE=hex${LFUSE} -DBOOT_PAGE_LEN=${BOOT_PAGE_LEN} -c -o ${PROGRAM}.o ${PROGRAM}.${SOURCE_TYPE}"
fi

echo "avr-gcc ${c_params}"
avr-gcc ${c_params}

#  we can determine the size of the loader with optiboot.o for Assembler source input,
#  but for C source we need to do some optimizing  with the binder call (.elf).
#  This call is done without the relolation of the .text section (instruction code).
#  After computing the required size (and the possible relocation address),
#  This x.elf is removed later.

c_paramsx="${CFLAGS} ${COMMON_OPTIONS} ${LDSECTIONS} ${LDFLAGS} -o ${PROGRAM}x.elf ${PROGRAM}.o ${LIBS}"

avr-gcc $c_paramsx

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

if (( 0${VIRTUAL_BOOT_PARTITION} == 1 ))
then
export BootPages=`echo "( ${prog_size}/${FLASH_PAGE_SIZE}/${FLASH_ERASE_CNT} +1) * ${FLASH_ERASE_CNT}" | bc`

else
export BootPages=`echo "${pg_anz} + (${pg_anz}==3 ) + (${pg_anz}==5)*3 + (${pg_anz}==6)*2 + (${pg_anz} == 7)" | bc`

fi


# With the  BootPages and the ${BOOT_PAGE_LEN} or ${FLASH_PAGE_SIZE} we can compute the 
# Start Address BL_StartAdr of the bootloader depending on the ${FLASH_SIZE} 
if (( 0${VIRTUAL_BOOT_PARTITION} == 1 ))
then
export BL_StartAdr=`echo "obase=16;${FLASH_SIZE} - (${BootPages}*${FLASH_PAGE_SIZE})" | bc`
else
export BL_StartAdr=`echo "obase=16;${FLASH_SIZE} - (${BootPages}*${BOOT_PAGE_LEN})" | bc` 
fi
rm ${PROGRAM}x.elf


# the C preprocessor make the script file baudcheck.tmp from baudcheck.S with real Parameters
avr-gcc ${CFLAGS} ${COMMON_OPTIONS} -DUART=0${UART} -DSOFT_UART=0${SOFT_UART} -DUART_RX=p${UART_RX} -DUART_TX=p${UART_TX} -DF_CPU=${AVR_FREQ} -o baudcheck.tmp -E baudcheck.S
# remove \r from baudcheck.tmp to make a bash script
cat baudcheck.tmp | tr -d "\r" > baudcheck.tmp.sh
source ./baudcheck.tmp.sh
rm ./baudcheck.tmp

# Generate the final ${PROGRAM}.elf file at the right Start Address,
# which is the base to generate the ${PROGRAM}_${TARGET}.hex and ${PROGRAM}_${TARGET}.lst files.
echo "###########################################"
echo "Boot Loader start address: 0x${BL_StartAdr} = `echo "ibase=16;${BL_StartAdr}" | bc`"
echo "###########################################"
echo " avr-gcc ${CFLAGS} ${COMMON_OPTIONS} ${LDSECTIONS} -Wl,--section-start=.text=0x${BL_StartAdr} ${LDFLAGS} -o ${PROGRAM}.elf ${PROGRAM}.o ${LIBS}"
avr-gcc ${CFLAGS} ${COMMON_OPTIONS} ${LDSECTIONS} -Wl,--section-start=.text=0x${BL_StartAdr} ${LDFLAGS} -o ${PROGRAM}.elf ${PROGRAM}.o ${LIBS}
echo " "

if (( ${AUTO_BAUD} > 0 ))
then
 if (( 0${SUPPORT_EEPROM} == 0 ))
 then
   echo "${AVR_FREQ} Hz operation with Auto-Baudrate configured."
 else
   echo "${AVR_FREQ} Hz operation with Auto-Baudrate and EEprom support configured."
 fi
else
 if (( 0${SUPPORT_EEPROM} == 0 ))
 then
   echo "${AVR_FREQ} Hz operation with Baudrate ${BAUD_RATE} configured."
 else
   echo "${AVR_FREQ} Hz operation with Baudrate ${BAUD_RATE} and EEprom support configured."
 fi
fi

# With the ${Bootpages} file we can set the required BOOTSZ1 and BOOTSZ0 bits, which are
# combined to BOOTSZ (0 for eight pages, 1 for four pages, 2 for two pages and 3 for one page)
# This ${BOOTSZ} is taken by the Makefile.isp file to correct the HFUSE or EFUSE.
# With option VIRTUAL_BOOT_PARTITION ${BOOTSZ} is allways set to 3
#
avr-size ${PROGRAM}.elf

if (( 0${VIRTUAL_BOOT_PARTITION} == 1 ))
then
  echo -n "Requires ${BootPages} Flash Pages, ${FLASH_PAGE_SIZE} Bytes each, which is `echo "scale=1;${BootPages}*${FLASH_PAGE_SIZE}*100/${FLASH_SIZE}" | bc`% of Flash Memory"
 if (( 0${FLASH_ERASE_CNT} > 1 ))
 then
    echo ", Cluster of ${FLASH_ERASE_CNT} Pages erasable"
 else
    echo " "
 fi
 if (( ${BOOT_PAGE_LEN} < 129))
 then
   echo "No Boot Pages present!"
 else
   echo "Boot Pages present, but No Boot Pages used!"
 fi
   BOOTSZ=3 
else
# no virtual boot page
 BOOTSZ=`echo "0+(${pg_anz}<5)+(${pg_anz}<3)+(${pg_anz}<2)" | bc`

 echo "Requires ${pg_anz} Boot Pages, ${BOOT_PAGE_LEN} Bytes each, which is `echo "scale=1;${pg_anz}*${BOOT_PAGE_LEN}*100/${FLASH_SIZE}" | bc`% of Flash Memory"
 echo "BOOTSZ= ${BOOTSZ}, which means ${BootPages} Boot Pages"
fi

# generate a new .hex and .lst file from the right .elf
avr-objcopy -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex ${PROGRAM}.elf ${PROGRAM}_${TARGET}.hex
avr-objdump -h -S ${PROGRAM}.elf > ${PROGRAM}_${TARGET}.lst

# copy the  .lst and .hex files to files which identify the target
# add some options to the end of the .lst file as comment
echo "; " >> ${PROGRAM}_${TARGET}.lst
echo "; FORCE_WATCHDOG=${FORCE_WATCHDOG}" >> ${PROGRAM}_${TARGET}.lst
echo "; LED_START_FLASHES=${LED_START_FLASHES}" >> ${PROGRAM}_${TARGET}.lst
echo "; LED_DATA_FLASH=${LED_DATA_FLASH}" >> ${PROGRAM}_${TARGET}.lst
echo "; LED=${LED}" >> ${PROGRAM}_${TARGET}.lst
if (( 0${SOFT_UART} > 0 ))
then
  echo "; SOFT_UART=${SOFT_UART}" >> ${PROGRAM}_${TARGET}.lst
  echo "; UART_RX=${UART_RX}" >> ${PROGRAM}_${TARGET}.lst
  echo "; UART_TX=${UART_TX}" >> ${PROGRAM}_${TARGET}.lst
fi
echo "; UART=${UART}" >> ${PROGRAM}_${TARGET}.lst
echo "; SOURCE_TYPE=${SOURCE_TYPE}" >> ${PROGRAM}_${TARGET}.lst
echo "; SUPPORT_EEPROM=${SUPPORT_EEPROM}" >> ${PROGRAM}_${TARGET}.lst
echo "; MCU_TARGET = ${MCU_TARGET}" >> ${PROGRAM}_${TARGET}.lst
echo "; AVR_FREQ= ${AVR_FREQ}" >> ${PROGRAM}_${TARGET}.lst
echo "; BAUD_RATE=${BAUD_RATE}" >> ${PROGRAM}_${TARGET}.lst

export TARGET FLASH_SIZE BOOT_PAGE_LEN BOOTSZ BL_StartAdr
export EFUSE HFUSE LFUSE
export PROGRAM TARGET MCU_TARGET AVR_FREQ
if (( 0${ISP} > 0 ))
then
 source  ./program_target.sh
# make --no-print-directory --warn-undefined-variables -r -f Makefile.isp do_isp
else
 if [ "${EFUSE}" = "" ]
 then
 echo "Fuse Setting: LFUSE=0x${LFUSE}, HFUSE=0x${HFUSE}"
 else
 echo "Fuse Setting: LFUSE=0x${LFUSE}, HFUSE=0x${HFUSE}, EFUSE=0x${EFUSE}"
 fi
fi
exit 0
