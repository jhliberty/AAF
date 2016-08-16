// @doc INTERNAL
// @com This file implements the module test for CAAFDataDef
//=---------------------------------------------------------------------=
//
// $Id: CAAFDataDefTest.cpp,v 1.33 2009/06/01 11:47:11 stuart_hc Exp $ $Name: V116 $
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

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#define kNumComponents	5
#define kNumSlots	11 // adjust slot-specific behaviour in test if you change this

static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x00c3de80, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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
	IAAFDictionary2*  pDictionary2 = NULL;
	IAAFMob*		pMob = NULL;
	IAAFTimelineMobSlot*	pMobSlot = NULL;
	IAAFSequence*	pSequence = NULL;
	IAAFSegment*	pSegment = NULL;
	IAAFComponent*	pComponent = NULL;
	IAAFDataDef*	pSeqDataDef = NULL;
	HRESULT			hr = S_OK;
	
	
	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		checkResult(pDictionary->QueryInterface (IID_IAAFDictionary2, (void **)&pDictionary2));
		
		CAAFBuiltinDefs defs (pDictionary);

		// Create a Composition Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFDataDefTest"));
		// Add the master mob to the file
		pHeader->AddMob(pMob);
		
		// Different component kinds are put on different slots because
		// they cannot be mixed within a slot
		for(int slot = 0; slot < kNumSlots; slot++)
		{

			// Add mob slot w/ Sequence
			checkResult(defs.cdSequence()->
						CreateInstance(IID_IAAFSequence, 
									   (IUnknown **)&pSequence));		

			if (slot==0)
				checkResult(pDictionary2->LookupPictureDataDef(&pSeqDataDef));
			else if (slot==1)
				checkResult(pDictionary2->LookupSoundDataDef(&pSeqDataDef));
			else if (slot==2)
				checkResult(pDictionary2->LookupTimecodeDataDef(&pSeqDataDef));
			else if (slot==3)
				checkResult(pDictionary2->LookupEdgecodeDataDef(&pSeqDataDef));
			else if (slot==4)
				checkResult(pDictionary2->LookupDescriptiveMetadataDataDef(&pSeqDataDef));
			else if (slot==5)
				checkResult(pDictionary2->LookupAuxiliaryDataDef(&pSeqDataDef));
			else if (slot==6)
				checkResult(pDictionary2->LookupLegacyPictureDataDef(&pSeqDataDef));
			else if (slot==7)
				checkResult(pDictionary2->LookupLegacySoundDataDef(&pSeqDataDef));
			else if (slot==8)
				checkResult(pDictionary2->LookupLegacyTimecodeDataDef(&pSeqDataDef));
			else if (slot==9)
				checkResult(pDictionary2->LookupPictureWithMatteDataDef(&pSeqDataDef));
			else if (slot==10)
				checkResult(pDictionary2->LookupMatteDataDef(&pSeqDataDef));

			checkResult(pSequence->Initialize( pSeqDataDef ));
			checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		
			aafRational_t editRate = { 0, 1};
			checkResult(pMob->AppendNewTimelineSlot(editRate,
													pSegment,
													slot+1, // SlotID is 1-based
													L"AAF Test Sequence",
													0,
													&pMobSlot));
			
			//
			//	Add some segments.  Need to test failure conditions
			//	(i.e. starting/ending w/ transition, two trans back
			//	to bacl).
			//
			for(int comp = 0; comp < kNumComponents; comp++)
			{
				aafLength_t		len = 10;
				
				checkResult(defs.cdFiller()->
							CreateInstance(IID_IAAFComponent, 
										   (IUnknown **)&pComponent));
				
				// This is the structure on the first and sixth slot:
				// 0 Picture        | PWM | LegPic | Pic | Pic | Pic |
				// 6 LegacyPic      | PWM | Pic | LegPic | LegPic | LegPic |

				// First component of picture slots gets a picture with matte component
				// so we can test that picture with matte converts to picture on read
				if((slot==0 || slot==6) && comp == 0)
				{
					checkResult(pComponent->SetDataDef(defs.ddkAAFPictureWithMatte()));
				}
				// Second component of picture slots gets a legacy picture component
				// so we can test that legacy picture is accepted into a picture sequence
				else if ((slot==0) && comp == 1)
				{
					checkResult(pComponent->SetDataDef(defs.ddPicture()));
				}
				// Second component of legacy picture slots gets a picture component
				// so we can test that picture is accepted into a legacy picture sequence
				else if ((slot==6) && comp == 1)
				{
					checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
				}
				else // otherwise component datadef matches sequence datadef
				{
					checkResult(pComponent->SetDataDef( pSeqDataDef ));
				}
				checkResult(pComponent->SetLength(len));
				checkResult(pSequence->AppendComponent(pComponent));
				
				pComponent->Release();
				pComponent = NULL;
			}
					
			pSeqDataDef->Release();
			pSeqDataDef = NULL;
			
			pMobSlot->Release();
			pMobSlot = NULL;
			
			pSegment->Release();
			pSegment = NULL;
			
			pSequence->Release();
			pSequence = NULL;
			
		}		
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
	
	if (pDictionary2)
		pDictionary2->Release();
	
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
	IAAFMob*		pMob = NULL;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFComponent*		pComp = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFDataDef*		pDataDef = NULL;
	IAAFDataDef2*		pDataDef2 = NULL;
	IAAFDataDef3*		pDataDef3 = NULL;
	IAAFSequence*		pSequence = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFComponents*	pCompIter = NULL;
	aafNumSlots_t		numMobs;
	aafSearchCrit_t		criteria;
	HRESULT				hr = S_OK;
	
	
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
		
		CAAFBuiltinDefs defs (pDictionary);

		// Enumerate over Composition MOBs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{
			aafNumSlots_t		numSlots = 0;
			
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(kNumSlots == numSlots, AAFRESULT_TEST_FAILED);
			
			// Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			int slot = 0;
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				aafUInt32			numCpnts;
				
				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
				
				checkResult(pSequence->CountComponents(&numCpnts));
				checkExpression(numCpnts == kNumComponents, AAFRESULT_TEST_FAILED);
				
				checkResult(pSequence->GetComponents(&pCompIter));
				numCpnts = 0;
				int comp = 0;
				while (pCompIter && pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
				{
					aafBool		testBool;

					numCpnts++;
					
					checkResult(pComp->GetDataDef(&pDataDef));
					checkResult(pDataDef->QueryInterface(IID_IAAFDataDef2, (void **) &pDataDef2));
					checkResult(pDataDef->QueryInterface(IID_IAAFDataDef3, (void **) &pDataDef3));

					// This is the structure on the first and sixth slot:
					// 0 Picture        | PWM | LegPic | Pic | Pic | Pic |
					// 6 LegacyPic      | PWM | Pic | LegPic | LegPic | LegPic |

					checkResult(pDataDef->IsPictureKind(&testBool));
					if ((slot==0 || slot==6) && comp!=0) // picture or legacy picture
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef->IsSoundKind(&testBool));
					if (slot==1 || slot==7) // sound or legacy sound
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef->IsPictureWithMatteKind(&testBool));
					if (slot==9 || ((slot==0 || slot==6) && comp==0)) // picturewithmatte
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef->IsMatteKind(&testBool));
					if (slot==10) // matte
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef2->IsTimecodeKind(&testBool));
					if (slot==2 || slot==8) // timecode or legacy timecode
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef2->IsEdgecodeKind(&testBool));
					if (slot==3) // edgecode
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef3->IsAuxiliaryKind(&testBool));
					if (slot==5) // auxiliary
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					checkResult(pDataDef3->IsDescriptiveMetadataKind(&testBool));
					if (slot==4) // descriptive metadata
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
					else
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

					if (slot==0 || slot==6)
					{
						// First component of picture slot has a picture with matte component
						// so we can test that picture with matte converts _to_ picture here
						if(comp == 0)
						{
							checkResult(pDataDef->IsDataDefOf(defs.ddkAAFPictureWithMatte(), &testBool));
							checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
							checkResult(pDataDef->IsPictureKind(&testBool));
							checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
							checkResult(pDataDef->IsPictureWithMatteKind(&testBool));
							checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						}
						else
						{
							checkResult(pDataDef->IsPictureWithMatteKind(&testBool));
							checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
							checkResult(pDataDef->DoesDataDefConvertFrom (defs.ddkAAFPictureWithMatte(), &testBool));
							checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						}

						checkResult(pDataDef->DoesDataDefConvertTo (defs.ddkAAFPicture(),
																	&testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->DoesDataDefConvertTo (defs.ddPicture(),
																	&testBool));
						checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->DoesDataDefConvertTo (defs.ddkAAFSound(), &testBool));
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
						checkResult(pDataDef->DoesDataDefConvertFrom (defs.ddkAAFSound(), &testBool));
						checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);
					}

					pComp->Release();
					pComp = NULL;
					pDataDef3->Release();
					pDataDef3 = NULL;
					pDataDef2->Release();
					pDataDef2 = NULL;
					pDataDef->Release();
					pDataDef = NULL;
					comp++;
				}
				
				pCompIter->Release();
				pCompIter = NULL;

				pSequence->Release();
				pSequence = NULL;
				
				pSegment->Release();
				pSegment = NULL;
				
				pSlot->Release();
				pSlot = NULL;
				slot++;
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
	
	if (pCompIter)
		pCompIter->Release();
	
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

extern "C" HRESULT CAAFDataDef_test(
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
		cerr << "CAAFDataDef_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following AAFDataDef methods have not been implemented:" << endl; 
//		cout << "     IsDataDefOf - needs unit test" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}





















		   






