//=---------------------------------------------------------------------=
//
// $Id: ImplEnumAAFPropertyValues.cpp,v 1.11 2009/06/01 11:47:10 stuart_hc Exp $ $Name: V116 $
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



#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#include "ImplAAFTypeDefFixedArray.h"
#include "ImplAAFTypeDefVariableArray.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFPropertyValues;

#include "ImplAAFSmartPointer.h"
#include "ImplAAFPropValData.h"

extern "C" const aafClassID_t CLSID_AAFPropValData;

AAFRESULT GetElementValueFromSet( ImplAAFPropertyValue* pVal,
				  aafUInt32 current,
				  ImplAAFTypeDefSet* pDefSet,
				  ImplAAFPropertyValue** ppItemVal )
{
  ASSERTU( pVal );
  ASSERTU( pDefSet );
  ASSERTU( ppItemVal );

  AAFRESULT hr;

  // Get the type def of the set's contained elements;
  ImplAAFSmartPointer<ImplAAFTypeDef> pElemTypeDef;
  hr = pDefSet->GetElementType( &pElemTypeDef );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }


  // Get the element's size, and compute offset of "current" element
  // in the propVal's buffer.
  ASSERTU( pElemTypeDef->IsFixedSize() );
  aafUInt32 elemSize = pElemTypeDef->PropValSize();
  aafUInt32 byteOffset = current * elemSize;

  // Create a PropValData and initialize it such that it points into pVal.
  ImplAAFPropValData* pElemValData = (ImplAAFPropValData*)CreateImpl(CLSID_AAFPropValData);
  if ( !pElemValData ) {
    return AAFRESULT_NOMEMORY;
  }

  hr = pElemValData->Initialize( pElemTypeDef );
  if ( AAFRESULT_SUCCESS != hr ) {
    pElemValData->ReleaseReference();
    return hr;
  }

  aafMemPtr_t unused;
  hr = pElemValData->AllocateFromPropVal(  dynamic_cast<ImplAAFPropValData*>( pVal ),
					   byteOffset, elemSize, &unused );
  if ( AAFRESULT_SUCCESS != hr ) {
    pElemValData->ReleaseReference();
    return hr;
  };

  *ppItemVal = pElemValData;
  (*ppItemVal)->AcquireReference();

  pElemValData->ReleaseReference();

  return AAFRESULT_SUCCESS;
}

ImplEnumAAFPropertyValues::ImplEnumAAFPropertyValues ()
  : _initialized(false),
    _pDefSet(0),
    _pDef(0),
    _pVal(0),
    _count(0),
    _current(0)
{}

ImplEnumAAFPropertyValues::~ImplEnumAAFPropertyValues ()
{
  if ( _pDefSet ) {
    _pDefSet->ReleaseReference();
    _pDefSet = 0;
  }

  if ( _pDef ) {
    _pDef->ReleaseReference();
    _pDef = 0;
  }

  if ( _pVal ) {
    _pVal->ReleaseReference();
    _pVal = 0;
  }
}

AAFRESULT ImplEnumAAFPropertyValues::Initialize( ImplAAFTypeDefArray* pDef,
						 ImplAAFPropertyValue* pVal )
{
  ASSERTU( !_initialized );

  if ( !pDef ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !pVal ) {
    return AAFRESULT_NULL_PARAM;
  }

  // Must resolve the array type in order to get the array size.
  ImplAAFTypeDefFixedArray *pFixedArray = dynamic_cast<ImplAAFTypeDefFixedArray*>(pDef);
  ImplAAFTypeDefVariableArray *pVariableArray = dynamic_cast<ImplAAFTypeDefVariableArray*>(pDef);

  // Intentional assignment in predicate (both cases below).
  AAFRESULT hr = AAFRESULT_SUCCESS;
  if ( pFixedArray  ) {
    hr = pFixedArray->GetCount( &_count );
  }
  else if ( pVariableArray  ) {
    hr = pVariableArray->GetCount( pVal, &_count );
  }
  else {
    hr = AAFRESULT_BAD_TYPE;
  }

  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  _current = 0;

  _pDef = pDef;
  _pDef->AcquireReference();

  _pVal = pVal;
  _pVal->AcquireReference();

  _initialized = true;

  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplEnumAAFPropertyValues::Initialize( ImplAAFTypeDefSet* pDefSet,
						 ImplAAFPropertyValue* pVal )
{
  ASSERTU( !_initialized );

  if ( !pDefSet ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !pVal ) {
    return AAFRESULT_NULL_PARAM;
  }

  AAFRESULT hr = pDefSet->GetCount( pVal, &_count );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  _current = 0;

  _pDefSet = pDefSet;
  _pDefSet->AcquireReference();

  _pVal = pVal;
  _pVal->AcquireReference();

  _initialized = true;

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::NextOne (
      ImplAAFPropertyValue ** ppPropertyValue )
{
  if ( !_initialized ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  if ( !ppPropertyValue ) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( _count == _current ) {
    return AAFRESULT_NO_MORE_OBJECTS;
  }

  // _current should never exceed _count if the implementation is correct
  ASSERTU( _current < _count );

  HRESULT hr;
  if ( _pDefSet ) {
    hr = GetElementValueFromSet( _pVal, _current, _pDefSet, ppPropertyValue ); 
  }
  else {
    ASSERTU( _pDef );
    hr = _pDef->GetElementValue( _pVal, _current, ppPropertyValue );
  }

  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  _current++;
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Next (
      aafUInt32  count,
      ImplAAFPropertyValue ** ppItems,
      aafUInt32 *  pFetched )
{
  if ( !_initialized ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  // Credits: This was nabbed from ImplAAFEnumerator.

  aafUInt32			numItems;
  AAFRESULT			ar=AAFRESULT_SUCCESS;

  if(ppItems==NULL||pFetched==NULL)
    return(AAFRESULT_NULL_PARAM);
	
  if(count==0)
    return(AAFRESULT_INVALID_PARAM);

  for (numItems = 0; numItems < count; numItems++)
    {
      ar = NextOne(&ppItems[numItems]);
      if (FAILED(ar))
	break;
    }
  
  if (pFetched)
    *pFetched=numItems;
  
  return(ar);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Skip (
      aafUInt32  count )
{
  if ( !_initialized ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  _current += count;

  if ( _current > _count ) {
    _current = _count;
    return AAFRESULT_NO_MORE_OBJECTS;
  }

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Reset ()
{
  if ( !_initialized ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  _current = 0;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPropertyValues::Clone (
      ImplEnumAAFPropertyValues ** ppEnum)
{
  if ( !_initialized ) {
    return AAFRESULT_NOT_INITIALIZED;
  }

  ImplEnumAAFPropertyValues* pEnum = 
    static_cast<ImplEnumAAFPropertyValues*>( ::CreateImpl(CLSID_EnumAAFPropertyValues) );

  AAFRESULT hr;
  if ( _pDefSet ) {
    hr = pEnum->Initialize( _pDefSet, _pVal );
  }
  else {
    ASSERTU( _pDef );
    hr = pEnum->Initialize( _pDef, _pVal );
  }

  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  // Initialize() sets _current to zero.  It must match the position of
  // this iterator.
  pEnum->_current = _current;

  pEnum->AcquireReference();
  *ppEnum = pEnum;

  return AAFRESULT_SUCCESS;
}



