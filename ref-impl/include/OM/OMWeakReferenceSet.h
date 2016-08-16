//=---------------------------------------------------------------------=
//
// $Id: OMWeakReferenceSet.h,v 1.10 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMWEAKREFERENCESET_H
#define OMWEAKREFERENCESET_H

#include "OMRefSetProperty.h"

template <typename Element>
class OMContainerIterator;
class OMWeakReferenceSetElement;

  // @class Persistent sets of uniquely identified weakly referenced
  //        (non-contained) objects supported by the Object Manager.
  //        Objects are accessible by unique identifier (the key).
  //        The objects are not ordered. Duplicates objects are not allowed.
  //   @base public | <c OMReferenceSetProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMWeakReferenceSet : public OMReferenceSetProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceSet(const OMPropertyId propertyId,
                       const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMWeakReferenceSet(void);

  virtual OMContainerIterator<OMWeakReferenceSetElement>*
                                                      iterator(void) const = 0;

  virtual void insert(void* key,
                      const OMWeakReferenceSetElement& element) = 0;

  // @cmember The size of the identification of <c OMObject> in this
  //          <c OMWeakReferenceSetProperty>. The size is given
  //          in bytes.
  virtual OMKeySize keySize(void) const = 0;

  virtual OMPropertyId keyPropertyId(void) const = 0;

  virtual OMPropertyTag targetTag(void) const = 0;

  virtual void setTargetTag(OMPropertyTag targetTag) = 0;

  virtual void clearTargetTag(void) const = 0;

  virtual const wchar_t* targetName(void) const = 0;

};

#endif
