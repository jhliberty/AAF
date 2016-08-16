//=---------------------------------------------------------------------=
//
// $Id: OMPropertyDefinition.cpp,v 1.12 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
//         OMBuiltinPropertyDefinition

#include "OMPropertyDefinition.h"

#include "OMAssertions.h"

  // @mfunc Constructor.
OMBuiltinPropertyDefinition::OMBuiltinPropertyDefinition(
                                                 const OMType* type,
                                                 const wchar_t* name,
                                                 const wchar_t* /*description*/,
                                                 const OMPropertyId propertyId,
                                                 const bool isOptional)
: _type(type),
  _name(name),
  _propertyId(propertyId),
  _isOptional(isOptional)
{
  TRACE("OMBuiltinPropertyDefinition::OMBuiltinPropertyDefinition");
}

  // @mfunc Destructor.
OMBuiltinPropertyDefinition::~OMBuiltinPropertyDefinition(void)
{
  TRACE("OMBuiltinPropertyDefinition::~OMBuiltinPropertyDefinition");
}

  // @mfunc The type of the <c OMProperty> defined by this
  //        <c OMBuiltinPropertyDefinition>.
const OMType* OMBuiltinPropertyDefinition::type(void) const
{
  TRACE("OMBuiltinPropertyDefinition::type");
  return _type;
}

  // @mfunc The unique identification of the <c OMProperty> defined by
  //        this <c OMPropertyDefinition>.
const OMUniqueObjectIdentification&
OMBuiltinPropertyDefinition::identification(void) const
{
  TRACE("OMBuiltinPropertyDefinition::identification");
  ASSERT("Unimplemented code not reached", false); // tjb -stub
  return nullOMUniqueObjectIdentification;
}

  // @mfunc The name of the <c OMProperty> defined by this
  //        <c OMBuiltinPropertyDefinition>.
const wchar_t* OMBuiltinPropertyDefinition::name(void) const
{
  TRACE("OMBuiltinPropertyDefinition::name");
  return _name;
}

  // @mfunc The description of the <c OMProperty> defined by
  //          this <c OMBuiltinPropertyDefinition>.
  //          @precondition <f hasDefinition()>
bool OMBuiltinPropertyDefinition::hasDescription(void) const
{
  TRACE("OMBuiltinPropertyDefinition::hasDescription");
  return false;
}

  // @mfunc The description of the <c OMProperty> defined by
  //          this <c OMBuiltinPropertyDefinition>.
  //          @precondition <f hasDefinition()>
const wchar_t* OMBuiltinPropertyDefinition::description(void) const
{
  TRACE("OMBuiltinPropertyDefinition::description");
  PRECONDITION("Definition has a description", hasDescription());
  return 0;
}

  // @mfunc The locally unique identification of the <c OMProperty>
  //        defined by this <c OMBuiltinPropertyDefinition>.
OMPropertyId OMBuiltinPropertyDefinition::localIdentification(void) const
{
  TRACE("OMBuiltinPropertyDefinition::localIdentification");
  return _propertyId;
}

  // @mfunc Is the <c OMProperty> defined by this
  //        <c OMBuiltinPropertyDefinition> optional?
bool OMBuiltinPropertyDefinition::isOptional(void) const
{
  TRACE("OMBuiltinPropertyDefinition::isOptional");
  return _isOptional;
}

OMClassDefinition* OMBuiltinPropertyDefinition::containingClass(void) const
{
  TRACE("OMBuiltinPropertyDefinition::containingClass");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Is the <c OMProperty> defined by this
  //          <c OMBuiltinPropertyDefinition> predefined
  //          with respect to a well known baseline?
bool OMBuiltinPropertyDefinition::isPredefined(void) const
{
  TRACE("OMBuiltinPropertyDefinition::isPredefined");
  ASSERT("Unimplemented code not reached", false);
  return true;
}

bool OMBuiltinPropertyDefinition::isUniqueIdentifier(void) const
{
  TRACE("OMBuiltinPropertyDefinition::isUniqueIdentifier");
  ASSERT("Unimplemented code not reached", false);
  return true;
}
