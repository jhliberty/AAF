//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFStaticMobSlot.cpp,v 1.11 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#include "CAAFStaticMobSlot.h"
#include "ImplAAFStaticMobSlot.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>




// CLSID for AAFStaticMobSlot 
// {e684d763-b935-11d2-bf9d-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFStaticMobSlot = { 0xe684d763, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFStaticMobSlot::CAAFStaticMobSlot (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMobSlot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFStaticMobSlot * newRep;
      newRep = new ImplAAFStaticMobSlot;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFStaticMobSlot::~CAAFStaticMobSlot ()
{
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFStaticMobSlot::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFStaticMobSlot)) 
    { 
        *ppvObj = (IAAFStaticMobSlot *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFMobSlot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFStaticMobSlot)

