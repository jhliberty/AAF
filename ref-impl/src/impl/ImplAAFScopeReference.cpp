//=---------------------------------------------------------------------=
//
// $Id: ImplAAFScopeReference.cpp,v 1.12 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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



/*************************************************************************
 * 
 * @class AAFScopeReference | AAFScopeReference refers to a section in the 
 *			specified AAFNestedScope slo tor AAFMobSLot.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/



#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFScopeReference_h__
#include "ImplAAFScopeReference.h"
#endif

#include <string.h>
#include "AAFResult.h"


ImplAAFScopeReference::ImplAAFScopeReference ():
	_relativeScope(			PID_ScopeReference_RelativeScope,		L"RelativeScope"),
	_relativeSlot(			PID_ScopeReference_RelativeSlot,		L"RelativeSlot")
{
	_persistentProperties.put(		_relativeScope.address());
	_persistentProperties.put(		_relativeSlot.address());
}


ImplAAFScopeReference::~ImplAAFScopeReference ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::Initialize(ImplAAFDataDef *pDataDef,
	aafUInt32 RelativeScope,aafUInt32 RelativeSlot)
{
	if(!pDataDef)
		return(AAFRESULT_NULL_PARAM);

	_relativeScope = RelativeScope;
	_relativeSlot = RelativeSlot;

	return(ImplAAFComponent::SetDataDef(pDataDef));
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::Create (aafUInt32 RelativeScope, aafUInt32 RelativeSlot)
{

	_relativeScope = RelativeScope;
	_relativeSlot = RelativeSlot;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::GetRelativeScope (aafUInt32* pnRelativeScope)
{
	if (pnRelativeScope == NULL)
		return AAFRESULT_NULL_PARAM;

	*pnRelativeScope = _relativeScope;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::GetRelativeSlot (aafUInt32* pnRelativeSlot)
{
	if (pnRelativeSlot == NULL)
		return AAFRESULT_NULL_PARAM;

	*pnRelativeSlot = _relativeSlot;

	return AAFRESULT_SUCCESS;
}





