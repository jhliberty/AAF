//=---------------------------------------------------------------------=
//
// $Id: ImplAAFOperationGroup.cpp,v 1.64 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFOperationDef_h__
#include "ImplAAFOperationDef.h"
#endif

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#ifndef __ImplEnumAAFOperationDefs_h__
#include "ImplEnumAAFOperationDefs.h"
#endif

#ifndef __ImplEnumAAFParameterDefs_h__
#include "ImplEnumAAFParameterDefs.h"
#endif

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFOperationGroup_h__
#include "ImplAAFOperationGroup.h"
#endif

#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"
#include "ImplEnumAAFParameters.h"

#include "OMAssertions.h"
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "AAFUtils.h"
#include "AAFDefUIDs.h"

extern "C" const aafClassID_t CLSID_AAFSourceReference;
extern "C" const aafClassID_t CLSID_AAFParameter;
extern "C" const aafClassID_t CLSID_AAFSegment;
extern "C" const aafClassID_t CLSID_EnumAAFParameters;


ImplAAFOperationGroup::ImplAAFOperationGroup ()
: _operationDefinition( PID_OperationGroup_Operation, L"OperationDefinition", L"/Header/Dictionary/OperationDefinitions", PID_DefinitionObject_Identification),
  _inputSegments( PID_OperationGroup_InputSegments, L"InputSegments"),
  _parameters( PID_OperationGroup_Parameters, L"Parameters", PID_DefinitionObject_Identification),
  _bypassOverride( PID_OperationGroup_BypassOverride, L"BypassOverride"),
  _rendering( PID_OperationGroup_Rendering, L"Rendering")
{
	_persistentProperties.put(_operationDefinition.address());
	_persistentProperties.put(_inputSegments.address());
	_persistentProperties.put(_parameters.address());
	_persistentProperties.put(_bypassOverride.address());
	_persistentProperties.put(_rendering.address());
}


