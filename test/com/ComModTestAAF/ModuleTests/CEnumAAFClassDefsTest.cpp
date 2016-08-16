// @doc INTERNAL
// @com This file implements the module test for CEnumAAFClassDefs
//=---------------------------------------------------------------------=
//
// $Id: CEnumAAFClassDefsTest.cpp,v 1.11 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <wchar.h>
#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include "CEnumeratorTest.h"

class CEnumAAFClassDefsTest: public CEnumeratorTest<IEnumAAFClassDefs,IAAFClassDef>
{
public:
	HRESULT CountItems(IAAFDictionary *pDictionary,aafUInt32 *piCount)
	{
		return(pDictionary->CountClassDefs(piCount));
	}
	HRESULT GetItems(IAAFDictionary *pDictionary,IEnumAAFClassDefs **ppEnumerator)
	{
		return(pDictionary->GetClassDefs(ppEnumerator));
	}
	aafBool ItemIsPresent(IAAFDictionary *pDictionary,aafUID_t& Id)
	{
		IAAFClassDefSP pClassDef;
		return(pDictionary->LookupClassDef(Id,&pClassDef)==AAFRESULT_SUCCESS?
			kAAFTrue:kAAFFalse);
	}
};

extern "C" HRESULT CEnumAAFClassDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFClassDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	try
	{
		CEnumAAFClassDefsTest Test;
		Test.Run(mode, fileKind, rawStorageType, productID);
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}
