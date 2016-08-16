//=---------------------------------------------------------------------=
//
// $Id: ImplAAFObject.cpp,v 1.87 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


#ifndef __ImplEnumAAFProperties_h__
#include "ImplEnumAAFProperties.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplEnumAAFPropertyDefs> ImplEnumAAFPropertyDefsSP;
typedef ImplAAFSmartPointer<ImplEnumAAFProperties> ImplEnumAAFPropertiesSP;

#ifndef __ImplAAFBaseClassFactory_h__
#include "ImplAAFBaseClassFactory.h"
#endif


#include "OMAssertions.h"
#include "aafErr.h"
#include "AAFResult.h"
#include "OMFile.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFClassDef.h"
#include "ImplAAFContext.h"
#include "ImplAAFModule.h"
#include "ImplAAFProperty.h"
#include "ImplAAFPropertyDef.h"
#include "ImplAAFPropertyValue.h"
#include "ImplAAFPropValData.h"
#include "ImplEnumAAFProperties.h"

#include "ImplAAFObjectCreation.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "OMPropertyDefinition.h"
#include "AAFUtils.h"
#include "AAFObjectModel.h"



extern "C" const aafClassID_t CLSID_AAFProperty;
extern "C" const aafClassID_t CLSID_EnumAAFProperties;

//
// Private class for implementing collections of properties.
//
class ImplPropertyCollection //: public ImplAAFCollection<ImplAAFProperty *>
{
public:
  ImplPropertyCollection ();
  virtual ~ImplPropertyCollection ();

  // overrides
  virtual AAFRESULT
    GetNumElements
        (aafUInt32 * pCount);

  AAFRESULT AddPropertyDef (ImplAAFPropertyDef * pPropDef );

  AAFRESULT SetPropertyValue (ImplAAFPropertyDef * pPropDef,
						 ImplAAFPropertyValue * pNewPropVal);

  AAFRESULT RemovePropertyValue(ImplAAFPropertyDef *pPropDef);

  AAFRESULT
    Initialize (ImplAAFObject * pObj,
				OMPropertySet * pOMPropSet);

  AAFRESULT LookupOMProperty(const OMPropertyId& pid,OMProperty **ppOMProperty);
  
  OMReferenceSetIterator<OMPropertyId, ImplAAFProperty> * GetProperties() const;
  
  AAFRESULT CreatePropertyInstance(ImplAAFPropertyDef * pPropDef, OMProperty *pOmProp, ImplAAFProperty **ppProp) const;

  AAFRESULT AddProperty(ImplAAFProperty *pProp);
  
  ImplAAFProperty * FindProperty(OMPropertyId pid) const; // not reference counted
  
  AAFRESULT SynchronizeProperty(ImplAAFObject * pObj, ImplAAFPropertyDef *pPropDef);
  
private:
  aafUInt32           _numAllocated;
  OMPropertySet     * _pOMPropSet;
  OMReferenceSet<OMPropertyId, ImplAAFProperty> _properties;
};

ImplPropertyCollection::ImplPropertyCollection ()
  : _pOMPropSet (0)
{}

ImplPropertyCollection::~ImplPropertyCollection ()
{
  // Cleanup any allocated ImplProperties...
  ImplAAFProperty * pProperty;
  OMReferenceSetIterator<OMPropertyId, ImplAAFProperty> iter(_properties);
  while (++iter)
  {
    pProperty = iter.setValue(NULL);
    if (pProperty)
      pProperty->ReleaseReference();
  }
}

AAFRESULT ImplPropertyCollection::GetNumElements
(aafUInt32 * pCount)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;
  *pCount = _properties.count();
  return AAFRESULT_SUCCESS;
}

//
// 1) See if there is an OMProperty in this OMPropertySet
//    corresponding to the given OMPropertyId.  If not, it's an
//    error.  If so, remember that OMProperty.
//
// 2) See if there is an existing ImplAAFProperty in _pProperties
//    corresponding to the given OMPropertyId.  If so, remember it.
//    If not, create one, and initialize it, and remember it.
//
// 3) Set the remembered property to contain the new property value.
//
// 4) Set the remembered OMProperty's bits to the new prop value's
//    bits.
//

AAFRESULT ImplPropertyCollection::AddPropertyDef(ImplAAFPropertyDef * pPropDef )
{
  if(!pPropDef)
	  return(AAFRESULT_NULL_PARAM);

  if(!_pOMPropSet)
    return(AAFRESULT_NOT_INITIALIZED);

  //
  // 1) See if there is an OMProperty in this OMPropertySet
  //    corresponding to the given OMPropertyId.  If not, it's an
  //    error.  If so, remember that OMProperty.
  //
  OMProperty *pOmProp;
  AAFRESULT ar=LookupOMProperty(pPropDef->OmPid(),&pOmProp);
  if(AAFRESULT_FAILED(ar))
	  return(ar);
	ASSERTU (pOmProp);

  //
  // 2) See if there is an existing ImplAAFProperty in _pProperties
  //    corresponding to the given OMPropertyId.  If so, remember it.
  //    If not, create one, initialize it, and remember it.
  //
  ImplAAFProperty * pProp = FindProperty(pPropDef->OmPid());

  if (! pProp)
	{
	  // There was no existing property in the collection.  Create and
	  // append a new one.
	  ImplAAFPropertySP pNewProp;
	  ar = CreatePropertyInstance(pPropDef, pOmProp, &pNewProp);
	  if (AAFRESULT_FAILED (ar))
	    return ar;
	  
	  ar = AddProperty(pNewProp);
	  if (AAFRESULT_FAILED (ar))
	    return ar;
	  pProp = pNewProp; // property is now owned by the collection.
	}

  return(ar);
}

