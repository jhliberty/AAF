//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefSet.cpp,v 1.33 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFRefSetValue_h__
#include "ImplAAFRefSetValue.h"
#endif

#ifndef __ImplAAFStrongRefSetValue_h__
#include "ImplAAFStrongRefSetValue.h"
#endif

#ifndef __ImplAAFWeakRefSetValue_h__
#include "ImplAAFWeakRefSetValue.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#include "OMStrongRefSetProperty.h"
#include "OMWeakRefSetProperty.h"
#include "OMDataTypes.h"
#include "OMSetProperty.h"
#include "OMTypeVisitor.h"


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFTypeDefObjectRef.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFPropValData.h"

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFStrongRefSetValue;
extern "C" const aafClassID_t CLSID_AAFWeakRefSetValue;
extern "C" const aafClassID_t CLSID_EnumAAFPropertyValues;


AAFRESULT GetSetElementCount( ImplAAFPropertyValue* pVal,
			      ImplAAFTypeDefSet* pDefSet,
			      aafUInt32* pRetVal )
{
  ASSERTU( pVal );
  ASSERTU( pDefSet );
  ASSERTU( pRetVal );

  AAFRESULT hr;

  // Get the type def of the set's contained elements;
  ImplAAFSmartPointer<ImplAAFTypeDef> pElemTypeDef;
  hr = pDefSet->GetElementType( &pElemTypeDef );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  // Get the element's size.
  ASSERTU( pElemTypeDef->IsFixedSize() );
  aafUInt32 elemSize = pElemTypeDef->PropValSize();

  // Get the set size.
  ImplAAFPropValData* pSetValData = dynamic_cast<ImplAAFPropValData*>( pVal );
  aafUInt32 propSize;
  hr = pSetValData->GetBitsSize( &propSize );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  // count is size of set divided by size of single element.
  *pRetVal = propSize/elemSize;

  return AAFRESULT_SUCCESS;
}


ImplAAFTypeDefSet::ImplAAFTypeDefSet () :
_ElementType  ( PID_TypeDefinitionSet_ElementType, 
         L"ElementType", 
         L"/MetaDictionary/TypeDefinitions", 
         PID_MetaDefinition_Identification),
 _uidProperty(NULL),
 _uidType(NULL)
{
  _persistentProperties.put(_ElementType.address());
}


ImplAAFTypeDefSet::~ImplAAFTypeDefSet ()
{}

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::Initialize (
      aafUID_constref  id,
      ImplAAFTypeDef * pTypeDef,
      aafCharacter_constptr  pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (! pTypeName) 
    return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)
    return AAFRESULT_NULL_PARAM;

  ASSERTU (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
    
  //
  // Validate: the given type definition exists in the
  // dictionary. Also, make sure that the associated class
  // definition contains a unique identifier!
  //
  
  // The given type must be in the current dictionary.
  aafUID_t typeID;
  result = pTypeDef->GetAUID(&typeID);
  if (AAFRESULT_FAILED(result))
    return result;
  ImplAAFDictionarySP pDictionary;
  result = GetDictionary(&pDictionary);
  if (AAFRESULT_FAILED(result))
    return result;
  ImplAAFTypeDefSP pType;
  result = pDictionary->LookupTypeDef(typeID, &pType);
  if (AAFRESULT_FAILED(result))
    return result;
  
   
  // The object reference must be for a class that has a unique
  // identifier property. NOTE: The utility GetUIDType performs the validation.
  GetUIDType(pTypeDef, result); // ignore return value (it is NOT reference counted).
  
  result = pvtInitialize(id, pTypeDef, pTypeName);
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatSet;
  return AAFRESULT_SUCCESS;
}

