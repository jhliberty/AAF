//=---------------------------------------------------------------------=
//
// $Id: CAAFTypeDefOpaqueTest.cpp,v 1.15 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFSmartPointer.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFPropertyDefs.h"
#include "CAAFBuiltinDefs.h"
#include "AAFDefUIDs.h"

typedef IAAFSmartPointer<IAAFFile> IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader> IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFKLVData> IAAFKLVDataSP;
typedef IAAFSmartPointer<IAAFMetaDefinition> IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFMob> IAAFMobSP;
typedef IAAFSmartPointer<IAAFMobSlot> IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFObject> IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef> IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue> IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSegment> IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFSequence> IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot> IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDefIndirect> IAAFTypeDefIndirectSP;
typedef IAAFSmartPointer<IAAFTypeDefInt> IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IAAFTypeDefOpaque> IAAFTypeDefOpaqueSP;
typedef IAAFSmartPointer<IAAFTypeDefRename> IAAFTypeDefRenameSP;
typedef IAAFSmartPointer<IEnumAAFTypeDefs> IEnumAAFTypeDefsSP;

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

// Function to test if two AUIDs are equal
static aafBoolean_t AreAUIDsEqual(aafUID_t& a1, aafUID_t& a2)
{
	// We cannot simply call memcmp() (or use the == operator), since there is 
	// padding in between the structure fields due to byte alignment, and this 
	// padding does not have to be equal in order for the AUIDs to be equal.
	if(memcmp(&a1.Data1,&a2.Data1,sizeof(a1.Data1)))
		return(kAAFFalse);
	if(memcmp(&a1.Data2,&a2.Data2,sizeof(a1.Data2)))
		return(kAAFFalse);
	if(memcmp(&a1.Data3,&a2.Data3,sizeof(a1.Data3)))
		return(kAAFFalse);
	if(memcmp(&a1.Data4,&a2.Data4,sizeof(a1.Data4)))
		return(kAAFFalse);
	return(kAAFTrue);
}

// ID of opaque property we will add to AAFSequence
static const aafUID_t testOpaquePropertyID = 
{ 0xb0636560, 0x4ec8, 0x11d4, { 0x92, 0x26, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// ID of opaque type we will register
static const aafUID_t testOpaqueTypeID = 
{ 0xe97ada40, 0x5419, 0x11d4, { 0x92, 0x29, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// ID of Mob we will create
static const aafMobID_t	testMobID =
	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0xfb6feaa0, 0x527a, 0x11d4, {0x92, 0x26, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d}}};

static char testSavedOpaqueData[]="Opaque data saved in file";

// Function to register an opaque type
static void RegisterOpaqueType(IAAFDictionary *pDictionary)
{
	// Nothing to do if the opaque type has already been registered
	IAAFTypeDefSP pTypeDef;
	if(pDictionary->LookupOpaqueTypeDef(testOpaqueTypeID,&pTypeDef)
		==AAFRESULT_SUCCESS)
		return;

	// Otherwise register it as a rename of UInt8Array
	IAAFTypeDefSP pBaseType;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_UInt8Array,&pBaseType));
	IAAFTypeDefRenameSP pTypeDefRename;
	checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefRename,
		IID_IAAFTypeDefRename,(IUnknown**)&pTypeDefRename));
	checkResult(pTypeDefRename->Initialize(testOpaqueTypeID,pBaseType,
		L"An opaque type"));
	checkResult(pTypeDefRename->QueryInterface(IID_IAAFTypeDef,(void**)&pTypeDef));
	checkResult(pDictionary->RegisterOpaqueTypeDef(pTypeDef));
}

// Function to create a property value of an opaque type
static void CreateOpaquePropertyValue(IAAFDictionary *pDictionary,
	aafDataBuffer_t pInitData,aafUInt32 initDataSize,
	IAAFPropertyValue **ppOpaquePropertyValue)
{
	RegisterOpaqueType(pDictionary);

	// Look up the opaque type we just registered
	IAAFTypeDefSP pRegisteredOpaqueType;
	checkResult(pDictionary->LookupOpaqueTypeDef(testOpaqueTypeID,
		&pRegisteredOpaqueType));

	// Look up type definition for IAAFTypeDefOpaque and QI for IAAFTypeDefIndirect
	IAAFTypeDefSP pTypeDef;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_Opaque,&pTypeDef));
	IAAFTypeDefIndirectSP pTypeDefIndirect;
	checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefIndirect,
		(void**)&pTypeDefIndirect));

	// Create opaque property value via CreateValueFromActualData()
	IAAFPropertyValueSP pIndirectPropertyValue;
	checkResult(pTypeDefIndirect->CreateValueFromActualData(pRegisteredOpaqueType,
		pInitData,initDataSize,ppOpaquePropertyValue));		
}

