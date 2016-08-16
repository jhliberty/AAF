//=---------------------------------------------------------------------=
//
// $Id: SlotOp.cpp,v 1.4 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

namespace {

//=---------------------------------------------------------------------=

STANDARD_TEST_DECLARATION(SlotDataDef);
StandardFactory<SlotDataDef> SlotDataDefFactory(
  "SlotDataDef",
  "Test that a MobSlot\'s data definition is of a particular type.",
  "mob_name slot_id {sound | picture}",
  "",
  4,
  4 
  );

void SlotDataDef::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name = argv[1];
  const unsigned long slotId = ToULong( argv[2] );
  string dataKindName( argv[3] );

  IAAFSmartPointer<IAAFMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  IAAFSmartPointer<IAAFMobSlot> slot;
  CHECK_HRESULT( mob->LookupSlot( slotId, &slot ) );

  IAAFSmartPointer<IAAFDataDef> dataDef;
  CHECK_HRESULT( slot->GetDataDef( &dataDef ) );

  if ( dataKindName == "sound" ) {
       aafBoolean_t isSound;
       CHECK_HRESULT( dataDef->IsSoundKind( &isSound ) );
       if ( !isSound ) {
	 throw TestFailedEx( "not sound" );
       }
  }
  else if ( dataKindName == "picture" )  {
    aafBoolean_t isPicture;
    CHECK_HRESULT( dataDef->IsPictureKind( &isPicture ) );
    if ( !isPicture ) {
      throw TestFailedEx( "not picture" );
    }
  }
  else {
    throw UsageEx( "data kind must be {sound|picture}" );
  }
}

//=---------------------------------------------------------------------=

} // end of namespace
