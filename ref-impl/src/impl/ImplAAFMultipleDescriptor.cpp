//=---------------------------------------------------------------------=
//
// $Id: ImplAAFMultipleDescriptor.cpp,v 1.5 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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



#ifndef __ImplAAFMultipleDescriptor_h__
#include "ImplAAFMultipleDescriptor.h"
#endif

#include "OMAssertions.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"
#include "AAFClassIDs.h"

ImplAAFMultipleDescriptor::ImplAAFMultipleDescriptor ()
: _Descriptors(         PID_MultipleDescriptor_FileDescriptors,          L"FileDescriptors")
{
  _persistentProperties.put(_Descriptors.address());
}


ImplAAFMultipleDescriptor::~ImplAAFMultipleDescriptor ()
{
	// Release all of the Descriptor pointers.
	aafUInt32 count = _Descriptors.count();
	for (aafUInt32 i = 0; i < count; i++)
	{
		ImplAAFFileDescriptor *pDescriptor = _Descriptors.clearValueAt(i);
		if (pDescriptor)
		{
		  pDescriptor->ReleaseReference();
		  pDescriptor = 0;
		}
	}
}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFMultipleDescriptor::Initialize ()
{
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::CountFileDescriptors (aafUInt32 *pCount)
{
	if (! pCount)
	{
		return AAFRESULT_NULL_PARAM;
	}

	*pCount = _Descriptors.count();
	return(AAFRESULT_SUCCESS);
}

  //@comm The number of Descriptors may be zero if the essence is in the current file.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::AppendFileDescriptor (ImplAAFFileDescriptor *pDescriptor)
{
	if(pDescriptor == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(pDescriptor->attached())
		return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

	_Descriptors.appendValue(pDescriptor);
	pDescriptor->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

  //@comm    Use this function to add a Descriptor to be scanned last when searching for
  // the essence (a new primary location).

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::PrependFileDescriptor (ImplAAFFileDescriptor *pDescriptor)
{
	if(pDescriptor == NULL)
		return(AAFRESULT_NULL_PARAM);
  if (pDescriptor->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  _Descriptors.prependValue(pDescriptor);
	pDescriptor->AcquireReference();

	return AAFRESULT_SUCCESS;
}

  //@comm    Use this function to add a Descriptor to be scanned first when searching for
  // the essence (a secondary location for the essence).


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::InsertFileDescriptorAt (aafUInt32 index,
											   ImplAAFFileDescriptor *pDescriptor)
{
	if (NULL == pDescriptor)
		return AAFRESULT_NULL_PARAM;
  if (pDescriptor->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  if (index > _Descriptors.count())
    return AAFRESULT_BADINDEX;

	_Descriptors.insertAt(pDescriptor, index);
	pDescriptor->AcquireReference();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::GetFileDescriptorAt (aafUInt32 index,
											ImplAAFFileDescriptor ** ppDescriptor)
{
	if (! ppDescriptor) return AAFRESULT_NULL_PARAM;
	
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountFileDescriptors (&count);
	if (AAFRESULT_FAILED (hr)) return hr;
	
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	_Descriptors.getValueAt(*ppDescriptor, index);
	(*ppDescriptor)->AcquireReference();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::RemoveFileDescriptorAt (aafUInt32 index)
{
	if (index >= _Descriptors.count())
	  return AAFRESULT_BADINDEX;
	
	ImplAAFFileDescriptor *pDescriptor = _Descriptors.removeAt(index);
  if (pDescriptor)
  {
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pDescriptor->ReleaseReference ();
  }
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMultipleDescriptor::GetFileDescriptors (
      ImplEnumAAFFileDescriptors **ppEnum)
{
  if (ppEnum == NULL)
    return AAFRESULT_NULL_PARAM;

  ImplEnumAAFFileDescriptors *theEnum = (ImplEnumAAFFileDescriptors *)CreateImpl(CLSID_EnumAAFFileDescriptors);
  
  XPROTECT()
  {
    OMStrongReferenceVectorIterator<ImplAAFFileDescriptor> *iter =
        new OMStrongReferenceVectorIterator<ImplAAFFileDescriptor>(_Descriptors);
    if (iter == 0)
        RAISE(AAFRESULT_NOMEMORY);
    CHECK(theEnum->Initialize(&CLSID_EnumAAFFileDescriptors, this, iter));
    *ppEnum = theEnum;
  }
  XEXCEPT
  {
    if (theEnum)
      theEnum->ReleaseReference();
    theEnum = 0;
  }
  XEND;
  
  return AAFRESULT_SUCCESS;
}


// Internal to the toolkit functions
AAFRESULT
    ImplAAFMultipleDescriptor::GetNthDescriptor (aafInt32 index, ImplAAFFileDescriptor **ppDescriptor)
{
	if(ppDescriptor == NULL)
		return(AAFRESULT_NULL_PARAM);
  if ((aafUInt32)index >= _Descriptors.count())
		return AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???

	_Descriptors.getValueAt(*ppDescriptor, index);
  ASSERTU(*ppDescriptor); // Descriptor should never be NULL.
	(*ppDescriptor)->AcquireReference();

	return AAFRESULT_SUCCESS;
}

