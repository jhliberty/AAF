//=---------------------------------------------------------------------=
//
// $Id: ImplAAFPropertyValue.cpp,v 1.20 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMProperty.h"
#include "OMPropertyDefinition.h"



#include "OMAssertions.h"
#include <string.h>


ImplAAFPropertyValue::ImplAAFPropertyValue ()
  : _pType (NULL),
  _property(NULL),
  _propertyContainer(NULL)
{}


ImplAAFPropertyValue::~ImplAAFPropertyValue ()
{
  // BobT: type is not to be reference counted.
  _pType = NULL;
  _property = NULL;
  
#if defined(REFERENCE_PROPERTY_CONTAINER)
  if (_propertyContainer)
  {
    _propertyContainer->ReleaseReference();
    _propertyContainer = NULL;
  }
#endif // #if defined(REFERENCE_PROPERTY_CONTAINER)
}

void ImplAAFPropertyValue::SetProperty(OMProperty * pProperty)
{
  _property = pProperty;
}

AAFRESULT ImplAAFPropertyValue::Initialize (
	  const ImplAAFTypeDef *propertyType)
{
  if (! propertyType) 
    return AAFRESULT_NULL_PARAM;
    
  // make sure we haven't been init'd yet
  ASSERTU (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  
  ASSERTU (! _pType);

  _pType = propertyType;
  // BobT: type is not to be reference counted.
  return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFPropertyValue::Initialize (
  const ImplAAFTypeDef *propertyType,    
  OMProperty *property)
{
  ASSERTU (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  ASSERTU (property);
  if (NULL == propertyType || NULL == property)
    return AAFRESULT_NULL_PARAM;

  // Get the type definition. This must be a stream type.
  ASSERTU (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  ASSERTU (type);

  // Get the storable container for this property. Since this is a "direct 
  // access" interface we need to hold onto a reference so tha the container
  // is not deleted.
  ImplAAFRoot * propertyContainer = dynamic_cast<ImplAAFRoot *>
                                      (property->propertySet()->container());
  ASSERTU (propertyContainer);
  if (NULL == propertyContainer)
    return AAFRESULT_INVALID_PARAM;
  
  // Save our initialized member data.
  SetType(propertyType);
  SetProperty(property);
  _propertyContainer = propertyContainer;
#if defined(REFERENCE_PROPERTY_CONTAINER)
  _propertyContainer->AcquireReference();
#endif // #if defined(REFERENCE_PROPERTY_CONTAINER)
    
  return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFPropertyValue::SetType (
	  const ImplAAFTypeDef * pType)
{
  return Initialize(pType);
}

// accessor methods for subclasses:
const ImplAAFTypeDef *ImplAAFPropertyValue::type(void) const
{
  return _pType;
}



OMProperty *ImplAAFPropertyValue::property(void) const
{
  return _property;
}



ImplAAFRoot *ImplAAFPropertyValue::propertyContainer(void) const
{
  return _propertyContainer;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::GetType (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *ppTypeDef = const_cast<ImplAAFTypeDef *>(type());
  ASSERTU (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::IsDefinedType (
      aafBool * pIsDefined)
{
  if (! pIsDefined)
	return AAFRESULT_NULL_PARAM;
  // Currently the type returned by GetType() is always the defined type.
  *pIsDefined = kAAFTrue;
  return AAFRESULT_SUCCESS;
}


const ImplAAFTypeDef * ImplAAFPropertyValue::pvtGetType (void) const
{
  return _pType;
}

AAFRESULT STDMETHODCALLTYPE ImplAAFPropertyValue::WriteTo(
      OMProperty* /* pOmProp */)
{
  // This method should be pure virtual but dodo won't allow it
  ASSERTU(false);
  return AAFRESULT_SUCCESS;
}
