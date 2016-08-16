//=---------------------------------------------------------------------=
//
// $Id: CAAFControlPointTest.cpp,v 1.38 2009/06/01 11:47:11 stuart_hc Exp $ $Name: V116 $
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
#include "AAFInterpolatorDefs.h"
#include "AAFTypeDefUIDs.h"

#include "CAAFBuiltinDefs.h"

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFControlPoint; // generated

static aafMobID_t	zeroMobID = {{0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,
					{0,0,0,{0,0,0,0,0,0,0,0}}};
static const aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

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
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"
#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafRational_t kTestLevel1 = { 1, 1 };
const aafRational_t kTestLevel2 = { 0, 1 };
const aafRational_t kTestTime1 = { 0, 1 };
const aafRational_t kTestTime2 = { 1, 1 };

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFOperationDef*		pOperationDef = NULL;
	IAAFParameterDef*	pParamDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
	IAAFOperationGroup			*pOperationGroup = NULL;
	IAAFMob				*pMob = NULL;
	IAAFSegment			*pSeg = NULL;
	IAAFTimelineMobSlot	*pSlot = NULL;
	IAAFParameter		*pParm = NULL;
	IAAFVaryingValue		*pVaryingValue = NULL;
	IAAFSegment			*pFiller = NULL;
	IAAFComponent		*pComponent = NULL;
	IAAFSourceClip		 *pSourceClip = NULL;
	IAAFControlPoint	 *pControlPoint = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	IAAFInterpolationDef *pInterpDef = NULL;
	IAAFPluginManager	*pMgr = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
//	aafUID_t			testInterpDef = kAAFTypeID_Rational;
	aafLength_t			effectLen = TEST_EFFECT_LEN;
	aafUID_t			effectID = kTestEffectID;
	aafUID_t			parmID = kTestParmID;
	aafRational_t		testLevel1 = kTestLevel1;
	aafRational_t		testLevel2 = kTestLevel2;
	aafRational_t		testTime1 = kTestTime1;
	aafRational_t		testTime2 = kTestTime2;
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

		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));
    
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

 		checkResult(pDictionary->LookupTypeDef (kAAFTypeID_Rational, &pTypeDef));
    checkResult(pParamDef->Initialize (parmID, TEST_PARAM_NAME, TEST_PARAM_DESC, pTypeDef));

		checkResult(AAFGetPluginManager(&pMgr));
		checkResult(pMgr->CreatePluginDefinition(LinearInterpolator, pDictionary, &pDefObject));
		checkResult(pDefObject->QueryInterface(IID_IAAFInterpolationDef, (void **) &pInterpDef));
		pDefObject->Release();
		pDefObject = NULL;

		checkResult(pOperationDef->Initialize (effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef (pOperationDef));
		checkResult(pDictionary->RegisterParameterDef (pParamDef));
		checkResult(pDictionary->RegisterInterpolationDef (pInterpDef));


		checkResult(pOperationDef->SetDataDef (defs.ddkAAFPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));

		//Make the first mob
		long	test;
		aafRational_t	videoRate = { 2997, 100 };
		// Create a Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));

		checkResult(pMob->SetName(L"AAFOperationGroupTest"));
	  
		// Add some slots
		for(test = 0; test < 2; test++)
		{
 			checkResult(defs.cdOperationGroup()->
						CreateInstance(IID_IAAFOperationGroup, 
									   (IUnknown **)&pOperationGroup));
			
			checkResult(defs.cdFiller()->
						CreateInstance(IID_IAAFSegment, 
									   (IUnknown **)&pFiller));
			checkResult(pFiller->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
			checkResult(pComponent->SetLength(effectLen));
			CAAFBuiltinDefs defs(pDictionary);
			checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
 			checkResult(pOperationGroup->Initialize(defs.ddkAAFPicture(), TEST_EFFECT_LEN, pOperationDef));

			checkResult(defs.cdVaryingValue()->
						CreateInstance(IID_IAAFVaryingValue, 
									   (IUnknown **)&pVaryingValue));
			checkResult(pVaryingValue->Initialize (pParamDef, pInterpDef));

			checkResult(defs.cdControlPoint()->
						CreateInstance(IID_IAAFControlPoint, 
									   (IUnknown **)&pControlPoint));
			checkResult(pControlPoint->Initialize (pVaryingValue, testTime1, sizeof(testLevel1), (aafDataBuffer_t)&testLevel1));
			checkResult(pControlPoint->SetEditHint(kAAFRelativeLeft));
			checkResult(pVaryingValue->AddControlPoint(pControlPoint));
			pControlPoint->Release();
			pControlPoint = NULL;
			checkResult(defs.cdControlPoint()->
						CreateInstance(IID_IAAFControlPoint, 
									   (IUnknown **)&pControlPoint));
			checkResult(pControlPoint->Initialize (pVaryingValue, testTime2, sizeof(testLevel2), (aafDataBuffer_t)&testLevel2));
			checkResult(pControlPoint->SetEditHint(kAAFProportional));
			checkResult(pVaryingValue->AddControlPoint(pControlPoint));
			pControlPoint->Release();
			pControlPoint = NULL;

			checkResult(pVaryingValue->QueryInterface (IID_IAAFParameter, (void **)&pParm));
			checkResult(pOperationGroup->AddParameter (pParm));

			checkResult(pOperationGroup->AppendInputSegment (pFiller));
			pFiller->Release();
			pFiller = NULL;

			checkResult(pOperationGroup->SetBypassOverride (1));
			checkResult(defs.cdSourceClip()->
						CreateInstance(IID_IAAFSourceClip, 
									   (IUnknown **)&pSourceClip));
			aafSourceRef_t	sourceRef;
			sourceRef.sourceID = zeroMobID;
			sourceRef.sourceSlotID = 0;
			sourceRef.startTime = 0;
			checkResult(pSourceClip->Initialize (defs.ddkAAFPicture(), effectLen, sourceRef));
			checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
			checkResult(pOperationGroup->SetRender (pSourceRef));
			checkResult(pOperationGroup->QueryInterface (IID_IAAFSegment, (void **)&pSeg));

			checkResult(pMob->AppendNewTimelineSlot (videoRate, pSeg, test+1, slotNames[test], 0, &pSlot));

			pSlot->Release();
			pSlot = NULL;

			pSeg->Release();
			pSeg = NULL;

			pOperationGroup->Release();
			pOperationGroup = NULL;
			pParm->Release();
			pParm = NULL;
			pVaryingValue->Release();
			pVaryingValue = NULL;
			pComponent->Release();
			pComponent = NULL;
			pSourceRef->Release();
			pSourceRef = NULL;
			pSourceClip->Release();
			pSourceClip = NULL;
		}

		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


	// Cleanup and return
	if(pSourceRef)
		pSourceRef->Release();
	if(pControlPoint)
		pControlPoint->Release();
	if(pSourceClip)
		pSourceClip->Release();
	if (pDefObject)
		pDefObject->Release();
	if (pOperationGroup)
		pOperationGroup->Release();
	if (pMob)
		pMob->Release();
	if (pSeg)
		pSeg->Release();
	if (pSlot)
		pSlot->Release();
	if (pComponent)
		pComponent->Release();

	if (pParm)
		pParm->Release();
//	if (pIntDef)
//		pIntDef->Release();
	if (pInterpDef)
		pInterpDef->Release();
	if (pVaryingValue)
		pVaryingValue->Release();
	if (pFiller)
		pFiller->Release();

	if (pOperationDef)
		pOperationDef->Release();

	if (pParamDef)
		pParamDef->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();
	if(pMgr)
		pMgr->Release();
	if(pTypeDef)
		pTypeDef->Release();
      
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
	IEnumAAFOperationDefs *pOperationGroupEnum = NULL;
	IEnumAAFParameterDefs *pParmDefEnum = NULL;
	IAAFOperationDef		*pOperationDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFParameter		*pParameter = NULL;
	IAAFMetaDefinition*		pMetaDefinition = NULL;
	IAAFSegment*		pSeg = NULL;
	IAAFOperationGroup*			pOperationGroup = NULL;
	IEnumAAFMobs		*mobIter = NULL;
	IAAFMob*			pMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFFiller*			pFill = NULL;
	IAAFSourceReference *pSourceRef = NULL;
	IEnumAAFControlPoints *pEnumCP = NULL;
	IAAFControlPoint	*pControlPoint = NULL;
	IAAFVaryingValue	*pVaryingValue = NULL;
	IAAFInterpolationDef	*pInterpDef = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	bool				bFileOpen = false;
	aafBool				readIsTimeWarp;
	aafUInt32			testNumSources, testNumParam;
	HRESULT				hr = S_OK;
	aafNumSlots_t		s;
	aafNumSlots_t	numSlots;
	aafUInt32			readOverride;
	aafBool				readValidTransition;
	aafRational_t		testTime;
	aafRational_t	sampleValue1 = kTestLevel1, sampleValue2 = kTestLevel2, testValue;
	aafRational_t		checkTime1 = kTestTime1;
	aafRational_t		checkTime2 = kTestTime2;
	aafEditHint_t		checkEditHint;
	aafUID_t			testInterpDef, checkInterpDef = kAAFTypeID_Rational;

	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		aafSearchCrit_t		criteria;
		criteria.searchTag = kAAFNoSearch;
		checkResult(pHeader->GetMobs (&criteria, &mobIter));
				
		checkResult(mobIter->NextOne (&pMob));			
		checkResult(pMob->GetSlots(&slotIter));
		checkResult(pMob->CountSlots (&numSlots));
		
		for(s = 0; s < numSlots; s++)
		{
			checkResult(slotIter->NextOne (&pSlot));
			checkResult(pSlot->GetSegment (&pSeg));
			checkResult(pSeg->QueryInterface (IID_IAAFOperationGroup, (void **)&pOperationGroup));
			pSeg->Release();
			pSeg = NULL;

			checkResult(pOperationGroup->CountSourceSegments(&testNumSources));
			checkExpression(testNumSources == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
			checkResult(pOperationGroup->CountParameters(&testNumParam));
			checkExpression(testNumSources == 1, AAFRESULT_TEST_FAILED);

			checkResult(pOperationGroup->IsATimeWarp (&readIsTimeWarp));
			checkExpression(readIsTimeWarp == kAAFFalse, AAFRESULT_TEST_FAILED);

			checkResult(pOperationGroup->GetBypassOverride (&readOverride));
			checkExpression(readOverride == 1, AAFRESULT_TEST_FAILED);

			checkResult(pOperationGroup->IsValidTranOperation (&readValidTransition));
			checkExpression(readValidTransition == kAAFFalse, AAFRESULT_TEST_FAILED);
			/**/
			checkResult(pOperationGroup->GetInputSegmentAt (0, &pSeg));
 			checkResult(pSeg->QueryInterface(IID_IAAFFiller, (void **) &pFill));
			pFill->Release();
			pFill = NULL;
			/**/
			checkResult(pOperationGroup->LookupParameter (kTestParmID, &pParameter));
 			checkResult(pParameter->QueryInterface(IID_IAAFVaryingValue, (void **) &pVaryingValue));

			/*** Check the VaryingValue methods **/
			aafUInt32		testLen, bytesRead;

			checkResult(pVaryingValue->GetControlPoints(&pEnumCP));
			checkResult(pEnumCP->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
 			checkExpression(testLen == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue1), (aafDataBuffer_t)&testValue, &bytesRead));
 			checkExpression(testValue.numerator == sampleValue1.numerator, AAFRESULT_TEST_FAILED);
 			checkExpression(testValue.denominator == sampleValue1.denominator, AAFRESULT_TEST_FAILED);
 			checkExpression(bytesRead == sizeof(sampleValue1), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetTime(&testTime));
 			checkExpression(testTime.numerator == checkTime1.numerator, AAFRESULT_TEST_FAILED);
 			checkExpression(testTime.denominator == checkTime1.denominator, AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetEditHint(&checkEditHint));
  			checkExpression(checkEditHint == kAAFRelativeLeft, AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetTypeDefinition (&pTypeDef));
 			checkResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition, (void **) &pMetaDefinition));
			checkResult(pMetaDefinition->GetAUID(&testInterpDef));
  			checkExpression(memcmp(&testInterpDef, &checkInterpDef, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
			pControlPoint->Release();
			pControlPoint = NULL;
			pTypeDef->Release();
			pTypeDef = NULL;
			pMetaDefinition->Release();
			pMetaDefinition = NULL;
			/**/
			checkResult(pEnumCP->NextOne(&pControlPoint));
			checkResult(pControlPoint->GetValueBufLen (&testLen));
 			checkExpression(testLen == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetValue (sizeof(sampleValue2), (aafDataBuffer_t)&testValue, &bytesRead));
 			checkExpression(testValue.numerator == sampleValue2.numerator, AAFRESULT_TEST_FAILED);
 			checkExpression(testValue.denominator == sampleValue2.denominator, AAFRESULT_TEST_FAILED);
 			checkExpression(bytesRead == sizeof(sampleValue2), AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetTime(&testTime));
 			checkExpression(testTime.numerator == checkTime2.numerator, AAFRESULT_TEST_FAILED);
 			checkExpression(testTime.denominator == checkTime2.denominator, AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetEditHint(&checkEditHint));
  			checkExpression(checkEditHint == kAAFProportional, AAFRESULT_TEST_FAILED);
			checkResult(pControlPoint->GetTypeDefinition (&pTypeDef));
 			checkResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition, (void **) &pMetaDefinition));
			checkResult(pMetaDefinition->GetAUID(&testInterpDef));
  			checkExpression(memcmp(&testInterpDef, &checkInterpDef, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);

			pControlPoint->Release();
			pControlPoint = NULL;
			pEnumCP->Release();
			pEnumCP = NULL;
			pTypeDef->Release();
			pTypeDef = NULL;
			pMetaDefinition->Release();
			pMetaDefinition = NULL;

			/*****/

			pVaryingValue->Release();
			pVaryingValue = NULL;
			pParameter->Release();
			pParameter = NULL;
			pSeg->Release();
			pSeg = NULL;
			pOperationGroup->Release();
			pOperationGroup = NULL;
			pSlot->Release();
			pSlot = NULL;
		}
		
		slotIter->Release();
		slotIter = NULL;
		pMob->Release();
		pMob = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pFill)
		pFill->Release();
      
	if(pInterpDef)
		pInterpDef->Release();
	if(pVaryingValue)
		pVaryingValue->Release();
	if (pSourceRef)
		pSourceRef->Release();
      
	if (mobIter)
		mobIter->Release();
      
	if (pSlot)
		pSlot->Release();
      
	if (slotIter)
		slotIter->Release();
      
	if (pMob)
		pMob->Release();
      
	if (pHeader)
		pHeader->Release();
      
	if (pSeg)
		pSeg->Release();
      
	if (pControlPoint)
		pControlPoint->Release();
      
	if(pEnumCP)
		pEnumCP->Release();
	
	if (pOperationGroup)
		pOperationGroup->Release();
      
	if (pDictionary)
		pDictionary->Release();
      
	if (pOperationGroupEnum)
		pOperationGroupEnum->Release();

	if (pParameter)
		pParameter->Release();

	if (pParmDefEnum)
		pParmDefEnum->Release();
      
	if (pOperationDef)
		pOperationDef->Release();
      
	if (pMetaDefinition)
		pMetaDefinition->Release();

	if (pParmDef)
		pParmDef->Release();

	if (pTypeDef)
		pTypeDef->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CAAFControlPoint_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFControlPoint_test(
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
		cerr << "CAAFControlPoint_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
