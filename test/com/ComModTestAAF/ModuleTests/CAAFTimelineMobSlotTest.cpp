//=---------------------------------------------------------------------=
//
// $Id: CAAFTimelineMobSlotTest.cpp,v 1.35 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>


#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

static aafPosition_t markInTestData[5] = { 0, 1, -3, AAFCONSTINT64(0x7ffffffffffffffd), AAFCONSTINT64(0x8000000000000000) };
static aafPosition_t userPosTestData[5] = { 0, 2, -2, AAFCONSTINT64(0x7ffffffffffffffe), AAFCONSTINT64(0x8000000000000001) };
static aafPosition_t markOutTestData[5] = { 0, 3, -1, AAFCONSTINT64(0x7fffffffffffffff), AAFCONSTINT64(0x8000000000000002) };

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x81e5b6d0, 0x0405, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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

static const aafRational_t	checkEditRate = { 30000, 1001 };

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile *				pFile = NULL;
	bool bFileOpen =		false;
	IAAFHeader *			pHeader = NULL;
	IAAFDictionary*			pDictionary = NULL;
	IAAFMob					*pMob = NULL;
	IAAFMobSlot				*newSlot = NULL;
	IAAFTimelineMobSlot		*timelineSlot = NULL;
	IAAFTimelineMobSlot2		*timelineSlot2 = NULL;
	IAAFSegment				*seg = NULL;
	IAAFSourceClip			*sclp = NULL;
	IAAFComponent*		pComponent = NULL;
	HRESULT						hr = S_OK;
	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		// Create the file.
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		//Make the first mob
		long	test;
		// audioRate not used
		// aafRational_t	audioRate = { 44100, 1 };
		
		// Create a concrete subclass of Mob
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"MOBTest"));
		
		// Add some slots
		for(test = 0; test < 5; test++)
		{
			checkResult(defs.cdSourceClip()->
						CreateInstance(IID_IAAFSourceClip, 
									   (IUnknown **)&sclp));		
		 checkResult(sclp->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		pComponent->Release();
		pComponent = NULL;
			
			checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
			
			// test  Interface
			// ----  ---------
			// 0     Use IAAFTimelineMobSlot for v1.0 props, don't add v1.1 props
			// 1     Use IAAFTimelineMobSlot for v1.0 props, IAAFTimelineMobSlot2 for v1.1 props
			// 2     Use IAAFTimelineMobSlot2 for all props
			// 3     Use IAAFTimelineMobSlot2 for all props
			// 4     Use IAAFTimelineMobSlot2 for all props

			if (test < 2)
			{
				checkResult(defs.cdTimelineMobSlot()->
							CreateInstance(IID_IAAFTimelineMobSlot, 
											 (IUnknown **)&timelineSlot));		
				checkResult(timelineSlot->SetEditRate (checkEditRate));
				checkResult(timelineSlot->SetOrigin (0));
				checkResult(timelineSlot->QueryInterface (IID_IAAFMobSlot, (void **)&newSlot));
				checkResult(timelineSlot->QueryInterface (IID_IAAFTimelineMobSlot2, (void **)&timelineSlot2));
			}
			else
			{
				checkResult(defs.cdTimelineMobSlot()->
							CreateInstance(IID_IAAFTimelineMobSlot2, 
											 (IUnknown **)&timelineSlot2));		
				checkResult(timelineSlot2->SetEditRate (checkEditRate));
				checkResult(timelineSlot2->SetOrigin (0));

				checkResult(timelineSlot2->QueryInterface (IID_IAAFMobSlot, (void **)&newSlot));
			}

			// use IAAFTimelineMobSlot2 to set MarkIn, MarkOut and UserPos
			// for test == 0, don't set these props, to check they are omitted correctly
			if (test != 0)
			{
				checkResult(timelineSlot2->SetMarkIn( markInTestData[test] ));
				checkResult(timelineSlot2->SetMarkOut( markOutTestData[test] ));
				checkResult(timelineSlot2->SetUserPos( userPosTestData[test] ));
			}

			checkResult(newSlot->SetSegment(seg));
			checkResult(newSlot->SetSlotID(test+1));
			checkResult(newSlot->SetName(slotNames[test]));

			checkResult(pMob->AppendSlot (newSlot));
			
			timelineSlot2->Release();
			timelineSlot2 = NULL;
			if (timelineSlot) {
				timelineSlot->Release();
				timelineSlot = NULL;
			}
			newSlot->Release();
			newSlot = NULL;
			
			seg->Release();
			seg = NULL;
			
			sclp->Release();
			sclp = NULL;
		}
		
		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (timelineSlot)
		timelineSlot->Release();

	if (timelineSlot2)
		timelineSlot2->Release();

	if (newSlot)
		newSlot->Release();
	
	if (seg)
		seg->Release();
	
	if (pComponent)
		pComponent->Release();

	if (sclp)
		sclp->Release();
	
	if (pMob)
		pMob->Release();
	
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

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *				pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *			pHeader = NULL;
	IEnumAAFMobs			*mobIter = NULL;
	IAAFMob					*aMob = NULL;
	IEnumAAFMobSlots		*slotIter = NULL;
	IAAFMobSlot				*slot = NULL;
	IAAFTimelineMobSlot		*timelineSlot = NULL;
	IAAFTimelineMobSlot2		*timelineSlot2 = NULL;
	aafNumSlots_t			numMobs, n, s;
	aafPosition_t			testOrigin;
	aafRational_t			testRate;
	aafPosition_t			testMarkIn = 0;
	aafPosition_t			testMarkOut = 0;
	aafPosition_t			testUserPos = 0;
	HRESULT					hr = S_OK;
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		
		aafSearchCrit_t		criteria;
		criteria.searchTag = kAAFNoSearch;
		checkResult(pHeader->GetMobs (&criteria, &mobIter));
		
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500], slotName[500];
			aafNumSlots_t	numSlots;
			aafMobID_t		mobID;
			aafSlotID_t		trackID;
			
			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));
			
			checkResult(aMob->CountSlots (&numSlots));
			checkExpression(5 == numSlots, AAFRESULT_TEST_FAILED);
			
			checkResult(aMob->GetSlots(&slotIter));
			

			for(s = 0; s < numSlots; s++)
			{
				// test  Interface
				// ----  ---------
				// 0     Use IAAFTimelineMobSlot for v1.0 props, don't add v1.1 props
				// 1     Use IAAFTimelineMobSlot for v1.0 props, IAAFTimelineMobSlot2 for v1.1 props
				// 2     Use IAAFTimelineMobSlot2 for all props
				// 3     Use IAAFTimelineMobSlot2 for all props
				// 4     Use IAAFTimelineMobSlot2 for all props

				checkResult(slotIter->NextOne (&slot));
				checkResult(slot->GetName (slotName, sizeof(slotName)));
				checkResult(slot->GetSlotID(&trackID));
				checkExpression (wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);

				if (s < 2)
				{
					checkResult(slot->QueryInterface(IID_IAAFTimelineMobSlot, (void **) &timelineSlot));
					checkResult(timelineSlot->GetEditRate (&testRate));
					checkExpression (testRate.numerator == checkEditRate.numerator, AAFRESULT_TEST_FAILED);
					checkExpression (testRate.denominator == checkEditRate.denominator, AAFRESULT_TEST_FAILED);
					checkResult(timelineSlot->GetOrigin (&testOrigin));
					checkExpression (testOrigin == 0, AAFRESULT_TEST_FAILED);
				
					checkResult(timelineSlot->QueryInterface (IID_IAAFTimelineMobSlot2, (void **)&timelineSlot2));
				}
				else
				{
					checkResult(slot->QueryInterface(IID_IAAFTimelineMobSlot2, (void **) &timelineSlot2));
					checkResult(timelineSlot2->GetEditRate (&testRate));
					checkExpression (testRate.numerator == checkEditRate.numerator, AAFRESULT_TEST_FAILED);
					checkExpression (testRate.denominator == checkEditRate.denominator, AAFRESULT_TEST_FAILED);
					checkResult(timelineSlot2->GetOrigin (&testOrigin));
					checkExpression (testOrigin == 0, AAFRESULT_TEST_FAILED);
				}

				if (s != 0)
				{
					checkResult( timelineSlot2->GetMarkIn( &testMarkIn ) );
					checkExpression( testMarkIn == markInTestData[s], AAFRESULT_TEST_FAILED);
					checkResult( timelineSlot2->GetMarkOut( &testMarkOut ) );
					checkExpression( testMarkOut == markOutTestData[s], AAFRESULT_TEST_FAILED);
					checkResult( timelineSlot2->GetUserPos( &testUserPos ) );
					checkExpression( testUserPos == userPosTestData[s], AAFRESULT_TEST_FAILED);
				}
				else
				{
					// check v1.1 props are not present for test == 0
					checkExpression( timelineSlot2->GetMarkIn( &testMarkIn ) == AAFRESULT_PROP_NOT_PRESENT, AAFRESULT_TEST_FAILED);
					checkExpression( timelineSlot2->GetMarkOut( &testMarkOut ) == AAFRESULT_PROP_NOT_PRESENT, AAFRESULT_TEST_FAILED);
					checkExpression( timelineSlot2->GetUserPos( &testUserPos ) == AAFRESULT_PROP_NOT_PRESENT, AAFRESULT_TEST_FAILED);
				}

				slot->Release();
				slot = NULL;
				if (timelineSlot) {
					timelineSlot->Release();
					timelineSlot = NULL;
				}
				timelineSlot2->Release();
				timelineSlot2 = NULL;
			}
			
			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (slot)
		slot->Release();
	
	if (timelineSlot2)
		timelineSlot2->Release();
	
	if (timelineSlot)
		timelineSlot->Release();
	
	if (slotIter)
		slotIter->Release();
	
	if (aMob)
		aMob->Release();
	
	if (mobIter)
		mobIter->Release();
	
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
 

extern "C" HRESULT CAAFTimelineMobSlot_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTimelineMobSlot_test(
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
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFTimelineMobSlot_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	// Cleanup our object if it exists.
	
	// When all of the functionality of this class is tested, we can return success
//!!!	if(hr == AAFRESULT_SUCCESS)
//	{
//		cout << "The following IAAFTimelineMobSlot methods have not been tested:" << endl; 
//		cout << "     SetEditRate" << endl; 
//		cout << "     SetOrigin" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}

