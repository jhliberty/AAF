//=---------------------------------------------------------------------=
//
// $Id: CAAFAIFCDescriptorTest.cpp,v 1.38 2009/06/01 11:47:11 stuart_hc Exp $ $Name: V116 $
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



#undef WIN32_LEAN_AND_MEAN


#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


#define FORM_HDR_SIZE		12
#define COMM_CHUNK_SIZE		46
#define SSND_CHUNK_SIZE		16
#define SUMMARY_SIZE		FORM_HDR_SIZE+COMM_CHUNK_SIZE+SSND_CHUNK_SIZE

//typedef struct tAIFCSUMMARY
//{
//	FormAIFCChunk	formChunk;		// size 12
//	CommonChunk		commChunk;
//	SoundDataChunk	ssndChunk;
//} AIFCSummary;

// our test Mob id 
static const aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x1f64f50a, 0x03fd, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

// Simple utilities to swap bytes.
static void SwapBytes(void *buffer, size_t count)
{
  unsigned char *pBuffer = (unsigned char *)buffer;
  unsigned char tmp;
  int front = 0;
  int back = count - 1;

  for (front = 0, back = count - 1; front < back; ++front, --back)
  {
    tmp = pBuffer[front];
    pBuffer[front] = pBuffer[back];
    pBuffer[back] = tmp;
  }
}

// The AIFC header is stored on disk in big endian byte order
// so provide some macros to swap byte order when necessary
#define BE_WRITE_LONG(bo, ptr, val) { memcpy(ptr, val, 4); if (bo == kAAFByteOrderLittle) SwapBytes(ptr,4); ptr += 4; }
#define BE_WRITE_SHORT(bo, ptr, val) { memcpy(ptr, val, 2); if (bo == kAAFByteOrderLittle) SwapBytes(ptr,2); ptr += 2; }
#define BE_WRITE_CHARS(bo, ptr, val, len) { memcpy(ptr, val, len); ptr += len; }
#define BE_READ_LONG(bo, ptr, val) { memcpy(val, ptr, 4); if (bo == kAAFByteOrderLittle) SwapBytes(val,4); ptr += 4; }
#define BE_READ_SHORT(bo, ptr, val) { memcpy(val, ptr, 2); if (bo == kAAFByteOrderLittle) SwapBytes(val,2); ptr += 2; }
#define BE_READ_CHARS(bo, ptr, val, len) { memcpy(val, ptr, len); ptr += len; }


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
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IAAFDictionary*			pDictionary = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFMob*				pMob = NULL;
	IAAFAIFCDescriptor*		pAIFCDesc = NULL;
	IAAFEssenceDescriptor*	pEssDesc = NULL;

	HRESULT			hr = AAFRESULT_SUCCESS;


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
 		
		// Create a source mob
		CAAFBuiltinDefs defs (pDictionary);
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));

		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));

		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AIFCDescriptorTest"));
		checkResult(defs.cdAIFCDescriptor()->
					CreateInstance(IID_IAAFAIFCDescriptor, 
								   (IUnknown **)&pAIFCDesc));		

		// Get Endianness so we can store the corrent binary sequence to disk
		IAAFEndian* pEndian = NULL;
		checkResult(pHeader->QueryInterface(IID_IAAFEndian, (void **)&pEndian));
		eAAFByteOrder_t byteorder;
		checkResult(pEndian->GetNativeByteOrder(&byteorder));
		pEndian->Release();
		pEndian = NULL;

