//=---------------------------------------------------------------------=
//
// $Id: MobOp.cpp,v 1.4 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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
#include <sstream>

namespace {

//====================================================================

STANDARD_TEST_DECLARATION(MasterMobAdd);
StandardFactory<MasterMobAdd> MasterMobAddFactory(
  "MasterMobAdd",
  "Add a master mob.",
  "mob_name",
  "",
  2, 2
  );

void MasterMobAdd::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name = argv[1];

  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );
  
  IAAFSmartPointer<IAAFDictionary> dictionary;
  CHECK_HRESULT( file->GetDictionary( &dictionary ) );

  IAAFSmartPointer<IAAFMasterMob> masmob;
  CHECK_HRESULT( dictionary->CreateInstance( AUID_AAFMasterMob,
					     IID_IAAFMasterMob,
					     ToIUnknown( &masmob ) ) );
  IAAFSmartPointer<IAAFMob> mob;
  CHECK_HRESULT( masmob->QueryInterface( IID_IAAFMob, ToVoid(&mob) ) );
  CHECK_HRESULT( masmob->Initialize() );
  
  auto_ptr<wchar_t> mobName( ToWideString( name ) );
  
  CHECK_HRESULT( mob->SetName( mobName.get() ) );
  
  CHECK_HRESULT( header->AddMob( mob ) );
}

//====================================================================

STANDARD_TEST_DECLARATION(RemoveMob);
StandardFactory<RemoveMob> RemoveMobFactory(
  "RemoveMob",
  "Remove a mob.",
  "mob_name",
  "",
  2, 2
  );

void RemoveMob::RunTest( CmdState& state, int argc, char** argv )
{
  const char *name = argv[1];

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );  

  CHECK_HRESULT( header->RemoveMob( mob ) );
}

//====================================================================

STANDARD_TEST_DECLARATION(FindMasterMob);
StandardFactory<FindMasterMob> FindMasterMobFactory(
  "FindMasterMob",
  "Find a named master mob.",
  "mob_name",
  "",
  2, 2
  );

void FindMasterMob::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name = argv[1];

  IAAFSmartPointer<IAAFMasterMob> mob;
  get_mob_throw_if_not_found( state, name, mob );
}

//====================================================================

STANDARD_TEST_DECLARATION(RenameMob);
StandardFactory<RenameMob> RenameMobFactory(
  "RenameMob",
  "Rename a mob",
  "mob_name new_mob_name",
  "",
  3, 3
  );

void RenameMob::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name     = argv[1];
  const char* new_name = argv[2];

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  auto_ptr<wchar_t> wnew_name( ToWideString( new_name ) );

  CHECK_HRESULT( mob->SetName( wnew_name.get() ) );
}

//====================================================================

STANDARD_TEST_DECLARATION(CountMasterMobs);
StandardFactory<CountMasterMobs> CountMasterMobsFactory(
  "CountMasterMobs",
  "Count a the number of master mobs.",
  "expected_count",
  "",
  2, 2
  );

void CountMasterMobs::RunTest( CmdState& state, int argc, char** argv )
{
  unsigned long int expectedCount = ToULong( argv[1] );

  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );  

  aafNumSlots_t count;
  CHECK_HRESULT( header->CountMobs( kAAFMasterMob, &count ) );

  if ( count != expectedCount ) {
    stringstream anError;
    anError << "Count does not match (" << count << " != " << expectedCount << ").";
    throw TestFailedEx( anError.str() );
  }
}

//====================================================================

STANDARD_TEST_DECLARATION(AppendComment);
StandardFactory<AppendComment> AppendCommentFactory(
  "AppendComment",
  "Append comment to a mob.",
  "mob_name category comment",
  "",
  4, 4
  );

void AppendComment::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name     = argv[1];
  const char* category = argv[2];
  const char* comment  = argv[3];

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  auto_ptr<wchar_t> wcategory( ToWideString( category ) );
  auto_ptr<wchar_t> wcomment( ToWideString( comment ) );

  CHECK_HRESULT( mob->AppendComment( wcategory.get(), wcomment.get() ) );
}

//====================================================================

STANDARD_TEST_DECLARATION(RemoveComment);
StandardFactory<RemoveComment> RemoveCommentFactory(
  "RemoveComment",
  "Remove comment from name mob.",
  "mob_name category comment",
  "",
  4, 4
  );

void RemoveComment::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name     = argv[1];
  const char* category = argv[2];
  const char* comment  = argv[3];

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  auto_ptr<wchar_t> wcategory( ToWideString( category ) );
  auto_ptr<wchar_t> wcomment( ToWideString( comment ) );

  IAAFSmartPointer<IAAFTaggedValue> taggedValue;

  if( !find_comment( mob, wcategory.get(), wcomment.get(), taggedValue ) ) {
    throw TestFailedEx( string( "Could not find comment." ) );
  }

  CHECK_HRESULT( mob->RemoveComment( taggedValue ) );
}

//====================================================================

STANDARD_TEST_DECLARATION(FindComment);
StandardFactory<FindComment> FindCommentFactory(
  "FindComment",
  "Verify that named mob has comment.",
  "mob_name category comment",
  "",
  4, 4
  );

void FindComment::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name     = argv[1];
  const char* category = argv[2];
  const char* comment  = argv[3];

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  auto_ptr<wchar_t> wcategory( ToWideString( category ) );
  auto_ptr<wchar_t> wcomment( ToWideString( comment ) );

  IAAFSmartPointer<IAAFTaggedValue> taggedValue;

  if( !find_comment( mob, wcategory.get(), wcomment.get(), taggedValue ) ) {
    throw TestFailedEx( string( "Could not find comment." ) );
  }
}

//====================================================================

STANDARD_TEST_DECLARATION(CountComments);
StandardFactory<CountComments> CountCommentsFactory(
  "CountComments",
  "Count comments attached to named mob.",
  "mob_name expected_count",
  "",
  3, 3
  );

void CountComments::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name     = argv[1];
  unsigned long int expectedCount = ToULong( argv[2] );

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  aafUInt32 count;
  CHECK_HRESULT( mob->CountComments( &count ) );

  if ( count != expectedCount ) {
    stringstream anError;
    anError << "Count does not match (" << count << " != " << expectedCount << ").";
    throw TestFailedEx( anError.str() );
  }
}

//====================================================================

STANDARD_TEST_DECLARATION(CopyMob);
StandardFactory<CopyMob> CopyMobFactory(
  "CopyMob",
  "Copy a mob.",
  "mob_name copied_mob_name",
  "Excercise the mob cloning code by cloning a mob with a single file.",
  3, 3
  );

void CopyMob::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name        = argv[1];
  const char* copied_name = argv[2];

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  IAAFSmartPointer<IAAFMob> copiedMob;

  auto_ptr<wchar_t> wcopiedName( ToWideString(copied_name) );
  CHECK_HRESULT( mob->Copy( wcopiedName.get(), &copiedMob ) );
}

//====================================================================

} // end of namespace
