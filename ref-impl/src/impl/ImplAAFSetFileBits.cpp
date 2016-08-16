//=---------------------------------------------------------------------=
//
// $Id: ImplAAFSetFileBits.cpp,v 1.6 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFSetFileBits_h__
#include "ImplAAFSetFileBits.h"
#endif

#include "OMAssertions.h"
#include <string.h>


ImplAAFSetFileBits::ImplAAFSetFileBits ()
  : _rep (0)
{}


ImplAAFSetFileBits::~ImplAAFSetFileBits ()
{}

void ImplAAFSetFileBits::Initialize
        (OMRawStorage * prs)
{
  ASSERTU (prs);
  ASSERTU (!_rep);
  _rep = prs;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSetFileBits::WriteAt (
      aafMemPtr_t  buf,
      aafUInt32  bufSize,
      aafUInt64  position)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if ( ! buf)
	return AAFRESULT_NULL_PARAM;

  aafUInt64 s = _rep->extent ();
  if (s < (position + bufSize))
	{
	  AAFRESULT hr;
	  hr = SetSize (position + bufSize);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	}

  aafUInt32 bytesWritten;
  _rep->writeAt (position, buf, bufSize, bytesWritten);
  if (bytesWritten != bufSize)
	return AAFRESULT_INVALID_PARAM;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFSetFileBits::SetSize (
      aafUInt64 size)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  // BobT Hack!  Some implementations may not understand 64-bit
  // integer sizes, so we'll only allow 32-bit sizes...
  if (size > ((aafUInt32) ~0))
	return AAFRESULT_DATA_SIZE;

  _rep->extend (size);
  aafUInt64 s = _rep->extent ();
  if (s < size)
	return AAFRESULT_DATA_SIZE;
  else
	return AAFRESULT_SUCCESS;
}
