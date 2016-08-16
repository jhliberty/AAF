//@doc
//@class    AAFRefArrayValue | Implementation class for AAFRefArrayValue
#ifndef __ImplAAFRefArrayValue_h__
#define __ImplAAFRefArrayValue_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRefArrayValue.h,v 1.8 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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



#ifndef __ImplAAFRefContainerValue_h__
#include "ImplAAFRefContainerValue.h"
#endif

// Forward references:
class ImplAAFTypeDefArray;
class OMObjectVector;
class ImplAAFStorable;
template <typename ReferencedObject> class OMReferenceVector;

class ImplAAFRefArrayValue : public ImplAAFRefContainerValue
{
protected:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefArrayValue ();
  
  virtual ~ImplAAFRefArrayValue ();

  // non-published method to initialize this object.
  // Initialize an instance from a type definition. This is the "old-style"
  // "non-direct" access initialization method. 
  AAFRESULT Initialize (const ImplAAFTypeDefArray *containerType,
                        bool fixedSize);
  
  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefArray *containerType,
                        OMProperty *property,
                        bool fixedSize);
  
  
  // Retrieve the property or temporary reference vector as an OMReferenceContainer.
  virtual OMReferenceContainer* referenceContainer(void) const;

	// Retrieve the property or temporary reference vector as an OMObjectVector.
	OMObjectVector * referenceVector(void) const;
  
  bool fixedSize(void) const;
  
public:

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);

  //
  // The following methods are overrides of ImplAAFRefContainerValue
  //
  
  // Insert the given object into this contain property.
  virtual AAFRESULT STDMETHODCALLTYPE InsertObject(ImplAAFStorable* pObject);
  
  // Remove the given object from the container property.
  virtual AAFRESULT STDMETHODCALLTYPE RemoveObject(ImplAAFStorable* pObject);
  

  //
  // Methods to access the elements from the OMReferenceVectorProperty
  // 

  // Set the value of the OMReferenceVectorProperty at position index to pObject.
  virtual AAFRESULT STDMETHODCALLTYPE SetObjectAt(ImplAAFStorable* pObject,
                                aafUInt32 index);

  // Get the value of the OMReferenceVectorProperty at position index.
  virtual AAFRESULT STDMETHODCALLTYPE GetObjectAt(aafUInt32 index, ImplAAFStorable** ppObject) const;

  // Append the given pObject to the OMReferenceVectorProperty.
  virtual AAFRESULT STDMETHODCALLTYPE AppendObject(ImplAAFStorable* pObject);

  // Prepend the given pObject> to the OMReferenceVectorProperty.
  virtual AAFRESULT STDMETHODCALLTYPE PrependObject(ImplAAFStorable* pObject);

  // Remove the object from the OMReferenceVectorProperty at position index.
  // Existing objects in the OMReferenceVectorProperty at index + 1 and higher 
  // are shifted down one index position.
  virtual AAFRESULT STDMETHODCALLTYPE RemoveObjectAt(aafUInt32 index);

  // Insert pObject into the OMReferenceVectorProperty at position index. 
  // Existing objects at index> and higher are shifted up one index position.
  virtual AAFRESULT STDMETHODCALLTYPE InsertObjectAt(ImplAAFStorable* pObject, aafUInt32 index);
  
  

  //
  // Methods to access the elements from the OMReferenceVectorProperty as
  // property values.
  // 

  // Set the value of the OMReferenceVectorProperty at position index to the
  // object inside the property value.
  virtual AAFRESULT STDMETHODCALLTYPE SetElementAt(ImplAAFPropertyValue* pPropertyValue,
                                aafUInt32 index);

  // Get the value of the OMReferenceVectorProperty at position index.
  virtual AAFRESULT STDMETHODCALLTYPE GetElementAt(aafUInt32 index, ImplAAFPropertyValue** ppPropertyValue) const;

  // Append the given pObject to the OMReferenceVectorProperty.
  virtual AAFRESULT STDMETHODCALLTYPE AppendElement(ImplAAFPropertyValue* pPropertyValue);

  // Prepend the given pObject> to the OMReferenceVectorProperty.
  virtual AAFRESULT STDMETHODCALLTYPE PrependElement(ImplAAFPropertyValue* pPropertyValue);

  // Insert pObject into the OMReferenceVectorProperty at position index. 
  // Existing objects at index> and higher are shifted up one index position.
  virtual AAFRESULT STDMETHODCALLTYPE InsertElementAt(ImplAAFPropertyValue* pPropertyValue, aafUInt32 index);
  
private:
  bool _fixedSize; // true if the array property is fixed size
  OMReferenceVector<ImplAAFStorable> * _tempStorableVector; // container for temporary storables
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefArrayValue> ImplAAFRefArrayValueSP;

#endif // ! __ImplAAFRefArrayValue_h__
