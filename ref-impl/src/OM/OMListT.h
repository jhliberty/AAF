//=---------------------------------------------------------------------=
//
// $Id: OMListT.h,v 1.9 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMList
#ifndef OMLISTT_H
#define OMLISTT_H

#include "OMAssertions.h"
#include "OMContainerIterator.h"

// This implementation was taken from [1].
//
// References ...
//
// [1] "Introduction to Algorithms", Thomas H. Cormen, Charles
// E. Leiserson, Ronald L. Rivest, 1997 MIT Press (McGraw-Hill) ISBN
// 0-262-03141-8, page 204
//

  // @mfunc Constructor.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
template <typename Element>
OMList<Element>::OMList()
: _nil(0),
  _count(0)
{
  TRACE("OMList<Element>::OMList");

  _nil = new Node();
  ASSERT("Valid heap pointer", _nil != 0);

  _nil->_next = _nil;
  _nil->_previous = _nil;
  //_nil->_value = 0;
}

  // @mfunc Destructor.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
template <typename Element>
OMList<Element>::~OMList(void)
{
  TRACE("OMList<Element>::~OMList");

  clear();

  delete _nil;
  _nil = 0;
}

  // @mfunc Insert <p value> into this <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @parm The Element to insert. A value of type <p Element> by value.
template <typename Element>
void OMList<Element>::insert(const Element value)
{
  TRACE("OMList<Element>::insert");

  prepend(value);
}

template <typename Element>
void OMList<Element>::insertAfter(
                                 const Element /* value */,
                                 const OMListIterator<Element>& /* position */)
{
  TRACE("OMList<Element>::insertAfter");
  ASSERT("Unimplemented code not reached", false);
}

template <typename Element>
void OMList<Element>::insertBefore(
                                 const Element /* value */,
                                 const OMListIterator<Element>& /* position */)
{
  TRACE("OMList<Element>::insertBefore");
  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Does this <c OMList> contain <p value> ?
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @parm The Element to search for. A value of type <p Element> by value.
  //   @this const
template <typename Element>
bool OMList<Element>::containsValue(const Element value) const
{
  TRACE("OMList<Element>::containsValue");

  // Cormen et. al. page 207, LIST-SEARCH'
  //
  bool result = false;
  Node* p = _nil->_next;
  while (p != _nil) {
    if (p->_value == value) {
      result = true;
      break;
    }
    p = p->_next;
  }
  return result;
}

  // @mfunc The number of elements in this <c OMList>.
  //        <mf OMList::count> returns the actual number
  //        of elements in the <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @rdesc The number of elements in this <c OMList>.
  //   @this const
template <typename Element>
OMUInt32 OMList<Element>::count(void) const
{
  TRACE("OMList<Element>::count");

#if defined(OM_DEBUG)
  OMUInt32 n = 0;
  Node* p = _nil->_next;;
  while (p != _nil) {
    n = n + 1;
    p = p->_next;
  }
  ASSERT("Consistent forward count", n == _count);

  n = 0;
  p = _nil->_previous;
  while (p != _nil) {
    n = n + 1;
    p = p->_previous;
  }
  ASSERT("Consistent backward count", n == _count);
#endif
  return _count;
}

template <typename Element>
OMListIterator<Element> OMList<Element>::findValue(const Element& value)
{
  TRACE("OMList<Element>::findValue");

  OMListIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    if (iterator.value() == value) {
      return iterator;
    }
  }
  return iterator;
}

  // @mfunc Remove <p value> from this <c OMList>.
  //        In the case of duplicate values, the one closest to
  //        the head of the list is removed.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //  @parm The Element to remove. A value of type <p Element> by value.
template <typename Element>
void OMList<Element>::removeValue(const Element value)
{
  TRACE("OMList<Element>::removeValue");

  OMListIterator<Element> iterator = findValue(value);
  if (iterator.valid()) {
    remove(iterator);
  }
}

