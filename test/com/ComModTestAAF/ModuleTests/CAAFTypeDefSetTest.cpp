//=---------------------------------------------------------------------=
//
// $Id: CAAFTypeDefSetTest.cpp,v 1.19 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyDefs.h"

#include <iostream>
using namespace std;
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IUnknown>                 IUnknownSP;
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFDefObject>            IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>       IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFMob>                  IAAFMobSP;
typedef IAAFSmartPointer<IAAFObject>               IAAFObjectSP;
typedef IAAFSmartPointer<IAAFClassDef>             IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>          IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>        IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>              IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef>     IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFTypeDefStrongObjRef>  IAAFTypeDefStrongObjRefSP;
typedef IAAFSmartPointer<IAAFTypeDefSet>           IAAFTypeDefSetSP;
typedef IAAFSmartPointer<IAAFDataDef>              IAAFDataDefSP;
typedef IAAFSmartPointer<IEnumAAFDataDefs>         IEnumAAFDataDefsSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues>   IEnumAAFPropertyValuesSP;
typedef IAAFSmartPointer<IAAFTypeDefWeakObjRef>    IAAFTypeDefWeakObjRefSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues>   IEnumAAFPropertyValuesSP;




struct MyDefRecord
{
  MyDefRecord(aafUID_constref xid,
                aafCharacter_constptr xname,
                aafCharacter_constptr xdescription,
                bool xremove) :
    id(xid),
    name(xname),
    description(xdescription),
    remove(xremove)
  {}

  aafUID_constref id;
  aafCharacter_constptr name;
  aafCharacter_constptr description;
  bool remove;
};


#ifndef NO_REFERENCE_TO_MOB_TEST
struct MyMobRecord
{
  MyMobRecord(aafMobID_constref xid,
                aafCharacter_constptr xname) :
    id(xid),
    name(xname)
  {}

  aafMobID_constref id;
  aafCharacter_constptr name;
};
#endif


//
// Test data for modidication of existing set of DataDefinitions.
//

// {ed74d481-88b2-11d4-a812-8c500d171f03}
static const aafUID_t kDataDefID1 = 
{ 0xed74d481, 0x88b2, 0x11d4, { 0xa8, 0x12, 0x8c, 0x50, 0x0d, 0x17, 0x1f, 0x03 } };

// {6d02a381-88b3-11d4-a812-8c500d171f03}
static const aafUID_t kDataDefID2 = 
{ 0x6d02a381, 0x88b3, 0x11d4, { 0xa8, 0x12, 0x8c, 0x50, 0x0d, 0x17, 0x1f, 0x03 } };

// {8a377201-88b3-11d4-a812-8c500d171f03}
static const aafUID_t kDataDefID3 = 
{ 0x8a377201, 0x88b3, 0x11d4, { 0xa8, 0x12, 0x8c, 0x50, 0x0d, 0x17, 0x1f, 0x03 } };

// {08007b01-88be-11d4-a812-8a896f177d33}
static const aafUID_t kDataDefID4 = 
{ 0x08007b01, 0x88be, 0x11d4, { 0xa8, 0x12, 0x8a, 0x89, 0x6f, 0x17, 0x7d, 0x33 } };

// {587ad701-88c4-11d4-a812-8c4af597f4a3}
static const aafUID_t kDataDefID5 = 
{ 0x587ad701, 0x88c4, 0x11d4, { 0xa8, 0x12, 0x8c, 0x4a, 0xf5, 0x97, 0xf4, 0xa3 } };

static const MyDefRecord kDDTestData[] = 
{
  
  MyDefRecord( kDataDefID1,
    L"Data Def 1",
    L"Fake DataDefinition 1 created for testing sets",
    true
  ),
  
  MyDefRecord( kDataDefID2,
    L"Data Def 2",
    L"Fake DataDefinition 2 created for testing sets",
    false
  ),
  
  MyDefRecord( kDataDefID3,
    L"Data Def 3",
    L"Fake DataDefinition 3 created for testing sets",
    true
  ),
  
  MyDefRecord( kDataDefID4,
    L"Data Def 4",
    L"Fake DataDefinition 4 created for testing sets",
    false
  ),
  
  MyDefRecord( kDataDefID5,
    L"Data Def 5",
    L"Fake DataDefinition 5 created for testing sets",
    true
  )  
};

static const aafUInt32 kDataDefTestDataCount = sizeof(kDDTestData) / sizeof(MyDefRecord);



//
// Test data for creating an new set of definitions in the dictionary.
//

