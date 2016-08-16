//=---------------------------------------------------------------------=
//
// $Id: AAFPluginCOM.cpp,v 1.15 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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

////////////////////////////////////////////////////////////////////////////////
// @doc
//
// @module AAFCOM.CPP - AAF COM InProc Server DLL |
//
// Implementation of DLL Exports
// 
// @end
//
////////////////////////////////////////////////////////////////////////////////



// WIN32 Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f aafcomps.mk in the project directory.


#include "AAFTypes.h"
#include "CAAFInProcServer.h"

static CAAFInProcServer g_AAFInProcServer;
CAAFServer* g_pAAFServer = &g_AAFInProcServer;

// Include the table the associates all of the CLSID's with class names and factory methods.
#include "AAFPluginObjectTable_i.cpp"

// Include the definitions for all of the interface IID's.
#include "AAFPlugin_i.c"


#if defined( OS_UNIX )

class PluginCOMInitialize {
public:
	PluginCOMInitialize();
	~PluginCOMInitialize();
};

PluginCOMInitialize::PluginCOMInitialize()
{
	// Initialize the inproc server object.
 	g_AAFInProcServer.Init(AAFPluginObjectMap, 0);
}

PluginCOMInitialize::~PluginCOMInitialize()
{}

PluginCOMInitialize init;

#endif  // OS_UNIX

#if defined( OS_WINDOWS )
// Include the entry point for the windows dll.
/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{	
		// Initialize the inproc server object.
		g_AAFInProcServer.Init(AAFPluginObjectMap, hInstance);

		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		g_AAFInProcServer.Term();
	}
	return TRUE;    // ok
}

#endif  // OS_WINDOWS


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return g_AAFInProcServer.CanUnloadNow();
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return g_AAFInProcServer.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers objects, typelib and all interfaces in typelib
	return g_AAFInProcServer.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	// Unregisters all objects.
	return g_AAFInProcServer.UnregisterServer();
}



//
// Calls that need to be available for platforms that do 
// not support COM or Registry.
//


// Return the number of coclasses exported from this dll.
STDAPI_(ULONG) AAFGetClassCount(void)
{
	return g_AAFInProcServer.GetClassCount();
}

// Get the nth implementation coclass id.
STDAPI AAFGetClassObjectID(ULONG index, CLSID *pClassID)
{
	return g_AAFInProcServer.GetClassObjectID(index, pClassID);
}
