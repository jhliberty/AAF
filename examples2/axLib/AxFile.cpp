//=---------------------------------------------------------------------=
//
// $Id: AxFile.cpp,v 1.7 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "AxFile.h"
#include "AxEx.h"

#include <iostream>

//=---------------------------------------------------------------------=

bool AxFile::isAAFFile( const AxString& name )
{
	aafBool isA;
	aafUID_t filekind;  // don't care

	CHECK_HRESULT( AAFFileIsAAFFile( name.c_str(), &filekind, &isA ) );

	return isA ? true : false;
}
	
AxFile::AxFile()
{}

AxFile::AxFile( IAAFFileSP spIaafFile )
: AxBaseObj( AxQueryInterface<IAAFFile, IUnknown>(spIaafFile) ),
  _spIaafFile( spIaafFile )
{}

AxFile::AxFile( const AxFile& other )
{
	init( other );
}

AxFile::~AxFile()
{}

AxFile&  AxFile::operator=( const AxFile& rhs )
{
	if ( this != &rhs ) {
		init( rhs );
	}

	return *this;
}

void AxFile::OpenNewModify( const AxString& name,
			    aafUInt32 mode,
			    const AxProductIdentification& ident )
{
	IAAFFileSP spIaafFile;

	const wchar_t* cname = name.c_str();

	CHECK_HRESULT( AAFFileOpenNewModify( cname, mode,
					     const_cast<aafProductIdentification_t*>( ident.getProductId() ),
					     &spIaafFile ) );

    _spIaafFile = spIaafFile;
    _name       = name;
    _mode	= mode;

	setIUnknown( _spIaafFile );
}

void AxFile::OpenExistingModify( const AxString& name,
				 aafUInt32 mode,
				 const AxProductIdentification& ident )
{
	IAAFFileSP spIaafFile;

	CHECK_HRESULT( AAFFileOpenExistingModify( name.c_str(), mode,
						  const_cast<aafProductIdentification_t*>( ident.getProductId() ),
						  &spIaafFile ) );

    _spIaafFile = spIaafFile;
    _name       = name;
    _mode	= mode;

	setIUnknown( _spIaafFile );
}

void AxFile::OpenExistingRead( const AxString& name,
			       aafUInt32 mode )
{
	IAAFFileSP spIaafFile;

	CHECK_HRESULT( AAFFileOpenExistingRead( name.c_str(), mode, &spIaafFile ) );

	_spIaafFile = spIaafFile;
	_name = name;
	_mode = mode;

	setIUnknown( _spIaafFile );
}

void AxFile::OpenTransient( const AxProductIdentification& ident )
{
	IAAFFileSP spIaafFile;

	CHECK_HRESULT( AAFFileOpenTransient( const_cast<aafProductIdentification_t*>(ident.getProductId()),
										 &spIaafFile ) );

	_spIaafFile = spIaafFile;
	_name = L"unnamed transient";
	_mode = 0;
	
	setIUnknown( spIaafFile );
}

void AxFile::Save()
{
	CHECK_HRESULT( _spIaafFile->Save() );
}

void AxFile::SaveCopyAs( IAAFFileSP spDstFile )
{
	CHECK_HRESULT( _spIaafFile->SaveCopyAs( spDstFile ) );
}

void AxFile::Close()
{
	CHECK_HRESULT( _spIaafFile->Close() );
}

const AxString& AxFile::getName() const
{
	return _name;
}

IAAFHeaderSP AxFile::getHeader() const
{
	IAAFHeaderSP spIaafHeader;

	CHECK_HRESULT( _spIaafFile->GetHeader( &spIaafHeader ) );
	
	return spIaafHeader;
}

std::wostream& AxFile::dump( std::wostream& os ) const
{
	using namespace std;


	os << L"AxFile:" << endl;
	os << L"\t" << _name << endl;

	return os;
}

void AxFile::init( const AxFile& other )
{
	_name = other._name;
	_mode = other._mode;
	_spIaafFile = other._spIaafFile;

	setIUnknown( _spIaafFile );
}

std::wostream& operator<<( std::wostream& os, AxFile& axFile )
{	
	return axFile.dump( os );
}

//=---------------------------------------------------------------------=

AxRandomFile::AxRandomFile( IAAFRandomFileSP spIaafRandomFile )
:	AxFile( AxQueryInterface<IAAFRandomFile,IAAFFile>(spIaafRandomFile) ),
	_spIaafRandomFile( spIaafRandomFile )
{}

AxRandomFile::~AxRandomFile()
{}

void AxRandomFile::SaveAsFile( IAAFFileSP spIaafFile )
{
	CHECK_HRESULT( _spIaafRandomFile->SaveAsFile( spIaafFile ) );
}
