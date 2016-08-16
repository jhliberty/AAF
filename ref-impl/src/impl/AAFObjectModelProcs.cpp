//=---------------------------------------------------------------------=
//
// $Id: AAFObjectModelProcs.cpp,v 1.12 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#include "AAFObjectModelProcs.h"

#include "AAFObjectModel.h"

// TEMPORARY: Up-cast to dictionary. This will change for the "two-root" meta dictionary.
#include "ImplAAFDictionary.h"

#include "ImplAAFMetaDictionary.h"
#include "ImplAAFClassDef.h"
#include "ImplAAFTypeDefInt.h"
#include "ImplAAFTypeDefEnum.h"
#include "ImplAAFTypeDefVariableArray.h"
#include "ImplAAFTypeDefFixedArray.h"
#include "ImplAAFTypeDefRecord.h"
#include "ImplAAFTypeDefRename.h"
#include "ImplAAFTypeDefCharacter.h"
#include "ImplAAFTypeDefString.h"
//#include "ImplAAFTypeDefExtEnum.h"
#include "ImplAAFTypeDefStrongObjRef.h"
#include "ImplAAFTypeDefWeakObjRef.h"
#include "ImplAAFTypeDefSet.h"
#include "ImplAAFTypeDefStream.h"
#include "ImplAAFTypeDefIndirect.h"
#include "ImplAAFTypeDefOpaque.h"

#include "ImplAAFBaseClassFactory.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyDefs.h"
#include "AAFTypeDefUIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFResult.h"


#include "ImplAAFObject.h"
#include "OMStrongRefProperty.h"
#include "OMStrongRefVectorProperty.h"

#include "OMAssertions.h"






//
// ClassDefinition callbacks
//
static bool CreateClassDefinition(
  const ClassDefinition *classDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFClassDef *pClass = NULL;

  
  if (classDefinition->axiomatic())
  {
    // Attempt to create the assoication instance.
    pClass = static_cast<ImplAAFClassDef *>(metaDictionary->pvtCreateMetaDefinition(AUID_AAFClassDef));
    ASSERTU (pClass);
    if (!pClass)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id and name. All other properties must be initialized
    // after all of the classes have been created.
    result = pClass->SetIdentification(*classDefinition->id());
    if (AAFRESULT_FAILED(result))
    {
      pClass->ReleaseReference();
      pClass = NULL;
      throw result;
    }

    // Save the new axiomatic class definition in a non-persistent set.
    metaDictionary->addAxiomaticClassDefinition(pClass);
    pClass->ReleaseReference();
    
    return true;
  }
  else // if (classDefinition->axiomatic())
  {
    return false;
  }
}

static bool InitializeClassDefinition(
  const ClassDefinition *classDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFClassDef *pClass = NULL;
  ImplAAFClassDef *pParentClass = NULL;
  aafBoolean_t isConcrete;


  if (classDefinition->axiomatic())
  {
    pClass = metaDictionary->findAxiomaticClassDefinition(*classDefinition->id());
    ASSERTU (pClass);
    if (!pClass)
      return false;

    result = pClass->SetName(classDefinition->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));

    // Find and set the parent class definition. (This must also be axiomatic.)
    if (!classDefinition->isRoot())
    {
      pParentClass = metaDictionary->findAxiomaticClassDefinition(*classDefinition->parentId());
      ASSERTU (pParentClass);
      result = pClass->SetBootstrapParent(pParentClass);
      ASSERTU (AAFRESULT_SUCCEEDED(result));
      result = pClass->SetParent(pParentClass);
      ASSERTU (AAFRESULT_SUCCEEDED(result));
    }
    else
    {
      result = pClass->SetBootstrapParent(pClass);
      ASSERTU (AAFRESULT_SUCCEEDED(result));
      result = pClass->SetParent(pClass);
      ASSERTU (AAFRESULT_SUCCEEDED(result));
    }
    if (AAFRESULT_FAILED(result))
      return false;

    // Is this a concrete class.
    isConcrete = (classDefinition->concrete()) ? kAAFTrue : kAAFFalse;
    pClass->pvtSetIsConcrete(isConcrete); // no result
    return true;
  }
  else // if (classDefinition->axiomatic())
  {
    return false;
  }
}

