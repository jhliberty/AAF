###############################################################################
#
# $Id: install-builtins.bash,v 1.2 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

function usage
{
  echo "Usage : $TOOLNAME [OPTIONS]"
  echo "OPTIONS:"
  echo "  -c = compare only, report files that need to be updated [default]."
  echo "  -u = update any changed files."
  echo "  -h = help, print this message and exit."
}

BUILTINS="\
ImplAAFDictionary.cpp_DataDefs \
ImplAAFDictionary.cpp_ContainerDefs \
"

SOURCE=./include
DEST=../../ref-impl/src/impl

FAILURE=1
SUCCESS=0

TOOLNAME=${0##*/}
TOOLPATH=${0%/${TOOLNAME}}

mode="compare"

while getopts ":cuh" opt
do
  case $opt in
  c)
    mode="compare"
    ;;
  u)
    mode="update"
    ;;
  h)
    usage
    exit $SUCCESS
    ;;
  \?)
    echo "$TOOLNAME : Error: \"-$OPTARG\" not recognized."
    usage
    exit $FAILURE
    ;;
  esac
done
shift $(($OPTIND - 1))

for h in $BUILTINS
do
  if [[ ! -f ${SOURCE}/$h ]]
  then
    echo "Can't find \"${SOURCE}/$h\"."
    exit $FAILURE
  fi
done

for h in $BUILTINS
do
  if [[ ! -f ${DEST}/$h ]]
  then
    if [[ $mode == "update" ]]
    then
      echo "Creating \"${DEST}/$h\"."
      cp ${SOURCE}/$h ${DEST}/$h
    else
      echo "Can't find \"${DEST}/$h\"."
    fi
  fi
done

for h in $BUILTINS
do
  cmp -s ${SOURCE}/$h ${DEST}/$h
  if [[ ${?} != 0 ]]
  then
    if [[ $mode == "update" ]]
    then
      echo "Updating \"${DEST}/$h\"."
      cp ${SOURCE}/$h ${DEST}/$h
    else
      echo "File needs updating \"${DEST}/$h\"."
    fi
  fi
done

exit $SUCCESS
