//=---------------------------------------------------------------------=
//
// $Id: OMSetPropertyT.h,v 1.4 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
//         OMSetProperty
#ifndef OMSETPROPERTYT_H
#define OMSETPROPERTYT_H

#include "OMSetPropertyIterator.h"

template <typename Element>
OMSetProperty<Element>::OMSetProperty(const OMPropertyId propertyId,
                                      const wchar_t* name)
: OMDataSet(propertyId, SF_DATA_SET, name)
{
}

template <typename Element>
OMSetProperty<Element>::~OMSetProperty(void)
{
  TRACE("OMSetProperty<Element>::~OMSetProperty");
}

template <typename Element>
OMUInt32 OMSetProperty<Element>::count(void) const
{
  TRACE("OMSetProperty<Element>::count");
  return _set.count();
}

template <typename Element>
void OMSetProperty<Element>::insert(const Element& element)
{
  TRACE("OMSetProperty<Element>::insert");

  PRECONDITION("Element is not present", !contains(element));
  _set.insert(element);
  POSTCONDITION("Element is present", contains(element));
  setPresent(); // Mark property present
}

template <typename Element>
bool OMSetProperty<Element>::ensurePresent(const Element& element)
{
  TRACE("OMSetProperty<Element>::ensurePresent");

  bool present = contains(element);
  if (!present) {
    insert(element);
  }
  POSTCONDITION("Element is present", contains(element));
  return present;
}

template <typename Element>
void OMSetProperty<Element>::remove(const Element& element)
{
  TRACE("OMSetProperty<Element>::remove");
  PRECONDITION("Element is present", contains(element));
  _set.remove(element);
  POSTCONDITION("Element is not present", !contains(element));
}

template <typename Element>
bool OMSetProperty<Element>::ensureAbsent(const Element& element)
{
  TRACE("OMSetProperty<Element>::ensureAbsent");

  bool result = contains(element);
  if (result) {
    _set.remove(element);
  }
  POSTCONDITION("Element is not present", !contains(element));
  return result;
}

template <typename Element>
bool OMSetProperty<Element>::contains(const Element& element) const
{
  TRACE("OMSetProperty<Element>::contains");

  return _set.contains(element);
}

template <typename Element>
void OMSetProperty<Element>::clear(void)
{
  TRACE("OMSetProperty<Element>::clear");

  _set.clear();
  POSTCONDITION("Set is empty", count() == 0);
}

template <typename Element>
OMDataContainerIterator* OMSetProperty<Element>::createIterator(void) const
{
  TRACE("OMSetProperty<Element>::createIterator");
  OMDataContainerIterator* result =
                           new OMSetPropertyIterator<Element>(*this, OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

template <typename Element>
OMUInt64 OMSetProperty<Element>::objectCount(void) const
{
  TRACE("OMSetProperty<Element>::objectCount");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

template <typename Element>
OMUInt32 OMSetProperty<Element>::bitsSize(void) const
{
  TRACE("OMSetProperty<Element>::bitsSize");

  OMUInt32 result = count() * elementSize();
  return result;
}

template <typename Element>
void OMSetProperty<Element>::getBits(OMByte* bits, OMUInt32 ANAME(size)) const
{
  TRACE("OMSetProperty<Element>::getBits");

  PRECONDITION("Valid buffer", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  Element* buffer = reinterpret_cast<Element*>(bits);

  OMSetPropertyIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    *buffer++ = iterator.value();
  }
}

template <typename Element>
void OMSetProperty<Element>::setBits(const OMByte* /* bits */,
                                     OMUInt32 /* size */)
{
  TRACE("OMSetProperty<Element>::setBits");
  ASSERT("Unimplemented code not reached", false);
}

template <typename Element>
void OMSetProperty<Element>::shallowCopyTo(OMProperty* destination) const
{
  TRACE("OMSetProperty<Element>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMSetProperty<Element> Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  dest->clear();
  OMSetPropertyIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    dest->insert(iterator.value());
  }
}

template <typename Element>
void OMSetProperty<Element>::deepCopyTo(OMProperty* /* destination */,
                                        void* /* clientContext */) const
{
  TRACE("OMSetProperty<Element>::deepCopyTo");
  // Nothing to do - this is a deep copy
}

template <typename Element>
void OMSetProperty<Element>::insert(void* value)
{
  TRACE("OMSetProperty<Element>::insert");
  insert(*static_cast<Element*>(value));
}

template <typename Element>
OMUInt32 OMSetProperty<Element>::elementSize(void) const
{
  TRACE("OMSetProperty<Element>::elementSize");

  return sizeof(Element);
}

#endif
