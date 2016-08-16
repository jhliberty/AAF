//=---------------------------------------------------------------------=
//
// $Id: Timecode.cpp,v 1.4 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "ParamMaps.h"

#include <axFileGen.h>

#include <AxDictionary.h>
#include <AxComponent.h>

namespace {


//=---------------------------------------------------------------------=

// Stick in a custom class here that has an aafTimecode_t private data member.
// Implement a timecode parser that will decipher hh:mm::ss::ff, and a static method
// that does a dynamic cast to TimecodeVal and returns the aafTimecode_t.... or
// something like that.

AXFG_OP(
  TimecodeVal,           
  L"TimecodeVal",
  L"Create a timecode value.",
  L"TimecodeValName {hh:mm:ss:ff | hh:mm:ss;ff}",
  L"TimecodeVal is not implemented yet so don't use it.",
  3,
  3 ) 

TimecodeVal::~TimecodeVal()
{}

void TimecodeVal::Execute( const std::vector<AxString>& argv )
{
	throw AxFGOpUsageEx( *this, L"not implemented" );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  Timecode,           
  L"Timecode",
  L"Create a timecode segment.",
  L"FileName TimecodeName length {{ TimecodeValName | hh:mm:ss:ff | hh:mm:ss;ff | {start_frame drop_type frames_per_second}}",
  L"Only the last timecode specification is support at the moment.  Length is in frames.",
  5,
  7 ) 

Timecode::~Timecode()
{}

void Timecode::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString tcName   = argv[2];
	AxString length   = argv[3];

	if ( 7 != argv.size() ) {
		throw AxFGOpUsageEx( *this, L"Option not implemented" );
	}

	AxString startFrame = argv[4];
	AxString dropType   = argv[5];
	AxString fps        = argv[6];

	IAAFTimecodeSP spTC;
	AxCreateInstance(  DictionaryFromFileOp( fileName ), spTC );
	AxTimecode axTC( spTC );

	aafTimecode_t tc;
	tc.startFrame = AxStringUtil::strtol( startFrame );
	tc.drop       = DropTypeParams::GetInstance().Find( *this, dropType );
	tc.fps        = static_cast<aafUInt16>(AxStringUtil::strtol( fps ));
	
	axTC.Initialize( AxStringUtil::strtol( length ), tc ) ;


	SetCOM( spTC );
	RegisterInstance( tcName );
}

} // end of namespace


