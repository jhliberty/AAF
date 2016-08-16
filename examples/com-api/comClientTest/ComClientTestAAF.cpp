//=---------------------------------------------------------------------=
//
// $Id: ComClientTestAAF.cpp,v 1.61 2009/06/01 11:46:53 stuart_hc Exp $ $Name: V116 $
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

// Include the AAF interface declarations.
#include "AAF.h"
#include "AAFTypes.h"
#include "AAFResult.h"


// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"
#include "AAFFileKinds.h"
#include "AAFDataDefs.h"


static const aafMobID_t TEST_MobID[5] = {

//{060c2b340205110101001000-13-00-00-00-{fcd55620-9bca-11d4-9f7f-080036210804}}
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00, 
{0xfcd55620, 0x9bca, 0x11d4, {0x9f, 0x7f, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}},

//{060c2b340205110101001000-13-00-00-00-{5cf19020-9f89-11d4-9f82-080036210804}}
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
0x13, 0x00, 0x00, 0x00, 
{0x5cf19020, 0x9f89, 0x11d4, {0x9f, 0x82, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}},

//{060c2b340205110101001000-13-00-00-00-{5de486f0-9f89-11d4-9f82-080036210804}}
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
0x13, 0x00, 0x00, 0x00, 
{0x5de486f0, 0x9f89, 0x11d4, {0x9f, 0x82, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}},

//{060c2b340205110101001000-13-00-00-00-{5e8e24c0-9f89-11d4-9f82-080036210804}}
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
0x13, 0x00, 0x00, 0x00, 
{0x5e8e24c0, 0x9f89, 0x11d4, {0x9f, 0x82, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}},

//{060c2b340205110101001000-13-00-00-00-{5f37c290-9f89-11d4-9f82-080036210804}}
{ {0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
0x13, 0x00, 0x00, 0x00, 
{0x5f37c290, 0x9f89, 0x11d4, {0x9f, 0x82, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}},

}; // TEST_MobID[]

static void     FatalErrorCode(HRESULT errcode, int line, const char *file)
{
  printf("\nError '%0x' returned at line %d in %s\n", errcode, line, file);
  exit(1);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     FatalErrorCode(moduleErrorTmp, __LINE__, __FILE__);\
}

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "\nASSERT: %s\n\n", msg); exit(1);}


static void convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, "\nError : Conversion failed.\n\n");
    exit(1);  
  }
}

static void convert(char* cName, size_t length, const char* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t sourceLength = strlen(name);
  if (sourceLength < length - 1) {
    strncpy(cName, name, length);
  } else {
    fprintf(stderr, "\nError : Failed to copy '%s'.\n\n", name);
    exit(1);  
  }
}

#if defined( OS_UNIX )

static const unsigned char guidMap[] =
{ 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15 }; 
static const wchar_t digits[] = L"0123456789ABCDEF"; 

#define GUIDSTRMAX 38 

typedef OLECHAR OMCHAR;

int StringFromGUID2(const GUID& guid, OMCHAR* buffer, int bufferSize) 
{
  const unsigned char* ip = (const unsigned char*) &guid; // input pointer
  OMCHAR* op = buffer;                                    // output pointer

  *op++ = L'{'; 
 
  for (size_t i = 0; i < sizeof(guidMap); i++) { 

    if (guidMap[i] == '-') { 
      *op++ = L'-'; 
    } else { 
      *op++ = digits[ (ip[guidMap[i]] & 0xF0) >> 4 ]; 
      *op++ = digits[ (ip[guidMap[i]] & 0x0F) ]; 
    } 
  } 
  *op++ = L'}'; 
  *op = L'\0'; 
 
  return GUIDSTRMAX; 
} 

#endif

// The maximum number of characters in the formated CLSID.
// (as returned by StringFromGUID2).
const size_t MAX_CLSID_BUFFER = 40;

