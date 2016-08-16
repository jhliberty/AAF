//=---------------------------------------------------------------------=
//
// $Id: AxUtil.cpp,v 1.27 2012/05/04 17:19:13 jptrainor Exp $ $Name: V116 $
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

#include "AxUtil.h"

#include <wchar.h>

#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

//=---------------------------------------------------------------------=

namespace {

AxString catUnknownStr( L"unknown" );

};

//=---------------------------------------------------------------------=

// AxContants go here

const aafMobID_t AxConstants::NULL_MOBID  =  { {0,0,0,0,0,0,0,0,0,0,0,0}, // SMPTELable
					       0, // length
					       0, // instanceHigh
					       0, // instanceMid
					       0, // instanceLow
					       { 0, 0, 0, {0,0,0,0,0,0,0,0} } // material (uuid) 
                                             };

const aafUID_t AxConstants::NULL_UID = {0,0,0,{0,0,0,0,0,0,0,0}};

//=---------------------------------------------------------------------=


AxTypeCatMap::AxTypeCatMap()
{
#define MAP_ADD( T ) _map[ kAAFTypeCat##T ] = \
	AxString( AxStringUtil::mbtowc( "IAAFTypeDef" #T ) );

	MAP_ADD( Int )
	MAP_ADD( Character )
	MAP_ADD( StrongObjRef )
	MAP_ADD( WeakObjRef )
	MAP_ADD( Rename )
	MAP_ADD( Enum )
	MAP_ADD( FixedArray )
	MAP_ADD( VariableArray )
	MAP_ADD( Set )
	MAP_ADD( Record )
	MAP_ADD( Stream )
	MAP_ADD( String )
	MAP_ADD( ExtEnum )
	MAP_ADD( Indirect )
	MAP_ADD( Opaque )
	MAP_ADD( VariableArray )
	MAP_ADD( Unknown )
	MAP_ADD( Encrypted )

#undef MAP_ADD
}

AxTypeCatMap::~AxTypeCatMap()
{}

const AxTypeCatMap& AxTypeCatMap::getInstance()
{
	static AxTypeCatMap* instance = 0;

	if ( !instance ) {
		instance = new AxTypeCatMap();
	}

	return *instance;
}
 
const AxString& AxTypeCatMap::getStr( eAAFTypeCategory_t cat ) const
{
	std::map<eAAFTypeCategory_t,AxString>::const_iterator iter = _map.find( cat );
	
	if ( iter == _map.end() ) {
		return catUnknownStr;
	}

	return iter->second;

}

//=---------------------------------------------------------------------=

AxCmdLineArgs::AxCmdLineArgs( int argc, char** argv )
:	_argc( argc ),
	_argv( argv ),
	_fetchSet()
{}

AxCmdLineArgs::~AxCmdLineArgs()
{}

std::pair<bool,int> AxCmdLineArgs::get( const char* opt, int i )
{
	std::pair<bool,int> result(false,-1);

	// A Hash map would be better here....

	for( ; i < _argc; i++ ) {
		if ( strcmp( opt, _argv[i] ) == 0 ) {
			result.first = true;
			result.second = i;
			MarkFetched(i);
			return result;
		}
	}

	return result;
}

std::pair<bool,const char*> AxCmdLineArgs::get( int n, int m )
{
	std::pair<bool,const char*> result(false,reinterpret_cast<const char *>(0));

	if ( m <= n  &&  n < _argc ) {
		result.first = true;
		result.second = _argv[n];
		MarkFetched(n);
	}

	return result;
}

bool AxCmdLineArgs::IsFetched( int n, int m )
{
  if ( n < m )
  {
    return true;
  }

  return _fetchSet.find(n) != _fetchSet.end();
}

void AxCmdLineArgs::MarkFetched( int n )
{
  _fetchSet.insert(n);
}

//=---------------------------------------------------------------------=

AxString AxStringUtil::bool2Str( bool value )
{
	return value ? L"true" : L"false";
}

AxString AxStringUtil::bool2Str( aafBoolean_t value )
{
	return AxStringUtil::bool2Str( value ? true : false );
}

AxString AxStringUtil::int2StrHex( aafUInt32 n )
{
    const unsigned int bufSize = 9;
	char buf[bufSize];

	sprintf( buf, "%08x", n );

	return AxStringUtil::mbtowc( buf );
}

AxString AxStringUtil::int2Str( aafInt32 n )
{
	// 32 bit int requires 1 character for sign, plus
	// 10 digits, plus terminating null.
    const unsigned int bufSize = 12;
    char buf[bufSize];

	sprintf( buf, "%d", n );

	return AxStringUtil::mbtowc( buf );
}
//=---------------------------------------------------------------------=

AxString AxStringUtil::mbtowc( const char* cstr )
{
	std::wostringstream os;

	size_t i;
	for (i = 0; i < strlen(cstr); i++ ){ 
		wchar_t wc;
		if ( -1 != ::mbtowc( &wc, &cstr[i], 1 ) ) {
			os << wc;
		}
	}

	return AxString( os.str() );
}


//=---------------------------------------------------------------------=

std::string AxStringUtil::wctomb( const AxString& s )
{
	std::stringstream os;
	const wchar_t* ws = s.c_str();
	
	size_t i;
	for (i = 0; i < s.length(); i++ ){ 
		char c;
		if ( -1 != ::wctomb( &c, ws[i] ) ) {
			os << c;
		}
	}

	return os.str();
}

//=---------------------------------------------------------------------=

long AxStringUtil::strtol( const char* s )
{
	long val;
	char *addr_of_last_processed_char;

	val = ::strtol( s, &addr_of_last_processed_char, 0 );

	// if no characters were converted
	if ( s == addr_of_last_processed_char ) {
		throw AxEx( L"strtol() failed, unable to convert: " + mbtowc( s ) );
	}

	if ( (LONG_MAX == val || LONG_MIN == val) &&
		 errno == ERANGE ) {
		throw AxEx( L"strtol() failed, value out of range: " + mbtowc( s ) );
	}


	return val;
}

//=---------------------------------------------------------------------=

unsigned long AxStringUtil::strtoul( const char* s )
{
	unsigned long val;
	char *addr_of_last_processed_char;

	val = ::strtoul( s, &addr_of_last_processed_char, 0 );

	// if no characters were converted
	if ( s == addr_of_last_processed_char ) {
		throw AxEx( L"strtoul() failed, unable to convert: " + mbtowc( s ) );
	}

	if ( ULONG_MAX == val && errno == ERANGE ) {
		throw AxEx( L"strtol() failed, value out of range: " + mbtowc( s ) );
	}


	return val;
}

//=---------------------------------------------------------------------=

long AxStringUtil::strtol( const AxString& s )
{
	std::string s8 = wctomb( s );
	return strtol( s8.c_str() );
}

//=---------------------------------------------------------------------=

unsigned long AxStringUtil::strtoul( const AxString& s )
{
	std::string s8 = wctomb( s );
	return strtoul( s8.c_str() );
}

//=---------------------------------------------------------------------=

// Returns a semi readable string representing a UID.  If people want to agree
// a different format thats fine.  This is just a first attempt - do not assume that
// the text format won't change in the future.
// Would rather use iostreams but for wostringstream MSVC6 has a problem with the setw
// manipulator.
AxString AxStringUtil::uid2Str(const aafUID_t & uid)
{
	const int bufSize = 2 * 16 + 3 + 1; // 16 bytes as hex + 3 hyphens + null termination
	char buf[bufSize];
	int rc;
	using namespace std;

	rc = sprintf( buf,
				"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x", // UID
				uid.Data1, uid.Data2, uid.Data3,
				uid.Data4[0], uid.Data4[1], uid.Data4[2], uid.Data4[3],
				uid.Data4[4], uid.Data4[5], uid.Data4[6], uid.Data4[7] );
	if (rc != bufSize - 1)
	{
		throw AxEx( L"Error in uid2Str" );
	}    
	return mbtowc( buf );
}

//=---------------------------------------------------------------------=

// Returns a semi readable string representing the Mob ID.  If people want to agree
// a different format thats fine.  This is just a first attempt - do not assume that
// the text format won't change in the future.
// Would rather use iostreams but for wostringstream MSVC6 has a problem with the setw
// manipulator.
AxString AxStringUtil::mobid2Str(const aafMobID_t & mobID)
{
	const int bufSize = 64 + 4 + 1; // 32 bytes as hex + 4 hyphens + null termination
	char buf[bufSize];
	int rc;
	using namespace std;

	rc = sprintf( buf,
				"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x" // SMPTE label
				"%02x" // length
				"%02x%02x%02x-" // instance high mid low
				"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x", //material UID
				mobID.SMPTELabel[0], mobID.SMPTELabel[1], mobID.SMPTELabel[2], 
				mobID.SMPTELabel[3], mobID.SMPTELabel[4], mobID.SMPTELabel[5],
				mobID.SMPTELabel[6], mobID.SMPTELabel[7], mobID.SMPTELabel[8],
				mobID.SMPTELabel[9], mobID.SMPTELabel[10], mobID.SMPTELabel[11],
				mobID.length,
				mobID.instanceHigh, mobID.instanceMid, mobID.instanceLow,
				mobID.material.Data1, mobID.material.Data2, mobID.material.Data3,
				mobID.material.Data4[0], mobID.material.Data4[1],
				mobID.material.Data4[2], mobID.material.Data4[3],
				mobID.material.Data4[4], mobID.material.Data4[5], 
				mobID.material.Data4[6], mobID.material.Data4[7] );
	if (rc != bufSize - 1)
	{
		throw AxEx( L"Error in mobid2Str" );
	}    
	return mbtowc( buf );
}


AxString AxTaggedValueToString( IAAFTaggedValueSP& sp )
{
	aafUInt32 sizeInBytes = 0;

	CHECK_HRESULT( sp->GetValueBufLen( &sizeInBytes ) );

	// size is in bytes!  Divide by sizeof(aafCharacter) to allocate correctly
	// sized aafCharacter array.  Add one to account for possible rounding.

	int sizeInChars = (sizeInBytes /sizeof(aafCharacter)) + 1;
	std::vector< aafCharacter > buf( sizeInChars );

	aafUInt32 bytesRead; // unused
	CHECK_HRESULT( sp->GetValue( sizeInChars*sizeof(aafCharacter),
				     reinterpret_cast<aafDataBuffer_t>(&buf[0]), &bytesRead ) );

	AxString name( &buf[0] );

	return name;
}
