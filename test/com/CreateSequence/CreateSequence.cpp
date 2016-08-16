//=---------------------------------------------------------------------=
//
// $Id: CreateSequence.cpp,v 1.52 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

// @com Executable test program by Chris Morgan, intern for
//      Avid Technology, Tewksbury
// @com This is used for scalability testing of AAF code.
// Last modified on 9/2/01.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"

#include <time.h>

// Include the AAF interface declarations.
#include "AAF.h"
#include "AAFFileKinds.h"

STDAPI MemoryFileOpenNewModify (
  aafUInt32  modeFlags,
  aafProductIdentification_t*  pIdent,
  IAAFFile** ppFile);

STDAPI MemoryFileSaveToDisk(
  aafCharacter_constptr  pFileName,
  IAAFFile* pFile);

// Include the AAF Stored Object identifiers. These symbols are defined
// in aaf.lib.
#include "AAFStoredObjectIDs.h"

// MAX is used for arrays when converting between types - set here for
// debugging.
const int MAX = 80;
static char niceFileName[FILENAME_MAX];
static void usage(void);
static const aafWChar* slotName = L"SLOT1";
//static aafInt32 fadeInLen  = 1000;
//static aafInt32 fadeOutLen = 2000;
//static aafFadeType_t fadeInType = kAAFFadeLinearAmp;
//static aafFadeType_t fadeOutType = kAAFFadeLinearPower;
static aafSourceRef_t sourceRef;


#define TAPE_LENGTH     1L * 60L *60L * 30L
#define FILE1_LENGTH    60L * 30L
#define SEG_LENGTH      30L
#define FILL_LENGTH     10L
#define TEST_PATH       L"AnotherFile.aaf"

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n", msg); exit(1);}

static void     LogError(HRESULT errcode, int line, const char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a)  \
{ moduleErrorTmp = a; \
        if (FAILED(moduleErrorTmp)) \
        { \
            LogError(moduleErrorTmp, __LINE__, __FILE__);\
                goto cleanup; \
        } \
}

#define checkFatal(a)  \
{ moduleErrorTmp = a; \
  if (FAILED(moduleErrorTmp)) \
     exit(1);\
}



static HRESULT convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, ": Error : Conversion failed.\n");
    return -1;
  } else {
    return S_OK;
  }
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"CreateSequence";

static HRESULT CreateAAFFile(aafWChar * pFileName, long int N)
{
  IAAFFile*                   pFile = NULL;
  IAAFHeader*                 pHeader = NULL;
  IAAFDictionary*             pDictionary = NULL;
  IAAFMob*                    pMob = NULL;
  IAAFMob*                    pCompMob = NULL;
  IAAFEssenceDescriptor*      aDesc = NULL;
  IAAFMasterMob*              pMasterMob = NULL;
  IAAFSourceMob*              pFileMob = NULL;
  IAAFSourceMob*              pTapeMob = NULL;
  IAAFSequence*               pSequence = NULL;
  IAAFComponent*              aComponent = NULL;
  IAAFFileDescriptor*         pFileDesc = NULL;
  IAAFAIFCDescriptor*         pAIFCDesc = NULL;
  IAAFTapeDescriptor*         pTapeDesc = NULL;
  IAAFTimelineMobSlot*        newSlot = NULL;
  IAAFSegment*                seg = NULL;
  IAAFSourceClip*             fileSclp = NULL;
  IAAFSourceClip*             masterSclp = NULL;
  IAAFSourceClip*             compSclp = NULL;
  IAAFComponent*              compFill = NULL;
  IAAFLocator*                pLocator = NULL;
  IAAFNetworkLocator*         pNetLocator = NULL;
  IAAFClassDef *              pCDCompositionMob = 0;
  IAAFClassDef *              pCDSequence = 0;
  IAAFClassDef *              pCDSourceMob = 0;
  IAAFClassDef *              pCDTapeDescriptor = 0;
  IAAFClassDef *              pCDAIFCDescriptor = 0;
  IAAFClassDef *              pCDNetworkLocator = 0;
  IAAFClassDef *              pCDMasterMob = 0;
  IAAFClassDef *              pCDSourceClip = 0;
  IAAFClassDef *              pCDFiller = 0;
  IAAFDataDef *               pDdefPicture = 0;
  aafRational_t               videoRate = { 30000, 1001 };
  aafMobID_t                  tapeMobID, fileMobID, masterMobID;
  aafTimecode_t               tapeTC = { 108000, kAAFTcNonDrop, 30};
  aafLength_t                 fileLen = FILE1_LENGTH;
  aafLength_t                 fillLen = FILL_LENGTH;
  aafLength_t                 segLen = SEG_LENGTH;
  aafProductIdentification_t  ProductInfo;
  long int                    i = 0;

  moduleErrorTmp = S_OK;


  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = companyName;
  ProductInfo.productName = productName;
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = NIL_UID;
  ProductInfo.platform = NULL;

#if defined(USE_MEMORY_FILE)
  check(MemoryFileOpenNewModify (0, &ProductInfo, &pFile));
#else
  check(AAFFileOpenNewModifyEx (pFileName, &kAAFFileKind_Aaf4KBinary, 0, &ProductInfo, &pFile));
#endif

  check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));

  check(pDictionary->LookupClassDef(AUID_AAFCompositionMob,
                                    &pCDCompositionMob));
  check(pDictionary->LookupClassDef(AUID_AAFSequence,
                                    &pCDSequence));
  check(pDictionary->LookupClassDef(AUID_AAFSourceMob,
                                    &pCDSourceMob));
  check(pDictionary->LookupClassDef(AUID_AAFTapeDescriptor,
                                    &pCDTapeDescriptor));
  check(pDictionary->LookupClassDef(AUID_AAFAIFCDescriptor,
                                    &pCDAIFCDescriptor));
  check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator,
                                    &pCDNetworkLocator));
  check(pDictionary->LookupClassDef(AUID_AAFMasterMob,
                                    &pCDMasterMob));
  check(pDictionary->LookupClassDef(AUID_AAFSourceClip,
                                    &pCDSourceClip));
  check(pDictionary->LookupClassDef(AUID_AAFFiller,
                                    &pCDFiller));
  check(pDictionary->LookupDataDef(kAAFDataDef_Picture,
                                   &pDdefPicture));

