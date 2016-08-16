// @com Executable example program by Stuart Cunningham, BBC R&D
// @com This is based upon ComEssenceDataTest.cpp.

//=---------------------------------------------------------------------=
//
// $Id: ScaleTest.cpp,v 1.15 2009/06/01 11:47:15 stuart_hc Exp $ $Name: V116 $
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <unistd.h>				// stat() under unix
#endif

#include <iostream>
using namespace std;

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFFileKinds.h"
#include "AAFFileMode.h"
#include "AAF.h"
#include "AAFStoredObjectIDs.h"

// Guess the toolkit version (found by experiment)
#if defined(__IAAFImportDescriptor_INTERFACE_DEFINED__)
	// V1.1 or greater toolkit
#else
#define AAF_TOOLKIT_V1_0
#endif

#ifdef AAF_TOOLKIT_V1_0
// using v1.0.x toolkit
const aafUID_t kAAFCompression_IEC_DV_525_60 = { 0x04010202, 0x0201, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };
const aafUID_t *filekind_4K = &aafFileKindAaf4KBinary;
#else
// using v1.1.x toolkit
#include "AAFCompressionDefs.h"
const aafUID_t *filekind_4K = &kAAFFileKind_Aaf4KBinary;
const aafUID_t *filekind_512 = &kAAFFileKind_Aaf512Binary;
#endif


// Provide a sample compressed DV frame
#include "../ComModTestAAF/ModuleTests/EssenceTestData.h"


#define SIZE_2GB AAFCONSTINT64(2147483648)

const aafInt32 UNC_PAL_FRAME_SIZE = 720*576*2;
const aafInt32 UNC_NTSC_FRAME_SIZE = 720*480*2;
const aafInt32 DV_PAL_FRAME_SIZE = 144000;
const aafInt32 DV_NTSC_FRAME_SIZE = 120000;

bool verbose = true;
bool deleteFiles = false;

#define aaf_assert(b, msg) \
	if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

#define aaf_simple_assert(x) aaf_assert( (x), #x );

static void LogError(HRESULT errcode, int line, const char *file)
{
	printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a) \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
	{ \
		LogError(moduleErrorTmp, __LINE__, __FILE__);\
		exit(1); \
	} \
}

#define checkFatal(a) \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
		exit(1);\
}

bool operator ==( const aafUID_t uid1, const aafUID_t uid2 )
{
    bool    are_equal = true;
    int     i = 0;

    for( i=0; i<8; i++ )
    {
        if( uid1.Data4[i] != uid2.Data4[i] )
        {
            are_equal = false;
            break;
        }
    }

    if( are_equal == true )
    {
        if( uid1.Data1 != uid2.Data1  ||
            uid1.Data2 != uid2.Data2  ||
            uid1.Data3 != uid2.Data3 )
        {
            are_equal = false;
        }
    }

    return are_equal;
}

static void convert(char* cName, size_t length, const wchar_t* name)
{
	aaf_assert((name && *name), "Valid input name");
	aaf_assert(cName != 0, "Valid output buffer");
	aaf_assert(length > 0, "Valid output buffer size");

	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
		fprintf(stderr, ": Error : Conversion failed.\n\n");
		exit(1);
	}
}

static aafLength_t getFilesize(const char *file)
{
	// For Large File Support (>2GB) we must use the awkward stat() and
	// _stati64() calls below.  C++ standard library ifstream seekg and tellg
	// aren't 64bit capable using GCC 3.3 and MSVC 7 (GCC 3.4 fixes this).
    int rc;
#ifdef _MSC_VER
	struct _stati64 statbuf;
	rc = _stati64(file, &statbuf);
#else
	struct stat statbuf;
	rc = stat(file, &statbuf);
#endif
	if ( rc != 0 )
	{
	  perror("stat:");
	  return -1;
	}

	return statbuf.st_size;
}

// Generate a video buffer containing uncompressed UYVY video representing
// the familiar colour bars test signal (or YUY2 video if specified).
static void create_colour_bars(unsigned char *video_buffer, bool convert_to_YUY2)
{
	int		i,j,b;
	int		UYVY_table[][5] = {
				{ 52, 0x80,0xEB,0x80,0xEB },	// white
				{ 140, 0x10,0xD2,0x92,0xD2 },	// yellow
				{ 228, 0xA5,0xA9,0x10,0xA9 },	// cyan
				{ 316, 0x35,0x90,0x22,0x90 },	// green
				{ 404, 0xCA,0x6A,0xDD,0x6A },	// magenta
				{ 492, 0x5A,0x51,0xF0,0x51 },	// red
				{ 580, 0xf0,0x29,0x6d,0x29 },	// blue
				{ 668, 0x80,0x10,0x80,0x10 },	// black
				{ 720, 0x80,0xEB,0x80,0xEB },	// white
			};

	for (j = 0; j < 576; j++)
	{
		for (i = 0; i < 720*2; i+=4)
		{
			for (b = 0; b < 9; b++)
			{
				if (i < UYVY_table[b][0] * 2)
				{
					video_buffer[j*720*2 + i + 0] = UYVY_table[b][1];
					video_buffer[j*720*2 + i + 1] = UYVY_table[b][2];
					video_buffer[j*720*2 + i + 2] = UYVY_table[b][3];
					video_buffer[j*720*2 + i + 3] = UYVY_table[b][4];
					break;
				}
			}
		}
	}

	if (convert_to_YUY2)
	{
		// Convert from UYVY -> YUY2
		for (i = 0; i < 720 * 576 * 2; i+=2)
		{
			unsigned char tmp = video_buffer[i+1];
			video_buffer[i+1] = video_buffer[i];
			video_buffer[i] = tmp;
		}
	}
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static const   aafMobID_t  TEST_MobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc302, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};
static const   aafMobID_t  TEST_SourceMobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc402, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ScaleTest";
static aafProductVersion_t TestVersion = { 1, 1, 0, 0, kAAFVersionUnknown };
static aafProductIdentification_t TestProductID;

