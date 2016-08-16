//=---------------------------------------------------------------------=
//
// $Id: OMVectorIteratorT.h,v 1.14 2009/06/01 11:47:02 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMVectorIterator
#include "OMAssertions.h"

#include "OMVector.h"

  // @mfunc Create an <c OMVectorIterator> over the given
  //        <c OMVector> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the forward direction
  //        (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the reverse direction
  //        (decreasing indexes).
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The <c OMVector> over which to iterate.
  //   @parm The initial position for this <c OMVectorIterator>.
template <typename Element>
OMVectorIterator<Element>::OMVectorIterator(const OMVector<Element>& vector,
                                            OMIteratorPosition initialPosition)
  : _vector(&vector)
{
  TRACE("OMVectorIterator<Element>::OMVectorIterator");
  reset(initialPosition);
}

  // @mfunc Destroy this <c OMVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
template <typename Element>
OMVectorIterator<Element>::~OMVectorIterator(void)
{
  TRACE("OMVectorIterator<Element>::~OMVectorIterator");
}

  // @mfunc Reset this <c OMVectorIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the forward direction
  //        (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the reverse direction
  //        (decreasing indexes).
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The position to which this <c OMVectorIterator> should be reset.
template <typename Element>
void OMVectorIterator<Element>::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMVectorIterator<Element>::reset");

  PRECONDITION("Valid initial position",
                (initialPosition == OMBefore) || (initialPosition == OMAfter));

  if (initialPosition == OMBefore) {
    _state = OMVectorIteratorBefore;
  } else {
    _state = OMVectorIteratorAfter;
  }

  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMBefore, before()));
  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMAfter,  after()));
}

  // @mfunc Is this <c OMVectorIterator> positioned before the first
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMVectorIterator> is positioned before
  //          the first <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMVectorIterator<Element>::before(void) const
{
  TRACE("OMVectorIterator<Element>::before");

  bool result;
  if (_state == OMVectorIteratorBefore) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMVectorIterator> positioned after the last
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMVectorIterator> is positioned after
  //          the last <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMVectorIterator<Element>::after(void) const
{
  TRACE("OMVectorIterator<Element>::after");

  bool result;
  if (_state == OMVectorIteratorAfter) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The number of <p Elements>s in the associated
  //        <c OMVector>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The number of <p Elements>s
  //   @this const
template <typename Element>
OMUInt32 OMVectorIterator<Element>::count(void) const
{
  TRACE("OMVectorIterator<Element>::count");

  return _vector->count();
}

  // @mfunc Advance this <c OMVectorIterator> to the next <p Element>,
  //        if any.
  //        If the end of the associated <c OMVector> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMVectorIterator::valid> becomes <e bool.true> and
  //        <mf OMVectorIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMVector> is reached
  //        then the result is <e bool.false>,
  //        <mf OMVectorIterator::valid> becomes <e bool.false> and
  //        <mf OMVectorIterator::after> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMVectorIterator> has passed
  //          the last <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMVectorIterator<Element>::operator++()
{
  TRACE("OMVectorIterator<Element>::operator++()");

  PRECONDITION("Valid iterator", before() || this->valid());

  bool result;
  if (_state == OMVectorIteratorBefore) {
    // we are in state "before", so point to first value, if any
    if (_vector->count() != 0) {
      // vector contains some elements
      _index = 0;
      _state = OMVectorIteratorValid;
    } else {
      // vector is empty, go to state "after"
      _state = OMVectorIteratorAfter;
    }
  } else if (_index < _vector->count() - 1) {
    // advance
    _index = _index + 1;
    _state = OMVectorIteratorValid;
  } else {
    // no more elements, go to state "after"
    _state = OMVectorIteratorAfter;
  }
  if (_state == OMVectorIteratorValid) {
    result = true;
  } else {
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !after()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  after()));
  POSTCONDITION("Consistent result", IMPLIES( result,  this->valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !this->valid()));
  POSTCONDITION("Valid index",
                            IMPLIES(this->valid(), _index < _vector->count()));
  return result;
}

  // @mfunc Retreat this <c OMVectorIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated <c OMVector> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMVectorIterator::valid> becomes <e bool.true> and
  //        <mf OMVectorIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMVector> is
  //        reached then the result is <e bool.false>,
  //        <mf OMVectorIterator::valid> becomes <e bool.false> and
  //        <mf OMVectorIterator::before> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMVectorIterator> has passed
  //          the first <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMVectorIterator<Element>::operator--()
{
  TRACE("OMVectorIterator<Element>::operator--()");

  PRECONDITION("Valid iterator", after() || this->valid());

  bool result;
  if (_state == OMVectorIteratorAfter) {
    // we are in state "after", so point to last value, if any
    if (_vector->count() != 0) {
      // vector contains some elements
      _index = _vector->count() - 1;
      _state = OMVectorIteratorValid;
    } else {
      // vector is empty, go to state "before"
      _state = OMVectorIteratorBefore;
    }
  } else if (_index > 0) {
    // retreat
    _index = _index - 1;
    _state = OMVectorIteratorValid;
  } else {
    // no more elements, go to state "before"
    _state = OMVectorIteratorBefore;
  }
  if (_state == OMVectorIteratorValid) {
    result = true;
  } else {
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !before()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  before()));
  POSTCONDITION("Consistent result", IMPLIES( result,  this->valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !this->valid()));
  POSTCONDITION("Valid index",
                            IMPLIES(this->valid(), _index < _vector->count()));
  return result;
}

  // @mfunc Return the <p Element> in the associated <c OMVector>
  //        at the position currently designated by this
  //        <c OMVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Element>
Element& OMVectorIterator<Element>::value(void) const
{
  TRACE("OMVectorIterator<Element>::value");

  PRECONDITION("Valid iterator", this->valid());

  return _vector->getAt(_index);
}

  // @mfunc Set the <p Element> in the associated <c OMVector> at the
  //        position currently designated by this
  //        <c OMVectorIterator> to <p newElement>. The previous
  //        <p Element> is returned.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The new <p Element>.
  //   @rdesc The previous <p Element>.
template <typename Element>
Element OMVectorIterator<Element>::setValue(Element newElement)
{
  TRACE("OMVectorIterator<Element>::setValue");

  PRECONDITION("Valid iterator", this->valid());

  OMVector<Element>* vector = const_cast<OMVector<Element>*>(_vector);

  Element result = vector->getAt(_index);
  vector->setAt(newElement, _index);

  return result;
}

  // @mfunc Return the index of the <p Element> in the
  //        associated <c OMVector> at the position
  //        currently designated by this <c OMVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The index of the current position.
  //   @this const
template <typename Element>
OMUInt32 OMVectorIterator<Element>::index(void) const
{
  TRACE("OMVectorIterator<Element>::index");

  PRECONDITION("Valid iterator", this->valid());

  OMUInt32 result = _index;

  POSTCONDITION("Valid index", _index < _vector->count());

  return result;
}

