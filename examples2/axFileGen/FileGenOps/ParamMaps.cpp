//=---------------------------------------------------------------------=
//
// $Id: ParamMaps.cpp,v 1.17 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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
//=---------------------------------------------------------------------=

// FIXME - This has generic utility - move to axLib.

//=---------------------------------------------------------------------=

#ifdef WIN32
#pragma warning(disable:4660)
// "template-class specialization 'ParamMap<struct _aafUID_t,class _ContainerDefsTag>'
// is already instantiated"  etc for the other instances.
#endif

#include "ParamMaps.h"

#include <axFileGen.h>

#include <AAFContainerDefs.h>
#include <AAFCodecDefs.h>
#include <AAFDataDefs.h>
#include <AAFOperationDefs.h>

//=---------------------------------------------------------------------=

template <class Type, class Tag>
ParamMap<Type,Tag>* ParamMap<Type,Tag>::_singleInstance = 0;

template <class Type, class Tag>
ParamMap<Type,Tag>::ParamMap()
{}

template <class Type, class Tag>
ParamMap<Type,Tag>::~ParamMap()
{}

template <class Type, class Tag>
ParamMap<Type,Tag>& ParamMap<Type,Tag>::GetInstance()
{
	if ( !_singleInstance ) {
		_singleInstance = new ParamMap();
	}

	return *_singleInstance;
}	

template <class Type, class Tag>
Type ParamMap<Type,Tag>::Find( AxFGOp& op, const AxString& name )
{
	MapIterType iter;

	iter = _map.find( name );
	if ( iter == _map.end() ) {
		throw AxFGOpUsageEx( op, L"\"" + name + L"\"" + L" unknown, try: " +
							  GenerateNameList() );
	}

	return _map[ name ];
}

template <class Type, class Tag>
AxString ParamMap<Type,Tag>::GenerateNameList()
{
	MapIterType iter;
	AxString listAsString ( L"" );

	for ( iter = _map.begin();
		  iter != _map.end();
		  ++iter ) {

		listAsString += L" " + iter->first;
		  
	}

	return listAsString;
}

template <class Type, class Tag>
void ParamMap<Type,Tag>::Add( AxString name, const Type& val )
{
	// If _map[name] exists, the old value is simply replaced.
	// This is intentional.
	_map[name] = val;
}

//=---------------------------------------------------------------------=
//=---------------------------------------------------------------------=

template <>
ParamMap< aafUID_t, _ContainerDefsTag >::ParamMap()
{
	_map[ L"ContainerAAF" ]  = ContainerAAF;
	_map[ L"ContainerFile" ] = ContainerFile;
	_map[ L"ContainerOMF" ]  = ContainerOMF;
}

template class ParamMap< aafUID_t, _ContainerDefsTag >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafCompressEnable_t, aafCompressEnable_e >::ParamMap()
{
	_map[ L"kAAFCompressionEnable" ]      = kAAFCompressionEnable;
	_map[ L"CompressionEnable" ]          = kAAFCompressionEnable;
	_map[ L"Enable" ]                     = kAAFCompressionEnable;

	_map[ L"kAAFCompressionDisable" ]    = kAAFCompressionDisable;
	_map[ L"CompressionDisable" ]        = kAAFCompressionDisable;
	_map[ L"Disable" ]                   = kAAFCompressionDisable;
}


