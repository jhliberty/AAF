//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: ImplAAFRoot.cpp,v 1.9 2012/06/26 20:54:51 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=







#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

// Include the declaration for the reference counting container callback function.
#include "ImplAAFObjectCreation.h"

#include <assert.h>


ImplAAFRoot::ImplAAFRoot ()
: _pContainer (0),
  _isInitialized (false)
{}


ImplAAFRoot::~ImplAAFRoot ()
{}


// Increment the object reference count.
aafUInt32 ImplAAFRoot::AcquireReference() const
{ // inforce conceptual 'constness'
  // Delegate to global function to increment the reference
  // count for this implementation object's COM or C++ API object.
  return AcquireImplReference(const_cast<ImplAAFRoot *>(this));
}

// Decrement the object reference count and delete the container.
aafUInt32 ImplAAFRoot::ReleaseReference()
{
  // If there is only a single reference left it is the container's
  // reference to this implementation object. Tell the container to
  // Delete itself. The container's destructor will delete this
  // object.
  return ReleaseImplReference(this);
}

// Just return the count. (this could be inline in the header...)
aafUInt32 ImplAAFRoot::ReferenceCount() const
{ // inforce conceptual 'constness'
  // Delegate to global function to return the reference
  // count for this implementation object's COM or C++ API object.
  return ReferenceImplCount(const_cast<ImplAAFRoot *>(this));
}

void ImplAAFRoot::InitContainer (void * pContainer)
{
  assert (! _pContainer);
  _pContainer = pContainer;
  assert (_pContainer);
}



void * ImplAAFRoot::GetContainer ()
{
  assert (_pContainer);
  return _pContainer;
}


bool ImplAAFRoot::isInitialized () const
{
  return _isInitialized;
}


void ImplAAFRoot::setInitialized ()
{
  _isInitialized = true;
  assert (isInitialized());
}


