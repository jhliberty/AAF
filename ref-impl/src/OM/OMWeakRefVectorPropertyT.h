//=---------------------------------------------------------------------=
//
// $Id: OMWeakRefVectorPropertyT.h,v 1.82 2009/06/01 11:47:02 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMWeakReferenceVectorProperty
#ifndef OMWEAKREFVECTORPROPERTYT_H
#define OMWEAKREFVECTORPROPERTYT_H

#include "OMAssertions.h"
#include "OMWeakReferenceVectorIter.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"
#include "OMStoredObject.h"
#include "OMFile.h"
#include "OMStrongReferenceSet.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceVectorProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the objects referenced by the
  //         elements of this <c OMWeakReferenceVectorProperty> reside.
  //   @parm The id of the property by which the <p ReferencedObject>s
  //         are uniquely identified (the key).
template <typename Key, typename ReferencedObject>
OMWeakReferenceVectorProperty<Key, ReferencedObject>::
                OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                              const wchar_t* name,
                                              const wchar_t* targetName,
                                              const OMPropertyId keyPropertyId)
: OMWeakReferenceVector(propertyId, name),
  _targetTag(nullOMPropertyTag),
  _targetName(targetName),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId),
  _targetSet(0)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::"
                                              "OMWeakReferenceVectorProperty");
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceVectorProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the objects referenced by the
  //         elements of this <c OMWeakReferenceVectorProperty> reside.
  //   @parm The id of the property by which the <p ReferencedObject>s
  //         are uniquely identified (the key).
template <typename Key, typename ReferencedObject>
OMWeakReferenceVectorProperty<Key, ReferencedObject>::OMWeakReferenceVectorProperty(
                                        const OMPropertyId propertyId,
                                        const wchar_t* name,
                                        const OMPropertyId keyPropertyId,
                                        const OMPropertyId* targetPropertyPath)
: OMWeakReferenceVector(propertyId, name),
  _targetTag(nullOMPropertyTag),
  _targetName(0),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId),
  _targetSet(0)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::"
                                              "OMWeakReferenceVectorProperty");

  _targetPropertyPath = savePropertyPath(targetPropertyPath);
}

  // @mfunc Destructor.
template <typename Key, typename ReferencedObject>
OMWeakReferenceVectorProperty<Key, ReferencedObject>::
                                           ~OMWeakReferenceVectorProperty(void)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::"
                                             "~OMWeakReferenceVectorProperty");

  delete [] _targetPropertyPath;
}

  // @mfunc Save this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @this const
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::save");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  store()->save(*this);
}

  // @mfunc Close this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::close(void)
{
  OMUInt32 count = _vector.count();
  for (OMUInt32 i = 0; i < count; i++) {
    VectorElement& element = _vector.getAt(i);
    element.close();
  }
}

  // @mfunc Detach this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::detach(void)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::detach");
  OMUInt32 count = _vector.count();
  for (OMUInt32 i = 0; i < count; i++) {
    VectorElement& element = _vector.getAt(i);
    element.detach();
  }
}

  // @mfunc Restore this <c OMWeakReferenceVectorProperty>, the external
  //        (persisted) size of the <c OMWeakReferenceVectorProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The external (persisted) size of the
  //         <c OMWeakReferenceVectorProperty>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::restore(
                                                   OMPropertySize externalSize)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::restore");

  store()->restore(*this, externalSize);
  setPresent();
}

  // @mfunc The number of objects contained within this
  //        <c OMWeakReferenceVectorProperty> if any.
  //   @rdesc The number of objects.
template <typename Key, typename ReferencedObject>
OMUInt64
OMWeakReferenceVectorProperty<Key, ReferencedObject>::objectCount(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::objectCount");
  // OMWeakReferenceVectorProperty doesn't contain objects.
  return 0;
}

  // @mfunc The number of <p ReferencedObject>s in this
  //        <c OMWeakReferenceVectorProperty>.
  //   @this const