//
// 1) See if there is an OMProperty in this OMPropertySet
//    corresponding to the given OMPropertyId.  If not, it's an
//    error.  If so, remember that OMProperty.
//
// 2) See if there is an existing ImplAAFProperty in _pProperties
//    corresponding to the given OMPropertyId.  If so, remember it.
//    If not, create one, and initialize it, and remember it.
//
// 3) Set the remembered property to contain the new property value.
//
// 4) Set the remembered OMProperty's bits to the new prop value's
//    bits.
//
AAFRESULT ImplPropertyCollection::SetPropertyValue
(
 ImplAAFPropertyDef * pPropDef,
 ImplAAFPropertyValue * pNewPropVal
)
{
  if(!pPropDef||!pNewPropVal)
	  return(AAFRESULT_NULL_PARAM);

  if(!_pOMPropSet)
    return(AAFRESULT_NOT_INITIALIZED);

  //
  // 1) See if there is an OMProperty in this OMPropertySet
  //    corresponding to the given OMPropertyId.  If not, it's an
  //    error.  If so, remember that OMProperty.
  //
  OMProperty *pOmProp;
  AAFRESULT ar=LookupOMProperty(pPropDef->OmPid(),&pOmProp);
  if(AAFRESULT_FAILED(ar))
	  return(ar);
	ASSERTU (pOmProp);

  //
  // 2) See if there is an existing ImplAAFProperty in _pProperties
  //    corresponding to the given OMPropertyId.  If so, remember it.
  //    If not, create one, initialize it, and remember it.
  //
  ImplAAFProperty * pProp = FindProperty(pPropDef->OmPid());

  if (! pProp)
	{
	  // There was no existing property in the collection.  Create and
	  // append a new one.
	  ImplAAFPropertySP pNewProp;
    ar = CreatePropertyInstance(pPropDef, pOmProp, &pNewProp);
	  if (AAFRESULT_FAILED (ar))
	    return ar;
	  
	  ar = AddProperty(pNewProp);
	  if (AAFRESULT_FAILED (ar))
	    return ar;
	  pProp = pNewProp; // property is now owned by the collection.
	}

  //
  // 3) Set the remembered property to contain the new property value.
  //
  ASSERTU (pProp);
  ar = pProp->pvtSetValue (pNewPropVal);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));
	  
  //
  // 4) Set the remembered OMProperty's bits to the new prop value's
  // bits.
  //
  ar = pNewPropVal->WriteTo(pOmProp);
//  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  return(ar);
}

AAFRESULT ImplPropertyCollection::RemovePropertyValue(ImplAAFPropertyDef *pPropDef)
{
  if(!pPropDef)
    return(AAFRESULT_NULL_PARAM);

  if(!_pOMPropSet)
    return(AAFRESULT_NOT_INITIALIZED);

  // Make sure 'pPropDef' is a definition for a property which is defined in
  // this collection.
  OMProperty *pOMProperty;
  AAFRESULT ar=LookupOMProperty(pPropDef->OmPid(),&pOMProperty);
  if(AAFRESULT_FAILED(ar))
	  return(ar);

  // If 'pPropDef' is a definition for a property which is also *present* in 
  // this collection, we remove it here.
  ImplAAFProperty * pProp = FindProperty(pPropDef->OmPid());
  if (NULL != pProp)
  {
    _properties.removeValue(pProp);
    pProp->ReleaseReference();
    pProp = NULL;
    

	  // Remove the OM property as well.
	  ASSERTU(pOMProperty->isOptional()==kAAFTrue);
	  pOMProperty->removeProperty();    
  }


  return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplPropertyCollection::SynchronizeProperty
(
 ImplAAFObject * pObj,
 ImplAAFPropertyDef *pPropDef
)
{
  AAFRESULT ar = AAFRESULT_SUCCESS;
  OMProperty * pOmProp = NULL;
	OMPropertyId opid = pPropDef->OmPid ();
  if (PID_InterchangeObject_ObjClass == opid) // objclass isn't a real property yet...
	  return AAFRESULT_SUCCESS;
	
	// If the pid is already in the set we don't need to create an new element. 
	if (_properties.contains(opid))
	  return AAFRESULT_SUCCESS;

	
	if (!_pOMPropSet->isPresent(opid))
	{
	  // If property is not present then make sure that it gets created and
	  // added to the object's property set.
	  pOmProp = pObj->InitOMProperty(pPropDef, _pOMPropSet);
	  if (NULL == pOmProp)
	    return AAFRESULT_NOMEMORY;
	}
	else
	{
	  pOmProp = _pOMPropSet->get (opid);
    ASSERTU (pOmProp);
  }
  
  // If the property is optional and not present then do not create the ImplAAFProperty
  // cache.
  if (pOmProp->isOptional() && !pOmProp->isPresent())
		return AAFRESULT_SUCCESS;
	
  ImplAAFPropertySP tmp;
  ar = CreatePropertyInstance(pPropDef, pOmProp, &tmp);
  if (AAFRESULT_FAILED (ar))
	  return ar; 
  ar = AddProperty(tmp);
  if (AAFRESULT_FAILED (ar))
    return ar;
  
  return ar;
}


AAFRESULT ImplPropertyCollection::Initialize
(
 ImplAAFObject * pObj,
 OMPropertySet * pOMPropSet
)
{
  ImplAAFPropertyDefSP pPropDef;

  // make sure we haven't called this before
  ASSERTU (0 == _properties.count());

  if (! pObj)
	  return AAFRESULT_NULL_PARAM;
  if (! pOMPropSet)
	return AAFRESULT_NULL_PARAM;

  _pOMPropSet = pOMPropSet;

  ImplAAFClassDefSP pClassDef;
  AAFRESULT ar = AAFRESULT_SUCCESS;
  ASSERTU (pObj);
  ar = pObj->GetDefinition(&pClassDef);
  if (AAFRESULT_FAILED(ar)) return ar;
  ASSERTU (pClassDef);

  while (1)
	{
	  ImplEnumAAFPropertyDefsSP pPropEnum;
	  ar = pClassDef->GetPropertyDefs (&pPropEnum);
	  if (AAFRESULT_FAILED(ar)) throw ar;
	  while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pPropDef)))
		{
      ar = SynchronizeProperty(pObj, pPropDef);
      if (AAFRESULT_FAILED(ar)) return ar;
		}
	  ImplAAFClassDefSP pParent;
	  ar = pClassDef->GetParent (&pParent);
	  if (AAFRESULT_IS_ROOT_CLASS == ar)
		  break;
	  if (AAFRESULT_FAILED (ar)) return ar;
	  pClassDef = pParent;
	}

  if (AAFRESULT_IS_ROOT_CLASS == ar)
    ar = AAFRESULT_SUCCESS;

  return ar;
}

