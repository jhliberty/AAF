//=---------------------------------------------------------------------=
//
// $Id: OMWeakReferenceVector.h,v 1.12 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMWEAKREFERENCEVECTOR_H
#define OMWEAKREFERENCEVECTOR_H

#include "OMDataTypes.h"
#include "OMRefVectorProperty.h"

template <typename Element>
class OMContainerIterator;
class OMWeakReferenceVectorElement;

  // @class Persistent elastic sequential collections of uniquely identified
  //        weakly referenced (non-contained) objects supported by the
  //        Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @base public | <c OMReferenceVectorProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMWeakReferenceVector : public OMReferenceVectorProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceVector(const OMPropertyId propertyId,
                        const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMWeakReferenceVector(void);

  virtual OMContainerIterator<OMWeakReferenceVectorElement>*
                                                      iterator(void) const = 0;

  virtual void insert(const OMUInt32 index,
                      const OMWeakReferenceVectorElement& element) = 0;

    // @cmember Increase the capacity of this
    //          <c OMWeakReferenceVector> so that it
    //          can contain at least <p capacity> <p OMObject>s
    //          without having to be resized.
  virtual void grow(const OMUInt32 capacity) = 0;

  // @cmember The size of the identification of <c OMObject> in this
  //          <c OMWeakReferenceVector>. The size is given
  //          in bytes.
  virtual OMKeySize keySize(void) const = 0;

  virtual OMPropertyId keyPropertyId(void) const = 0;

  virtual OMPropertyTag targetTag(void) const = 0;

  virtual void setTargetTag(OMPropertyTag targetTag) = 0;

  virtual void clearTargetTag(void) const = 0;

  virtual const wchar_t* targetName(void) const = 0;

};

#endif
