#!/system/bin/sh

MCE_CFG=/modemfs/CONF/MCE/MCE.CFG
PMST_CFG=/modemfs/PERM/PLAIN/PMST.CFG 
TRACE_AUTO_CONF=/modemfs/trace_auto.conf
JIG_SMD=/sys/class/usb_switch/FSA_SWITCH/jig_smd

if cat /sys/class/usb_switch/FSA_SWITCH/jig_smd ; then
	echo "[JIG USB] detected";
	echo "[Boot Mode] SMD";
	echo "[MODEM] local mode";
	echo -en "\x01" > $MCE_CFG
	echo "[MODEM] non sleep mode";
	echo -en "\xff\xff\x60\x60" > $PMST_CFG
else
	echo "[JIG USB] Not detected";
	if ls /linuxrc > /dev/null ; then
		echo "[Boot Mode] HATS";
		echo "[modem] local mode";
		echo -en "\x01" > /mnt$MCE_CFG
		echo "[MODEM] non sleep mode";
		echo -en "\xff\xff\x60\x60" > /mnt$PMST_CFG
#          	rm /mnt/modemfs/RFHAL/*
#		cp -r /mnt/modemfs/RFHAL_const/* /mnt/modemfs/RFHAL/
	else
		echo "[Boot Mode] Normal";
		echo "[modem] normal mode.";
		echo -en "\x00" > $MCE_CFG
		echo "[modem] sleep mode";
		echo -en "\xff\xff\xff\xff" > $PMST_CFG
	fi
fi

# chown for test application
if ls $MCE_CFG > /dev/null ; then
	chown radio.radio $MCE_CFG
fi
if ls $PMST_CFG > /dev/null ; then
	chown radio.radio $PMST_CFG
fi
if ls $TRACE_AUTO_CONF > /dev/null ; then
chown radio.radio $TRACE_AUTO_CONF
fi
if ls $JIG_SMD > /dev/null ; then
	chown radio.radio $JIG_SMD
fi
								
# Start CSPSA2NWM which uses CSPSA server to copy radio parameters
# from CSPSA and store them as files in the file system in the way expected
# by NWM. Note that this process does not background itself, but rather
# blocks until the copying has completed.

#echo "Synchronizing modem parameters from CSPSA to file system"
#mv /modemfs/guardfile /modemfs/GUARDFIL
#/system/bin/cspsa2nwm -r / -u 0xfffffffd:0 -g /modemfs/GUARDFIL -p /modemfs/RFHAL CSPSA0

# This is a initial script for copying a default calibration data.  
# After device downloading, phone calibration data is erased.
# so it is necessary to copy a backup data. 

echo "Synchronizing modem parameter & CAL data";

MODEMFS_GUARD=/modemfs/GUARDFIL
MODEMFS_RFHAL=/modemfs/RFHAL
RFHALDEF_COPIED=/modemfs/rfhal_copied
EFS_GUARD=/efs/GUARDFIL
EFS_RFHAL=/efs/RFHAL

if ! ls $MODEMFS_GUARD > /dev/null ; then
	if ! ls $EFS_GUARD > /dev/null ; then
 		echo "There is no CAL data. Copy the default modem CAL data to modemfs partition.";
		if ! ls $MODEMFS_RFHAL > /dev/null ; then
			cp -r /efs/RFHAL.DEF /modemfs/RFHAL
			echo 0 > $RFHALDEF_COPIED
		else
		  if ! ls $RFHALDEF_COPIED > /dev/null ; then
			  cp -r /efs/RFHAL.DEF/* /modemfs/RFHAL/
			  echo 0 > $RFHALDEF_COPIED
		  fi
		fi
	else
		echo "modemfs partition is erased. Copy the previous CAL data to modemfs partition.";
		if ! ls $MODEMFS_RFHAL > /dev/null ; then
			cp -r /efs/RFHAL /modemfs/RFHAL
		else
			cp -r /efs/RFHAL/* /modemfs/RFHAL/
		fi
		echo 0 > /modemfs/GUARDFIL
	fi
else
	if ! ls $EFS_GUARD > /dev/null ; then
		if ! ls $EFS_RFHAL > /dev/null ; then
			cp -r /modemfs/RFHAL /efs/RFHAL
		else
			cp -r /modemfs/RFHAL/* /efs/RFHAL/
		fi
		echo 0 > /efs/GUARDFIL
	fi
fi

# Flushing MCE.CFG and PMST.CFG before executing MSA
echo "Sync for MCE.CFG and PMST.CFG";
sync

# Finally start Modem Storage Agent, serving files system requests from the
# modem. It is important that this process does not background itself since
# Androids init will restart this script once this command returns. The
# necessary device nodes were created by /system/etc/ste_audio_mknods.sh
# prior to executing this command.
echo "Starting Modem Storage Agent"
/system/bin/msa
