//=---------------------------------------------------------------------=
//
// $Id: CEnumAAFOperationDefsTest.cpp,v 1.37 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

inline void verifyAUID(const aafUID_t *uid1, const aafUID_t **effectIDs, bool *bFoundArray)
{
	for (int i=0; i < 2; ++i)
		if (0 == memcmp(uid1, effectIDs[i], sizeof(aafUID_t)) )
			if (bFoundArray[i] == false)
			{
				bFoundArray[i] = true;
				break;
			}
			else
				// Found the same AUID
				throw AAFRESULT_TEST_FAILED;			
		else if (i == 1 )
			// Should only get here if effectIDs does not contain uid1
			throw AAFRESULT_TEST_FAILED;			
}

inline void resetFoundArray(bool *bFoundArray)
{
	for (int i=0; i < 2; i++)
		bFoundArray[i] = false;
}

inline void setAUIDtoNULL(aafUID_t *anAUID)
{		
	anAUID->Data1 = 0;
	anAUID->Data2 = 0;
	anAUID->Data3 = 0;
	for (int i=0; i < 8; ++i)
		anAUID->Data4[i] = 0;
}

#define TEST_NUM_INPUTS		1
static const aafUID_t TEST_CATEGORY = 
{ 0x0d010102, 0x0101, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };

#define TEST_BYPASS			1
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
static const wchar_t *sName[2] = { L"Test Descriptor Name1", L"Test Descriptor Name2" };
static const wchar_t *sDescription[2] = { L"Test Descriptor Description1", L"Test Descriptor Description2" };

#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"

const aafUID_t kTestEffectID1 = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID1 = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
// {81831633-EDF4-11d3-A353-009027DFCA6A}
const aafUID_t kTestEffectID2 = 
{ 0x81831633, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };
// {81831634-EDF4-11d3-A353-009027DFCA6A}
const aafUID_t kTestParmID2 = 
{ 0x81831634, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

const aafUID_t *effectIDs[] = { &kTestEffectID1, &kTestEffectID2 };
const aafUID_t *parmIDs[] = { &kTestParmID1, &kTestParmID2 };

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  IAAFFile*			pFile = NULL;
  IAAFHeader *		pHeader = NULL;
  IAAFDictionary*		pDictionary = NULL;
  IAAFDefObject*		pDef = NULL;
  IAAFOperationDef*	pOperationDef = NULL;
  IAAFParameterDef*	pParamDef = NULL;
  bool				bFileOpen = false;
  HRESULT				hr = S_OK;
  long				n;

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
    
	for(n = 0; n < 2; n++)
	{
		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));    
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));
		
		checkResult(pOperationDef->Initialize (*(effectIDs[n]), TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pParamDef->Initialize (*(parmIDs[n]), TEST_PARAM_NAME, TEST_PARAM_DESC, defs.tdRational()));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef));
		checkResult(pDef->SetName (sName[n]));
		checkResult(pDef->SetDescription (sDescription[n]));
		pDef->Release();
		pDef = NULL;
		
		//!!!Not testing the INIT on AAFDefObject
		checkResult(pOperationDef->SetDataDef (defs.ddkAAFPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));
		
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef));
		checkResult(pDef->SetName (TEST_PARAM_NAME));
		checkResult(pDef->SetDescription (TEST_PARAM_DESC));
		pDef->Release();
		pDef = NULL;
		pParamDef->Release();
		pParamDef = NULL;
		
		pOperationDef->Release();
		pOperationDef = NULL;
	}
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pDef)
    pDef->Release();

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
	IEnumAAFOperationDefs *pPlug = NULL;
	IEnumAAFOperationDefs *pClonePlug = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFOperationDef*	pOperationDef = NULL;
	IAAFOperationDef*	pArray[2] = { NULL, NULL };
	IAAFOperationDef**	pArrayDef = pArray;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUInt32			resultCount;
	aafUID_t			tempAUID;
	bool				bFoundArray[2] = {false, false};		

	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetOperationDefs(&pPlug));
		/* Read and check the first element */
		resetFoundArray(bFoundArray);
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;

		/* Read and check the second element */
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, and check the first element again*/
		resetFoundArray(bFoundArray);
		checkResult(pPlug->Reset());
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, Skip, and check the second element again*/
		checkResult(pPlug->Reset());
		checkResult(pPlug->Skip(1));
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, and read both elements */
		resetFoundArray(bFoundArray);
		checkResult(pPlug->Reset());
		checkResult(pPlug->Next (2, (IAAFOperationDef **)&pArray, &resultCount));
		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		checkResult(pArrayDef[0]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pDef->Release();
		pDef = NULL;
		checkResult(pArrayDef[1]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pDef->Release();
		pDef = NULL;
		/* Read one past to make sure that it fails */
		checkExpression(pPlug->NextOne(&pOperationDef) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		/* Clone the enumerator, and read one element */
		checkResult(pPlug->Clone(&pClonePlug));
		checkResult(pClonePlug->Reset());
		checkResult(pClonePlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		resetFoundArray(bFoundArray);
		setAUIDtoNULL(&tempAUID);
		checkResult(pDef->GetAUID (&tempAUID));
		verifyAUID(&tempAUID, effectIDs, bFoundArray);

		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pHeader)
		pHeader->Release();
      
	if (pPlug)
		pPlug->Release();

	if (pClonePlug)
		pClonePlug->Release();

	if (pDef)
		pDef->Release();
      
	if (pOperationDef)
		pOperationDef->Release();
            
	if (pArray[0])
		pArray[0]->Release();
	if (pArray[1])
		pArray[1]->Release();
            
	if (pDictionary)
		pDictionary->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CEnumAAFOperationDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFOperationDefs_test(
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
		cerr << "CEnumAAFOperationDefs_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	return hr;
}