AAFRESULT ImplPropertyCollection::LookupOMProperty(const OMPropertyId& pid,
												   OMProperty **ppOMProperty)
{
  if(!ppOMProperty)
	  return(AAFRESULT_NULL_PARAM);

  if (!_pOMPropSet->isPresent(pid))
	  return(AAFRESULT_PROP_NOT_PRESENT);
  
  *ppOMProperty = 0;
  *ppOMProperty = _pOMPropSet->get(pid);
  ASSERTU(*ppOMProperty);
  
  return(AAFRESULT_SUCCESS);
}


OMReferenceSetIterator<OMPropertyId, ImplAAFProperty> * ImplPropertyCollection::GetProperties() const
{
  return new OMReferenceSetIterator<OMPropertyId, ImplAAFProperty>(_properties);  
}


AAFRESULT ImplPropertyCollection::CreatePropertyInstance(ImplAAFPropertyDef * pPropDef, OMProperty *pOmProp, ImplAAFProperty **ppProp) const
{
  AAFRESULT ar = AAFRESULT_SUCCESS;
  ASSERTU (pPropDef && pOmProp && ppProp); // internal function.
  
	ImplAAFProperty * pProp = (ImplAAFProperty*) CreateImpl (CLSID_AAFProperty);
  if (! pProp) 
	  return AAFRESULT_NOMEMORY;

  ar = pProp->Initialize (pPropDef, pOmProp);
  if (AAFRESULT_SUCCEEDED (ar))
  {
    *ppProp = pProp; // return reference counted object.
  }
  else
  {
    pProp->ReleaseReference();
    pProp = NULL;
  }

  return ar;  
}


AAFRESULT ImplPropertyCollection::AddProperty(ImplAAFProperty *pProp)
{
  if (NULL == pProp)
    return AAFRESULT_NULL_PARAM;
  if (_properties.containsValue(pProp))
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  _properties.insert(pProp);
  pProp->AcquireReference();
  
  return(AAFRESULT_SUCCESS);
}

  
ImplAAFProperty * ImplPropertyCollection::FindProperty(OMPropertyId pid) const // not reference counted
{
  ImplAAFProperty * pProp = NULL;
  if (_properties.find(pid, pProp))
    return pProp; // not reference counted
  else
    return NULL;
}



ImplAAFObject::ImplAAFObject ()
  : _generation(PID_InterchangeObject_Generation, L"Generation"),
	_pProperties (0),
	_apSavedProps (0),
	_savedPropsSize (0),
	_savedPropsCount (0)
{
  _persistentProperties.put(_generation.address());

  const aafUID_t null_uid = { 0 };
  _soid = null_uid;
}


ImplAAFObject::SavedProp::SavedProp (OMProperty * p)
  : _p(p)
{
  ASSERTU (p);
}