template class ParamMap< aafCompressEnable_t, aafCompressEnable_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafUID_t, _CodecIDTag >::ParamMap()
{
	_map[ L"kAAFNoCodec" ]         = kAAFNoCodec;
	_map[ L"NoCodec" ]             = kAAFNoCodec;

	_map[ L"kAAFCodecWAVE" ]       = kAAFCodecWAVE;
	_map[ L"CodecWAVE" ]           = kAAFCodecWAVE;

	_map[ L"kAAFCODEC_AIFC" ]       = kAAFCODEC_AIFC;
	_map[ L"CODEC_AIFC" ]           = kAAFCODEC_AIFC;
    // It just had to be different - bring it in line.
	_map[ L"CodecAIFC" ]            = kAAFCODEC_AIFC;

	_map[ L"kAAFCodecJPEG" ]       = kAAFCodecJPEG;
	_map[ L"CodecJPEG" ]           = kAAFCodecJPEG;

	_map[ L"kAAFCodecCDCI" ]       = kAAFCodecCDCI;
	_map[ L"CodecCDCI" ]           = kAAFCodecCDCI;

	_map[ L"kAAFCodecRGBA" ]       = kAAFCodecRGBA;
	_map[ L"CodecRGBA" ]           = kAAFCodecRGBA;

	_map[ L"kAAFNilCodecFlavour" ] = kAAFNilCodecFlavour;
	_map[ L"NilCodecFlavour" ]     = kAAFNilCodecFlavour;
}

template class ParamMap< aafUID_t, _CodecIDTag >;



//=---------------------------------------------------------------------=

template <>
ParamMap< aafTapeCaseType_t, aafTapeCaseType_e >::ParamMap()
{
	_map[ L"kAAFTapeCaseNull"]              = kAAFTapeCaseNull;
	_map[ L"TapeCaseNull"]					= kAAFTapeCaseNull;
	_map[ L"Null"]					        = kAAFTapeCaseNull;

	_map[ L"kAAFThreeFourthInchVideoTape" ] = kAAFThreeFourthInchVideoTape;
	_map[ L"ThreeFourthInchVideoTape" ]		= kAAFThreeFourthInchVideoTape;
	_map[ L"ThreeFourthInch" ]				= kAAFThreeFourthInchVideoTape;

	_map[ L"kAAFVHSVideoTape" ]             = kAAFVHSVideoTape,
	_map[ L"VHSVideoTape" ]					= kAAFVHSVideoTape,
	_map[ L"VHS" ]							= kAAFVHSVideoTape,

	_map[ L"kAAF8mmVideoTape" ]             = kAAF8mmVideoTape,
	_map[ L"F8mmVideoTape" ]				= kAAF8mmVideoTape,
	_map[ L"F8mm" ]							= kAAF8mmVideoTape,

	_map[ L"kAAFBetacamVideoTape" ]         = kAAFBetacamVideoTape,
	_map[ L"BetacamVideoTape" ]				= kAAFBetacamVideoTape,
	_map[ L"Betacam" ]						= kAAFBetacamVideoTape,

	_map[ L"kAAFCompactCassette" ]          = kAAFCompactCassette,
	_map[ L"CompactCassette" ]				= kAAFCompactCassette,

	_map[ L"kAAFDATCartridge" ]             = kAAFDATCartridge,
	_map[ L"DATCartridge" ] 				= kAAFDATCartridge,
	_map[ L"DATCartridge" ] 				= kAAFDATCartridge,

	_map[ L"kAAFNagraAudioTape" ]           = kAAFNagraAudioTape;
	_map[ L"NagraAudioTape" ]				= kAAFNagraAudioTape;
	_map[ L"NagraAudio" ]					= kAAFNagraAudioTape;
}

template class ParamMap< aafTapeCaseType_t, aafTapeCaseType_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafVideoSignalType_t, aafVideoSignalType_e >::ParamMap()
{
	_map[ L"kAAFVideoSignalNull" ] = kAAFVideoSignalNull;
	_map[ L"VideoSignalNull" ]     = kAAFVideoSignalNull;
	_map[ L"Null" ]                = kAAFVideoSignalNull;

	_map[ L"kAAFNTSCSignal" ]      = kAAFNTSCSignal;
	_map[ L"NTSCSignal" ]          = kAAFNTSCSignal;
	_map[ L"NTSC" ]                = kAAFNTSCSignal;

	_map[ L"kAAFPALSignal" ]       = kAAFPALSignal;
	_map[ L"PALSignal" ]           = kAAFPALSignal;
	_map[ L"PAL" ]                 = kAAFPALSignal;

	_map[ L"kAAFSECAMSignal" ]     = kAAFSECAMSignal;
	_map[ L"SECAMSignal" ]         = kAAFSECAMSignal;
	_map[ L"SECAM" ]               = kAAFSECAMSignal;
}