static HRESULT CreateAAFFileEssenceData(const aafWChar *pFileName, bool useRawStorage,
									const aafUID_t *filekind,
									aafLength_t frame_limit,
									aafLength_t *p_totalbytes)
{
	TestProductID.companyName = companyName;
	TestProductID.productName = productName;
	TestProductID.productVersionString = NULL;
	TestProductID.productID = UnitTestProductID;
	TestProductID.platform = NULL;
	TestProductID.productVersion = &TestVersion;

	HRESULT			hr = S_OK;

	try
	{
		// Delete any previous test file before continuing...
		char cFileName[FILENAME_MAX];
		convert(cFileName, sizeof(cFileName), pFileName);
		remove(cFileName);

		// Open new file
		IAAFFile		*pFile = NULL;

		// AAFFileOpenNewModifyEx() uses RawStorage internally while
		// AAFFileOpenNewModify() does not.  Since libgsf doesn't yet
		// support RawStorage, AAFFileOpenNewModifyEx() will fail with libgsf
		if (useRawStorage)
		{
			printf("Creating %s using AAFFileOpenNewModifyEx()\n", cFileName);

			check( AAFFileOpenNewModifyEx(
						pFileName,
						filekind,
						0,
						&TestProductID,
						&pFile) );
		}
		else
		{
			printf("Creating %s using AAFFileOpenNewModify()\n", cFileName);

			// Large sectors for new files
			int modeFlags = AAF_FILE_MODE_USE_LARGE_SS_SECTORS;

			check( AAFFileOpenNewModify(
						pFileName,
						modeFlags,
						&TestProductID,
						&pFile) );
		}

		// Get the header & dictionary
		IAAFHeader		*pHeader = NULL;
		IAAFDictionary	*pDictionary = NULL;
		check(pFile->GetHeader(&pHeader));
		check(pHeader->GetDictionary(&pDictionary));

		// Create a MasterMob
		IAAFClassDef	*classDef = NULL;
		IAAFMob			*pMob = NULL;
		check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &classDef));
		check(classDef->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
		classDef->Release();
		check(pMob->SetMobID(TEST_MobID));
		check(pMob->SetName(L"CreateAAFFile - MasterMob"));
		check(pHeader->AddMob(pMob));
		pMob->Release();

		// Create a SourceMob 
		IAAFSourceMob			*pSourceMob = NULL;
		check(pDictionary->LookupClassDef(AUID_AAFSourceMob, &classDef));
		check(classDef->CreateInstance(IID_IAAFSourceMob, (IUnknown **)&pSourceMob));
		classDef->Release();
		check(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		check(pMob->SetMobID(TEST_SourceMobID));
		check(pMob->SetName(L"CreateAAFFile - SourceMob"));
		check(pHeader->AddMob(pMob));

		// Add an EssenceDescriptor to the SourceMob
		IAAFEssenceDescriptor		*edesc = NULL;
		IAAFFileDescriptor			*pFileDesc = NULL;
		IAAFCDCIDescriptor			*pCDCIDesc = NULL;
		IAAFDigitalImageDescriptor	*pDIDesc = NULL;
		check(pDictionary->LookupClassDef(AUID_AAFCDCIDescriptor, &classDef));
		check(classDef->CreateInstance(IID_IAAFEssenceDescriptor, (IUnknown **)&edesc));
		classDef->Release();
		check(edesc->QueryInterface(IID_IAAFFileDescriptor, (void **)&pFileDesc));
		check(edesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDIDesc));
		check(edesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pCDCIDesc));
		check(pFileDesc->SetLength(frame_limit));
		check(pDIDesc->SetCompression(kAAFCompressionDef_IEC_DV_525_60));
		check(pDIDesc->SetStoredView(576/2, 720));
		check(pDIDesc->SetFrameLayout(kAAFSeparateFields));
		check(pCDCIDesc->SetHorizontalSubsampling(2));
		check(pCDCIDesc->SetVerticalSubsampling(2));
		check(pSourceMob->SetEssenceDescriptor(edesc));
		pDIDesc->Release();
		pCDCIDesc->Release();
		pFileDesc->Release();
		edesc->Release();

		// Add an EssenceData object containing DV frames
		IAAFEssenceData			*pEssenceData = NULL;
		aafUInt32				bytesWritten = 0;
		check(pDictionary->LookupClassDef(AUID_AAFEssenceData, &classDef));
		check(classDef->CreateInstance(IID_IAAFEssenceData, (IUnknown **)&pEssenceData));
		classDef->Release();
		check(pEssenceData->SetFileMob(pSourceMob));
		check(pHeader->AddEssenceData(pEssenceData));

		// Setup compressed DV frame which will contain a frame counter
		unsigned char dv_buf[sizeof(compressedDV_25_625)];
		memcpy(dv_buf, compressedDV_25_625, sizeof(compressedDV_25_625));

		*p_totalbytes = 0;
		for (aafLength_t i = 0; i < frame_limit; i++)
		{
			// write a frame counter into an unused part of the DV frame metadata
			memcpy(&dv_buf[16], &i, sizeof(i));

			check(pEssenceData->Write(
									sizeof(compressedDV_25_625),
									(aafUInt8*)dv_buf,
									&bytesWritten));

			aaf_assert(bytesWritten == sizeof(compressedDV_25_625), "pEssenceData->Write() wrote correct size");
			*p_totalbytes += bytesWritten;

			if (verbose && (i+1) % 25 == 0)
			{
				printf("  %6"AAFFMT64"d frames %6.2f%% %"AAFFMT64"d bytes\r", i+1, (i+1) * 100.0 / frame_limit, *p_totalbytes);
				fflush(stdout);
			}
		}
		if (verbose)
			printf("  %6"AAFFMT64"d frames %6.2f%%  bytes=%"AAFFMT64"d\n",
					frame_limit, frame_limit * 100.0 / frame_limit, *p_totalbytes);
		pEssenceData->Release();

		pSourceMob->Release();
		pMob->Release();

		pDictionary->Release();
		pHeader->Release();

		// Save & close the file
		check(pFile->Save());
		check(pFile->Close());
		check(pFile->Release());

		aafLength_t filesize = getFilesize(cFileName);
		aafLength_t diff = filesize - SIZE_2GB;
		printf("  Wrote %s (EssenceData API) filesize=%"AAFFMT64"d (2GB %s%"AAFFMT64"d)\n",
					cFileName, filesize,
					(diff >= 0) ? "+" : "", diff);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** CreateAAFFileEssenceData: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

