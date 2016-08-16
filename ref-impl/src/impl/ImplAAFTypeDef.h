//@doc
//@class    AAFTypeDef | Implementation class for AAFTypeDef
#ifndef __ImplAAFTypeDef_h__
#define __ImplAAFTypeDef_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDef.h,v 1.31 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#ifndef OMTYPE_H
#include "OMType.h"
#endif



class ImplAAFPropertyValue;
class OMTypeVisitor;

class ImplAAFTypeDef : public ImplAAFMetaDefinition
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDef ();

protected:
  virtual ~ImplAAFTypeDef ();

public:

  //****************
  // GetTypeCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory
        // @parm [out] Returned type category
        (eAAFTypeCategory_t *  pTid);


  //****************
  // RawAccessType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        // @parm [out] the raw access type definition
        (ImplAAFTypeDef ** ppRawTypeDef);

  //*************************************************************
  //
  // Overrides from OMType
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


  //
  // non-published methods
  //

  // Returns true if property values of this type are of a fixed size.
  virtual aafBool IsFixedSize (void) const;

  // If this->IsFixedSize(), then will return the size of property
  // values of this type.  If not fixed size, will assert().
  virtual OMUInt32 PropValSize (void) const;

  // Returns true if offsets have been registered for this type def.
  virtual aafBool IsRegistered (void) const;

  // Will attempt to identify this as a built-in type; if it is, will
  // register the offsets.
  virtual void AttemptBuiltinRegistration (void);

  // If this->IsRegistered(), then will return the native in-memory
  // size of this type.  If not registered, will assert().
  virtual OMUInt32 NativeSize (void) const;

  // If this->IsRegistered(), then will return the native in-memory
  // size of this type.  If not registered, will PropValSize.
  virtual OMUInt32 ActualSize (void) const;

  // Create a copy of this type definition in the destination
  // dictionary.
  virtual AAFRESULT MergeTo( ImplAAFDictionary* pDestDictionary );

  // Methods to return OMType. Helps to maintain client code once
  // ImplAAFTypeDef inheritance from OMType is removed.
  virtual const OMType* type() const;
  virtual OMType*       type();

  // Allocate an OMProperty that can represent this type.  Implemented
  // by derived classes.
  virtual OMProperty *
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;


  //
  // Pure virtual methods to assist in the composition of types.  Each
  // type must implement these, to assist other types in determining
  // if particular aggregations are legal.
  //

  // Returns true if aggregates (that is, Record types) may be made
  // using this type as an element.
  virtual bool IsAggregatable () const;

  // Returns true if this type may be used as an element of a stream.
  virtual bool IsStreamable () const;

  // Returns true if this type may be used as an element of a
  // fixed-size array.
  virtual bool IsFixedArrayable () const;

  // Returns true if this type may be used as an element of a
  // variably-size array.
  virtual bool IsVariableArrayable () const;

  // Returns true if this type may be used as an element of a string.
  virtual bool IsStringable () const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;
  virtual void onCopy(void* clientContext) const;

  // Overrides from OMDefinition
  virtual const OMUniqueObjectIdentification& identification(void) const;
  virtual const wchar_t* name(void) const;
  virtual bool hasDescription(void) const;
  virtual const wchar_t* description(void) const;
  virtual bool isPredefined(void) const;

protected:
  // Helper function to return the raw type of UInt8Array (if
  // applicable for the derived type)
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtGetUInt8Array8Type
        (ImplAAFTypeDef ** ppRawTypeDef);


  // The function returns the number of NULL-terminated strings
  // found in the specified array.
  static size_t stringArrayStringCount( const wchar_t* buffer,
                                        size_t bufferSize );

  // The function parses an array containing a sequence
  // of NULL-terminated strings returning a list of pointers
  // to the beginning of each string.
  static void getStringArrayStrings( const wchar_t* buffer,
                                     size_t bufferSize,
                                     const wchar_t** strings,
                                     size_t stringCount );
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDef> ImplAAFTypeDefSP;

#endif // ! __ImplAAFTypeDef_h__
