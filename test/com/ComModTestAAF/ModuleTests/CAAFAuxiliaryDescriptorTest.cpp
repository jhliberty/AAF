//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFAuxiliaryDescriptorTest.cpp,v 1.7 2009/06/01 11:47:11 stuart_hc Exp $ $Name: V116 $
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

#include "ModuleTestsCommon.h"

#include "ModuleTest.h"

#include "AAF.h"
#include "AAFResult.h"
#include "AAFTypes.h"
#include "AAFStoredObjectIDs.h"
#include "AAFWideString.h"

#include <iostream>
using namespace std;

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  try {
    using namespace mtc;

    IAAFSmartPointer<IAAFHeader> pHeader;
    IAAFSmartPointer<IAAFDictionary> pDict;
    SimpleFilePointers filePointers;
    CreateSimpleAAFFile( pFileName, 
			 fileKind,
			 rawStorageType,
			 productID,
			 &filePointers );

    IAAFSmartPointer<IAAFSourceMob> spSourceMob = AddChainedSourceMob( &filePointers );

    // Finally... add the aux descriptor.
    IAAFSmartPointer<IAAFAuxiliaryDescriptor> pAuxDesc;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFAuxiliaryDescriptor,
							   IID_IAAFAuxiliaryDescriptor,
							   (IUnknown**)&pAuxDesc ) );
    
    CheckResult( pAuxDesc->Initialize() );
    CheckResult( pAuxDesc->SetMimeType( L"Marcel Marceau" ) );
    CheckResult( pAuxDesc->SetCharSet( L"csISO069French" ) );

    IAAFSmartPointer<IAAFEssenceDescriptor> pDesc;
    CheckResult( pAuxDesc->QueryInterface( IID_IAAFEssenceDescriptor, (void**)&pDesc ) );
    CheckResult( spSourceMob->SetEssenceDescriptor( pDesc ) );

    CheckResult( filePointers.pFile->Save() );
    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  try {
    using namespace mtc;

    SimpleFilePointers filePointers;
    ReadSimpleAAFFile( pFileName, &filePointers );

    // Get the source mob, and check that the RecordingDescriptor is
    // attached.
    IAAFSmartPointer<IAAFSourceClip> pSourceClip
      = GetSourceClipFromSlot( filePointers.pReferencedMasterMob, 1 );

    IAAFSmartPointer<IAAFMob> pMob;
    CheckResult( pSourceClip->ResolveRef( &pMob ) );
    
    IAAFSmartPointer<IAAFSourceMob> pSourceMob;
    CheckResult( pMob->QueryInterface( IID_IAAFSourceMob, (void**)&pSourceMob ) );

    IAAFSmartPointer<IAAFEssenceDescriptor> pDesc;
    CheckResult( pSourceMob->GetEssenceDescriptor( &pDesc ) );

    // Finally...
    IAAFSmartPointer<IAAFAuxiliaryDescriptor> pAuxDesc;
    CheckResult( pDesc->QueryInterface( IID_IAAFAuxiliaryDescriptor, (void**)&pAuxDesc ));

    aafCharacter stringBuf[15];
    CheckResult( pAuxDesc->GetMimeType( stringBuf, sizeof(stringBuf)) );
    CheckExpression( ::wcscmp( stringBuf, L"Marcel Marceau" ) == 0, AAFRESULT_TEST_FAILED );
    
    CheckResult( pAuxDesc->GetCharSet( stringBuf, sizeof(stringBuf) ) );
    CheckExpression( ::wcscmp( stringBuf, L"csISO069French" ) == 0, AAFRESULT_TEST_FAILED );

    CheckResult( filePointers.pFile->Close() );

    //test the get buf length methods
    aafUInt32 bufSize;

    if(pAuxDesc->GetMimeTypeBufLen(&bufSize) != AAFRESULT_SUCCESS)
      CheckResult(AAFRESULT_TEST_FAILED);
    
    //ensure the sizes match
    if(bufSize != sizeof(stringBuf))
      CheckResult(AAFRESULT_TEST_FAILED);

    if(pAuxDesc->GetCharSetBufLen(&bufSize) != AAFRESULT_SUCCESS)
      CheckResult(AAFRESULT_TEST_FAILED);

    //ensure the sizes match
    if(bufSize != sizeof(stringBuf))
      CheckResult(AAFRESULT_TEST_FAILED);

  }
  catch( const AAFRESULT& hr ) {
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

// Required function prototype.
extern "C" HRESULT CAAFAuxiliaryDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFAuxiliaryDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

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
		cerr << "CAAFAuxiliaryDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
