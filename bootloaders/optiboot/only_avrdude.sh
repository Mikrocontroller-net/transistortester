#!/bin/bash
#
# K.-H. Kübbeler (kh_kuebbeler@web.de)  Jun 2020
# 
# This only_avrdude.sh script only call the avrdude program with
# the parameter list given in the variable DUDE_PARAMS .
# Additionally the variable ISPPORT should be set to the port  of your
# ISP-programmer.
# After an error return from avrdude, a error message is shown.
# At a Linux systen  the script find_serials.sh search for
# serial devices. The accessability of the devices for you
# are checked also.

echo "avrdude ${DUDE_PARAMS}"
avrdude ${DUDE_PARAMS}
# avrdude can tell more about the operation with the -v option:
# avrdude -v ${DUDE_PARAMS}
DUDE_FAIL=$?
if (( ${DUDE_FAIL} != 0 ))
then
  echo "#-#-#-#-#-#-#-#"
  echo "*** avrdude *** failed with return value ${DUDE_FAIL}!"
  echo "#-#-#-#-#-#-#-#"
  echo "If avrdude has not started with the Programmer communication,"
  echo "this can be caused by a wrong ISPPORT [now: ${ISPPORT}] setting!" 
  if [ "${OS}" = "Windows_NT" ]
  then
    echo "You can try to find any device with the device manager to check for"
    echo "your ${ISPPORT} setting."
    echo "Usually a serial device like COMx is taken by the ISP-programmer."
    echo "The device manager selection can be called with the two key <Windows><x> sequence."
  else
    # only for Linux
    echo "Looking for serial Ports at your Linux system:"
    source ./find_serials.sh
  fi
  echo "If the avrdude has started the communication to your target with the ISP-programmer,"
  echo "your ISPPORT setting should be OK. So you have to check the cable connection"
  echo "between the ISP-Programmer and your AVR target."
  echo "You should avoid the using of USB-Hubs with your ISP-Programmer!"
  echo "USB-Hubs can cause timing problems with the half duplex protocol of avrdude."
  echo " "
fi
return ${DUDE_FAIL}