ImplAAFObject::SavedProp::~SavedProp ()
{
  ASSERTU (_p);
  
  // NOTE: We need a better way to release strong object references
  // contained in optional properties! 
  
  // The template argument here *must* match the type allocated in
  // ImplAAFTypeDefFixedArray::pvtCreateOMProperty() and
  // ImplAAFTypeDefVariableArray::pvtCreateOMProperty().  
  OMStrongReferenceVectorProperty<ImplAAFObject> * srv =
	dynamic_cast<OMStrongReferenceVectorProperty <ImplAAFObject>*>(_p);
  if (srv)
	{
	  aafUInt32 count = srv->count();
	  for (aafUInt32 i = 0; i < count; i++)
		{
		  ImplAAFObject* oldObj = srv->clearValueAt(i);
		  if (oldObj)
			{
			  oldObj->ReleaseReference ();
			  oldObj = 0;
			}
		}
	}
  else
	{
	  // The template argument here *must* match the type
	  // allocated in
	  // ImplAAFTypeDefStrongObjRef::pvtCreateOMProperty().
	  OMStrongReferenceProperty<ImplAAFObject> * sro =
		dynamic_cast<OMStrongReferenceProperty<ImplAAFObject>*>(_p);
	  if (sro)
		{
		  ImplAAFObject* oldObj = sro->clearValue();
		  if (oldObj)
			{
			  oldObj->ReleaseReference ();
			  oldObj = 0;
			}
		}
		else
		{
		  // This code is elaborate because there is currently no "non-template strong reference set"
		  // property that can be used in a simple dynamic_cast. 2001-JAN-22: trr.
		  // OMStrongReferenceSetProperty * srs = dynamic_cast<OMStrongReferenceSetProperty *>(_p); // ???
		  // 
		  
		  // Check for definition object set (dos).
		  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFDefObject> * dos =
		    dynamic_cast<OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFDefObject> *>(_p);
      if (NULL != dos)
      {
      	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDefObject>defObjects(*dos);
      	while(++defObjects)
      	{
      		ImplAAFDefObject *pDefObject = defObjects.clearValue();
      		if (pDefObject)
      		{
      		  pDefObject->ReleaseReference();
      		  pDefObject = 0;
      		}
      	}
      }
    }
	}
  delete _p;
}



ImplAAFObject::~ImplAAFObject ()
{
  if (_pProperties)
	delete _pProperties;

  for (size_t i = 0;
	   i <_savedPropsCount;
	   i++)
	{
	  delete _apSavedProps[i];
	}
  delete [] _apSavedProps;
}


