#!/bin/bash

# Look for definitions in the database avr_params.def.
# AVR_FAMILY with avr_family.sh .
# the entries in avr_params.def are separated by <Tab>
# 1. Entry  is the Target name followed by a blank to anable a grep search
# 2. Entry is the Flash Size in bytes,  FLASH_SIZE
# 3. Entry is the page size of the flash memory,  FLASH_PAGE_SIZE
# 4. Entry is the length  of a boot page,  BOOT_PAGE_LEN
# 5. Entry is a combined value, 0=<no virtual Boot>, >0=<virtual Boot and No. of erasable flash pages> 
# 6. Entry specifies a default operating frequency,  AVR_FREQ
# 7. Entry specifies a default Baud rate of serial interface,  BAUD_RATE
# 8. Entry specifies a default setting for the high fuse,  HFUSE
# 9. Entry specifies a default setting for the low fuse,  LFUSE
# 10. Entry specifies a default setting for the extended fuse,  EFUSE
#  VIRTUAL_BOOT_PARTITION and PAGE_ERASE_CNT
# The entries 2,3,4 can not be defined by user!
# The Entry 5 is only changeable to 1 by user if entry in avr_params.def is 0 !
# the other Enries 5-9 are changeable by user

if [ "${MCU_TARGET}" = "" ] ; then
 return 1
fi
avr_zeile=`grep --color=never "^${MCU_TARGET} " avr_params.def` 2> /dev/null
if (( $? == 0 )) ; then
  FLASH_SIZE=`echo "${avr_zeile}" | cut -f2`
  FLASH_PAGE_SIZE=`echo "${avr_zeile}" | cut -f3`
  BOOT_PAGE_LEN=`echo "${avr_zeile}" | cut -f4`
  my_vb=`echo "${avr_zeile}" | cut -f5`
  my_uarts=`echo "${avr_zeile}" | cut -f6`
  my_frq=`echo "${avr_zeile}" | cut -f7`
  my_bd=`echo "${avr_zeile}" | cut -f8`
  my_hf=`echo "${avr_zeile}" | cut -f9`
  my_lf=`echo "${avr_zeile}" | cut -f10`
  my_ef=`echo "${avr_zeile}" | cut -f11`
  if (( ${my_vb} > 0 )) ; then
    VIRTUAL_BOOT_PARTITION=${my_vb}
  elif (( 0${VIRTUAL_BOOT_PARTITION} != 1 )) ; then
    VIRTUAL_BOOT_PARTITION=0
  fi
  if (( ${my_uarts} == 0 )) ; then
    SOFT_UART=1
    if (( ${VerboseLev} > 3 )) ; then echo "SOFT_UART=1 is set in get_avr_params.sh, no Hardware!" ; fi
  fi
  # all other parameters are user changeable
  if [ "${AVR_FREQ}" == "" ] ; then
    AVR_FREQ=${my_frq}
    if (( ${VerboseLev} > 3 )) ; then echo "set AVR_FREQ to default ${AVR_FREQ} in get_avr_params.sh" ; fi
  fi
  if [ "${BAUD_RATE}" == "" ] ; then
    BAUD_RATE=${my_bd}
    if (( ${VerboseLev} > 3 )) ; then echo "set BAUD_RATE to default ${BAUD_RATE} in get_avr_params.sh" ; fi
  fi
  if [ "${HFUSE}" == "" ] ; then
    HFUSE=${my_hf}
    if (( ${VerboseLev} > 3 )) ; then echo "set HFUSE to default ${HFUSE} in get_avr_params.sh" ; fi
  fi
  if [ "${LFUSE}" == "" ] ; then
    LFUSE=${my_lf}
    if (( ${VerboseLev} > 3 )) ; then echo "set LFUSE to default ${LFUSE} in get_avr_params.sh" ; fi
  fi
  if [ "${EFUSE}" == "" ] ; then
    if [ "${myef}" != "-" ] ; then
          EFUSE=${my_lf}
	  if (( ${VerboseLev} > 3 )) ; then echo "set EFUSE to default ${EFUSE} in get_avr_params.sh" ; fi
    fi
  fi
else
 echo "File avr_params.def not found!"
fi
