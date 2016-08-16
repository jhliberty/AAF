//=---------------------------------------------------------------------=
//
//
// $Id: CAAFDescriptiveClipTest.cpp,v 1.9 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include "ModuleTestsCommon.h"

#include "ModuleTest.h"

#include "AAF.h"
#include "AAFResult.h"
#include "AAFTypes.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyDefs.h"
#include "CAAFBuiltinDefs.h"
#include "AAFDefUIDs.h"

#include <iostream>

#include <string.h>

using namespace std;


static aafUInt32 TestDescribedSlotIDsVector[] = { 1, 3, 5, 7, 11 };
static const aafUInt32 TestDescribedSlotIDsVectorSize = 5;

static aafSourceRef_t sourceRef; 
static const aafWChar *slotName = L"SLOT1";

static const	aafMobID_t	TEST_referencedMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xa85e495e, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xa121c634, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

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
  // IAAFSession*				pSession = NULL;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
  	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IAAFTimelineMobSlot*		newSlot = NULL;
	IAAFSegment*				seg = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFDescriptiveClip*		 pDescClip = NULL;
	bool bFileOpen = false;
	HRESULT						hr = AAFRESULT_SUCCESS;
	
	try
	{
		
	    // Remove the previous test file if any.
	    RemoveTestFile(pFileName);

		// Create the file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		//Make the MOB to be referenced
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pReferencedMob));
		checkResult(pReferencedMob->SetMobID(TEST_referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFDescriptiveClipTest::ReferencedMob"));

		// Create a Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFDescriptiveClipTest"));

		// Create a DescriptiveClip
		checkResult(defs.cdDescriptiveClip()->
					CreateInstance(IID_IAAFDescriptiveClip, 
								   (IUnknown **)&pDescClip));
		checkResult(pDescClip->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		checkResult(pComponent->SetDataDef(defs.ddkAAFSound()));
		pComponent->Release();
		pComponent = NULL;
		
		// Set the properties for the SourceClip
		//checkResult(pDescClip->SetFade( fadeInLen, fadeInType, fadeOutLen, fadeOutType));
	
		sourceRef.sourceID = TEST_referencedMobID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		checkResult(pDescClip->Initialize(defs.ddkAAFSound(), 1000, sourceRef));

    	// Get described slots - should not be present.
    	aafUInt32 size = 0;
    	AAFRESULT hr = pDescClip->CountDescribedSlotIDs( &size );
    	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr, AAFRESULT_TEST_FAILED );
    	
	    // Set/Get single described slot id
	    aafUInt32 setSingleDescribedSlotID = 0xdeadbeef;
	    checkResult( pDescClip->AddDescribedSlotID( setSingleDescribedSlotID ) );
	    aafUInt32 getSingleDescribedSlotID = 0;
	    checkResult( pDescClip->GetDescribedSlotIDs( 1, &getSingleDescribedSlotID ) );
	    checkExpression( setSingleDescribedSlotID == getSingleDescribedSlotID, AAFRESULT_TEST_FAILED );

	    // Clear the ID set for the next test
	    checkResult( pDescClip->RemoveDescribedSlotID( setSingleDescribedSlotID ) );

	    // Set the persistent described slots.
	    for( aafUInt32 i_tid = 0; i_tid < TestDescribedSlotIDsVectorSize; ++i_tid )
	    {
		    checkResult( pDescClip->AddDescribedSlotID( TestDescribedSlotIDsVector[i_tid] ) );
	    }

		aafUInt32 getDescribedSlotIDsVector[TestDescribedSlotIDsVectorSize];

		checkResult( pDescClip->GetDescribedSlotIDs( TestDescribedSlotIDsVectorSize, getDescribedSlotIDsVector ) );
		checkExpression( 0 == memcmp( getDescribedSlotIDsVector, TestDescribedSlotIDsVector, sizeof(TestDescribedSlotIDsVector) ),
					     AAFRESULT_TEST_FAILED );
		
		checkResult(pDescClip->QueryInterface (IID_IAAFSegment, (void **)&seg));
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot (editRate,
												 seg,
												 1,
												 slotName,
												 0,
												 &newSlot));
		checkResult(pHeader->AddMob(pMob));
		checkResult(pHeader->AddMob(pReferencedMob));
  	}
  	catch( const AAFRESULT& hr ) {
    	return hr;
  	}
  		// Cleanup and return


		if (seg)
			seg->Release();

		if (newSlot)
			newSlot->Release();

		if (pDescClip)
			pDescClip->Release();
	
		if (pComponent)
			pComponent->Release();
	
		if (pMob)
			pMob->Release();
	
		if (pReferencedMob)
			pReferencedMob->Release();
	
		if (pDictionary)
			pDictionary->Release();
	
		if (pHeader)
			pHeader->Release();
	
		if (pFile) 
		{
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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFDescriptiveClip*				pDescClip = NULL;
	bool bFileOpen = false;
	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	HRESULT						hr = AAFRESULT_SUCCESS;

	try
	{ 
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(2 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

			checkResult(pMob->GetSlots(&pSlotIter));
			while (AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
			{
				// The segment should be a source clip...
				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFDescriptiveClip, (void **) &pDescClip));
				
				// Get, and test, the described slots from the marker.
			    aafUInt32 getDescribedSlotIDsVector[TestDescribedSlotIDsVectorSize];
			    aafUInt32 getDescribedSlotIDsVectorSize = 0;
			
			    checkResult( pDescClip->CountDescribedSlotIDs( &getDescribedSlotIDsVectorSize ) );
			    checkExpression( TestDescribedSlotIDsVectorSize == getDescribedSlotIDsVectorSize, AAFRESULT_TEST_FAILED );
			
			    checkExpression( AAFRESULT_SMALLBUF ==
					       pDescClip->GetDescribedSlotIDs( getDescribedSlotIDsVectorSize-1, getDescribedSlotIDsVector ),
					     AAFRESULT_TEST_FAILED );
			
			    checkResult( pDescClip->GetDescribedSlotIDs( getDescribedSlotIDsVectorSize, getDescribedSlotIDsVector ) );
			    checkExpression( 0 == memcmp( getDescribedSlotIDsVector, TestDescribedSlotIDsVector, sizeof(TestDescribedSlotIDsVector) ),
					     AAFRESULT_TEST_FAILED );
			
				
				
				pSlot->Release();
				pSlot = NULL;
			}

			pMob->Release();
			pMob = NULL;
		}

	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

	// Cleanup and return
	if (pReferencedMob)
		pReferencedMob->Release();

	if (pDescClip)
		pDescClip->Release();

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

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}


extern "C" HRESULT CAAFDescriptiveClip_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFDescriptiveClip_test(
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
	  cerr << "CAAFSourceClip_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


	return hr;
}
