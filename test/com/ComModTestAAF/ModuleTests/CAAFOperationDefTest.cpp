// @doc INTERNAL
// @com This file implements the module test for CAAFOperationDef

//=---------------------------------------------------------------------=
//
// $Id: CAAFOperationDefTest.cpp,v 1.41 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include "CAAFBuiltinDefs.h"

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

#define TEST_NUM_INPUTS		1
static const aafUID_t TEST_CATEGORY = 
{ 0x0d010102, 0x0101, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
#define TEST_BYPASS			1
#define TEST_EFFECT_NAME1	L"A TestEffect"
#define TEST_EFFECT_DESC1	L"A longer description of the TestEffect"
#define TEST_EFFECT_NAME2	L"An appended TestEffect"
#define TEST_EFFECT_DESC2	L"A longer description of the appended TestEffect"
#define TEST_EFFECT_NAME3	L"A prepended TestEffect"
#define TEST_EFFECT_DESC3	L"A longer description of the prepended TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"

const aafUID_t kParmID = { 0x88928EB4, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static const wchar_t *effectNames[] = { TEST_EFFECT_NAME1, TEST_EFFECT_NAME2, TEST_EFFECT_NAME3 };
static const wchar_t *effectDesc[] = { TEST_EFFECT_DESC1, TEST_EFFECT_DESC2, TEST_EFFECT_DESC3 };
static aafUID_t effectID[3] =
	{ { 0x88928EB1, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } },
	{ 0x88928EB2, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } },
	{ 0x88928EB3, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } } };

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFParameterDef*	pParamDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUID_t			testParmID = kParmID;
	aafInt32			index;
	IAAFOperationDef	*pOperationDef = NULL, *defResults[3] = { NULL, NULL, NULL };

/*	long				test;
*/

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
    
 		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));
		checkResult(pParamDef->Initialize (testParmID, TEST_PARAM_NAME, TEST_PARAM_DESC, defs.tdRational()));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));

		for(index = 0; index < 3; index++)
		{
			checkResult(defs.cdOperationDef()->
						CreateInstance(IID_IAAFOperationDef, 
									   (IUnknown **)&pOperationDef));
			checkResult(pOperationDef->Initialize (effectID[index], effectNames[index], effectDesc[index]));
			checkResult(pDictionary->RegisterOperationDef(pOperationDef));
			
			
			checkResult(pOperationDef->SetDataDef (defs.ddkAAFPicture()));
			checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
			checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
			checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
			checkResult(pOperationDef->AddParameterDef (pParamDef));
			checkResult(pOperationDef->SetBypass (TEST_BYPASS));
			
			defResults[index] = pOperationDef;
			pOperationDef = NULL;
		}

		checkResult(defResults[1]->AppendDegradeToOperation (defResults[1]));
		checkResult(defResults[2]->AppendDegradeToOperation (defResults[2]));
		checkResult(defResults[0]->AppendDegradeToOperation (defResults[1]));
		checkResult(defResults[0]->PrependDegradeToOperation (defResults[2]));
		// Add an extra one to delete for the test
//!!!		checkResult(defResults[0]->CountDegradeToOperations(&numDegrade));
//		checkExpression(2 == numDegrade, AAFRESULT_TEST_FAILED);
//		checkResult(defResults[0]->AppendDegradeToOperation (defResults[1]));
//		checkResult(defResults[0]->CountDegradeToOperations(&numDegrade));
//		checkExpression(3 == numDegrade, AAFRESULT_TEST_FAILED);
//		checkResult(defResults[0]->RemoveDegradeToOperationAt (2));
//		checkResult(defResults[0]->CountDegradeToOperations(&numDegrade));
//		checkExpression(2 == numDegrade, AAFRESULT_TEST_FAILED);

		for(index = 0; index < 3; index++)
		{
			defResults[index]->Release();
			defResults[index] = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


	// Cleanup and return
	if (pOperationDef)
		pOperationDef->Release();

	if (pParamDef)
		pParamDef->Release();

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
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFOperationDefs *pEffectEnum = NULL;
	IEnumAAFOperationDefs *pDegradeEnum = NULL;
	IEnumAAFParameterDefs *pParmDefEnum = NULL;
	IAAFOperationDef		*pOperationDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
	bool				bFileOpen = false;
	IAAFDataDefSP		pReadDataDef;
	aafBool				readIsTimeWarp;
	aafInt32			checkNumInputs;
	aafUInt32			checkBypass;
	HRESULT				hr = S_OK;
	wchar_t				checkName[256];
	aafUID_t			checkCat;
	aafBool				bResult = kAAFFalse;

	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
	
		checkResult(pDictionary->GetOperationDefs(&pEffectEnum));
		checkResult(pEffectEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->GetDataDef(&pReadDataDef));

		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectNames[0]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectDesc[0]) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		
		checkResult(pReadDataDef->IsDataDefOf(defs.ddkAAFPicture(), &bResult));
		checkExpression(bResult == kAAFTrue, AAFRESULT_TEST_FAILED);
						
		checkResult(pOperationDef->IsTimeWarp (&readIsTimeWarp));
		checkExpression(readIsTimeWarp == kAAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetCategory (&checkCat));
		checkExpression(memcmp(&checkCat, &TEST_CATEGORY, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetBypass (&checkBypass));
		checkExpression(checkBypass == TEST_BYPASS, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetNumberInputs (&checkNumInputs));
		checkExpression(checkNumInputs == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetParameterDefs (&pParmDefEnum));
		checkResult(pParmDefEnum->NextOne (&pParmDef));

		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;

		checkResult(pOperationDef->GetDegradeToOperations (&pDegradeEnum));
		pOperationDef->Release();
		pOperationDef = NULL;

		// Check for prepended one first
		checkResult(pDegradeEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectNames[2]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectDesc[2]) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		pOperationDef->Release();
		pOperationDef = NULL;

		// Check for appended one second
		checkResult(pDegradeEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectNames[1]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectDesc[1]) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		pOperationDef->Release();
		pOperationDef = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pHeader)
		pHeader->Release();
      
	if (pDictionary)
		pDictionary->Release();
      
	if (pEffectEnum)
		pEffectEnum->Release();

	if (pDegradeEnum)
		pDegradeEnum->Release();

	if (pParmDefEnum)
		pParmDefEnum->Release();
      
	if (pOperationDef)
		pOperationDef->Release();
      
	if (pDefObject)
		pDefObject->Release();

	if (pParmDef)
		pParmDef->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CAAFOperationDef_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFOperationDef_test(
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
		cerr << "CAAFOperationDef_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFOperationDef methods have not been implemented:" << endl; 
//		cout << "     GetDegradeToOperations" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
