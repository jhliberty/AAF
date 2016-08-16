//=---------------------------------------------------------------------=
//
// $Id: ImplAAFMetaDefinition.cpp,v 1.22 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

/*************************************************************************
 * 
 * @module AAFMetaDefinition | AAFMetaDefinition is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFMetaDefinition specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/



#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif


#ifndef __ImplAAFClassDef_h_
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFDictionary_h_
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFMetaDictionary_h_
#include "ImplAAFMetaDictionary.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplEnumAAFPropertyDefs> ImplEnumAAFPropertyDefsSP;

//#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#define HACK_ISPREDEFINED 1
#ifdef HACK_ISPREDEFINED
#include "AAFObjectModel.h"
#endif

#include "OMAssertions.h"
#include <string.h>
#include <wchar.h>


ImplAAFMetaDefinition::ImplAAFMetaDefinition ()
: _name           (PID_MetaDefinition_Name,           L"Name"),
  _description    (PID_MetaDefinition_Description,    L"Description"),
  _identification (PID_MetaDefinition_Identification, L"Identification")
{
  _persistentProperties.put(_name.address());
  _persistentProperties.put(_description.address());
  _persistentProperties.put(_identification.address());
}


ImplAAFMetaDefinition::~ImplAAFMetaDefinition ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::Initialize (
    aafUID_constref id,
	  aafCharacter_constptr pName,
    aafCharacter_constptr pDescription)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
	//validate pName
  if (pName == NULL)
  {
    return AAFRESULT_NULL_PARAM;
  }

  result = SetIdentification (id);
  if (AAFRESULT_SUCCEEDED(result))
  {
    result = SetName (pName);
    if (AAFRESULT_SUCCEEDED(result))
    {
      if (pDescription)
      {
        result = SetDescription (pDescription);
      }
    }
  }

  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetAUID (
      aafUID_t *pAuid) const
{
  if (pAuid == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  else
	{
	  *pAuid = _identification;
	}

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::SetName (
      aafCharacter_constptr  pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  _name = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetName (
      aafCharacter * pName,
      aafUInt32  bufSize)
{
  bool stat;
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _name.copyToBuffer(pName, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetNameBufLen (
      aafUInt32 *  pBufSize)  //@parm [in,out] Definition Name length
{
  if (! pBufSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pBufSize = _name.size();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::SetDescription (
      aafCharacter_constptr pDescription)
{
  if (! pDescription)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (wcslen(pDescription)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  _description = pDescription;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetDescription (
      aafCharacter * pDescription,
      aafUInt32 bufSize)
{
	bool stat;
	if (! pDescription)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if (!_description.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	stat = _description.copyToBuffer(pDescription, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;
	}
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDefinition::GetDescriptionBufLen (
      aafUInt32 * pBufSize)  //@parm [in,out] Definition Name length
{
	if (! pBufSize)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if (!_description.isPresent())
		*pBufSize = 0;
	else
		*pBufSize = _description.size();
	
	return AAFRESULT_SUCCESS;
}



// Gets the dictionary used to create this instance.
AAFRESULT STDMETHODCALLTYPE
ImplAAFMetaDefinition::GetDictionary(ImplAAFDictionary **ppDictionary) const
{
  if(NULL == ppDictionary)
    return AAFRESULT_NULL_PARAM;

  *ppDictionary = dynamic_cast<ImplAAFDictionary *>(classFactory());

  if (NULL == *ppDictionary)
  {
    // The other OMFactory is the meta dictionary. If so, then return the 
    // data dictionary set when the meta dictionary was created.
    // (NOTE: This may be temporary code...transdel:2000-APR-14)
    ImplAAFMetaDictionary *pMetaDictionary = dynamic_cast<ImplAAFMetaDictionary *>(classFactory());
    if (pMetaDictionary)
    {
      *ppDictionary = pMetaDictionary->dataDictionary(); // not reference counted!
    }
  }  
  
  ASSERTU(NULL != *ppDictionary);
  if (NULL == *ppDictionary)
    return AAFRESULT_INVALID_OBJ;
  
  // Bump the reference count...
  (*ppDictionary)->AcquireReference();
  
  return AAFRESULT_SUCCESS;
}


ImplAAFTypeDef*
ImplAAFMetaDefinition::bootstrapTypeWeakReference(
                const OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef>& reference) const
{
  ImplAAFTypeDef* result = 0;
  if (reference.isResolved() || reference.isResolvable()) {
    result = reference;
  } else {
    const aafUID_t id = *reinterpret_cast<const aafUID_t*>(&reference.identification());

    ImplAAFDictionary* pDictionary = 0;
    HRESULT h = GetDictionary(&pDictionary);
    ASSERTU(h == 0);
    h = pDictionary->LookupTypeDef(id, &result);
    ASSERTU(h == 0);
    pDictionary->ReleaseReference();

    // The return value is not reference-counted.
    result->ReleaseReference();
  }
  ASSERTU(result);
  return result;
}

ImplAAFTypeDef*
ImplAAFMetaDefinition::bootstrapTypeWeakReferenceVectorElement(
                   const OMWeakReferenceVectorProperty<OMUniqueObjectIdentification, ImplAAFTypeDef>& vector,
                   aafUInt32 index) const
{
  ImplAAFTypeDef* result = 0;
  if (vector.isResolved(index) || vector.isResolvable(index)) {
    vector.getValueAt(result, index);
  } else {
    const aafUID_t id = *reinterpret_cast<const aafUID_t*>(&vector.identification(index));

    ImplAAFDictionary* pDictionary = 0;
    HRESULT h = GetDictionary(&pDictionary);
    ASSERTU(h == 0);
    h = pDictionary->LookupTypeDef(id, &result);
    ASSERTU(h == 0);
    pDictionary->ReleaseReference();

    // The return value is not reference-counted.
    result->ReleaseReference();
  }
  ASSERTU(result);
  return result;
}

ImplAAFClassDef*
ImplAAFMetaDefinition::bootstrapClassWeakReference(
               const OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFClassDef>& reference) const
{
  ImplAAFClassDef* result = 0;
  if (reference.isResolved() || reference.isResolvable()) {
    result = reference;
  } else {
    const aafUID_t id = *reinterpret_cast<const aafUID_t*>(&reference.identification());

    ImplAAFDictionary* pDictionary = 0;
    HRESULT h = GetDictionary(&pDictionary);
    ASSERTU(h == 0);
    h = pDictionary->LookupClassDef(id, &result);
    ASSERTU(h == 0);
    pDictionary->ReleaseReference();

    // The return value is not reference-counted.
    result->ReleaseReference();
  }
  ASSERTU(result);
  return result;
}

// Associate the existing OMProperties with corresponding property definitions from
// the given class definition. NOTE: This call is recursive, it calls itself again
// for the parent class of the given class until current class is a "root" class.
void ImplAAFMetaDefinition::InitOMProperties (ImplAAFClassDef * pClassDef)
{
  ASSERTU (pClassDef);
  AAFRESULT hr;

  //
  // Init base class properties first
  //
  ImplAAFClassDefSP parentSP;
  hr = pClassDef->GetParent (&parentSP);
  // check that only a "root" will have no parent class definition.
  ASSERTU (AAFRESULT_SUCCEEDED(hr) || (AAFRESULT_FAILED(hr) && AAFRESULT_IS_ROOT_CLASS == hr));
  if(AAFRESULT_SUCCEEDED(hr))
  {
    ASSERTU (parentSP);
    InitOMProperties (parentSP);
  }

  // See if currently existing OM properties are defined in the class
  // def.
  //
  OMPropertySet * ps = propertySet();
  ASSERTU (ps);

  // Loop through properties of this class
  ImplEnumAAFPropertyDefsSP pdEnumSP;
  hr = pClassDef->GetPropertyDefs (&pdEnumSP);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));

  ImplAAFPropertyDefSP propDefSP;
  while (AAFRESULT_SUCCEEDED (pdEnumSP->NextOne (&propDefSP)))
  {
    OMPropertyId defPid = propDefSP->OmPid ();
    // ASSERTU (ps->isAllowed (defPid));
    OMProperty * pProp = 0;
    if (ps->isPresent (defPid))
    {
      // Defined property was already in property set.  (Most
      // probably declared in the impl constructor.)  Get that
      // property.
      pProp = ps->get (defPid);
    }      
    else if(defPid != PID_InterchangeObject_ObjClass
      && (defPid != PID_InterchangeObject_Generation))
    {
      ASSERTU (0);
    }
    
  if(defPid != PID_InterchangeObject_ObjClass
      && (defPid != PID_InterchangeObject_Generation))
  {
      ImplAAFPropertyDef * pPropDef =
        (ImplAAFPropertyDef*) propDefSP;
      OMPropertyDefinition * pOMPropDef =
        dynamic_cast<OMPropertyDefinition*>(pPropDef);
      ASSERTU (pOMPropDef);
      
      ASSERTU (pProp);
      pProp->initialize (pOMPropDef);
      
      pPropDef = 0;
      pOMPropDef = 0;
    }
    propDefSP = 0;
    pProp = 0;
  }
}

const OMUniqueObjectIdentification&
  ImplAAFMetaDefinition::identification(void) const
{
  return *reinterpret_cast<const OMUniqueObjectIdentification*>(&_identification.reference());
}



const wchar_t* ImplAAFMetaDefinition::name(void) const
{
  return _name;
}



bool ImplAAFMetaDefinition::hasDescription(void) const
{
  return _description.isPresent();
}



const wchar_t* ImplAAFMetaDefinition::description(void) const
{
  ASSERTU(hasDescription() == true);
  return _description;
}



bool ImplAAFMetaDefinition::isPredefined(void) const
{
#ifdef HACK_ISPREDEFINED
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const Definition* builtinDefinition =
                        objectModel->findDefinition(&_identification);

  bool result = false;
  if(!(builtinDefinition->isNil()))
  {
    result = true;
  }

  return result;

#else
  ASSERTU(false); // not implemented
  return false;
#endif
}



// Private method to install the AAFObjectModel definition associated with
// this meta definition.
AAFRESULT ImplAAFMetaDefinition::SetIdentification(aafUID_constref identification)
{
  _identification = identification;

  return AAFRESULT_SUCCESS;
}



// override from OMStorable.
const OMClassId& ImplAAFMetaDefinition::classId(void) const
{
  // This method needs to be overridden for all subclasses.
  // All meta definitions class id's are known statically.
  // The corresponding class defintions cannot be extended
  // at runtime.
  static OMObjectIdentification null = {0};
  return null;
}

// Override callbacks from OMStorable
void ImplAAFMetaDefinition::onSave(void* /*clientContext*/) const
{
  // MetaDefinitions are not generation-tracked since they are
  // immutable.
}

void ImplAAFMetaDefinition::onRestore(void* /*clientContext*/) const
{
  // clientContext currently unused

  // Cast away constness (maintaining logical constness)
  ((ImplAAFMetaDefinition*) this)->setInitialized ();
  Progress();
}


// Overrides of ImplAAFStorable.
// Return true if this is a meta object
// NOTE: These objects will eventually owned by the Object Manager.
bool ImplAAFMetaDefinition::metaObject(void) const
{
  return true;
}

// Return true is this is a data object (Interchange object).
bool ImplAAFMetaDefinition::dataObject(void) const
{
  return false;
}


// Method is called after associated class has been added to MetaDictionary.
// If this method fails the class is removed from the MetaDictionary and the
// registration method will fail.
HRESULT ImplAAFMetaDefinition::CompleteClassRegistration(void)
{
  // Default implementation just returns success.
  return AAFRESULT_SUCCESS;
}



