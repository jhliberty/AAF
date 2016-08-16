//=---------------------------------------------------------------------=
//
// $Id: OMSet.h,v 1.16 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMSET_H
#define OMSET_H

#include "OMDataTypes.h"
#include "OMContainer.h"
#include "OMRedBlackTree.h"

template <typename Key, typename Element>
class OMSetIterator;

  // @class Sets of uniquely identified elements. Duplicate elements
  //        are not allowed.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @base public | <c OMContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Key, typename Element>
class OMSet : public OMContainer<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMSet();

    // @cmember Destructor.
  virtual ~OMSet(void);

    // @cmember Insert <p value> into this <c OMSet>.
  virtual void insert(const Key, const Element value);

    // @cmember Does this <c OMSet> contain an <p Element>
    //          identified by <p key>?
  virtual bool contains(const Key key) const;

    // @cmember Find the <p Element> in this <c OMSet> identified by
    //          <p key>.  If the element is found it is returned in
    //          <p value> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const Key key, Element& value) const;

    // @cmember Find the <p Element> in this <c OMSet> identified by
    //          <p key>.  If the element is found it is returned in
    //          <p value> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const Key key, Element** value) const;

    // @cmember The number of elements in this <c OMSet>.
    //          <mf OMSet::count> returns the actual number
    //          of elements in the <c OMSet>.
  OMUInt32 count(void) const;

    // @cmember Append the given <p Element> <p value> to
    //          this <c OMSet>.
  void append(const Key, const Element value);

    // @cmember Remove the <p Element> with <p Key> <p key>
    //          from this <c OMSet>.
  virtual void remove(const Key key);

    // @cmember Remove all elements from this <c OMSet>.
  virtual void clear(void);

private:
  // @access Private members.

    // The set is implemented as a binary search tree.
    //
  OMRedBlackTree<Key, Element> _tree;

  friend class OMSetIterator<Key, Element>;

};

#include "OMSetT.h"

#endif
