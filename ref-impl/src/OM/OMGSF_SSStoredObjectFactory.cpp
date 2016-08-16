//=---------------------------------------------------------------------=
//
// $Id: OMGSF_SSStoredObjectFactory.cpp,v 1.5 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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

// @doc OMINTERNAL

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMGSF_SSStoredObjectFactory.h"

#if defined(OM_USE_GSF_SS)

#include "OMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"
#include "OMGSFStructuredStorage.h"
#include "OMExceptions.h"

static void checkStatus(HRESULT status);

  // @mfunc Constructor.
OMGSF_SSStoredObjectFactory::OMGSF_SSStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMSSStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMGSF_SSStoredObjectFactory::OMGSF_SSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMGSF_SSStoredObjectFactory::~OMGSF_SSStoredObjectFactory(void)
{
  TRACE("OMGSF_SSStoredObjectFactory::~OMGSF_SSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMGSF_SSStoredObjectFactory>.
void OMGSF_SSStoredObjectFactory::initialize(void)
{
  TRACE("OMGSF_SSStoredObjectFactory::initialize");
  OMGSFInitialize();
}

  // @mfunc Finalize this <c OMGSF_SSStoredObjectFactory>.
void OMGSF_SSStoredObjectFactory::finalize(void)
{
  TRACE("OMGSF_SSStoredObjectFactory::finalize");
  OMGSFFinalize();
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMGSF_SSStoredObjectFactory::compatibleRawStorage(
                                         const OMRawStorage* rawStorage,
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMGSF_SSStoredObjectFactory::compatibleRawStorage");

  return false;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMGSF_SSStoredObjectFactory::compatibleNamedFile(
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMGSF_SSStoredObjectFactory::compatibleNamedFile");

  bool result = true;

  // Read/Write GSF Structured Storage files are not supported.
  // But currently there is no method to create write-only named files.
  // So the following code is commented out until it GSF support improves.
  // if (accessMode == OMFile::modifyMode) {
  //   result = false;
  // }
  return result;
}

OMStoredObject*
OMGSF_SSStoredObjectFactory::openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMGSF_SSStoredObjectFactory::openFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));
  ASSERT("Unimplemented code to open GSF SS using raw storage not reached", 0);
  return 0;
}

OMStoredObject*
OMGSF_SSStoredObjectFactory::createFile (OMRawStorage* rawStorage,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
	TRACE("OMGSF_SSStoredObjectFactory::createFile");
	PRECONDITION("Valid raw storage", rawStorage != 0);

	ASSERT("Unimplemented code to create GSF file on raw storage not reached", 0);
	return 0;
}


OMStoredObject*
OMGSF_SSStoredObjectFactory::openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMGSF_SSStoredObjectFactory::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  if (mode == OMFile::modifyMode) {
  	ASSERT("Unimplemented GSF open in Read Modify mode not reached", false);
		return 0;
  }
  SSCHAR omFileName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, FILENAME_MAX);
#else
  copyWideString (omFileName, fileName, FILENAME_MAX);
#endif
  IStorage* storage = 0;

  HRESULT status = OMGSFIStorage::StgOpenStorageEx(
										omFileName,
										OMFile::readOnlyMode,
										(void **)&storage);
  checkStatus(status);
  ASSERT("StgOpenStorageEx() succeeded", SUCCEEDED(status));

  return OMSSStoredObject::open(storage, mode);
}

OMStoredObject*
OMGSF_SSStoredObjectFactory::createFile (const wchar_t* fileName,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
  TRACE("OMGSF_SSStoredObjectFactory::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

  // choose sector size based on signature from factory
  ULONG sectorSize=0;
  if( OMSignature_Aaf512Binary == signature ) sectorSize=512;
  else if( OMSignature_Aaf4KBinary == signature ) sectorSize=4096;
  PRECONDITION("Valid Signature", sectorSize!=0 );

  SSCHAR omFileName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, FILENAME_MAX);
#else
  copyWideString (omFileName, fileName, FILENAME_MAX);
#endif

  IStorage* storage = 0;

  HRESULT status = OMGSFIStorage::StgCreateStorageEx(
											omFileName,
											OMFile::writeOnlyMode,
											(void **)&storage,
											sectorSize);
  checkStatus(status);
  ASSERT("StgCreateStorageEx() succeeded", SUCCEEDED(status));

  return OMSSStoredObject::create(storage, byteOrder);
}

static void checkStatus(HRESULT status)
{
  TRACE("checkStatus");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

#endif // OM_USE_GSF_SS

#endif // !OM_NO_STRUCTURED_STORAGE