ImplAAFOperationGroup::~ImplAAFOperationGroup ()
{
	// Release all of the mob slot pointers.
	aafUInt32 count = _inputSegments.count();
	for (aafUInt32 i = 0; i < count; i++)
	{
		ImplAAFSegment *pSeg = _inputSegments.clearValueAt(i);
		if (pSeg)
		{
		  pSeg->ReleaseReference();
		  pSeg = 0;
		}
	}
	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameter>parameters(_parameters);
	while(++parameters)
	{
		ImplAAFParameter *pParm = parameters.clearValue();
		if (pParm)
		{
		  pParm->ReleaseReference();
		  pParm = 0;
		}
	}
	ImplAAFSourceReference *ref = _rendering.clearValue();
	if (ref)
	{
	  ref->ReleaseReference();
	  ref = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::Initialize(ImplAAFDataDef * pDataDef,
							 aafLength_t    length,
                             ImplAAFOperationDef* pOperationDef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
//	ImplAAFOperationDef*	pOldOperationDef = NULL;

	if (pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (pDataDef == NULL)
		return AAFRESULT_NULL_PARAM;

  // Make sure objects are already attached (to the dictionary).
  if (!pDataDef->attached() || !pOperationDef->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;

  // Check if given definition is in the dict.
  if( !aafLookupOperationDef( this, pOperationDef ) )
    return AAFRESULT_INVALID_OBJ;

	XPROTECT()
	{
		CHECK(SetNewProps(length, pDataDef));

    // The operation definition is a weak reference that is
    // owned by the dictionary so we do NOT need to reference
    // count it.
		_operationDefinition = pOperationDef;
	}
	XEXCEPT
	{
	}
	XEND;

	return rc;
}

	//@comm  This function takes an already created OperationGroup definition object as an argument.
	//@comm  To add slots to the OperationGroup, call AddSlot.
	//@comm  To add renderings, call SetRender.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetOperationDefinition (ImplAAFOperationDef **OperationDef)
{
  ImplAAFOperationDef *pOpDef = _operationDefinition;
  if(!pOpDef)
		return AAFRESULT_OBJECT_NOT_FOUND;

  *OperationDef = pOpDef;
  ASSERTU (*OperationDef);
  (*OperationDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::SetOperationDefinition (ImplAAFOperationDef *OperationDef)
{
	if(OperationDef == NULL)
		return AAFRESULT_NULL_PARAM;

  // Make sure object is already attached (to the dictionary).
  if (!OperationDef->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;

  // Check if given definition is in the dict.
  if( !aafLookupOperationDef( this, OperationDef ) )
    return AAFRESULT_INVALID_OBJ;

	_operationDefinition = OperationDef;

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiOperationGroupGetInfo


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetRender (ImplAAFSourceReference **sourceRef)
{
	if(sourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (_rendering.isPresent())
	{
		if (_rendering)
		{
			*sourceRef = _rendering;
			_rendering->AcquireReference();
		}
		else
			return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
		return AAFRESULT_PROP_NOT_PRESENT;

	return AAFRESULT_SUCCESS;
}

	//@comm If this property does not exist the error
	// AAFRESULT_PROP_NOT_PRESENT will be returned.
	//@comm Working and final renderings are handled by using
	// a media group as the segment.
	//@comm Replaces omfiOperationGroupGetFinalRender and omfiOperationGroupGetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::IsATimeWarp (aafBool *isTimeWarp)
{
	ImplAAFOperationDef	*def = NULL;
	
	XPROTECT()
	{
		if(isTimeWarp == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		CHECK(GetOperationDefinition(&def));
		CHECK(def->IsTimeWarp (isTimeWarp));
		def->ReleaseReference();
		def = NULL;
	}
	XEXCEPT
	{
		if(def)
		  def->ReleaseReference();
		def = 0;
	}
	XEND

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiOperationGroupIsATimeWarp


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetBypassOverride (aafUInt32* pBypassOverride)
{
	if(pBypassOverride == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_bypassOverride.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBypassOverride = _bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm If the property does not exist, the error AAFRESULT_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiOperationGroupGetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::CountSourceSegments (aafUInt32 *pNumSources)
{
	if(pNumSources == NULL)
		return AAFRESULT_NULL_PARAM;

	aafUInt32 numSlots = _inputSegments.count();
	
	*pNumSources = numSlots;

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces omfiOperationGroupGetNumSlots


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::CountParameters (aafUInt32 * pNumParameters)
{
   aafUInt32 numSlots;

	if(pNumParameters == NULL)
		return AAFRESULT_NULL_PARAM;
	numSlots = _parameters.count();
	
	*pNumParameters = numSlots;

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces omfiOperationGroupGetNumSlots

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::IsValidTranOperation (aafBool * validTransition)
{
	ImplAAFOperationDef	*def = NULL;
	aafInt32			numInputs;
	
	XPROTECT()
	{
		if(validTransition == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		CHECK(GetOperationDefinition(&def));
		CHECK(def->GetNumberInputs (&numInputs));
		*validTransition = (numInputs == 2 ? kAAFTrue : kAAFFalse);
		//!!!Must also have a "level" parameter (Need definition for this!)
		def->ReleaseReference();
		def = NULL;
	}
	XEXCEPT
	{
		if(def)
		  def->ReleaseReference();
		def = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::AddParameter (ImplAAFParameter *pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

  // Make sure object is not already attached.
  if (pValue->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_parameters.appendValue(pValue);
	pValue->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces part of omfiOperationGroupAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::AppendInputSegment (ImplAAFSegment * value)
{
	if(value == NULL)
		return(AAFRESULT_NULL_PARAM);

  // Make sure object is not already attached.
  if (value->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_inputSegments.appendValue(value);
	value->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiOperationGroupAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::PrependInputSegment (ImplAAFSegment * value)
{
  if(value == NULL)
		return(AAFRESULT_NULL_PARAM);

  // Make sure object is not already attached.
  if (value->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  _inputSegments.prependValue(value);
  value->AcquireReference();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::InsertInputSegmentAt
      (aafUInt32 index,
	   ImplAAFSegment * value)
{
  if(value == NULL)
		return(AAFRESULT_NULL_PARAM);

  aafUInt32 count;
  AAFRESULT ar;
  ar = CountSourceSegments (&count);
  if (AAFRESULT_FAILED (ar)) return ar;

  if (index > (aafUInt32)count)
    return AAFRESULT_BADINDEX;

  // Make sure object is not already attached.
  if (value->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  _inputSegments.insertAt(value,index);
  value->AcquireReference();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::SetRender (ImplAAFSourceReference *sourceRef)
{
	if(sourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

  // Make sure object is not already attached.
  if (sourceRef->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_rendering = sourceRef;
	_rendering->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiOperationGroupSetFinalRender and omfiOperationGroupSetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::SetBypassOverride (aafUInt32  bypassOverride)
{
	_bypassOverride = bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiOperationGroupSetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::LookupParameter (aafArgIDType_t  argID,
                           ImplAAFParameter ** ppParameter)
{
#if 1
	if (!ppParameter) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_parameters.find((*reinterpret_cast<const OMObjectIdentification *>(&argID)),
                             *ppParameter))
	{
		ASSERTU(NULL != *ppParameter);
		(*ppParameter)->AcquireReference();
	}
	else
	{
	// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);

#else
	ImplAAFParameter	*parm = NULL;
	ImplAAFParameterDef	*parmDef = NULL;
	aafInt32			numParm, n;
	aafUID_t			testAUID;
	aafBool				found;

	XPROTECT()
	{
		if(ppParameter == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
	
		found = kAAFFalse;
		CHECK(CountParameters (&numParm))
		for(n = 0; n < numParm; n++)
		{
			_parameters.getValueAt(parm, n);
			if (parm)
			{
				CHECK(parm->GetParameterDefinition (&parmDef));	
				CHECK(parmDef->GetAUID(&testAUID));
				parmDef->ReleaseReference();
				parmDef = NULL;
				if(EqualAUID(&testAUID, &argID))
				{
					parm->AcquireReference();
					*ppParameter = parm;
					found = kAAFTrue;
					break;
				}

			}
		}
		if(!found)
			RAISE(AAFRESULT_PARAMETER_NOT_FOUND);
	}
	XEXCEPT
	{
//		if(parm != NULL)
//		  parm->ReleaseReference();
//		parm = 0;
		if(parmDef != NULL)
		  parmDef->ReleaseReference();
		parmDef = 0;
	}
	XEND

	return AAFRESULT_SUCCESS;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetParameters
        (// @parm [out] enumerator across parameters
         ImplEnumAAFParameters ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFParameters *theEnum = (ImplEnumAAFParameters *)CreateImpl (CLSID_EnumAAFParameters);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameter>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameter>(_parameters);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFParameters, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetInputSegmentAt (aafUInt32  index,
                           ImplAAFSegment ** ppInputSegment)
{
	ImplAAFSegment	*obj;

	XPROTECT()
	{
		_inputSegments.getValueAt(obj, index);
		if (obj)
			obj->AcquireReference();
		else
			RAISE(AAFRESULT_NO_MORE_OBJECTS); // AAFRESULT_BADINDEX ???
		*ppInputSegment = obj;
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::RemoveInputSegmentAt (aafUInt32  index)
{
	aafUInt32 count;
	AAFRESULT hr;
	ImplAAFSegment	*pSeg;
	
	hr = CountSourceSegments (&count);
	if (AAFRESULT_FAILED (hr)) return hr;
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	pSeg = _inputSegments.removeAt(index);
	if(pSeg)
		pSeg->ReleaseReference();

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFOperationGroup::ChangeContainedReferences(aafMobID_constref from,
													aafMobID_constref to)
{
	aafUInt32			n, count;
	ImplAAFSegment		*seg = NULL;
	
	XPROTECT()
	{
		CHECK(CountSourceSegments (&count));
		for(n = 0; n < count; n++)
		{
			CHECK(GetInputSegmentAt (n, &seg));
			CHECK(seg->ChangeContainedReferences(from, to));
			seg->ReleaseReference();
			seg = NULL;
		}
	}
	XEXCEPT
	{
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


void ImplAAFOperationGroup::Accept(AAFComponentVisitor& visitor)
{
	aafUInt32 count = 0;
	CountSourceSegments(&count);
	for(aafUInt32 i=0; i<count; i++)
	{
		ImplAAFSegment* pSegment = 0;
		GetInputSegmentAt(i, &pSegment);

       	        pSegment->Accept(visitor);

		pSegment->ReleaseReference();
		pSegment = NULL;
	}

	// TODO
	// visitor.VisitOperationGroup(this);
}

