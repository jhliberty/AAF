//=---------------------------------------------------------------------=
//
// $Id: Aif2XtlTimecode.cpp,v 1.4 2009/06/02 07:43:22 stuart_hc Exp $ $Name: V116 $
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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Siemens SBS Media.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <Aif2XtlTimecode.h>

#include <sstream>

#include <assert.h>
#include <stdio.h>

Aif2XtlTimecode::Aif2XtlTimecode( aafLength_t sampleCount,
				  const aafRational_t& rate )
: _sampleCount(sampleCount),
  _rateNum(rate.numerator),
  _rateDen(rate.denominator)
{}

Aif2XtlTimecode::~Aif2XtlTimecode()
{}

std::string Aif2XtlTimecode::GetXtlTimecodeString()
{
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned int millisecond;

	// quick and dirty, convert better code would not resort to
	// floating point

	double timeInSeconds = static_cast<double>(_sampleCount) *
						   static_cast<double>(_rateDen) /
						   static_cast<double>(_rateNum);

	hour = static_cast<unsigned int>(timeInSeconds / 3600.0);
	timeInSeconds -= hour * 3600.0;

	minute = static_cast<unsigned int>(timeInSeconds / 60.0);
	timeInSeconds -= minute * 60.0;

	second = static_cast<unsigned int>(timeInSeconds);
	timeInSeconds -= second;

	millisecond = static_cast<unsigned int>(timeInSeconds * 1000.0);

	// Sanity check.
	// I am a little leary of the floating point calcs.
	// Ensure millisecond is less than 1000 until some thought,
	// and use of integers, is invested.  This would be an
	// assertion in better code.
	if ( millisecond >= 1000 ) {
		 millisecond  =  999;
	}

	assert( hour        <   24 );
	assert( minute      <   60 );
	assert( second      <   60 );
	assert( millisecond < 1000 );
 
	char millisecond_string[4];
	::sprintf( millisecond_string, "%03d", millisecond ); 


	std::stringstream os;
	os << hour << ":" << minute << ":" << second << "." << millisecond_string;

	return os.str();
}
