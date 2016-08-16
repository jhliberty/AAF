//=---------------------------------------------------------------------=
//
// $Id: CAAFPersonnelResource.h,v 1.6 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
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
#ifndef __CAAFPersonnelResource_h__
#define __CAAFPersonnelResource_h__

// Include our common defintion of a class factory. Uses our base class for
// IUnknown, CAAFUnknown that supports aggregation
//
#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

#ifndef __AAFPersonnelExtension_h__
#include "AAFPersonnelExtension.h"
#endif

//
// Declaration for the class that implements the Personnel Resource
// plugin extension.
//

class CAAFPersonnelResource : 
  public IAAFPersonnelResource,
  public IAAFPlugin,
  public IAAFClassExtension,
  public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPersonnelResource (IUnknown * pControllingUnknown);
  virtual ~CAAFPersonnelResource ();

public:

  //
  // IUnknown interface methods
	//
  AAF_DECLARE_CONCRETE(); // see CAAFUnknown.h


  //
  // IAAFPersonnelResource interface methods
	//

  STDMETHOD(Initialize)
     (aafCharacter_constptr pFamilyName,
      aafCharacter_constptr pGivenName,
      ePosition_constref position);


  STDMETHOD(SetFamilyName)
     (aafCharacter_constptr pFamilyName);

  STDMETHOD(GetFamilyNameBufLen)
     (aafUInt32 *pBufSize);

  STDMETHOD(GetFamilyName)
     (aafCharacter* pFamilyName, aafUInt32 bufSize);


  STDMETHOD(SetGivenName)
     (aafCharacter_constptr pGivenName);

  STDMETHOD(GetGivenNameBufLen)
     (aafUInt32 *pBufSize);

  STDMETHOD(GetGivenName)
     (aafCharacter* pGivenName, aafUInt32 bufSize);


  STDMETHOD(SetPosition)
     (ePosition_constref position);

  STDMETHOD(GetPosition)
     (ePosition_t *        position);


  STDMETHOD(SetContractID)
     (contractID_constref contractID);

  STDMETHOD(GetContractID)
     (contractID_t *      contractID);


  STDMETHOD(SetPart)
     (aafCharacter_constptr pPart);

  STDMETHOD(GetPartBufLen)
     (aafUInt32 *pBufSize);

  STDMETHOD(GetPart)
     (aafCharacter* pPart, aafUInt32 bufSize);


  //
  // IAAFPlugin interface methods
	//

  STDMETHOD (CountDefinitions)
	  (/*[out]*/ aafUInt32 *pDefCount);

  STDMETHOD (GetIndexedDefinitionID)
	  (/*[in] */ aafUInt32 index, 
		 /*[out]*/ aafUID_t *result);

  STDMETHOD (GetPluginDescriptorID)
	  (/*[out]*/ aafUID_t *result);

  STDMETHOD (GetIndexedDefinitionObject)
	  (/*[in] */ aafUInt32 index, 
		 /*[in] */ IAAFDictionary *dict, 
		 /*[out]*/ IAAFDefObject **def);

  STDMETHOD (CreateDescriptor)
	  (/*[in] */ IAAFDictionary *dict,
		 /*[out]*/ IAAFPluginDef **desc);

  //
  // IAAFClassExtension interface methods
	//

  STDMETHOD (RegisterDefinitions)
    (/*[in] */ IAAFDictionary *pDictionary);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


private:

  // This is an interface pointer back into the object that owns this extension.
  // since extensions are implemented as an aggregate the object the object
  // lifetimes are tied together. This means that we do not reference count
  // this interface pointer. If we did then this extension as well as the 
  // controlling/owning object will never be deleted.
  HRESULT InitializeObject();
  bool _bInitializedObject;
  IAAFObject *_pObject;
};


#endif //__CAAFPersonnelResource_h__
