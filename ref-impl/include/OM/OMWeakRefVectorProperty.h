//=---------------------------------------------------------------------=
//
// $Id: OMWeakRefVectorProperty.h,v 1.39 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMWEAKREFVECTORPROPERTY_H
#define OMWEAKREFVECTORPROPERTY_H

#include "OMVector.h"
#include "OMContainerElement.h"
#include "OMWeakReferenceVector.h"

template <typename Key, typename ReferencedObject>
class OMWeakReferenceVectorIterator;

template <typename Element>
class OMVectorIterator;

  // @class Persistent elastic sequential collections of uniquely identified
  //        weakly referenced (non-contained) objects supported by the
  //        Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @base public | <c OMWeakReferenceVector>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Key, typename ReferencedObject>
class OMWeakReferenceVectorProperty : public OMWeakReferenceVector {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                const wchar_t* name,
                                const wchar_t* targetName,
                                const OMPropertyId keyPropertyId);

    // @cmember Constructor.
  OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                const wchar_t* name,
                                const OMPropertyId keyPropertyId,
                                const OMPropertyId* targetPropertyPath);
    // @cmember Destructor.
  virtual ~OMWeakReferenceVectorProperty(void);

    // @cmember Save this <c OMWeakReferenceVectorProperty>.
  virtual void save(void) const;

    // @cmember Close this <c OMWeakReferenceVectorProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMWeakReferenceVectorProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMWeakReferenceVectorProperty>, the
    //          external (persisted) size of the
    //          <c OMWeakReferenceVectorProperty> is <p externalSize>.
  virtual void restore(OMPropertySize externalSize);

    // @cmember The number of objects contained within this
    //          <c OMWeakReferenceVectorProperty> if any.
  virtual OMUInt64 objectCount(void) const;

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMWeakReferenceVectorProperty>.
  OMUInt32 count(void) const;

    // @cmember Set the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> to <p object>.
  ReferencedObject* setValueAt(const ReferencedObject* object,
                               const OMUInt32 index);

    // @cmember Set the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> to 0.
  ReferencedObject* clearValueAt(const OMUInt32 index);

    // @cmember The value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index>.
  ReferencedObject* valueAt(const OMUInt32 index) const;

    // @cmember Get the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> into <p object>.
  void getValueAt(ReferencedObject*& object, const OMUInt32 index) const;

    // @cmember If <p index> is valid, get the value of this
    //          <c OMWeakReferenceVectorProperty> at position <p index>
    //          into <p object> and return true, otherwise return false.
  bool find(const OMUInt32 index, ReferencedObject*& object) const;

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  void appendValue(const ReferencedObject* object);

    // @cmember Prepend the given <p ReferencedObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  void prependValue(const ReferencedObject* object);

    // @cmember Insert <p object> into this
    //          <c OMWeakReferenceVectorProperty>. This function is
    //          redefined from <c OMContainerProperty> as
    //          <mf OMWeakReferenceVectorProperty::appendValue>.
  void insert(const ReferencedObject* object);

    // @cmember Insert <p object> into this <c OMWeakReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  void insertAt(const ReferencedObject* object, const OMUInt32 index);

    // @cmember Does this <c OMWeakReferenceVectorProperty> contain
    //          <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMWeakReferenceVectorProperty>.
  void removeValue(const ReferencedObject* object);

    // @cmember Remove the object from this
    //          <c OMWeakReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMWeakReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  ReferencedObject* removeAt(const OMUInt32 index);

    // @cmember Remove the last (index == count() - 1) object
    //          from this <c OMWeakReferenceVectorProperty>.
  ReferencedObject* removeLast(void);

    // @cmember Remove the first (index == 0) object
    //          from this <c OMWeakReferenceVectorProperty>. Existing
    //          objects in this <c OMWeakReferenceVectorProperty> are
    //          shifted down one index position.
  ReferencedObject* removeFirst(void);

    // @cmember The index of the <p ReferencedObject*> <p object>.
  OMUInt32 indexOfValue(const ReferencedObject* object) const;

    // @cmember The number of occurrences of <p object> in this
    //          <c OMWeakReferenceVectorProperty>.
  OMUInt32 countOfValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMWeakReferenceVectorProperty> contain
    //          <p index> ? Is <p index> valid ?
  bool containsIndex(const OMUInt32 index) const;

    // @cmember If this <c OMWeakReferenceProperty> contains <p object>
    //          then place its index in <p index> and return true, otherwise
    //          return false.
  bool findIndex(const ReferencedObject* object, OMUInt32& index) const;

    // @cmember Increase the capacity of this
    //          <c OMWeakReferenceVectorProperty> so that it
    //          can contain at least <p capacity> <p ReferencedObject>s
    //          without having to be resized.
  void grow(const OMUInt32 capacity);

  // Optional property interface

    // @cmember Is this <c OMWeakReferenceVectorProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMWeakReferenceVectorProperty>.
  virtual void removeProperty(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMWeakReferenceVectorProperty>. The size is given
    //          in bytes.
  virtual OMUInt32 bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMWeakReferenceVectorProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, OMUInt32 size) const;

    // @cmember Set the raw bits of this
    //          <c OMWeakReferenceVectorProperty>. The raw bits are
    //          copied from the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void setBits(const OMByte* bits, OMUInt32 size);

    // @cmember Insert <p object> into this
    //          <c OMWeakReferenceVectorProperty>.
  virtual void insertObject(const OMObject* object);

    // @cmember Does this <c OMWeakReferenceVectorProperty> contain
    //          <p object> ?
  virtual bool containsObject(const OMObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMWeakReferenceVectorProperty>.
  virtual void removeObject(const OMObject* object);

    // @cmember Remove all objects from this
    //          <c OMWeakReferenceVectorProperty>.
  virtual void removeAllObjects(void);

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMWeakReferenceVectorProperty>.
  virtual OMReferenceContainerIterator* createIterator(void) const;

    // @cmember Set the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> to <p object>.
  virtual OMObject* setObjectAt(const OMObject* object,
                                const OMUInt32 index);

    // @cmember The value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index>.
  virtual OMObject* getObjectAt(const OMUInt32 index) const;

    // @cmember Append the given <p OMObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  virtual void appendObject(const OMObject* object);

    // @cmember Prepend the given <p OMObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  virtual void prependObject(const OMObject* object);

    // @cmember Remove the object from this
    //          <c OMWeakReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMWeakReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  virtual OMObject* removeObjectAt(const OMUInt32 index);

    // @cmember Insert <p object> into this <c OMWeakReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  virtual void insertObjectAt(const OMObject* object, const OMUInt32 index);

  virtual OMContainerIterator<OMWeakReferenceVectorElement>*
                                                          iterator(void) const;

  virtual void insert(const OMUInt32 index,
                      const OMWeakReferenceVectorElement& element);

  const Key& identification(OMUInt32 index) const;

  // @cmember The size of the identification of <c OMObject> in this
  //          <c OMWeakReferenceVectorProperty>. The size is given
  //          in bytes.
  virtual OMKeySize keySize(void) const;

    // @cmember The <c OMStrongReferenceSet> in which the objects
    //          referenced by this <c OMWeakReferenceVectorProperty>
    //          must reside.
  virtual OMStrongReferenceSet* targetSet(void) const;

  virtual OMPropertyId keyPropertyId(void) const;

  virtual OMPropertyTag targetTag(void) const;

  virtual void setTargetTag(OMPropertyTag targetTag);

  virtual void clearTargetTag(void) const;

  virtual const wchar_t* targetName(void) const;

  virtual bool isResolved(OMUInt32 index) const;

  virtual bool isResolvable(OMUInt32 index) const;

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

private:

  typedef OMWeakReferenceVectorElement VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

  OMPropertyId* targetPropertyPath(void) const;

    // The vector of references.
  OMVector<VectorElement> _vector;
  OMPropertyTag _targetTag;
  const wchar_t* _targetName;
  OMPropertyId* _targetPropertyPath;
  OMPropertyId _keyPropertyId;
  OMStrongReferenceSet* _targetSet;

  friend class OMWeakReferenceVectorIterator<Key, ReferencedObject>;

    // OMWeakReferenceVectorProperty can't be assigned - declare but
    // don't define
  OMWeakReferenceVectorProperty& operator = (
                                     const OMWeakReferenceVectorProperty& rhs);

    // OMWeakReferenceVectorProperty can't be copied - declare but don't define
  OMWeakReferenceVectorProperty(const OMWeakReferenceVectorProperty& rhs);

};

#include "OMWeakRefVectorPropertyT.h"

#endif