OMType* ImplAAFTypeDefSet::elementType(void) const
{
  ImplAAFTypeDef* result = 0;
  AAFRESULT hr = GetElementType(&result);
  ASSERTU(hr == 0);
  result->ReleaseReference();
  return result->type();
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::pvtInitialize (
                  aafUID_constref  id,
                  ImplAAFTypeDef * pTypeDef,
                  aafCharacter_constptr  pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  if (! pTypeName) 
    return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)
    return AAFRESULT_NULL_PARAM;
  
  
  result = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
  if (AAFRESULT_SUCCEEDED(result))
  {
    _ElementType = pTypeDef;

    // This instance is now fully initialized.
    setInitialized();
  }
  return result;
}

         
ImplAAFTypeDefRecord* STDMETHODCALLTYPE
  ImplAAFTypeDefSet::GetUIDType(ImplAAFTypeDef* pElementType, AAFRESULT& result) const
{
  ImplAAFTypeDefObjectRef  *objRef = NULL;
  result = AAFRESULT_SUCCESS;
  if (NULL != _uidType)
    return _uidType;
  
  if (NULL == pElementType)
  {
    result = AAFRESULT_NULL_PARAM;
    return NULL;
  }

  // JeffB: only allow strong and weak references to objects
  objRef = dynamic_cast<ImplAAFTypeDefObjectRef*>(pElementType);
  if(objRef == NULL)
  {
    result = AAFRESULT_ELEMENT_NOT_OBJECT;
    return NULL;
  }
  
  // The object reference must be for a class that has a unique
  // identifier property.
  ImplAAFClassDefSP pElementClass;
  result = objRef->GetObjectType(&pElementClass);
  if (AAFRESULT_FAILED(result))
    return NULL;
  ImplAAFPropertyDefSP pUIDPropertyDef;
  result = pElementClass->GetUniqueIdentifier(&pUIDPropertyDef);
  if (AAFRESULT_FAILED(result))
    return NULL;

	// TEMPORARY???: There are only a few types of object reference sets supported
	// at this time.
	switch (pUIDPropertyDef->localIdentification())
	{
    case PID_MetaDefinition_Identification:
      break;
    case PID_DefinitionObject_Identification:
      break;
		case PID_Mob_MobID:
			break;
		case PID_EssenceData_MobID:
			break;
			
		default:
			result = AAFRESULT_BAD_TYPE;
			return NULL;
			break;
	}
 
  // Preserve logical const-ness even though this method is bitwise non-const.
  ImplAAFTypeDefSet* pNonConstThis = const_cast<ImplAAFTypeDefSet *>(this);
  
  // Cache the pid for the uid. 
  pNonConstThis->_uidProperty = pUIDPropertyDef;
    
  ImplAAFTypeDefSP pUIDType;
  result = pUIDPropertyDef->GetTypeDef(&pUIDType);
  if (AAFRESULT_FAILED(result))
    return NULL;
  
  // We only support record types for unique identifiers of objects...
  // Preserve logical const-ness even though this method is bitwise non-const.
  pNonConstThis->_uidType = dynamic_cast<ImplAAFTypeDefRecord*>((ImplAAFTypeDef*)pUIDType);
  if (NULL == _uidType)
  {
    result = AAFRESULT_BAD_TYPE; // ??? TODO: Add support fo Renamed record...
    return NULL;
  }
  
  return _uidType;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::GetElementType (
              ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef)
    return AAFRESULT_NULL_PARAM;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  if(_ElementType.isVoid())
    return AAFRESULT_OBJECT_NOT_FOUND;
  
  *ppTypeDef = bootstrapTypeWeakReference(_ElementType);

  ASSERTU (*ppTypeDef);
  
  (*ppTypeDef)->AcquireReference ();
  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::AddElement (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pElementPropertyValue)
{
  if (!pSetPropertyValue || !pElementPropertyValue)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->InsertElement(pElementPropertyValue);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::RemoveElement(
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pElementPropertyValue)
{
  if (!pSetPropertyValue || !pElementPropertyValue)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->RemoveElement(pElementPropertyValue);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}

//****************
// ContainsElement()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::ContainsElement(
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pElementPropertyValue,
      aafBoolean_t*  pContainsElement)
{
  if (!pSetPropertyValue || !pElementPropertyValue || !pContainsElement)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->ContainsElement(pElementPropertyValue, pContainsElement);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::GetCount (
      ImplAAFPropertyValue * pSetPropertyValue,
      aafUInt32 *  pCount)
{
  if (!pSetPropertyValue || !pCount)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->Count(pCount);
  }
  else
  {
    return GetSetElementCount( pSetPropertyValue, this, pCount );
  }
}



//***********************************************************
// CreateKey()
//
AAFRESULT ImplAAFTypeDefSet::CreateKey (
      aafDataBuffer_t  pKeyPtr,
      aafUInt32  length,
      ImplAAFPropertyValue ** ppKey)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pKeyPtr || !ppKey)
    return AAFRESULT_NULL_PARAM;
  if (0 == length)
    return AAFRESULT_INVALID_PARAM;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  // Use the type of the unique identifier to create the key
  // property value.
  ImplAAFTypeDefSP pElementType;
  result = GetElementType(&pElementType);
  if (AAFRESULT_FAILED(result))
    return result;
  ImplAAFTypeDefRecord* pUIDType = GetUIDType(pElementType, result);
  if (AAFRESULT_FAILED(result))
    return result;
  result = pUIDType->CreateValueFromStruct(pKeyPtr, length, ppKey);

  return result;
}