// {afcd8582-895e-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefClassID = 
{ 0xafcd8582, 0x895e, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyClassDefData =
  MyDefRecord( kMyDefClassID,
      L"My Def Class",
      L"My Def Class Defintion created for testing sets",
      false
    );

// {1904c702-8966-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefStrongRefTypeID = 
{ 0x1904c702, 0x8966, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyStrongRefTypeData =
  MyDefRecord( kMyDefStrongRefTypeID,
      L"StrongRefernce<My Def>",
      L"Strong reference to MyDefs created for testing sets",
      false
    );


// {3cc80d02-8966-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefStrongRefSetTypeID = 
{ 0x3cc80d02, 0x8966, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyStrongRefSetTypeData =
  MyDefRecord( kMyDefStrongRefSetTypeID,
      L"StrongRefernceSet<My Def>",
      L"Strong reference set to MyDefs created for testing sets",
      false
    );


// {48afd102-895e-11d4-a812-8b94a917bb03}
static const aafUID_t kMyPropID_Dictionary_MyDefinitions = 
{ 0x48afd102, 0x895e, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

static const MyDefRecord kMyStrongRefSetPropData =
  MyDefRecord( kMyPropID_Dictionary_MyDefinitions,
      L"MyDefinitions",
      L"StrongReferenceSetProperty<MyDef>",
      false
    );


// {54e95d82-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID1 = 
{ 0x54e95d82, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {6468a682-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID2 = 
{ 0x6468a682, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {7ea28482-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID3 = 
{ 0x7ea28482, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {cd501e82-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID4 = 
{ 0xcd501e82, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {df31c182-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID5 = 
{ 0xdf31c182, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {ece74702-8961-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID6 = 
{ 0xece74702, 0x8961, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };

// {1607d782-8962-11d4-a812-8b94a917bb03}
static const aafUID_t kMyDefID7 = 
{ 0x1607d782, 0x8962, 0x11d4, { 0xa8, 0x12, 0x8b, 0x94, 0xa9, 0x17, 0xbb, 0x03 } };


static const MyDefRecord kMyDefTestData[] = 
{
  
  MyDefRecord( kMyDefID1,
    L"My Def 1",
    L"My Definition 1 created for testing sets",
    true
  ),
  
  MyDefRecord( kMyDefID2,
    L"My Def 2",
    L"My Definition 2 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID3,
    L"My Def 3",
    L"My Definition 3 created for testing sets",
    true
  ),
  
  MyDefRecord( kMyDefID4,
    L"My Def 4",
    L"My Definition 4 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID5,
    L"My Def 5",
    L"My Definition 5 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID6,
    L"My Def 6",
    L"My Definition 6 created for testing sets",
    false
  ),
  
  MyDefRecord( kMyDefID7,
    L"My Def 7",
    L"My Definition 7 created for testing sets",
    true
  )    
};

static const aafUInt32 kMyDefTestDataCount = sizeof(kMyDefTestData) / sizeof(MyDefRecord);

//
// Data for weak reference set test.
// {c0444f80-f46a-11d4-8cc5-8c00111e02b0}
static const aafUID_t kAAFTypeID_WeakReferenceToMyDefinition = 
{ 0xc0444f80, 0xf46a, 0x11d4, { 0x8c, 0xc5, 0x8c, 0x00, 0x11, 0x1e, 0x02, 0xb0 } };

static const MyDefRecord kMyWeakRefData =
  MyDefRecord( kAAFTypeID_WeakReferenceToMyDefinition,
      L"WeakReference<MyDefinition>",
      L"My Weak Reference to MyDefinition",
      false
    );


// {c0444f81-f46a-11d4-8cc5-8c00111e02b0}
static const aafUID_t kAAFTypeID_WeakReferenceSetToMyDefinitions = 
{ 0xc0444f81, 0xf46a, 0x11d4, { 0x8c, 0xc5, 0x8c, 0x00, 0x11, 0x1e, 0x02, 0xb0 } };


static const MyDefRecord kMyWeakRefSetData =
  MyDefRecord( kAAFTypeID_WeakReferenceSetToMyDefinitions,
      L"WeakReferenceSet<MyDefinition>",
      L"My Weak Reference Set of MyDefinitions",
      false
    );


// {24872080-f5dd-11d4-9ba2-8cc72a2d6747}
static const aafUID_t kAAFPropID_Header_MyDefinitions = 
{ 0x24872080, 0xf5dd, 0x11d4, { 0x9b, 0xa2, 0x8c, 0xc7, 0x2a, 0x2d, 0x67, 0x47 } };

static const MyDefRecord kMyWeakRefSetPropData =
  MyDefRecord( kAAFPropID_Header_MyDefinitions,
      L"Test Property - WeakReferenceSet<MyDefinition>",
      L"Test Property - My Weak Reference Set of MyDefinitions",
      false
    );


#ifndef NO_REFERENCE_TO_MOB_TEST
//
// Test data for creating a set of weak references to mobs.
//

// {B652FB20-0EA4-400a-B817-F04B1D1223AD}
static const aafMobID_t kMobID1 =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{ 0xb652fb20, 0xea4, 0x400a, { 0xb8, 0x17, 0xf0, 0x4b, 0x1d, 0x12, 0x23, 0xad } } };

// {7EE3CC99-D0FA-4a49-893B-E244951388BD}
static const aafMobID_t kMobID2 =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{ 0x7ee3cc99, 0xd0fa, 0x4a49, { 0x89, 0x3b, 0xe2, 0x44, 0x95, 0x13, 0x88, 0xbd } } };

// {14DD6322-38BC-4ca1-9BF6-B044364382C3}
static const aafMobID_t kMobID3 =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{ 0x14dd6322, 0x38bc, 0x4ca1, { 0x9b, 0xf6, 0xb0, 0x44, 0x36, 0x43, 0x82, 0xc3 } } };

static const MyMobRecord kMobTestData[] = 
{
  
  MyMobRecord( kMobID1,
    L"Mob 1"
  ),
  
  MyMobRecord( kMobID2,
    L"Mob 2"
  ),
  
  MyMobRecord( kMobID3,
    L"Mob 3"
  )  
};

static const aafUInt32 kMobTestDataCount = sizeof(kMobTestData) / sizeof(MyMobRecord);


// {7E6865FA-655F-46b3-98F0-CA0DC067B81F}
static const aafUID_t kAAFTypeID_WeakReferenceToMobs = 
{ 0x7e6865fa, 0x655f, 0x46b3, { 0x98, 0xf0, 0xca, 0xd, 0xc0, 0x67, 0xb8, 0x1f } };


static const MyDefRecord kMyMobWeakRefData =
  MyDefRecord( kAAFTypeID_WeakReferenceToMobs,
      L"WeakReference<Mob>",
      L"My Weak Reference to Mobs",
      false
    );


// {387BA659-FAEE-4432-A3B3-EC0A793FCEB0}
static const aafUID_t kAAFTypeID_WeakReferenceSetToMobs = 
{ 0x387ba659, 0xfaee, 0x4432, { 0xa3, 0xb3, 0xec, 0xa, 0x79, 0x3f, 0xce, 0xb0 } };


static const MyDefRecord kMyMobWeakRefSetData =
  MyDefRecord( kAAFTypeID_WeakReferenceSetToMobs,
      L"WeakReferenceSet<Mob>",
      L"My Weak Reference Set of Mobs",
      false
    );


// {0B707DAA-9DB1-49e2-B4F3-EE51B425F652}
static const aafUID_t kAAFPropID_Header_MyMobs = 
{ 0x0b707daa, 0x9db1, 0x49e2, { 0xb4, 0xf3, 0xee, 0x51, 0xb4, 0x25, 0xf6, 0x52 } };

static const MyDefRecord kMyMobWeakRefSetPropData =
  MyDefRecord( kAAFPropID_Header_MyMobs,
      L"Test Property - WeakReferenceSet<Mob>",
      L"Test Property - My Weak Reference Set of Mobs",
      false
    );
#endif


// forward declarations and prototypes
extern "C"
{
  HRESULT CAAFTypeDefSet_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
} 

  void CAAFTypeDefSet_Create(
    aafCharacter_constptr fileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID);

  void CAAFTypeDefSet_Open(aafCharacter_constptr fileName);
  
  void CAAFTypeDefSet_Register(IAAFHeader * pHeader, IAAFDictionary* pDictionary);
  void CAAFTypeDefSet_Write(IAAFHeader* pHeader, IAAFDictionary* pDictionary);
  void CAAFTypeDefSet_Read(IAAFHeader* pHeader, IAAFDictionary* pDictionary);

extern "C" HRESULT CAAFTypeDefSet_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = S_OK;
  const size_t fileNameBufLen = 128;
  aafWChar pFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );
  
  try
  {
    if(mode == kAAFUnitTestReadWrite)
		  CAAFTypeDefSet_Create(pFileName, fileKind, rawStorageType, productID);
    CAAFTypeDefSet_Open(pFileName);
  }
  catch (HRESULT& rhr)
  {
    hr = rhr;
  }

  return hr;
}

// convenient error handlers.
/*inline*/ static void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
/*inline*/ static void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}

static bool EqualObject(IUnknown* pObject1, IUnknown* pObject2)
{
  IUnknownSP pUnknown1, pUnknown2;
  checkResult(pObject1->QueryInterface(IID_IUnknown, (void**)&pUnknown1));
  checkResult(pObject2->QueryInterface(IID_IUnknown, (void**)&pUnknown2));
  if (pUnknown1 == pUnknown2)
    return true;
  else
    return false;
}


void CAAFTypeDefSet_Create(
    aafCharacter_constptr fileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;
  
  // Cleanup the old test file...
  RemoveTestFile(fileName);
  
  checkResult(CreateTestFile( fileName, fileKind, rawStorageType, productID, &pFile ));
  try
  {
    checkResult(pFile->GetHeader(&pHeader));
    checkResult(pHeader->GetDictionary(&pDictionary));
    
    CAAFTypeDefSet_Register(pHeader, pDictionary);
    CAAFTypeDefSet_Write(pHeader, pDictionary);
    CAAFTypeDefSet_Read(pHeader, pDictionary);
    
    checkResult(pFile->Save());
    checkResult(pFile->Close());
  }
  catch (...)
  {
    pFile->Save(); // ignore result
    pFile->Close(); // ignore result
    throw;
  }
}


void CAAFTypeDefSet_Open(aafCharacter_constptr fileName)
{
  IAAFFileSP pFile;
  IAAFHeaderSP pHeader;
  IAAFDictionarySP pDictionary;

  checkResult(AAFFileOpenExistingRead(fileName, 0, &pFile));
  try
  {
    checkResult(pFile->GetHeader(&pHeader));
    checkResult(pHeader->GetDictionary(&pDictionary));
    
    CAAFTypeDefSet_Read(pHeader, pDictionary);
    
    checkResult(pFile->Close());
  }
  catch (...)
  {
    pFile->Close(); // ignore result
    throw;
  }
}




void CAAFTypeDefSet_Register(IAAFHeader * pHeader, IAAFDictionary* pDictionary)
{
  aafProductVersion_t toolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, NULL));

  checkExpression(StrongReferenceSetsSupported(toolkitVersion), AAFRESULT_NOT_IN_CURRENT_VERSION);

  //
  // Create and register a new subclass of AAFDefObject.
  //
  IAAFClassDefSP pDefObjectClass, pMyDefClass;
  checkResult(pDictionary->LookupClassDef(AUID_AAFDefObject, &pDefObjectClass));
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFClassDef, IID_IAAFClassDef, (IUnknown**)&pMyDefClass));
  checkResult(pMyDefClass->Initialize(kMyClassDefData.id, pDefObjectClass, kMyClassDefData.name, kAAFTrue /*concrete*/));
  checkResult(pDictionary->RegisterClassDef(pMyDefClass));

  //
  // Now create and register types for a set of my definitions.
  //
  IAAFTypeDefSP pTempType;
  IAAFTypeDefStrongObjRefSP pMyStrongRefType;
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefStrongObjRef, IID_IAAFTypeDefStrongObjRef, (IUnknown**)&pMyStrongRefType));
  checkResult(pMyStrongRefType->Initialize(kMyStrongRefTypeData.id, pDefObjectClass, kMyStrongRefTypeData.name));
  checkResult(pMyStrongRefType->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType));
  checkResult(pDictionary->RegisterTypeDef(pTempType));
  
  IAAFTypeDefSetSP pMyStrongRefSetType;
  checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefSet, IID_IAAFTypeDefSet, (IUnknown**)&pMyStrongRefSetType));
  checkResult(pMyStrongRefSetType->Initialize(kMyStrongRefSetTypeData.id, pTempType, kMyStrongRefSetTypeData.name));
  checkResult(pMyStrongRefSetType->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType)); // recycle pTempType smart ptr
  checkResult(pDictionary->RegisterTypeDef(pTempType));
  

  if (ExtendingAAFObjectSupported(toolkitVersion))
  {  
    //
    // Attempt to register a new set type as an optional property on the dictionary.
    //
    IAAFClassDefSP pDictionaryClass;
    checkResult(pDictionary->LookupClassDef(AUID_AAFDictionary, &pDictionaryClass));
    IAAFPropertyDefSP pMyDefsSetPropertyDef;
    checkResult(pDictionaryClass->RegisterOptionalPropertyDef(kMyStrongRefSetPropData.id, kMyStrongRefSetPropData.name, pTempType, &pMyDefsSetPropertyDef));

    if (WeakReferenceSetsSupported(toolkitVersion))
    {
      // Create the weak reference set of my definitions type definition.

      // Target array for initializing the weak reference to my definitions.
      aafUID_t myDefinitionsTargetArray[3];
      myDefinitionsTargetArray[0] = kAAFPropID_Root_Header;
      myDefinitionsTargetArray[1] = kAAFPropID_Header_Dictionary;
      myDefinitionsTargetArray[2] = kMyPropID_Dictionary_MyDefinitions;

      const aafUInt32 myDefinitionsTargetArrayCount = 
               sizeof(myDefinitionsTargetArray)/sizeof(aafUID_t);

      
      // Create and initialize the element type for the weak reference set.
      IAAFTypeDefWeakObjRefSP pWeakObjRef;
      checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef,
                                                  IID_IAAFTypeDefWeakObjRef,
                                                  (IUnknown **)&pWeakObjRef));
      IAAFClassDefSP pTargetClass;
      checkResult(pDictionary->LookupClassDef(kMyClassDefData.id, &pTargetClass));   
      checkResult(pWeakObjRef->Initialize(kMyWeakRefData.id,
                                          pTargetClass,
                                          kMyWeakRefData.name,
                                          myDefinitionsTargetArrayCount,
                                          myDefinitionsTargetArray));
      checkResult(pWeakObjRef->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType)); // recycle pTempType smart ptr
      checkResult(pDictionary->RegisterTypeDef(pTempType));
      
      // Create and initialize the weak reference set.
      IAAFTypeDefSetSP pMyWeakRefSetType;
      checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefSet, IID_IAAFTypeDefSet, (IUnknown**)&pMyWeakRefSetType));
      checkResult(pMyWeakRefSetType->Initialize(kMyWeakRefSetData.id, pTempType, kMyWeakRefSetData.name)); // pTempType <==> weak reference type
      checkResult(pMyWeakRefSetType->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType)); // recycle pTempType smart ptr
      checkResult(pDictionary->RegisterTypeDef(pTempType));
      
      // Now add the weak reference set property to the header.
      IAAFClassDefSP pHeaderClass;
      checkResult(pDictionary->LookupClassDef(AUID_AAFHeader, &pHeaderClass));
      IAAFPropertyDefSP pMyWeakRerenceSetPropertyDef;
      checkResult(pHeaderClass->RegisterOptionalPropertyDef(kMyWeakRefSetData.id, kMyWeakRefSetData.name, pTempType, &pMyWeakRerenceSetPropertyDef));
    }