static HRESULT ReadAAFFileEssenceData(const aafWChar *pFileName, aafLength_t frame_limit, aafLength_t bytes)
{
	HRESULT			hr = S_OK;

	try
	{
		// Open new file
		IAAFFile		*pFile = NULL;

		hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);
		if (hr == S_OK)
		{
			printf("  FileOpenExistingRead for %ls succeeded, reading using EssenceData\n", pFileName);
		}
		else
		{
			printf("  FileOpenExistingRead for %ls failed hr=0x%08x\n", pFileName, hr);
		}

		// Check that size of essence data stream is what we expect
		IAAFHeader				*pHeader = NULL;
		aafUInt32				essenceDataCount = 0;
		IEnumAAFEssenceData		*pEnumEssenceData;
		IAAFEssenceData			*pEssenceData;

		aaf_simple_assert(pFile);
		check(pFile->GetHeader(&pHeader));
		aaf_simple_assert(pHeader);
		check(pHeader->CountEssenceData(&essenceDataCount));
		check(pHeader->EnumEssenceData(&pEnumEssenceData));
		aaf_simple_assert(pEnumEssenceData);
		for (unsigned i = 0; i < essenceDataCount; i++)
		{
			aafLength_t		size = 0;
			check(pEnumEssenceData->NextOne(&pEssenceData));
			aaf_simple_assert(pEssenceData)
			check(pEssenceData->GetSize(&size));
			aaf_assert(size == bytes, "GetSize() matches total written bytes");

			printf("    EssenceData GetSize() correct    (%"AAFFMT64"d bytes)\n", size);

			// Only test the frame counter when it was written into the DV frame.
			// Work out whether this EssenceData was written using a codec since
			// the codec will not put a frame counter into the DV frame.
			bool testFrameCounter = true;
			bool testDVFrame = true;
			IAAFSourceMob *pSourceMob = NULL;
			IAAFMob *pMob = NULL;
			IEnumAAFMobSlots *pMobSlotIter = NULL;
			IAAFMobSlot *pMobSlot = NULL;
			IAAFDataDef *pDataDef = NULL;
			aafBool bIsPictureKind = kAAFTrue;
			aafBool bIsSoundKind = kAAFFalse;
			check(pEssenceData->GetFileMob(&pSourceMob));
			check(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
			check(pMob->GetSlots(&pMobSlotIter));
			if (pMobSlotIter->NextOne(&pMobSlot) == AAFRESULT_SUCCESS) {
				check(pMobSlot->GetDataDef(&pDataDef));
				check(pDataDef->IsPictureKind(&bIsPictureKind));
				check(pDataDef->IsSoundKind(&bIsSoundKind));
			}
			aafWChar mobname[256];
			if (pMob->GetName(mobname, sizeof(mobname)) == AAFRESULT_PROP_NOT_PRESENT)
				// SourceMob was created by EssenceAccess API - don't check DV frame counter
				testFrameCounter = false;
			if (bIsSoundKind)
				testDVFrame = false;

			aafLength_t			totalRead = 0;
			aafUInt8			buf[sizeof(compressedDV_25_625)];	// size of one DV frame

			// Setup compressed DV frame for checking data read is correct
			unsigned char dv_buf[sizeof(compressedDV_25_625)];
			memcpy(dv_buf, compressedDV_25_625, sizeof(compressedDV_25_625));
			aafLength_t frame_counter = 0;

			while (true)
			{
				aafUInt32	bytesRead = 0;

				hr = pEssenceData->Read(sizeof(buf), buf, &bytesRead);
				totalRead += bytesRead;

				if (verbose && (frame_counter+1) % 25 == 0)
				{
					printf("  %6"AAFFMT64"d frames %6.2f%% %"AAFFMT64"d bytes\r", i+frame_counter, (i+frame_counter) * 100.0 / frame_limit, totalRead);
					fflush(stdout);
				}

				if (AAFRESULT_FAILED(hr))
				{
					aaf_assert(hr == AAFRESULT_END_OF_DATA, "END_OF_DATA found as expected");
					hr = S_OK;
					break;
				}

				if (testFrameCounter)
				{
					// set up comparision frame containing frame counter
					memcpy(&dv_buf[16], &frame_counter, sizeof(frame_counter));
					frame_counter++;
				}

				if (testDVFrame)
					aaf_assert(memcmp(buf, dv_buf, sizeof(compressedDV_25_625)) == 0,
							"pEssenceData->Read() off-disk frame identical to computed frame");
			}
			aaf_assert(totalRead == bytes, "total Read() matches total written bytes");

			printf("    EssenceData total Read() correct (%"AAFFMT64"d bytes)\n", totalRead);

			check(pMob->Release());
			check(pSourceMob->Release());
			check(pMobSlotIter->Release());
			if (pMobSlot)
				check(pMobSlot->Release());
			if (pDataDef)
				check(pDataDef->Release());

			check(pEssenceData->Release());
		}
		check(pEnumEssenceData->Release());
		check(pHeader->Release());

		check(pFile->Close());
		check(pFile->Release());
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** ReadAAFFileEssenceData: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

static void deleteTestFile(const aafWChar *pFileName)
{
	if (! deleteFiles)
		return;

	char cFileName[FILENAME_MAX];
	convert(cFileName, sizeof(cFileName), pFileName);
	remove(cFileName);
}

static HRESULT ReadAAFFileCodec(const aafWChar *pFileName, aafLength_t frame_limit, aafLength_t bytes)
{
	HRESULT			hr = S_OK;

	try
	{
		// Open new file
		IAAFFile		*pFile = NULL;

		hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);
		if (hr == S_OK)
		{
			printf("  FileOpenExistingRead for %ls succeeded, reading using EssenceAccess\n", pFileName);
		}
		else
		{
			printf("  FileOpenExistingRead for %ls failed hr=0x%08x\n", pFileName, hr);
		}

		// Read the samples using the EssenceAccess Codec API
		IAAFHeader				*pHeader = NULL;
		IAAFDataDef				*pPictureDef = NULL, *pSoundDef = NULL;
		IAAFDictionary*			pDictionary = NULL;
		IAAFMob*				pMob = NULL;
		IAAFMobSlot*			pMobSlot = NULL;
		aafSearchCrit_t			criteria;
		IEnumAAFMobs*			pMobIter = NULL;

		aaf_simple_assert(pFile);
		check(pFile->GetHeader(&pHeader));
		aaf_simple_assert(pHeader);
		check(pHeader->GetDictionary(&pDictionary));
		check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));
		check(pDictionary->LookupDataDef(kAAFDataDef_Sound, &pSoundDef));

		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFMasterMob;
		check(pHeader->GetMobs(&criteria, &pMobIter));

		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			aafWChar mobname[256];
			check(pMob->GetName(mobname, sizeof(mobname)));

			IEnumAAFMobSlots* pMobSlotIter = NULL;
			check(pMob->GetSlots(&pMobSlotIter));
			while(AAFRESULT_SUCCESS == pMobSlotIter->NextOne(&pMobSlot))
			{
				IAAFEssenceAccess*			pEssenceAccess = NULL;
				IAAFMasterMob*			pMasterMob = NULL;
				aafUInt32 MobSlotID;
				IAAFDataDef *pDataDef = NULL;
				IAAFTimelineMobSlot* pTimelineMobSlot = NULL;
				HRESULT hr;

				// Confirm this Mob has a timeline mob slot
				hr = pMobSlot->QueryInterface(IID_IAAFTimelineMobSlot,(void **) &pTimelineMobSlot);

				if (FAILED(hr))
					continue;

				check(pMobSlot->GetDataDef(&pDataDef));
				aafBool bIsPictureKind = kAAFFalse;
				aafBool bIsSoundKind = kAAFFalse;
				check(pDataDef->IsPictureKind(&bIsPictureKind));
				check(pDataDef->IsSoundKind(&bIsSoundKind));

				check(pMobSlot->GetSlotID(&MobSlotID));
				check(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));
				check(pMasterMob->OpenEssence(MobSlotID,
											 	  NULL,	
												  kAAFMediaOpenReadOnly,
												  kAAFCompressionDisable, 
												  &pEssenceAccess));

				IAAFEssenceFormat		*fmtTemplate =  NULL;
				IAAFEssenceFormat*			pFormat = NULL;
				check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
				aafRational_t		sampleRate;
				aafUInt32			maxSampleSize;
				aafRect_t			storedRect;
				aafFrameLayout_t	frameLayout;
				aafInt32			fmtBytesRead;
				aafUInt32			samples_to_read = 0;

				if (bIsPictureKind)
				{
					check(fmtTemplate->AddFormatSpecifier(kAAFSampleRate, 0, NULL));
					check(fmtTemplate->AddFormatSpecifier(kAAFMaxSampleBytes, 0, NULL));
					check(fmtTemplate->AddFormatSpecifier(kAAFStoredRect, 0, NULL));
					check(fmtTemplate->AddFormatSpecifier(kAAFFrameLayout, 0, NULL));
					check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
					check(pFormat->GetFormatSpecifier(kAAFSampleRate, sizeof(sampleRate), (aafDataBuffer_t)&sampleRate, &fmtBytesRead));
					check(pFormat->GetFormatSpecifier(kAAFMaxSampleBytes, sizeof(maxSampleSize), (aafDataBuffer_t)&maxSampleSize, &fmtBytesRead));
					check(pFormat->GetFormatSpecifier(kAAFStoredRect, sizeof(storedRect), (aafDataBuffer_t)&storedRect, &fmtBytesRead));
					check(pFormat->GetFormatSpecifier(kAAFFrameLayout, sizeof(frameLayout), (aafDataBuffer_t)&frameLayout, &fmtBytesRead));
					samples_to_read = 1;
				}
				else if (bIsSoundKind)
				{
					check(fmtTemplate->AddFormatSpecifier(kAAFSampleRate, 0, NULL));
					check(fmtTemplate->AddFormatSpecifier(kAAFMaxSampleBytes, 0, NULL));
					check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
					check(pFormat->GetFormatSpecifier(kAAFSampleRate, sizeof(sampleRate), (aafDataBuffer_t)&sampleRate, &fmtBytesRead));
					check(pFormat->GetFormatSpecifier(kAAFMaxSampleBytes, sizeof(maxSampleSize), (aafDataBuffer_t)&maxSampleSize, &fmtBytesRead));
					samples_to_read = 4000;
				}

				aafUInt8 buf[UNC_PAL_FRAME_SIZE];
				aafLength_t total_samples = 0, total_bytes = 0;
				while (true)
				{
					aafUInt32	samplesRead, actualBytesRead;
					HRESULT hr;

					hr = pEssenceAccess->ReadSamples(
											samples_to_read, 	// number of samples to read
											sizeof(buf),		// maximum buffer size
											buf,				// output buffer for audio data
											&samplesRead,		// number of samples read
											&actualBytesRead);
					total_samples += samplesRead;
					total_bytes += actualBytesRead;

					if (hr == AAFRESULT_EOF)
						break;
					else
						check(hr);
				}
				aaf_assert(total_bytes == bytes, "total Read() matches total written bytes");

				printf("  EssenceAccess total Read() correct (%"AAFFMT64"d bytes, %"AAFFMT64"d samples)\n", total_bytes, total_samples);

				check(fmtTemplate->Release());
				check(pFormat->Release());
				check(pTimelineMobSlot->Release());
				check(pMasterMob->Release());
				check(pEssenceAccess->Release());
				check(pDataDef->Release());
				check(pMobSlot->Release());
			}
			check(pMob->Release());
		}
		check(pMobIter->Release());
		check(pDictionary->Release());
		check(pHeader->Release());

		check(pFile->Close());
		check(pFile->Release());
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** ReadAAFFileEssenceData: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

