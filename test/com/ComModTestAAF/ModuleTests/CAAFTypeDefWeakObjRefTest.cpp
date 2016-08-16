
//=---------------------------------------------------------------------=
//
// $Id: CAAFTypeDefWeakObjRefTest.cpp,v 1.23 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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


// Undefine NO_REFERENCE_TO_MOB_TEST to enable the test of
// sets of references to mob. The tests a complied out because
// files with property definitions which are references to mobs
// cannot be opened by the previous version of the toolkit.
#define NO_REFERENCE_TO_MOB_TEST

#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFPropertyDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"
#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IUnknown>                  IUnknownSP;
typedef IAAFSmartPointer<IAAFFile>                  IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>                IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFIdentification>        IAAFIdentificationSP;
typedef IAAFSmartPointer<IAAFDictionary>            IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFObject>                IAAFObjectSP;
typedef IAAFSmartPointer<IAAFClassDef>              IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>           IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>         IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>               IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefWeakObjRef>     IAAFTypeDefWeakObjRefSP;
typedef IAAFSmartPointer<IAAFMob>                   IAAFMobSP;
typedef IAAFSmartPointer<IAAFSequence>              IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFFiller>                IAAFFillerSP;
typedef IAAFSmartPointer<IAAFComponent>             IAAFComponentSP;
typedef IAAFSmartPointer<IAAFSegment>               IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>       IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFMobSlot>               IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef>      IAAFTypeDefObjectRefSP;

#include <iostream>
using namespace std;
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


// Required function prototypes

  // Create the test file.
  void CAAFTypeDefWeakObjRef_create (
    aafCharacter_constptr pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID); // throw HRESULT

  // Open the test file read only and validate the data.
  void CAAFTypeDefWeakObjRef_read (aafCharacter_constptr pFileName); // throw HRESULT
  
  void CAAFTypeDefWeakObjRef_verify (IAAFHeader * pHeader); // throw HRESULT


extern "C" HRESULT CAAFTypeDefWeakObjRef_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTypeDefWeakObjRef_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT result = AAFRESULT_SUCCESS;
  const size_t fileNameBufLen = 128;
  aafCharacter wFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, wFileName );

  try
  {
    // Run through a basic set of tests. Create the file, 
    // and then read and validate the new file.
    if(mode == kAAFUnitTestReadWrite)
      CAAFTypeDefWeakObjRef_create (wFileName, fileKind, rawStorageType, productID);
    CAAFTypeDefWeakObjRef_read (wFileName);
  }
  catch (HRESULT &rhr)
  {
    result = rhr;
  }

  return result;
}



// Constant for "new" weak reference to a type definition.
extern "C" const aafUID_t kAAFTypeID_TestWeakReferenceToType = 
{ 0x012345678, 0x0123, 0x0123, {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77}};

// Constant for "new" weak reference to a type definition.
extern "C" const aafUID_t kAAFPropID_TestWeakReferenceToType = 
{ 0x012345679, 0x0123, 0x0123, {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77}};

const aafCharacter kMyWeakReferenceToTypeDefinitionName[] = L"My Weak Reference to Type Definitions";
const aafCharacter kMyWeakReferenceToTypeDefinitionPropertyName[] = L"My Weak Reference to Type Definition Property";


#ifndef NO_REFERENCE_TO_MOB_TEST
// Constant for "new" weak reference to a mob.
extern "C" const aafUID_t kAAFTypeID_TestWeakReferenceToMob = 
{ 0xfda25dda, 0xbb25, 0x493a, {0x84, 0x28, 0xf2, 0x54, 0x77, 0x23, 0x67, 0x74}};

// Constant for "new" weak reference to a mob.
extern "C" const aafUID_t kAAFPropID_TestWeakReferenceToMob = 
{ 0xacbc7853, 0xfbdc, 0x47b4, {0xab, 0x8b, 0xc6, 0xef, 0xcd, 0x99, 0x31, 0x1b}};

const aafCharacter kMyWeakReferenceToMobName[] = L"My Weak Reference to Mobs";
const aafCharacter kMyWeakReferenceToMobPropertyName[] = L"My Weak Reference to Mob Property";
#endif


