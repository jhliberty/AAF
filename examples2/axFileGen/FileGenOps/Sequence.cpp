//=---------------------------------------------------------------------=
//
// $Id: Sequence.cpp,v 1.3 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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
#include <AxComponent.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  Sequence,           
  L"Sequence",
  L"Create a sequence object.",
  L"FileName DataDefName SequenceName",
  L"",
  4,
  4 ) 

Sequence::~Sequence()
{}

void Sequence::Execute( const std::vector<AxString>& argv )
{
	AxString fileName    = argv[1];
	AxString dataDefName = argv[2];
	AxString seqName     = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	IAAFSequenceSP spSeq;
	AxCreateInstance( axDict, spSeq );
	AxSequence axSeq( spSeq );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName).GetCOM( spDataDef );
	
	axSeq.Initialize( spDataDef );

	SetCOM( spSeq );
	RegisterInstance( seqName );
}


//=---------------------------------------------------------------------=

AXFG_OP(
  AppendComponent,           
  L"AppendComponent",
  L"Append a component to a sequence.",
  L"SequenceName ComponentName",
  L"",
  3,
  3 ) 

AppendComponent::~AppendComponent()
{}

void AppendComponent::Execute( const std::vector<AxString>& argv )
{
	AxString seqName  = argv[1];
	AxString compName = argv[2];

	IAAFSequenceSP spSeq;
	GetInstance( seqName ).GetCOM( spSeq );
	AxSequence axSeq( spSeq );

	IAAFComponentSP spComp;
	GetInstance( compName ).GetCOM( spComp );

	axSeq.AppendComponent( spComp );
}

//=---------------------------------------------------------------------=

} // end of namespace


