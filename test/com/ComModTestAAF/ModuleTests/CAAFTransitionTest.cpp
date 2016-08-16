//=---------------------------------------------------------------------=
//
// $Id: CAAFTransitionTest.cpp,v 1.51 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
#include <iomanip>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

// This values are used for testing purposes
static aafLength_t  fillerLength = 3200;
static aafMobID_t	zeroMobID = {{0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,{0,0,0,{0,0,0,0,0,0,0,0}}};


#ifndef _DEBUG
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

#else // #ifndef _DEBUG

// convenient error handlers.
#define checkResult(r)\
do {\
  if (FAILED(r))\
  {\
    cerr << "FILE:" << __FILE__ << " LINE:" << __LINE__ << " Error code = " << hex << r << dec << endl;\
    throw (HRESULT)r;\
  }\
} while (false)

#define checkExpression(expression, r)\
do {\
  if (!(expression))\
  {\
    cerr << "FILE:" << __FILE__ << " LINE:" << __LINE__ << " Expression failed = " << #expression << endl;\
    throw (HRESULT)r;\
  }\
} while (false)

#endif // #else // #ifndef _DEBUG


#define TEST_NUM_INPUTS		1
static const aafUID_t TEST_CATEGORY = 
{ 0x0d010102, 0x0101, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
#define TEST_BYPASS			1
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x9c671b5c, 0x0405, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile *					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob*					pMob = NULL;
	IAAFTimelineMobSlot*		pNewSlot = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFSourceReference*		pSourceRef = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFOperationGroup*			pOperationGroup = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSegment*				pEffectFiller = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFOperationDef*				pOperationDef = NULL;
	IAAFParameter				*pParm = NULL;
	IAAFParameterDef*			pParamDef = NULL;
	IAAFConstantValue*			pConstantValue = NULL;
	
	HRESULT						hr = S_OK;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint = 0;
	aafLength_t					effectLen = TEST_EFFECT_LEN;
	aafUID_t					effectID = kTestEffectID;
	aafUID_t					parmID = kTestParmID;

	transitionLength = 100;

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
 		
		// Create the effect and parameter definitions
		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));
    
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

		checkResult(pOperationDef->Initialize (effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pParamDef->Initialize (parmID, TEST_PARAM_NAME, TEST_PARAM_DESC, defs.tdRational ()));
		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));


		checkResult(pOperationDef->SetDataDef (defs.ddkAAFPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));


		// ------------------------------------------------------------
		//	To test a Transition we need to create a Sequence which will 
		//	a Filler, a transition and another Filler. I know this is not 
		//  very interesting, but it will let us test the Transition 
		//  interface with the least amount of other stuff.
		// ------------------------------------------------------------
		//
		// Create a CompositionMob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFCompositionMob, 
								   (IUnknown **)&pCompMob));

		checkResult(pCompMob->Initialize(L"Transition Test"));
		// Get a MOB interface
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));

		// Create a Sequence
		checkResult(defs.cdSequence()->
					CreateInstance(IID_IAAFSequence,
								   (IUnknown **) &pSequence));

		// Get a Segment interface
		checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
		// Get a component interface and 
		checkResult(pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		// set the Data definition for it !
		checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Create a new Mob Slot that will contain the sequence
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot(editRate,
												pSegment,
												1,
												L"Transition",
												0,
												&pNewSlot));

		// Create a Filler
		checkResult(defs.cdFiller()->
					CreateInstance(IID_IAAFFiller,
								   (IUnknown **) &pFiller));

		// Get a component interface
		checkResult(pFiller->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
		// Set values for the filler
	    checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));
		// append the filler to the sequence
		checkResult(pSequence->AppendComponent(pComponent));

		// Release the component - because we need to reuse the pointer later
		pFiller->Release();
		pFiller = NULL;
		pComponent->Release();
		pComponent = NULL;

		
	    checkResult(defs.cdTransition()->
					CreateInstance(IID_IAAFTransition, 
								   (IUnknown **)&pTransition));

		// Create an empty EffectGroup object !!
		checkResult(defs.cdOperationGroup()->
					CreateInstance(IID_IAAFOperationGroup,
								   (IUnknown **)&pOperationGroup));

		checkResult(pOperationGroup->Initialize(defs.ddkAAFPicture(), transitionLength, pOperationDef));

    // Create a constant value parameter.
		checkResult(defs.cdConstantValue()->
					CreateInstance(IID_IAAFConstantValue, 
								   (IUnknown **)&pConstantValue));
    aafRational_t testLevel = {1, 2};
		checkResult(pConstantValue->Initialize (pParamDef, sizeof(testLevel), (aafDataBuffer_t)&testLevel));

    checkResult(pConstantValue->QueryInterface (IID_IAAFParameter, (void **)&pParm));
    checkResult(pOperationGroup->AddParameter (pParm));
    pParm->Release();
    pParm = NULL;
    pConstantValue->Release();
    pConstantValue = NULL;

		checkResult(defs.cdFiller()->
					CreateInstance(IID_IAAFSegment,
								   (IUnknown **) &pEffectFiller));
		 checkResult(pEffectFiller->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		pComponent->Release();
		pComponent = NULL;
		checkResult(pOperationGroup->AppendInputSegment (pEffectFiller));
		// release the filler
		pEffectFiller->Release();
		pEffectFiller  = NULL;

		checkResult(pOperationGroup->SetBypassOverride (1));
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceClip, 
								   (IUnknown **)&pSourceClip));
		aafSourceRef_t	sourceRef;
		sourceRef.sourceID = zeroMobID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		checkResult(pSourceClip->Initialize (defs.ddkAAFPicture(),
											 effectLen,
											 sourceRef));
		checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference,
												 (void **)&pSourceRef));
		checkResult(pOperationGroup->SetRender (pSourceRef));

		checkResult(pTransition->Initialize (defs.ddkAAFPicture(),
											 transitionLength,
											 cutPoint,
											 pOperationGroup));
		checkResult(pTransition->QueryInterface (IID_IAAFComponent,
												 (void **)&pComponent));

		// now append the transition
		checkResult(pSequence->AppendComponent(pComponent));

		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Create the second filler 
		checkResult(defs.cdFiller()->
					CreateInstance(IID_IAAFFiller,
								   (IUnknown **) &pFiller));

		checkResult(pFiller->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
		// Set values for the filler
	    checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));
		// append the filler to the sequence
		checkResult(pSequence->AppendComponent(pComponent));
		pComponent->Release();
		pComponent = NULL;
		pFiller->Release();
		pFiller = NULL;

		// Now, we append the composition mob to the file	
		checkResult(pHeader->AddMob(pMob));
		// and we are done !
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	

	// Cleanup and return
	if (pParm)
		pParm->Release();

  if (pConstantValue)
		pConstantValue->Release();

	if (pParamDef)
		pParamDef->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pSourceRef)
		pSourceRef->Release();

	if (pNewSlot)
		pNewSlot->Release();

	if (pSegment)
		pSegment->Release();

	if (pSequence)
		pSequence->Release();

	if (pFiller)
		pFiller->Release();
	if (pOperationDef)
		pOperationDef->Release();


	if (pOperationGroup)
		pOperationGroup->Release();

	if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();


	if (pTransition)
		pTransition->Release();

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
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IEnumAAFMobSlots*			pEnum = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFOperationGroup*					pOperationGroup = NULL;
	IEnumAAFComponents*			pCompIter = NULL;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint;

	aafNumSlots_t				numMobs;
	aafUInt32					numComponents = 0;
	HRESULT						hr = S_OK;

	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->GetMobs( NULL, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->GetSlots (&pEnum));

			while (AAFRESULT_SUCCESS == pEnum->NextOne (&pMobSlot))
			{
				checkResult(pMobSlot->GetSegment (&pSegment));
				// Check to see if Segment is a Sequence
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
				// It is, so get a Component Iterator
				checkResult(pSequence->CountComponents(&numComponents));
				// Verify that all 3 components(Filler, Transition, Filler) are present
				checkExpression(numComponents == 3,  AAFRESULT_TEST_FAILED);
				checkResult(pSequence->GetComponents(&pCompIter));
				// Now visit each and every one of the components.
				while(AAFRESULT_SUCCESS == pCompIter->NextOne(&pComponent))
				{
					// Find out what kind of segment we have
					if ((pComponent->QueryInterface(IID_IAAFTransition, (void **)&pTransition)) == AAFRESULT_SUCCESS)
					{
						// This is the transition 
						checkResult(pTransition->GetCutPoint (&cutPoint));
						checkResult(pComponent->GetLength(&transitionLength));
						checkResult(pTransition->GetOperationGroup(&pOperationGroup));
						// Check results !!
						checkExpression(cutPoint == 0, AAFRESULT_TEST_FAILED);
						checkExpression(transitionLength == 100, AAFRESULT_TEST_FAILED);
						pTransition->Release();
						pTransition = NULL;
					}
					else
					{
						// validate that the other segments are Fillers
						checkResult(pComponent->QueryInterface(IID_IAAFFiller, (void **)&pFiller));
						pFiller->Release();
						pFiller = NULL;
					}
					pComponent->Release();
					pComponent = NULL;
				}
				pSegment->Release();
				pSegment = NULL;
				pSequence->Release();
				pSequence = NULL;
				pCompIter->Release();
				pCompIter = NULL;
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
	if (pTransition)
		pTransition->Release();

	if (pOperationGroup)
		pOperationGroup->Release();

	if (pComponent)
		pComponent->Release();

	if (pSegment)
		pSegment->Release();

	if (pMobSlot)
		pMobSlot->Release();

	if (pSequence)
		pSequence->Release();

	if (pCompIter)
		pCompIter->Release();

	if (pEnum)
		pEnum->Release();

	if (pFiller)
		pFiller->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pDictionary)
		pDictionary->Release();

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

extern "C" HRESULT CAAFTransition_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTransition_test(
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
	  cerr << "CAAFTransition_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
