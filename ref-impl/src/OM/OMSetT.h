//=---------------------------------------------------------------------=
//
// $Id: OMSetT.h,v 1.18 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSet
#ifndef OMSETT_H
#define OMSETT_H

#include "OMAssertions.h"

template <typename Key, typename Element>
OMSet<Key, Element>::OMSet()
{
}

template <typename Key, typename Element>
OMSet<Key, Element>::~OMSet()
{
}

  // @mfunc Insert <p value> into this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key>.
  //   @parm The <p Element> to insert.
template <typename Key, typename Element>
void OMSet<Key, Element>::insert(const Key key, const Element value)
{
  TRACE("OMSet<Key, Element>::insert");
  PRECONDITION("Element not present", !_tree.contains(key));

  _tree.insert(key, value);

  POSTCONDITION("Element present", _tree.contains(key));
}

  // @mfunc Does this <c OMSet> contain an <p Element>
  //        identified by <p key>?
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> for which to search.
  //   @rdesc True if this <c OMSet> contains an <p Element>
  //          identified by <p key>, false otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSet<Key, Element>::contains(const Key key) const
{
  TRACE("OMSet<Key, Element>::contains");

  bool result = _tree.contains(key);

  return result;
}

  // @mfunc Find the <p Element> in this <c OMSet> identified by
  //        <p key>.  If the element is found it is returned in
  //        <p value> and the result is true. If the element is
  //        not found the result is false.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> for which to search.
  //   @parm The value that was found, if any, by reference.
  //   @rdesc True if this <c OMSet> contains an <p Element>
  //          identified by <p key>, false otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSet<Key, Element>::find(const Key key, Element& value) const
{
  TRACE("OMSet<Key, Element>::find");

  bool result = _tree.find(key, value);

  return result;
}

  // @mfunc Find the <p Element> in this <c OMSet> identified by
  //        <p key>.  If the element is found it is returned in
  //        <p value> and the result is true. If the element is
  //        not found the result is false.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> for which to search.
  //   @parm The value that was found, if any, by pointer.
  //   @rdesc True if this <c OMSet> contains an <p Element>
  //          identified by <p key>, false otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSet<Key, Element>::find(const Key key, Element** value) const
{
  TRACE("OMSet<Key, Element>::find");

  bool result = _tree.find(key, value);

  return result;
}

  // @mfunc The number of elements in this <c OMSet>.
  //        <mf OMSet::count> returns the actual number
  //        of elements in the <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @rdesc The count of elements in this <c OMSet>.
  //   @this const
template <typename Key, typename Element>
OMUInt32 OMSet<Key, Element>::count(void) const
{
  TRACE("OMSet<Key, Element>::count");

  OMUInt32 result = _tree.count();

  return result;
}

  // @mfunc Append the given <p Element> <p value> to
  //        this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key>.
  //   @parm The <p Element> to append.
template <typename Key, typename Element>
void OMSet<Key, Element>::append(const Key key, const Element value)
{
  TRACE("OMSet<Key, Element>::append");

  insert(key, value);
}

  // @mfunc Remove the <p Element> with <p Key> <p key> from this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> of the <p Element> to remove.
template <typename Key, typename Element>
void OMSet<Key, Element>::remove(const Key key)
{
  TRACE("OMSet<Key, Element>::remove");

  _tree.remove(key);
}

  // @mfunc Remove all elements from this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Element> to remove.
template <typename Key, typename Element>
void OMSet<Key, Element>::clear(void)
{
  TRACE("OMSet<Key, Element>::clear");

  _tree.clear();
  POSTCONDITION("All elements removed", count() == 0);
}

#endif
