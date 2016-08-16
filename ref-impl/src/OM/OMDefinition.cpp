//=---------------------------------------------------------------------=
//
// $Id: OMDefinition.cpp,v 1.7 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDefinition

#include "OMDefinition.h"
#include "OMAssertions.h"

OMBuiltinDefinition::OMBuiltinDefinition(
                            const OMStoredObjectIdentification& identification,
                            const wchar_t* name)
: _identification(identification),
  _name(name),
  _description(0),
  _hasDescription(false)
{
  TRACE("OMBuiltinDefinition::OMBuiltinDefinition");
}

OMBuiltinDefinition::OMBuiltinDefinition(
                            const OMStoredObjectIdentification& identification,
                            const wchar_t* name,
                            const wchar_t* descritpion)
: _identification(identification),
  _name(name),
  _description(descritpion),
  _hasDescription(true)
{
  TRACE("OMBuiltinDefinition::OMBuiltinDefinition");
}

OMBuiltinDefinition::~OMBuiltinDefinition(void)
{
}

const OMUniqueObjectIdentification&
OMBuiltinDefinition::identification(void) const
{
  return _identification;
}

const wchar_t* OMBuiltinDefinition::name(void) const
{
  return _name;
}

bool OMBuiltinDefinition::hasDescription(void) const
{
  TRACE("OMBuiltinDefinition::hasDescription");
  return _hasDescription;
}

const wchar_t* OMBuiltinDefinition::description(void) const
{
  TRACE("OMBuiltinDefinition::description");
  return _description;
}

bool OMBuiltinDefinition::isPredefined(void) const
{
  TRACE("OMBuiltinDefinition::isPredefined");
  ASSERT("Unimplemented code not reached", false);
  return true;
}

