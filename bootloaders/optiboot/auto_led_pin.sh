#!/bin/bash

#set default LED for known AVR targets, if not specified
#is called by build_target.sh
#a run of avr_family.sh is required to build the family name

if [ "${LED}" != "" ] ; then
 return
fi
#LED is undefined!

# first check, if a LED Pin is required
if (( 0${LED_START_FLASHES} == 0 )) && (( 0${LED_DATA_FLASH} == 0 )) ; then
 return ; # No setup for LED Port and Pin required
fi
#set to a default value, according to the processor

if [ "${AVR_FAMILY}" = "attiny84" ] ; then
LED="A4"

elif [ "${AVR_FAMILY}" = "attiny841" ] ; then
LED="A0"

elif [ "${AVR_FAMILY}" = "attiny88" ]  ; then
LED="B5"

elif [ "${AVR_FAMILY}" = "attiny167" ] ; then
LED="A2"

elif [ "${AVR_FAMILY}" = "attiny85" ] ; then
LED="B2"

elif [ "${AVR_FAMILY}" = "attiny861" ] ; then
LED="A0"

elif [ "${AVR_FAMILY}" = "attiny4313" ] ; then
LED="B1"

elif [ "${AVR_FAMILY}" = "attiny1634" ]  ; then
LED="B1"

elif [ "${AVR_FAMILY}" = "atmega1284" ] || [ "${AVR_FAMILY}" = "atmega32" ] ||
     [ "${AVR_FAMILY}" = "atmega8535" ] ; then
LED="B0"

elif [ "${AVR_FAMILY}" = "at90can128" ] ; then
LED="B5"

elif [ "${AVR_FAMILY}" = "at90pwm2" ] ; then
LED="D2"

elif [ "${AVR_FAMILY}" = "at90pwm3" ] ; then
LED="C1"

elif [ "${AVR_FAMILY}" = "atmega32u2" ] ; then
LED="B5"

elif [ "${AVR_FAMILY}" = "atmega32u4" ] ; then
LED="C6"

elif [ "${AVR_FAMILY}" = "atmega128" ] ; then
LED="B7"

elif [ "${AVR_FAMILY}" = "atmega323" ] ; then
LED="B0"

elif [ "${AVR_FAMILY}" = "atmega169" ] ; then
LED="B5"

elif [ "${AVR_FAMILY}" = "atmega645" ] || [ "${AVR_FAMILY}" = "atmega6450" ] ||
     [ "${AVR_FAMILY}" = "atmega649" ] || [ "${AVR_FAMILY}" = "atmega6490" ] ; then
LED="B7"

elif [ "${AVR_FAMILY}" = "atmega2560" ] || [ "${AVR_FAMILY}" = "atmega2561" ] ; then
LED="B7"

elif [ "${AVR_FAMILY}" = "atmega8" ] ||  [ "${AVR_FAMILY}" = "atmega328" ] ; then
LED="B5"
else

 echo "auto_led.sh does ${Vfett}not find a LED preset${Vnormal} for processor ${MCU_TARGET}!"
return
fi
 # LED was unset before and should be set now. Otherwise is the AVR-ID not found!
echo "LED is set to ${LED} ${Vgrau}by auto_led.sh for processor ${MCU_TARGET}${Vnormal}"


