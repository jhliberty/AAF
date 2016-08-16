//=---------------------------------------------------------------------=
//
// $Id: ImplAAFWeakRefSetValue.cpp,v 1.8 2009/06/01 11:47:09 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFWeakRefSetValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

// TBD: Note the following include for ImplAAFClassDef.h really belongs
// in ImplAAFTypeDefWeakObjRef.h!
#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif


#include "OMProperty.h"

#include "OMAssertions.h"
#include <string.h>


ImplAAFWeakRefSetValue::ImplAAFWeakRefSetValue ()
{}


ImplAAFWeakRefSetValue::~ImplAAFWeakRefSetValue ()
{
}







AAFRESULT ImplAAFWeakRefSetValue::Initialize (
  const ImplAAFTypeDefSet *containerType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  result = ImplAAFRefSetValue::Initialize(containerType, property);
  
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefSetValue::WriteTo(
  OMProperty* pOmProp)
{
  return (ImplAAFRefSetValue::WriteTo(pOmProp));
}




  
//
// ImplAAFRefContainer overrides
//

// Return the type of object references in the container.
ImplAAFTypeDefObjectRef * ImplAAFWeakRefSetValue::GetElementType(void) const // the result is NOT reference counted.
{
  ImplAAFTypeDefObjectRef * pContainerElementType = NULL;
  ImplAAFTypeDefSet *pContainerType = NULL;
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDefSP pType, pElementType;
  
  result = GetType(&pType);
  ASSERTU(AAFRESULT_SUCCEEDED(result));
  if (AAFRESULT_SUCCEEDED(result))
  {
    pContainerType = dynamic_cast<ImplAAFTypeDefSet *>((ImplAAFTypeDef *)pType); // extract obj from smartptr
    ASSERTU(NULL != pContainerType);
    if (NULL != pContainerType)
    {
      result = pContainerType->GetElementType(&pElementType);
      ASSERTU(AAFRESULT_SUCCEEDED(result));
      if (AAFRESULT_SUCCEEDED(result))
      {
        pContainerElementType = dynamic_cast<ImplAAFTypeDefWeakObjRef *>((ImplAAFTypeDef *)pElementType); // extract obj from smartptr
      }
    }
  }
  
  ASSERTU(pContainerElementType);
  return pContainerElementType;
}


// Perform specialized validation for an object before it is added
// to a container.
AAFRESULT ImplAAFWeakRefSetValue::ValidateNewObject(ImplAAFStorable *pNewObject) const
{
  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  ASSERTU(NULL != pNewObject);
  if (!pNewObject->attached())
  {
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  }
 
  // Hand off to the OMReferenceContainer
  OMReferenceContainer* pReferenceContainer = referenceContainer();

  // Object refernce containers should only contain a single reference
  // to an object.
  if (pReferenceContainer->containsObject(pNewObject))
  {
    return AAFRESULT_TABLE_DUP_KEY; // Object is already in the set!
  }

  return AAFRESULT_SUCCESS;
}


// Perform any specialized cleanup of any object after it has been removed
// from the container.
bool ImplAAFWeakRefSetValue::usesReferenceCounting(void) const
{
  return false;
}
  
