//@doc
//@class    AAFGPITrigger | Implementation class for AAFGPITrigger
#ifndef __CAAFGPITrigger_h__
#define __CAAFGPITrigger_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFGPITrigger.h,v 1.7 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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


class CAAFGPITrigger
  : public IAAFGPITrigger,
    public CAAFEvent
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFGPITrigger (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFGPITrigger ();

public:



  //***********************************************************
  //
  // GetActiveState()
  //
  // Sets *pActiveState to AAFTrue it the event is on otherwise
  /// sets it to AAFFalse.
  ///
  /// Succeeds if:
  /// - The pActiveState pointer is valid.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - The pActiveState pointer is NULL.
  //
  STDMETHOD (GetActiveState) (
    // pointer to the result 
    /*[out, retval]*/ aafBoolean_t *  pActiveState);


  //***********************************************************
  //
  // SetActiveState()
  //
  // Set to AAFTrue to turn the trigger on or AAFFalse
  /// to turn the trigger off.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetActiveState) (
    // the active state of the trigger 
    /*[in]*/ aafBoolean_t  ActiveState);


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

#endif // ! __CAAFGPITrigger_h__


