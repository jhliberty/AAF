//@doc
//@class    AAFRoot | Implementation class for AAFRoot
#ifndef __CAAFRoot_h__
#define __CAAFRoot_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFRoot.h,v 1.6 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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



#ifndef __AAF_h__
#include "AAF.h"
#endif




#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

#ifndef __AAFPrivate_h__
#include "AAFPrivate.h"
#endif

//
// Forward declaration
//
class ImplAAFRoot;


class CAAFRoot
  : public IAAFRoot,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRoot (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRoot ();

public:



protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********

  //
  // Return private implementation pointer for delegation.
  // NOTE: This is NOT the pointer to the COM object's implementation
  // object!
  //
  STDMETHOD(GetImplRep)(/*[retval, out]*/ void **);

  //
  // Intialize class extensions. This method is called after the
  // contained Impl object has been completely initialized. This
  // allows the aggregated extension object access to all of the
  // interfaces and property data of its controlling unknown.
  //
  STDMETHOD(InitializeExtension)(REFCLSID clsid);

protected:

  ImplAAFRoot * GetRepObject ();

  void InitRep (ImplAAFRoot * pObj);

private:

  ImplAAFRoot * _rep;

};


#endif // ! __CAAFRoot_h__