#ifndef NO_REFERENCE_TO_MOB_TEST
    if (WeakReferenceSetsSupported(toolkitVersion))
    {
      // Create the weak reference set of my definitions type definition.

      // Target array for initializing the weak reference to my definitions.
      aafUID_t myDefinitionsTargetArray[3];
      myDefinitionsTargetArray[0] = kAAFPropID_Root_Header;
      myDefinitionsTargetArray[1] = kAAFPropID_Header_Content;
      myDefinitionsTargetArray[2] = kAAFPropID_ContentStorage_Mobs;

      const aafUInt32 myDefinitionsTargetArrayCount = 
               sizeof(myDefinitionsTargetArray)/sizeof(aafUID_t);

      
      // Create and initialize the element type for the weak reference set.
      IAAFTypeDefWeakObjRefSP pWeakObjRef;
      checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef,
                                                  IID_IAAFTypeDefWeakObjRef,
                                                  (IUnknown **)&pWeakObjRef));
      IAAFClassDefSP pTargetClass;
      checkResult(pDictionary->LookupClassDef(AUID_AAFMob, &pTargetClass));   
      checkResult(pWeakObjRef->Initialize(kMyMobWeakRefData.id,
                                          pTargetClass,
                                          kMyMobWeakRefData.name,
                                          myDefinitionsTargetArrayCount,
                                          myDefinitionsTargetArray));
      checkResult(pWeakObjRef->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType)); // recycle pTempType smart ptr
      checkResult(pDictionary->RegisterTypeDef(pTempType));
      
      // Create and initialize the weak reference set.
      IAAFTypeDefSetSP pMyMobWeakRefSetType;
      checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefSet, IID_IAAFTypeDefSet, (IUnknown**)&pMyMobWeakRefSetType));
      checkResult(pMyMobWeakRefSetType->Initialize(kMyMobWeakRefSetData.id, pTempType, kMyMobWeakRefSetData.name)); // pTempType <==> weak reference type
      checkResult(pMyMobWeakRefSetType->QueryInterface(IID_IAAFTypeDef, (void**)&pTempType)); // recycle pTempType smart ptr
      checkResult(pDictionary->RegisterTypeDef(pTempType));
      
      // Now add the weak reference set property to the header.
      IAAFClassDefSP pHeaderClass;
      checkResult(pDictionary->LookupClassDef(AUID_AAFHeader, &pHeaderClass));
      IAAFPropertyDefSP pMyMobWeakRerenceSetPropertyDef;
      checkResult(pHeaderClass->RegisterOptionalPropertyDef(kMyMobWeakRefSetPropData.id, kMyWeakRefSetPropData.name, pTempType, &pMyMobWeakRerenceSetPropertyDef));
    }
