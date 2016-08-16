//=---------------------------------------------------------------------=
//
// $Id: ImplAAFFileDescriptor.cpp,v 1.35 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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




#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "OMAssertions.h"
#include "AAFResult.h"

// Declare these two functions to avoid including the whole ImplAAFDictionary.h
aafBoolean_t aafLookupCodecDef( ImplAAFObject *, ImplAAFCodecDef *p_codecdef );
aafBoolean_t aafLookupContainerDef( ImplAAFObject *,ImplAAFContainerDef * );

ImplAAFFileDescriptor::ImplAAFFileDescriptor ()
: _sampleRate(			PID_FileDescriptor_SampleRate,		L"SampleRate"),
 _length(				PID_FileDescriptor_Length,			L"Length"),
 _codecDef(				PID_FileDescriptor_CodecDefinition,		L"CodecDefinition", L"/Header/Dictionary/CodecDefinitions", PID_DefinitionObject_Identification),
 _containerFmt(         PID_FileDescriptor_ContainerFormat,	L"ContainerFormat", L"/Header/Dictionary/ContainerDefinitions", PID_DefinitionObject_Identification),
 _linkedSlotID(				PID_FileDescriptor_LinkedSlotID,			L"LinkedSlotID")
{
  _persistentProperties.put(_sampleRate.address());
  _persistentProperties.put(_length.address());
  _persistentProperties.put(_codecDef.address());
  _persistentProperties.put(_containerFmt.address());
  _persistentProperties.put(_linkedSlotID.address());
}


ImplAAFFileDescriptor::~ImplAAFFileDescriptor ()
{}


//@access Public Members

AAFRESULT STDMETHODCALLTYPE
	ImplAAFFileDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetLength (aafLength_t length)
{
	_length = length;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetLength (aafLength_t *pLength)
{
	if(pLength == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pLength = _length;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetCodecDef (ImplAAFCodecDef *pDef)
{
  if (! pDef)
    return AAFRESULT_NULL_PARAM;
  if (!pDef->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  if (!aafLookupCodecDef( this, pDef ) )
    return AAFRESULT_INVALID_OBJ;

  _codecDef = pDef;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetCodecDef (ImplAAFCodecDef **ppDef)
{
	if(ppDef == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_codecDef.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;	

	// Should isVoid() be sufficient? It's not in the case
	// of references to MXF container definitions, which are not in
	// the built-in dictionary.
        /*
	if (_codecDef.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
        */
	if (!_codecDef)
		return AAFRESULT_OBJECT_NOT_FOUND;

	*ppDef = _codecDef;
	ASSERTU (*ppDef);

  if (*ppDef)
	 (*ppDef)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetSampleRate (const aafRational_t & rate)
{
	_sampleRate = rate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetSampleRate (aafRational_t *pRate)
{
	if(pRate == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pRate = _sampleRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetContainerFormat (ImplAAFContainerDef *pDef)
{
	if( pDef == NULL )
		return AAFRESULT_NULL_PARAM;
	if( !aafLookupContainerDef( this, pDef ) )
		return AAFRESULT_INVALID_OBJ;

	_containerFmt = pDef;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetContainerFormat (ImplAAFContainerDef **ppDef)
{
	if(ppDef == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_containerFmt.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;	

	if (_containerFmt.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;

	// Should the isVoid() above be sufficient? It's not in the case
	// of references to MXF container definitions, which are not in
	// the built-in dictionary.
	if (!_containerFmt)
		return AAFRESULT_OBJECT_NOT_FOUND;

	*ppDef = _containerFmt;
	ASSERTU (*ppDef);
	 (*ppDef)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind  == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kAAFFileMob;
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetLinkedSlotID (aafUInt32 linkedSlotID)
{
	_linkedSlotID = linkedSlotID;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetLinkedSlotID (aafUInt32* pLinkedSlotID)
{
	if (pLinkedSlotID == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_linkedSlotID.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLinkedSlotID = _linkedSlotID;

	return AAFRESULT_SUCCESS;
}