template class ParamMap< aafVideoSignalType_t, aafVideoSignalType_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafTapeFormatType_t, aafTapeFormatType_e >::ParamMap()
{
	_map[ L"kAAFTapeFormatNull" ]	= kAAFTapeFormatNull;
	_map[ L"TapeFormatNull" ]		= kAAFTapeFormatNull;
	_map[ L"Null" ]					= kAAFTapeFormatNull;

	_map[ L"kAAFBetacamFormat" ]    = kAAFBetacamFormat;
	_map[ L"BetacamFormat" ]		= kAAFBetacamFormat;
	_map[ L"Betacam" ]				= kAAFBetacamFormat;

	_map[ L"kAAFBetacamSPFormat" ]  = kAAFBetacamSPFormat;
	_map[ L"BetacamSPFormat" ]		= kAAFBetacamSPFormat;
	_map[ L"BetacamSP" ]			= kAAFBetacamSPFormat;

	_map[ L"kAAFVHSFormat" ]        = kAAFVHSFormat;
	_map[ L"VHSFormat" ]			= kAAFVHSFormat;
	_map[ L"VHS" ]					= kAAFVHSFormat;

	_map[ L"kAAFSVHSFormat" ]       = kAAFVHSFormat;
	_map[ L"SVHSFormat" ]			= kAAFVHSFormat;
	_map[ L"SVHS" ]					= kAAFVHSFormat;

	_map[ L"kAAF8mmFormat" ]        = kAAF8mmFormat;
	_map[ L"8mmFormat" ]			= kAAF8mmFormat;
	_map[ L"8mm" ]					= kAAF8mmFormat;

	_map[ L"kAAFHi8Format" ]        = kAAFHi8Format;
	_map[ L"Hi8Format" ]			= kAAFHi8Format;
	_map[ L"Hi8" ]					= kAAFHi8Format;
}

template class ParamMap< aafTapeFormatType_t, aafTapeFormatType_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafUID_t, _DDefTag >::ParamMap()
{
	_map[ L"kAAFDataDef_Picture" ]          = kAAFDataDef_Picture;
	_map[ L"Picture" ]               = kAAFDataDef_Picture;

	_map[ L"kAAFDataDef_Matte" ]            = kAAFDataDef_Matte;
	_map[ L"Matte" ]                 = kAAFDataDef_Matte;

	_map[ L"kAAFDataDef_PictureWithMatte" ] = kAAFDataDef_PictureWithMatte;
	_map[ L"PictureWithMatte" ]      = kAAFDataDef_PictureWithMatte;

	_map[ L"kAAFDataDef_Sound" ]            = kAAFDataDef_Sound;
	_map[ L"Sound" ]                 = kAAFDataDef_Sound;

	_map[ L"kAAFDataDef_Timecode" ]         = kAAFDataDef_Timecode;
	_map[ L"Timecode" ]              = kAAFDataDef_Timecode;

	_map[ L"kAAFDataDef_Edgecode" ]         = kAAFDataDef_Edgecode;
	_map[ L"Edgecode" ]              = kAAFDataDef_Edgecode;
		
	_map[ L"kAAFDataDef_Unknown" ]          = kAAFDataDef_Unknown;
	_map[ L"Unknown" ]               = kAAFDataDef_Unknown;
}

template class ParamMap< aafUID_t, _DDefTag >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafFrameLayout_t, aafFrameLayout_e >::ParamMap()
{
	_map[ L"kAAFFullFrame" ]      = kAAFFullFrame;
	_map[ L"FullFrame" ]          = kAAFFullFrame;

	_map[ L"kAAFSeparateFields" ] = kAAFSeparateFields;
	_map[ L"SeparateFields" ]     = kAAFSeparateFields;

	_map[ L"kAAFOneField" ]       = kAAFOneField;
	_map[ L"OneField" ]           = kAAFOneField;

	_map[ L"kAAFMixedFields" ]    = kAAFMixedFields;
	_map[ L"MixedFields" ]        = kAAFMixedFields;
}

