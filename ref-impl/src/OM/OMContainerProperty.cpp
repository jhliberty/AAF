//=---------------------------------------------------------------------=
//
// $Id: OMContainerProperty.cpp,v 1.8 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMContainerProperty

#include "OMContainerProperty.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"

#include <stdio.h>

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The stored form of this property.
  //   @parm The name of this property.
OMContainerProperty::OMContainerProperty(const OMPropertyId propertyId,
                                         const OMStoredForm storedForm,
                                         const wchar_t* name)
: OMProperty(propertyId, storedForm, name), _localKey(0)
{
  TRACE("OMContainerProperty::OMContainerProperty");
}

  // @mfunc Destructor.
OMContainerProperty::~OMContainerProperty(void)
{
  TRACE("OMContainerProperty::~OMContainerProperty");
}

  // @mfunc The current local key.
  //   @rdesc The current local key.
  //   @this const
OMUInt32 OMContainerProperty::localKey(void) const
{
  return _localKey;
}

  // @mfunc Set the current local key. Used on restore to restart
  //        local key assignment.
  //   @parm The new local key.
void OMContainerProperty::setLocalKey(OMUInt32 newLocalKey)
{
  _localKey = newLocalKey;
}

  // @mfunc Compute the name of an element in this <c OMContainer>
  //        given the element's <p localKey>.
  //   @parm The element's local key.
wchar_t* OMContainerProperty::elementName(OMUInt32 localKey)
{
  TRACE("OMContainerProperty::elementName");

  wchar_t* name = OMStoredObject::elementName(_name, propertyId(), localKey);
  return name;
}

  // @mfunc Obtain the next available local key.
  //   @rdesc The next available local key.
OMUInt32 OMContainerProperty::nextLocalKey(void)
{
  return _localKey++;
}

const wchar_t* OMContainerProperty::storedName(void) const
{
  TRACE("OMContainerProperty::storedName");

  if (_storedName == 0) {
    OMContainerProperty* p = const_cast<OMContainerProperty*>(this);
    p->_storedName = OMStoredObject::collectionName(_name, propertyId());
  }
  return _storedName;
}
