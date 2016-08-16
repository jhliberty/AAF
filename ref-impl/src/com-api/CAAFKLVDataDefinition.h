//@doc
//@class    AAFKLVDataDefinition | Implementation class for AAFKLVDataDefinition
#ifndef __CAAFKLVDataDefinition_h__
#define __CAAFKLVDataDefinition_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFKLVDataDefinition.h,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif









#ifndef __CAAFDefObject_h__
#include "CAAFDefObject.h"
#endif


class CAAFKLVDataDefinition
  : public IAAFKLVDataDefinition,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFKLVDataDefinition (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFKLVDataDefinition ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // Init all fields of a definition object.
  //
  STDMETHOD (Initialize) (
    // AUID for new DefObject
    /*[in, ref]*/ aafUID_constref  id,

    // Name for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pName,

    // Description for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pDescription);

  //***********************************************************
  //
  // AddParentProperty()
  //
  // Add a property definition to the KLV data definition.
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pParentProperty is null.
  //
  STDMETHOD (AddParentProperty) (
    // Property definition to add. 
    /*[in]*/ IAAFPropertyDef * pParentProperty);

  //***********************************************************
  //
  // GetParentProperties()
  //
  // Writes an enumerator for the property definitions into *ppEnum.
  /// A reference is added to the new enumerator before it is returned.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (GetParentProperties) (
    // Property definition enumerator. 
    /*[out]*/ IEnumAAFPropertyDefs ** ppEnum);

  //***********************************************************
  //
  // CountParentProperties()
  //
  // Writes the total number of property definitions into *pNumProperties if,
  /// and only if, the method succeeds.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (CountParentProperties) (
    // Number of property definitions. 
    /*[out]*/ aafUInt32*  pNumProperties);

  //***********************************************************
  //
  // RemoveParentProperty()
  //
  // Removes the given property definition.
  ///
  /// Return codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  ///
  /// AAFRESULT_NO_MORE_OBJECTS
  ///   - The property definition is not in this tagged value definition.
  //
  STDMETHOD (RemoveParentProperty) (
    // Property to remove. 
    /*[in]*/ IAAFPropertyDef * pParentProperty);

  //***********************************************************
  //
  // GetKLVDataType()
  //
  // Writes the data definition object attached to this KLV data
  /// definition into the *ppTypeDef argument if, and only if, the call succeeds.
  /// If none exists, NULL is written to the *ppTypeDef argument.
  ///
  /// The returned data definition object, if it exists, is
  /// AddRef()ed before it is returned.
  ///
  /// Return codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppTypeDef is null.
  //
  STDMETHOD (GetKLVDataType) (
    // Returned data definition object 
    /*[out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // SetKLVDataType()
  //
  // Sets the date type of this KLV data definition to be the given one.
  ///
  /// Return codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTypeDef is null.
  //
  STDMETHOD (SetKLVDataType) (
    // Data definition object 
    /*[in]*/ IAAFTypeDef * pTypeDef);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFKLVDataDefinition_h__


