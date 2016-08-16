//=---------------------------------------------------------------------=
//
// $Id: CEnumAAFDataDefsTest.cpp,v 1.28 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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
#include <stdio.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#define kNumComponents	5

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x3dd432fe, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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



static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*		pMob = NULL;
	IAAFTimelineMobSlot*	pMobSlot = NULL;
	IAAFSequence*	pSequence = NULL;
	IAAFSegment*	pSegment = NULL;
	IAAFComponent*	pComponent = NULL;
	int				i;
	HRESULT			hr = S_OK;
	
	
	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		// Create a Composition Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"EnumAAFDataDefTest"));
		
		// Add mob slot w/ Sequence
		checkResult(defs.cdSequence()->
					CreateInstance(IID_IAAFSequence, 
								   (IUnknown **)&pSequence));		
		checkResult(pSequence->Initialize(defs.ddkAAFPicture()));
		
		//
		//	Add some segments.  Need to test failure conditions
		//	(i.e. starting/ending w/ transition, two trans back
		//	to bacl).
		//
		for(i = 0; i < kNumComponents; i++)
		{
			aafLength_t		len = 10;
			
			checkResult(defs.cdFiller()->
						CreateInstance(IID_IAAFComponent, 
									   (IUnknown **)&pComponent));
			
			if(i == 0)
			{
				checkResult(pComponent->SetDataDef(defs.ddkAAFPictureWithMatte()));
			}
			else
			{
				checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
			}

			checkResult(pComponent->SetLength(len));
			checkResult(pSequence->AppendComponent(pComponent));
			
			pComponent->Release();
			pComponent = NULL;
		}
		
		checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot(editRate,
												pSegment,
												1,
												L"AAF Test Sequence",
												0,
												&pMobSlot));
		
		pMobSlot->Release();
		pMobSlot = NULL;
		
		pSegment->Release();
		pSegment = NULL;
		
		// Add the master mob to the file and cleanup
		pHeader->AddMob(pMob);
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (pMobSlot)
		pMobSlot->Release();
	
	if (pSegment)
		pSegment->Release();
	
	if (pComponent)
		pComponent->Release();
	
	if (pSequence)
		pSequence->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}
	
	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFComponent*		pComp = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFDataDef*		pDataDef = NULL;
	IAAFSequence*		pSequence = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFDataDefs*	pEnumDataDef = NULL;
	IEnumAAFDataDefs*	pCloneEnum = NULL;
	IEnumAAFComponents*	pCompIter = NULL;
	IAAFDataDef*		pArray[2] = { NULL, NULL };
	aafNumSlots_t		numMobs;
	aafInt32			index;
	aafSearchCrit_t		criteria;
	HRESULT				hr = S_OK;
	aafBool				testBool;
	aafUInt32			resultCount;
	
	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Validate that there is only one composition mob.
		checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// The test can't check the types on these because the order of adding data definitions
		// is defined by the toolkit, and not the test.  !!!Change this to determine the order on
		// the first two tests, and then use to test the other functions.

		checkResult(pDictionary->GetDataDefs(&pEnumDataDef));
		/* Read and check the first element */
		checkResult(pEnumDataDef->NextOne(&pDataDef));
		checkResult(pDataDef->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pDataDef->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pDataDef->Release();
		pDataDef = NULL;
		
		/**/
		/* Read and check the second element */
		checkResult(pEnumDataDef->NextOne(&pDataDef));
		checkResult(pDataDef->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pDataDef->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pDataDef->Release();
		pDataDef = NULL;
		/*****/
		
		/* Reset, and check the first element again*/
		checkResult(pEnumDataDef->Reset());
		checkResult(pEnumDataDef->NextOne(&pDataDef));
		checkResult(pDataDef->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pDataDef->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pDataDef->Release();
		pDataDef = NULL;
		
		/* Reset, Skip, and check the second element again*/
		checkResult(pEnumDataDef->Reset());
		checkResult(pEnumDataDef->Skip(1));
		checkResult(pEnumDataDef->NextOne(&pDataDef));
		checkResult(pDataDef->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pDataDef->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pDataDef->Release();
		pDataDef = NULL;
		
		/* Reset, and read both elements */
		checkResult(pEnumDataDef->Reset());
		checkResult(pEnumDataDef->Next (2, (IAAFDataDef **)&pArray, &resultCount));
		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		checkResult(pArray[0]->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pArray[0]->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pArray[0]->Release();
		pArray[0] = NULL;
		
		checkResult(pArray[1]->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pArray[1]->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pArray[1]->Release();
		pArray[1] = NULL;
		
//		/* Read one past to make sure that it fails */
//		checkExpression(pEnumDataDef->NextOne(&pDataDef) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		/* Clone the enumerator, and read one element */
		checkResult(pEnumDataDef->Clone(&pCloneEnum));
		checkResult(pCloneEnum->Reset());
		checkResult(pCloneEnum->NextOne(&pDataDef));
		checkResult(pDataDef->IsPictureKind(&testBool));
//		checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pDataDef->IsSoundKind(&testBool));
//		checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
		pDataDef->Release();
		pDataDef = NULL;
		pCloneEnum->Release();
		pCloneEnum = NULL;
		
		// Enumerate over Composition MOBs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		CAAFBuiltinDefs defs (pDictionary);
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{
			aafNumSlots_t		numSlots = 0;
			
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
			
			// Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				aafUInt32			numCpnts;
				
				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
				
				checkResult(pSequence->CountComponents(&numCpnts));
				checkExpression(numCpnts == kNumComponents, AAFRESULT_TEST_FAILED);
				
				checkResult(pSequence->GetComponents(&pCompIter));
				numCpnts = 0;
				index = 0;
				while (pCompIter && pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
				{
					aafBool		testBool;
					
					numCpnts++;
					
					checkResult(pComp->GetDataDef(&pDataDef));
					checkResult(pDataDef->IsSoundKind(&testBool));
					checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
					checkResult(pDataDef->IsMatteKind(&testBool));
					checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
					
					if(index == 0)	// First segment is Picture with Matte, converts to picture
					{
						checkResult(pDataDef->IsDataDefOf(defs.ddkAAFPictureWithMatte(),
														  &testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->IsPictureKind(&testBool));
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->IsPictureWithMatteKind(&testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->DoesDataDefConvertTo (defs.ddkAAFPicture(), &testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					}
					else		// First segment is Picture, converts from picture with Matte
					{
						checkResult(pDataDef->IsDataDefOf(defs.ddkAAFPicture(), &testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->IsPictureKind(&testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->IsPictureWithMatteKind(&testBool));
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->DoesDataDefConvertFrom (defs.ddkAAFPictureWithMatte(),
																	  &testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					}
					checkResult(pDataDef->DoesDataDefConvertTo (defs.ddkAAFSound(),
																&testBool));
					checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
					checkResult(pDataDef->DoesDataDefConvertFrom (defs.ddkAAFSound(), &testBool));
					checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
					
					pComp->Release();
					pComp = NULL;
					pDataDef->Release();
					pDataDef = NULL;
					index++;
				}
				
				pCompIter->Release();
				pCompIter = NULL;
				
				pSequence->Release();
				pSequence = NULL;
				
				pSegment->Release();
				pSegment = NULL;
				
				pSlot->Release();
				pSlot = NULL;
			}
			
			pSlotIter->Release();
			pSlotIter = NULL;
			
			pMob->Release();
			pMob = NULL;
		}
		
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (pComp)
		pComp->Release();
	

	if (pEnumDataDef)
		pEnumDataDef->Release();
	if (pCloneEnum)
		pCloneEnum->Release();
	if (pCompIter)
		pCompIter->Release();
	if (pArray[0])
		pArray[0]->Release();
	if (pArray[1])
		pArray[1]->Release();
	
	if (pDataDef)
		pDataDef->Release();
	
	if (pSequence)
		pSequence->Release();
	
	if (pSegment)
		pSegment->Release();
	
	if (pSlot)
		pSlot->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pSlotIter)
		pSlotIter->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pMobIter)
		pMobIter->Release();
	
	if (pHeader) pHeader->Release();
	
	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}
	
	
	return 	hr;
}

extern "C" HRESULT CEnumAAFDataDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFDataDefs_test(
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
		cerr << "CEnumAAFDataDef_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following EnumAAFDataDef methods have not been implemented:" << endl; 
//		cout << "     IsDataDefOf - needs unit test" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
