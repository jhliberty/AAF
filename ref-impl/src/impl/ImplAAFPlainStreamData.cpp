//=---------------------------------------------------------------------=
//
// $Id: ImplAAFPlainStreamData.cpp,v 1.6 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFPlainStreamData.h"
#include "ImplAAFStreamPropertyValue.h"
#include "ImplAAFTypeDef.h"


//
// Macro to encapsulate the convertions and validation of a ImplAAFPropertyValue
// into an ImplAAFStreamPropertyValue.
//
#define PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pv, spv)\
  AAFRESULT result = AAFRESULT_SUCCESS;\
  ImplAAFStreamPropertyValue * spv = NULL;\
  result = GetStreamPropertyValue(pv, spv);\
  if (AAFRESULT_FAILED(result))\
    return result



ImplAAFPlainStreamData::ImplAAFPlainStreamData ()
: ImplAAFRoot()
{
}


ImplAAFPlainStreamData::~ImplAAFPlainStreamData ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::GetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetSizeFiltered(pSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::SetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetSizeFiltered(newSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::GetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetPositionFiltered(pPosition);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::SetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetPositionFiltered(newPosition);
}



AAFRESULT STDMETHODCALLTYPE
   ImplAAFPlainStreamData::Read (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ReadFiltered(dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::Write (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->WriteFiltered(dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::Append (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->AppendFiltered(dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::HasStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      aafBoolean_t *  pHasByteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->HasStoredByteOrder(pHasByteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::GetStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      eAAFByteOrder_t *  pByteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetStoredByteOrder(pByteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::SetStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      eAAFByteOrder_t  byteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetStoredByteOrder(byteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::ClearStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ClearStoredByteOrder();
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFPlainStreamData::ReadElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ReadElementsFiltered(pElementType, dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::WriteElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->WriteElementsFiltered(pElementType, dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::AppendElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->AppendElementsFiltered(pElementType, dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::GetEssenceElementKey (
      ImplAAFPropertyValue * pPropertyValue,
      aafUID_t * pEssenceElementKey)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);

  return pStreamPropertyValue->GetEssenceElementKey(pEssenceElementKey);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPlainStreamData::SetEssenceElementKey (
      ImplAAFPropertyValue * pPropertyValue,
      aafUID_constref  key)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetEssenceElementKey(key);
}


AAFRESULT ImplAAFPlainStreamData::GetStreamPropertyValue(
  ImplAAFPropertyValue * pPropertyValue,
  ImplAAFStreamPropertyValue *& pStreamPropertyValue)
{
  pStreamPropertyValue = NULL; // init out parameter
  
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
  
  // The stream property value's type should be this instance of 
  // ImplAAFTypeDefStream.
  ImplAAFTypeDefSP pPropertyValueType;
  pPropertyValue->GetType(&pPropertyValueType);
  eAAFTypeCategory_t propertyTypeCategory = kAAFTypeCatUnknown;
  pPropertyValueType->GetTypeCategory(&propertyTypeCategory);
  if (propertyTypeCategory != kAAFTypeCatStream)
    return AAFRESULT_INVALID_PARAM;
  
  pStreamPropertyValue = dynamic_cast<ImplAAFStreamPropertyValue *>(pPropertyValue);
  if (NULL == pStreamPropertyValue)
    return AAFRESULT_INVALID_PARAM;
    
  return AAFRESULT_SUCCESS;
}

