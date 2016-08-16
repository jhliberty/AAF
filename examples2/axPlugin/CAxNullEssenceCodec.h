#ifndef __CAxUnknownCodec_h__
#define __CAxUnknownCodec_h__

//=---------------------------------------------------------------------=
//
// $Id: CAxNullEssenceCodec.h,v 1.7 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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
//=---------------------------------------------------------------------=

#include "CAxUnknown.h"
#include "CAxPlugin.h"
#include "CAxEssenceCodec.h"

#include "AxImplNullEssenceCodec.h"
#include "AxImplPlugin.h"
#include "AxPlugin.h"

// CAxNullEssenceCodec - sample implementation of an AAF Plugin.
//
// The IUknown implementation is provided by CAxUnknown.
//
// The IAAFPlugin implementation is provided by CAxPlugin and AxImplPlugin.
// CAxPlugin implements the IAAFPlugin COM interface by wrapping an instance of
// AxImplPlugin.  CAxPlugin catches exceptions and returns HRESULT's to the caller.
// AxImplPlugin has a set of methods that match CAxPlugin.  However, these methods al
// use a void return type.  AxImplPlugin reports routine errors by throwing AxExHResult.
// The CAxPlugin and AxImplPlugins are generic and resusable.  The CAxNullEssenceCodec
// illustrates how to initialize AxImplePlugin.
//
// CAxEssenceCodec implements the IAAFEssenceCodec COM interface by wrapping an instance
// AxImplEssenceCodec.  CAxEssenceCodec catches exceptions and returns HRESULT's to the
// caller.  CAxEssenceCodec is generic and reusable.
// AxImplNullEssenceCodec has a set of methods that match CAxEssenceCodec except they all
// have a void return type.  AxImplNullEssenceCodec is an example codec implementation
// that is not reusable without modification.

class CAxNullEssenceCodec
: public CAxEssenceCodec<AxImplNullEssenceCodec>,
  public CAxPlugin<AxImplPlugin>,
  public CAxUnknown
{
public:

	CAxNullEssenceCodec( IUnknown * pUnkOuter );
	virtual ~CAxNullEssenceCodec();

	//CAXUNKNOWN_DECLARE_IUNKNOWN_METHODS
	STDMETHOD(QueryInterface)(const IID& iid, void** ppv)    
	{	                                                     
		return GetOuterUnknown()->QueryInterface(iid,ppv) ;  
	}
	STDMETHOD_(ULONG, AddRef)()	                             
	{	                                                     
		return GetOuterUnknown()->AddRef() ;                 
	}	                                                     
	STDMETHOD_(ULONG, Release)() 	                         
	{	                                                     
		return GetOuterUnknown()->Release() ;               
	}

	
	// Override CAxUnknown::NondelegatingQueryInterface() in order to add
	// support for the interfaces supported by this class.
	STDMETHOD( NondelegatingQueryInterface(const IID& iid, void** ppv) );
};

#endif
