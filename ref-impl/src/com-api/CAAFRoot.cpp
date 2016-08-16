//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFRoot.cpp,v 1.11 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#include "CAAFRoot.h"
#include "ImplAAFRoot.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>

// CLSID for AAFRoot 
// {7C3712C1-390D-11D2-841B-00600832ACB8}
EXTERN_C const CLSID CLSID_AAFRoot = { 0x7C3712C1, 0x390D, 0x11D2, { 0x84, 0x1B, 0x00, 0x60, 0x08, 0x32, 0xAC, 0xB8 } };



CAAFRoot::CAAFRoot (IUnknown * pControllingUnknown, aafBool /*doInit*/)
  : CAAFUnknown(pControllingUnknown),
    _rep(NULL)
{}


CAAFRoot::~CAAFRoot ()
{
  try {
    // The base class destructor of ImplAAFRoot must be declared as virtual.
    // Eventhough the ImplAAFRoot is reference counted we call delete 
    // directly since the ImplAAFRoot::ReleaseReference() delegates its
    // release to CAAFRoot::Release().
    if (_rep)
    {
      delete _rep;
      _rep = NULL;
    } 
  } catch (...) {
    // Exceptions should be handled by the impl code. However, if an
    // unhandled exception occurs, control reaches here. We must not
    // allow the unhandled exception to leave this destructor and
    // reach the client code. Since we cannot return an exception code
    // the exception is swallowed.
  }
}


void CAAFRoot::InitRep (ImplAAFRoot * newRep)
{
  assert (newRep);
  assert (!_rep);
  _rep = newRep;
  newRep->InitContainer (this);
}


ImplAAFRoot * CAAFRoot::GetRepObject ()
{
  assert (_rep);
  return _rep;
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFRoot::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFRoot)) 
    { 
        *ppvObj = (IAAFRoot *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Return private implementation pointer for delegation.
// NOTE: This is NOT the pointer to the COM object's implementation
// object!
//
HRESULT STDMETHODCALLTYPE
    CAAFRoot::GetImplRep (void **ppRep)
{
    if (!ppRep)
        return E_INVALIDARG;  
    *ppRep = static_cast<void*>(GetRepObject());
    return S_OK;
}

//
// Intialize class extensions. This method is called after the
// contained Impl object has been completely initialized. This
// allows the aggregated extension object access to all of the
// interfaces and property data of its controlling unknown.
//
HRESULT STDMETHODCALLTYPE
    CAAFRoot::InitializeExtension(REFCLSID /*clsid*/)
{
  // Extensions are not supported for the shared
  // CAAFRoot module.
  return AAFRESULT_NOT_IMPLEMENTED;
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFRoot)