#endif
  }  
} 


static void CAAFTypeDefSet_GetDefinitionsSet(
  IAAFObject * pObject,
  aafUID_constref propertyID,
  bool createOptional,
  IAAFTypeDefSet** ppDefinitionsSet,
  IAAFTypeDefObjectRef** ppElementType, 
  IAAFPropertyValue** ppDefinitionsValue)
{    
  // Read selected set property in the given object.
  IAAFClassDefSP pClass;
  checkResult(pObject->GetDefinition(&pClass));

  //
  // Read the data definitions
  //
  IAAFPropertyDefSP pDefinitionsPropertyDef;
  checkResult(pClass->LookupPropertyDef(propertyID, &pDefinitionsPropertyDef));
  // Make sure that the type is a set
  IAAFTypeDefSP pDefinitionsType;
  checkResult(pDefinitionsPropertyDef->GetTypeDef(&pDefinitionsType));
  IAAFTypeDefSetSP pDataDefinitionsSet;
  checkResult(pDefinitionsType->QueryInterface(IID_IAAFTypeDefSet, (void**)&pDataDefinitionsSet));

  // If this an optional property?
  aafBoolean_t optional = kAAFFalse;
  checkResult(pDefinitionsPropertyDef->GetIsOptional(&optional));
  
  // Get the property value that represents the set.
  if (kAAFTrue == optional)
  {
    aafBoolean_t present = kAAFFalse;
    checkResult(pObject->IsPropertyPresent(pDefinitionsPropertyDef, &present));
    if (kAAFTrue == present)
    {
      checkResult(pObject->GetPropertyValue(pDefinitionsPropertyDef, ppDefinitionsValue));
    }
    else
    {
      if (createOptional)
      {
        checkResult(pObject->CreateOptionalPropertyValue(pDefinitionsPropertyDef, ppDefinitionsValue));
      }
      else
      {
        checkResult(AAFRESULT_PROP_NOT_PRESENT);
      }
    }
  }
  else
  {
    checkResult(pObject->GetPropertyValue(pDefinitionsPropertyDef, ppDefinitionsValue));
  }
  
  // If this value really a set type?
  checkResult((*ppDefinitionsValue)->GetType(&pDefinitionsType));
  checkResult(pDefinitionsType->QueryInterface(IID_IAAFTypeDefSet, (void**)ppDefinitionsSet));
  
  // Validate the types...
  checkExpression(EqualObject(*ppDefinitionsSet, pDataDefinitionsSet), AAFRESULT_TEST_FAILED);
  
  // Get the element type.
  IAAFTypeDefSP pType;
  checkResult(pDataDefinitionsSet->GetElementType(&pType));
  checkResult(pType->QueryInterface(IID_IAAFTypeDefObjectRef, (void**)ppElementType));
}


