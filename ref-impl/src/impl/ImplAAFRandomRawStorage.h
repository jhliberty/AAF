#ifndef __ImplAAFRandomRawStorage_h__
#define __ImplAAFRandomRawStorage_h__
//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRandomRawStorage.h,v 1.4 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFRawStorage_h__
#include "ImplAAFRawStorage.h"
#endif

class ImplAAFRandomRawStorage : public ImplAAFRawStorage
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRandomRawStorage ();

protected:
  virtual ~ImplAAFRandomRawStorage ();

public:

  //****************
  // ReadAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadAt
        (
		 // position in this storage from which data is read
		 aafUInt64  position,

		 // Buffer into which data is read
		 aafMemPtr_t  buf,

		 // Size of buf in bytes
		 aafUInt32  bufSize,

		 // Number of bytes read
		 aafUInt32 *  pNumRead);


  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteAt
        (
		 // position in this storage into which data is written
		 aafUInt64  position,

		 // Buffer from which data is written
		 aafMemConstPtr_t  buf,

		 // Number of bytes requested to write
		 aafUInt32  bufSize,

		 // Number of bytes written
		 aafUInt32 *  pNumWritten);


  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize
        // @parm [out] Number of bytes currently in this stream
        (aafUInt64 *  pSize);


  //****************
  // IsExtendable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsExtendable
        (
		 // set to true if extendable
		 aafBoolean_t *  pResult);


  //****************
  // GetExtent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetExtent
        (
		 // Number of bytes currently allocated for this stream
		 aafUInt64 *  pExtent);


  //****************
  // SetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetExtent
        // @parm [in] Number of bytes capacity requested for this stream
        (aafUInt64  size);
};

#endif // ! __ImplAAFRandomRawStorage_h__
