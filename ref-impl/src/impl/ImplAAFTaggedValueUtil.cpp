//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTaggedValueUtil.cpp,v 1.6 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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
//
//=---------------------------------------------------------------------=

#include "ImplAAFTaggedValueUtil.h"

#include "ImplAAFTaggedValue.h"
#include "ImplEnumAAFTaggedValues.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFRoot.h"
#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFSmartPointer.h"
#include "aafErr.h"

#include "AAFResult.h"

#include <wchar.h>
#include "OMAssertions.h"

extern "C" const aafClassID_t CLSID_EnumAAFTaggedValues;


AAFRESULT ImplAAFTaggedValueUtil::AppendNameValuePair(
  ImplAAFObject* pContainingObject,  
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  aafCharacter_constptr  pName,
  aafCharacter_constptr  pValue )
{
  if ( pName  == NULL ||
       pValue == NULL ) {
    return AAFRESULT_NULL_PARAM;
  }


  ImplAAFTaggedValue* pTaggedVal = NULL;

  XPROTECT()
  {
    ImplAAFSmartPointer<ImplAAFDictionary> spDict;
    CHECK( pContainingObject->GetDictionary(&spDict) );

    // Get a type def for the tagged value.  GetBuiltinDefs passes an
    // unowned pointer.  Don't release it (hence, a bare pointer).
    ImplAAFTypeDef* pTaggedValType = spDict->GetBuiltinDefs()->tdString();
    ASSERTU( pTaggedValType );


    // The TaggedValueDef returned by GetBuildinDefs is not reference
    // counted - don't release it.
    // pTaggedDef is the object we will return, so it is not put in a smart pointer
    // either.
    ImplAAFClassDef* pTaggedValDef = spDict->GetBuiltinDefs()->cdTaggedValue();
    if ( !pTaggedValDef ) {
      RAISE( E_FAIL );
    }


    // Create the tagged value.
    CHECK( pTaggedValDef->CreateInstance( reinterpret_cast<ImplAAFObject**>(&pTaggedVal) ) );
    ASSERTU( pTaggedVal );

    // Init the tagged value.
    size_t cc = wcslen(pValue)+1;
    ASSERTU(cc <= OMUINT32_MAX);
    OMUInt32 characterCount = static_cast<OMUInt32>(cc);
    OMUInt32 byteCount = characterCount * sizeof(aafCharacter);
    CHECK( pTaggedVal->Initialize( pName,
				   pTaggedValType,
				   byteCount,
				   reinterpret_cast<aafDataBuffer_t>(const_cast<aafCharacter*>(pValue)) ) );

    taggedValVector.appendValue( pTaggedVal );

  }
  XEXCEPT
  {
    if (pTaggedVal)
    {
      pTaggedVal->ReleaseReference();
      pTaggedVal = NULL;
    }
  }
  XEND;


  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFTaggedValueUtil::CountEntries(
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  aafUInt32* pNumEntries )
{
  if ( NULL == pNumEntries ) {
    return AAFRESULT_NULL_PARAM;
  }

  // The convention is to return zero if the property is not present.
  if ( !taggedValVector.isPresent() ) {
    *pNumEntries = 0;
  }
  else {
    *pNumEntries = taggedValVector.count();
  }

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFTaggedValueUtil::GetEnumerator(
  ImplAAFObject* pContainingObject,  
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  ImplEnumAAFTaggedValues ** ppEnum )
{
  if ( NULL == ppEnum ) {
    return AAFRESULT_NULL_PARAM;
  }

  ImplEnumAAFTaggedValues* pEnum = 
    reinterpret_cast<ImplEnumAAFTaggedValues*>( CreateImpl(CLSID_EnumAAFTaggedValues) );
  if ( NULL == pEnum ) {
    return E_FAIL;
  }

  XPROTECT()
  {
    OMStrongReferenceVectorIterator<ImplAAFTaggedValue>* iter =
      new OMStrongReferenceVectorIterator<ImplAAFTaggedValue>(taggedValVector);
    if ( NULL == iter ) {
      RAISE( AAFRESULT_NOMEMORY );
    }

    CHECK(pEnum->Initialize( &CLSID_EnumAAFTaggedValues, pContainingObject, iter ) );
    *ppEnum = pEnum;

  }
  XEXCEPT
  {
    if ( pEnum ) {
      pEnum->ReleaseReference();
    }
  }
  XEND;

  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFTaggedValueUtil::RemoveEntry(
  OMStrongReferenceVectorProperty<ImplAAFTaggedValue>& taggedValVector,
  ImplAAFTaggedValue* pEntry )
{
  if (NULL == pEntry) {
    return AAFRESULT_NULL_PARAM;
  }

  if ( !pEntry->attached () ) {
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  }

  if ( !taggedValVector.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  OMUInt32 index;
  if ( taggedValVector.findIndex (pEntry, index) ) {
    taggedValVector.removeAt(index);
    pEntry->ReleaseReference();
  }
  else {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

  return AAFRESULT_SUCCESS;
}


