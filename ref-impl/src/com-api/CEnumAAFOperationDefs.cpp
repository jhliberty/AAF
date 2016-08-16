//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CEnumAAFOperationDefs.cpp,v 1.10 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#include "CEnumAAFOperationDefs.h"
#include "ImplEnumAAFOperationDefs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>

#include "CAAFOperationDef.h"
#include "ImplAAFOperationDef.h"

// CLSID for EnumAAFOperationDefs 
// {92B88022-2B2B-11D2-BF7E-00104BC9156D}
EXTERN_C const CLSID CLSID_EnumAAFOperationDefs = { 0x92B88022, 0x2B2B, 0x11D2, { 0xBF, 0x7E, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CEnumAAFOperationDefs::CEnumAAFOperationDefs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFOperationDefs * newRep;
      newRep = new ImplEnumAAFOperationDefs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFOperationDefs::~CEnumAAFOperationDefs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFOperationDefs::NextOne (IAAFOperationDef ** ppOperationDefs)
{
  HRESULT hr;

  ImplEnumAAFOperationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFOperationDefs*> (pO);
  assert (ptr);

  //
  // set up for ppOperationDefs
  //
  ImplAAFOperationDef * internalppOperationDefs = NULL;
  ImplAAFOperationDef ** pinternalppOperationDefs = NULL;
  if (ppOperationDefs)
    {
      pinternalppOperationDefs = &internalppOperationDefs;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppOperationDefs);
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
  // cleanup for ppOperationDefs
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppOperationDefs)
        {
          pUnknown = static_cast<IUnknown *> (internalppOperationDefs->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFOperationDef, (void **)ppOperationDefs);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppOperationDefs->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFOperationDefs::Next (aafUInt32  count,
        IAAFOperationDef ** ppOperationDefs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFOperationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFOperationDefs*> (pO);
  assert (ptr);

  //
  // set up for ppOperationDefs
  //
  ImplAAFOperationDef ** internalppOperationDefs = NULL;
  assert (count >= 0);
  internalppOperationDefs = new ImplAAFOperationDef*[count];
  assert (internalppOperationDefs);

  ImplAAFOperationDef ** pinternalppOperationDefs = NULL;
  if (ppOperationDefs)
    {
      pinternalppOperationDefs = internalppOperationDefs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppOperationDefs,
        pNumFetched);
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
  // cleanup for ppOperationDefs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppOperationDefs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFOperationDef, (void **)(ppOperationDefs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppOperationDefs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppOperationDefs;
  internalppOperationDefs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFOperationDefs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFOperationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFOperationDefs*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Skip
       (count);
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
    CEnumAAFOperationDefs::Reset ()
{
  ImplEnumAAFOperationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFOperationDefs*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Reset();
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
    CEnumAAFOperationDefs::Clone (IEnumAAFOperationDefs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFOperationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFOperationDefs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFOperationDefs * internalppEnum = NULL;
  ImplEnumAAFOperationDefs ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->Clone
       (pinternalppEnum);
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
  // cleanup for ppEnum
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEnum)
        {
          pUnknown = static_cast<IUnknown *> (internalppEnum->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFOperationDefs, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
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
HRESULT CEnumAAFOperationDefs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFOperationDefs)) 
    { 
        *ppvObj = (IEnumAAFOperationDefs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFOperationDefs)

