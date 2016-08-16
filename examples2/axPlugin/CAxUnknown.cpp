//=---------------------------------------------------------------------=
//
// $Id: CAxUnknown.cpp,v 1.6 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

// Credits:
//
// The CAxUnknown implementation is a slightly modified version
// of the IUnknown implementation presented in:
// "Inside COM", Dale Rogerson, Microsoft Press
//
// The CAxUnknown class methods have been defined using the AAF SDK
// STDMETHODCALLTYPE macro, and the windows specific InterlockedDecrement and
// InterlockedIncrement calls are not used.  Other than that, it is
// identical to the code presented in the "Inside COM" book.
//
// Concerning the non-use of Interlocked{Decrement,Increment} calls by this code:
// these Windows functions (macros?..) provide thread safe access to the static instance
// count, and per object reference count.  The AAF SDK, is not thread safe hence an AAF 
// plugin COM object should never be accessed by multiple threads. Hence, there is no need
// to introduce platform dependent atomic counter access code.

///////////////////////////////////////////////////////////
//
// CAxUnknown.cpp 
//   - Implementation of IUnknown base class
//
#include "CAxUnknown.h"

///////////////////////////////////////////////////////////
//
// Count of active objects
//   - Used to determine if we can unload the DLL.
//
long CAxUnknown::s_cActiveComponents = 0 ;

///////////////////////////////////////////////////////////
//
// Constructor
//
CAxUnknown::CAxUnknown(IUnknown* pUnknownOuter)
: m_cRef(1)
{
	// Set m_pUnknownOuter pointer.
	if (pUnknownOuter == NULL)
	{
		m_pUnknownOuter = reinterpret_cast<IUnknown*>
		                     (static_cast<INondelegatingUnknown*>
		                     (this)) ;  // notice cast
	}
	else
	{
		m_pUnknownOuter = pUnknownOuter ;
	}

	// Increment count of active components.
	s_cActiveComponents++;
}

//
// Destructor
//
CAxUnknown::~CAxUnknown()
{
	s_cActiveComponents--;
}

//
// FinalRelease - called by Release before it deletes the component
//
void CAxUnknown::FinalRelease()
{
	m_cRef = 1 ;
}

//
// Nondelegating IUnknown
//   - Override to handle custom interfaces.
//
HRESULT STDMETHODCALLTYPE
	CAxUnknown::NondelegatingQueryInterface(const IID& iid, void** ppv)
{
	// CAxUnknown supports only IUnknown.
	if (iid == IID_IUnknown)
	{
		return FinishQI(reinterpret_cast<IUnknown*>
		                   (static_cast<INondelegatingUnknown*>(this)), ppv) ;
	}	
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
}

//
// AddRef
//
ULONG STDMETHODCALLTYPE CAxUnknown::NondelegatingAddRef()
{
	m_cRef++;
	return m_cRef;
}

//
// Release
//
ULONG STDMETHODCALLTYPE CAxUnknown::NondelegatingRelease()
{
	m_cRef--;
	if (m_cRef == 0)
	{
		FinalRelease() ;
		delete this ;
		return 0 ;
	}
	return m_cRef ;
}

//
// FinishQI
//   - Helper function to simplify overriding
//     NondelegatingQueryInterface
//
HRESULT CAxUnknown::FinishQI(IUnknown* pI, void** ppv) 
{
	*ppv = pI ;
	pI->AddRef() ;
	return S_OK ;
}

