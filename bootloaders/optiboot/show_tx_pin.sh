#!/bin/bash

# Look for  a pin description in the database avr_pins.
# Requires UART_TX set to a valid pin in short form like B5 for Port B, Pin No. 5.
# If a build in hardware UART is used, the TXD Port is given
# with the AVR model by the manufactor.
# If more than one hardware UART is present at your  AVR model,
# you can select the UART with the UART number.
# If you select a Software-UART, the port setting is the same as 
# used by the hardware, but you can override by setting a different UART_TX port.
# Additionally AVR_FAMILY should hold the name of the AVR family,
# which is usually the biggest member like atmega328 for atmega168.
# You can convert the name of the target processor in MCI_TARGET to
# AVR_FAMILY with avr_family.sh .
# Please note, thall all entries in avr_pins let the grep search
# find at lease one TX port. For devices without a hardware UART,
# the TX entry is listed in a unused column.

if (( ${my_uarts} > 1 )) ; then
  # more than one UART in this device RXD/TDX is followed by a digit
  if [ "${UART_TX}" = "" ] ; then
    tx_grep_txt="TXD${UART}"	  
  else
    tx_grep_txt="^p${UART_TX}"	  
  fi
else
  if [ "${UART_TX}" = "" ] ; then
    tx_grep_txt="TXD"	  
  else
    tx_grep_txt="^p${UART_TX}"
  fi
fi
if (( ${VerboseLev} > 3 )) ; then echo "tx_zeile=grep --color=never \"${tx_grep_txt}\" avr_pins/${AVR_FAMILY}.pins" ; fi
tx_zeile=`grep --color=never "${tx_grep_txt}" avr_pins/${AVR_FAMILY}.pins` 2> /dev/null
if (( $? == 0 )) ; then
  if [ "${UART_TX}" = "" ] ; then
     ntx=`echo ${tx_zeile} | cut -c2-3`
     if (( ${VerboseLev} > 2 )) ; then
       if [ "${LANGUAGE}" == "de_DE" ] ; then
         echo "UART_TX wird in list_tx_pins.sh auf ${ntx} gesetzt." 
       else
         echo "UART_TX set to ${ntx} in list_tx_pins.sh." 
       fi
     fi
     UART_TX=${ntx}
     unset ntx
  fi
  tx_desc=`echo "${tx_zeile}" | cut -f3`
  tx_layout=`echo "${tx_zeile}" | cut -f2`
  if [ "${LANGUAGE}" == "de_DE" ] ; then
    echo -n "${Vgrau}TX-Pin ${Vnormal}P${UART_TX} ${Vgrau}benutzt Pin ${Vnormal}${tx_layout}"
  else
    echo -n "${Vgrau}TX-Pin ${Vnormal}P${UART_TX} ${Vgrau}use Pin ${Vnormal}${tx_layout}"
  fi
  if [ "${tx_desc}" = "-" ] || [ "${tx_desc}" = "" ] ; then
   echo "."
  else
   if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "${Vgrau}, mit Spezialfunktionen: ${Vnormal}${tx_desc}."
   else
     echo "${Vgrau}, with special functions: ${Vnormal}${tx_desc}."
   fi
  fi
  unset tx_desc
  unset tx_layout
else
  if [ "${LANGUAGE}" == "de_DE" ] ; then
    echo "list_tx_pin.sh hat ${rx_grep_txt} in avr_pins/${AVR_FAMILY}.pins nicht gefunden."
    echo "Liste der verfügbaren Pinne der ${AVR_FAMILY} Gruppe:"
  else
    echo "list_tx_pin.sh has not found ${tx_grep_txt} in avr_pins/${AVR_FAMILY}.pins"
    echo "List of available pins for ${AVR_FAMILY} group:"
  fi
  grep "^p[A-N][0-7]" avr_pins/${AVR_FAMILY}.pins
  exit 1
fi
unset tx_zeile