template <typename Element>
void OMList<Element>::remove(OMListIterator<Element>& position)
{
  TRACE("OMList<Element>::remove");

  PRECONDITION("Valid iterator", position.valid());
  PRECONDITION("Consistent iterator and list", position._list == this);

  Node* node = position._current;
  ASSERT("Valid node", node != 0);
  ++position; // Advance past the node to be removed
  remove(node);
}

  // @mfunc Is this <c OMList> empty ?
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @rdesc True if this <c OMList> is empty, false otherwise ?
  //   @this const
template <typename Element>
bool OMList<Element>::empty(void) const
{
  TRACE("OMList<Element>::empty");

  bool result;
  if (count() == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Append the given <p Element> <p value> to
  //        this <c OMList>. The new element is added after
  //        the last element currently in this <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @parm The value to append.
template <typename Element>
void OMList<Element>::append(const Element value)
{
  TRACE("OMList<Element>::append");

  insertAfter(value, _nil->_previous);
}

  // @mfunc Prepend the given <p Element> <p value> to
  //        this <c OMList>. The new element is added before
  //        the first element currently in this <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @parm The value to prepend.
template <typename Element>
void OMList<Element>::prepend(const Element value)
{
  TRACE("OMList<Element>::prepend");

  insertBefore(value, _nil->_next);
}

  // @mfunc Remove the last element from this <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
template <typename Element>
void OMList<Element>::removeLast(void)
{
  TRACE("OMList<Element>::removeLast");
  PRECONDITION("Not empty", !empty());

  remove(_nil->_previous);
}

  // @mfunc Remove the first element from this <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
template <typename Element>
void OMList<Element>::removeFirst(void)
{
  TRACE("OMList<Element>::removeFirst");
  PRECONDITION("Not empty", !empty());

  remove(_nil->_previous);
}

  // @mfunc Remove all elements from this <c OMList>.
  //        from this <c OMList>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
template <typename Element>
void OMList<Element>::clear(void)
{
  TRACE("OMList<Element>::clear");

  Node* p = _nil->_next;
  while (p != _nil) {
    Node* q = p;
    p = p->_next;
    delete q;
  }
  _nil->_next = _nil;
  _nil->_previous = _nil;
  _count = 0;
}

  // @mfunc The number of elements with value <p value>.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @parm The number of times the value occurs.
  //   @this const
template <typename Element>
OMUInt32 OMList<Element>::countValue(const Element value) const
{
  TRACE("OMList<Element>::countValue");

  OMUInt32 result = 0;
  Node* p = _nil->_next;
  while (p != _nil) {
    if (p->_value == value) {
      result = result + 1;
    }
    p = p->_next;
  }
  return result;
}

template <typename Element>
OMListIterator<Element> OMList<Element>::first(void) const
{
  TRACE("OMList<Element>::first");
  PRECONDITION("List not empty", !empty());

  OMListIterator<Element> result(*this, OMBefore);
  ++result;
  return result;
}

template <typename Element>
OMListIterator<Element> OMList<Element>::last(void) const
{
  TRACE("OMList<Element>::last");
  PRECONDITION("List not empty", !empty());

  OMListIterator<Element> result(*this, OMAfter);
  --result;
  return result;
}

template <typename Element>
void OMList<Element>::insertAfter(const Element value, Node* node)
{
  TRACE("OMList<Element>::insertAfter");
  PRECONDITION("Valid node", node != 0);

  // Cormen et. al. page 207, LIST-INSERT'
  //
  Node* q =  new Node();
  ASSERT("Valid heap pointer", q != 0);

  q->_value = value;
  q->_next = node->_next;
  node->_next = q;
  q->_previous = node;
  q->_next->_previous = q;

  _count = _count + 1;
}

template <typename Element>
void OMList<Element>::insertBefore(const Element value, Node* node)
{
  TRACE("OMList<Element>::insertBefore");
  PRECONDITION("Valid node", node != 0);

  insertAfter(value, node->_previous);
}

template <typename Element>
void OMList<Element>::remove(Node* node)
{
  TRACE("OMList<Element>::remove");

  // Cormen et. al. page 206, LIST-DELETE'
  //
  Node* p = node;

  p->_previous->_next = p->_next;
  p->_next->_previous = p->_previous;

  delete p;
  _count = _count - 1;
}

#endif
