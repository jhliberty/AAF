//=---------------------------------------------------------------------=
//
// $Id: AxPluginFormatSpecifiers.cpp,v 1.6 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include "AxPluginFormatSpecifiers.h"

#include <AxTypes.h>

#include <string.h>

#include <memory>
#include <algorithm>

namespace {

class MaxIntFunc {
public:
	MaxIntFunc()
		: _val( 1 << (8*sizeof(int) - 1) )
	{}

	~MaxIntFunc()
	{}

	void operator()( AxPluginSpecifierMap::PairType pair )
	{
		int size = pair.second->GetValBufSize();
		if ( size > _val ) {
			_val = size;
		}
	}

	int GetVal()
	{
		return _val;
	}

private:
	
	int _val;
};

const aafUID_t NULL_UID =  { 0x00000000, 0x0000, 0x0000,
			     { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
};

AxPluginUntypedFormatSpecifier::AxPluginUntypedFormatSpecifier( int size )
: _readOnly( false ),
  _uid( NULL_UID ),
  _valBuf( 0 ),
  _valBufSize( size )
{}

AxPluginUntypedFormatSpecifier::AxPluginUntypedFormatSpecifier( aafUID_t uid, int size, bool readOnly )
: _readOnly( readOnly ),
  _uid( uid ),
  _valBuf( 0 ),
  _valBufSize( size )
{}

AxPluginUntypedFormatSpecifier::~AxPluginUntypedFormatSpecifier()
{
	if ( _valBuf ) {
		delete _valBuf;
	}
}

void AxPluginUntypedFormatSpecifier::SetUID( const aafUID_t& uid )
{
	_uid = uid;
}
		
aafUID_t AxPluginUntypedFormatSpecifier::GetUID() const
{
	return _uid;
}

bool AxPluginUntypedFormatSpecifier::IsEqual( const aafUID_t& other )
{
	return other == _uid;
}

void AxPluginUntypedFormatSpecifier::SetValZero( int size )
{
	std::auto_ptr<aafUInt8> buf( new aafUInt8[ size ] );
	memset( buf.get(), 0, size );
	SetValBuf( buf.get(), size );
}

void AxPluginUntypedFormatSpecifier::SetValBuf( const aafUInt8* valBuf, int size )
{
	if ( !valBuf ) {
		// FIXME - require exception
		// throw 
	}
	
	if ( size <= 0 ) {
		// FIXME - require exception
		// throw 
	}

	if ( _valBuf ) {
		delete _valBuf;
	}

	_valBuf = new aafUInt8[ size ];

	memcpy( _valBuf, valBuf, size );

	_valBufSize = size;
}

aafUInt8* AxPluginUntypedFormatSpecifier::GetValBuf()
{
	if ( !_valBuf ) {
		// FIXME - require exception
		// throw 
	}
	
	return _valBuf;
}

int AxPluginUntypedFormatSpecifier::GetValBufSize()
{
	return _valBufSize;
}

bool AxPluginUntypedFormatSpecifier::IsValSet()
{
	return _valBuf ? true : false;
}

void AxPluginUntypedFormatSpecifier::SetReadOnly( bool readOnly )
{
	_readOnly = readOnly;
}

bool AxPluginUntypedFormatSpecifier::IsReadOnly()
{
	return _readOnly;
}

//=---------------------------------------------------------------------=

bool AxPluginSpecifierMap::IsFound( const aafUID_t& uid )
{
	if ( find(uid) == end() ) {
		return false;
	}
	else {
		return true;
	}
}


int AxPluginSpecifierMap::GetMaxValBufSize()
{
	MaxIntFunc max;

	return std::for_each( begin(), end(), max ).GetVal();
}

