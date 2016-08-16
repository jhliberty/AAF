//=---------------------------------------------------------------------=
//
// $Id: CAAFCDCICodec.cpp,v 1.41 2012/03/19 17:59:55 philipn Exp $ $Name: V116 $
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
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=
#include "CAAFCDCICodec.h"

#include <stdarg.h>		// for varargs
#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFCompressionDefs.h"
#include "AAFTypeDefUIDs.h"

#include "utf8.h"


// {C995E9A9-4156-11d4-A367-009027DFCA6A}
const CLSID CLSID_AAFCDCICodec = 
{ 0xc995e9a9, 0x4156, 0x11d4, { 0xa3, 0x67, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

// This plugin currently only supports a single definition
const aafUInt32 kSupportedDefinitions = 1;

const wchar_t kDisplayName[] = L"AAF CDCI Codec";
const wchar_t kDescription[] = L"Handles uncompressed YUV & YCbCr and (compressed) IEC 61834 DV family & DV-Based family";
const wchar_t kNilCodecFlavourName[] = L"AAF CDCI Codec (no flavour)";


const aafProductVersion_t kAAFPluginVersion = {1, 0, 0, 1, kAAFVersionBeta};
const aafRational_t		kDefaultRate = { 30000, 1001 };
const aafInt32			kDefaultPixelWidth = 24;
const aafUInt32			kDefaultNumCh = 1;
const aafInt32			kDefaultComponentWidth = 8;

//
// Plugin Descriptor information
//
// {C995E9AA-4156-11d4-A367-009027DFCA6A}
const aafUID_t AVID_CDCI_PLUGIN = 
{ 0xc995e9aa, 0x4156, 0x11d4, { 0xa3, 0x67, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

static const wchar_t *kManufURL = L"http://www.avid.com";
static const wchar_t *kDownloadURL = L"ftp://ftp.avid.com/pub/";
static const wchar_t *kManufName = L"Avid Technology, Inc.";
static const wchar_t *kManufRev = L"Rev 0.1";

// Should be shared by all built-in plugins created by AVID. /* TRR */
const aafUID_t MANUF_AVID_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


const aafUID_t NULL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
const aafRational_t NULL_RATIONAL = {0, 0};
const aafRational_t DEFAULT_ASPECT_RATIO = {4, 3};


// Debugging log function which is optimised away for default builds
inline void plugin_trace(const char *fmt, ...)
{
#ifdef PLUGIN_TRACE
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw HRESULT(r);
}

inline void checkExpression(bool test, AAFRESULT r)
{
  if (!test)
    throw r;
}

inline void checkAssertion(bool test)
{
  if (!test)
    throw HRESULT(AAFRESULT_ASSERTION_VIOLATION);
}

// Legacy property names
const aafCharacter kAAFPropName_DIDResolutionID[] = { 'R','e','s','o','l','u','t','i','o','n','I','D','\0' };
const aafCharacter kAAFPropName_DIDFrameSampleSize[] = { 'F','r','a','m','e','S','a','m','p','l','e','S','i','z','e','\0' };
const aafCharacter kAAFPropName_DIDFrameIndexByteOrder[]	= { 'F','r','a','m','e','I','n','d','e','x','B','y','t','e','O','r','d','e','r','\0' };
const aafCharacter kAAFPropName_DIDFirstFrameOffset[]	= { 'F','i','r','s','t','F','r','a','m','e','O','f','f','s','e','t','\0' };
const aafCharacter kAAFPropName_DIDImageSize[]	= { 'I','m','a','g','e','S','i','z','e','\0' };
const aafCharacter kAAFPropName_CDCIOffsetToFrameIndexes[]	= { 'O','f','f','s','e','t','T','o','F','r','a','m','e','I','n','d','e','x','e','s','\0'};


typedef enum {
	YUV422,
	YUV420,
	YUV411
} VideoSamplingFormat_t;

typedef enum {
	codecDV,
	codecMPEG
} CodecFamily_t;

typedef enum {
	SepFields,
	FullFrame,
	MixedFields
} FrameLayout_t;

typedef struct {
	aafUID_constref flavour;
	aafUID_constref compressionDef;
	const aafCharacter *name;
	VideoSamplingFormat_t vidFormat;
	int sampleRate;
	CodecFamily_t codecFamily;
	FrameLayout_t frameLayout;
	int width;
	int height;
	int storedSize;
	int storedWidth;
	int storedHeight;
	int yOffset;
} FlavourParameters_t;

const FlavourParameters_t FlavourParams[] = {
	{kAAFCodecFlavour_LegacyDV_625_50, kAAFCompressionDef_LegacyDV, L"AAF CDCI Codec (LegacyDV 625 lines 50Hz)", YUV420, 25, codecDV, MixedFields, 720, 576, 144000, 720, 576, 0},
	{kAAFCodecFlavour_LegacyDV_525_60, kAAFCompressionDef_LegacyDV, L"AAF CDCI Codec (LegacyDV 525 lines 60Hz)", YUV411, 30, codecDV, MixedFields, 720, 480, 120000, 720, 480, 0},
	{kAAFCodecFlavour_IEC_DV_625_50, kAAFCompressionDef_IEC_DV_625_50, L"AAF CDCI Codec (IEC DV 625 lines 50Hz)", YUV420, 25, codecDV, SepFields, 720, 576, 144000, 720, 576, 0},
	{kAAFCodecFlavour_IEC_DV_525_60, kAAFCompressionDef_IEC_DV_525_60, L"AAF CDCI Codec (IEC DV 525 lines 60Hz)", YUV411, 30, codecDV, SepFields, 720, 480, 120000, 720, 480, 0},
	{kAAFCodecFlavour_DV_Based_25Mbps_625_50, kAAFCompressionDef_DV_Based_25Mbps_625_50, L"AAF CDCI Codec (DV-Based 25 Mbps 525 lines 60Hz)", YUV411, 25, codecDV, SepFields, 720, 576, 144000, 720, 576, 0},
	{kAAFCodecFlavour_DV_Based_25Mbps_525_60, kAAFCompressionDef_DV_Based_25Mbps_525_60, L"AAF CDCI Codec (DV-Based 25 Mbps 625 lines 50Hz)", YUV411, 30, codecDV, SepFields, 720, 480, 120000, 720, 480, 0},
	{kAAFCodecFlavour_DV_Based_50Mbps_625_50, kAAFCompressionDef_DV_Based_50Mbps_625_50, L"AAF CDCI Codec (DV-Based 50 Mbps 525 lines 60Hz)", YUV422, 25, codecDV, SepFields, 720, 576, 288000, 720, 576, 0},
	{kAAFCodecFlavour_DV_Based_50Mbps_525_60, kAAFCompressionDef_DV_Based_50Mbps_525_60, L"AAF CDCI Codec (DV-Based 50 Mbps 625 lines 50Hz)", YUV422, 30, codecDV, SepFields, 720, 480, 240000, 720, 480, 0},

	{kAAFCodecFlavour_SMPTE_D10_50Mbps_625x50I, kAAFCompressionDef_SMPTE_D10_50Mbps_625x50I, L"AAF CDCI Codec (SMPTE D10 50Mbps 625x50I)", YUV422, 25, codecMPEG, SepFields, 720, 576, 250000, 720, 608, 32},
	{kAAFCodecFlavour_SMPTE_D10_50Mbps_525x5994I, kAAFCompressionDef_SMPTE_D10_50Mbps_525x5994I, L"AAF CDCI Codec (SMPTE D10 50Mbps 525x5994I)", YUV422, 30, codecMPEG, SepFields, 720, 480, 208541, 720, 512, 26},
	{kAAFCodecFlavour_SMPTE_D10_40Mbps_625x50I, kAAFCompressionDef_SMPTE_D10_40Mbps_625x50I, L"AAF CDCI Codec (SMPTE D10 40Mbps 625x50I)", YUV422, 25, codecMPEG, SepFields, 720, 576, 200000, 720, 608, 32},
	{kAAFCodecFlavour_SMPTE_D10_40Mbps_525x5994I, kAAFCompressionDef_SMPTE_D10_40Mbps_525x5994I, L"AAF CDCI Codec (SMPTE D10 40Mbps 525x5994I)", YUV422, 30, codecMPEG, SepFields, 720, 480, 166833, 720, 512, 26},
	{kAAFCodecFlavour_SMPTE_D10_30Mbps_625x50I, kAAFCompressionDef_SMPTE_D10_30Mbps_625x50I, L"AAF CDCI Codec (SMPTE D10 30Mbps 625x50I)", YUV422, 25, codecMPEG, SepFields, 720, 576, 150000, 720, 608, 32},
	{kAAFCodecFlavour_SMPTE_D10_30Mbps_525x5994I, kAAFCompressionDef_SMPTE_D10_30Mbps_525x5994I, L"AAF CDCI Codec (SMPTE D10 30Mbps 525x5994I)", YUV422, 30, codecMPEG, SepFields, 720, 480, 125125, 720, 512, 26},

	{kAAFCodecFlavour_DV_Based_100Mbps_1080x50I, kAAFCompressionDef_DV_Based_100Mbps_1080x50I, L"AAF CDCI Codec (DV-Based 100Mbps 1080x50I)", YUV422, 25, codecDV, SepFields, 1920, 1080, 576000, 1440, 1080, 0},
	{kAAFCodecFlavour_DV_Based_100Mbps_1080x5994I, kAAFCompressionDef_DV_Based_100Mbps_1080x5994I, L"AAF CDCI Codec (DV-Based 100Mbps 1080x5994I)", YUV422, 30, codecDV, SepFields, 1920, 1080, 480000, 1280, 1080, 0},
	{kAAFCodecFlavour_DV_Based_100Mbps_720x50P, kAAFCompressionDef_DV_Based_100Mbps_720x50P, L"AAF CDCI Codec (DV-Based 100Mbps 720x50P)", YUV422, 50, codecDV, FullFrame, 1280, 720, 288000, 960, 720, 0},
	{kAAFCodecFlavour_DV_Based_100Mbps_720x5994P, kAAFCompressionDef_DV_Based_100Mbps_720x5994P, L"AAF CDCI Codec (DV-Based 100Mbps 720x5994P)", YUV422, 60, codecDV, FullFrame, 1280, 720, 240000, 960, 720, 0},
};

static const FlavourParameters_t* lookupFlavourParams(aafUID_constref flavour)
{
	for (size_t i = 0; i < sizeof(FlavourParams) / sizeof(FlavourParams[0]); i++) {
		if (FlavourParams[i].flavour == flavour)
			return &FlavourParams[i];
	}
	// Return NULL for no match 
	return NULL;
}

static const FlavourParameters_t* lookupFlavourParamsByCompression(const aafUID_t &compId, aafUInt32 height)
{
	// kAAFCompressionDef_LegacyDV is a special case since both PAL and NTSC
	// versions have the same CompressionDef, so use height to distinguish them.
	if (EqualAUID(&compId, &kAAFCompressionDef_LegacyDV))
	{
		if (height == 240)			// height of one field for 720x480 picture
			return lookupFlavourParams(kAAFCodecFlavour_LegacyDV_525_60);
		else
			return lookupFlavourParams(kAAFCodecFlavour_LegacyDV_625_50);
	}

	for (size_t i = 0; i < sizeof(FlavourParams) / sizeof(FlavourParams[0]); i++) {
		if (EqualAUID(&compId, &FlavourParams[i].compressionDef))
			return &FlavourParams[i];
	}

	return NULL;
}

static int vidFormat_horizontal(VideoSamplingFormat_t format)
{
	if (format == YUV411)
		return 4;
	return 2;		// YUV422 and YUV420
}

static int vidFormat_vertical(VideoSamplingFormat_t format)
{
	if (format == YUV420)
		return 2;
	return 1;		// YUV422 and YUV411
}

static int apiFrameSizeFromParams(const FlavourParameters_t* params)
{
	// IMX formats store extra 32 lines but the API uses displayed size
	if (params->codecFamily == codecMPEG)
		return params->width * params->height * 2;

	// All other formats have API size same as stored size
	if (params->vidFormat == YUV422)
		return params->storedWidth * params->storedHeight * 2;
	else
		return params->storedWidth * params->storedHeight * 3/2;
}

#if defined(USE_FFMPEG)
static int bitrate_for_flavour(aafUID_constref flavour)
{
	// Use fixed values for IMX formats since the NTSC IMX rates
	// cannot be reliably calculated from sampleRate * storedSize
	if (flavour == kAAFCodecFlavour_SMPTE_D10_50Mbps_625x50I || flavour == kAAFCodecFlavour_SMPTE_D10_50Mbps_525x5994I)
		return 50000000;
	if (flavour == kAAFCodecFlavour_SMPTE_D10_40Mbps_625x50I || flavour == kAAFCodecFlavour_SMPTE_D10_40Mbps_525x5994I)
		return 40000000;
	if (flavour == kAAFCodecFlavour_SMPTE_D10_30Mbps_625x50I || flavour == kAAFCodecFlavour_SMPTE_D10_30Mbps_525x5994I)
		return 30000000;

	// Use (sampleRate * storedSize) calculation for all others
	const FlavourParameters_t *p = lookupFlavourParams(flavour);
	return p->storedSize * p->sampleRate * 8;
}
#endif

inline bool operator == (const aafRational_t a, const aafRational_t b)
{
	return memcmp(&a, &b, sizeof(a)) == 0;
}

inline bool IsDV(const aafUID_t &compId)
{
	for (size_t i = 0; i < sizeof(FlavourParams) / sizeof(FlavourParams[0]); i++) {
		// Find matching compressionDef (if any)
		if (EqualAUID(&compId, &FlavourParams[i].compressionDef)) {
			// Is this compressionDef a DV one?
			if (FlavourParams[i].codecFamily == codecDV)
				return true;
		}
	}

	return false;
}

inline bool IsSupportedCompressionID(const aafUID_t &compId)
{
	// Null or uncompressed?
	if (EqualAUID(&compId, &NULL_UID) || EqualAUID(&compId, &AAF_CMPR_AUNC422))
		return true;

	// A supported compressionDef?
	for (size_t i = 0; i < sizeof(FlavourParams) / sizeof(FlavourParams[0]); i++) {
		if (EqualAUID(&compId, &FlavourParams[i].compressionDef))
			return true;
	}

	return false;
}


// Constructor

CAAFCDCICodec::CAAFCDCICodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{
	plugin_trace("CAAFCDCICodec::CAAFCDCICodec() constructor\n");

	_nativeByteOrder = GetNativeByteOrder();
	_access = NULL;
	_stream = NULL;
	_openMode = kAAFMediaOpenReadOnly;
	_flavour = kAAFNilCodecFlavour;
	_length = 0;
	_sampleRate = NULL_RATIONAL;
	_containerFormat = NULL_UID;
	_compression = NULL_UID;
	_storedHeight = 0;
	_storedWidth = 0;
	_sampledHeight = 0;
	_sampledWidth = 0;
	_sampledXOffset = 0;
	_sampledYOffset = 0;
	_displayHeight = 0;
	_displayWidth = 0;
	_displayXOffset = 0;
	_displayYOffset = 0;
	_frameLayout = kAAFFullFrame;
	_videoLineMapSize = 0;
	_videoLineMap[0] = _videoLineMap[1] = 0;
	_imageAspectRatio = NULL_RATIONAL;
	_alphaTransparency = kAAFMinValueTransparent;
	_gamma = NULL_UID;
	_imageAlignmentFactor = 0;
	_componentWidth = kDefaultComponentWidth;
	_horizontalSubsampling = 1;
	_verticalSubsampling = 1;
	_colorSiting = kAAFCoSiting;
	_blackReferenceLevel = 0;
	_whiteReferenceLevel = 255;
	_colorRange = 0;
	_paddingBits = 0;
	_fieldDominance = kAAFNoDominant;
	_fieldStartOffset = 0;
	_fieldEndOffset = 0;
	_imageHeight = 0;
	_imageWidth = 0;
	_apiFrameSampleSize = 0;
	_storedFrameSampleSize = 0;
	_pixelFormat = kAAFColorSpaceYUV;
	_bitsPerPixelAvg = kDefaultPixelWidth;
	_numberOfSamples = 0;
	_padBytesPerRow = 0;
}

// Desctructor

CAAFCDCICodec::~CAAFCDCICodec ()
{
	plugin_trace("CAAFCDCICodec::~CAAFCDCICodec()\n");
  // Do NOT release the _access interface since this object
  // contains the reference to this codec instance! We need
  // avoid the dreaded reference counting cycle of death!
	if(_stream != NULL)
	{
		_stream->Release();
		_stream = NULL;
	}
}


// Only save a new reference.
void CAAFCDCICodec::SetEssenceStream(IAAFEssenceStream *stream)
{
	plugin_trace("CAAFCDCICodec::SetEssenceStream()\n");
	if (_stream != stream)
	{
		if (NULL != _stream)
			_stream->Release();
		_stream = stream;
		if (NULL != stream)
			stream->AddRef();
	}
}

void CAAFCDCICodec::SetCompressionEnabled(aafCompressEnable_t compEnable)
{
	_compressEnable = (compEnable == kAAFCompressionEnable) ? kAAFCompressionEnable : kAAFCompressionDisable;
}

void CAAFCDCICodec::SetNumberOfSamples(const aafLength_t& numberOfSamples)
{
	_numberOfSamples = numberOfSamples;
}




HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountDefinitions (aafUInt32 *pDefCount)
{
	plugin_trace("CAAFCDCICodec::CountDefinitions()\n");
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedDefinitionID (aafUInt32 /* index */, aafUID_t *uid)
{
	plugin_trace("CAAFCDCICodec::GetIndexedDefinitionID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = kAAFCodecCDCI;		// UID of the CDCI codec definition

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetPluginDescriptorID (aafUID_t *uid)
{
	plugin_trace("CAAFCDCICodec::GetPluginDescriptorID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AVID_CDCI_PLUGIN;		// UID of the PluginDescriptor

	return AAFRESULT_SUCCESS;
}


// The method creates some descriptor properties to handle legacy CDCI 
// variations and legacy DV variations.
HRESULT STDMETHODCALLTYPE CAAFCDCICodec::CreateLegacyPropDefs( 
    IAAFDictionary	*p_dict )
{
	plugin_trace("CAAFCDCICodec::CreateLegacyPropDefs()\n");
    HRESULT		hr = S_OK;
    IAAFClassDef	*p_did_classdef = NULL;
    IAAFClassDef	*p_cdci_classdef = NULL;
    IAAFTypeDef		*p_typedef_int64 = NULL;
    IAAFTypeDef		*p_typedef_int32 = NULL;
    IAAFTypeDef		*p_typedef_int16 = NULL;
    IAAFPropertyDef	*p_propdef = NULL;


    try
    {
	// Get CDCI descriptor class definition,
	// DigitalImage descriptor class definition and
	// and integer type definition.
	checkResult( p_dict->LookupClassDef( kAAFClassID_DigitalImageDescriptor, 
	    &p_did_classdef ) );
	checkResult( p_dict->LookupClassDef( kAAFClassID_CDCIDescriptor, 
	    &p_cdci_classdef ) );
	checkResult( p_dict->LookupTypeDef( kAAFTypeID_Int64, 
	    &p_typedef_int64 ) );
	checkResult( p_dict->LookupTypeDef( kAAFTypeID_Int32, 
	    &p_typedef_int32 ) );
	checkResult( p_dict->LookupTypeDef( kAAFTypeID_Int16, 
	    &p_typedef_int16 ) );


	// Register legacy property definitions
	if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDResolutionID, &p_propdef) != AAFRESULT_SUCCESS)
	{
		checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
			kAAFPropID_DIDResolutionID, kAAFPropName_DIDResolutionID, 
			p_typedef_int32, &p_propdef ) );
	}
	p_propdef->Release();
	p_propdef = NULL;

	if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFrameSampleSize, &p_propdef) != AAFRESULT_SUCCESS)
	{
		checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
			kAAFPropID_DIDFrameSampleSize, kAAFPropName_DIDFrameSampleSize, 
			p_typedef_int32, &p_propdef ) );
	}
	p_propdef->Release();
	p_propdef = NULL;

	if (p_cdci_classdef->LookupPropertyDef(kAAFPropID_CDCIOffsetToFrameIndexes, &p_propdef) != AAFRESULT_SUCCESS)
	{
		checkResult( p_cdci_classdef->RegisterOptionalPropertyDef( 
			kAAFPropID_CDCIOffsetToFrameIndexes, 
			kAAFPropName_CDCIOffsetToFrameIndexes, 
			p_typedef_int32, &p_propdef ) );
	}
	p_propdef->Release();
	p_propdef = NULL;

	if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFrameIndexByteOrder, &p_propdef) != AAFRESULT_SUCCESS)
	{
		checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
			kAAFPropID_DIDFrameIndexByteOrder, kAAFPropName_DIDFrameIndexByteOrder, 
			p_typedef_int16, &p_propdef ) );
	}
	p_propdef->Release();
	p_propdef = NULL;

	if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFirstFrameOffset, &p_propdef) != AAFRESULT_SUCCESS)
	{
		checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
			kAAFPropID_DIDFirstFrameOffset, kAAFPropName_DIDFirstFrameOffset, 
			p_typedef_int32, &p_propdef ) );
	}
	p_propdef->Release();
	p_propdef = NULL;

	if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDImageSize, &p_propdef) != AAFRESULT_SUCCESS)
	{
		checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
			kAAFPropID_DIDImageSize, kAAFPropName_DIDImageSize, 
			p_typedef_int32, &p_propdef ) );
	}
	p_propdef->Release();
	p_propdef = NULL;

    }
    catch( HRESULT& rhr )
    {
	hr = rhr; // return thrown error code.
    }

    if( p_cdci_classdef )
	p_cdci_classdef->Release();
    if( p_did_classdef )
	p_did_classdef->Release();
    if( p_typedef_int64 )
	p_typedef_int64->Release();
    if( p_typedef_int32 )
	p_typedef_int32->Release();
    if( p_typedef_int16 )
	p_typedef_int16->Release();
    if( p_propdef )
	p_propdef->Release();


    return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	plugin_trace("CAAFCDCICodec::GetIndexedDefinitionObject()\n");
	HRESULT hr = S_OK;
	IAAFCodecDef	*codecDef = NULL;
	IAAFDataDef		*pDefPicture = NULL, *pDefLegacyPicture = NULL;
	IAAFClassDef	*fileClass = NULL;
	IAAFDefObject	*obj = NULL;
	IAAFClassDef    *pcd = 0;
	aafUID_t		uid;
	
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;

	if (0 > kSupportedDefinitions || kSupportedDefinitions <= index)
		return AAFRESULT_BADINDEX;

	try
	{
		//!!!Later, add in dataDefs supported & filedescriptor class

		// Creates descriptor properties to handle legacy CDCI.
		checkResult( CreateLegacyPropDefs( dict ) );

		// Create the Codec Definition:
		checkResult(dict->LookupClassDef(AUID_AAFCodecDef, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFCodecDef, 
										(IUnknown **)&codecDef));
		pcd->Release();
		pcd = 0;
		
		// Support "Picture" type of data definition.
		checkResult(dict->LookupDataDef(kAAFDataDef_Picture, &pDefPicture));
		checkResult(codecDef->AddEssenceKind(pDefPicture));
		checkResult(dict->LookupDataDef(kAAFDataDef_LegacyPicture, &pDefLegacyPicture));
		checkResult(codecDef->AddEssenceKind(pDefLegacyPicture));

		// Initialize the standard Definition properties.
		checkResult(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		uid = kAAFCodecCDCI;
		checkResult(codecDef->Initialize(uid, 
		                      const_cast<wchar_t *>(kDisplayName),
		                      const_cast<wchar_t *>(kDescription)));

	  	checkResult(dict->LookupClassDef(AUID_AAFCDCIDescriptor, &fileClass));
		checkResult(codecDef->SetFileDescriptorClass (fileClass));
		fileClass->Release ();
		fileClass = 0;

		// Cleanup
		codecDef->Release();
		codecDef = NULL;


		// Return a reference to our initialized definition object.
		*def = obj; // reference count already incremented, we do not need to call AddRef().
		obj = NULL;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup on error.
	if (NULL != pDefLegacyPicture)
	  {
		pDefLegacyPicture->Release();
		pDefLegacyPicture = 0;
	  }
	if (NULL != pDefPicture)
	  {
		pDefPicture->Release();
		pDefPicture = 0;
	  }
	if (NULL != codecDef)
	  {
		codecDef->Release();
		codecDef = 0;
	  }
	if (NULL != obj)
	  {
		obj->Release();
		obj = 0;
	  }
	if (pcd)
	  {
		pcd->Release();
		pcd = 0;
	  }
	if (fileClass)
	  {
		fileClass->Release ();
		fileClass = 0;
	  }

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
	plugin_trace("CAAFCDCICodec::CreateDescriptor()\n");
	HRESULT hr = S_OK;
	IAAFPluginDef	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	IAAFClassDef            *pcd = 0;
	
	if ((NULL == dict) || (NULL == descPtr))
		return AAFRESULT_NULL_PARAM;
	
	// Initialize return parameter.
	*descPtr = NULL;

	try
	{
	    checkResult(dict->LookupClassDef(AUID_AAFPluginDef, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFPluginDef, 
										(IUnknown **)&desc));
		pcd->Release ();
		pcd = 0;

		checkResult(desc->Initialize(AVID_CDCI_PLUGIN,
		                       const_cast<wchar_t *>(kDisplayName),
		                       const_cast<wchar_t *>(kDescription)));
		checkResult(desc->SetCategoryClass(AUID_AAFDefObject));
		checkResult(desc->SetPluginVersionString(kManufRev));
		checkResult(desc->SetManufacturerID(MANUF_AVID_PLUGINS));
		checkResult(desc->SetPluginManufacturerName(kManufName));
		checkResult(desc->SetIsSoftwareOnly(kAAFTrue));
		checkResult(desc->SetIsAccelerated(kAAFFalse));
		checkResult(desc->SetSupportsAuthentication(kAAFFalse));

		// Create the network locator for the Manufacturer's web site: 
		checkResult(dict->LookupClassDef(AUID_AAFNetworkLocator, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFLocator, 
										(IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kManufURL));
		checkResult(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		checkResult(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		
		// Create a Network locator to point to our default download site.
		checkResult(pcd->CreateInstance(IID_IAAFLocator, 
										(IUnknown **)&pLoc));
		pcd->Release ();
		pcd = 0;
		checkResult(pLoc->SetPath (kDownloadURL));
		checkResult(desc->AppendLocator(pLoc));
	
		pLoc->Release();
		pLoc = NULL;

		*descPtr = desc; // reference count already incremented, we do not need to call AddRef()
		desc = NULL;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup...
	if (NULL != desc)
	  {
		desc->Release();
		desc = 0;
	  }
	if (NULL != pLoc)
	  {
		pLoc->Release();
		pLoc = 0;
	  }
	if (NULL != pNetLoc)
	  {
		pNetLoc->Release();
		pNetLoc = 0;
	  }
	if (pcd)
	  {
		pcd->Release ();
		pcd = 0;
	  }

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	plugin_trace("CAAFCDCICodec::SetEssenceAccess()\n");
	if(access == NULL)
		return AAFRESULT_NULL_PARAM;

	_access = access;

  // NOTE: We do NOT reference count this interface or ANY interface
  // obtained from this interface with QueryInterface. The access
  // interface is actually the container for this codec and controls
  // the lifetime of this instance. If we were to acquire another
  // reference to this container then this object and its container will NOT
  // be deleted from memory.

  return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountFlavours(aafUInt32 *pCount)
{
	plugin_trace("CAAFCDCICodec::CountFlavours()\n");
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;

	int num_flavours = sizeof(FlavourParams) / sizeof(FlavourParams[0]);
	*pCount = num_flavours;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedFlavourID (aafUInt32 index,
        aafUID_t *  pFlavour)
{
	plugin_trace("CAAFCDCICodec::GetIndexedFlavourID()\n");
	if (pFlavour == NULL)
		return AAFRESULT_NULL_PARAM;

	aafUInt32 num_flavours = sizeof(FlavourParams) / sizeof(FlavourParams[0]);

	if (index >= num_flavours)
		return AAFRESULT_BADINDEX;

	*pFlavour = FlavourParams[index].flavour;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	plugin_trace("CAAFCDCICodec::CountDataDefinitions()\n");
	if (NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedDataDefinition (aafUInt32  /* index */,
        aafUID_t * pFlavour)
{
	plugin_trace("CAAFCDCICodec::GetIndexedDataDefinition()\n");
	if (NULL == pFlavour)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetMaxCodecDisplayNameLength (
        aafUInt32  *bufSize)
{
	plugin_trace("CAAFCDCICodec::GetMaxCodecDisplayNameLength()\n");
	if (NULL == bufSize)
		return AAFRESULT_NULL_PARAM;

	// Loop over all flavours recording the largest name's buffer size
	aafUInt32 max = (wcsu8slen(kNilCodecFlavourName) + 1) * sizeof(aafCharacter);
	for (size_t i = 0; i < sizeof(FlavourParams) / sizeof(FlavourParams[0]); i++)
	{
		// compute length of name in bytes including terminating NUL
		aafUInt32 len = (wcsu8slen(FlavourParams[i].name) + 1) *
									sizeof(aafCharacter);
		if (len > max)
			max = len;
	}
	*bufSize = max;

	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetCodecDisplayName (aafUID_constref flavour,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
	plugin_trace("CAAFCDCICodec::GetCodecDisplayName()\n");
	if (NULL == pName)
		return AAFRESULT_NULL_PARAM;
	if (0 >= bufSize)
		return AAFRESULT_INVALID_PARAM;

	const aafCharacter *name;
	if (flavour == kAAFNilCodecFlavour) {
		name = kNilCodecFlavourName;
	}
	else {
		const FlavourParameters_t *p = lookupFlavourParams(flavour);
		if (p == NULL)
			return AAFRESULT_NOT_IMPLEMENTED;
		name = p->name;
	}

	// compute length of name in bytes including terminating NUL
	aafUInt32 len = (wcsu8slen(name) + 1) * sizeof(aafCharacter);
	if (len > bufSize)
		len = bufSize;

	memcpy(pName, name, len);
	return AAFRESULT_SUCCESS;
}


// SetFlavour is used as an alternative to PutEssenceFormat.
// All internal settings and descriptor properties are set using
// a flavour instead of requiring many format specifiers to be used.
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::SetFlavour (aafUID_constref flavour)
{
	plugin_trace("CAAFCDCICodec::SetFlavour()\n");

	// Set current flavour
	_flavour = flavour;

	if (flavour == kAAFNilCodecFlavour)
		return AAFRESULT_SUCCESS;

	const FlavourParameters_t *p_params = lookupFlavourParams(flavour);
	if (p_params == NULL)
		return AAFRESULT_NOT_IMPLEMENTED;

	// Set properties based on flavour params
	_compression = p_params->compressionDef;
	_frameLayout = (p_params->frameLayout == SepFields) ? kAAFSeparateFields :
					(p_params->frameLayout == FullFrame ? kAAFFullFrame : kAAFMixedFields);
	_imageWidth = _storedWidth = p_params->storedWidth;
	_displayWidth = p_params->width;
	if (p_params->frameLayout == FullFrame) {
		_imageHeight = _storedHeight = p_params->storedHeight;
		_displayHeight = p_params->height;
	}
	else {
		_imageHeight = _storedHeight = p_params->storedHeight / 2;
		_displayHeight = p_params->height / 2;
	}
	if (p_params->codecFamily == codecMPEG) {
		_displayYOffset = 16;
		if (p_params->height == 480) {
			_displayHeight = 243;		// Observed in Avid IMX files
			_displayYOffset = 13;
		}
	}
	_horizontalSubsampling = vidFormat_horizontal(p_params->vidFormat);
	_verticalSubsampling = vidFormat_vertical(p_params->vidFormat);

	// The following are non-Property variables needed to be setup
	if (_compressEnable == kAAFCompressionEnable)
	{
		_apiFrameSampleSize = apiFrameSizeFromParams(p_params);
	}
	else
	{
		// Just wrapping, no compressing
		_apiFrameSampleSize = p_params->storedSize;
	}

	_storedFrameSampleSize = p_params->storedSize;

	SetCodecState();

	return AAFRESULT_SUCCESS;
}

// IsCompressionSupported is used by the toolkit to test whether this plugin
// is capable of opening and reading essence for the given compression ID.
HRESULT STDMETHODCALLTYPE
	CAAFCDCICodec::IsCompressionSupported(
		aafUID_constref compression,
		aafBool* pIsSupported)
{
	plugin_trace("CAAFCDCICodec::IsCompressionSupported()\n");

	if (NULL == pIsSupported)
		return AAFRESULT_NULL_PARAM;

	if (IsSupportedCompressionID(compression))
		*pIsSupported = kAAFTrue;
	else
		*pIsSupported = kAAFFalse;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountChannels (IAAFSourceMob *fileMob,
        aafUID_constref essenceKind,
        IAAFEssenceStream *stream,
        aafUInt16 *  pNumChannels)
{
	plugin_trace("CAAFCDCICodec::CountChannels()\n");
	if (NULL == fileMob || NULL == stream || NULL == pNumChannels)
		return AAFRESULT_NULL_PARAM;

	if(EqualAUID(&essenceKind, &kAAFDataDef_Picture)
		|| EqualAUID(&essenceKind, &kAAFDataDef_LegacyPicture))
	{
		*pNumChannels = 1;
	}
	else
		*pNumChannels = 0;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetSelectInfo (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
        aafSelectInfo_t *  pSelectInfo)
{
	plugin_trace("CAAFCDCICodec::GetSelectInfo()\n");
	HRESULT hr = S_OK;
	aafUInt32		storedWidth, storedHeight;
	aafInt16		padBits;
	aafUInt32		compWidth, bitsPerPixel = 0;
	aafUInt32		subSampling;
	aafRational_t		sampleRate = NULL_RATIONAL;
	aafFrameLayout_t	frameLayout;
	aafUInt32		numFields = 0;
	aafUInt32		bitsPerSample; // uncompressed.
	aafUID_t		compressionID = NULL_UID;


	if (NULL == fileMob || NULL == stream || NULL == pSelectInfo)
		return AAFRESULT_NULL_PARAM;


	try
	{
		// Make sure that given file mob contains the expected descriptor.
		CAAFCDCIDescriptorHelper descriptorHelper;
		checkResult(descriptorHelper.Initialize(fileMob));
		
		// Padding bits must be zero for this codec 
		hr = descriptorHelper.GetPaddingBits(&padBits);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			padBits = 0;

		// Get the compression code id from the descriptor and make sure that we
		// can handle it in this codec.
		hr = descriptorHelper.GetCompression(&compressionID);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_SUCCESS == hr && IsSupportedCompressionID(compressionID) && 0 == padBits)
		{
			pSelectInfo->willHandleMDES = kAAFTrue;
		}
		else
		{ // If the compression property was not present or the 
			// give compression is not supported or padBits was not zero
			// the set to false.
			pSelectInfo->willHandleMDES = kAAFFalse;
		}


		pSelectInfo->isNative = kAAFTrue;

		// This is a software only codec.
		pSelectInfo->hwAssisted = kAAFFalse;

		pSelectInfo->relativeLoss = 0;

		// Compute the avgBitsPerSec:
		checkResult(descriptorHelper.GetStoredView(&storedHeight, &storedWidth));
		checkResult(descriptorHelper.GetComponentWidth(&compWidth));

		checkResult(descriptorHelper.GetFrameLayout(&frameLayout));

		switch (frameLayout)
		{
			case kAAFFullFrame:
			case kAAFOneField:
				numFields = 1;
				break;
		
			case kAAFSeparateFields:
			case kAAFMixedFields:
				numFields = 2;
				break;
		
			default:
				break;
		} /* end switch */
		
		
		checkResult(descriptorHelper.GetHorizontalSubsampling(&subSampling));

		switch (subSampling)
		{
			/* 4:4:4 = 1 sample for each of luma and two chroma channels */   
			case 1:
				bitsPerPixel = (compWidth * 3) + padBits;
				break;
				
			/* 4:2:2 = two-pixels get 2 samples of luma, 1 of each chroma channel, avg == 2 */
			case 2:
				bitsPerPixel = (compWidth * 2) + padBits;					
				break;
		}


//  The following line from the omcCDCI.c (line 397?) appears to be computing the
//  bits per sample not bits per second!
//	info->spc.selectInfo.info.avgBitsPerSec = storedWidth * storedHeight * bitsPerPixel;

		bitsPerSample = (bitsPerPixel * storedWidth * storedHeight) * numFields;

		checkResult(descriptorHelper.GetSampleRate(&sampleRate));
		checkExpression(0 < sampleRate.denominator && 0 < sampleRate.numerator,
			              AAFRESULT_BADRATE);


		pSelectInfo->avgBitsPerSec = (bitsPerSample * sampleRate.numerator) / sampleRate.denominator;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}
	
	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountSamples (
        aafUID_constref essenceKind,
        aafLength_t *  pNumSamples)
{
	plugin_trace("CAAFCDCICodec::CountSamples()\n");
	if (NULL == pNumSamples)
		return AAFRESULT_NULL_PARAM;

	if(EqualAUID(&essenceKind, &kAAFDataDef_Picture)
		|| EqualAUID(&essenceKind, &kAAFDataDef_LegacyPicture))
	{
		*pNumSamples = _numberOfSamples;
	}
	else
		*pNumSamples = 0;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::ValidateEssence (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
		aafCheckVerbose_t  /* verbose */,
        aafCheckWarnings_t /* warning */,
         aafUInt32  /* bufSize */,
		wchar_t *  pName,
        aafUInt32  *bytesWritten)
{
	plugin_trace("CAAFCDCICodec::ValidateEssence()\n");
	if (NULL == fileMob || NULL == stream ||
		  NULL == pName   || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}



// Check input arguments.
// Use the given source mob to initialize codec's descriptor helper.
// Use the descriptor helper to read codec parameters from descriptor.
// Set appropriate open mode.
// Set parameters which require specific initialization upon essence creation.
// Save the given essence stream to use with codec.
// Call UpdateCalculatedData() to set calculated parameters.
// Finally, put modified codec parameters back to descriptor calling 
//  UpdateDescriptor().
//
HRESULT STDMETHODCALLTYPE
CAAFCDCICodec::Create (IAAFSourceMob *p_srcmob,
  aafUID_constref flavour,
  aafUID_constref essenceKind,
  aafRational_constref sampleRate,
  IAAFEssenceStream * stream,
  aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFCDCICodec::Create()\n");

    HRESULT	    hr = S_OK;
    
    // Check input arguments.
	if ( NULL == p_srcmob || NULL == stream )
		return AAFRESULT_NULL_PARAM;

	// Due to a bug in the CreateEssence() API, flavour must not be set here
	// Instead, flavour can be set with the SetFlavour() API.
	if ( EqualAUID( &kAAFNilCodecFlavour, &flavour) != kAAFTrue )
		return AAFRESULT_NULL_PARAM;

    try
    {
	    // Check essence kind
		checkExpression( kAAFTrue == EqualAUID(&essenceKind, &kAAFDataDef_Picture)
			|| kAAFTrue == EqualAUID(&essenceKind, &kAAFDataDef_LegacyPicture),
			AAFRESULT_INVALID_DATADEF );

		// Sets whether or not to compress samples as they are written
		SetCompressionEnabled(compEnable);

		if (_compressEnable == kAAFCompressionEnable)
		{
#if defined(USE_FFMPEG)
			// Prepare FFmpeg codec for encoding
			av_register_all();
			_encoder = (internal_ffmpeg_encoder_t*)av_mallocz(sizeof(internal_ffmpeg_encoder_t));
#else
			// Cannot compress without FFmpeg
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}

	    // Initialize the descriptor helper:
	    checkResult(_descriptorHelper.Initialize(p_srcmob));

	    // Initialize codec parameters reading descriptor information.
	    ReadDescriptor( _descriptorHelper );

	    // We are allowed to write to the given stream.
	    _openMode = kAAFMediaOpenAppend;

	    // Save the given essence stream.
	    SetEssenceStream(stream);

	    // Reset some parameters.
	    _imageAspectRatio = DEFAULT_ASPECT_RATIO;
	    _sampleRate = sampleRate;
	    _length = 0;
	    _numberOfSamples = 0;
	    UpdateCalculatedData();

	    // Update descriptor.
	    UpdateDescriptor( _descriptorHelper );
    }
    catch (HRESULT& rhr)
    {
	    hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	    // We CANNOT throw an exception out of a COM interface method!
	    // Return a reasonable exception code.
	    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

    // Cleanup

    return hr;
}



// Check input arguments.
// Use the given source mob to initialize codec's descriptor helper.
// Use the descriptor helper to read codec parameters from descriptor.
// Set appropriate open mode.
// Save the given essence stream to use with codec.
//
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::Open (IAAFSourceMob *p_srcmob,
		aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFCDCICodec::Open()\n");
    HRESULT hr = S_OK;


    // Check input arguments.
    if( p_srcmob == NULL || stream == NULL )
	return AAFRESULT_NULL_PARAM;

    try
    {
		// Initialize the descriptor helper:
		checkResult( _descriptorHelper.Initialize( p_srcmob ) );

		// Save the mode to the given stream.
		_openMode = openMode;

		// Save the given essence stream.
		SetEssenceStream(stream);

		// Sets whether or not to decompress the samples as they are read.
	    SetCompressionEnabled(compEnable);

		// Initialize codec parameters reading descriptor information.
		hr = ReadDescriptor( _descriptorHelper );
		checkExpression (hr == S_OK, hr);

		if (_compressEnable == kAAFCompressionEnable)
		{
#if defined(USE_FFMPEG)
			// Use compression read from Descriptor to determine if we can decompress this format
			if (! IsSupportedCompressionID(_compression))
				throw HRESULT( AAFRESULT_INVALID_OP_CODEC );

			// Prepare FFmpeg codec for decoding
			av_register_all();
			_decoder = (internal_ffmpeg_decoder_t*)av_mallocz(sizeof(internal_ffmpeg_decoder_t));
#else
			// Cannot decompress without FFmpeg
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}
    }
    catch (HRESULT& rhr)
    {
	    hr = rhr; // return thrown error code.
    }
    catch(...)
    {
	    // We CANNOT throw an exception out of a COM interface method!
	    // Return a reasonable exception code.
	    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

	// Setup variables based on _compression
	const FlavourParameters_t *params = lookupFlavourParamsByCompression(_compression, _storedHeight);
	if (params)
	{
		_apiFrameSampleSize = apiFrameSizeFromParams(params);

		if (_compressEnable != kAAFCompressionEnable)
			_apiFrameSampleSize = params->storedSize;

		_storedFrameSampleSize = params->storedSize;
	}

    return hr;
}



HRESULT STDMETHODCALLTYPE CAAFCDCICodec::WriteSamples(
	aafUInt32	nSamples,
        aafUInt32	buflen,
        aafDataBuffer_t	buffer,
        aafUInt32	*pTotalSamplesWritten,
        aafUInt32	*pTotalBytesWritten)
{
	plugin_trace("CAAFCDCICodec::WriteSamples()\n");
    HRESULT	hr = S_OK;
    aafUInt32	n;
    aafUInt32	sampleBytesWritten;	// Number of sample bytes written.
    aafUInt32	alignmentBytes = 0;	// Number of alignment bytes.
    aafUInt8	*pAligmentBuffer = NULL;// Padding bytes to be written.


    try
    {
	// Check input args
	checkExpression( buffer != NULL && pTotalSamplesWritten != NULL && 
					pTotalBytesWritten != NULL, AAFRESULT_NULL_PARAM );

	// Init output
	*pTotalBytesWritten = 0;
	*pTotalSamplesWritten = 0;

	// More input args check
	checkExpression( nSamples != 0, AAFRESULT_INVALID_PARAM );

	// Preconditions.
	checkAssertion( NULL != _stream );
	checkExpression( _componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE );
	checkExpression( _apiFrameSampleSize != 0, AAFRESULT_ZERO_SAMPLESIZE );
#if defined(USE_FFMPEG)
	checkAssertion( !(_compressEnable == kAAFCompressionEnable && NULL == _encoder ));
#endif

	// Make sure the given buffer is really large enough for the complete
	// pixel data.
	checkExpression( _apiFrameSampleSize*nSamples <= buflen, AAFRESULT_SMALLBUF );

	// Write samples one by one aligning them if necessary.
	for( n=0; n<nSamples; n++ )
	{
		if (_compressEnable == kAAFCompressionEnable)
		{
#if defined(USE_FFMPEG)
			if (_encoder->tmpFrame) {
				int padColour[3] = {16, 128, 128};

				avpicture_fill(	_encoder->tmpFrame,
							buffer,
							_encoder->codec_context->pix_fmt,
							_encoder->codec_context->width,
							_encoder->codec_context->height - 32);
				avpicture_fill((AVPicture*)	_encoder->inputFrame,
							_encoder->inputBuffer,
							_encoder->codec_context->pix_fmt,
							_encoder->codec_context->width,
							_encoder->codec_context->height);
				av_picture_pad((AVPicture*)	_encoder->inputFrame,
							_encoder->tmpFrame,
							_encoder->codec_context->height,
							_encoder->codec_context->width,
							_encoder->codec_context->pix_fmt,
							32, 0, 0, 0,
							padColour);
			}
			else {
				// Setup AVPicture pointers to point to YUV planes
				avpicture_fill(	(AVPicture*)_encoder->inputFrame,
							buffer,
							_encoder->codec_context->pix_fmt,
							_encoder->codec_context->width,
							_encoder->codec_context->height);
			}

			// Encode one frame
			int size = avcodec_encode_video(_encoder->codec_context,
							_encoder->outputBuffer,
							_encoder->bufferSize,
							_encoder->inputFrame);
			if (size < 0)
			{
				fprintf(stderr, "avcodec_encode_video() failed\n");
				throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
			}

		    sampleBytesWritten = 0;
			checkResult(_stream->Write(size, _encoder->outputBuffer, &sampleBytesWritten));

#else
			// Can't compress without ffmpeg
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}
		else
		{
	    	sampleBytesWritten = 0;
	    	checkResult( _stream->Write( _apiFrameSampleSize, buffer, 
				&sampleBytesWritten));
		}
	    buffer += sampleBytesWritten;
	    *pTotalBytesWritten += sampleBytesWritten;


	    // Add padding for _imageAlignmentFactor.
	    // Do not count these bytes when returning 
	    // total number of bytes written.
	    if( _imageAlignmentFactor > 0 )
	    {
		alignmentBytes = sampleBytesWritten % _imageAlignmentFactor;

		if( alignmentBytes > 0)
		{
		    pAligmentBuffer = new aafUInt8[ alignmentBytes ];
		    memset( static_cast<void*>(pAligmentBuffer), 0, 
			    alignmentBytes );
		    checkResult( _stream->Write( 
			    alignmentBytes, 
			    static_cast<aafDataBuffer_t>(pAligmentBuffer), 
			    &sampleBytesWritten ) );
		    delete[] pAligmentBuffer;
		    pAligmentBuffer = NULL;
		}
	    }

	    SetNumberOfSamples( _numberOfSamples + 1 );

	    // Update total number of samples written.
	    (*pTotalSamplesWritten)++;
	}
    }
    catch (HRESULT& rhr)
    {
	hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	// We CANNOT throw an exception out of a COM interface method!
	// Return a reasonable exception code.
	hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

    delete[] pAligmentBuffer;


    return hr;
}


/*
	nSamples		- number of samples to read.
	buflen			- size of the buffer to hold read data.
	buffer			- buffer to hold read data.
	pTotalSamplesRead	- number of read samples.
	pTotalBytesRead		- number of read bytes. This covers only 
				    number of bytes transfered to user buffer. 
				    Alignment bytes are not counted here.
*/
HRESULT STDMETHODCALLTYPE CAAFCDCICodec::ReadSamples(
	aafUInt32	nSamples,
	aafUInt32	buflen,
	aafDataBuffer_t	buffer,
	aafUInt32	*pTotalSamplesRead,
	aafUInt32	*pTotalBytesRead )
{
	plugin_trace("CAAFCDCICodec::ReadSamples()\n");
    HRESULT	hr = S_OK;
    aafUInt32	n;
    aafUInt32	sampleBytesRead;	// Number of sample bytes read.
    aafUInt32	sampleSize;		// Number of bytes per sample.
    aafUInt32	alignmentBytes;		// Number of unused alignment bytes.


    try
    {
	// Check input args
	checkExpression( buffer != NULL && pTotalSamplesRead != NULL && 
		pTotalBytesRead != NULL, AAFRESULT_NULL_PARAM );

	// Init output
	*pTotalSamplesRead = 0;
	*pTotalBytesRead = 0;

	// More input args check
	checkExpression( nSamples != 0, AAFRESULT_INVALID_PARAM );

	// Preconditions.
	checkAssertion( NULL != _stream );
	checkExpression( _componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE );
	checkExpression( _apiFrameSampleSize != 0, AAFRESULT_ZERO_SAMPLESIZE );
#if defined(USE_FFMPEG)
	checkAssertion( !(_compressEnable == kAAFCompressionEnable && NULL == _decoder ));
#endif

	sampleSize = _apiFrameSampleSize;

	plugin_trace("CAAFCDCICodec::ReadSamples() sampleSize=%d nSamples=%d sampleSize*nSamples=%d buflen=%d\n",
				sampleSize, nSamples, sampleSize*nSamples, buflen);

	// Make sure the given buffer is really large enough for the complete
	// pixel data.
	checkExpression( sampleSize*nSamples <= buflen, AAFRESULT_SMALLBUF );

#if defined(USE_FFMPEG)
	const FlavourParameters_t *params = lookupFlavourParamsByCompression(_compression, _storedHeight);
#endif

	// Read samples one by one skipping alignment data
	for( n = 0; n < nSamples; n++ )
	{
	    sampleBytesRead = 0;

		if (_compressEnable == kAAFCompressionEnable)
		{
#if defined(USE_FFMPEG)
			// Setup decoder first time through
			if (_decoder->codec == NULL)
			{
				enum CodecID codecId = CODEC_ID_DVVIDEO;
				if (params->codecFamily == codecMPEG)
					codecId = CODEC_ID_MPEG2VIDEO;

				_decoder->codec = avcodec_find_decoder(codecId);
				checkExpression( _decoder->codec, AAFRESULT_DECOMPRESS );

				_decoder->codec_context = avcodec_alloc_context();
				checkExpression( _decoder->codec_context, AAFRESULT_DECOMPRESS );

				// Setup outputFrame's dimensions and format
				avcodec_set_dimensions(_decoder->codec_context, params->width, params->height);
				if (params->vidFormat == YUV411)
					_decoder->codec_context->pix_fmt = PIX_FMT_YUV411P;
				else if (params->vidFormat == YUV420)
					_decoder->codec_context->pix_fmt = PIX_FMT_YUV420P;
				else
					_decoder->codec_context->pix_fmt = PIX_FMT_YUV422P;
		
				// Open codec
				if (avcodec_open(_decoder->codec_context, _decoder->codec) < 0) {
					printf("ReadSamples(): avcodec_open() failed\n");
					throw HRESULT( AAFRESULT_DECOMPRESS );
				}

				_decoder->outputFrame = avcodec_alloc_frame();
				checkExpression( _decoder->outputFrame, AAFRESULT_DECOMPRESS );

				// The extra 4 bytes are to allow ffmpeg's optimized functions to read past array
				_decoder->inputBuffer = (unsigned char*)av_mallocz(params->storedSize + 4);
				checkExpression( _decoder->inputBuffer, AAFRESULT_DECOMPRESS );
			}

			// Read one frame's worth of uncompressed data
		    checkResult( _stream->Read( params->storedSize, _decoder->inputBuffer, &sampleBytesRead ));

			// Setup AVFrame's YUV pointers to point into supplied output buffer
			avpicture_fill(	(AVPicture*)_decoder->outputFrame,
							buffer,
							_decoder->codec_context->pix_fmt,
							_decoder->codec_context->width,
							_decoder->codec_context->height);

			// Decode one frame
			int dec_picture_ok = 0;
			int bytes_dec;
#if LIBAVCODEC_VERSION_INT >= ((52<<16)+(25<<8)+0)
			// avcodec_decode_video was deprecated in favour of avcodec_decode_video2
			AVPacket packet;
			av_init_packet(&packet);
			packet.data = _decoder->inputBuffer;
			packet.size = params->storedSize;
			bytes_dec = avcodec_decode_video2(_decoder->codec_context, _decoder->outputFrame, &dec_picture_ok,
												&packet);
#else
			bytes_dec = avcodec_decode_video(_decoder->codec_context, _decoder->outputFrame, &dec_picture_ok,
												_decoder->inputBuffer, params->storedSize);
#endif

			if (bytes_dec <= 0 || !dec_picture_ok) 
			{
				fprintf(stderr, "avcodec_decode_video() Error decoding video\n");
				throw HRESULT( AAFRESULT_DECOMPRESS );	// decompression failed
			}

			// Copy the AVFrame uncompressed video data into a contiguous video frame
			// at the supplied buffer address.  Ideally AVCodecContext's get_buffer(),
			// could be used to avoid this copy, but it is undocumented.
			int w, h;
			if (params->codecFamily == codecMPEG)
			{
				// IMX formats store extra 32/26 lines but the API uses displayed size
				w = params->width;
				h = params->height;
			}
			else
			{
				// All other formats have API size same as stored size
				w = params->storedWidth;
				h = params->storedHeight;
			}
			checkAssertion(w <= _decoder->codec_context->width && h <= _decoder->codec_context->height);
			int	colour_w = (params->vidFormat == YUV411) ? w / 4 : w / 2;
			int	colour_h = (params->vidFormat == YUV420) ? h / 2 : h;
			unsigned char *dest_y = buffer;
			unsigned char *dest_u = buffer + w * h;
			unsigned char *dest_v = buffer + w * h + colour_w * colour_h;
			unsigned char *src_y = _decoder->outputFrame->data[0];
			unsigned char *src_u = _decoder->outputFrame->data[1];
			unsigned char *src_v = _decoder->outputFrame->data[2];
			if (params->yOffset > 0 &&
			    _decoder->codec_context->height >= h + params->yOffset)
			{
				src_y += params->yOffset * _decoder->outputFrame->linesize[0];
				if (params->vidFormat == YUV420)
				{
					src_u += params->yOffset / 2 * _decoder->outputFrame->linesize[1];
					src_v += params->yOffset / 2 * _decoder->outputFrame->linesize[2];
				}
				else
				{
					src_u += params->yOffset * _decoder->outputFrame->linesize[1];
					src_v += params->yOffset * _decoder->outputFrame->linesize[2];
				}
			}
			for (int j = 0; j < h; j++) {
				memcpy(dest_y, src_y, w);
				dest_y += w;
				src_y += _decoder->outputFrame->linesize[0];

				if (params->vidFormat != YUV420 || j % 2 == 0)
				{
					memcpy(dest_u, src_u, colour_w);
					dest_u += colour_w;
					src_u += _decoder->outputFrame->linesize[1];
					memcpy(dest_v, src_v, colour_w);
					dest_v += colour_w;
					src_v += _decoder->outputFrame->linesize[2];
				}
			}

			buffer += _apiFrameSampleSize;
			*pTotalBytesRead += bytes_dec;
#else
			// Can't decompress without ffmpeg
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}
		else
		{
	    	checkResult( _stream->Read( sampleSize, buffer, &sampleBytesRead ));
	    	buffer += sampleBytesRead;
	    	*pTotalBytesRead += sampleBytesRead;
		}

	    // If sample is aligned skip unused alignment bytes.
	    // Do not count these bytes when returning total number of 
	    // bytes read.
	    if( _imageAlignmentFactor > 0 )
	    {
		alignmentBytes = sampleBytesRead % _imageAlignmentFactor;
		if( alignmentBytes > 0)
		    checkResult( _stream->Seek( alignmentBytes ) );
	    }

	    // Update total number of samples read.
	    (*pTotalSamplesRead)++;
	}
    }
    catch (HRESULT& rhr)
    {
	hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	// We CANNOT throw an exception out of a COM interface method!
	// Return a reasonable exception code.
	hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }



    return hr;
}



/*
    Seek

    Seek to a particular sample frame on the media.

    Input:
	sampleFrame	- zero-based number of frame to seek to.

    Return:
    AAFRESULT_SUCCESS		- success.
    AAFRESULT_BADFRAMEOFFSET	- invalid frame number.
    AAFRESULT_BADSAMPLEOFFSET	- invalid offset in the essnce stream.
				    This error is returned by stream->Seek().
    AAFRESULT_NOT_INITIALIZED	- returned by stream->Seek().
    AAFRESULT_UNEXPECTED_EXCEPTION
*/
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::Seek (aafPosition_t  sampleFrame)
{
	plugin_trace("CAAFCDCICodec::Seek()\n");
	HRESULT	hr = S_OK;
	aafUInt32	alignmentBytes = _imageAlignmentFactor > 0 ? 
		(_storedFrameSampleSize % _imageAlignmentFactor) : 0;


	try
	{
		checkAssertion(NULL != _stream);
		checkExpression( sampleFrame <= _numberOfSamples, AAFRESULT_BADFRAMEOFFSET );

		checkResult(_stream->Seek( (_storedFrameSampleSize + alignmentBytes) * sampleFrame) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	return hr;
}


// Helper utility to make sure the current information in the codec is 
// synchronized with the given descriptor information . 
// Called in Open and Create methods.
HRESULT STDMETHODCALLTYPE CAAFCDCICodec::ReadDescriptor( 
    CAAFCDCIDescriptorHelper&	descriptorHelper )
{
	plugin_trace("CAAFCDCICodec::ReadDescriptor()\n");
    HRESULT		hr = S_OK;
    IAAFContainerDef	*pContainerDef = NULL;
    IAAFDefObject	*pDefObj = NULL;


    try
    {
	//
	// FileDescriptor methods:
	//

	// Length is optional property. If it's not present 
	// set it to 0.
	hr = descriptorHelper.GetLength( &_length );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _length = 0;

	_numberOfSamples = (aafUInt32)_length;

	// SampleRate is optional property. If it's not present 
	// set it to 0.
	hr = descriptorHelper.GetSampleRate( &_sampleRate );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _sampleRate = NULL_RATIONAL;

	// ContainerFormat
	// pContainerDef and pDefObj released below...
	checkResult(descriptorHelper.GetContainerFormat(&pContainerDef));
	checkResult(pContainerDef->QueryInterface(IID_IAAFDefObject, 
	    (void **)&pDefObj));
	checkResult(pDefObj->GetAUID(&_containerFormat));


	//
	// DigitalImageDescriptor methods
	//

	// Compression is optional property. If it's not present
	// set it to default value. 
	hr = descriptorHelper.GetCompression( &_compression );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _compression = NULL_UID;

	// Stored view
	checkResult( descriptorHelper.GetStoredView(
	    &_storedHeight, &_storedWidth ) );
	_imageHeight = _storedHeight;
	_imageWidth = _storedWidth;

	// Sampled view is optional property. If it's not present 
	// set sampled view to be the same as stored view.
	hr = descriptorHelper.GetSampledView(
	    &_sampledHeight, &_sampledWidth,
	    &_sampledXOffset, &_sampledYOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );

	if( AAFRESULT_PROP_NOT_PRESENT == hr )
	{
	    _sampledWidth = _storedWidth;
	    _sampledHeight = _storedHeight;
	    _sampledXOffset = 0;
	    _sampledYOffset = 0;
	}

	// Display view is optional property. If it's not present 
	// set display view to be the same as stored view.
	hr = descriptorHelper.GetDisplayView( 
	    &_displayHeight, &_displayWidth, 
	    &_displayXOffset, &_displayYOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
	{
	    _displayWidth = _storedWidth;
	    _displayHeight = _storedHeight;
	    _displayXOffset = 0;
	    _displayYOffset = 0;
	}

	// FrameLayout (required).
	checkResult(descriptorHelper.GetFrameLayout( &_frameLayout ) );

	// VideoLineMap (required).
	// Video line map contains either 1 or 2 values depending
	// on frame layout. 
	checkResult( descriptorHelper.GetVideoLineMapSize( 
	    &_videoLineMapSize ) );
	checkResult( descriptorHelper.GetVideoLineMap(
	    _videoLineMapSize, _videoLineMap ) );

	// ImageAspectRatio (required).
	checkResult(descriptorHelper.GetImageAspectRatio(
	    &_imageAspectRatio));

	// AlphaTransparency (optional).
	hr = descriptorHelper.GetAlphaTransparency( &_alphaTransparency );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _alphaTransparency = kAAFMinValueTransparent;

	// Gamma (optional).
	hr = descriptorHelper.GetGamma( &_gamma );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _gamma = NULL_UID;

	// ImageAlignmentFactor (optional).
	hr = descriptorHelper.GetImageAlignmentFactor( 
	    &_imageAlignmentFactor );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _imageAlignmentFactor = 0;

	// FieldDominance (optional)
	// Although FieldDominance is optional property I would expect it 
	// to be there for 2 fields layout. But if it's not there, let 
	// user decide what to do.
	hr = descriptorHelper.GetFieldDominance( &_fieldDominance );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _fieldDominance = kAAFNoDominant;

	// FieldStartOffset (optional)
	hr = descriptorHelper.GetFieldStartOffset( &_fieldStartOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _fieldStartOffset = 0;

	// Get FieldEndOffset (optional)
	hr = descriptorHelper.GetFieldEndOffset( &_fieldEndOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _fieldEndOffset = 0;


 	//
	// CDCIDescriptor
	//

	// ComponentWidth (required)
	checkResult( descriptorHelper.GetComponentWidth(&_componentWidth));
	// Check for supported values.
	checkExpression( _componentWidth == 8 || 
			 _componentWidth == 10 ||
			 _componentWidth == 16, AAFRESULT_BADPIXFORM );

	// HorizontalSubsampling (required)
	checkResult(descriptorHelper.GetHorizontalSubsampling(
	    &_horizontalSubsampling ) );

	if (IsDV(_compression))
	{
		checkExpression( _horizontalSubsampling == 1 || 
				 _horizontalSubsampling == 2 ||
				 _horizontalSubsampling == 4, 
				 AAFRESULT_BADPIXFORM );
	}
	else
	{
		checkExpression( _horizontalSubsampling == 1 || 
				 _horizontalSubsampling == 2, 
				 AAFRESULT_BADPIXFORM );
	}

	// VerticalSubsampling (optional)
	hr = descriptorHelper.GetVerticalSubsampling(
		    &_verticalSubsampling );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	{
	    _verticalSubsampling = 1;
	}

	// ColorSiting (optional)
	// Do not check for supported values because ColorSiting does not
	// participate in codec's internal calculations. 
	// Let user handle it.
	hr = descriptorHelper.GetColorSiting(&_colorSiting);
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_colorSiting = kAAFCoSiting;

	// BlackReferenceLevel (optional)
	hr = descriptorHelper.GetBlackReferenceLevel( 
	    &_blackReferenceLevel );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_blackReferenceLevel = 0;

	// WhiteReferenceLevel (optional)
	hr = descriptorHelper.GetWhiteReferenceLevel( 
	    &_whiteReferenceLevel );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
	    _whiteReferenceLevel = (1U << _componentWidth) - 1;

	// ColorRange (optional)
	hr = descriptorHelper.GetColorRange(&_colorRange);
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_colorRange = (1U << _componentWidth) - 1;

	// PaddingBits (optional)
	hr = descriptorHelper.GetPaddingBits( &_paddingBits );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_paddingBits = 0;


	UpdateCalculatedData();
	hr = S_OK; //reset hr here
    }
    catch (HRESULT& rhr)
    {
	    hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	    // We CANNOT throw an exception out of a COM interface method!
	    // Return a reasonable exception code.
	    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }


    // Cleanup
    if( pContainerDef != NULL )
	pContainerDef->Release();
    if( pDefObj != NULL )
	pDefObj->Release();


    return hr;
}


// Prepares Codec internals for WriteSamples()
// Called by PutEssenceFormat() and SetFlavour()
void CAAFCDCICodec::SetCodecState(void)
{
	if (_compressEnable == kAAFCompressionEnable)
	{
#if defined(USE_FFMPEG)
		const FlavourParameters_t *params = lookupFlavourParams(_flavour);

		CodecID codec_id = CODEC_ID_DVVIDEO;
		if (params->codecFamily == codecMPEG)
			codec_id = CODEC_ID_MPEG2VIDEO;

		if (! (_encoder->codec = avcodec_find_encoder(codec_id)))
		{
			fprintf(stderr, "Could not find encoder\n");
			//cleanup(_encoder);
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
		}

		if (! (_encoder->codec_context = avcodec_alloc_context()))
		{
			fprintf(stderr, "Could not allocate encoder context\n");
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
		}

		// Note FFmpeg time_base is the inverse of _sampleRate
		_encoder->codec_context->time_base.num = _sampleRate.denominator;
		_encoder->codec_context->time_base.den = _sampleRate.numerator;

		_encoder->codec_context->codec_type = CODEC_TYPE_VIDEO;
		_encoder->codec_context->sample_aspect_ratio.num = _imageAspectRatio.numerator;
		_encoder->codec_context->sample_aspect_ratio.den = _imageAspectRatio.denominator;

		if (params->codecFamily == codecMPEG) {
			int bit_rate = bitrate_for_flavour(_flavour);
			// vbv_buffer_size values found by experiment to match Avid IMX bitstreams
			// Value for 50Mbps 40Mbps 30Mbps are: 2,000,000 1,600,000 1,200,000
			int buffer_size = bit_rate / 25;
			_encoder->codec_context->rc_min_rate = bit_rate;
			_encoder->codec_context->rc_max_rate = bit_rate;
			_encoder->codec_context->bit_rate = bit_rate;
			_encoder->codec_context->rc_buffer_size = buffer_size;
			_encoder->codec_context->rc_initial_buffer_occupancy = buffer_size;
			_encoder->codec_context->rtp_payload_size = 1;
			_encoder->codec_context->global_quality = 1 * FF_QP2LAMBDA;
			_encoder->codec_context->gop_size = 0;
			_encoder->codec_context->qmin = 1;
			_encoder->codec_context->qmax = 3;
			_encoder->codec_context->lmin = 1 * FF_QP2LAMBDA;
			_encoder->codec_context->lmax = 3 * FF_QP2LAMBDA;
			_encoder->codec_context->flags |= CODEC_FLAG_INTERLACED_DCT | CODEC_FLAG_LOW_DELAY;
			_encoder->codec_context->intra_dc_precision = 2;
			_encoder->codec_context->flags2 |= CODEC_FLAG2_NON_LINEAR_QUANT;
			_encoder->codec_context->flags2 |= CODEC_FLAG2_INTRA_VLC;

			_encoder->tmpFrame = (AVPicture*)av_mallocz(sizeof(AVPicture));
			_encoder->inputBuffer = (unsigned char*)av_mallocz(params->storedWidth * params->storedHeight * 2);
		}

		if (params->vidFormat == YUV411)
			_encoder->codec_context->pix_fmt = PIX_FMT_YUV411P;
		else if (params->vidFormat == YUV420)
			_encoder->codec_context->pix_fmt = PIX_FMT_YUV420P;
		else
			_encoder->codec_context->pix_fmt = PIX_FMT_YUV422P;

		// storedHeight includes 32 lines of padding for IMX format
		avcodec_set_dimensions(_encoder->codec_context, params->storedWidth, params->storedHeight);

		if (avcodec_open(_encoder->codec_context, _encoder->codec) < 0)
		{
			fprintf(stderr, "Could not open encoder\n");
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
		}

		if (! (_encoder->inputFrame = avcodec_alloc_frame()))
		{
			fprintf(stderr, "Could not allocate input frame\n");
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
		}

		// Extra 5000 bytes needed to avoid "stuffing too large" error at runtime
		_encoder->bufferSize = params->storedSize + 5000;
		_encoder->outputBuffer = (unsigned char*)av_mallocz(_encoder->bufferSize);

#else
		// Can't compress without ffmpeg
		throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
	}

	UpdateDescriptor( _descriptorHelper );
	UpdateCalculatedData();
}


void CAAFCDCICodec::UpdateDescriptor (CAAFCDCIDescriptorHelper& descriptorHelper)
{
	plugin_trace("CAAFCDCICodec::UpdateDescriptor()\n");
	// Update FileDescriptor properties
	checkResult(descriptorHelper.SetLength(_numberOfSamples));
	checkResult(descriptorHelper.SetSampleRate(_sampleRate));

	// Update DigitalImageDescriptor properties
	checkResult(descriptorHelper.SetCompression(_compression));
	checkResult(descriptorHelper.SetStoredView(_storedHeight, _storedWidth));
	checkResult(descriptorHelper.SetSampledView(_sampledHeight, _sampledWidth, _sampledXOffset, _sampledYOffset));
	checkResult(descriptorHelper.SetDisplayView(_displayHeight, _displayWidth, _displayXOffset, _displayYOffset));
	checkResult(descriptorHelper.SetFrameLayout(_frameLayout));
	checkResult(descriptorHelper.SetVideoLineMap(_videoLineMapSize, _videoLineMap));
	checkResult(descriptorHelper.SetImageAspectRatio(_imageAspectRatio));
	checkResult(descriptorHelper.SetAlphaTransparency(_alphaTransparency));
	checkResult(descriptorHelper.SetImageAlignmentFactor(_imageAlignmentFactor));
	checkResult(descriptorHelper.SetFieldDominance(_fieldDominance));
	checkResult(descriptorHelper.SetFieldStartOffset(_fieldStartOffset));
	checkResult(descriptorHelper.SetFieldEndOffset(_fieldEndOffset));
	if (_gamma != NULL_UID)
		checkResult(descriptorHelper.SetGamma(_gamma));

	// CDCIDescriptor methods:
	checkResult(descriptorHelper.SetComponentWidth(_componentWidth));
	checkResult(descriptorHelper.SetHorizontalSubsampling(_horizontalSubsampling));
	checkResult(descriptorHelper.SetVerticalSubsampling(_verticalSubsampling));
	checkResult(descriptorHelper.SetColorSiting(_colorSiting));
	checkResult(descriptorHelper.SetBlackReferenceLevel(_blackReferenceLevel));
	checkResult(descriptorHelper.SetWhiteReferenceLevel(_whiteReferenceLevel));
	checkResult(descriptorHelper.SetColorRange(_colorRange));
	checkResult(descriptorHelper.SetPaddingBits(_paddingBits));

	// If working with legacy CDCI set some specific descriptor properties.
	if( memcmp(&_compression,&AAF_CMPR_AUNC422,sizeof(_compression))==0  &&
	    _fieldStartOffset == 0  &&  _fieldEndOffset == 4 )
	{
	    checkResult( descriptorHelper.SetResolutionID( 0x97 ) );
	    checkResult( descriptorHelper.SetFrameSampleSize( _storedFrameSampleSize ) );
	}

	// LegacyDV needs several legacy properties set
	if (EqualAUID(&_compression, &kAAFCompressionDef_LegacyDV))
	{
	    checkResult( descriptorHelper.SetOffsetToFrameIndexes( 0 ) );
	    checkResult( descriptorHelper.SetFrameIndexByteOrder( 0x4949) );
	    checkResult( descriptorHelper.SetFirstFrameOffset( 0 ) );

	    checkResult( descriptorHelper.SetFrameSampleSize( _storedFrameSampleSize ));
	    checkResult( descriptorHelper.SetImageSize( static_cast<aafInt32>(_numberOfSamples * _storedFrameSampleSize) ) );

		if (_imageHeight == 288)		// i.e. PAL size
		{
	    	checkResult( descriptorHelper.SetResolutionID( 0x8d ) );
		}
		else
		{
	    	checkResult( descriptorHelper.SetResolutionID( 0x8c ) );
		}
	}

	// Prepare to set Essence Element key
	IAAFEssenceDataStream2 *pEDS2;
	checkResult(_stream->QueryInterface(IID_IAAFEssenceDataStream2, (void **)&pEDS2));

	aafUID_t GC_EEK = { 0x0d010301, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01 } };
	HRESULT hr;

	// Set Essence Element key depending upon essence type
	if (IsSupportedCompressionID(_compression))
		hr = pEDS2->SetEssenceElementKey( GC_EEK, 0x18, 1, 0x02, 1, 1 );
	else		// uncompressed clip wrapped
		hr = pEDS2->SetEssenceElementKey( GC_EEK, 0x15, 1, 0x03, 1, 1 );

	pEDS2->Release();

	// EssenceDataStream2 will return AAFRESULT_CODEC_SEMANTIC_WARN when SetEssenceElementKey()
	// is not appropriate for container, so ignore the error in this case.
	if (AAFRESULT_FAILED(hr) && hr != AAFRESULT_CODEC_SEMANTIC_WARN)
		throw HRESULT(AAFRESULT_CODEC_SEMANTIC_WARN);
}


// Routine to keep calculated member data up-to-date.
void CAAFCDCICodec::UpdateCalculatedData( void )
{
	plugin_trace("CAAFCDCICodec::UpdateCalculatedData()\n");
    aafUInt32 numFields = 0;

    // Number of fields per frame
    if ( _frameLayout == kAAFSeparateFields || _frameLayout == kAAFMixedFields )
		numFields = 2;
	else
		numFields = 1;


    // Computer the file bytes per sample.
    _bitsPerPixelAvg = 0;

   	if (_horizontalSubsampling == 1)
		_bitsPerPixelAvg = (aafInt16)((_componentWidth * 3) + _paddingBits);
	else
		_bitsPerPixelAvg = (aafInt16)((_componentWidth * 2) + _paddingBits);
    checkAssertion( _bitsPerPixelAvg % 8  ==  0 );


	const FlavourParameters_t *p_params = lookupFlavourParams(_flavour);
	if (p_params != NULL)
	{
		// Do nothing since SetFlavour has been called and necessary variables set.
	}
	else if (EqualAUID(&NULL_UID, &_compression) || EqualAUID(&AAF_CMPR_AUNC422, &_compression))
	{
		aafUInt32 _bitsPerSample = 0;
    	//
	    // Calculate number of bits per pixel. 
    	// Be aware that for 4:2:2 format number of bits per pi
    	// 4:4:4 = 1 sample for each of luma and two chroma channels
    	if (_horizontalSubsampling == 1)
    	{
		// Number of bits per sample = num bits per field * num of fields.
		// Number of bits per field  = num bits per image pixels + 
		//	    num bits per client fill (field start/end offsets)..
		// Number of bits per image pixels = num bits per line * num of 
		//	    lines (height).
		// Number of bits per line = num of bits per pixel * width + num 
		//	    of padding bits per line.
		//
		// Note that number of padding bits per line is not number of padding
		//	    bits per pixel.
			_bitsPerSample = ( (_imageWidth * _bitsPerPixelAvg + 
					_padBytesPerRow * 8) * _imageHeight  + 
					(_fieldStartOffset + _fieldEndOffset) * 8 ) *
					numFields;
   	 	}
   	 	// 4:2:2 = two-pixels get 2 samples of luma, 1 of each chroma channel, 
   	 	// avg == 2
    	//
    	// Note: Supported component sizes named by the spec are 8, 10 and 16 bits.
    	//		With these values YCrYCb will always be on a byte boundary. 
    	else if (_horizontalSubsampling == 2)
    	{
			// See comments for 4:4:4
			_bitsPerSample = ( (_imageWidth/2  * (_componentWidth*4 + _paddingBits)+
				_padBytesPerRow * 8) * _imageHeight  + 
				(_fieldStartOffset + _fieldEndOffset) * 8 ) *
				numFields;
    	}

    	_apiFrameSampleSize = (_bitsPerSample + 7) / 8;
		_storedFrameSampleSize = _apiFrameSampleSize;
	}
}


/*
    CompleteWrite

    Keeps essence descriptor information up to date.

    Input:
    fileMob	- file source mob we want to keep in sync 
		    with the original one. This parameter is optional.

    Return:
    AAFRESULT_ASSERTION_VIOLATION
    AAFRESULT_UNEXPECTED_EXCEPTION
*/
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CompleteWrite (IAAFSourceMob *fileMob = NULL)
{
	plugin_trace("CAAFCDCICodec::CompleteWrite()\n");
    HRESULT hr = S_OK;


    try
    {
	// Make sure the descriptor information is up-to-date.
	UpdateDescriptor(_descriptorHelper); // throw HRESULT


	// Handle the source mob we want to keep synchronized.
	if (NULL != fileMob)
	{	// Initialize the descriptor helper:
	    CAAFCDCIDescriptorHelper descriptorHelper;
	    checkResult(descriptorHelper.Initialize(fileMob));

	    // Make sure the descriptor information is up-to-date.
	    UpdateDescriptor(descriptorHelper); // throw HRESULT
	}

    }
    catch (HRESULT& rhr)
    {
	    hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	    // We CANNOT throw an exception out of a COM interface method!
	    // Return a reasonable exception code.
	    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

    return hr;
}		




HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CreateDescriptorFromStream (IAAFEssenceStream * pStream,
        IAAFSourceMob *fileMob)
{
	plugin_trace("CAAFCDCICodec::CreateDescriptorFromStream()\n");
	if (NULL == pStream || NULL == fileMob)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
	plugin_trace("CAAFCDCICodec::GetCurrentEssenceStream()\n");
	if (NULL == ppStream)
		return AAFRESULT_NULL_PARAM;
	if (NULL == _stream)
		return AAFRESULT_NULLOBJECT;
	
	*ppStream = _stream;
	_stream->AddRef();

	return AAFRESULT_SUCCESS;
}



// Private data structure for handling essence format specifier data.
const size_t kMaxEssenceFormatData = 64;

typedef struct _aafEssenceFormatData_t
{
	aafUID_t opcode;
	aafInt32 size;
	union
	{
	    aafUInt8		expData[kMaxEssenceFormatData];
	    aafUInt32		expUInt32;
	    aafInt32		expInt32;
	    aafUInt16		expUInt16;
	    aafInt16		expInt16;
	    aafUInt8		expUInt8;
	    aafInt8		expInt8;
	    aafRational_t	expRational;
	    aafBoolean_t	expBoolean;
	    aafRect_t		expRect;
	    aafColorSpace_t	expColorSpace;
	    aafCompArray_t	expCompArray;
	    aafCompSizeArray_t	expCompSizeArray;
	    aafFrameLayout_t	expFrameLayout;
	    aafVideoLineMap_t	expVideoLineMap;
	    aafColorSiting_t	expColorSiting;
	    aafFieldDom_t	expFieldDominance;
	    aafAlphaTransparency_t expAlphaTransparency;
	    aafUID_t		expUID;
	} operand;
} aafEssenceFormatData_t;



#define ADD_FORMAT_SPECIFIER( f, id, p ) \
		(f->AddFormatSpecifier( \
			id, \
			sizeof(p), \
			reinterpret_cast<aafDataBuffer_t>(&(p))))


//  kAAFPixelSize is calculated value and cannot be set. 
//	Set kAAFCDCIPadBits, kAAFCDCICompWidth and kAAFCDCIHorizSubsampling
//	to change kAAFPixelSize.
// 
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	plugin_trace("CAAFCDCICodec::PutEssenceFormat()\n");
    HRESULT			hr = S_OK;
    aafInt32			numSpecifiers, i;
    aafEssenceFormatData_t	param;


    if (NULL == pFormat)
	    return AAFRESULT_NULL_PARAM;


    try
    {
	checkResult(pFormat->NumFormatSpecifiers (&numSpecifiers));



	for (i = 0; i < numSpecifiers; ++i)
	{
		memset(&param, 0, sizeof(param));
		checkResult(pFormat->GetIndexedFormatSpecifier (i, 
			&param.opcode, sizeof(param.operand.expData), param.operand.expData, &param.size));

		if (EqualAUID(&kAAFCompression, &param.opcode))
		{	// Validate the in-memory size.
			aafUID_t		nullCompID = NULL_UID;
			checkExpression(param.size == sizeof(param.operand.expUID), AAFRESULT_INVALID_PARM_SIZE);

			checkExpression(IsSupportedCompressionID(param.operand.expUID), AAFRESULT_BADCOMPR);

			memcpy( &_compression, &(param.operand.expUID), 
			    sizeof(param.operand.expUID) );
		}
		else if (EqualAUID(&kAAFPixelFormat, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expColorSpace), AAFRESULT_INVALID_PARM_SIZE);

			// Currently we only support the following pixel transformations.
			checkExpression(kAAFColorSpaceRGB == param.operand.expColorSpace ||
					      kAAFColorSpaceYUV == param.operand.expColorSpace,
					      AAFRESULT_BADPIXFORM);

			_pixelFormat = param.operand.expColorSpace;
		}
		else if (EqualAUID(&kAAFFrameLayout, &param.opcode))
		{
			// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expFrameLayout), AAFRESULT_INVALID_PARM_SIZE);


			// @@@
			// What do we do if the codec is asked to change the frame layout
			// after samples have already been written to the stream? This
			// should be a error in essence access...
			checkAssertion( param.operand.expFrameLayout == _frameLayout || _numberOfSamples == 0 );


			// Validate the frame layout:
			checkExpression( 
			    param.operand.expFrameLayout == kAAFFullFrame ||
			    param.operand.expFrameLayout == kAAFOneField ||
			    param.operand.expFrameLayout == kAAFSeparateFields ||
			    param.operand.expFrameLayout == kAAFMixedFields,
			    AAFRESULT_BADEXPORTLAYOUT );


			_frameLayout = param.operand.expFrameLayout;
		}
		else if( EqualAUID( &kAAFFieldDominance, &param.opcode ) )
		{
			// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expFieldDominance), AAFRESULT_INVALID_PARM_SIZE);

			// 1 or 2 are valid values
			checkExpression( param.operand.expFieldDominance == 1 || 
					    param.operand.expFieldDominance == 2,
					    AAFRESULT_ILLEGAL_VALUE );

			_fieldDominance = param.operand.expFieldDominance;
		}
		else if (EqualAUID(&kAAFStoredRect, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
			
			_storedWidth = param.operand.expRect.xSize;
			_storedHeight = param.operand.expRect.ySize;
			_imageWidth = _storedWidth;
			_imageHeight = _storedHeight;
		}
		else if (EqualAUID(&kAAFSampledRect, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
			
			_sampledXOffset = param.operand.expRect.xOffset;
			_sampledYOffset = param.operand.expRect.yOffset;
			_sampledHeight = param.operand.expRect.ySize;
			_sampledWidth = param.operand.expRect.xSize;
			_sampledHeight = param.operand.expRect.ySize;
		}
		else if (EqualAUID(&kAAFDisplayRect, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
			
			_displayXOffset = param.operand.expRect.xOffset;
			_displayYOffset = param.operand.expRect.yOffset;
			_displayHeight = param.operand.expRect.ySize;
			_displayWidth = param.operand.expRect.xSize;
			_displayHeight = param.operand.expRect.ySize;
		}
		else if (EqualAUID(&kAAFAspectRatio, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expRational), AAFRESULT_INVALID_PARM_SIZE);

			_imageAspectRatio = param.operand.expRational;
		}
		else if( EqualAUID( &kAAFAlphaTransparency, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expAlphaTransparency), AAFRESULT_INVALID_PARM_SIZE);

		    // kAAFMinValueTransparent or kAAFMaxValueTransparent are valid values
		    checkExpression( param.operand.expAlphaTransparency == kAAFMinValueTransparent || 
					param.operand.expAlphaTransparency == kAAFMaxValueTransparent,
					AAFRESULT_ILLEGAL_VALUE );

		    _alphaTransparency = param.operand.expAlphaTransparency;
		}
		else if( EqualAUID( &kAAFGamma, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUID), AAFRESULT_INVALID_PARM_SIZE);

		    _gamma = param.operand.expUID;
		}
		else if( EqualAUID( &kAAFImageAlignmentFactor, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _imageAlignmentFactor = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFVideoLineMap, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expVideoLineMap), AAFRESULT_INVALID_PARM_SIZE);
			_videoLineMap[0] = param.operand.expVideoLineMap[0];
			_videoLineMap[1] = param.operand.expVideoLineMap[1];
		}
		else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

			// 8, 10 or 16 are valid values
			checkExpression( param.operand.expUInt32 == 8 || 
					 param.operand.expUInt32 == 10 ||
					 param.operand.expUInt32 == 16, AAFRESULT_ILLEGAL_VALUE );

			_componentWidth = param.operand.expUInt32;
		}
		else if( EqualAUID( &kAAFCDCIHorizSubsampling, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    // 1, 2 or 4 are valid values
		    checkExpression( param.operand.expUInt32 == 1 ||
					param.operand.expUInt32 == 2 ||
					param.operand.expUInt32 == 4, AAFRESULT_BADPIXFORM );

		    _horizontalSubsampling = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFCDCIColorSiting, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expColorSiting), AAFRESULT_INVALID_PARM_SIZE);
			
			// 0, 1 or 2 are valid values
			checkExpression( param.operand.expColorSiting == 0 || 
					 param.operand.expColorSiting == 1 ||
					 param.operand.expColorSiting == 2, AAFRESULT_ILLEGAL_VALUE );

			// We currently only support 8 bit components...
			_colorSiting = param.operand.expColorSiting;
		}
		else if (EqualAUID(&kAAFCDCIBlackLevel, &param.opcode))
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _blackReferenceLevel = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFCDCIWhiteLevel, &param.opcode))
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _whiteReferenceLevel = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFCDCIColorRange, &param.opcode))
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _colorRange = param.operand.expUInt32;
		}
		else if( EqualAUID( &kAAFCDCIPadBits, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expInt16), AAFRESULT_INVALID_PARM_SIZE);

		    _paddingBits = param.operand.expInt16;
		}
		else if (EqualAUID(&kAAFPadBytesPerRow, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expUInt16), AAFRESULT_INVALID_PARM_SIZE);

			_padBytesPerRow = param.operand.expUInt16;
		}
		else if( EqualAUID( &kAAFFieldStartOffset, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _fieldStartOffset = param.operand.expUInt32;
		}
		else if( EqualAUID( &kAAFFieldEndOffset, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _fieldEndOffset = param.operand.expUInt32;
		}
		else if (EqualAUID( &kAAFSampleRate, &param.opcode ) )
		{
			// Validate the in-memory size.
			checkExpression( param.size == sizeof(param.operand.expRational), AAFRESULT_INVALID_PARM_SIZE);
			_sampleRate = param.operand.expRational;
		}
		else if (EqualAUID( &kAAFLegacyCDCI, &param.opcode ) )
		{
			memcpy( &_compression, &AAF_CMPR_AUNC422, 
			    sizeof(_compression) );
			_fieldStartOffset = 0;
			_fieldEndOffset = 4;
		}
		// Below are parameters which are accessible for client to read 
		// but not to modify. Some of them are constant for this codec
		// (like kAAFNumChannels), some are calculated values like 
		// kAAFMaxSampleBytes.
		// This implementation ignores them.
		else if( EqualAUID( &kAAFNumChannels, &param.opcode ) ||
			 EqualAUID( &kAAFPixelSize, &param.opcode ) ||
			 EqualAUID( &kAAFMaxSampleBytes, &param.opcode ) ||
			 EqualAUID( &kAAFWillTransferLines, &param.opcode ) ||
			 EqualAUID( &kAAFIsCompressed, &param.opcode ) )
		{
		    // Do nothing
		}
		else
		    throw HRESULT( AAFRESULT_ILLEGAL_FILEFMT );

		
	} // for (i = 0...)

	SetCodecState();

    }
    catch (HRESULT& rhr)
    {
	    hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	    // We CANNOT throw an exception out of a COM interface method!
	    // Return a reasonable exception code.
	    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

    // Cleanup

    return hr;
}


HRESULT STDMETHODCALLTYPE CAAFCDCICodec::GetEssenceFormat(
	IAAFEssenceFormat	*pTemplate,
	IAAFEssenceFormat	**pResult)
{
	plugin_trace("CAAFCDCICodec::GetEssenceFormat()\n");
    HRESULT			hr = S_OK;
    IAAFEssenceFormat		*p_fmt = NULL;
    aafInt32			numSpecifiers;
    aafInt32			i;
    aafEssenceFormatData_t	param;


    if( NULL == pTemplate || NULL == pResult )
	return AAFRESULT_NULL_PARAM;

    // Initialize out parameter in case of failure.
    *pResult = NULL;

    try
    {
	// Start with an "empty" file format specifier.
	checkResult( _access->GetEmptyFileFormat( &p_fmt ) );
	
	checkResult( pTemplate->NumFormatSpecifiers( &numSpecifiers ) );

	// Use the data in the given template to create and initialize
	// a corresponding data from the codec.
	for( i=0; i<numSpecifiers; i++ )
	{
	    memset( &param, 0, sizeof(param) );
	    checkResult( pTemplate->GetIndexedFormatSpecifier( 
		i, &param.opcode, sizeof(param.operand.expData), 
		param.operand.expData, &param.size ) );


	    if( EqualAUID( &kAAFCompression, &param.opcode) )
	    {
		// Write out current compression ID
		memcpy( &(param.operand.expUID), &_compression, 
		    sizeof(param.operand.expUID) );
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCompression, param.operand.expUID ) );
	    }
	    else if( EqualAUID( &kAAFPixelFormat, &param.opcode ) )
	    {
		// Write out the current pixel format.
		param.operand.expColorSpace = _pixelFormat;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelFormat, param.operand.expColorSpace ) );
	    }
	    else if( EqualAUID( &kAAFFrameLayout, &param.opcode ) )
	    {
		// Write out the current framelayout.
		param.operand.expFrameLayout = _frameLayout;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFrameLayout, param.operand.expFrameLayout ) );
	    }
	    else if( EqualAUID( &kAAFFieldDominance, &param.opcode ) )
	    {
		// Write out the current framelayout.
		param.operand.expFieldDominance = _fieldDominance;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldDominance, param.operand.expFieldDominance ));
	    }
	    else if( EqualAUID( &kAAFStoredRect, &param.opcode ) )
	    {
		// Write out the current image dimentions for 
		// the default stored rectangle.
		param.operand.expRect.xOffset = 0;
		param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _storedWidth;
		param.operand.expRect.ySize = _storedHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFStoredRect, param.operand.expRect ));
	    }
	    else if( EqualAUID( &kAAFSampledRect, &param.opcode ) )
	    {
		// Write out the current sampled rectangle.
		param.operand.expRect.xOffset = _sampledXOffset;
		param.operand.expRect.yOffset = _sampledYOffset;
		param.operand.expRect.xSize = _sampledWidth;
		param.operand.expRect.ySize = _sampledHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampledRect, param.operand.expRect ));
	    }
	    else if( EqualAUID( &kAAFDisplayRect, &param.opcode ) )
	    {
		// Write out the current displayed rectangle.
		param.operand.expRect.xOffset = _displayXOffset;
		param.operand.expRect.yOffset = _displayYOffset;
		param.operand.expRect.xSize = _displayWidth;
		param.operand.expRect.ySize = _displayHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFDisplayRect, param.operand.expRect ) );
	    }
	    else if( EqualAUID( &kAAFPixelSize, &param.opcode ) )
	    {
		/// Write out the current bitsPerPixel 
		// (make sure that this has been computed!
		checkAssertion( _bitsPerPixelAvg != 0 );
		param.operand.expInt16 = _bitsPerPixelAvg;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelSize, param.operand.expInt16 ) );
	    }
	    else if( EqualAUID( &kAAFAspectRatio, &param.opcode ) )
	    {
		// Write out the current aspect ratio.
		param.operand.expRational = _imageAspectRatio;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAspectRatio, param.operand.expRational ) );
	    }
	    else if( EqualAUID( &kAAFAlphaTransparency, &param.opcode ) )
	    {
		// Write out the current alpha transparency.
		param.operand.expAlphaTransparency = _alphaTransparency;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAlphaTransparency, 
		    param.operand.expAlphaTransparency ) );
	    }
	    else if( EqualAUID( &kAAFGamma, &param.opcode ) )
	    {
		// Write out the current gamma value.
		param.operand.expUID = _gamma;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFGamma, param.operand.expUID ) );
	    }
	    else if( EqualAUID( &kAAFImageAlignmentFactor, &param.opcode ) )
	    {
		// Write out the current alignment.
		param.operand.expUInt32 = _imageAlignmentFactor;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFImageAlignmentFactor, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFVideoLineMap, &param.opcode ) )
	    {
		// Write out both values from the video line map. 
		// The frameLayout will determine which entries are valid.
		param.operand.expVideoLineMap[0] = _videoLineMap[0];
		param.operand.expVideoLineMap[1] = _videoLineMap[1];
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFVideoLineMap, param.operand.expVideoLineMap ) );
	    }
	    else if( EqualAUID( &kAAFWillTransferLines, &param.opcode ) )
	    {
		// CDCI codec does not transfer separate lines.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFWillTransferLines, param.operand.expBoolean ) );
	    }
	    else if( EqualAUID( &kAAFIsCompressed, &param.opcode ) )
	    {
		// CDCI codec does not handle compressed data.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFIsCompressed, param.operand.expBoolean ) );
	    }
	    else if( EqualAUID( &kAAFCDCICompWidth, &param.opcode ) )
	    {
		param.operand.expUInt32 = _componentWidth;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCICompWidth, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFCDCIHorizSubsampling, &param.opcode ) )
	    {
		param.operand.expUInt32 = _horizontalSubsampling;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIHorizSubsampling, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFCDCIColorSiting, &param.opcode ) )
	    {
		// Write out the current color siting value.
		param.operand.expColorSiting = _colorSiting;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorSiting, param.operand.expColorSiting ) );
	    }
	    else if( EqualAUID( &kAAFCDCIBlackLevel, &param.opcode ) )
	    {
		// Write out the current black reference level.
		param.operand.expUInt32 = _blackReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIBlackLevel, param.operand.expUInt32));
	    }
	    else if( EqualAUID( &kAAFCDCIWhiteLevel, &param.opcode ) )
	    {
		// Write out the current white reference level.
		param.operand.expUInt32 = _whiteReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIWhiteLevel, param.operand.expUInt32));
	    }
	    else if( EqualAUID( &kAAFCDCIColorRange, &param.opcode ) )
	    {
		// Write out the current color range.
		param.operand.expUInt32 = _colorRange;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorRange, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFCDCIPadBits, &param.opcode ) )
	    {
		// Write out the current padding bits value.
		param.operand.expInt16 = _paddingBits;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIPadBits, param.operand.expInt16 ) );
	    }
	    else if( EqualAUID( &kAAFFieldStartOffset, &param.opcode ) )
	    {
		param.operand.expUInt32 = _fieldStartOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldStartOffset, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFFieldEndOffset, &param.opcode ) )
	    {
		param.operand.expUInt32 = _fieldEndOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldEndOffset, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFPadBytesPerRow, &param.opcode ) )
	    {
		// Write out the current pad bytes per row.
		param.operand.expUInt16 = _padBytesPerRow;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPadBytesPerRow, param.operand.expUInt16 ) );
	    }
	    else if( EqualAUID( &kAAFMaxSampleBytes, &param.opcode ) )
	    {
		// Write out the sample size.
		param.operand.expUInt32 = _apiFrameSampleSize;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFMaxSampleBytes, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFSampleRate, &param.opcode ) )
	    {
		// Write out the current sample rate value.
		param.operand.expRational = _sampleRate;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampleRate, param.operand.expRational ) );
	    }
	    else if( EqualAUID( &kAAFNumChannels, &param.opcode ) )
	    {
		/// We only support a single channel...
		param.operand.expUInt32 = kDefaultNumCh;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFNumChannels, param.operand.expUInt32 ) );
	    }
	    else
	    {	
		// The given opcode was not handled!
		throw HRESULT(AAFRESULT_INVALID_OP_CODEC);
	    }


	}  // for(i)


	// If the format specifier has been completely initialized
	// then return this value to the caller (clear local variable
	// so that we don't perform an extra release).
	*pResult = p_fmt;
	p_fmt = NULL;
    }
    catch (HRESULT& rhr)
    {
	hr = rhr; // return thrown error code.
    }
    catch (...)
    {
	// We CANNOT throw an exception out of a COM interface method!
	// Return a reasonable exception code.
	hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }


    // Cleanup
    if (NULL != p_fmt) // should delete the object on failure
	p_fmt->Release();



    return hr;

}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
	plugin_trace("CAAFCDCICodec::GetDefaultEssenceFormat()\n");
	HRESULT			hr = S_OK;
	IAAFEssenceFormat	*p_fmt = NULL;
	aafEssenceFormatData_t	param;

	
	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;


	// Initialize out parameter in case of failure.
	*pResult = NULL;



	try
	{
		// Start with an "empty" file format specifier.
		checkResult( _access->GetEmptyFileFormat( &p_fmt ) );
		

		// Write out current compression ID
		memcpy( &(param.operand.expUID), &_compression, 
		    sizeof(param.operand.expUID) );
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCompression, param.operand.expUID ) );

		// Write out the current pixel format.
		param.operand.expColorSpace = _pixelFormat;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelFormat, param.operand.expColorSpace ) );

		// Write out the current framelayout.
		param.operand.expFrameLayout = _frameLayout;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFrameLayout, param.operand.expFrameLayout ) );

		// Write out the current framelayout.
		param.operand.expFieldDominance = _fieldDominance;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldDominance, param.operand.expFieldDominance ));

		// Write out the current image dimentions for 
		// the default stored rectangle.
		param.operand.expRect.xOffset = 0;
		param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _storedWidth;
		param.operand.expRect.ySize = _storedHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFStoredRect, param.operand.expRect ));

		// Write out the current sampled rectangle.
		param.operand.expRect.xOffset = _sampledXOffset;
		param.operand.expRect.yOffset = _sampledYOffset;
		param.operand.expRect.xSize = _sampledWidth;
		param.operand.expRect.ySize = _sampledHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampledRect, param.operand.expRect ));

		// Write out the current displayed rectangle.
		param.operand.expRect.xOffset = _displayXOffset;
		param.operand.expRect.yOffset = _displayYOffset;
		param.operand.expRect.xSize = _displayWidth;
		param.operand.expRect.ySize = _displayHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFDisplayRect, param.operand.expRect ) );

		/// Write out the current bitsPerPixel 
		// (make sure that this has been computed!
		checkAssertion( _bitsPerPixelAvg != 0 );
		param.operand.expInt16 = _bitsPerPixelAvg;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelSize, param.operand.expInt16 ) );

		// Write out the current aspect ratio.
		param.operand.expRational = _imageAspectRatio;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAspectRatio, param.operand.expRational ) );

		// Write out the current alpha transparency.
		param.operand.expAlphaTransparency = _alphaTransparency;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAlphaTransparency, 
		    param.operand.expAlphaTransparency ) );

		// Write out the current gamma value.
		param.operand.expUID = _gamma;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFGamma, param.operand.expUID ) );

		// Write out the current alignment.
		param.operand.expUInt32 = _imageAlignmentFactor;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFImageAlignmentFactor, param.operand.expUInt32 ) );

		// Write out both values from the video line map. 
		// The frameLayout will determine which entries are valid.
		param.operand.expVideoLineMap[0] = _videoLineMap[0];
		param.operand.expVideoLineMap[1] = _videoLineMap[1];
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFVideoLineMap, param.operand.expVideoLineMap ) );

		// CDCI codec does not transfer separate lines.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFWillTransferLines, param.operand.expBoolean ) );

		// Is this compression supported?
		if (IsSupportedCompressionID(_compression))
			param.operand.expBoolean = kAAFTrue;
		else
			param.operand.expBoolean = kAAFFalse;
		
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFIsCompressed, param.operand.expBoolean ) );

		param.operand.expUInt32 = _componentWidth;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCICompWidth, param.operand.expUInt32 ) );

		param.operand.expUInt32 = _horizontalSubsampling;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIHorizSubsampling, param.operand.expUInt32 ) );

		// Write out the current color siting value.
		param.operand.expColorSiting = _colorSiting;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorSiting, param.operand.expColorSiting ) );

		// Write out the current black reference level.
		param.operand.expUInt32 = _blackReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIBlackLevel, param.operand.expUInt32));

		// Write out the current white reference level.
		param.operand.expUInt32 = _whiteReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIWhiteLevel, param.operand.expUInt32));

		// Write out the current color range.
		param.operand.expUInt32 = _colorRange;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorRange, param.operand.expUInt32 ) );

		// Write out the current padding bits value.
		param.operand.expInt16 = _paddingBits;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIPadBits, param.operand.expInt16 ) );

		param.operand.expUInt32 = _fieldStartOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldStartOffset, param.operand.expUInt32 ) );

		param.operand.expUInt32 = _fieldEndOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldEndOffset, param.operand.expUInt32 ) );

		// Write out the current pad bytes per row.
		param.operand.expUInt16 = _padBytesPerRow;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPadBytesPerRow, param.operand.expUInt16 ) );

		// Write out the sample size.
		param.operand.expUInt32 = _apiFrameSampleSize;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFMaxSampleBytes, param.operand.expUInt32 ) );

		// Write out the current sample rate value.
		param.operand.expRational = _sampleRate;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampleRate, param.operand.expRational ) );

		/// We only support a single channel...
		param.operand.expUInt32 = kDefaultNumCh;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFNumChannels, param.operand.expUInt32 ) );

		// If the format specifier has been completely initialized
		// then return this value to the caller (clear local variable
		// so that we don't perform an extra release).
		*pResult = p_fmt;
		p_fmt = NULL;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if (NULL != p_fmt) // should delete the object on failure
		p_fmt->Release();


	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	plugin_trace("CAAFCDCICodec::GetEssenceDescriptorID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFCDCIDescriptor; // stored class UID of Compressed Digital Image Decriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetEssenceDataID (aafUID_t *uid)
{
	plugin_trace("CAAFCDCICodec::GetEssenceDataID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFEssenceData;			// stored class UID of the CDCI data object

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE CAAFCDCICodec::GetIndexedSampleSize (
    aafUID_constref	dataDefID,
    aafPosition_t	pos,
    aafLength_t		*pResult)
{
	plugin_trace("CAAFCDCICodec::GetIndexedSampleSize()\n");
	if(pos < 0 || pos > _numberOfSamples) // zero based sample index.
		return AAFRESULT_EOF;


	// All samlples are the same... 
	return GetLargestSampleSize( dataDefID, pResult );
}



HRESULT STDMETHODCALLTYPE CAAFCDCICodec::GetLargestSampleSize(
    aafUID_constref	dataDefID,
    aafLength_t		*pResult )
{
	plugin_trace("CAAFCDCICodec::GetLargestSampleSize()\n");
	HRESULT hr = AAFRESULT_SUCCESS;


	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = 0;

	try
	{
		if(EqualAUID(&dataDefID, &kAAFDataDef_Picture)
			|| EqualAUID(&dataDefID, &kAAFDataDef_LegacyPicture))
		{ 
			// The samples will be uncompressed so return the 
			// previously computed value for a single 
			// uncompressed sample.
			*pResult = _apiFrameSampleSize;
		}
		else
			hr = AAFRESULT_CODEC_CHANNELS;
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	return hr;
}



//
// COM Infrastructure
// 

// What interfaces does this plugin support
// Override of CAAFUnknown method.
HRESULT CAAFCDCICodec::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    plugin_trace("CAAFCDCICodec::InternalQueryInterface()\n");
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IAAFEssenceCodec interface 
    if( aafIsEqualIID( riid, IID_IAAFEssenceCodec ) )
    { 
        *ppvObj = (IAAFEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    // and the IAAFEssenceCodec2 interface 
    else if( aafIsEqualIID( riid, IID_IAAFEssenceCodec2 ) )
    { 
        *ppvObj = (IAAFEssenceCodec2 *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    // and the IAAFEssenceCodec3 interface 
    else if( aafIsEqualIID( riid, IID_IAAFEssenceCodec3 ) )
    { 
        *ppvObj = (IAAFEssenceCodec3 *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    // and the IAAFPlugin interface.
    else if( aafIsEqualIID( riid, IID_IAAFPlugin ) )
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFCDCICodec)
