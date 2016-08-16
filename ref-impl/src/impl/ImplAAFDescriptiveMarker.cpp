//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: ImplAAFDescriptiveMarker.cpp,v 1.4 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFDescriptiveFramework_h__
#include "ImplAAFDescriptiveFramework.h"
#endif

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDescriptiveMarker_h__
#include "ImplAAFDescriptiveMarker.h"
#endif

#include "AAFPropertyIDs.h"

#include <string.h>

ImplAAFDescriptiveMarker::ImplAAFDescriptiveMarker ()
  : _describedSlots( PID_DescriptiveMarker_DescribedSlots, L"DescribedSlots" ),
    _descriptiveFramework( PID_DescriptiveMarker_Description, L"Description" )
{
  _persistentProperties.put( _describedSlots.address() );
  _persistentProperties.put( _descriptiveFramework.address() );
}

ImplAAFDescriptiveMarker::~ImplAAFDescriptiveMarker ()
{
  ImplAAFDescriptiveFramework* pFramework = _descriptiveFramework.clearValue();
  if ( pFramework ) {
    pFramework->ReleaseReference();
    pFramework = 0;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::Initialize ()
{
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::SetDescribedSlotIDs (
      aafUInt32  numberElements,
      aafUInt32*  pDescribedSlotIDs)
{
  if (!pDescribedSlotIDs) {
    return AAFRESULT_NULL_PARAM;
  }

  _describedSlots.clear();
  unsigned int i;
  for ( i = 0; i < numberElements; ++i ) {
    _describedSlots.insert( pDescribedSlotIDs[i] );
  }

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::GetDescribedSlotIDs (
      aafUInt32  numberElements,
      aafUInt32* pDescribedSlotIDs)
{
  if ( !pDescribedSlotIDs ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !_describedSlots.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  if ( _describedSlots.count() > numberElements ) {
    return AAFRESULT_SMALLBUF;
  }

  aafUInt32* pNextDescribedSlotID = pDescribedSlotIDs;
  OMSetPropertyIterator<aafUInt32> iter( _describedSlots, OMBefore );
  while( ++iter ) {
    *pNextDescribedSlotID = iter.value();
    pNextDescribedSlotID++;
  }

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::GetDescribedSlotIDsSize (
      aafUInt32* pNumberElements )
{
  if ( NULL == pNumberElements ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !_describedSlots.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  *pNumberElements = _describedSlots.count();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::SetDescriptiveFramework (
      ImplAAFDescriptiveFramework * pDescriptiveFramework )
{
  if ( NULL == pDescriptiveFramework ) {
    return AAFRESULT_NULL_PARAM;
  }

  // Make sure another object does not already own pDescriptiveFramework.
  if ( pDescriptiveFramework->attached() ) {
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  }

  _descriptiveFramework = pDescriptiveFramework;
  _descriptiveFramework->AcquireReference();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDescriptiveMarker::GetDescriptiveFramework (
      ImplAAFDescriptiveFramework ** ppDescriptiveFramework )
{
  if ( NULL == ppDescriptiveFramework ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !(_descriptiveFramework && _descriptiveFramework.isPresent()) ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  *ppDescriptiveFramework = _descriptiveFramework;
  _descriptiveFramework->AcquireReference();

  return AAFRESULT_SUCCESS;
}
