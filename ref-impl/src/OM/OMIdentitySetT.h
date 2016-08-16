//=---------------------------------------------------------------------=
//
// $Id: OMIdentitySetT.h,v 1.7 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMIdentitySet
#ifndef OMIDENTITYSETT_H
#define OMIDENTITYSETT_H

#include "OMAssertions.h"

template <typename Element>
OMIdentitySet<Element>::OMIdentitySet()
{
}

template <typename Element>
OMIdentitySet<Element>::~OMIdentitySet()
{
}

template <typename Element>
void OMIdentitySet<Element>::insert(const Element& element)
{
  TRACE("OMIdentitySet<Element>::insert");
  PRECONDITION("Element not present", !_tree.contains(element));

  _tree.insert(element, 0);

  POSTCONDITION("Element present", _tree.contains(element));
}

template <typename Element>
bool OMIdentitySet<Element>::ensurePresent(const Element& element)
{
  TRACE("OMIdentitySet<Element>::ensurePesent");

  bool present = contains(element);
  if (!present) {
    insert(element);
  }
  POSTCONDITION("Object is present", contains(element));
  return !present;
}

template <typename Element>
bool OMIdentitySet<Element>::contains(const Element& element) const
{
  TRACE("OMIdentitySet<Element>::contains");

  bool result = _tree.contains(element);

  return result;
}

template <typename Element>
OMUInt32 OMIdentitySet<Element>::count(void) const
{
  TRACE("OMIdentitySet<Element>::count");

  OMUInt32 result = _tree.count();

  return result;
}

template <typename Element>
void OMIdentitySet<Element>::remove(const Element& element)
{
  TRACE("OMIdentitySet<Element>::remove");

  _tree.remove(element);
}

template <typename Element>
void OMIdentitySet<Element>::clear(void)
{
  TRACE("OMIdentitySet<Element>::clear");

  _tree.clear();
}

template <typename Element>
bool OMIdentitySet<Element>::ensureAbsent(const Element& element)
{
  TRACE("OMIdentitySet<Element>::ensureAbsent");

  bool result = contains(element);
  if (result) {
    remove(element);
  }

  POSTCONDITION("Object is not present", !contains(element));
  return result;
}

#endif
