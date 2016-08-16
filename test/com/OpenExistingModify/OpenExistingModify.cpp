// @doc INTERNAL
// @com This file implements tests for variour file kinds
//=---------------------------------------------------------------------=
//
// $Id: OpenExistingModify.cpp,v 1.9 2009/06/01 11:47:15 stuart_hc Exp $ $Name: V116 $
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

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <limits.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFFileMode.h"
#include "AAFFileKinds.h"

using namespace std;

static void RemoveTestFile(const aafWChar* pFileName)
{
  char cFileName[FILENAME_MAX];

  size_t status = wcstombs(cFileName, pFileName, FILENAME_MAX);
  if (status != (size_t)-1) {
    remove(cFileName);
  }
}

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

#define CR(x) checkResult(x)

static const   aafMobID_t  TEST_MobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc302, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};
static const   aafMobID_t  TEST_SourceMobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc402, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"OpenExistingModify";
static aafProductVersion_t TestVersion = { 1, 1, 0, 0, kAAFVersionUnknown };
static aafProductIdentification_t TestProductID;


static HRESULT CreateAAFFile(aafWChar *filename, aafUID_constref fileKind)
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
		RemoveTestFile(filename);

		// Open new file
		IAAFFile		*pFile = NULL;
		TestProductID.productVersionString = const_cast<aafWChar*>(L"CreateAAFFile");
		checkResult( AAFFileOpenNewModifyEx(
						filename,
						&fileKind,
						0,
						&TestProductID,
						&pFile) );

		// Get the header & dictionary
		IAAFHeader		*pHeader = NULL;
		IAAFDictionary	*pDictionary = NULL;
		checkResult(pFile->GetHeader(&pHeader));
		checkResult(pHeader->GetDictionary(&pDictionary));

		// Create a MasterMob
		IAAFMob			*pMob = NULL;
		IAAFClassDef	*classDef = NULL;
		checkResult(pDictionary->LookupClassDef(AUID_AAFMasterMob, &classDef));
		checkResult(classDef->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
		classDef->Release();
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"CreateAAFFile - MasterMob"));
		checkResult(pHeader->AddMob(pMob));
		pMob->Release();

		// Create a SourceMob 
		IAAFSourceMob			*pSourceMob = NULL;
		checkResult(pDictionary->LookupClassDef(AUID_AAFSourceMob, &classDef));
		checkResult(classDef->CreateInstance(IID_IAAFSourceMob, (IUnknown **)&pSourceMob));
		classDef->Release();
		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_SourceMobID));
		checkResult(pMob->SetName(L"CreateAAFFile - SourceMob"));

		IAAFEssenceDescriptor	*edesc = NULL;
		IAAFAIFCDescriptor		*pAIFCDesc = NULL;
		checkResult(pDictionary->LookupClassDef(AUID_AAFAIFCDescriptor, &classDef));
		checkResult(classDef->CreateInstance(IID_IAAFEssenceDescriptor, (IUnknown **)&edesc));
		classDef->Release();
		checkResult(edesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		aafUInt8	buf[] = {0x00};
		checkResult(pAIFCDesc->SetSummary(sizeof(buf), buf));
		checkResult(pSourceMob->SetEssenceDescriptor(edesc));
		checkResult(pHeader->AddMob(pMob));
		pAIFCDesc->Release();
		edesc->Release();
		pSourceMob->Release();
		pMob->Release();

		pDictionary->Release();
		pHeader->Release();

		// Save & close the file
		checkResult(pFile->Save());
		checkResult(pFile->Close());
		checkResult(pFile->Release());

		cout << "CreateAAFFile() - created new file" << endl;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** CreateAAFFile: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

static HRESULT ModifyAAFFile(aafWChar *filename, int level)
{
	HRESULT			hr = S_OK;

	try
	{
		// Open existing file for modification
		IAAFFile		*pFile = NULL;
		TestProductID.productVersionString = const_cast<aafWChar*>(L"ModifyAAFFile");
		checkResult( AAFFileOpenExistingModify(
						filename,
						0,					// modeFlags
						&TestProductID,
						&pFile) );

		cout << "ModifyAAFFile() - appended Identification" << endl;

		// Get the header & dictionary
		IAAFHeader		*pHeader = NULL;
		IAAFDictionary	*pDictionary = NULL;
		checkResult(pFile->GetHeader(&pHeader));
		checkResult(pHeader->GetDictionary(&pDictionary));

		// Search for Mobs
		IAAFMob			*pFileMob = NULL;
		IEnumAAFMobs	*pFileMobIter = NULL;
		aafSearchCrit_t				criteria;
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFFileMob;		// Search by File Mob
	
		checkResult(pHeader->GetMobs(&criteria, &pFileMobIter));
	
		while (AAFRESULT_SUCCESS == pFileMobIter->NextOne(&pFileMob))
		{
			if (level == 0)
				break;

			IAAFEssenceDescriptor	*edesc = NULL;
			IAAFSourceMob			*pSourceMob = NULL;
			CR(pFileMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
			CR(pSourceMob->GetEssenceDescriptor(&edesc));

			// Change the Name property
			CR(pFileMob->SetName(L"ModifyAAFFile - modified Name"));
			cout << "ModifyAAFFile() - changed FileMob's Name property" << endl;

			if (level == 1)
				break;

			// Change descriptor's properties
			IAAFAIFCDescriptor		*pAIFCDesc = NULL;
			CR(edesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
			aafUInt8				AIFCsum[] = {0xa1,0xfc};
			CR(pAIFCDesc->SetSummary(sizeof(AIFCsum), AIFCsum));
			pAIFCDesc->Release();
			edesc->Release();
			cout << "ModifyAAFFile() - changed AIFCDescriptor's Summary" << endl;

			if (level == 2)
				break;

			// Change descriptor to new one (overwriting old one)
			IAAFClassDef			*classDef = NULL;
			IAAFFileDescriptor		*pFileDesc = NULL;
			IAAFWAVEDescriptor		*pWAVEDesc = NULL;
			IAAFEssenceDescriptor	*pNewEdesc = NULL;

			CR(pDictionary->LookupClassDef(AUID_AAFWAVEDescriptor, &classDef));
			CR(classDef->CreateInstance(IID_IAAFFileDescriptor, (IUnknown **)&pFileDesc));
			CR(pFileDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc));
			CR(pFileDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pNewEdesc));
			aafUInt8				WAVEsum[] = {0x1a,0x1e,0xee,0xee};
			CR(pWAVEDesc->SetSummary(sizeof(WAVEsum), WAVEsum));
			CR(pSourceMob->SetEssenceDescriptor(pNewEdesc));
			pNewEdesc->Release();
			pWAVEDesc->Release();
			pFileDesc->Release();
			classDef->Release();
			cout << "ModifyAAFFile() - replaced AIFCDescriptor with WAVEDescriptor" << endl;

			if (level == 3)
				break;

			// Add EssenceData
			IAAFEssenceData			*pEssenceData = NULL;
			IAAFEssenceData2		*pEssenceData2 = NULL;
			IAAFPlainEssenceData		*pPlainEssenceData = NULL;
			aafUInt32				bytesWritten = 0;
			aafUInt8				essdata[] = "Zaphod Beeblebrox";
			CR(pDictionary->LookupClassDef(AUID_AAFEssenceData, &classDef));
			CR(classDef->CreateInstance(IID_IAAFEssenceData, (IUnknown **)&pEssenceData));
			CR(pEssenceData->SetFileMob(pSourceMob));
			CR(pHeader->AddEssenceData(pEssenceData));
			CR(pEssenceData->QueryInterface(IID_IAAFEssenceData2, (void**)&pEssenceData2));
			CR(pEssenceData2->GetPlainEssenceData(0, &pPlainEssenceData));
			CR(pPlainEssenceData->Write(sizeof(essdata), essdata, &bytesWritten));
			pEssenceData->Release();
			pEssenceData2->Release();
			pPlainEssenceData->Release();
			classDef->Release();
			cout << "ModifyAAFFile() - added EssenceData" << endl;


			pSourceMob->Release();
		}
		pFileMob->Release();
		pFileMobIter->Release();

		pDictionary->Release();
		pHeader->Release();

		// Save & close the file
		checkResult(pFile->Save());
		checkResult(pFile->Close());
		checkResult(pFile->Release());
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** ModifyAAFFile: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar *filename)
{
	HRESULT			hr = S_OK;

	try
	{
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
		cout << "*** ReadAAFFile: caught error hr=0x" << hex << hr << dec << endl;
	}

	return hr;
}

void printUsage(const char *progname)
{
	fprintf(stderr, "Usage:\n\t%s [options] [file_to_modify.aaf]\n", progname);
	fprintf(stderr, "\n");
	fprintf(stderr, "\tWhen no file argument is provided a test file will be written first\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\t-l   \"level\" of modification to perform (default is all levels)\n");
	fprintf(stderr, "\t      0 - open then close, causing Identification to be appended\n");
	fprintf(stderr, "\t      1 - change Mob's Name property\n");
	fprintf(stderr, "\t      2 - change AIFCDescriptor's Summary property\n");
	fprintf(stderr, "\t      3 - replace AIFCDescriptor with WAVEDescriptor\n");
	fprintf(stderr, "\t      4 - add EssenceData\n");
	fprintf(stderr, "\t-e    \"encoding\" of initial test file\n");
	fprintf(stderr, "\t      AAF-S4K - AAF 4K Structured Storage encoding (default)\n");
	fprintf(stderr, "\t      AAF-S   - AAF Structured Storage encoding\n");
	fprintf(stderr, "\t      KLV     - AAF KLV encoding\n");
	fprintf(stderr, "\t      XML     - AAF XML encoding\n");
}

int main(int argc, char* argv[])
{
	aafWChar		filename[FILENAME_MAX] = L"openExistingModify_test.aaf";
	int				level = INT_MAX;
	bool			create_input_file = true;
	aafUID_t			fileKind = kAAFFileKind_Aaf4KBinary;

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
			else if (!strcmp(argv[i], "-l"))
			{
				
				level = atoi(argv[i+1]);
				i += 2;
			}
			else if (!strcmp(argv[i], "-e"))
			{
				if (!strcmp(argv[i+1], "AAF-S4K"))
				{
					fileKind = kAAFFileKind_Aaf4KBinary;
				}
				else if (!strcmp(argv[i+1], "AAF-S"))
				{
					fileKind = kAAFFileKind_Aaf512Binary;
				}
				else if (!strcmp(argv[i+1], "KLV"))
				{
					fileKind = kAAFFileKind_AafKlvBinary;
				}
				else if (!strcmp(argv[i+1], "XML"))
				{
					fileKind = kAAFFileKind_AafXmlText;
				}
				i += 2;
			}
			else
			{
				break;
			}
		}
	}

	if (argc - i == 1)			// one arg left - must be filename
	{
		if ((mbstowcs(filename, argv[i], sizeof(filename))) == (size_t)(-1))
		{
			fprintf(stderr, "%s: mbstowcs conversion failed\n", argv[i]);
			return 1;
		}
		create_input_file = false;
	}
	else if ( argc - i > 1)
	{
		printUsage(argv[0]);
		return 1;
	}

	try
	{
		if (create_input_file)
		{
			checkResult(CreateAAFFile(filename, fileKind));

			checkResult(ModifyAAFFile(filename, level));

			checkResult(ReadAAFFile(filename));
		}
		else
		{
			// So far, only appending Identification is supported
			// for files created outside this program.
			checkResult(ModifyAAFFile(filename, 0));
		}
	}
	catch (HRESULT& r)
	{
		cerr << "[main] Error : Caught HRESULT 0x" << hex << r << dec << endl;
		return 1;
	}
	return 0;
}
