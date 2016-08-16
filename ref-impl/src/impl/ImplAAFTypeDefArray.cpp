//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefArray.cpp,v 1.26 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFRefArrayValue_h__
#include "ImplAAFRefArrayValue.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFStrongRefArrayValue_h__
#include "ImplAAFStrongRefArrayValue.h"
#endif

#ifndef __ImplAAFWeakRefArrayValue_h__
#include "ImplAAFWeakRefArrayValue.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;
extern "C" const aafClassID_t CLSID_AAFPropValObjVectElem;
extern "C" const aafClassID_t CLSID_AAFStrongRefArrayValue;
extern "C" const aafClassID_t CLSID_AAFWeakRefArrayValue;

ImplAAFTypeDefArray::ImplAAFTypeDefArray ()
{}


ImplAAFTypeDefArray::~ImplAAFTypeDefArray ()
{}



// PATCH DR4_CPR: transdel. Create the appropriate ImplAAFRefArray without an associated
// property. The property value's writeTo method needs to copy (or replace) the contents
// of the current property. This a hybrid of the old-pseudo "direct access" and the new
// more direct property access.

// Method used internally by "fixed arrays" but called directly by variable arrays.

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::CreateEmptyValue
(ImplAAFPropertyValue ** ppPropVal)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == ppPropVal)
    return AAFRESULT_NULL_PARAM;
      
  ImplAAFTypeDefSP pElementType;
  result = GetType(&pElementType);
  if (AAFRESULT_FAILED(result))
    return result;
      
      
  if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) pElementType))
  {
    // element is strong ref
    ImplAAFStrongRefArrayValue* pStrongRefArray = NULL;
    pStrongRefArray = (ImplAAFStrongRefArrayValue*) CreateImpl (CLSID_AAFStrongRefArrayValue);
    if (!pStrongRefArray) 
      return AAFRESULT_NOMEMORY;
    result = pStrongRefArray->Initialize(this, kAAFTrue == IsFixedSize());
    if (AAFRESULT_SUCCEEDED(result))
    {
      *ppPropVal = pStrongRefArray;
    }
    else
    {
      pStrongRefArray->ReleaseReference();
    }
  }
  else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) pElementType))
  {
    // element is weak ref
    ImplAAFWeakRefArrayValue* pWeakRefArray = NULL;
    pWeakRefArray = (ImplAAFWeakRefArrayValue*) CreateImpl (CLSID_AAFWeakRefArrayValue);
    if (!pWeakRefArray) 
      return AAFRESULT_NOMEMORY;
    result = pWeakRefArray->Initialize(this, kAAFTrue == IsFixedSize());
    if (AAFRESULT_SUCCEEDED(result))
    {
      *ppPropVal = pWeakRefArray;
    }
    else
    {
      pWeakRefArray->ReleaseReference();
    }
  }
  else
  { 
  	//simply defer to base impl (size is 0)
  	result = CreateValue(ppPropVal);
  }
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::ValidateInputParams (
												  ImplAAFPropertyValue ** ppElementValues,
												  aafUInt32  numElements)
												  
