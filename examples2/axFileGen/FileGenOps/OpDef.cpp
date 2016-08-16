//=---------------------------------------------------------------------=
//
// $Id: OpDef.cpp,v 1.4 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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
#include <AxEssence.h>
#include <AxDefObject.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  OperationDef,           
  L"OperationDef",
  L"Create an operation definition.",
  L"FileName OpDefName DataDefName is_timewarp op_def_id num_inputs bypass name description",
  L"is_timewarp is a bool, its value is true|false",
  10,
  10 ) 

OperationDef::~OperationDef()
{}

void OperationDef::Execute( const std::vector<AxString>& argv )
{
	AxString fileName   = argv[1];
	AxString opDefName  = argv[2];
	AxString dataDefName = argv[3];
	AxString isTimeWarp = argv[4];
	AxString opDefID    = argv[5];
	AxString numInputs  = argv[6];
	AxString bypass     = argv[7];
	AxString name       = argv[8];
	AxString desc       = argv[9];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );
	
	aafUID_t opDefIDVal = OperationDefParams::GetInstance().Find( *this, opDefID );

	if ( !axDict.isKnownOperationDef( opDefIDVal ) ) {

		IAAFOperationDefSP spOpDef;
		AxCreateInstance( axDict, spOpDef );
		AxOperationDef axOpDef( spOpDef );

		axOpDef.Initialize( opDefIDVal,
							name,
							desc );

		IAAFDataDefSP spDataDef;
		GetInstance( dataDefName ).GetCOM( spDataDef );

		axOpDef.SetDataDef( spDataDef );
		axOpDef.SetIsTimeWarp( BooleanParams::GetInstance().Find( *this, isTimeWarp ) );
		axOpDef.SetCategory( opDefIDVal );
		axOpDef.SetNumberInputs( AxStringUtil::strtol( numInputs ) );
		axOpDef.SetBypass( AxStringUtil::strtol( bypass ) );

		axDict.RegisterOperationDef( axOpDef );
	}
	
	SetCOM( axDict.LookupOperationDef( opDefIDVal ) );
	RegisterInstance( opDefName );
}

} // end of namespace


