//=---------------------------------------------------------------------=
//
// $Id: OMStoredPropertySetIndex.cpp,v 1.33 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMStoredPropertySetIndex

#include "OMStoredPropertySetIndex.h"

#include "OMAssertions.h"

OMStoredPropertySetIndex::OMStoredPropertySetIndex(OMUInt16 capacity)
: _capacity(capacity), _index(0), _entries(0)
{
  TRACE("OMStoredPropertySetIndex::OMStoredPropertySetIndex");

  _index = new IndexEntry[_capacity];
  ASSERT("Valid heap pointer", _index != 0);

#if defined(OM_DEBUG)
  for (OMUInt16 i = 0; i < _capacity; i++) {
    _index[i]._propertyId = 0;
    _index[i]._storedForm = 0;
    _index[i]._length = 0;
    _index[i]._offset = 0;
  }
#endif
}

OMStoredPropertySetIndex::~OMStoredPropertySetIndex(void)
{
  TRACE("OMStoredPropertySetIndex::~OMStoredPropertySetIndex");

  delete [] _index;
  _index = 0;
}

  // @mfunc Insert a new property into this <c OMStoredPropertySetIndex>.
  //        The new property has id <p propertyId>. The stored property
  //        representation is <p storedForm>. The property value
  //        occupies <p length> bytes starting at offset <p offset>.
  //   @parm The id of the property to insert.
  //   @parm The stored form to use for the property.
  //   @parm The offset of the property value in bytes.
  //   @parm The size of the property value in bytes.
void OMStoredPropertySetIndex::insert(OMPropertyId propertyId,
                                      OMStoredForm storedForm,
                                      OMPropertyOffset offset,
                                      OMPropertySize length)
{
  TRACE("OMStoredPropertySetIndex::insert");

  PRECONDITION("Space for new entry", _entries < _capacity);
  OMStoredPropertySetIndex::IndexEntry* entry = &_index[_entries];
  _entries = _entries + 1;

  entry->_propertyId = propertyId;
  entry->_storedForm = storedForm;
  entry->_offset = offset;
  entry->_length = length;
}

  // @mfunc The number of properties in this <c OMStoredPropertySetIndex>.
  //   @rdesc The number of properties.
  //   @this const
OMUInt16 OMStoredPropertySetIndex::entries(void) const
{
  TRACE("OMStoredPropertySetIndex::entries");

  return _entries;
}

  // @mfunc Iterate over the properties in this <c OMStoredPropertySetIndex>.
  //   @parm Iteration  context. Set this to 0 to start with the
  //         "first" property.
  //   @parm The id of the "current" property.
  //   @parm The stored form used for the "current" property.
  //   @parm The offset of the "current" property value in bytes.
  //   @parm The size of the "current" property value in bytes.
  //   @this const
void OMStoredPropertySetIndex::iterate(OMUInt16& context,
                                       OMPropertyId& propertyId,
                                       OMStoredForm& storedForm,
                                       OMPropertyOffset& offset,
                                       OMPropertySize& length) const
{
  TRACE("OMStoredPropertySetIndex::iterate");

  PRECONDITION("Valid context", context < _entries);
  OMStoredPropertySetIndex::IndexEntry* entry = &_index[context];

  propertyId = entry->_propertyId;
  storedForm = entry->_storedForm;
  offset = entry->_offset;
  length = entry->_length;

  context = context + 1;
}

  // @mfunc Find the property with property id <p propertyId> in this
  //        <c OMStoredPropertySetIndex>. If found the <p storedForm>,
  //        <p offset> and <p length> of the property are returned.
  //   @parm The id of the property to find.
  //   @parm The stored form used for the property.
  //   @parm The offset of the property value in bytes.
  //   @parm The size of the property value in bytes.
  //   @rdesc True if a property with the given id was found, false otherwise.
  //   @this const
bool OMStoredPropertySetIndex::find(const OMPropertyId& propertyId,
                                    OMStoredForm& storedForm,
                                    OMPropertyOffset& offset,
                                    OMPropertySize& length) const
{
  bool result;

  OMStoredPropertySetIndex::IndexEntry* e = find(propertyId);
  if (e != 0) {
    storedForm = e->_storedForm;
    offset = e->_offset;
    length = e->_length;
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMStoredPropertySetIndex> valid ?
  //   @rdesc True if this <c OMStoredPropertySetIndex> is valid,
  //          false otherwise.
  //   @this const
bool OMStoredPropertySetIndex::isValid(OMPropertyOffset baseOffset) const
{
  TRACE("OMStoredPropertySetIndex::isValid");

  // The validity constraints are ...
  // 1) Each entry must have a non-zero length
  // 2) Entries must not overlap
  // 3) Entries must be in order of offset
  // 4) There must be no gaps between entries
  // We may choose to relax 3 and 4 in the future
  bool result = true;
  OMUInt16 entries = 0;
  OMUInt32 currentOffset;
  OMUInt16 currentLength;
  OMUInt32 position = baseOffset;

  for (OMUInt16 i = 0; i < _capacity; i++) {
    entries++; // count valid entries
    currentOffset = _index[i]._offset;
    currentLength = _index[i]._length;

    if (currentLength == 0) {
      result = false; // entry has invalid length
      break;
    }

    if (currentOffset != position) {
      result = false;  // gap or overlap
      break;
    }
    // calculate the expected next position
    position = position + currentLength;
  }

  if (entries != _entries) {
    result = false;
  }

  return result;
}

OMStoredPropertySetIndex::IndexEntry* OMStoredPropertySetIndex::find(
                                                 OMPropertyId propertyId) const
{
  TRACE("OMStoredPropertySetIndex::find");

  OMStoredPropertySetIndex::IndexEntry* result = 0;

  for (OMUInt16 i = 0; i < _capacity; i++) {
    if (_index[i]._propertyId == propertyId) {
      result = &_index[i];
      break;
    }
  }
  return result;
}
