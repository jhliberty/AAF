//=---------------------------------------------------------------------=
//
// $Id: OMVector.h,v 1.17 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMVECTOR_H
#define OMVECTOR_H

#include "OMDataTypes.h"
#include "OMContainer.h"

  // @class Elastic sequential collections of elements accessible by
  //        index.  The order of elements is determined externally.
  //        Duplicate elements are allowed.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @base public | <c OMContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMVector : public OMContainer<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVector();

    // @cmember Destructor.
  virtual ~OMVector(void);

    // @cmember Insert <p value> into this <c OMVector>.
  virtual void insert(const Element value);

    // @cmember Does this <c OMVector> contain <p value> ?
  virtual bool containsValue(const Element value) const;

    // @cmember The number of elements in this <c OMVector>.
    //          <mf OMVector::count> returns the actual number
    //          of elements in the <c OMVector>.
  OMUInt32 count(void) const;

    // @cmember Remove <p value> from this <c OMVector>.
    //          In the case of duplicate values, the one with the lowest
    //          index is removed.
  virtual void removeValue(const Element value);

    // @cmember The capacity of this <c OMVector>.
    //          <mf OMVector::capacity> returns the potential
    //          number of elements in the <c OMVector>.
  virtual OMUInt32 capacity(void) const;

    // @cmember Increase the capacity of this <c OMVector> so that it
    //          can contain at least <p capacity> elements without
    //          having to be resized.
  virtual void grow(OMUInt32 capacity);

    // @cmember Free any unused capacity in this <c OMVector> while
    //          ensuring that it can contain at least <p capacity>
    //          elements.
  virtual void shrink(OMUInt32 capacity);

    // @cmember Is this <c OMVector> full ?
  virtual bool full(void) const;

    // @cmember Is this <c OMVector> empty ?
  virtual bool empty(void) const;

    // @cmember Set the value of the <p Element> at
    //          position <p index> in this <c OMVector>.
    //          The existing <p Element> at <p index> is replaced.
  void setAt(const Element value, const OMUInt32 index);

    // @cmember Get the value of the <p Element> at
    //          position <p index> in this <c OMVector>.
  void getAt(Element& value, const OMUInt32 index) const;

    // @cmember Get the value of the <p Element> at
    //          position <p index> in this <c OMVector>.
  Element& getAt(const OMUInt32 index) const;

    // @cmember The value of the <p Element> at
    //          position <p index> in this <c OMVector>.
  Element& valueAt(const OMUInt32 index) const;

    // @cmember Insert <p value> into this <c OMVector> at
    //          position <p index>. Existing values in this
    //          <c OMVector> at <p index> and higher are
    //          shifted up one index position.
  virtual void insertAt(const Element value, const OMUInt32 index);

    // @cmember Append the given <p Element> <p value> to
    //          this <c OMVector>. The new element is added after
    //          the last element currently in this <c OMVector>.
  void append(const Element value);

    // @cmember Prepend the given <p Element> <p value> to
    //          this <c OMVector>. The new element is added before
    //          the first element currently in this <c OMVector>.
    //          Existing values in this <c OMVector> are shifted
    //          up one index position.
  void prepend(const Element value);

    // @cmember Remove the value from this <c OMVector> at
    //          position <p index>. Existing values in this
    //          <c OMVector> at <p index> + 1 and higher are
    //          shifted down on index position.
  virtual void removeAt(const OMUInt32 index);

    // @cmember Remove the last (index == count() - 1) element
    //          from this <c OMVector>.
  void removeLast(void);

    // @cmember Remove the first (index == 0) element
    //          from this <c OMVector>. Existing values in this
    //          <c OMVector> are shifted down one index position.
  void removeFirst(void);

    // @cmember Remove all elements from this <c OMVector>.
  virtual void clear(void);

    // @cmember The index of the element with value <p value>.
    //          In the case of duplicate values, lowest index is returned.
  OMUInt32 indexOfValue(const Element value) const;

    // @cmember The number of elements with value <p value>.
  OMUInt32 countValue(const Element value) const;

private:
  // @access Private members.

  Element* _vector;   // The representation of the vector
  OMUInt32 _capacity; // Potential number of elements
  OMUInt32 _count;    // Actual number of elements

    // @cmember Calculate the next valid capacity higher
    //          than <p capacity>.
  static OMUInt32 nextHigherCapacity(OMUInt32 capacity);

};

#include "OMVectorT.h"

#endif
