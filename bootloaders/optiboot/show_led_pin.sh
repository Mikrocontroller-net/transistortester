#!/bin/bash

# Look for  a pin description in the database avr_pins.
# Requires LED set to a valid pin in short form like B5 for Port B, Pin No. 5.
# If LED is unset, tries to find a default pin in the avr_pins/${AVR_FAMILY} desription
# Additionally AVR_FAMILY should hold the name of the AVR family,
# which is usually the biggest member like atmega328 for atmega168.
# You can convert the name of the target processor in MCI_TARGET to
# AVR_FAMILY with avr_family.sh .

if (( ${LED_START_FLASHES} == 0 )) && (( ${LED_DATA_FLASH} == 0)) ; then
  echo "${Vgrau}LED-Pin not used!${Vnormal}"
else
  if [ "${LED}" = "" ] ; then
   led_grep_txt="LED"
  else
   led_grep_txt="^p${LED}"
  fi
  if (( ${VerboseLev} > 3 )) ; then echo "rx_zeile=grep --color=never \"${led_grep_txt}\" avr_pins/${AVR_FAMILY}.pins" ; fi
  pin_zeile=`grep --color=never "${led_grep_txt}" avr_pins/${AVR_FAMILY}.pins` 2> /dev/null
  if (( $? == 0 )) ; then
    if [ "${LED}" = "" ] ; then
      nled=`echo ${pin_zeile} | cut -c2-3`
      if (( ${VerboseLev} > 2 )) ; then echo "LED set to ${nrx} in list_led_pins.sh" ; fi
      LED=${nled}
      unset nled
    fi
    pin_desc=`echo "${pin_zeile}" | cut -f3`
    pin_layout=`echo "${pin_zeile}" | cut -f2`
    echo -n "${Vgrau}LED-Pin ${Vnormal}P${LED} ${Vgrau}use Pin ${Vnormal}${pin_layout}"
    if [ "${pin_desc}" = "-" ] || [ "${pin_desc}" = "" ] ; then
     echo " "
    else
     echo "${Vgrau}, with special functions: ${Vnormal}${pin_desc}"
    fi
    unset pin_layout
    unset pin_desc
  else
    echo "list_led_pin.sh has not found \"${led_grep_txt}\" in avr_pins/${AVR_FAMILY}.pins"
    echo "List of availavle pins for ${AVR_FAMILY} group:"
    grep "^p[A-N][0-7]" avr_pins/${AVR_FAMILY}.pins
    exit 1
  fi
  unset pin_zeile
fi