// IMPORTANT: major remodification is from this point onwards...

  // sequence creation code pulled out of the subsequent loop.
  // Create a Composition Mob
  check(pCDCompositionMob->CreateInstance(IID_IAAFMob,
                                          (IUnknown **)&pCompMob));

  check(pCDSequence->CreateInstance(IID_IAAFSequence,
                                    (IUnknown **)&pSequence));
  check(pSequence->QueryInterface (IID_IAAFSegment, (void **)&seg));

  check(pSequence->QueryInterface(IID_IAAFComponent, (void **)&aComponent));

  check(aComponent->SetDataDef(pDdefPicture));
  aComponent->Release();
  aComponent = NULL;

  check(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
  check(pMob->AppendNewTimelineSlot(videoRate, seg, i, slotName, 0, &newSlot));
  pMob->Release();
  pMob = NULL;
  newSlot->Release();
  newSlot = NULL;
  seg->Release();
  seg = NULL;

  check(pHeader->AddMob(pCompMob));


  // now looping around the remainder N times to make N components
  for (i=0; i < N; i++) {
    //Make the Tape MOB
    check(pCDSourceMob->CreateInstance(IID_IAAFSourceMob,
                                       (IUnknown **)&pTapeMob));
    check(pCDTapeDescriptor->CreateInstance(IID_IAAFTapeDescriptor,
                                            (IUnknown **)&pTapeDesc));
    check(pTapeDesc->QueryInterface (IID_IAAFEssenceDescriptor,
                                     (void **)&aDesc));
    check(pTapeMob->SetEssenceDescriptor(aDesc));
    aDesc->Release();
    aDesc = NULL;
    pTapeDesc->Release();
    pTapeDesc = NULL;

    check(pTapeMob->AppendTimecodeSlot (videoRate, 0, tapeTC, TAPE_LENGTH));
    check(pTapeMob->AddNilReference (1,TAPE_LENGTH, pDdefPicture, videoRate));
    check(pTapeMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
    pTapeMob->Release();
    pTapeMob = NULL;


    // NOTE: TapeMob name is updated to change with number of objects
    // requested at cli.
    // In order to fit with the specification, it is made wide
    char TapeMobNameBuffer[MAX];
    sprintf(TapeMobNameBuffer,"Tape Mob %ld",i);
    aafWChar TapeMobName[MAX];
    mbstowcs(TapeMobName,TapeMobNameBuffer,MAX);

    check(pMob->SetName (TapeMobName));

    check(pHeader->AddMob(pMob));
    check(pMob->GetMobID (&tapeMobID));
    pMob->Release();
    pMob = NULL;

    // Make a FileMob
    check(pCDSourceMob->CreateInstance(IID_IAAFSourceMob,
                                       (IUnknown **)&pFileMob));
    check(pCDAIFCDescriptor->CreateInstance(IID_IAAFFileDescriptor,
                                            (IUnknown **)&pFileDesc));
    check(pFileDesc->QueryInterface (IID_IAAFEssenceDescriptor,
                                     (void **)&aDesc));
    check(pFileDesc->QueryInterface (IID_IAAFAIFCDescriptor,
                                     (void **)&pAIFCDesc));
    check(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
    pAIFCDesc->Release();
    pAIFCDesc = NULL;

    // Make a locator, and attach it to the EssenceDescriptor
    check(pCDNetworkLocator->CreateInstance(IID_IAAFNetworkLocator,
                                            (IUnknown **)&pNetLocator));
    check(pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));


    check(pLocator->SetPath (TEST_PATH));
    check(aDesc->AppendLocator(pLocator));
    pLocator->Release();
    pLocator = NULL;
    pNetLocator->Release();
    pNetLocator = NULL;


    check(pFileMob->SetEssenceDescriptor(aDesc));
    aDesc->Release();
    aDesc = NULL;
    pFileDesc->Release();
    pFileDesc = NULL;

    sourceRef.sourceID = tapeMobID;
    sourceRef.sourceSlotID = 1;
    sourceRef.startTime = 0;
    check(pFileMob->NewPhysSourceRef (videoRate, 1, pDdefPicture,
                                      sourceRef, fileLen));

    check(pFileMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
    check(pMob->GetMobID (&fileMobID));
    check(pHeader->AddMob(pMob));
    pMob->Release();
    pMob = NULL;

    //Make the Master MOB
    check(pCDMasterMob->CreateInstance(IID_IAAFMasterMob,
                                       (IUnknown **)&pMasterMob));

    sourceRef.sourceID = fileMobID;
    sourceRef.sourceSlotID = 1;
    sourceRef.startTime = 0;
    check(pMasterMob->NewPhysSourceRef(videoRate, 1, pDdefPicture,
                                       sourceRef, fileLen));
    check(pMasterMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
    check(pMob->GetMobID (&masterMobID));

    // NOTE: MasterMob name is updated to change with number of objects
    // requested at cli.
    // In order to fit with the specification, it is made wide
    char MasterMobNameBuffer[MAX];
    sprintf(MasterMobNameBuffer,"Master Mob %ld",i);
    aafWChar MasterMobName[MAX];
    mbstowcs(MasterMobName,MasterMobNameBuffer,MAX);

    check(pMob->SetName (MasterMobName));

    check(pHeader->AddMob(pMob));
    pMob->Release();
    pMob = NULL;

    // the remaining part of the sequence code, adapted for updating slot names


    // Create a SourceClip
    check(pCDSourceClip->CreateInstance(IID_IAAFSourceClip,
                                        (IUnknown **)&compSclp));

    sourceRef.sourceID = masterMobID;
    sourceRef.sourceSlotID = 1;
    sourceRef.startTime = 0;
    check(compSclp->SetSourceReference (sourceRef));
    check(compSclp->QueryInterface (IID_IAAFComponent, (void **)&aComponent));
    check(aComponent->SetDataDef(pDdefPicture));
    check(aComponent->SetLength (segLen));
    check(pSequence->AppendComponent (aComponent));

    // Create a filler - Get the component interface only (IID_IAAFComponent)
    check(pCDFiller->CreateInstance(IID_IAAFComponent,
                                    (IUnknown **)&compFill));

    check(compFill->SetLength (fillLen));

    check(compFill->SetDataDef(pDdefPicture));
    check(pSequence->AppendComponent (compFill));

    compFill->Release();
    compFill = NULL;

    aComponent->Release();
    aComponent = NULL;

    compSclp->Release();
    compSclp = NULL;

    pMasterMob->Release();
    pMasterMob = NULL;

    pFileMob->Release();
    pFileMob = NULL;

    //  end of loop since only one dictionary and header are needed
    //  the file is then saved, closed and released after all modifications
    //  are complete
  }


cleanup:
  // Cleanup and return
  if (pNetLocator)
    pNetLocator->Release();

  if (pLocator)
    pLocator->Release();

  if (compFill)
    compFill->Release();

  if (compSclp)
    compSclp->Release();

  if (masterSclp)
    masterSclp->Release();

  if (fileSclp)
    fileSclp->Release();

  if (pTapeDesc)
    pTapeDesc->Release();

  if (pFileDesc)
    pFileDesc->Release();

  if (pTapeMob)
    pTapeMob->Release();

  if (pFileMob)
    pFileMob->Release();

  if (pMasterMob)
    pMasterMob->Release();

  if (aDesc)
    aDesc->Release();

  if (newSlot)
    newSlot->Release();

  if (pMob)
    pMob->Release();


  if (pCompMob)
    pCompMob->Release();

  if (seg)
    seg->Release();

  if (aComponent)
    aComponent->Release();

  if (pSequence)
    pSequence->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();

  if (pCDCompositionMob) {
    pCDCompositionMob->Release();
    pCDCompositionMob = 0;
  }

  if (pCDSequence) {
    pCDSequence->Release();
    pCDSequence = 0;
  }

  if (pCDSourceMob) {
    pCDSourceMob->Release();
    pCDSourceMob = 0;
  }

  if (pCDTapeDescriptor) {
    pCDTapeDescriptor->Release();
    pCDTapeDescriptor = 0;
  }

  if (pCDAIFCDescriptor) {
    pCDAIFCDescriptor->Release();
    pCDAIFCDescriptor = 0;
  }

  if (pCDNetworkLocator) {
    pCDNetworkLocator->Release();
    pCDNetworkLocator = 0;
  }

  if (pCDMasterMob) {
    pCDMasterMob->Release();
    pCDMasterMob = 0;
  }

  if (pCDSourceClip) {
    pCDSourceClip->Release();
    pCDSourceClip = 0;
  }

  if (pCDFiller) {
    pCDFiller->Release();
    pCDFiller = 0;
  }

  if (pDdefPicture) {
    pDdefPicture->Release();
    pDdefPicture = 0;
  }

  if (pFile) {
    clock_t start = clock();
    clock_t finish;
    double duration;

    pFile->Save();
    pFile->Close();

#if defined(USE_MEMORY_FILE)
    check(MemoryFileSaveToDisk(pFileName, pFile));
#endif
    finish = clock();
    duration = ((double) (finish - start) / CLOCKS_PER_SEC);

    pFile->Release();

    printf("Save time = %f seconds\n", duration);
  }

  return moduleErrorTmp;
}

// now need the reading functionality
static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  IAAFFile * pFile = NULL;
  // printing file open time

  clock_t start = clock();
  clock_t finish;
  double duration;

  check(AAFFileOpenExistingRead ( pFileName, 0, &pFile));

  finish = clock();
  duration = ((double) (finish - start) / CLOCKS_PER_SEC);

  pFile->Close();
  pFile->Release();
  pFile=NULL;

#if 0
  printf("Open time = %f seconds\n", duration);
#endif

cleanup:
  if (pFile) {
    pFile->Close();
    pFile->Release();
  }
  return AAFRESULT_SUCCESS;
}

//  A new usage function to make program more friendly
void usage(void)
{
  printf("Usage:\n");
  printf("  CreateSequence <Number of components> <file>.aaf\n");
  printf("  CreateSequence <Number of components> <file>\n");
  printf("  CreateSequence <Number of components>\n");
  printf("<Number of components> must be greater than zero.\n");
  printf("<file> defaults to <Number of components>.aaf\n");
}

STDAPI MemoryFileOpenNewModify (
  aafUInt32  modeFlags,
  aafProductIdentification_t*  pIdent,
  IAAFFile** ppFile)
{
  HRESULT hr;
  IAAFRawStorage* rs = 0;

  hr = AAFCreateRawStorageMemory(
    kAAFFileAccess_modify,
    &rs);
  if (!SUCCEEDED(hr)) {
    return hr;
  }

  hr = AAFCreateAAFFileOnRawStorage(
    rs,
    kAAFFileExistence_new,
    kAAFFileAccess_write,
    &kAAFFileKind_Aaf4KBinary,
    modeFlags,
    pIdent,
    ppFile);
  if (!SUCCEEDED(hr)) {
    (*ppFile)->Release();
    return hr;
  }

  hr = (*ppFile)->Open();
  if (!SUCCEEDED(hr)) {
    (*ppFile)->Release();
    rs->Release();
    return hr;
  }

  rs->Release();
  rs = 0;

  return S_OK;
}

STDAPI MemoryFileSaveToDisk(
  aafCharacter_constptr  pFileName,
  IAAFFile* pFile)
{
  IAAFRandomFile* pRandFile;
  HRESULT hr = pFile->QueryInterface(IID_IAAFRandomFile, (void **)&pRandFile);
  if (FAILED(hr)) {
    return hr;
  }
  
  IAAFGetFileBits* pGetBits;
  hr = pRandFile->GetFileBits(&pGetBits);
  if (FAILED(hr)) {
    pRandFile->Release();
    return hr;
  }

  aafUInt64 size = 0;
  hr = pGetBits->GetSize(&size);
  if (FAILED(hr)) {
    pRandFile->Release();
    pGetBits->Release();
    return hr;
  }

  char cFileName[FILENAME_MAX];
  size_t status = wcstombs(cFileName, pFileName, FILENAME_MAX);
  if (status == (size_t)-1) {
    pRandFile->Release();
    pGetBits->Release();
    return E_FAIL;
  }

  FILE *f = fopen(cFileName, "wb");
  if (f == 0) {
    pRandFile->Release();
    pGetBits->Release();
    return E_FAIL;
  }

  aafUInt64 position = 0;
  aafUInt64 remaining = size;
  while (remaining != 0) {
    aafUInt32 readSize;
    aafUInt8 buffer[4 * 1024];
    if (remaining > sizeof(buffer)) {
      readSize = sizeof(buffer);
	} else {
      readSize = (aafUInt32)remaining;
    } 
	hr = pGetBits->ReadAt(buffer, readSize, position);
	if (FAILED(hr)) {
      pRandFile->Release();
      pGetBits->Release();
      return hr;
    }
    fwrite(buffer, 1, readSize, f);
    position = position + readSize;
    remaining = remaining - readSize;
  }
  fclose(f);

  pRandFile->Release();
  pGetBits->Release();
  return S_OK;
}

//  Main adapted to use command-line arguments with argument checking
//  NOTE:  defining [0] program name; [1] Number N of components;
//  [2] filename.aaf;
int main(int argumentCount, char *argumentVector[])
{
  //  First check for correct number of arguments
  //  printf("%ld\n",argumentCount);
  if ((argumentCount < 2) || (argumentCount > 3)) {
    usage();
    return 0;
  }
  //  Processing the second argument to be stored as global variable N
  char* Ns = argumentVector[1];
  char* expectedEnd = &Ns[strlen(Ns)];
  char* end = 0;
  long int N = strtoul(Ns, &end, 10);

  //  Testing for correct second argument
  if ((end != expectedEnd) || (N < 1)) {
    printf("The first argument was of the incorrect form. [%s]\n",
           argumentVector[1]);
    usage();
    return 0;
  }

  //  With no second argument, set output filename to CreateSequence<N>.aaf
  if (argumentCount == 2) {
    strncpy(niceFileName, Ns, FILENAME_MAX);
  } else  {
    //  Otherwise output to filename specified in the second argument
    //  NB this case must have argC ==3 from earlier check

    strncpy(niceFileName, argumentVector[2], FILENAME_MAX);
  }
  if (strstr(niceFileName, ".aaf") == 0)
    strcat (niceFileName, ".aaf");

  //  and then carry on...

  // Load the AAF library explicity to catch the common error
  // that the AAF DLL is not in the user's path, otherwise this
  // error looks like an error opening the file.
  //
  HRESULT hr = AAFLoad(0);
  if (!AAFRESULT_SUCCEEDED(hr)) {
    fprintf(stderr, "Error : Failed to load the AAF library, ");
    fprintf(stderr, "check environment variables -\n");
    fprintf(stderr, "  Windows    - $PATH\n");
    fprintf(stderr, "  Unix/Linux - $LD_LIBRARY_PATH\n");
    exit(hr);
  }

  aafWChar FileNameBuffer[MAX];
  mbstowcs(FileNameBuffer, niceFileName, MAX);

  aafWChar * pwFileName = FileNameBuffer;

  //  Give a nice output here too...
  printf("Creating file %s with %ld components.\n", niceFileName, N);
  checkFatal(CreateAAFFile(pwFileName, N));

  // Open the file and gather statistics
  ReadAAFFile(pwFileName);

  return(0);
}
