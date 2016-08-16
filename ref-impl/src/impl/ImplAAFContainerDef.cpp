//=---------------------------------------------------------------------=
//
// $Id: ImplAAFContainerDef.cpp,v 1.12 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFContainerDef_h__
#include "ImplAAFContainerDef.h"
#endif

#include <string.h>
#include "AAFPropertyIDs.h"


ImplAAFContainerDef::ImplAAFContainerDef () :
_isIdentified(		PID_ContainerDefinition_EssenceIsIdentified, L"EssenceIsIdentified")
{
  _persistentProperties.put(_isIdentified.address());
}


ImplAAFContainerDef::~ImplAAFContainerDef ()
{}

  
AAFRESULT STDMETHODCALLTYPE
    ImplAAFContainerDef::Initialize (
      const aafUID_t & id,
	  const aafWChar * pName,
	  const aafWChar * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
	else
	{
	  return pvtInitialize(id, pName, pDesc);
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContainerDef::EssenceIsIdentified (
      aafBool *pEssenceIsIdentified)
{
	
	if (pEssenceIsIdentified == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	else if (!_isIdentified.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	else
	{
		*pEssenceIsIdentified = _isIdentified;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContainerDef::SetEssenceIsIdentified (
      aafBool EssenceIsIdentified)
{
	_isIdentified = EssenceIsIdentified;
	return AAFRESULT_SUCCESS;
}