template class ParamMap< aafFrameLayout_t, aafFrameLayout_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafColorSpace_t, aafColorSpace_e >::ParamMap()
{
 	_map[ L"kAAFColorSpaceRGB" ]   = kAAFColorSpaceRGB;
 	_map[ L"ColorSpaceRGB" ]       = kAAFColorSpaceRGB;
 	_map[ L"RGB" ]                 = kAAFColorSpaceRGB;

	_map[ L"kAAFColorSpaceYUV" ]   = kAAFColorSpaceYUV;
	_map[ L"ColorSpaceYUV" ]       = kAAFColorSpaceYUV;
	_map[ L"YUV" ]                 = kAAFColorSpaceYUV;

	_map[ L"kAAFColorSpaceYIQ" ]   = kAAFColorSpaceYIQ;
	_map[ L"ColorSpaceYIQ" ]       = kAAFColorSpaceYIQ;
	_map[ L"YIQ" ]                 = kAAFColorSpaceYIQ;

	_map[ L"kAAFColorSpaceHSI" ]   = kAAFColorSpaceHSI;
	_map[ L"ColorSpaceHSI" ]       = kAAFColorSpaceHSI;
	_map[ L"HSI" ]                 = kAAFColorSpaceHSI;

	_map[ L"kAAFColorSpaceHSV" ]   = kAAFColorSpaceHSV;
	_map[ L"ColorSpaceHSV" ]       = kAAFColorSpaceHSV;
	_map[ L"HSV" ]                 = kAAFColorSpaceHSV;

	_map[ L"kAAFColorSpaceYCrCb" ] = kAAFColorSpaceYCrCb;
	_map[ L"ColorSpaceYCrCb" ]     = kAAFColorSpaceYCrCb;
	_map[ L"YCrCb" ]               = kAAFColorSpaceYCrCb;

	_map[ L"kAAFColorSpaceYDrDb" ] = kAAFColorSpaceYDrDb;
	_map[ L"ColorSpaceYDrDb" ]     = kAAFColorSpaceYDrDb;
	_map[ L"YDrDb" ]               = kAAFColorSpaceYDrDb;

	_map[ L"kAAFColorSpaceCMYK" ]  = kAAFColorSpaceCMYK;
	_map[ L"ColorSpaceCMYK" ]      = kAAFColorSpaceCMYK;
	_map[ L"CMYK" ]                = kAAFColorSpaceCMYK;
}

template class ParamMap< aafColorSpace_t, aafColorSpace_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafPixelFormat_t, aafPixelFormat_e >::ParamMap()
{
	_map[ L"kAAFPixNone" ] = kAAFPixNone;
	_map[ L"PixNone" ]     = kAAFPixNone;
	_map[ L"None" ]        = kAAFPixNone;

	_map[ L"kAAFPixRGBA" ] = kAAFPixRGBA;
	_map[ L"PixRGBA" ]     = kAAFPixRGBA;
	_map[ L"RGBA" ]        = kAAFPixRGBA;

	_map[ L"kAAFPixYUV" ]  = kAAFPixYUV;
	_map[ L"PixYUV" ]      = kAAFPixYUV;
	_map[ L"YUV" ]         = kAAFPixYUV;
}

template class ParamMap< aafPixelFormat_t, aafPixelFormat_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafDropType_t, aafDropType_e >::ParamMap()
{
	_map[ L"kAAFTcNonDrop" ] = kAAFTcNonDrop;
	_map[ L"TcNonDrop" ]     = kAAFTcNonDrop;
	_map[ L"NonDrop" ]       = kAAFTcNonDrop;

	_map[ L"kAAFTcDrop" ] = kAAFTcDrop;
	_map[ L"TcDrop" ]     = kAAFTcDrop;
	_map[ L"Drop" ]       = kAAFTcDrop;
}

