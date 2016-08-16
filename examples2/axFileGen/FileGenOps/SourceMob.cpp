//=---------------------------------------------------------------------=
//
// $Id: SourceMob.cpp,v 1.4 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include <axFileGen.h>

#include <AxDictionary.h>
#include <AxMob.h>
#include <AxHeader.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  SourceMob,           
  L"SourceMob",
  L"Create a source mob and add it to the file header.",
  L"file_ref name essence_desc_ref",
  L"Initialized with a nill source reference.",
  4,
  4 ) 

SourceMob::~SourceMob()
{}

void SourceMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName        = argv[1];
	AxString sourceMobName   = argv[2];
	AxString essenceDescName = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );
	IAAFSourceMobSP spSourceMob;
	AxCreateInstance( axDict, spSourceMob );
	AxSourceMob axSrcMob( spSourceMob );

	IAAFEssenceDescriptorSP spDesc;
	GetInstance( essenceDescName ).GetCOM( spDesc );
	axSrcMob.SetEssenceDescriptor( spDesc );

	AxHeader axHeader( HeaderFromFileOp( fileName) );
	axHeader.AddMob( axSrcMob );

	SetCOM( spSourceMob );
	RegisterInstance( sourceMobName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SetEssenceDesc,           
  L"SetEssenceDesc",
  L"Set essence descriptor on a SourceMob",
  L"SourceMobName DescName",
  L"",
  3,
  3 ) 

SetEssenceDesc::~SetEssenceDesc()
{}

void SetEssenceDesc::Execute( const std::vector<AxString>& argv )
{
	AxString srcMobName = argv[1];
	AxString descName = argv[2];

	IAAFSourceMobSP spSrcMob;
	GetInstance( srcMobName ).GetCOM( spSrcMob );
	AxSourceMob axSrcMob( spSrcMob );

	IAAFEssenceDescriptorSP spDesc;
	GetInstance( descName ).GetCOM( spDesc );

	axSrcMob.SetEssenceDescriptor( spDesc );
}

} // end of namespace


