//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFEnumValidation.cpp,v 1.14 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "CAAFEnumValidation.h"

bool Is_AAFDataDefinitionMatch_t_Valid(AAFDataDefinitionMatch_t param)
{
  switch (param) {
    case kAAFExactMatch:
    case kAAFConvertTo:
    case kAAFConvertFrom:
      return true;
    default:
      return false;
  }
}

bool Is_aafAlphaTransparency_t_Valid(aafAlphaTransparency_t param)
{
  switch (param) {
    case kAAFMinValueTransparent:
    case kAAFMaxValueTransparent:
      return true;
    default:
      return false;
  }
}

bool Is_aafAppendOption_t_Valid(aafAppendOption_t param)
{
  switch (param) {
    case kAAFAppend:
    case kAAFForceOverwrite:
      return true;
    default:
      return false;
  }
}

bool Is_aafAttributeKind_t_Valid(aafAttributeKind_t param)
{
  switch (param) {
    case kAAFNullAttribute:
    case kAAFIntegerAttribute:
    case kAAFStringAttribute:
    case kAAFObjectAttribute:
      return true;
    default:
      return false;
  }
}

bool Is_aafAudioSampleType_t_Valid(aafAudioSampleType_t param)
{
  switch (param) {
    case kAAFOffsetBinary:
    case kAAFSignedMagnitude:
      return true;
    default:
      return false;
  }
}

bool Is_aafAuxBitsModeType_t_Valid(aafAuxBitsModeType_t param)
{
  switch (param) {
    case kAAFAuxBitsMode_NotDefined:
    case kAAFAuxBitsMode_MainAudioSampleData:
    case kAAFAuxBitsMode_SingleCoordinationSignal:
    case kAAFAuxBitsMode_UserDefined:
    case kAAFAuxBitsMode_Reserved0:
    case kAAFAuxBitsMode_Reserved1:
    case kAAFAuxBitsMode_Reserved2:
    case kAAFAuxBitsMode_Reserved3:
      return true;
    default:
      return false;
  }
}

bool Is_aafBoolean_t_Valid(aafBoolean_t param)
{
  switch (param) {
    case kAAFFalse:
    case kAAFTrue:
      return true;
    default:
      return false;
  }
}

bool Is_aafChannelStatusModeType_t_Valid(aafChannelStatusModeType_t param)
{
  switch (param) {
    case kAAFChannelStatusMode_None:
    case kAAFChannelStatusMode_Minimum:
    case kAAFChannelStatusMode_Standard:
    case kAAFChannelStatusMode_Fixed:
    case kAAFChannelStatusMode_Stream:
    case kAAFChannelStatusMode_Essence:
      return true;
    default:
      return false;
  }
}

bool Is_aafCheckVerbose_t_Valid(aafCheckVerbose_t param)
{
  switch (param) {
    case kAAFCheckVerbose:
    case kAAFCheckQuiet:
      return true;
    default:
      return false;
  }
}

bool Is_aafCheckWarnings_t_Valid(aafCheckWarnings_t param)
{
  switch (param) {
    case kAAFCheckPrintWarnings:
    case kAAFCheckNoWarnings:
      return true;
    default:
      return false;
  }
}

bool Is_aafColorSiting_t_Valid(aafColorSiting_t param)
{
  switch (param) {
    case kAAFCoSiting:
    case kAAFAveraging:
    case kAAFThreeTap:
    case kAAFQuincunx:
    case kAAFRec601:
    case kAAFUnknownSiting:
      return true;
    default:
      return false;
  }
}

bool Is_aafColorSpace_t_Valid(aafColorSpace_t param)
{
  switch (param) {
    case kAAFColorSpaceRGB:
    case kAAFColorSpaceYUV:
    case kAAFColorSpaceYIQ:
    case kAAFColorSpaceHSI:
    case kAAFColorSpaceHSV:
    case kAAFColorSpaceYCrCb:
    case kAAFColorSpaceYDrDb:
    case kAAFColorSpaceCMYK:
      return true;
    default:
      return false;
  }
}

bool Is_aafCompressEnable_t_Valid(aafCompressEnable_t param)
{
  switch (param) {
    case kAAFCompressionEnable:
    case kAAFCompressionDisable:
      return true;
    default:
      return false;
  }
}

bool Is_aafContentScanningType_t_Valid(aafContentScanningType_t param)
{
  switch (param) {
    case kAAFContentScanning_NotKnown:
    case kAAFContentScanning_Progressive:
    case kAAFContentScanning_Interlace:
    case kAAFContentScanning_Mixed:
      return true;
    default:
      return false;
  }
}

