#!/bin/bash

frequency=16000000
looptics=9
calltics=7
while getopts hc:f:s: opt
do
   case $opt in
       c) calltics="${OPTARG}";;
       f) frequency="${OPTARG}";;
       h) echo "calc_UART_baud.sh shows you the expected difference to some"
	  echo "higher standard baud rates for hardware UART and"
	  echo "software UART with different optiboot configurations."
	  echo " "
	  echo "Optionen:" 
	  echo " -c <nr> Number of tics for call/return ,usually 7, but  9 for AVRs with more than 128k flash" 
	  echo " -f <nr> operating frequency in Hz"
	  echo " -s <nr> number of tics for serial IO software loop, 9 tics or 10 tics"
	  echo " "
	  echo "The delay loop can take 6, 8, 10 or 16 tics for one pass"
	  exit 0 ;;
       s) looptics="${OPTARG}";;

   esac
done

Vgrau=`tput setaf 245`
Vgelb=`tput setaf 11`
Vrot=`tput setaf 9`
Vnormal=`tput sgr0`

baudtab=(1000000 500000 250000 230400 115200 57600 38400 19200 9600 4800 2400)
BaseBaud=`echo "${frequency} / 8" | bc`

echo "Higher Baudrates for clock frequency ${frequency}Hz :"
 for (( ti=0 ; ti < 8 ; ti++ ))
 do
    baud=${baudtab[${ti}]}
    div=`echo "(${BaseBaud}  + ${baud}/2)/ ${baud}" | bc`
    tics=`echo "${frequency} / ${baud}" | bc`
    tics_wp=`echo "scale=1;${frequency} / ${baud}" | bc`
    echo "STD-Baud=${baud} with ${tics_wp} clock tics :"
    if [ ${div} -gt 0  ]
    then
      realbaud=`echo "${BaseBaud} / ${div}" | bc`
      diff=`echo "scale=1; (100*( ${realbaud} - ${baud} ) ) / ${baud}" | bc`
      toobig=`echo "((${diff} < -2) || (${diff} > 2)) + ((${diff} < -4) || (${diff} > 4))" | bc`
      Vmarker="${Vnormal}"
      if [ ${toobig} -eq 1 ]
      then
	Vmarker="${Vgelb}"
      fi
      if [ ${toobig} -eq 2 ]
      then
	Vmarker="${Vrot}"
      fi
      echo -e " ${div}\tHW-real  =${realbaud}, ${Vmarker}diff=${diff}%${Vnormal}"
    fi
    loop_passes6=`echo "(((${tics} - ${looptics} - (${calltics}*2) + 3) / 6)/2)*2" | bc`
    if [ ${loop_passes6} -gt 0 ]
    then
      realbaud6=`echo "${frequency} / (${loop_passes6}  * 6 + ${looptics} + (${calltics}*2))" | bc`
      diff6=`echo "scale=1; (100*( ${realbaud6} - ${baud} ) ) / ${baud}" | bc`
      Vmarker="${Vnormal}"
      toobig=`echo "((${diff6} < -2) || (${diff6} > 2)) + ((${diff6} < -4) || (${diff6} > 4))" | bc`
      if [ ${toobig} -eq 1 ] 
      then
	Vmarker="${Vgelb}"
      fi
      if [ ${toobig} -eq 2 ] 
      then
	Vmarker="${Vrot}"
      fi
      echo -e " ${loop_passes6}\tSW-real6 =${realbaud6}, ${Vmarker}diff=${diff6}%${Vnormal}"
    fi
    loop_passes8=`echo "(((${tics} - ${looptics} - (${calltics}*2) + 4) / 8)/2)*2" | bc`
    if [ ${loop_passes8} -gt 0 ]
    then
      realbaud8=`echo "${frequency} / (${loop_passes8}  * 8 + ${looptics} + (${calltics}*2))" | bc`
      diff8=`echo "scale=1; (100*( ${realbaud8} - ${baud} ) ) / ${baud}" | bc`
      toobig=`echo "((${diff8} < -2) || (${diff8} > 2)) + ((${diff8} < -4) || (${diff8} > 4))" | bc`
      Vmarker="${Vnormal}"
      if [ ${toobig} -eq 1 ] 
      then
	Vmarker="${Vgelb}"
      fi
      if [ ${toobig} -eq 2 ] 
      then
	Vmarker="${Vrot}"
      fi
      echo -e "${Vgrau} ${loop_passes8}\tSW_real8 =${realbaud8}, ${Vmarker}diff=${diff8}%${Vnormal}"
    fi
    loop_passes10=`echo "(((${tics} - ${looptics} - ((${calltics}+1)*2) + 5) / 10)/2)*2" | bc`
    if [ ${loop_passes10} -gt 0 ]
    then
      realbaud10=`echo "${frequency} / (${loop_passes10} * 10 + ${looptics} + (${calltics}*2))" | bc`
      diff10=`echo "scale=1; (100*( ${realbaud10} - ${baud} ) ) / ${baud}" | bc`
      toobig=`echo "((${diff10} < -2) || (${diff10} > 2)) + ((${diff10} < -4) || (${diff10} > 4))" | bc`
      Vmarker="${Vnormal}"
      if [ ${toobig} -eq 1 ] 
      then
	Vmarker="${Vgelb}"
      fi
      if [ ${toobig} -eq 2 ] 
      then
	Vmarker="${Vrot}"
      fi
      echo -e " ${loop_passes10}\tSW_real10=${realbaud10}, ${Vmarker}diff=${diff10}%${Vnormal}"
    fi
    loop_passes16=`echo "(((${tics} - ${looptics} - ((${calltics}+1)*2) + 8) / 16)/2)*2" | bc`
    if [ ${loop_passes16} -gt 0 ]
    then
      realbaud16=`echo "${frequency} / (${loop_passes16} * 16 + ${looptics} + (${calltics}*2))" | bc`
      diff16=`echo "scale=1; (100*( ${realbaud16} - ${baud} ) ) / ${baud}" | bc`
      toobig=`echo "((${diff16} < -2) || (${diff16} > 2)) + ((${diff16} < -4) || (${diff16} > 4))" | bc`
      Vmarker="${Vnormal}"
      if [ ${toobig} -eq 1 ] 
      then
	Vmarker="${Vgelb}"
      fi
      if [ ${toobig} -eq 2 ] 
      then
	Vmarker="${Vrot}"
      fi
      echo -e "${Vgrau} ${loop_passes16}\tSW_real16=${realbaud16}, ${Vmarker}diff=${diff16}%${Vnormal}"
    fi
    echo " "
 done