void CAAFTypeDefSet_Write(IAAFHeader* pHeader, IAAFDictionary* pDictionary)
{
  if (!pHeader || !pDictionary)
    throw AAFRESULT_NULL_PARAM;

  aafProductVersion_t toolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, NULL));

  checkExpression(StrongReferenceSetsSupported(toolkitVersion), AAFRESULT_NOT_IN_CURRENT_VERSION);

  
  // Get the property value that represents the set of data definitions.
  IAAFObjectSP pDictionaryObject;
  checkResult(pDictionary->QueryInterface(IID_IAAFObject, (void**)&pDictionaryObject));
  IAAFTypeDefSetSP pDataDefinitionsSet;
  IAAFTypeDefObjectRefSP pElementType;
  IAAFPropertyValueSP pDataDefinitionsValue;
  CAAFTypeDefSet_GetDefinitionsSet(pDictionaryObject,
                                   kAAFPropID_Dictionary_DataDefinitions,
                                   false, /*createOptional*/
                                   &pDataDefinitionsSet,
                                   &pElementType,
                                   &pDataDefinitionsValue);

  // Save the current count...
  aafUInt32 expectedDefinitionCount = 0;
  aafUInt32 definitionCount = 0;
  checkResult(pDictionary->CountDataDefs(&expectedDefinitionCount));
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == expectedDefinitionCount, AAFRESULT_TEST_FAILED);


  //
  // Add new data definitions to the set.
  //
  aafUInt32 index;
  IAAFDataDefSP pDataDef;
  IAAFPropertyValueSP pDataDefValue;
  for (index = 0; index < kDataDefTestDataCount; ++index)
  {
    checkResult(pDictionary->CreateInstance(AUID_AAFDataDef, IID_IAAFDataDef, (IUnknown**)&pDataDef));
    checkResult(pDataDef->Initialize(kDDTestData[index].id, kDDTestData[index].name, kDDTestData[index].description));
    
    // Create the new element and add it to the set.
    checkResult(pElementType->CreateValue(pDataDef, &pDataDefValue));
    checkResult(pDataDefinitionsSet->AddElement(pDataDefinitionsValue, pDataDefValue));
  }

  // Check the count...
  expectedDefinitionCount += kDataDefTestDataCount;
  checkResult(pDictionary->CountDataDefs(&expectedDefinitionCount));
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == expectedDefinitionCount, AAFRESULT_TEST_FAILED);

  
  // Now test removal of selected elements.
  IAAFPropertyValueSP pKeyValue;
  aafUInt32 removeCount = 0;
  for (index = 0; index < kDataDefTestDataCount; ++index)
  {
     checkResult(pDataDefinitionsSet->CreateKey((aafDataBuffer_t)&kDDTestData[index].id, sizeof(aafUID_t), &pKeyValue));
  
    // Get the value with the same key from the set.
    checkResult(pDataDefinitionsSet->LookupElement(pDataDefinitionsValue, pKeyValue, &pDataDefValue));
    
    if (kDDTestData[index].remove)
    {
      ++removeCount;    
      checkResult(pDataDefinitionsSet->RemoveElement(pDataDefinitionsValue, pDataDefValue));
    }    
  }  

  // Check the count...
  expectedDefinitionCount -= removeCount;
  checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
  checkExpression(definitionCount == expectedDefinitionCount, AAFRESULT_TEST_FAILED);


  
  if (ExtendingAAFObjectSupported(toolkitVersion))
  {  
    // Get the property value that represents the set of the new definitions.
    IAAFTypeDefSetSP pMyDefinitionsSet;
    IAAFPropertyValueSP pMyDefinitionsValue;
    IAAFTypeDefObjectRefSP pMyElementType;
    CAAFTypeDefSet_GetDefinitionsSet(pDictionaryObject,
                                     kMyStrongRefSetPropData.id,
                                     true, /*createOptional*/
                                     &pMyDefinitionsSet,
                                     &pMyElementType,
                                     &pMyDefinitionsValue);

    IAAFDefObjectSP pMyDefObject;
    IAAFPropertyValueSP pMyDefValue;
    for (index = 0; index < kMyDefTestDataCount; ++index)
    {
      checkResult(pDictionary->CreateInstance(kMyDefClassID, IID_IAAFDefObject, (IUnknown**)&pMyDefObject));
      checkResult(pMyDefObject->Initialize(kMyDefTestData[index].id, kMyDefTestData[index].name));
      checkResult(pMyDefObject->SetDescription(kMyDefTestData[index].description));
      
      // Create the new element and add it to the set.
      checkResult(pMyElementType->CreateValue(pMyDefObject, &pMyDefValue));
      checkResult(pMyDefinitionsSet->AddElement(pMyDefinitionsValue, pMyDefValue));
    }
    
    for (index = 0; index < kMyDefTestDataCount; ++index)
    {
      checkResult(pMyDefinitionsSet->CreateKey((aafDataBuffer_t)&kMyDefTestData[index].id, sizeof(aafUID_t), &pKeyValue));
    
      // Get the value with the same key from the set.
      checkResult(pMyDefinitionsSet->LookupElement(pMyDefinitionsValue, pKeyValue, &pMyDefValue));
      
      if (kMyDefTestData[index].remove)
      {
        ++removeCount;    
        checkResult(pMyDefinitionsSet->RemoveElement(pMyDefinitionsValue, pMyDefValue));
      }    
    }
    
    if (WeakReferenceSetsSupported(toolkitVersion))
    {
      // Get the property value that represents the set of the new definitions.
      IAAFObjectSP pHeaderObject;
      checkResult(pHeader->QueryInterface(IID_IAAFObject, (void**)&pHeaderObject));
      IAAFTypeDefSetSP pMyWeakDefinitionsSet;
      IAAFPropertyValueSP pMyWeakDefinitionsValue;
      IAAFTypeDefObjectRefSP pMyWeakElementType;
      CAAFTypeDefSet_GetDefinitionsSet(pHeaderObject,
                                       kMyWeakRefSetData.id,
                                       true, /*createOptional*/
                                       &pMyWeakDefinitionsSet,
                                       &pMyWeakElementType,
                                       &pMyWeakDefinitionsValue);
      
      // Add all of the current objects in the my definitions strong reference set into the
      // weak reference set on the header.                                
      IEnumAAFPropertyValuesSP pEnumMyDefinitions;
      IAAFPropertyValueSP pMyWeakReferenceValue;
      checkResult(pMyDefinitionsSet->GetElements(pMyDefinitionsValue, &pEnumMyDefinitions));
      while (SUCCEEDED(pEnumMyDefinitions->NextOne(&pMyDefValue)))
      {
        checkResult(pMyElementType->GetObject(pMyDefValue, IID_IAAFDefObject, (IUnknown **)&pMyDefObject));
        // "convert" into a weak reference...
        checkResult(pMyWeakElementType->CreateValue(pMyDefObject, &pMyWeakReferenceValue));
        checkResult(pMyWeakDefinitionsSet->AddElement(pMyWeakDefinitionsValue, pMyWeakReferenceValue));
      }
    } 
#ifndef NO_REFERENCE_TO_MOB_TEST
    if (WeakReferenceSetsSupported(toolkitVersion))
    {
      aafUInt32 index;
      for (index = 0; index < kMobTestDataCount; ++index)
      {
        IAAFMobSP pMob;
        checkResult(pDictionary->CreateInstance(AUID_AAFMasterMob, IID_IAAFMob, (IUnknown**)&pMob));
        checkResult(pMob->SetMobID(kMobTestData[index].id));
        checkResult(pMob->SetName(kMobTestData[index].name));

        checkResult(pHeader->AddMob(pMob));
      }

      // Get the property value that represents the set of the new definitions.
      IAAFObjectSP pHeaderObject;
      checkResult(pHeader->QueryInterface(IID_IAAFObject, (void**)&pHeaderObject));
      IAAFTypeDefSetSP pMyWeakDefinitionsSet;
      IAAFPropertyValueSP pMyWeakDefinitionsValue;
      IAAFTypeDefObjectRefSP pMyWeakElementType;
      CAAFTypeDefSet_GetDefinitionsSet(pHeaderObject,
                                       kMyMobWeakRefSetPropData.id,
                                       true, /*createOptional*/
                                       &pMyWeakDefinitionsSet,
                                       &pMyWeakElementType,
                                       &pMyWeakDefinitionsValue);

      for (index = 0; index < kMobTestDataCount; ++index)
      {
        IAAFMobSP pMob;
        checkResult(pHeader->LookupMob(kMobTestData[index].id, &pMob));

        IAAFDefObjectSP pMobObject;
        checkResult(pMob->QueryInterface(IID_IAAFObject, (void**)&pMobObject));

        IAAFPropertyValueSP pMyWeakReferenceValue;
        checkResult(pMyWeakElementType->CreateValue(pMobObject, &pMyWeakReferenceValue));
        checkResult(pMyWeakDefinitionsSet->AddElement(pMyWeakDefinitionsValue, pMyWeakReferenceValue));
      }
    }
#endif
  }  

} // CAAFTypeDefSet_Write