void ImplAAFObject::RememberAddedProp (OMProperty * pProp)
{
  ASSERTU (pProp);

  // make sure that number of slots allocated (size) is no smaller
  // than number used (count).
  ASSERTU (_savedPropsSize >= _savedPropsCount);

  // If we need a new one, allocate more.
  if (_savedPropsSize == _savedPropsCount)
	{
	  // Need more.  We'll allocate in chunks of 10 props.
	  aafUInt32 newSize = _savedPropsSize + 10;
	  SavedProp ** pNewSavedProps = new SavedProp*[newSize];
	  // Clear the allocated ones.
	  memset (pNewSavedProps, 0, sizeof (SavedProp*) * newSize);
	  // Copy over the ones already allocated.
	  for (size_t i = 0; i < _savedPropsCount; i++)
		{
		  pNewSavedProps[i] = _apSavedProps[i];
		}
	  // ditch the old storage
	  delete [] _apSavedProps;
	  // zero this to keep Bounds Checker happy
	  _apSavedProps = 0;
	  // remember the new storage
	  _apSavedProps = pNewSavedProps;
	  // zero this to keep Bounds Checker happy
	  pNewSavedProps = 0;
	  _savedPropsSize = newSize;
	}
  // We have the space.  Save the prop.
  ASSERTU (_savedPropsSize >= _savedPropsCount);
  _apSavedProps[_savedPropsCount] = new SavedProp(pProp);
  ASSERTU (_apSavedProps[_savedPropsCount]);
  _savedPropsCount++;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGeneration (ImplAAFIdentification ** ppResult)
{
  if (!ppResult)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT ar;

  if (! pvtIsGenerationTracked ())
	return AAFRESULT_INVALID_PARAM;

  aafUID_t gen;
  ar = GetGenerationAUID (&gen);
  if (AAFRESULT_FAILED (ar))
	return ar;

  ImplAAFHeaderSP pHead;
  ar = MyHeadObject (&pHead);
  if (AAFRESULT_FAILED (ar))
	// This object is not attached to a file.
	return AAFRESULT_OBJECT_NOT_ATTACHED;

  ASSERTU (ppResult);
  return pHead->LookupIdentification (gen, ppResult);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGenerationAUID (aafUID_t * pResult)
{
  /*
   * BobT note!  We may eventually implement
   * InterchangeObject::Generation as a weak reference to an
   * Identification object (instead of an AUID).  If so, then this
   * method will simply call GetGeneration() for the Identification,
   * and get the AUID from the returned Identification.  It will
   * probably look something like this:
   * 
   * if (!pResult)
   *   return AAFRESULT_NULL_PARAM;
   * 
   * ImplAAFIdentificationSP pId;
   * AAFRESULT ar;
   * ar = GetGeneration (&pId);
   * if (AAFRESULT_FAILED (ar))
   *   return ar;
   * 
   * return pId->GetGeneration (pResult);
   * 
   * Instead, the Generation property is implemented as an AUID so
   * we'll simply return the AUID from the property without going
   * through the Identification object.
   */

  if (!pResult)
	return AAFRESULT_NULL_PARAM;

  if (! pvtIsGenerationTracked())
	return AAFRESULT_INVALID_PARAM;

  ASSERTU (_generation.isPresent());
  *pResult = _generation;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetObjectClass (aafUID_t * pClass)
{
  if (NULL == pClass)
    return AAFRESULT_NULL_PARAM;

  //
  // Take advantage of the virtual method that 
  // returns the stored class id for a given class of OMStorable.
  //
#ifndef _DEBUG
  *pClass = *reinterpret_cast<const aafUID_t *>(&classId());
#else
  // In a debug build copy all of the fields separately so
  // that we can detect changes in the definitions of public aafUID_t
  // and private OMClassId types.
  const OMClassId& id = classId();
  pClass->Data1 = id.Data1;
  pClass->Data2 = id.Data2;
  pClass->Data3 = id.Data3;
  pClass->Data4[0] = id.Data4[0];
  pClass->Data4[1] = id.Data4[1];
  pClass->Data4[2] = id.Data4[2];
  pClass->Data4[3] = id.Data4[3];
  pClass->Data4[4] = id.Data4[4];
  pClass->Data4[5] = id.Data4[5];
  pClass->Data4[6] = id.Data4[6];
  pClass->Data4[7] = id.Data4[7];
#endif

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFObject::InitProperties ()
{
  try {

  if (! _pProperties)
	{
	  _pProperties = new ImplPropertyCollection;
	  if (! _pProperties)
		return AAFRESULT_NOMEMORY;
	  OMPropertySet * ps = propertySet();
	  ASSERTU (ps);
	  AAFRESULT ar = _pProperties->Initialize (this, ps);
	  if (AAFRESULT_FAILED (ar)) return ar;
	}

  }

  catch (...) {
	  // clean up and rethrow
	  if ( _pProperties ) {
		delete _pProperties;
		_pProperties = 0;
	  }
	  throw;
  }
  
  
  ASSERTU (_pProperties);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetProperties (ImplEnumAAFProperties ** ppEnum)
{
  if (! ppEnum)
	return AAFRESULT_NULL_PARAM;

  if(!_pProperties)
  {
    AAFRESULT ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
  }
  ASSERTU (_pProperties);
  
  ImplEnumAAFProperties * pEnum = NULL;
  pEnum = dynamic_cast<ImplEnumAAFProperties*>(CreateImpl(CLSID_EnumAAFProperties));
  if (!pEnum)
	return E_FAIL;
  ASSERTU (pEnum);
  
  OMReferenceSetIterator<OMPropertyId, ImplAAFProperty> * iter = _pProperties->GetProperties();
	if(iter == 0)
    return AAFRESULT_NOMEMORY;

  AAFRESULT ar = pEnum->Initialize (&CLSID_EnumAAFProperties, this, iter);
  if (! AAFRESULT_SUCCEEDED (ar)) return ar;
  
  ASSERTU (ppEnum);
  *ppEnum = pEnum;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::CountProperties (aafUInt32 * pCount)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;

  ASSERTU (pCount);

  if(!_pProperties)
  {
    AAFRESULT ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
  }

  ASSERTU (_pProperties);
  AAFRESULT ar = _pProperties->GetNumElements (pCount);
  if (! AAFRESULT_SUCCEEDED (ar)) return ar;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetPropertyValue (ImplAAFPropertyDef * pPropDef,
									 ImplAAFPropertyValue ** ppPropVal)
{
  if (! pPropDef)
	return AAFRESULT_NULL_PARAM;

  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT ar;
  if(!_pProperties)
  {
    ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
  }

  ImplAAFClassDefSP pClass;
  ar = GetDefinition (&pClass);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  ar = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (ar))
	return AAFRESULT_BAD_PROP;


  aafBoolean_t alreadyPresent;
  ar=IsPropertyPresent(pPropDef, &alreadyPresent);
  if(AAFRESULT_FAILED(ar))
	  return ar;

  if(alreadyPresent==kAAFFalse)
	  return AAFRESULT_PROP_NOT_PRESENT;

  // The property is in this object's OMPropertySet.
  // See if it the corresponding ImplAAFProperty has already
  // been cached.
  ImplAAFProperty * pProp = _pProperties->FindProperty(pid);
  if (NULL == pProp)
  {
    ar = _pProperties->SynchronizeProperty(this, pPropDef);
    if(AAFRESULT_FAILED(ar))
  	  return ar;
  	pProp = _pProperties->FindProperty(pid);
  }
  ASSERTU(pProp);
    
  return pProp->GetValue (ppPropVal);  
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::SetPropertyValue (ImplAAFPropertyDef * pPropDef,
									 ImplAAFPropertyValue * pPropVal)
{
  if (!pPropDef||!pPropVal)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT ar;
  if(!_pProperties)
  {
    ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
  }

  ImplAAFClassDefSP pClass;
  ar = GetDefinition (&pClass);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  ar = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (ar))
	return AAFRESULT_BAD_PROP;

  if (! _pProperties)
	{
	  ar = InitProperties();
	  if (AAFRESULT_FAILED (ar))
		return ar;
	}
  ASSERTU (_pProperties);


  // Make sure the given property exists in the collection
  ar = _pProperties->SynchronizeProperty(this, pPropDef);


  return(_pProperties->SetPropertyValue (pPropDef, pPropVal));
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::IsPropertyPresent (ImplAAFPropertyDef * pPropDef,
									  aafBoolean_t * pResult)
{
  if (! pPropDef)
	return AAFRESULT_NULL_PARAM;

  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  
  AAFRESULT ar = AAFRESULT_SUCCESS;

  ImplAAFClassDefSP pClass;
  ar = GetDefinition (&pClass);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  ar = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (ar))
	return AAFRESULT_BAD_PROP;

  // This method cannot have any side effects. For example no properties 
  // should be loaded into the DM. This can be accomplished by only
  // calling OM methods...
  *pResult = kAAFFalse; // default return value.
  bool found = propertySet()->isPresent(pid);
  if (found)
  {
    OMProperty * property = propertySet()->get(pid);
    if (property->isOptional())
    {
      if (property->isPresent())
      {
        *pResult = kAAFTrue;
      }
    }
    else
    { // required properties are always present
      *pResult = kAAFTrue;
    }
  }

  return ar;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::RemoveOptionalProperty (
      ImplAAFPropertyDef *pPropDef)
{
  if(!pPropDef)
    return(AAFRESULT_NULL_PARAM);

  AAFRESULT ar;

  aafBoolean_t alreadyPresent;
  ar=IsPropertyPresent(pPropDef, &alreadyPresent);
  if(AAFRESULT_FAILED(ar))
	  return ar;

  if(alreadyPresent==kAAFFalse)
	  return AAFRESULT_PROP_NOT_PRESENT;

  if(!_pProperties)
  {
    ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
  }
  ASSERTU(_pProperties);
  ar=_pProperties->RemovePropertyValue(pPropDef);
  if(AAFRESULT_FAILED(ar))
	  return ar;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::CreateOptionalPropertyValue (
      ImplAAFPropertyDef * pPropDef,
      ImplAAFPropertyValue ** ppPropVal)
{
  if (!pPropDef || !ppPropVal)
	  return AAFRESULT_NULL_PARAM;

  AAFRESULT ar = AAFRESULT_SUCCESS;
  if(!_pProperties)
  {
    ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
    ASSERTU (_pProperties);
  }

  *ppPropVal = NULL;
  ImplAAFTypeDefSP pPropertyType;
  ar=pPropDef->GetTypeDef(&pPropertyType);
  if(AAFRESULT_FAILED(ar))
	  return(ar);

  ImplAAFClassDefSP pClass;
  ar = GetDefinition (&pClass);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  ar = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (ar))
	return AAFRESULT_BAD_PROP;


  aafBoolean_t alreadyPresent;
  ar=IsPropertyPresent(pPropDef, &alreadyPresent);
  if(AAFRESULT_FAILED(ar))
	  return ar;

  if(alreadyPresent==kAAFTrue)
	  return AAFRESULT_PROP_ALREADY_PRESENT;



  // Make sure the given property exists and has been
  // initialized.
  OMProperty *pOMProperty = InitOMProperty(pPropDef, propertySet());
  if (!pOMProperty)
  {
    // Initialization failed! Determine the type of failure...
    if (pPropDef->OmPid() == PID_InterchangeObject_ObjClass)
      return (AAFRESULT_BAD_PROP);
    else
      return (AAFRESULT_NOMEMORY);
  }

  ar = pPropertyType->CreatePropertyValue(pOMProperty, ppPropVal);
  if(AAFRESULT_FAILED(ar))
	  return(ar);
  ASSERTU(NULL != *ppPropVal);
  
  // Make sure the new property value will be returned by a subsequent call
  // to GetPropertyValue. We need to do this because, among other things,
  // the property value is cached in the "property collection".
  return ar; //return (SetPropertyValue(pPropDef, *ppPropVal));
}


//************
// Interfaces ivisible inside the toolkit, but not exposed through the API

// Gets the head object of the file containing this object.
// This function is used to maintain MOB and Definition tables in the
// head object.
AAFRESULT ImplAAFObject::MyHeadObject
	(ImplAAFHeader **header) const
{
	OMFile			*myFile;
	OMStorable		*theRoot;
	ImplAAFHeader	*theHeader;

	XPROTECT()
	{
		if(header == NULL)
			RAISE(AAFRESULT_NULL_PARAM);

		if(!attached())
			RAISE(AAFRESULT_OBJECT_NOT_ATTACHED); 

                if(!inFile())
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE); 

		myFile = file();
		if(myFile == NULL)
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE);

		theRoot = myFile->clientRoot();
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);

		theHeader = dynamic_cast<ImplAAFHeader*>(theRoot);
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);
		theHeader->AcquireReference();

		*header = theHeader;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}

// Gets the dictionary used to create this instance.
AAFRESULT STDMETHODCALLTYPE
ImplAAFObject::GetDictionary(ImplAAFDictionary **ppDictionary) const
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


void ImplAAFObject::pvtSetSoid (const aafUID_t & id)
{
#if defined(OM_DEBUG)
  const aafUID_t null_uid = { 0 };

  // make sure it hasn't been set already
  ASSERTU (!EqualAUID (&id, &null_uid));

  // make sure new one is valid
  ASSERTU (EqualAUID (&_soid, &null_uid));
#endif
  _soid = id;
}


//
// Here is the mapping of DM type defs to OMProperty concrete
// classes.
//
// DM TypeDef				Treatment
// ----------				-------------------------
// AAFTypeDefEnum			FixedData(sizeof rep'd type)
//
// AAFTypeDefExtEnum		FixedData(sizeof auid)
//
// AAFTypeDefFixedArray     FixedData(sizeof elem * num elems)
//
// AAFTypeDefInt			FixedData(sizeof int)
//
// AAFTypeDefRecord         FixedData(sum of sizes of elements)
//
// AAFTypeDefRename         <refer to referenced type>
//
// AAFTypeDefSet:
//   if elem type is StrRef	OMStrongReferenceVectorProperty<AAFObject>
//   if elem type is WkRef  VariableData(sizeof auid)
//   if elem is fixed data  VariableData(sizeof elem)
//   else                   <not yet supported; maybe never!>
//
// AAFTypeDefString			VariableData(sizeof elem)
//
// AAFTypeDefStrongObjRef	OMStrongReferenceProperty<AAFObject>
//
// AAFTypeDefVariableArray
//   if elem type is StrRef	OMStrongReferenceVectorProperty<AAFObject>
// 	 if elem type is WkRef  VariableData(sizeof auid)
// 	 if elem is fixed data  VariableData(sizeof elem)
//   else                   <not yet supported; maybe never!>
//
// AAFTypeDefWeakObjRef     FixedData(sizeof auid)
//


// Associate the existing OMProperties with corresponding property definitions from
// the given class definition. NOTE: This call is recursive, it calls itself again
// for the parent class of the given class until current class is a "root" class.
void ImplAAFObject::InitOMProperties (ImplAAFClassDef * pClassDef)
{
  ASSERTU (pClassDef);
  AAFRESULT ar;


  //
  // Init base class properties first
  //
  ImplAAFClassDefSP parentSP;
  ar = pClassDef->GetParent (&parentSP);
  // check that only a "root" will have no parent class definition.
  ASSERTU (AAFRESULT_SUCCEEDED(ar) || (AAFRESULT_FAILED(ar) && AAFRESULT_IS_ROOT_CLASS == ar));
  if(AAFRESULT_SUCCEEDED(ar))
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
  ar = pClassDef->GetPropertyDefs (&pdEnumSP);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  ImplAAFPropertyDefSP propDefSP;
  while (AAFRESULT_SUCCEEDED (pdEnumSP->NextOne (&propDefSP)))
	{
    InitOMProperty(propDefSP, ps);
  }
}

  
// Same as above for a single property (not recursive).
OMProperty * ImplAAFObject::InitOMProperty(ImplAAFPropertyDef * pPropertyDef, OMPropertySet * ps)
{
  OMPropertyId defPid = pPropertyDef->OmPid ();
  // ASSERTU (ps->isAllowed (defPid));
  OMProperty * pProp = 0;

  if (ps->dynamicBuiltinIsPresent(pPropertyDef->identification()))
  {
    // the property is a built-in dynamic property
    
    // the property id of the built-in property definition needs to be set to 
    // the correct value if the property definition originated from a file
    ASSERTU(defPid != 0);
    HRESULT hr;
    ImplAAFDictionary* pDictionary = 0;
    hr = GetDictionary(&pDictionary);
    ASSERTU(AAFRESULT_SUCCEEDED(hr) && pDictionary != 0);
    pDictionary->associate(*reinterpret_cast<const aafUID_t*>(&pPropertyDef->identification()), 
      defPid);
    pDictionary->ReleaseReference();
    
    // the dynamic built-in property needs to be finalised with the property id
    ps->finaliseDynamicBuiltin(pPropertyDef->identification(), defPid);
  }

  if (ps->isPresent (defPid))
	{
	  // Defined property was already in property set.  (Most
	  // probably declared in the impl constructor.)  Get that
	  // property.
	  pProp = ps->get (defPid);
	}		  
	else if(defPid != PID_InterchangeObject_ObjClass
		/* && (defPid != PID_InterchangeObject_Generation)
		 && (defPid != PID_PropertyDefinition_DefaultValue) */)
	{
      // tjb - This code is intended for the case
      // of an extended property. However, this code will also
      // get executed for properties that are predefined
      // (in AAFMetaDictionary.h) but that are erroneously omitted
      // from the property set defined by the impl code. The predefined
      // properties are supposed to be declared in the impl header
      // and initialized in the impl constructor. 
#if 0
      // Assert that we're not trying to dynamically add a built-in 
      ASSERTU(AAFObjectModel::singleton()->findPropertyDefinition (reinterpret_cast<const aafUID_t *>(&pPropertyDef->identification()))->isNil());
#endif

	  // Defined property wasn't found in OM property set.
	  // We'll have to install one.
	  pProp = pPropertyDef->CreateOMProperty ();
	  ASSERTU (pProp);
	  
	  // Remember this property so we can delete it later.
	  RememberAddedProp (pProp);
	  
	  // Add the property to the property set.
	  ps->put (pProp);
  }
  
  if(defPid != PID_InterchangeObject_ObjClass
     /* && (defPid != PID_InterchangeObject_Generation)
        && (defPid != PID_PropertyDefinition_DefaultValue) */)
  {
  	  OMPropertyDefinition * pOMPropDef =
  		  dynamic_cast<OMPropertyDefinition*>(pPropertyDef);
  	  ASSERTU (pOMPropDef);
  	  
  	  ASSERTU (pProp);
  	  pProp->initialize (pOMPropDef);
  	  
  	  pOMPropDef = 0;
  }
  
  return pProp;
}


//
// Define the symbol for the stored object id
//
const OMClassId& ImplAAFObject::classId(void) const
{
#if defined(OM_DEBUG)
  const aafUID_t null_uid = { 0 };
  ASSERTU (! EqualAUID (&_soid, &null_uid));
#endif
  return *reinterpret_cast<const OMClassId*>(&_soid);
}



// Create and intialize associated external extensions.
AAFRESULT ImplAAFObject::InitializeExtensions(void)
{
  AAFRESULT ar = AAFRESULT_SUCCESS;
  ImplAAFClassDef *pDef = NULL;
  ImplAAFClassDef *pParentDef;
  const aafClassID_t* id;


  try
  {
    // We need to walk the class definitions:
    // 1. If the class definition is an extension class, and
    // 2. If the class definition has an associated plugin code (clsid), and
    // 3. Attempt to intialize the associated extension for this object's container.
    // 4. If step 3 fails then goto step 1 for the parent class definition.

    ar = GetDefinition (&pDef);
  
    while (AAFRESULT_SUCCESS == ar)
    {
      aafUID_t auid;
      ar = pDef->GetAUID(&auid);
      if (AAFRESULT_SUCCESS != ar)
        break;

      // There should probably be a method on ImplAAFClassDef to 
      // determine whether or not a particular class is an extended
      // class. At the very least this method could cache the result
      // of the following lookup so that lookup is only performed
      // once per file. (TomR:991111)
      //
      // Lookup the code class id for the given stored object id.
      id = ImplAAFBaseClassFactory::LookupClassID(auid);
      if (NULL != id)
        break; // we don't support extenting built-in classes!
    
      // If the intialize is successful then we are done. We currently
      // only support a one class extension per object.
      ar = InitializeImplExtension(this, auid);
      if (AAFRESULT_SUCCESS == ar)
        break;

      // Try again with the parent class defintion.
      ar = pDef->GetParent (&pParentDef);
      if (AAFRESULT_SUCCESS == ar)
      {
        // NOTE:We only maintain one class definition reference in the loop.
        pDef->ReleaseReference();
        pDef = pParentDef;
      }

    } // while

    if (NULL != pDef)
      pDef->ReleaseReference();
  }
  catch (...)
  {
    if (NULL != pDef)
      pDef->ReleaseReference();
    
    throw;
  }


  return ar;
}



aafBoolean_t ImplAAFObject::pvtIsGenerationTracked() const
{
  return _generation.isPresent() ? kAAFTrue : kAAFFalse;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::IsGenerationTracked (aafBoolean_t * pResult) const
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  *pResult = pvtIsGenerationTracked();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::EnableGenerationTracking ()
{
  if (! _generation.isPresent())
	{
	  aafUID_t nullUid = { 0 };
	  _generation = nullUid;
	}
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::DisableGenerationTracking ()
{
  if (_generation.isPresent())
	{
	  _generation.removeProperty();
	}
  return AAFRESULT_SUCCESS;
}


void ImplAAFObject::onSave(void* clientContext) const
{
  if (clientContext)
	{
	  aafUID_t * pGen = (aafUID_t*) clientContext;
	  if (pvtIsGenerationTracked())
		{
		  ASSERTU (pGen);
		  ImplAAFObject * pNonConstThis = (ImplAAFObject*) this;
		  pNonConstThis->_generation = *pGen;
		}
	}
  Progress();
}


void ImplAAFObject::onRestore(void* /*clientContext*/) const
{
  // clientContext currently unused

  // Cast away constness (maintaining logical constness)
  ((ImplAAFObject*) this)->setInitialized ();
  Progress();
}


// Overrides of ImplAAFStorable.
// Return true if this is a meta object
// NOTE: These objects will eventually owned by the Object Manager.
bool ImplAAFObject::metaObject(void) const
{
  return false;
}

// Return true is this is a data object (Interchange object).
bool ImplAAFObject::dataObject(void) const
{
  return true;
}

AAFRESULT ImplAAFObject::CreatePropertyInstanceAndAdd( ImplAAFPropertyDef* pPropDef )
{
  if (!pPropDef)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT ar;
  if(!_pProperties)
  {
    ar=InitProperties();
	if (AAFRESULT_FAILED(ar))
		return ar;
  }
  ASSERTU (_pProperties);

  ImplAAFClassDefSP pClass;
  ar = GetDefinition (&pClass);
  ASSERTU (AAFRESULT_SUCCEEDED (ar));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  ar = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (ar))
	return AAFRESULT_BAD_PROP;

  ar = _pProperties->SynchronizeProperty(this,pPropDef);
  if (AAFRESULT_FAILED (ar))
	return AAFRESULT_BAD_PROP;

  if (!pPropDef) {
    return AAFRESULT_NULL_PARAM;
  }

  return _pProperties->AddPropertyDef(pPropDef);
}
