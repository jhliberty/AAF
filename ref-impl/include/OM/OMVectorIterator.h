//=---------------------------------------------------------------------=
//
// $Id: OMVectorIterator.h,v 1.13 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMVECTORITERATOR_H
#define OMVECTORITERATOR_H

#include "OMDataTypes.h"
#include "OMContainerIterator.h"

template <typename Element>
class OMVector;

  // @class Iterators over <c OMVector>s.
  //   @tcarg class | Element | The type of the contained elements.
  //   @base public | <c OMContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMVectorIterator : public OMContainerIterator<Element> {
public:
  // @access Public members.

    // @cmember Create an <c OMVectorIterator> over the given
    //          <c OMVector> <p vector> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the forward direction
    //          (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the reverse direction
    //          (decreasing indexes).
  OMVectorIterator(const OMVector<Element>& vector,
                   OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMVectorIterator>.
  virtual ~OMVectorIterator(void);

    // @cmember Reset this <c OMVectorIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the forward direction
    //          (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the reverse direction
    //          (decreasing indexes).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMVectorIterator> positioned before the first
    //          <p Element> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMVectorIterator> positioned after the last
    //          <p Element> ?
  virtual bool after(void) const;

    // @cmember The number of <p Elements>s in the associated
    //          <c OMVector>.
  virtual OMUInt32 count(void) const;

    // @cmember Advance this <c OMVectorIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMVector> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMVectorIterator::valid> becomes <e bool.true> and
    //          <mf OMVectorIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMVector> is reached
    //          then the result is <e bool.false>,
    //          <mf OMVectorIterator::valid> becomes <e bool.false> and
    //          <mf OMVectorIterator::after> becomes <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMVectorIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMVector> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMVectorIterator::valid> becomes <e bool.true> and
    //          <mf OMVectorIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMVector> is
    //          reached then the result is <e bool.false>,
    //          <mf OMVectorIterator::valid> becomes <e bool.false> and
    //          <mf OMVectorIterator::before> becomes <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p Element> in the associated <c OMVector>
    //          at the position currently designated by this
    //          <c OMVectorIterator>.
  virtual Element& value(void) const;

    // @cmember Set the <p Element> in the associated <c OMVector> at the
    //          position currently designated by this
    //          <c OMVectorIterator> to <p newElement>. The previous
    //          <p Element> is returned.
  virtual Element setValue(Element newElement);

    // @cmember Return the index of the <p Element> in the
    //          associated <c OMVector> at the position
    //          currently designated by this <c OMVectorIterator>.
  virtual OMUInt32 index(void) const;

private:

  const OMVector<Element>* _vector;
  OMUInt32 _index;
  enum OMVectorIteratorState {OMVectorIteratorBefore,
                              OMVectorIteratorValid,
                              OMVectorIteratorAfter};
  OMVectorIteratorState _state;

};

#include "OMVectorIteratorT.h"

#endif

