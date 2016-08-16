//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: ImplAAFRIFFChunk.cpp,v 1.5 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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











#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFRIFFChunk_h__
#include "ImplAAFRIFFChunk.h"
#endif

#include "OMDataStreamPropertyFilter.h"

#include <string.h>


ImplAAFRIFFChunk::ImplAAFRIFFChunk () :

  _chunkID(PID_RIFFChunk_ChunkID,	L"ChunkID"),
  _chunkData(PID_RIFFChunk_ChunkData,	L"ChunkData"),
  _chunkDataFilter(_chunkData.createFilter())
{
  // Add the properties into the property set.
  _persistentProperties.put(_chunkID.address());
  _persistentProperties.put(_chunkData.address());

  // Initial default property values.
  _chunkID = 0;

}



ImplAAFRIFFChunk::~ImplAAFRIFFChunk ()
{
    delete _chunkDataFilter;
    _chunkDataFilter = NULL;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::Initialize (
      aafUInt32  chunkID)
{
	if( isInitialized() )
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }
	_chunkID = chunkID;

	AAFRESULT ar = AAFRESULT_SUCCESS;
    setInitialized();

  	return ar;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::SetChunkID (
      aafUInt32  chunkID)
{
	_chunkID = chunkID;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::GetChunkID (
      aafUInt32 *  pChunkID)
{
	if( pChunkID == NULL )
    {
        return AAFRESULT_NULL_PARAM;
    }

    *pChunkID = _chunkID;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::GetLength (
      aafLength_t *  pLength)
{
	if( pLength == NULL )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omSize = _chunkDataFilter->size();
	*pLength = omSize;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::Write (
      aafUInt32  bytes,
      aafDataBuffer_t  buffer,
      aafUInt32 *  pBytesWritten)
{
	if (NULL == buffer || NULL == pBytesWritten)
    	return AAFRESULT_NULL_PARAM;
  	// Cannot access the data property if it is NOT associated with a file.
  	if (!persistent())
    	return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  	_chunkDataFilter->write(buffer, bytes, *pBytesWritten);
  	
  	if (0 < bytes && 0 == *pBytesWritten)
    	return AAFRESULT_CONTAINERWRITE;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::Read (
      aafUInt32  bytes,
      aafDataBuffer_t  buffer,
      aafUInt32 *  pBytesRead)
{
	
	if (NULL == buffer || NULL == pBytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _chunkDataFilter->read(buffer, bytes, *pBytesRead);
  if (0 < bytes && 0 == *pBytesRead)
    return AAFRESULT_END_OF_DATA;

  return AAFRESULT_SUCCESS;
	
}


AAFRESULT STDMETHODCALLTYPE ImplAAFRIFFChunk::SetPosition(
    aafPosition_t offset )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omPosition = offset;
    _chunkDataFilter->setPosition( omPosition );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFRIFFChunk::GetPosition(
    aafPosition_t*  pOffset )
{
    if( pOffset == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omPosition = _chunkDataFilter->position();
    *pOffset = omPosition;


    return AAFRESULT_SUCCESS;
}



