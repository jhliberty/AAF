//@doc
//@class    AAFKLVDataDefinition | Implementation class for AAFKLVDataDefinition
#ifndef __ImplAAFKLVDataDefinition_h__
#define __ImplAAFKLVDataDefinition_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: ImplAAFKLVDataDefinition.h,v 1.4 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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





#include "ImplAAFTypeDef.h"

#include "ImplAAFPropertyDef.h"

template <class T> class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFPropertyDef> ImplEnumAAFPropertyDefs;


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "OMWeakRefSetProperty.h"
#include "OMWeakRefProperty.h"

class ImplAAFKLVDataDefinition : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFKLVDataDefinition ();

protected:
  virtual ~ImplAAFKLVDataDefinition ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const aafCharacter *name,
		 const aafCharacter *description);

  //****************
  // AddParentProperty()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddParentProperty
        // @parm [in] Property definition to add.
        (ImplAAFPropertyDef * pParentProperty);

  //****************
  // GetParentProperties()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParentProperties
        // @parm [out] Property definition enumerator.
        (ImplEnumAAFPropertyDefs ** ppEnum);

  //****************
  // CountParentProperties()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountParentProperties
        // @parm [out] Number of property definitions.
        (aafUInt32*  pNumProperties);

  //****************
  // RemoveParentProperty()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveParentProperty
        // @parm [in] Property to remove.
        (ImplAAFPropertyDef * pParentProperty);

  //****************
  // GetKLVDataType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetKLVDataType
        // @parm [out] Returned data definition object
        (ImplAAFTypeDef ** ppTypeDef);


  //****************
  // SetKLVDataType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetKLVDataType
        // @parm [in] Data definition object
        (ImplAAFTypeDef * pTypeDef);

 private:
  
  OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _klvDataTypeDef;
    
};

#endif // ! __ImplAAFKLVDataDefinition_h__


