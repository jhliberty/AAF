//@doc
//@class    AAFNestedScope | Implementation class for AAFNestedScope
#ifndef __CAAFNestedScope_h__
#define __CAAFNestedScope_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFNestedScope.h,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif






#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFNestedScope
  : public IAAFNestedScope,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFNestedScope (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFNestedScope ();

public:


  //***********************************************************
  //
  // AppendSegment()
  //
  // Append another input segment to the list of source segments.  The
  /// last segment added will be used as the output of the nested
  /// scope, and usually contains operations whose inputs are scope
  /// references.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSegment pointer is valid.
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
  ///   - pSegment is null.
  //
  STDMETHOD (AppendSegment) (
    // Pointer to segment to be added 
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // PrependSegment()
  //
  // Prepend another input segment to the list of source segments.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSegment pointer is valid.
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
  ///   - pSegment is null.
  //
  STDMETHOD (PrependSegment) (
    // Pointer to segment to be added 
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // InsertSegmentAt()
  //
  // Insert an input segment to the list of source segments at the
  /// given index.  Segments already existing at the given and higher
  /// indices will be moved up one index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSegment pointer is valid.
  /// - index is less than or equal to the value returned by
  ///   CountSegments().
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
  ///   - pSegment is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than the value returned by
  ///     CountSegments().
  //
  STDMETHOD (InsertSegmentAt) (
    // index where segment is to be inserted
    /*[in]*/ aafUInt32  index,

    // Pointer to segment to be added
    /*[in]*/ IAAFSegment * pSegment);


  //***********************************************************
  //
  // RemoveSegmentAt()
  //
  // Removes the indexed segment.  Segments already existing at
  /// indices greater than the given index will be moved down by one
  /// index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pSegment pointer is valid.
  /// - index is less than the value returned by CountSegments().
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
  ///   - pSegment is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountSegments().
  //
  STDMETHOD (RemoveSegmentAt) (
    // index of segment to be removed 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // CountSegments()
  //
  // Returns the number of source segments currently contained.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails no value will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountSegments) (
    // number of segments contained 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetSegmentAt()
  //
  // Retrieves the indexed segment.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// - index is less than the value returned by CountSegments().
  /// 
  /// If this method fails nothing will be written to *ppSegment.
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
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountSegments().
  //
  STDMETHOD (GetSegmentAt) (
    // index of segment to retrieve
    /*[in]*/ aafUInt32  index,

    // retrieved segment
    /*[out, retval]*/ IAAFSegment ** ppSegment);


  //***********************************************************
  //
  // GetSegments()
  //
  // Return an enumerator for the ordered list of AAFSegments which
  /// make up the nested scope.
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
  STDMETHOD (GetSegments) (
    // Slots - segment list  enumeration 
    /*[out, retval]*/ IEnumAAFSegments ** ppEnum);

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

#endif // ! __CAAFNestedScope_h__


