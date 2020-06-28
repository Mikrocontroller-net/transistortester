#!/bin/bash
# this avr_family script will find a family name for AVR processors with the same pin layout
# They are listed in the file avr_family.def
# AVR_FAMILY with hold the family name or the MCU_TARGET name, when no entry is found. 

if (( ${VerboseLev} > 3 )) ; then echo "fam_zeile=grep --color=never \"${MCU_TARGET}\s\" Avr_family.def" ; fi
fam_zeile=`grep --color=never "${MCU_TARGET}\s" Avr_family.def` 2> /dev/null
if (( $? == 0 )) ; then
  AVR_FAMILY=`echo "${fam_zeile}" | cut -f1`
else
  AVR_FAMILY="${MCU_TARGET}"
fi
unset pin_zeile
