//=---------------------------------------------------------------------=
//
// $Id: CAAFObjectTest.cpp,v 1.36 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFCompositionMob>	IAAFCompositionMobSP;
typedef IAAFSmartPointer<IAAFFile>				IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>			IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFIdentification>	IAAFIdentificationSP;
typedef IAAFSmartPointer<IAAFMob>				IAAFMobSP;
typedef IAAFSmartPointer<IAAFMobSlot>			IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFObject>			IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef>		IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>		IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSegment>			IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFSequence>			IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>	IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>		IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IEnumAAFProperties> IEnumAAFPropertiesSP;
typedef IAAFSmartPointer<IAAFProperty> IAAFPropertySP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef> IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFTypeDefSet> IAAFTypeDefSetSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues> IEnumAAFPropertyValuesSP;
typedef IAAFSmartPointer<IAAFTypeDefVariableArray> IAAFTypeDefVariableArraySP;
typedef IAAFSmartPointer<IAAFTypeDefFixedArray> IAAFTypeDefFixedArraySP;

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

const size_t fileNameBufLen = 128;
static aafWChar testFileName[ fileNameBufLen ] = L"";
// AUIDs of optional properties we will create
static const aafUID_t AUID_OptionalProperty = 
{ 0xacf15840, 0x58d6, 0x11d4, { 0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

static const aafUID_t AUID_OptionalProperty2 = 
{ 0xecde5700, 0x58e8, 0x11d4, { 0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// {e209dd80-f369-11d4-9ba2-8fd4b4adfda7}
static const aafUID_t AUID_OptionalProperty3 = 
{ 0xe209dd80, 0xf369, 0x11d4, { 0x9b, 0xa2, 0x8f, 0xd4, 0xb4, 0xad, 0xfd, 0xa7 } };

static const aafUInt32 kOptionalObjectPropertyValue = 42;

// IDs of Mobs we will create
static const aafMobID_t	kTestMobID =
	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x49394200, 0x58ed, 0x11d4, {0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d}}};

//static const aafMobID_t	kTestMobID2 =
//	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
//	0x13, 0x00, 0x00, 0x00,
//	{0x4d540120, 0x5900, 0x11d4,0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d}};
enum traversalType_e
{
  traversalOnly,
  traversalWrite,
  traversalRead
};

static void TraverseObject(IAAFObject *, IAAFPropertyDef *, IAAFTypeDefInt *, traversalType_e); // throw HRESULT

static HRESULT ObjectWriteTest (
	testMode_t mode,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)

{
  HRESULT hr = AAFRESULT_TEST_FAILED;

  IAAFFile* pFile = NULL;
  IAAFHeader * pHeader = NULL;
  IAAFDictionary * pDict = NULL;
  IAAFCompositionMob * pCMob = NULL;
  IAAFMob * pMob = NULL;
  IAAFObject * pObj = NULL;
  IEnumAAFProperties * pEnum = NULL;

  try
  {
	  RemoveTestFile (testFileName);
	  checkResult (CreateTestFile( testFileName, fileKind, rawStorageType, productID, &pFile ));
	  assert (pFile);
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);
	  checkResult (pHeader->GetDictionary (&pDict));
	  assert (pDict);
	  
	  aafProductVersion_t			testRev;
	  checkResult(GetAAFVersions(pHeader, &testRev, NULL));
	  if(testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3))
	  {
		  
		  CAAFBuiltinDefs defs (pDict);
		  
		  // Add an optional property to the class definition for AAFCompositionMob
		  // (pOptionalPropertyDef)
		  IAAFClassDefSP pClassDef;
		  checkResult(pDict->LookupClassDef(AUID_AAFCompositionMob,&pClassDef));
		  IAAFPropertyDefSP pOptionalPropertyDef;
		  checkResult(pClassDef->RegisterOptionalPropertyDef(AUID_OptionalProperty,
			  L"New property",
			  defs.tdUInt16(),
			  &pOptionalPropertyDef));
		  checkResult(pDict->RegisterClassDef(pClassDef));
		  
		  // Add an optional property to the class definition for AAFSequence
		  // (pOptionalPropertyDef2)
		  checkResult(pDict->LookupClassDef(AUID_AAFSequence,&pClassDef));
		  IAAFPropertyDefSP pOptionalPropertyDef2;
		  checkResult(pClassDef->RegisterOptionalPropertyDef(AUID_OptionalProperty2,
			  L"New property",
			  defs.tdUInt16(),
			  &pOptionalPropertyDef2));
		  checkResult(pDict->RegisterClassDef(pClassDef));


      if (ExtendingAAFObjectSupported(testRev))
      {
        // Add an optional property to the class definition for AAFObject.
        checkResult(pDict->LookupClassDef(AUID_AAFObject,&pClassDef));
        IAAFPropertyDefSP pOptionalPropertyDef3;
        checkResult(pClassDef->RegisterOptionalPropertyDef(AUID_OptionalProperty3,
          L"New property on AAFObject",
          defs.tdUInt32(),
          &pOptionalPropertyDef3));
      }


		  // Create an instance of AAFCompositionMob using the newly registered
		  // class definition.
		  checkResult(pDict->LookupClassDef(AUID_AAFCompositionMob,&pClassDef));
		  checkResult (pClassDef->CreateInstance (IID_IAAFCompositionMob,
			  (IUnknown **) &pCMob));
		  assert (pCMob);
		  
		  checkResult (pCMob->QueryInterface (IID_IAAFMob,
			  (void **) &pMob));
		  assert (pMob);
		  
		  checkResult(pMob->SetMobID(kTestMobID));
		  checkResult (pCMob->Initialize (L"TestMob"));
		  checkResult (pHeader->AddMob (pMob));
		  
		  checkResult (pCMob->QueryInterface (IID_IAAFObject,
			  (void **) &pObj));
		  assert (pObj);
		  
		  // Before the optional property is set, calls to RemoveOptionalProperty()
		  // should return AAFRESULT_PROP_NOT_PRESENT
		  hr = pObj->RemoveOptionalProperty(pOptionalPropertyDef);
		  if(testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3))
		  {
			  checkExpression(hr == AAFRESULT_PROP_NOT_PRESENT);
		  }
		  
		  // Set the optional property we just created
		  IAAFTypeDefIntSP pTypeDefInt;
		  checkResult(defs.tdUInt16()->QueryInterface(IID_IAAFTypeDefInt,
			  (void**)&pTypeDefInt));
		  IAAFPropertyValueSP pOptionalPropertyValue;
		  aafUInt16 five=5;
		  checkResult(pTypeDefInt->CreateValue((aafMemPtr_t)&five,sizeof(aafUInt16),
			  &pOptionalPropertyValue));
		  checkResult(pObj->SetPropertyValue(pOptionalPropertyDef,
			  pOptionalPropertyValue));
		  
		  // Now try getting the optional property value from the object
		  IAAFPropertyValueSP pReturnedPropertyValue;
		  checkResult(pObj->GetPropertyValue(pOptionalPropertyDef,
			  &pReturnedPropertyValue));
		  
		  // Perform some negative tests on RemoveOptionalProperty()
		  
		  // RemoveOptionalProperty() with NULL property definition should return
		  // AAFRESULT_NULL_PARAM
		  checkExpression(pObj->RemoveOptionalProperty(0)==AAFRESULT_NULL_PARAM);
		  
		  // RemoveOptionalProperty() with property def. for a property that is not
		  // in the class should return AAFRESULT_BAD_PROP
		  checkExpression(pObj->RemoveOptionalProperty(pOptionalPropertyDef2)
			  ==AAFRESULT_BAD_PROP);
		  
		  // Count number of properties before removing optional property value
		  aafUInt32 oldNumProperties=0;
		  checkResult(pObj->CountProperties(&oldNumProperties));
		  checkExpression(oldNumProperties>=1);
		  
		  // Remove the optional property value
		  checkResult(pObj->RemoveOptionalProperty(pOptionalPropertyDef));
		  
		  // Verify that property count is reduced by 1.
		  aafUInt32 newNumProperties=(aafUInt32)(-1);
		  checkResult(pObj->CountProperties(&newNumProperties));
		  checkExpression(newNumProperties==oldNumProperties-1);
		  
		  // Now we should not be able to get the optional property value from the
		  // object
		  checkExpression(pObj->GetPropertyValue(pOptionalPropertyDef,
			  &pReturnedPropertyValue)==AAFRESULT_PROP_NOT_PRESENT);
		  
		  
		  checkResult (pObj->GetProperties (&pEnum));
		  checkExpression (pEnum != 0, AAFRESULT_TEST_FAILED);
		  while (newNumProperties--)
		  {
			  IAAFProperty * pProp = NULL;
			  checkResult (pEnum->NextOne (&pProp));
			  checkExpression (pProp != 0, AAFRESULT_TEST_FAILED);
			  pProp->Release();
		  }
		  
		  // Now use CreateOptionalPropertyValue() to add an optional property to
		  // an instance of AAFSequence
		  
		  // Create an instance of AAFSequence & initialize it
		  checkResult(pDict->LookupClassDef(AUID_AAFSequence,&pClassDef));
		  IAAFSequenceSP pSequence;
		  checkResult (pClassDef->CreateInstance (IID_IAAFSequence,
			  (IUnknown **)&pSequence));
		  checkResult(pSequence->Initialize(defs.ddkAAFSound()));
		  
		  // Add sequence to composition Mob
		  IAAFSegmentSP pSegment;
		  checkResult(pSequence->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
		  aafRational_t editRate = { 0, 1};
		  IAAFTimelineMobSlotSP pTimelineMobSlot;
		  checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,1,
			  L"AAF Test Sequence",0,&pTimelineMobSlot));
		  
		  IAAFObjectSP pObj2;
		  checkResult(pSequence->QueryInterface(IID_IAAFObject,(void**)&pObj2));
		  
		  // Test CreateOptionalPropertyValue()
		  
		  // First perform some negative tests on CreateOptionalPropertyValue()
		  IAAFPropertyValueSP pCreatedPropVal;
		  checkExpression(pObj2->CreateOptionalPropertyValue(0,&pCreatedPropVal)
			  ==AAFRESULT_NULL_PARAM);
		  checkExpression(pObj2->CreateOptionalPropertyValue(pOptionalPropertyDef2,0)
			  ==AAFRESULT_NULL_PARAM);
		  hr = pObj2->CreateOptionalPropertyValue(pOptionalPropertyDef,
			  &pCreatedPropVal);
		  if(testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3))
		  {
			  checkExpression(hr == AAFRESULT_BAD_PROP);
		  }
		  checkExpression(pCreatedPropVal==0);
		  
		  // Now use CreateOptionalPropertyValue() to create a property value and
		  // set it as an optional property of an instance of AAFSequence.
		  checkResult(pObj2->CreateOptionalPropertyValue(pOptionalPropertyDef2,
			  &pCreatedPropVal));
		  
		  IAAFTypeDefSP pTypeDef;
		  checkResult(pCreatedPropVal->GetType(&pTypeDef));
		  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt,
			  (void**)&pTypeDefInt));
		  aafUInt16 ten=10;
		  checkResult(pTypeDefInt->SetInteger(pCreatedPropVal,(aafMemPtr_t)&ten,
			  sizeof(aafUInt16)));
		  
		  checkResult(pObj2->SetPropertyValue(pOptionalPropertyDef2,
			  pCreatedPropVal));
		  
		  //
		  // Check Generation functionality.
		  //
		  
		  // Test IsGenerationTracked(), DisableGenerationTracking()
		  aafBoolean_t b;
		  checkResult (pObj->IsGenerationTracked (&b));
		  // Generation is not tracked by default.
		  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);
		  
		  // check for redundant disables
		  checkResult (pObj->DisableGenerationTracking());
		  checkResult (pObj->IsGenerationTracked (&b));
		  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);
		  
		  // Change the object (using the CompMob interface) so that
		  // subsequent Save() will know it's a dirty object.
		  aafRational_t eu = { 1, 1 };
		  checkResult (pCMob->SetDefaultFade (0, kAAFFadeNone, eu));
		  
		  // Perform one save and verify that generation was not tracked.
		  checkResult (pFile->Save());
		  
		  AAFRESULT testhr;
		  IAAFIdentificationSP pIdent;
		  aafUID_t identAuid;
		  testhr = pObj->GetGeneration(&pIdent);
		  checkExpression (AAFRESULT_INVALID_PARAM == testhr,
			  AAFRESULT_TEST_FAILED);
		  testhr = pObj->GetGenerationAUID(&identAuid);
		  checkExpression (AAFRESULT_INVALID_PARAM == testhr,
			  AAFRESULT_TEST_FAILED);
		  // make sure gen still isn't tracked
		  checkResult (pObj->IsGenerationTracked (&b));
		  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);
		  
		  // Test EnableGenerationTracking()
		  checkResult (pObj->EnableGenerationTracking());
		  checkResult (pObj->IsGenerationTracked (&b));
		  checkExpression (b == kAAFTrue, AAFRESULT_TEST_FAILED);
		  
		  checkResult (pObj->DisableGenerationTracking());
		  checkResult (pObj->IsGenerationTracked (&b));
		  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);
		  
		  // Check for redundant enables
		  checkResult (pObj->EnableGenerationTracking());
		  checkResult (pObj->EnableGenerationTracking());
		  checkResult (pObj->IsGenerationTracked (&b));
		  checkExpression (b == kAAFTrue, AAFRESULT_TEST_FAILED);
		  
		  // Now that generation tracking is enabled, try a couple of
		  // save()s to see if the latest ident is saved.
		  //
		  // First, add a new identification.  We'll keep track of the
		  // identifications we're putting in by using a bogus productID
		  // AUID, and put small integers in the aafUID_t.Data1 field.
		  //
		  // Here's where we define two version AUIDs for our use.
		  aafUID_t versionUid1 = { 0 };
		  versionUid1.Data1 = 1;
		  aafUID_t versionUid2 = { 0 };
		  versionUid2.Data1 = 2;
		  
		  // Now make a new identification with the first version AUID,
		  // and append it to the header.
		  IAAFIdentificationSP pNewIdent;
		  checkResult (defs.cdIdentification()->
			  CreateInstance (IID_IAAFIdentification,
			  (IUnknown **) &pNewIdent));
		  assert (pNewIdent);
		  checkResult (pNewIdent->Initialize (L"Avid",
			  L"Test File",
			  L"First Test Version",
			  versionUid1));
		  checkResult (pHeader->AppendIdentification (pNewIdent));
		  
		  // With Version1 identification in the header, do a save and see
		  // if that generation made it to our file.
		  //
		  // First, change object to make sure Save() will save it
		  checkResult (pCMob->SetDefaultFade (1, kAAFFadeNone, eu));
		  
		  checkResult (pFile->Save());
		  IAAFIdentificationSP pReadIdent1;
		  checkResult (pObj->GetGeneration(&pReadIdent1));
		  aafUID_t readAuid1 = { 0 };
		  checkResult (pReadIdent1->GetProductID(&readAuid1));
		  checkExpression (1 == readAuid1.Data1, AAFRESULT_TEST_FAILED);
		  
		  // Now make a new identification with the second version AUID,
		  // and append it to the header.
		  checkResult (defs.cdIdentification()->
			  CreateInstance (IID_IAAFIdentification,
			  (IUnknown **) &pNewIdent));
		  assert (pNewIdent);
		  aafUID_t junkAuid;
		  // make sure this isn't allowed before initialization
		  HRESULT temphr = pNewIdent->GetProductID(&junkAuid);
		  checkExpression (AAFRESULT_NOT_INITIALIZED == temphr,
			  AAFRESULT_TEST_FAILED);
		  
		  checkResult (pNewIdent->Initialize (L"Avid",
			  L"Test File",
			  L"Second Test Version",
			  versionUid2));
		  
		  // this should now work after initialization
		  checkResult (pNewIdent->GetProductID(&junkAuid));
		  
		  checkResult (pHeader->AppendIdentification (pNewIdent));
		  
		  // With Version2 identification in the header, do a save and see
		  // if that generation made it to our file.
		  //
		  // First, change object to make sure Save() will save it
		  checkResult (pCMob->SetDefaultFade (2, kAAFFadeNone, eu));
		  checkResult (pFile->Save());
		  IAAFIdentificationSP pReadIdent2;
		  checkResult (pObj->GetGeneration(&pReadIdent2));
		  aafUID_t readAuid2 = { 0 };
		  checkResult (pReadIdent2->GetProductID(&readAuid2));
		  checkExpression (2 == readAuid2.Data1, AAFRESULT_TEST_FAILED);
		  
		  if (ExtendingAAFObjectSupported(testRev))
		  {
        checkResult(pDict->LookupClassDef(AUID_AAFObject,&pClassDef));
        IAAFPropertyDefSP pOptionalObjectPropertyDef;
        checkResult(pClassDef->LookupPropertyDef(AUID_OptionalProperty3, &pOptionalObjectPropertyDef));
        IAAFTypeDefIntSP pOptionalObjectPropertyTypeDef;
        checkResult(defs.tdUInt32()->QueryInterface(IID_IAAFTypeDefInt, (void**)&pOptionalObjectPropertyTypeDef));
        IAAFObjectSP pHeaderObject;
        checkResult(pHeader->QueryInterface(IID_IAAFObject, (void**)&pHeaderObject));
		    TraverseObject(pHeaderObject, pOptionalObjectPropertyDef, pOptionalObjectPropertyTypeDef, traversalWrite);
		  }
}
hr = AAFRESULT_SUCCESS;
	}
  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pEnum) pEnum->Release();
  if (pCMob) pCMob->Release();
  if (pMob) pCMob->Release();
  if (pObj) pObj->Release();
  if (pDict) pDict->Release();
  if (pHeader) pHeader->Release();
  if (pFile)
	{
	  AAFRESULT temphr;
	  temphr = pFile->Save();
	  if (! SUCCEEDED (temphr)) return temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
	  pFile->Release();
	}
  
  return hr;
}


