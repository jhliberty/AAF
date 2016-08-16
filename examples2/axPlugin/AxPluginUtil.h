#ifndef __AxPluginUtil_h__
#define __AxPluginUtil_h__

//=---------------------------------------------------------------------=
//
// $Id: AxPluginUtil.h,v 1.10 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include <AxEx.h>

#include <AxTypes.h>

#include <map>

#ifdef _DEBUG
#include <iostream>
#endif

//
// Miscellaneous items.
//

// FIXME - This is similar to AxPluginSpecifierMap.  If a generic means
// to implement GetMaxValBufSize() is determined, then a generic implementation
// is possible.

class AxPluginNameMap
	: public std::map<aafUID_t, AxString>
{
public:

	typedef std::map<aafUID_t, AxString>::iterator IterType;
	typedef std::pair<aafUID_t, AxString>		   PairType;
	typedef AxCharTraits                           CharTraits;

	// Use this to double check if a uid is in the map before
	// using operator[]().
	bool IsFound( const aafUID_t& uid );

	// Returns largest string size in characters.
	int GetMaxStringSize();

	// Return size of buffer (in bytes) required to store largest string.
	// Including null terminator required by C string.
	int GetMaxCBufferSize();

};

// Call object OBJ method NAME using args ARGS.
// Catch hresult exception and return error value, or
// any other exception and return error code, or return
// S_OK on success.
// Use to implement COM interfaces that wrap an underlying
// implementation that, generally, has identical method
// name and arguments but does not return an error code.

#ifdef _DEBUG
#define AX_PLUGIN_LOG_WHAT( EX )  std::wcout << L"Codec error: " << (EX).what() << std::endl; 
#else
#define AX_PLUGIN_LOG_WHAT( EX )
#endif

#define AX_PLUGIN_TRY( OBJ, NAME, ARGS )		\
	try {										\
		(OBJ)->NAME ARGS ;						\
	}											\
	catch( const AxExHResult& ex ) {			\
		AX_PLUGIN_LOG_WHAT( ex )                \
		return ex.getHResult();					\
	}											\
	catch( ... ) {								\
		return AAFRESULT_UNEXPECTED_EXCEPTION;	\
	}											\
	return S_OK;


#define AX_PLUGIN_CHECK_NULL_PARAM( PNTR ) \
{ if ( !(PNTR) ) { CHECK_HRESULT( AAFRESULT_NULL_PARAM ) } }


#endif // __AxPluginUtil_h__
