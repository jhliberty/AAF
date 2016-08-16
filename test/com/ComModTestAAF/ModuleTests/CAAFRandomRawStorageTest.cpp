//=---------------------------------------------------------------------=
//
// $Id: CAAFRandomRawStorageTest.cpp,v 1.29 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
#include "AAFFileMode.h"
#include "AAFDefUIDs.h"
#include "AAFFileKinds.h"

#include "ModuleTest.h"
#include "CAAFBuiltinDefs.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "AAFWideString.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFRawStorage>  IAAFRawStorageSP;
typedef IAAFSmartPointer<IAAFRandomFile>  IAAFRandomFileSP;
typedef IAAFSmartPointer<IAAFSetFileBits> IAAFSetFileBitsSP;
typedef IAAFSmartPointer<IAAFGetFileBits> IAAFGetFileBitsSP;


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

// Function to compare COM interface pointers, taken from
// CAAFTypeDefFixedArrayTest.cpp.
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
  IAAFSmartPointer<IUnknown>    spUnk1, spUnk2;
	
  checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
  checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
  if (spUnk1 == spUnk2)
	return kAAFTrue;
  else
	return kAAFFalse;
}

static HRESULT checkModeFlag (aafUInt32 modeFlags,
							  HRESULT expectedResult)
{
  // Check mod flags
  aafProductIdentification_t	ProductInfo = { 0 };
  HRESULT temphr;
  IAAFFile * pFile = 0;
  temphr = CreateTestFile( L"foo",
                           aafFileKindAafSSBinary,
                           kAAFNamedFile,
                           modeFlags,
                           ProductInfo,
                           &pFile );
  if (expectedResult != temphr)
	{
	  return AAFRESULT_TEST_FAILED;
	}
  if (pFile)
	{
	  return AAFRESULT_TEST_FAILED;
	}

  return AAFRESULT_SUCCESS;
}


static HRESULT checkModeFlags ()
{
  HRESULT temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_REVERTABLE,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_UNBUFFERED,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_RECLAIMABLE,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_USE_LARGE_SS_SECTORS,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_CLOSE_FAIL_DIRTY,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_DEBUG0_ON,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_DEBUG1_ON,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  aafUInt32 i;
  for (i = 4; i < 28; i++)
	{
	  temphr = checkModeFlag ((1 << i),
							  AAFRESULT_BAD_FLAGS);
	  if (AAFRESULT_FAILED (temphr)) return temphr;
	}
  return AAFRESULT_SUCCESS;
}


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
 0x13, 0x00, 0x00, 0x00,
 {0x3920c87a, 0xa466, 0x11d4, {0x93, 0x6f, 0x00, 0x06, 0x29, 0xee, 0xa6, 0x5f}}};




//
// Functions to open files using standard disk API
//
static HRESULT localOpenFileDiskWrite
  (const aafWChar * pFileName,
   aafUID_constref fileKind,
   aafProductIdentification_constref productID,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  if( memcmp( &fileKind, &aafFileKindAafSSBinary, sizeof(aafUID_t) ) != 0 )
  {
    return AAFFileOpenNewModifyEx(pFileName, &fileKind, 0, const_cast<aafProductIdentification_t*>(&productID), ppFile);
  }
  else
  {
    return AAFFileOpenNewModify(pFileName, 0, const_cast<aafProductIdentification_t*>(&productID), ppFile);
  }
}


static HRESULT localOpenFileDiskRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  return AAFFileOpenExistingRead(pFileName, 0, ppFile);
}


