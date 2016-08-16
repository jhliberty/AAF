//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRefValue.cpp,v 1.7 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFRefValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif


#include "OMProperty.h"
#include "OMRefProperty.h" // Include header for base class for singleton references.
#include "OMPropertyDefinition.h"

#include "OMAssertions.h"
#include <string.h>

#ifndef USE_IMPLSTORABLE
#define USE_IMPLSTORABLE 1
#endif

// Utility routine
/* static */ ImplAAFStorable * ImplAAFRefValue::ConvertOMObjectToRoot(OMObject *object)
{
  ImplAAFStorable* result = NULL;
#if USE_IMPLSTORABLE
  ImplAAFStorable *pObject = dynamic_cast<ImplAAFStorable *>(object);
  result = pObject;
#else
  ImplAAFObject *pObject = dynamic_cast<ImplAAFObject *>(object);
  if (NULL != pObject)
  {
    result = pObject;
  }
  else
  {
    ImplAAFMetaDefinition* metaDefinition = dynamic_cast<ImplAAFMetaDefinition *>(object);
    if (NULL != metaDefinition)
    {
      result = metaDefinition;
    }
  }
#endif  
  // A strong reference MUST be either an ImplAAFObject or an ImplAAFMetaDefintion!!!
  ASSERTU (NULL != result);
  return result;
}

/* static */ ImplAAFStorable * ImplAAFRefValue::ConvertRootToOMStorable(ImplAAFRoot *object)
{
  ImplAAFStorable* result = NULL;
#if USE_IMPLSTORABLE
  ImplAAFStorable *pObject = dynamic_cast<ImplAAFStorable *>(object);
  result = pObject;
#else
  ImplAAFObject *pObject = dynamic_cast<ImplAAFObject *>(object);
  if (NULL != pObject)
  {
    result = pObject;
  }
  else
  {
    ImplAAFMetaDefinition* metaDefinition = dynamic_cast<ImplAAFMetaDefinition *>(object);
    if (NULL != metaDefinition)
    {
      result = metaDefinition;
    }
  }
#endif  
  
  // A strong reference MUST be either an ImplAAFObject or an ImplAAFMetaDefintion!!!
  ASSERTU (NULL != result);
  return result;
}

ImplAAFRefValue::ImplAAFRefValue () :
  _localObject(NULL)
{}


ImplAAFRefValue::~ImplAAFRefValue ()
{
  // Release the old object (if necessary)
  SetLocalObject(NULL);
}



AAFRESULT ImplAAFRefValue::Initialize (
  const ImplAAFTypeDefObjectRef *referenceType)
{
  AAFRESULT result = ImplAAFPropertyValue::Initialize(referenceType);
  return result;
}


AAFRESULT ImplAAFRefValue::Initialize (
  const ImplAAFTypeDefObjectRef *referenceType,    
  OMProperty *property)
{
  AAFRESULT result = ImplAAFPropertyValue::Initialize(referenceType, property);
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefValue::WriteTo(
  OMProperty* pOmProp)
{
  if (NULL != property())
  {
    // Make sure that the given property is the same one that was used to 
    // initialize this property value. NOTE: Copying an object reference to a 
    // different OMProperty should be handled through another interface.
    if (pOmProp != property())
      return AAFRESULT_INVALID_PARAM;
  }
  
  // The first version of this class defers to the older   
//  return ImplAAFPropValData::WriteTo(pOmProp);
  return AAFRESULT_SUCCESS;
}


// Return the instance's property as a reference property.
OMReferenceProperty * ImplAAFRefValue::referenceProperty(void) const
{
  return (static_cast<OMReferenceProperty *>(property()));
}


void ImplAAFRefValue::SetLocalObject(ImplAAFStorable * localObject)
{
  if (NULL != _localObject)
    _localObject->ReleaseReference();

  _localObject = localObject;

  if (NULL != localObject)
    localObject->AcquireReference();
}


ImplAAFStorable * ImplAAFRefValue::GetLocalObject(void) const // not reference counted
{
  return _localObject;
}
