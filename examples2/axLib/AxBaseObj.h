#ifndef __AxBaseObj_h__
#define __AxBaseObj_h__

//=---------------------------------------------------------------------=
//
// $Id: AxBaseObj.h,v 1.5 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "AxEx.h"

#include <AxSmartPointer.h>

// AxBaseObj - the root object in the COM wrapper inheritance
// treel.  A better name would be AxUnknown.
// It exposes the QueryInterface() method because it is an easy
// way to extract a bare pointer to the underlying COM interface.

class AxBaseObj {
public:

	AxBaseObj();

	AxBaseObj( IUnknownSP sp );

	virtual ~AxBaseObj() = 0;

	template <class T>
	void QueryInterface( const IID& iid, T** pPntr )
	{
		CHECK_HRESULT( _spIUnknown->QueryInterface( iid,
						reinterpret_cast<void**>(pPntr) ) );
	}

	template <class T>
	void QueryInterface( T** pPntr )
	{
		T* dummy = 0;
		QueryInterface( AxIID(dummy), pPntr );
	}

	inline operator IUnknownSP ()
	{ return _spIUnknown; }


protected:

	template <class T>
	void setIUnknown( IAAFSmartPointer<T> sp )
	{ 
		AxQueryInterface( sp, _spIUnknown );
	}

private:
	IUnknownSP _spIUnknown;
};


template < class ObjType >
class AxBaseObjAny : public AxBaseObj {
public:
	AxBaseObjAny()
	{}

	AxBaseObjAny( const ObjType& obj )
	: _obj( obj )
	{}

	~AxBaseObjAny()
	{}

	const ObjType& get() const
	{
		return _obj;
	}

	ObjType& get()
	{
		return _obj;
	}

private:
	ObjType _obj;
};

//=---------------------------------------------------------------------=

#endif
