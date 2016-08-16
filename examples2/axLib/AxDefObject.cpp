//=---------------------------------------------------------------------=
//
// $Id: AxDefObject.cpp,v 1.8 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include <AxDefObject.h>

//=---------------------------------------------------------------------=

AxDefObject::AxDefObject( IAAFDefObjectSP spIaafDefObject )
:	AxObject( AxQueryInterface<IAAFDefObject, IAAFObject>(spIaafDefObject) ),
	_spIaafDefObject( spIaafDefObject )
{}

AxDefObject::~AxDefObject()
{}

aafUID_t AxDefObject::GetAUID()
{
	aafUID_t uid;

	CHECK_HRESULT( _spIaafDefObject->GetAUID( &uid ) );

	return uid;
}

AxString AxDefObject::GetName()
{
	return AxNameToString( _spIaafDefObject );
}

AxString AxDefObject::GetDescription()
{
	return AxDescriptionToString( _spIaafDefObject );
}


//=---------------------------------------------------------------------=

AxContainerDef::AxContainerDef( IAAFContainerDefSP spIaafContainerDef )
  :	AxDefObject( AxQueryInterface<IAAFContainerDef, IAAFDefObject>(spIaafContainerDef) ),
	_spIaafContainerDef( spIaafContainerDef )
{}

AxContainerDef::~AxContainerDef()
{}

