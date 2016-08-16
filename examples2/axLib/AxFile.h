#ifndef __AxFile_h__
#define __AxFile_h__

//=---------------------------------------------------------------------=
//
// $Id: AxFile.h,v 1.7 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxBaseObj.h"

class AxFile : public AxBaseObj {

 public:

	// Is it an aaf file?
    static bool isAAFFile( const AxString& name );
	 
	AxFile();

	AxFile( IAAFFileSP spIaafFile );
	
	AxFile( const AxFile& other );

	virtual ~AxFile();

	AxFile& operator=( const AxFile& rhs );

	void OpenNewModify( const AxString& name,
						aafUInt32 mode = 0,
						const AxProductIdentification& ident = AxProductIdentification() );

	void OpenExistingModify( const AxString& name,
				 aafUInt32 mode = 0,
				 const AxProductIdentification& ident = AxProductIdentification() );

	void OpenExistingRead( const AxString& name,
						   aafUInt32 mode = 0 );
	

	void OpenTransient( const AxProductIdentification& ident = AxProductIdentification() );
	
	void Save();

	void SaveCopyAs( IAAFFileSP spDstFile );

	void Close();

	const AxString& getName() const;

	IAAFHeaderSP getHeader() const;

	inline operator IAAFFileSP ()
	{ return _spIaafFile; }
	
	std::wostream& dump( std::wostream& os ) const;

 private:
  
	void init( const AxFile& other );
	 
	aafUInt32 _mode;
	AxString _name;

	mutable IAAFFileSP _spIaafFile;
};

std::wostream& operator<<( std::wostream& os, const AxFile& file );


class AxRandomFile : public AxFile {
public:
	AxRandomFile( IAAFRandomFileSP spIaafRandomFile );
	virtual ~AxRandomFile();

	void SaveAsFile( IAAFFileSP spIaafFile );

	inline operator IAAFRandomFileSP ()
	{ return _spIaafRandomFile; }

private:

	IAAFRandomFileSP _spIaafRandomFile;
};


#endif
