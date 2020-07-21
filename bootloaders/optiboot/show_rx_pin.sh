#!/bin/bash

# Look for a RX pin description in the database avr_pins.
# Requires UART_RX set to a valid pin in short form like B5 for Port B, Pin No. 5.
# If a build in hardware UART is used, the RXD Port is given
# with the AVR model by the manufactor.
# If more than one hardware UART is present at your  AVR model,
# you can select the UART with the UART number.
# If you select a Software-UART, the port setting is the same as 
# used by the hardware, but you can override by setting a different UART_RX port
# Additionally AVR_FAMILY should hold the name of the AVR family,
# which is usually the biggest member like atmega328 for atmega168.
# You can convert the name of the target processor in MCI_TARGET to
# AVR_FAMILY with avr_family.sh .
# Please note, that all entries in avr_pins let the grep search
# find at lease one RX port. For devices without a hardware UART,
# the RX entry is listed in a unused column.

if (( ${my_uarts} > 1 )) ; then
  # more than one UART in this device RXD/TDX is followed by a digit
  if [ "${UART_RX}" = "" ] ; then
    rx_grep_txt="RXD${UART}"	  
  else
    rx_grep_txt="^p${UART_RX}"
  fi
else
  if [ "${UART_RX}" = "" ] ; then
    rx_grep_txt="RXD"
  else
    rx_grep_txt="^p${UART_RX}"
  fi
fi
if (( ${VerboseLev} > 3 )) ; then echo "rx_zeile=grep --color=never \"${rx_grep_txt}\" avr_pins/${AVR_FAMILY}.pins" ; fi
rx_zeile=`grep --color=never "${rx_grep_txt}" avr_pins/${AVR_FAMILY}.pins` 2> /dev/null
if (( $? == 0 )) ; then
  if [ "${UART_RX}" = "" ] ; then
     nrx=`echo ${rx_zeile} | cut -c2-3`
     if (( ${VerboseLev} > 2 )) ; then
       if [ "${LANGUAGE}" == "de_DE" ] ; then
       echo "UART_RX wird in list_rx_pins.sh auf ${nrx} gesetzt." 
       else
       echo "UART_RX set to ${nrx} in list_rx_pins.sh" 
       fi
     fi
     UART_RX=${nrx}
     unset nrx
  fi
  rx_desc=`echo "${rx_zeile}" | cut -f3`
  rx_layout=`echo "${rx_zeile}" | cut -f2`
  if [ "${LANGUAGE}" == "de_DE" ] ; then
    echo -n "${Vgrau}RX-Pin ${Vnormal}P${UART_RX} ${Vgrau}benutzt Pin ${Vnormal}${rx_layout}"
  else
    echo -n "${Vgrau}RX-Pin ${Vnormal}P${UART_RX} ${Vgrau}use Pin ${Vnormal}${rx_layout}"
  fi
  if [ "${rx_desc}" = "-" ] || [ "${rx_desc}" = "" ] ; then
   echo "."
  else
   if [ "${LANGUAGE}" == "de_DE" ] ; then
     echo "${Vgrau}, mit Spezialfunktionen: ${Vnormal}${rx_desc}."
   else
     echo "${Vgrau}, with special functions: ${Vnormal}${rx_desc}."
   fi
  fi
  unset rx_desc
  unset rx_layout
else
  if [ "${LANGUAGE}" == "de_DE" ] ; then
    echo "list_rx_pin.sh hat ${rx_grep_txt} in avr_pins/${AVR_FAMILY}.pins nicht gefunden."
    echo "Liste der verfügbaren Pinne der ${AVR_FAMILY} Gruppe:"
  else
    echo "list_rx_pin.sh has not found ${rx_grep_txt} in avr_pins/${AVR_FAMILY}.pins"
    echo "List of available pins for ${AVR_FAMILY} group:"
  fi
  grep "^p[A-N][0-7]" avr_pins/${AVR_FAMILY}.pins
  exit 1
fi
unset rx_zeile