void CAAFTypeDefSet_Read(IAAFHeader* pHeader, IAAFDictionary* pDictionary)
{
  if (!pHeader || !pDictionary)
    throw AAFRESULT_NULL_PARAM;    
  
  aafProductVersion_t toolkitVersion, fileToolkitVersion;
  checkResult(GetAAFVersions(pHeader, &toolkitVersion, &fileToolkitVersion));

  checkExpression(StrongReferenceSetsSupported(toolkitVersion), AAFRESULT_NOT_IN_CURRENT_VERSION);

  if (StrongReferenceSetsSupported(fileToolkitVersion))
  {
    // Get the property value that represents the set of data definitions.
    IAAFObjectSP pDictionaryObject;
    checkResult(pDictionary->QueryInterface(IID_IAAFObject, (void**)&pDictionaryObject));
    IAAFTypeDefSetSP pDataDefinitionsSet;
    IAAFPropertyValueSP pDataDefinitionsValue;
    IAAFTypeDefObjectRefSP pElementType;
    CAAFTypeDefSet_GetDefinitionsSet(pDictionaryObject,
                                     kAAFPropID_Dictionary_DataDefinitions,
                                     false, /*createOptional*/
                                     &pDataDefinitionsSet,
                                     &pElementType,
                                     &pDataDefinitionsValue);

    // Validate the property value set with the public data defintiion access methods
    //

    // Check the count...
    aafUInt32 actualDataDefinitionCount = 0;
    aafUInt32 definitionCount = 0;
    checkResult(pDictionary->CountDataDefs(&actualDataDefinitionCount));
    checkResult(pDataDefinitionsSet->GetCount(pDataDefinitionsValue, &definitionCount));
    checkExpression(definitionCount == actualDataDefinitionCount, AAFRESULT_TEST_FAILED);
    
    // Make sure all of the objects can be found in the set value.
    IEnumAAFDataDefsSP pEnumDataDefs;
    checkResult(pDictionary->GetDataDefs(&pEnumDataDefs));
    IAAFDataDefSP pDataDef, pSetDataDef;
    IAAFDefObjectSP pDefObject;
    IAAFPropertyValueSP pKeyValue;
    IAAFPropertyValueSP pDataDefValue;
    IAAFTypeDefSP pDataDefValueType;
    IAAFTypeDefObjectRefSP pDataDefValueObjectRef;
    aafUID_t id;
    aafBoolean_t containsKey = kAAFFalse;
    while (SUCCEEDED(pEnumDataDefs->NextOne(&pDataDef)))
    {
      checkResult(pDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObject));
      checkResult(pDefObject->GetAUID(&id));
      checkResult(pDataDefinitionsSet->CreateKey((aafDataBuffer_t)&id, sizeof(id), &pKeyValue));
      
      // Is the key in the set?
      checkResult(pDataDefinitionsSet->ContainsKey(pDataDefinitionsValue, pKeyValue, &containsKey));
      checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
      
      // Get the value with the same key from the set.
      checkResult(pDataDefinitionsSet->LookupElement(pDataDefinitionsValue, pKeyValue, &pDataDefValue));
      
      // The property value's type  must be the same as the elment type of the set!
      checkResult(pDataDefValue->GetType(&pDataDefValueType));
      checkExpression(EqualObject(pDataDefValueType, pElementType), AAFRESULT_TEST_FAILED);

      // Make sure that we actually found the "same" data definition.
      checkResult(pElementType->GetObject(pDataDefValue, IID_IAAFDataDef, (IUnknown**)&pSetDataDef));
      checkExpression(EqualObject(pSetDataDef, pDataDef), AAFRESULT_TEST_FAILED);
      
      // Is the element in the set?
      checkResult(pDataDefinitionsSet->ContainsElement(pDataDefinitionsValue, pDataDefValue, &containsKey));
      checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
    }

    
    // Turn the test around...
    IEnumAAFPropertyValuesSP pEnumDataDefValues;
    checkResult(pDataDefinitionsSet->GetElements(pDataDefinitionsValue, &pEnumDataDefValues));
    while (SUCCEEDED(pEnumDataDefValues->NextOne(&pDataDefValue)))
    {  
      // The property value's type  must be the same as the elment type of the set!
      checkResult(pDataDefValue->GetType(&pDataDefValueType));
      checkExpression(EqualObject(pDataDefValueType, pElementType), AAFRESULT_TEST_FAILED);
      
      // Make sure that we actually found the "same" data definition.
      checkResult(pElementType->GetObject(pDataDefValue, IID_IAAFDataDef, (IUnknown**)&pSetDataDef));
      checkResult(pSetDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObject));
      checkResult(pDefObject->GetAUID(&id));
      
      checkResult(pDictionary->LookupDataDef(id, &pDataDef));
      checkExpression(EqualObject(pSetDataDef, pDataDef), AAFRESULT_TEST_FAILED);
    }  



    if (ExtendingAAFObjectSupported(toolkitVersion) && ExtendingAAFObjectSupported(fileToolkitVersion))
    {  
      // Get the property value that represents the set of data definitions.
      IAAFTypeDefSetSP pMyDefinitionsSet;
      IAAFPropertyValueSP pMyDefinitionsValue;
      IAAFTypeDefObjectRefSP pMyElementType;
      CAAFTypeDefSet_GetDefinitionsSet(pDictionaryObject,
                                       kMyStrongRefSetPropData.id,
                                       false, /*createOptional*/
                                       &pMyDefinitionsSet,
                                       &pMyElementType,
                                       &pMyDefinitionsValue);

      
      // Attempt to read back all of the new definitions in the set.
      // Turn the test around...
      IEnumAAFPropertyValuesSP pEnumMyDefValues;
      IAAFPropertyValueSP pMyDefValue;
      IAAFTypeDefSP pMyDefValueType;
      IAAFDefObjectSP pMyDefObject;
      checkResult(pMyDefinitionsSet->GetElements(pMyDefinitionsValue, &pEnumMyDefValues));
      while (SUCCEEDED(pEnumMyDefValues->NextOne(&pMyDefValue)))
      {  
        // The property value's type  must be the same as the elment type of the set!
        checkResult(pMyDefValue->GetType(&pMyDefValueType));
        checkExpression(EqualObject(pMyDefValueType, pMyElementType), AAFRESULT_TEST_FAILED);
        
        // Make sure that we actually found the "same" data definition.
        checkResult(pElementType->GetObject(pMyDefValue, IID_IAAFDefObject, (IUnknown**)&pMyDefObject));
        checkResult(pMyDefObject->GetAUID(&id));
      } 
      
       
      aafUInt32 index;
      for (index = 0; index < kMyDefTestDataCount; ++index)
      {
        //kMyDefTestData[index].id;
        
        if (!kMyDefTestData[index].remove)
        {
          checkResult(pMyDefinitionsSet->CreateKey((aafDataBuffer_t)&kMyDefTestData[index].id, sizeof(id), &pKeyValue));
          
          // Is the key in the set?
          checkResult(pMyDefinitionsSet->ContainsKey(pMyDefinitionsValue, pKeyValue, &containsKey));
          checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
      
          // Get the value with the same key from the set.
          checkResult(pMyDefinitionsSet->LookupElement(pMyDefinitionsValue, pKeyValue, &pMyDefValue));
          
          // The property value's type  must be the same as the elment type of the set!
          checkResult(pMyDefValue->GetType(&pMyDefValueType));
          checkExpression(EqualObject(pMyDefValueType, pMyElementType), AAFRESULT_TEST_FAILED);

          // Make sure that we actually found the "same" definition.
          checkResult(pElementType->GetObject(pMyDefValue, IID_IAAFDefObject, (IUnknown**)&pMyDefObject));
          checkResult(pMyDefObject->GetAUID(&id));
          
          // Is the element in the set?
          checkResult(pMyDefinitionsSet->ContainsElement(pMyDefinitionsValue, pMyDefValue, &containsKey));
          checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
        }
      }

      if (WeakReferenceSetsSupported(toolkitVersion) && WeakReferenceSetsSupported(fileToolkitVersion))
      {
        // Get the property value that represents the set of the new definitions.
        IAAFObjectSP pHeaderObject;
        checkResult(pHeader->QueryInterface(IID_IAAFObject, (void**)&pHeaderObject));
        IAAFTypeDefSetSP pMyWeakDefinitionsSet;
        IAAFPropertyValueSP pMyWeakDefinitionsValue;
        IAAFTypeDefObjectRefSP pMyWeakElementType;
        CAAFTypeDefSet_GetDefinitionsSet(pHeaderObject,
                                         kMyWeakRefSetData.id,
                                         false, /*!createOptional*/
                                         &pMyWeakDefinitionsSet,
                                         &pMyWeakElementType,
                                         &pMyWeakDefinitionsValue);

        pEnumMyDefValues->Reset();
        IAAFPropertyValueSP pMyWeakReferenceValue;
        IAAFDefObjectSP pMyWeakDefObject;
        while (SUCCEEDED(pEnumMyDefValues->NextOne(&pMyDefValue)))
        {  
          // Make sure that we actually found the "same" definition.
          checkResult(pMyElementType->GetObject(pMyDefValue, IID_IAAFDefObject, (IUnknown**)&pMyDefObject));
          checkResult(pMyDefObject->GetAUID(&id));
          
          // See if the given id can be found in the weak reference set.
          checkResult(pMyWeakDefinitionsSet->CreateKey((aafDataBuffer_t)&id, sizeof(id), &pKeyValue));
          
          // Is the key in the set?
          checkResult(pMyWeakDefinitionsSet->ContainsKey(pMyWeakDefinitionsValue, pKeyValue, &containsKey));
          checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
      
          // Get the value with the same key from the set.
          checkResult(pMyWeakDefinitionsSet->LookupElement(pMyWeakDefinitionsValue, pKeyValue, &pMyWeakReferenceValue));
          
          // The property value's type  must be the same as the elment type of the set!
          checkResult(pMyWeakReferenceValue->GetType(&pMyDefValueType));
          checkExpression(EqualObject(pMyDefValueType, pMyWeakElementType), AAFRESULT_TEST_FAILED);
          
          checkResult(pMyWeakElementType->GetObject(pMyWeakReferenceValue, IID_IAAFDefObject, (IUnknown**)&pMyWeakDefObject));
          checkExpression(EqualObject(pMyWeakDefObject, pMyDefObject), AAFRESULT_TEST_FAILED);
        }
        
        // Turn the test around...
        IEnumAAFPropertyValuesSP pEnumMyWeakDefinitions;
        checkResult(pMyWeakDefinitionsSet->GetElements(pMyWeakDefinitionsValue, &pEnumMyWeakDefinitions));
        while (SUCCEEDED(pEnumMyWeakDefinitions->NextOne(&pMyWeakReferenceValue)))
        {           
          // The property value's type  must be the same as the elment type of the set!
          checkResult(pMyWeakReferenceValue->GetType(&pMyDefValueType));
          checkExpression(EqualObject(pMyDefValueType, pMyWeakElementType), AAFRESULT_TEST_FAILED);

          // Make sure that we actually found the "same" definition.
          checkResult(pMyWeakElementType->GetObject(pMyWeakReferenceValue, IID_IAAFDefObject, (IUnknown**)&pMyWeakDefObject));
          checkResult(pMyWeakDefObject->GetAUID(&id));
         
          // See if the given id can be found in the strong reference set.
          checkResult(pMyDefinitionsSet->CreateKey((aafDataBuffer_t)&id, sizeof(id), &pKeyValue));
          
          // Is the key in the set?
          checkResult(pMyDefinitionsSet->ContainsKey(pMyDefinitionsValue, pKeyValue, &containsKey));
          checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
      
          // Get the value with the same key from the set.
          checkResult(pMyDefinitionsSet->LookupElement(pMyDefinitionsValue, pKeyValue, &pMyDefValue));
          
          checkResult(pMyElementType->GetObject(pMyDefValue, IID_IAAFDefObject, (IUnknown**)&pMyDefObject));
          checkExpression(EqualObject(pMyWeakDefObject, pMyDefObject), AAFRESULT_TEST_FAILED);
        }
        
      }

#ifndef NO_REFERENCE_TO_MOB_TEST
      if (WeakReferenceSetsSupported(toolkitVersion) && WeakReferenceSetsSupported(fileToolkitVersion))
      {
       // Get the property value that represents the set of weak references to mobs.
        IAAFObjectSP pHeaderObject;
        checkResult(pHeader->QueryInterface(IID_IAAFObject, (void**)&pHeaderObject));
        IAAFTypeDefSetSP pMyMobWeakRefsSet;
        IAAFPropertyValueSP pMyMobWeakRefsValue;
        IAAFTypeDefObjectRefSP pSetElementType;
        CAAFTypeDefSet_GetDefinitionsSet(pHeaderObject,
                                         kMyMobWeakRefSetPropData.id,
                                         false, /*!createOptional*/
                                         &pMyMobWeakRefsSet,
                                         &pSetElementType,
                                         &pMyMobWeakRefsValue);

        aafMobID_t mobID;
        IEnumAAFPropertyValuesSP pEnumMyMobWeakRefValues;
        IAAFPropertyValueSP pMyMobWeakRefValue;
        IAAFPropertyValueSP pMyOtherMobWeakRefValue;
        IAAFMobSP pMyMob;
        IAAFMobSP pMyOtherMob;
        IAAFTypeDefSP pValueType;
        IAAFPropertyValueSP pKeyValue;
        aafBoolean_t containsKey = kAAFFalse;
        checkResult(pMyMobWeakRefsSet->GetElements(pMyMobWeakRefsValue, &pEnumMyMobWeakRefValues));
        while (SUCCEEDED(pEnumMyMobWeakRefValues->NextOne(&pMyMobWeakRefValue)))
        {  
          // Make sure that we actually found the "same" definition.
          checkResult(pSetElementType->GetObject(pMyMobWeakRefValue, IID_IAAFMob, (IUnknown**)&pMyMob));
          checkResult(pMyMob->GetMobID(&mobID));
          
          // See if the given id can be found in the weak reference set.
          checkResult(pMyMobWeakRefsSet->CreateKey((aafDataBuffer_t)&mobID, sizeof(mobID), &pKeyValue));
          
          // Is the key in the set?
          checkResult(pMyMobWeakRefsSet->ContainsKey(pMyMobWeakRefsValue, pKeyValue, &containsKey));
          checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
      
          // Get the value with the same key from the set.
          checkResult(pMyMobWeakRefsSet->LookupElement(pMyMobWeakRefsValue, pKeyValue, &pMyOtherMobWeakRefValue));
          
          // The property value's type  must be the same as the elment type of the set!
          checkResult(pMyOtherMobWeakRefValue->GetType(&pValueType));
          checkExpression(EqualObject(pValueType, pSetElementType), AAFRESULT_TEST_FAILED);
          
          checkResult(pSetElementType->GetObject(pMyOtherMobWeakRefValue, IID_IAAFMob, (IUnknown**)&pMyOtherMob));
          checkExpression(EqualObject(pMyOtherMob, pMyMob), AAFRESULT_TEST_FAILED);
        }

        // Turn the test around...
        IEnumAAFPropertyValuesSP pEnumMyOtherMobWeakRefValues;
        checkResult(pMyMobWeakRefsSet->GetElements(pMyMobWeakRefsValue, &pEnumMyOtherMobWeakRefValues));
        while (SUCCEEDED(pEnumMyOtherMobWeakRefValues->NextOne(&pMyOtherMobWeakRefValue)))
        {           
          // The property value's type  must be the same as the elment type of the set!
          checkResult(pMyOtherMobWeakRefValue->GetType(&pValueType));
          checkExpression(EqualObject(pValueType, pSetElementType), AAFRESULT_TEST_FAILED);

          // Make sure that we actually found the "same" definition.
          checkResult(pSetElementType->GetObject(pMyOtherMobWeakRefValue, IID_IAAFMob, (IUnknown**)&pMyMob));
          checkResult(pMyMob->GetMobID(&mobID));
         
          // See if the given id can be found in the strong reference set.
          checkResult(pMyMobWeakRefsSet->CreateKey((aafDataBuffer_t)&mobID, sizeof(mobID), &pKeyValue));
          
          // Is the key in the set?
          checkResult(pMyMobWeakRefsSet->ContainsKey(pMyMobWeakRefsValue, pKeyValue, &containsKey));
          checkExpression(kAAFTrue == containsKey, AAFRESULT_TEST_FAILED);
      
          // Get the value with the same key from the set.
          checkResult(pMyMobWeakRefsSet->LookupElement(pMyMobWeakRefsValue, pKeyValue, &pMyMobWeakRefValue));
          
          checkResult(pSetElementType->GetObject(pMyMobWeakRefValue, IID_IAAFMob, (IUnknown**)&pMyOtherMob));
          checkExpression(EqualObject(pMyMob, pMyOtherMob), AAFRESULT_TEST_FAILED);
        }
        
      }
#endif

    } // reading extended objects supported
    
  } // Reading sets supported.
  
} // CAAFTypeDefSet_Read
