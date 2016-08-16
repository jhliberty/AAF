//=---------------------------------------------------------------------=
//
// $Id: CAAFCDCIDescriptorHelper.h,v 1.13 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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

//@doc
//@class    CAAFCDCIDescriptorHelper | class that wraps all of the descriptor interfaces
// needed by the CDCI codec. This class should be replaced by a future
// set of wrapper classed to be provided with the AAF Toolkit.

#ifndef __CAAFCDCIDescriptorHelper_h__
#define __CAAFCDCIDescriptorHelper_h__

// Include the interfaces that our plugin needs to support.
#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif


// MediaComposer property extensions to AAFDigitalImageDescriptor and AAFCDCIDescriptor.
const aafUID_t kAAFPropID_DIDResolutionID			= { 0xce2aca4d, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameSampleSize		= { 0xce2aca50, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameIndexByteOrder	= { 0xb57e925d, 0x170d, 0x11d4, { 0xa0, 0x8f, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFirstFrameOffset		= { 0xce2aca4e, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDImageSize				= { 0xce2aca4f, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_CDCIOffsetToFrameIndexes	= { 0x9d15fca3, 0x54c5, 0x11d3, { 0xa0, 0x29, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };



class CAAFCDCIDescriptorHelper
{
public:
	CAAFCDCIDescriptorHelper();
	virtual ~CAAFCDCIDescriptorHelper();
	
	// Initialize all of the internal interfaces required by the CDCI Codec.
	// This method must be called before any other method (except Clear()).
	HRESULT Initialize(IAAFSourceMob *filemob);

	// Release all of the internal interfaces and reset the object.
	// Called by destructor and Initialize method.
	void Clear(void);

	// Comparison operators to check for initialization by the same file mob.
	bool operator==(const CAAFCDCIDescriptorHelper& rhs);
	bool operator!=(const CAAFCDCIDescriptorHelper& rhs);

	//
	// EssenceDescriptor methods:
	//
	STDMETHOD (GetNumLocators) (aafUInt32 *  pCount);
	STDMETHOD (AppendLocator) (IAAFLocator * pLocator);
	STDMETHOD (PrependLocator) (IAAFLocator * pLocator);
	STDMETHOD (EnumAAFAllLocators) (IEnumAAFLocators ** ppEnum);
	//
	// FileDescriptor methods:
	//
	STDMETHOD (SetLength) (aafLength_t  length);
	STDMETHOD (GetLength) (aafLength_t *  pLength);
	STDMETHOD (SetCodecDef) (IAAFCodecDef *codecDef);
	STDMETHOD (GetCodecDef) (IAAFCodecDef **  ppCodecDef);
	STDMETHOD (SetSampleRate) (const aafRational_t & rate);
	STDMETHOD (GetSampleRate) (aafRational_t*  pRate);
	STDMETHOD (SetContainerFormat) (IAAFContainerDef *pFormat);
	STDMETHOD (GetContainerFormat) (IAAFContainerDef **ppFormat);
	// Added methods to go direct to AUID
	STDMETHOD (SetContainerFormat) (const aafUID_t &FormatID);
	STDMETHOD (GetContainerFormat) (aafUID_t *pFormatID);
	//
	// DigitalImageDescriptor methods:
	//
	STDMETHOD (SetCompression) (const aafUID_t & codecID);
	STDMETHOD (GetCompression) (aafUID_t *  pCompression);
	STDMETHOD (SetStoredView) (aafUInt32  StoredHeight, aafUInt32  StoredWidth);
	STDMETHOD (GetStoredView) (aafUInt32 *  pStoredHeight, aafUInt32 *  pStoredWidth);
	STDMETHOD (SetSampledView) (aafUInt32  SampledHeight, aafUInt32  SampledWidth, aafInt32  SampledXOffset, aafInt32  SampledYOffset);
	STDMETHOD (GetSampledView) (aafUInt32 *  pSampledHeight, aafUInt32 *  pSampledWidth, aafInt32 *  pSampledXOffset, aafInt32 *  pSampledYOffset);
	STDMETHOD (SetDisplayView) (aafUInt32  DisplayHeight, aafUInt32  DisplayWidth, aafInt32  DisplayXOffset, aafInt32  DisplayYOffset);
	STDMETHOD (GetDisplayView) (aafUInt32 *  pDisplayHeight, aafUInt32 *  pDisplayWidth, aafInt32 *  pDisplayXOffset, aafInt32 *  pDisplayYOffset);
	STDMETHOD (SetFrameLayout) (aafFrameLayout_t  FrameLayout);
	STDMETHOD (GetFrameLayout) (aafFrameLayout_t *  pFrameLayout);
	STDMETHOD (SetVideoLineMap) (aafUInt32  numberElements, aafInt32 *  pVideoLineMap);
	STDMETHOD (GetVideoLineMap) (aafUInt32  numberElements, aafInt32 *  pVideoLineMap);
	STDMETHOD (GetVideoLineMapSize) (aafUInt32 *  pNumberElements);
	STDMETHOD (SetImageAspectRatio) (aafRational_t  ImageAspectRatio);
	STDMETHOD (GetImageAspectRatio) (aafRational_t *  pImageAspectRatio);
	STDMETHOD (SetAlphaTransparency) (aafAlphaTransparency_t  AlphaTransparency);
	STDMETHOD (GetAlphaTransparency) (aafAlphaTransparency_t *  pAlphaTransparency);
	STDMETHOD (SetGamma) (aafUID_t  Gamma);
	STDMETHOD (GetGamma) (aafUID_t *  pGamma);
	STDMETHOD (SetImageAlignmentFactor) (aafUInt32  ImageAlignmentFactor);
	STDMETHOD (GetImageAlignmentFactor) (aafUInt32 *  pImageAlignmentFactor);
	STDMETHOD (SetFieldDominance) (aafFieldNumber_t  FieldDominance);
	STDMETHOD (GetFieldDominance) (aafFieldNumber_t *  pFieldDominance);
	STDMETHOD (SetFieldStartOffset) (aafUInt32  FieldStartOffset);
	STDMETHOD (GetFieldStartOffset) (aafUInt32 *  pFieldStartOffset);
	STDMETHOD (SetFieldEndOffset) (aafUInt32  FieldEndOffset);
	STDMETHOD (GetFieldEndOffset) (aafUInt32 *  pFieldEndOffset);
 	//
	// CDCIDescriptor methods:
	//
	STDMETHOD (SetComponentWidth) (aafUInt32  ComponentWidth);
	STDMETHOD (GetComponentWidth) (aafUInt32 *  pComponentWidth);
	STDMETHOD (SetHorizontalSubsampling) (aafUInt32  HorizontalSubsampling);
	STDMETHOD (GetHorizontalSubsampling) (aafUInt32 *  pHorizontalSubsampling);
	STDMETHOD (SetVerticalSubsampling) (aafUInt32  VerticalSubsampling);
	STDMETHOD (GetVerticalSubsampling) (aafUInt32 *  pVerticalSubsampling);
	STDMETHOD (SetColorSiting) (aafColorSiting_t  ColorSiting);
	STDMETHOD (GetColorSiting) (aafColorSiting_t *  pColorSiting);
	STDMETHOD (SetBlackReferenceLevel) (aafUInt32  BlackReferenceLevel);
	STDMETHOD (GetBlackReferenceLevel) (aafUInt32 *  pBlackReferenceLevel);
	STDMETHOD (SetWhiteReferenceLevel) (aafUInt32  WhiteReferenceLevel);
	STDMETHOD (GetWhiteReferenceLevel) (aafUInt32 *  pWhiteReferenceLevel);
	STDMETHOD (SetColorRange) (aafUInt32  ColorRange);
	STDMETHOD (GetColorRange) (aafUInt32 *  pColorRange);
	STDMETHOD (SetPaddingBits) (aafInt16  PaddingBits);
	STDMETHOD (GetPaddingBits) ( aafInt16 *  pPaddingBits);


	STDMETHOD (SetResolutionID) ( aafInt32  resolutionID );
	STDMETHOD (SetFrameSampleSize) ( aafInt32 frameSampleSize );
	STDMETHOD (GetFrameSampleSize) ( aafUInt32 * frameSampleSize );

	STDMETHOD (SetOffsetToFrameIndexes) ( aafInt32  offsetToFrameIndexes );
	STDMETHOD (SetFrameIndexByteOrder) ( aafInt16 frameIndexByteOrder );
	STDMETHOD (SetFirstFrameOffset) ( aafInt32 firstFrameOffset );
	STDMETHOD (SetImageSize) ( aafInt32 imageSize );


private:
	IUnknown *_filemob_unk; // used for equality testing.
	IAAFSourceMob *_filemob;
	IAAFEssenceDescriptor *_edes;
	IAAFFileDescriptor *_filedes;
	IAAFDigitalImageDescriptor *_dides;
	IAAFCDCIDescriptor *_cdcides;  // Compressed digital image descriptor
};



#endif // #ifndef __CAAFCDCIDescriptorHelper_h__
