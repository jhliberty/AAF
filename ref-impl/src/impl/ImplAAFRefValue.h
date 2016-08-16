//@doc
//@class    AAFRefValue | Implementation class for AAFPropertyValue
#ifndef __ImplAAFRefValue_h__
#define __ImplAAFRefValue_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRefValue.h,v 1.7 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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
class OMStorable;
class ImplAAFRoot;
class ImplAAFStorable;

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif


class ImplAAFTypeDefObjectRef;

class OMProperty;
class OMReferenceProperty; // The base class for singleton references


class ImplAAFRefValue : public ImplAAFPropertyValue
{
public:

protected:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefValue ();
  virtual ~ImplAAFRefValue ();


  // non-published method to initialize this object.
  // Initialize an instance from a type definition. This is the "old-style"
  // "non-direct" access initialization method. 
  AAFRESULT Initialize (const ImplAAFTypeDefObjectRef *referenceType);

  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefObjectRef *referenceType,
                        OMProperty *property);
  
  // Return the propertyType as a referenceType.
  const ImplAAFTypeDefObjectRef *referenceType(void) const;
  
  // Return the instance's property as a reference property.
  OMReferenceProperty * referenceProperty(void) const;
  
  void SetLocalObject(ImplAAFStorable * localObject); // reference counted
  ImplAAFStorable * GetLocalObject(void) const; // not reference counted
  
public:
  virtual AAFRESULT STDMETHODCALLTYPE GetObject(ImplAAFStorable **ppObject) const = 0;
  virtual AAFRESULT STDMETHODCALLTYPE SetObject(ImplAAFStorable *pObject) = 0;

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);
  
  // Conversion utilities
  static ImplAAFStorable * ConvertOMObjectToRoot(OMObject *object);
  static ImplAAFStorable * ConvertRootToOMStorable(ImplAAFRoot *object);

private:
  ImplAAFStorable *_localObject; // Should be NULL if there is an associated property.

};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefValue> ImplAAFRefValueSP;

#endif // ! __ImplAAFRefValue_h__
