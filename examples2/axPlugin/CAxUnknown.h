#ifndef __CAxUnknown_h__
#define __CAxUnknown_h__

//=---------------------------------------------------------------------=
//
// $Id: CAxUnknown.h,v 1.9 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include <AAFCOMPlatform.h>

#include "AxPluginUtil.h"

// CAxUnknown is a standard implementation of IUnknown.
//
// Credits:
//
// The CAxUnknown implementation is a (very) slightly modified version
// of the IUnknown implementation presented in:
// "Inside COM", Dale Rogerson, Microsoft Press
//
// Modifications include:
// - The CAxUnknown class methods have been declared using the AAF SDK
// STDMETHOD macros.
// - The use of the windows specific InterlockedIncrement,
// and InterlockedDecrement, have been removed.  This version
// of the code is, therefor, not thread safe.  The AAF SDK, is not
// thread safe to begin with, hence, this is a non-issue.
//
// "Inside COM" should be referenced for a full explanation of the
// "INondelegatingUnknown" interface.  Suffice to say, the
//  INondelegatingUnknown interface exists to satisfy the requirements
//  of COM object aggregation.  Classes that derive from CAxUnknown
//  must provide an implementation of IUnknown that delagates calls
//  to the aggregating (i.e. "outer") object if it exists.  At the
//  same time, CAxUnknown must provide the outer object with an independent
//  implementation of IUnknown that does not delegate to the outer object.
//  This is the purpose of INondelegatingUnknown.  Insofar as the vtable
//  (and the COM binary interface specification) is concerned,
//  INondelgatingUnkonw is identical to IUnknown.  Hence,
//  a pointer to INondelegatingUnknown can be used in place of a pointer
//  to IUnknown.  CAxUnknown::QueryInterface() uses this fact to provide
//  its own independent implementation of IUnknown (the non delegating implementation).
//  At the same time, classes that derive from CAxUnknown can implement IUnknown
//  by delegating the IUknown interface calls to the outer object.
//
//  HOW TO USE CAxUnknown:
// 
//  Classes that derive from CAxUnknown *must* use the
//  CAXUNKNOWN_DECLARE_IUNKNOWN_METHODS to declare (and define)
//  their IUnknown implementation.  For example:
//
//     class CMyComponent : public IAnInterface : CAxUnknown {
//        public:
//          // IUnknown methods provided by the following macro
//          CAXUNKNOWN_DECLARE_IUNKNOWN_METHODS
//		
//          // IAnInterface method declarations follow...
//		    .
//		    .
//		    .
//       };
//
//       ... in case you are wondering, IAnInterface is assumed to derive
//       from IUnknown.
//

///////////////////////////////////////////////////////////
//
// Nondelegating IUnknown interface
//   - Nondelegating version of IUnknown
//


interface INondelegatingUnknown
{
	STDMETHOD(NondelegatingQueryInterface)(const IID& iid, void** ppv) = 0 ;
	STDMETHOD_(ULONG, NondelegatingAddRef)() = 0 ;
	STDMETHOD_(ULONG, NondelegatingRelease)() = 0 ;
} ;


///////////////////////////////////////////////////////////
//
// Declaration of CAxUnknown 
//   - Base class for implementing IUnknown
//

class CAxUnknown : public INondelegatingUnknown
{
public:

	// Nondelegating IUnknown implementation
	STDMETHOD(NondelegatingQueryInterface)(const IID&, void**);
	STDMETHOD_(ULONG,NondelegatingAddRef)();
	STDMETHOD_(ULONG,NondelegatingRelease)();

	// Constructor
	CAxUnknown(IUnknown* pUnknownOuter);

	// Destructor
	virtual ~CAxUnknown();

	// Initialization (especially for aggregates)
	STDMETHOD(Init)()
		{ return S_OK; }

	// Notification to derived classes that we are releasing
	STDMETHOD_(void, FinalRelease)();

	// Count of currently active components
	static long GetActiveComponents() 
		{ return s_cActiveComponents; }
	
	// Helper function
	HRESULT FinishQI(IUnknown* pI, void** ppv);

protected:

	// Support for delegation
	IUnknown* GetOuterUnknown() const
		{ return m_pUnknownOuter; }

private:
	// Reference count for this object
	long m_cRef ;
	
	// Pointer to (external) outer IUnknown
	IUnknown* m_pUnknownOuter ;

	// Count of all active instances
	static long s_cActiveComponents ; 
};


///////////////////////////////////////////////////////////
//
// Delegating IUnknown
//   - Delegates to the nondelegating IUnknown, or to the
//     outer IUnknown if the component is aggregated.
//
#define CAXUNKNOWN_DECLARE_IUNKNOWN_METHODS		             \
	STDMETHOD(QueryInterface)(const IID& iid, void** ppv)    \
	{	                                                     \
		return GetOuterUnknown()->QueryInterface(iid,ppv) ;  \
	} ;	                                                     \
	STDMETHOD_(ULONG, AddRef)()	                             \
	{	                                                     \
		return GetOuterUnknown()->AddRef() ;                 \
	} ;	                                                     \
	STDMETHOD_(ULONG, Release)() 	                         \
	{	                                                     \
		return GetOuterUnknown()->Release() ;                \
	} ;


#endif