template <typename Key, typename ReferencedObject>
OMUInt32 OMWeakReferenceVectorProperty<Key, ReferencedObject>::count(void) const
{
  return _vector.count();
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> to <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject* OMWeakReferenceVectorProperty<Key, ReferencedObject>::setValueAt(
                                                const ReferencedObject* object,
                                                const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::setValueAt");

  PRECONDITION("Valid index", index < count());
  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
  PRECONDITION("Valid target object", targetSet()->containsObject(object));
#endif

  // Set the vector to contain the new object
  //
  VectorElement& element = _vector.getAt(index);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  element.reference().setTargetTag(targetTag());
#endif
  ReferencedObject* result = 0;
  const Key key = object->identification();
  OMStorable* p = element.setValue(&key, object);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  setPresent();

  POSTCONDITION("Object properly inserted",
     _vector.getAt(index).getValue() == const_cast<ReferencedObject*>(object));
  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> to 0.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The position to clear.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::clearValueAt(
                                                          const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::clearValueAt");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);
  OMStorable* p = element.setValue(&OMConstant<Key>::null, 0);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  POSTCONDITION("Object properly cleared",
                                         _vector.getAt(index).getValue() == 0);
  return result;
}

  // @mfunc The value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename Key, typename ReferencedObject>
ReferencedObject* OMWeakReferenceVectorProperty<Key, ReferencedObject>::valueAt(
                                                    const OMUInt32 index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::valueAt");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  OMStorable* p = element.getValue();
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  return result;
}

  // @mfunc Get the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> into <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @this const
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::getValueAt(
                                                    ReferencedObject*& object,
                                                    const OMUInt32 index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::getValueAt");
  OBSOLETE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::valueAt");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  OMStorable* p = element.getValue();
  if (p != 0) {
    object = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", object != 0);
  }
}

  // @mfunc If <p index> is valid, get the value of this
  //        <c OMWeakReferenceVectorProperty> at position <p index>
  //        into <p object> and return true, otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @rdesc True if <p index> is valid, false otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorProperty<Key, ReferencedObject>::find(
                                               const OMUInt32 index,
                                               ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::find");

  bool result;
  if (index < count()) {
    object = valueAt(index);
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Append the given <p ReferencedObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::appendValue");

  PRECONDITION("Valid object", object != 0);

  insertAt(object, count());

}

  // @mfunc Prepend the given <p ReferencedObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::prependValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::prependValue");

  PRECONDITION("Valid object", object != 0);

  insertAt(object, 0);
}

  // @mfunc Insert <p object> into this
  //        <c OMWeakReferenceVectorProperty>. This function is
  //        redefined from <c OMContainerProperty> as
  //        <mf OMWeakReferenceVectorProperty::appendValue>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::insert");

  PRECONDITION("Valid object", object != 0);

  appendValue(object);
}

  // @mfunc Insert <p value> into this <c OMWeakReferenceVectorProperty>
  //        at position <p index>. Existing values at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::insertAt(
                                                const ReferencedObject* object,
                                                const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::insertAt");

  PRECONDITION("Valid index", index <= count());
  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
  PRECONDITION("Valid target object", targetSet()->containsObject(object));
#endif

  const Key key = object->identification();
  VectorElement newElement(this, &key, sizeof(Key), _targetTag);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  newElement.reference().setTargetTag(targetTag());
#endif
  newElement.setValue(&key, object);
  _vector.insertAt(newElement, index);
  setPresent();

  POSTCONDITION("Object properly inserted",
     _vector.getAt(index).getValue() == const_cast<ReferencedObject*>(object));
}

  // @mfunc Does this <c OMWeakReferenceVectorProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorProperty<Key, ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::containsValue");

  PRECONDITION("Valid object", object != 0);

  bool result = false;
  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == const_cast<ReferencedObject*>(object)) {
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc Remove <p object> from this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  OMUInt32 index = indexOfValue(object);
  removeAt(index);
}

  // @mfunc Remove the object from this
  //        <c OMWeakReferenceVectorProperty> at position <p index>.
  //        Existing objects in this <c OMWeakReferenceVectorProperty>
  //        at <p index> + 1 and higher are shifted down one index
  //        position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The position from which to remove the <p ReferencedObject>.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeAt(const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeAt");
  PRECONDITION("Valid index", index < count());

  ReferencedObject* result = clearValueAt(index);
  _vector.removeAt(index);
  return result;
}

  // @mfunc Remove the last (index == count() - 1) object
  //        from this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeLast(void)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeLast");

  return removeAt(count() - 1);
}

  // @mfunc Remove the first (index == 0) object
  //        from this <c OMWeakReferenceVectorProperty>. Existing
  //        objects in this <c OMWeakReferenceVectorProperty> are
  //        shifted down one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeFirst(void)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeFirst");

  return removeAt(0);
}

  // @mfunc The index of the <p ReferencedObject*> <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm A pointer to the <p ReferencedObject> to find.
  //   @rdesc The index.
  //   @this const
