#!/bin/bash

# avr_family.sh build a family name for different pin equal MCU_TARGET

#set to a default value, according to the processor
# a explicit entry is only required if a group has more than one processor
# with identical pin layout

if [ "${MCU_TARGET}" = "attiny84" ] ||  [ "${MCU_TARGET}" = "attiny44" ] ||
   [ "${MCU_TARGET}" = "attiny24" ] ||  [ "${MCU_TARGET}" = "attiny84a" ] || 
   [ "${MCU_TARGET}" = "attiny44a" ] ||  [ "${MCU_TARGET}" = "attiny24a" ] ; then
AVR_FAMILY="attiny84"

elif [ "${MCU_TARGET}" = "attiny441" ] ||  [ "${MCU_TARGET}" = "attiny841" ] ; then
AVR_FAMILY="attiny841"

elif [ "${MCU_TARGET}" = "attiny88" ] ||  [ "${MCU_TARGET}" = "attiny48" ] ; then
AVR_FAMILY="attiny88"

elif [ "${MCU_TARGET}" = "attiny87" ] ||  [ "${MCU_TARGET}" = "attiny167" ] ; then
AVR_FAMILY="attiny167"

elif [ "${MCU_TARGET}" = "attiny25" ] ||  [ "${MCU_TARGET}" = "attiny45" ] ||
   [ "${MCU_TARGET}" = "attiny85" ] ; then
AVR_FAMILY="attiny85"

elif [ "${MCU_TARGET}" = "attiny861" ] ||  [ "${MCU_TARGET}" = "attiny461" ] ||
   [ "${MCU_TARGET}" = "attiny261" ] ||  [ "${MCU_TARGET}" = "attiny861a" ] || 
   [ "${MCU_TARGET}" = "attiny461a" ] ||  [ "${MCU_TARGET}" = "attiny261a" ] ; then
AVR_FAMILY="attiny861"

elif [ "${MCU_TARGET}" = "attiny4313" ] ||  [ "${MCU_TARGET}" = "attiny2313" ] ||
   [ "${MCU_TARGET}" = "attiny4313a" ] ||  [ "${MCU_TARGET}" = "attiny2313a" ] ; then
AVR_FAMILY="attiny4313"

elif [ "${MCU_TARGET}" = "atmega164" ] || [ "${MCU_TARGET}" = "atmega164a" ] ||
   [ "${MCU_TARGET}" = "atmega164p" ] || [ "${MCU_TARGET}" = "atmega164pa" ]  || 
   [ "${MCU_TARGET}" = "atmega324" ] || [ "${MCU_TARGET}" = "atmega324a" ] ||
   [ "${MCU_TARGET}" = "atmega324p" ] || [ "${MCU_TARGET}" = "atmega324pa" ]  || 
   [ "${MCU_TARGET}" = "atmega644" ]    || [ "${MCU_TARGET}" = "atmega644p" ] || 
   [ "${MCU_TARGET}" = "atmega644a" ]   || [ "${MCU_TARGET}" = "atmega644pa" ] || 
   [ "${MCU_TARGET}" = "atmega1284" ]   || [ "${MCU_TARGET}" = "atmega1284p" ] ||
   [ "${MCU_TARGET}" = "atmega1284a" ] ; then
AVR_FAMILY="atmega1284"

elif [ "${MCU_TARGET}" = "atmega16" ] || [ "${MCU_TARGET}" = "atmega16a" ] ||
   [ "${MCU_TARGET}" = "atmega32" ] || [ "${MCU_TARGET}" = "atmega32a" ] ; then
AVR_FAMILY="atmega32"

elif [ "${MCU_TARGET}" = "at90can32" ] ||  [ "${MCU_TARGET}" = "at90can64" ] ||
   [ "${MCU_TARGET}" = "at90can128" ] ; then
AVR_FAMILY="at90can128"

elif [ "${MCU_TARGET}" = "at90pwm2" ] ||  [ "${MCU_TARGET}" = "at90pwm2b" ] ; then
AVR_FAMILY="at90pwm2"

elif [ "${MCU_TARGET}" = "at90pwm3" ] ||  [ "${MCU_TARGET}" = "at90pwm3b" ] ; then
AVR_FAMILY="at90pwm3"

elif [ "${MCU_TARGET}" = "atmega8u2" ] ||  [ "${MCU_TARGET}" = "atmega16u2" ] ||
   [ "${MCU_TARGET}" = "atmega32u2" ] ; then
AVR_FAMILY="atmega32u2"

elif [ "${MCU_TARGET}" = "atmega16u4" ] || [ "${MCU_TARGET}" = "atmega32u4" ] ; then
AVR_FAMILY="atmega32u4"

