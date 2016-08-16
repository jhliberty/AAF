//=---------------------------------------------------------------------=
//
// $Id: OMDefinition.h,v 1.8 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMDEFINITION_H
#define OMDEFINITION_H

#include "OMDataTypes.h"
#include <stddef.h>

  // @class Abstract base class used to define persistent entities
  //        supported by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDefinition {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMDefinition(void) {}

    // @cmember The unique identification of the entity defined by
    //          this <c OMDefinition>.
  virtual const OMUniqueObjectIdentification& identification(void) const = 0;

    // @cmember The name of the entity defined by
    //          this <c OMDefinition>.
  virtual const wchar_t* name(void) const = 0;

    // @cmember Does this <c OMDefinition> have a description.
  virtual bool hasDescription(void) const = 0;

    // @cmember The description of the entity defined by
    //          this <c OMDefinition>. @precondition <f hasDefinition()>
  virtual const wchar_t* description(void) const = 0;

    // @cmember Is the entity defined by this <c OMDefinition> predefined
    //          with respect to a well known baseline?
  virtual bool isPredefined(void) const = 0;

};

class OMBuiltinDefinition {
public:
  // @access Public members.

    // @cmember Constructor.
  OMBuiltinDefinition(const OMStoredObjectIdentification& identification,
                      const wchar_t* name);

    // @cmember Constructor.
  OMBuiltinDefinition(const OMStoredObjectIdentification& identification,
                      const wchar_t* name,
                      const wchar_t* description);

    // @cmember Destructor.
  ~OMBuiltinDefinition(void);

    // @cmember The unique identification of the entity defined by
    //          this <c OMDefinition>.
  virtual const OMUniqueObjectIdentification& identification(void) const;

    // @cmember The name of the entity defined by
    //          this <c OMDefinition>.
  virtual const wchar_t* name(void) const;

    // @cmember Does this <c OMDefinition> have a description.
  virtual bool hasDescription(void) const;

    // @cmember The description of the entity defined by
    //          this <c OMDefinition>. @precondition <f hasDefinition()>
  virtual const wchar_t* description(void) const;

    // @cmember Is the entity defined by this <c OMDefinition> predefined
    //          with respect to a well known baseline?
  virtual bool isPredefined(void) const = 0;

private:
  OMUniqueObjectIdentification _identification;
  const wchar_t* _name;
  const wchar_t* _description;
  bool _hasDescription;
};

#endif
