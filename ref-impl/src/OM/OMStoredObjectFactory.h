//=---------------------------------------------------------------------=
//
// $Id: OMStoredObjectFactory.h,v 1.13 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMSTOREDOBJECTFACTORY_H
#define OMSTOREDOBJECTFACTORY_H

#include "OMDataTypes.h"
#include "OMFile.h"

#include <stdlib.h>

class OMRawStorage;
class OMStoredObject;

  // @class Abstract base class describing factories for <c OMStoredObject>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStoredObjectFactory {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStoredObjectFactory(const OMStoredObjectEncoding& encoding,
                        const OMUniqueObjectIdentification& signature,
                        const wchar_t* name,
                        const wchar_t* description);

    // @cmember Destructor.
  virtual ~OMStoredObjectFactory(void);

    // @cmember Initialize this <c OMStoredObjectFactory>.
  virtual void initialize(void) = 0;

    // @cmember Finalize this <c OMStoredObjectFactory>.
  virtual void finalize(void) = 0;

    // @cmember The key used to identify the encoding of
    //          <c OMStoredObject>s created by this <c OMStoredObjectFactory>.
  virtual OMStoredObjectEncoding encoding(void) const;

    // @cmember Used to identify files created
    //          by this <c OMStoredObjectFactory>.
  virtual OMUniqueObjectIdentification signature(void) const;

    // @cmember The name used to identify the encoding of
    //          <c OMStoredObject>s created by this <c OMStoredObjectFactory>.
  virtual const wchar_t* name(void) const;

    // @cmember The description of the encoding of
    //         <c OMStoredObject>s created by this <c OMStoredObjectFactory>.
  virtual const wchar_t* description(void) const;

    // @cmember Open the root <c OMStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  virtual OMStoredObject* openRead(OMRawStorage* rawStorage) = 0;

    // @cmember Open the root <c OMStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  virtual OMStoredObject* openModify(OMRawStorage* rawStorage) = 0;

    // @cmember Create a new root <c OMStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder) = 0;

    // @cmember Create a new root <c OMStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder) = 0;

    // @cmember Open the root <c OMStoredObject> in the disk file
    //          <p fileName> for reading only.
  virtual OMStoredObject* openRead(const wchar_t* fileName) = 0;

    // @cmember Open the root <c OMStoredObject> in the disk file
    //          <p fileName> for modification.
  virtual OMStoredObject* openModify(const wchar_t* fileName) = 0;

    // @cmember Create a new root <c OMStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder) = 0;

    // @cmember Create a new root <c OMStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  virtual OMStoredObject* createWrite(const wchar_t* fileName,
                                      const OMByteOrder byteOrder) = 0;

    // @cmember Is the file named <p fileName> a recognized file ?
    //          If so, the result is true.
  virtual bool isRecognized(const wchar_t* fileName) = 0;

    // @cmember Does <p rawStorage> contain a recognized file ?
    //          If so, the result is true.
  virtual bool isRecognized(OMRawStorage* rawStorage) = 0;

    // @cmember Can a file be created successfully on the given
    //          <c OMRawStorage> and accessed successfully in the mode
    //          specified by <p accessMode> ?
  virtual bool compatibleRawStorage(const OMRawStorage* rawStorage,
                                    const OMFile::OMAccessMode accessMode) = 0;

    // @cmember Can a file be created successfully as a named file and
    //          accessed successfully in the mode specified by <p accessMode> ?
  virtual bool compatibleNamedFile(const OMFile::OMAccessMode accessMode) = 0;

    // @cmember Perform any necessary actions when the file
    //          contained in <p file> is closed.
  virtual void close(OMFile* file) = 0;

private:
  // @access Private members.

  OMStoredObjectEncoding _encoding;
  OMUniqueObjectIdentification _signature;
  wchar_t* _name;
  wchar_t* _description;
};

#endif