elif [ "${MCU_TARGET}" = "atmega64" ] ||  [ "${MCU_TARGET}" = "atmega64a" ] ||
   [ "${MCU_TARGET}" = "atmega128" ] ; then
AVR_FAMILY="atmega128"

elif [ "${MCU_TARGET}" = "atmega163" ] || [ "${MCU_TARGET}" = "atmega323" ] ; then
AVR_FAMILY="atmega323"


elif [ "${MCU_TARGET}" = "atmega165" ] ||  [ "${MCU_TARGET}" = "atmega165p" ] ||
   [ "${MCU_TARGET}" = "atmega165a" ] || [ "${MCU_TARGET}" = "atmega165pa" ] || 
   [ "${MCU_TARGET}" = "atmega325" ] ||  [ "${MCU_TARGET}" = "atmega325p" ] || 
   [ "${MCU_TARGET}" = "atmega325a" ] || [ "${MCU_TARGET}" = "atmega325pa" ] || 
   [ "${MCU_TARGET}" = "atmega645" ] || [ "${MCU_TARGET}" = "atmega645p" ] || 
   [ "${MCU_TARGET}" = "atmega645a" ] || [ "${MCU_TARGET}" = "atmega645pa" ] ; then
AVR_FAMILY="atmega645"

elif [ "${MCU_TARGET}" = "atmega3250" ] || [ "${MCU_TARGET}" = "atmega3250p" ] ||
   [ "${MCU_TARGET}" = "atmega3250a" ] || [ "${MCU_TARGET}" = "atmega3250pa" ] || 
   [ "${MCU_TARGET}" = "atmega6450" ] || [ "${MCU_TARGET}" = "atmega6450p" ] || 
   [ "${MCU_TARGET}" = "atmega6450a" ] || [ "${MCU_TARGET}" = "atmega6450pa" ] ; then
AVR_FAMILY="atmega6450"

elif [ "${MCU_TARGET}" = "atmega169" ] ||  [ "${MCU_TARGET}" = "atmega169p" ] ||
   [ "${MCU_TARGET}" = "atmega329" ] || [ "${MCU_TARGET}" = "atmega329p" ] ||
   [ "${MCU_TARGET}" = "atmega329a" ] || [ "${MCU_TARGET}" = "atmega329pa" ] ||
   [ "${MCU_TARGET}" = "atmega649" ] || [ "${MCU_TARGET}" = "atmega649p" ] ||
   [ "${MCU_TARGET}" = "atmega649a" ] ; then
AVR_FAMILY="atmega649"

elif [ "${MCU_TARGET}" = "atmega3290" ] || [ "${MCU_TARGET}" = "atmega3290p" ] ||
   [ "${MCU_TARGET}" = "atmega3290a" ] || [ "${MCU_TARGET}" = "atmega3290pa" ] ||
   [ "${MCU_TARGET}" = "atmega6490" ] || [ "${MCU_TARGET}" = "atmega6490p" ] ||
   [ "${MCU_TARGET}" = "atmega6490a" ] ; then
AVR_FAMILY="atmega6490"

elif [ "${MCU_TARGET}" = "atmega640" ] ||  [ "${MCU_TARGET}" = "atmega1280" ] ||
   [ "${MCU_TARGET}" = "atmega2560" ] ; then
AVR_FAMILY="atmega2560"
elif [ "${MCU_TARGET}" = "atmega1281" ] || [ "${MCU_TARGET}" = "atmega2561" ] ; then
AVR_FAMILY="atmega2561"

elif [ "${MCU_TARGET}" = "atmega8" ] ; then
AVR_FAMILY="atmega8"
elif [ "${MCU_TARGET}" = "atmega48" ] || [ "${MCU_TARGET}" = "atmega48p" ] ||
   [ "${MCU_TARGET}" = "atmega88" ] || [ "${MCU_TARGET}" = "atmega88p" ] ||
   [ "${MCU_TARGET}" = "atmega168" ] || [ "${MCU_TARGET}" = "atmega168a" ] ||
   [ "${MCU_TARGET}" = "atmega168p" ] || [ "${MCU_TARGET}" = "atmega328" ] ||
   [ "${MCU_TARGET}" = "atmega328p" ] ; then
AVR_FAMILY="atmega328"
else
  if (( ${VerboseLev} > 3 )) ; then echo "Warning, no AVR_FAMILY found for ${MCU_TARGET} by avr_family.sh" ; fi
  AVR_FAMILY="${MCU_TARGET}"
fi
if (( ${VerboseLev} > 3 )) ; then echo "AVR_FAMILY set to ${AVR_FAMILY} for ${MCU_TARGET} in avr_family.sh" ; fi



