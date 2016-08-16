//=---------------------------------------------------------------------=
//
// $Id: OMStoredVectorIndex.cpp,v 1.20 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStoredVectorIndex

#include "OMStoredVectorIndex.h"
#include "OMAssertions.h"

  // @mfunc Constructor.
  //   @parm The capacity of this <c OMStoredVectorIndex>.
OMStoredVectorIndex::OMStoredVectorIndex(OMUInt32 capacity)
: _firstFreeKey(0), _lastFreeKey(~(OMUInt32)0),
  _capacity(capacity), _entries(0), _localKeys(0)
{
  TRACE("OMStoredVectorIndex::OMStoredVectorIndex");

  _localKeys = new OMUInt32[_capacity];
  ASSERT("Valid heap pointer", _localKeys != 0);

  for (size_t i = 0; i < _capacity; i++) {
    _localKeys[i] = 0;
  }
}

  // @mfunc Destructor.
OMStoredVectorIndex::~OMStoredVectorIndex(void)
{
  TRACE("OMStoredVectorIndex::~OMStoredVectorIndex");

  delete [] _localKeys;
  _localKeys = 0;
}

  // @mfunc The first free key in the set of local keys assigned to
  //        this <c OMStoredVectorIndex>.
  //   @rdesc The highest previously allocated local key.
  //   @this const
OMUInt32 OMStoredVectorIndex::firstFreeKey(void) const
{
  TRACE("OMStoredVectorIndex::firstFreeKey");

  return _firstFreeKey;
}

  // @mfunc Set the first free key in the set of local keys assigned to
  //        this <c OMStoredVectorIndex>.
  //   @parm The highest allocated local key.
void OMStoredVectorIndex::setFirstFreeKey(OMUInt32 firstFreeKey)
{
  TRACE("OMStoredVectorIndex::setFirstFreeKey");

  _firstFreeKey = firstFreeKey;
}

  // @mfunc The last free key in the set of local keys assigned to
  //        this <c OMStoredVectorIndex>.
  //   @rdesc The highest previously allocated local key.
  //   @this const
OMUInt32 OMStoredVectorIndex::lastFreeKey(void) const
{
  TRACE("OMStoredVectorIndex::lastFreeKey");

  return _lastFreeKey;
}

  // @mfunc Set the last free key in the set of local keys assigned to
  //        this <c OMStoredVectorIndex>.
  //   @parm The highest allocated local key.
void OMStoredVectorIndex::setLastFreeKey(OMUInt32 lastFreeKey)
{
  TRACE("OMStoredVectorIndex::setLastFreeKey");

  _lastFreeKey = lastFreeKey;
}

  // @mfunc Insert a new element in this <c OMStoredVectorIndex>
  //        at position <p position> with local key <p localKey>.
  //        The local key of an element is an integer.
  //        The local key is used to derive the name of the storage
  //        on which an element is saved. Local keys are assigned
  //        such that the names of existing elements do not have to
  //        change when other elements are added to or removed from
  //        the associated <c OMStrongReferenceVector>. The local key is
  //        independent of the element's logical or physical position
  //        within the associated <c OMStrongReferenceVector>.
  //   @parm The position at which the new element should be inserted.
  //   @parm The local key assigned to the element.
void OMStoredVectorIndex::insert(OMUInt32 position, OMUInt32 localKey)
{
  TRACE("OMStoredVectorIndex::insert");
  PRECONDITION("Valid position", position < _capacity);

  _localKeys[position] = localKey;
  _entries = _entries + 1;
}

  // @mfunc The number of elements in this <c OMStoredVectorIndex>.
  //   @rdesc The number of elements.
  //   @this const
OMUInt32 OMStoredVectorIndex::entries(void) const
{
  TRACE("OMStoredVectorIndex::entries");

  return _entries;
}

  // @mfunc Iterate over the elements in this <c OMStoredVectorIndex>.
  //   @parm Iteration context. Set this to 0 to start with the
  //         "first" element.
  //   @parm The local key of the "current" element.
  //   @this const
void OMStoredVectorIndex::iterate(OMUInt32& context, OMUInt32& localKey) const
{
  TRACE("OMStoredVectorIndex::iterate");
  PRECONDITION("Valid context", context < _capacity);

  localKey = _localKeys[context];
  context = context + 1;
}

  // @mfunc Is this <c OMStoredVectorIndex> valid ?
  //   @rdesc True if this is a valid <c OMStoredVectorIndex>, false otherwise.
  //   @this const
bool OMStoredVectorIndex::isValid(void) const
{
  // No checks yet.
  // Possible checks include checking that all of the local keys are unique
  return true;
}
