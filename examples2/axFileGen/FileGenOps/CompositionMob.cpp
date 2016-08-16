//=---------------------------------------------------------------------=
//
// $Id: CompositionMob.cpp,v 1.4 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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

#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxMob.h>

#include <axFileGen.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  CompositionMob,           
  L"CompositionMob",
  L"Create a composition mob.",
  L"FileName CompMobName mob_name",
  L"CompMobName is your reference to the object.  mob_name is the persistent mob name",
  4,
  4 ) 

CompositionMob::~CompositionMob()
{}

void CompositionMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString mobName  = argv[2];
	AxString name     = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	IAAFCompositionMobSP spCompMob;
	AxCreateInstance( axDict, spCompMob );
	AxCompositionMob axCompMob( spCompMob );

	axCompMob.Initialize( name );
 
	// gcc 3.2.2 doesn't like this:
	//
	// AxHeader( HeaderFromFileOp( fileName ) ).AddMob( axCompMob );
	//
	// It generates: "CompositionMob.cpp:57: no matching function
	// for call to `AxHeader::AxHeader(AxString&)'".  Why does it
	// think HeaderFromFileOp() returns an AxString& !!!!  Is that
	// a bug in gcc 3.2.2?  Avoiding temporary object creation
	// works around the problem.  A similar change was made to
	// MasterMob.cpp
	AxHeader axHeader( HeaderFromFileOp( fileName ) );
	axHeader.AddMob( axCompMob );

	SetCOM( spCompMob );
	RegisterInstance( mobName );
}

} // end of namespace


