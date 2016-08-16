
//=---------------------------------------------------------------------=
//
// $Id: ImplAAFContentStorage.cpp,v 1.61 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


#include "AAFTypes.h"
#include "aafErr.h"
#include "AAFResult.h"
#include "AAFUtils.h"

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplEnumAAFMobs_h__
#include "ImplEnumAAFMobs.h"
#endif

#ifndef __ImplEnumAAFEssenceData_h__
#include "ImplEnumAAFEssenceData.h"
#endif

#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif

#ifndef __ImplAAFContentStorage_h__
#include "ImplAAFContentStorage.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"

#include "OMAssertions.h"

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200


extern "C" const aafClassID_t CLSID_EnumAAFMobs;
extern "C" const aafClassID_t CLSID_EnumAAFEssenceData;

ImplAAFContentStorage::ImplAAFContentStorage ()
: _mobs(PID_ContentStorage_Mobs, L"Mobs", PID_Mob_MobID),
  _essenceData(PID_ContentStorage_EssenceData, L"EssenceData", PID_EssenceData_MobID)
{
	_persistentProperties.put(_mobs.address());
	_persistentProperties.put(_essenceData.address());
}



ImplAAFContentStorage::~ImplAAFContentStorage ()
{
	// Cleanup the persistent data...
	// Release the essence data
	OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFEssenceData>essenceData(_essenceData);
	while(++essenceData)
	{
		ImplAAFEssenceData *pData = essenceData.clearValue();
		if (pData)
		{
		  pData->ReleaseReference();
		  pData = 0;
		}
	}
	
	// Release the mobs
	OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFMob>mobs(_mobs);
	while(++mobs)
	{
		ImplAAFMob *pMob = mobs.clearValue();
		if (pMob)
		{
		  pMob->ReleaseReference();
		  pMob = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::LookupMob (aafMobID_constref mobID,
                           ImplAAFMob **ppMob)
{
  if (! ppMob)
    return AAFRESULT_NULL_PARAM;

	
	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_mobs.find((*reinterpret_cast<const OMMaterialIdentification *>(&mobID)),
                             *ppMob))
	{
		ASSERTU(NULL != *ppMob);
		(*ppMob)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_MOB_NOT_FOUND;
	}

	return (result);

}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::CountMobs (aafMobKind_t mobKind,
                           aafNumSlots_t *pNumMobs)
{
	aafUInt32				siz;
	ImplEnumAAFMobs		*mobEnum = NULL;
	aafSearchCrit_t		criteria;
	AAFRESULT			hr = AAFRESULT_SUCCESS;
	ImplAAFMob			*aMob = NULL;

	if(pNumMobs == NULL)
		return AAFRESULT_NULL_PARAM;
	
	if(mobKind == kAAFAllMob)
	{
		siz = _mobs.count();
	}
	else
	{
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = mobKind;
		hr = GetMobs (&criteria,&mobEnum);
		siz = 0;
		do {
			hr = mobEnum->NextOne (&aMob);
			if(hr == AAFRESULT_SUCCESS)
			{
			  siz++;
			  ASSERTU (aMob);
			  aMob->ReleaseReference();
			  aMob = NULL;
			}
 		} while(hr == AAFRESULT_SUCCESS);
		if(hr == AAFRESULT_NO_MORE_OBJECTS)
			hr = AAFRESULT_SUCCESS;
		if (mobEnum)
		  {
			mobEnum->ReleaseReference();
			mobEnum = 0;
		  }
	}
	
	*pNumMobs = siz;
	return hr;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::GetMobs (aafSearchCrit_t *pSearchCriteria,
                           ImplEnumAAFMobs **ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFMobs *theEnum = (ImplEnumAAFMobs *)CreateImpl (CLSID_EnumAAFMobs);
	
  XPROTECT()
	{
		OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFMob>* iter = 
			new OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFMob>(_mobs);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFMobs, this, iter));
		CHECK(theEnum->SetCriteria(pSearchCriteria));
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
    ImplAAFContentStorage::AddMob (ImplAAFMob *pMob)
{
	aafMobID_t	mobID;

	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));

		// JeffB: Test is a throwaway, so don't bump the refcount
		if(!_mobs.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			if (pMob->attached ())
				return AAFRESULT_OBJECT_ALREADY_ATTACHED;
				
			_mobs.appendValue(pMob);
			// trr - We are saving a copy of pointer in _mobs so we need
			// to bump its reference count.
			pMob->AcquireReference();
		}
		else
			RAISE(AAFRESULT_DUPLICATE_MOBID);
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::RemoveMob (ImplAAFMob *pMob)
{
	aafMobID_t	mobID;

	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;
		
	if (!pMob->attached())
		return AAFRESULT_MOB_NOT_FOUND;	
		
	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));

		if(_mobs.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			_mobs.removeValue(pMob);
			pMob->ReleaseReference(); // the set no longer owns this reference.
		}
		else
			RAISE(AAFRESULT_MOB_NOT_FOUND);
	}
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::CountEssenceData(aafUInt32 *  pNumEssenceData)
{
  if(pNumEssenceData == NULL)
    return AAFRESULT_NULL_PARAM;
    
  *pNumEssenceData = _essenceData.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::IsEssenceDataPresent (aafMobID_constref fileMobID,
                           aafFileFormat_t /*fmt*/,
                           aafBool *pResult)
{
	*pResult = _essenceData.contains((*reinterpret_cast<const OMMaterialIdentification *>(&fileMobID)));
	return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplAAFContentStorage::LookupEssenceData (aafMobID_constref fileMobID,
                           ImplAAFEssenceData **ppEssence)
{
  if (! ppEssence)
    return AAFRESULT_NULL_PARAM;

	
	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_essenceData.find((*reinterpret_cast<const OMMaterialIdentification *>(&fileMobID)),
                             *ppEssence))
	{
		ASSERTU(NULL != *ppEssence);
		(*ppEssence)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_MOB_NOT_FOUND;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::EnumEssenceData (ImplEnumAAFEssenceData ** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFEssenceData *theEnum = (ImplEnumAAFEssenceData *)CreateImpl (CLSID_EnumAAFEssenceData);
	
  XPROTECT()
	{
		OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFEssenceData>* iter = 
			new OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFEssenceData>(_essenceData);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFEssenceData, this, iter));
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
    ImplAAFContentStorage::AddEssenceData (ImplAAFEssenceData * pEssenceData)
{
	aafMobID_t	mobID;

	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pEssenceData->GetFileMobID(&mobID));

		// JeffB: Test is a throwaway, so don't bump the refcount
		if(!_essenceData.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			if (pEssenceData->attached())
				return AAFRESULT_OBJECT_ALREADY_ATTACHED;
				
			_essenceData.appendValue(pEssenceData);
			// trr - We are saving a copy of pointer in _mobs so we need
			// to bump its reference count.
			pEssenceData->AcquireReference();
		}
		else
			RAISE(AAFRESULT_DUPLICATE_MOBID);
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::RemoveEssenceData (ImplAAFEssenceData * pEssenceData)
{
	aafMobID_t	mobID;

	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;

	if (!pEssenceData->attached())
		return AAFRESULT_ESSENCE_NOT_FOUND;

	XPROTECT()
	{
		CHECK(pEssenceData->GetFileMobID(&mobID));

		if(_essenceData.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			_essenceData.removeValue(pEssenceData);
			pEssenceData->ReleaseReference();
		}
		else
			RAISE(AAFRESULT_ESSENCE_NOT_FOUND);
	} 
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}