//***********************************************************
// LookupElement()
//
AAFRESULT ImplAAFTypeDefSet::LookupElement (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pKey,
      ImplAAFPropertyValue ** ppElementPropertyValue)
{
  if (!pSetPropertyValue || !pKey || !ppElementPropertyValue)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->LookupElement(pKey, ppElementPropertyValue);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}

//***********************************************************
// ContainsKey()
//
AAFRESULT ImplAAFTypeDefSet::ContainsKey (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pKey,
      aafBoolean_t*  pContainsKey)
{
  if (!pSetPropertyValue || !pKey || !pContainsKey)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->ContainsKey(pKey, pContainsKey);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}


AAFRESULT
ImplAAFTypeDefSet::GetElements (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplEnumAAFPropertyValues ** ppEnum)
{
  if (!pSetPropertyValue || !ppEnum)
    return AAFRESULT_NULL_PARAM;

  // Get the property value's embedded type and 
  // check if it's the same as the base type.
  ImplAAFTypeDefSP pInPropType;
  if( AAFRESULT_FAILED( pSetPropertyValue->GetType( &pInPropType ) ) )
	return AAFRESULT_BAD_TYPE;
  ASSERTU (pInPropType);
  if( (ImplAAFTypeDef *)pInPropType != this )
	return AAFRESULT_BAD_TYPE;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  // Get the elements if the set property value is a object reference set.
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->GetElements(ppEnum);
  }
  else
  {
    ImplEnumAAFPropertyValues* pEnumPropVals = (ImplEnumAAFPropertyValues*)CreateImpl(CLSID_EnumAAFPropertyValues);
    if ( !pEnumPropVals ) {
      return AAFRESULT_NOMEMORY;
    }

    AAFRESULT hr = pEnumPropVals->Initialize( this, pSetPropertyValue );
    if ( AAFRESULT_SUCCESS == hr ) {
      pEnumPropVals->AcquireReference();
      *ppEnum = pEnumPropVals;
    }

    pEnumPropVals->ReleaseReference();
    pEnumPropVals = NULL;

    return hr;
  }
}


bool ImplAAFTypeDefSet::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefSet::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefSet::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefSet::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefSet::IsStringable () const
{ return false; }




