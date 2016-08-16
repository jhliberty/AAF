//@doc
//@class    AAFTimecode | Implementation class for AAFTimecode
#ifndef __ImplAAFTimecode_h__
#define __ImplAAFTimecode_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTimecode.h,v 1.16 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFTimecode : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimecode ();
  virtual ~ImplAAFTimecode ();



  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
	    (// @parm [in] Length Property Value
         aafLength_t  length,

		 // @parm [in] Timecode Value (startFrame, drop, fps)
		 aafTimecode_t  *timecode);

/****/
  //****************
  // GetTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTimecode
		// @parm [out] Timecode (startFrame, drop, fps)
        (aafTimecode_t*  timecode);

/****/
  //****************
  // SetTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTimecode
		// @parm [in] Timecode (startFrame, drop, fps)
        (aafTimecode_t  *timecode);

/****/
  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC (/*[in]*/ aafPosition_t *  pOffset,
	  /*[out]*/ aafTimecode_t *  pTimecode);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset (/*[in]*/ aafTimecode_t *  pTimecode,
	  /*[in]*/ aafRational_t *  pEditRate,
	  /*[out]*/ aafFrameOffset_t *  pOffset);

public:
	// Methods which are internal to the SDK
virtual AAFRESULT OffsetToTimecodeClip(aafPosition_t offset, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos);
private:
	OMFixedSizeProperty<aafPosition_t>	_start;
	OMFixedSizeProperty<aafUInt16>	_FPS;
	OMFixedSizeProperty<aafBool>	_drop;
};

#endif // ! __ImplAAFTimecode_h__

