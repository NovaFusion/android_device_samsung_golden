#!/bin/bash
echo -e "\e[0;32mNovaFusion cherry-picker for CyanogenMod 11\nStarting..."

PATCHERDIR=$(dirname $(readlink -f "$0"))
cd $PATCHERDIR
cd ../../../..
BASEDIR=$(pwd)

echo -e "\n\e[0;31mPatching RECOVERY:\e[0;34m"
cd $BASEDIR/bootable/recovery
git fetch http://review.cyanogenmod.org/CyanogenMod/android_bootable_recovery refs/changes/76/62676/1 && git cherry-pick FETCH_HEAD

echo -e "\n\e[0;31mPatching AV:\e[0;34m"
cd $BASEDIR/frameworks/av
git fetch http://review.cyanogenmod.org/CyanogenMod/android_frameworks_av refs/changes/72/62672/1 && git cherry-pick FETCH_HEAD

echo -e "\n\e[0;31mPatching BASE:\e[0;34m"
cd $BASEDIR/frameworks/base
git fetch http://review.cyanogenmod.org/CyanogenMod/android_frameworks_base refs/changes/77/62677/1 && git cherry-pick FETCH_HEAD

echo -e "\n\e[0;31mPatching NATIVE:\e[0;34m"
cd $BASEDIR/frameworks/native
git fetch http://review.cyanogenmod.org/CyanogenMod/android_frameworks_native refs/changes/52/63052/1 && git cherry-pick FETCH_HEAD

echo -e "\n\e[0;31mPatching CORE:\e[0;34m"
cd $BASEDIR/system/core
git fetch http://review.cyanogenmod.org/CyanogenMod/android_system_core refs/changes/74/62674/1 && git cherry-pick FETCH_HEAD

echo -e "\n\e[0;31mPatching TELEPHONY:\e[0;34m"
cd $BASEDIR/packages/services/Telephony
git fetch http://review.cyanogenmod.org/CyanogenMod/android_packages_services_Telephony refs/changes/75/62675/1 && git cherry-pick FETCH_HEAD

echo -e "\n\e[0;32mEverything (probably) patched, have a nice day!\e[0m"
