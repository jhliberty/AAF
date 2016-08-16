//=---------------------------------------------------------------------=
//
// $Id: axPluginInfo.cpp,v 1.10 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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


#include <AxInit.h>
#include <AxFile.h>
#include <AxEx.h>
#include <AxUtil.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxPluginMgr.h>
#include <AxEssence.h>
#include <AxDefObject.h>
#include <AxDefObject.h>
#include <AxIterator.h>

#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

void throwUsage()
{
	wostringstream msg;
	msg << L"Usage:\t" << L"-file filename {-load plugin_library_name}" << endl;
	msg << L"Notes:\t" << L"Specify a file to create, and the name of the plugin to load." << endl;
	throw AxEx ( msg.str().c_str() );
}

void ProcessEssenceFlavours( AxCodecDef& axCodecDef )
{
        using namespace std;

	wcout << L"Essence Flavours:\t";

	if ( axCodecDef.AreThereFlavours() ) {

		AxCodecFlavoursIter axCodecFlavoursIter( axCodecDef.EnumCodecFlavours() );
		bool notAtEnd;
		aafUID_t flavourId;
	
		for( notAtEnd = axCodecFlavoursIter.NextOne( flavourId );
			 notAtEnd;
			 notAtEnd = axCodecFlavoursIter.NextOne( flavourId ) ) {

			wcout << flavourId << " ";
		}

	}
	else {
		wcout << L"none" << endl;
	}

	wcout << endl;
}

void ProcessEssenceKinds( AxCodecDef& axCodecDef )
{
	AxDataDefsIter axDataDefsIter( axCodecDef.GetEssenceKinds() );
	bool notAtEnd;
	IAAFSmartPointer2<IAAFDataDef> spDataDef;

	wcout << L"Essence Kinds:\t\t";
	for( notAtEnd = axDataDefsIter.NextOne( spDataDef );
	     notAtEnd;
	     notAtEnd = axDataDefsIter.NextOne( spDataDef ) ) {

		AxDataDef axDataDef( spDataDef );
		
		if ( axDataDef.IsPictureKind() ) {
			wcout << L"picture "; 
		}
		else if ( axDataDef.IsSoundKind() ) {
			wcout << L"sound "; 
		}
	}

	wcout << endl;
}

void ReportPluginInfo( AxDictionary& axDictionary, AxPluginMgr& axPluginMgr )
{
	// Question - The first argument of EnumLoadedPlugins is a
	// "category".  Indeed, the code even checks to see if the category
	// of the plugin is "kAAFPluginNoCategory" if ID does not match that
	// given.  Why then, is the AUID_AAFCodecDef required?
 	AxLoadedPluginsIter axLoadedPluginsIter(
			axPluginMgr.EnumLoadedPlugins(AUID_AAFCodecDef) );

	aafUID_t nextPluginId;
	bool notAtEnd;
	for( notAtEnd = axLoadedPluginsIter.NextOne( nextPluginId );
		notAtEnd;
		notAtEnd = axLoadedPluginsIter.NextOne( nextPluginId ) ) {
			 
		AxDefObject axDefObject(
		  axPluginMgr.CreatePluginDefinition( nextPluginId, axDictionary ) );

		assert( nextPluginId == axDefObject.GetAUID() );

		AxCodecDef axCodecDef( AxQueryInterface<IAAFDefObject,IAAFCodecDef>( axDefObject ) );
		
		wcout << L"Name:\t\t\t" << axCodecDef.GetName() << endl;
		wcout << L"Desc:\t\t\t" << axCodecDef.GetDescription() << endl;
		wcout << L"Codec Def AUID:\t\t" << axCodecDef.GetAUID() << endl;

		wcout << L"Essence Kind Count:\t" << axCodecDef.CountEssenceKinds() << endl;
		wcout << L"Are there flavours:\t" << AxStringUtil::bool2Str( axCodecDef.AreThereFlavours() ) << endl;

		ProcessEssenceKinds( axCodecDef );

		ProcessEssenceFlavours( axCodecDef );

		wcout << endl;
	}

}

int main ( int argc, char* argv[] ){
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

		pair<bool, int> loadOpArg = args.get( "-load" );
		pair<bool, const char*> loadNameArg;
		if ( loadOpArg.first ) {
			loadNameArg = args.get( loadOpArg.second + 1 );
			if ( !loadNameArg.first ) {
				throwUsage();
			}
		}
	
		// This will load the com library, and the default
		// plugins.
		AxInit initObj;
		
		AxFile axFile;

		AxString fileName( AxStringUtil::mbtowc( fileNameArg.second ) );
		axFile.OpenNewModify( fileName );

		AxHeader axHeader( axFile.getHeader() );
		AxDictionary axDictionary( axHeader.GetDictionary() );
		
		AxPluginMgr axPluginMgr;

		if ( loadOpArg.first ) {
			AxString loadName( AxStringUtil::mbtowc( loadNameArg.second ) );
			axPluginMgr.RegisterPluginFile( loadName );
		}
		
		ReportPluginInfo( axDictionary, axPluginMgr );
		
		axFile.Save();
		axFile.Close();

	}
	catch ( const AxEx& ex ) {
		wcout << ex.what() << endl;
	}

	return 0;
}