template <typename Key, typename ReferencedObject>
OMUInt32 OMWeakReferenceVectorProperty<Key, ReferencedObject>::indexOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::indexOfValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  OMUInt32 result = 0;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == const_cast<ReferencedObject*>(object)) {
      result = iterator.index();
      break;
    }
  }
  return result;
}

  // @mfunc The number of occurrences of <p object> in this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The object to count.
  //   @rdesc The number of occurrences.
  //   @this const
template <typename Key, typename ReferencedObject>
OMUInt32 OMWeakReferenceVectorProperty<Key, ReferencedObject>::countOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::countOfValue");

  PRECONDITION("Valid object", object != 0);

  OMUInt32 result = 0;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == const_cast<ReferencedObject*>(object)) {
      result = result + 1;
    }
  }
  return result;
}

  // @mfunc Does this <c OMWeakReferenceVectorProperty> contain
  //        <p index> ? Is <p index> valid ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The index.
  //   @rdesc True if the index is valid, false otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorProperty<Key, ReferencedObject>::containsIndex(
                                                    const OMUInt32 index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::containsIndex");

  bool result;
  if (index < count()) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc If this <c OMWeakReferenceProperty> contains <p object>
  //        then place its index in <p index> and return true, otherwise
  //        return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The object for which to search.
  //   @parm The index of the object.
  //   @rdesc True if the object was found, false otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorProperty<Key, ReferencedObject>::findIndex(
                                                const ReferencedObject* object,
                                                OMUInt32& index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::findIndex");

  PRECONDITION("Valid object", object != 0);

  bool result = false;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == object) {
      index = iterator.index();
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc Increase the capacity of this
  //        <c OMWeakReferenceVectorProperty> so that it
  //        can contain at least <p capacity> <p ReferencedObject>s
  //        without having to be resized.
  //   @parm The desired capacity.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::grow(
                                                       const OMUInt32 capacity)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::grow");
  PRECONDITION("Valid capacity", capacity > count());

  // Increase the capacity of the vector.
  size_t oldCount = _vector.count();
  _vector.grow(capacity);

  // Make sure the new elements are defined.
  for (size_t i = oldCount; i < capacity; i++) {
    VectorElement voidElement;
    _vector.insert(voidElement);
  }
}

  // @mfunc Is this <c OMWeakReferenceVectorProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @rdesc True if this <c OMWeakReferenceVectorProperty> is void,
  //          false otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorProperty<Key, ReferencedObject>::isVoid(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::isVoid");

  bool result = true;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    OMStorable* object = element.getValue();
    if (object != 0) {
      result = false;
      break;
    }
  }
  return result;
}

  // @mfunc Remove this optional <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeProperty(void)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeProperty");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc The size of the raw bits of this
  //        <c OMWeakReferenceVectorProperty>. The size is given in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @rdesc The size of the raw bits of this
  //          <c OMWeakReferenceVectorProperty> in bytes.
  //   @this const
template <typename Key, typename ReferencedObject>
OMUInt32 OMWeakReferenceVectorProperty<Key, ReferencedObject>::bitsSize(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::bitsSize");
  OBSOLETE("methods on class OMReferenceVectorProperty");

  return sizeof(ReferencedObject*) * count();
}

  // @mfunc Get the raw bits of this <c OMWeakReferenceVectorProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm OMUInt32 | size | The size of the buffer.
  //   @this const
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::getBits(
                                                    OMByte* bits,
                                                    OMUInt32 ANAME(size)) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceVectorProperty");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const OMStorable** p = (const OMStorable**)bits;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    *p++ = element.getValue();
  }
}

  // @mfunc Set the raw bits of this
  //        <c OMWeakReferenceVectorProperty>. The raw bits are
  //        copied from the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::setBits(
                                                            const OMByte* bits,
                                                            OMUInt32 size)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::setBits");
  OBSOLETE("methods on class OMReferenceVectorProperty");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  size_t elementCount = size / sizeof(ReferencedObject*);
  ReferencedObject** p = (ReferencedObject**)bits;

  for (OMUInt32 i = 0; i < elementCount; i++) {
    ReferencedObject* object = p[i];
    if (i < count()) {
      setValueAt(object, i);
    } else {
      appendValue(object);
    }
  }

}

  // @mfunc Insert <p object> into this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The <c OMObject> to insert.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::insertObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::insertObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  insert(p);
}

  // @mfunc Does this <c OMWeakReferenceVectorProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The <c OMObject> for which to search.
  //   @rdesc True if <p object> is present, false otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool
OMWeakReferenceVectorProperty<Key, ReferencedObject>::containsObject(
                                                  const OMObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::containsObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return containsValue(p);
}

  // @mfunc Remove <p object> from this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The <c OMObject> to remove.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  removeValue(p);
}

  // @mfunc Remove all objects from this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeAllObjects(void)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeAllObjects");

  _vector.clear();
  POSTCONDITION("All objects removed", count() == 0);
}

  // @mfunc Create an <c OMReferenceContainerIterator> over this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @rdesc An <c OMReferenceContainerIterator> over this
  //          <c OMWeakReferenceVectorProperty>.
  //   @this const
template <typename Key, typename ReferencedObject>
OMReferenceContainerIterator*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::createIterator(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::createIterator");

  OMWeakReferenceVectorIterator<Key, ReferencedObject>* result =
     new OMWeakReferenceVectorIterator<Key, ReferencedObject>(*this, OMBefore);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> to <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The new object.
  //   @parm The index.
  //   @rdesc The old object.
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::setObjectAt(
                                                        const OMObject* object,
                                                        const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::setObjectAt");

  PRECONDITION("Valid index", index < count());
  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return setValueAt(p, index);
}

  // @mfunc The value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The index.
  //   @rdesc The object.
  //   @this const
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::getObjectAt(
                                                    const OMUInt32 index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::getObjectAt");

  PRECONDITION("Valid index", index < count());

  return valueAt(index);
}

  // @mfunc Append the given <p OMObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The object to append.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::appendObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::appendObject");

  PRECONDITION("Valid object", object != 0);

  insertObjectAt(object, count());
}

  // @mfunc Prepend the given <p OMObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The object to prepend.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::prependObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::prependObject");

  PRECONDITION("Valid object", object != 0);

  insertObjectAt(object, 0);
}

  // @mfunc Remove the object from this
  //        <c OMWeakReferenceVectorProperty> at position <p index>.
  //        Existing objects in this <c OMWeakReferenceVectorProperty>
  //        at <p index> + 1 and higher are shifted down one index
  //        position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The index of the object to remove.
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeObjectAt(
                                                          const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::removeObjectAt");

  PRECONDITION("Valid index", index < count());

  return removeAt(index);
}

  // @mfunc Insert <p object> into this <c OMWeakReferenceVectorProperty>
  //        at position <p index>. Existing objects at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @parm The object to insert.
  //   @parm The index at which to insert the object.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::insertObjectAt(
                                                        const OMObject* object,
                                                        const OMUInt32 index)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::insertObjectAt");

  PRECONDITION("Valid index", index <= count());
  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  insertAt(p, index);
}

template <typename Key, typename ReferencedObject>
OMContainerIterator<OMWeakReferenceVectorElement>*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::iterator(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::iterator");

  OMVectorIterator<VectorElement>* result =
                        new OMVectorIterator<VectorElement>(_vector, OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::insert(
                                   const OMUInt32 index,
                                   const OMWeakReferenceVectorElement& element)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::insert");

  _vector.setAt(element, index);
}

template <typename Key, typename ReferencedObject>
OMPropertyTag
OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceVectorProperty<Key, ReferencedObject>* nonConstThis =
       const_cast<OMWeakReferenceVectorProperty<Key, ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                  file()->referencedProperties()->insert(targetPropertyPath());
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

template <typename Key, typename ReferencedObject>
const Key&
OMWeakReferenceVectorProperty<Key, ReferencedObject>::identification(
                                                          OMUInt32 index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::identification");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  return *reinterpret_cast<const Key*>(element.identification());
}

  // @mfunc The size of the identification of <c OMObject> in this
  //        <c OMWeakReferenceSetProperty>. The size is given
  //        in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @tcarg class | Key | The type of the identifier of the referenced
  //          (contained) object.
  //   @rdesc The size of the identification.
template <typename Key, typename ReferencedObject>
OMKeySize
OMWeakReferenceVectorProperty<Key, ReferencedObject>::keySize(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::keySize");

  return sizeof(Key);
}

template <typename Key, typename ReferencedObject>
OMStrongReferenceSet*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetSet(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetSet");
  OMWeakReferenceVectorProperty<Key, ReferencedObject>* nonConstThis =
       const_cast<OMWeakReferenceVectorProperty<Key, ReferencedObject>*>(this);
  if (_targetSet == 0) {
    nonConstThis->_targetSet = OMWeakObjectReference::targetSet(this,
                                                                targetTag());
  }
  POSTCONDITION("Valid result", _targetSet != 0);
  return _targetSet;
}

template <typename Key, typename ReferencedObject>
OMPropertyId
OMWeakReferenceVectorProperty<Key, ReferencedObject>::keyPropertyId(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::keyPropertyId");

  return _keyPropertyId;
}

template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::setTargetTag(
                                                       OMPropertyTag targetTag)
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::setTargetTag");

  _targetTag = targetTag;
}