template class ParamMap< aafDropType_t, aafDropType_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafBoolean_t, aafBoolean_e >::ParamMap()
{
	_map[ L"kAAFFalse" ] = kAAFFalse;
	_map[ L"False" ]     = kAAFFalse;
	_map[ L"FALSE" ]     = kAAFFalse;
	_map[ L"false" ]     = kAAFFalse;

	_map[ L"kAAFTrue" ]  = kAAFTrue;
	_map[ L"True" ]      = kAAFTrue;
	_map[ L"TRUE" ]      = kAAFTrue;
	_map[ L"true" ]      = kAAFTrue;
}

template class ParamMap< aafBoolean_t, aafBoolean_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafUID_t, _OperationDefTag >::ParamMap()
{

	_map[ L"kAAFEffectVideoDissolve" ]       = kAAFEffectVideoDissolve;
	_map[ L"EffectVideoDissolve" ]           = kAAFEffectVideoDissolve;
	_map[ L"VideoDissolve" ]                 = kAAFEffectVideoDissolve;

	_map[ L"kAAFEffectMonoAudioDissolve" ]   = kAAFEffectMonoAudioDissolve;
	_map[ L"EffectMonoAudioDissolve" ]       = kAAFEffectMonoAudioDissolve;
	_map[ L"MonoAudioDissolve" ]             = kAAFEffectMonoAudioDissolve;

	_map[ L"kAAFEffectStereoAudioDissolve" ] = kAAFEffectStereoAudioDissolve;
	_map[ L"EffectStereoAudioDissolve" ]     = kAAFEffectStereoAudioDissolve;
	_map[ L"StereoAudioDissolve" ]           = kAAFEffectStereoAudioDissolve;

	_map[ L"kAAFEffectVideoFadeToBlack" ]    = kAAFEffectVideoFadeToBlack;
	_map[ L"EffectVideoFadeToBlack" ]        = kAAFEffectVideoFadeToBlack;
	_map[ L"StereoAudioDissolve" ]           = kAAFEffectStereoAudioDissolve;

	_map[ L"kAAFEffectSMPTEVideoWipe" ]      = kAAFEffectSMPTEVideoWipe;
	_map[ L"EffectSMPTEVideoWipe" ]          = kAAFEffectSMPTEVideoWipe;
	_map[ L"SMPTEVideoWipe" ]                = kAAFEffectSMPTEVideoWipe;

	_map[ L"kAAFEffectPictureWithMate" ]     = kAAFEffectPictureWithMate;
	_map[ L"EffectPictureWithMate" ]         = kAAFEffectPictureWithMate;
	_map[ L"PictureWithMate" ]               = kAAFEffectPictureWithMate;

	_map[ L"kAAFEffectVideoSpeedControl" ]   = kAAFEffectVideoSpeedControl;
	_map[ L"EffectVideoSpeedControl" ]       = kAAFEffectVideoSpeedControl;
	_map[ L"VideoSpeedControl" ]             = kAAFEffectVideoSpeedControl;

	_map[ L"kAAFEffectVideoRepeat" ]         = kAAFEffectVideoRepeat;
	_map[ L"EffectVideoRepeat" ]             = kAAFEffectVideoRepeat;
	_map[ L"VideoRepeat" ]                   = kAAFEffectVideoRepeat;

	_map[ L"kAAFEffectVideoFrameToMask" ]    = kAAFEffectVideoFrameToMask;
	_map[ L"EffectVideoFrameToMask" ]        = kAAFEffectVideoFrameToMask;
	_map[ L"VideoFrameToMask" ]              = kAAFEffectVideoFrameToMask;

	_map[ L"kAAFEffectMonoAudioPan" ]        = kAAFEffectMonoAudioPan;
	_map[ L"EffectMonoAudioPan" ]            = kAAFEffectMonoAudioPan;
	_map[ L"MonoAudioPan" ]                  = kAAFEffectMonoAudioPan;

	_map[ L"kAAFEffectMonoAudioGain" ]       = kAAFEffectMonoAudioGain;
	_map[ L"EffectMonoAudioGain" ]           = kAAFEffectMonoAudioGain;
	_map[ L"MonoAudioGain" ]                 = kAAFEffectMonoAudioGain;

	_map[ L"kAAFEffectStereoAudioGain" ]     = kAAFEffectStereoAudioGain;
	_map[ L"EffectStereoAudioGain" ]         = kAAFEffectStereoAudioGain;
	_map[ L"StereoAudioGain" ]               = kAAFEffectStereoAudioGain;

	_map[ L"kAAFEffectMonoAudioMixdown" ]    = kAAFEffectMonoAudioMixdown;
	_map[ L"EffectMonoAudioMixdown" ]        = kAAFEffectMonoAudioMixdown;
	_map[ L"MonoAudioMixdown" ]              = kAAFEffectMonoAudioMixdown;

	_map[ L"kAAFOperationUnknown" ]          = kAAFOperationUnknown;
	_map[ L"OperationUnknown" ]              = kAAFOperationUnknown;
}

