#!/bin/sh
CDIR=$(pwd)
#
# Copyright (C) 2014 NovaFusion http://novafusion.pl
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

if [ $# -eq 0 ]
then
    PATCHFLAGS=""
elif [ $# -eq 1 ] && [ $1 = "-r" ]
then
    PATCHFLAGS="-R"
else
    echo "Usage: $0 [-r]"
    exit 1
fi

PATCHDIR=$CDIR
cd $PATCHDIR
cd ../../../..

PATCH()
{
    patch -p1 -N $PATCHFLAGS -i$PATCHDIR/$1
}

shopt -s nullglob
file=( "$CDIR"/*.patch )
for i in "${file[@]}"
do
	PATCH $(basename $i)
done