template <typename Key, typename ReferencedObject>
OMPropertyId*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetPropertyPath(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetPropertyPath");

  if (_targetPropertyPath == 0) {
    ASSERT("Valid target name", validWideString(_targetName));
    OMWeakReferenceVectorProperty<Key, ReferencedObject>* nonConstThis =
       const_cast<OMWeakReferenceVectorProperty<Key, ReferencedObject>*>(this);
    nonConstThis->_targetPropertyPath = file()->path(_targetName);
  }
  POSTCONDITION("Valid result", _targetPropertyPath != 0);
  return _targetPropertyPath;
}

template <typename Key, typename ReferencedObject>
void
OMWeakReferenceVectorProperty<Key, ReferencedObject>::clearTargetTag(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::clearTargetTag");

  OMWeakReferenceVectorProperty<Key, ReferencedObject>* nonConstThis =
       const_cast<OMWeakReferenceVectorProperty<Key, ReferencedObject>*>(this);

  nonConstThis->_targetTag = nullOMPropertyTag;
  delete [] nonConstThis->_targetPropertyPath;
  nonConstThis->_targetPropertyPath = 0;
}

template <typename Key, typename ReferencedObject>
const wchar_t*
OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetName(void) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::targetName");

  return _targetName;
}

template <typename Key, typename ReferencedObject>
bool
OMWeakReferenceVectorProperty<Key, ReferencedObject>::isResolved(
                                                          OMUInt32 index) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::isResolved");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  bool result;
  if (element.pointer() != 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

template <typename Key, typename ReferencedObject>
bool
OMWeakReferenceVectorProperty<Key, ReferencedObject>::isResolvable(
                                                   OMUInt32 ANAME(index)) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::isResolvable");
  PRECONDITION("Valid index", index < count());

  bool result;
  OMFile* file = propertySet()->container()->file();
  ASSERT("Valid file", file != 0);
  if (file->propertyTableExists()) {
    OMPropertyTable* table = file->referencedProperties();
    ASSERT("Valid table", table != 0);
    if (table->isValid(_targetTag)) {
      result = true;
    } else {
      result = false;
    }
  } else {
    result = false;
  }
  return result;
}

template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::shallowCopyTo(
                                                 OMProperty* destination) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMWeakReferenceVectorProperty Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  ASSERT("Destination vector is void", dest->isVoid());
  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    VectorElement destElement(dest,
                              element.identification(),
                              sizeof(Key),
                              nullOMPropertyTag);
    dest->_vector.insert(destElement);
  }

  dest->_targetTag = nullOMPropertyTag;
  dest->_targetName = _targetName;
  delete [] dest->_targetPropertyPath;
  dest->_targetPropertyPath = 0;
  dest->_keyPropertyId = _keyPropertyId;
  dest->setPresent();
}

template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorProperty<Key, ReferencedObject>::deepCopyTo(
                                                     OMProperty* destination,
                                                     void* clientContext) const
{
  TRACE("OMWeakReferenceVectorProperty<Key, ReferencedObject>::deepCopyTo");
  PRECONDITION( "Valid destination", destination != 0 );

  typedef OMWeakReferenceVectorProperty<Key, ReferencedObject> Property;
  Property* wp = dynamic_cast<Property*>(destination);
  ASSERT("Correct property type", wp != 0);

  // Update the target tags on elements
  VectorIterator destIterator(wp->_vector, OMBefore);
  while (++destIterator) {
    VectorElement& element = destIterator.value();
    element.reference().setTargetTag(wp->targetTag());
  }

  OMStrongReferenceSet* dest = wp->targetSet();
  ASSERT("Destination is correct type", dest != 0);

  OMStorable* container = dest->container();
  ASSERT("Valid container", container != 0);
  OMClassFactory* factory = container->classFactory();
  ASSERT("Valid class factory", factory != 0);

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    OMStorable* source = element.getValue();
    if (source != 0) {
      const void* id = element.identification();
      // contains() should take 'const void*'
      if (!dest->contains(const_cast<void*>(id))) {
        OMStorable* d = source->shallowCopy(factory);
        dest->insertObject(d);
        d->onCopy(clientContext);
        source->deepCopyTo(d, clientContext);
      }
    }
  }
}

#endif
