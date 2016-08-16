//=---------------------------------------------------------------------=
//
// $Id: OMWideStringProperty.h,v 1.11 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMWIDESTRINGPROPERTY_H
#define OMWIDESTRINGPROPERTY_H

#include "OMCharacterStringProperty.h"

  // @class Persistent wide character strings supported by the Object Manager.
  //   @base public | <c OMCharacterStringProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMWideStringProperty : public OMCharacterStringProperty<wchar_t> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWideStringProperty(const OMPropertyId propertyId, const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMWideStringProperty(void);

    // @cmember Assignment operator.
  OMWideStringProperty& operator = (const wchar_t* value);

};

#endif
