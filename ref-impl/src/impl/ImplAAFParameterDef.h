//@doc
//@class    AAFParameterDef | Implementation class for AAFParameterDef
#ifndef __ImplAAFParameterDef_h__
#define __ImplAAFParameterDef_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFParameterDef.h,v 1.20 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFTypeDeft_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefProperty.h"
#include "OMWideStringProperty.h"

class ImplAAFParameterDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFParameterDef ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const aafCharacter *name,
		 const aafCharacter *description,
     ImplAAFTypeDef * pType);


protected:
  virtual ~ImplAAFParameterDef ();

public:


  //****************
  // GetParameterDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition
        // @parm [retval][out] Pointer to an AUID reference
        (ImplAAFTypeDef **  pParameterDataDefID);

  //****************
  // SetParameterDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTypeDef
        // @parm [in] an AUID reference
        (ImplAAFTypeDef * ParameterDataDefID);

  //****************
  // GetDisplayUnits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayUnits
        (// @parm [in,string] DisplayUnits
         aafCharacter *  pDisplayUnits,

         // @parm [in] length of the buffer to hold DisplayUnits
         aafUInt32  bufSize);

  //****************
  // GetDisplayUnitsBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayUnitsBufLen
        // @parm [out] DisplayUnits
        (aafUInt32 *  pLen);



  //****************
  // SetDisplayUnits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayUnits
        // @parm [in, string] DisplayUnits
        (const aafCharacter *  pDisplayUnits);

private:
	OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef>					_typeDef;
	OMWideStringProperty									_displayUnits;
};

#endif // ! __ImplAAFParameterDef_h__

