//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefStream.cpp,v 1.34 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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






#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#ifndef __ImplAAFStreamPropertyValue_h__
#include "ImplAAFStreamPropertyValue.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include "OMPropertyDefinition.h"
#include "OMDataStreamAccess.h"
#include "OMDataStreamProperty.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMTypeVisitor.h"
#include "AAF.h"
#include "AAFUtils.h"

#include "OMAssertions.h"
#include <string.h>


extern "C" const aafClassID_t CLSID_AAFStreamPropertyValue;
extern "C" const aafClassID_t CLSID_AAFPlainStreamData;

struct IAAFPropertyValue;

class ImplAAFOMDataStreamAccess : public OMDataStreamAccess {
public:
  // @access Public members.

    // @cmember Constructor.
  ImplAAFOMDataStreamAccess(IAAFPropertyValue *pVal, IAAFStreamAccess *access, void* context);

  virtual ~ImplAAFOMDataStreamAccess();

  virtual void save(OMDataStream& stream, void* context);


private:
  IAAFStreamAccess*		_callback;
  IAAFPropertyValue*	_pVal;
};

ImplAAFOMDataStreamAccess::ImplAAFOMDataStreamAccess(IAAFPropertyValue *pVal, IAAFStreamAccess *access, void* context) :
  OMDataStreamAccess(context), _callback(access), _pVal(pVal)
{
  if (_callback != 0) {
    _callback->AddRef();
  }
  _pVal->AddRef(); 
}

ImplAAFOMDataStreamAccess::~ImplAAFOMDataStreamAccess()
{
  if (_callback != 0) {
    _callback->Release();
  }
  _pVal->Release();
}

void ImplAAFOMDataStreamAccess::save(OMDataStream& /* stream */, void* context)
{
	(void)_callback->WriteStream(_pVal, (unsigned char*)context);
}

ImplAAFTypeDefStream::ImplAAFTypeDefStream ()
{
}


ImplAAFTypeDefStream::~ImplAAFTypeDefStream ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::pvtInitialize (
      const aafUID_t & id,
      const aafCharacter * pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (! pTypeName)
	return AAFRESULT_NULL_PARAM;

  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
 
   // Initialize the type definition with its unique identifier and name.
  result = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
  if (AAFRESULT_FAILED (result))
    return result; 

  // This type definition stream instance has been initialized.
  setInitialized();

  return AAFRESULT_SUCCESS;
}



// 
// Make sure that this is a valid stream property value.
//
AAFRESULT ImplAAFTypeDefStream::GetStreamPropertyValue(
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
  eAAFTypeCategory_t thisTypeCategory = kAAFTypeCatUnknown;
  GetTypeCategory(&thisTypeCategory);
  if (propertyTypeCategory != thisTypeCategory)
    return AAFRESULT_INVALID_PARAM;
  
  pStreamPropertyValue = dynamic_cast<ImplAAFStreamPropertyValue *>(pPropertyValue);
  if (NULL == pStreamPropertyValue)
    return AAFRESULT_INVALID_PARAM;
    
  return AAFRESULT_SUCCESS;
}

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










AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetSize(pSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::SetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetSize(newSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetPosition(pPosition);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::SetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetPosition(newPosition);
}





AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefStream::Read (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->Read(dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::Write (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->Write(dataSize, pData);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::Append (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->Append(dataSize, pData);
}






AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::HasStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      aafBoolean_t *  pHasByteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->HasStoredByteOrder(pHasByteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      eAAFByteOrder_t *  pByteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetStoredByteOrder(pByteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::SetStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      eAAFByteOrder_t  byteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetStoredByteOrder(byteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::ClearStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ClearStoredByteOrder();
}






AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefStream::ReadElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ReadElements(pElementType, dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::WriteElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->WriteElements(pElementType, dataSize, pData);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::AppendElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->AppendElements(pElementType, dataSize, pData);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetPlainStreamData (
      aafUInt32 reserved,
      ImplAAFPlainStreamData ** pPlainStreamData)
{
  AAFRESULT result = AAFRESULT_NOT_IMPLEMENTED;

  if (reserved != 0)
    return AAFRESULT_INVALID_PARAM;

  ImplAAFPlainStreamData* pResult =
    (ImplAAFPlainStreamData*) CreateImpl (CLSID_AAFPlainStreamData);
  if (pPlainStreamData)
  {
    *pPlainStreamData = pResult;
    result = AAFRESULT_SUCCESS;
  }
  else
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetTypeCategory (
      eAAFTypeCategory_t *  pTid)
{
  if (NULL == pTid)
    return AAFRESULT_NULL_PARAM;
  
  *pTid = kAAFTypeCatStream;
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  if (NULL == ppRawTypeDef)
    return AAFRESULT_NULL_PARAM;
  
  // There is no "raw access type" for a stream. For now
  // just return this stream type and let the caller handle
  // reading the stream.
  *ppRawTypeDef = this;
  (*ppRawTypeDef)->AcquireReference();
  
  return AAFRESULT_SUCCESS;
}



// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefStream::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ASSERTU (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
 
  ImplAAFStreamPropertyValue *pStreamValue = NULL;
  pStreamValue = (ImplAAFStreamPropertyValue*) CreateImpl (CLSID_AAFStreamPropertyValue);
  if (!pStreamValue) 
    return AAFRESULT_NOMEMORY;

  // Attempt to initialize the stream value. This will fail if given property is not a valid
  // stream property.
  result = pStreamValue->Initialize (this, property);
  if (AAFRESULT_SUCCEEDED(result))
  {
	*ppPropertyValue = pStreamValue; // The reference count is already 1.
	pStreamValue = NULL;
  }
  else
  {
    pStreamValue->ReleaseReference();
  }
  return result;
}



AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefStream::SetCallback
		(
		ImplAAFPropertyValue * pPropertyValue,
        IAAFStreamAccess*  pCallbackIF,
        aafMemPtr_t  pUserData)
{
  ImplAAFOMDataStreamAccess	*access;
  IAAFPropertyValue		*pvalInterface;

  CHECK_CLIENT_IMPLEMENTED_QI(pCallbackIF, IID_IAAFStreamAccess);

  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  IUnknown *iUnk = static_cast<IUnknown *> (pPropertyValue->GetContainer());
  iUnk->QueryInterface(IID_IAAFPropertyValue, (void **)&pvalInterface); //This call increments the reference counter
  access = new ImplAAFOMDataStreamAccess(pvalInterface, pCallbackIF, pUserData);
  pvalInterface->Release(); //Decrement reference counter 
 
  pStreamPropertyValue->setStreamAccess(access);

  return AAFRESULT_SUCCESS;
}

const OMUniqueObjectIdentification&
ImplAAFTypeDefStream::identification(void) const
{
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDefStream::name(void) const
{
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDefStream::hasDescription(void) const
{
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDefStream::description(void) const
{
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDefStream::isPredefined(void) const
{
  return ImplAAFMetaDefinition::isPredefined();
}

bool ImplAAFTypeDefStream::isFixedSize(void) const
{
  bool result = false;
  if (IsFixedSize() == kAAFTrue) {
    result = true;
  }
  return result;
}

void ImplAAFTypeDefStream::reorder(OMByte* /* bytes */,
                                   OMUInt32 /* bytesSize */) const
{
  // Should be properly implemented
  ASSERTU (0);
}

OMUInt32 ImplAAFTypeDefStream::externalSize(const OMByte* /* internalBytes */,
                                            OMUInt32 /* internalBytesSize */) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}

OMUInt32 ImplAAFTypeDefStream::externalSize(void) const
{
  return PropValSize();
}

void ImplAAFTypeDefStream::externalize(const OMByte* /* internalBytes */,
                                       OMUInt32 /* internalBytesSize */,
                                       OMByte* /* externalBytes */,
                                       OMUInt32 /* externalBytesSize */,
                                       OMByteOrder /* byteOrder */) const
{
  // Should be properly implemented
  ASSERTU (0);
}

OMUInt32 ImplAAFTypeDefStream::internalSize(const OMByte* /* externalBytes */,
                                            OMUInt32 /* externalBytesSize */) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}

OMUInt32 ImplAAFTypeDefStream::internalSize(void) const
{
  return NativeSize();
}

void ImplAAFTypeDefStream::internalize(const OMByte* /* externalBytes */,
                                       OMUInt32 /* externalBytesSize */,
                                       OMByte* /* internalBytes */,
                                       OMUInt32 /* internalBytesSize */,
                                       OMByteOrder /* byteOrder */) const
{
  // Should be properly implemented
  ASSERTU (0);
}

void ImplAAFTypeDefStream::accept(OMTypeVisitor& visitor) const
{
  visitor.visitStreamType(this);
}

//
// Overrides of ImplAAFTypeDef
//

aafBool ImplAAFTypeDefStream::IsFixedSize (void) const
{
  return kAAFFalse;
}


OMUInt32 ImplAAFTypeDefStream::PropValSize (void) const
{
  ASSERTU (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDefStream::IsRegistered (void) const
{
  // This is an axiomatic type.
  return kAAFTrue;
}


OMUInt32 ImplAAFTypeDefStream::NativeSize (void) const
{
  ASSERTU (0);
  return 0; // not reached!
}


bool ImplAAFTypeDefStream::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefStream::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefStream::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefStream::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefStream::IsStringable () const
{ return false; }



OMProperty * ImplAAFTypeDefStream::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  // TEMPORARY: Need to use non-template class: Use existing OMDataStreamProperty
  // until new non-template typed-stream class is available.
  return new OMDataStreamProperty(pid, name);
}




// override from OMStorable.
const OMClassId& ImplAAFTypeDefStream::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefStream));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefStream::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefStream::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