#ifndef AAF_TOOLKIT_V1_0
static HRESULT CreateAAFFileCodec(const aafWChar * pFileName, bool useRawStorage,
								const aafUID_t *filekind,
								bool comp_enable,
								aafUID_t codec, aafUID_t container,
								aafLength_t size_limit, aafLength_t *p_totalbytes)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	aafMobID_t					masterMobID;
	aafRational_t				pictureEditRate = {25, 1}, pictureSampleRate = {25, 1};
	aafRational_t				soundEditRate = {25, 1}, soundSampleRate = {48000, 1};
	aafRational_t				editRate, sampleRate;
	IAAFClassDef				*pCDMasterMob = NULL;
	IAAFDataDef					*pPictureDef = NULL, *pSoundDef;
	IAAFDataDef					*media_kind = NULL;
	aafUInt32					samplesWritten, bytesWritten;
	aafInt32					frameSize = 0, samplesPerWrite = 0;

	// Delete any previous test file before continuing...
	char cFileName[FILENAME_MAX];
	convert(cFileName, sizeof(cFileName), pFileName);
	remove(cFileName);

	aafProductVersion_t ver = {1, 0, 0, 0, kAAFVersionBeta};
	TestProductID.companyName = companyName;
	TestProductID.productName = productName;
	TestProductID.productVersion = &ver;
	TestProductID.productVersionString = NULL;
	TestProductID.productID = NIL_UID;
	TestProductID.platform = NULL;		// Set by SDK when saving

	// AAFFileOpenNewModifyEx() uses RawStorage internally while
	// AAFFileOpenNewModify() does not.  Since libgsf doesn't yet
	// support RawStorage, AAFFileOpenNewModifyEx() will fail with libgsf
	if (useRawStorage)
	{
		printf("Creating %s using CreateEssence(%s) via AAFFileOpenNewModifyEx()\n", cFileName,
					(comp_enable ? "CompressionEnable" : "CompressionDisable"));

		check( AAFFileOpenNewModifyEx(
					pFileName,
					filekind,
					0,
					&TestProductID,
					&pFile) );
	}
	else
	{
		printf("Creating %s using CreateEssence(%s) via AAFFileOpenNewModify()\n", cFileName,
					(comp_enable ? "CompressionEnable" : "CompressionDisable"));

		// Large sectors for new files
		int modeFlags = AAF_FILE_MODE_USE_LARGE_SS_SECTORS;

		check( AAFFileOpenNewModify(
					pFileName,
					modeFlags,
					&TestProductID,
					&pFile) );
	}

	check(pFile->GetHeader(&pHeader));

	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup class definitions for the objects we want to create. */
	check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &pCDMasterMob));

	/* Lookup any necessary data definitions. */
	check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));
	check(pDictionary->LookupDataDef(kAAFDataDef_Sound, &pSoundDef));

	/* Create a Mastermob */

	// Get a Master MOB Interface
	check(pCDMasterMob->CreateInstance(IID_IAAFMasterMob, (IUnknown **)&pMasterMob));

	// Get a Mob interface and set its variables.
	check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	check(pMob->GetMobID(&masterMobID));

	unsigned char video_buf[UNC_PAL_FRAME_SIZE];
	if (comp_enable)
	{
		// Create a frame of colour bars
		// experiment shows 4 bytes per pixel needed to avoid SMALLBUF using WriteSamples
		// with kAAFMixedFields and 720x576
		// 2 bytes per pixel work fine with kAAFSeparateFields and 720x288
		create_colour_bars(video_buf, true);
		check(pMob->SetName(L"colour_bars"));
	}
	else
	{
		check(pMob->SetName(L"flower"));
	}

	// Add Mobs to the Header
	check(pHeader->AddMob(pMob));

	// Get a pointer to samples data for WriteSamples
	unsigned char *dataPtr;
	dataPtr = video_buf;

	// Prepare parameters based on codec selected
	if (codec == kAAFCodecDef_PCM)
	{
		comp_enable = false;		// No compression engine (yet) for PCM codec
		media_kind = pSoundDef;
		editRate = soundEditRate;
		sampleRate = soundSampleRate;

		samplesPerWrite = 1920;		// 1920 samples per video frame (48000/25)
		frameSize = 1920;			// 1 channel at 8 bits per sample
	}
	else if (codec == kAAFCodecDef_CDCI)
	{
		media_kind = pPictureDef;
		editRate = pictureEditRate;
		sampleRate = pictureSampleRate;

		samplesPerWrite = 1;		// write one video frame at a time
		frameSize = (comp_enable ? UNC_PAL_FRAME_SIZE : DV_PAL_FRAME_SIZE);
	}
	else if (codec == kAAFCodecDef_WAVE)
	{
		media_kind = pSoundDef;
		editRate = soundEditRate;
		sampleRate = soundSampleRate;

		samplesPerWrite = 1920;
		frameSize = 1920;
		dataPtr = (unsigned char*)uncompressedWAVE_Laser + 44;	// audio samples offset by WAVE header
	}
	else
	{
		aaf_assert(0, "codec choice not implemented");
	}

	/* Create the Essence Data specifying the codec, container, edit rate and sample rate */
	check(pMasterMob->CreateEssence(1,			// Slot ID within MasterMob
						media_kind,				// MediaKind
						codec,					// E.g. CDCI, WAVE, PCM
						editRate,				// edit rate
						sampleRate,				// sample rate
						comp_enable ? kAAFCompressionEnable : kAAFCompressionDisable,
						NULL,					// No Locator used
						container,				// Essence embedded or external?
						&pEssenceAccess));		//

	// Set the codec flavour for desired video format
	if (codec == kAAFCodecDef_CDCI)
	{
		pEssenceAccess->SetEssenceCodecFlavour( kAAFCodecFlavour_IEC_DV_625_50 );
	}

	// For fun, print the name of the selected codec flavour
	aafWChar codec_name[128] = L"";
	check(pEssenceAccess->GetCodecName(sizeof(codec_name), codec_name));
	printf("  using codec flavour \"%ls\"\n", codec_name);

	// Write the video samples
	int total_samples = 0;
	*p_totalbytes = 0;

	if (comp_enable)				// using generated uncompressed video?
	{
		while (*p_totalbytes < size_limit)
		{
			check(pEssenceAccess->WriteSamples(	1,					//
												sizeof(video_buf),	// buffer size
												dataPtr,			// pointer to video frame
												&samplesWritten,
												&bytesWritten));
			total_samples += samplesWritten;
			*p_totalbytes += bytesWritten;
		}
	}
	else
	{
		// using pre-compressed DV frames, uncompressed PCM etc
		memcpy(video_buf, compressedDV_25_625, sizeof(compressedDV_25_625));

		while (*p_totalbytes < size_limit)
		{
			check(pEssenceAccess->WriteSamples(
									samplesPerWrite,	// number of samples
									frameSize,			// buffer size
									dataPtr,			// samples buffer
									&samplesWritten,
									&bytesWritten));
			total_samples += samplesWritten;
			*p_totalbytes += bytesWritten;
			if (verbose && (*p_totalbytes) % 1024*1024 == 0)
			{
				printf("  %12"AAFFMT64"d bytes %6.2f%%\r", *p_totalbytes, (*p_totalbytes) * 100.0 / size_limit);
				fflush(stdout);
			}
		}
	}
	if (verbose)
		printf("  %12"AAFFMT64"d bytes %6.2f%%\n", *p_totalbytes, (*p_totalbytes) * 100.0 / size_limit);

	/* Set the essence to indicate that you have finished writing the samples */
	check(pEssenceAccess->CompleteWrite());

	pEssenceAccess->Release();

	pMob->Release();
	pMasterMob->Release();

	pPictureDef->Release();
	pSoundDef->Release();
	pCDMasterMob->Release();

	pDictionary->Release();
	pHeader->Release();

	/* Save the AAF file */
	pFile->Save();

	/* Close the AAF file */
	pFile->Close();
	pFile->Release();

	aafLength_t filesize = getFilesize(cFileName);
	aafLength_t diff = filesize - SIZE_2GB;
	printf("  Wrote %s (EssenceAccess API) filesize=%"AAFFMT64"d (2GB %s%"AAFFMT64"d)\n",
				cFileName, filesize,
				(diff >= 0) ? "+" : "", diff);

	return moduleErrorTmp;
}
#endif

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
	CAAFInitialize(const char *dllname = NULL)
	{
		HRESULT hr = AAFLoad(dllname);
		if (!AAFRESULT_SUCCEEDED(hr)) {
			fprintf(stderr, "Error : Failed to load the AAF library, ");
			fprintf(stderr, "check environment variables -\n");
			fprintf(stderr, "	Windows		- $PATH\n");
			fprintf(stderr, "	Unix/Linux	- $LD_LIBRARY_PATH\n");
			exit(hr);
		}
	}

	~CAAFInitialize()
	{
		AAFUnload();
	}
};