//
// PropertyDefinition/ImplAAFPropertyDef callbacks
//
static bool CreatePropertyDefinition(
  const PropertyDefinition *propertyDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFPropertyDef *pProperty = NULL;
  AAFRESULT result;

  
  if (propertyDefinition->axiomatic())
  {
    // Attempt to create the assoication instance.
    pProperty = static_cast<ImplAAFPropertyDef *>(metaDictionary->pvtCreateMetaDefinition(AUID_AAFPropertyDef));
    ASSERTU (pProperty);
    if (!pProperty)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id and name. All other properties must be initialized
    // after all of the classes and types have been created.
    result = pProperty->SetIdentification(*propertyDefinition->id());
    if (AAFRESULT_FAILED(result))
    {
      pProperty->ReleaseReference();
      pProperty = NULL;
      throw result;
    }

    // Save the new axiomatic property definition in a non-persistent set.
    metaDictionary->addAxiomaticPropertyDefinition(pProperty);
    pProperty->ReleaseReference();
    
    return true;
  }
  else // if (propertyDefinition->axiomatic())
  {
    return false;
  }
}


static bool InitializePropertyDefinition(
  const PropertyDefinition *propertyDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFPropertyDef *pProperty = NULL;
  ImplAAFTypeDef *pType = NULL;
  const TypeDefinition *typeDefinition = NULL;
  ImplAAFClassDef *pClass = NULL;
  AAFRESULT result;

  
  if (propertyDefinition->axiomatic())
  {
    pProperty = metaDictionary->findAxiomaticPropertyDefinition(*propertyDefinition->id());
    ASSERTU (pProperty);

    typeDefinition = propertyDefinition->typeDefinition();
    pType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinition->id());
    ASSERTU (pType);

    pClass = metaDictionary->findAxiomaticClassDefinition(*(propertyDefinition->classId()));
    ASSERTU (pClass);

    result = pProperty->pvtInitialize (
      *propertyDefinition->id(),
      propertyDefinition->pid(),
      propertyDefinition->name(),
      pType,
      (propertyDefinition->required()) ? kAAFFalse : kAAFTrue,
      (propertyDefinition->uid()) ? kAAFTrue : kAAFFalse,
      pClass);
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      pClass->ReleaseReference();
      pClass = NULL;
      throw result;
    }

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinition callbacks
//
static bool CreateTypeDefinition(
  const TypeDefinition *typeDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  const ClassDefinition *classDefinition;
  ImplAAFTypeDef *pType = NULL;
  AAFRESULT result;

  // We cannot create abstract types.
  if (!typeDefinition->concrete())
    return false;

  if (typeDefinition->axiomatic())
  {
    // Attempt to create the assoication type definition instance.
    classDefinition = typeDefinition->classDefinition();
    pType = static_cast<ImplAAFTypeDef *>(metaDictionary->pvtCreateMetaDefinition(*classDefinition->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id. All other properties must be initialized
    // after all of the classes and types have been created.
    result = pType->SetIdentification(*typeDefinition->id());
    if (AAFRESULT_FAILED(result))
    {
      pType->ReleaseReference();
      pType = NULL;
      throw result;
    }

    // Save the new axiomatic type definition in a non-persistent set.
    metaDictionary->addAxiomaticTypeDefinition(pType);
    pType->ReleaseReference();
    
    return true;
  }
  else
  {
    return false;
  }
}



//
// TypeDefinitionInteger/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionInteger(
  const TypeDefinitionInteger *typeDefinitionInteger, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionInteger->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionInteger, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionInteger(
  const TypeDefinitionInteger *typeDefinitionInteger, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefInt *pType = NULL;
  AAFRESULT result;


  if (typeDefinitionInteger->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefInt *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionInteger->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->Initialize(*typeDefinitionInteger->id(), 
                               typeDefinitionInteger->size(), 
                               (typeDefinitionInteger->signedNumber()) ? kAAFTrue : kAAFFalse,
                               typeDefinitionInteger->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionEnumeration/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionEnumeration(
  const TypeDefinitionEnumeration *typeDefinitionEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionEnumeration->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionEnumeration, metaDictionary);
  }
  else
  {
    return false;
  }
}


static void InitializeTypeDefinitionEnumeration_cleanup(aafInt64 *memberValues, aafCharacter_constptr *memberNames)
{
  delete [] memberValues;
  delete [] memberNames;
}

static bool InitializeTypeDefinitionEnumeration(
  const TypeDefinitionEnumeration *typeDefinitionEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefEnum *pType = NULL;
  ImplAAFTypeDefInt *pElementType = NULL;
  AAFRESULT result;
  aafInt64 *memberValues = NULL;
  aafCharacter_constptr *memberNames = NULL;
  aafUInt32 index;


  if (typeDefinitionEnumeration->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefEnum *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionEnumeration->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    // Find element type.
    const TypeDefinitionInteger *typeDefinitionInteger = typeDefinitionEnumeration->typeDefinition();
    pElementType = dynamic_cast<ImplAAFTypeDefInt *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionInteger->id()));
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    // Allocate arrays to hold the values and the names.
    memberValues = new aafInt64 [typeDefinitionEnumeration->memberCount()];
    if (NULL == memberValues)
      throw AAFRESULT_NOMEMORY;
    memberNames = new aafCharacter_constptr [typeDefinitionEnumeration->memberCount()];
    if (NULL == memberNames)
    {
      InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
      throw AAFRESULT_NOMEMORY;
    }

    // Initialize the arrays from the static object model.
    const DefinitionEnumerationMember * member;
    for (index = 0; index < typeDefinitionEnumeration->memberCount(); ++index)
    {
      member = typeDefinitionEnumeration->memberAt(index);
      memberValues[index] = member->value();
      memberNames[index] = member->name();
    }

    result = pType->Initialize (*typeDefinitionEnumeration->id(),
        pElementType,
        memberValues,
        memberNames,
        typeDefinitionEnumeration->memberCount(),
        typeDefinitionEnumeration->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
      throw result;
    }


    // Register the enumeration size.
    result = pType->RegisterSize(typeDefinitionInteger->size());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
      throw result;
    }

   InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
   return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionVariableArray/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionVariableArray(
  const TypeDefinitionVariableArray *typeDefinitionVariableArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionVariableArray->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionVariableArray, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionVariableArray(
  const TypeDefinitionVariableArray *typeDefinitionVariableArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefVariableArray *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionVariableArray->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefVariableArray *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionVariableArray->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionVariableArray->elementTypeId());
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionVariableArray->id(),
                                  pElementType,
                                  typeDefinitionVariableArray->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionFixedArray/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionFixedArray(
  const TypeDefinitionFixedArray *typeDefinitionFixedArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionFixedArray->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionFixedArray, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionFixedArray(
  const TypeDefinitionFixedArray *typeDefinitionFixedArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefFixedArray *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionFixedArray->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefFixedArray *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionFixedArray->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionFixedArray->elementTypeId());
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionFixedArray->id(),
                                  pElementType,
                                  typeDefinitionFixedArray->elementCount(),
                                  typeDefinitionFixedArray->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}




//
// TypeDefinition/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionRecord(
  const TypeDefinitionRecord *typeDefinitionRecord, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionRecord->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionRecord, metaDictionary);
  }
  else
  {
    return false;
  }
}