OMProperty * ImplAAFTypeDefSet::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  ASSERTU (name);

  ASSERTU (isInitialized());
  if (!isInitialized())
    return NULL;

  ImplAAFTypeDefSP ptd;
  AAFRESULT rc = (const_cast<ImplAAFTypeDefSet*>(this))->GetElementType(&ptd);
	if (AAFRESULT_FAILED(rc))
		return NULL;
  ASSERTU (ptd);

  


  OMProperty * result = 0;
  ImplAAFTypeDefWeakObjRef *pWeakRefType = NULL;

  if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is strong ref
	  //

	  // TODO: Need a hook to complete the initialization of objects
	  // after they are restored. There is no hook to initialize non-persistent
	  // members.
	  if (NULL == _uidType || NULL == _uidProperty)
	  {
		AAFRESULT status = AAFRESULT_SUCCESS;
		GetUIDType(ptd, status);
		if (AAFRESULT_FAILED(status))
		  return NULL;
		ASSERTU (_uidType && _uidProperty);
	  }

		// TEMPORARY???: There are only a few types of object reference sets supported
		// at this time.
		switch (_uidProperty->localIdentification())
		{
//	    case PID_MetaDefinition_Identification:
//	  		return new OMStrongReferenceSetProperty<OMObjectIdentification, ImplAAFMetaDefinition> (pid, name, _uidProperty->localIdentification());

	    case PID_DefinitionObject_Identification:
	  		return new OMStrongReferenceSetProperty<OMObjectIdentification, ImplAAFDefObject> (pid, name, _uidProperty->localIdentification());

//			case PID_Mob_MobID:
//				return new OMStrongReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFMob> (pid, name, _uidProperty->localIdentification());

//			case PID_EssenceData_MobID:
//				return new OMStrongReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFEssenceData> (pid, name, _uidProperty->localIdentification());
		}

		return NULL;
	}
  else if (NULL != (pWeakRefType = dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) ptd)))
	{
    if (pWeakRefType->GetTargetPids())
    {
      
      switch (pWeakRefType->GetUniqueIdentifierPid())
      {
        case PID_MetaDefinition_Identification:
          result = new OMWeakReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFMetaDefinition>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
      
        case PID_DefinitionObject_Identification:
          result = new OMWeakReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFDefObject>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;
    
        case PID_Mob_MobID:
          result = new OMWeakReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFMob>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;

        case PID_EssenceData_MobID:
          result = new OMWeakReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFEssenceData>(pid, name, pWeakRefType->GetUniqueIdentifierPid(), pWeakRefType->GetTargetPids());
          break;

        default:
          // No support for other "key properties"
          ASSERTU (0);
          break;
      }
    }

  }
  else
  {
    ASSERTU (ptd->IsFixedSize());

    const aafUInt32 elemSize = ptd->NativeSize();
    switch (elemSize)
    {
      case 1:
        result = new OMSetProperty<aafUInt8> (pid, name);
        break;
      case 2:
        result = new OMSetProperty<aafUInt16> (pid, name);
        break;
      case 4:
        result = new OMSetProperty<aafUInt32> (pid, name);
        break;
      case 8:
        result = new OMSetProperty<aafUInt64> (pid, name);
        break;
      case 16:
        result = new OMSetProperty<aafUID_t> (pid, name);
        break;
      default:
        // bad type
        break;
    }
  }

  ASSERTU (result);
  return result;
}





// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefSet::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ASSERTU (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  
  OMReferenceSetProperty* pReferenceSetProperty = dynamic_cast<OMReferenceSetProperty*>(property);
  if (NULL != pReferenceSetProperty)
  {
    ASSERTU (property->definition());
    if (NULL == property->definition())
      return AAFRESULT_INVALID_PARAM;
    const OMType *type = property->definition()->type();
    ASSERTU (type);
    ImplAAFTypeDefSet *ptd = const_cast<ImplAAFTypeDefSet *> (dynamic_cast<const ImplAAFTypeDefSet *>(type));
    ASSERTU (ptd);
    if (NULL == ptd)
      return AAFRESULT_INVALID_PARAM;
      
    ImplAAFTypeDefSP pElementType;
    result = GetElementType(&pElementType);
    if (AAFRESULT_FAILED(result))
      return result;
      
    if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) pElementType))
    {
      // element is strong ref
      ImplAAFStrongRefSetValue* pStrongRefSet = NULL;
      pStrongRefSet = (ImplAAFStrongRefSetValue*) CreateImpl (CLSID_AAFStrongRefSetValue);
      if (!pStrongRefSet) 
        return AAFRESULT_NOMEMORY;
      result = pStrongRefSet->Initialize(this, property);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPropertyValue = pStrongRefSet;
      }
      else
      {
        pStrongRefSet->ReleaseReference();
      }
    }
    else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) pElementType))
    {
      // element is weak ref
      ImplAAFWeakRefSetValue* pWeakRefSet = NULL;
      pWeakRefSet = (ImplAAFWeakRefSetValue*) CreateImpl (CLSID_AAFWeakRefSetValue);
      if (!pWeakRefSet) 
        return AAFRESULT_NOMEMORY;
      result = pWeakRefSet->Initialize(this, property);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPropertyValue = pWeakRefSet;
      }
      else
      {
        pWeakRefSet->ReleaseReference();
      }
    }
    else
    { 
      ASSERTU (NULL != *ppPropertyValue);     
      return AAFRESULT_INVALID_PARAM;

      // TEMPORARY HACK!
      // Weak reference arrays are still implemented as an array of records!
      
      // If the property is not a reference vector then use the "old" method
      // for creating a variable array property value.
      //      result = ImplAAFTypeDef::CreatePropertyValue(property, ppPropertyValue);
    }
  }
  else
  {
    // If the property is not a reference vector then use the "old" method
    // for creating a variable array property value.
    result = ImplAAFTypeDef::CreatePropertyValue(property, ppPropertyValue);
  }
  return result;
}




