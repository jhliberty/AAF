//=---------------------------------------------------------------------=
//
// $Id: CAAFPropertyValueTest.cpp,v 1.23 2009/06/01 11:47:13 stuart_hc Exp $ $Name: V116 $
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
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include <iostream>
using namespace std;
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


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


static HRESULT TestPropertyValue (
    testMode_t mode,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  // HRESULT hr = E_FAIL;
  long hr = E_FAIL;

  const size_t fileNameBufLen = 128;
  aafWChar testFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, testFileName );

  IAAFFile* pFile = NULL;
  if(mode == kAAFUnitTestReadWrite)
  {
  	RemoveTestFile (testFileName);
  	checkResult (CreateTestFile( testFileName, fileKind, rawStorageType, productID, &pFile ));
 	 assert (pFile);
 }
  else
  {
  	checkResult (AAFFileOpenExistingRead(testFileName, 0, &pFile));
 	 assert (pFile);
 }

  IAAFHeader * pHeader = NULL;
  hr = pFile->GetHeader (&pHeader);
  if (! SUCCEEDED (hr)) return hr;
  assert (pHeader);

  IAAFDictionary * pDict = NULL;
  hr = pHeader->GetDictionary (&pDict);
  if (! SUCCEEDED (hr)) return hr;
  assert (pDict);

  CAAFBuiltinDefs defs (pDict);

  // Let's try to do something interesting with a type definition
  IAAFTypeDefInt * pTypeDef = NULL;
  hr = defs.tdInt32()->QueryInterface (IID_IAAFTypeDefInt, (void **) &pTypeDef);
  if (! SUCCEEDED (hr)) return hr;
  assert (pTypeDef);

  if(mode == kAAFUnitTestReadWrite)
	{
		  // Now attempt to create invalid property values; check for errors.
	  const aafInt32 forty_two = 42;
	  IAAFPropertyValue * pv = NULL;
	  // This is what a correct one would look like; we're not ready for that yet:
	  // hr = pTypeDef->CreateValue (&forty_two, 4, &pv);

	  // set this to -1 to see if it gets modified
	  pv = (IAAFPropertyValue *) (-1);

	  // Try null pVal
	  hr = pTypeDef->CreateValue (NULL, 4, &pv);
	  if (AAFRESULT_NULL_PARAM != hr)
		return AAFRESULT_TEST_FAILED;
	  if ((IAAFPropertyValue *)(-1) != pv)
		return AAFRESULT_TEST_FAILED;

	  // Try valSize too large
	  hr = pTypeDef->CreateValue ((aafMemPtr_t) &forty_two, 8, &pv);
	  if (AAFRESULT_BAD_SIZE != hr)
		return AAFRESULT_TEST_FAILED;
	  if ((IAAFPropertyValue *)(-1) != pv)
		return AAFRESULT_TEST_FAILED;

	  // Now try correct one
	  pv = NULL;
	  hr = pTypeDef->CreateValue ((aafMemPtr_t) &forty_two, 4, &pv);
	  if (! SUCCEEDED (hr)) return hr;
	  if (! pv)
		return AAFRESULT_TEST_FAILED;

	  // That one worked; let's try one with a smaller init size (should
	  // also work)
	  pv->Release();
	  pv = NULL;
	  const aafInt16 fifty_seven = 57;
	  hr = pTypeDef->CreateValue ((aafMemPtr_t) &fifty_seven, 2, &pv);
	  if (! SUCCEEDED (hr)) return hr;
	  if (! pv)
		return AAFRESULT_TEST_FAILED;

	  // cool.  Now we should have a good property value whose value is 57.
	  // check GetType() for error condition
	  hr = pv->GetType (NULL);
	  if (AAFRESULT_NULL_PARAM != hr)
		return AAFRESULT_TEST_FAILED;

	  // this GetType() call should work, and return the original type def.
	  IAAFTypeDef * propType = NULL;
	  hr = pv->GetType (&propType);
	  if (! SUCCEEDED (hr)) return hr;
	  if (! propType)
		return AAFRESULT_TEST_FAILED;

	  // Convert both to IUnknown for comparison
	  IUnknown * pPropUnknown = NULL;
	  IUnknown * pTypeDefUnknown = NULL;
	  hr = propType->QueryInterface(IID_IUnknown, (void **)&pPropUnknown);
	  if (! SUCCEEDED (hr)) return hr;
	  assert (pPropUnknown);
	  hr = pTypeDef->QueryInterface(IID_IUnknown, (void **)&pTypeDefUnknown);
	  if (! SUCCEEDED (hr)) return hr;
	  assert (pTypeDefUnknown);

	  if (pPropUnknown != pTypeDefUnknown)
		return AAFRESULT_TEST_FAILED;
	  // Test IsDefinedType ()
	  // (Currently only returns true.)
	  aafBool b = kAAFFalse;
	  hr = pv->IsDefinedType (&b);
		if (! SUCCEEDED (hr)) return hr;
	  if (kAAFTrue != b)
		return AAFRESULT_TEST_FAILED;

	  pTypeDefUnknown->Release();
	  pPropUnknown->Release();
	  pTypeDef->Release();
	  propType->Release();
	  pv->Release();
	  pDict->Release();
	  pHeader->Release();
	  hr = pFile->Save();
	}
  if (! SUCCEEDED (hr)) return hr;
  hr = pFile->Close();
  if (! SUCCEEDED (hr)) return hr;
  pFile->Release();

  return AAFRESULT_SUCCESS;
}



extern "C" HRESULT CAAFPropertyValue_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFPropertyValue_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  	hr = TestPropertyValue(mode, fileKind, rawStorageType, productID);
	}
  catch (...)
	{
	  cerr << "CAAFPropertyValue_test..."
		   << "Caught general C++ exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