static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc68dee89, 0x0405, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const aafSlotID_t TEST_SlotID = 1;

#if 1 //#ifndef _DEBUG
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
#define checkResult(x)\
do {\
  HRESULT r = (x);\
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

static bool EqualObjects(IUnknown *pObj1, IUnknown *pObj2) // throw HRESULT
{
  checkExpression(NULL != pObj1 && NULL != pObj2, AAFRESULT_NULL_PARAM);

  IUnknownSP pUnk1, pUnk2;  
  checkResult(pObj1->QueryInterface(IID_IUnknown, (void **)&pUnk1));
  checkResult(pObj2->QueryInterface(IID_IUnknown, (void **)&pUnk2));
  if ((IUnknown *)pUnk1 == (IUnknown *)pUnk2)
  {
    return true;
  }
  else
  {
    return false;
  }
}


static void CreateWeakReference(
  IAAFFiller * pFiller,
  IAAFTypeDef * pTargetType)
{
  IAAFObjectSP pObject;
  checkResult(pFiller->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));
  IAAFPropertyDefSP pWeakRefPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_TestWeakReferenceToType, &pWeakRefPropertyDef));
  
  // Make sure the property's type definition is in fact a weak reference.
  IAAFTypeDefSP pPropertyType;
  checkResult(pWeakRefPropertyDef->GetTypeDef(&pPropertyType));
  IAAFTypeDefWeakObjRefSP pWeakReferenceType;
  checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefWeakObjRef, (void **)&pWeakReferenceType));

  // Create the weak reference property value.  
  IAAFTypeDefObjectRefSP pObjectReferenceType;
  checkResult(pWeakReferenceType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectReferenceType));
  IAAFPropertyValueSP pWeakReferenceValue;
  checkResult(pObjectReferenceType->CreateValue(pTargetType, &pWeakReferenceValue));
  
  // Install the new weak reference value into the filler object.
  checkResult(pObject->SetPropertyValue(pWeakRefPropertyDef, pWeakReferenceValue));
}

static void ChangeWeakReference(
  IAAFFiller * pFiller,
  IAAFTypeDef * pTargetType)
{
  IAAFObjectSP pObject;
  checkResult(pFiller->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));
  IAAFPropertyDefSP pWeakRefPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_TestWeakReferenceToType, &pWeakRefPropertyDef));
  
  // Get weak reference value from the filler object.
  IAAFPropertyValueSP pWeakReferenceValue;
  checkResult(pObject->GetPropertyValue(pWeakRefPropertyDef, &pWeakReferenceValue));

  // Make sure the value's type definition is in fact a weak reference.
  IAAFTypeDefSP pPropertyType;
  checkResult(pWeakReferenceValue->GetType(&pPropertyType));
  IAAFTypeDefWeakObjRefSP pWeakReferenceType;
  checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefWeakObjRef, (void **)&pWeakReferenceType));

  // Create the weak reference property value.  
  IAAFTypeDefObjectRefSP pObjectReferenceType;
  checkResult(pWeakReferenceType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectReferenceType));
  
  // Make sure the target of the weak reference is a type definition.
  checkResult(pObjectReferenceType->SetObject(pWeakReferenceValue, pTargetType));
}

static void CheckWeakReference(
  IAAFFiller * pFiller,
  IAAFTypeDef * pTargetType)
{
  IAAFObjectSP pObject;
  checkResult(pFiller->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));
  IAAFPropertyDefSP pWeakRefPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_TestWeakReferenceToType, &pWeakRefPropertyDef));
  
  // Get weak reference value from the filler object.
  IAAFPropertyValueSP pWeakReferenceValue;
  checkResult(pObject->GetPropertyValue(pWeakRefPropertyDef, &pWeakReferenceValue));

  // Make sure the value's type definition is in fact a weak reference.
  IAAFTypeDefSP pPropertyType;
  checkResult(pWeakReferenceValue->GetType(&pPropertyType));
  IAAFTypeDefWeakObjRefSP pWeakReferenceType;
  checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefWeakObjRef, (void **)&pWeakReferenceType));

  // Create the weak reference property value.  
  IAAFTypeDefObjectRefSP pObjectReferenceType;
  checkResult(pWeakReferenceType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectReferenceType));
  
  // Make sure the target of the weak reference is a type definition.
  IAAFTypeDefSP pFoundTargetType;
  checkResult(pObjectReferenceType->GetObject(pWeakReferenceValue, IID_IAAFTypeDef, (IUnknown **)&pFoundTargetType));
  
  // Verify that the object that was the target of the weak reference was the
  // type that we were expecting.
  checkExpression(EqualObjects(pFoundTargetType, pTargetType), AAFRESULT_TEST_FAILED);
}