//
// Functions to open files using RawStorage API, using raw storages on
// disk
//
static HRESULT localOpenFileDiskStgWrite
  (const aafWChar * pFileName,
   aafUID_constref fileKind,
   aafProductIdentification_constref productID,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a disk raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageDisk (pFileName,
							  kAAFFileExistence_new,
							  kAAFFileAccess_modify,
							  &pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_modify,
								   &fileKind,
								   0,
								   &productID,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}


static HRESULT localOpenFileDiskStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a disk raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageDisk (pFileName,
							  kAAFFileExistence_existing,
							  kAAFFileAccess_read,
							  &pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}

//
// Functions to open files using RawStorage API, using cached raw storages on
// disk
//
const aafUInt32 CDRS_PAGE_COUNT =   64;
const aafUInt32	CDRS_PAGE_SIZE  = 4096;

static HRESULT localOpenFileCachedDiskStgWrite
  (const aafWChar * pFileName,
   aafUID_constref fileKind,
   aafProductIdentification_constref productID,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a cached disk raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageCachedDisk (pFileName,
							        kAAFFileExistence_new,
							        kAAFFileAccess_modify,
									CDRS_PAGE_COUNT,
									CDRS_PAGE_SIZE,
							        &pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_modify,
								   &fileKind,
								   0,
								   &productID,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}

static HRESULT localOpenFileCachedDiskStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a cached disk raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageCachedDisk (pFileName,
							        kAAFFileExistence_existing,
							        kAAFFileAccess_read,
									CDRS_PAGE_COUNT,
									CDRS_PAGE_SIZE,
							        &pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}



//
// Functions to open files using RawStorage API, using raw storages in
// memory
//
static HRESULT localOpenFileMemStgWrite
  (const aafWChar * /* pFileName */,
   aafUID_constref fileKind,
   aafProductIdentification_constref productID,
   IAAFFile ** ppFile)
{
  assert (ppFile);

  // Create a mem raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_modify,
								&pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_modify,
								   &fileKind,
								   0,
								   &productID,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}


static HRESULT localOpenFileMemStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a mem raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_read,
								&pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);

  IAAFRandomFileSP pRandFile;
  HRESULT hr;
  hr = (*ppFile)->QueryInterface(IID_IAAFRandomFile,
								 (void **)&pRandFile);
  if (FAILED (hr)) return hr;
  
  IAAFSetFileBitsSP psfb;
  hr = pRandFile->SetFileBits (&psfb);
  if (FAILED (hr)) return hr;

  char cFileName[FILENAME_MAX];
#ifdef _DEBUG
  size_t status =
#endif
  wcstombs(cFileName, pFileName, FILENAME_MAX);
  assert (status != (size_t)-1);

  FILE * f = fopen (cFileName, "rb");
  assert (f);

  aafUInt64 position = 0;

  while (1)
	{
	  aafUInt8 c;
	  size_t numRead;
	  numRead = fread (&c, 1, 1, f);
	  if (1 != numRead)
		break;
	  hr = psfb->SetSize (position+1);
	  if (FAILED (hr)) return hr;
	  hr = psfb->WriteAt (&c, 1, position++);
	  if (FAILED (hr)) return hr;
	}
  fclose (f);

  return (*ppFile)->Open();
}



