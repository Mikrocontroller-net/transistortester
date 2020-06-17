#!/bin/bash

# look for ttyACM and ttyUSB devices.
LS_AC=`ls -C /dev/tty[AU][CS][BM]*`

MYGROUPS="`groups`"

for datei in ${LS_AC}
do
  LSL=`ls -l ${datei}`
  OWNE=`echo "${LSL}"|cut -c14-18`
  OWNA=`echo "${LSL}"|cut -c1-4`
  GROUPID=`echo "${LSL}"|cut -c19-25`
  GROUPA=`echo "${LSL}"|cut -c5-7`
  if [[ "${OWNE}" = "root " ]] && [[ "${OWNA}" = "crw-" ]]
  then
    # char device belong to member root with rw access
    gr_match=0
    for gr_id in ${MYGROUPS}
    do
      if [[ "${gr_id}" = "${GROUPID}" ]] && [[ "${GROUPA}" = "rw-" ]] 
      then
	# you are member in this group
	gr_match=1
	break
      fi
    done
    if (( gr_match == 1 ))
    then
      echo "${datei}: is characterdevice from ${OWNE},${OWNA} with group ${GROUPID},${GROUPA} : with Access for you"
    else
      echo "${datei}: is characterdevice from ${OWNE},${OWNA} with group ${GROUPID},${GROUPA} : NO Access for group"
    fi
  fi
# [[ -t ${datei} ]] && echo "${datei}: is serielles Terminal"
done
