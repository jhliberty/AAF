//=---------------------------------------------------------------------=
//
// $Id: CAAFDefaultStream.cpp,v 1.10 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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

#include "CAAFDefaultStream.h"

#include <assert.h>
#include "AAFResult.h"

const CLSID CLSID_AAFDefaultStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

// Default Interface for AAFDefaultStream 
// {83402902-9146-11d2-8088-006008143e6f}
const IID IID_IAAFEssenceStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


CAAFDefaultStream::CAAFDefaultStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFUnknown (pControllingUnknown)
{
}


CAAFDefaultStream::~CAAFDefaultStream ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Seek (aafInt64  byteOffset)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::SeekRelative (aafInt32  byteOffset)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::IsPosValid (aafInt64  byteOffset,
        aafBool *  isValid)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::GetPosition (aafInt64 *  position)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::GetLength (aafInt64 *  position)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::omcFlushCache ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::SetCacheSize (aafInt32  itsSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


//
// 
// 
HRESULT CAAFDefaultStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFEssenceStream) 
    { 
        *ppvObj = (IAAFEssenceStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
//
// Define the contrete object support implementation.
// 
HRESULT CAAFDefaultStream::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFDefaultStream *pAAFDefaultStream = new CAAFDefaultStream(pUnkOuter);
 	if (NULL == pAAFDefaultStream)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceStream *>(pAAFDefaultStream);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }
