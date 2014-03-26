#!/bin/sh

CURRENTDIR=$(pwd)
PATCHERPATH=$(readlink -f "$0")
PATCHERDIR=$(dirname "$PATCHERPATH")

echo "NovaFusion patcher for CyanogenMod 11"
cd $PATCHERDIR

echo "Copying NATIVE patch...";
cp native.patch ../../../../frameworks/native
echo "Copying AV patch...";
cp av.patch ../../../../frameworks/av
echo "Copying CORE patch...";
cp core.patch ../../../../system/core
echo "Copying TELEPHONY patch...";
cp telephony.patch ../../../../packages/services/Telephony

cd ../../../../frameworks/native
echo "Patching NATIVE...";
patch -p1 < native.patch
rm native.patch
cd ../av
echo "Patching AV...";
patch -p1 < av.patch
rm av.patch
cd ../../system/core
echo "Patching CORE...";
patch -p1 < core.patch
rm core.patch
cd ../../packages/services/Telephony
echo "Patching TELEPHONY...";
patch -p1 < telephony.patch
rm telephony.patch

echo "Everything done (probably)... Have a nice day!"
cd $CURRENTDIR
