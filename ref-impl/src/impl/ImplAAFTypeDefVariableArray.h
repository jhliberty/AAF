#ifndef __ImplAAFTypeDefVariableArray_h__
#define __ImplAAFTypeDefVariableArray_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefVariableArray.h,v 1.40 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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
class OMTypeVisitor;

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif

#include "OMArrayType.h"
#include "OMWeakRefProperty.h"

class ImplEnumAAFPropertyValues;

class ImplAAFTypeDefVariableArray : public ImplAAFTypeDefArray, public OMVaryingArrayType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefVariableArray ();

protected:
  virtual ~ImplAAFTypeDefVariableArray ();

public:

  // override from ImplAAFTypeDefArray
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef) const;


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pPropVal,

         // @parm [out] count of elements in the specified array property value
         aafUInt32 *  pCount) const;


  //****************
  // AppendElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElement
        (// @parm [in] property value corresponding to array to which element is appended
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] value to be appended to this array
         ImplAAFPropertyValue * pMemberPropVal);


  //****************
  // CreateEmptyValue()
  //
  // Implementation inherited from ImplAAFTypeDefArray


  //****************
  // CreateValueFromValues() 
  //
  // Implementation inherited from ImplAAFTypeDefArray


  //****************
  // GetElements() 
  //
  virtual AAFRESULT STDMETHODCALLTYPE
		GetElements (
								ImplAAFPropertyValue *pInPropVal,
								ImplEnumAAFPropertyValues **ppEnum);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);


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
								OMUInt32 externalBytesSize) const;

  virtual OMUInt32 internalSize(void) const;

  virtual void internalize(const OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual void accept(OMTypeVisitor& visitor) const;


  //*************************************************************
  //
  // Overrides from OMArrayType
  //
  //*************************************************************

  virtual OMType* elementType(void) const;

  
  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this array
         const ImplAAFTypeDef *pType,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);

  /**********************************
	   EX - METHODS
	   ***********************************************/
  
  virtual AAFRESULT STDMETHODCALLTYPE
	  PrependElement(
	  // property value corresponding to array to which element is prepended, [in]
	  ImplAAFPropertyValue * pInPropVal, 
	  // value to be prepended to this array,  [in]
	  ImplAAFPropertyValue * pMemberPropVal );
  
  virtual AAFRESULT STDMETHODCALLTYPE
	  RemoveElement(
	  // property value corresponding to array;  [in] 
	  ImplAAFPropertyValue * pInPropVal,
	  // zero-based index into elements in this array type; [in] 
	  aafUInt32  index);
  
  virtual AAFRESULT STDMETHODCALLTYPE  
	  InsertElement(
	  // property value corresponding to array; [in] 
	  ImplAAFPropertyValue * pInPropVal,
	  // zero-based index into elements in this array type;  [in]
	  aafUInt32  index,
	  // value to be inserted into this array; [in]
	  ImplAAFPropertyValue * pMemberPropVal);
  
  /////////////////////////////////////////////////////////
  
protected:
	// overrides from ImplAAFTypeDefArray ...

  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal) const;

  virtual AAFRESULT STDMETHODCALLTYPE
	  ValidateInputParams (ImplAAFPropertyValue ** ppElementValues,
						aafUInt32  numElements);

  virtual bool IsArrayable(ImplAAFTypeDef * pSourceTypeDef) const;

public:

  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void) const;
  virtual OMUInt32 PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual OMUInt32 NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;

  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        (ImplAAFTypeDef ** ppRawTypeDef);

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;

  // Override from OMDefinition
  virtual const OMUniqueObjectIdentification& identification(void) const;
  virtual const wchar_t* name(void) const;
  virtual bool  hasDescription(void) const;
  virtual const wchar_t* description(void) const;
  virtual bool isPredefined(void) const;

  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

private:
   OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _ElementType;

  ImplAAFTypeDefSP BaseType (void) const;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefVariableArray> ImplAAFTypeDefVariableArraySP;

#endif // ! __ImplAAFTypeDefVariableArray_h__
