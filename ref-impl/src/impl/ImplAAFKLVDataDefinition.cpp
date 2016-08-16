//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: ImplAAFKLVDataDefinition.cpp,v 1.4 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#ifndef __ImplAAFKLVDataDefinition_h__
#include "ImplAAFKLVDataDefinition.h"
#endif

#include "ImplAAFDictionary.h"

#include "AAFResult.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFPropertyDefs;

ImplAAFKLVDataDefinition::ImplAAFKLVDataDefinition ()

  : _klvDataTypeDef(   PID_KLVDataDefinition_KLVDataType,
	               L"KLVDataType",
	               L"/MetaDictionary/TypeDefinitions",
		       PID_MetaDefinition_Identification )
{
  _persistentProperties.put( _klvDataTypeDef.address() );
}


ImplAAFKLVDataDefinition::~ImplAAFKLVDataDefinition ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::Initialize (
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
    ImplAAFKLVDataDefinition::AddParentProperty (
      ImplAAFPropertyDef * pParentProperty )
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFKLVDataDefinition::GetParentProperties (
      ImplEnumAAFPropertyDefs ** ppEnum )
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::CountParentProperties (
      aafUInt32* pNumProperties )
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::RemoveParentProperty (
      ImplAAFPropertyDef * /*pParentProperty*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


// FIXME - With the exception of the isPresent() test, these type
// Set/Get methods are identical to
// ImplAAFParameterDef::{Set,Get}TypeDefinition(), and no doubt
// several others.  The code could be refactored to avoid the
// duplication.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::GetKLVDataType (
      ImplAAFTypeDef ** ppTypeDef )
{
  if ( !ppTypeDef ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !_klvDataTypeDef.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  if ( _klvDataTypeDef.isVoid() ) {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

  *ppTypeDef = _klvDataTypeDef;
  ASSERTU( *ppTypeDef );
  
  (*ppTypeDef)->AcquireReference();

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::SetKLVDataType (
      ImplAAFTypeDef* pTypeDef )
{
  if ( pTypeDef == NULL ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !aafLookupTypeDef( this, pTypeDef ) ) {
    return AAFRESULT_INVALID_OBJ;
  }

  _klvDataTypeDef = pTypeDef;

  return AAFRESULT_SUCCESS;
}
