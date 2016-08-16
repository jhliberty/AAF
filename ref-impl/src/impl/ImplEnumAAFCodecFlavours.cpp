//=---------------------------------------------------------------------=
//
// $Id: ImplEnumAAFCodecFlavours.cpp,v 1.13 2009/06/01 11:47:09 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplEnumAAFCodecFlavours_h__
#include "ImplEnumAAFCodecFlavours.h"
#endif

#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFCodecFlavours;

ImplEnumAAFCodecFlavours::ImplEnumAAFCodecFlavours ()
{
	_current = 0;
	_codec = 0;
}


ImplEnumAAFCodecFlavours::~ImplEnumAAFCodecFlavours ()
{
	if (_codec)
	{
		_codec->Release();
		_codec = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::NextOne (
      aafUID_t *pAAFCodecFlavour)
{
	aafUInt32			numElem;

	XPROTECT()
	{
		CHECK(_codec->CountFlavours(&numElem))
		if(pAAFCodecFlavour == 0)
			RAISE(AAFRESULT_NULL_PARAM);
		if(_current >= numElem)
			RAISE(AAFRESULT_NO_MORE_FLAVOURS);
		CHECK(_codec->GetIndexedFlavourID (_current, pAAFCodecFlavour));

		_current++;
	}
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Next (
      aafUInt32  count,
      aafUID_t *  pAAFCodecFlavours,
      aafUInt32 *  pFetched)
{
	aafUID_t*			pDef;
	aafUInt32			numDefs;
	HRESULT				hr = E_FAIL;

	if ((!pFetched && count != 1) || (pFetched && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	pDef = pAAFCodecFlavours;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(pDef);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		pDef++;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem;

	hr = _codec->CountFlavours(&numElem);
	if(hr != AAFRESULT_SUCCESS)
		return hr;

	newCurrent = _current + count;

	if(newCurrent < numElem)
	{
		_current = newCurrent;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = AAFRESULT_NO_MORE_FLAVOURS;
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Clone (
      ImplEnumAAFCodecFlavours **ppEnum)
{
	ImplEnumAAFCodecFlavours	*result;
	AAFRESULT				hr;

	result = (ImplEnumAAFCodecFlavours *)CreateImpl(CLSID_EnumAAFCodecFlavours);
	if (!result)
		return E_FAIL;

	hr = result->SetEnumCodec(_codec);

	if (SUCCEEDED(hr))
	{
		result->_current = _current;
		*ppEnum = result;
	}
	else
	{
	  result->ReleaseReference();
	  result = 0;
	  *ppEnum = 0;
	}
	
	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::SetEnumCodec( IAAFEssenceCodec *pCodec)
{
	if (_codec)
		_codec->Release();
	_codec = pCodec;
	if (pCodec)
		pCodec->AddRef();
	/**/

	return AAFRESULT_SUCCESS;
}



