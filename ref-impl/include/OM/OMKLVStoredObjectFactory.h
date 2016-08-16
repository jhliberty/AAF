//=---------------------------------------------------------------------=
//
// $Id: OMKLVStoredObjectFactory.h,v 1.21 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMKLVSTOREDOBJECTFACTORY_H
#define OMKLVSTOREDOBJECTFACTORY_H

#include "OMStoredObjectFactory.h"
#include "OMDataTypes.h"
#include <stdlib.h>

class OMRawStorage;

  // @class Factories for <c OMKLVStoredObject>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredObjectFactory : public OMStoredObjectFactory {
public:
  // @access Public members.

    // @cmember Constructor.
  OMKLVStoredObjectFactory(const OMStoredObjectEncoding& encoding,
                           const OMUniqueObjectIdentification& signature,
                           const wchar_t* name,
                           const wchar_t* description);

    // @cmember Destructor.
  virtual ~OMKLVStoredObjectFactory(void);

    // @cmember Initialize this <c OMKLVStoredObjectFactory>.
  virtual void initialize(void);

    // @cmember Finalize this <c OMKLVStoredObjectFactory>.
  virtual void finalize(void);

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  virtual OMStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  virtual OMStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder);

    // @cmember Open the root <c OMKLVStoredObject> in the disk file
    //          <p fileName> for reading only.
  virtual OMStoredObject* openRead(const wchar_t* fileName);

    // @cmember Open the root <c OMKLVStoredObject> in the disk file
    //          <p fileName> for modification.
  virtual OMStoredObject* openModify(const wchar_t* fileName);

    // @cmember Create a new root <c OMKLVStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMKLVStoredObject> in the disk file
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

    // @cmember Perform any necessary actions when <p file> is closed.
  virtual void close(OMFile* file);

};

#endif
