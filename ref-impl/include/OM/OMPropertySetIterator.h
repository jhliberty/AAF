//=---------------------------------------------------------------------=
//
// $Id: OMPropertySetIterator.h,v 1.10 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMPROPERTYSETITERATOR_H
#define OMPROPERTYSETITERATOR_H

#include "OMContainerIterator.h"
#include "OMVectorIterator.h"
#include "OMDataTypes.h"

class OMProperty;
class OMPropertySet;

  // @class Iterators over <c OMPropertySet>s.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMPropertySetIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMPropertySetIterator> over the given
    //          <c OMPropertySet> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the forward direction
    //          (increasing <p OMPropertyId>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the reverse direction
    //          (decreasing <p OMPropertyId>s).
  OMPropertySetIterator(const OMPropertySet& set,
                        OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMPropertySetIterator>.
  virtual ~OMPropertySetIterator(void);

    // @cmember Reset this <c OMPropertySetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the forward direction
    //          (increasing <p OMPropertyId>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the reverse direction
    //          (decreasing <p OMPropertyId>s).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMPropertySetIterator> positioned before the first
    //          <c OMProperty> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMPropertySetIterator> positioned after the last
    //          <c OMProperty> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMPropertySetIterator> validly positioned on
    //          an <c OMProperty> ?
  virtual bool valid(void) const;

    // @cmember The number of <c OMProperty>s in the associated
    //          <c OMPropertySet>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMPropertySetIterator> to the next
    //          <c OMProperty>, if any.
    //          If the end of the associated <c OMPropertySet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.true> and
    //          <mf OMPropertySetIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMPropertySet> is reached
    //          then the result is <e bool.false>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.false> and
    //          <mf OMPropertySetIterator::after> becomes <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMPropertySetIterator> to the previous
    //          <c OMProperty>, if any.
    //          If the beginning of the associated <c OMPropertySet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.true> and
    //          <mf OMPropertySetIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMPropertySet> is
    //          reached then the result is <e bool.false>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.false> and
    //          <mf OMPropertySetIterator::before> becomes <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <c OMProperty> in the associated <c OMPropertySet>
    //          at the position currently designated by this
    //          <c OMPropertySetIterator>.
  virtual OMProperty* property(void) const;

    // @cmember Return the <p OMPropertyId> of the <c OMProperty> in the
    //          associated <c OMPropertySet> at the position currently
    //          designated by this <c OMPropertySetIterator>.
  OMPropertyId propertyId(void) const;

private:

  OMVectorIterator<OMProperty*> _iterator;

};


#endif

