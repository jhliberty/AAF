//=---------------------------------------------------------------------=
//
// $Id: ImplAAFHTMLClip.cpp,v 1.15 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFHTMLClip_h__
#include "ImplAAFHTMLClip.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <string.h>
#include <wchar.h>


ImplAAFHTMLClip::ImplAAFHTMLClip () :
  _beginAnchor(PID_HTMLClip_BeginAnchor, L"BeginAnchor"),
  _endAnchor(PID_HTMLClip_EndAnchor, L"EndAnchor")
{
  _persistentProperties.put(_beginAnchor.address());
  _persistentProperties.put(_endAnchor.address());
}


ImplAAFHTMLClip::~ImplAAFHTMLClip ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetBeginAnchor (aafCharacter *  pName,
								 aafInt32  bufSize)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_beginAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat = _beginAnchor.copyToBuffer(pName, bufSize);
	if (! stat)
		return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	
	return (AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetBeginAnchorBufLen (aafUInt32 *  pLen)
{
	if (NULL == pLen)
		return (AAFRESULT_NULL_PARAM);
	
	if(!_beginAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLen = _beginAnchor.size();
	
	return (AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::SetBeginAnchor (const aafCharacter *  pName)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_beginAnchor = pName;
	
	return (AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetEndAnchor (aafCharacter *  pName,
							   aafInt32  bufSize)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_endAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat = _endAnchor.copyToBuffer(pName, bufSize);
	if (! stat)
		return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	
	return (AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetEndAnchorBufLen (aafUInt32 *  pLen)
{
	if (NULL == pLen)
		return (AAFRESULT_NULL_PARAM);
	
	if (!_endAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pLen = _endAnchor.size();
	
	return (AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::SetEndAnchor (const aafCharacter *  pName)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_endAnchor = pName;
	
	return (AAFRESULT_SUCCESS); 
}






