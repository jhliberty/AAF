//=---------------------------------------------------------------------=
//
// $Id: OMUniqueObjectIdentType.h,v 1.10 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMUNIQUEOBJECTIDENTTYPE_H
#define OMUNIQUEOBJECTIDENTTYPE_H

#include "OMType.h"
#include "OMRecordType.h"
#include "OMDefinition.h"
#include "OMSingleton.h"

class OMUniqueObjectIdentificationType;

  // @class Type definition for OMUniqueObjectIdentification.
  //   @base public | <c OMType>
  //   @base public | <c OMSingleton>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMUniqueObjectIdentificationType :
                         public OMRecordType,
                         public OMSingleton<OMUniqueObjectIdentificationType>,
                         private OMBuiltinDefinition {
public:

  OMUniqueObjectIdentificationType(void);

  virtual ~OMUniqueObjectIdentificationType(void);

  virtual bool isFixedSize(void) const;

  virtual void reorder(OMByte* externalBytes,
                       OMUInt32 externalBytesSize) const;

  virtual OMUInt32 externalSize(const OMByte* internalBytes,
                                OMUInt32 internalBytesSize) const;

  virtual OMUInt32 externalSize(void) const;

  virtual void externalize(const OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual OMUInt32 internalSize(const OMByte* externalBytes,
                                OMUInt32 externalSize) const;

  virtual OMUInt32 internalSize(void) const;

  virtual void internalize(const OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual const OMUniqueObjectIdentification& identification(void) const;

  virtual const wchar_t* name(void) const;

  virtual bool hasDescription(void) const;

  virtual const wchar_t* description(void) const;

  virtual bool isPredefined(void) const;

  virtual OMUInt32 memberCount(void) const;

  virtual const wchar_t* memberName(OMUInt32 index) const;

  virtual const OMType* memberType(OMUInt32 index) const;

};

#endif