static void formatMobID(char *cBuffer, size_t length, aafMobID_t *pMobID)
{
  assert(pMobID, "Valid input mobID");
  assert(cBuffer != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  int bytesCopied = 0;
  OLECHAR wCLSID[MAX_CLSID_BUFFER];

  bytesCopied = StringFromGUID2(*((GUID *)pMobID), wCLSID, MAX_CLSID_BUFFER);
  if (0 < bytesCopied) {
    convert(cBuffer, length, wCLSID);
  } else {
    fprintf(stderr, "\nError : formatMobID failed.\n\n");
    exit(1);  
  }
}

static void printIdentification(IAAFIdentification* pIdent)
{
  aafWChar wchName[500];
  char chName[1000];
    
    
  check(pIdent->GetCompanyName(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("CompanyName          = \"%s\"\n", chName);

  check(pIdent->GetProductName(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("ProductName          = \"%s\"\n", chName);

  check(pIdent->GetProductVersionString(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("ProductVersionString = \"%s\"\n", chName);

  check(pIdent->GetPlatform(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("Platform             = \"%s\"\n", chName);
}

static void ReadAAFFile(const aafWChar * pFileName)
{
  HRESULT hr = S_OK;
  IAAFFile * pFile = NULL;


  hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);
  check(hr);
  if (SUCCEEDED(hr))
  {
    IAAFHeader * pHeader = NULL;


    hr = pFile->GetHeader(&pHeader);
    check(hr); // display error message
    if (SUCCEEDED(hr))
    {
      IAAFDictionary * pDictionary = NULL;

      hr = pHeader->GetDictionary(&pDictionary);
      check(hr); // display error message
      if (SUCCEEDED(hr))
      {
        IAAFIdentification *pIdent = NULL;
        aafNumSlots_t numMobs = 0;


        hr = pHeader->GetLastIdentification(&pIdent);
        check(hr); // display error message
        if (SUCCEEDED(hr))
        {
          printf("LastIdentification\n");
          printIdentification(pIdent);

          pIdent->Release();
          pIdent = NULL;
        }

        hr = pHeader->CountMobs(kAAFAllMob, &numMobs);
        check(hr); // display error message
        if (FAILED(hr))
          numMobs = 0;
        printf("Number of Mobs = %d\n", numMobs);


        IEnumAAFMobs      *mobIter = NULL;

        if (SUCCEEDED(hr))
        {
          //!!!  aafSearchCrit_t    criteria;
          //!!!  criteria.searchTag = kAAFNoSearch;
          hr = pHeader->GetMobs (NULL, &mobIter);
          check(hr); // display error message
        }
        if (SUCCEEDED(hr)) // EnumAAFAllMobs && GetNumMobs SUCCEEDED
        {
          aafUInt32 n = 0;


          for(n = 0; n < numMobs; n++)
          {
            // Buffer for string version of each object's class id.
            IAAFMob *aMob = NULL;
            aafWChar name[500], slotName[500];
            char chName[1000], chMobID[MAX_CLSID_BUFFER];
            aafNumSlots_t  numSlots;
            aafMobID_t    mobID = {{0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0,
				{0,0,0,{0,0,0,0,0,0,0,0}}};
            aafSlotID_t    trackID;
            aafRational_t  rate;

            hr = mobIter->NextOne (&aMob);
            check(hr); // display error message
            if (SUCCEEDED(hr))
            {
              IAAFSourceMob      *smob = NULL;

              hr = aMob->GetName (name, sizeof(slotName));
              check(hr); // display error message
              if (SUCCEEDED(hr))
                convert(chName, sizeof(chName), name);
              else
                strcpy(chName, "<mob name not available>");

              hr = aMob->GetMobID (&mobID);
              check(hr); // display error message
              if (SUCCEEDED(hr))
                formatMobID(chMobID, MAX_CLSID_BUFFER, &mobID);
              else
                strcpy(chMobID, "<mob id not available>");
            
              printf("Mob %d: (ID %s) is named '%s'\n", n, chMobID, chName);

              hr = aMob->CountSlots (&numSlots);
              check(hr); // display error message
              if (FAILED(hr))
                numSlots = 0;
              printf("Found %d slots\n", numSlots);

              hr = aMob->QueryInterface (IID_IAAFSourceMob, (void **)&smob);
              check(hr); // display error message
              if(SUCCEEDED(hr))
              {
                IAAFEssenceDescriptor  *essenceDesc = NULL;
                aafUInt32 numLocators;


                hr = smob->GetEssenceDescriptor(&essenceDesc);
                check(hr); // display error message
                if (SUCCEEDED(hr))
                {
                  IAAFFileDescriptor *fileDesc = NULL;


                  hr = essenceDesc->QueryInterface (IID_IAAFFileDescriptor, (void **)&fileDesc);
                  check(hr); // display error message
                  if(SUCCEEDED(hr))
                  {
                    hr = fileDesc->GetSampleRate(&rate);
                    check(hr); // display error message
                    if(SUCCEEDED(hr))
                    {
                      printf("    It is a file source mob of sample rate %d/%d.\n",
                             rate.numerator, rate.denominator);
                    }
                    // cleanup references...
                    fileDesc->Release();
                    fileDesc = NULL;
                  }
                  else
                    printf("    It is a source mob, but not a file source mob\n");

                  numLocators = 0;
                  hr = essenceDesc->CountLocators(&numLocators);
                  check(hr); // display error message
                  if(SUCCEEDED(hr))
                  {
                    printf ("    It has %d locator%s attached.\n",
                            numLocators,
                            numLocators==1 ? "" : "s");
                  }

                  // cleanup references...
                  essenceDesc->Release();
                  essenceDesc = NULL;
                }

                smob->Release();
                smob = NULL;
              }

              if(numSlots != 0)
              {
                IEnumAAFMobSlots  *slotIter = NULL;


                hr = aMob->GetSlots(&slotIter);
                check(hr); // display error message
                if(SUCCEEDED(hr))
                {
                  IAAFMobSlot    *slot = NULL;
                  aafUInt32 s;


                  for(s = 0; s < numSlots; s++)
                  {
                    hr = slotIter->NextOne (&slot);
                    check(hr); // display error message
                    if(SUCCEEDED(hr))
                    {
                      check(slot->GetName (slotName, sizeof(slotName)));
                      check(slot->GetSlotID(&trackID));
                      convert(chName, sizeof(chName), slotName);
                      printf("    Slot %d: (ID %d), is named '%s'\n",
                            s, trackID, chName);
    
                      // cleanup references
                      slot->Release();
                      slot = NULL;
                    } // NextOne
                  } // for

                  // cleanup references
                  slotIter->Release();
                  slotIter = NULL;
                } // SUCCEEDED(EnumAAFAllMobSlots)
              } // if(numSlots != 0)

              // cleanup references
              aMob->Release();
              aMob = NULL;
            }
          }


          // cleanup references
          mobIter->Release();
          mobIter = NULL;
        } // EnumAAFAllMobs && GetNumMobs SUCCEEDED
        
        pDictionary->Release();
        pDictionary = NULL;
      }

      pHeader->Release();
      pHeader = NULL;
    }

    check(pFile->Close());

    pFile->Release();
    pFile = NULL;

  }
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ComClientTestAAF";

static void CreateAAFFile(const aafWChar * pFileName)
{
  IAAFFile *          pFile = NULL;
  IAAFHeader *        pHeader = NULL;
  IAAFDictionary *pDictionary = NULL;
  aafProductIdentification_t  ProductInfo;
 
  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);

  // Create a new file...
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
  
  check(AAFFileOpenNewModifyEx(pFileName, &kAAFFileKind_Aaf4KBinary, 0, &ProductInfo, &pFile));
  check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));
   
//Make the first mob
  IAAFMob            *pMob = NULL;
  long  test;
  const aafWChar    *names[5] = { L"FOOBAR1", L"FOOBAR2", L"FOOBAR3", L"FOOBAR4", L"FOOBAR5" };
  aafRational_t  editRate = { 2997, 100 };
  IAAFTimelineMobSlot    *newSlot = NULL;
  IAAFSegment    *seg = NULL;
  IAAFSourceClip  *sclp = NULL;
  IAAFSourceMob  *smob = NULL;
  aafInt32    testSlot;
  const aafWChar    *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };
  IAAFClassDef  *classDef = NULL;
  IAAFFileDescriptor  *fileDesc = NULL;
  IAAFEssenceDescriptor *essenceDesc = NULL;
  aafRational_t  audioRate = { 44100, 1 };
  IAAFLocator    *pLocator = NULL;
  IAAFComponent*		pComponent = NULL;
  IAAFAIFCDescriptor*			pAIFCDesc = NULL;

  for(test = 0; test < 5; test++)
  {
     // Create a source Mob with a FileDescriptor attached
    check(pDictionary->LookupClassDef(AUID_AAFSourceMob, &classDef));
    check(classDef->CreateInstance(IID_IAAFSourceMob, (IUnknown **)&smob));
    check(smob->QueryInterface (IID_IAAFMob, (void **)&pMob));
    check(pMob->SetMobID( TEST_MobID[ test ] ));
    check(pMob->SetName(names[test]));
	classDef->Release();
	classDef = NULL;

	// Create a concrete subclass of FileDescriptor
    check(pDictionary->LookupClassDef(AUID_AAFAIFCDescriptor, &classDef));
    check(classDef->CreateInstance(IID_IAAFFileDescriptor, (IUnknown **)&fileDesc));
    check(fileDesc->SetSampleRate(audioRate));
    check(fileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&essenceDesc));
	check(fileDesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
	check(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
	classDef->Release();
	classDef = NULL;
	pAIFCDesc->Release();
	pAIFCDesc = NULL;

    {
      HRESULT stat;
      check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator, &classDef));
      stat = classDef->CreateInstance(IID_IAAFLocator, (IUnknown **)&pLocator);
      check (stat);
    }
	classDef->Release();
	classDef = NULL;

    check(fileDesc->SetSampleRate(audioRate));

    check(essenceDesc->AppendLocator (pLocator));
    check(smob->SetEssenceDescriptor(essenceDesc));

    // Add some slots
	IAAFDataDef *pDefPicture = NULL;
    check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pDefPicture));
    check(pDictionary->LookupClassDef(AUID_AAFSourceClip, &classDef));
    for(testSlot = 0; testSlot < 3; testSlot++)
    {
       check(classDef->CreateInstance(IID_IAAFSourceClip, (IUnknown **)&sclp));
		 check(sclp->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 check(pComponent->SetDataDef(pDefPicture));
		pComponent->Release();
		pComponent = NULL;
      check(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
      check(pMob->AppendNewTimelineSlot
			(editRate,
			 seg,
			 testSlot+1,
			 slotNames[testSlot],
			 0,
			 &newSlot));
      
      // Cleanup references...
      newSlot->Release();
      newSlot = NULL;
      
      seg->Release();
      seg = NULL;
      
      sclp->Release();
      sclp = NULL;
    }
	pDefPicture->Release();
	pDefPicture = NULL;
	classDef->Release();
	classDef = NULL;
    
    // Add the newly created and initialized Mob to the end of the mob index.
    check(pHeader->AddMob(pMob));
    
    
    // Cleanup references...
    pLocator->Release();
    pLocator = NULL;

    essenceDesc->Release();
    essenceDesc = NULL;

    fileDesc->Release();
    fileDesc = NULL;
    
    pMob->Release();
    pMob = NULL;
    
    smob->Release();
    smob = NULL;
  }
  
  // Cleanup
  pDictionary->Release();
  pDictionary = NULL;

	if (pComponent)
		pComponent->Release();

  pHeader->Release();
  pHeader = NULL;
  check(pFile->Save());
  check(pFile->Close());
  if (pFile)
    pFile->Release();

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
	  fprintf(stderr, "  Windows    - $PATH\n");
	  fprintf(stderr, "  Unix/Linux - $LD_LIBRARY_PATH\n");
	  exit(hr);
	}
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};


int main(int argc, char *argv[])
{
  CAAFInitialize aafInit;

  const aafWChar * pwFileName = L"Foo.aaf";
  const char * pFileName = "Foo.aaf";

  printf("***Creating file %s\n", pFileName);
  CreateAAFFile(pwFileName);
  printf("***Re-opening file %s\n", pFileName);
  ReadAAFFile(pwFileName);
  
  printf("Done\n");
  return(0);
}