// Function to extract raw data from an opaque property value
static void GetDataFromOpaquePropertyValue(IAAFDictionary *pDictionary,
	IAAFPropertyValue *pPropVal,aafDataBuffer_t *ppOpaqueData,
	aafUInt32 *pOpaqueDataLen)
{
	RegisterOpaqueType(pDictionary);

	// Look up type definition for IAAFTypeDefOpaque and QI for IAAFTypeDefIndirect
	IAAFTypeDefSP pTypeDef;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_Opaque,&pTypeDef));
	IAAFTypeDefIndirectSP pTypeDefIndirect;
	checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefIndirect,
		(void**)&pTypeDefIndirect));

	// Get opaque data via IAAFTypeDefIndirect interface
	checkResult(pTypeDefIndirect->GetActualSize(pPropVal,pOpaqueDataLen));
	(*ppOpaqueData)=new unsigned char[*pOpaqueDataLen];
	checkResult(pTypeDefIndirect->GetActualData(pPropVal,*ppOpaqueData,
		*pOpaqueDataLen));
}

static void CreateTypeDefOpaqueFile(
    aafWChar *pFilename,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	// Remove the previous test file, if any.
	RemoveTestFile(pFilename);

	// Create new AAF file.
	IAAFFileSP pFile;
	checkResult(CreateTestFile( pFilename, fileKind, rawStorageType, productID, &pFile ));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	CAAFBuiltinDefs defs(pDictionary);

	char testInitialOpaqueData[]="Initial opaque data";

	// Create an opaque property value
	IAAFPropertyValueSP pOpaquePropertyValue;
	CreateOpaquePropertyValue(pDictionary,(aafDataBuffer_t)testInitialOpaqueData,
		sizeof(testInitialOpaqueData),&pOpaquePropertyValue);

	// Look up type definition for IAAFTypeDefOpaque
	IAAFTypeDefSP pTypeDef;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_Opaque,&pTypeDef));
	IAAFTypeDefOpaqueSP pTypeDefOpaque;
	checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefOpaque,
		(void**)&pTypeDefOpaque));

	// Use opaque property value to test IAAFTypeDefOpaque methods
	aafUInt32 handleBufLen;
	checkResult(pTypeDefOpaque->GetHandleBufLen(pOpaquePropertyValue,
		&handleBufLen));
	
	unsigned char *pHandle=new unsigned char[handleBufLen];
	aafUInt32 bytesRead;
	checkResult(pTypeDefOpaque->GetHandle(pOpaquePropertyValue,handleBufLen,
		(aafDataBuffer_t)pHandle,&bytesRead));

	checkExpression(bytesRead==handleBufLen);

	// Calling GetHandle() with a buffer 1 byte too small should return 
	// AAFRESULT_SMALLBUF
	checkResult(pTypeDefOpaque->GetHandle(pOpaquePropertyValue,handleBufLen-1,
		(aafDataBuffer_t)pHandle,&bytesRead)==AAFRESULT_SMALLBUF);

	// Make sure GetActualTypeID() returns correct value
	aafUID_t actualTypeID;
	checkResult(pTypeDefOpaque->GetActualTypeID(pOpaquePropertyValue,
		&actualTypeID));
	// Have to assign testOpaqueTypeID to non-const aafUID_t to call 
	// AreAUIDsEqual()
	aafUID_t OpaqueTypeID=testOpaqueTypeID;
	checkExpression(AreAUIDsEqual(actualTypeID,OpaqueTypeID)==kAAFTrue);

	// Create an opaque property value from a handle
	IAAFPropertyValueSP pCreatedOpaquePropertyValue;
	checkResult(pTypeDefOpaque->CreateValueFromHandle(pHandle,handleBufLen,
		&pCreatedOpaquePropertyValue));
	// No longer need this handle
	delete [] pHandle;

	// Verify opaque data
	aafDataBuffer_t pOpaqueData;
	aafUInt32 opaqueDataLen;
	GetDataFromOpaquePropertyValue(pDictionary,pOpaquePropertyValue,
		&pOpaqueData,&opaqueDataLen);
	checkExpression(opaqueDataLen==sizeof(testInitialOpaqueData));
	checkExpression(!memcmp(testInitialOpaqueData,pOpaqueData,
		sizeof(testInitialOpaqueData)));
	delete [] pOpaqueData;

	// Create an new opaque property, get a handle from it, and use this
	// to set the handle of our created property via SetHandle()
	IAAFPropertyValueSP pNewOpaquePropertyValue;
	CreateOpaquePropertyValue(pDictionary,(aafDataBuffer_t)testSavedOpaqueData,
		sizeof(testSavedOpaqueData),&pOpaquePropertyValue);
	aafUInt32 newHandleBufLen;
	checkResult(pTypeDefOpaque->GetHandleBufLen(pOpaquePropertyValue,
		&newHandleBufLen));
	unsigned char *pNewHandle=new unsigned char[newHandleBufLen];
	checkResult(pTypeDefOpaque->GetHandle(pOpaquePropertyValue,newHandleBufLen,
		(aafDataBuffer_t)pNewHandle,&bytesRead));
	checkResult(pTypeDefOpaque->SetHandle(pCreatedOpaquePropertyValue,
		newHandleBufLen,pNewHandle));
	// no longer need this handle
	delete [] pNewHandle;

	// Verify new opaque data
	GetDataFromOpaquePropertyValue(pDictionary,pOpaquePropertyValue,
		&pOpaqueData,&opaqueDataLen);
	checkExpression(opaqueDataLen==sizeof(testSavedOpaqueData));
	checkExpression(!memcmp(testSavedOpaqueData,pOpaqueData,
		sizeof(testSavedOpaqueData)));
	delete [] pOpaqueData;

	// Add an optional opaque property to AAFSequence.
	IAAFClassDefSP pSequenceClassDef;
	checkResult(pDictionary->LookupClassDef(AUID_AAFSequence,
		&pSequenceClassDef));
	IAAFPropertyDefSP pOpaqueProperty;
	checkResult(pSequenceClassDef->RegisterOptionalPropertyDef(
		testOpaquePropertyID,
		L"Opaque property",
		pTypeDef,
		&pOpaqueProperty));
	checkResult(pDictionary->RegisterClassDef(pSequenceClassDef));

	// Create an instance of IAAFSequence & initialize it
	IAAFSequenceSP pSequence;
	checkResult(pDictionary->CreateInstance(AUID_AAFSequence,IID_IAAFSequence,
		(IUnknown**)&pSequence));
	checkResult(pSequence->Initialize(defs.ddkAAFSound()));

	// Set value of our new property to property value created above
	IAAFObjectSP pObject;
	checkResult(pSequence->QueryInterface(IID_IAAFObject,(void**)&pObject));
	checkResult(pObject->SetPropertyValue(pOpaqueProperty,
		pCreatedOpaquePropertyValue));

	// Create a Composition Mob
	IAAFMobSP pMob;
	checkResult(defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, 
		(IUnknown **)&pMob));
	checkResult(pMob->SetMobID(testMobID));
	checkResult(pMob->SetName(L"TypeDefObjectRefTest"));
  
	// Add sequence to composition Mob
	IAAFSegmentSP pSegment;
	checkResult(pSequence->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
	aafRational_t editRate = { 0, 1};
	IAAFTimelineMobSlotSP pTimelineMobSlot;
	checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,1,
		L"AAF Test Sequence",0,&pTimelineMobSlot));

	// Add composition Mob to file
	pHeader->AddMob(pMob);

	// Save & close file
	checkResult(pFile->Save());
	checkResult(pFile->Close());
}

