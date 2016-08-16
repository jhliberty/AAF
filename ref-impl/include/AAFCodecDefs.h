#ifndef __CodecDefinition_h__
#define __CodecDefinition_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: AAFCodecDefs.h,v 1.23 2009/06/01 11:46:57 stuart_hc Exp $ $Name: V116 $
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
// Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

// AAF well-known CodecDefinition instances
//

//{568fb761-9458-11d2-8089-006008143e6f}
const aafUID_t kAAFCodecDef_None =
{0x568fb761, 0x9458, 0x11d2, {0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{90ac17c8-e3e2-4596-9e9e-a6dd1c70c892}
const aafUID_t kAAFCodecDef_PCM =
{0x90ac17c8, 0xe3e2, 0x4596, {0x9e, 0x9e, 0xa6, 0xdd, 0x1c, 0x70, 0xc8, 0x92}};


//{820f09b1-eb9b-11d2-809f-006008143e6f}
const aafUID_t kAAFCodecDef_WAVE =
{0x820f09b1, 0xeb9b, 0x11d2, {0x80, 0x9f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{4b1c1a45-03f2-11d4-80fb-006008143e6f}
const aafUID_t kAAFCodecDef_AIFC =
{0x4b1c1a45, 0x03f2, 0x11d4, {0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{18634f8c-3bab-11d3-bfd6-00104bc9156d}
const aafUID_t kAAFCodecDef_JPEG =
{0x18634f8c, 0x3bab, 0x11d3, {0xbf, 0xd6, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d}};


//{4e84045e-0f29-11d4-a359-009027dfca6a}
const aafUID_t kAAFCodecDef_CDCI =
{0x4e84045e, 0x0f29, 0x11d4, {0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


//{4e84045f-0f29-11d4-a359-009027dfca6a}
const aafUID_t kAAFCodecDef_RGBA =
{0x4e84045f, 0x0f29, 0x11d4, {0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


//{6c2a61c2-e7a2-46ee-8d90-6a1d06e15f41}
const aafUID_t kAAFCodecDef_VC3 =
{0x6c2a61c2, 0xe7a2, 0x46ee, {0x8d, 0x90, 0x6a, 0x1d, 0x06, 0xe1, 0x5f, 0x41}};


//{8ef593f6-9521-4344-9ede-b84e8cfdc7da}
const aafUID_t kAAFCodecDef_DNxHD =
{0x8ef593f6, 0x9521, 0x4344, {0x9e, 0xde, 0xb8, 0x4e, 0x8c, 0xfd, 0xc7, 0xda}};


//{1b31f3b1-9450-11d2-8089-006008143e6f}
const aafUID_t kAAFCodecFlavour_None =
{0x1b31f3b1, 0x9450, 0x11d2, {0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{af4de587-23d7-4c8a-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_100Mbps_1080x50I =
{0xaf4de587, 0x23d7, 0x4c8a, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c8b-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_100Mbps_1080x5994I =
{0xaf4de587, 0x23d7, 0x4c8b, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c8c-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_100Mbps_720x50P =
{0xaf4de587, 0x23d7, 0x4c8c, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c8d-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_100Mbps_720x5994P =
{0xaf4de587, 0x23d7, 0x4c8d, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c80-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_25Mbps_525_60 =
{0xaf4de587, 0x23d7, 0x4c80, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c81-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_25Mbps_625_50 =
{0xaf4de587, 0x23d7, 0x4c81, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c82-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_50Mbps_525_60 =
{0xaf4de587, 0x23d7, 0x4c82, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c83-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_DV_Based_50Mbps_625_50 =
{0xaf4de587, 0x23d7, 0x4c83, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7f-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_IEC_DV_525_60 =
{0xaf4de587, 0x23d7, 0x4c7f, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7e-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_IEC_DV_625_50 =
{0xaf4de587, 0x23d7, 0x4c7e, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7d-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_LegacyDV_525_60 =
{0xaf4de587, 0x23d7, 0x4c7d, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c7c-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_LegacyDV_625_50 =
{0xaf4de587, 0x23d7, 0x4c7c, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c84-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_SMPTE_D10_50Mbps_625x50I =
{0xaf4de587, 0x23d7, 0x4c84, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c85-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_SMPTE_D10_50Mbps_525x5994I =
{0xaf4de587, 0x23d7, 0x4c85, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c86-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_SMPTE_D10_40Mbps_625x50I =
{0xaf4de587, 0x23d7, 0x4c86, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c87-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_SMPTE_D10_40Mbps_525x5994I =
{0xaf4de587, 0x23d7, 0x4c87, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c88-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_SMPTE_D10_30Mbps_625x50I =
{0xaf4de587, 0x23d7, 0x4c88, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{af4de587-23d7-4c89-b37b-c1c13870e711}
const aafUID_t kAAFCodecFlavour_SMPTE_D10_30Mbps_525x5994I =
{0xaf4de587, 0x23d7, 0x4c89, {0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11}};


//{effdb6b4-fe99-4768-88fe-3422a5762961}
const aafUID_t kAAFCodecFlavour_VC3_1235 =
{0xeffdb6b4, 0xfe99, 0x4768, {0x88, 0xfe, 0x34, 0x22, 0xa5, 0x76, 0x29, 0x61}};


//{21b15f27-2781-4656-aa1b-dc5e63862738}
const aafUID_t kAAFCodecFlavour_VC3_1237 =
{0x21b15f27, 0x2781, 0x4656, {0xaa, 0x1b, 0xdc, 0x5e, 0x63, 0x86, 0x27, 0x38}};


//{62f37363-b1d1-4fa0-9fb7-6e7044371396}
const aafUID_t kAAFCodecFlavour_VC3_1238 =
{0x62f37363, 0xb1d1, 0x4fa0, {0x9f, 0xb7, 0x6e, 0x70, 0x44, 0x37, 0x13, 0x96}};


//{1e9b855a-323e-4999-b0fa-8444267a63a7}
const aafUID_t kAAFCodecFlavour_VC3_1241 =
{0x1e9b855a, 0x323e, 0x4999, {0xb0, 0xfa, 0x84, 0x44, 0x26, 0x7a, 0x63, 0xa7}};


//{8b4c29cf-b255-4ef0-bf79-b5b616479238}
const aafUID_t kAAFCodecFlavour_VC3_1242 =
{0x8b4c29cf, 0xb255, 0x4ef0, {0xbf, 0x79, 0xb5, 0xb6, 0x16, 0x47, 0x92, 0x38}};


//{e063fd16-6a70-4128-936d-ac776f2630cf}
const aafUID_t kAAFCodecFlavour_VC3_1243 =
{0xe063fd16, 0x6a70, 0x4128, {0x93, 0x6d, 0xac, 0x77, 0x6f, 0x26, 0x30, 0xcf}};


//{c80d0143-be86-45fd-aacc-7f612b4b9139}
const aafUID_t kAAFCodecFlavour_VC3_1244 =
{0xc80d0143, 0xbe86, 0x45fd, {0xaa, 0xcc, 0x7f, 0x61, 0x2b, 0x4b, 0x91, 0x39}};


//{47eb10b5-72fa-4dbb-9801-e0fe9ab8d9f0}
const aafUID_t kAAFCodecFlavour_VC3_1250 =
{0x47eb10b5, 0x72fa, 0x4dbb, {0x98, 0x01, 0xe0, 0xfe, 0x9a, 0xb8, 0xd9, 0xf0}};


//{26cf3984-c716-4315-9de7-9228b5c0f922}
const aafUID_t kAAFCodecFlavour_VC3_1251 =
{0x26cf3984, 0xc716, 0x4315, {0x9d, 0xe7, 0x92, 0x28, 0xb5, 0xc0, 0xf9, 0x22}};


//{0909cf52-475a-4abc-9e13-0ddb9d60d16c}
const aafUID_t kAAFCodecFlavour_VC3_1252 =
{0x0909cf52, 0x475a, 0x4abc, {0x9e, 0x13, 0x0d, 0xdb, 0x9d, 0x60, 0xd1, 0x6c}};


//{7f5d77dd-5402-45e0-9128-038016f55406}
const aafUID_t kAAFCodecFlavour_VC3_1253 =
{0x7f5d77dd, 0x5402, 0x45e0, {0x91, 0x28, 0x03, 0x80, 0x16, 0xf5, 0x54, 0x06}};


//{a362d3cb-dcef-4ffb-bb35-be72a16561ce}
const aafUID_t kAAFCodecFlavour_VC3_1254 =
{0xa362d3cb, 0xdcef, 0x4ffb, {0xbb, 0x35, 0xbe, 0x72, 0xa1, 0x65, 0x61, 0xce}};


// AAF CodecDefinition legacy aliases
//

const aafUID_t kAAFNoCodec = kAAFCodecDef_None;
const aafUID_t kAAFCodecPCM = kAAFCodecDef_PCM;
const aafUID_t kAAFCodecWAVE = kAAFCodecDef_WAVE;
const aafUID_t kAAFCODEC_AIFC = kAAFCodecDef_AIFC;
const aafUID_t kAAFCodecJPEG = kAAFCodecDef_JPEG;
const aafUID_t kAAFCodecCDCI = kAAFCodecDef_CDCI;
const aafUID_t kAAFCodecRGBA = kAAFCodecDef_RGBA;
const aafUID_t kAAFCodecVC3 = kAAFCodecDef_VC3;
const aafUID_t kAAFCodecDNxHD = kAAFCodecDef_DNxHD;
const aafUID_t kAAFNilCodecFlavour = kAAFCodecFlavour_None;

#endif // ! __CodecDefinition_h__
