//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTimecode.cpp,v 1.31 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFTimecode_h__
#include "ImplAAFTimecode.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFBuiltinDefs.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"

ImplAAFTimecode::ImplAAFTimecode ():
_start(	PID_Timecode_Start,	L"Start"),
_FPS(	PID_Timecode_FPS,	L"FPS"),
_drop(	PID_Timecode_Drop,	L"Drop")
{
	_persistentProperties.put(_start.address());
	_persistentProperties.put(_FPS.address());
	_persistentProperties.put(_drop.address());
}

ImplAAFTimecode::~ImplAAFTimecode ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::Initialize (aafLength_t length,
                           aafTimecode_t *timecode)
{
	XPROTECT()
	{
		if (isInitialized ()) 
		{
			return AAFRESULT_ALREADY_INITIALIZED;
		}

		if (timecode == NULL)
		{
			return AAFRESULT_NULL_PARAM;
		}
		if ((timecode->drop != kAAFTcDrop) && (timecode->drop != kAAFTcNonDrop))
		{
			return AAFRESULT_INVALID_TIMECODE;
		}

		ImplAAFDictionarySP pDict;
		CHECK(GetDictionary (&pDict));
		CHECK(SetNewProps(length,
						  pDict->GetBuiltinDefs()->ddTimecode()));
		_start = timecode->startFrame;
		if (timecode->drop == kAAFTcDrop)
		{
		  _drop = kAAFTrue;
		}
		else
		{
		  _drop = kAAFFalse;
		}
		_FPS = timecode->fps;
		
		setInitialized ();
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::GetTimecode (aafTimecode_t *timecode)
{
	if ( !isInitialized ()) 
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if (timecode == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}

	timecode->startFrame = _start;

	if (kAAFTrue == _drop)
	{
		timecode->drop = kAAFTcDrop;
	}
	else
	{
		timecode->drop = kAAFTcNonDrop;
	}
	timecode->fps = _FPS;
	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SetTimecode (aafTimecode_t  *timecode)
{
	if ( !isInitialized ()) 
	{
		return AAFRESULT_NOT_INITIALIZED;
	}
	if (timecode == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if ((timecode->drop != kAAFTcDrop) && (timecode->drop != kAAFTcNonDrop))
	{
		return AAFRESULT_INVALID_TIMECODE;
	}
	_start = timecode->startFrame;
	if (timecode->drop == kAAFTcDrop)
	{
		_drop = kAAFTrue;
	}
	else
	{
		_drop = kAAFFalse;
	}
	_FPS = timecode->fps;

	return(AAFRESULT_SUCCESS);
}


/****/
  // Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SegmentOffsetToTC (/*[in]*/ aafPosition_t *pOffset,
	  /*[out]*/ aafTimecode_t *pTimecode)
{
  	if(pOffset == NULL)
		return(AAFRESULT_NULL_PARAM);
  	if(pTimecode == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
    {
		CHECK(GetTimecode(pTimecode));
		pTimecode->startFrame += *pOffset;
    }

  XEXCEPT
    {
    }
  XEND;

  return(AAFRESULT_SUCCESS);
}

  // Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SegmentTCToOffset (/*[in]*/ aafTimecode_t *pTimecode,
	  /*[in]*/ aafRational_t *pEditRate,
	  /*[out]*/ aafFrameOffset_t *pOffset)
{
	aafTimecode_t	startTC;
	aafPosition_t	oldStart;
	aafLength_t	tcLen;
  
  	if(pTimecode == NULL)
		return(AAFRESULT_NULL_PARAM);
  	if(pEditRate == NULL)
		return(AAFRESULT_NULL_PARAM);
  	if(pOffset == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(GetTimecode(&startTC));
		CHECK(GetLength(&tcLen));
		
		// Assume found at this point, so finish generating result
		oldStart = pTimecode->startFrame - startTC.startFrame;

// Since the new call works only on a segment, the offset is assumed to be in the units
// of the enclosing segment, so no conversion need to be done.  The old code took a mob
// pointer, and so had some context
// CHECK(AAFConvertEditRate(_editRate, oldStart,
//		  							*pEditRate , kRoundFloor, &newStart));
		*pOffset = oldStart;

	   /* check for out of bound timecode */
	   if (pTimecode->startFrame < startTC.startFrame) 
	   {
			  /* out of left bound */
		    RAISE(AAFRESULT_BADSAMPLEOFFSET);
	   }
	   else
	   {
		    if (pTimecode->startFrame > (startTC.startFrame + tcLen))
		    {
					/* out of right bound */
			     RAISE(AAFRESULT_BADSAMPLEOFFSET);
		    }
	   }
	 }
  XEXCEPT
	{
	}
  XEND;
  
  return(AAFRESULT_SUCCESS);

}


aafErr_t ImplAAFTimecode::OffsetToTimecodeClip(aafPosition_t /* offset !!!*/, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos)
{
  	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	*result = this;
	*tcStartPos = 0;
	return(AAFRESULT_SUCCESS);
}


