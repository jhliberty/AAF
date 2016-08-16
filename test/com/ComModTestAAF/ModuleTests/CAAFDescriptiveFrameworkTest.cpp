//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFDescriptiveFrameworkTest.cpp,v 1.4 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include "ModuleTest.h"

#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
#include "AAFResult.h"

// Test routine defined in CAAFDescriptiveMarkerTest.cpp
extern "C" HRESULT CAAFDescriptiveMarker_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

// Required function prototype.
extern "C" HRESULT CAAFDescriptiveFramework_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFDescriptiveFramework_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  // DescriptiveFramework is an abstract base class.  By itself, there
  // is nothing to test other than that a concrete instance of a
  // DescriptiveFramework can be correctly contained by a
  // DescriptiveMarker.  This is tested in
  // CAAFDescriptiveMarkerTest.cpp and there is little point in
  // repeating the code here.  The test is re-executed here to ensure
  // "ComModAAF AAFDescriptiveFramework" executes a meaningful test in
  // the event it is run on its own.

  return CAAFDescriptiveMarker_test( mode,
					    fileKind,
					    rawStorageType,
					    productID);
}
