//=---------------------------------------------------------------------=
//
// $Id: ImplAAFEssencePlugin.cpp,v 1.14 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFEssencePlugin_h__
#include "ImplAAFEssencePlugin.h"
#endif

#include <string.h>

//const aafClassID_t CLSID_AAFDefaultCodec = { 0xDC089C31, 0x9527, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
//const aafClassID_t CLSID_AAFDefaultStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

ImplAAFEssencePlugin::ImplAAFEssencePlugin ()
{
}


ImplAAFEssencePlugin::~ImplAAFEssencePlugin ()
{}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFEssencePlugin::NumCodecsMatching(
		aafUID_t		/*dataDefinition*/,	//Looking for audio or video codecs
		aafFileRev_t	/*revision*/,		//for which variety of file,
		aafInt32		*result)		// Returns the number of matches,
{
	*result = 1;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFEssencePlugin::GetIndexedCodecInfo(
		aafUID_t		/*dataDefinition*/,	//Looking for audio or video codecs,
		aafFileRev_t	/*revision*/,		//for which variety of file,
		aafInt32		/*index*/,			// Which codec. 1 to number of codecs,
		wchar_t			*pName,			// Codec Name,
		aafInt32		/*bufSize*/,		//length of the buffer to hold Codec Name,
		aafUID_t		* /*codecID*/)		//ID used to access the codec,
{
	memcpy(pName, L"Test Codec", 28);
	return AAFRESULT_SUCCESS;
}
