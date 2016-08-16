//=---------------------------------------------------------------------=
//
// $Id: AddMasterMob.cpp,v 1.5 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

#include <MultiGenTest.h>

#include "MultiGenCommon.h"

#include <AAFStoredObjectIDs.h>

#include <memory>

namespace {

class AddMasterMobs : public MultiGenTest
{ 
public:
  AddMasterMobs()
    : MultiGenTest()
  {}

  virtual ~AddMasterMobs()
  {}

  virtual void RunTest( CmdState& state, int argc, char** argv );
};

void AddMasterMobs::RunTest( CmdState& state, int argc, char** argv )
{
  // expect at least one argument (beyond argv[0])
  if ( argc < 2 ) {
    throw UsageEx( "AddMasterMob expects one more mob names as arguments." );
  }

  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );
  
  IAAFSmartPointer<IAAFDictionary> dictionary;
  CHECK_HRESULT( file->GetDictionary( &dictionary ) );

  int i;
  for( i = 1; i < argc; i++ ) {

    IAAFSmartPointer<IAAFMasterMob> masmob;
    CHECK_HRESULT( dictionary->CreateInstance( AUID_AAFMasterMob,
					   IID_IAAFMasterMob,
					   ToIUnknown( &masmob ) ) );
    IAAFSmartPointer<IAAFMob> mob;
    CHECK_HRESULT( masmob->QueryInterface( IID_IAAFMob, ToVoid(&mob) ) );
    CHECK_HRESULT( masmob->Initialize() );

    auto_ptr<wchar_t> mobName( ToWideString( argv[i] ) );
    
    CHECK_HRESULT( mob->SetName( mobName.get() ) );

    CHECK_HRESULT( header->AddMob( mob ) );
  }
}

StandardFactory<AddMasterMobs> factory( "AddMasterMobs",
					"Add on or more MasterMob's to a file.",
					"[mod_name mob_name mob_name ...]",
					"Depricated.  Do not use in new test configurations.",
					1, -1
				      );

} // end of namespace
