
//=---------------------------------------------------------------------=
//
// $Id: CAAFFillerTest.cpp,v 1.34 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafUID_t    fillerUID = kAAFDataDef_Timecode;
static aafLength_t  fillerLength = 3200;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x1d760b6a, 0x03ff, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

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
    aafProductIdentification_constref /* productID */)
{
  IAAFFile *          pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *        pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
  IAAFCompositionMob*      pCompMob = NULL;
  IAAFMob*          pMob = NULL;
  IAAFFiller*          pFiller = NULL;
  IAAFTimelineMobSlot*        pSlot = NULL;
  IAAFSegment*        pSegment = NULL;
  HRESULT            hr = AAFRESULT_SUCCESS;

  try 
	{
	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);

	  // Create the file.
	  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, &pFile ));

	  bFileOpen = true;
  
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
	  CAAFBuiltinDefs defs (pDictionary);
     
	  // Create a Composition mob - it should work !!
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFCompositionMob, 
								 (IUnknown **)&pCompMob));
	  // get a IAAFMob interface
	  checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	  // Initialize the CompMob
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"AAFFillerTest"));

	  // Create a AAFFiller - since it is the first time we will check
	  // the error code
	  checkResult(defs.cdFiller()->
				  CreateInstance(IID_IAAFFiller, 
								 (IUnknown **)&pFiller));
	  // Get a IAAFSegment interface for it
	  checkResult(pFiller->QueryInterface (IID_IAAFSegment,
										   (void **)&pSegment));
	  // Set filler properties
	  checkResult(pFiller->Initialize(defs.ddkAAFTimecode(), fillerLength));
	  // append the filler to the MOB tree
	  aafRational_t editRate = { 0, 1};
	  checkResult(pMob->AppendNewTimelineSlot(editRate,
											  pSegment,
											  1,
											  L"FillerSlot",
											  0,
											  &pSlot)); 

	  // Add the Mob to the file
	  checkResult(pHeader->AddMob(pMob));
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}


  // Cleanup and return
  if (pSlot)
    pSlot->Release();

  if (pSegment)
    pSegment->Release();

  if (pFiller)
    pFiller->Release();

  if (pMob)
    pMob->Release();

  if (pCompMob)
    pCompMob->Release();

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
  IAAFFile *          pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *        pHeader = NULL;

  //  IAAFSourceMob*        pSourceMob = NULL;
  IAAFMob*          pMob = NULL;
  IAAFFiller*          pFiller = NULL;
  IAAFMobSlot*        pSlot = NULL;
  IAAFSegment*        pSegment = NULL;
  IAAFComponent*        pComponent = NULL;
  IEnumAAFMobs*        pMobIter = NULL;
  IEnumAAFMobSlots*      pSlotIter = NULL;
  IAAFDataDef * pDataDef = 0;
  IAAFDefObject * pDefObj = 0;
  
  aafNumSlots_t        numMobs, numSlots;
  aafSearchCrit_t        criteria;
  aafUID_t          readFillerUID;
  aafLength_t          readFillerLength;
  HRESULT            hr = AAFRESULT_SUCCESS;


  try 
	{
	  // Open the file
	  checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

	  // Enumerate over all Composition Mobs
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFCompMob;
	  checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	  while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
		{
		  checkResult(pMob->CountSlots(&numSlots));
		  checkExpression (1 == numSlots, AAFRESULT_TEST_FAILED);

		  checkResult(pMob->GetSlots(&pSlotIter));
		  while (pSlotIter && (pSlotIter->NextOne(&pSlot) ==
							   AAFRESULT_SUCCESS))
			{
			  checkResult(pSlot->GetSegment(&pSegment));
			  checkResult(pSegment->QueryInterface(IID_IAAFFiller,
												   (void **) &pFiller));
			  // Make sure the segment is a AAFFiller !!

			  // Get The component interface
			  checkResult(pSegment->QueryInterface(IID_IAAFComponent,
												   (void **) &pComponent));
			  // retrieve properties
			  checkResult(pComponent->GetLength( &readFillerLength));
			  checkResult(pComponent->GetDataDef( &pDataDef));
			  checkResult(pDataDef->QueryInterface(IID_IAAFDefObject,
												   (void **) &pDefObj));
			  pDataDef->Release ();
			  pDataDef = 0;
			  checkResult(pDefObj->GetAUID( &readFillerUID));
			  pDefObj->Release ();
			  pDefObj = 0;
        
			  // Compare results
			  checkExpression((readFillerLength == fillerLength) &&
							  (memcmp(&readFillerUID,
									  &fillerUID,
									  sizeof(fillerUID)) == 0),
							  AAFRESULT_TEST_FAILED);

			  pComponent->Release();
			  pComponent = NULL;

			  pFiller->Release();
			  pFiller = NULL;

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

  // Cleanup and return

  if (pDataDef)
	{
	  pDataDef->Release ();
	  pDataDef = 0;
	}

  if (pDefObj)
	{
	  pDefObj->Release ();
	  pDefObj = 0;
	}

  if (pComponent)
	{
	  pComponent->Release();
	  pComponent = 0;
	}

  if (pSegment)
	{
	  pSegment->Release();
	  pSegment = 0;
	}

  if (pSlot)
	{
	  pSlot->Release();
	  pSlot = 0;
	}

  if (pFiller)
	{
	  pFiller->Release();
	  pFiller = 0;
	}

  if (pSlotIter)
	{
	  pSlotIter->Release();
	  pSlotIter = 0;
	}

  if (pMob)
	{
	  pMob->Release();
	  pMob = 0;
	}

  if (pMobIter)
	{
	  pMobIter->Release();
	  pMobIter = 0;
	}

  if (pHeader)
	{
	  pHeader->Release();
	  pHeader = 0;
	}
      
  if (pFile)
	{
	  if (bFileOpen)
		pFile->Close();
	  pFile->Release();
	  pFile = 0;
	}

  return hr;
}

extern "C" HRESULT CAAFFiller_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFFiller_test(
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
    if (hr == AAFRESULT_SUCCESS)
      hr = ReadAAFFile( pFileName );
  }
  catch (...)
  {
    cerr << "CAAFFiller_test...Caught general C++"
		 << " exception!" << endl; 
	hr = AAFRESULT_TEST_FAILED;
  }

  // Cleanup our object if it exists.
  return hr;
}
