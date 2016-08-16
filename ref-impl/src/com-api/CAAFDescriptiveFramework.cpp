//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFDescriptiveFramework.cpp,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#include "CAAFDescriptiveFramework.h"
#include "ImplAAFDescriptiveFramework.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>




// CLSID for AAFDescriptiveFramework 
// {778f56fc-9e54-4334-abea-972639c68464}
EXTERN_C const CLSID CLSID_AAFDescriptiveFramework = { 0x778f56fc, 0x9e54, 0x4334, { 0xab, 0xea, 0x97, 0x26, 0x39, 0xc6, 0x84, 0x64 } };





CAAFDescriptiveFramework::CAAFDescriptiveFramework (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFDescriptiveFramework * newRep;
      newRep = new ImplAAFDescriptiveFramework;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFDescriptiveFramework::~CAAFDescriptiveFramework ()
{
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFDescriptiveFramework::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFDescriptiveFramework)) 
    { 
        *ppvObj = (IAAFDescriptiveFramework *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFDescriptiveFramework)