void AxContainerDef::Initialize( const aafUID_t& uid,
				 const AxString& name,
				 const AxString& desc )
{
  CHECK_HRESULT( _spIaafContainerDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

//=---------------------------------------------------------------------=

AxInterpolationDef::AxInterpolationDef( IAAFInterpolationDefSP spIaafInterpolationDef )
  :	AxDefObject( AxQueryInterface<IAAFInterpolationDef, IAAFDefObject>(spIaafInterpolationDef) ),
	_spIaafInterpolationDef( spIaafInterpolationDef )
{}

AxInterpolationDef::~AxInterpolationDef()
{}

void AxInterpolationDef::Initialize( const aafUID_t& uid,
				     const AxString& name,
				     const AxString& desc )
{
  CHECK_HRESULT( _spIaafInterpolationDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

//=---------------------------------------------------------------------=

AxParameterDef::AxParameterDef( IAAFParameterDefSP spIaafParameterDef )
  :	AxDefObject( AxQueryInterface<IAAFParameterDef, IAAFDefObject>(spIaafParameterDef) ),
	_spIaafParameterDef( spIaafParameterDef )
{}

AxParameterDef::~AxParameterDef()
{}

void AxParameterDef::Initialize( const aafUID_t& uid,
				 const AxString& name,
				 const AxString& desc,
				 IAAFTypeDefSP spIaafTypeDef )
{
  CHECK_HRESULT( _spIaafParameterDef->Initialize( uid, name.c_str(), desc.c_str(), spIaafTypeDef ) );
}

IAAFTypeDefSP AxParameterDef::GetTypeDefinition ()
{
  IAAFTypeDefSP spIaafTypeDef;

  CHECK_HRESULT( _spIaafParameterDef->GetTypeDefinition( &spIaafTypeDef ) );

  return spIaafTypeDef;
}

void AxParameterDef::SetDisplayUnits ( const AxString& displayUnits )
{
  CHECK_HRESULT( _spIaafParameterDef->SetDisplayUnits( displayUnits.c_str() ) );
}

AxString AxParameterDef::GetDisplayUnits ()
{
  wchar_t checkName[256];

  CHECK_HRESULT( _spIaafParameterDef->GetDisplayUnits( checkName, sizeof(checkName) ) );

  return AxString(checkName);
}

aafUInt32 AxParameterDef::GetDisplayUnitsBufLen ()
{
  aafUInt32 bufSize;

  CHECK_HRESULT( _spIaafParameterDef->GetDisplayUnitsBufLen( &bufSize ) );

  return bufSize;
}

//=---------------------------------------------------------------------=

AxPluginDef::AxPluginDef( IAAFPluginDefSP spIaafPluginDef )
:	AxDefObject( AxQueryInterface<IAAFPluginDef, IAAFDefObject>(spIaafPluginDef) ),
	_spIaafPluginDef( spIaafPluginDef )
{}

AxPluginDef::~AxPluginDef()
{}

void AxPluginDef::Initialize( const aafUID_t& uid,
							 const AxString& name,
							 const AxString& desc )
{
	CHECK_HRESULT( _spIaafPluginDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxPluginDef::SetCategoryClass( const aafUID_t& uid )
{
	CHECK_HRESULT( _spIaafPluginDef->SetCategoryClass( uid ) );
}

void AxPluginDef::SetPluginVersionString( const AxString& ver )
{
	CHECK_HRESULT( _spIaafPluginDef->SetPluginVersionString( ver.c_str() ) );
}

void AxPluginDef::SetManufacturerID( const aafUID_t& uid )
{
	CHECK_HRESULT( _spIaafPluginDef->SetManufacturerID( uid ) );
}

void AxPluginDef::SetPluginManufacturerName( const AxString& name)
{
	CHECK_HRESULT( _spIaafPluginDef->SetPluginManufacturerName( name.c_str() ) );
}

void AxPluginDef::SetIsSoftwareOnly( bool isSoftware )
{
	CHECK_HRESULT( _spIaafPluginDef->SetIsSoftwareOnly( isSoftware ) );
}

void AxPluginDef::SetIsAccelerated( bool isAccel )
{
	CHECK_HRESULT( _spIaafPluginDef->SetIsAccelerated( isAccel ) );
}

void AxPluginDef::SetSupportsAuthentication( bool supportsAuth )
{
	CHECK_HRESULT( _spIaafPluginDef->SetSupportsAuthentication( supportsAuth ) );
}

void AxPluginDef::SetManufacturerInfo( IAAFNetworkLocatorSP manuInfo )
{
	CHECK_HRESULT( _spIaafPluginDef->SetManufacturerInfo( manuInfo ) );
}

//=---------------------------------------------------------------------=

AxCodecDef::AxCodecDef( IAAFCodecDefSP spIaafCodecDef )
:	AxDefObject( AxQueryInterface<IAAFCodecDef, IAAFDefObject>(spIaafCodecDef) ),
	_spIaafCodecDef( spIaafCodecDef )
{}

AxCodecDef::~AxCodecDef()
{}

void AxCodecDef::Initialize( const aafUID_t& uid,
							 const AxString& name,
							 const AxString& desc )
{
	CHECK_HRESULT( _spIaafCodecDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxCodecDef::SetFileDescriptorClass( IAAFClassDefSP spClass )
{
	CHECK_HRESULT( _spIaafCodecDef->SetFileDescriptorClass( spClass ) );
}

void AxCodecDef::AddEssenceKind( IAAFDataDefSP spEssenceKind )
{
	CHECK_HRESULT( _spIaafCodecDef->AddEssenceKind( spEssenceKind ) );
}

aafBoolean_t AxCodecDef::IsEssenceKindSupported( IAAFDataDefSP spIaafDataDef )
{
	aafBoolean_t isSupported;

	CHECK_HRESULT( _spIaafCodecDef->IsEssenceKindSupported( spIaafDataDef, &isSupported ) );

	return isSupported;
}

aafUInt32 AxCodecDef::CountEssenceKinds()
{
	aafUInt32 count;

	CHECK_HRESULT( _spIaafCodecDef->CountEssenceKinds( &count ) );

	return count;
}

IEnumAAFDataDefsSP AxCodecDef::GetEssenceKinds()
{
	IEnumAAFDataDefsSP spIEnumDataDefs;

	CHECK_HRESULT( _spIaafCodecDef->GetEssenceKinds( &spIEnumDataDefs ) );

	return spIEnumDataDefs;
}

aafBoolean_t AxCodecDef::AreThereFlavours()
{
	aafBoolean_t areThereFlavours;

	CHECK_HRESULT( _spIaafCodecDef->AreThereFlavours( &areThereFlavours ) );

	return areThereFlavours;
}

IEnumAAFCodecFlavoursSP AxCodecDef::EnumCodecFlavours()
{
	IEnumAAFCodecFlavoursSP spIEnumCodecFlavours;

	CHECK_HRESULT( _spIaafCodecDef->EnumCodecFlavours( &spIEnumCodecFlavours ) );

	return spIEnumCodecFlavours;
}

//=---------------------------------------------------------------------=

AxDataDef::AxDataDef( IAAFDataDefSP spIaafDataDef )
:	AxDefObject( AxQueryInterface<IAAFDataDef, IAAFDefObject>(spIaafDataDef) ),
	_spIaafDataDef( AxQueryInterface<IAAFDataDef,IAAFDataDef3>( spIaafDataDef ) )
{}

AxDataDef::AxDataDef( IAAFDataDef2SP spIaafDataDef )
:   AxDefObject( AxQueryInterface<IAAFDataDef2, IAAFDefObject>(spIaafDataDef) ),
    _spIaafDataDef( AxQueryInterface<IAAFDataDef2,IAAFDataDef3>( spIaafDataDef ) )
{}

AxDataDef::AxDataDef( IAAFDataDef3SP spIaafDataDef )
:   AxDefObject( AxQueryInterface<IAAFDataDef3, IAAFDefObject>(spIaafDataDef) ),
    _spIaafDataDef( spIaafDataDef )
{}

AxDataDef::~AxDataDef()
{}

void AxDataDef::Initialize( const aafUID_t& uid,
                 const AxString& name,
                 const AxString& desc )
{
    CHECK_HRESULT( _spIaafDataDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

bool AxDataDef::IsSoundKind()
{
	aafBoolean_t isSoundKind;

	CHECK_HRESULT( _spIaafDataDef->IsSoundKind( &isSoundKind ) );

	return isSoundKind ? true : false;
}

bool AxDataDef::IsPictureKind()
{
	aafBoolean_t isPictureKind;

	CHECK_HRESULT( _spIaafDataDef->IsPictureKind( &isPictureKind ) );

	return isPictureKind ? true : false;
}

bool AxDataDef::IsMatteKind()
{
    aafBoolean_t isMatteKind;

    CHECK_HRESULT( _spIaafDataDef->IsMatteKind( &isMatteKind ) );

    return isMatteKind ? true : false;
}

bool AxDataDef::IsPictureWithMatteKind()
{
    aafBoolean_t isPictureWithMatteKind;

    CHECK_HRESULT( _spIaafDataDef->IsPictureWithMatteKind( &isPictureWithMatteKind ) );

    return isPictureWithMatteKind ? true : false;
}

bool AxDataDef::IsEdgecodeKind()
{
    aafBoolean_t isEdgecodeKind;

    CHECK_HRESULT( _spIaafDataDef->IsEdgecodeKind( &isEdgecodeKind ) );

    return isEdgecodeKind ? true : false;
}

bool AxDataDef::IsTimecodeKind()
{
    aafBoolean_t isTimecodeKind;

    CHECK_HRESULT( _spIaafDataDef->IsTimecodeKind( &isTimecodeKind ) );

    return isTimecodeKind ? true : false;
}

bool AxDataDef::IsAuxiliaryKind()
{
    aafBoolean_t isAuxiliaryKind;

    CHECK_HRESULT( _spIaafDataDef->IsAuxiliaryKind( &isAuxiliaryKind ) );

    return isAuxiliaryKind ? true : false;
}

bool AxDataDef::IsDescriptiveMetadataKind()
{
    aafBoolean_t isDescriptiveMetadataKind;

    CHECK_HRESULT( _spIaafDataDef->IsDescriptiveMetadataKind( &isDescriptiveMetadataKind ) );

    return isDescriptiveMetadataKind ? true : false;
}

bool AxDataDef::DoesDataDefConvertTo ( IAAFDataDefSP id )
{
    aafBoolean_t doesConvertTo;

    CHECK_HRESULT( _spIaafDataDef->DoesDataDefConvertTo( id, &doesConvertTo ) );
    
    return doesConvertTo ? true : false;
}

bool AxDataDef::IsDataDefOf ( IAAFDataDefSP dataDef )
{
    aafBoolean_t isDataDefOf;
    
    CHECK_HRESULT( _spIaafDataDef->IsDataDefOf( dataDef, &isDataDefOf ) );
    
    return isDataDefOf ? true : false;
}

bool AxDataDef::DoesDataDefConvertFrom ( IAAFDataDefSP dataDef )
{
    aafBoolean_t doesConvertFrom;
    
    CHECK_HRESULT( _spIaafDataDef->DoesDataDefConvertFrom( dataDef, &doesConvertFrom ) );
    
    return doesConvertFrom ? true : false;
}

//=---------------------------------------------------------------------=

AxOperationDef::AxOperationDef( IAAFOperationDefSP spIaafOperationDef )
:	AxDefObject( AxQueryInterface<IAAFOperationDef, IAAFDefObject>(spIaafOperationDef) ),
	_spIaafOperationDef( spIaafOperationDef )
{}

AxOperationDef::~AxOperationDef()
{}

void AxOperationDef::Initialize( const aafUID_t& uid,
				 const AxString& name,
				 const AxString& desc )
{
	CHECK_HRESULT( _spIaafOperationDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxOperationDef::SetDataDef( IAAFDataDefSP spIaafDataDef  )
{
	CHECK_HRESULT( _spIaafOperationDef->SetDataDef( spIaafDataDef ) );
}

void AxOperationDef::SetIsTimeWarp( aafBoolean_t isWarp )
{
	CHECK_HRESULT( _spIaafOperationDef->SetIsTimeWarp( isWarp ) );
}

void AxOperationDef::SetCategory( const aafUID_t& category_auid )
{
	CHECK_HRESULT( _spIaafOperationDef->SetCategory( category_auid ) );
}

void AxOperationDef::SetNumberInputs( aafInt32 numInputs )
{
	CHECK_HRESULT( _spIaafOperationDef->SetNumberInputs( numInputs ) );
}

void AxOperationDef::SetBypass( aafUInt32 bypassTrack )
{
	CHECK_HRESULT( _spIaafOperationDef->SetBypass( bypassTrack ) );
}

IAAFDataDefSP AxOperationDef::GetDataDef()
{
  IAAFDataDefSP spDataDef;
  CHECK_HRESULT( _spIaafOperationDef->GetDataDef( &spDataDef ) );
  return spDataDef;
}

aafBoolean_t AxOperationDef::IsTimeWarp()
{
  aafBoolean_t isTimeWarp;
  CHECK_HRESULT( _spIaafOperationDef->IsTimeWarp( &isTimeWarp ) );
  return isTimeWarp;
}

aafUID_t AxOperationDef::GetCategory()
{
  aafUID_t uid;
  CHECK_HRESULT( _spIaafOperationDef->GetCategory( &uid ) );
  return uid;
}

aafInt32 AxOperationDef::GetNumberInputs()
{
  aafInt32 numInputs;
  CHECK_HRESULT( _spIaafOperationDef->GetNumberInputs( &numInputs ) );
  return numInputs;
}

aafUInt32 AxOperationDef::GetBypass()
{
  aafUInt32 bypass;
  CHECK_HRESULT( _spIaafOperationDef->GetBypass( &bypass ) );
  return bypass;
}

//=---------------------------------------------------------------------=

AxKLVDataDef::AxKLVDataDef( IAAFKLVDataDefinitionSP spIaafKLVDataDef )
:   AxDefObject( AxQueryInterface<IAAFKLVDataDefinition, IAAFDefObject>(spIaafKLVDataDef) ),
    _spIaafKLVDataDef( spIaafKLVDataDef )
{}

AxKLVDataDef::~AxKLVDataDef()
{}

void AxKLVDataDef::Initialize( const aafUID_t& uid,
                 const AxString& name,
                 const AxString& desc )
{
    CHECK_HRESULT( _spIaafKLVDataDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxKLVDataDef::AddParentProperty( IAAFPropertyDefSP parentProperty )
{
    CHECK_HRESULT( _spIaafKLVDataDef->AddParentProperty( parentProperty ) );
}

void AxKLVDataDef::RemoveParentProperty( IAAFPropertyDefSP parentProperty )
{
    CHECK_HRESULT( _spIaafKLVDataDef->RemoveParentProperty( parentProperty ) );
}

void AxKLVDataDef::SetKLVDataType( IAAFTypeDefSP typeDef )
{
    CHECK_HRESULT( _spIaafKLVDataDef->SetKLVDataType( typeDef ) );
}

IEnumAAFPropertyDefsSP AxKLVDataDef::GetParentProperties()
{
  IEnumAAFPropertyDefsSP parentProperties;
  CHECK_HRESULT( _spIaafKLVDataDef->GetParentProperties( &parentProperties ) );
  return parentProperties;
}

aafUInt32 AxKLVDataDef::CountParentProperties()
{
  aafUInt32 numProperties;
  CHECK_HRESULT( _spIaafKLVDataDef->CountParentProperties( &numProperties ) );
  return numProperties;
}

IAAFTypeDefSP AxKLVDataDef::GetKLVDataType()
{
  IAAFTypeDefSP klvData;
  CHECK_HRESULT( _spIaafKLVDataDef->GetKLVDataType( &klvData ) );
  return klvData;
}

//=---------------------------------------------------------------------=

AxTaggedValueDef::AxTaggedValueDef( IAAFTaggedValueDefinitionSP spIaafTaggedValueDef )
:   AxDefObject( AxQueryInterface<IAAFTaggedValueDefinition, IAAFDefObject>(spIaafTaggedValueDef) ),
    _spIaafTaggedValueDef( spIaafTaggedValueDef )
{}

AxTaggedValueDef::~AxTaggedValueDef()
{}

void AxTaggedValueDef::Initialize( const aafUID_t& uid,
                 const AxString& name,
                 const AxString& desc )
{
    CHECK_HRESULT( _spIaafTaggedValueDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxTaggedValueDef::AddParentProperty( IAAFPropertyDefSP parentProperty )
{
    CHECK_HRESULT( _spIaafTaggedValueDef->AddParentProperty( parentProperty ) );
}

void AxTaggedValueDef::RemoveParentProperty( IAAFPropertyDefSP parentProperty )
{
    CHECK_HRESULT( _spIaafTaggedValueDef->RemoveParentProperty( parentProperty ) );
}

IEnumAAFPropertyDefsSP AxTaggedValueDef::GetParentProperties()
{
  IEnumAAFPropertyDefsSP parentProperties;
  CHECK_HRESULT( _spIaafTaggedValueDef->GetParentProperties( &parentProperties ) );
  return parentProperties;
}

aafUInt32 AxTaggedValueDef::CountParentProperties()
{
  aafUInt32 numProperties;
  CHECK_HRESULT( _spIaafTaggedValueDef->CountParentProperties( &numProperties ) );
  return numProperties;
}