static void InitializeTypeDefinitionRecord_cleanup(
  ImplAAFTypeDef ** memberTypes, 
  aafCharacter_constptr *memberNames, 
  aafUInt32 * memberOffsets)
{
  if (memberTypes)
    delete [] memberTypes;
  if (memberNames)
    delete [] memberNames;
  if (memberOffsets)
    delete [] memberOffsets;
}


static bool InitializeTypeDefinitionRecord(
  const TypeDefinitionRecord *typeDefinitionRecord, 
  ImplAAFMetaDictionary *metaDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDefRecord *pType = NULL;
  ImplAAFTypeDef ** memberTypes = NULL;
  aafCharacter_constptr * memberNames = NULL;
  aafUInt32 * memberOffsets = NULL;
  aafUInt32 fieldIndex;
  const DefinitionRecordField *field = NULL;


  if (typeDefinitionRecord->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefRecord *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionRecord->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    // allocate arrays to hold memberTypes, memberNames, and memberOffsets.
    memberTypes = new ImplAAFTypeDef * [typeDefinitionRecord->fieldCount()];
    ASSERTU (memberTypes);
    if (!memberTypes)
      throw AAFRESULT_NOMEMORY;

    memberNames = new aafCharacter_constptr[typeDefinitionRecord->fieldCount()];
    ASSERTU (memberNames);
    if (!memberNames)
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw AAFRESULT_NOMEMORY;
    }

    memberOffsets = new aafUInt32[typeDefinitionRecord->fieldCount()];
    ASSERTU (memberOffsets);
    if (!memberOffsets)
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw AAFRESULT_NOMEMORY;
    }

    for (fieldIndex = 0; fieldIndex < typeDefinitionRecord->fieldCount(); ++fieldIndex)
    {
      field = typeDefinitionRecord->fieldAt(fieldIndex);
      memberNames[fieldIndex] = field->name();
      memberOffsets[fieldIndex] = field->fieldOffset();
      memberTypes[fieldIndex] = metaDictionary->findAxiomaticTypeDefinition(*field->typeId());
      ASSERTU(memberTypes[fieldIndex]);
      if (!memberTypes[fieldIndex])
        throw AAFRESULT_TYPE_NOT_FOUND;
    }

    // Used the "high-level" Initialize method since the pvtInitialize is obsolete.
    result = pType->Initialize (*typeDefinitionRecord->id(),
                                memberTypes,
                                memberNames,
                                typeDefinitionRecord->fieldCount(),
                                typeDefinitionRecord->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw result;
    }

    result = pType->RegisterMembers (memberOffsets,
                                     typeDefinitionRecord->fieldCount(),
                                     typeDefinitionRecord->recordSize());
    ASSERTU (AAFRESULT_SUCCEEDED (result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw result;
    }


    InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionRename/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionRename(
  const TypeDefinitionRename *typeDefinitionRename, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionRename->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionRename, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionRename(
  const TypeDefinitionRename *typeDefinitionRename, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefRename *pType = NULL;
  ImplAAFTypeDef *pBaseType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionRename->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefRename *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionRename->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pBaseType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionRename->renamedTypeId());
    ASSERTU (pBaseType);
    if (!pBaseType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->Initialize(*typeDefinitionRename->id(),
                               pBaseType,
                               typeDefinitionRename->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionCharacter/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionCharacter(
  const TypeDefinitionCharacter *typeDefinitionCharacter, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionCharacter->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionCharacter, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionCharacter(
  const TypeDefinitionCharacter *typeDefinitionCharacter, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefCharacter *pType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionCharacter->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefCharacter *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionCharacter->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionCharacter->id(), typeDefinitionCharacter->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionString/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionString(
  const TypeDefinitionString *typeDefinitionString, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionString->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionString, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionString(
  const TypeDefinitionString *typeDefinitionString, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefString *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionString->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefString *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionString->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionString->stringTypeId());
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionString->id(),
                                  pElementType,
                                  typeDefinitionString->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionExtendibleEnumeration/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionExtendibleEnumeration(
  const TypeDefinitionExtendibleEnumeration *typeDefinitionExtendibleEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionExtendibleEnumeration->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionExtendibleEnumeration, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionExtendibleEnumeration(
  const TypeDefinitionExtendibleEnumeration *typeDefinitionExtendibleEnumeration, 
  ImplAAFMetaDictionary * /* metaDictionary */)
{
  if (typeDefinitionExtendibleEnumeration->axiomatic())
  {
    // There are currently no aximatic extendible enumerators 2000-MAY-06.
    // We will have to implement this method if this changes in the 
    // future.
    ASSERTU (0);
    return false;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionValueSet/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionValueSet(
  const TypeDefinitionValueSet *typeDefinitionValueSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionValueSet->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionValueSet, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionValueSet(
  const TypeDefinitionValueSet *typeDefinitionValueSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefSet *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;

  
  if (typeDefinitionValueSet->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefSet *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionValueSet->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionValueSet->elementTypeId());
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionValueSet->id(),
                                  pElementType,
                                  typeDefinitionValueSet->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionStrongReference/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionStrongReference(
  const TypeDefinitionStrongReference *typeDefinitionStrongReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReference->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStrongReference, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionStrongReference(
  const TypeDefinitionStrongReference *typeDefinitionStrongReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefStrongObjRef *pType = NULL;
  ImplAAFClassDef *pTarget = NULL;
  AAFRESULT result;
  
  if (typeDefinitionStrongReference->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefStrongObjRef *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStrongReference->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTarget = metaDictionary->findAxiomaticClassDefinition(*typeDefinitionStrongReference->targetId());
    ASSERTU (pTarget);
    if (!pTarget)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionStrongReference->id(),
                                  pTarget,
                                  typeDefinitionStrongReference->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}






//
// TypeDefinitionStrongReferenceSet/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionStrongReferenceSet(
  const TypeDefinitionStrongReferenceSet *typeDefinitionStrongReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReferenceSet->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStrongReferenceSet, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionStrongReferenceSet(
  const TypeDefinitionStrongReferenceSet *typeDefinitionStrongReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefSet *pType = NULL;
  ImplAAFTypeDefStrongObjRef *pElementType = NULL;
  AAFRESULT result;

  
  if (typeDefinitionStrongReferenceSet->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefSet *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStrongReferenceSet->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = dynamic_cast<ImplAAFTypeDefStrongObjRef *>
                   (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStrongReferenceSet->elementTypeId()));
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionStrongReferenceSet->id(),
                                  pElementType,
                                  typeDefinitionStrongReferenceSet->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionStrongReferenceVector/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionStrongReferenceVector(
  const TypeDefinitionStrongReferenceVector *typeDefinitionStrongReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReferenceVector->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStrongReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionStrongReferenceVector(
  const TypeDefinitionStrongReferenceVector *typeDefinitionStrongReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReferenceVector->axiomatic())
  {
    // Vectors are still implemented as variable arrays in the DM.
    return InitializeTypeDefinitionVariableArray(typeDefinitionStrongReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionWeakReference/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionWeakReference(
  const TypeDefinitionWeakReference *typeDefinitionWeakReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReference->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionWeakReference, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionWeakReference(
  const TypeDefinitionWeakReference *typeDefinitionWeakReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefWeakObjRef *pType = NULL;
  ImplAAFClassDef *pTarget = NULL;
  AAFRESULT result;
  aafUInt32 i;
  const AAFObjectModel * objectModel = typeDefinitionWeakReference->objectModel();
  
  if (typeDefinitionWeakReference->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefWeakObjRef *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionWeakReference->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTarget = metaDictionary->findAxiomaticClassDefinition(*typeDefinitionWeakReference->targetId());
    ASSERTU (pTarget);
    if (!pTarget)
      throw AAFRESULT_TYPE_NOT_FOUND;

    aafUID_t * targetSet = new aafUID_t[typeDefinitionWeakReference->targetSetCount()];
    if (NULL == targetSet)
      throw AAFRESULT_NOMEMORY;
    
    OMPropertyId *targetPids = new OMPropertyId[typeDefinitionWeakReference->targetSetCount() + 1];
    if (NULL == targetPids)
    {
      delete [] targetSet;
      throw AAFRESULT_NOMEMORY;
    }
    
    for (i = 0; i < typeDefinitionWeakReference->targetSetCount(); i++)
    {
      memcpy(&targetSet[i], typeDefinitionWeakReference->targetAt(i), sizeof(aafUID_t));
    }

    // The first property id must be one of the two-roots strong reference properties.
    targetPids[0] = 0;
    if (0 == memcmp(&targetSet[0], &kAAFPropID_Root_MetaDictionary, sizeof(kAAFPropID_Root_MetaDictionary)))
    {
      targetPids[0] = 1;
    }
    else if (0 == memcmp(&targetSet[0], &kAAFPropID_Root_Header, sizeof(kAAFPropID_Root_Header)))
    {
      targetPids[0] = 2;
    }
    ASSERTU (1 == targetPids[0] || 2 == targetPids[0]);
    
    const PropertyDefinition * propertyDefinition = NULL;  
    for (i = 1; i < typeDefinitionWeakReference->targetSetCount(); i++)
    {
      propertyDefinition = objectModel->findPropertyDefinition(&targetSet[i]);
      ASSERTU (propertyDefinition && !propertyDefinition->isNil() && propertyDefinition->axiomatic());
      targetPids[i] = propertyDefinition->pid();
    }
    targetPids[i] = 0; // null terminal the array.
    
    // The target of a weak reference must have a unique identifier.
    const ClassDefinition * classDefinition = objectModel->findClassDefinition(typeDefinitionWeakReference->targetId());
    ASSERTU (classDefinition && ClassDefinition::null() != classDefinition && classDefinition->axiomatic());
    propertyDefinition = classDefinition->uniqueIdentifierProperty();
    ASSERTU (propertyDefinition && !propertyDefinition->isNil() && propertyDefinition->axiomatic() && propertyDefinition->uid());
    OMPropertyId uniqueIdentifier = propertyDefinition->pid();
    ASSERTU(0 != uniqueIdentifier);
    
    result = pType->pvtInitialize(*typeDefinitionWeakReference->id(),
                                  pTarget,
                                  typeDefinitionWeakReference->name(),
                                  typeDefinitionWeakReference->targetSetCount(),
                                  targetSet,
                                  targetPids,
                                  uniqueIdentifier);
    delete [] targetPids;
    delete [] targetSet;
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionWeakReferenceSet/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionWeakReferenceSet(
  const TypeDefinitionWeakReferenceSet *typeDefinitionWeakReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReferenceSet->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionWeakReferenceSet, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionWeakReferenceSet(
  const TypeDefinitionWeakReferenceSet *typeDefinitionWeakReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefSet *pType = NULL;
  ImplAAFTypeDefWeakObjRef *pElementType = NULL;
  AAFRESULT result;

  
  if (typeDefinitionWeakReferenceSet->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefSet *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionWeakReferenceSet->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = dynamic_cast<ImplAAFTypeDefWeakObjRef *>
                   (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionWeakReferenceSet->elementTypeId()));
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionWeakReferenceSet->id(),
                                  pElementType,
                                  typeDefinitionWeakReferenceSet->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionWeakReferenceVector/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionWeakReferenceVector(
  const TypeDefinitionWeakReferenceVector *typeDefinitionWeakReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReferenceVector->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionWeakReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionWeakReferenceVector(
  const TypeDefinitionWeakReferenceVector *typeDefinitionWeakReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReferenceVector->axiomatic())
  {
    // Vectors are still implemented as variable arrays in the DM.
    return InitializeTypeDefinitionVariableArray(typeDefinitionWeakReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionStream/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionStream(
  const TypeDefinitionStream *typeDefinitionStream, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStream->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStream, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionStream(
  const TypeDefinitionStream *typeDefinitionStream, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefStream *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionStream->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefStream *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStream->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStream->elementTypeId());
    ASSERTU (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionStream->id(), typeDefinitionStream->name());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionIndirect/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionIndirect(
  const TypeDefinitionIndirect *typeDefinitionIndirect, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionIndirect->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionIndirect, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionIndirect(
  const TypeDefinitionIndirect *typeDefinitionIndirect, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefIndirect *pType = NULL;
  ImplAAFTypeDef *pTypeDefAUID = NULL;
  AAFRESULT result;
  
  if (typeDefinitionIndirect->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefIndirect *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionIndirect->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTypeDefAUID = metaDictionary->findAxiomaticTypeDefinition(kAAFTypeID_AUID);
    ASSERTU (pTypeDefAUID);
    if (!pTypeDefAUID)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionIndirect->id(), 
                                  typeDefinitionIndirect->name(), 
                                  pTypeDefAUID, 
                                  metaDictionary->dataDictionary());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionOpaque/ImplAAFTypeDef callbacks
//
static bool CreateTypeDefinitionOpaque(
  const TypeDefinitionOpaque *typeDefinitionOpaque, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionOpaque->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionOpaque, metaDictionary);
  }
  else
  {
    return false;
  }
}


static bool InitializeTypeDefinitionOpaque(
  const TypeDefinitionOpaque *typeDefinitionOpaque, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefOpaque *pType = NULL;
  ImplAAFTypeDef *pTypeDefAUID = NULL;
  AAFRESULT result;
  
  if (typeDefinitionOpaque->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefOpaque *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionOpaque->id()));
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTypeDefAUID = metaDictionary->findAxiomaticTypeDefinition(kAAFTypeID_AUID);
    ASSERTU (pTypeDefAUID);
    if (!pTypeDefAUID)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionOpaque->id(), 
                                  typeDefinitionOpaque->name(), 
                                  pTypeDefAUID, 
                                  metaDictionary->dataDictionary());
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}




// Install all of the callback procs into the MetaDictionary.
void InstallAAFObjectModelProcs(void)
{
  static bool sInstalled = false;

  if (sInstalled)
  {
    // This is a one-time initialization. The object model call backs
    // are shared by all meta dictionaries.
    return;
  }


  //
  // Install all of the shared callback procs.
  //
  aafUInt32 i;

  // Get the one-and-only meta dictionary.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();

  // Install the callbacks for the class definitions.
  // NOTE: The first version uses the same functions for 
  // both axiomatic and normal built-in classes (properties,
  // and types).
  for (i = 0; i < objectModel->countClassDefinitions(); ++i)
  {
    const ClassDefinition *classDefinition = objectModel->classDefinitionAt(i);

    const_cast<ClassDefinition *>(classDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateClassDefinition);
    const_cast<ClassDefinition *>(classDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeClassDefinition);
  }

  // Install the callbacks for the property definitions.
  for (i = 0; i < objectModel->countPropertyDefinitions(); ++i)
  {
    const PropertyDefinition * propertyDefinition = objectModel->propertyDefinitionAt(i);

    const_cast<PropertyDefinition *>(propertyDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreatePropertyDefinition);
    const_cast<PropertyDefinition *>(propertyDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializePropertyDefinition);
  }

  // Install the callbacks for the type definitions.
  for (i = 0; i < objectModel->countTypeDefinitions(); ++i)
  {
    const TypeDefinition *typeDefinition;
    typeDefinition = objectModel->typeDefinitionAt(i);
    
    switch (typeDefinition->category())
    {
    case kAAFTypeCatInt:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionInteger);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionInteger);
      break;

    case kAAFTypeCatCharacter:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionCharacter);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionCharacter);
      break;

    case kAAFTypeCatStrongObjRef:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStrongReference);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStrongReference);
      break;

    case kAAFTypeCatWeakObjRef:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionWeakReference);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionWeakReference);
      break;

    case kAAFTypeCatRename:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionRename);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionRename);
      break;

    case kAAFTypeCatEnum:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionEnumeration);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionEnumeration);
      break;

    case kAAFTypeCatFixedArray:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionFixedArray);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionFixedArray);
      break;

    case kAAFTypeCatVariableArray:
      if (dynamic_cast<const TypeDefinitionStrongReferenceVector *>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStrongReferenceVector);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStrongReferenceVector);
      }
      else if (dynamic_cast<const TypeDefinitionWeakReferenceVector *>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionWeakReferenceVector);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionWeakReferenceVector);
      }
      else
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionVariableArray);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionVariableArray);
      }
      break;

    case kAAFTypeCatSet:
      if (dynamic_cast<const TypeDefinitionStrongReferenceSet*>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStrongReferenceSet);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStrongReferenceSet);
      }
      else if (dynamic_cast<const TypeDefinitionWeakReferenceSet*>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionWeakReferenceSet);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionWeakReferenceSet);
      }
      else if (dynamic_cast<const TypeDefinitionValueSet*>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionValueSet);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionValueSet);
      }
      else
      {
        ASSERTU (dynamic_cast<const TypeDefinitionStrongReferenceSet*>(typeDefinition) ||
                dynamic_cast<const TypeDefinitionWeakReferenceSet*>(typeDefinition) ||
                dynamic_cast<const TypeDefinitionValueSet*>(typeDefinition));
      }
      break;

    case kAAFTypeCatRecord:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionRecord);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionRecord);
      break;

    case kAAFTypeCatStream:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStream);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStream);
      break;

    case kAAFTypeCatString:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionString);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionString);
      break;

    case kAAFTypeCatExtEnum:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionExtendibleEnumeration);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionExtendibleEnumeration);
      break;

    case kAAFTypeCatIndirect:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionIndirect);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionIndirect);
      break;

    case kAAFTypeCatOpaque:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionOpaque);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionOpaque);
      break;

//    case kAAFTypeCatEncrypted:
//      break;

    default:
      // Unknown type!
      ASSERTU (0);
      break;
    }
  }

  
  // All of the callback procs have been installed.
  sInstalled = true;
}

