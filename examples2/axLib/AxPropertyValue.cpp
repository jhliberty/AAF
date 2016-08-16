//=---------------------------------------------------------------------=
//
// $Id: AxPropertyValue.cpp,v 1.8 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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


#include "AxPropertyValue.h"
#include "AxMetaDef.h"
#include "AxEx.h"

#include <AAF.h>

//=---------------------------------------------------------------------=

AxPropertyValueNoopPrtcl::AxPropertyValueNoopPrtcl()
{}

AxPropertyValueNoopPrtcl::~AxPropertyValueNoopPrtcl()
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefCharacterSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefIndirectSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefIntSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefRenameSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefEnumSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefExtEnumSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefFixedArraySP&)
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefRecordSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefSetSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefStreamSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefStringSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefStrongObjRefSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefWeakObjRefSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefObjectRefSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefOpaqueSP& )
{}

void AxPropertyValueNoopPrtcl::process( IAAFPropertyValueSP&, IAAFTypeDefVariableArraySP& )
{}

//=---------------------------------------------------------------------=

AxPropertyValue::AxPropertyValue( IAAFPropertyValueSP spIaafPropertyValue )
:	AxBaseObj( AxQueryInterface<IAAFPropertyValue, IUnknown>( spIaafPropertyValue )) ,
	_spIaafPropertyValue( spIaafPropertyValue )
{}

AxPropertyValue::AxPropertyValue( const AxPropertyValue& other )
:	AxBaseObj( AxQueryInterface<IAAFPropertyValue, IUnknown>( other._spIaafPropertyValue )),
	_spIaafPropertyValue( other._spIaafPropertyValue )
{}

AxPropertyValue::~AxPropertyValue()
{}

IAAFTypeDefSP AxPropertyValue::GetType()
{
	IAAFTypeDefSP spIaafTypeDef;

	CHECK_HRESULT( _spIaafPropertyValue->GetType( &spIaafTypeDef ) );

	return spIaafTypeDef;
}

void AxPropertyValue::Process( AxPropertyValuePrtcl& prtcl )
{
	AxTypeDef axTypeDef( GetType() );
	
	
	eAAFTypeCategory_t cat = axTypeDef.GetTypeCategory();

	switch( cat )
	{

#define CASE(T)	\
		case kAAFTypeCat##T :	\
		{ \
		    IAAFTypeDef##T##SP sp; \
			AxQueryInterface( axTypeDef.GetTypeDefSP(), sp ); \
			prtcl.process( _spIaafPropertyValue, sp ); \
			break; \
		}

		CASE( Int )
		CASE( Character )
		CASE( StrongObjRef )
		CASE( WeakObjRef )
		CASE( Rename )
		CASE( Enum )
		CASE( FixedArray )
		// CASE( VariableArray )
		CASE( Set )
		CASE( Record )
		CASE( Stream )
		CASE( String )
		CASE( ExtEnum )
		CASE( Indirect )
		CASE( Opaque )
		CASE( VariableArray )

#undef CASE
	
		case kAAFTypeCatUnknown:
			// FIXME - What to do here?  Get RawAccessType perhaps, but how is that
			// distinquished from encrypted using only the process() argument type?
			break;
		case kAAFTypeCatEncrypted:
			// FIXME - see kAAFTypeCatUnknown above.
			break;

		default:
			throw AxExBadImp( L"unknown type category" );
	}
}



