//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFHTMLClip.cpp,v 1.11 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#include "CAAFHTMLClip.h"
#include "ImplAAFHTMLClip.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


// CLSID for AAFHTMLClip 
// {e684d783-b935-11d2-bf9d-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFHTMLClip = { 0xe684d783, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFHTMLClip::CAAFHTMLClip (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTextClip (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFHTMLClip * newRep;
      newRep = new ImplAAFHTMLClip;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFHTMLClip::~CAAFHTMLClip ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFHTMLClip::SetBeginAnchor (aafCharacter_constptr  pBeginAnchor)
{
  HRESULT hr;

  ImplAAFHTMLClip * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHTMLClip*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetBeginAnchor
       (pBeginAnchor);
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
    CAAFHTMLClip::GetBeginAnchor (aafCharacter *  pBeginAnchor,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFHTMLClip * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHTMLClip*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetBeginAnchor
       (pBeginAnchor,
        bufSize);
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
    CAAFHTMLClip::GetBeginAnchorBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFHTMLClip * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHTMLClip*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetBeginAnchorBufLen
       (pBufSize);
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
    CAAFHTMLClip::SetEndAnchor (aafCharacter_constptr  pEndAnchor)
{
  HRESULT hr;

  ImplAAFHTMLClip * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHTMLClip*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetEndAnchor
       (pEndAnchor);
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
    CAAFHTMLClip::GetEndAnchor (aafCharacter *  pEndAnchor,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFHTMLClip * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHTMLClip*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetEndAnchor
       (pEndAnchor,
        bufSize);
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
    CAAFHTMLClip::GetEndAnchorBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFHTMLClip * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHTMLClip*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetEndAnchorBufLen
       (pBufSize);
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

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFHTMLClip::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFHTMLClip)) 
    { 
        *ppvObj = (IAAFHTMLClip *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTextClip::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFHTMLClip)

