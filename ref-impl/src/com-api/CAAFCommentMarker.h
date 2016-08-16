//@doc
//@class    AAFCommentMarker | Implementation class for AAFCommentMarker
#ifndef __CAAFCommentMarker_h__
#define __CAAFCommentMarker_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFCommentMarker.h,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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









#ifndef __CAAFEvent_h__
#include "CAAFEvent.h"
#endif


class CAAFCommentMarker
  : public IAAFCommentMarker,
    public CAAFEvent
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFCommentMarker (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFCommentMarker ();

public:



  //***********************************************************
  //
  // GetAnnotation()
  //
  // This method will get the annotation for this comment marker and place an
  /// interface for it into the **ppResult argument. 
  ///
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult arg is NULL.
  //
  STDMETHOD (GetAnnotation) (
    // Annotation property value 
    /*[out,retval]*/ IAAFSourceReference ** ppResult);

  //***********************************************************
  //
  // SetAnnotation()
  //
  // This method will set the Annotation for this comment marker.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetAnnotation) (
    // Annotation property value 
    /*[in]*/ IAAFSourceReference * pAnnotation);


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

#endif // ! __CAAFCommentMarker_h__


