//=---------------------------------------------------------------------=
//
// $Id: CAAFCDCIDescriptorHelper.cpp,v 1.12 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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
#include "CAAFCDCIDescriptorHelper.h"

#include <assert.h>
#include "AAFResult.h"

#include "AAF.h"
#include "AAFTypeDefUIDs.h"

#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFPropertyDefs.h"


// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw r;
}

inline void checkExpression(bool test, AAFRESULT r)
{
  if (!test)
    throw r;
}


inline void checkAssertion(bool test)
{
  if (!test)
    throw HRESULT(AAFRESULT_ASSERTION_VIOLATION);
}



CAAFCDCIDescriptorHelper::CAAFCDCIDescriptorHelper() :
	_filemob_unk(NULL),
	_filemob(NULL),
	_edes(NULL),
	_filedes(NULL),
	_dides(NULL),
	_cdcides(NULL)
{
}

CAAFCDCIDescriptorHelper::~CAAFCDCIDescriptorHelper()
{
	Clear();
}

void CAAFCDCIDescriptorHelper::Clear(void)
{
	// Release all of our references...
	if (NULL != _cdcides)
	{
		_cdcides->Release();
		_cdcides = NULL;
	}
	if (NULL != _dides)
	{
		_dides->Release();
		_dides = NULL;
	}
	if (NULL != _filedes)
	{
		_filedes->Release();
		_filedes = NULL;
	}
	if (NULL != _edes)
	{
		_edes->Release();
		_edes = NULL;
	}
	if (NULL != _filemob)
	{
		_filemob->Release();
		_filemob = NULL;
	}

	if (NULL != _filemob_unk)
	{
		_filemob_unk->Release();
		_filemob_unk = NULL;
	}
}

HRESULT CAAFCDCIDescriptorHelper::Initialize(IAAFSourceMob *filemob)
{
	HRESULT hr = S_OK;


	try
	{
		// Release all previous interface pointers if any exist...
		Clear();

		// Save the unknown interface pointer for equality testing.
		// NOTE: When testing objects for equality only IUnknown pointers can be
		// compared since they are on the only implemented interface pointers that
		// cannot be tearoff interfaces (implemented by another object).
		checkResult(filemob->QueryInterface(IID_IUnknown, (void **)&_filemob_unk));
		

		// Get the file mob interface, we need it in order to get the associated
		// essence descriptor.
		checkResult(_filemob_unk->QueryInterface(IID_IAAFSourceMob, (void **)&_filemob));
		checkResult(_filemob->GetEssenceDescriptor(&_edes));

		// Get the interfaces needed for the rest of the descriptor methods.
		checkResult(_edes->QueryInterface(IID_IAAFFileDescriptor, (void **)&_filedes));
		checkResult(_edes->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&_dides));
		checkResult(_edes->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&_cdcides));
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	return hr;
}


bool CAAFCDCIDescriptorHelper::operator==(const CAAFCDCIDescriptorHelper& rhs)
{
	// Assertion: Objects must have been successfully initialized.
	checkAssertion((NULL != _filemob_unk) && (NULL != rhs._filemob_unk));
	return (_filemob_unk == rhs._filemob_unk);
}


