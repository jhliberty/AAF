//=---------------------------------------------------------------------=
//
// $Id: MultiGenCommon.h,v 1.5 2009/06/01 11:47:14 stuart_hc Exp $ $Name: V116 $
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

#include <AAF.h>
#include <AAFSmartPointer.h>

#include <AAFCodecDefs.h>

#include <sstream>

#include "MultiGenTest.h"	// for CmdState class

const int MULTI_GEN_AUDIO_SAMPLE_BPP  =      16;
const int MULTI_GEN_VIDEO_WIDTH       = (720/4);
const int MULTI_GEN_VIDEO_HEIGHT      = (486/4);

const int MULTI_GEN_VIDEO_RATE_NUM = 30000;
const int MULTI_GEN_VIDEO_RATE_DEN =  1001;
const int MULTI_GEN_AUDIO_RATE_NUM = 22500;
const int MULTI_GEN_AUDIO_RATE_DEN =     1;
const int MULTI_GEN_EDIT_RATE_NUM  =   100;
const int MULTI_GEN_EDIT_RATE_DEN  =     1;

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      aafSearchCrit_t* criteria,
		      IAAFSmartPointer<IAAFMob>& mob );

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      IAAFSmartPointer<IAAFMob>& mob );

bool get_mob_by_name( IAAFSmartPointer<IAAFHeader> header,
		      const char* name,
		      IAAFSmartPointer<IAAFMasterMob>& mob );

bool find_comment( IAAFSmartPointer<IAAFMob> mob,
		   const wchar_t* category,
		   const wchar_t* comment,
		   IAAFSmartPointer<IAAFTaggedValue>& taggedValue );

// Throws UsageEx if conversion fails.
unsigned long int ToULong( const char* str );

// Caller takes ownership of returned pointer.
wchar_t* ToWideString( const char* str );

// wide string compare
bool wstrcmp( const wchar_t* a, const wchar_t* b );

template <class T>
inline IUnknown** ToIUnknown( T** p )
{
  return reinterpret_cast<IUnknown**>(p);
}

template <class T>
inline void** ToVoid( T** p )
{
  return reinterpret_cast<void**>(p);
}

template <class MobType>
void get_mob_throw_if_not_found( CmdState& state,
				 const char* name,
				 IAAFSmartPointer<MobType>& mob )
{
  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );
    
  if ( !get_mob_by_name( header, name, mob ) ) {
    std::stringstream anError;
    anError << name << " not found";
    throw TestFailedEx( anError.str() );
  }
}
