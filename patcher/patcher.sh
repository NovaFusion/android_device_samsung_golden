#!/bin/sh
echo -e "\e[0;32mNovaFusion patcher for CyanogenMod 11\nStarting..."

PATCHERDIR=$(dirname $(readlink -f "$0"))
cd $PATCHERDIR
cd ../../../..
BASEDIR=$(pwd)

echo -e "\n\e[0;31mPatching RECOVERY:\e[0;34m"
cd $BASEDIR/bootable/recovery
patch -p1 -N -i$PATCHERDIR/recovery.patch

echo -e "\n\e[0;31mPatching AV:\e[0;34m"
cd $BASEDIR/frameworks/av
patch -p1 -N -i$PATCHERDIR/av.patch

echo -e "\n\e[0;31mPatching BASE:\e[0;34m"
cd $BASEDIR/frameworks/base
patch -p1 -N -i$PATCHERDIR/base.patch

echo -e "\n\e[0;31mPatching NATIVE:\e[0;34m"
cd $BASEDIR/frameworks/native
patch -p1 -N -i$PATCHERDIR/native.patch

echo -e "\n\e[0;31mPatching CORE:\e[0;34m"
cd $BASEDIR/system/core
patch -p1 -N -i$PATCHERDIR/core.patch

echo -e "\n\e[0;31mPatching TELEPHONY:\e[0;34m"
cd $BASEDIR/packages/services/Telephony
patch -p1 -N -i$PATCHERDIR/telephony.patch

echo -e "\n\e[0;32mEverything (probably) patched, have a nice day!\e[0m"
