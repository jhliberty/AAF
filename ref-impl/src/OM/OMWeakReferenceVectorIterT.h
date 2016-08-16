//=---------------------------------------------------------------------=
//
// $Id: OMWeakReferenceVectorIterT.h,v 1.16 2009/06/01 11:47:02 stuart_hc Exp $ $Name: V116 $
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
//         OMWeakReferenceVectorIterator
#ifndef OMWEAKREFERENCEVECTORITERT_H
#define OMWEAKREFERENCEVECTORITERT_H

#include "OMAssertions.h"

  // @mfunc Create an <c OMWeakReferenceVectorIterator> over the given
  //        <c OMWeakReferenceVectorProperty> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The <c OMWeakReferenceVector> over which to iterate.
  //   @parm The initial position for this <c OMWeakReferenceVectorIterator>.
template <typename Key, typename ReferencedObject>
OMWeakReferenceVectorIterator<Key, ReferencedObject>::OMWeakReferenceVectorIterator(
            const OMWeakReferenceVectorProperty<Key, ReferencedObject>& vector,
            OMIteratorPosition initialPosition)
  : _iterator(vector._vector, initialPosition)
{
}

  // @mfunc Destroy this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
template <typename Key, typename ReferencedObject>
OMWeakReferenceVectorIterator<Key, ReferencedObject>::
                                                ~OMWeakReferenceVectorIterator(
                                                                          void)
{
}

  // @mfunc Create a copy of this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The new <c OMWeakReferenceVectorIterator>.
  //   @this const
template <typename Key, typename ReferencedObject>
OMReferenceContainerIterator*
              OMWeakReferenceVectorIterator<Key, ReferencedObject>::copy(void) const
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::copy");

  OMWeakReferenceVectorIterator<Key, ReferencedObject>*
  result = new OMWeakReferenceVectorIterator<Key, ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMWeakReferenceVectorIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The position to which this <c OMWeakReferenceVectorIterator>
  //         should be reset.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceVectorIterator<Key, ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMWeakReferenceVectorIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceVectorIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorIterator<Key, ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMWeakReferenceVectorIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceVectorIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorIterator<Key, ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMWeakReferenceVectorIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceVectorIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorIterator<Key, ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename Key, typename ReferencedObject>
size_t OMWeakReferenceVectorIterator<Key, ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMWeakReferenceVectorIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMWeakReferenceVectorProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMWeakReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMWeakReferenceVectorIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMWeakReferenceVectorProperty> is reached then the result
  //        is <e bool.false>, <mf OMWeakReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMWeakReferenceVectorIterator::after> becomes
  //        <e bool.true>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.false> if this <c OMWeakReferenceVectorIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorIterator<Key, ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMWeakReferenceVectorIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMWeakReferenceVectorProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMWeakReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMWeakReferenceVectorIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMWeakReferenceVectorProperty> is reached then the result
  //        is <e bool.false>, <mf OMWeakReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMWeakReferenceVectorIterator::before> becomes
  //        <e bool.true>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.false> if this <c OMWeakReferenceVectorIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceVectorIterator<Key, ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorIterator<Key, ReferencedObject>::value(void) const
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::value");

  const VectorElement& element = _iterator.value();

  OMStorable* p = element.getValue();
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorIterator<Key, ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::setValue");

  Key id = OMConstant<Key>::null;
  if (newObject != 0) {
    id = newObject->identification();
  }

  VectorElement& element = _iterator.value();

  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(&id, newObject);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator> to 0.
  //        The previous <p ReferencedObject>, if any, is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorIterator<Key, ReferencedObject>::clearValue(void)
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::clearValue");

  VectorElement& element = _iterator.value();

  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(&(OMConstant<Key>::null), 0);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  return result;
}

  // @mfunc Return the index of the <p ReferencedObject> in the
  //        associated <c OMWeakReferenceVectorProperty> at the position
  //        currently designated by this
  //        <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The index of the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
size_t OMWeakReferenceVectorIterator<Key, ReferencedObject>::index(void) const
{
  return _iterator.index();
}

  // @mfunc Return the <p Key> of the <p ReferencedObject> in the
  //        associated <c OMWeakReferenceVectorProperty> at the position
  //        currently designated by this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
Key
OMWeakReferenceVectorIterator<Key, ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::identification");

  const VectorElement& element = _iterator.value();

  return element.identification();
}

  // @mfunc Return the <c OMObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The <c OMObject> at the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceVectorIterator<Key, ReferencedObject>::currentObject(void) const
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::currentObject");

  OMObject* result = value();

  return result;
}

  // @mfunc Clear (set to 0) the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator>.
  //        The existing object, if any, is returned. The associated
  //        reference container is not modified in that no entry is
  //        removed, the existing entry remains but no longer refers
  //        to a valid object.
  //   @rdesc The previous <c OMObject> at the current position.
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceVectorIterator<Key, ReferencedObject>::clearObject(void)
{
  TRACE("OMWeakReferenceVectorIterator<Key, ReferencedObject>::clearObject");

  OMObject* result = 0;
  ASSERT("Unimplemented code not reached", false); // tjb
  return result;
}

  // @mfunc Create an <c OMWeakReferenceVectorIterator> given
  //        an underlying <c OMVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The underlying <c OMVectorIterator>.
template <typename Key, typename ReferencedObject>
OMWeakReferenceVectorIterator<Key, ReferencedObject>::OMWeakReferenceVectorIterator(
                                                    const VectorIterator& iter)
  : _iterator(iter) // probably bitwise
{
}

#endif