// Make sure all of our required plugins have been registered.
static HRESULT RegisterRequiredPlugins(void)
{
	IAAFPluginManager	*mgr = NULL;

	// Load the plugin manager
	check(AAFGetPluginManager(&mgr));

	// Attempt load and register all of the plugins
	// in the shared plugin directory.
	check(mgr->RegisterSharedPlugins());

	if (mgr)
		mgr->Release();

	return moduleErrorTmp;
}

void printUsage(const char *progname)
{
	cout << "Usage : " << progname << " [-q] [-c] [-n frames]" << endl;
	cout << endl;
	cout << "\tWith no arguments creates TestEssData.aaf containing ~10 minutes of DV frames" << endl;
	cout << endl;
	cout << "\t-n frames      number of DV frames to write [default 18611 (about 2.5GiB)]" << endl;
	cout << "\t               30000 DV frames is > 4GiB" << endl;
	cout << "\t-c             Test the codecs only (not EssenceData API)" << endl;
	cout << "\t-d             Delete test files upon completion" << endl;
	cout << "\t-f <filekind>  filekind for RawStorage API (filekind_512, filekind_4K (default))" << endl;
	cout << "\t-compress      Test using uncompressed video passing through codec's compressor" << endl;
	cout << "\t-q             Quiet (less verbose)" << endl;
}

