//@doc
//@class    AAFEssenceCodec | Implementation class for basic interpolators plugin
#ifndef __CAAFBasicInterp_h__
#define __CAAFBasicInterp_h__

//=---------------------------------------------------------------------=
//
// $Id: CAAFBasicInterp.h,v 1.11 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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


#include "AAF.h"
#include "AAFPlugin.h"
#include "CAAFUnknown.h"
#include "AAFRational.h"

EXTERN_C const CLSID CLSID_AAFBasicInterp;

class CAAFBasicInterp
  : public IAAFInterpolator,
	public IAAFPlugin,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFBasicInterp (IUnknown * pControllingUnknown);
  virtual ~CAAFBasicInterp ();

public:

  STDMETHOD (CountDefinitions)(aafUInt32 *pDefCount);
  STDMETHOD (GetIndexedDefinitionID)(aafUInt32 index, aafUID_t *result);
  STDMETHOD (GetPluginDescriptorID)(aafUID_t *result);
  STDMETHOD (GetIndexedDefinitionObject)(aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def);
  STDMETHOD (CreateDescriptor)(IAAFDictionary *dict, IAAFPluginDef **desc);

  STDMETHOD (GetNumTypesSupported)(
    /* [out] */aafUInt32*  pCount);
  STDMETHOD (GetIndexedSupportedType)(
    /* [in] */ aafUInt32  index,
    /* [out] */IAAFTypeDef ** ppType);
  STDMETHOD (GetTypeDefinition)(
    /* [out] */IAAFTypeDef ** ppTypeDef);
  STDMETHOD (SetTypeDefinition)(
    /* [in] */ IAAFTypeDef * pTypeDef);
  STDMETHOD (GetParameter)(
    /* [out] */IAAFParameter ** ppParameter);
  STDMETHOD (SetParameter)(
    /* [in] */ IAAFParameter * pParameter);
  STDMETHOD (InterpolateOne)(
    /* [in] */ aafRational_t *  pInputValue,
	/* [in] */aafUInt32	bufSize,
    /* [out] */aafMemPtr_t pOutputValue,
	/* [in] */aafUInt32	*bytesRead);
  STDMETHOD (InterpolateMany)(
	/* [in] */ aafRational_t *  pStartInputValue,
    /* [in] */ aafRational_t *  pInputStep,
    /* [in] */ aafUInt32  pGenerateCount,
    /* [out] */aafMemPtr_t pOutputValue,
    /* [out] */aafUInt32 *  ppResultCount);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);

  HRESULT FindBoundValues(aafRational_t point, aafUInt32 valueSize,
						AAFRational *lowerBoundTime, aafMemPtr_t lowerBoundValue,
						AAFRational *upperBoundTime, aafMemPtr_t upperBoundValue);
public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********

  // Declare the module test method. The implementation of the will be be
  // in /test/CAAFEssenceCodecTest.cpp.
  static HRESULT test();

private:
	IAAFTypeDef		*_typeDef;
	IAAFParameter	*_parameter;
};

#endif // ! __CAAFBasicInterp_h__