//
// We'll implement our own raw storage, to support the IAAFRawStorage
// and IAAFRandomRawStorage interfaces.
//
class CCustomRawStorage :
  public IAAFRawStorage,
  public IAAFRandomRawStorage
{
public:
  CCustomRawStorage (const aafWChar * pFileName,
					 aafFileAccess_e access,
					 aafFileExistence_e exist)
	: _refCnt (1),
	  _access (access),
	  _file (0),
	  _extent (0)
  { assert (pFileName);
    char cFileName[FILENAME_MAX];
#ifdef _DEBUG
	size_t status =
#endif
	wcstombs(cFileName, pFileName, FILENAME_MAX);
	assert (status != (size_t)-1);
	if (kAAFFileAccess_write == access)
	  _file = fopen (cFileName, "wb");
	else if (kAAFFileAccess_read == access)
	  {
		assert (kAAFFileExistence_existing == exist);
		_file = fopen (cFileName, "rb");
	  }
	else if (kAAFFileAccess_modify == access)
	  {
		switch (exist)
		  {
		  case kAAFFileExistence_existing:
			_file = fopen (cFileName, "r+b");
			break;
		  case kAAFFileExistence_new:
			_file = fopen (cFileName, "w+b");
			break;
		  default:
			assert (0);
		  }
	  }
	else
	  assert (0);
	assert (_file); }

  virtual ~CCustomRawStorage ()
  { if (_file) { fclose (_file); _file = 0; } }

  // IUnknown overrides
  HRESULT STDMETHODCALLTYPE
    QueryInterface (REFIID iid, void ** ppIfc)
    { if (! ppIfc) return AAFRESULT_NULL_PARAM;
	  if (equalUID (iid, IID_IUnknown))
		{ IUnknown * runk = (IAAFRawStorage*) this;
		*ppIfc = (void*) runk;
		AddRef ();
		return AAFRESULT_SUCCESS; }
	  else if (equalUID (iid, IID_IAAFRawStorage))
		{ IAAFRawStorage * rrs = this;
		*ppIfc = (void*) rrs;
		AddRef ();
		return AAFRESULT_SUCCESS; }
	  else if (equalUID (iid, IID_IAAFRandomRawStorage))
		{ IAAFRandomRawStorage * rrrs = this;
		*ppIfc = (void*) rrrs;
		AddRef ();
		return AAFRESULT_SUCCESS; }
	  else
		return E_NOINTERFACE; }

  aafUInt32 STDMETHODCALLTYPE
    AddRef () {return ++_refCnt;}

  aafUInt32 STDMETHODCALLTYPE
    Release ()
    { aafUInt32 r = --_refCnt;
	  if (! r) delete this;
	  return r; }

  // IAAFRawStorage overrides
  HRESULT STDMETHODCALLTYPE
    IsReadable (aafBoolean_t * pResult)
    { if (!pResult) return AAFRESULT_NULL_PARAM;
	  *pResult = ((kAAFFileAccess_read == _access) ||
				  (kAAFFileAccess_modify == _access)) ?
		kAAFTrue : kAAFFalse;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    Read (aafMemPtr_t buf,
		  aafUInt32 bufSize,
		  aafUInt32 *pNumRead)
    { if (! _file) return AAFRESULT_NOT_INITIALIZED;
	  if (! pNumRead) return AAFRESULT_NULL_PARAM;
	  aafBoolean_t readable = kAAFFalse;
	  IsReadable (&readable);
	  if (! readable) return AAFRESULT_NOT_READABLE;
	  size_t actualByteCount = fread(buf, 1, bufSize, _file);
	  *pNumRead = actualByteCount;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    IsWriteable (aafBoolean_t * pResult)
    { if (!pResult) return AAFRESULT_NULL_PARAM;
	  *pResult = ((kAAFFileAccess_write == _access) ||
				  (kAAFFileAccess_modify == _access)) ?
		kAAFTrue : kAAFFalse;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    Write (aafMemConstPtr_t buf,
		   aafUInt32 bufSize,
		   aafUInt32 *pNumWritten)
    { if (! _file) return AAFRESULT_NOT_INITIALIZED;
	  if (! pNumWritten) return AAFRESULT_NULL_PARAM;
	  aafBoolean_t writeable = kAAFFalse;
	  IsWriteable (&writeable);
	  if (! writeable) return AAFRESULT_NOT_WRITEABLE;
	  size_t actualByteCount = fwrite(buf, 1, bufSize, _file);
	  *pNumWritten = actualByteCount;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    Synchronize ()
  { if (! _file) return AAFRESULT_NOT_INITIALIZED;
	aafBoolean_t writeable = kAAFFalse;
	IsWriteable (&writeable);
    if (writeable) fflush(_file);
    return AAFRESULT_SUCCESS; }

  // IAAFRandomRawStorage overrides
  HRESULT STDMETHODCALLTYPE
    ReadAt (aafUInt64 position,
			aafMemPtr_t buf,
			aafUInt32 bufSize,
			aafUInt32 *pNumRead)
  { pvtSetPosition (position);
    return Read (buf, bufSize, pNumRead); }

  HRESULT STDMETHODCALLTYPE
    WriteAt(aafUInt64 position,
			aafMemConstPtr_t buf,
			aafUInt32 bufSize,
			aafUInt32 *pNumWritten)
  { pvtSetPosition (position);
    return Write (buf, bufSize, pNumWritten); }

  HRESULT STDMETHODCALLTYPE
    GetSize (aafUInt64 * pSize)
    { if (! pSize) return AAFRESULT_NULL_PARAM;
	  long int oldPosition = ftell(_file);
#ifdef _DEBUG
	  long int seekStatus =
#endif
	  fseek(_file, 0, SEEK_END);
	  assert (seekStatus == 0);
	  long int position = ftell(_file);
	  *pSize = position;
	  pvtSetPosition (oldPosition);
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    IsExtendable (aafBoolean_t * pResult)
      { if (! pResult) return AAFRESULT_NULL_PARAM;
	    *pResult = kAAFTrue;
		return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    GetExtent (aafUInt64 * pExtent)
    { if (! pExtent) return AAFRESULT_NULL_PARAM;
	  *pExtent = _extent;
	return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    SetExtent (aafUInt64 extent)
    { // Our hack implementation depends on ANSI file stuff to extend
	  // the file for us.  Simply cache the requested extent, and feed
	  // it back to the client if requested (via GetExtent()).
	  _extent = extent;
	  return AAFRESULT_SUCCESS; }

private:
  void pvtSetPosition (aafUInt64 position)
  {
#ifdef _DEBUG
    int seekStatus =
#endif
    fseek(_file, static_cast<long>(position), SEEK_SET);
    assert (0 == seekStatus);
  }

  int equalUID(const GUID & a, const GUID & b)
  { return (0 == memcmp((&a), (&b), sizeof (aafUID_t))); }

  aafUInt32       _refCnt;
  aafFileAccess_e _access;  
  FILE *          _file;
  aafUInt64       _extent;
};


//
// Functions to open files using RawStorage API, using raw storages
// implemented by client (in this case, implemented by our own
// CCustomRawStorage).
//
static HRESULT localOpenFileCustomStgWrite
  (const aafWChar * pFileName,
   aafUID_constref fileKind,
   aafProductIdentification_constref productID,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a custom raw storage.
  IAAFRawStorage * pStg =
	new CCustomRawStorage (pFileName,
						   kAAFFileAccess_modify,
						   kAAFFileExistence_new);

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_modify,
								   &fileKind,
								   0,
								   &productID,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  pStg->Release ();
  return (*ppFile)->Open();
}


static HRESULT localOpenFileCustomStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a custom raw storage.
  IAAFRawStorage * pStg =
	new CCustomRawStorage (pFileName,
						   kAAFFileAccess_read,
						   kAAFFileExistence_existing);

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  pStg->Release ();
  return (*ppFile)->Open();
}

//
// Functions to open files using RawStorage API, using raw storages
// implemented by client (in this case, implemented by our own
// CCustomRawStorage) and which have caching added using the AAF toolkit
// function AAFCreateRawStorageCached ().
//
static HRESULT localOpenFileCachedCustomStgWrite
  (const aafWChar * pFileName,
   aafUID_constref fileKind,
   aafProductIdentification_constref productID,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a custom raw storage.
  IAAFRawStorage * pStg =
	new CCustomRawStorage (pFileName,
						   kAAFFileAccess_modify,
						   kAAFFileExistence_new);

  // Add caching
  IAAFRawStorage* pCachedStg = 0;
  checkResult
    (AAFCreateRawStorageCached (pStg,
                                16,
                                4 * 1024,
                                &pCachedStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pCachedStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_modify,
								   &fileKind,
								   0,
								   &productID,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  pCachedStg->Release();
  pStg->Release ();
  return (*ppFile)->Open();
}


static HRESULT localOpenFileCachedCustomStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a custom raw storage.
  IAAFRawStorage * pStg =
	new CCustomRawStorage (pFileName,
						   kAAFFileAccess_read,
						   kAAFFileExistence_existing);

  // Add caching
  IAAFRawStorage* pCachedStg = 0;
  checkResult
    (AAFCreateRawStorageCached (pStg,
                                16,
                                4 * 1024,
                                &pCachedStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pCachedStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  pCachedStg->Release();
  pStg->Release ();
  return (*ppFile)->Open();
}



//
// Use for both standard disk api, and raw storage on disk API.
//
static HRESULT
localCloseDisk (const aafWChar *,
				IAAFFile ** ppFile)
{
  assert (ppFile);
  assert (*ppFile);
  return (*ppFile)->Close ();
}


//
// Use for raw storage in memory, to write out the memory bits to
// disk.
//
static HRESULT
localCloseFileMemStgWrite (const aafWChar * pFileName,
						   IAAFFile ** ppFile)
{
  HRESULT hr;
  assert (pFileName);
  assert (ppFile);
  assert (*ppFile);
  hr = (*ppFile)->Close ();
  if (FAILED (hr)) return hr;

  IAAFRandomFileSP pRandFile;
  hr = (*ppFile)->QueryInterface(IID_IAAFRandomFile,
								 (void **)&pRandFile);
  if (FAILED (hr)) return hr;
  
  IAAFGetFileBitsSP pgfb;
  hr = pRandFile->GetFileBits (&pgfb);
  if (FAILED (hr)) return hr;

  aafUInt64 size = 0;
  hr = pgfb->GetSize (&size);
  if (FAILED (hr)) return hr;

  char cFileName[FILENAME_MAX];
#ifdef _DEBUG
  size_t status =
#endif
  wcstombs(cFileName, pFileName, FILENAME_MAX);
  assert (status != (size_t)-1);

  FILE * f = fopen (cFileName, "wb");
  assert (f);

  aafUInt64 position;
  for (position = 0; position < size; position ++)
	{
	  aafUInt8 c;
	  hr = pgfb->ReadAt (&c, 1, position);
	  if (FAILED (hr)) return hr;
	  fwrite (&c, 1, 1, f);
	}
  fclose (f);

  return S_OK;
}


//
// Table (and types it needs) to drive selection of open/close
// functions for various types of storages.
//
typedef HRESULT (*fileCrtnFunc_t)(const aafWChar *,
								  aafUID_constref,
								  aafProductIdentification_constref,
								  IAAFFile**);
typedef HRESULT (*fileMgmtFunc_t)(const aafWChar *,
								  IAAFFile**);
//
struct fileInfo_t
{
  enum
  {
	kStgTypeDisk
	, kStgTypeDiskStg
	, kStgTypeCachedDiskStg
	, kStgTypeMemStg
	, kStgTypeCustomStg
	, kStgTypeCachedCustomStg
  }              storageType;
  const wchar_t * fileName;
  const char *    description;
  fileCrtnFunc_t  openWriteFunc;
  fileMgmtFunc_t  openReadFunc;
  fileMgmtFunc_t  closeWriteFunc;
  fileMgmtFunc_t  closeReadFunc;

};
//
static const fileInfo_t sFileDescriptions[] =
  {
	{
	  fileInfo_t::kStgTypeDisk,
	  L"AAFRandomRawStgTest",
	  "Disk",
	  localOpenFileDiskWrite,
	  localOpenFileDiskRead,
	  localCloseDisk,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeDiskStg,
	  L"AAFRandomRawStgTestDisk",
	  "Disk Storage",
	  localOpenFileDiskStgWrite,
	  localOpenFileDiskStgRead,
	  localCloseDisk,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeCachedDiskStg,
	  L"AAFRandomRawStgTestCDisk",
	  "Cached Disk Storage",
	  localOpenFileCachedDiskStgWrite,
	  localOpenFileCachedDiskStgRead,
	  localCloseDisk,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeMemStg,
	  L"AAFRandomRawStgTestMem",
	  "Mem Storage",
	  localOpenFileMemStgWrite,
	  localOpenFileMemStgRead,
	  localCloseFileMemStgWrite,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeCustomStg,
	  L"AAFRandomRawStgTestCstm",
	  "Custom Storage",
	  localOpenFileCustomStgWrite,
	  localOpenFileCustomStgRead,
	  localCloseDisk,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeCachedCustomStg,
	  L"AAFRandomRawStgTestCCstm",
	  "Cached Custom Storage",
	  localOpenFileCachedCustomStgWrite,
	  localOpenFileCachedCustomStgRead,
	  localCloseDisk,
	  localCloseDisk
	}
  };
//
const int kNumDescriptions =
  sizeof (sFileDescriptions) / sizeof (sFileDescriptions[0]);



static HRESULT CreateAAFFile(
    fileInfo_t info,
    const aafWChar* pFileName,
    aafUID_constref fileKind,
    aafProductIdentification_constref productID)
{
  IAAFFile *       pFile = NULL;
  bool             bFileOpen = false;
  IAAFHeader *     pHeader = NULL;
  IAAFDictionary * pDictionary = NULL;
  IAAFMob *        pMob = NULL;
  HRESULT  	       hr = S_OK;

  try 
	{
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);

	  // Determine the file encoding to be used.
	  aafUID_t effectiveFileKind = EffectiveTestFileEncoding( fileKind );

	  // Check for illegal mode flags.
	  checkModeFlags ();

	  checkResult
		(info.openWriteFunc (pFileName, effectiveFileKind, productID, &pFile));
	  bFileOpen = true;
  
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF
	  // objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

      // Make sure the header returns us the same dictionary as the
	  // file
	  IAAFDictionarySP pDictionaryFromHeader;
	  checkResult(pHeader->GetDictionary(&pDictionaryFromHeader));
	  checkExpression(AreUnksSame(pDictionary,
								  pDictionaryFromHeader)==kAAFTrue);

	  CAAFBuiltinDefs defs (pDictionary);
 	  
	  // Create a concrete subclass of Mob
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));
    
	  // Initialize the Mob properties
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(MOB_NAME_TEST));

	  // Add the source mob into the tree
	  checkResult(pHeader->AddMob(pMob));

	  // Attempt to save the file.
	  checkResult(pFile->Save());

	  // Attempt to close the file.
	  checkResult(info.closeWriteFunc(pFileName, &pFile));
	  bFileOpen = false;
    }
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}


  // Cleanup and return
  if (pMob)
	pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
	pHeader->Release();
			
  if (pFile)
	{	// Close file
	  if (bFileOpen)
		{
		  pFile->Save();
		  info.closeWriteFunc(pFileName, &pFile);
		}
	  pFile->Release();
	}

  return hr;
}

static HRESULT ReadAAFFile(fileInfo_t info, wchar_t * filename)
{
  IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *				pHeader = NULL;
  IEnumAAFMobs *mobIter = NULL;
  IAAFMob			*pMob = NULL;
  aafNumSlots_t				numMobs, n;
  HRESULT						hr = S_OK;
  aafWChar					name[500];
  aafMobID_t					mobID;
  aafFileRev_t					testRev;
	  
  try
	{
	  // Open the file
	  checkResult
		(info.openReadFunc (filename, &pFile));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->GetMobs (NULL, &mobIter));
	  for(n = 0; n < numMobs; n++)
		{
		  checkResult(mobIter->NextOne (&pMob));
		  checkResult(pMob->GetName (name, sizeof(name)));
		  checkResult(pMob->GetMobID (&mobID));
		  checkExpression(wcscmp( name,
								  MOB_NAME_TEST) == 0,
						  AAFRESULT_TEST_FAILED);
		  checkExpression(memcmp(&mobID,
								 &TEST_MobID,
								 sizeof(mobID)) == 0,
						  AAFRESULT_TEST_FAILED);

		  pMob->Release();
		  pMob = NULL;
		}

	  mobIter->Release();
	  mobIter = NULL;

	  checkResult(pFile->GetRevision(&testRev));
	  checkExpression(kAAFRev2 == testRev, AAFRESULT_TEST_FAILED);

	  checkResult(info.closeReadFunc(filename, &pFile));
	  bFileOpen = false;

	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}

  // Cleanup and return
  if (mobIter)
    mobIter->Release();

  if (pMob)
	pMob->Release();

  if (pHeader)
	pHeader->Release();
			
  if (pFile)
	{	// Close file
	  if (bFileOpen)
		info.closeReadFunc(filename, &pFile);
 		pFile->Release();
	}

	return hr;
}

// Required function prototype.
extern "C" HRESULT CAAFRandomRawStorage_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

HRESULT CAAFRandomRawStorage_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t /*rawStorageType*/,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  if (memcmp(&fileKind, &aafFileKindAafSSBinary, sizeof(aafUID_t)) == 0)
  	return AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  for (int wType = 0;
		   wType < kNumDescriptions;
		   wType++)
		{
		  AAFRESULT subtest_result = AAFRESULT_SUCCESS;
		  const size_t fileNameBufLen = 128;
		  aafWChar fileName[ fileNameBufLen ] = L"";
		  GenerateTestFileName( sFileDescriptions[wType].fileName, fileKind, fileNameBufLen, fileName );

		  if (mode == kAAFUnitTestReadWrite)
			{
			  cout << "\tWriting "
				   << sFileDescriptions[wType].description
				   << endl;
			  subtest_result = CreateAAFFile (sFileDescriptions[wType], fileName, fileKind, productID);
			}
		  else
			subtest_result = AAFRESULT_SUCCESS;
		  if (AAFRESULT_SUCCEEDED (subtest_result))
			{
			  for (int rType = 0;
				   rType < kNumDescriptions;
				   rType++)
				{
				  cout << "\t\tReading "
					   << sFileDescriptions[rType].description
					   << endl;
				  subtest_result = ReadAAFFile (sFileDescriptions[rType],
									fileName);
				  if (AAFRESULT_FAILED (subtest_result))
					break;
				}
			}



		  // Update the module test status.
		  if (AAFRESULT_FAILED (subtest_result))
			{
			  if (subtest_result == AAFRESULT_NOT_IMPLEMENTED)
				{
				  hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
				}
			  else
				{
				  hr = subtest_result;
                                  break;
				}
			}
		}
	}
  catch (...)
	{
	  cerr << "CAAFRandomRawStorage_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}
  return hr;
}
