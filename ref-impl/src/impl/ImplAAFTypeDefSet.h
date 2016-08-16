//@doc
//@class    AAFTypeDefSet | Implementation class for AAFTypeDefSet
#ifndef __ImplAAFTypeDefSet_h__
#define __ImplAAFTypeDefSet_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefSet.h,v 1.21 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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


class ImplAAFPropertyValue;
class ImplEnumAAFPropertyValues;
class ImplAAFTypeDefRecord;
class ImplAAFPropertyDef;
class OMTypeVisitor;


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMSetType.h"
#include "OMWeakRefProperty.h"

class ImplAAFTypeDefSet : public ImplAAFTypeDef, public OMSetType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefSet ();

protected:
  virtual ~ImplAAFTypeDefSet ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in] type of each element to be contained in this set
         ImplAAFTypeDef * pTypeDef,

         // @parm [in,string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);

  //****************
  // GetElementType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef) const;

  //****************
  // AddElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddElement
        (// @parm [in] property value corresponding to set to which element is added
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] value to be added to this set
         ImplAAFPropertyValue * pElementPropertyValue);

  //****************
  // RemoveElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveElement
        (// @parm [in] property value corresponding to set from which element is removed
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] value to be removed from this set
         ImplAAFPropertyValue * pElementPropertyValue);

  //****************
  // ContainsElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ContainsElement
        (// @parm [in] property value corresponding to set to which element is added
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] value whose presence is being tested in this set
         ImplAAFPropertyValue * pElementPropertyValue,

         // @parm [out] value to be added to this set
         aafBoolean_t*  pContainsElement);



  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [out] count of elements in the specified set property value
         aafUInt32 *  pCount);


  //****************
  // CreateKey()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateKey
        (// @parm [in,size_is(length)] Pointer to the key value bytes
         aafDataBuffer_t  pKeyPtr,

         // @parm [in] The size of the key in bytes
         aafUInt32  length,

         // @parm [out] An interface which may be passed to LookupElement() or ContainsKey()
         ImplAAFPropertyValue ** ppKey);


  //****************
  // LookupElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupElement
        (// @parm [in] property value of set
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] A key returned from CreateKey()
         ImplAAFPropertyValue * pKey,

         // @parm [out] The returned property value
         ImplAAFPropertyValue ** ppElementPropertyValue);

  //****************
  // ContainsKey()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ContainsKey
        (// @parm [in] property value of set
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [in] A key returned from CreateKey()
         ImplAAFPropertyValue * pKey,

         // @parm [out] Value returned is AAFTrue if an entry with the correct key is present
         aafBoolean_t*  pContainsKey);


  //****************
  // GetElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElements
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pSetPropertyValue,

         // @parm [out] enumerator across property values
         ImplEnumAAFPropertyValues ** ppEnum);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);

public:

  virtual OMType* elementType(void) const;

  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this set
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);
         
  ImplAAFTypeDefRecord* STDMETHODCALLTYPE 
    GetUIDType(ImplAAFTypeDef* pElementType, AAFRESULT& result) const;

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;


  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

 
  // Override from OMDefinition
  virtual const OMUniqueObjectIdentification& identification(void) const;
  virtual const wchar_t* name(void) const;
  virtual bool  hasDescription(void) const;
  virtual const wchar_t* description(void) const;
  virtual bool isPredefined(void) const;

  // Override from OMType
  virtual bool isFixedSize(void) const;
  virtual void reorder(OMByte* externalBytes,
                       OMUInt32 externalBytesSize) const;
  virtual OMUInt32 externalSize(const OMByte* internalBytes,
                                OMUInt32 internalBytesSize) const;
  virtual OMUInt32 externalSize(void) const;
  virtual void externalize(const OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByteOrder byteOrder) const;
  virtual OMUInt32 internalSize(const OMByte* externalBytes,
                                OMUInt32 externalSize) const;
  virtual OMUInt32 internalSize(void) const;
  virtual void internalize(const OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual void accept(OMTypeVisitor& visitor) const;

  // Method is called after class has been added to MetaDictionary.
  // If this method fails the class is removed from the MetaDictionary and the
  // registration method will fail.
  virtual HRESULT CompleteClassRegistration(void);


private:
  //
  // Persistent properties
  //
  OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _ElementType;
  
  //
  // Non-persistent data.
  //
  ImplAAFPropertyDef* _uidProperty; // pid for the uid
  ImplAAFTypeDefRecord* _uidType; // cached type for the unique identifier property.
};

#endif // ! __ImplAAFTypeDefSet_h__
