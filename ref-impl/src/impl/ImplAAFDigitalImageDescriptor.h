#ifndef __ImplAAFDigitalImageDescriptor_h__
#define __ImplAAFDigitalImageDescriptor_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFDigitalImageDescriptor.h,v 1.19 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include "OMArrayProperty.h"

class ImplAAFDigitalImageDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDigitalImageDescriptor ();

protected:
  virtual ~ImplAAFDigitalImageDescriptor ();

public:
  //****************
  // SetCompression()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCompression
		// @parm [in] Identifies the compression and format of compression information
        (const aafUID_t & compression);

  //****************
  // SetStoredView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStoredView
	    (// @parm [in] Number of pixels in vertical dimension of stored view.
         aafUInt32  StoredHeight,

		 // @parm [in] Number of pixels in horizontal dimension of stored view.
		 aafUInt32  StoredWidth);

  //****************
  // SetSampledView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampledView
        (// @parm [in] Number of pixels in vertical dimension of sampled view.
		 aafUInt32  SampledHeight,

		 // @parm [in] Number of pixels in horizontal dimension of sampled view.
		 aafUInt32  SampledWidth,

		 // @parm [in] Number of pixels from top left corner of sampled view. Optional.
         aafInt32  SampledXOffset,

		 // @parm [in] Number of pixels from top left corner of sampled view. Optional.
		 aafInt32  SampledYOffset);

  //****************
  // SetDisplayView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayView
        (// @parm [in] Number of pixels in vertical dimension of display view. Optional.
		 aafUInt32  DisplayHeight,

		 // @parm [in] Number of pixels in horizontal dimension of display view. Optional.
		 aafUInt32  DisplayWidth,

		 // @parm [in] Number of pixels from the top-left corner of the display view. Optional.
         aafInt32  DisplayXOffset,

		 // @parm [in] Number pixels from the top-left corner of the display view. Optional.
		 aafInt32  DisplayYOffset);

  //****************
  // SetFrameLayout()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFrameLayout
		// @parm [in] layout of the frame
        (aafFrameLayout_t  FrameLayout);

  //****************
  // SetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetVideoLineMap
	    (// @parm [in] The number of elements in the array
         aafUInt32  numberElements,

		 // @parm [in, size_is(size)] Array to hold the Video Line Map information
		 aafInt32 *  pVideoLineMap);

  //****************
  // SetImageAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetImageAspectRatio
		// @parm [in] Ratio between horizontal and vertical size
        (aafRational_t  ImageAspectRatio);

  //****************
  // SetAlphaTransparency()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAlphaTransparency
		// @parm [in] Optional.
        (aafAlphaTransparency_t  AlphaTransparency);

  //****************
  // SetGamma()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetGamma
		// @parm [in] Optional
        (aafUID_t  gamma);

  //****************
  // SetImageAlignmentFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetImageAlignmentFactor
		// @parm [in] Optional.
        (aafInt32  ImageAlignmentFactor);

  //****************
  // GetCompression()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompression
		// @parm [out] Identifies the compression and format of compression information
        (aafUID_t *  pCompression);

  //****************
  // GetStoredView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStoredView
	    (// @parm [out] Number of pixels in vertical dimension of stored view.
         aafUInt32 *  pStoredHeight,

		 // @parm [out] Number of pixels in horizontal dimension of stored view.
		 aafUInt32 *  pStoredWidth);

  //****************
  // GetSampledView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampledView
        (// @parm [out] Number of pixels in vertical dimension of sampled view.
		 aafUInt32 *  pSampledHeight,

		 // @parm [out] Number of pixels in horizontal dimension of sampled view.
		 aafUInt32 *  pSampledWidth,

		 // @parm [out] Number of pixels from top left corner of sampled view. Optional.
         aafInt32 *  pSampledXOffset,

		 // @parm [out] Number of pixels from top left corner of sampled view. Optional.
		 aafInt32 *  pSampledYOffset);

  //****************
  // GetDisplayView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayView
        (// @parm [out] Number of pixels in vertical dimension of display view. Optional.
		 aafUInt32 *  pDisplayHeight,

		 // @parm [out] Number of pixels in horizontal dimension of display view. Optional.
		 aafUInt32 *  pDisplayWidth,

		 // @parm [out] Number of pixels from the top-left corner of the display view. Optional.
         aafInt32 *  pDisplayXOffset,

		 // @parm [out] Number pixels from the top-left corner of the display view. Optional.
		 aafInt32 *  pDisplayYOffset);

  //****************
  // GetFrameLayout()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFrameLayout
		// @parm [out] layout of the frame
        (aafFrameLayout_t *  pFrameLayout);

  //****************
  // GetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoLineMapSize
	    (// @parm [out] The number of elements in the array
         aafUInt32 *  pNumberElements);

  //****************
  // GetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoLineMap
	    (// @parm [in] The number of elements in the array
         aafUInt32  numberElements,

		 // @parm [out, size_is(numberElements)] Array to hold the Video Line Map information
		 aafInt32 *  pVideoLineMap);

  //****************
  // GetImageAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetImageAspectRatio
		// @parm [out] Ratio between horizontal and vertical size
        (aafRational_t *  pImageAspectRatio);

  //****************
  // GetAlphaTransparency()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAlphaTransparency
		// @parm [out] Optional.
        (aafAlphaTransparency_t *  pAlphaTransparency);

  //****************
  // GetGamma()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGamma
		// @parm [out] Optional.
        (aafUID_t *  pGamma);

  //****************
  // GetImageAlignmentFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetImageAlignmentFactor
		// @parm [out] Optional.
        (aafUInt32 *  pImageAlignmentFactor);

  //****************
  // SetTransferCharacteristic()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTransferCharacteristic
        // @parm [in] Optional
        (const aafUID_t & transferCharacteristic);

  //****************
  // GetTransferCharacteristic()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTransferCharacteristic
        // @parm [out] Optional.
        (aafUID_t *  pTransferCharacteristic);

  //****************
  // SetCodingEquations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCodingEquations
        // @parm [in] Optional
        (const aafUID_t & codingEquations);


  //****************
  // GetCodingEquations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodingEquations
        // @parm [out] Optional.
        (aafUID_t *  pCodingEquations);

  //****************
  // SetColorPrimaries()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetColorPrimaries
        // @parm [in] Optional
        (const aafUID_t & colorPrimaries);


  //****************
  // GetColorPrimaries()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetColorPrimaries
        // @parm [out] Optional.
        (aafUID_t *  pColorPrimaries);

  //****************
  // SetFieldStartOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFieldStartOffset
        // @parm [in] Optional.
        (aafUInt32  fieldStartOffset);


  //****************
  // GetFieldStartOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFieldStartOffset
        // @parm [out] Optional.
        (aafUInt32 *  pFieldStartOffset);

  //****************
  // SetFieldEndOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFieldEndOffset
        // @parm [in] Optional.
        (aafUInt32  fieldEndOffset);


  //****************
  // GetFieldEndOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFieldEndOffset
        // @parm [out] Optional.
        (aafUInt32 *  pFieldEndOffset);

  //****************
  // SetFieldDominance()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFieldDominance
        // @parm [in] Optional.
        (aafFieldNumber_t  fieldDominance);


  //****************
  // GetFieldDominance()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFieldDominance
        // @parm [out] Optional.
        (aafFieldNumber_t *  pFieldDominance);

  //****************
  // SetStoredF2Offset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStoredF2Offset
        // @parm [in] Optional.
        (aafInt32  storedF2Offset);


  //****************
  // GetStoredF2Offset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStoredF2Offset
        // @parm [out] Optional.
        (aafInt32 *  pStoredF2Offset);

  //****************
  // SetDisplayF2Offset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayF2Offset
        // @parm [in] Optional.
        (aafInt32  displayF2Offset);


  //****************
  // GetDisplayF2Offset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayF2Offset
        // @parm [out] Optional.
        (aafInt32 *  pDisplayF2Offset);

  //****************
  // SetActiveFormatDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetActiveFormatDescriptor
        // @parm [in] Optional.
        (aafUInt8  activeFormatDescriptor);


  //****************
  // GetActiveFormatDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActiveFormatDescriptor
        // @parm [out] Optional.
        (aafUInt8 *  pActiveFormatDescriptor);

  //****************
  // SetSignalStandard()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSignalStandard
        // @parm [in] Optional
        (aafSignalStandard_t  signalStandard);


  //****************
  // GetSignalStandard()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSignalStandard
        // @parm [out] Optional.
        (aafSignalStandard_t *  pSignalStandard);

