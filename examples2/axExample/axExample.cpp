//=---------------------------------------------------------------------=
//
// $Id: axExample.cpp,v 1.9 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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

#include "axExample.h"

#include <AxInit.h>
#include <AxFile.h>
#include <AxEx.h>
#include <AxUtil.h>

#include <iostream>
#include <sstream>

#include <AAFResult.h>

//=---------------------------------------------------------------------

void throwUsage()
{
	using namespace std;
	wostringstream msg;
	msg << L"Usage:\t" << L"-file filename [-metadata] [-essence] [-composition]" << endl;
	msg << L"Notes:\t" << L"Run -essence before composition.  Run -metadata before -essence." << endl;
	throw AxEx( msg.str().c_str() );
}

int main( int argc, char* argv[] )
{
	using namespace std;
	
    try {

		AxCmdLineArgs args( argc, argv );

		pair<bool, int> fileOpArg = args.get( "-file" );
		if ( !fileOpArg.first ) {
			throwUsage();
		}

		pair<bool, const char*> fileNameArg = args.get( fileOpArg.second + 1 );
		if ( !fileNameArg.first ) {
			throwUsage();
		}

		AxInit initObj;
				
		AxString fileName( AxStringUtil::mbtowc( fileNameArg.second ) );

		bool openExisting = false;
		
		// Test if left over file from previous test exists.

		// FIXME - logic implementted using an exception.  Fix AxFile
		// interface, or add separate file io code to determine
		// if a file exists before testing if it is an AAF file.
		try {
			if ( AxFile::isAAFFile( fileName ) ) {
				wostringstream msg;
				msg << L"Existing file will be modified: ";
				msg << fileName << endl;
				openExisting = true;
			}
		}
		catch( const AxExHResult& ex ) {
			//  AAFRESULT_FILE_NOT_FOUND is okay.  The file does
			// not exist.
			if ( ex.getHResult() != AAFRESULT_FILE_NOT_FOUND ) {
				throw;
			}
		}

		AxFile axFile;

		if ( openExisting ) {
			wcout << L"Open existing AAF File: " << fileName << endl;
			axFile.OpenExistingModify( fileName );
		}
		else {
			wcout << L"Create new AAF File: " << fileName << endl;
			axFile.OpenNewModify( fileName );
		}


		// Test each option argument and run the appropriate module.
		// Each example module takes only the file as an argument,
		// hence, these option can be run once at a time or all at
		// once.

		pair<bool,int> metaOpArg = args.get( "-metadata" );
		if ( metaOpArg.first ) {
			wcout << L"Creating metadata..." << endl;
			AxCreateMetaDataExample( axFile, args );
		}

		pair<bool,int> essOpArg = args.get( "-essence" );
		if ( essOpArg.first ) {
			wcout << L"Creating essence..." << endl;
			AxCreateEssenceExample( axFile, args );
		}

		pair<bool,int> compOpArg = args.get( "-composition" );
		if ( compOpArg.first ) {
			wcout << L"Creating composition..." << endl;
			AxCreateCompositionExample( axFile, args );
		}

		wcout << L"Complete." << endl;
		
		axFile.Save();
		axFile.Close();
	}

	catch ( const AxEx& ex ) {
		wcout << ex.what() << endl;
	}

	return 0;
}
