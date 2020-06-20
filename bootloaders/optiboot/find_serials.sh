#!/bin/bash

# look for ttyACM and ttyUSB devices.
# native serial ports are usually accessable with the /dev/ttySx name,
# but they are statically defined. So a search is useless,
# because often 32 or more entries are found.
# so only known names for USB-serial converters are searched!

LS_AC=`ls -C --color=never /dev/ttyACM* /dev/ttyUSB* 2> /dev/null`
# LS_AC is only a list of the found names.

MYGROUPS="`groups`"
ser_found_with_access=0
ser_found_without=0
echo " "
for datei in ${LS_AC}
do
  LSL=`ls -l --color=never ${datei}` ; # make a long ls as shown in the next line
#crw-rw---- 1 root dialout 166, 0 Jun 19 20:52 /dev/ttyACM0
  OWNE=`echo "${LSL}"|cut -c14-18` ; # fetch the owner (usually root)
  OWNA=`echo "${LSL}"|cut -c1-4`
  GROUPID=`echo "${LSL}"|cut -c19-25` ; # get the group name
  GROUPA=`echo "${LSL}"|cut -c5-7`
  ser_found=1
  if [[ "${OWNE}" = "root " ]] && [[ "${OWNA}" = "crw-" ]] ; then
    # char device belong to member root with rw access
    gr_match=0
    for gr_id in ${MYGROUPS}
    do
      if [[ "${gr_id}" = "${GROUPID}" ]] && [[ "${GROUPA}" = "rw-" ]] 
      then
	# you are member in this group and have read/write access
	gr_match=1
	break
      fi
    done
    if (( gr_match == 1 )) ; then
      ser_found_with_access=${ser_found_with_access}+1
      echo "${datei}: is characterdevice from ${OWNE},${OWNA} with group ${GROUPID},${GROUPA} : with Access for you"
    else
      ser_found_without=${ser_found_without}+1
      echo "${datei}: is characterdevice from ${OWNE},${OWNA} with group ${GROUPID},${GROUPA} : NO Access for group"
    fi
  fi
done
if (( ${ser_found_with_access} == 0 )) && (( ${ser_found_without} == 0 )) ; then
  echo " "
  echo "No device with name /dev/ttyUSBx or /dev/ttyACMx found! x can be any sequence number!"
  echo " "
  echo "Check the connection of your  ISP programmer to a USB interface of your PC."
  echo "If you are in doubt about the plugged in programmer, the command 'lsusb' may help,"
  echo "The command 'lsusb' show all USB devices of your systemn."
  echo " "
  echo "You can learn more about the details of the just plugged programmer with"
  echo "a command like 'dmesg | tail -20' , 'dmesg' shows the total log file of your Linux system"
  echo "and '| tail -20' will display only the last 20 lines."
elif (( ${ser_found_with_access} == 0 )) && (( ${ser_found_without} > 0 )) ; then
  echo "${ser_found_without} serial device(s) are found, but you have no access!!!"
  echo "If one or more of the devives are the right one, you should make shure"
  echo "to be a member in the group, to wich the device will allow the access,"
  echo "as it is shown in the list above."
  echo "You can try to add ${USER} to the group of the device with the"
  echo "command 'sudo usermod -aG dialout ${USER}' , where dialout is a sample name,"
  echo "which should match to the listed group of the device."
  echo "For 'sudo' a retyped password entry is required."
#elif (( ${ser_found_with_access} > 0 )) && (( ${ser_found_without} == 0 )) ; then
#  echo "You have access to all listed serial devices as shown above."
#  echo "Please make shure, that your application program selects the right port.

fi
echo " "
