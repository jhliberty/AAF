
//=---------------------------------------------------------------------=
//
// $Id: CAAFNestedScopeTest.cpp,v 1.35 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFNestedScope; // generated



static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc8e35d46, 0x0403, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const	aafMobID_t	TEST_referencedMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xcf95980c, 0x0403, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
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
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMob*			pMob = NULL;
	IAAFTimelineMobSlot*		pMobSlot = NULL;
	IAAFMob*			pReferencedMob = NULL;
	IAAFSourceClip*		pSourceClip = NULL;
	IAAFFiller*			pFiller = NULL;
	IAAFComponent*		pComponent = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFNestedScope*		pNestedScope = NULL;
	IAAFCompositionMob*	pCompMob = NULL;
	aafSourceRef_t		sourceRef; 
	aafLength_t			fillerLength = 3200;
	aafUInt32			numSegments;

	HRESULT				hr = AAFRESULT_SUCCESS;

	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		aafProductVersion_t			testRev;
       checkResult(pHeader->GetRefImplVersion(&testRev));

	   // Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
		// Create a mob to be referenced by the source clip
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pReferencedMob));
		checkResult(pReferencedMob->SetMobID(TEST_referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));
		checkResult(pHeader->AddMob(pReferencedMob));
		pReferencedMob->Release();
		pReferencedMob = NULL;

		// Create a Composition Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFCompositionMob, 
								   (IUnknown **)&pCompMob));

	    // get a IAAFMob interface
		checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFNestedScopeTest"));
	  
		// Create a Source clip 
 		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceClip, 
								   (IUnknown **)&pSourceClip));		
		 checkResult(pSourceClip->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		 checkResult(pComponent->SetLength(fillerLength));
		pComponent->Release();
		pComponent = NULL;

		// Set the properties for the SourceClip
		sourceRef.sourceID = TEST_referencedMobID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		checkResult(pSourceClip->SetSourceReference(sourceRef));

		// create a filler 
	    checkResult(defs.cdFiller()->
					CreateInstance(IID_IAAFFiller, 
								   (IUnknown **)&pFiller));
		// Set its properties.
	    checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));

		// Now create a nested scope 
	    checkResult(defs.cdNestedScope()->
					CreateInstance(IID_IAAFNestedScope, 
								   (IUnknown **)&pNestedScope));
		 checkResult(pNestedScope->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		 checkResult(pComponent->SetLength(fillerLength));
		pComponent->Release();
		pComponent = NULL;

		// Get a segment interface from the source clip
		checkResult(pSourceClip->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		// -----------------------------------------------------------------
		// Set all properties on the nested scope 
		//	Set the selected segment on the nested scope 
		checkResult(pNestedScope->AppendSegment(pSegment));
		// Release the interface so we can reuse the pointer
		pSegment->Release();
		pSegment = NULL;
		pFiller->Release();
		pFiller = NULL;

		checkResult(GetAAFVersions(pHeader, &testRev, NULL));

		if(DR4TestSupported(testRev))
		{
			// Prepend a new filler
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFFiller, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));
			checkResult(pFiller->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
			checkResult(pNestedScope->PrependSegment(pSegment));
			// Release the intreface so we can reuse the pointer
			pFiller->Release();
			pFiller = NULL;
			pSegment->Release();
			pSegment = NULL;
			
			// Create another filler, set its properties, and insert it in the middle
			checkResult(defs.cdFiller()->
				CreateInstance(IID_IAAFFiller, 
				(IUnknown **)&pFiller));
			checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));
			checkResult(pFiller->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
			checkResult(pNestedScope->InsertSegmentAt(1,pSegment));
			pFiller->Release();
			pFiller = NULL;
			pSegment->Release();
			pSegment = NULL;
			checkResult(pNestedScope->CountSegments (&numSegments));
			checkExpression(3 == numSegments, AAFRESULT_TEST_FAILED);
			checkResult(pNestedScope->RemoveSegmentAt (2));
			checkResult(pNestedScope->CountSegments (&numSegments));
			checkExpression(2 == numSegments, AAFRESULT_TEST_FAILED);
		}

		checkResult(pNestedScope->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
	    // append the Selector to the MOB tree
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot(editRate,
												pSegment,
												1,
												L"SelectorSlot",
												0,
												&pMobSlot)); 
		
		// Add the composition mob to the file
		pHeader->AddMob(pMob);

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pMobSlot)
		pMobSlot->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pFiller)
		pFiller->Release();

	if (pSegment)
		pSegment->Release();

	if (pComponent)
		pComponent->Release();

	if (pNestedScope)
		pNestedScope->Release();

	if (pCompMob)
		pCompMob->Release();

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
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IEnumAAFMobs*		pMobIter = NULL;
	IAAFMob*			pMob = NULL;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFCompositionMob*	pCompMob = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFComponent*		pComponent = NULL;
	IAAFFiller*			pFiller = NULL;
	IAAFNestedScope*		pNestedScope = NULL;
	IEnumAAFSegments*	pSegIter = NULL;
	aafUInt32			expectedChoices;
	aafNumSlots_t		numMobs;

	aafSearchCrit_t		criteria;
	HRESULT				hr = AAFRESULT_SUCCESS;


	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		// Validate that there is only one composition mob.
		checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		aafProductVersion_t testRev, testFileRev;
		checkResult(GetAAFVersions(pHeader, &testRev, &testFileRev));

	   // Enumerate over Composition MOBs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{
			aafNumSlots_t		numSlots = 0;

			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

			// Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				checkResult(pSlot->GetSegment(&pSegment));
				// See if it is a Selector
				checkResult(pSegment->QueryInterface(IID_IAAFNestedScope, (void **) &pNestedScope));

				aafUInt32 numSegments;
				checkResult(pNestedScope->CountSegments (&numSegments));

				if ( DR4TestSupported(testRev) && DR4TestSupported(testFileRev) )
					expectedChoices = 2;
				else
					expectedChoices = 1;
				checkExpression(expectedChoices == numSegments, AAFRESULT_TEST_FAILED);

				// -----------------------------------------------------------				
				// Enumerate slots
				checkResult(pNestedScope->GetSegments(&pSegIter));
				pSegment->Release();
				pSegment = NULL;

				aafUInt32 segmentIndex=0;
				while (pSegIter && pSegIter->NextOne(&pSegment) == AAFRESULT_SUCCESS)
				{
					// Make sure further segments are filler & verify lengths
					if(expectedChoices != 1)
					{
						checkResult(pSegment->QueryInterface(IID_IAAFFiller, (void **)&pFiller));
					}
					checkResult(pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
					aafLength_t fillerLength;
					checkResult(pComponent->GetLength(&fillerLength));
					checkExpression(fillerLength==3200/*+segmentIndex*/);
					pSegment->Release();
					pSegment = NULL;
					if(expectedChoices != 1)
					{
						pFiller->Release();
						pFiller = NULL;
					}
					pComponent->Release();
					pComponent = NULL;
					segmentIndex++;
				}
				checkExpression(segmentIndex==expectedChoices);

				pSegIter->Release();
				pSegIter = NULL;

				pNestedScope->Release();
				pNestedScope = NULL;

				pSlot->Release();
				pSlot = NULL;
			}

			pSlotIter->Release();
			pSlotIter = NULL;
		}
		pMobIter->Release();
		pMobIter = NULL;
	}
	catch (HRESULT& rResult)
	{
	    hr = rResult;
	}

	// Cleanup object references
 	if (pCompMob)
		pCompMob->Release();

	if (pSegment)
		pSegment->Release();

	if (pSlot)
		pSlot->Release();

	if (pSlotIter)
		pSlotIter->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pSegIter)
		pSegIter->Release();

	if (pFiller)
		pFiller->Release();

	if (pHeader) pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}


	return 	hr;
}

extern "C" HRESULT CAAFNestedScope_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFNestedScope_test(
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
		cerr << "CAAFNestedScope_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
