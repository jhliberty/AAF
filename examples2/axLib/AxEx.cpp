//=---------------------------------------------------------------------=
//
// $Id: AxEx.cpp,v 1.10 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "AxEx.h"
#include "AxUtil.h"
#include "AxHrMap.h"

#include <wchar.h>

#include <sstream>

namespace {

AxString hresult( L"HRESULT" );
AxString spc( L" " );
AxString zerox( L"0x" );
AxString colon( L":" );

};

//=---------------------------------------------------------------------=

AxEx::AxEx()
  : _widewhat(L""),
    _what("")
{}

AxEx::AxEx( const wchar_t* what )
  : _widewhat( what ),
    _what( AxStringUtil::wctomb( what ) )
{}

AxEx::AxEx( const AxString& what )
  : _widewhat( what ),
    _what( AxStringUtil::wctomb( what ) )
{}

AxEx::~AxEx() throw()
{}
	
const wchar_t* AxEx::widewhat() const throw()
{
	return _widewhat.c_str();
}

const char* AxEx::what() const throw()
{
  return _what.c_str();
}

//=---------------------------------------------------------------------=

AxExHResult::AxExHResult()
: AxEx(),
  _widewhat(L""),
  _what(""),
  _hresult( 0 )
{}

AxExHResult::AxExHResult( HRESULT hr )
: AxEx(),
  _widewhat( L"" ),
  _what( "" ),
  _hresult( hr )
{}

AxExHResult::AxExHResult( HRESULT hr, const wchar_t* what )
: AxEx(),
  _widewhat( what ),
  _what( "" ),
  _hresult( hr )
{
	std::wostringstream os;

	os << hresult << spc;
	os << zerox << AxStringUtil::int2StrHex( hr );
	os << spc << AxHrMap::getInstance().getStr( hr ) << spc;
	os << AxString( what );

	_widewhat = os.str();
	_what = AxStringUtil::wctomb( _widewhat );
}

AxExHResult::AxExHResult( HRESULT hr, const char* file, int line )
: AxEx(),
  _file( file ),
  _line( line ),	
  _widewhat( L"" ),
  _what( "" ),
  _hresult( hr )
{
	std::wostringstream os;

	os << hresult << spc;
	os << AxStringUtil::mbtowc( file );
	os << colon << line << spc;
	os << zerox << AxStringUtil::int2StrHex( hr );
	os << spc << AxHrMap::getInstance().getStr( hr );

	_widewhat = os.str();
	_what = AxStringUtil::wctomb( _widewhat );
}

AxExHResult::~AxExHResult() throw()
{}

const wchar_t* AxExHResult::widewhat() const throw()
{
	return _widewhat.c_str();
}

const char* AxExHResult::what() const throw()
{
  return _what.c_str();
}

HRESULT AxExHResult::getHResult() const
{
	return _hresult;
}

int AxExHResult::getLine() const
{
	return _line;
}

const char* AxExHResult::getFile() const
{
	return _file;
}


//=---------------------------------------------------------------------=

AxExSmartPointer::AxExSmartPointer()
:	AxEx( L"AAFSmartPointer assertion failed" )
{}

AxExSmartPointer::~AxExSmartPointer() throw()
{}

//=---------------------------------------------------------------------=

AxExBadImp::AxExBadImp( const wchar_t* what )
  :	AxEx( AxString(AxString(L"Bad implementation: ") + AxString(what)).c_str() )
{}

AxExBadImp::~AxExBadImp() throw()
{}