extern int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	bool			compressionEnable = false;	// use compressed frames by default
	bool			testEssenceData = true;
	bool			testCodecs = false;
	bool			testRawStorage = true;
	const aafUID_t *filekind = filekind_4K;
	aafLength_t		frame_limit = 18611;		// about 2.5GiB

	int i = 1;
	if (argc > 1)
	{
		while (i < argc)
		{
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			{
				printUsage(argv[0]);
				return 0;
			}
			else if (!strcmp(argv[i], "-n"))
			{
				frame_limit = strtol(argv[i+1], NULL, 10);
				if (frame_limit < 1)
				{
					printUsage(argv[0]);
					return 1;
				}
				i+=2;
			}
			else if (!strcmp(argv[i], "-f"))
			{
				if (strcmp(argv[i+1], "filekind_512") == 0)
					filekind = filekind_512;
				if (strcmp(argv[i+1], "filekind_4K") == 0)
					filekind = filekind_4K;
				i+=2;
			}
			else if (!strcmp(argv[i], "-q"))
			{
				verbose = false;
				i++;
			}
			else if (!strcmp(argv[i], "-c"))
			{
				testEssenceData = false;
				testCodecs = true;
				i++;
			}
			else if (!strcmp(argv[i], "-d"))
			{
				deleteFiles = true;
				i++;
			}
#ifndef AAF_TOOLKIT_V1_0
			else if (!strcmp(argv[i], "-compress"))
			{
				compressionEnable = true;
				i++;
			}
#endif
			else
			{
				break;
			}
		}
	}

	aafLength_t			size_limit = frame_limit * DV_PAL_FRAME_SIZE;

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	if (testEssenceData)
	{
		const aafWChar *		pwFileName	= L"TestEssData.aaf";
		aafLength_t bytes = 0;

		checkFatal(CreateAAFFileEssenceData(pwFileName, false, filekind, frame_limit, &bytes));
		checkFatal(ReadAAFFileEssenceData(pwFileName, frame_limit, bytes));

		if (testRawStorage)
		{
			checkFatal(CreateAAFFileEssenceData(pwFileName, true, filekind, frame_limit, &bytes));
			checkFatal(ReadAAFFileEssenceData(pwFileName, frame_limit, bytes));
		}
	}

