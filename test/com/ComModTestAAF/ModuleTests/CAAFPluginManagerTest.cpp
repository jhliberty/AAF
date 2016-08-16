//=---------------------------------------------------------------------=
//
// $Id: CAAFPluginManagerTest.cpp,v 1.23 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
#include <stdlib.h>
#include <stdio.h>

#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"
#include "AAF.h"

#include "AAFPlugin.h"
#ifndef DISABLE_DYNAMIC_LOADING
// AAFPlugin_i.c's definitions conflict with the definitions instantiated in ImplAAFPluginManager.cpp
// So don't include it for static builds
#include "AAFPlugin_i.c"
#endif

const CLSID CLSID_AAFBasicInterp = { 0x5B6C85A1, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}

extern "C" HRESULT CAAFPluginManager_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFPluginManager_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IEnumAAFLoadedPlugins	*pEnum = NULL;
	IAAFPluginManager		*pMgr = NULL;
	aafUID_t				testUID;
	IAAFFile*		pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFDefObject	*pPluginDef = NULL;
	IAAFPlugin	*plugin			= NULL;

	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode != kAAFUnitTestReadWrite)
			return AAFRESULT_SUCCESS;			// Can't run this test read-only

		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		
		checkResult(AAFGetPluginManager (&pMgr));
		
		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
		
		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));

		checkResult(pMgr->EnumLoadedPlugins (AUID_AAFCodecDef, &pEnum));
		while(pEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkResult(pMgr->CreatePluginDefinition (testUID, pDictionary, &pPluginDef));
			pPluginDef->Release();
			pPluginDef = NULL;
		}
		hr = AAFRESULT_SUCCESS;

		checkResult(pMgr->CreateInstance(CLSID_AAFBasicInterp,
			NULL, 
			IID_IAAFPlugin, 
			(void **)&plugin));
	}
	catch (HRESULT& rhr)
	{
		hr = rhr;
	}

	if (plugin)
		plugin->Release();

	if (pEnum)
		pEnum->Release();

	if (pMgr)
		pMgr->Release();

	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile)
	{  // Close file
		if (bFileOpen)
		{
			pFile->Save();
			pFile->Close();
		}
		pFile->Release();
	}
	
	// JeffB: RegisterPluginDirectory and RegisterPluginFile have their implementations tested
	// by the RegisterStandard() function, and will require a special local plugin to test.  Therefore
	// I'm pushing the test off post 1.0 and declaring success here.
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFPluginManager tests have not been implemented:" << endl; 
////!!!		cout << "     RegisterPluginDirectory" << endl; 
////!!!		cout << "     RegisterPluginFile" << endl; 
////		cout << "     EnumLoadedPlugins" << endl; 
//		cout << "     CreateInstance" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}







