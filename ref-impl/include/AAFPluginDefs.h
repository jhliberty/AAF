#ifndef __PluginDefinition_h__
#define __PluginDefinition_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: AAFPluginDefs.h,v 1.13 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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

// AAF well-known PluginDefinition instances
//

//{3d1dd891-e793-11d2-809e-006008143e6f}
const aafUID_t kAAFPlatform_Independent =
{0x3d1dd891, 0xe793, 0x11d2, {0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{9fdef8c1-e847-11d2-809e-006008143e6f}
const aafUID_t kAAFEngine_None =
{0x9fdef8c1, 0xe847, 0x11d2, {0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{69c870a1-e793-11d2-809e-006008143e6f}
const aafUID_t kAAFPluginAPI_EssenceAccess =
{0x69c870a1, 0xe793, 0x11d2, {0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f}};


//{56905e0b-537d-11d4-a36c-009027dfca6a}
const aafUID_t kAAFPluginCategory_Codec =
{0x56905e0b, 0x537d, 0x11d4, {0xa3, 0x6c, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}};


// AAF PluginDefinition legacy aliases
//

const aafUID_t kAAFPlatformIndependant = kAAFPlatform_Independent;
const aafUID_t kAAFNoEngine = kAAFEngine_None;
const aafUID_t kAAFEssencePluginAPI = kAAFPluginAPI_EssenceAccess;
const aafUID_t kAAFPluginNoCategory = kAAFPluginCategory_Codec;

#endif // ! __PluginDefinition_h__
