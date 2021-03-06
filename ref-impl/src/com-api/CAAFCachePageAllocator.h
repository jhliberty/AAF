//@doc
//@class    AAFCachePageAllocator | Implementation class for AAFCachePageAllocator
#ifndef __CAAFCachePageAllocator_h__
#define __CAAFCachePageAllocator_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFCachePageAllocator.h,v 1.3 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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





#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFCachePageAllocator
  : public IAAFCachePageAllocator,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFCachePageAllocator (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFCachePageAllocator ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initialize.
  //
  STDMETHOD (Initialize) (
    // Maximum number of pages to allocate
    /*[in]*/ aafUInt32  pageCount,

    // Size in bytes of each page
    /*[in]*/ aafUInt32  pageSize);

  //***********************************************************
  //
  // Allocate()
  //
  // Allocate a cache page.
  //
  STDMETHOD (Allocate) (
    // Pointer to the newly allocated page 
    /*[out]*/ aafMemPtr_t *  pPage);

  //***********************************************************
  //
  // Deallocate()
  //
  // Dellocate a cache page.
  //
  STDMETHOD (Deallocate) (
    // Pointer to the page to deallocate 
    /*[in]*/ aafMemPtr_t  page);


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

#endif // ! __CAAFCachePageAllocator_h__


