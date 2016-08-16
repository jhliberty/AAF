//=---------------------------------------------------------------------=
//
// $Id: OMKLVStoredObject.h,v 1.78 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMKLVSTOREDOBJECT_H
#define OMKLVSTOREDOBJECT_H

#include "OMStoredObject.h"
#include "OMDataTypes.h"
#include "OMFile.h"

class OMSimpleProperty;
class OMDataVector;
class OMDataSet;
class OMDataStream;
class OMMXFStorage;
class OMStrongReference;
class OMStrongReferenceSet;
class OMStrongReferenceVector;
class OMWeakReference;
class OMWeakReferenceSet;
class OMWeakReferenceVector;
class OMStoredStream;
class OMDataStreamProperty;
class OMKLVStoredStream;

class OMDictionary;
class OMDefinition;
class OMClassDefinition;
class OMPropertyDefinition;
class OMType;

  // @class In-memory representation of an object persisted in a
  //        SMPTE (Society of Motion Picture and Television Engineers)
  //        Key Length Value (KLV) binary file.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredObject : public OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  static OMKLVStoredObject* openRead(OMMXFStorage* rawStorage);

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  static OMKLVStoredObject* openModify(OMMXFStorage* rawStorage);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMKLVStoredObject* createWrite(OMMXFStorage* rawStorage,
                                        const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMKLVStoredObject* createModify(OMMXFStorage* rawStorage,
                                         const OMByteOrder byteOrder);

    // @cmember Does <p rawStorage> contain a recognized file ?
  static bool isRecognized(OMRawStorage* rawStorage);

    // @cmember Does <p file> have an <c OMMXFStorage>.
  static bool hasMxfStorage(const OMFile* file);

    // @cmember The <c OMMXFStorage> associated with <p file>.
  static OMMXFStorage* mxfStorage(const OMFile* file);

    // @cmember Does <p stream> represent MXF essence ?
  static bool isMxfEssence(const OMDataStreamProperty* stream);

    // @cmember The <c OMKLVStoredStream> associated with <p stream>
  static OMKLVStoredStream* mxfEssence(const OMDataStreamProperty* stream);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMKLVStoredObject(void);

    // @cmember Create a new <c OMKLVStoredObject>, named <p name>,
    //          contained by this <c OMKLVStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMKLVStoredObject>, named <p name>,
    //          contained by this <c OMKLVStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMKLVStoredObject>.
  virtual void close(void);

    // @cmember The byte order of this <c OMKLVStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  virtual OMByteOrder byteOrder(void) const;

  virtual void save(OMFile& file);

  virtual void save(OMStorable& object);

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMKLVStoredObject>.
  virtual void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMDataVector> <p property> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMDataVector& property);

    // @cmember Save the <c OMDataSet> <p property> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMDataSet& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMKLVStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMDataStream& stream);

  virtual OMRootStorable* restore(OMFile& file);

  virtual OMStorable* restoreObject(const OMStrongObjectReference& reference);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMKLVStoredObject> into <p id>.
  virtual void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMKLVStoredObject>.
  virtual void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMKLVStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMSimpleProperty& property,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMDataVector> <p property> into this
    //          <c OMKLVStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMDataVector& property,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMDataSet> <p property> into this
    //          <c OMKLVStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMDataSet& property,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReference& singleton,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReferenceVector& vector,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReferenceSet& set,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReference& singleton,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReferenceVector& vector,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReferenceSet& set,
                       OMPropertySize externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMKLVStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMDataStream& stream,
                       OMPropertySize externalSize);

  static void reorderString(OMCharacter* string,
                            size_t characterCount);

  static void externalizeString(const wchar_t* internalString,
                                OMCharacter* externalString,
                                size_t characterCount);

  static void internalizeString(const OMCharacter* externalString,
                                wchar_t* internalString,
                                size_t characterCount);

  virtual void write(const wchar_t* string);
  virtual void write(bool b);

  virtual void read(const wchar_t* string, OMUInt16 characterCount);
  virtual void read(bool& b);

  virtual void writeProperty(OMPropertyId pid, const OMUInt32& value);
  virtual void readProperty(const OMPropertyId& pid, OMUInt32& value);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMKLVStoredObject>.
  virtual OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMKLVStoredObject>.
  virtual OMStoredStream* createStoredStream(const OMDataStream& property);

  // KLV functions

  virtual void flatSave(const OMPropertySet& properties) const;
  virtual void deepSave(const OMPropertySet& properties) const;

  virtual void referenceSave(OMStorable* object, OMPropertyId pid) const;

  virtual void flatRestore(const OMPropertySet& properties);
  virtual void deepRestore(const OMPropertySet& properties);

  virtual void referenceRestore(OMStorable* object, OMPropertyId pid);

  virtual void writePrimerPack(const OMDictionary* dictionary);

  virtual void readPrimerPack(OMDictionary* dictionary);

  void saveObjectDirectoryReference(const OMUniqueObjectIdentification& id);

  OMUInt64 restoreObjectDirectoryReference(OMUniqueObjectIdentification& id);

private:
  // @access Private members.

    // @cmember The root storage of <p file>.
  static OMKLVStoredObject* root(const OMFile* file);

    // @cmember The <c OMKLVStoredStream> associated with <p stream>
  static OMKLVStoredStream* mxfStream(const OMDataStreamProperty* stream);

    // @cmember Constructor.
  OMKLVStoredObject(OMMXFStorage* s, OMByteOrder byteOrder);

  OMMXFStorage* _storage;
  OMByteOrder _byteOrder;
  bool _reorderBytes;
};

#endif