protected:
	OMFixedSizeProperty<aafUID_t>				_compression;
	OMFixedSizeProperty<aafUInt32>				_storedHeight;
	OMFixedSizeProperty<aafUInt32>				_storedWidth;
	OMFixedSizeProperty<aafUInt32>				_sampledHeight;
	OMFixedSizeProperty<aafUInt32>				_sampledWidth;
	OMFixedSizeProperty<aafInt32>				_sampledXOffset;
	OMFixedSizeProperty<aafInt32>				_sampledYOffset;
	OMFixedSizeProperty<aafUInt32>				_displayHeight;
	OMFixedSizeProperty<aafUInt32>				_displayWidth;
	OMFixedSizeProperty<aafInt32>				_displayXOffset;
	OMFixedSizeProperty<aafInt32>				_displayYOffset;
	OMFixedSizeProperty<aafFrameLayout_t>		_frameLayout;
	OMArrayProperty<aafInt32>			_videoLineMap;
	OMFixedSizeProperty<aafRational_t>			_imageAspectRatio;
	OMFixedSizeProperty<aafAlphaTransparency_t>	_alphaTransparency;
	OMFixedSizeProperty<aafUID_t>				_transferCharacteristic;
	OMFixedSizeProperty<aafUID_t>				_colorPrimaries;
	OMFixedSizeProperty<aafUID_t>				_codingEquations;
	OMFixedSizeProperty<aafUInt32>				_imageAlignmentFactor;
	OMFixedSizeProperty<aafFieldNumber_t>				_fieldDominance;
	OMFixedSizeProperty<aafUInt32>				_fieldStartOffset;
	OMFixedSizeProperty<aafUInt32>				_fieldEndOffset;
	OMFixedSizeProperty<aafSignalStandard_t>		_signalStandard;
	OMFixedSizeProperty<aafInt32>				_storedF2Offset;
	OMFixedSizeProperty<aafInt32>				_displayF2Offset;
	OMFixedSizeProperty<aafUInt8>				_activeFormatDescriptor;
};

#endif // ! __ImplAAFDigitalImageDescriptor_h__