template class ParamMap< aafUID_t, _OperationDefTag >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafIncMedia_t, aafIncMedia_e >::ParamMap()
{
	_map[ L"kAAFNoIncludeMedia" ] = kAAFNoIncludeMedia;
	_map[ L"NoIncludeMedia" ]     = kAAFNoIncludeMedia;

	_map[ L"kAAFIncludeMedia" ]   = kAAFIncludeMedia;
	_map[ L"IncludeMedia" ]       = kAAFIncludeMedia;
}

template class ParamMap< aafIncMedia_t, aafIncMedia_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafDepend_t, aafDepend_e >::ParamMap()
{
	_map[ L"kAAFNoFollowDepend" ] = kAAFNoFollowDepend;
	_map[ L"NoFollowDepend" ]     = kAAFNoFollowDepend;

	_map[ L"kAAFFollowDepend" ]   = kAAFFollowDepend;
	_map[ L"FollowDepend" ]       = kAAFFollowDepend;
}

template class ParamMap< aafDepend_t, aafDepend_e >;

//=---------------------------------------------------------------------=

template <>
ParamMap< aafFileAccess_t, aafFileAccess_e >::ParamMap()
{
  _map[ L"kAAFFileAccess_none" ]   = kAAFFileAccess_none;
  _map[ L"none" ]                  = kAAFFileAccess_none;

  _map[ L"kAAFFileAccess_read" ]   = kAAFFileAccess_read;
  _map[ L"read" ]                  = kAAFFileAccess_read;

  _map[ L"kAAFFileAccess_write" ]  = kAAFFileAccess_write;
  _map[ L"write" ]                 = kAAFFileAccess_write;

  _map[ L"kAAFFileAccess_modify" ] = kAAFFileAccess_modify;
  _map[ L"modify" ]                = kAAFFileAccess_modify;
}

template class ParamMap< aafFileAccess_t, aafFileAccess_e >;

//=---------------------------------------------------------------------=

#if 0
// Recipe:
// 1) Copy this example.
// 2) Define _TemplateTag to something unique and appropriate.  If the
// type is an enum typedef, then the underlying enum type is a good
// tag to use. (i.e. if X_t is the type, then X_e usually exists).
// Else, invent a tag name.  A forward declaration of a non existant
// class is useful for this.
// 3) Change Type to what ever you need.
// 4) In the constructor, initialize the _map;
// 5) Move the _TemplateTag declaration and typedef to the header
// file.

class _TemplateTag;
typedef ParamMap< Type, _TemplateTag > TemplateParams;

template <>
ParamMap< Type, _TemplateTag >::ParamMap()
{
	_map[ L"NAME" ] = 0;
}

template class ParamMap< Type, _TemplateTag >;


#endif

//=---------------------------------------------------------------------=
//=---------------------------------------------------------------------=
