#ifndef __CAxPlugin_h__
#define __CAxPlugin_h__

//=---------------------------------------------------------------------=
//
// $Id: CAxPlugin.h,v 1.7 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include <AAFResult.h>
#include <AAFPlugin.h>

#include <memory>

template <class T>
class CAxPlugin : public IAAFPlugin
{
public:

	CAxPlugin( std::auto_ptr<T> impl )
		: _impl( impl )
	{}

	virtual ~CAxPlugin()
	{}

	T& GetPluginImpl()
	{
		return *_impl;
	}

	//
	// IAAFPlugin Interface methods
	//

	HRESULT STDMETHODCALLTYPE CountDefinitions (
		aafUInt32 *pDefCount )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( pDefCount );
		AX_PLUGIN_TRY( _impl, CountDefinitions, (pDefCount) );
	}


	HRESULT STDMETHODCALLTYPE GetIndexedDefinitionID (
		aafUInt32 index, aafUID_t *result )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( result );
		AX_PLUGIN_TRY( _impl, GetIndexedDefinitionID, (index, result) );
	}


	HRESULT STDMETHODCALLTYPE GetPluginDescriptorID (
		aafUID_t *result )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( result );
		AX_PLUGIN_TRY( _impl, GetPluginDescriptorID, (result) );
	}

    HRESULT STDMETHODCALLTYPE GetIndexedDefinitionObject ( 
		aafUInt32 index,
        IAAFDictionary *dict,
        IAAFDefObject **def )
	{
		AX_PLUGIN_CHECK_NULL_PARAM( dict );
		AX_PLUGIN_CHECK_NULL_PARAM( def );
		AX_PLUGIN_TRY( _impl, GetIndexedDefinitionObject, (index, dict, def) );
	}
 
	HRESULT STDMETHODCALLTYPE CreateDescriptor (
		IAAFDictionary *dict,
		IAAFPluginDef **desc )
	{	
		AX_PLUGIN_CHECK_NULL_PARAM( dict );
		AX_PLUGIN_CHECK_NULL_PARAM( desc );
		AX_PLUGIN_TRY( _impl, CreateDescriptor, (dict, desc) );
	}

private:

	// Prohibited
	CAxPlugin();
	CAxPlugin( const CAxPlugin& );
	CAxPlugin& operator&( const CAxPlugin& );

	std::auto_ptr<T> _impl;
};

#endif


