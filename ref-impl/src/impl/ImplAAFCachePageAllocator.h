#ifndef __ImplAAFCachePageAllocator_h__
#define __ImplAAFCachePageAllocator_h__
//=---------------------------------------------------------------------=
//
// $Id: ImplAAFCachePageAllocator.h,v 1.2 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

class OMCachePageAllocator;

class ImplAAFCachePageAllocator : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCachePageAllocator ();

protected:
  virtual ~ImplAAFCachePageAllocator ();

public:
  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE Initialize (aafUInt32  pageCount,
                                                  aafUInt32  pageSize);

  //****************
  // Allocate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE Allocate(aafMemPtr_t *  pPtr);

  //****************
  // Deallocate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE Deallocate(aafMemPtr_t ptr);

private:
  OMCachePageAllocator* _pAllocator;
};

#endif // ! __ImplAAFCachePageAllocator_h__