static void ReadTypeDefOpaqueFile(aafWChar *pFilename)
{
	IAAFFileSP pFile;
	checkResult(AAFFileOpenExistingRead(pFilename,0,&pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	CAAFBuiltinDefs defs(pDictionary);

	// Retrieve the sequence we created

	// Look up Mob 
	IAAFMobSP pMob;
	checkResult(pHeader->LookupMob(testMobID,&pMob));
	IAAFMobSlotSP pSlot;
	checkResult(pMob->GetSlotAt(0,&pSlot));

	IAAFSegmentSP pSegment;
	checkResult(pSlot->GetSegment(&pSegment));
	IAAFObjectSP pObject;
	checkResult(pSegment->QueryInterface(IID_IAAFObject,(void**)&pObject));

	// Look up our opaque property definition
	IAAFClassDefSP pClassDef;
	checkResult(pDictionary->LookupClassDef(AUID_AAFSequence,&pClassDef));
	IAAFPropertyDefSP pOpaqueProperty;
	checkResult(pClassDef->LookupPropertyDef(testOpaquePropertyID,
		&pOpaqueProperty));

	// Get property value
	IAAFPropertyValueSP pOpaquePropertyValue;
	checkResult(pObject->GetPropertyValue(pOpaqueProperty,&pOpaquePropertyValue));

	// Look up type definition for IAAFTypeDefOpaque
	IAAFTypeDefSP pTypeDef;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_Opaque,&pTypeDef));
	IAAFTypeDefOpaqueSP pTypeDefOpaque;
	checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefOpaque,
		(void**)&pTypeDefOpaque));

	// Make sure GetActualTypeID() returns expected value
	aafUID_t ActualTypeID;
	checkResult(pTypeDefOpaque->GetActualTypeID(pOpaquePropertyValue,
		&ActualTypeID));
	// Have to assign to non-const aafUID_t to call AreAUIDsEqual()
	aafUID_t OpaqueTypeID=testOpaqueTypeID;
	checkExpression(AreAUIDsEqual(OpaqueTypeID,ActualTypeID)==kAAFTrue);

	// Verify saved opaque data
	aafDataBuffer_t pOpaqueData;
	aafUInt32 opaqueDataLen;
	GetDataFromOpaquePropertyValue(pDictionary,pOpaquePropertyValue,
		&pOpaqueData,&opaqueDataLen);
	checkExpression(opaqueDataLen==sizeof(testSavedOpaqueData));
	checkExpression(!memcmp(testSavedOpaqueData,pOpaqueData,
		sizeof(testSavedOpaqueData)));
	delete [] pOpaqueData;

	pFile->Close();
}

extern "C" HRESULT CAAFTypeDefOpaque_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTypeDefOpaque_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	const size_t fileNameBufLen = 128;
	aafWChar pTestFilename[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pTestFilename );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			CreateTypeDefOpaqueFile(pTestFilename, fileKind, rawStorageType, productID);
		ReadTypeDefOpaqueFile(pTestFilename);
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}