#ifndef NO_REFERENCE_TO_MOB_TEST
static void CreateWeakReference(
  IAAFFiller * pFiller,
  IAAFMob * pTargetMob)
{
  IAAFObjectSP pObject;
  checkResult(pFiller->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));
  IAAFPropertyDefSP pWeakRefPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_TestWeakReferenceToMob, &pWeakRefPropertyDef));
  
  // Make sure the property's type definition is in fact a weak reference.
  IAAFTypeDefSP pPropertyType;
  checkResult(pWeakRefPropertyDef->GetTypeDef(&pPropertyType));
  IAAFTypeDefWeakObjRefSP pWeakReferenceType;
  checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefWeakObjRef, (void **)&pWeakReferenceType));

  // Create the weak reference property value.  
  IAAFTypeDefObjectRefSP pObjectReferenceType;
  checkResult(pWeakReferenceType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectReferenceType));
  IAAFPropertyValueSP pWeakReferenceValue;
  checkResult(pObjectReferenceType->CreateValue(pTargetMob, &pWeakReferenceValue));
  
  // Install the new weak reference value into the filler object.
  checkResult(pObject->SetPropertyValue(pWeakRefPropertyDef, pWeakReferenceValue));
}

static void CheckWeakReference(
  IAAFFiller * pFiller,
  IAAFMob * pTargetMob)
{
  IAAFObjectSP pObject;
  checkResult(pFiller->QueryInterface(IID_IAAFObject, (void **)&pObject));

  IAAFClassDefSP pClassDef;
  checkResult(pObject->GetDefinition(&pClassDef));
  IAAFPropertyDefSP pWeakRefPropertyDef;
  checkResult(pClassDef->LookupPropertyDef(kAAFPropID_TestWeakReferenceToMob, &pWeakRefPropertyDef));
  
  // Get weak reference value from the filler object.
  IAAFPropertyValueSP pWeakReferenceValue;
  checkResult(pObject->GetPropertyValue(pWeakRefPropertyDef, &pWeakReferenceValue));

  // Make sure the value's type definition is in fact a weak reference.
  IAAFTypeDefSP pPropertyType;
  checkResult(pWeakReferenceValue->GetType(&pPropertyType));
  IAAFTypeDefWeakObjRefSP pWeakReferenceType;
  checkResult(pPropertyType->QueryInterface(IID_IAAFTypeDefWeakObjRef, (void **)&pWeakReferenceType));

  // Create the weak reference property value.  
  IAAFTypeDefObjectRefSP pObjectReferenceType;
  checkResult(pWeakReferenceType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectReferenceType));
  
  // Make sure the target of the weak reference is a type definition.
  IAAFMobSP pFoundTargetMob;
  checkResult(pObjectReferenceType->GetObject(pWeakReferenceValue, IID_IAAFMob, (IUnknown **)&pFoundTargetMob));
  
  // Verify that the object that was the target of the weak reference was the
  // type that we were expecting.
  checkExpression(EqualObjects(pFoundTargetMob, pTargetMob), AAFRESULT_TEST_FAILED);
}
#endif