static HRESULT ObjectReadTest ()
{
  HRESULT hr = AAFRESULT_TEST_FAILED;
	IAAFFile * pFile = NULL;

  try
	{
	  checkResult (AAFFileOpenExistingRead(testFileName,
										   0,
										   &pFile));
	  assert (pFile);
	  IAAFHeaderSP pHeader;
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);

      IAAFDictionarySP pDict;
	  checkResult(pHeader->GetDictionary(&pDict));
    CAAFBuiltinDefs defs (pDict);
    
	  aafProductVersion_t			testRev, fileRev;
	  checkResult(GetAAFVersions(pHeader, &testRev, &fileRev));

	  if ((testRev.major >= 1 && (testRev.minor > 0 || testRev.patchLevel > 3)) &&
	      (fileRev.major >= 1 && (fileRev.minor > 0 || fileRev.patchLevel > 3)))
	  {
	  IAAFIdentificationSP pReadIdent;
	  checkResult (pHeader->GetLastIdentification(&pReadIdent));

	  // ident should be initialized, so this should work 
	  aafUID_t junkAuid;
	  checkResult (pReadIdent->GetProductID(&junkAuid));

      // Look up the (extended) class definition for AAFCompositionMob, and get
	  // the definition for the optional property we added.
	  IAAFClassDefSP pClassDef;
	  checkResult(pDict->LookupClassDef(AUID_AAFCompositionMob,&pClassDef));
	  IAAFPropertyDefSP pOptionalPropertyDef;
	  checkResult(pClassDef->LookupPropertyDef(AUID_OptionalProperty,
		  &pOptionalPropertyDef));

      // Also look up the optional property we added to AAFSequence
	  checkResult(pDict->LookupClassDef(AUID_AAFSequence,&pClassDef));
	  IAAFPropertyDefSP pOptionalPropertyDef2;
	  checkResult(pClassDef->LookupPropertyDef(AUID_OptionalProperty2,
		  &pOptionalPropertyDef2));

	  // Look up the Mob we created
	  IAAFMobSP pMob;
	  checkResult(pHeader->LookupMob(kTestMobID,&pMob));

	  // Verify that this is a composition Mob
	  IAAFCompositionMobSP pCompositionMob;
	  checkResult(pMob->QueryInterface(IID_IAAFCompositionMob,
		  (void**)&pCompositionMob));

      IAAFObjectSP pObject;
	  checkResult(pMob->QueryInterface(IID_IAAFObject,(void**)&pObject));

	  // Property should *not* be present in the object, since we removed it via
	  // RemoveOptionalProperty()
	  IAAFPropertyValueSP pOptionalPropertyValue;
	  checkExpression(pObject->GetPropertyValue(pOptionalPropertyDef,
		  &pOptionalPropertyValue)==AAFRESULT_PROP_NOT_PRESENT);

	  // RemoveOptionalProperty() should fail since the property is not present
	  checkExpression(pObject->RemoveOptionalProperty(pOptionalPropertyDef)
		  ==AAFRESULT_PROP_NOT_PRESENT);

      // Look up the sequence we created & get the property value we added
      IAAFMobSlotSP pSlot;
      checkResult(pMob->GetSlotAt(0,&pSlot));
      IAAFObjectSP pObj2;
	  IAAFSegmentSP pSegment;
	  checkResult(pSlot->GetSegment(&pSegment));
      checkResult(pSegment->QueryInterface(IID_IAAFObject,(void**)&pObj2));
      checkResult(pObj2->GetPropertyValue(pOptionalPropertyDef2,
		  &pOptionalPropertyValue));

      // Verify optional property value created via CreateOptionalPropertyValue()
	  IAAFTypeDefSP pTypeDef;
	  checkResult(pOptionalPropertyValue->GetType(&pTypeDef));
	  IAAFTypeDefIntSP pTypeDefInt;
	  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt,
		  (void**)&pTypeDefInt));
	  aafUInt16 storedValue;
	  checkResult(pTypeDefInt->GetInteger(pOptionalPropertyValue,
		  (aafMemPtr_t)&storedValue,sizeof(aafUInt16)));
      checkExpression(storedValue==10);
      
		  
		  if (ExtendingAAFObjectSupported(testRev) && ExtendingAAFObjectSupported(fileRev))
		  {
        checkResult(pDict->LookupClassDef(AUID_AAFObject,&pClassDef));
        IAAFPropertyDefSP pOptionalObjectPropertyDef;
        checkResult(pClassDef->LookupPropertyDef(AUID_OptionalProperty3, &pOptionalObjectPropertyDef));
        IAAFTypeDefIntSP pOptionalObjectPropertyTypeDef;
        checkResult(defs.tdUInt32()->QueryInterface(IID_IAAFTypeDefInt, (void**)&pOptionalObjectPropertyTypeDef));
        IAAFObjectSP pHeaderObject;
        checkResult(pHeader->QueryInterface(IID_IAAFObject, (void**)&pHeaderObject));
		    TraverseObject(pHeaderObject, pOptionalObjectPropertyDef, pOptionalObjectPropertyTypeDef, traversalRead);
		  }
      
	  }

	  hr = AAFRESULT_SUCCESS;
	}

  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pFile)
  {
	  AAFRESULT temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
    pFile->Release();
    pFile = NULL;
  }

  return hr;
}

