//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFDescriptiveMarkerTest.cpp,v 1.7 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include <iostream>

#include <string.h>

using namespace std;

const aafUID_t TestDesciptiveFrameworkClassID = 
{ 0x023a1cae, 0xdc16, 0x4db9, { 0x95, 0xf9, 0x43, 0xd0, 0x56, 0xca, 0xd3, 0x78 } };

aafUInt32 TestDescribedSlotIDsVector[] = { 1, 3, 5, 7, 11 };
const aafUInt32 TestDescribedIDsVectorSize = 5;

// Verify that values in pEnumSetValues match those in the IDsVector.

aafUInt32 GetNextUInt32( IAAFSmartPointer<IEnumAAFPropertyValues> pEnumSetValues )
{
  using namespace mtc;

  IAAFSmartPointer<IAAFPropertyValue> pPropVal;
  CheckResult( pEnumSetValues->NextOne( &pPropVal ) );

  IAAFSmartPointer<IAAFTypeDef> pElemType;
  CheckResult( pPropVal->GetType( &pElemType ) );

  IAAFSmartPointer<IAAFTypeDefInt> pIntTypeDef;
  CheckResult( pElemType->QueryInterface( IID_IAAFTypeDefInt, (void**)&pIntTypeDef ) );

  aafUInt32 val;
  CheckResult( pIntTypeDef->GetInteger( pPropVal, (aafMemPtr_t)&val, sizeof(val) ) );

  return val;
}

void VerifySetContents( IAAFSmartPointer<IEnumAAFPropertyValues> pEnumSetValues )
{
  using namespace mtc;

  aafUInt32 setVector[TestDescribedIDsVectorSize];
  bool isMarked[TestDescribedIDsVectorSize];

  // Load contents of set into array.
  aafUInt32 i;
  for ( i = 0; i < TestDescribedIDsVectorSize; i++ ) {
    setVector[i] = GetNextUInt32( pEnumSetValues );
  }

  // Compare set against expected vector.  Ordering doesn't matter.
  for ( i = 0; i < TestDescribedIDsVectorSize; i++ ) {
    isMarked[i] = false;
  }  

  // For each value in TestDescribedSlotIDsVecto
  // ... search for first occurence in set that is not marked.
  // ... and mark found entires in set using isFound

  for ( i = 0; i < TestDescribedIDsVectorSize; i++ ) {

	aafUInt32 j;
    for ( j = 0; j < TestDescribedIDsVectorSize; j++ ) {
      if ( !isMarked[j] &&  setVector[j] == TestDescribedSlotIDsVector[i] ) {
		isMarked[j] = true;
		break;
      }
    }

    // We should find one unmarked ID, and break out of the loop,
    // each time through the inner loop.
    CheckExpression( j < TestDescribedIDsVectorSize, AAFRESULT_TEST_FAILED );

  } 
}	
	


static void RegisterDescriptiveTestFramework( IAAFSmartPointer<IAAFDictionary>& pDict )
{
  using namespace mtc;

  // Create a new class definition for the TestDescriptiveFramework.

  IAAFSmartPointer<IAAFClassDef> pClassDef;
  CheckResult( pDict->CreateMetaInstance( AUID_AAFClassDef, IID_IAAFClassDef,
					  (IUnknown**)&pClassDef ) );

  IAAFSmartPointer<IAAFClassDef> pBaseClassDef;
  CheckResult( pDict->LookupClassDef( AUID_AAFDescriptiveFramework, &pBaseClassDef ) );

  CheckResult( pClassDef->Initialize( TestDesciptiveFrameworkClassID,
				      pBaseClassDef,
				      L"TestDescriptiveFramework",
				      kAAFTrue ) );

  CheckResult( pDict->RegisterClassDef( pClassDef ) );
}


