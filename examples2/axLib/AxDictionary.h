#ifndef __AxDictionary_h__
#define __AxDictionary_h__

//=---------------------------------------------------------------------=
//
// $Id: AxDictionary.h,v 1.23 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxDictionary : public AxObject {

public:
	AxDictionary( IAAFDictionarySP spIaafDictionary );
    AxDictionary( IAAFDictionary2SP spIaafDictionary );
	~AxDictionary();

	bool isKnownTypeDef( const aafUID_t& typeId );
	bool isKnownOperationDef( const aafUID_t& opId );
	bool isKnownParameterDef( const aafUID_t& paramId );
    bool isKnownInterpolationDef( const aafUID_t& intId );
	
	void RegisterClassDef( IAAFClassDefSP spIaafClassDef );

	IAAFTypeDefSP LookupTypeDef( const aafUID_t& typeId );
	IAAFClassDefSP LookupClassDef( const aafUID_t& classId );
	IAAFDataDefSP LookupDataDef( const aafUID_t& dataDefId );
	IAAFOperationDefSP LookupOperationDef( const aafUID_t& opDefId );
	IAAFParameterDefSP LookupParameterDef( const aafUID_t& interpDefId );
	IAAFContainerDefSP LookupContainerDef( const aafUID_t& contDefId );
	IAAFInterpolationDefSP LookupInterpolationDef( const aafUID_t& interpDefId );
	IAAFCodecDefSP LookupCodecDef( const aafUID_t& codecDefId );

	IEnumAAFClassDefsSP GetClassDefs();
	IEnumAAFTypeDefsSP GetTypeDefs();
	IEnumAAFDataDefsSP GetDataDefs();
	IEnumAAFOperationDefsSP GetOperationDefs();
	IEnumAAFParameterDefsSP GetParameterDefs();
    IEnumAAFPluginDefsSP GetPluginDefs();
    IEnumAAFContainerDefsSP GetContainerDefs();
    IEnumAAFInterpolationDefsSP GetInterpolationDefs();
    IEnumAAFCodecDefsSP GetCodecDefs();
    IEnumAAFKLVDataDefsSP GetKLVDataDefs();
    IEnumAAFTaggedValueDefsSP GetTaggedValueDefs();

	void RegisterOpaqueTypeDef( IAAFTypeDefSP );
	void RegisterOperationDef( IAAFOperationDefSP );
	void RegisterParameterDef( IAAFParameterDefSP );
	void RegisterCodecDef( IAAFCodecDefSP );
	void RegisterKLVDataKey( const aafUID_t& uuid,
				 IAAFTypeDefSP spTypeDef );
    void RegisterDataDef( IAAFDataDefSP );
    void RegisterPluginDef( IAAFPluginDefSP );
    void RegisterContainerDef( IAAFContainerDefSP );
    void RegisterInterpolationDef( IAAFInterpolationDefSP );
    void RegisterKLVDataDef( IAAFKLVDataDefinitionSP );
    void RegisterTaggedValueDef( IAAFTaggedValueDefinitionSP );

	IUnknownSP CreateInstance( const aafUID_t& auid,
				  			   const IID& iid );

	
	IUnknownSP CreateMetaInstance( const aafUID_t& auid,
				  				   const IID& iid );

	aafUInt32 CountOpaqueTypeDefs();

	inline operator IAAFDictionarySP ()
	{ return AxQueryInterface<IAAFDictionary2,IAAFDictionary>( _spIaafDictionary ); }
    
    inline operator IAAFDictionary2SP ()
    { return _spIaafDictionary; }

private:
	AxDictionary();
	AxDictionary( const AxDictionary& );
	AxDictionary& operator=( const AxDictionary& );
	IAAFDictionary2SP _spIaafDictionary;
};

//=---------------------------------------------------------------------=


template <class Type>
void AxCreateInstance( AxDictionary& axDict,
	    	           IAAFSmartPointer<Type>& sp )
{
        Type* dummy = 0;

	IUnknownSP spIUnknown = axDict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	AxQueryInterface( spIUnknown, sp );
}

template <class Type>
void AxCreateInstance( IAAFDictionarySP spDict,
	    	           IAAFSmartPointer<Type>& sp )
{
        Type* dummy = 0;

	AxDictionary axDict( spDict );
	IUnknownSP spIUnknown = axDict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	AxQueryInterface( spIUnknown, sp );
}

template <class Type>
IAAFSmartPointer<Type> AxCreateInstance( AxDictionary& dict )
{
	Type* dummy = 0;

	IUnknownSP spIUnknown = dict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp );
	return sp;
}

template <class Type>
IAAFSmartPointer<Type> AxCreateInstance( IAAFDictionarySP spDict )
{
	Type* dummy = 0;
	
	AxDictionary dict(spDict);
	IUnknownSP spIUnknown = dict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp );
	return sp;
}

template <class Type>
IAAFSmartPointer<Type> AxCreateMetaInstance( AxDictionary& dict )
{
	Type* dummy = 0;

	IUnknownSP spIUnknown = dict.CreateMetaInstance( AxAUID(dummy), AxIID(dummy) );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp );
	return sp;
}

#endif
