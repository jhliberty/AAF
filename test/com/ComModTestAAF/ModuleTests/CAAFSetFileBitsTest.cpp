//=---------------------------------------------------------------------=
//
// $Id: CAAFSetFileBitsTest.cpp,v 1.9 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
#include <assert.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFRawStorage>  IAAFRawStorageSP;
typedef IAAFSmartPointer<IAAFFile>        IAAFFileSP;
typedef IAAFSmartPointer<IAAFRandomFile>  IAAFRandomFileSP;
typedef IAAFSmartPointer<IAAFSetFileBits> IAAFSetFileBitsSP;

// Required function prototype.
extern "C" HRESULT CAAFSetFileBits_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

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

static HRESULT SetFileBitsTest ()
{
  // Create a memory raw storage and a file on it, to be opened for
  // reading.  We'll use this to get the SetFileBits to be tested.
  IAAFRawStorageSP pReadStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_read,
								&pReadStg));
  IAAFFileSP pReadFile;
  checkResult
	(AAFCreateAAFFileOnRawStorage (pReadStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   &pReadFile));
  assert (pReadFile);
  checkExpression (0 != (IAAFFile*)pReadFile,
				   AAFRESULT_TEST_FAILED);
  
  IAAFRandomFileSP pReadRandFile;
  checkResult(pReadFile->QueryInterface(IID_IAAFRandomFile,
										(void **)&pReadRandFile));

  //
  // Cool.  Now we can get the SetFileBits interface to test.
  //

  IAAFSetFileBitsSP psfb;
  checkResult (pReadRandFile->SetFileBits (&psfb));

  //
  // test SetSize
  //
  HRESULT hr;
  const aafUInt64 kBig = (~0) - 0x7fffffff;

  // Test size too large
  hr = psfb->SetSize (kBig);
  checkExpression (AAFRESULT_DATA_SIZE == hr, AAFRESULT_TEST_FAILED);

  // Try to set a reasonable size
  checkResult (psfb->SetSize (100));

  //
  // test WriteAt
  //

  // null param
  hr = psfb->WriteAt (0, 1, 0);
  checkExpression (AAFRESULT_NULL_PARAM == hr, AAFRESULT_TEST_FAILED);

  // Attempt to write way off end
  aafUInt8 foo[100];
  hr = psfb->WriteAt (foo, sizeof (foo), kBig);
  checkExpression (AAFRESULT_DATA_SIZE == hr, AAFRESULT_TEST_FAILED);

  // try a couple of reasonable writes
  checkResult (psfb->WriteAt (foo, sizeof (foo), 0));
  checkResult (psfb->WriteAt (foo, sizeof (foo), 100));
  checkResult (psfb->WriteAt (foo, sizeof (foo), 10000));

  return AAFRESULT_SUCCESS;
}

HRESULT CAAFSetFileBits_test(
    testMode_t /* mode */,
    aafUID_t /* fileKind */,
    testRawStorageType_t /* rawStorageType */,
    aafProductIdentification_t /* productID */)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  try
	{
	  hr = SetFileBitsTest ();
	}
  catch (...)
	{
	  cerr << "CAAFSetFileBitsTest_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
