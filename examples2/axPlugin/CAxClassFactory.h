#ifndef __CAxClassFactory_h__
#define __CAxClassFactory_h__

//=---------------------------------------------------------------------=
//
// $Id: CAxClassFactory.h,v 1.12 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include "AxPluginUtil.h"

#include <AAFCOMPlatform.h>

#include <assert.h>

// Simple base class for use with CAxClassFactory. Maintains some
// static counters used to implement instance and lock counts.
// This allows the counters to be cleanly accessed for all types
// on which the CAxClassFactory is instantiead.

class CAxClassFactoryCounters
{
public:

	CAxClassFactoryCounters();
	~CAxClassFactoryCounters();

	static unsigned int GetInstanceCount();
	static unsigned int GetLockCount();

protected:

	void IncLockCount();
	void DecLockCount();

private:

	static unsigned int _instanceCount;
	static unsigned int _lockCount;
};


// Credits:
//
// The CAxClassFactory::CreateInstance() implementation is a slightly
// modified version of the IClassFactory::CreateInstance() implementation
// presented in: "Inside COM", Dale Rogerson, Microsoft Press.
// This implementation uses a template to identify the type of COM object
// that must be created. The implementation presented the "Inside COM" uses
// a different technique.


// CAxClassFactory implements IClassFactory for the template class
// ComObjectType.  ComObjectType should be derived from CAxUnknown.
//
// The CAxClassFactoryCounters is used to implement a CAxClassFactory<>
// instance count, and to implement the LockServer() method.

// gcc3 complains that CAxClassFactory has a private destructor
// and no friends.  This prevents the warning.
class CAxClassFactoryFriend_nonexistant;

template <class ComObjectType>
class CAxClassFactory : public CAxClassFactoryCounters, public IClassFactory {

public:

// Schlock required to declare LockServer
typedef BOOL    LockServerBool_t;

public:

	STDMETHOD(QueryInterface)( REFIID riid, void** ppv )
	{
		if ( IID_IUnknown == riid ) {
			*ppv = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
			
			
		if ( IID_IClassFactory == riid ) {
			*ppv = static_cast<IClassFactory*>(this);
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	STDMETHOD_(ULONG, AddRef)()
	{
		_refCount++;
		return _refCount;
	}

	STDMETHOD_(ULONG, Release)()
	{
		assert( _refCount > 0 );

		_refCount--;

		if ( 0 == _refCount ) {
			delete this;
			return 0;
		}

		return _refCount;
	}
	
	STDMETHOD(CreateInstance)(
		IUnknown * pUnkOuter,  //Pointer to outer object that will aggregate this object.
		REFIID riid,           //Reference to the identifier of the interface.
		void ** ppv            //Address of output variable.
		)
	{
		// Fail if aggregation support requested, but riid not IID_IUnknown.
		if ( pUnkOuter  &&  IID_IUnknown  != riid ) {
		  // Note the appropriate return value is (according
		  // the "Inside COM", Dale Rogerson) is
		  // CLASS_E_NOAGGREGATION.  This is not currently
		  // defined in AAFCOMPlatformTypes.h, so we stick with
		  // E_INVALIDARG;
		  return E_INVALIDARG;
		}

		// Create the new component instance.  Pessimistic try
		// catch block in the event new throws and exception.
		// MSVC doesn't by default (which is wrong), but
		// others may.  It wouldn't cause problems in the
		// relm of AAF because the AAF com interfaces would
		// catch the exception... but stil.

		ComObjectType* pObj;
		try {
		  pObj = new ComObjectType( pUnkOuter );
		}
		catch(...) {
		  pObj = 0;
		}
		if ( !pObj ) {
		  return E_OUTOFMEMORY;
		}

		// Initialize the new component;
		HRESULT hr = pObj->Init();
		if ( S_OK != hr ) {
			pObj->NondelegatingRelease();
			return hr;
		}

		// Get the requested interface.
		hr = pObj->NondelegatingQueryInterface( riid, ppv );

		pObj->NondelegatingRelease();

		return hr;
	}

	STDMETHOD(LockServer)(
		LockServerBool_t fLock			   //Increments or decrements the lock count
	)
	{
		if (fLock) {
			IncLockCount();
		}
		else {
			DecLockCount();
		}

		return S_OK;
	}

	CAxClassFactory() 
		: CAxClassFactoryCounters(),
		  _refCount(1)
	{}

private:

	friend class CAxClassFactoryFriend_nonexistant;

	// Object deletes itself when reference count drops to zero.
	// Hence, destructor is declared private.
	virtual ~CAxClassFactory()
	{}


	unsigned int _refCount;
};


#endif
