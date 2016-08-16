//@doc
//@class    AAFSegment | Implementation class for AAFSegment
#ifndef __ImplAAFSegment_h__
#define __ImplAAFSegment_h__

#include "OMStorable.h"

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFSegment.h,v 1.20 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

class ImplAAFPulldown;
class ImplAAFTimecode;
class ImplAAFSequence;

class ImplAAFSegment : public ImplAAFComponent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSegment ();
  virtual ~ImplAAFSegment ();



  //****************
  // SegmentOffsetToTC()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC
	    (// @parm [in] Pointer to a Segment Offset to be convert to Timecode
         aafPosition_t *  pOffset,

		 // @parm [out] The converted timecode to be returned
		 aafTimecode_t *  pTimecode);

  //****************
  // SegmentTCToOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset
        (// @parm [in] Pointer to a timecode to be converted to Offset
		 aafTimecode_t *  pTimecode,

		 // @parm [in] The edit rate for the given timecode 
		 aafRational_t *  pEditRate,

		 // @parm [out] Frame Offset to be returned if found
         aafFrameOffset_t *  pOffset);



public:

	virtual AAFRESULT NumRepresentations(aafUInt32 *  pCount);

	virtual AAFRESULT AccumulateLength(aafLength_t *length);

	virtual AAFRESULT FindSubSegment( aafPosition_t offset,
									  aafPosition_t *sequPosPtr,
									  ImplAAFSegment **subseg,
									  aafBool *found);

	virtual AAFRESULT TraverseToClip( aafLength_t length,
									  ImplAAFSegment **sclp,
									  ImplAAFPulldown **pulldownObj,
									  aafInt32 *pulldownPhase,
									  aafLength_t *sclpLen,
									  aafBool *isMask);

	virtual AAFRESULT OffsetToTimecodeClip( aafPosition_t offset, 
											ImplAAFTimecode **result,
											aafPosition_t *tcStartPos);

	virtual AAFRESULT GenerateSequence(ImplAAFSequence **seq);

	virtual AAFRESULT GetComponentType(implCompType_t* pType) {*pType = kSegment; return AAFRESULT_SUCCESS;}
};

#endif // ! __ImplAAFSegment_h__

