#ifndef __AxPluginRegistry_h__
#define __AxPluginRegistry_h__

//=---------------------------------------------------------------------=
//
// $Id: AxPluginRegistry.h,v 1.6 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

class AxPluginFctryPrtcl;

#include <memory>
#include <vector>


// AxPluginRegistry is a singleton that is responsible for
// managing the list of components that are implemented by a single
// library.
//
// All components implemented by a single library are responsible for
// registering an implementation of AxPluginFctryPrtcl with the
// registry at init time (i.e. when the library is loaded).
//
// To avoid memory leaks, component implementors should be sure to
// call RemoveFactory() at fini time (i.e., when the library is unloaded).
//
// When the last factory object is removed, the single instance of 
// AxPluginRegistry will destoy itself (no leaked memory!).  Note,
// subsequent calls to GetInstance() are safe - a new singleton instance
// will be created.
//
// This registration can happen where ever the library and component
// implementor sees fit.  The most convenient is probably in the constructor
// of a static global object that accompanies the component implementation.
// The destructor of the static global is also a logical place to call
// RemoveFactory().
// 
// Please refer the UML.

class AxPluginRegistry {
public:
	
	// It's a singleton, hence...
	static AxPluginRegistry& GetInstance();
		
	// Add and remove AxPluginFctryPrtcl implementations.   Obviously,
	// the CLSID passed to RemoveFactory must match the CLSID of the
	// factory object you wish to remove.
	// CLSID uniqueness is enforced by an assertion.
	void AddFactory( std::auto_ptr<AxPluginFctryPrtcl> factory );
	void RemoveFactory( const CLSID& clsid ); 

	// Return the number of registered factory objects.
	int GetClassCount();

	// Return the id of the factory with specified index.
	HRESULT GetClassObjectID( int index, CLSID *pClassID );

	// Look up the factory with rid "cslid".  Create the underlying
	// COM factory and return a pointer to the interface with id "riid".
	HRESULT GetClassObject( const CLSID& clsid, REFIID riid, LPVOID* ppv );

private:

	// It's a singleton... nobody should access these.
	AxPluginRegistry();
	~AxPluginRegistry();

	// Prohibited
	AxPluginRegistry( const AxPluginRegistry& other );
	AxPluginRegistry& operator=( const AxPluginRegistry& rhs );

	// Returns null if not found
	std::vector<AxPluginFctryPrtcl*>::iterator Find( const CLSID& clsid );

	// The registry will de-initialize itself (i.e. "Fini") automatically when
	// the the last factory object is removed.
	void Fini();
		
	std::vector<AxPluginFctryPrtcl*> _factoryV;
	
	static AxPluginRegistry* _instance;
};


#endif
