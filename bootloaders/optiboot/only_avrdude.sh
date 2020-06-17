#!/bin/bash
#
# K.-H. Kübbeler (kh_kuebbeler@web.de)  Jun 2020
# 
echo "avrdude ${DUDE_PARAMS}"
avrdude ${DUDE_PARAMS}
# avrdude can tell more about the operation with the -v option:
# avrdude -v ${DUDE_PARAMS}
DUDE_FAIL=$?
if (( ${DUDE_FAIL} != 0 ))
then
  echo "#-#-#-#-#-#-#-#"
  echo "*** avrdude *** failed with return value ${DUDE_FAIL}!"
  echo "If avrdude has not started with the Programmer communication,"
  echo "this can be caused by a wrong ISPPORT [currently:${ISPPORT}] setting!" 
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
    echo " "
    echo "If access to the right Port is not allowed , try to add ${USER} to the group of the device"
    echo "with the command 'sudo usermod -aG dialout ${USER}' ,"
    echo "where dialout is a sample name."
    echo "Probably the supervisor or user Password is required for the command."
    echo "If the access is allowed to the correct device, the usermod action is not required!"
  fi
  echo "If the avrdude has started the communication to your taget with the ISP-programmer,"
  echo "your ISPPORT setting should be OK. So you have to check the cable connection"
  echo "between the ISP-Programmer and your AVR target."
  echo "You should avoid the using of USB-Hubs with your ISP-Programmer!"
  echo "USB-Hubs can cause timing problems with the half duplex protocol of avrdude."
  echo " "
fi
return ${DUDE_FAIL}
