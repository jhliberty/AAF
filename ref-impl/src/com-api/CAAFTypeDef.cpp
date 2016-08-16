//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFTypeDef.cpp,v 1.11 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


// CLSID for AAFTypeDef 
// {dfbd6524-1d81-11d2-bf96-006097116212}
EXTERN_C const CLSID CLSID_AAFTypeDef = { 0xdfbd6524, 0x1d81, 0x11d2, { 0xbf, 0x96, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFTypeDef::CAAFTypeDef (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMetaDefinition (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDef * newRep;
      newRep = new ImplAAFTypeDef;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDef::~CAAFTypeDef ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDef::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  HRESULT hr;

  ImplAAFTypeDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetTypeCategory
       (pTid);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDef::RawAccessType (IAAFTypeDef ** ppRawTypeDef)
{
  HRESULT hr;

  ImplAAFTypeDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDef*> (pO);
  assert (ptr);

  //
  // set up for ppRawTypeDef
  //
  ImplAAFTypeDef * internalppRawTypeDef = NULL;
  ImplAAFTypeDef ** pinternalppRawTypeDef = NULL;
  if (ppRawTypeDef)
    {
      pinternalppRawTypeDef = &internalppRawTypeDef;
    }

  try
    {
      hr = ptr->RawAccessType
       (pinternalppRawTypeDef);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppRawTypeDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppRawTypeDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppRawTypeDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTypeDef, (void **)ppRawTypeDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppRawTypeDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTypeDef::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDef)) 
    { 
        *ppvObj = (IAAFTypeDef *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFMetaDefinition::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDef)

