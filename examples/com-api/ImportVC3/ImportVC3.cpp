// @com Executable example program by Stuart Cunningham, BBC R&D
// @com This is based upon ComEssenceDataTest.cpp.

//=---------------------------------------------------------------------=
//
// $Id: ImportVC3.cpp,v 1.3 2009/06/01 11:46:52 stuart_hc Exp $ $Name: V116 $
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

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"


#define MAX_VC3_UNCOMPRESSED_FRAME (1080*1920*8)
#define MAX_VC3_COMPRESSED_FRAME (917504)

#define aaf_assert(b, msg) \
	if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

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

static void convert(char* cName, size_t length, const wchar_t* name)
{
	aaf_assert((name && *name), "Valid input name");
	aaf_assert(cName != 0, "Valid output buffer");
	aaf_assert(length > 0, "Valid output buffer size");

	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
		fprintf(stderr, ": Error : Conversion failed.\n\n");
		// exit(1);
		// note that this may overrun the input buffer if it is not null-terminated
		// TODO: replace this routine throughout the SDK
		// with one based arounf mbtowc() and verified length of input string
	}
}

static void MobIDtoString(aafMobID_constref uid, char *buf)
{
    sprintf( buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-" \
		  "%02x-%02x-%02x-%02x-" \
		  "%08x%04x%04x" \
		  "%02x%02x%02x%02x%02x%02x%02x%02x",

	(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], 
	(int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3],
	(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], 
	(int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7],
	(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[9], 
	(int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11],

	(int)uid.length, (int)uid.instanceHigh, 
	(int)uid.instanceMid, (int)uid.instanceLow,

	uid.material.Data1, uid.material.Data2, uid.material.Data3,

	(int)uid.material.Data4[0], (int)uid.material.Data4[1], 
	(int)uid.material.Data4[2], (int)uid.material.Data4[3],
	(int)uid.material.Data4[4], (int)uid.material.Data4[5], 
	(int)uid.material.Data4[6], (int)uid.material.Data4[7] );
}

static HRESULT OpenAAFFile(aafWChar * pFileName, bool comp_enable, const char * output_file )
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceFormat*			pFormat = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafMobID_t					mobID;
	IAAFDataDef					*pPictureDef = NULL;
	IAAFMobSlot*				pMobSlot = NULL;


	// Open an AAF file
	check(AAFFileOpenExistingRead (pFileName, 0, &pFile));
	check(pFile->GetHeader(&pHeader));

	// Open raw video output file
	FILE		*output;

	if ((output = fopen(output_file, "wb")) == NULL)
	{
		perror(output_file);
		exit(1);
	}

	// Get the AAF Dictionary from the file
	check(pHeader->GetDictionary(&pDictionary));

	/* Lookup any necessary data definitions. */
	check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDef));

	/* Check number of Mobs in file */
	check(pHeader->CountMobs(kAAFMasterMob, &numMobs));
	if (numMobs == 0)
		return 0;

	printf("Found %d Master Mobs\n", numMobs);
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFMasterMob;
	check(pHeader->GetMobs(&criteria, &pMobIter));

	while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
	{
		char mobIDstr[256];
		char mobName[256];
		aafWChar	namebuf[1024];
		IAAFTimelineMobSlot* pTimelineMobSlot = NULL;
		IAAFDataDef *pDataDef = NULL;
		IAAFEssenceFormat			*fmtTemplate =  NULL;
		unsigned char *dataBuff = NULL;

		IEnumAAFMobSlots* pMobSlotIter = NULL;


		check(pMob->GetMobID (&mobID));
		check(pMob->GetName (namebuf, sizeof(namebuf)));
		convert(mobName, sizeof(mobName), namebuf);

		MobIDtoString(mobID, mobIDstr);
		printf("    MasterMob Name = '%s'\n", mobName);
		printf("        (mobID %s)\n", mobIDstr);
		
		// Get the number of slots
		check(pMob->CountSlots(&numSlots));
		
		// Iterating through all Mob Slots

		check(pMob->GetSlots(&pMobSlotIter));
		while(AAFRESULT_SUCCESS == pMobSlotIter->NextOne(&pMobSlot))
		{
			// Check to see if it is a Video Timeline Mob Slot
			HRESULT hr;
			aafUInt32 MobSlotID;
	
			hr=pMobSlot->QueryInterface(IID_IAAFTimelineMobSlot,(void **) &pTimelineMobSlot);
			if (SUCCEEDED(hr))
			{
				check(pMobSlot->GetDataDef(&pDataDef));
	
				// Check that we have a sound file by examining its data definition
				aafBool bIsPictureKind = kAAFFalse;
				check(pDataDef->IsPictureKind(&bIsPictureKind));
	
				if (kAAFTrue == bIsPictureKind)
				{
					IAAFMasterMob*			pMasterMob = NULL;

					// Prepare to get video data: first get MobSlotID
					check(pMobSlot->GetSlotID(&MobSlotID));
	
					// Then get a Master Mob interface
					check(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));
					
					// Open the Essence Data
					check(pMasterMob->OpenEssence(MobSlotID,
											 	  NULL,	
												  kAAFMediaOpenReadOnly,
												  comp_enable ? kAAFCompressionEnable : kAAFCompressionDisable, 
												  &pEssenceAccess));
					
					// Get the information about the format of the video data.
					// The pFormat object must be setup with each specifier you
					// wish to access, otherwise you get AAFRESULT_FORMAT_NOT_FOUND.
					aafRational_t		sampleRate;
					aafUInt32			maxSampleSize;
					aafRect_t			storedRect;
					aafFrameLayout_t	frameLayout;

					if(comp_enable)
					{
						maxSampleSize=MAX_VC3_UNCOMPRESSED_FRAME; 
					}
					else
					{
						maxSampleSize=MAX_VC3_COMPRESSED_FRAME; 
					}
					// Set a suitable buffer size						
					dataBuff = new unsigned char[maxSampleSize];
	
					// Get some essence properties and print them
					check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
					check(fmtTemplate->AddFormatSpecifier (kAAFSampleRate, 0, NULL));
					//check(fmtTemplate->AddFormatSpecifier (kAAFMaxSampleBytes, 0, NULL));
					check(fmtTemplate->AddFormatSpecifier (kAAFStoredRect, 0, NULL));
					check(fmtTemplate->AddFormatSpecifier (kAAFFrameLayout, 0, NULL));
					check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));

					fmtTemplate->Release();
					fmtTemplate = NULL;

					aafInt32 fmtBytesRead;
					check(pFormat->GetFormatSpecifier(kAAFSampleRate, sizeof(sampleRate),
						(aafDataBuffer_t)&sampleRate, &fmtBytesRead));
					//check(pFormat->GetFormatSpecifier(kAAFMaxSampleBytes, sizeof(maxSampleSize),
					//	(aafDataBuffer_t)&maxSampleSize, &fmtBytesRead));
					check(pFormat->GetFormatSpecifier(kAAFStoredRect, sizeof(storedRect),
						(aafDataBuffer_t)&storedRect, &fmtBytesRead));
					check(pFormat->GetFormatSpecifier(kAAFFrameLayout, sizeof(frameLayout),
						(aafDataBuffer_t)&frameLayout, &fmtBytesRead));

					pFormat->Release();
					pFormat = NULL;

					const char *frameLayoutStr = "";
					switch (frameLayout)
					{
						case kAAFFullFrame: frameLayoutStr = "FullFrame"; break;
						case kAAFOneField: frameLayoutStr = "OneField"; break;
						case kAAFSeparateFields: frameLayoutStr = "SeparateFields"; break;
						case kAAFMixedFields: frameLayoutStr = "MixedFields"; break;
						default: frameLayoutStr = "Unknown"; break;
					}

					// Get the sample count which is in terms of EditRate
					aafLength_t sampleCount;
					check(pEssenceAccess->CountSamples(pPictureDef, &sampleCount));

					// Get and print the name of the selected codec flavour
					aafWChar codec_name[128] = L"";
					check(pEssenceAccess->GetCodecName(sizeof(codec_name), codec_name));
					printf("\n\tUsing codec flavour \"%ls\"", codec_name);

					printf("\n\tSlotID %u: \n\t\tSampleRate=%d/%d \n\t\tMaxSampleBytes=%u \n\t\tStoredRect=%dx%d",
								MobSlotID, sampleRate.numerator, sampleRate.denominator,
								maxSampleSize, storedRect.xSize, storedRect.ySize);
			
					printf("\n\t\tFrameLayout=%s \n\t\tCountSamples=%"AAFFMT64"d\n", frameLayoutStr, sampleCount);

					// Read samples until no more are available
					aafUInt32	samplesRead, actualBytesRead, total_samples = 0;
					while (true)
					{
						hr = (pEssenceAccess->ReadSamples(
										1, 					// number of samples to read
										maxSampleSize,		// maximum buffer size
										dataBuff,			// output buffer for audio data
										&samplesRead,		// number of samples read
										&actualBytesRead));	// number of bytes read
						if (hr == AAFRESULT_EOF)
							break;
						else
							check(hr);
	
						if (actualBytesRead!=0)
						{
							total_samples += samplesRead;

							// Write out video
							if ( fwrite(dataBuff, actualBytesRead, 1, output) != 1 )
							{
								perror(output_file);
								return 1;
							}
						} 
					}
					printf("\tTotal samples = %u (written to %s)\n", total_samples, output_file);
					
					delete [] dataBuff;
					dataBuff = NULL;
					
					pEssenceAccess->Release();
					pEssenceAccess = NULL;
					pMasterMob->Release();
					pMasterMob = NULL;
				}
	
				pTimelineMobSlot->Release();
				pTimelineMobSlot = NULL;
	
				pDataDef->Release();
				pDataDef = NULL;
			}	
	
			pMobSlot->Release();
			pMobSlot = NULL;
		}
	
		pMobSlotIter->Release();
		pMobSlotIter = NULL;
	
		pMob->Release();
		pMob = NULL;
	}

	pMobIter->Release();
	pMobIter = NULL;

	return moduleErrorTmp;
}

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
	fprintf(stderr, "Usage:\n\t%s [-compressionEnable] [-c] [-e=essenceoutputfilename] file.aaf\n", progname);
}