// Create the test file.
void CAAFTypeDefWeakObjRef_create (
    aafCharacter_constptr pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  // Remove the previous test file is one exists
  RemoveTestFile (pFileName);

  // Create the file.
  IAAFFileSP pFile;
  checkResult (CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
  
  try
  {
    IAAFHeaderSP pHeader;
    checkResult (pFile->GetHeader (&pHeader));
    
    aafProductVersion_t toolkitVersion;
    checkResult(GetAAFVersions(pHeader, &toolkitVersion, NULL));
    bool weakReferencesSupported = WeakReferencesSupported(toolkitVersion);
    
    IAAFDictionarySP pDictionary;
    checkResult (pHeader->GetDictionary (&pDictionary));
    
    CAAFBuiltinDefs defs (pDictionary);
   
    if (weakReferencesSupported)
    {
      // Create a Weak reference to a type definition.
      IAAFTypeDefWeakObjRefSP pWeakObjRef;
      checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef,
                                                  IID_IAAFTypeDefWeakObjRef,
                                                  (IUnknown **)&pWeakObjRef));
      
      // Find the class definition for all type definitions.
      IAAFClassDefSP pClassDef;
      checkResult(pDictionary->LookupClassDef(AUID_AAFTypeDef, &pClassDef));
      

  	  aafUID_t targetSet[2];
      targetSet[0] = kAAFPropID_Root_MetaDictionary;
      targetSet[1] = kAAFPropID_MetaDictionary_TypeDefinitions;
      checkResult(pWeakObjRef->Initialize(kAAFTypeID_TestWeakReferenceToType,
                                          pClassDef,
                                          kMyWeakReferenceToTypeDefinitionName,
                                          sizeof(targetSet)/sizeof(aafUID_t),
                                          targetSet));

      // Validate that we make the correct "type" by inspecting the type category.
      IAAFTypeDefSP pTypeDef;
      checkResult(pWeakObjRef->QueryInterface(IID_IAAFTypeDef, (void **)&pTypeDef));

      eAAFTypeCategory_t category;
      checkResult(pTypeDef->GetTypeCategory(&category));
      checkExpression(kAAFTypeCatWeakObjRef == category, AAFRESULT_TEST_FAILED);


    	// Add the new type to the dictionary.
    	checkResult(pDictionary->RegisterTypeDef(pTypeDef));
    	
    	
    	// Now add a new optional weak reference property to an existing class.
    	IAAFClassDefSP pFillerClass;
      IAAFPropertyDefSP pWeakRefPropertyDef;  	
      checkResult(pDictionary->LookupClassDef(AUID_AAFComponent, &pFillerClass));   
      checkResult(pFillerClass->RegisterOptionalPropertyDef(
        kAAFPropID_TestWeakReferenceToType,
        kMyWeakReferenceToTypeDefinitionPropertyName,
        pTypeDef,
        &pWeakRefPropertyDef));      
    }
  

#ifndef NO_REFERENCE_TO_MOB_TEST
    if (weakReferencesSupported)
    {
      // Create a Weak reference to a mob.
      IAAFTypeDefWeakObjRefSP pWeakObjRef;
      checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef,
                                                  IID_IAAFTypeDefWeakObjRef,
                                                  (IUnknown **)&pWeakObjRef));

      // Find the class definition for all mobs.
      IAAFClassDefSP pClassDef;
      checkResult(pDictionary->LookupClassDef(AUID_AAFMob, &pClassDef));


      aafUID_t targetSet[3];
      targetSet[0] = kAAFPropID_Root_Header;
      targetSet[1] = kAAFPropID_Header_Content;
      targetSet[2] = kAAFPropID_ContentStorage_Mobs;
      checkResult(pWeakObjRef->Initialize(kAAFTypeID_TestWeakReferenceToMob,
                                          pClassDef,
                                          kMyWeakReferenceToMobName,
                                          sizeof(targetSet)/sizeof(aafUID_t),
                                          targetSet));

      // Validate that we make the correct "type" by inspecting the type category.
      IAAFTypeDefSP pTypeDef;
      checkResult(pWeakObjRef->QueryInterface(IID_IAAFTypeDef, (void **)&pTypeDef));

      eAAFTypeCategory_t category;
      checkResult(pTypeDef->GetTypeCategory(&category));
      checkExpression(kAAFTypeCatWeakObjRef == category, AAFRESULT_TEST_FAILED);


      // Add the new type to the dictionary.
      checkResult(pDictionary->RegisterTypeDef(pTypeDef));


      // Now add a new optional weak reference property to an existing class.
      IAAFClassDefSP pFillerClass;
      IAAFPropertyDefSP pWeakRefPropertyDef;
      checkResult(pDictionary->LookupClassDef(AUID_AAFComponent, &pFillerClass));
      checkResult(pFillerClass->RegisterOptionalPropertyDef(
        kAAFPropID_TestWeakReferenceToMob,
        kMyWeakReferenceToMobPropertyName,
        pTypeDef,
        &pWeakRefPropertyDef));
    }
