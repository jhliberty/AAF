#!/bin/sh
###############################################################################
#
# $Id: createtest.sh,v 1.4 2009/06/01 11:47:15 stuart_hc Exp $ $Name: V116 $
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# British Broadcasting Corporation.
# All rights reserved.
#
###############################################################################
#
#	Shell script for creating test data for scalability tests.
#	Requires a sample raw DV file called 1frame.dv to be present.
#
###############################################################################

# Creates the ELI input files
createTestFile()
{
	cuts_var=$N
	elifile=$datadir/$cuts_var.eli
	echo "Writing $elifile"

	echo "LAV Edit List" > $elifile
	echo "PAL"   >> $elifile
	echo "$cuts_var"  >> $elifile
	i=0
	dvfile="1frame"
	while [ $i -lt $cuts_var ];
	do
		outfile=`echo "$dvhome $dvfile $i" | awk '{printf "%s/%s_%07d", $1, $2, $3}'`
		echo $outfile.dv >> $elifile
		i=`expr $i + 1`
	done
	
	i=0
	while [ $i -lt $cuts_var ];
	do
		echo "$i 0 1" >> $elifile
		i=`expr $i + 1`
	done
}

#base dir - set to $HOME on unix
#           set to the appropriate drive in MS Windows E.g. c: or /c
BASEDIR=$HOME

datadir=$BASEDIR/aaftestdata
dvhome=$datadir/dv

max_cuts=100000
if [ -n "$1" ] 
then
	max_cuts=$1
fi

if [ ! -d $datadir ]
then
	mkdir $datadir
fi

if [ ! -d $dvhome ]
then
	mkdir $dvhome
fi


countDecades="5"
stepsPerDecade="5"
m=0

while [ $m -le $countDecades ]
do
	k=0;
	while [ $k -lt $stepsPerDecade ]
	do
		logN=`echo "$k $stepsPerDecade" | awk '{ printf ("%f", $1/$2) }'`
		N=`echo $m $logN | awk '{ printf ("%d", int(10**$1 * int((10*(10**$2) + 0.5))/10 +0.5))}'`
		if [ $N -gt $max_cuts ]
		then
			break
		fi

		if [ $m -eq $countDecades ]
		then
			break
		fi

		createTestFile $N
		k=`expr $k + 1`
	done
	m=`expr $m + 1`
	if [ $N -gt $max_cuts ]
	then
		break
	fi
done

echo "max_cuts=$max_cuts"
i=0
while [ $i -lt $cuts_var ];
do
		outfile=`echo "$dvhome $dvfile $i" | awk '{printf "%s/%s_%07d", $1, $2, $3}'`
		echo "Writing $outfile.dv"
		cp $dvfile.dv $outfile.dv
		i=`expr $i + 1`
done

echo "Copying runaaftest.sh into $datadir"
cp runaaftest.sh $datadir

chmod +x $datadir/runaaftest.sh
