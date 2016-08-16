//@doc
//@class    AAFComponent | Implementation class for AAFComponent
#ifndef __CAAFComponent_h__
#define __CAAFComponent_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFComponent.h,v 1.8 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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









#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFComponent
  : public IAAFComponent,
    public IAAFComponent2,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFComponent (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFComponent ();

public:

  //***********************************************************
  //
  // SetLength()
  //
  // Sets the length property value on this component object.
  /// 
  /// Succeeds if all of the following are true:
  /// - the optional length property is present for this object.
  ///
  /// This method deals with an optional property, which will only be
  /// present for time-varying media.
  /// 
  /// If this method fails the length property will not be
  /// changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_BAD_PROP
  ///   - the optional length property is not present for this object.
  //
  STDMETHOD (SetLength) (
    // The duration in edit units of this component 
    /*[in]*/ aafLength_constref  length);


  //***********************************************************
  //
  // GetLength()
  //
  // Gets the duration in edit units of this component.
  ///	
  /// Succeeds if all of the following are true:
  /// - the pLength pointer is valid.
  /// - the optional length property is present for this object.
  ///
  /// This method deals with an optional property, which will only be
  /// present for time-varying media.
  /// 
  /// If this method fails nothing will be written to *pLength.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength arg is NULL.
  ///
  /// AAFRESULT_BAD_PROP
  ///   - the optional length property is not present for this object.
  //
  STDMETHOD (GetLength) (
    // Length of this component 
    /*[retval][out]*/ aafLength_t *  pLength);
	

  //***********************************************************
  //
  // SetDataDef()
  //
  // Sets the data definition property AUID on this component.
  /// 
  /// If this method fails the Data Definition property will not be
  /// changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDataDef arg is NULL.
  //
  STDMETHOD (SetDataDef) (
    // DataDef of this object 
    /*[in]*/ IAAFDataDef * pDataDef);


  //***********************************************************
  //
  // GetDataDef()
  //
  // Returns data definition object.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppDatadef pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppDatadef.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppDatadef arg is NULL.
  //
  STDMETHOD (GetDataDef) (
    // DataDef of this object 
    /*[out, retval]*/ IAAFDataDef ** ppDatadef);

  //***********************************************************
  //
  // AppendKLVData()
  //
  // Appends a pre-existing KLV Data object to the specified
  /// component.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pKLV pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - the pData arg is NULL.
  //
  STDMETHOD (AppendKLVData) (
    // KLV object 
    /*[in]*/ IAAFKLVData * pData);


  //***********************************************************
  //
  // CountKLVData()
  //
  // Return total number of KLV data objects attached to this component.
  ///
  /// Succeeds if all of the following are true:
  /// - the pNumData pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pNumComments.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumData arg is NULL.
  //
  STDMETHOD (CountKLVData) (
    // Number  of KLV data objects 
    /*[out]*/ aafUInt32 *  pNumData);


  //***********************************************************
  //
  // GetKLVData()
  //
  // Return the enumeration for all KLV data objects on this component.  The returned
  /// enumerator is AddRef()ed before it is returned.  The enumerator
  /// is implemented as a EnumAAFKLVData.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (GetKLVData) (
    // KLV data objects 
    /*[out]*/ IEnumAAFKLVData ** ppEnum);


  //***********************************************************
  //
  // RemoveKLVData()
  //
  // Removes the given KLV data object from this component.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
  /// - the given KLV data object is present in the component.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pData is null.
  ///
  /// AAFRESULT_OBJECT_NOT_FOUND
  ///   - the given KLV data object is not in this component.
  //
  STDMETHOD (RemoveKLVData) (
    // KLV data object to remove 
    /*[in]*/ IAAFKLVData * pData);


  //***********************************************************
  // METHOD NAME: AppendComment()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | AppendComment |
  // Append an attribute name/value pair to the attribute list.
  ///
  /// Creates a new tagged value, initializes it with the specified comment
  /// name/value pair, and appends it to the comment list.
  ///
  /// Succeeds if:
  ///   - pName and pValue are valid pointers.
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pName or pValue is null.
  // @end
  // 
  STDMETHOD (AppendComment)
   (
    // @parm [in] aafCharacter_constptr | pName | The attribute name.
    aafCharacter_constptr  pName,

    // @parm [in] aafCharacter_constptr | pValue | The attribute value.
    aafCharacter_constptr  pValue
  );

  //***********************************************************
  // METHOD NAME: CountComments()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | CountComments |
  // Return the number of comments contained in this component.
  ///
  /// Succeeds if:
  ///   - pNumComments is a valid pointer
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pNumComments is null.
  // @end
  // 
  STDMETHOD (CountComments)
   (
    // @parm [out] aafUInt32* | pNumComments | Pointer to comment count.
    aafUInt32*  pNumComments
  );

  //***********************************************************
  // METHOD NAME: GetComments()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | GetComments |
  // Return a comment enumerator for this component.
  ///
  /// Creates an enumerator for this component's comments.  The new enumerator is
  /// AddRef()ed before it is returned.
  ///
  /// Succeeds if:
  ///   - pName and pValue are valid pointers.
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pEnum was null.
  // @end
  // 
  STDMETHOD (GetComments)
   (
    // @parm [out] EnumAAFTaggedValues | ppEnum | Pointer to the new enumerator object created by this method.
    IEnumAAFTaggedValues ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: RemoveComment()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | RemoveComment |
  // Remove a component comment (tagged value).
  ///
  /// Succeeds if:
  ///   - pName and pValue are valid pointers.
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pName or pValue is null.
  // @end
  // 
  STDMETHOD (RemoveComment)
   (
    // @parm [in] AAFTaggedValue | pComment | Pointer to the tagged value comment.
    IAAFTaggedValue * pComment
  );

  //***********************************************************
  // METHOD NAME: AppendAttribute()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | AppendAttribute |
  // Append an attribute name/value pair to the attribute list.
  ///
  /// Creates a new tagged value, initializes it with the specified attribute
  /// name/value pair, and appends it to the attribute list.
  ///
  /// Succeeds if:
  ///   - pName and pValue are valid pointers.
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pName or pValue is null.
  // @end
  // 
  STDMETHOD (AppendAttribute)
   (
    // @parm [in] aafCharacter_constptr | pName | The attribute name.
    aafCharacter_constptr  pName,

    // @parm [in] aafCharacter_constptr | pValue | The attribute value.
    aafCharacter_constptr  pValue
  );

  //***********************************************************
  // METHOD NAME: CountAttributes()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | CountAttributes |
  // Return the number of attributes contained in this component.
  ///
  /// Succeeds if:
  ///   - pNumAttributes is a valid pointer
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pNumAttributes is null.
  // @end
  // 
  STDMETHOD (CountAttributes)
   (
    // @parm [out] aafUInt32* | pNumAttributes | Pointer to attribute count.
    aafUInt32*  pNumAttributes
  );

  //***********************************************************
  // METHOD NAME: GetAttributes()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | GetAttributes |
  // Return an attribute enumerator for this component.
  ///
  /// Creates an enumerator for this component's attributes.  The new enumerator is
  /// AddRef()ed before it is returned.
  ///
  /// Succeeds if:
  ///   - pName and pValue are valid pointers.
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pEnum was null.
  // @end
  // 
  STDMETHOD (GetAttributes)
   (
    // @parm [out] EnumAAFTaggedValues | ppEnum | Pointer to the new enumerator object created by this method.
    IEnumAAFTaggedValues ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: RemoveAttribute()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFComponent2 | RemoveAttribute |
  // Remove a component attribute (tagged value).
  ///
  /// Succeeds if:
  ///   - pName and pValue are valid pointers.
  ///
  /// Return codes:
  ///
  ///   AAFRESULT_SUCCESS
  ///
  ///   AAFRESULT_NULL_PARAM
  ///	     - pName or pValue is null.
  // @end
  // 
  STDMETHOD (RemoveAttribute)
   (
    // @parm [in] AAFTaggedValue | pAttribute | Pointer to the tagged value attribute.
    IAAFTaggedValue * pAttribute
  );



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

#endif // ! __CAAFComponent_h__