#endif
  


  
    //
	  // Create a Composition Mob
	  //
	  IAAFMobSP pMob;
	  checkResult(defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"TestCompMob"));
	  
	  //
	  // Create a sequence to hold our test fillers components.
	  //
	  IAAFSequenceSP pSequence;
	  checkResult (defs.cdSequence()->CreateInstance(IID_IAAFSequence, (IUnknown **)&pSequence));
	  checkResult (pSequence->Initialize (defs.ddkAAFPicture()));

	  
	  IAAFFillerSP pFiller1;
	  checkResult (defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IUnknown **)&pFiller1));
	  checkResult (pFiller1->Initialize (defs.ddkAAFPicture(), 16));
	
		IAAFFillerSP pFiller2;
	  checkResult (defs.cdFiller()->CreateInstance(IID_IAAFFiller, (IUnknown **)&pFiller2));
	  checkResult (pFiller2->Initialize (defs.ddkAAFPicture(), 32));


    if (weakReferencesSupported)
    {
      // Try adding a weak reference before filler is attached to the file.
      CreateWeakReference(pFiller1, defs.tdInt64());
      CheckWeakReference(pFiller1, defs.tdInt64());
      
      ChangeWeakReference(pFiller1, defs.tdString());
      CreateWeakReference(pFiller2, defs.tdInt32()); 	
    }


    //
    // Add the initialized fillers to the sequence.
    //
    IAAFComponentSP pComp1;
  	checkResult(pFiller1->QueryInterface(IID_IAAFComponent, (void **)&pComp1));
  	checkResult(pSequence->AppendComponent(pComp1));

    IAAFComponentSP pComp2;
  	checkResult(pFiller2->QueryInterface(IID_IAAFComponent, (void **)&pComp2));	
  	checkResult(pSequence->AppendComponent(pComp2));
  	
  	//
  	// Append the sequence as the segment in a new timeline mob slot.
  	//
  	IAAFSegmentSP pSegment;
  	checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
   	aafRational_t  editRate = {30000, 1001};
   	aafCharacter_constptr pSlotName = L"Slot 1";
   	aafPosition_t  origin = 0;
   	IAAFTimelineMobSlotSP pNewSlot;
   	checkResult(pMob->AppendNewTimelineSlot(editRate, pSegment, TEST_SlotID, pSlotName, origin, &pNewSlot));

    //
    // Add to the set of mobs in the file.
    //
    checkResult(pHeader->AddMob(pMob));

#ifndef NO_REFERENCE_TO_MOB_TEST
    if (weakReferencesSupported)
    {
      // The referenced object needs to be attached to the file.
      CreateWeakReference(pFiller1, pMob);
      CheckWeakReference(pFiller1, pMob);
    }
#endif
//    if (weakReferencesSupported)
//    {
//      // Try adding a weak reference after filler is attached to the file.
//      CreateWeakReference(pFiller2, defs.tdInt32());  	
//    }

    // Verify the data before the save.
    CAAFTypeDefWeakObjRef_verify (pHeader);

    checkResult(pFile->Save());
    checkResult(pFile->Close());
  }
  catch (HRESULT& rhr)
  {
    if (pFile) // only save & close the file if it was actually opened
    {
      pFile->Save();  // This may not be safe???
      pFile->Close();
    }
    throw rhr;
  }
  catch (...)
  {
    if (pFile) // only close the file if it was actually opened
      pFile->Close();  
    throw;
  }
}



// Create the test file.
void CAAFTypeDefWeakObjRef_read (aafCharacter_constptr pFileName) // throw HRESULT
{
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  HRESULT result = S_OK;

  try
  {
    checkResult (AAFFileOpenExistingRead(pFileName, 0, &pFile));
    checkResult (pFile->GetHeader (&pHeader));
    
    CAAFTypeDefWeakObjRef_verify (pHeader);

    result = pFile->Close();
  }
  catch (...)
  {
    if (pFile) // only close the file if it was actually opened
      pFile->Close();  
    throw;
  }
  checkResult(result);
}



