#!/bin/bash
# This is the main build process for a optiboot Bootloader.
# Usually it is called by the makefile, which set up required parameters

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

echo "${Vgelb}List of possible AVR targets:${Vnormal}"
echo " "
echo "atmega8"
echo "atmega32 atmega16"
echo "atmega128 atmega64"
echo "atmega8535 atmega8515"
echo " "
echo "${Vgrau}atmega162"
echo "atmega323 atmega163${Vnormal}"
echo "${Vfett}atmega1284p atmega644${Vgrau}/644a/644p/644pa ${Vnormal}atmega324a${Vgrau}/324p/324pa${Vnormal} atmega164${Vgrau}/164a/164p${Vnormal}"
echo "atmega645${Vgrau}/645a/645p${Vnormal} atmega325${Vgrau}/325a/325p ${Vnormal}atmega165${Vgrau}/165a/165p${Vnormal}"
echo "atmega6450${Vgrau}/6450a/6450p ${Vnormal}atmega3250${Vgrau}/3250a/3250p/3250pa"
echo "${Vfett}atmega328${Vgrau}/328p ${Vnormal}atmega168${Vgrau}/168a/168p ${Vnormal}atmega88${Vgrau}/88p ${Vnormal}atmega48${Vgrau}/48p${Vnormal}"
echo "atmega649${Vgrau}/649p ${Vnormal}atmega329${Vgrau}/329p ${Vnormal}atmega169${Vgrau}/169p${Vnormal}"
echo "atmega6490${Vgrau}/6490p ${Vnormal}atmega3290${Vgrau}/3290p${Vnormal}"
echo "atmega2561 atmega1281 "
echo "${Vfett}atmega2560${Vnormal} atmega1280 atmega640"
echo " "
echo "atmega32u2 atmega16u2 atmega8u2"
echo "atmega32u4 atmega16u4"
echo " "
echo "${Vfett}attiny84 attiny44${Vnormal} attiny24"
echo "attiny841 attiny441"
echo "attiny85 attiny45 attiny25"
echo "attiny861 attiny461 attiny261"
echo "attiny167 attiny87"
echo "${Vfett}attiny88 attiny48${Vnormal}"
echo "attiny4313 attiny2313"
echo "${Vfett}attiny1634${Vnormal}"
echo " "
echo "t90can128 at90can64 at90can32"
echo "at90pwm2 at90pwm2b"
echo "at90pwm3 at90pwm3b"
echo " "
echo "At least one model of each target types are tested! But there is no warranty given!"
