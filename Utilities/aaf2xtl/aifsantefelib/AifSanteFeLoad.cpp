//=---------------------------------------------------------------------=
//
// $Id: AifSanteFeLoad.cpp,v 1.4 2009/06/02 07:43:22 stuart_hc Exp $ $Name: V116 $
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

#include <AifSanteFeLoad.h>

#include <AifVersions.h>
#include <AifParseTree.h>
#include <AifParseTreeNodeFactory.h>
#include <AifParseTreeBuilder.h>

#include <AxInit.h>
#include <AxSmartPointer.h>
#include <AxFile.h>
#include <AxHeader.h>
#include <AxMob.h>
#include <AxIterator.h>

#include <AAFExtEnum.h>
#include <AAFSDKBuild.h>
#include <AAFResult.h>

#include <assert.h>

int AifSanteFeLoader::_instanceCount = 0;

AifSanteFeLoader::AifSanteFeLoader(const AxString& fileName,
								   const char* pAafComLib,
								   bool initAAF )
: _fileName( fileName )
{
	if ( initAAF && _instanceCount == 0) {
		std::auto_ptr<AxInit> initObj( new AxInit( pAafComLib ) );
		_initObj = initObj;
	}

	_instanceCount++;

#if AAF2XTL_MODIFY_FILE
	_axFile.OpenExistingModify( _fileName );
#else
	_axFile.OpenExistingRead( _fileName );
#endif
}

AifSanteFeLoader::~AifSanteFeLoader()
{
	_instanceCount--;

#if AAF2XTL_MODIFY_FILE
	_axFile.Save();
	_axFile.Close();
#else
	_axFile.Close();
#endif
}

std::auto_ptr<AifParseTree> AifSanteFeLoader::ParseFile( AifParseTreeNodeFactory& nodeFactory )
{
	AxMob axPrimaryMob( GetPrimaryMob() );
	
	return std::auto_ptr<AifParseTree>( 
		new AifParseTree( AifBuildParseTree( axPrimaryMob, nodeFactory ) ) );
}

IAAFMobSP AifSanteFeLoader::GetPrimaryMob()
{
	const wchar_t* errorMsg = L"No suitable composition mob found.";

	AxHeader axHeader( _axFile.getHeader() );

#if defined(AIF_V110)
	// First, search for the TopLevel composition mob.
	{
		aafSearchCrit_t crit;
		crit.searchTag = kAAFByCompositionMobUsageCode;
		crit.tags.usageCode = kAAFUsage_TopLevel;
		AxMobIter axCompMobIter( axHeader.GetMobs( crit ) );
		IAAFMobSP spMob;
		bool found = axCompMobIter.NextOne( spMob );
		if ( found ) {
			return spMob;
		}
	}
#else
	// Search for a mob named "TopLevel" until V1.1 support is folded in.
	{
		aafSearchCrit_t crit;
		crit.searchTag = kAAFByMobKind;
		crit.tags.mobKind = kAAFCompMob;
		AxMobIter axCompMobIter( axHeader.GetMobs( crit ) );
		IAAFMobSP spMob;
		bool found;
		for ( found = axCompMobIter.NextOne( spMob );
			  found;
			  found = axCompMobIter.NextOne( spMob ) ) {
            AxMob axMob( spMob );
  		    AxString topLevel( L"TopLevel" );
			try {
				if ( topLevel == axMob.GetName() ) {
					return spMob;
				}
			}
			catch( const AxExHResult& ex ) {
				if ( AAFRESULT_PROP_NOT_PRESENT != ex.getHResult() ) {
					throw;
				}
			}
		}
	}


#endif

	// There is no TopLevel composition mob. Check if there is
	// a single composition mob, and if so use it.
	{
		aafSearchCrit_t crit;
		crit.searchTag = kAAFByMobKind;
		crit.tags.mobKind = kAAFCompMob;
		AxMobIter axCompMobIter( axHeader.GetMobs( crit ) );
		IAAFMobSP spMob;
		bool found = axCompMobIter.NextOne( spMob );
		if ( found ) {
			// There should not be another.  If there is, throw an
			// exception because we have no way of determining which,
			// if any, is top level without a complex analysis.
			IAAFMobSP spUnused;
			bool nextFound = axCompMobIter.NextOne( spUnused );
			if ( nextFound ) {
				throw AxEx( errorMsg );
			}
			return spMob;
		}
	}

	// No suitable composition mob was found.  Log an error.
	throw AxEx( errorMsg );
}

IAAFHeaderSP AifSanteFeLoader::GetHeader()
{
	return _axFile.getHeader();
}