//		AIFCSummary summary;
		unsigned char	writeBuf[SUMMARY_SIZE], *writePtr;
		aafUInt32		longVal, lZero = 0, n;
		aafUInt16		shortVal;
		static const char *compressionName = "Not compressed";
		// Form Header
		// typedef struct tAIFCFORMATCHUNK
		// {
		//		char	ckID[4];		// "FORM"
		// 		long	ckDataSize;
		// 		char	formType[4];	// "AIFC"
		// } FormAIFCChunk;
		writePtr = writeBuf;
		BE_WRITE_CHARS(byteorder, writePtr, "FORM", 4);
		BE_WRITE_CHARS(byteorder, writePtr, "AIFC", 4);
		longVal = sizeof(writeBuf) - 8;
		BE_WRITE_LONG(byteorder, writePtr, &longVal);

		// COMM Chunk
		// typedef struct tCommonChunk
		// {
		// 		char			ckID[4];		// "COMM"
		// 		long			ckDataSize;
		// 		short			numChannels;
		// 		unsigned long	numSampleFrames;
		// 		short			sampleSize;
		// 		unsigned char	sampleRate[10];	// This should be IEEE floating point number
		// 		char			compressionType[4];
		// 		unsigned char	compNameLength;
		// 		char			compressionName[15];
		// } CommonChunk;
		BE_WRITE_CHARS(byteorder, writePtr, "COMM", 4);	// ckID
		longVal = 46;	// sizeof(CommonChunk) with no padding
		BE_WRITE_LONG(byteorder, writePtr, &longVal);		// ckDataSize
		shortVal = 1;
		BE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// numChannels
		BE_WRITE_LONG(byteorder, writePtr, &lZero);		// numSampleFrames
		shortVal = 16;
		BE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// sampleSize
		*writePtr++ = 0x40;					// SampleRate
		*writePtr++ = 0x0E;
		*writePtr++ = (char)0xac;
		*writePtr++ = 0x44;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		BE_WRITE_CHARS(byteorder, writePtr, "NONE", 4);	// CompressionType
		*writePtr++ = strlen(compressionName);
		BE_WRITE_CHARS(byteorder, writePtr, compressionName, strlen(compressionName)); // compressionName
		for(n = 15 - strlen(compressionName); n >= 1; n--)
			*writePtr++ = 0;	// Chunks must not be an odd length

		// Sound Chunk
		// typedef struct tSoundDataChunk
		// {
		// 	char			ckID[4];		// "SSND"
		// 	long			ckDataSize;
		// 	unsigned long	offset;
		// 	unsigned long	blockSize;
		// 	char			soundData;
		// } SoundDataChunk;
		BE_WRITE_CHARS(byteorder, writePtr, "SSND", 4);		// ckID
		BE_WRITE_LONG(byteorder, writePtr, &lZero);			// ckDataSize
		BE_WRITE_LONG(byteorder, writePtr, &lZero);			// offset
		BE_WRITE_LONG(byteorder, writePtr, &lZero);			// blockSize
		// Check that writePtr-writeBuf == sizeof(writeBuf);
		checkExpression(writePtr - writeBuf == sizeof(writeBuf), AAFRESULT_TEST_FAILED);

		checkResult(pAIFCDesc->SetSummary(SUMMARY_SIZE, (aafDataValue_t)writeBuf));

		checkResult(pAIFCDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));

		// Add the MOB to the file
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
  
	// Cleanup and return
	if (pEssDesc)
		pEssDesc->Release();

	if (pAIFCDesc)
		pAIFCDesc->Release();

	if (pMob)
		pMob->Release();

	if (pSourceMob)
		pSourceMob->Release();

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

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFEssenceDescriptor*	pEssDesc = NULL;
	IAAFAIFCDescriptor*		pAIFCDesc = NULL;
	aafNumSlots_t			numMobs = 0;
	HRESULT					hr = AAFRESULT_SUCCESS;


	try
	{
		// Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->GetMobs(NULL, &pMobIter));
		checkResult(pMobIter->NextOne(&pMob));
		checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
		
		// Back into testing mode
		checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
		
		// if there is an Essence Descriptor then it MUST be an (essence) AIFC Descriptor
		checkResult(pEssDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **) &pAIFCDesc));

		unsigned char summary[SUMMARY_SIZE], tmp[SUMMARY_SIZE], *readPtr;
		aafInt32		longVal;
		aafInt16		shortVal;
	    aafUInt32		size = 0;

		checkResult(pAIFCDesc->GetSummaryBufferSize(&size));
		checkExpression(size == sizeof(summary), AAFRESULT_TEST_FAILED);


		checkResult(pAIFCDesc->GetSummary(size, (aafDataValue_t)&summary));

		// Get Endianness so we can read the corrent binary sequence from disk
		IAAFEndian* pEndian = NULL;
		checkResult(pHeader->QueryInterface(IID_IAAFEndian, (void **)&pEndian));
		eAAFByteOrder_t byteorder;
		checkResult(pEndian->GetNativeByteOrder(&byteorder));
		pEndian->Release();
		pEndian = NULL;

		checkExpression((strncmp((char*)summary+FORM_HDR_SIZE, "COMM", 4) == 0) &&
						(strncmp((char*)summary, "FORM", 4) == 0) &&
						(strncmp((char*)summary+FORM_HDR_SIZE+COMM_CHUNK_SIZE, "SSND", 4) == 0),
		                AAFRESULT_TEST_FAILED);

		readPtr = summary;
		BE_READ_CHARS(byteorder, readPtr, tmp, 4);
		checkExpression(strncmp((char*)tmp, "FORM", 4) == 0, AAFRESULT_TEST_FAILED);
		BE_READ_CHARS(byteorder, readPtr, tmp, 4);
		checkExpression(strncmp((char*)tmp, "AIFC", 4) == 0, AAFRESULT_TEST_FAILED);
		BE_READ_LONG(byteorder, readPtr, &longVal);
		checkExpression((unsigned)longVal == size - 8, AAFRESULT_TEST_FAILED);
		BE_READ_CHARS(byteorder, readPtr, tmp, 4);
		checkExpression(strncmp((char*)tmp, "COMM", 4) == 0, AAFRESULT_TEST_FAILED);
		BE_READ_LONG(byteorder, readPtr, &longVal);
		checkExpression(longVal == 46, AAFRESULT_TEST_FAILED);
		BE_READ_SHORT(byteorder, readPtr, &shortVal);
		checkExpression(shortVal == 1, AAFRESULT_TEST_FAILED);
		BE_READ_LONG(byteorder, readPtr, &longVal);
		checkExpression(longVal == 0, AAFRESULT_TEST_FAILED);
		BE_READ_SHORT(byteorder, readPtr, &shortVal);
		checkExpression(shortVal == 16, AAFRESULT_TEST_FAILED);

		unsigned char floatingSampleSize[] = { 0x40, 0x0E, 0xac, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		BE_READ_CHARS(byteorder, readPtr, tmp, 10);
		checkExpression(memcmp(tmp, floatingSampleSize, sizeof(floatingSampleSize)) == 0, AAFRESULT_TEST_FAILED);
		BE_READ_CHARS(byteorder, readPtr, tmp, 4);
		checkExpression(strncmp((char*)tmp, "NONE", 4) == 0, AAFRESULT_TEST_FAILED);
		BE_READ_CHARS(byteorder, readPtr, tmp, 1);
		unsigned compNameLength = tmp[0];
		BE_READ_CHARS(byteorder, readPtr, tmp, compNameLength);
		const char *compressionName = "Not compressed";
		checkExpression(strncmp((char*)tmp, compressionName, strlen(compressionName)) == 0, AAFRESULT_TEST_FAILED);
		checkExpression(compNameLength == strlen(compressionName), AAFRESULT_TEST_FAILED);
		BE_READ_CHARS(byteorder, readPtr, tmp, (1 + compNameLength) % 2);	// discard padding to even boundary

		BE_READ_CHARS(byteorder, readPtr, tmp, 4);
		checkExpression(strncmp((char*)tmp, "SSND", 4) == 0, AAFRESULT_TEST_FAILED);
		BE_READ_LONG(byteorder, readPtr, &longVal);
		checkExpression(longVal == 0, AAFRESULT_TEST_FAILED);
		BE_READ_LONG(byteorder, readPtr, &longVal);
		checkExpression(longVal == 0, AAFRESULT_TEST_FAILED);
		BE_READ_LONG(byteorder, readPtr, &longVal);
		checkExpression(longVal == 0, AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
  
	// Cleanup and return
	if (pAIFCDesc)
		pAIFCDesc->Release();

  if (pEssDesc)
		pEssDesc->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

extern "C" HRESULT CAAFAIFCDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFAIFCDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	const size_t	fileNameBufLen = 128;
	aafWChar	pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

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
		cerr << "CAAFAIFCDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}