bool Is_aafCriteriaType_t_Valid(aafCriteriaType_t param)
{
  switch (param) {
    case kAAFAnyRepresentation:
    case kAAFFastestRepresentation:
    case kAAFBestFidelityRepresentation:
    case kAAFSmallestRepresentation:
      return true;
    default:
      return false;
  }
}

bool Is_aafDefinitionCritType_t_Valid(aafDefinitionCritType_t param)
{
  switch (param) {
    case kAAFNoDefSearch:
    case kAAFByDefKind:
    case kAAFByDefName:
    case kAAFByDefClass:
      return true;
    default:
      return false;
  }
}

bool Is_aafDefinitionKind_t_Valid(aafDefinitionKind_t param)
{
  switch (param) {
    case kAAFClassDefinition:
    case kAAFPropertyDefinition:
    case kAAFTypeDefinition:
    case kAAFDataDefinition:
    case kAAFOperationDefinition:
    case kAAFAllDefinitions:
      return true;
    default:
      return false;
  }
}

bool Is_aafDeinterleave_t_Valid(aafDeinterleave_t param)
{
  switch (param) {
    case kAAFdeinterleave:
    case kAAFleaveInterleaved:
      return true;
    default:
      return false;
  }
}

bool Is_aafDepend_t_Valid(aafDepend_t param)
{
  switch (param) {
    case kAAFNoFollowDepend:
    case kAAFFollowDepend:
      return true;
    default:
      return false;
  }
}

bool Is_aafDirectionCode_t_Valid(aafDirectionCode_t param)
{
  switch (param) {
    case kAAFDirCodeRight:
    case kAAFDirCodeUpperRight:
    case kAAFDirCodeUp:
    case kAAFDirCodeUpperLeft:
    case kAAFDirCodeLeft:
    case kAAFDirCodeLowerLeft:
    case kAAFDirCodeDown:
    case kAAFDirCodeLowerRight:
      return true;
    default:
      return false;
  }
}

bool Is_aafDropType_t_Valid(aafDropType_t param)
{
  switch (param) {
    case kAAFTcNonDrop:
    case kAAFTcDrop:
      return true;
    default:
      return false;
  }
}

bool Is_aafEdgeType_t_Valid(aafEdgeType_t param)
{
  switch (param) {
    case kAAFEtNull:
    case kAAFEtKeycode:
    case kAAFEtEdgenum4:
    case kAAFEtEdgenum5:
    case kAAFEtHeaderSize:
      return true;
    default:
      return false;
  }
}

bool Is_aafEditHint_t_Valid(aafEditHint_t param)
{
  switch (param) {
    case kAAFNoEditHint:
    case kAAFProportional:
    case kAAFRelativeLeft:
    case kAAFRelativeRight:
    case kAAFRelativeFixed:
      return true;
    default:
      return false;
  }
}

bool Is_aafElectroSpatialFormulation_t_Valid(aafElectroSpatialFormulation_t param)
{
  switch (param) {
    case kAAFElectroSpatialFormulation_Default:
    case kAAFElectroSpatialFormulation_TwoChannelMode:
    case kAAFElectroSpatialFormulation_SingleChannelMode:
    case kAAFElectroSpatialFormulation_PrimarySecondaryMode:
    case kAAFElectroSpatialFormulation_StereophonicMode:
    case kAAFElectroSpatialFormulation_SingleChannelDoubleSamplingFrequencyMode:
    case kAAFElectroSpatialFormulation_StereoLeftChannelDoubleSamplingFrequencyMode:
    case kAAFElectroSpatialFormulation_StereoRightChannelDoubleSamplingFrequencyMode:
    case kAAFElectroSpatialFormulation_MultiChannelMode:
      return true;
    default:
      return false;
  }
}

bool Is_aafEmphasisType_t_Valid(aafEmphasisType_t param)
{
  switch (param) {
    case kAAFEmphasis_Unknown:
    case kAAFEmphasis_Reserved0:
    case kAAFEmphasis_Reserved1:
    case kAAFEmphasis_Reserved2:
    case kAAFEmphasis_None:
    case kAAFEmphasis_Reserved3:
    case kAAFEmphasis_15and50:
    case kAAFEmphasis_ITU:
      return true;
    default:
      return false;
  }
}

bool Is_aafFadeType_t_Valid(aafFadeType_t param)
{
  switch (param) {
    case kAAFFadeNone:
    case kAAFFadeLinearAmp:
    case kAAFFadeLinearPower:
      return true;
    default:
      return false;
  }
}

