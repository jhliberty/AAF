//=---------------------------------------------------------------------=
//
// $Id: FileOp.cpp,v 1.17 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

#include <MultiGenTest.h>

#include "MultiGenCommon.h"

#include <AAFDefUIDs.h>
#include <AAFFileKinds.h>

#include "../../../../ref-impl/include/AAFSDKBuild.h"

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>

#include <stdlib.h>
#include <assert.h>

namespace {

class KindMap {
public:
  KindMap()
  {
#if AAF_MAJOR_VERSION == 1 && AAF_MINOR_VERSION == 0 && AAF_MAINT_RELEASE == 2

    #define ADD_KIND( X ) \
    _kindMap[ string( #X ) ] = aafFileKindAaf##X;

    ADD_KIND( MSSBinary );
    ADD_KIND( SSSBinary );
    ADD_KIND( M4KBinary );
    ADD_KIND( S4KBinary );
    
#elif AAF_MAJOR_VERSION >= 1 && AAF_MINOR_VERSION >= 0

    #define ADD_KIND( X ) \
    _kindMap[ string( #X ) ] = kAAFFileKind_Aaf##X;

    ADD_KIND( M512Binary );
    ADD_KIND( S512Binary );
    ADD_KIND( M4KBinary );
    ADD_KIND( S4KBinary )

#else
#error unsupported version
#endif
  }

  ~KindMap()
  {}


  aafUID_t GetKind( const char* kindName )
  {
    std::map<std::string, aafUID_t >::const_iterator iter;
    iter = _kindMap.find( kindName );

    if ( iter == _kindMap.end() ) {
      std::stringstream what;
      what << "File kind unknown: " << kindName;
      throw UsageEx( what.str().c_str() );
    }

    return iter->second;
  }

private:
  std::map< std::string, aafUID_t > _kindMap;
};


class FileOp : public MultiGenTest
{ 
public:
  FileOp()
    : MultiGenTest()
  {}

  virtual ~FileOp()
  {}

  virtual void RunTest( CmdState& state, int argc, char** argv );
};

IAAFSmartPointer<IAAFFile> CreateFileOfKind( const std::string& fileName,
					     aafFileExistence_e existence,
					     aafFileAccess_e access,
					     const aafUID_t& fileKind,
					     const aafProductIdentification_t& prodId )
#if 1
{
  std::auto_ptr<wchar_t> wfileName( ToWideString( fileName.c_str() ) );

  IAAFSmartPointer<IAAFRawStorage> spRawStorage;
  CHECK_HRESULT( AAFCreateRawStorageDisk( wfileName.get(),
					  existence,
					  access,
					  &spRawStorage) );
  
  IAAFSmartPointer<IAAFFile> spFile;
  CHECK_HRESULT( AAFCreateAAFFileOnRawStorage( spRawStorage,
					       existence,
					       access,
					       &fileKind,
					       0,
					       &prodId,
					       &spFile));

  CHECK_HRESULT( spFile->Open() );

  return spFile;
}
#else
{
  std::auto_ptr<wchar_t> wfileName( ToWideString( fileName.c_str() ) );

  IAAFSmartPointer<IAAFFile> spFile;

  if ( kAAFFileExistence_existing == existence ) {
    if ( kAAFFileAccess_read == access )
    {
      CHECK_HRESULT( AAFFileOpenExistingRead( wfileName.get(), 0, &spFile ) );      
    }
    else
    {
      assert( kAAFFileAccess_modify == access );
      CHECK_HRESULT( AAFFileOpenExistingModify( wfileName.get(), 0,
						const_cast<aafProductIdentification_t*>(&prodId),
						&spFile ) );
    }
  }
  else {
    CHECK_HRESULT( AAFFileOpenNewModify( wfileName.get(), 0,
					 const_cast<aafProductIdentification_t*>(&prodId),
					 &spFile ) );
  }

  return spFile;
}
#endif

static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"FileOp";

void FileOp::RunTest( CmdState& state, int argc, char** argv )
{
  using namespace std;
	
  IAAFSmartPointer<IAAFFile> iaafFile;

  aafProductVersion_t v;
  aafProductIdentification_t	productInfo;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  productInfo.companyName = companyName;
  productInfo.productName = productName;
  productInfo.productVersion = &v;
  productInfo.productVersionString = NULL;
  productInfo.productID = UnitTestProductID;
  productInfo.platform = NULL;

  if ( argc < 2 ) {
    throw UsageEx("FileOp requires least one argument.");
  }

  string which(argv[1]);

  if ( which == "write" || which == "read" || which == "modify" ) {

    if ( argc != 4 ) {
      throw UsageEx("FileOp write|read|modify expects 3 arguments.");
    }

    std::string fileName( argv[3] );
    IAAFSmartPointer<IAAFFile> spFile;
    
    std::string kind( argv[2] );
    
    KindMap kmap;
    aafUID_t fileKind = kmap.GetKind( argv[2] );
    
    aafFileExistence_e existance;
    aafFileAccess_e access;
    
    if ( which == "write" ) {
      existance = kAAFFileExistence_new;
      access = kAAFFileAccess_modify;
    }
    else if ( which == "read" ) {
      existance = kAAFFileExistence_existing;
      access = kAAFFileAccess_read;
    }
    else if ( which == "modify" ) {
      existance = kAAFFileExistence_existing;
      access = kAAFFileAccess_modify;
    }
    else {
      assert(0);
	  return;
    }

    spFile =
      CreateFileOfKind( fileName,
			existance,
			access,
			fileKind,
			productInfo );
    
    state.SetFile( spFile );
  }
  else if ( which == "save" ) {
    CHECK_HRESULT( state.GetFile()->Save() );
  }
  else if ( which == "close" ) {
    CHECK_HRESULT( state.GetFile()->Close() );
  }
  else if ( which == "save_and_close" ) {
    CHECK_HRESULT( state.GetFile()->Save() );
    CHECK_HRESULT( state.GetFile()->Close() );
  }
  else {
    string msg("FileOp option unrecognized: " + which);
    throw UsageEx( msg );
  }
}

StandardFactory<FileOp> factory(
  "FileOp",
  "File Operations",
  "{ (write|read|modify M512Binary|S512Binary|M4KBinary|S4KBinary filename} | save | close | save_and_close }",
  "",
  2, 4
  );

} // end of namespace