// override from OMStorable.
const OMClassId& ImplAAFTypeDefSet::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefSet));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefSet::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefSet::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

const OMUniqueObjectIdentification& ImplAAFTypeDefSet::identification(void) const
{
  // Re-implement pure virtual method inherited from OMSetType
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDefSet::name(void) const
{
  // Re-implement pure virtual method inherited from OMSetType
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDefSet::hasDescription(void) const
{
  // Re-implement pure virtual method inherited from OMSetType
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDefSet::description(void) const
{
  // Re-implement pure virtual method inherited from OMSetType
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDefSet::isPredefined(void) const
{
  // Re-implement pure virtual method inherited from OMSetType
  return ImplAAFMetaDefinition::isPredefined();
}

bool ImplAAFTypeDefSet::isFixedSize(void) const
{
  bool result = false;
  if (IsFixedSize() == kAAFTrue) {
    result = true;
  }
  return result;
}

void ImplAAFTypeDefSet::reorder(OMByte* /*bytes*/,
							 OMUInt32 /*bytesSize*/) const
{
  // Should be properly implemented
  ASSERTU (0);
}

OMUInt32 ImplAAFTypeDefSet::externalSize(const OMByte* /*internalBytes*/,
									OMUInt32 /*internalBytesSize*/) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}

OMUInt32 ImplAAFTypeDefSet::externalSize(void) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}

void ImplAAFTypeDefSet::externalize(const OMByte* /*internalBytes*/,
								 OMUInt32 /*internalBytesSize*/,
								 OMByte* /*externalBytes*/,
								 OMUInt32 /*externalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  // Should be properly implemented
  ASSERTU (0);
}

OMUInt32 ImplAAFTypeDefSet::internalSize(const OMByte* /*externalBytes*/,
									OMUInt32 /*externalSize*/) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}

OMUInt32 ImplAAFTypeDefSet::internalSize(void) const
{
  // Should be properly implemented
  ASSERTU (0);
  return 0; // Not reached!
}

void ImplAAFTypeDefSet::internalize(const OMByte* /*externalBytes*/,
								 OMUInt32 /*externalBytesSize*/,
								 OMByte* /*internalBytes*/,
								 OMUInt32 /*internalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  // Should be properly implemented
  ASSERTU (0);
}

void ImplAAFTypeDefSet::accept(OMTypeVisitor& visitor) const
{
  visitor.visitSetType(this);
  elementType()->accept(visitor);
}

// Method is called after associated class has been added to MetaDictionary.
// If this method fails the class is removed from the MetaDictionary and the
// registration method will fail.
HRESULT ImplAAFTypeDefSet::CompleteClassRegistration(void)
{
  ImplAAFTypeDefSP pElementType;
  AAFRESULT rc = GetElementType(&pElementType);
  if (AAFRESULT_SUCCEEDED(rc))
  {
    rc = pElementType->CompleteClassRegistration();
  }

  return rc;
}
