//=---------------------------------------------------------------------=
//
// $Id: ImplAAFPlainEssenceData.cpp,v 1.3 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFPlainEssenceData.h"
#include "ImplAAFEssenceData.h"

#include "AAFResult.h"

#include "OMDataStreamPropertyFilter.h"


ImplAAFPlainEssenceData::ImplAAFPlainEssenceData ()
: _mediaDataContainer(0),
  _mediaDataFilter(0)
{
}


ImplAAFPlainEssenceData::~ImplAAFPlainEssenceData ()
{
  delete _mediaDataFilter;
  _mediaDataFilter = 0;

  if (_mediaDataContainer)
  {
    _mediaDataContainer->ReleaseReference();
    _mediaDataContainer = 0;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::Initialize (ImplAAFSourceMob * pFileMob)
{
  return _mediaDataContainer->SetFileMob(pFileMob);
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFPlainEssenceData::Write (aafUInt32  bytes,
                           aafDataBuffer_t buffer,
                           aafUInt32 *bytesWritten)
{
  if (NULL == buffer || NULL == bytesWritten)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!_mediaDataContainer->persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  ASSERTU(_mediaDataFilter);
  _mediaDataFilter->write(buffer, bytes, *bytesWritten);
  if (0 < bytes && 0 == *bytesWritten)
    return AAFRESULT_CONTAINERWRITE;

  return AAFRESULT_SUCCESS;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFPlainEssenceData::Read (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!_mediaDataContainer->persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  ASSERTU(_mediaDataFilter);
  _mediaDataFilter->read(buffer, bytes, *bytesRead);
  if (0 < bytes && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::SetPosition (aafPosition_t  offset)
{
  // Cannot access the data property if it is NOT associated with a file.
  if (!_mediaDataContainer->persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  ASSERTU(_mediaDataFilter);
  OMUInt64 tmpOffset = offset;
  _mediaDataFilter->setPosition(tmpOffset);

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetPosition (aafPosition_t  *pOffset)
{
  if (NULL == pOffset)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!_mediaDataContainer->persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  ASSERTU(_mediaDataFilter);
  OMUInt64 tmpOffset;
  tmpOffset = _mediaDataFilter->position();
  *pOffset = tmpOffset;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetSize (aafLength_t *pSize)
{
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!_mediaDataContainer->persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  ASSERTU(_mediaDataFilter);
  *pSize = _mediaDataFilter->size();

  return AAFRESULT_SUCCESS;
}

/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFPlainEssenceData::WriteSampleIndex (aafUInt32  bytes,
                           aafDataBuffer_t buffer,
                           aafUInt32 *bytesWritten)
{
  return _mediaDataContainer->WriteSampleIndex(bytes, buffer, bytesWritten);
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFPlainEssenceData::ReadSampleIndex (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  return _mediaDataContainer->ReadSampleIndex(bytes, buffer, bytesRead);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::SetSampleIndexPosition (aafPosition_t  offset)
{
  return _mediaDataContainer->SetSampleIndexPosition(offset);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetSampleIndexPosition (aafPosition_t  *pOffset)
{
  return _mediaDataContainer->GetSampleIndexPosition(pOffset);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetSampleIndexSize (aafLength_t *pSize)
{
  return _mediaDataContainer->GetSampleIndexSize(pSize);
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::SetFileMob (ImplAAFSourceMob * pFileMob)
{
  return _mediaDataContainer->SetFileMob(pFileMob);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetFileMob (ImplAAFSourceMob ** ppFileMob)
{
  return _mediaDataContainer->GetFileMob(ppFileMob);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetFileMobID (aafMobID_t *  pFileMobID)
{
  return _mediaDataContainer->GetFileMobID(pFileMobID);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::GetEssenceElementKey (aafUID_t * pEssenceElementKey)
{
  return _mediaDataContainer->GetEssenceElementKey(pEssenceElementKey);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::SetEssenceElementKey (aafUID_constref  key)
{
  return _mediaDataContainer->SetEssenceElementKey(key);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainEssenceData::pvtInitialize (
      ImplAAFEssenceData* pRawEssenceData)
{
  ASSERTU(pRawEssenceData);

  _mediaDataContainer = pRawEssenceData;
  _mediaDataContainer->AcquireReference();

  _mediaDataFilter = pRawEssenceData->createMediaDataFilter();
  ASSERTU(_mediaDataFilter);


  return AAFRESULT_SUCCESS;
}