bool Is_aafFieldDom_t_Valid(aafFieldDom_t param)
{
  switch (param) {
    case kAAFDominantField1:
    case kAAFDominantField2:
    case kAAFNoDominant:
      return true;
    default:
      return false;
  }
}

bool Is_aafFieldNumber_t_Valid(aafFieldNumber_t param)
{
  switch (param) {
    case kAAFUnspecifiedField:
    case kAAFFieldOne:
    case kAAFFieldTwo:
      return true;
    default:
      return false;
  }
}

bool Is_aafFieldTop_t_Valid(aafFieldTop_t param)
{
  switch (param) {
    case kAAFTopField1:
    case kAAFTopField2:
    case kAAFTopFieldNone:
      return true;
    default:
      return false;
  }
}

bool Is_aafFileAccess_t_Valid(aafFileAccess_t param)
{
  switch (param) {
    case kAAFFileAccess_none:
    case kAAFFileAccess_read:
    case kAAFFileAccess_write:
    case kAAFFileAccess_modify:
      return true;
    default:
      return false;
  }
}

bool Is_aafFileExistence_t_Valid(aafFileExistence_t param)
{
  switch (param) {
    case kAAFFileExistence_new:
    case kAAFFileExistence_existing:
      return true;
    default:
      return false;
  }
}

bool Is_aafFileFormat_t_Valid(aafFileFormat_t param)
{
  switch (param) {
    case kAAFEssence:
    case kAAFForeignMedia:
      return true;
    default:
      return false;
  }
}

bool Is_aafFileRev_t_Valid(aafFileRev_t param)
{
  switch (param) {
    case kAAFRev1:
    case kAAFRev2:
      return true;
    default:
      return false;
  }
}

bool Is_aafFilmType_t_Valid(aafFilmType_t param)
{
  switch (param) {
    case kAAFFtNull:
    case kAAFFt35MM:
    case kAAFFt16MM:
    case kAAFFt8MM:
    case kAAFFt65MM:
      return true;
    default:
      return false;
  }
}

bool Is_aafFrameLayout_t_Valid(aafFrameLayout_t param)
{
  switch (param) {
    case kAAFFullFrame:
    case kAAFSeparateFields:
    case kAAFOneField:
    case kAAFMixedFields:
    case kAAFSegmentedFrame:
      return true;
    default:
      return false;
  }
}

bool Is_aafIdentificationCritType_t_Valid(aafIdentificationCritType_t param)
{
  switch (param) {
    case kAAFNoIdSearch:
    case kAAFByIdProductID:
    case kAAFByIdGeneration:
    case kAAFByIdRefImplVersion:
      return true;
    default:
      return false;
  }
}

bool Is_aafIncMedia_t_Valid(aafIncMedia_t param)
{
  switch (param) {
    case kAAFNoIncludeMedia:
    case kAAFIncludeMedia:
      return true;
    default:
      return false;
  }
}

bool Is_aafIndexType_t_Valid(aafIndexType_t param)
{
  switch (param) {
    case kAAFCompMobIndex:
    case kAAFSourceMobIndex:
    case kAAFMediaIndex:
    case kAAFPrimaryMobIndex:
      return true;
    default:
      return false;
  }
}

bool Is_aafInterpKind_t_Valid(aafInterpKind_t param)
{
  switch (param) {
    case kAAFConstInterp:
    case kAAFLinearInterp:
      return true;
    default:
      return false;
  }
}

bool Is_aafJPEGcomponent_t_Valid(aafJPEGcomponent_t param)
{
  switch (param) {
    case kAAFJcLuminance:
    case kAAFJcChrominance:
    case kAAFJcLuminanceFP16:
    case kAAFJcChrominanceFP16:
      return true;
    default:
      return false;
  }
}

bool Is_aafMediaOpenMode_t_Valid(aafMediaOpenMode_t param)
{
  switch (param) {
    case kAAFMediaOpenReadOnly:
    case kAAFMediaOpenAppend:
      return true;
    default:
      return false;
  }
}

bool Is_aafMobKind_t_Valid(aafMobKind_t param)
{
  switch (param) {
    case kAAFCompMob:
    case kAAFMasterMob:
    case kAAFFileMob:
    case kAAFTapeMob:
    case kAAFFilmMob:
    case kAAFPrimaryMob:
    case kAAFAllMob:
    case kAAFPhysicalMob:
      return true;
    default:
      return false;
  }
}

