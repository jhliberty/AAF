//@doc
//@class    AAFTypeDefIndirect | Implementation class for AAFTypeDefIndirect
#ifndef __ImplAAFTypeDefIndirect_h__
#define __ImplAAFTypeDefIndirect_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefIndirect.h,v 1.13 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

#include "OMIndirectType.h"


// Forward declarations:
class ImplAAFPropertyValue;
class ImplAAFDictioanry;
class OMTypeVisitor;



class ImplAAFTypeDefIndirect : public ImplAAFTypeDef, public OMIndirectType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefIndirect ();

protected:
  virtual ~ImplAAFTypeDefIndirect ();

public:

  //****************
  // CreateValueFromActualValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromActualValue
        (// @parm [in] pointer to actual value
         ImplAAFPropertyValue * pActualValue,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);

  //****************
  // CreateValueFromActualData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromActualData
        (// @parm [in] pointer to actual type
         ImplAAFTypeDef * pActualType,

         // @parm [in, size_is(initDataSize)] pointer to buffer containing data to use
         aafMemPtr_t  pInitData,

         // @parm [in] size of data in pInitData
         aafUInt32  initDataSize,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetActualValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualValue
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] value that is read
         ImplAAFPropertyValue ** ppOutPropVal);

  //****************
  // GetActualSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualSize
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pPropVal,
         
         // @parm [out] actual data size
         aafUInt32 * dataSize);

  //****************
  // GetActualType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualType
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pPropVal,
         
         // @parm [out] actual data type
         ImplAAFTypeDef ** ppActualType);

  //****************
  // GetActualData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualData
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pPropVal,

         // @parm [out, size_is(dataSize)] buffer into which actual data should be written
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType (/*[out]*/ ImplAAFTypeDef ** ppRawTypeDef);

public:

  //
  // Methods used internally by the SDK.
  //

  //
  // OMDefinition methods
  //
  virtual const OMUniqueObjectIdentification& identification(void) const;

  virtual const wchar_t* name(void) const;

  virtual bool hasDescription(void) const;

  virtual const wchar_t* description(void) const;

  virtual bool isPredefined(void) const;

  //
  // OMType methods
  //
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

  
  //
  // OMIndirectType methods
  //
  
  virtual OMUniqueObjectIdentification actualTypeId(const OMByte* externalBytes, 
      OMUInt32 externalSize) const;
  
  virtual OMType* actualType(const OMByte* externalBytes, OMUInt32 externalSize) const;
  
  virtual OMByteOrder byteOrder(const OMByte* externalBytes, OMUInt32 externalSize) const;

  virtual void actualSize(const OMByte* externalBytes, OMUInt32 externalSize, 
    OMUInt32& actualSize) const;
    
  virtual void actualData(const OMByte* externalBytes, OMUInt32 externalSize,
    OMByte* actualBytes, OMUInt32& actualSize) const;
  
  virtual OMType* actualType(OMUniqueObjectIdentification id) const;

  virtual void externalData(const OMByte* externalBytes, OMUInt32 externalSize, 
        const OMByte*& externalDataBytes, OMUInt32& externalDataBytesSize) const;
        
  
  //****************
  // Initialize() 
  //   Called when we initialize as one of the "builtin" types.
  //
  AAFRESULT
    Initialize
        (// @parm [in, ref] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in, ref, string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);



  //****************
  // pvtInitialize() 
  //   Called when we initialize as one of the "builtin" types.
  //
  AAFRESULT
    pvtInitialize
        (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName,
       
       // @parm [in] the type definition for kAAFTypeID_AUID.
       ImplAAFTypeDef *pTypeDefAUID,

       // @parm [in] the dictionary for this instance
       ImplAAFDictionary *pDictionary);


  //
  // Utilities to extract information from an "indirect" property.
  // Hopefully this is temporary and this routine will be removed
  // when there is a specific "indirect" OM property.
  //
  static AAFRESULT GetActualPropertySize (
	  const OMProperty & indirectProperty,
	  aafUInt32 * pActualValueSize);

  static AAFRESULT GetActualPropertyValue (
	  const OMProperty & indirectProperty,
    aafDataBuffer_t value,
    aafUInt32 valueSize,
	  aafUInt32 * bytesRead);

  static AAFRESULT SetActualPropertyValue (
	  const OMProperty & indirectProperty,
    ImplAAFTypeDef *pActualType,
    aafDataBuffer_t value,
    aafUInt32 valueSize);

  static AAFRESULT GetActualPropertyType (
	  const OMProperty & property,
	  ImplAAFTypeDef ** pActualType); // REFERENCE COUNTED!

  static AAFRESULT GetActualPropertyTypeID (
	  const OMProperty & property,
	  aafUID_t * pTypeID);



  // Called internally by the dm because there is NO OM property to hide this
  // from the DM.
  aafUInt32 GetIndirectValueOverhead (void) const;

  //
  // ImplAAFTypeDef methods
  //
  virtual aafBool IsFixedSize (void) const;
  virtual OMUInt32 PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual OMUInt32 NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

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

protected:
  // Utility (possibly temporary) that returns true if the given 
  // actual type can be used in an indirect type property.
  bool supportedActualType (ImplAAFTypeDef *pActualType, aafUInt32 level = 0);

  // Find the actual type definition from the dictionary.
  virtual AAFRESULT LookupActualType (aafUID_constref typeID, ImplAAFTypeDef ** ppActualType) const;

	// Utility to extract common information from the given indirect value.
  AAFRESULT GetIndirectValueInfo (
      ImplAAFPropertyValue * pIndirectValue,
			aafUInt32 & indirectValueSize,
			aafMemPtr_t & pIndirectValueDataBits,
      ImplAAFTypeDef ** ppActualType,
			aafUInt32 * actualValueSize = NULL,
      aafUID_t *actualTypeID = NULL);



protected:
  bool _initialized;
  ImplAAFDictionary *_dictionary;
  ImplAAFTypeDef *_typeDefAUID;
  aafUInt32 _internalAUIDSize;
  aafUInt32 _externalAUIDSize;
  aafUInt32 _internalIndirectSize;
  aafUInt32 _externalIndirectSize;
};

#endif // ! __ImplAAFTypeDefIndirect_h__


