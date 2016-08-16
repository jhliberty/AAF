//=---------------------------------------------------------------------=
//
// $Id: OMStrongRefVectorProperty.h,v 1.37 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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

// @doc OMEXTERNAL
#ifndef OMSTRONGREFVECTORPROPERTY_H
#define OMSTRONGREFVECTORPROPERTY_H

#include "OMVector.h"
#include "OMContainerElement.h"
#include "OMStrongReferenceVector.h"

template <typename ReferencedObject>
class OMStrongReferenceVectorIterator;

template <typename Element>
class OMVectorIterator;

  // @class Persistent elastic sequential collections of strongly
  //        referenced (contained) objects supported by the Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMStrongReferenceVector>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMStrongReferenceVectorProperty : public OMStrongReferenceVector {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceVectorProperty(const OMPropertyId propertyId,
                                  const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceVectorProperty(void);

    // @cmember Save this <c OMStrongReferenceVectorProperty>.
  virtual void save(void) const;

    // @cmember Close this <c OMStrongReferenceVectorProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMStrongReferenceVectorProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMStrongReferenceVectorProperty>, the
    //          external (persisted) size of the
    //          <c OMStrongReferenceVectorProperty> is <p externalSize>.
  virtual void restore(OMPropertySize externalSize);

    // @cmember The number of objects contained within this
    //          <c OMStrongReferenceVectorProperty> if any.
  virtual OMUInt64 objectCount(void) const;

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMStrongReferenceVectorProperty>.
  OMUInt32 count(void) const;

    // @cmember Set the value of this <c OMStrongReferenceVectorProperty>
    //          at position <p index> to <p object>.
  ReferencedObject* setValueAt(const ReferencedObject* object,
                               const OMUInt32 index);

    // @cmember Set the value of this <c OMStrongReferenceVectorProperty>
    //          at position <p index> to 0.
  ReferencedObject* clearValueAt(const OMUInt32 index);

    // @cmember The value of this <c OMStrongReferenceVectorProperty>
    //          at position <p index>.
  ReferencedObject* valueAt(const OMUInt32 index) const;

    // @cmember Get the value of this <c OMStrongReferenceVectorProperty>
    //          at position <p index> into <p object>.
  void getValueAt(ReferencedObject*& object, const OMUInt32 index) const;

    // @cmember If <p index> is valid, get the value of this
    //          <c OMStrongReferenceVectorProperty> at position <p index>
    //          into <p object> and return true, otherwise return false.
  bool find(const OMUInt32 index, ReferencedObject*& object) const;

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMStrongReferenceVectorProperty>.
  void appendValue(const ReferencedObject* object);

    // @cmember Prepend the given <p ReferencedObject> <p object> to
    //          this <c OMStrongReferenceVectorProperty>.
  void prependValue(const ReferencedObject* object);

    // @cmember Insert <p object> into this
    //          <c OMStrongReferenceVectorProperty>. This function is
    //          redefined from <c OMContainerProperty> as
    //          <mf OMStrongReferenceVectorProperty::appendValue>.
  void insert(const ReferencedObject* object);

    // @cmember Insert <p object> into this <c OMStrongReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  void insertAt(const ReferencedObject* object, const OMUInt32 index);

    // @cmember Does this <c OMStrongReferenceVectorProperty> contain
    //          <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMStrongReferenceVectorProperty>.
  void removeValue(const ReferencedObject* object);

    // @cmember Remove the object from this
    //          <c OMStrongReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMStrongReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  ReferencedObject* removeAt(const OMUInt32 index);

    // @cmember Remove the last (index == count() - 1) object
    //          from this <c OMStrongReferenceVectorProperty>.
  ReferencedObject* removeLast(void);

    // @cmember Remove the first (index == 0) object
    //          from this <c OMStrongReferenceVectorProperty>. Existing
    //          objects in this <c OMStrongReferenceVectorProperty> are
    //          shifted down one index position.
  ReferencedObject* removeFirst(void);

    // @cmember The index of the <p ReferencedObject*> <p object>.
  OMUInt32 indexOfValue(const ReferencedObject* object) const;

    // @cmember The number of occurrences of <p object> in this
    //          <c OMStrongReferenceVectorProperty>.
  OMUInt32 countOfValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMStrongReferenceVectorProperty> contain
    //          <p index> ? Is <p index> valid ?
  bool containsIndex(const OMUInt32 index) const;

    // @cmember If this <c OMStrongReferenceProperty> contains <p object>
    //          then place its index in <p index> and return true, otherwise
    //          return false.
  bool findIndex(const ReferencedObject* object, OMUInt32& index) const;

    // @cmember Increase the capacity of this
    //          <c OMStrongReferenceVectorProperty> so that it
    //          can contain at least <p capacity> <p ReferencedObject>s
    //          without having to be resized.
  virtual void grow(const OMUInt32 capacity);

  // Optional property interface

    // @cmember Is this <c OMStrongReferenceVectorProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMStrongReferenceVectorProperty>.
  virtual void removeProperty(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMStrongReferenceVectorProperty>. The size is given
    //          in bytes.
  virtual OMUInt32 bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMStrongReferenceVectorProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, OMUInt32 size) const;

    // @cmember Set the raw bits of this
    //          <c OMStrongReferenceVectorProperty>. The raw bits are
    //          copied from the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void setBits(const OMByte* bits, OMUInt32 size);

    // @cmember Insert <p object> into this
    //          <c OMStrongReferenceVectorProperty>.
  virtual void insertObject(const OMObject* object);

    // @cmember Does this <c OMStrongReferenceVectorProperty> contain
    //          <p object> ?
  virtual bool containsObject(const OMObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMStrongReferenceVectorProperty>.
  virtual void removeObject(const OMObject* object);

    // @cmember Remove all objects from this
    //          <c OMStrongReferenceVectorProperty>.
  virtual void removeAllObjects(void);

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMStrongReferenceVectorProperty>.
  virtual OMReferenceContainerIterator* createIterator(void) const;

    // @cmember Set the value of this <c OMStrongReferenceVectorProperty>
    //          at position <p index> to <p object>.
  virtual OMObject* setObjectAt(const OMObject* object,
                                const OMUInt32 index);

    // @cmember The value of this <c OMStrongReferenceVectorProperty>
    //          at position <p index>.
  virtual OMObject* getObjectAt(const OMUInt32 index) const;

    // @cmember Append the given <p OMObject> <p object> to
    //          this <c OMStrongReferenceVectorProperty>.
  virtual void appendObject(const OMObject* object);

    // @cmember Prepend the given <p OMObject> <p object> to
    //          this <c OMStrongReferenceVectorProperty>.
  virtual void prependObject(const OMObject* object);

    // @cmember Remove the object from this
    //          <c OMStrongReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMStrongReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  virtual OMObject* removeObjectAt(const OMUInt32 index);

    // @cmember Insert <p object> into this <c OMStrongReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  virtual void insertObjectAt(const OMObject* object, const OMUInt32 index);

  virtual OMContainerIterator<OMStrongReferenceVectorElement>*
                                                          iterator(void) const;

  virtual void insert(const OMUInt32 index,
                      const OMStrongReferenceVectorElement& element);

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

private:

  typedef OMStrongReferenceVectorElement VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // The vector of references.
  OMVector<VectorElement> _vector;

  friend class OMStrongReferenceVectorIterator<ReferencedObject>;

    // OMStrongReferenceVectorProperty can't be assigned - declare but
    // don't define
  OMStrongReferenceVectorProperty& operator = (
                                   const OMStrongReferenceVectorProperty& rhs);

    // OMStrongReferenceVectorProperty can't be copied - declare but
    // don't define
  OMStrongReferenceVectorProperty(const OMStrongReferenceVectorProperty& rhs);

};

#include "OMStrongRefVectorPropertyT.h"

#endif
