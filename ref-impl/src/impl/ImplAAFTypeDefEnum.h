//@doc
//@class    AAFTypeDefEnum | Implementation class for AAFTypeDefEnum
#ifndef __ImplAAFTypeDefEnum_h__
#define __ImplAAFTypeDefEnum_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefEnum.h,v 1.37 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefProperty.h"
#include "OMArrayProperty.h"
#include "OMEnumeratedType.h"

class ImplAAFPropertyValue;
class OMTypeVistor;

class ImplAAFTypeDefEnum : public ImplAAFTypeDef, public OMEnumeratedType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefEnum ();

protected:
  virtual ~ImplAAFTypeDefEnum ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] Type of values in this enumeration
         ImplAAFTypeDef * pType,

         // @parm [in, size_is(numElems)] array of element values to be represented in this enumerated
    // type
         aafInt64 * pElementValues,

         // @parm [in, size_is(numElems)] array of element names to be represented in this enumerated
    // type
         aafCharacter_constptr *  pElementNames,

         // @parm [in] number of members in pElementValues and pElementNames arrays
         aafUInt32  numElems,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // CreateValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE CreateValue (
      aafMemPtr_t  pVal,
      aafUInt32  valSize,
      ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetElementType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementType
        // @parm [out] type definition of values of this enumeration
        (ImplAAFTypeDef ** ppTypeDef) const;


  //****************
  // CountElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountElements
        // @parm [out] count of elements within this enumeration
        (aafUInt32 *  pCount);


  //****************
  // GetElementValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementValue
        (// @parm [in] index of element to retrieve
         aafUInt32 index,

         // @parm [out] requested value
         aafInt64 * pOutValue);



  //****************
  // GetNameFromValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameFromValue
        (// @parm [in] value of element to get
         ImplAAFPropertyValue * pValue,

         // @parm [out, size_is(bufSize), string] buffer into which the element name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLenFromValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLenFromValue
        (// @parm [in] value of element to get
         ImplAAFPropertyValue * pValue,

         // @parm [out] required buffer length, in bytes
         aafUInt32 *  pLen);


  //****************
  // GetNameFromInteger()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameFromInteger
        (// @parm [in] value of element to get
         aafInt64  value,

         // @parm [out, size_is(bufSize), string] buffer into which the element name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLenFromInteger()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLenFromInteger
        (// @parm [in] value of element to get
         aafInt64  value,

         // @parm [out] required buffer length, in bytes
         aafUInt32 *  pLen);


  //****************
  // GetIntegerValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIntegerValue
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropValIn,

         // @parm [out] value of the enum represented by the given input property value
         aafInt64 *  pValueOut);


  //****************
  // SetIntegerValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIntegerValue
        (// @parm [in] property value to set
         ImplAAFPropertyValue * pPropValToSet,

         // @parm [in] new value of the enum represented by the given property value
         aafInt64  valueIn);

///////////////////
  virtual AAFRESULT STDMETHODCALLTYPE
		CreateValueFromName (
    // the Name of a valid Enum symbol
    /*[in]*/ aafCharacter_constptr  Name,

    // newly created property value
    /*[out]*/ ImplAAFPropertyValue ** ppPropVal);

  virtual AAFRESULT STDMETHODCALLTYPE
	  GetElementName (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // requested value
    /*[out, size_is(bufSize)]*/ aafCharacter *  pOutValue,

    // The size of the pOutValue buffer in bytes
    /*[in]*/ aafUInt32  bufSize);
  
  virtual AAFRESULT STDMETHODCALLTYPE
	 GetElementNameBufLen (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // required buffer length, in bytes
    /*[out]*/ aafUInt32 *  pLen);


  //****************
  // RegisterSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterSize
        (// @parm [in] size of this enum
         aafUInt32  enumSize);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



  // non-published overrides from AAFTypeDef
  aafBool IsFixedSize (void) const;
  OMUInt32 PropValSize (void) const;
  aafBool IsRegistered (void) const;
  OMUInt32 NativeSize (void) const;
  void AttemptBuiltinRegistration (void);

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;


  //*************************************************************
  //
  // Overrides from OMDefinition
  //
  //*************************************************************
  virtual const OMUniqueObjectIdentification& identification(void) const;

  virtual const wchar_t* name(void) const;

  virtual bool hasDescription(void) const;

  virtual const wchar_t* description(void) const;

  virtual bool isPredefined(void) const;


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

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

  //*************************************************************
  //
  // Overrides from OMEnumeratedType
  //
  //*************************************************************

  virtual OMType* elementType(void) const;

  virtual OMUInt32 elementCount(void) const;

  virtual const wchar_t* elementName(OMUInt32 index) const;

  virtual OMInt64 elementValue(OMUInt32 index) const;
  
  virtual wchar_t* elementNameFromValue(OMInt64 value) const;

  virtual OMInt64 elementValueFromName(const wchar_t* name) const;

  
  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] Type of values in this enumeration
         const ImplAAFTypeDef * pType,

         // @parm [in, size_is(numElems)] array of element values to be represented in this enumerated
    // type
         aafInt64 * pElementValues,

         // @parm [in, size_is(numElems)] array of element names to be represented in this enumerated
    // type
         aafCharacter_constptr *  pElementNames,

         // @parm [in] number of members in pElementValues and pElementNames arrays
         aafUInt32  numElems,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


private:
   OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _ElementType;

  // names of elements in this record; stored as single wchar_t array
  // with embedded nulls
  OMVariableSizeProperty<wchar_t> _ElementNames;

  // array of values for elements.
  OMArrayProperty<aafInt64> _ElementValues;

  aafBool          _isRegistered;
  aafBool          _registrationAttempted;
  aafUInt32        _registeredSize;
  ImplAAFTypeDef     *_cachedBaseType;
  aafBool            _baseTypeIsCached;

  //
  // private methods
  //

  ImplAAFTypeDef* NonRefCountedBaseType (void) const;

  // Lookup a value identifier by name. pName is assumed
  // to be non-null. Returns AAFRESULT_SUCCESS if found.
  // Returns AAFRESULT_INVALID_PARAM if not found.
  AAFRESULT LookupValByName(aafInt64 *pVal, const aafCharacter *pName);


public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefEnum> ImplAAFTypeDefEnumSP;

#endif // ! __ImplAAFTypeDefEnum_h__