bool Is_aafOperationChoice_t_Valid(aafOperationChoice_t param)
{
  switch (param) {
    case kAAFFindNull:
    case kAAFFindIncoming:
    case kAAFFindOutgoing:
    case kAAFFindRender:
    case kAAFFindOperationSrc1:
    case kAAFFindOperationSrc2:
    case kAAFFindOperationSrc3:
    case kAAFFindOperationSrc4:
    case kAAFFindOperationSrc5:
    case kAAFFindOperationSrc6:
    case kAAFFindOperationSrc7:
    case kAAFFindOperationSrc8:
      return true;
    default:
      return false;
  }
}

bool Is_aafPixelFormat_t_Valid(aafPixelFormat_t param)
{
  switch (param) {
    case kAAFPixNone:
    case kAAFPixRGBA:
    case kAAFPixYUV:
      return true;
    default:
      return false;
  }
}

bool Is_aafProductReleaseType_t_Valid(aafProductReleaseType_t param)
{
  switch (param) {
    case kAAFVersionUnknown:
    case kAAFVersionReleased:
    case kAAFVersionDebug:
    case kAAFVersionPatched:
    case kAAFVersionBeta:
    case kAAFVersionPrivateBuild:
      return true;
    default:
      return false;
  }
}

bool Is_aafPropertyOpt_t_Valid(aafPropertyOpt_t param)
{
  switch (param) {
    case kAAFPropRequired:
    case kAAFPropOptional:
      return true;
    default:
      return false;
  }
}

bool Is_aafPulldownDir_t_Valid(aafPulldownDir_t param)
{
  switch (param) {
    case kAAFTapeToFilmSpeed:
    case kAAFFilmToTapeSpeed:
      return true;
    default:
      return false;
  }
}

bool Is_aafPulldownKind_t_Valid(aafPulldownKind_t param)
{
  switch (param) {
    case kAAFTwoThreePD:
    case kAAFPALPD:
    case kAAFOneToOneNTSC:
    case kAAFOneToOnePAL:
    case kAAFVideoTapNTSC:
    case kAAFOneToOneHDSixty:
    case kAAFTwentyFourToSixtyPD:
    case kAAFTwoToOnePD:
      return true;
    default:
      return false;
  }
}

bool Is_aafRGBAComponentKind_t_Valid(aafRGBAComponentKind_t param)
{
  switch (param) {
    case kAAFCompNone:
    case kAAFCompAlpha:
    case kAAFCompBlue:
    case kAAFCompFill:
    case kAAFCompGreen:
    case kAAFCompPalette:
    case kAAFCompRed:
    case kAAFCompNull:
      return true;
    default:
      return false;
  }
}

bool Is_aafReferenceType_t_Valid(aafReferenceType_t param)
{
  switch (param) {
    case kAAFRefLimitMinimum:
    case kAAFRefLimitMaximum:
    case kAAFRefMinimum:
    case kAAFRefMaximum:
    case kAAFRefEnumvalue:
      return true;
    default:
      return false;
  }
}

bool Is_aafScanningDirection_t_Valid(aafScanningDirection_t param)
{
  switch (param) {
    case kAAFScanningDirection_LeftToRightTopToBottom:
    case kAAFScanningDirection_RightToLeftTopToBottom:
    case kAAFScanningDirection_LeftToRightBottomToTop:
    case kAAFScanningDirection_RightToLeftBottomToTop:
    case kAAFScanningDirection_TopToBottomLeftToRight:
    case kAAFScanningDirection_TopToBottomRightToLeft:
    case kAAFScanningDirection_BottomToTopLeftToRight:
    case kAAFScanningDirection_BottomToTopRightToLeft:
      return true;
    default:
      return false;
  }
}

bool Is_aafSearchDictionaryTag_t_Valid(aafSearchDictionaryTag_t param)
{
  switch (param) {
    case kAAFAllClasses:
    case kAAFByClassID:
    case kAAFByParentClassID:
    case kAAFByPropertyID:
    case kAAFByTypeID:
    case kAAFByClassName:
      return true;
    default:
      return false;
  }
}

bool Is_aafSearchTag_t_Valid(aafSearchTag_t param)
{
  switch (param) {
    case kAAFNoSearch:
    case kAAFByMobID:
    case kAAFByMobKind:
    case kAAFByName:
    case kAAFByClass:
    case kAAFByDataDef:
    case kAAFByMediaCrit:
    case kAAFByUsageCode:
    case kAAFByMasterMobUsageCode:
    case kAAFBySourceMobUsageCode:
    case kAAFByCompositionMobUsageCode:
      return true;
    default:
      return false;
  }
}

