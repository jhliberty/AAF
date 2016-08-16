//=---------------------------------------------------------------------=
//
// $Id: CAAFCompositionMobTest.cpp,v 1.38 2009/06/01 11:47:11 stuart_hc Exp $ $Name: V116 $
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
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


static aafInt32 fadeInLen  = 1000;
static aafFadeType_t fadeInType = kAAFFadeLinearAmp;
static aafRational_t fadeInEditUnit = { 25, 1};

static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x9269dfa2, 0x0407, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const	aafMobID_t	TEST_rendering_mobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x9269dfa3, 0x0407, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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
	IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;

	IAAFCompositionMob*			pCompMob = NULL;
	IAAFCompositionMob2*			pCompMob2 = NULL;
	IAAFMob*					pMob = NULL;

	HRESULT						hr = S_OK;

  try
  {
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);

    
		// Create the new AAF file.
	  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

	  CAAFBuiltinDefs defs (pDictionary);
	  // Create a CompositionMob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFCompositionMob, 
								 (IUnknown **)&pCompMob));
	  // Get a MOB Interface 
	  checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		
	  // Assign the mob a new id.
	  checkResult(pMob->SetMobID(TEST_MobID));
    
	  // Initialize the composition mob.
	  checkResult(pCompMob->Initialize( L"COMPMOB01" ));
	  checkResult(pCompMob->SetDefaultFade(fadeInLen, fadeInType, fadeInEditUnit));

		// Check the CompositionMob2 Rendering implementation
		checkResult( pCompMob->QueryInterface( IID_IAAFCompositionMob2, reinterpret_cast<void**>(&pCompMob2) ) );
		checkResult( pCompMob2->SetRendering( TEST_rendering_mobID ) );

	  // Add the mob to the file.
	  checkResult(pHeader->AddMob(pMob));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pCompMob2)
		pCompMob2->Release();

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
  IAAFFile *					pFile = NULL;
  IAAFHeader *				pHeader = NULL;
 
  IEnumAAFMobs*				pMobIter = NULL;
  IAAFMob*					pMob = NULL;
  IAAFCompositionMob*			pCompMob = NULL;
  IAAFCompositionMob2*			pCompMob2 = NULL;

  aafSearchCrit_t				criteria;
  aafDefaultFade_t			defaultFade;
  aafNumSlots_t				numMobs;
	aafMobID_t	mobID;
  HRESULT						hr = S_OK;

  try
  {
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs( kAAFAllMob, &numMobs ));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
		{
			// Get A CompositionMob Interface 
			checkResult(pMob->QueryInterface(IID_IAAFCompositionMob,(void **)&pCompMob));
			checkResult(pCompMob->GetDefaultFade( &defaultFade ));
			
      checkExpression( (defaultFade.fadeLength == fadeInLen) &&
				  (defaultFade.fadeType == fadeInType) &&
				  (memcmp( &( defaultFade.fadeEditUnit), &fadeInEditUnit, sizeof( fadeInEditUnit ))== 0) &&
				  (defaultFade.valid == kAAFTrue), 
         AAFRESULT_TEST_FAILED);

			// Check the CompositionMob2 Rendering implementation
			checkResult( pCompMob->QueryInterface( IID_IAAFCompositionMob2, reinterpret_cast<void**>(&pCompMob2) ) );
			checkResult( pCompMob2->GetRendering( &mobID ) );

			checkExpression( memcmp( &mobID, &TEST_rendering_mobID, sizeof(aafMobID_t)) == 0, AAFRESULT_TEST_FAILED);

			pCompMob2->Release();
			pCompMob2 = NULL;

			pCompMob->Release();
			pCompMob = NULL;

			pMob->Release();
			pMob = NULL;
		}
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

	// Cleanup and return
	if (pMobIter)
		pMobIter->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pMob)
		pMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}



	return hr;
}

extern "C" HRESULT CAAFCompositionMob_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFCompositionMob_test(
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
	  cerr << "CAAFCompositionMob_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.
	return hr;
}


  //@access Public Members

  //@rdesc Error code [see below].


  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to omfiSourceClipGetFade() in all cases.
  //@comm Maps to omfiMobGetDefaultFade
	


  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade


