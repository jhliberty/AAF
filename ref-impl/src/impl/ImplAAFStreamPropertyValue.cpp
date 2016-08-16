//=---------------------------------------------------------------------=
//
// $Id: ImplAAFStreamPropertyValue.cpp,v 1.17 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFStreamPropertyValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#include "OMProperty.h"
#include "OMPropertyDefinition.h"
#include "OMDataStreamProperty.h"
#include "OMDataStreamPropertyFilter.h"
#include "OMUtilities.h"

#include "OMAssertions.h"
#include <string.h>


ImplAAFStreamPropertyValue::ImplAAFStreamPropertyValue () :
  _streamProperty(NULL),
  _streamPropertyFilter(NULL)
{
}


ImplAAFStreamPropertyValue::~ImplAAFStreamPropertyValue ()
{
  delete _streamPropertyFilter;
  _streamPropertyFilter = 0;
}
 

void ImplAAFStreamPropertyValue::setStreamAccess(OMDataStreamAccess* access)
{
	 _streamProperty->setStreamAccess(access);
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetSize (
      aafInt64 *  pSize)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;    
  
  *pSize = (aafInt64)(_streamProperty->size());
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetSize (
      aafInt64  newSize)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newSize) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;

  // *** Structured Storage PATCH! *** transdel:2000-JUN-20
  // Save the old position so that we can detect whether
  // or not the stream is being truncated.
  OMUInt64 position = _streamProperty->position();


  // Set the new size of the data stream.
  _streamProperty->setSize((OMUInt64)newSize); // What happens if this call fails?


  // *** Structured Storage PATCH! *** transdel:2000-JUN-20
  // If the file is truncated then force the position
  // to be the same as the new stream size (eof).
  // Without this PATCH Structrured Storage may leave
  // "stale bytes" in the stream after the next write
  // operation.
  if (position > (OMUInt64)newSize)
    _streamProperty->setPosition((OMUInt64)newSize); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetPosition (
      aafInt64 *  pPosition)
{
 if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pPosition)
    return AAFRESULT_NULL_PARAM;
    
  *pPosition = (aafInt64)_streamProperty->position();
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetPosition (
      aafInt64  newPosition)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newPosition) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;
  
  // Set the new size of the data stream.
  _streamProperty->setPosition((OMUInt64)newPosition); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}





 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::Read (
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;

  // Cannot read from an optional property unless it is present.
  if (_streamProperty->isOptional() && !_streamProperty->isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;
    
  // Read the bytes from the data stream.
  _streamProperty->read(pData, dataSize, *bytesRead);
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(dataSize == *bytesRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::Write (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;
    
  // Write the bytes from the data stream.
  OMUInt32 bytesWritten = 0;
  if (0 < dataSize)
  {
    _streamProperty->write(pData, dataSize, bytesWritten);
  }
  
  if (0 < dataSize && 0 == bytesWritten)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(dataSize == bytesWritten);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::Append (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;

  // Set the position to the size of the stream.
  _streamProperty->setPosition(_streamProperty->size());
       
  // Write the bytes from the data stream.
  OMUInt32 bytesWritten = 0;
  if (0 < dataSize)
  {
    _streamProperty->write(pData, dataSize, bytesWritten);
  }
  
  if (0 < dataSize && 0 == bytesWritten)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(dataSize == bytesWritten);  
  return AAFRESULT_SUCCESS;
}






AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::HasStoredByteOrder (
      aafBoolean_t *  pHasByteOrder)
{
  if (NULL == pHasByteOrder)
    return AAFRESULT_NULL_PARAM;

  if (_streamProperty->hasByteOrder())
    *pHasByteOrder = kAAFTrue;
  else
    *pHasByteOrder = kAAFFalse;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetStoredByteOrder (
      eAAFByteOrder_t *  pByteOrder)
{
  if (NULL == pByteOrder)
    return AAFRESULT_NULL_PARAM;

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;

  OMByteOrder byteOrder = _streamProperty->byteOrder();
  if (byteOrder == littleEndian)
    *pByteOrder = kAAFByteOrderLittle;
  else
    *pByteOrder = kAAFByteOrderBig;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetStoredByteOrder (
      eAAFByteOrder_t  byteOrder)
{      
  // Cannot set the byte order if there is an existing byte order.
  if (_streamProperty->hasByteOrder())
    return AAFRESULT_INVALID_BYTEORDER;
  
  // Cannot set the byte order if the stream is non-empty.
  if (0 < _streamProperty->size())
    return AAFRESULT_INVALID_BYTEORDER;
  
  if (byteOrder == kAAFByteOrderLittle)
    _streamProperty->setByteOrder(littleEndian);
  else
    _streamProperty->setByteOrder(bigEndian);

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::ClearStoredByteOrder (void)
{      
  // Cannot clear the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;

  _streamProperty->clearByteOrder();
  return AAFRESULT_SUCCESS;
}



 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::ReadElements (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;    

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize();
  ASSERTU (0 < internalElementSize);
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Get the external size of an element from the type.  
  OMUInt32 externalElementSize = pElementType->PropValSize();
  ASSERTU (0 < externalElementSize);
  if (0 == externalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Cannot read from an optional property unless it is present.
  if (_streamProperty->isOptional() && !_streamProperty->isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;
  
  // Read the elements from the data stream.
  OMUInt32 elementsRead;
  _streamProperty->readTypedElements(pElementType->type(),
                                     externalElementSize,
                                     pData,
                                     elementCount,
                                     elementsRead);
  *bytesRead = elementsRead * internalElementSize;
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(elementCount == elementsRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::WriteElements (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData)
    return AAFRESULT_NULL_PARAM;    

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize(); 
  ASSERTU (0 < internalElementSize);
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(pElementType->type(),
                                     internalElementSize,
                                     pData,
                                     elementCount,
                                     elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  ASSERTU(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::AppendElements (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize(); 
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Set the position to the size of the stream.
  _streamProperty->setPosition(_streamProperty->size());
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamProperty->writeTypedElements(pElementType->type(),
                                      internalElementSize,
                                      pData,
                                      elementCount,
                                      elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  ASSERTU(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}





AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetSizeFiltered (
      aafInt64 *  pSize)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;    
  
  *pSize = (aafInt64)(_streamPropertyFilter->size());
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetSizeFiltered (
      aafInt64  newSize)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newSize) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;

  // *** Structured Storage PATCH! *** transdel:2000-JUN-20
  // Save the old position so that we can detect whether
  // or not the stream is being truncated.
  OMUInt64 position = _streamPropertyFilter->position();


  // Set the new size of the data stream.
  _streamPropertyFilter->setSize((OMUInt64)newSize); // What happens if this call fails?


  // *** Structured Storage PATCH! *** transdel:2000-JUN-20
  // If the file is truncated then force the position
  // to be the same as the new stream size (eof).
  // Without this PATCH Structrured Storage may leave
  // "stale bytes" in the stream after the next write
  // operation.
  if (position > (OMUInt64)newSize)
    _streamPropertyFilter->setPosition((OMUInt64)newSize); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::GetPositionFiltered (
      aafInt64 *  pPosition)
{
 if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pPosition)
    return AAFRESULT_NULL_PARAM;
    
  *pPosition = (aafInt64)_streamPropertyFilter->position();
    
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::SetPositionFiltered (
      aafInt64  newPosition)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (0 > newPosition) // TEMP: need unsigned aafUInt64!
    return AAFRESULT_INVALID_PARAM;
  
  // Set the new size of the data stream.
  _streamPropertyFilter->setPosition((OMUInt64)newPosition); // What happens if this call fails?
    
  return AAFRESULT_SUCCESS;
}





 AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::ReadFiltered (
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;

  // Cannot read from an optional property unless it is present.
  if (_streamProperty->isOptional() && !_streamProperty->isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;
    
  // Read the bytes from the data stream.
  _streamPropertyFilter->read(pData, dataSize, *bytesRead);
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(dataSize == *bytesRead);  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::WriteFiltered (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;
    
  // Write the bytes from the data stream.
  OMUInt32 bytesWritten = 0;
  if (0 < dataSize)
  {
    _streamPropertyFilter->write(pData, dataSize, bytesWritten);
  }
  
  if (0 < dataSize && 0 == bytesWritten)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(dataSize == bytesWritten);  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::AppendFiltered (
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pData)
    return AAFRESULT_NULL_PARAM;    

  // Set the position to the size of the stream.
  _streamPropertyFilter->setPosition(_streamPropertyFilter->size());
       
  // Write the bytes from the data stream.
  OMUInt32 bytesWritten = 0;
  if (0 < dataSize)
  {
    _streamPropertyFilter->write(pData, dataSize, bytesWritten);
  }
  
  if (0 < dataSize && 0 == bytesWritten)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(dataSize == bytesWritten);  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::AppendElementsFiltered (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData)
    return AAFRESULT_NULL_PARAM;    
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize(); 
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Set the position to the size of the stream.
  _streamPropertyFilter->setPosition(_streamPropertyFilter->size());
   
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamPropertyFilter->writeTypedElements(pElementType->type(),
                                    internalElementSize,
                                    pData,
                                    elementCount,
                                    elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  ASSERTU(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFStreamPropertyValue::WriteElementsFiltered (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData)
    return AAFRESULT_NULL_PARAM;    

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize(); 
  ASSERTU (0 < internalElementSize);
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Write the elements to the data stream.
  OMUInt32 elementsWritten;
  _streamPropertyFilter->writeTypedElements(pElementType->type(),
                                    internalElementSize,
                                    pData,
                                    elementCount,
                                    elementsWritten);
  
  if (0 < dataSize && 0 == elementsWritten)
    return AAFRESULT_CONTAINERWRITE; 
    
  ASSERTU(elementCount == elementsWritten); 
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
   ImplAAFStreamPropertyValue::ReadElementsFiltered (
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pElementType || NULL == pData || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;    

  // Cannot get the byte order if it has never been set.
  if (!_streamProperty->hasByteOrder())
    return AAFRESULT_NOBYTEORDER;
  
  // Element access methods need to fail if the stream element type
  // offsets have not been registered. 
  if (!pElementType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;
  
  OMUInt32 internalElementSize = pElementType->NativeSize();
  ASSERTU (0 < internalElementSize);
  if (0 == internalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Make sure that the given dataSize is an integral number of stream elements.
  OMUInt32 elementCount = dataSize / internalElementSize;
  if (dataSize != (elementCount * internalElementSize))
    return AAFRESULT_INVALID_PARAM;
  
  // Get the external size of an element from the type.  
  OMUInt32 externalElementSize = pElementType->PropValSize();
  ASSERTU (0 < externalElementSize);
  if (0 == externalElementSize)
     return AAFRESULT_INVALID_PARAM; 

  // Cannot read from an optional property unless it is present.
  if (_streamProperty->isOptional() && !_streamProperty->isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;
  
  // Read the elements from the data stream.
  OMUInt32 elementsRead;
  _streamPropertyFilter->readTypedElements(pElementType->type(),
                                   externalElementSize,
                                   pData,
                                   elementCount,
                                   elementsRead);
  *bytesRead = elementsRead * internalElementSize;
  
  if (0 < dataSize && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;
  
  ASSERTU(elementCount == elementsRead);  
  return AAFRESULT_SUCCESS;
}





AAFRESULT ImplAAFStreamPropertyValue::Initialize (
  const ImplAAFTypeDefStream *streamType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  ASSERTU (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  ASSERTU (property);
  if (NULL == streamType || NULL == property)
    return AAFRESULT_NULL_PARAM;

  // Get the type definition. This must be a stream type.
  ASSERTU (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  ASSERTU (type);
  
  // The given property must be an OM stream property.  
  OMDataStreamProperty *streamProperty = dynamic_cast<OMDataStreamProperty *>(property);
  ASSERTU (streamProperty);
  if (NULL == streamProperty)
    return AAFRESULT_INVALID_PARAM;
  
  // Make sure the stream is valid and attached to a persisted storable.
  const OMPropertySet *propertySet = streamProperty->propertySet();
  ASSERTU(propertySet);
  if (!propertySet)
    return AAFRESULT_INVALID_PARAM;
  OMStorable * container = propertySet->container();
  ASSERTU(container);
  if (!container)
    return AAFRESULT_INVALID_PARAM;

  // Cannot access the data property if it is NOT associated with a file.
  if (!container->persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  result = ImplAAFPropertyValue::Initialize(streamType, streamProperty);
  if (AAFRESULT_SUCCEEDED(result))
  {
    _streamProperty = streamProperty;
    _streamPropertyFilter = streamProperty->createFilter();
   
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFStreamPropertyValue::WriteTo(
  OMProperty* pOmProp)
{
  // Make sure that the given property is the same one that was used to 
  // initialize this property value. NOTE: Copying stream data to a 
  // different OMProperty should be handled through another interface.
  if (pOmProp != _streamProperty)
    return AAFRESULT_INVALID_PARAM;
    
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE ImplAAFStreamPropertyValue::GetEssenceElementKey(
  aafUID_t * pEssenceElementKey)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == pEssenceElementKey)
    return AAFRESULT_NULL_PARAM;
  if (!_streamProperty->hasEssenceElementKey())
      return AAFRESULT_OPERATION_NOT_PERMITTED;

  convert( *reinterpret_cast<OMUniqueObjectIdentification*>(pEssenceElementKey),
           _streamProperty->essenceElementKey() );

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE ImplAAFStreamPropertyValue::SetEssenceElementKey(
  aafUID_constref  key)
{
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  if (!_streamProperty->hasEssenceElementKey())
      return AAFRESULT_OPERATION_NOT_PERMITTED;

  OMKLVKey klvKey;
  convert( klvKey,
           *reinterpret_cast<const OMUniqueObjectIdentification*>(&key) );
  _streamProperty->setEssenceElementKey(klvKey);

  return AAFRESULT_SUCCESS;
}