bool Is_aafSignalStandard_t_Valid(aafSignalStandard_t param)
{
  switch (param) {
    case kAAFSignalStandard_None:
    case kAAFSignalStandard_ITU601:
    case kAAFSignalStandard_ITU1358:
    case kAAFSignalStandard_SMPTE347M:
    case kAAFSignalStandard_SMPTE274M:
    case kAAFSignalStandard_SMPTE296M:
    case kAAFSignalStandard_SMPTE349M:
      return true;
    default:
      return false;
  }
}

bool Is_aafSwapNeeded_t_Valid(aafSwapNeeded_t param)
{
  switch (param) {
    case kAAFSwapIfNeeded:
    case kAAFNeverSwap:
      return true;
    default:
      return false;
  }
}

bool Is_aafTapeCaseType_t_Valid(aafTapeCaseType_t param)
{
  switch (param) {
    case kAAFTapeCaseNull:
    case kAAFThreeFourthInchVideoTape:
    case kAAFVHSVideoTape:
    case kAAF8mmVideoTape:
    case kAAFBetacamVideoTape:
    case kAAFCompactCassette:
    case kAAFDATCartridge:
    case kAAFNagraAudioTape:
      return true;
    default:
      return false;
  }
}

bool Is_aafTapeFormatType_t_Valid(aafTapeFormatType_t param)
{
  switch (param) {
    case kAAFTapeFormatNull:
    case kAAFBetacamFormat:
    case kAAFBetacamSPFormat:
    case kAAFVHSFormat:
    case kAAFSVHSFormat:
    case kAAF8mmFormat:
    case kAAFHi8Format:
      return true;
    default:
      return false;
  }
}

bool Is_aafTimecodeSourceType_t_Valid(aafTimecodeSourceType_t param)
{
  switch (param) {
    case kAAFTimecodeLTC:
    case kAAFTimecodeVITC:
      return true;
    default:
      return false;
  }
}

bool Is_aafTitleAlignmentType_t_Valid(aafTitleAlignmentType_t param)
{
  switch (param) {
    case kAAFTitleAlignment_Left:
    case kAAFTitleAlignment_Center:
    case kAAFTitleAlignment_Right:
      return true;
    default:
      return false;
  }
}

bool Is_aafUserDataModeType_t_Valid(aafUserDataModeType_t param)
{
  switch (param) {
    case kAAFUserDataMode_NotDefined:
    case kAAFUserDataMode_192BitBlockStructure:
    case kAAFUserDataMode_AES18:
    case kAAFUserDataMode_UserDefined:
    case kAAFUserDataMode_IEC:
    case kAAFUserDataMode_Metadata:
    case kAAFUserDataMode_Reserved0:
    case kAAFUserDataMode_Reserved1:
    case kAAFUserDataMode_Reserved2:
    case kAAFUserDataMode_Reserved3:
    case kAAFUserDataMode_Reserved4:
    case kAAFUserDataMode_Reserved5:
    case kAAFUserDataMode_Reserved6:
    case kAAFUserDataMode_Reserved7:
    case kAAFUserDataMode_Reserved8:
    case kAAFUserDataMode_Reserved9:
      return true;
    default:
      return false;
  }
}

bool Is_aafVideoSignalType_t_Valid(aafVideoSignalType_t param)
{
  switch (param) {
    case kAAFVideoSignalNull:
    case kAAFNTSCSignal:
    case kAAFPALSignal:
    case kAAFSECAMSignal:
      return true;
    default:
      return false;
  }
}

bool Is_eAAFByteOrder_t_Valid(eAAFByteOrder_t param)
{
  switch (param) {
    case kAAFByteOrderLittle:
    case kAAFByteOrderBig:
      return true;
    default:
      return false;
  }
}

bool Is_eAAFTypeCategory_t_Valid(eAAFTypeCategory_t param)
{
  switch (param) {
    case kAAFTypeCatUnknown:
    case kAAFTypeCatInt:
    case kAAFTypeCatCharacter:
    case kAAFTypeCatStrongObjRef:
    case kAAFTypeCatWeakObjRef:
    case kAAFTypeCatRename:
    case kAAFTypeCatEnum:
    case kAAFTypeCatFixedArray:
    case kAAFTypeCatVariableArray:
    case kAAFTypeCatSet:
    case kAAFTypeCatRecord:
    case kAAFTypeCatStream:
    case kAAFTypeCatString:
    case kAAFTypeCatExtEnum:
    case kAAFTypeCatIndirect:
    case kAAFTypeCatOpaque:
    case kAAFTypeCatEncrypted:
      return true;
    default:
      return false;
  }
}

