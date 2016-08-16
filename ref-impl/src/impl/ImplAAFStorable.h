#ifndef __ImplAAFStorable_h__
#define __ImplAAFStorable_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFStorable.h,v 1.8 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFRoot.h"
#include "OMStorable.h"

class ImplAAFClassDef;

class ImplAAFStorable : 
  public OMStorable,
  public ImplAAFRoot
{
protected:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFStorable ();

  virtual ~ImplAAFStorable ();

public:
  // Private SDK methods.
  
  // Associate OMClassDefinition and OMPropertyDefinitions with this object.
  virtual void InitializeOMStorable(ImplAAFClassDef * pClassDef);
  
  virtual AAFRESULT GetDefinition(ImplAAFClassDef ** ppClassDef);

  // Return true if this is a meta object
  // NOTE: These objects will eventually owned by the Object Manager.
  virtual bool metaObject(void) const = 0;
  
  // Return true is this is a data object (Interchange object).
  virtual bool dataObject(void) const = 0;

protected:
  // Associate the existing OMProperties with corresponding property definitions from
  // the given class definition. NOTE: This call is recursive, it calls itself again
  // for the parent class of the given class until current class is a "root" class.
  virtual void InitOMProperties (ImplAAFClassDef * pClassDef) = 0;

  // Call during long operations in order to give time to the calling application
  void Progress(void) const;

};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFStorable> ImplAAFStorableSP;


//
// Inlude the generic enumerator for storables.
//
#ifndef __ImplAAFEnumerator_h__
#include "ImplAAFEnumerator.h"
#endif
typedef ImplAAFEnumerator<ImplAAFStorable> ImplEnumAAFStorables;

#endif // ! __ImplAAFStorable_h__
