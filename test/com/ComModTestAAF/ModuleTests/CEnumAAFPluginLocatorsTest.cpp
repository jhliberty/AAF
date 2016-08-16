//=---------------------------------------------------------------------=
//
// $Id: CEnumAAFPluginLocatorsTest.cpp,v 1.32 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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


#include "AAF.h"
#include "AAFPluginDefs.h"

#include <iostream>
using namespace std;


static const wchar_t *manuf1URL = L"www.microsoft.com";
static const wchar_t *manuf2URL = L"www.avid.com";

#include <iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"
#include "AAFCodecDefs.h"

#include "CAAFBuiltinDefs.h"

const aafUID_t MANUF_JEFFS_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expPixelFormat */
//static aafVersionType_t samplePluginVersion = { 0, 0 };//, 0, 0, kVersionReleased };
static aafVersionType_t sampleMinPlatformVersion = { 1, 2 }; //, 3, 4, kVersionDebug };
static aafVersionType_t sampleMinEngineVersion = { 5, 6 }; //7, 9, kVersionPatched };
static aafVersionType_t sampleMinAPIVersion = { 10, 11 };//, 12, 13, kAAFVersionBeta };
static aafVersionType_t sampleMaxPlatformVersion = { 31, 32 };//3, 34, kVersionDebug };
static aafVersionType_t sampleMaxEngineVersion = { 35, 36 };//, 37, 39, kVersionPatched };
static aafVersionType_t sampleMaxAPIVersion = { 40, 41 };//, 42, 43, kAAFVersionBeta };

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		3

static const wchar_t *manufName = L"Jeff's Plugin-O-Rama";
static const wchar_t *manufRev = L"Rev0.0.0a0";

// convenient error handlers.
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

