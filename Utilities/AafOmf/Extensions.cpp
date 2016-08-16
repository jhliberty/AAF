// @doc INTERNAL
// @com This file handles bdirectional translation of effect metadata between OMF and AAF

//=---------------------------------------------------------------------=
//
// $Id: Extensions.cpp,v 1.7 2009/06/01 11:46:48 stuart_hc Exp $ $Name: V116 $
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#include "omPublic.h"
#include "omMedia.h"

// OMF Includes

#include "AafOmf.h"
#include "AAFDomainUtils.h"
#include "AAFDomainExtensions.h"
#include "OMFDomainExtensionUtils.h"
#include "EffectTranslate.h"
#include "Extensions.h"
#include "AAFClassDefUIDs.h" 
#include "AAFException.h"
#include "AAFDomainUtils.h"
#include "AAFException.h"
#include "OMFException.h"
#include "AutoRelease.h"

extern AafOmfGlobals* gpGlobals;

#define CHECKAAF
static void     LogError(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define checkAAF(a)  \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}

#define checkOMF(a)  \
{ moduleErrorTmp = a; \
	if (moduleErrorTmp != OM_ERR_NONE) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}
#define checkFatal(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     exit(1);\
}



// ============================================================================
// Constructor
// ============================================================================
ExtendedOmf2Aaf::ExtendedOmf2Aaf( AAFDomainUtils *aafDomainUtils, OMFDomainUtils *omfDomainUtils, EffectTranslate *effectTranslate ) 
	: Omf2Aaf( aafDomainUtils, omfDomainUtils, effectTranslate ) 
{
}
// ============================================================================
// Destructor
// ============================================================================
ExtendedOmf2Aaf::~ExtendedOmf2Aaf()
{
}

// ============================================================================
// Constructor
// ============================================================================
ExtendedAaf2Omf::ExtendedAaf2Omf(AAFDomainUtils *aafDomainUtils, OMFDomainUtils *omfDomainUtils, EffectTranslate *effectTranslate ) 
	: Aaf2Omf( aafDomainUtils, omfDomainUtils, effectTranslate )
{
}
// ============================================================================
// Destructor
// ============================================================================
ExtendedAaf2Omf::~ExtendedAaf2Omf()
{
}