static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  mtc::SimpleFilePointers filePointers;

  try {
    using namespace mtc;

    IAAFSmartPointer<IAAFHeader> pHeader;
    IAAFSmartPointer<IAAFDictionary> pDict;
    CreateSimpleAAFFile( pFileName, 
			 fileKind,
			 rawStorageType,
			 productID,
			 &filePointers );

    // Add add a timeline, and add a DescriptiveMarker to the
    // composition mob.
    IAAFSmartPointer<IAAFDescriptiveMarker> pDescMarker;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFDescriptiveMarker,
							   IID_IAAFDescriptiveMarker,
							   (IUnknown**)&pDescMarker ));

    CheckResult( pDescMarker->Initialize() );

    IAAFSmartPointer<IAAFSegment> pSeg;
    CheckResult(pDescMarker->QueryInterface( IID_IAAFSegment, (void**)&pSeg ));

    IAAFSmartPointer<IAAFComponent> pComp;
    CheckResult( pDescMarker->QueryInterface( IID_IAAFComponent, (void**)&pComp ));
    CheckResult( pComp->SetDataDef( filePointers.pDataDef ));
	
    IAAFSmartPointer<IAAFTimelineMobSlot> pNewSlot;
    CheckResult( filePointers.pCompositionMob->AppendNewTimelineSlot(TEST_EditRate,
								     pSeg,
								     2,
								     L"Descriptive Content",
								     0,
								     &pNewSlot ));

    // Attach a (concrete) descriptive framework object to the marker.

    RegisterDescriptiveTestFramework( filePointers.pDictionary );

    IAAFSmartPointer<IAAFDescriptiveFramework> pDescFramework;
    CheckResult( filePointers.pDictionary->CreateInstance( TestDesciptiveFrameworkClassID,
							   IID_IAAFDescriptiveFramework,
							   (IUnknown**)&pDescFramework ) );
    CheckResult( pDescMarker->SetDescriptiveFramework( pDescFramework ) );

    // Get described slots - should not be present.
    aafUInt32 size = 0;
    HRESULT hr = pDescMarker->GetDescribedSlotIDsSize( &size );
    CheckExpression( AAFRESULT_PROP_NOT_PRESENT == hr, AAFRESULT_TEST_FAILED );

    // Set/Get single described slot.
    aafUInt32 setSingleSlotID = 0xdeadbeef;
    CheckResult( pDescMarker->SetDescribedSlotIDs( 1, &setSingleSlotID ) );
    aafUInt32 getSingleSlotID = 0;
    CheckResult( pDescMarker->GetDescribedSlotIDs( 1, &getSingleSlotID ) );
    CheckExpression( setSingleSlotID == getSingleSlotID, AAFRESULT_TEST_FAILED );

    // Set the persistent described slots.
    CheckResult( pDescMarker->SetDescribedSlotIDs( TestDescribedIDsVectorSize, TestDescribedSlotIDsVector ) );

    CheckResult( filePointers.pFile->Save() );
    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    if(filePointers.pFile) {
      filePointers.pFile->Close();
    }
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  mtc::SimpleFilePointers filePointers;

  try {
    using namespace mtc;

    ReadSimpleAAFFile( pFileName, &filePointers );

    // Get slot 2 from the composition and verify that that attached
    // segment is a DescriptiveMarker.
    
    IAAFSmartPointer<IAAFMobSlot> pSlot = GetSlotById( filePointers.pCompositionMob, 2 );

    IAAFSmartPointer<IAAFSegment> pSeg;
    CheckResult( pSlot->GetSegment( &pSeg ) );

    IAAFSmartPointer<IAAFDescriptiveMarker> pDescMarker;
    CheckResult( pSeg->QueryInterface( IID_IAAFDescriptiveMarker, (void**)&pDescMarker ) );
    
    IAAFSmartPointer<IAAFDescriptiveFramework> pDescFramework;
    CheckResult( pDescMarker->GetDescriptiveFramework( &pDescFramework ) );

    IAAFSmartPointer<IAAFObject> pDescFrameworkAsObj;
    CheckResult( pDescFramework->QueryInterface( IID_IAAFObject, (void**)&pDescFrameworkAsObj ) );
    
    IAAFSmartPointer<IAAFClassDef> pClassDef;
    CheckResult( pDescFrameworkAsObj->GetDefinition( &pClassDef ) );

    IAAFSmartPointer<IAAFMetaDefinition> pMetaDef;
    CheckResult( pClassDef->QueryInterface( IID_IAAFMetaDefinition, (void**)&pMetaDef ) );

    aafUID_t auid;

    CheckResult( pMetaDef->GetAUID( &auid ) );

    CheckExpression( memcmp( &auid, &TestDesciptiveFrameworkClassID, sizeof(auid) ) == 0,
		     AAFRESULT_TEST_FAILED );

    // Get, and test, the described slots from the marker.
    aafUInt32 getSlotIDsVector[TestDescribedIDsVectorSize];
    aafUInt32 getSlotIDsVectorSize = 0;

    CheckResult( pDescMarker->GetDescribedSlotIDsSize( &getSlotIDsVectorSize ) );
    CheckExpression( TestDescribedIDsVectorSize == getSlotIDsVectorSize, AAFRESULT_TEST_FAILED );

    CheckExpression( AAFRESULT_SMALLBUF ==
		       pDescMarker->GetDescribedSlotIDs( getSlotIDsVectorSize-1, getSlotIDsVector ),
		     AAFRESULT_TEST_FAILED );

    CheckResult( pDescMarker->GetDescribedSlotIDs( getSlotIDsVectorSize, getSlotIDsVector ) );

    CheckExpression( 0 == memcmp( getSlotIDsVector, TestDescribedSlotIDsVector, sizeof(TestDescribedSlotIDsVector) ),
		     AAFRESULT_TEST_FAILED );

    // Check that the described slots set can be accessed using a
    // property value enumerator.
    IAAFSmartPointer<IAAFObject> pDescMarkerObj;
    CheckResult( pDescMarker->QueryInterface( IID_IAAFObject, (void**)&pDescMarkerObj ) );

    CheckResult( pDescMarkerObj->GetDefinition( &pClassDef ) );

    IAAFSmartPointer<IAAFPropertyDef> pPropDef;
    CheckResult( pClassDef->LookupPropertyDef( kAAFPropID_DescriptiveMarker_DescribedSlots,
					       &pPropDef ) );

    IAAFSmartPointer<IAAFPropertyValue> pPropVal;
    CheckResult( pDescMarkerObj->GetPropertyValue( pPropDef, &pPropVal ) );

    IAAFSmartPointer<IAAFTypeDef> pTypeDef;
    CheckResult( pPropDef->GetTypeDef( &pTypeDef ) );

    IAAFSmartPointer<IAAFTypeDefSet> pTypeDefSet;
    CheckResult( pTypeDef->QueryInterface( IID_IAAFTypeDefSet, (void**)&pTypeDefSet ) );

    IAAFSmartPointer<IEnumAAFPropertyValues> pEnumSetValues;
    CheckResult( pTypeDefSet->GetElements( pPropVal, &pEnumSetValues ) );
    
    VerifySetContents( pEnumSetValues );
	
	
    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    if(filePointers.pFile) {
      filePointers.pFile->Close();
    }
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}


extern "C" HRESULT CAAFDescriptiveMarker_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFDescriptiveMarker_test(
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
		cerr << "CAAFDescriptiveMarker_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
