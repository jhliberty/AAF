//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRandomFile.cpp,v 1.10 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFGetFileBits.h"
#include "ImplAAFSetFileBits.h"
#include "ImplAAFRandomFile.h"
#include "ImplAAFObjectCreation.h"
#include "OMFile.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFGetFileBits;
extern "C" const aafClassID_t CLSID_AAFSetFileBits;

ImplAAFRandomFile::ImplAAFRandomFile ()
{}


ImplAAFRandomFile::~ImplAAFRandomFile ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomFile::GetFileBits (
      ImplAAFGetFileBits ** ppGetFileBits)
{
  if (! ppGetFileBits)
	return AAFRESULT_NULL_PARAM;

  if ((! IsOpen()) && (! IsClosed()))
	// never been opened
	return AAFRESULT_BADOPEN;

  if (IsOpen())
	// opened, but not closed
	return AAFRESULT_BADCLOSE;
	
  if (!IsClosed())
	// opened, but not closed
	return AAFRESULT_BADCLOSE;
  
  // Note that 'writeable' is from the point of view of the AAF file;
  // the GetFileBits operation (which reads bits) should only succeed
  // for AAF files which are to be written.
  if (!IsWriteable ())
	return AAFRESULT_NOT_WRITEABLE;

  ASSERTU (IsClosed() && IsWriteable());

  OMRawStorage * os = RawStorage ();
  ASSERTU (os);

  ImplAAFGetFileBits * pResult =
	(ImplAAFGetFileBits*)CreateImpl (CLSID_AAFGetFileBits);
  ASSERTU (pResult);
  pResult->Initialize (os);
  ASSERTU (ppGetFileBits);
  *ppGetFileBits = pResult;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomFile::SaveAsFile (
      ImplAAFFile * /*pDestFile*/ )
{
#if 0
  // Copy this file to the destination file.
  // Note, If this == pDestFile, then SaveCopyAs() simply saves this
  // file.
  AAFRESULT hr = SaveCopyAs( pDestFile );
  if ( hr != AAFRESULT_SUCCESS ) {
    return hr;
  }

  return AAFRESULT_SUCCESS;
#else
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomFile::SetFileBits (
      ImplAAFSetFileBits ** ppSetFileBits)
{
  if (! ppSetFileBits)
	return AAFRESULT_NULL_PARAM;

  if (IsOpen())
	return AAFRESULT_BADOPEN;
	
  // We only care that this file has never been opened; note that
  // isClosed() means that this file was opened at one time, which to
  // us is a BADOPEN.
  if (IsClosed())
	return AAFRESULT_BADOPEN;
  
  // Note that 'readable' is from the point of view of the AAF file;
  // the SetFileBits operation (which writes bits) should only succeed
  // for AAF files which are to be read.
  if (! IsReadable ())
	return AAFRESULT_NOT_READABLE;

  ASSERTU (!IsOpen() && !IsClosed() && IsReadable());

  OMRawStorage * os = RawStorage ();
  ASSERTU (os);

  ImplAAFSetFileBits * pResult =
	(ImplAAFSetFileBits*)CreateImpl (CLSID_AAFSetFileBits);
  ASSERTU (pResult);
  pResult->Initialize (os);
  ASSERTU (ppSetFileBits);
  *ppSetFileBits = pResult;
  return AAFRESULT_SUCCESS;
}