bool CAAFCDCIDescriptorHelper::operator!=(const CAAFCDCIDescriptorHelper& rhs)
{
	// Assertion: Objects must have been successfully initialized.
	checkAssertion((NULL != _filemob_unk) && (NULL != rhs._filemob_unk));
	return (_filemob_unk != rhs._filemob_unk);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetNumLocators (aafUInt32 *  pCount)
{
	checkAssertion(NULL != _edes);
	return _edes->CountLocators(pCount);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::AppendLocator (IAAFLocator * pLocator)
{
	checkAssertion(NULL != _edes);
	return _edes->AppendLocator (pLocator);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::PrependLocator (IAAFLocator * pLocator)
{
	checkAssertion(NULL != _edes);
	return _edes->PrependLocator (pLocator);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::EnumAAFAllLocators (IEnumAAFLocators ** ppEnum)
{
	checkAssertion(NULL != _edes);
	return _edes->GetLocators (ppEnum);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetLength (aafLength_t  length)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetLength(length);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetLength (aafLength_t *  pLength)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetLength(pLength);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetCodecDef (IAAFCodecDef *pCodecDef)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetCodecDef (pCodecDef);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetCodecDef (IAAFCodecDef **ppCodecDef)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetCodecDef (ppCodecDef);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetSampleRate (const aafRational_t & rate)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetSampleRate (rate);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetSampleRate (aafRational_t*  pRate)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetSampleRate (pRate);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetContainerFormat (IAAFContainerDef *pFormat)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetContainerFormat (pFormat);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetContainerFormat (IAAFContainerDef **ppFormat)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetContainerFormat (ppFormat);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetContainerFormat (const aafUID_t &FormatID)
{
	HRESULT				hr = S_OK;
	IAAFObject*			p_obj = NULL;
	IAAFDictionary*		p_dict = NULL;
	IAAFContainerDef*	p_containerdef = NULL;

	checkAssertion(NULL != _filedes);

	try
	{
	    checkResult( _filedes->QueryInterface( IID_IAAFObject, (void**)&p_obj ) );
	    checkResult( p_obj->GetDictionary( &p_dict ) );
	    checkResult( p_dict->LookupContainerDef( FormatID, &p_containerdef ) );
	    checkResult( _filedes->SetContainerFormat( p_containerdef ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_dict != NULL )
	    p_dict->Release();
	if( p_containerdef != NULL )
	    p_containerdef->Release();

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetContainerFormat (aafUID_t *pFormatID)
{
	HRESULT				hr = S_OK;
	IAAFDefObject*		p_obj = NULL;
	IAAFContainerDef*	p_containerdef = NULL;

	checkAssertion(NULL != _filedes);

	try
	{
		checkResult( _filedes->GetContainerFormat (&p_containerdef) );

	    checkResult( p_containerdef->QueryInterface( IID_IAAFDefObject, (void**)&p_obj ) );
	    checkResult( p_obj->GetAUID( pFormatID ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup
	if( p_containerdef != NULL )
	    p_containerdef->Release();
	if( p_obj != NULL )
	    p_obj->Release();

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetCompression (const aafUID_t & compression)
{
  checkAssertion(NULL != _dides);
	return _dides->SetCompression (compression);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetCompression (aafUID_t *  pCompression)
{
  checkAssertion(NULL != _dides);
	return _dides->GetCompression (pCompression);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetStoredView (aafUInt32  StoredHeight,
        aafUInt32  StoredWidth)
{
  checkAssertion(NULL != _dides);
	return _dides->SetStoredView (StoredHeight, StoredWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetStoredView (aafUInt32 *  pStoredHeight,
        aafUInt32 *  pStoredWidth)
{
  checkAssertion(NULL != _dides);
	return _dides->GetStoredView (pStoredHeight, pStoredWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetSampledView (aafUInt32  SampledHeight,
        aafUInt32  SampledWidth,
        aafInt32  SampledXOffset,
        aafInt32  SampledYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->SetSampledView (SampledHeight, SampledWidth, SampledXOffset, SampledYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetSampledView (aafUInt32 *  pSampledHeight,
        aafUInt32 *  pSampledWidth,
        aafInt32 *  pSampledXOffset,
        aafInt32 *  pSampledYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->GetSampledView (pSampledHeight, pSampledWidth, pSampledXOffset, pSampledYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetDisplayView (aafUInt32  DisplayHeight,
        aafUInt32  DisplayWidth,
        aafInt32  DisplayXOffset,
        aafInt32  DisplayYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->SetDisplayView (DisplayHeight, DisplayWidth, DisplayXOffset, DisplayYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetDisplayView (aafUInt32 *  pDisplayHeight,
        aafUInt32 *  pDisplayWidth,
        aafInt32 *  pDisplayXOffset,
        aafInt32 *  pDisplayYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->GetDisplayView (pDisplayHeight, pDisplayWidth, pDisplayXOffset, pDisplayYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFrameLayout (aafFrameLayout_t  FrameLayout)
{
  checkAssertion(NULL != _dides);
	return _dides->SetFrameLayout (FrameLayout);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetFrameLayout (aafFrameLayout_t *  pFrameLayout)
{
  checkAssertion(NULL != _dides);
	return _dides->GetFrameLayout (pFrameLayout);
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetVideoLineMap (aafUInt32  numberElements,
        aafInt32 *  pVideoLineMap)
{
  checkAssertion(NULL != _dides);
	return _dides->SetVideoLineMap (numberElements, pVideoLineMap);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetVideoLineMap (aafUInt32  numberElements,
        aafInt32 *  pVideoLineMap)
{
  checkAssertion(NULL != _dides);
	return _dides->GetVideoLineMap (numberElements, pVideoLineMap);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetVideoLineMapSize (aafUInt32 *  pNumberElements)
{
  checkAssertion(NULL != _dides);
	return _dides->GetVideoLineMapSize (pNumberElements);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetImageAspectRatio (aafRational_t  ImageAspectRatio)
{
  checkAssertion(NULL != _dides);
	return _dides->SetImageAspectRatio (ImageAspectRatio);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetImageAspectRatio (aafRational_t *  pImageAspectRatio)
{
  checkAssertion(NULL != _dides);
	return _dides->GetImageAspectRatio (pImageAspectRatio);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetAlphaTransparency (aafAlphaTransparency_t  AlphaTransparency)
{
  checkAssertion(NULL != _dides);
	return _dides->SetAlphaTransparency (AlphaTransparency);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetAlphaTransparency (aafAlphaTransparency_t *  pAlphaTransparency)
{
  checkAssertion(NULL != _dides);
	return _dides->GetAlphaTransparency (pAlphaTransparency);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetGamma (aafUID_t  Gamma)
{
  checkAssertion(NULL != _dides);
	return _dides->SetGamma (Gamma);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetGamma (aafUID_t *  pGamma)
{
  checkAssertion(NULL != _dides);
	return _dides->GetGamma (pGamma);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetImageAlignmentFactor (aafUInt32  ImageAlignmentFactor)
{
  checkAssertion(NULL != _dides);
	return _dides->SetImageAlignmentFactor (ImageAlignmentFactor);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetImageAlignmentFactor (aafUInt32 *  pImageAlignmentFactor)
{
  checkAssertion(NULL != _dides);
	return _dides->GetImageAlignmentFactor (pImageAlignmentFactor);
}





HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFieldDominance (aafFieldNumber_t   fieldDominance)
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefEnum		*p_typedef_enum = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DigitalImageDescriptor_FieldDominance, &p_propdef));

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefEnum, 
		(void**)&p_typedef_enum ) );

	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    aafCharacter    name[ 128 ];

		    checkResult( p_typedef_enum->GetNameFromInteger( 
			fieldDominance, name, 128 ) );
		    checkResult( p_typedef_enum->CreateValueFromName(
			name, &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_enum->SetIntegerValue ( 
		    p_propval, static_cast<aafInt64>( fieldDominance ) ) );
	    }


	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_enum != NULL )
	    p_typedef_enum->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetFieldDominance (aafFieldNumber_t *  pFieldDominance)
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefEnum		*p_typedef_enum = NULL;
	eAAFTypeCategory_t	type_category;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DigitalImageDescriptor_FieldDominance, 
		&p_propdef));


		hr=p_obj->GetPropertyValue( p_propdef, &p_propval);

		if(SUCCEEDED(hr)) //this is an optional property so don't deliberately throw an exception
		{
			checkResult( p_propval->GetType( &p_typedef ) );

			checkResult( p_typedef->GetTypeCategory( &type_category ) );

			// @@@Warning
			// enumTypedef::createValue has a bug in it. Instead of
			// creating enum value it creates an integer. That's why here 
			// we have to query for integer interface to get around 
			// this problem.
			if( type_category == kAAFTypeCatInt )
			{
				checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
					(void**)&p_typedef_int ) );

				checkResult( p_typedef_int->GetInteger( p_propval, 
					reinterpret_cast<aafMemPtr_t>(pFieldDominance), 
					sizeof(*pFieldDominance) ) );
			}
			else
			{
				aafInt64	int64_val = 0;

				checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefEnum, 
					(void**)&p_typedef_enum ) );

				checkResult( p_typedef_enum->GetIntegerValue( p_propval, 
					&int64_val ) );
				*pFieldDominance = static_cast<aafFieldNumber_t>( int64_val );
			}
		}
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();
	if( p_typedef_enum != NULL )
	    p_typedef_enum->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFieldStartOffset(aafUInt32  fieldStartOffset)
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DigitalImageDescriptor_FieldStartOffset, &p_propdef));

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );

	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&fieldStartOffset), 
			sizeof(fieldStartOffset), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&fieldStartOffset), 
		    sizeof(fieldStartOffset) ) );
	    }

	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetFieldStartOffset(aafUInt32*  pFieldStartOffset)
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DigitalImageDescriptor_FieldStartOffset, &p_propdef));

	    // Try to get prop value. If it doesn't exist, escape.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr == AAFRESULT_SUCCESS )
	    {
			checkResult( p_propval->GetType( &p_typedef ) );

			checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
			(void**)&p_typedef_int ) );

			checkResult( p_typedef_int->GetInteger( p_propval, 
			reinterpret_cast<aafMemPtr_t>(pFieldStartOffset), 
			sizeof(*pFieldStartOffset) ) );
		}
		else if( hr != AAFRESULT_PROP_NOT_PRESENT ) throw hr;
	    // else return AAFRESULT_PROP_NOT_PRESENT

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFieldEndOffset( aafUInt32  fieldEndOffset )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DigitalImageDescriptor_FieldEndOffset, &p_propdef));

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );

	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&fieldEndOffset), 
			sizeof(fieldEndOffset), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&fieldEndOffset), 
		    sizeof(fieldEndOffset) ) );
	    }

	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetFieldEndOffset( aafUInt32*  pFieldEndOffset )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DigitalImageDescriptor_FieldEndOffset, &p_propdef));

	    // Try to get prop value. If it doesn't exist, escape.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr == AAFRESULT_SUCCESS )
	    {
			checkResult( p_propval->GetType( &p_typedef ) );

			checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
			(void**)&p_typedef_int ) );

			checkResult( p_typedef_int->GetInteger( p_propval, 
			reinterpret_cast<aafMemPtr_t>(pFieldEndOffset), 
			sizeof(*pFieldEndOffset) ) );
		}
		else if( hr != AAFRESULT_PROP_NOT_PRESENT ) throw hr;
	    // else return AAFRESULT_PROP_NOT_PRESENT

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetComponentWidth (aafUInt32  ComponentWidth)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetComponentWidth (ComponentWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetComponentWidth (aafUInt32 *  pComponentWidth)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetComponentWidth (
	    reinterpret_cast<aafInt32*>(pComponentWidth));
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetHorizontalSubsampling (aafUInt32  HorizontalSubsampling)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetHorizontalSubsampling (HorizontalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetHorizontalSubsampling (aafUInt32 *  pHorizontalSubsampling)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetHorizontalSubsampling (pHorizontalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetVerticalSubsampling (aafUInt32  VerticalSubsampling)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetVerticalSubsampling (VerticalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetVerticalSubsampling (aafUInt32 *  pVerticalSubsampling)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetVerticalSubsampling (pVerticalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetColorSiting (aafColorSiting_t  ColorSiting)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetColorSiting (ColorSiting);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetColorSiting (aafColorSiting_t *  pColorSiting)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetColorSiting (pColorSiting);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetBlackReferenceLevel (aafUInt32  BlackReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetBlackReferenceLevel (BlackReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetBlackReferenceLevel (aafUInt32 *  pBlackReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetBlackReferenceLevel (pBlackReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetWhiteReferenceLevel (aafUInt32  WhiteReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetWhiteReferenceLevel (WhiteReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetWhiteReferenceLevel (aafUInt32 *  pWhiteReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetWhiteReferenceLevel (pWhiteReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetColorRange (aafUInt32  ColorRange)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetColorRange (ColorRange);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetColorRange (aafUInt32 *  pColorRange)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetColorRange (pColorRange);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetPaddingBits (aafInt16  PaddingBits)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetPaddingBits (PaddingBits);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::GetPaddingBits (aafInt16 *  pPaddingBits)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetPaddingBits (pPaddingBits);
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetResolutionID( aafInt32  resolutionID )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    // Get DigitalImageDescriptor class definition
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );


	    // Get ResolutionID property definition
	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDResolutionID, &p_propdef ) );

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );


	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&resolutionID), 
			sizeof(resolutionID), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&resolutionID), 
		    sizeof(resolutionID) ) );
	    }

	    // Set modified property value.
	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}


HRESULT STDMETHODCALLTYPE
CAAFCDCIDescriptorHelper::GetFrameSampleSize (aafUInt32 *  pframeSampleSize)
{
	IAAFClassDef		*pClassDef = NULL;
	IAAFObject			*pObj = NULL;
	IAAFPropertyDef		*pPropertyDef = NULL;
	IAAFPropertyValue	*pPropValue = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	IAAFTypeDefInt		*pTypeDefInt = NULL;
	aafInt32			val;

	HRESULT				hr = S_OK;

	checkAssertion(NULL != _dides);
	checkAssertion(NULL != pframeSampleSize);
	try
	{
		checkResult(_dides->QueryInterface(IID_IAAFObject, (void **)&pObj));
		checkResult(pObj->GetDefinition (&pClassDef));
		checkResult(pClassDef->LookupPropertyDef(kAAFPropID_DIDFrameSampleSize, &pPropertyDef));
	    // Try to get prop value. If it doesn't exist, escape.
	    hr = pObj->GetPropertyValue( pPropertyDef, &pPropValue );
	    if( hr == AAFRESULT_SUCCESS )
	    {
			checkResult(pPropValue->GetType(&pTypeDef));
			checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
			checkResult(pTypeDefInt->GetInteger(pPropValue, (aafMemPtr_t) &val, sizeof (val)));
			*pframeSampleSize  = val;
		}
		else if( hr != AAFRESULT_PROP_NOT_PRESENT ) throw hr;
	    // else return AAFRESULT_PROP_NOT_PRESENT

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	if( pClassDef != NULL )
		pClassDef->Release();
	if( pObj != NULL )
		pObj->Release();
	if( pPropertyDef != NULL )
		pPropertyDef->Release();
	if( pPropValue != NULL )
		pPropValue->Release();
	if( pTypeDef != NULL )
		pTypeDef->Release();
	if( pTypeDefInt != NULL )
		pTypeDefInt->Release();

	return hr;
}




HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFrameSampleSize( aafInt32 frameSampleSize )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDFrameSampleSize, &p_propdef));

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );

	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&frameSampleSize), 
			sizeof(frameSampleSize), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, 
		    reinterpret_cast<aafMemPtr_t>(&frameSampleSize),
		    sizeof(frameSampleSize) ) );
	    }

	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetOffsetToFrameIndexes( aafInt32  offsetToFrameIndexes )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _cdcides);

	try
	{
	    // Get DigitalImageDescriptor class definition
	    checkResult( _cdcides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );


	    // Get ResolutionID property definition
	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_CDCIOffsetToFrameIndexes, &p_propdef ) );

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );


	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&offsetToFrameIndexes), 
			sizeof(offsetToFrameIndexes), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&offsetToFrameIndexes), 
		    sizeof(offsetToFrameIndexes) ) );
	    }

	    // Set modified property value.
	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFrameIndexByteOrder( aafInt16 frameIndexByteOrder )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    // Get DigitalImageDescriptor class definition
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );


	    // Get ResolutionID property definition
	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDFrameIndexByteOrder, &p_propdef ) );

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );


	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&frameIndexByteOrder), 
			sizeof(frameIndexByteOrder), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&frameIndexByteOrder), 
		    sizeof(frameIndexByteOrder) ) );
	    }

	    // Set modified property value.
	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetFirstFrameOffset( aafInt32 firstFrameOffset )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    // Get DigitalImageDescriptor class definition
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );


	    // Get ResolutionID property definition
	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDFirstFrameOffset, &p_propdef ) );

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );


	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&firstFrameOffset), 
			sizeof(firstFrameOffset), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&firstFrameOffset), 
		    sizeof(firstFrameOffset) ) );
	    }

	    // Set modified property value.
	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCIDescriptorHelper::SetImageSize( aafInt32 imageSize )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    // Get DigitalImageDescriptor class definition
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );


	    // Get ResolutionID property definition
	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDImageSize, &p_propdef ) );

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );


	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&imageSize), 
			sizeof(imageSize), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&imageSize), 
		    sizeof(imageSize) ) );
	    }

	    // Set modified property value.
	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}