#ifndef AAF_TOOLKIT_V1_0
	// Codec tests are not run under v1.0 toolkit due to 2GB limits in codecs
	if (testCodecs)
	{
		// 2005-03-21
		// Codecs that should support > 2GB files
		//	CDCI codec
		//		internal ContainerAAF
		//		external ContainerAAF, ContainerFile
		//	PCM codec
		//		internal ContainerAAF
		//		external ContainerAAF, ContainerFile
		//		(note internal/external ContainerRIFFWAVE must be 2GB limited)

		const aafWChar		*CDCIName = L"TestCDCI.aaf",
							*PCMName = L"TestPCM.aaf";
		aafLength_t bytes = 0;

		checkFatal(CreateAAFFileCodec(CDCIName, false, filekind, compressionEnable,
							kAAFCodecCDCI, ContainerAAF, size_limit, &bytes));
		checkFatal(ReadAAFFileEssenceData(CDCIName, size_limit, bytes));
		checkFatal(ReadAAFFileCodec(CDCIName, size_limit, bytes));
		deleteTestFile(CDCIName);

		if (testRawStorage)
        {
			checkFatal(CreateAAFFileCodec(CDCIName, true, filekind, compressionEnable,
								kAAFCodecCDCI, ContainerAAF, size_limit, &bytes));
			checkFatal(ReadAAFFileEssenceData(CDCIName, size_limit, bytes));
			checkFatal(ReadAAFFileCodec(CDCIName, size_limit, bytes));
			deleteTestFile(CDCIName);
		}

		checkFatal(CreateAAFFileCodec(PCMName, false, filekind, compressionEnable,
							kAAFCodecPCM, ContainerAAF, size_limit, &bytes));
		checkFatal(ReadAAFFileEssenceData(PCMName, size_limit, bytes));
		checkFatal(ReadAAFFileCodec(PCMName, size_limit, bytes));
		deleteTestFile(PCMName);

		if (testRawStorage)
        {
			checkFatal(CreateAAFFileCodec(PCMName, true, filekind, compressionEnable,
								kAAFCodecPCM, ContainerAAF, size_limit, &bytes));
			checkFatal(ReadAAFFileEssenceData(PCMName, size_limit, bytes));
			checkFatal(ReadAAFFileCodec(PCMName, size_limit, bytes));
			deleteTestFile(PCMName);
		}

		// TODO: write a ReadAAFFileCodec() which uses the codec to read back samples
	}
#endif
	return(0);
}
