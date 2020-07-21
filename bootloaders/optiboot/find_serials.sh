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
#  OWNE=`echo "${LSL}"|cut -c14-18` ; # fetch the owner (usually root)
#  OWNA=`echo "${LSL}"|cut -c1-4`
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
    if [ "${LANGUAGE}" == "de_DE" ] ; then
      echo -n "${datei}: ist ein character Gerät, das den ${GROUPA} Zugriff für Gruppe ${GROUPID} erlaubt"
    else
      echo -n "${datei}: is a character device which allowes ${GROUPA} access for group ${GROUPID}"
    fi
    if (( gr_match == 1 )) ; then
      ser_found_with_access=${ser_found_with_access}+1
      if [ "${LANGUAGE}" == "de_DE" ] ; then
        echo ", bei welcher Sie Mitglied sind."
      else
        echo ", where you are member."
      fi
    else
      ser_found_without=${ser_found_without}+1
      if [ "${LANGUAGE}" == "de_DE" ] ; then
      echo ": für Sie kein Zugriff möglich!"
      else
      echo ": NO Access for you!"
      fi
    fi
  fi
done
if (( ${ser_found_with_access} == 0 )) && (( ${ser_found_without} == 0 )) ; then
   echo " "
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "Kein Gerät mit Namen /dev/ttyUSBx oder /dev/ttyACMx gefunden! x steht für eine vergebene Nummer!"
   echo " "
   echo "Prüfen Sie die Verbindung ihres ISP-Programmers zu einer USB-Schnittstelle ihres PCs."
   echo "Wenn sie unsicher sind, welcher Programmer eingesteckt ist, kann das Kommando 'lsusb' helfen."
   echo "Das Kommando 'lsusb' zeigt alle angeschlossenen USB-Geräte ihres Systems."
   echo " "
   echo "Sie können mehr über den gerade angeschlossene Programmer erfahren mit einem Kommando"
   echo "wie 'dmesg | tail -20'. 'dmesg' gibt die gesamte Logdatei iihres Linux-Systems aus und"
   echo "'| tail -20' zeigt nur die letzten 20 Zeilen davon."
  else
   echo "No device with name /dev/ttyUSBx or /dev/ttyACMx found! x can be any sequence number!"
   echo " "
   echo "Check the connection of your  ISP programmer to a USB interface of your PC."
   echo "If you are in doubt about the plugged in programmer, the command 'lsusb' may help,"
   echo "The command 'lsusb' show all USB devices of your system."
   echo " "
   echo "You can learn more about the details of the just plugged programmer with"
   echo "a command like 'dmesg | tail -20' , 'dmesg' shows the total log file of your Linux system"
   echo "and '| tail -20' will display only the last 20 lines."
  fi
elif (( ${ser_found_with_access} == 0 )) && (( ${ser_found_without} > 0 )) ; then
  if [ "${LANGUAGE}" == "de_DE" ] ; then
   echo "Es wurden ${ser_found_without} serielle Sxchnittstellen gefunden, aber Sie haben keinen Zugriff!!!"
   echo "Wenn ein Gerät davon das richtige ist, sollten Sie dafür sorgen,"
   echo "daß Sie Mitglied der Gruppe sind, die oben angezeigt ist und für die das Gerät den Zugriff erlaubt,"
   echo "Sie können versuchen, den Benutzer ${USER} zu der Gruppe hizuzufügen mit dem"
   echo "Kommando 'sudo usermod -aG dialout ${USER}' , wenn dialout als Gruppe in der"
   echo "oben angeführten Liste steht."
   echo "Für 'sudo' ist die Wiedereingabe des Passwortes erforderlich."
  else
   echo "${ser_found_without} serial device(s) are found, but you have no access!!!"
   echo "If one or more of the devives are the right one, you should make shure"
   echo "to be a member in the group, to wich the device will allow the access,"
   echo "as it is shown in the list above."
   echo "You can try to add ${USER} to the group of the device with the"
   echo "command 'sudo usermod -aG dialout ${USER}' , where dialout is a sample name,"
   echo "which should match to the listed group of the device."
   echo "For 'sudo' a retyped password entry is required."
  fi
#elif (( ${ser_found_with_access} > 0 )) && (( ${ser_found_without} == 0 )) ; then
#  echo "You have access to all listed serial devices as shown above."
#  echo "Please make shure, that your application program selects the right port.

fi
echo " "
