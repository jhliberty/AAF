// @doc INTERNAL
// @com This file implements the AAF Assertion handling utility. 
//=---------------------------------------------------------------------=
//
// $Id: Assertion.cpp,v 1.4 2009/06/01 11:46:48 stuart_hc Exp $ $Name: V116 $
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
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "Assertion.h"

/*******************************************************************
Name:
	Assertion
Description:
	Test the given assertion for a failure condition. If a failure
	occurred, print the given printf() style message to the log (if 
	applicable) and throw an exception.
Returns:
	None.
********************************************************************/

void Assertion::Check( bool assertion, const char *fmt, ... ) 
{
	if( assertion == false )
	{
		if( fmt != 0 && iLogger != 0 )
		{
			va_list args;
			va_start( args, fmt );
			VaList valist( args );
			iLogger->Log( 0, fmt, valist );
			va_end( args );
		}
		throw Assertion( -1 ); // A minus 1 is as good as anything.
	}
	return;
}
