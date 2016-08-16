//@doc
//@class    AAFRefArrayValue | Implementation class for AAFRefArrayValue
#ifndef __ImplAAFRefSetValue_h__
#define __ImplAAFRefSetValue_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRefSetValue.h,v 1.6 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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

class ImplAAFTypeDefSet;
class ImplAAFTypeDefRecord;
class OMReferenceSetProperty;


class ImplAAFRefSetValue : public ImplAAFRefContainerValue
{
protected:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefSetValue ();
  
  virtual ~ImplAAFRefSetValue ();
  
  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefSet *containerType,
                        OMProperty *property);
  
  // Retrieve the property as an OMReferenceSetProperty.
  OMReferenceSetProperty * referenceSetProperty(void) const;

public:

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);


  // Remove the object identified by identification from the 
  // reference set.
  virtual AAFRESULT STDMETHODCALLTYPE 
    Remove(void* identification);

  // Does the reference set contain an object identified
  // by identification?
  virtual AAFRESULT STDMETHODCALLTYPE 
    Contains(void* identification,
             aafBoolean_t* pResult) const;

  // Find the object in the reference set property identified by 
  // identification.  If the object is found it is returned in object.
  // If the object is not found the result is false.
  virtual AAFRESULT STDMETHODCALLTYPE 
    FindObject(void* identification,
               ImplAAFStorable **pObject,
               aafBoolean_t* pResult) const;

  
  // Find the element associated with the given key.
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupElement(ImplAAFPropertyValue * pKey,
                  ImplAAFPropertyValue ** ppElementPropertyValue);

  // Test for containment by key.
  virtual AAFRESULT STDMETHODCALLTYPE
    ContainsKey(ImplAAFPropertyValue * pKey,
                aafBoolean_t*  pContainsKey);

private:  
  // Load the key (if valid) into the _keyBuffer
  void LoadKey(ImplAAFPropertyValue * pKey,
               AAFRESULT& result);

private:
  ImplAAFTypeDefRecord* _uidType; // cached type for the unique identifier property.
  aafUInt8* _keyBuffer;
  aafUInt32 _keyBufferSize;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefSetValue> ImplAAFRefSetValueSP;

#endif // ! __ImplAAFRefSetValue_h__