static aafUID_t TestPluginDesc = { 0x7C77C181, 0x2283, 0x11d2, { 0x80, 0xAD, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*		pFile = NULL;
  IAAFHeader *      pHeader = NULL;
  IAAFDictionary*	pDictionary = NULL;
  IAAFDefObject*	pPlugDef = NULL;
  IAAFCodecDef*		pCodecDef = NULL;
  IAAFClassDef*		pClassDef = NULL;
  IAAFPluginDef *pDesc = NULL;
  IAAFNetworkLocator *pNetLoc = NULL, *pNetLoc2 = NULL, *pNetLoc3 = NULL;
  IAAFLocator		*pLoc = NULL, *pLoc2 = NULL, *pLoc3 = NULL;
  aafUID_t			category = AUID_AAFDefObject, manufacturer = MANUF_JEFFS_PLUGINS;
  bool				bFileOpen = false;
	HRESULT			hr = S_OK;
/*	long			test;
*/

  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


    // Create the AAF file
    checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
    bFileOpen = true;

    // We can't really do anthing in AAF without the header.
    checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
	CAAFBuiltinDefs defs (pDictionary);
    
	checkResult(defs.cdCodecDef()->
				CreateInstance(IID_IAAFDefObject, 
							   (IUnknown **)&pPlugDef));
    
	checkResult(defs.cdPluginDef()->
				CreateInstance(IID_IAAFPluginDef, 
							   (IUnknown **)&pDesc));
	checkResult(defs.cdNetworkLocator()->
				CreateInstance(IID_IAAFNetworkLocator, 
							   (IUnknown **)&pNetLoc));
	checkResult(pNetLoc->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc));
	checkResult(pLoc->SetPath (manuf2URL));

	checkResult(pDesc->Initialize (TestPluginDesc, L"Test Plugin", L"TestPlugin Description"));
	checkResult(pDesc->SetCategoryClass(category));
	checkResult(pDesc->SetPluginVersionString(manufRev));
    checkResult(pDesc->SetManufacturerInfo(pNetLoc));
    checkResult(pDesc->SetManufacturerID(manufacturer));
    checkResult(pDesc->SetPluginManufacturerName(manufName));
    checkResult(pDesc->SetIsSoftwareOnly(kAAFTrue));
    checkResult(pDesc->SetIsAccelerated(kAAFFalse));
    checkResult(pDesc->SetSupportsAuthentication(kAAFFalse));
 
//!!!	aafProductVersion_t samplePluginVersion = { 0, 0, 0, 0, kVersionReleased };
    /**/
    checkResult(pDesc->SetHardwarePlatform(kAAFPlatformIndependant));
    checkResult(pDesc->SetPlatformMinimumVersion(sampleMinPlatformVersion));
    checkResult(pDesc->SetPlatformMaximumVersion(sampleMaxPlatformVersion));
    /**/
 	checkResult(pDesc->SetEngine(kAAFNoEngine));
    checkResult(pDesc->SetEngineMinimumVersion(sampleMinEngineVersion));
    checkResult(pDesc->SetEngineMaximumVersion(sampleMaxEngineVersion));
    /**/
	checkResult(pDesc->SetPluginAPI(kAAFEssencePluginAPI));
    checkResult(pDesc->SetPluginAPIMinimumVersion(sampleMinAPIVersion));
    checkResult(pDesc->SetPluginAPIMaximumVersion(sampleMaxAPIVersion));

	checkResult(pDictionary->RegisterPluginDef (	pDesc));

	  /**/
	checkResult(defs.cdNetworkLocator()->
				CreateInstance(IID_IAAFNetworkLocator, 
							   (IUnknown **)&pNetLoc2));
	checkResult(pNetLoc2->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc2));
	checkResult(pLoc2->SetPath (manuf2URL));
    checkResult(pDesc->AppendLocator(pLoc2));
	/**/
	checkResult(pDesc->SetDefinitionObjectID(TestPluginDesc));

	
	checkResult(pPlugDef->QueryInterface (IID_IAAFCodecDef,
                                          (void **)&pCodecDef));
	checkResult(pCodecDef->Initialize (kAAFNoCodec, L"TestCodec", L"Just a test"));
	checkResult(pCodecDef->AddEssenceKind (defs.ddkAAFMatte()));
	checkResult(pDictionary->LookupClassDef(kAAFClassID_EssenceDescriptor, &pClassDef));
	checkResult(pCodecDef->SetFileDescriptorClass (pClassDef));
	checkResult(pDictionary->RegisterCodecDef(pCodecDef));
	/**/
	checkResult(defs.cdNetworkLocator()->
				CreateInstance(IID_IAAFNetworkLocator, 
							   (IUnknown **)&pNetLoc3));
	checkResult(pNetLoc3->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc3));
	checkResult(pLoc3->SetPath (manuf1URL));
    checkResult(pDesc->PrependLocator(pLoc3));

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pPlugDef)
    pPlugDef->Release();

  if (pCodecDef)
    pCodecDef->Release();

  if (pDesc)
    pDesc->Release();

  if (pNetLoc)
    pNetLoc->Release();
  if (pNetLoc2)
    pNetLoc2->Release();
  if (pNetLoc3)
    pNetLoc3->Release();
  if (pClassDef)
    pClassDef->Release();
  if (pLoc)
    pLoc->Release();
  if (pLoc2)
    pLoc2->Release();
  if (pLoc3)
    pLoc3->Release();

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

  return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IEnumAAFCodecDefs *pEnumPluggable = NULL;
	IAAFCodecDef *pCodecDef = NULL;
	IAAFDefObject *pDefObj = NULL;
	IEnumAAFPluginDefs *pEnumDesc;
	IAAFPluginDef *pPlugin = NULL;
	IAAFNetworkLocator	*pNetLoc = NULL;
	IAAFLocator			*pLocator = NULL;
	IEnumAAFPluginLocators *pEnumLoc = NULL;
	IAAFLocator				*	pArray[2] = { NULL, NULL };
	IAAFLocator				**	pArrayPoint = pArray;
	IEnumAAFPluginLocators *pCloneEnum = NULL;
	aafUInt32			resultCount;

  bool bFileOpen = false;
	HRESULT			hr = S_OK;
	aafUInt32		count;
	wchar_t			testname[256];

	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetPluginDefs (&pEnumDesc));
		checkResult(pEnumDesc->NextOne (&pPlugin));

		/**/
		checkResult(pPlugin->CountLocators(&count));
		checkExpression (count == 2, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetLocators(&pEnumLoc));

			/* Read and check the first element */
			checkResult(pEnumLoc->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf1URL) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;

			/**/
			/* Read and check the second element */
			checkResult(pEnumLoc->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf2URL) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;
			/*****/
			
			/* Reset, and check the first element again*/
			checkResult(pEnumLoc->Reset());
			checkResult(pEnumLoc->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf1URL) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;
			
			/* Reset, Skip, and check the second element again*/
			checkResult(pEnumLoc->Reset());
			checkResult(pEnumLoc->Skip(1));
			checkResult(pEnumLoc->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf2URL) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;

			/* Reset, and read both elements */
			checkResult(pEnumLoc->Reset());
			checkResult(pEnumLoc->Next (2, (IAAFLocator **)&pArray, &resultCount));
			checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
			checkResult(pArrayPoint[0]->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf1URL) == 0, AAFRESULT_TEST_FAILED);
			pArrayPoint[0]->Release();
			pArrayPoint[0] = NULL;
			
			checkResult(pArrayPoint[1]->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf2URL) == 0, AAFRESULT_TEST_FAILED);
			pArrayPoint[1]->Release();
			pArrayPoint[1] = NULL;
			
			/* Read one past to make sure that it fails */
			checkExpression(pEnumLoc->NextOne(&pLocator) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
			/* Clone the enumerator, and read one element */
			checkResult(pEnumLoc->Clone(&pCloneEnum));
			checkResult(pCloneEnum->Reset());
			checkResult(pCloneEnum->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, manuf1URL) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pEnumPluggable)
		pEnumPluggable->Release();

	if (pCloneEnum)
		pCloneEnum->Release();

	if (pEnumLoc)
		pEnumLoc->Release();

	if (pLocator)
		pLocator->Release();

  if (pNetLoc)
		pNetLoc->Release();

	if (pPlugin)
		pPlugin->Release();

	if (pEnumDesc)
		pEnumDesc->Release();

	if (pCodecDef)
		pCodecDef->Release();
	if (pDefObj)
		pDefObj->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CEnumAAFPluginLocators_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFPluginLocators_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CEnumAAFPluginLocators_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFPluginLocators methods have not been implemented:" << endl;       
//		cout << "     Next" << endl; 
//		cout << "     Skip" << endl; 
//		cout << "     Reset" << endl; 
//		cout << "     Clone" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
