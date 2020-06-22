#!/bin/bash

# look for  a pin description in the database avr_pins.
if [ "${LED}" = "" ] ; then
 return 1
fi
pin_zeile=`grep --color=never "^p${LED}" avr_pins/${AVR_FAMILY}.pins` 2> /dev/null
if (( $? == 0 )) ; then
  pin_desc=`echo "${pin_zeile}" | cut -f3`
  pin_layout=`echo "${pin_zeile}" | cut -f2`
  echo -n "${Vgrau}LED-Pin ${Vnormal}P${LED} ${Vgrau}use Pin ${Vnormal}${pin_layout}"
  if [ "${pin_desc}" = "-" ] || [ "${pin_desc}" = "" ] ; then
   echo " "
  else
   echo "${Vgrau}, with special functions: ${Vnormal}${pin_desc}"
  fi
fi