{
	//first validate params + basic stuff ...
	if (!ppElementValues)
		return AAFRESULT_NULL_PARAM;
	
	//verify that all the individual elem types are the same as each other,
	
	HRESULT hr;
	
	//get Base TD
	ImplAAFTypeDefSP spTargetTD;
	hr = GetType(&spTargetTD); //gets base elem type
	if (AAFRESULT_FAILED (hr)) return hr;
	if (! spTargetTD->IsRegistered ())
		return AAFRESULT_NOT_REGISTERED;
	
	//Get size
	aafUInt32 targetElemSize = spTargetTD->NativeSize();
	
	for (aafUInt32 i=0; i<numElements; i++)
	{
		//get  source type
		ImplAAFTypeDefSP  spSourceTD;
		hr = ppElementValues[i]->GetType (&spSourceTD);
		if (AAFRESULT_FAILED (hr)) 
			return hr;
		
		//verify that spTargetTD == spSourceTD
		if (spSourceTD != spTargetTD )
			return AAFRESULT_BAD_TYPE;
		
		//verify FIXED/VARIABLE Arrayable
		if (! IsArrayable(spSourceTD) )
			return AAFRESULT_BAD_TYPE;
		
		//verify that the target elem size is equal to that of source 
		aafUInt32 sourceSize = spSourceTD->NativeSize();	
		if (sourceSize != targetElemSize )
			return AAFRESULT_BAD_SIZE;
		
	}//for each elem

	return AAFRESULT_SUCCESS;

}//ValidateInputParams()


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::CreateValue(ImplAAFPropertyValue ** ppPropVal, 
								 aafUInt32 dataSize)
{
	if (! ppPropVal)
		return AAFRESULT_NULL_PARAM;
	
	ImplAAFPropValDataSP pvd;
	ImplAAFPropValData * tmp;
	tmp = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
	if (!tmp) return AAFRESULT_NOMEMORY;
	pvd = tmp;
	// the pvd smart pointer will maintain a reference for us...
	tmp->ReleaseReference ();
	
	//Initialize pvd to this type
	HRESULT hr = pvd->Initialize(this);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	//deal with dataSize - Allocate bits if this param is non-zero
	if (dataSize)
	{
		//Allocate the necesary bits
		aafMemPtr_t pTargetData = 0;
		hr = pvd->AllocateBits(dataSize, &pTargetData);
		if (AAFRESULT_FAILED (hr))
			return hr;
	}//if dataSize
	else
		//we're effectively creating an "Empty" value
		;
	
	*ppPropVal = pvd;
	ASSERTU (*ppPropVal);
	(*ppPropVal)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}//CreateValue()


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::CopyValuesIntoValue (
										  ImplAAFPropertyValue ** ppElementValues,
										  aafUInt32  numElements,
										  aafUInt32  sizeElem,
										  ImplAAFPropertyValue ** ppPropVal) 
{
	AAFRESULT hr;
	
	//first validate params 
	if (!ppElementValues || !ppPropVal)
		return AAFRESULT_NULL_PARAM;
	
	if (!*ppPropVal)
		return AAFRESULT_NOT_INITIALIZED;
	
	// Get the property value's embedded type and 
	// check if it's the same as the base type.
	ImplAAFTypeDefSP	pIncomingType;
	if( AAFRESULT_FAILED( (*ppPropVal)->GetType( &pIncomingType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pIncomingType);
	if( (ImplAAFTypeDef *)pIncomingType != this )
		return AAFRESULT_BAD_TYPE;

	// proceed ....
	
	//get pTargetData
	ImplAAFPropValData * pvd_Target;
	pvd_Target = dynamic_cast<ImplAAFPropValData*> (*ppPropVal);
	if (!pvd_Target) 
		return AAFRESULT_BAD_TYPE;
	
	//get Bits from Target Data
	aafMemPtr_t pTargetData = 0;
	hr = pvd_Target->GetBits(&pTargetData);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	//Copy each source elements' bits over ...
	
	for (aafUInt32 i=0; i<numElements; i++)
	{
		//get  Source Data
		ImplAAFPropValData * pvd_Source = dynamic_cast<ImplAAFPropValData*> (ppElementValues[i]);
		if (!pvd_Source) return AAFRESULT_BAD_TYPE;
		
		aafUInt32 source_bitsSize;
		hr = pvd_Source->GetBitsSize (&source_bitsSize);
		if (AAFRESULT_FAILED (hr))
			return hr;
		//make sure the bits-size is same as the reference
		if (source_bitsSize != sizeElem) return AAFRESULT_BAD_SIZE; 
		
		aafMemPtr_t pSourceData = 0;
		hr = pvd_Source->GetBits (&pSourceData);
		if (AAFRESULT_FAILED (hr))
			return hr;
		
		//copy the bits
		memcpy(pTargetData, pSourceData, source_bitsSize);
		//once done, incr the target pointer by the amt. of bits copied
		pTargetData += source_bitsSize;
		
	}//for each element
	
	*ppPropVal = pvd_Target;
	return AAFRESULT_SUCCESS;
	
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::CreateValueFromValues (
													ImplAAFPropertyValue ** ppElementValues,
													aafUInt32  numElements,
													ImplAAFPropertyValue ** ppPropVal)
{
	AAFRESULT hr = AAFRESULT_SUCCESS;
	
	//first validate params ...
	if (!ppPropVal)
		return AAFRESULT_NULL_PARAM;
	
	hr = ValidateInputParams(ppElementValues, numElements);
	if (AAFRESULT_FAILED (hr))
	  return hr;

	// All params validated; proceed ....

  // PATCH DR4_CPR: transdel. Create the appropriate ImplAAFRefArray without an associated
  // property. The property value's writeTo method needs to copy (or replace) the contents
  // of the current property. This a hybrid of the old-pseudo "direct access" and the new
  // more direct property access.

  ImplAAFTypeDefSP pElementType;
  hr = GetType(&pElementType);
  if (AAFRESULT_FAILED(hr))
    return hr;

  if (dynamic_cast<ImplAAFTypeDefObjectRef*>((ImplAAFTypeDef*) pElementType))
  {
    ImplAAFPropertyValue *pPropertyValue = NULL;
    hr = CreateEmptyValue(&pPropertyValue);
    if (AAFRESULT_SUCCEEDED(hr))
    {
      ImplAAFRefArrayValue *pRefArray = dynamic_cast<ImplAAFRefArrayValue *>(pPropertyValue);
      ASSERTU(NULL != pRefArray);
      if (NULL == pRefArray)
        hr = AAFRESULT_INVALID_OBJ;
      
      aafUInt32 index;
      for (index = 0; (index < numElements) && AAFRESULT_SUCCEEDED(hr); ++index)
      {
        hr = pRefArray->AppendElement(ppElementValues[index]);
      }
      
      if (AAFRESULT_SUCCEEDED(hr))
        *ppPropVal = pPropertyValue; // refcount already incremented
      else
        pPropertyValue->ReleaseReference();
        
      pPropertyValue = NULL;
    }
  }
  else
  {
  	//2nd step - Create the value;  defer to base impl

  	//first, calculate the total size needed for allocation, based on type
  	//get Base TD
  	ImplAAFTypeDefSP spTargetTD;
  	hr = GetType(&spTargetTD); //gets base elem type
  	if (AAFRESULT_FAILED (hr)) return hr;
  	//Get Elem size
  	aafUInt32 targetElemSize = spTargetTD->NativeSize();
  	//Total size ...
  	aafUInt32 totalSize = targetElemSize * numElements; //size based on "n" Elements
  	
  	//create the value
  	hr = ImplAAFTypeDefArray::CreateValue(ppPropVal, totalSize);
  	if (AAFRESULT_FAILED(hr)) return hr;
  	
  	
  	//... Copy the values ; defer to base impl
  	hr = ImplAAFTypeDefArray::CopyValuesIntoValue(ppElementValues,numElements,
  												targetElemSize, ppPropVal);
	}
	
	return hr;
}




AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::CreateValueFromCArray (
											aafMemPtr_t pInitData,
											aafUInt32 initDataSize,
											ImplAAFPropertyValue ** ppPropVal)
{
	if (! pInitData)
		return AAFRESULT_NULL_PARAM;
	
	if (! ppPropVal)
		return AAFRESULT_NULL_PARAM;
	
  ImplAAFTypeDefSP pElementType;
  AAFRESULT result = GetType(&pElementType);
  if (AAFRESULT_FAILED(result))
    return result;     
      
  if (dynamic_cast<ImplAAFTypeDefObjectRef*>((ImplAAFTypeDef*) pElementType))
  {
    // This interface is not type-safe for accessing objects! There is also no
    // mechanism in place to convert between a buffer pointer and an array
    // of interface pointers; this convertion would not be necessary for
    // arrays of non-objects.
    return AAFRESULT_BAD_TYPE;
  }

	ImplAAFPropValDataSP pvd;
	ImplAAFPropValData * tmp;
	tmp = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
	if (!tmp) return AAFRESULT_NOMEMORY;
	pvd = tmp;
	// the pvd smart pointer will maintain a reference for us...
	aafUInt32 refCount;
	refCount = tmp->ReleaseReference ();
	// ...make sure it really does
	ASSERTU (1 == refCount);
	
	AAFRESULT hr;
	hr = pvd->Initialize(this);
	if (! AAFRESULT_SUCCEEDED (hr))
		return hr;
	
	hr = SetCArray (pvd, pInitData, initDataSize);
	if (AAFRESULT_FAILED (hr))
		return hr;
	
	ASSERTU (ppPropVal);
	*ppPropVal = pvd;
	ASSERTU (*ppPropVal);
	(*ppPropVal)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::GetElementValue (
									  ImplAAFPropertyValue * pInPropVal,
									  aafUInt32  index,
									  ImplAAFPropertyValue ** ppOutPropVal)
{
	if (! pInPropVal) return AAFRESULT_NULL_PARAM;
	if (! ppOutPropVal) return AAFRESULT_NULL_PARAM;
	
	// Get the property value's embedded type and 
	// check if it's the same as the base type.
	ImplAAFTypeDefSP	pIncomingType;
	if( AAFRESULT_FAILED( pInPropVal->GetType( &pIncomingType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pIncomingType);
	if( (ImplAAFTypeDef *)pIncomingType != this )
		return AAFRESULT_BAD_TYPE;

  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pInPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->GetElementAt(index, ppOutPropVal);
  }

	if (index >= pvtCount (pInPropVal))
		return AAFRESULT_BADINDEX;
	
	aafUInt32 inBitsSize;
	ImplAAFPropValDataSP pOutPVData;
	ImplAAFPropValDataSP pvd;
	ImplAAFTypeDefSP ptd;
	
	AAFRESULT hr;
	hr = GetType (&ptd);
	if (AAFRESULT_FAILED (hr)) return hr;

  // aafUInt32 elementSize = ptd->PropValSize();
	aafUInt32 elementSize = ptd->ActualSize(); // okay for data not to be registered?
	
	ASSERTU (pInPropVal);
	pvd = dynamic_cast<ImplAAFPropValData*> (pInPropVal);
	
	hr = pvd->GetBitsSize (&inBitsSize);
	if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	ASSERTU ((index+1) * elementSize <= inBitsSize);
	
	pOutPVData = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
	if (! pOutPVData) return AAFRESULT_NOMEMORY;
	// Bobt: Hack bugfix! SmartPointer operator= will automatically
	// AddRef; CreateImpl *also* will addref, so we've got one too
	// many.  Put us back to normal.
	pOutPVData->ReleaseReference ();
	
	hr = pOutPVData->Initialize (ptd);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	hr = pOutPVData->AllocateFromPropVal (pvd,
		index * elementSize,
		elementSize,
		NULL);
	if (AAFRESULT_FAILED(hr)) return hr;
	
	ASSERTU (ppOutPropVal);
	*ppOutPropVal = pOutPVData;
	ASSERTU (*ppOutPropVal);
	(*ppOutPropVal)->AcquireReference ();
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::GetCArray (
								ImplAAFPropertyValue * pPropVal,
								aafMemPtr_t pData,
								aafUInt32 dataSize)
{
	if (! pPropVal)
		return AAFRESULT_NULL_PARAM;
	
	if (! pData)
		return AAFRESULT_NULL_PARAM;
	
	if (! IsRegistered ())
		return AAFRESULT_NOT_REGISTERED;
	
	// Get the property value's embedded type and 
	// check if it's the same as the base type.
	ImplAAFTypeDefSP	pIncomingType;
	if( AAFRESULT_FAILED( pPropVal->GetType( &pIncomingType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pIncomingType);
	if( (ImplAAFTypeDef *)pIncomingType != this )
		return AAFRESULT_BAD_TYPE;

	ImplAAFTypeDefSP pBaseType;
	HRESULT hr = GetType (&pBaseType);
	
	ASSERTU (pBaseType->IsFixedSize ());
	pBaseType->AttemptBuiltinRegistration ();
	ASSERTU (pBaseType->IsRegistered ());

	
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pPropVal);
  if (NULL != pRefArray)
  {
    // This interface is not type-safe for accessing objects! There is also no
    // mechanism in place to convert between a buffer pointer and an array
    // of interface pointers; this convertion would not be necessary for
    // arrays of non-objects.
    return AAFRESULT_BAD_TYPE;
  }


	aafUInt32 elemSize = pBaseType->NativeSize ();
	aafUInt32 elemCount = pvtCount (pPropVal);
	aafUInt32 propSize = elemSize * elemCount;
	
	if (dataSize < propSize)
		return AAFRESULT_BAD_SIZE;
	
	ImplAAFPropValData * pvd = 0;
	ASSERTU (pPropVal);
	pvd = dynamic_cast<ImplAAFPropValData*> (pPropVal);
	ASSERTU (pvd);
	
	aafUInt32 bitsSize;
	hr = pvd->GetBitsSize (&bitsSize);
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU (bitsSize >= propSize);
	
	aafMemPtr_t pBits = 0;
	hr = pvd->GetBits (&pBits);
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU (pBits);
	
	memcpy (pData, pBits, propSize);
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::SetElementValue (
									  ImplAAFPropertyValue * pPropVal,
									  aafUInt32  index,
									  ImplAAFPropertyValue * pMemberPropVal)
{
	
	//Ensure our input pointers are valid
	if (!pPropVal || !pMemberPropVal)
		return AAFRESULT_NULL_PARAM;

	// Get the property value's embedded type and 
	// check if it's the same as the base type.
	ImplAAFTypeDefSP	pIncomingType;
	if( AAFRESULT_FAILED( pPropVal->GetType( &pIncomingType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pIncomingType);
	if( (ImplAAFTypeDef *)pIncomingType != this )
		return AAFRESULT_BAD_TYPE;

  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pPropVal);
  if (NULL != pRefArray)
  {
    return pRefArray->SetElementAt(pMemberPropVal, index);
  }
	
	//Ensure index is within range
	if (index >= pvtCount(pPropVal))  //if index is > 0..n-1
		return AAFRESULT_BADINDEX; //AAFRESULT_BAD_PARAM;
	
	//all parameters validated;  proceed ....
	
	AAFRESULT hr;
	
	//get  source size
	ImplAAFTypeDefSP  spSourceTD;
	hr = pMemberPropVal->GetType (&spSourceTD);
	if (AAFRESULT_FAILED (hr)) 
		return hr;

 	if (! spSourceTD->IsRegistered ())
		return AAFRESULT_NOT_REGISTERED;

	aafUInt32 sourceSize = spSourceTD->NativeSize();
	
	//get Base TD and size
	ImplAAFTypeDefSP spTargetTD;
	hr = GetType (&spTargetTD); //gets base elem type
	if (AAFRESULT_FAILED (hr)) 
		return hr;
	aafUInt32 targetElemSize = spTargetTD->NativeSize();
	
	//verify that spTargetTD == spSourceTD
	if (spTargetTD != spSourceTD)
		return AAFRESULT_BAD_TYPE;
	
	//verify that the target elem size is equal to that of source 
	if (targetElemSize != sourceSize)
		return AAFRESULT_BAD_SIZE;

	//// On to data ...
	
	//get  Source Data
	ImplAAFPropValData * pvd_Source = dynamic_cast<ImplAAFPropValData*> (pMemberPropVal);
	ASSERTU (pvd_Source);
	
	aafUInt32 source_bitsSize;
	hr = pvd_Source->GetBitsSize (&source_bitsSize);
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU (source_bitsSize);
	ASSERTU ( targetElemSize == source_bitsSize);
	
	aafMemPtr_t pSourceData = 0;
	hr = pvd_Source->GetBits (&pSourceData);
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU (pSourceData);
	
	
	//get  Target  Data
	ImplAAFPropValData * pvd_Target = dynamic_cast<ImplAAFPropValData*> (pPropVal);
    ASSERTU (pvd_Target);
	
	aafUInt32 target_bitsSize;
	hr = pvd_Target->GetBitsSize (&target_bitsSize);
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU ( target_bitsSize >= ((index+1) * targetElemSize)  );
	
	aafMemPtr_t pTargetData = 0;
	hr = pvd_Target->GetBits (&pTargetData); //gets the 0th index location
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU (pTargetData);
	
	//make target point to the right element
	pTargetData += (index * targetElemSize);
	
	//finally, copy over the appropriate bits
	memcpy (pTargetData, pSourceData, sourceSize);
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefArray::SetCArray (
								ImplAAFPropertyValue * pPropVal,
								aafMemPtr_t pData,
								aafUInt32 dataSize)
{
	if (! pPropVal)
		return AAFRESULT_NULL_PARAM;
	
	if (! pData)
		return AAFRESULT_NULL_PARAM;
	
	if (! IsRegistered ())
		return AAFRESULT_NOT_REGISTERED;
	
	// Get the property value's embedded type and 
	// check if it's the same as the base type.
	ImplAAFTypeDefSP	pIncomingType;
	if( AAFRESULT_FAILED( pPropVal->GetType( &pIncomingType ) ) )
		return AAFRESULT_BAD_TYPE;
	ASSERTU (pIncomingType);
	if( (ImplAAFTypeDef *)pIncomingType != this )
		return AAFRESULT_BAD_TYPE;

	AAFRESULT hr;
	ImplAAFTypeDefSP pBaseType;
	hr = GetType (&pBaseType);
	
	ASSERTU (pBaseType->IsFixedSize ());
	pBaseType->AttemptBuiltinRegistration ();
	ASSERTU (pBaseType->IsRegistered ());
	
  ImplAAFRefArrayValue* pRefArray = dynamic_cast<ImplAAFRefArrayValue*>(pPropVal);
  if (NULL != pRefArray)
  {
    // This interface is not type-safe for accessing objects! There is also no
    // mechanism in place to convert between a buffer pointer and an array
    // of interface pointers; this convertion would not be necessary for
    // arrays of non-objects.
    return AAFRESULT_BAD_TYPE;
  }
	
	// Size of individual elements
	aafUInt32 elemSize = pBaseType->NativeSize ();
	// number of elements in input data.  If this is not an integral
	// number, this will round down and the test below will fail.
	aafUInt32 elemCount = dataSize / elemSize;
	// The size of the new property, calculated from number of elements
	// and the size of each element.
	aafUInt32 propSize = elemSize * elemCount;
	
	// If the given dataSize was not an integral multiple of the size of
	// each element, then we'll signal an error.
	if (propSize != dataSize)
		return AAFRESULT_BAD_SIZE;
	
	// In case of fixed-size arrays, we'll also have to see if the data
	// size matches what we're expecting.
	if (IsFixedSize ())
	{
		aafUInt32 nativeSize = NativeSize ();
		if (nativeSize != dataSize)
			return AAFRESULT_BAD_SIZE;
	}
	
	ImplAAFPropValData * pvd = 0;
	ASSERTU (pPropVal);
	pvd = dynamic_cast<ImplAAFPropValData*> (pPropVal);
	ASSERTU (pvd);
	
	aafMemPtr_t pBits = 0;
	hr = pvd->AllocateBits (propSize, &pBits);
	if (AAFRESULT_FAILED (hr))
		return hr;
	ASSERTU (pBits);
	
	memcpy (pBits, pData, propSize);
	return AAFRESULT_SUCCESS;
}



// Override callbacks from OMStorable
void ImplAAFTypeDefArray::onSave(void* clientContext) const
{
	ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefArray::onRestore(void* clientContext) const
{
	ImplAAFTypeDef::onRestore(clientContext);
}



// Method is called after associated class has been added to MetaDictionary.
// If this method fails the class is removed from the MetaDictionary and the
// registration method will fail.
HRESULT ImplAAFTypeDefArray::CompleteClassRegistration(void)
{
  ImplAAFTypeDefSP pElementType;
  AAFRESULT rc = GetType(&pElementType);
  if (AAFRESULT_SUCCEEDED(rc))
  {
    rc = pElementType->CompleteClassRegistration();
  }

  return rc;
}


