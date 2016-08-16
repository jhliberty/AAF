//=---------------------------------------------------------------------=
//
// $Id: OMSSStoredObjectFactory.h,v 1.7 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMSSSTOREDOBJECTFACTORY_H
#define OMSSSTOREDOBJECTFACTORY_H

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMStoredObjectFactory.h"
#include "OMDataTypes.h"
#include <stdlib.h>

class OMRawStorage;

// the following consts duplicate those in AAFFileKinds.h
// should be abstracted into OMSignatures.h

// AAF files encoded as structured storage (binary).
// the signature actually stored in all AAF SS (512) files
// note this is not a properly-formed SMPTE label, but this is legacy
const OMUniqueObjectIdentification OMSignature_Aaf512Binary = 
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// the signature actually stored in all AAF SS (4096) files
// [060e2b34.0302.0101.0d010201.02000000]
const OMUniqueObjectIdentification OMSignature_Aaf4KBinary = 
{0x0d010201, 0x0200, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x03, 0x02, 0x01, 0x01}};

// @class Factories for <c OMSSStoredObject>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMSSStoredObjectFactory : public OMStoredObjectFactory {
public:
  // @data Public members.
  // @access Public members.

    // @cmember Constructor.
  OMSSStoredObjectFactory(const OMStoredObjectEncoding& encoding,
                           const OMUniqueObjectIdentification& signature,
                           const wchar_t* name,
                           const wchar_t* description);

    // @cmember Destructor.
  virtual ~OMSSStoredObjectFactory(void);

    // @cmember Initialize this <c OMSSStoredObjectFactory>.
  virtual void initialize(void) = 0;

    // @cmember Finalize this <c OMSSStoredObjectFactory>.
  virtual void finalize(void) = 0;

    // @cmember Open the root <c OMSSStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  virtual OMStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMSSStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  virtual OMStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMSSStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMSSStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder);

    // @cmember Open the root <c OMSSStoredObject> in the disk file
    //          <p fileName> for reading only.
  virtual OMStoredObject* openRead(const wchar_t* fileName);

    // @cmember Open the root <c OMSSStoredObject> in the disk file
    //          <p fileName> for modification.
  virtual OMStoredObject* openModify(const wchar_t* fileName);

    // @cmember Create a new root <c OMSSStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMSSStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(const wchar_t* fileName,
                                      const OMByteOrder byteOrder);

    // @cmember Is the file named <p fileName> a recognized file ?
    //          If so, the result is true.
  virtual bool isRecognized(const wchar_t* fileName);

    // @cmember Does <p rawStorage> contain a recognized file ?
    //          If so, the result is true.
  virtual bool isRecognized(OMRawStorage* rawStorage);

    // @cmember Can a file be created successfully on the given
    //          <c OMRawStorage> and accessed successfully in the mode
    //          specified by <p accessMode> ?
  virtual bool compatibleRawStorage(const OMRawStorage* rawStorage,
                                    const OMFile::OMAccessMode accessMode);

    // @cmember Can a file be created successfully as a named file and
    //          accessed successfully in the mode specified by <p accessMode> ?
  virtual bool compatibleNamedFile(const OMFile::OMAccessMode accessMode);

    // @cmember Perform any necessary actions when the file
    //          contained in <p file> is closed.
  virtual void close(OMFile* file);

protected:
  virtual OMStoredObject* openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode) = 0;

  virtual OMStoredObject* openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode) = 0;

  virtual OMStoredObject* createFile (OMRawStorage* rawStorage,
                             const OMByteOrder byteOrder,
							 const OMUniqueObjectIdentification& signature) = 0;

  virtual OMStoredObject* createFile (const wchar_t* fileName,
                             const OMByteOrder byteOrder,
							 const OMUniqueObjectIdentification& signature) = 0;

private:
    // @cmember Write the signature to the given raw storage.
  static void writeSignature(OMRawStorage* rawStorage,
                             const OMUniqueObjectIdentification& signature);

    // @cmember Write the signature to the given file.
  static void writeSignature(const wchar_t* fileName,
                             const OMUniqueObjectIdentification& signature);

};

#endif // !OM_NO_STRUCTURED_STORAGE

#endif // !OMSSSTOREDOBJECTFACTORY_H
