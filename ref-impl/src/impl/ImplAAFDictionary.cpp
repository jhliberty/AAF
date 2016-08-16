//=---------------------------------------------------------------------=
//
// $Id: ImplAAFDictionary.cpp,v 1.138 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFClassDef.h"
#include "ImplEnumAAFClassDefs.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFPropertyDef.h"
#include "ImplEnumAAFTypeDefs.h"
#include "ImplAAFDataDef.h"
#include "ImplEnumAAFDataDefs.h"
#include "ImplAAFOperationDef.h"
#include "ImplAAFParameterDef.h"
#include "ImplEnumAAFOperationDefs.h"
#include "ImplEnumAAFCodecDefs.h"
#include "ImplEnumAAFPropertyDefs.h"
#include "ImplAAFTypeDefRename.h"
#include "ImplEnumAAFContainerDefs.h"
#include "ImplEnumAAFInterpolationDefs.h"
#include "ImplEnumAAFPluginDefs.h"
#include "ImplEnumAAFKLVDataDefs.h"
#include "ImplEnumAAFTaggedValueDefs.h"
#include "AAFTypeDefUIDs.h"
#include "ImplAAFBaseClassFactory.h"
#include "ImplAAFBuiltinClasses.h"
#include "ImplAAFBuiltinTypes.h"

#include "ImplAAFMob.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFBuiltinDefs.h"
#include "AAFContainerDefs.h"
#include "AAFClassDefUIDs.h"

#include "OMVectorIterator.h"

#include "OMAssertions.h"
#include <string.h>
#include "aafErr.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"

#include "ImplAAFSmartPointer.h"

typedef ImplAAFSmartPointer<ImplEnumAAFClassDefs> ImplEnumAAFClassDefsSP;
typedef ImplAAFSmartPointer<ImplEnumAAFPropertyDefs> ImplEnumAAFPropertyDefsSP;

extern "C" const aafClassID_t CLSID_EnumAAFCodecDefs;
extern "C" const aafClassID_t CLSID_EnumAAFContainerDefs;
extern "C" const aafClassID_t CLSID_EnumAAFDataDefs;
extern "C" const aafClassID_t CLSID_EnumAAFDefObjects;
extern "C" const aafClassID_t CLSID_EnumAAFInterpolationDefs;
extern "C" const aafClassID_t CLSID_EnumAAFOperationDefs;
extern "C" const aafClassID_t CLSID_EnumAAFParameterDefs;
extern "C" const aafClassID_t CLSID_EnumAAFPluginDefs;
extern "C" const aafClassID_t CLSID_EnumAAFKLVDataDefs;
extern "C" const aafClassID_t CLSID_EnumAAFTaggedValueDefs;






ImplAAFDictionary::ImplAAFDictionary ()
: _operationDefinitions(PID_Dictionary_OperationDefinitions, L"OperationDefinitions", PID_DefinitionObject_Identification), 
  _parameterDefinitions(PID_Dictionary_ParameterDefinitions, L"ParameterDefinitions", PID_DefinitionObject_Identification),
  _dataDefinitions      (PID_Dictionary_DataDefinitions,    L"DataDefinitions", PID_DefinitionObject_Identification),
  _pluginDefinitions      (PID_Dictionary_PluginDefinitions,    L"PluginDefinitions", PID_DefinitionObject_Identification),
  _codecDefinitions(PID_Dictionary_CodecDefinitions, L"CodecDefinitions", PID_DefinitionObject_Identification),
  _containerDefinitions(PID_Dictionary_ContainerDefinitions, L"ContainerDefinitions", PID_DefinitionObject_Identification),
  _interpolationDefinitions      (PID_Dictionary_InterpolationDefinitions,    L"InterpolationDefinitions", PID_DefinitionObject_Identification),
  _klvDataDefinitions( PID_Dictionary_KLVDataDefinitions, L"KLVDataDefinitions", PID_DefinitionObject_Identification),
  _taggedValueDefinitions( PID_Dictionary_TaggedValueDefinitions, L"TaggedValueDefinitions", PID_DefinitionObject_Identification),
  _pBuiltinClasses (0),
  _pBuiltinTypes (0),
  _pBuiltinDefs (0),
  _pidSegmentsInitialised(false),
  _axiomaticTypes (0),
  _OKToAssurePropTypes (false),
  _defRegistrationAllowed (true),
  _metaDefinitionsInitialized(false),
  _metaDictionary(0)
{
  _persistentProperties.put (_operationDefinitions.address());
  _persistentProperties.put (_parameterDefinitions.address());
  _persistentProperties.put(_dataDefinitions.address());
  _persistentProperties.put(_pluginDefinitions.address());
  _persistentProperties.put(_codecDefinitions.address());
  _persistentProperties.put(_containerDefinitions.address());
  _persistentProperties.put(_interpolationDefinitions.address());
  _persistentProperties.put(_klvDataDefinitions.address());
  _persistentProperties.put(_taggedValueDefinitions.address());

}


