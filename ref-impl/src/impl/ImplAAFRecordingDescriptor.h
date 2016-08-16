//@doc
//@class    AAFRecordingDescriptor | Implementation class for AAFRecordingDescriptor
#ifndef __ImplAAFRecordingDescriptor_h__
#define __ImplAAFRecordingDescriptor_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: ImplAAFRecordingDescriptor.h,v 1.3 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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




#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif









#ifndef __ImplAAFPhysicalDescriptor_h__
#include "ImplAAFPhysicalDescriptor.h"
#endif


class ImplAAFRecordingDescriptor : public ImplAAFPhysicalDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRecordingDescriptor ();

protected:
  virtual ~ImplAAFRecordingDescriptor ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();


};

#endif // ! __ImplAAFRecordingDescriptor_h__


