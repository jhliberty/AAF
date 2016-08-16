//=---------------------------------------------------------------------=
//
// $Id: OMTypeVisitor.cpp,v 1.2 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
#include "OMTypeVisitor.h"

#include "OMIntegerType.h"
#include "OMCharacterType.h"
#include "OMObjectReferenceType.h"
#include "OMRenamedType.h"
#include "OMEnumeratedType.h"
#include "OMArrayType.h"
#include "OMSetType.h"
#include "OMRecordType.h"
#include "OMStreamType.h"
#include "OMStringType.h"
#include "OMExtendibleEnumeratedType.h"
#include "OMIndirectType.h"
#include "OMOpaqueType.h"

#include "OMAssertions.h"

// class OMTypeCollector
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMTypeCollector

TypeCollector::TypeCollector(OMSet<OMUniqueObjectIdentification,
                             const OMType*>& types)
: _types(types)
{
  TRACE("TypeCollector::TypeCollector");
}

void
TypeCollector::visitIntegerType(const OMIntegerType* type)
{
  TRACE("TypeCollector::visitIntegerType");

  add(type);
}

void
TypeCollector::visitCharacterType(const OMCharacterType* type)
{
  TRACE("TypeCollector::visitCharacterType");

  add(type);
}

void
TypeCollector::visitStrongObjectReferenceType(
                                       const OMStrongObjectReferenceType* type)
{
  TRACE("TypeCollector::visitStrongObjectReferenceType");

  add(type);
}

void
TypeCollector::visitWeakObjectReferenceType(
                                         const OMWeakObjectReferenceType* type)
{
  TRACE("TypeCollector::visitWeakObjectReferenceType");

  add(type);
}

void TypeCollector::visitRenamedType(const OMRenamedType* type)
{
  TRACE("TypeCollector::visitRenamedType");

  add(type);
}

void
TypeCollector::visitEnumeratedType(const OMEnumeratedType* type)
{
  TRACE("TypeCollector::visitEnumeratedType");

  add(type);
}

void
TypeCollector::visitFixedArrayType(const OMFixedArrayType* type)
{
  TRACE("TypeCollector::visitFixedArrayType");

  add(type);
}

void
TypeCollector::visitVaryingArrayType(const OMVaryingArrayType* type)
{
  TRACE("TypeCollector::visitVaryingArrayType");

  add(type);
}

void
TypeCollector::visitSetType(const OMSetType* type)
{
  TRACE("TypeCollector::visitSetType");

  add(type);
}

void
TypeCollector::visitRecordType(const OMRecordType* type)
{
  TRACE("TypeCollector::visitRecordType");

  add(type);
}

void
TypeCollector::visitStreamType(const OMStreamType* type)
{
  TRACE("TypeCollector::visitStreamType");

  add(type);
}

void
TypeCollector::visitStringType(const OMStringType* type)
{
  TRACE("TypeCollector::visitStringType");

  add(type);
}

void
TypeCollector::visitExtendibleEnumeratedType(
                                        const OMExtendibleEnumeratedType* type)
{
  TRACE("TypeCollector::visitExtendibleEnumeratedType");

  add(type);
}

void
TypeCollector::visitIndirectType(const OMIndirectType* type)
{
  TRACE("TypeCollector::visitIndirectType");

  add(type);
}

void
TypeCollector::visitOpaqueType(const OMOpaqueType* type)
{
  TRACE("TypeCollector::visitOpaqueType");

  add(type);
}

void
TypeCollector::add(const OMType* /* type */)
{
#if defined(OM_EXTENSIONSONLY)
  TRACE("TypeCollector::add");

  ASSERT("Valid type", type != 0);
  if (!type->isPredefined()) {
    OMUniqueObjectIdentification id = type->identification();
    if (!_types.contains(id)) {
      _types.insert(id, type);
    }
  }
#endif
}