ImplAAFDictionary::~ImplAAFDictionary ()
{

  // Release the _codecDefinitions
	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFCodecDef>codecDefinitions(_codecDefinitions);
	while(++codecDefinitions)
	{
		ImplAAFCodecDef *pCodec = codecDefinitions.clearValue();
		if (pCodec)
		{
		  pCodec->ReleaseReference();
		  pCodec = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFContainerDef>containerDefinitions(_containerDefinitions);
	while(++containerDefinitions)
	{
		ImplAAFContainerDef *pContainer = containerDefinitions.clearValue();
		if (pContainer)
		{
		  pContainer->ReleaseReference();
		  pContainer = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFOperationDef>operationDefinitions(_operationDefinitions);
	while(++operationDefinitions)
	{
		ImplAAFOperationDef *pOp = operationDefinitions.clearValue();
		if (pOp)
		{
		  pOp->ReleaseReference();
		  pOp = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameterDef>parameterDefinitions(_parameterDefinitions);
	while(++parameterDefinitions)
	{
		ImplAAFParameterDef *pParm = parameterDefinitions.clearValue();
		if (pParm)
		{
		  pParm->ReleaseReference();
		  pParm = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFInterpolationDef>interpolateDefinitions(_interpolationDefinitions);
	while(++interpolateDefinitions)
	{
		ImplAAFInterpolationDef *pInterp = interpolateDefinitions.clearValue();
		if (pInterp)
		{
		  pInterp->ReleaseReference();
		  pInterp = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPluginDef>pluginDefinitions(_pluginDefinitions);
	while(++pluginDefinitions)
	{
		ImplAAFPluginDef *pPlug = pluginDefinitions.clearValue();
		if (pPlug)
		{
		  pPlug->ReleaseReference();
		  pPlug = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDataDef>dataDefinitions(_dataDefinitions);
	while(++dataDefinitions)
	{
		ImplAAFDataDef *pData = dataDefinitions.clearValue();
		if (pData)
		{
		  pData->ReleaseReference();
		  pData = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFKLVDataDefinition>klvDataDefinitions(_klvDataDefinitions);
	while(++klvDataDefinitions)
	{
		ImplAAFKLVDataDefinition *pKLVData = klvDataDefinitions.clearValue();
		if (pKLVData)
		{
		  pKLVData->ReleaseReference();
		  pKLVData = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTaggedValueDefinition>taggedValueDefinitions(_taggedValueDefinitions);
	while(++taggedValueDefinitions)
	{
		ImplAAFTaggedValueDefinition *pTaggedValue = taggedValueDefinitions.clearValue();
		if (pTaggedValue)
		{
		  pTaggedValue->ReleaseReference();
		  pTaggedValue = 0;
		}
	}

  if (_pBuiltinClasses)
	{
	  delete _pBuiltinClasses;
	  _pBuiltinClasses = 0;
	}

  if (_pBuiltinTypes)
	{
	  delete _pBuiltinTypes;
	  _pBuiltinTypes = 0;
	}

  if (_pBuiltinDefs)
	{
	  delete _pBuiltinDefs;
	  _pBuiltinDefs = 0;
	}

  delete [] _axiomaticTypes;
}

// Return a pointer to the meta dictionary.
ImplAAFMetaDictionary *ImplAAFDictionary::metaDictionary(void) const
{
  // Return the dictionary for creating meta objects, classes, types and 
  // properties.
  ASSERTU(NULL != _metaDictionary);
  return (_metaDictionary);
}


// Install the meta dictionary (i.e. the factory for creating
// meta data: classes, properties and types). This method
// can only be called once.
void ImplAAFDictionary::setMetaDictionary(ImplAAFMetaDictionary *metaDictionary)
{
  ASSERTU(!_metaDictionary);
  // do not reference count this pointer.
  _metaDictionary = metaDictionary;
}

//
// Factory function for all built-in classes.
//
/*static*/ ImplAAFObject *
ImplAAFDictionary::pvtCreateBaseClassInstance(const aafUID_t & auid) 
{

  // Lookup the code class id for the given stored object id.
  const aafClassID_t* id = ImplAAFBaseClassFactory::LookupClassID(auid);
  if (NULL == id)
    return NULL;
  
  // Attempt to create the corresponding storable object.
  ImplAAFRoot *impl = ::CreateImpl(*id);
  if (NULL == impl)
  { // This is a serious programming error. A stored object id was found in the file
	  // with a known base class id but no base object could be created.
    ASSERTU(NULL != impl);
    return NULL;
  }

  // Make sure that the object we created was actually one of our
  // ImplAAFObject derived classes.
  ImplAAFObject* object = dynamic_cast<ImplAAFObject*>(impl);
  if (NULL == object)
  { // Not a valid object. Release the pointer so we don't leak memory.
    impl->ReleaseReference();
    impl = 0;
    ASSERTU(NULL != object);
	  return NULL;
  }

  return object;
}


// Factory method for creating a Dictionary.
ImplAAFDictionary *ImplAAFDictionary::CreateDictionary(void)
{
  // Create a dictionary.
  ImplAAFDictionary * pDictionary = NULL;
  
  
  pDictionary = static_cast<ImplAAFDictionary *>(pvtCreateBaseClassInstance(AUID_AAFDictionary));
  ASSERTU(NULL != pDictionary);
  if (NULL != pDictionary)
  {
    // If we created a dictionary then give it a reference to a factory
    // (dictionary) to satisfy the OMStorable class invariant: Every OMStorabe
    // must have a reference to a factory. Since the dictionary is not created
    // by the OMClassFactory interface we just set the factory to "itself".
    //
    pDictionary->setClassFactory(pDictionary);
  }
  
  pDictionary->pvtSetSoid (AUID_AAFDictionary);

  return pDictionary;
}

bool ImplAAFDictionary::isRegistered(const OMClassId& classId) const
{
  bool result;
  const aafUID_t* auid  = reinterpret_cast<const aafUID_t*>(&classId);
  ImplAAFDictionary* pNonConstThis = const_cast<ImplAAFDictionary*>(this);
  ImplAAFClassDef* pClassDef = 0;
  HRESULT hr = pNonConstThis->LookupClassDef(*auid, &pClassDef);
  if (AAFRESULT_SUCCEEDED(hr)) {
    result = true;
    ASSERTU(pClassDef != 0);
    pClassDef->ReleaseReference();
    pClassDef = 0;
  } else {
    result = false;
  }
  return result;
}

//
// Create an instance of the appropriate derived class, given the class id.
//  This method implements the OMClassFactory interface.
//
OMStorable* ImplAAFDictionary::create(const OMClassId& classId) const
{
  AAFRESULT hr;
  const aafUID_t * auid  = reinterpret_cast<const aafUID_t*>(&classId);
  ImplAAFDictionary * pNonConstThis = (ImplAAFDictionary*) this;

  if (memcmp(auid, &AUID_AAFMetaDictionary, sizeof(aafUID_t)) == 0)
  { // TEMPORARY: Set the factory of the meta dictionary to this dictionary.
    metaDictionary()->setClassFactory(this);
    // Do not bump the reference count. The meta dictionary is currently
    // not publicly available and it is owned by ImplAAFFile not be another
    // OMStorable or ImplAAFObject.
	  return metaDictionary();
  } 
  else
  {
    // Call the sample top-level dictionary method that is called
    // by the API client to create objects.
    ImplAAFObject *pObject = NULL;
    hr = pNonConstThis->CreateInstance(*auid, &pObject);
    ASSERTU (AAFRESULT_SUCCEEDED (hr));
    return pObject;
  }
  // ImplAAFClassDefSP pcd;
  // hr = pNonConstThis->LookupClassDef(*auid, &pcd);
  // ASSERTU (AAFRESULT_SUCCEEDED (hr));
  //
  // return CreateAndInit (pcd);
}

void ImplAAFDictionary::destroy(OMStorable* victim) const
{
  ImplAAFObject* v = dynamic_cast<ImplAAFObject*>(victim);
  ASSERTU(v != 0);
  v->ReleaseReference();
}

void ImplAAFDictionary::associate(const aafUID_t& id,
                                  const OMPropertyId propertyId)
{
  ASSERTU (_pBuiltinClasses);
  if (propertyId >= 0x8000) { // Only remap dynamic pids
    OMPropertyId oldPid;
    AAFRESULT r = _pBuiltinClasses->LookupOmPid(id, oldPid);
    if (AAFRESULT_SUCCEEDED(r)) {
      r = _pBuiltinClasses->MapOmPid(id, propertyId);
      ASSERTU(AAFRESULT_SUCCEEDED(r));
    } // else doesn't currently work for properties that aren't compiled-in
  }
}

ImplAAFObject *
ImplAAFDictionary::CreateAndInit(ImplAAFClassDef * pClassDef) const
{
  ASSERTU (pClassDef);
  AAFRESULT hr;
  aafUID_t auid;
  hr = pClassDef->GetAUID(&auid);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));

  ImplAAFObject * pNewObject = 0;
  pNewObject = pvtInstantiate (auid);
  if (pNewObject)
	{
	  pNewObject->InitializeOMStorable (pClassDef);

	  // Attempt to initialize any class extensions associated
	  // with this object. Only the most derived extension that has an
	  // associated  plugin is created.
	  // QUESTION: How should we "deal with" failure? We really need
	  // an  error/warning log file for this kind of information.
	  pNewObject->InitializeExtensions();
	}

  return pNewObject;
}

ImplAAFObject* ImplAAFDictionary::pvtInstantiate(const aafUID_t & auid) const
{
  ImplAAFObject *result = 0;
  ImplAAFClassDef	*parent;

  // Is this a request to create the dictionary?
  if (memcmp(&auid, &AUID_AAFDictionary, sizeof(aafUID_t)) == 0)
	{ // The result is just this instance.
	  result = const_cast<ImplAAFDictionary*>(this);
	  // Bump the reference count.
	  AcquireReference();
	} 
  else
	{
	  // Create an instance of the class corresponsing to the given
	  // stored object id.  In other words, we instantiate an
	  // implementation object which can represent this stored object
	  // in code.  In the case of built-in types, each one *has* a
	  // code object which can represent it.  However in the case of
	  // client-defined classes, the best we can do is instantiate the
	  // most-derived code object which is an inheritance parent of
	  // the desired class.

	  // First see if this is a built-in class.
	  //
	  result = pvtCreateBaseClassInstance(auid);

//DAB+ 9-Sep-2001 Code corrected to REALLY iterate up the inheritance hierarchy
	  aafUID_t parentAUID = auid;
	  while (result == 0)
	  {
//	    aafUID_t parentAUID = auid;
//DAB- 9-Sep-2001 Code corrected to REALLY iterate up the inheritance hierarchy
	    aafBool	isRoot;

	    // Not a built-in class; find the nearest built-in parent.
	    // That is, iterate up the inheritance hierarchy until we
	    // find a class which we know how to instantiate.
	    //
	    ImplAAFClassDefSP pcd;
	    AAFRESULT hr;

	    hr = ((ImplAAFDictionary*)this)->LookupClassDef (parentAUID, &pcd);
	    if (AAFRESULT_FAILED (hr))
	    {
	      // AUID does not correspond to any class in the
	      // dictionary; bail out with NULL result
	      ASSERTU (0 == result);
	      break;
	    }

	    hr = pcd->IsRoot(&isRoot);
	    if (isRoot || hr != AAFRESULT_SUCCESS)
	    {
	      // Class was apparently registered, but no appropriate
	      // parent class found!  This should not happen, as every
	      // registered class must have a registered parent class.
	      // The only exception is AAFObject, which would have
	      // been found by the earlier
	      // pvtCreateBaseClassInstance() call.
	      ASSERTU (0);
	    }

	    hr = pcd->GetParent (&parent);
	    hr = parent->GetAUID(&parentAUID);
	    parent->ReleaseReference();
	    result = pvtCreateBaseClassInstance(parentAUID);
	  }
	}

  if (result)
	{
	  if ((ImplAAFDictionary *)result != this)
		{
		  // If we created an object then give it a reference to the
		  // factory (dictionary) that was used to created it.
		  //
		  result->setClassFactory(this);

		  // Set this object's stored ID.  Be sure to set it to the
		  // requested ID, not the instantiated one.  (These will be
		  // different if the requested ID is a client-supplied
		  // extension class.)
		  result->pvtSetSoid (auid);
		}
	}

  return result;
}


// Creates a single uninitialized AAF object of the class associated 
  // with a specified stored object id.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFDictionary::CreateInstance (
    // Stored Object ID of the stored object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in auid
    ImplAAFObject ** ppvObject)
{
  if (NULL == ppvObject)
    return AAFRESULT_NULL_PARAM;
  
  // Lookup the class definition for the given classId. If the class
  // definition is one of the "built-in" class definitions then the
  // definition will be created and registered with the dictionary
  // if necessary. (TRR 2000-MAR-01)
  ImplAAFClassDefSP pClassDef;
  AAFRESULT hr;
  hr = LookupClassDef (classId, &pClassDef);
  if (AAFRESULT_FAILED (hr))
	return hr;

  if (! pClassDef->pvtIsConcrete ())
	return AAFRESULT_ABSTRACT_CLASS;

  *ppvObject = CreateAndInit (pClassDef);

  if (NULL == *ppvObject)
    return AAFRESULT_INVALID_CLASS_ID;
  else
    return AAFRESULT_SUCCESS;
}




//Creates a single uninitialized AAF meta definition associated 
  // with a specified stored object id.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFDictionary::CreateMetaInstance (
    // Stored Object ID of the meta object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in auid
    ImplAAFMetaDefinition ** ppMetaObject)
{
  // Ask the meta dictionary to create the meta definition
  return (metaDictionary()->CreateMetaInstance(classId, ppMetaObject));
}





AAFRESULT ImplAAFDictionary::dictLookupClassDef (
      const aafUID_t & classID,
      ImplAAFClassDef ** ppClassDef)
{
  // Ask the meta dictionary to see if the class has already in the set.
  return (metaDictionary()->LookupClassDef(classID, ppClassDef));
}

bool ImplAAFDictionary::PvtIsClassPresent (
      const aafUID_t & classID)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->containsClass(classID));
}

bool
ImplAAFDictionary::IsAxiomaticClass (const aafUID_t &classID) const
{
  ImplAAFClassDef *pAxiomaticClass = metaDictionary()->findAxiomaticClassDefinition(classID); // return value NOT reference counted! 
  if (pAxiomaticClass)
    return true;
  else
    return false;
}


bool
ImplAAFDictionary::pvtLookupAxiomaticClassDef (const aafUID_t &classID,
										   ImplAAFClassDef **
										   ppClassDef)
{
  *ppClassDef = metaDictionary()->findAxiomaticClassDefinition(classID); // return value NOT reference counted!
  if (*ppClassDef)
  {
    (*ppClassDef)->AcquireReference(); // We will be returning this references!
    return true;
  }
  else
  {
    return false;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupClassDef (
      const aafUID_t & classID,
      ImplAAFClassDef ** ppClassDef)
{
  AAFRESULT					status;

  //
  // TEMPORARY:
  // Initialize the built-in types and classes if necessary.
  //
  InitializeMetaDefinitions();



  if (! ppClassDef) return AAFRESULT_NULL_PARAM;

  if (pvtLookupAxiomaticClassDef (classID, ppClassDef))
  {
	  ASSERTU (*ppClassDef);

	  // Yes, this is an axiomatic class.  classDef should be filled
	  // in.  Assure that it's in the dictionary, and return it.
	  
	  // here's where we assure it's in the dictionary.
	  if(_defRegistrationAllowed)
	  {
		  ImplAAFClassDef	*parent;
		  // These classes fail with doubly-opened storages
#if 0
		  if(memcmp(&classID, &kAAFClassID_ClassDefinition, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionCharacter, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionString, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionVariableArray, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionRecord, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionFixedArray, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionInteger, sizeof(aafUID_t)) != 0
		   )
#endif
		  {
			  if(!PvtIsClassPresent(classID))
			  {
				aafBool		isRoot;
				aafUID_t	uid;

				  (*ppClassDef)->IsRoot(&isRoot);
				  if(isRoot)
				  {
					 (*ppClassDef)->SetParent(*ppClassDef);
				  }
				  else
				  {
					 (*ppClassDef)->GetParent(&parent);	// Uses bootstrap parent if set
					 parent->GetAUID(&uid);
					 parent->ReleaseReference();
					 parent = NULL;
					 LookupClassDef (uid, &parent);
					 (*ppClassDef)->SetParent(parent);
					 parent->ReleaseReference();
				  }
				  (*ppClassDef)->SetBootstrapParent(NULL);
				  status = PvtRegisterClassDef(*ppClassDef);
				  ASSERTU (AAFRESULT_SUCCEEDED (status));
			  }
		  }
	  }
	  
	  AssurePropertyTypes (*ppClassDef);
	  return AAFRESULT_SUCCESS;
  }




  // Not axiomatic.  Check to see if it's already in the dict.
  status = dictLookupClassDef (classID, ppClassDef);
  if (AAFRESULT_SUCCEEDED (status))
	{
	  // Yup, already here. Pass on the good news.
	  AssurePropertyTypes (*ppClassDef);
	  return status;
	}

  // Not found in the dict; check to see if it was due to failure
  // other than not being found
  if (AAFRESULT_NO_MORE_OBJECTS != status)
	{
	  // Yup, some other failure. Pass on the bad news.
	  return status;
	}

  // If we're here, it was not found in dict.  Try it in builtins.
  status = _pBuiltinClasses->NewBuiltinClassDef (classID, ppClassDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized class guid
	  return status;
	}

  // Yup, found it in builtins.  Register it.
  ASSERTU (*ppClassDef);
  status = PvtRegisterClassDef (*ppClassDef);
  if (AAFRESULT_FAILED (status))
	return status;
		  
  AssurePropertyTypes (*ppClassDef);
	
  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CreateForwardClassReference (
      aafUID_constref classId)
{
  // Defer to the meta dictionary.
  return (metaDictionary()->CreateForwardClassReference(classId));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::HasForwardClassReference (
      aafUID_constref classId,
      aafBoolean_t * pResult)
{
  // Defer to the meta dictionary.
  return (metaDictionary()->HasForwardClassReference(classId, pResult));
}


// Private class registration. This version does not perform any
// initialization that requires other classes, types or properties or
// types to be in the dictionary...it only adds the given class
// to the set in the dictionary.
AAFRESULT ImplAAFDictionary::PvtRegisterClassDef(ImplAAFClassDef * pClassDef)
{
  ASSERTU (_defRegistrationAllowed);
  // Defer to the meta dictionary.
  return (metaDictionary()->PvtRegisterClassDef(pClassDef));
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterClassDef (
      ImplAAFClassDef * pClassDef)
{
  ASSERTU (_defRegistrationAllowed);
  // Defer to the meta dictionary.
  return (metaDictionary()->RegisterClassDef(pClassDef));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetClassDefs (
      ImplEnumAAFClassDefs ** ppEnum)
{
  // Defer to the meta dictionary.
  return (metaDictionary()->GetClassDefs(ppEnum));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountClassDefs
        (aafUInt32 * pResult)
{
  // Defer to the meta dictionary.
  return (metaDictionary()->CountClassDefs(pResult));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
  ASSERTU (_defRegistrationAllowed);

  // Defer to the meta dictionary.
  return(metaDictionary()->RegisterTypeDef(pTypeDef));
}


AAFRESULT ImplAAFDictionary::dictLookupTypeDef (
      const aafUID_t & typeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  // Defer to the meta dictionary.
  return (metaDictionary()->LookupTypeDef(typeID, ppTypeDef));
}

bool ImplAAFDictionary::PvtIsTypePresent (
      const aafUID_t & typeID)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->containsType(typeID));
}


bool
ImplAAFDictionary::pvtLookupAxiomaticTypeDef (const aafUID_t &typeID,
										   ImplAAFTypeDef **
										   ppTypeDef)
{
  *ppTypeDef = metaDictionary()->findAxiomaticTypeDefinition(typeID); // return value NOT reference counted!
  if (*ppTypeDef)
  {
    (*ppTypeDef)->AcquireReference (); // We will be returning this references!
    return true;
  }
  else
  {
    return false;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupTypeDef (
      const aafUID_t & typeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  ImplAAFTypeDefSP			typeDef;
  AAFRESULT					status;


  //
  // TEMPORARY:
  // Initialize the built-in types and classes if necessary.
  //
  InitializeMetaDefinitions();


  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (pvtLookupAxiomaticTypeDef (typeID, &typeDef))
  {
	  if(_defRegistrationAllowed && !PvtIsTypePresent(typeID))
	  {
		  status = RegisterTypeDef(typeDef);
		  ASSERTU (AAFRESULT_SUCCEEDED (status));
	  }
	  
	  ASSERTU (ppTypeDef);
	  *ppTypeDef = typeDef;
	  ASSERTU (*ppTypeDef);
	  (*ppTypeDef)->AcquireReference ();
	  return AAFRESULT_SUCCESS;
  }

  // Not axiomatic.  Check to see if it's already in the dict.
  status = dictLookupTypeDef (typeID, ppTypeDef);
  if (AAFRESULT_SUCCEEDED (status))
	{
	  // Yup, already here. Pass on the good news.
	  return status;
	}

  // Not found in the dict; check to see if it was due to failure
  // other than not being found
  if (AAFRESULT_NO_MORE_OBJECTS != status)
	{
	  // Yup, some other failure. Pass on the bad news.
	  return status;
	}

  // If we're here, it was not found in dict.  Try it in builtins.
  ASSERTU (0 == (ImplAAFTypeDef*) typeDef);
  status = _pBuiltinTypes->NewBuiltinTypeDef (typeID, &typeDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized type guid
	  return AAFRESULT_NO_MORE_OBJECTS;
	}
  // Yup, found it in builtins.  Register it.
  if(_defRegistrationAllowed)
	 {
	  ASSERTU (typeDef);
	 status = RegisterTypeDef (typeDef);
	 if (AAFRESULT_FAILED (status))
		return status;
  }
		  
  ASSERTU (ppTypeDef);
  *ppTypeDef = typeDef;
  ASSERTU (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();
	
  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetTypeDefs (
      ImplEnumAAFTypeDefs ** ppEnum)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->GetTypeDefs(ppEnum));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountTypeDefs
        (aafUInt32 * pResult)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->CountTypeDefs(pResult));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterOpaqueTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->RegisterOpaqueTypeDef(pTypeDef));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupOpaqueTypeDef (
      const aafUID_t & typeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->LookupOpaqueTypeDef(typeID, ppTypeDef));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetOpaqueTypeDefs (
      ImplEnumAAFTypeDefs ** ppEnum)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->GetOpaqueTypeDefs(ppEnum));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountOpaqueTypeDefs
        (aafUInt32 * pResult)
{
  // Defer to the meta dictionary.
  return(metaDictionary()->CountOpaqueTypeDefs(pResult));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterKLVDataKey (
      aafUID_t keyUID, 
	  ImplAAFTypeDef *underlyingType)
{
	ImplAAFTypeDefRename	*pRenameDef = NULL;
	
	XPROTECT()
	{
		CHECK(CreateMetaInstance(AUID_AAFTypeDefRename, (ImplAAFMetaDefinition **)&pRenameDef));
		CHECK(pRenameDef->Initialize (keyUID, underlyingType, L"KLV Data"));
		CHECK(RegisterOpaqueTypeDef(pRenameDef));
		pRenameDef->ReleaseReference();
		pRenameDef = NULL;
	}
	XEXCEPT
	{
		if (pRenameDef)
			pRenameDef->ReleaseReference();
	}
	XEND
		
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterDataDef (
      ImplAAFDataDef *pDataDef)
{
  ASSERTU (_defRegistrationAllowed);

  if (NULL == pDataDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pDataDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  ImplAAFDataDef * pExistingDataDef = NULL;
  hr = LookupDataDef(newAUID, &pExistingDataDef);

  if (hr != AAFRESULT_SUCCESS) {
    // This type is not yet registered, add it to the dictionary.
    // first making sure it's being used somewhere else.
    if (pDataDef->attached())
	return AAFRESULT_OBJECT_ALREADY_ATTACHED;
    _dataDefinitions.appendValue(pDataDef);
    pDataDef->AcquireReference();
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pDataDef->SetDict(this);
  } else {
    // This type is already registered, probably because it was
    // already in the persisted dictionary.
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pExistingDataDef->SetDict(this);
    pExistingDataDef->ReleaseReference();
	pExistingDataDef = 0;
  }

  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupDataDef (
      const aafUID_t & dataDefinitionID,
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_dataDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&dataDefinitionID)),
                             *ppDataDef))
	{
		ASSERTU(NULL != *ppDataDef);
		(*ppDataDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetDataDefs (
      ImplEnumAAFDataDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFDataDefs *theEnum = (ImplEnumAAFDataDefs *)CreateImpl (CLSID_EnumAAFDataDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDataDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDataDef>(_dataDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFDataDefs,this,iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountDataDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _dataDefinitions.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterOperationDef (
      ImplAAFOperationDef *pOperationDef)
{
  ASSERTU (_defRegistrationAllowed);

	if (NULL == pOperationDef)
		return AAFRESULT_NULL_PARAM;

	if (pOperationDef->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_operationDefinitions.appendValue(pOperationDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pOperationDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupOperationDef (
      const aafUID_t & effectID,
      ImplAAFOperationDef **ppOperationDef)
{
  if (!ppOperationDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_operationDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&effectID)),
                             *ppOperationDef))
	{
		ASSERTU(NULL != *ppOperationDef);
		(*ppOperationDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetOperationDefs (
      ImplEnumAAFOperationDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFOperationDefs *theEnum = (ImplEnumAAFOperationDefs *)CreateImpl (CLSID_EnumAAFOperationDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFOperationDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFOperationDef>(_operationDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFOperationDefs, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountOperationDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _operationDefinitions.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterParameterDef (
      ImplAAFParameterDef *pParameterDef)
{
  ASSERTU (_defRegistrationAllowed);

	if (NULL == pParameterDef)
		return AAFRESULT_NULL_PARAM;
	if (pParameterDef->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;


	_parameterDefinitions.appendValue(pParameterDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pParameterDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupParameterDef (
      const aafUID_t & parameterID,
      ImplAAFParameterDef **ppParameterDef)
{
  if (!ppParameterDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_parameterDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&parameterID)),
                             *ppParameterDef))
	{
		ASSERTU(NULL != *ppParameterDef);
		(*ppParameterDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetParameterDefs (
      ImplEnumAAFParameterDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFParameterDefs *theEnum = (ImplEnumAAFParameterDefs *)CreateImpl (CLSID_EnumAAFParameterDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameterDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameterDef>(_parameterDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFParameterDefs, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountParameterDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _parameterDefinitions.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterCodecDef (
      ImplAAFCodecDef *pPlugDef)
{
  ASSERTU (_defRegistrationAllowed);

	if (NULL == pPlugDef)
		return AAFRESULT_NULL_PARAM;

	if (pPlugDef->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_codecDefinitions.appendValue(pPlugDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pPlugDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT
    ImplAAFDictionary::GetNumCodecDefs(aafUInt32 *  pNumCodecDefs)
{
  aafUInt32 siz;

  if(pNumCodecDefs == NULL)
    return AAFRESULT_NULL_PARAM;
  
  siz = _codecDefinitions.count();
  
  *pNumCodecDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFDictionary::LookupCodecDef
  (const aafUID_t & defID,
   ImplAAFCodecDef **ppResult)
{
  if (!ppResult) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_codecDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&defID)),
                             *ppResult))
	{
		ASSERTU(NULL != *ppResult);
		(*ppResult)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetCodecDefs (
      ImplEnumAAFCodecDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFCodecDefs *theEnum = (ImplEnumAAFCodecDefs *)CreateImpl (CLSID_EnumAAFCodecDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFCodecDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFCodecDef>(_codecDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFCodecDefs,this,iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountCodecDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _codecDefinitions.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterContainerDef (
      ImplAAFContainerDef *pPlugDef)
{
  ASSERTU (_defRegistrationAllowed);

	if (NULL == pPlugDef)
		return AAFRESULT_NULL_PARAM;

	if (pPlugDef->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_containerDefinitions.appendValue(pPlugDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pPlugDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupContainerDef
	  (const aafUID_t & defID,
	   ImplAAFContainerDef **ppResult)
{
  if (!ppResult) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_containerDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&defID)),
                             *ppResult))
	{
		ASSERTU(NULL != *ppResult);
		(*ppResult)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}

AAFRESULT
    ImplAAFDictionary::GetNumContainerDefs(aafUInt32 *  pNumContainerDefs)
{
  aafUInt32 siz;

  if(pNumContainerDefs == NULL)
    return AAFRESULT_NULL_PARAM;
  
  siz = _containerDefinitions.count();
  
  *pNumContainerDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetContainerDefs (
      ImplEnumAAFContainerDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFContainerDefs *theEnum = (ImplEnumAAFContainerDefs *)CreateImpl (CLSID_EnumAAFContainerDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFContainerDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFContainerDef>(_containerDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFContainerDefs,this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountContainerDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _containerDefinitions.count();
  return AAFRESULT_SUCCESS;
}

void ImplAAFDictionary::InitDataDefinition(const aafUID_t & dataDefinitionID,
                                    const aafCharacter *name,
		                            const aafCharacter *description)
{
  ImplAAFDataDef		*dataDef = NULL;
  AAFRESULT		hr;

  hr = LookupDataDef (dataDefinitionID, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (dataDefinitionID, name, description);
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;
}

void ImplAAFDictionary::InitContainerDefinition(const aafUID_t & defID,
                                         const aafCharacter *name,
				                         const aafCharacter *description)
{
  ImplAAFContainerDef	*containerDef = NULL;
  AAFRESULT		hr;
  hr = LookupContainerDef (defID, &containerDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdContainerDef()->
		CreateInstance ((ImplAAFObject **)&containerDef);
	  hr = containerDef->Initialize (defID, name, description);
	  hr = RegisterContainerDef (containerDef);
	}
  containerDef->ReleaseReference();
  containerDef = NULL;
}

void ImplAAFDictionary::InitBuiltins()
{
#include "ImplAAFDictionary.cpp_DataDefs"
#include "ImplAAFDictionary.cpp_ContainerDefs"

  AssureClassPropertyTypes ();
}

#define check_result(result)     \
  if (AAFRESULT_FAILED (result)) \
	return result;


/* Note!  Will modify argument... */
#define release_if_set(pIfc)     \
{                                \
  if (pIfc)                      \
	{                            \
	  pIfc->ReleaseReference (); \
	  pIfc = 0;                  \
	}                            \
}

/* Note!  Will modify argument... */
#define release_and_zero(pIfc)   \
{                                \
  ASSERTU (pIfc);                 \
  pIfc->ReleaseReference ();     \
  pIfc = 0;                      \
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterInterpolationDef (
      ImplAAFInterpolationDef *pInterpolationDef)
{
  ASSERTU (_defRegistrationAllowed);

	if (NULL == pInterpolationDef)
		return AAFRESULT_NULL_PARAM;

	if (pInterpolationDef->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_interpolationDefinitions.appendValue(pInterpolationDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pInterpolationDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupInterpolationDef (
      const aafUID_t & interpolationID,
      ImplAAFInterpolationDef **ppInterpolationDef)
{
  if (!ppInterpolationDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_interpolationDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&interpolationID)),
                             *ppInterpolationDef))
	{
		ASSERTU(NULL != *ppInterpolationDef);
		(*ppInterpolationDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetInterpolationDefs (
      ImplEnumAAFInterpolationDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFInterpolationDefs *theEnum = (ImplEnumAAFInterpolationDefs *)CreateImpl (CLSID_EnumAAFInterpolationDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFInterpolationDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFInterpolationDef>(_interpolationDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFInterpolationDefs,this,iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountInterpolationDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _interpolationDefinitions.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterPluginDef (		//!!! Bring this out through the IDL
      ImplAAFPluginDef *pDesc)
{
  ASSERTU (_defRegistrationAllowed);

	if (NULL == pDesc)
		return AAFRESULT_NULL_PARAM;

	if (pDesc->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_pluginDefinitions.appendValue(pDesc);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pDesc->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupPluginDef (		//!!! Bring this out through the IDL
      const aafUID_t & interpolationID,
      ImplAAFPluginDef **ppPluginDesc)
{
  if (!ppPluginDesc) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_pluginDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&interpolationID)),
                             *ppPluginDesc))
	{
		ASSERTU(NULL != *ppPluginDesc);
		(*ppPluginDesc)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetPluginDefs (		//!!! Bring this out through the IDL
      ImplEnumAAFPluginDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFPluginDefs *theEnum = (ImplEnumAAFPluginDefs *)CreateImpl (CLSID_EnumAAFPluginDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPluginDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPluginDef>(_pluginDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFPluginDefs, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountPluginDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _pluginDefinitions.count();
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterKLVDataDef (ImplAAFKLVDataDefinition* pDef )
{
  ASSERTU (_defRegistrationAllowed);

  if ( NULL == pDef ) {
    return AAFRESULT_NULL_PARAM;
  }
  
  if ( pDef->attached() ) {
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  }

  _klvDataDefinitions.appendValue(pDef);
  pDef->AcquireReference();
  
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupKLVDataDef (aafUID_constref  id,
					 ImplAAFKLVDataDefinition ** ppDef )
{
  if ( !ppDef ) {
    return AAFRESULT_NULL_PARAM;
  }

  AAFRESULT result = AAFRESULT_SUCCESS;
  
  if ( _klvDataDefinitions.find( *reinterpret_cast<const OMObjectIdentification*>(&id),
				 *ppDef) ) {
    ASSERTU( *ppDef );
    (*ppDef)->AcquireReference();
  }
  else {
    result = AAFRESULT_NO_MORE_OBJECTS;
  }
  
  return result;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetKLVDataDefs (ImplEnumAAFKLVDataDefs** ppEnum )
{
  if ( NULL == ppEnum ) {
    return AAFRESULT_NULL_PARAM;
  }

  *ppEnum = 0;
  
  ImplEnumAAFKLVDataDefs *theEnum = (ImplEnumAAFKLVDataDefs *)CreateImpl (CLSID_EnumAAFKLVDataDefs);
  
  XPROTECT()
  {
    OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFKLVDataDefinition>* iter = 
      new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFKLVDataDefinition>(_klvDataDefinitions);
    if ( !iter ) {
      RAISE(AAFRESULT_NOMEMORY);
    }
    CHECK( theEnum->Initialize(&CLSID_EnumAAFKLVDataDefs,this,iter) );
    *ppEnum = theEnum;
  }
  XEXCEPT
  {
    if ( theEnum )
      {
	theEnum->ReleaseReference();
	theEnum = 0;
      }
  }
  XEND;
  
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountKLVDataDefs (aafUInt32 *pResult)
{
  if(pResult == NULL)
    return AAFRESULT_NULL_PARAM;
  
  *pResult = _klvDataDefinitions.count();
  
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterTaggedValueDef (ImplAAFTaggedValueDefinition* pDef)
{
  ASSERTU (_defRegistrationAllowed);

  if ( NULL == pDef ) {
    return AAFRESULT_NULL_PARAM;
  }
  
  if ( pDef->attached() ) {
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  }

  _taggedValueDefinitions.appendValue(pDef);
  pDef->AcquireReference();
  
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupTaggedValueDef (aafUID_constref  id,
					     ImplAAFTaggedValueDefinition ** ppDef )
{
  if ( !ppDef ) {
    return AAFRESULT_NULL_PARAM;
  }

  AAFRESULT result = AAFRESULT_SUCCESS;
  
  if ( _taggedValueDefinitions.find( *reinterpret_cast<const OMObjectIdentification*>(&id),
				     *ppDef) ) {
    ASSERTU( *ppDef );
    (*ppDef)->AcquireReference();
  }
  else {
    result = AAFRESULT_NO_MORE_OBJECTS;
  }
  
  return result;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetTaggedValueDefs (ImplEnumAAFTaggedValueDefs** ppEnum)
{
  if ( NULL == ppEnum ) {
    return AAFRESULT_NULL_PARAM;
  }

  *ppEnum = 0;
  
  ImplEnumAAFTaggedValueDefs *theEnum = (ImplEnumAAFTaggedValueDefs *)CreateImpl (CLSID_EnumAAFTaggedValueDefs);
  
  XPROTECT()
  {
    OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTaggedValueDefinition>* iter = 
      new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTaggedValueDefinition>(_taggedValueDefinitions);
    if ( !iter ) {
      RAISE(AAFRESULT_NOMEMORY);
    }
    CHECK( theEnum->Initialize(&CLSID_EnumAAFTaggedValueDefs,this,iter) );
    *ppEnum = theEnum;
  }
  XEXCEPT
  {
    if ( theEnum )
      {
	theEnum->ReleaseReference();
	theEnum = 0;
      }
  }
  XEND;
  
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountTaggedValueDefs (aafUInt32* pResult)
{
  if(pResult == NULL)
    return AAFRESULT_NULL_PARAM;
  
  *pResult = _taggedValueDefinitions.count();
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupAuxiliaryDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_Auxiliary, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupDescriptiveMetadataDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_DescriptiveMetadata, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupEdgecodeDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_Edgecode, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupLegacyPictureDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_LegacyPicture, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupLegacySoundDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_LegacySound, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupLegacyTimecodeDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_LegacyTimecode, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupMatteDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_Matte, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupPictureDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_Picture, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupPictureWithMatteDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_PictureWithMatte, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupSoundDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_Sound, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupTimecodeDataDef (
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr = LookupDataDef( kAAFDataDef_Timecode, ppDataDef );
  ASSERTU(AAFRESULT_SUCCEEDED (hr));
  ASSERTU(NULL != *ppDataDef);

  return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFDictionary::PvtIsPropertyDefDuplicate(
							aafUID_t propertyDefID,
							ImplAAFClassDef *correctClass,
							bool	*isDuplicate)
{
	ImplEnumAAFClassDefs	*classEnum = NULL;
	ImplAAFClassDef			*pClassDef = NULL;
	aafUID_t				testClassID, correctClassID;
	bool					foundDup = false;

	if (NULL == correctClass)
		return AAFRESULT_NULL_PARAM;
	if (NULL == isDuplicate)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(correctClass->GetAUID(&correctClassID));
		CHECK(GetClassDefs(&classEnum));
		while((foundDup == false) && classEnum->NextOne(&pClassDef) == AAFRESULT_SUCCESS)
		{
			CHECK(pClassDef->GetAUID(&testClassID));
			if(memcmp(&testClassID, &correctClassID, sizeof(aafUID_t)) != 0)
			{
				foundDup = pClassDef->PvtIsPropertyDefRegistered(propertyDefID);
			}
			pClassDef->ReleaseReference();
			pClassDef = NULL;
		}
		classEnum->ReleaseReference();
		classEnum = 0;
	}
	XEXCEPT
	{
		if (pClassDef)
		  {
			pClassDef->ReleaseReference();
			pClassDef = 0;
		  }
		if (classEnum)
		  {
			classEnum->ReleaseReference();
			classEnum = 0;
		  }
	}
	XEND;

	*isDuplicate = foundDup;
	
	return(AAFRESULT_SUCCESS);
}


bool ImplAAFDictionary::PIDSegment::operator==(const ImplAAFDictionary::PIDSegment& r)
{
    return firstPid == r.firstPid && lastPid == r.lastPid;
}

AAFRESULT ImplAAFDictionary::GenerateOmPid
(
 const aafUID_t & rAuid,
 OMPropertyId & rOutPid
)
{
    OMPropertyId result;
    AAFRESULT hr;
    
    ASSERTU(_pBuiltinClasses);
    hr = _pBuiltinClasses->LookupOmPid(rAuid, result);
    if (AAFRESULT_SUCCEEDED(hr) && result != 0) 
    {
        rOutPid = result;
    }
    else
    {
        // Generate an om pid for user-extended properties (either in
        // user-extended classes, or from user-added properties to
        // existing classes).
        //
        // OM PID rules:
        // - guaranteed to be unique within this file
        // - not guaranteed to be unique across files
        // - all builtin properties have a fixed prop<->PID mapping
        // - all user properties not guaranted a mapping across files
        // - all builtin properties have *non-negative* PIDs
        // - all user properties have *negative* PIDs.

        // Specifics of this implementation:
        // A list of segments of continuous sequence of pids is maintained in the
        // _pidSegments variable. New pids are generated by filling in the gaps
        // starting from the top (0xffff).
        // The _pidSegments variable is initially empty, and is created by looping 
        // through all the property definitions.
      
        if (!_pidSegmentsInitialised)
        {
            // initialise the pid segments vector using the properties
            // already registered in the dictionary
            
            // make sure it is clear in case there was a previous (failed) attempt
            _pidSegments.clear();
            
            ImplEnumAAFClassDefsSP enumClassDefs;
            hr = GetClassDefs(&enumClassDefs);
            if (AAFRESULT_FAILED(hr))
            {
                return hr;
            }
            
            ImplAAFClassDefSP classDef;
            while (AAFRESULT_SUCCEEDED(enumClassDefs->NextOne(&classDef)))
            {
                ImplEnumAAFPropertyDefsSP enumPropDefs;
                hr = classDef->GetPropertyDefs(&enumPropDefs);
                if (AAFRESULT_FAILED(hr))
                {
                    return hr;
                }
                
                ImplAAFPropertyDefSP propDef;
                while (AAFRESULT_SUCCEEDED(enumPropDefs->NextOne(&propDef)))
                {
                    OMPropertyId pid = propDef->OmPid();
                    
                    if (pid < 0x8000) // static pid
                    {
                        continue;
                    }
                    
                    // add the dynamic pid to the vector of pid segments
                    bool haveProcessedPid = false;
                    OMVectorIterator<PIDSegment> iter(_pidSegments, OMBefore);
                    while (++iter)
                    {
                        PIDSegment& segment = iter.value();
                        
                        if (pid >= segment.firstPid && pid <= segment.lastPid)
                        {
                            // this shouldn't happen - a pid should be unique within a file
                            // ASSERTU(pid < segment.firstPid && pid > segment.lastPid);
                            haveProcessedPid = true;
                            break;
                        }

                        if (pid < segment.firstPid - 1)
                        {
                            // the pid is between this segment and the previous segment
                            PIDSegment newSegment;
                            newSegment.firstPid = pid;
                            newSegment.lastPid = pid;
                            _pidSegments.insertAt(newSegment, iter.index());
                            haveProcessedPid = true;
                            break;
                        }
                        else if (pid == segment.firstPid - 1)
                        {
                            // extend the segment back 1
                            segment.firstPid = pid;
                            haveProcessedPid = true;
                            break;
                        }
                        else if (pid == segment.lastPid + 1)
                        {
                            // extend the segment forwards 1
                            segment.lastPid = pid;

                            // check whether the extension takes us into the next segment
                            if (++iter)
                            {
                                PIDSegment& nextSegment = iter.value();
                                if (pid + 1 >= nextSegment.firstPid)
                                {
                                    // merge the segments and remove the next segment
                                    segment.lastPid = nextSegment.lastPid;
                                    _pidSegments.removeAt(iter.index());
                                }
                            }
                            haveProcessedPid = true;
                            break;
                        }
                    }
                    
                    if (!haveProcessedPid)
                    {
                        // pid is beyond the last segment
                        PIDSegment newSegment;
                        newSegment.firstPid = pid;
                        newSegment.lastPid = pid;
                        _pidSegments.append(newSegment);
                    }
                }
            }
            
            _pidSegmentsInitialised = true;
        }
      
        // generate a new pid 
        
        if (_pidSegments.count() == 0)
        {
            // this is the first dynamic pid; we start from the top
            rOutPid = 0xffff;
            
            PIDSegment newSegment;
            newSegment.firstPid = rOutPid;
            newSegment.lastPid = rOutPid;
            _pidSegments.append(newSegment);
        }
        else
        {
            // extend the last segment with a new pid
            
            OMVectorIterator<PIDSegment> iter(_pidSegments, OMAfter);
            --iter;
            PIDSegment& lastSegment = iter.value();
            if (lastSegment.lastPid < 0xffff)
            {
                // the new pid becomes one above the last pid in the last segment
                lastSegment.lastPid++;
                rOutPid = lastSegment.lastPid;
            }
            else
            {
                // the new pid becomes one below the first pid in the last segment
                lastSegment.firstPid--;
                rOutPid = lastSegment.firstPid;
                
                // check whether the segment must be merged with the previous one
                if (--iter)
                {
                    PIDSegment& prevSegment = iter.value();
                    if (prevSegment.lastPid + 1 >= lastSegment.firstPid)
                    {
                        // merge the segments and remove the previous segment
                        lastSegment.firstPid = prevSegment.firstPid;
                        _pidSegments.removeAt(iter.index());
                    }
                }
            }
        }
        
        ASSERTU(rOutPid >= 0x8000);
    }
    
    return AAFRESULT_SUCCESS;
}

void ImplAAFDictionary::pvtAttemptBuiltinSizeRegistration
  (ImplAAFTypeDefEnum * ptde) const
{
  ImplAAFBuiltinTypes::RegisterExistingType (ptde);
}


void ImplAAFDictionary::pvtAttemptBuiltinSizeRegistration
  (ImplAAFTypeDefRecord * ptdr) const
{
  ImplAAFBuiltinTypes::RegisterExistingType (ptdr);
}


void ImplAAFDictionary::AssurePropertyTypes (ImplAAFClassDef * pcd)
{
  ASSERTU (pcd);
// All axiomatic definitions have already been loaded all other 
// property and types can be loaded "lazily" if necessary.
// Why do we need this stuff??? transdel 2000-DEC-20
  if (_OKToAssurePropTypes)
	{
	  pcd->AssurePropertyTypesLoaded ();
	}
}


void ImplAAFDictionary::AssureClassPropertyTypes ()
{
  AAFRESULT hr;
  ImplEnumAAFClassDefsSP enumClassDefs;
  ImplAAFClassDefSP classDef;

  _OKToAssurePropTypes = true;

  hr = GetClassDefs (&enumClassDefs);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));

  // do registered (normal) classes
  while (AAFRESULT_SUCCEEDED
		 (enumClassDefs->NextOne (&classDef)))
	{
	  ASSERTU (classDef);
	  classDef->AssurePropertyTypesLoaded ();
	  classDef = 0;
	}
}


bool ImplAAFDictionary::SetEnableDefRegistration (bool isEnabled)
{
  bool retval = _defRegistrationAllowed;
  _defRegistrationAllowed = isEnabled;
  return retval;
}


ImplAAFBuiltinDefs * ImplAAFDictionary::GetBuiltinDefs ()
{
  if (! _pBuiltinDefs)
	{
	  _pBuiltinDefs = new ImplAAFBuiltinDefs (this);
	}
  ASSERTU (_pBuiltinDefs);
  return _pBuiltinDefs;
}

// Initialize all of the axiomatic and required built-in definitions
// have been initialized. This should be called after the file has been opened.
void ImplAAFDictionary::InitializeMetaDefinitions(void)
{
  if (!_metaDefinitionsInitialized)
  {
    _metaDefinitionsInitialized = true;

      //
      // TEMPORARY:
      // Initialize the built-in types and classes if necessary.
      //

    if (!_pBuiltinTypes)
      _pBuiltinTypes   = new ImplAAFBuiltinTypes (this);
    ASSERTU (_pBuiltinTypes);

    if (!_pBuiltinClasses)
      _pBuiltinClasses = new ImplAAFBuiltinClasses (this);
    ASSERTU (_pBuiltinClasses);

  }
}


AAFRESULT ImplAAFDictionary::MergeTo( ImplAAFDictionary* pDestDictionary )
{
    ASSERTU( pDestDictionary );


    ImplEnumAAFClassDefs* pEnumSrcClassDefs = NULL;
    AAFRESULT hr = GetClassDefs( &pEnumSrcClassDefs );
    if( AAFRESULT_SUCCEEDED(hr) )
    {
        ImplAAFClassDef* pSrcClassDef = NULL;
        while( AAFRESULT_SUCCEEDED(pEnumSrcClassDefs->NextOne(&pSrcClassDef)) )
        {
            pSrcClassDef->MergeTo( pDestDictionary );
            pSrcClassDef->ReleaseReference();
            pSrcClassDef = NULL;
        }

        pEnumSrcClassDefs->ReleaseReference();
        pEnumSrcClassDefs = NULL;
    }


    return hr;
}


/*************************************************************************
    aafLookupTypeDef()

	This helper function searches for specified type definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_typedef	- type definition to look for.

    Returns:
	kAAFTrue - type definition found in given objects dictionary.
	kAAFFalse - type def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupTypeDef( 
    ImplAAFObject	*p_holder,
    ImplAAFTypeDef		*p_typedef )
{
    ASSERTU( p_holder );
    ASSERTU( p_typedef );

    AAFRESULT		hr = AAFRESULT_TYPE_NOT_FOUND; // Important init.
    aafUID_t		typedef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the type def we're looking for.
    p_typedef->GetAUID( &typedef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFTypeDef		*p_tmp_typedef = NULL;

	hr = p_dict->LookupTypeDef( typedef_id, &p_tmp_typedef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_typedef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupOperationDef()

	This helper function searches for specified operation definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_operdef	- operation definition to look for.

    Returns:
	kAAFTrue - operation definition found in given objects dictionary.
	kAAFFalse - operation def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupOperationDef( 
    ImplAAFObject	*p_holder,
    ImplAAFOperationDef		*p_operdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_operdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		operdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the operation def we're looking for.
    p_operdef->GetAUID( &operdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFOperationDef		*p_tmp_operdef = NULL;

	hr = p_dict->LookupOperationDef( operdef_id, &p_tmp_operdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_operdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupParameterDef()

	This helper function searches for specified parameter definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_paramdef	- parameter definition to look for.

    Returns:
	kAAFTrue - parameter definition found in given objects dictionary.
	kAAFFalse - parameter def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupParameterDef( 
    ImplAAFObject	*p_holder,
    ImplAAFParameterDef		*p_paramdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_paramdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		paramdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the parameter def we're looking for.
    p_paramdef->GetAUID( &paramdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFParameterDef		*p_tmp_paramdef = NULL;

	hr = p_dict->LookupParameterDef( paramdef_id, &p_tmp_paramdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_paramdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupClassDef()

	This helper function searches for specified class definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_classdef	- class definition to look for.

    Returns:
	kAAFTrue - class definition found in given objects dictionary.
	kAAFFalse - class def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupClassDef( 
    ImplAAFObject	*p_holder,
    ImplAAFClassDef		*p_classdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_classdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		classdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the class def we're looking for.
    p_classdef->GetAUID( &classdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFClassDef		*p_tmp_classdef = NULL;

	hr = p_dict->LookupClassDef( classdef_id, &p_tmp_classdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_classdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupDataDef()

	This helper function searches for specified data definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_datadef	- data definition to look for.

    Returns:
	kAAFTrue - data definition found in given objects dictionary.
	kAAFFalse - data def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupDataDef( 
    ImplAAFObject	*p_holder,
    ImplAAFDataDef		*p_datadef )
{
    ASSERTU( p_holder );
    ASSERTU( p_datadef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		datadef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the data def we're looking for.
    p_datadef->GetAUID( &datadef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFDataDef		*p_tmp_datadef = NULL;

	hr = p_dict->LookupDataDef( datadef_id, &p_tmp_datadef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_datadef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupCodecDef()

	This helper function searches for specified codec definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_codecdef	- codec definition to look for.

    Returns:
	kAAFTrue - codec definition found in given objects dictionary.
	kAAFFalse - codec def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupCodecDef( 
    ImplAAFObject	*p_holder,
    ImplAAFCodecDef		*p_codecdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_codecdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		codecdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the codec def we're looking for.
    p_codecdef->GetAUID( &codecdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFCodecDef		*p_tmp_codecdef = NULL;

	hr = p_dict->LookupCodecDef( codecdef_id, &p_tmp_codecdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_codecdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupContainerDef()

	This helper function searches for specified container definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_containerdef	- container definition to look for.

    Returns:
	kAAFTrue - container definition found in given objects dictionary.
	kAAFFalse - container def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupContainerDef( 
    ImplAAFObject	*p_holder,
    ImplAAFContainerDef		*p_containerdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_containerdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		containerdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the container def we're looking for.
    p_containerdef->GetAUID( &containerdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFContainerDef		*p_tmp_containerdef = NULL;

	hr = p_dict->LookupContainerDef( containerdef_id, &p_tmp_containerdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_containerdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupInterpolationDef()

	This helper function searches for specified interpolation definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_interpoldef	- interpolation definition to look for.

    Returns:
	kAAFTrue - interpolation definition found in given objects dictionary.
	kAAFFalse - interpolation def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupInterpolationDef( 
    ImplAAFObject	*p_holder,
    ImplAAFInterpolationDef		*p_interpoldef )
{
    ASSERTU( p_holder );
    ASSERTU( p_interpoldef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		interpoldef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the interpolation def we're looking for.
    p_interpoldef->GetAUID( &interpoldef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFInterpolationDef		*p_tmp_interpoldef = NULL;

	hr = p_dict->LookupInterpolationDef( interpoldef_id, &p_tmp_interpoldef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_interpoldef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/****/
/*************************************************************************
    aafLookupTypeDef()

	This helper function searches for specified type definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_typedef	- type definition to look for.

    Returns:
	kAAFTrue - type definition found in given objects dictionary.
	kAAFFalse - type def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupTypeDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFTypeDef		*p_typedef )
{
    ASSERTU( p_holder );
    ASSERTU( p_typedef );

    AAFRESULT		hr = AAFRESULT_TYPE_NOT_FOUND; // Important init.
    aafUID_t		typedef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the type def we're looking for.
    p_typedef->GetAUID( &typedef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFTypeDef		*p_tmp_typedef = NULL;

	hr = p_dict->LookupTypeDef( typedef_id, &p_tmp_typedef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_typedef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupOperationDef()

	This helper function searches for specified operation definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_operdef	- operation definition to look for.

    Returns:
	kAAFTrue - operation definition found in given objects dictionary.
	kAAFFalse - operation def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupOperationDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFOperationDef		*p_operdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_operdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		operdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the operation def we're looking for.
    p_operdef->GetAUID( &operdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFOperationDef		*p_tmp_operdef = NULL;

	hr = p_dict->LookupOperationDef( operdef_id, &p_tmp_operdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_operdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupParameterDef()

	This helper function searches for specified parameter definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_paramdef	- parameter definition to look for.

    Returns:
	kAAFTrue - parameter definition found in given objects dictionary.
	kAAFFalse - parameter def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupParameterDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFParameterDef		*p_paramdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_paramdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		paramdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the parameter def we're looking for.
    p_paramdef->GetAUID( &paramdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFParameterDef		*p_tmp_paramdef = NULL;

	hr = p_dict->LookupParameterDef( paramdef_id, &p_tmp_paramdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_paramdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupClassDef()

	This helper function searches for specified class definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_classdef	- class definition to look for.

    Returns:
	kAAFTrue - class definition found in given objects dictionary.
	kAAFFalse - class def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupClassDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFClassDef		*p_classdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_classdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		classdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the class def we're looking for.
    p_classdef->GetAUID( &classdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFClassDef		*p_tmp_classdef = NULL;

	hr = p_dict->LookupClassDef( classdef_id, &p_tmp_classdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_classdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupDataDef()

	This helper function searches for specified data definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_datadef	- data definition to look for.

    Returns:
	kAAFTrue - data definition found in given objects dictionary.
	kAAFFalse - data def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupDataDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFDataDef		*p_datadef )
{
    ASSERTU( p_holder );
    ASSERTU( p_datadef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		datadef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the data def we're looking for.
    p_datadef->GetAUID( &datadef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFDataDef		*p_tmp_datadef = NULL;

	hr = p_dict->LookupDataDef( datadef_id, &p_tmp_datadef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_datadef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupCodecDef()

	This helper function searches for specified codec definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_codecdef	- codec definition to look for.

    Returns:
	kAAFTrue - codec definition found in given objects dictionary.
	kAAFFalse - codec def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupCodecDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFCodecDef		*p_codecdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_codecdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		codecdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the codec def we're looking for.
    p_codecdef->GetAUID( &codecdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFCodecDef		*p_tmp_codecdef = NULL;

	hr = p_dict->LookupCodecDef( codecdef_id, &p_tmp_codecdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_codecdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupContainerDef()

	This helper function searches for specified container definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_containerdef	- container definition to look for.

    Returns:
	kAAFTrue - container definition found in given objects dictionary.
	kAAFFalse - container def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupContainerDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFContainerDef		*p_containerdef )
{
    ASSERTU( p_holder );
    ASSERTU( p_containerdef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		containerdef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the container def we're looking for.
    p_containerdef->GetAUID( &containerdef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFContainerDef		*p_tmp_containerdef = NULL;

	hr = p_dict->LookupContainerDef( containerdef_id, &p_tmp_containerdef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_containerdef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



/*************************************************************************
    aafLookupInterpolationDef()

	This helper function searches for specified interpolation definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_interpoldef	- interpolation definition to look for.

    Returns:
	kAAFTrue - interpolation definition found in given objects dictionary.
	kAAFFalse - interpolation def is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupInterpolationDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFInterpolationDef		*p_interpoldef )
{
    ASSERTU( p_holder );
    ASSERTU( p_interpoldef );

    AAFRESULT		hr = AAFRESULT_OBJECT_NOT_FOUND; // Important init.
    aafUID_t		interpoldef_id;
    ImplAAFDictionary	*p_dict = NULL;


    // Get UID of the interpolation def we're looking for.
    p_interpoldef->GetAUID( &interpoldef_id );

    if( p_holder->GetDictionary( &p_dict ) == AAFRESULT_SUCCESS )
    {
	ImplAAFInterpolationDef		*p_tmp_interpoldef = NULL;

	hr = p_dict->LookupInterpolationDef( interpoldef_id, &p_tmp_interpoldef );
	if( hr == AAFRESULT_SUCCESS )
	    p_tmp_interpoldef->ReleaseReference();

	p_dict->ReleaseReference();
    }


    return (hr == AAFRESULT_SUCCESS ? kAAFTrue : kAAFFalse);
}



