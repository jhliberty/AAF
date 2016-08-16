//@doc
//@class    AAFRefContainerValue | Implementation class for AAFPropertyValue
#ifndef __ImplAAFRefContainerValue_h__
#define __ImplAAFRefContainerValue_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRefContainerValue.h,v 1.10 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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


class OMObject;
class OMProperty;
class OMReferenceContainer;


class ImplAAFStorable;
class ImplAAFTypeDef;
class ImplEnumAAFPropertyValues;
class ImplAAFTypeDefObjectRef;

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

class ImplAAFRefContainerValue : public ImplAAFPropertyValue // TBD:
{
protected:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefContainerValue ();
  virtual ~ImplAAFRefContainerValue ();

  // non-published method to initialize this object.
  // Initialize an instance from a type definition. This is the "old-style"
  // "non-direct" access initialization method. 
  AAFRESULT Initialize (const ImplAAFTypeDef *propertyType);

  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDef *containerType,
                        OMProperty *property);
  
  // Retrieve the property as an OMReferenceContainer.
  virtual OMReferenceContainer* referenceContainer(void) const;

  // Utility for converting the a property value (parameter) into an object.
  static ImplAAFStorable * GetStorableFromPropertyValue(ImplAAFPropertyValue* pPropertyValue, AAFRESULT & result);
  
  // Utility to release an old OMObject from the container.
  void ReleaseOldObject(OMObject *object);
  
  // Utility to release all old OMObjects from the given container.
  AAFRESULT ReleaseAllObjects(OMReferenceContainer *pContainerProperty);
  
  //
  // Methods that must be overridden by subclasses of reference containers.
  //
public:  
  // Return the type of object references in the container.
  virtual ImplAAFTypeDefObjectRef * GetElementType(void) const = 0; // the result is NOT reference counted.
  
public:
  // Perform specialized validation for an object before it is added
  // to a container.
  virtual AAFRESULT ValidateNewObject(ImplAAFStorable *pNewObject) const = 0;
  
  // Perform any specialized cleanup of any object after it has been removed
  // from the container.
  virtual bool usesReferenceCounting(void) const = 0;
  
public:
  // Override of ImplAAFPropertyValue (just validates input property).
  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);

  
  // Insert the given object into this contain property.
  virtual AAFRESULT STDMETHODCALLTYPE InsertObject(ImplAAFStorable* pObject);

  // Is the given object in the container property.
  virtual AAFRESULT STDMETHODCALLTYPE ContainsObject(ImplAAFStorable* pObject, aafBoolean_t* pResult);
  
  // Remove the given object from the container property.
  virtual AAFRESULT STDMETHODCALLTYPE RemoveObject(ImplAAFStorable* pObject);


  
  // Insert the given object into this contain property.
  virtual AAFRESULT STDMETHODCALLTYPE InsertElement(ImplAAFPropertyValue* pPropertyValue);

  // Is the given object in the container property.
  virtual AAFRESULT STDMETHODCALLTYPE ContainsElement(ImplAAFPropertyValue* pPropertyValue, aafBoolean_t* pResult);

  // The number of objects in the container property
  virtual AAFRESULT STDMETHODCALLTYPE Count(aafUInt32* pCount);
  
  // Remove the given object from the container property.
  virtual AAFRESULT STDMETHODCALLTYPE RemoveElement(ImplAAFPropertyValue* pPropertyValue);

  // Get an enumerator for the given container property
  virtual AAFRESULT STDMETHODCALLTYPE GetElements(ImplEnumAAFPropertyValues** ppEnum);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefContainerValue> ImplAAFRefContainerValueSP;

#endif // ! __ImplAAFRefContainerValue_h__
