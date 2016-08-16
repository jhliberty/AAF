###############################################################################
#
# $Id: update.ksh,v 1.3 2009/06/01 11:46:43 stuart_hc Exp $ $Name: V116 $
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
# Avid Technology.
# All rights reserved.
#
###############################################################################

#------------------------------------------------------------------------------
#
# Usage: update.sh <new file> <file to update>
#
# The script checks if two files are different. If
# they are, it copies the first file over the second 
# one and makes it writable to be recognized by some 
# source control systems as modified.
# 
# If files are identical but the first file is newer 
# then the second one then the script sets the second
# file's modification date to latest so that 'make' 
# doesn't try to rebuild it.
#
# If none of the above is true, nothing happens.
#
#------------------------------------------------------------------------------

if [ -f $2 ] && cmp -s $1 $2; then
    echo $2 did not change;
    if [ $1 -nt $2 ]; then
	echo updating timestamp $2
        touch $2
    fi
else
    echo updating $2
    cp -f $1 $2
    chmod +w $2
fi

