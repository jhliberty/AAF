#ifndef __ImplAAFWAVEDescriptor_h__
#define __ImplAAFWAVEDescriptor_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFWAVEDescriptor.h,v 1.15 2009/06/01 11:47:09 stuart_hc Exp $ $Name: V116 $
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


#include "ImplAAFFileDescriptor.h"

#include "OMVariableSizeProperty.h"

class ImplAAFWAVEDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFWAVEDescriptor ();

protected:
  virtual ~ImplAAFWAVEDescriptor ();

public:
  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // GetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummary
	    (// [in] Size of preallocated buffer
         aafUInt32  size,

		 // [out, size_is(size)] Preallocated buffer to hold the WAVE file information
		 aafDataValue_t  pSummary);

  //****************
  // GetSummaryBufferSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummaryBufferSize
		// [out] required buffer size
        (aafUInt32 *  pSize);

  //****************
  // SetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSummary
	    (// [in] Size of preallocated buffer
         aafUInt32  size,

		 // [in, size_is(size)] buffer containing value
		 aafDataValue_t  pSummary);


private:
	OMVariableSizeProperty<aafUInt8> _summary;

};

#endif // ! __ImplAAFWAVEDescriptor_h__

