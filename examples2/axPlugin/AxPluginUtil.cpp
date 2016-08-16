//=---------------------------------------------------------------------=
//
// $Id: AxPluginUtil.cpp,v 1.9 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include "AxPluginUtil.h"

#include <memory>
#include <algorithm>

namespace {

class MaxIntFunc {
public:
	MaxIntFunc()
		: _val( 1 << (8*sizeof(int) - 1) )
	{}

	~MaxIntFunc()
	{}

	void operator()( AxPluginNameMap::PairType pair )
	{
		int size = pair.second.size();
		if ( size > _val ) {
			_val = size;
		}
	}

	int GetVal()
	{
		return _val;
	}

private:
	int _val;

};

} // end of namespace


//=---------------------------------------------------------------------=

bool AxPluginNameMap::IsFound( const aafUID_t& uid )
{
	if ( find(uid) == end() ) {
		return false;
	}
	else {
		return true;
	}
}


int AxPluginNameMap::GetMaxStringSize()
{
	MaxIntFunc max;

	return std::for_each( begin(), end(), max ).GetVal();
}

int AxPluginNameMap::GetMaxCBufferSize()
{
	return (GetMaxStringSize()+1)*sizeof(CharTraits::char_type);
}
