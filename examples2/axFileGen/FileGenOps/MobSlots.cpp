//=---------------------------------------------------------------------=
//
// $Id: MobSlots.cpp,v 1.11 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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

#include "Rate.h"

#include <axFileGen.h>

#include <AxDictionary.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxIterator.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP_DECLARATION( MobSlotSet )

// A collection of operations on MobSlots, all implemented by the same
// AxFGOp.

AXFG_OP_FACTORY_DECLARATION_B(
  MobSlotSet,
  MobSlotSetSlotId,
  L"MobSlotSetSlotId",
  L"Set the slot id.",
  L"MobSlotName slot_id",
  L"",
  3,
  3 )

AXFG_OP_FACTORY_DECLARATION_B(
  MobSlotSet,
  MobSlotSetPhysNum,
  L"MobSlotSetPhysNum",
  L"Set the physical track number for any mob slot.",
  L"MobSlotName phys_num",
  L"",
  3,
  3 )

AXFG_OP_FACTORY_DECLARATION_B(
  MobSlotSet,
  MobSlotSetName,
  L"MobSlotSetName",
  L"Set the mob slot name.",
  L"MobSlotName slot_name",
  L"",
  3,
  3 )


MobSlotSet::~MobSlotSet()
{}

void MobSlotSet::Execute( const std::vector<AxString>& argv )
{
	AxString opName   = argv[0];
	AxString slotName = argv[1];
	// See below for further opName specific arguments.

	IAAFMobSlotSP spSlot;
	GetInstance( slotName ).GetCOM( spSlot );
	AxMobSlot axSlot( spSlot );

	if ( opName == L"MobSlotSetPhysNum" ) {
		AxString physNum = argv[2];
		axSlot.SetPhysicalNum( AxStringUtil::strtol( physNum ) );
	}
	else if ( opName == L"MobSlotSetName" ) {
		AxString name = argv[2];
		axSlot.SetName( name );
	}
	else if ( opName == L"MobSlotSetSlotId" ) {
		AxString slotId = argv[2];
		axSlot.SetSlotID( AxStringUtil::strtol( slotId ) );
	}
	else {
		// This should never happen in a correct implementation.
		// If it does, one of strings used in the above opName
		// comparisons does not match the factory name.
		throw AxFGEx( L"bad implementation" );
	}
}

//=---------------------------------------------------------------------=

AXFG_OP(
  TimeLineMobSlot,           
  L"TimeLineMobSlot",
  L"Creates a TimeLineMobSlot.",
  L"FileName SlotName origin RateName.",
  L"The origin is an integer.  The RateName is a reference to a Rate object. SlotName is the name of the object - not the AAF slot name.",
  5,
  5 ) 

TimeLineMobSlot::~TimeLineMobSlot()
{}

void TimeLineMobSlot::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString slotName = argv[2];
	AxString origin   = argv[3];
	AxString rateName = argv[4];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );
	IAAFTimelineMobSlotSP spSlot;
	AxCreateInstance( axDict, spSlot );
	AxTimelineMobSlot axSlot( spSlot );

	axSlot.Initialize();

	axSlot.SetOrigin( AxStringUtil::strtol( origin ) );

	axSlot.SetEditRate( RateOpToRational( GetInstance( rateName ) ) );

	SetCOM( spSlot );
	RegisterInstance( slotName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  EventMobSlot,           
  L"EventMobSlot",
  L"Creates a EventMobSlot.",
  L"FileName SlotName EditRateName",
  L"SlotName is the name of the reference to this object - not the AAF slot name.",
  4,
  4 )

EventMobSlot::~EventMobSlot()
{}

void EventMobSlot::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString slotName = argv[2];
	AxString editRate = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	IAAFEventMobSlotSP spSlot;
	AxCreateInstance( axDict, spSlot );

	AxEventMobSlot axEventMobSlot( spSlot );
	axEventMobSlot.SetEditRate( RateOpToRational( GetInstance( editRate ) ) );

	SetCOM( spSlot );
	RegisterInstance( slotName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  StaticMobSlot,           
  L"StaticMobSlot",
  L"Creates a StaticMobSlot.",
  L"FileName SlotName",
  L"SlotName is the name of the reference to this object - not the AAF slot name.",
  3,
  3 )

StaticMobSlot::~StaticMobSlot()
{}

void StaticMobSlot::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString slotName = argv[2];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	IAAFStaticMobSlotSP spSlot;
	AxCreateInstance( axDict, spSlot );

	SetCOM( spSlot );
	RegisterInstance( slotName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  GetMobSlot,
  L"GetMobSlot",
  L"Fetches a mob slot from a mob.",
  L"MobName slot_id SlotName",
  L"The fetched slot is reference using SlotName.",
  4,
  4 )

GetMobSlot::~GetMobSlot()
{}

void GetMobSlot::Execute( const std::vector<AxString>& argv )
{
	AxString mobName = argv[1];
	AxString slotId = argv[2];
	AxString slotName = argv[3];
  
	IAAFMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxMob axMob( spMob );

	aafSlotID_t slotIdVal = AxStringUtil::strtol( slotId );

	AxMobSlotIter axSlotIter( axMob.GetSlots() );
	IAAFSmartPointer2<IAAFMobSlot> nextSlot;
	bool notAtEnd;

	for( notAtEnd = axSlotIter.NextOne( nextSlot );
	     notAtEnd;
	     notAtEnd = axSlotIter.NextOne( nextSlot ) ) {
		AxMobSlot axSlot( nextSlot );
		if ( slotIdVal == axSlot.GetSlotID() ) {
			break;
		}
	}

	if ( notAtEnd ) {
		SetCOM( nextSlot );
		RegisterInstance( slotName );
	}
	else {
		throw AxFGOpUsageEx( *this, slotId + L" not found" );
	}
}

//=---------------------------------------------------------------------=

AXFG_OP(
  GetSlotSegment,
  L"GetSlotSegment",
  L"Get a Segment from a MobSlot.",
  L"SlotName SegmentName",
  L"",
  3,
  3 )

GetSlotSegment::~GetSlotSegment()
{}

void GetSlotSegment::Execute( const std::vector<AxString>& argv )
{
	AxString slotName = argv[1];
	AxString segName = argv[2];

	IAAFMobSlotSP spSlot;
	GetInstance( slotName ).GetCOM( spSlot );
	AxMobSlot axSlot( spSlot );

	SetCOM( axSlot.GetSegment() );
	RegisterInstance( segName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SetSlotSegment,
  L"SetSlotSegment",
  L"Set a Segment on a MobSlot.",
  L"SlotName SegmentName",
  L"",
  3,
  3 )

SetSlotSegment::~SetSlotSegment()
{}

void SetSlotSegment::Execute( const std::vector<AxString>& argv )
{
	AxString slotName = argv[1];
	AxString segName = argv[2];

	IAAFMobSlotSP spSlot;
	GetInstance( slotName ).GetCOM( spSlot );
	AxMobSlot axSlot( spSlot );

	IAAFSegmentSP spSeg;
	GetInstance( segName ).GetCOM( spSeg );
	axSlot.SetSegment( spSeg);
}

} // end of namespace