int main(int argc, char *argv[])
{
	CAAFInitialize aafInit;

	aafWChar 		pwFileName[FILENAME_MAX*4];
	bool			compressionEnable = false;
	const char *	output_file = NULL;


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
			else if ( !strcmp(argv[i], "-e") || !strcmp(argv[i], "-extract"))
			{
				// extract filename in next arg
				if( (i+1) < argc ) output_file = argv[++i];
				i++;
			}
			else if ( !strncmp(argv[i], "-e=", 3) )
			{
				// extract filename in this arg
				output_file = argv[i]+3;
				i++;
			}
			else if ( !strcmp(argv[i], "-c") || !strcmp(argv[i], "-compressionEnable") )
			{
				compressionEnable = true;
				i++;
			}
			else
			{
				break;
			}
		}
	}

	// set default output filename
	if( !output_file ) output_file = compressionEnable ? "raw.yuv" : "raw.dnx";


	if (i >= argc)
	{
		printUsage(argv[0]);
		return 1;
	}

	// last arg is input filename
	if ((mbstowcs(pwFileName, argv[i], sizeof(pwFileName))) == (size_t)(-1))
	{
		fprintf(stderr, "%s: mbstowcs conversion failed\n", argv[i]);
		return 1;
	}
	printf("Using %s for input file\n", argv[i]);

	// Make sure all of our required plugins have been registered.
	checkFatal(RegisterRequiredPlugins());

	checkFatal(OpenAAFFile(pwFileName, compressionEnable, output_file));

	printf("DONE\n\n");

	return 0;
}
