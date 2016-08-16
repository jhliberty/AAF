//=---------------------------------------------------------------------=
//
// $Id: ImplAAFParameterDef.cpp,v 1.27 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#include "ImplAAFDictionary.h"

#include "OMAssertions.h"
#include <string.h>
#include <wchar.h>
#include "aafErr.h"

ImplAAFParameterDef::ImplAAFParameterDef ()
: _typeDef     ( PID_ParameterDefinition_Type,
                 L"Type",
                 L"/MetaDictionary/TypeDefinitions",
                 PID_MetaDefinition_Identification),
  _displayUnits(	PID_ParameterDefinition_DisplayUnits,			L"DisplayUnits")
{
	_persistentProperties.put(_typeDef.address());
	_persistentProperties.put(_displayUnits.address());
}


ImplAAFParameterDef::~ImplAAFParameterDef ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::Initialize (
      const aafUID_t & id,
	  const aafWChar * pName,
	  const aafWChar * pDesc,
    ImplAAFTypeDef * pType)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
	if (pName == NULL || pDesc == NULL || pType == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
	else
	{
    AAFRESULT result = pvtInitialize(id, pName, pDesc);
	  if (AAFRESULT_SUCCEEDED (result))
      result = SetTypeDef (pType);
	}
	return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
  if (! ppTypeDef)
	return AAFRESULT_NULL_PARAM;

   if(_typeDef.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFTypeDef *pTypeDef = _typeDef;

  *ppTypeDef = pTypeDef;
  ASSERTU (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	// Check if given definition is in the dict.
	if( !aafLookupTypeDef( this, pTypeDef ) )
		return AAFRESULT_INVALID_OBJ;

	_typeDef = pTypeDef;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnits (
      aafCharacter *pDisplayUnits,
      aafUInt32  bufSize)
{
	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_displayUnits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _displayUnits.copyToBuffer(pDisplayUnits, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnitsBufLen (
      aafUInt32 * pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_displayUnits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLen = _displayUnits.size();
	return(AAFRESULT_SUCCESS); 
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetDisplayUnits (
      const aafCharacter *pDisplayUnits)
{
	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pDisplayUnits)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_displayUnits = pDisplayUnits;

	return(AAFRESULT_SUCCESS); 
}



