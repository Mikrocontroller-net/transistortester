#!/bin/bash

# Look for  a pin description in the database avr_pins.
# Requires WRITE_PROTECT_PIN set to a valid pin in short form like B5 for Port B, Pin No. 5.

if [ "${WRITE_PROTECT_PIN}" = "" ] ; then
 return 0
fi
wrpp_grep_txt="^p${WRITE_PROTECT_PIN}"

if (( ${VerboseLev} > 3 )) ; then echo "wrpp_zeile=grep --color=never \"${wrpp_grep_txt}\" avr_pins/${AVR_FAMILY}.pins" ; fi
wrpp_zeile=`grep --color=never "${wrpp_grep_txt}" avr_pins/${AVR_FAMILY}.pins` 2> /dev/null
if (( $? == 0 )) ; then
  wrpp_desc=`echo "${wrpp_zeile}" | cut -f3`
  wrpp_layout=`echo "${wrpp_zeile}" | cut -f2`
  echo -n "${Vgrau}Pin ${Vnormal}P${WRITE_PROTECT_PIN} ${Vgrau}is used as Write Protect input at Pin ${Vnormal}${wrpp_layout}"
  if [ "${wrpp_desc}" = "-" ] || [ "${wrpp_desc}" = "" ] ; then
   echo " "
  else
   echo "${Vgrau}, with special functions: ${Vnormal}${wrpp_desc}"
  fi
  unset wrpp_desc
  unset wrpp_layout
else
  echo "show_write_protect.sh has not found ${wrpp_grep_txt} in avr_pins/${AVR_FAMILY}.pins"
  echo "List of available pins for ${AVR_FAMILY} group:"
  grep "^p[A-N][0-7]" avr_pins/${AVR_FAMILY}.pins
  exit 1
fi
unset wrpp_zeile
