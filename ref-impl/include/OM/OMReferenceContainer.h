//=---------------------------------------------------------------------=
//
// $Id: OMReferenceContainer.h,v 1.8 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMREFERENCECONTAINER_H
#define OMREFERENCECONTAINER_H

class OMObject;
class OMReferenceContainerIterator;

#include "OMDataTypes.h"

  // @class Abstract base class for object reference containers
  //        supported by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMReferenceContainer {
public:
  // @access Public members.

    // @cmember Insert <p object> into this <c OMReferenceContainer>.
  virtual void insertObject(const OMObject* object) = 0;

    // @cmember Does this <c OMReferenceContainer> contain <p object> ?
  virtual bool containsObject(const OMObject* object) const = 0;

    // @cmember The number of <p OMObject>s in this <c OMReferenceContainer>.
  virtual OMUInt32 count(void) const = 0;

    // @cmember Remove <p object> from this <c OMReferenceContainer>.
  virtual void removeObject(const OMObject* object) = 0;

    // @cmember Remove all objects from this <c OMReferenceContainer>.
  virtual void removeAllObjects(void) = 0;

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMReferenceContainer>.
  virtual OMReferenceContainerIterator* createIterator(void) const = 0;

};

#endif