void CAAFTypeDefWeakObjRef_verify (IAAFHeader * pHeader)
{
  IAAFDictionarySP pDictionary;
  IAAFPropertyDefSP pWeakRefPropertyDef;

  checkResult (pHeader->GetDictionary (&pDictionary));
  CAAFBuiltinDefs defs (pDictionary);
  
  // Determine if it is okay to ready/validate weak references from the
  // test file with the current version of the AAF.
  bool weakReferencesSupported = false;
  aafProductVersion_t toolkitVersion, fileToolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, &fileToolkitVersion));
  if (WeakReferencesSupported(toolkitVersion) && WeakReferencesSupported(fileToolkitVersion))
  {
    weakReferencesSupported = true;
  } 


 
  if (weakReferencesSupported)
  {    
    //
    // Find and validate the new weak reference.
    IAAFTypeDefSP pType;
    checkResult(pDictionary->LookupTypeDef (kAAFTypeID_TestWeakReferenceToType, &pType));
    eAAFTypeCategory_t category;
    checkResult(pType->GetTypeCategory(&category));
    checkExpression(kAAFTypeCatWeakObjRef == category, AAFRESULT_TEST_FAILED);

    IAAFTypeDefWeakObjRefSP pWeakReferenceType;
  	checkResult(pType->QueryInterface(IID_IAAFTypeDefWeakObjRef, (void **)&pWeakReferenceType));
  	
  	checkResult(defs.cdFiller()->LookupPropertyDef(kAAFPropID_TestWeakReferenceToType, &pWeakRefPropertyDef));
  	
  	// Validate the property's type
  	checkResult(pWeakRefPropertyDef->GetTypeDef(&pType));
    checkExpression(EqualObjects(pType, pWeakReferenceType), AAFRESULT_TEST_FAILED);
    
    // Use GetObjectType to make sure that the target class definitions are
    // the same.
    IAAFTypeDefObjectRefSP pObjectReferenceType;
    checkResult(pWeakReferenceType->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pObjectReferenceType));

    IAAFClassDefSP pReferencedObjectClass;
    checkResult(pObjectReferenceType->GetObjectType(&pReferencedObjectClass));

    // Find the class definition for all type definitions.
    IAAFClassDefSP pTypeDefClass;
    checkResult(pDictionary->LookupClassDef(AUID_AAFTypeDef, &pTypeDefClass));
    checkExpression(EqualObjects(pReferencedObjectClass, pTypeDefClass), AAFRESULT_TEST_FAILED);


    //
    // Find our composition mob.
    IAAFMobSP pMob;
    checkResult(pHeader->LookupMob(TEST_MobID, &pMob));
    
    IAAFMobSlotSP pSlot;
    checkResult(pMob->LookupSlot(TEST_SlotID, &pSlot));
    
    IAAFSegmentSP pSegment;
    checkResult(pSlot->GetSegment(&pSegment));
    
    IAAFSequenceSP pSequence;
    checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **)&pSequence));	
    
    aafUInt32 elementCount;
    checkResult(pSequence->CountComponents(&elementCount));
    checkExpression(2 == elementCount, AAFRESULT_TEST_FAILED);
    
    IAAFComponentSP pComp1;
    checkResult(pSequence->GetComponentAt(0, &pComp1));
    IAAFFillerSP pFiller1;
    checkResult(pComp1->QueryInterface(IID_IAAFFiller, (void **)&pFiller1));
    
    IAAFComponentSP pComp2;
    checkResult(pSequence->GetComponentAt(1, &pComp2));
    IAAFFillerSP pFiller2;
    checkResult(pComp2->QueryInterface(IID_IAAFFiller, (void **)&pFiller2));
    
    CheckWeakReference(pFiller1, defs.tdString());  	
    CheckWeakReference(pFiller2, defs.tdInt32());  	
#ifndef NO_REFERENCE_TO_MOB_TEST
    CheckWeakReference(pFiller1, pMob);
#endif
  }
  else if (!WeakReferencesSupported(toolkitVersion))
  {
    // This version does not support reading weak references.
	  throw AAFRESULT_NOT_IN_CURRENT_VERSION;
	}
}



