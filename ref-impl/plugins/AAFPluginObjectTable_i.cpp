//=---------------------------------------------------------------------=
//
// $Id: AAFPluginObjectTable_i.cpp,v 1.19 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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

//
// Include the appropriate header files:
//
#include "CAAFEssenceDataStream.h"
#include "CAAFEssenceFileContainer.h"
#include "CAAFEssenceRIFFWAVEContainer.h"
#include "CAAFAIFCCodec.h"
#include "CAAFWAVECodec.h"
#include "CAAFPCMCodec.h"
#include "CAAFJPEGCodec.h"
#include "CAAFCDCICodec.h"
#include "CAAFVC3Codec.h"
#include "CAAFDNxHDCodec.h"

//
// Include all objects in the following table:
//
AAF_BEGIN_OBJECT_MAP(AAFPluginObjectMap)
	AAF_OBJECT_ENTRY(AAFEssenceDataStream)
	AAF_OBJECT_ENTRY(AAFEssenceFileContainer)
	AAF_OBJECT_ENTRY(AAFEssenceRIFFWAVEContainer)
	AAF_OBJECT_ENTRY(AAFAIFCCodec)
	AAF_OBJECT_ENTRY(AAFWaveCodec)
	AAF_OBJECT_ENTRY(AAFPCMCodec)
	AAF_OBJECT_ENTRY(AAFJPEGCodec)
	AAF_OBJECT_ENTRY(AAFCDCICodec)
	AAF_OBJECT_ENTRY(AAFVC3Codec)
	AAF_OBJECT_ENTRY(AAFDNxHDCodec)
AAF_END_OBJECT_MAP()
