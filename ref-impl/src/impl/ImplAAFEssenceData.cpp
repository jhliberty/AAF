//=---------------------------------------------------------------------=
//
// $Id: ImplAAFEssenceData.cpp,v 1.37 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif
#ifndef __ImplAAFHeader_h__
#include "ImplAAFHeader.h"
#endif
#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include "ImplAAFPlainEssenceData.h"

#include "OMUtilities.h"

#include <string.h>
#include "AAFResult.h"


extern "C" const aafClassID_t CLSID_AAFPlainEssenceData;


const aafMobID_t kNullMobID = {{0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,
	{0,0,0,{0,0,0,0,0,0,0,0}}};

ImplAAFEssenceData::ImplAAFEssenceData () :
  _fileMobID(PID_EssenceData_MobID,	L"MobID"),
  _mediaData(PID_EssenceData_Data,	L"Data"),
  _indexData(PID_EssenceData_SampleIndex,	L"SampleIndex")
{
  // Add the properties into the property set.
  _persistentProperties.put(_fileMobID.address());
  _persistentProperties.put(_mediaData.address());
  _persistentProperties.put(_indexData.address());

  // Initial default property values.
  _fileMobID = kNullMobID;
}


ImplAAFEssenceData::~ImplAAFEssenceData ()
{
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::Initialize (ImplAAFSourceMob * pFileMob)
{
	return(SetFileMob (pFileMob));
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Write (aafUInt32  bytes,
                           aafDataBuffer_t buffer,
                           aafUInt32 *bytesWritten)
{
  if (NULL == buffer || NULL == bytesWritten)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _mediaData.write(buffer, bytes, *bytesWritten);
  if (0 < bytes && 0 == *bytesWritten)
    return AAFRESULT_CONTAINERWRITE;

  return AAFRESULT_SUCCESS;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Read (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _mediaData.read(buffer, bytes, *bytesRead);
  if (0 < bytes && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetPosition (aafPosition_t  offset)
{
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  OMUInt64 tmpOffset = offset;
  _mediaData.setPosition(tmpOffset);

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetPosition (aafPosition_t  *pOffset)
{
  if (NULL == pOffset)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  OMUInt64 tmpOffset;
  tmpOffset = _mediaData.position();
  *pOffset = tmpOffset;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetSize (aafLength_t *pSize)
{
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  *pSize = _mediaData.size();

  return AAFRESULT_SUCCESS;
}

/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::WriteSampleIndex (aafUInt32  bytes,
                           aafDataBuffer_t buffer,
                           aafUInt32 *bytesWritten)
{
  if (NULL == buffer || NULL == bytesWritten)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _indexData.write(buffer, bytes, *bytesWritten);
  if (0 < bytes && 0 == *bytesWritten)
    return AAFRESULT_CONTAINERWRITE;

  return AAFRESULT_SUCCESS;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::ReadSampleIndex (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _indexData.read(buffer, bytes, *bytesRead);
  if (0 < bytes && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetSampleIndexPosition (aafPosition_t  offset)
{
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  OMUInt64 tmpOffset = offset;
  _indexData.setPosition(tmpOffset);

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetSampleIndexPosition (aafPosition_t  *pOffset)
{
  if (NULL == pOffset)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  OMUInt64 tmpOffset;
  tmpOffset = _indexData.position();
  *pOffset = tmpOffset;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetSampleIndexSize (aafLength_t *pSize)
{
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  *pSize = _indexData.size();

  return AAFRESULT_SUCCESS;
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetFileMob (ImplAAFSourceMob * pFileMob)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFMob *pMob = NULL;
  ImplAAFSourceMob *pSourceMob = NULL;
  ImplAAFEssenceDescriptor *pEssenceDescriptor = NULL;
  ImplAAFFileDescriptor *pFileDescriptor = NULL;
  ImplAAFHeader *pHeader = NULL;
  aafMobID_t mobID;

  if(NULL == pFileMob)
    return(AAFRESULT_NULL_PARAM);

  XPROTECT()
  {
    CHECK(pFileMob->GetMobID(&mobID));

 // move to ImplAAFContentStorage::AppendEssenceData().
    // Does a mob with the ID already exist?  If not, return error
    CHECK(pFileMob->MyHeadObject(&pHeader));
    CHECK(pHeader->LookupMob(mobID, &pMob));


    // Make sure the mob is a valid File source mob???
    pSourceMob = dynamic_cast<ImplAAFSourceMob *>(pMob);
    if (NULL == pSourceMob)
      RAISE(AAFRESULT_INVALID_FILE_MOB);
    // Must       
    result = pSourceMob->GetEssenceDescriptor(&pEssenceDescriptor);
    if (AAFRESULT_SUCCESS != result)
      RAISE(AAFRESULT_INVALID_FILE_MOB);

    pFileDescriptor = dynamic_cast<ImplAAFFileDescriptor *>(pEssenceDescriptor);
    if (NULL == pFileDescriptor)
      RAISE(AAFRESULT_INVALID_FILE_MOB);

    // The mob id refers to a valid file source mob so we can save
    // the id.
    _fileMobID = mobID;
  } /* XPROTECT */
  XEXCEPT
  {  // save the error code.
    result = (XCODE());
	NO_PROPAGATE();
  }
  XEND;

  // cleanup
  if (pEssenceDescriptor)
  {
    pEssenceDescriptor->ReleaseReference();
    pEssenceDescriptor = NULL;
  }
  if (pMob)
  {
    pMob->ReleaseReference();
    pMob = NULL;
  }
  if (pHeader)
  {
    pHeader->ReleaseReference();
    pHeader = NULL;
  }
  return(result);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetFileMob (ImplAAFSourceMob ** ppFileMob)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFMob *pMob = NULL;
  ImplAAFSourceMob *pSourceMob = NULL;
  ImplAAFEssenceDescriptor *pEssenceDescriptor = NULL;
  ImplAAFHeader *pHeader = NULL;
  aafMobID_t mobID;

  if(NULL == ppFileMob)
    return(AAFRESULT_NULL_PARAM);

  XPROTECT()
  {
    CHECK(GetFileMobID(&mobID));

    // Does a mob with the ID already exist?  If not, return error.
    // NOTE: Will return AAFRESULT_OBJECT_NOT_PERSISTENT if this object has
    // not been appended to to the file.
    CHECK(MyHeadObject(&pHeader));
    CHECK(pHeader->LookupMob(mobID, &pMob));

    // This should be a valid file mob which is a file mob.
    pSourceMob = dynamic_cast<ImplAAFSourceMob *>(pMob);
    if (NULL == pSourceMob)
      RAISE(AAFRESULT_NOT_FILEMOB);

    // Does the source mob contain a file descriptor?
    CHECK(pSourceMob->GetEssenceDescriptor(&pEssenceDescriptor));
    if (dynamic_cast<ImplAAFFileDescriptor *>(pEssenceDescriptor))
    {
      (*ppFileMob) = pSourceMob;
      pSourceMob->AcquireReference();
    }

  } /* XPROTECT */
  XEXCEPT
  {  // save the error code.
    result = (XCODE());
	NO_PROPAGATE();
  }
  XEND;

  // cleanup
  // Note: pMob and pSourceMob are temp and should not be released.
  if (pEssenceDescriptor)
  {
    pEssenceDescriptor->ReleaseReference();
    pEssenceDescriptor = NULL;
  }
  if (pMob)
  {
    pMob->ReleaseReference();
    pMob = NULL;
  }
  if (pHeader)
  {
    pHeader->ReleaseReference();
    pHeader = NULL;
  }
  return(result);

}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetFileMobID (aafMobID_t *  pFileMobID)
{
  if (NULL == pFileMobID)
    return AAFRESULT_NULL_PARAM;

  *pFileMobID = _fileMobID;
  return AAFRESULT_SUCCESS;
}



/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetPlainEssenceData (
      aafUInt32 reserved,
      ImplAAFPlainEssenceData ** pPlainEssenceData)
{
  AAFRESULT result = AAFRESULT_NOT_IMPLEMENTED;

  if (reserved != 0)
    return AAFRESULT_INVALID_PARAM;

  ImplAAFPlainEssenceData* pResult =
    (ImplAAFPlainEssenceData*) CreateImpl (CLSID_AAFPlainEssenceData);
  if (pResult)
  {
    pResult->pvtInitialize(this);
    *pPlainEssenceData = pResult;
    result = AAFRESULT_SUCCESS;
  }
  else
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFEssenceData::GetEssenceElementKey(
  aafUID_t * pEssenceElementKey)
{
  if (NULL == pEssenceElementKey)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  if (!_mediaData.hasEssenceElementKey())
      return AAFRESULT_OPERATION_NOT_PERMITTED;

  convert( *reinterpret_cast<OMUniqueObjectIdentification*>(pEssenceElementKey),
           _mediaData.essenceElementKey() );

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFEssenceData::SetEssenceElementKey(
  aafUID_constref  key)
{
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  if (!_mediaData.hasEssenceElementKey())
      return AAFRESULT_OPERATION_NOT_PERMITTED;

  OMKLVKey klvKey;
  convert( klvKey,
           *reinterpret_cast<const OMUniqueObjectIdentification*>(&key) );
  _mediaData.setEssenceElementKey(klvKey);

  return AAFRESULT_SUCCESS;
}



const OMMaterialIdentification&
  ImplAAFEssenceData::identification(void) const
{
  return *reinterpret_cast<const OMMaterialIdentification*>(&_fileMobID.reference());
}



OMDataStreamPropertyFilter* ImplAAFEssenceData::createMediaDataFilter()
{
  return _mediaData.createFilter();
}

