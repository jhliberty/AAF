//@doc
//@class    AAFSegment | Implementation class for AAFSegment
#ifndef __CAAFSegment_h__
#define __CAAFSegment_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFSegment.h,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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





#ifndef __CAAFComponent_h__
#include "CAAFComponent.h"
#endif


class CAAFSegment
  : public IAAFSegment,
    public CAAFComponent
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSegment (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSegment ();

public:


  //***********************************************************
  //
  // SegmentOffsetToTC()
  //
  // Converts the given Segment offset to timecode.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pOffset pointer is valid.
  /// - the pTimeCode pointer is valid.
  /// 
  /// If this method fails the value of pTimecode is left unchanged.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - any arg is NULL.
  ///
  /// AAFRESULT_TIMECODE_NOT_FOUND
  ///   - the given offset is not available in this segment.
  //
  STDMETHOD (SegmentOffsetToTC) (
    // Pointer to a Segment Offset to be convert to Timecode
    /*[in]*/ aafPosition_t *  pOffset,

    // The converted timecode to be returned
    /*[out]*/ aafTimecode_t *  pTimecode);


  //***********************************************************
  //
  // SegmentTCToOffset()
  //
  // Converts the given Timecode to an Offset.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pTimeCode pointer is valid.
  /// - the pEditRate pointer is valid
  /// - the pFrameOffset pointer is valid.
  /// 
  /// If this method fails the value of pOffset is left unchanged.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - any arg is NULL.
  ///
  /// AAFRESULT_TIMECODE_NOT_FOUND
  ///   - the given timecode is not available in this segment.
  //
  STDMETHOD (SegmentTCToOffset) (
    // Pointer to a timecode to be converted to Offset
    /*[in]*/ aafTimecode_t *  pTimecode,

    // The edit rate for the given timecode 
    /*[in]*/ aafRational_t *  pEditRate,

    // Frame Offset to be returned if found
    /*[out]*/ aafFrameOffset_t *  pOffset);

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

#endif // ! __CAAFSegment_h__


