//=---------------------------------------------------------------------=
//
// $Id: ImplAAFPulldown.cpp,v 1.27 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFPulldown_h__
#include "ImplAAFPulldown.h"
#endif

#include "OMAssertions.h"
#include <stdlib.h>
#include <string.h>
#include "aafErr.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFTimecode.h"

ImplAAFPulldown::ImplAAFPulldown ()
: _inputSegment(        PID_Pulldown_InputSegment,          L"InputSegment"),
  _pulldownKind(        PID_Pulldown_PulldownKind,          L"PulldownKind"),
  _pulldownDirection(   PID_Pulldown_PulldownDirection,     L"PulldownDirection"),
  _phaseFrame(			PID_Pulldown_PhaseFrame,	        L"PhaseFrame")
{
	_persistentProperties.put(_inputSegment.address());
	_persistentProperties.put(_pulldownKind.address());
	_persistentProperties.put(_pulldownDirection.address());
	_persistentProperties.put(_phaseFrame.address());
}


ImplAAFPulldown::~ImplAAFPulldown ()
{
	ImplAAFSegment *seg = _inputSegment.clearValue();
	if (seg)
	{
	  seg->ReleaseReference();
	  seg = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::GetInputSegment (
      ImplAAFSegment **ppInputSegment)
{
	if (ppInputSegment == NULL)
		return AAFRESULT_NULL_PARAM;

	*ppInputSegment = _inputSegment;
	// !!!Handle case where inputseg may not
	// exist, return  AAFRESULT_NO_ESSENCE_DESC.

  if (*ppInputSegment)
    (*ppInputSegment)->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::SetInputSegment (
      ImplAAFSegment *pInputSegment)
{
	if (pInputSegment == NULL)
		return AAFRESULT_NULL_PARAM;

	ImplAAFSegment *pOldSeg = _inputSegment;
	if (pOldSeg)
	{
	  if( pOldSeg == pInputSegment )
		return AAFRESULT_SUCCESS;

	  pOldSeg->ReleaseReference();
	  pOldSeg = 0;
	}

	if (pInputSegment->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_inputSegment = pInputSegment;
	
	if (pInputSegment)
		pInputSegment->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::GetPulldownKind (
      aafPulldownKind_t *pPulldownKind)
{
	if (pPulldownKind == NULL)
		return AAFRESULT_NULL_PARAM;

	*pPulldownKind = _pulldownKind;

	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::SetPulldownKind (
      aafPulldownKind_t  pulldownKind)
{
	_pulldownKind = pulldownKind;
	
	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::GetPulldownDirection (
      aafPulldownDir_t *pPulldownDirection)
{
	if (pPulldownDirection == NULL)
		return AAFRESULT_NULL_PARAM;

	*pPulldownDirection = _pulldownDirection;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::SetPulldownDirection (
      aafPulldownDir_t  pulldownDirection)
{
	_pulldownDirection = pulldownDirection;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::GetPhaseFrame (
      aafPhaseFrame_t *pPhaseFrame)
{
	if (pPhaseFrame == NULL)
		return AAFRESULT_NULL_PARAM;

	*pPhaseFrame = _phaseFrame;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::SetPhaseFrame (
      aafPhaseFrame_t  phaseFrame)
{
	_phaseFrame = phaseFrame;
	
	return AAFRESULT_SUCCESS;
}


  // Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::SegmentOffsetToTC (aafPosition_t *pOffset,
      aafTimecode_t *pTimecode)
{
	aafBool	junk;
	return(intSegmentOffsetToTC(*pOffset, pTimecode, &junk));
}

  // Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
    ImplAAFPulldown::SegmentTCToOffset (aafTimecode_t *pTimecode,
      aafRational_t * /*pEditRate*/,
      aafFrameOffset_t *pOffset)
{
	ImplAAFTimecode	*pdwnInput;
	ImplAAFSegment	*seg;
	aafTimecode_t	startTC;
	aafLength_t		length;
	aafPosition_t	offset;

	XPROTECT()
    {
		seg = _inputSegment;
		pdwnInput = dynamic_cast<ImplAAFTimecode*>(seg);
		if(pdwnInput == NULL)
			RAISE(AAFRESULT_TIMECODE_NOT_FOUND);

		CHECK(pdwnInput->GetTimecode(&startTC));
		CHECK(pdwnInput->GetLength(&length));
		if((startTC.fps != pTimecode->fps) || (startTC.drop != pTimecode->drop))
			RAISE(AAFRESULT_TIMECODE_NOT_FOUND);

		offset = pTimecode->startFrame - startTC.startFrame;
		if((offset < 0) || (offset >= length))
			RAISE(AAFRESULT_TIMECODE_NOT_FOUND);

		CHECK(MapOffset(offset, kAAFTrue, pOffset, NULL));
    }
	XEXCEPT
    {
    }
	XEND;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPulldown::MapOffset(aafPosition_t offset,
			   aafBool reverse,
			   aafLength_t *numFrames,
			   aafInt32 *srcPhase)
{
  aafUInt32     maskBits;
  aafBool       drop;
  aafUInt32     phaseOffset = 0;
  aafInt32	        sign, revolutions, ones = 0L, tmpOnes = 0L;
  char          	masksize, maskones, remainder;
  aafPosition_t 	zeroPos;
  aafInt32     		offset32, full = 0L, maskLen;
	aafBool				oneToOne;
	
  XPROTECT()
    {
      /* Get properties off of the pulldown */
		{
			phaseOffset = (aafUInt32)_phaseFrame;
			
			drop = (_pulldownDirection == kAAFTapeToFilmSpeed ? kAAFTrue : kAAFFalse);
			CHECK(aafPvtGetPulldownMask(_pulldownKind, &maskBits, &maskLen, &oneToOne));
		}

	  if(oneToOne)
	  {
	  	if(numFrames)
	  		*numFrames = offset;
	  }
	  else
	  {		
		  if (reverse)
			  drop = (drop ? kAAFFalse : kAAFTrue);

		  zeroPos = 0;
		  if (offset < zeroPos)
			sign = -1;
		  else 
			sign = 1;
		  offset32 = (aafInt32)offset;	// truncate from 64bit to 32bit integer

		  MaskGetBits(maskBits, &maskones);
		  masksize = (char)maskLen;
		  /* If dropping frames */
		  if (drop)
			{
			  if (maskBits)
				{
				  revolutions = abs(offset32) / masksize;
				  remainder = (char)(offset32 % masksize);
				  if(srcPhase != NULL)
				  	*srcPhase = remainder;
				  
				  ones = revolutions * maskones;
				  GetRemFramesDrop(maskBits, remainder, phaseOffset, masksize, &tmpOnes);
				  ones += tmpOnes;
				  ones *= sign;

				  if (numFrames)
					*numFrames = ones;
				}
			} 

		  else /* Doubling frames */
			{
			  if (maskBits)
				{
				  revolutions = abs(offset32) / maskones;
				  remainder = (char)(offset32 % maskones);
				  if(srcPhase != NULL)
				  	*srcPhase = remainder;

				  full = revolutions * masksize;
				  GetRemFramesDouble(maskBits, remainder, phaseOffset, masksize, &tmpOnes);
				  full += tmpOnes;
				  full *= sign;

				  if (numFrames)
					*numFrames = full;
				}
			}
		}
    }

  XEXCEPT
    {
    }
  XEND;

  return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPulldown::GetRemFramesDrop(aafUInt32 maskBits, 
							char ones, 
							aafUInt32 phase,
							aafUInt32 masksize,
							aafInt32 *result)
{
  int remMask, maskBitsLeft;
  aafInt32 ret;
  unsigned char phasePtr;

  remMask = maskBits;
  
  /* Shift mask to start at phase we want */
  for (phasePtr=0; phasePtr<phase; remMask<<=1,phasePtr++)
	;

  for (ret=0, maskBitsLeft = masksize; ones--; remMask<<=1, maskBitsLeft--)
	{
	  if (maskBitsLeft == 0)
	  {
	  	remMask = maskBits;
	  	maskBitsLeft = masksize;
	  }
	  if (remMask<0L)  /* Is high bit a 1 */
		ret++;
	}

  if (result)
	*result = ret;

  return(AAFRESULT_SUCCESS);
 }

AAFRESULT ImplAAFPulldown::GetRemFramesDouble(aafUInt32 maskBits, 
							char ones,
							aafUInt32 phase,
							aafUInt32 masksize,
							aafInt32 *result)
{
  int remMask, maskBitsLeft;
  aafInt32 ret;
  aafUInt32 phasePtr;

  remMask = maskBits;

  /* Shift mask to start at phase we want */
  for (phasePtr=0; phasePtr<phase; remMask<<=1,phasePtr++)
	;
  for (ret=0,phasePtr=phase, maskBitsLeft = masksize; ones; remMask<<=1,
  										phasePtr++, maskBitsLeft--)
	{
	  if (maskBitsLeft == 0)
		{
		  remMask = maskBits;
		  phasePtr = 0;
	  	maskBitsLeft = masksize;
		}
	  ret++;
	  if (remMask<0L)
		ones--;
	}

  if (result)
	*result = ret;
  return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPulldown::MaskGetBits(aafUInt32 maskBits, char *maskones)
{
  aafInt32 tmpMask;
  char		ones;

  /* Iterate over the mask counting all bits and 1-bits */
  for (tmpMask=maskBits,ones=0; tmpMask; tmpMask<<=1)
	{
	  /* If high bit is 1 */
	  if (tmpMask<0) 
		ones++;
	}

  if (maskones)
	*maskones = ones;
  return(AAFRESULT_SUCCESS);
 }

AAFRESULT ImplAAFPulldown::aafPvtGetPulldownMask(
			   aafPulldownKind_t	pulldown,
			   aafUInt32 			*outMask,
			   aafInt32				*maskLen,
			   aafBool	 			*isOneToOne)
{
	switch(pulldown)
	{
		/* !! How do we handle "average of fields" mode (it's still even or odd dominant */
		/* NTSC pullown pattern is:
		 *		AA BB BC CD DD
		 *			Looking at the odd field, we see:
		 *				A B B C D
		 */
		case kAAFTwoThreePD:
			*outMask = 0xD8000000;	/* 3623878656 decimal */
			*maskLen = 5;
			*isOneToOne = kAAFFalse;
			break;
		/* PAL pullown pattern is:
		 *		AA BB CC DD EE FF GG HH II JJ KK LL MM MN NO OP PQ QR RS ST TU UV VW WX XY YY
		 *			Looking at the odd field, we see:
		 *				A B C D E F G H I J K L M M N O P Q R S T U V W X Y
		 */
		case kAAFPALPD:
			*outMask = 0xFFF7FF80;
			*maskLen = 25;
			*isOneToOne = kAAFFalse;
			break;
			
		case kAAFOneToOneNTSC:
		case kAAFOneToOnePAL:
			*isOneToOne = kAAFTrue;
			
		default:
			return(AAFRESULT_PULLDOWN_KIND);
	}
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPulldown::intSegmentOffsetToTC(aafPosition_t offset, aafTimecode_t *tc, aafBool *found)
{
	ImplAAFSegment	*pdwnInput;
	aafLength_t	newStart;
	
  XPROTECT()
    {
		pdwnInput = _inputSegment;
		CHECK(((ImplAAFTimecode *)pdwnInput)->GetTimecode(tc));
		*found = kAAFTrue;
		  CHECK(MapOffset(offset, kAAFFalse, &newStart, NULL));
		  tc->startFrame += newStart;
    }

  XEXCEPT
    {
    }
  XEND;

  return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPulldown::TraverseToClip(aafLength_t length,
					ImplAAFSegment **sclp,
					 ImplAAFPulldown **pulldownObj,
					 aafInt32 *pulldownPhase,
					 aafLength_t *sclpLen,
					 aafBool *isMask)
{
  aafLength_t	tmpLen;
  aafInt32		phase;
  
  XPROTECT()
    {
	  *isMask = kAAFTrue;
	  /* Get the (assumed) source clip out of the mask */
	  *sclp = _inputSegment;
	  (*sclp)->AcquireReference();
//!!!	  if (!(*sclp)->IsTypeOf("SCLP", &aafError));
////		{
//		  RAISE(OM_ERR_NO_MORE_MOBS);
//		}
	  tmpLen = length;
	  CHECK((*sclp)->GetLength(sclpLen));
	  CHECK(MapOffset(tmpLen, kAAFFalse, &length, &phase));
	  if(pulldownObj != NULL)
	  	*pulldownObj = (ImplAAFPulldown *)this;
	  if(pulldownPhase != NULL)
	  	*pulldownPhase = phase;
 	  if (length < *sclpLen)
		{
		  *sclpLen = length;
		}
    }

  XEXCEPT
    {
		if ( *sclp )
			(*sclp)->ReleaseReference();
	}
  XEND;

  return(AAFRESULT_SUCCESS);
}



AAFRESULT ImplAAFPulldown::ChangeContainedReferences(aafMobID_constref from,
													aafMobID_constref to)
{
	ImplAAFSegment	*seg;
	
	seg = _inputSegment;

	if(seg != NULL)
		seg->ChangeContainedReferences(from, to);

	return AAFRESULT_SUCCESS;
}



void ImplAAFPulldown::Accept(AAFComponentVisitor& visitor)
{
	ASSERTU(_inputSegment);

	_inputSegment->Accept(visitor);

	// TODO
	// visitor.VisitPulldown(this);
}