static void TraverseObjectValue(
  IAAFPropertyValue * pPropertyValue, 
  IAAFPropertyDef * pOptionalPropertyDef,
  IAAFTypeDefInt * pTypeDefInt, 
  traversalType_e traversalType) // throw HRESULT
{
  IAAFTypeDefSP pPropertyValueType;
  checkResult(pPropertyValue->GetType(&pPropertyValueType));

  IAAFTypeDefObjectRefSP pObjectRefType;
  checkResult(pPropertyValueType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectRefType));
  
  IAAFObjectSP pObject;
  checkResult(pObjectRefType->GetObject(pPropertyValue, IID_IAAFObject, (IUnknown **)&pObject));
  
  TraverseObject(pObject, pOptionalPropertyDef, pTypeDefInt, traversalType);
}

// Routine to traverse the object tree by following strong references.
void TraverseObject(
  IAAFObject * pObject,
  IAAFPropertyDef * pOptionalPropertyDef,
  IAAFTypeDefInt * pTypeDefInt, 
  traversalType_e traversalType) // throw HRESULT
{
  checkExpression(pObject && pOptionalPropertyDef && pTypeDefInt);

  IAAFPropertyValueSP pOptionalPropertyValue;
  if (traversalWrite == traversalType)
  {
    checkExpression(pOptionalPropertyDef && pTypeDefInt);
    checkResult(pObject->CreateOptionalPropertyValue(pOptionalPropertyDef, &pOptionalPropertyValue));
    checkResult(pTypeDefInt->SetInteger(pOptionalPropertyValue, (aafMemPtr_t)&kOptionalObjectPropertyValue, sizeof(kOptionalObjectPropertyValue)));
    checkResult(pObject->SetPropertyValue(pOptionalPropertyDef, pOptionalPropertyValue));
  }    
  else if (traversalRead == traversalType)
  {
    checkExpression(pOptionalPropertyDef && pTypeDefInt);
    checkResult(pObject->GetPropertyValue(pOptionalPropertyDef, &pOptionalPropertyValue));
    aafUInt32 testValue = 0;
    checkResult(pTypeDefInt->GetInteger(pOptionalPropertyValue, (aafMemPtr_t)&testValue, sizeof(testValue)));
    checkExpression(kOptionalObjectPropertyValue == testValue, AAFRESULT_TEST_FAILED);
  }
  
  //
  // Now traverse the properties of this instance and recursively
  // call TraverseObject for any strong reference objects...
  //
  IEnumAAFPropertiesSP pEnumProperties;
  checkResult(pObject->GetProperties(&pEnumProperties));
  IAAFPropertySP pProperty;
  while (SUCCEEDED(pEnumProperties->NextOne(&pProperty)))
  {
    IAAFPropertyDefSP pPropertyDef;
    checkResult(pProperty->GetDefinition(&pPropertyDef));
    IAAFTypeDefSP pPropertyType;
    checkResult(pPropertyDef->GetTypeDef(&pPropertyType));
    eAAFTypeCategory_t category = kAAFTypeCatUnknown;
    checkResult(pPropertyType->GetTypeCategory(&category));
    
    IAAFPropertyValueSP pObjectPropertyValue;
    
    if (kAAFTypeCatStrongObjRef == category)
    {
      checkResult(pProperty->GetValue(&pObjectPropertyValue));
      TraverseObjectValue(pObjectPropertyValue, pOptionalPropertyDef, pTypeDefInt, traversalType); 
    }
    else if (kAAFTypeCatSet == category || kAAFTypeCatVariableArray == category || kAAFTypeCatFixedArray == category)
    {
      // Eventhough sets, fixed and variable arrays all have a GetElements method the 
      // method is not on a common interface.
      IAAFPropertyValueSP pCollectionPropertyValue;
      IAAFTypeDefSP pElementType;
      IEnumAAFPropertyValuesSP pEnumPropertyValues;
      eAAFTypeCategory_t elementCategory = kAAFTypeCatUnknown;
      
      if (kAAFTypeCatSet == category)
      {
        IAAFTypeDefSetSP pTypeDefSet;
        checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefSet, (void **)&pTypeDefSet));
        checkResult(pTypeDefSet->GetElementType(&pElementType));
        checkResult(pElementType->GetTypeCategory(&elementCategory));
        if (kAAFTypeCatStrongObjRef == elementCategory)
        {
          checkResult(pProperty->GetValue(&pCollectionPropertyValue));
          checkResult(pTypeDefSet->GetElements(pCollectionPropertyValue, &pEnumPropertyValues));
        }
      }
      else if (kAAFTypeCatVariableArray == category)
      {
        IAAFTypeDefVariableArraySP pTypeDefVariableArray;
        checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pTypeDefVariableArray));
        checkResult(pTypeDefVariableArray->GetType(&pElementType));
        checkResult(pElementType->GetTypeCategory(&elementCategory));
        if (kAAFTypeCatStrongObjRef == elementCategory)
        {
          checkResult(pProperty->GetValue(&pCollectionPropertyValue));
          checkResult(pTypeDefVariableArray->GetElements(pCollectionPropertyValue, &pEnumPropertyValues));
        }
      }
      else if (kAAFTypeCatFixedArray == category)
      {
        IAAFTypeDefFixedArraySP pTypeDefFixedArray;
        checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&pTypeDefFixedArray));
        checkResult(pTypeDefFixedArray->GetType(&pElementType));
        checkResult(pElementType->GetTypeCategory(&elementCategory));
        if (kAAFTypeCatStrongObjRef == elementCategory)
        {
          checkResult(pProperty->GetValue(&pCollectionPropertyValue));
          checkResult(pTypeDefFixedArray->GetElements(pCollectionPropertyValue, &pEnumPropertyValues));
        }
      }
      
      if (kAAFTypeCatStrongObjRef == elementCategory)
      {
        checkExpression(NULL != (IEnumAAFPropertyValues *)pEnumPropertyValues, AAFRESULT_TEST_FAILED);
        IAAFPropertyValueSP pElementPropertyValue;
        while (SUCCEEDED(pEnumPropertyValues->NextOne(&pElementPropertyValue)))
        {
          TraverseObjectValue(pElementPropertyValue, pOptionalPropertyDef, pTypeDefInt, traversalType);
        }
      }
      
    } // collection type category
    
  } // next property
  
} // TraverseObject


extern "C" HRESULT CAAFObject_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFObject_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, testFileName );

  try
  {
		  if(mode == kAAFUnitTestReadWrite)
		  {
			  hr = ObjectWriteTest (mode, fileKind, rawStorageType, productID);
			  if (FAILED(hr))
			  {
				  cerr << "CAAFObject_test...FAILED!" << endl;
				  return hr;
			  }
		  }
		  
		  hr = ObjectReadTest ();
		  if (FAILED(hr))
		  {
			  cerr << "CAAFObject_test...FAILED!" << endl;
			  return hr;
		  }
  }
  catch (...)
	{
	  cerr << "CAAFObject_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
