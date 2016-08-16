//=---------------------------------------------------------------------=
//
// $Id: MasterMob.cpp,v 1.6 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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

#include "ParamMaps.h"
#include "Rate.h"

#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxMob.h>

#include <AAFContainerDefs.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  MasterMob,           
  L"MasterMob",
  L"Creates a new master mob and adds it to the file.",
  L"FileName MobName mob_name",
  L"MobName is your reference to the object.  mob_name is the persistent mob name",
  4,
  4 ) 

MasterMob::~MasterMob()
{}

void MasterMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName    = argv[1];
	AxString mobName     = argv[2];
	AxString name        = argv[3];

	IAAFMasterMobSP spMasMob;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spMasMob );

	AxMasterMob axMasMob( spMasMob );

	// FIXME - Why does the composition mob initialize take a name,
	// but the master mob doesn't???
	axMasMob.Initialize();

	axMasMob.SetName( name );
	
	AxHeader axHeader( HeaderFromFileOp(fileName) );
	axHeader.AddMob( axMasMob );

	SetCOM( spMasMob );
	RegisterInstance( mobName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CreateEssence,   
  L"CreateEssence",
  L"Creates essence via the master mob CreateEssence call.",
  L"MobName EssenceName slot_id DataDefName codec_id EditeRate SampleRate"
            L"compression_enable container [LocatorName]",
  L"The resulting essence access object is referenced by EssenceName.  If file_format is AAFContainer, omit locator.",
  10,
  11 ) 

CreateEssence::~CreateEssence()
{}

void CreateEssence::Execute( const std::vector<AxString>& argv )
{
	AxString mobName     = argv[1];
	AxString essenceName = argv[2];
	AxString slotId      = argv[3];
	AxString dataDefName = argv[4];
	AxString codecId     = argv[5];
	AxString editRate   = argv[6];
	AxString sampleRate  = argv[7];
	AxString compression = argv[8];
	AxString container   = argv[9];

	bool locatorDefined = false;
	AxString locatorName;

	if ( argv.size() == GetMaxArgC() ) {
		locatorDefined = true;
		locatorName = argv[10];
	}
	else if ( ContainerDefParams::GetInstance().Find( *this, container ) !=
		     ContainerAAF ) {

		throw AxFGOpUsageEx( *this );
	}

	IAAFMasterMobSP spMasMob;
	GetInstance( mobName ).GetCOM( spMasMob );
	AxMasterMob axMasMob( spMasMob );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName ).GetCOM( spDataDef );
		
	IAAFLocatorSP spLocator;
	if ( locatorDefined ) {
		GetInstance( locatorName ).GetCOM( spLocator );
	}
		
	IAAFEssenceAccessSP spEssenceAccess;

	spEssenceAccess = axMasMob.CreateEssence(
		AxStringUtil::strtol( slotId ),
		spDataDef,
		CodecIDParams::GetInstance().Find( *this, codecId ),
		RateOpToRational( GetInstance( editRate ) ),
		RateOpToRational( GetInstance( sampleRate ) ),
		CompressionEnableParams::GetInstance().Find( *this, compression ),
		spLocator,
		ContainerDefParams::GetInstance().Find( *this, container ) );

	SetCOM( spEssenceAccess );
	RegisterInstance( essenceName );
}


} // end of namespace


