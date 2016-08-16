//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFPhysicalDescriptor.cpp,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "CAAFPhysicalDescriptor.h"
#include "ImplAAFPhysicalDescriptor.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



// CLSID for AAFPhysicalDescriptor 
// {f128cd24-dc67-49d7-8b4b-e1211cb62095}
EXTERN_C const CLSID CLSID_AAFPhysicalDescriptor = { 0xf128cd24, 0xdc67, 0x49d7, { 0x8b, 0x4b, 0xe1, 0x21, 0x1c, 0xb6, 0x20, 0x95 } };





CAAFPhysicalDescriptor::CAAFPhysicalDescriptor (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFEssenceDescriptor (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFPhysicalDescriptor * newRep;
      newRep = new ImplAAFPhysicalDescriptor;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFPhysicalDescriptor::~CAAFPhysicalDescriptor ()
{
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFPhysicalDescriptor::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFPhysicalDescriptor)) 
    { 
        *ppvObj = (IAAFPhysicalDescriptor *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFEssenceDescriptor::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPhysicalDescriptor)

