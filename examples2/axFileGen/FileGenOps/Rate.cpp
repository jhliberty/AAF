//=---------------------------------------------------------------------=
//
// $Id: Rate.cpp,v 1.3 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include <AxUtil.h>

namespace {


// Rate declares private data and an extra method, hence, is not declared
// using the AXFG_OP macro.
	
class Rate : public AxFGOp { 
public:

  Rate ( const AxString& opName )
    : AxFGOp( opName )
	{}

  virtual ~Rate();

  virtual void Execute( const std::vector<AxString>& args );

  aafRational_t GetRational()
  {
	aafRational_t rat;
	rat.numerator = _num;
	rat.denominator = _den;
	return rat;
  }

private:
	aafUInt32 _num;
	aafUInt32 _den;
};


AXFG_OP_FACTORY_DECLARATION(
  Rate,           
  L"Rate",
  L"Represent a rate as rational number.",
  L"Name numerator denominator",
  L"Refer to this rate using Name.",
  4,
  4 ) 

Rate::~Rate()
{}

void Rate::Execute( const std::vector<AxString>& argv )
{
	AxString name = argv[1];
	AxString num  = argv[2];
	AxString den  = argv[3];

	_num = AxStringUtil::strtol( num );
	_den = AxStringUtil::strtol( den );

	RegisterInstance( name );
}

} // end of namespace


aafRational_t RateOpToRational( AxFGOp& op )
{
	Rate& rateOp = dynamic_cast<Rate&>( op );

	return rateOp.GetRational();
}
