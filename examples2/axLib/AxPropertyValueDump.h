#ifndef __AxPropertyValueDump_h__
#define __AxPropertyValueDump_h__

//=---------------------------------------------------------------------=
//
// $Id: AxPropertyValueDump.h,v 1.3 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#include "AxPropertyValue.h"
  
#include <iosfwd>

class AxPropertyValueDump : public AxPropertyValuePrtcl {

public:

	AxPropertyValueDump( std::wostream& os,
			     bool dumpValueType = true,
			     bool dumpValue = true,
			     const AxString& sep = AxString( L": " ) );

	virtual ~AxPropertyValueDump();

	void SetDumpValue( bool flag );
	void SetDumpValueType( bool flag );

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefCharacterSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIndirectSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIntSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRenameSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefEnumSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefExtEnumSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefFixedArraySP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRecordSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefSetSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStreamSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStringSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStrongObjRefSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefWeakObjRefSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefObjectRefSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefOpaqueSP& );
	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefVariableArraySP& );
	
private:

	AxPropertyValueDump();
	AxPropertyValueDump( const AxPropertyValueDump& );
	AxPropertyValueDump& operator=( const AxPropertyValueDump& );

	std::wostream& _os;

	bool _dumpValue;
	bool _dumpValueType;
	AxString _sep;

	AxString _s;
};


#endif
