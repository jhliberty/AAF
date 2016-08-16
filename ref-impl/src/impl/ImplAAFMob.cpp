
//=---------------------------------------------------------------------=
//
// $Id: ImplAAFMob.cpp,v 1.107 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#ifndef __ImplAAFFile_h__
#include "ImplAAFFile.h"
#endif

#ifndef __ImplEnumAAFMobSlots_h__
#include "ImplEnumAAFMobSlots.h"
#endif

#ifndef __ImplAAFOperationGroup_h__
#include "ImplAAFOperationGroup.h"
#endif

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#ifndef __ImplEnumAAFTaggedValues_h__
#include "ImplEnumAAFTaggedValues.h"
#endif

#ifndef __ImplAAFKLVData_h__
#include "ImplAAFKLVData.h"
#endif

#ifndef __ImplEnumAAFKLVData_h__
#include "ImplEnumAAFKLVData.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#include "ImplAAFHeader.h"
#include "ImplAAFContentStorage.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFTimelineMobSlot.h"
#include "ImplAAFStaticMobSlot.h"
#include "ImplAAFEventMobSlot.h"
#include "ImplAAFSourceClip.h"
#include "ImplAAFSequence.h"
#include "ImplAAFPulldown.h"
#include "ImplAAFFindSourceInfo.h"
#include "ImplAAFDictionary.h"
#include "AAFDataDefs.h"
#include "ImplEnumAAFMobSlots.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFTaggedValueUtil.h"

#include "OMAssertions.h"
#include <string.h>
#include <wchar.h>
#include "AAFResult.h"
#include "AAFComponentVisitor.h"
#include "AAFUtils.h"

#include "OMIdentitySet.h"
#include "OMIdentitySetIter.h"

#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef> ImplAAFDataDefSP;

extern "C" const aafClassID_t CLSID_EnumAAFMobSlots;
extern "C" const aafClassID_t CLSID_EnumAAFTaggedValues;
extern "C" const aafClassID_t CLSID_AAFFindSourceInfo;
extern "C" const aafClassID_t CLSID_AAFTypeDefString;
extern "C" const aafClassID_t CLSID_EnumAAFKLVData;

ImplAAFMob::ImplAAFMob ()
: _mobID(	 PID_Mob_MobID,	        L"MobID"),
  _name(	 PID_Mob_Name,	        L"Name"),
  _creationTime( PID_Mob_CreationTime,  L"CreationTime"),
  _lastModified( PID_Mob_LastModified,  L"LastModified"),
  _slots(	 PID_Mob_Slots,	        L"Slots"),
  _userComments( PID_Mob_UserComments,  L"UserComments"),
  _KLVData(	 PID_Mob_KLVData,       L"KLVData"),
  _attributes(   PID_Mob_Attributes,    L"Attributes" ),
  _usageCode(    PID_Mob_UsageCode,     L"UsageCode" )
{
	_persistentProperties.put(_mobID.address());
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_creationTime.address());
	_persistentProperties.put(_lastModified.address());
	_persistentProperties.put(_slots.address());
	_persistentProperties.put(_userComments.address());
	_persistentProperties.put(_KLVData.address());
	_persistentProperties.put(_attributes.address());
	_persistentProperties.put(_usageCode.address());

	(void)aafMobIDNew(&_mobID);		// Move this out of constructor when we get 2-stage create
	AAFGetDateTime(&_creationTime);
	AAFGetDateTime(&_lastModified);
	::memset( &_clsid, 0, sizeof( aafClassID_t ) );
}


ImplAAFMob::~ImplAAFMob ()
{
	// Release all of the mob slot pointers.
	aafUInt32 count = _slots.count();
	for (aafUInt32 i = 0; i < count; i++)
	{
		ImplAAFMobSlot *pSlot = _slots.clearValueAt(i);
		if (pSlot)
		{
		  pSlot->ReleaseReference();
		  pSlot = 0;
		}
	}

	if(_userComments.isPresent())
	{
		count = _userComments.count();
		for (aafUInt32 j = 0; j < count; j++)
		{
			ImplAAFTaggedValue* pTaggedValue = _userComments.clearValueAt(j);
			if (pTaggedValue)
			  pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
	}
	if(_KLVData.isPresent())
	{
		count = _KLVData.count();
		for (aafUInt32 j = 0; j < count; j++)
		{
			ImplAAFKLVData* pKLVData = _KLVData.clearValueAt(j);
			if (pKLVData)
			  pKLVData->ReleaseReference(); 
			pKLVData = 0;
		}
	}

	if(_attributes.isPresent())
	{
		count = _attributes.count();
		for (aafUInt32 j = 0; j < count; j++)
		{
			ImplAAFTaggedValue* pTaggedValue = _attributes.clearValueAt(j);
			if (pTaggedValue)
			  pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
	}
}

//****************
// AppendSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::AppendSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (pSlot->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_slots.appendValue(pSlot);
	pSlot->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// PrependSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::PrependSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (pSlot->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_slots.prependValue(pSlot);
	pSlot->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// InsertSlotAt()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::InsertSlotAt
        (aafUInt32 index,          //@parm [in] index to insert
		 ImplAAFMobSlot *  pSlot)  //@parm [in] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (pSlot->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	if (index > _slots.count()) // we can "insert" one after the end
	  return AAFRESULT_BADINDEX;

	_slots.insertAt(pSlot, index);
	pSlot->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// RemoveSlotAt()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveSlotAt
        (aafUInt32 index)  //@parm [in] index of slot to remove
{
	if (index >= _slots.count())
	  return AAFRESULT_BADINDEX;

	ImplAAFMobSlot *pSlot = NULL;
  pSlot = _slots.removeAt(index);
  if (pSlot)
  {
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pSlot->ReleaseReference ();
  }

  return AAFRESULT_SUCCESS;
}

//****************
// GetSlotAt()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::GetSlotAt
        (aafUInt32 index,          //@parm [in] index to insert
		 ImplAAFMobSlot ** ppSlot)  //@parm [out] returned slot
{
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (NULL == ppSlot)
		return AAFRESULT_NULL_PARAM;


	if (_slots.find (index, *ppSlot))
  {
    ASSERTU (*ppSlot); // It is an internal DM programming error if this assertion fails!
    (*ppSlot)->AcquireReference ();
  }
  else
  {
	  aafError = AAFRESULT_BADINDEX;
  }

	return aafError;
}

//****************
// LookupSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::LookupSlot
        (aafSlotID_t slotId,          //@parm [in] ID of slot to get
		 ImplAAFMobSlot ** ppSlot)  //@parm [out] returned slot
{
	return FindSlotBySlotID (slotId, ppSlot);
}

//****************
// RemoveSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (!pSlot->attached ()) // slot could not possibly be in _slots container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;

  OMUInt32 index;
  if (_slots.findIndex (pSlot, index))
	  return RemoveSlotAt (index);
  else
    return AAFRESULT_OBJECT_NOT_FOUND;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobID (aafMobID_t *pMobID)
{
	XPROTECT()
	  {
		/* This will support both 1.x and 2.x */
		if (pMobID)
		  {
			*pMobID = _mobID;
		  }
		else
		  {
			RAISE(AAFRESULT_NULL_PARAM);
		  }
	  }

	XEXCEPT
	  {
	  }
	XEND;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetName (aafCharacter *pName,
	aafUInt32 bufSize)
{
	bool stat;

	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_name.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	else
		stat = _name.copyToBuffer(pName, bufSize);
	
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

//****************
// GetNameBufLen()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFMob::GetNameBufLen
        (aafUInt32 *  pSize)  //@parm [in,out] Mob Name length
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(!_name.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	else
	{
		*pSize = _name.size();
		return(AAFRESULT_SUCCESS); 
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetModTime (aafTimeStamp_t *lastModified)
{
  // Validate input pointer...
  if (NULL == lastModified)
    return (AAFRESULT_NULL_PARAM);

	*lastModified = _lastModified;
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetCreateTime (aafTimeStamp_t *creationTime)
{
  // Validate input pointer...
  if (NULL == creationTime)
    return (AAFRESULT_NULL_PARAM);

	*creationTime = _creationTime;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobInfo (aafTimeStamp_t *lastModified,
                           aafTimeStamp_t *creationTime,
							aafCharacter *name,
							aafUInt32 nameLen)
{
  // Validate input pointers...
  if (NULL == lastModified || NULL == creationTime || NULL == name)
    return (AAFRESULT_NULL_PARAM);

    
  XPROTECT()
	{
		// Assign from local copies so that we do not change the
    // contents of the input parameters if there is a failure.
    // This is the documented behavior for output parameters.
    aafTimeStamp_t modified, created;

		CHECK(GetCreateTime (&created));
		CHECK(GetModTime (&modified));
		CHECK(GetName(name, nameLen));

		*lastModified = modified;
		*creationTime = created;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountSlots (aafNumSlots_t *pNumSlots)
{
  // Validate input pointer...
  if (NULL == pNumSlots)
    return (AAFRESULT_NULL_PARAM);
	
	*pNumSlots = _slots.count();

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetModTime (aafTimeStamp_constref modTime)
{
	_lastModified = modTime;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetCreateTime (aafTimeStamp_constref createTime)
{
	_creationTime = createTime;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetMobID (aafMobID_constref newMobID)
{
	AAFRESULT				hr = AAFRESULT_SUCCESS;
	ImplAAFMob				*mobPtr = NULL;
	ImplAAFHeader			*head = NULL;
	ImplAAFContentStorage	*cstore = NULL;

	XPROTECT()
	{
	  if (inFile())
		{
		hr = MyHeadObject(&head);
		if(hr == AAFRESULT_SUCCESS)
		{			
			hr = head->GetContentStorage(&cstore);
			if (hr == AAFRESULT_SUCCESS)
			{
				// Does a mob with the new ID already exist?  If so, return error
				hr= cstore->LookupMob(newMobID, &mobPtr) ;
				if(hr== AAFRESULT_SUCCESS)
				{
					mobPtr->ReleaseReference();
					mobPtr = NULL;
					RAISE(AAFRESULT_DUPLICATE_MOBID);
				}	
				else if(hr== AAFRESULT_MOB_NOT_FOUND)
				{
				    CHECK(cstore->RemoveMob(this));
					_mobID = newMobID;
					CHECK(cstore->AddMob(this));
				}
				else
					RAISE(hr);
				
				cstore->ReleaseReference();
				cstore = NULL;
			}

			head->ReleaseReference();
			head = NULL;
		}
		else
			RAISE(hr);
	}
    else
		 _mobID = newMobID;

	} /* XPROTECT */
	XEXCEPT
	{
		if (cstore)
		{
			cstore->ReleaseReference();
			cstore = NULL;
		}
		if (head)
		{
			head->ReleaseReference();
			head = NULL;
		}

	}
	XEND;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetName (const aafCharacter *pName)
{	
	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_name = pName;

	return(AAFRESULT_SUCCESS); 
}

// skip virtual AAFRESULT Verify(char *buf, validateData_t *result);
// What doe's this do?

//****************
// GetMobKind()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kAAFAllMob;				// Abstract superclass, only match "all"
	return AAFRESULT_SUCCESS;
}

// This method creates an "abstract" class. It should FAIL but it does not.
// We should either remove this method from the API or change it so that
// it taks the class definition for one of the "derived" classes TimelineMobSlot,
// StaticMobSlot, EventMobSlot, etc...(transdel 2000-MAR-10)
//
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::AppendNewSlot (ImplAAFSegment *segment,
                           aafSlotID_t  slotID,
                           const aafCharacter *slotName,
                           ImplAAFMobSlot **newSlot)
{
	ImplAAFMobSlot * tmpSlot = NULL;
  ImplAAFDictionary *pDictionary = NULL;

  // Validate input pointers...
  if (NULL == segment || NULL == slotName || NULL == newSlot)
    return (AAFRESULT_NULL_PARAM);

	*newSlot = NULL;

	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdMobSlot()->
			  CreateInstance ((ImplAAFObject**) &tmpSlot));
		pDictionary->ReleaseReference();
		pDictionary = NULL;

//!!!	CHECK(tmpSlot->WriteRational(OMMSLTEditRate, editRate));
		CHECK(tmpSlot->SetSegment(segment));
		CHECK(tmpSlot->SetSlotID(slotID));
		CHECK(tmpSlot->SetName(slotName));

		/* Append new slot to mob */
		_slots.appendValue(tmpSlot);
	} /* XPROTECT */
	XEXCEPT
	{
		if (tmpSlot)
		  tmpSlot->ReleaseReference();
		tmpSlot = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

	*newSlot = tmpSlot;
	if (tmpSlot)
		tmpSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

//****************
// AppendNewTimelineSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::AppendNewTimelineSlot
        (const aafRational_t &editRate,   //@parm [in] Edit rate property value
		 ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         const aafCharacter *  slotName,   //@parm [in] Slot Name (optional)
		 aafPosition_t  origin,
		 ImplAAFTimelineMobSlot ** newSlot)  //@parm [out] Newly created slot
{
	ImplAAFTimelineMobSlot	*aSlot = NULL;
	ImplAAFMobSlot			*tmpSlot = NULL;
  ImplAAFDictionary *pDictionary = NULL;


  // Validate input pointers...
  if (NULL == segment || NULL == slotName || NULL == newSlot)
    return (AAFRESULT_NULL_PARAM);

	*newSlot = NULL;

	XPROTECT()
	  {
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdTimelineMobSlot()->
			  CreateInstance ((ImplAAFObject**) &aSlot));
		pDictionary->ReleaseReference();
		pDictionary = NULL;
    CHECK(aSlot->Initialize());
		CHECK(aSlot->SetSegment(segment));
		CHECK(aSlot->SetSlotID(slotID));
		CHECK(aSlot->SetName(slotName));
		CHECK(aSlot->SetEditRate(editRate));
		CHECK(aSlot->SetOrigin(origin));

		/* Append new slot to mob */
		tmpSlot = aSlot;
		_slots.appendValue(tmpSlot);

	  } /* XPROTECT */

	XEXCEPT
	  {
		if (aSlot)
		  aSlot->ReleaseReference();
		aSlot = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	  }
	XEND;

	*newSlot = aSlot;
	if (aSlot)
		aSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFMob::AppendNewStaticSlot (ImplAAFSegment * pSegment,
								 aafSlotID_t  slotID,
								 aafCharacter_constptr  pSlotName,
								 ImplAAFStaticMobSlot ** ppNewSlot)
{
	ImplAAFStaticMobSlot	*aSlot = NULL;
	ImplAAFMobSlot			*tmpSlot = NULL;
	ImplAAFDictionary *pDictionary = NULL;

	// Validate input pointers...
	if (NULL == pSegment || NULL == pSlotName || NULL == ppNewSlot)
		return (AAFRESULT_NULL_PARAM);

	*ppNewSlot = NULL;

	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdStaticMobSlot()->
			CreateInstance ((ImplAAFObject**) &aSlot));
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(aSlot->SetSegment(pSegment));
		CHECK(aSlot->SetSlotID(slotID));
		CHECK(aSlot->SetName(pSlotName));

		/* Append new slot to mob */
		tmpSlot = aSlot;
		_slots.appendValue(tmpSlot);

	} /* XPROTECT */

	XEXCEPT
	{
		if (aSlot)
			aSlot->ReleaseReference();
		aSlot = 0;
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

	*ppNewSlot = aSlot;
	if (aSlot)
		aSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::AppendNewEventSlot (aafRational_t  editRate,
                           ImplAAFSegment * pSegment,
                           aafSlotID_t  slotID,
                           aafCharacter_constptr  pSlotName,
                           aafPosition_t  origin,
                           ImplAAFEventMobSlot ** ppNewSlot)
{
	ImplAAFEventMobSlot	*aSlot = NULL;
	ImplAAFMobSlot		*tmpSlot = NULL;
  ImplAAFDictionary		*pDictionary = NULL;


  // Validate input pointers...
  if (NULL == pSegment || NULL == pSlotName || NULL == ppNewSlot)
    return (AAFRESULT_NULL_PARAM);

	*ppNewSlot = NULL;

	XPROTECT()
	  {
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdEventMobSlot()->
			  CreateInstance ((ImplAAFObject**) &aSlot));
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(aSlot->SetSegment(pSegment));
		CHECK(aSlot->SetSlotID(slotID));
		CHECK(aSlot->SetName(pSlotName));
		CHECK(aSlot->SetEditRate( &editRate));
		

		/* Append new slot to mob */
		tmpSlot = aSlot;
		_slots.appendValue(tmpSlot);

	  } /* XPROTECT */

	XEXCEPT
	  {
		if (aSlot)
		  aSlot->ReleaseReference();
		aSlot = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	  }
	XEND;

	*ppNewSlot = aSlot;
	if (aSlot)
		aSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetSlots (ImplEnumAAFMobSlots **ppEnum)
{
  // Validate input pointer...
  if (NULL == ppEnum)
    return (AAFRESULT_NULL_PARAM);

  
  ImplEnumAAFMobSlots *theEnum =
	(ImplEnumAAFMobSlots *) CreateImpl (CLSID_EnumAAFMobSlots);
  if(theEnum == NULL)
	return(E_FAIL);

	// !!!Does not obey search criteria yet
	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFMobSlot>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFMobSlot>(_slots);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFMobSlots, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendComment ( const aafCharacter*  pTagName,
								const aafCharacter*  pComment)
{
	ImplAAFTaggedValue*			pTaggedValue = NULL;
	ImplEnumAAFTaggedValues*	pEnum = NULL;
	
	aafCharacter					oldTagName[64];
	aafBool						commentFound = kAAFFalse;
	aafUInt32					numComments = 0;
	ImplAAFDictionary *pDictionary = NULL;
	
	if (pTagName == NULL || pComment == NULL)
		return AAFRESULT_NULL_PARAM;


	XPROTECT()
	{
		CHECK(CountComments(&numComments));
		if (numComments > 0)
		{
			CHECK(GetComments(&pEnum));
			CHECK(pEnum->NextOne(&pTaggedValue));
			while(pTaggedValue)
			{
				CHECK(pTaggedValue->GetName(oldTagName, sizeof(oldTagName)));
				if (wcscmp(oldTagName, pTagName) == 0)
				{
					commentFound = kAAFTrue;
					break;
				}
				pTaggedValue->ReleaseReference();
				pTaggedValue = NULL;
				pEnum->NextOne(&pTaggedValue);
			}
			pEnum->ReleaseReference();
			pEnum = 0;
		}

		CHECK(GetDictionary(&pDictionary));
    // Do not reference count the following type and class definitions...
    ImplAAFTypeDef *pTaggedValueType = pDictionary->GetBuiltinDefs()->tdString();
    ASSERTU (pTaggedValueType); // this is supposed to be a builtin type.
 		ImplAAFClassDef *pTaggedValueClass = pDictionary->GetBuiltinDefs()->cdTaggedValue();
    ASSERTU (pTaggedValueClass); // this is supposed to be a builtin type.

    size_t cc = wcslen(pComment)+1;
    ASSERTU(cc <= OMUINT32_MAX);
    OMUInt32 characterCount = static_cast<OMUInt32>(cc);
    OMUInt32 byteCount = characterCount * sizeof(aafCharacter);

		if (commentFound)
		{
			// Update existing comment
			CHECK(pTaggedValue->SetValue(byteCount, (aafDataValue_t)pComment));
			pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
		else
		{
			// Create a new comment and add it to the list!
			CHECK(pTaggedValueClass->CreateInstance ((ImplAAFObject**) &pTaggedValue));
			CHECK(pTaggedValue->Initialize(pTagName,
										   pTaggedValueType,
                       byteCount, 
                       (aafDataValue_t)pComment));
			_userComments.appendValue(pTaggedValue);
		}
		pDictionary->ReleaseReference();
		pDictionary = NULL;

	}
	XEXCEPT
	{
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

//	if (pTaggedValue)
//		pTaggedValue->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

//****************
// RemoveComment()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveComment
        (ImplAAFTaggedValue * comment)
{
	if (! comment)
		return AAFRESULT_NULL_PARAM;
  if (!comment->attached ()) // object could not possibly be in container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;
	if(!_userComments.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
  OMUInt32 index;
  if (_userComments.findIndex (comment, index))
  {
	  _userComments.removeAt(index);
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    comment->ReleaseReference ();
  }
  else
  {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountComments (aafUInt32*  pNumComments)
{
	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_userComments.isPresent())
	{	// If the userComments property is not present then
		// number of user comments is zero!
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumComments = _userComments.count();
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetComments (ImplEnumAAFTaggedValues** ppEnum)
{
  // Validate input pointer...
  if (NULL == ppEnum)
    return (AAFRESULT_NULL_PARAM);

	if(!_userComments.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
		
	ImplEnumAAFTaggedValues* theEnum =
	  (ImplEnumAAFTaggedValues *)CreateImpl(CLSID_EnumAAFTaggedValues);

	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFTaggedValue>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFTaggedValue>(_userComments);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFTaggedValues, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendKLVData (ImplAAFKLVData * pData)
{
	if (NULL == pData)
		return AAFRESULT_NULL_PARAM;
  if (pData->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_KLVData.appendValue(pData);
	pData->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// RemoveKLVData()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveKLVData
        (ImplAAFKLVData * pData)
{
	if (! pData)
		return AAFRESULT_NULL_PARAM;
  if (!pData->attached ()) // object could not possibly be in container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;
	if(!_KLVData.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
  OMUInt32 index;
  if (_KLVData.findIndex (pData, index))
  {
	  _KLVData.removeAt(index);
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pData->ReleaseReference ();
  }
  else
  {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountKLVData (aafUInt32*  pNumComments)
{
	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_KLVData.isPresent())
	{	// If the userComments property is not present then
		// number of user comments is zero!
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumComments = _KLVData.count();
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetKLVData (ImplEnumAAFKLVData** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFKLVData *theEnum = (ImplEnumAAFKLVData *)CreateImpl (CLSID_EnumAAFKLVData);
	
  XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFKLVData>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFKLVData>(_KLVData);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFKLVData, this, iter));
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
    ImplAAFMob::AppendAttribute (aafCharacter_constptr  pName,
				 aafCharacter_constptr  pValue )
{
  return ImplAAFTaggedValueUtil::AppendNameValuePair( this, _attributes, pName, pValue );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountAttributes (aafUInt32*  pNumAttributes )
{
  return ImplAAFTaggedValueUtil::CountEntries( _attributes, pNumAttributes );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetAttributes (ImplEnumAAFTaggedValues ** ppEnum)
{
  return ImplAAFTaggedValueUtil::GetEnumerator( this, _attributes, ppEnum );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::RemoveAttribute (ImplAAFTaggedValue * pAttribute )
{
  return ImplAAFTaggedValueUtil::RemoveEntry( _attributes, pAttribute );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetUsageCode (aafUID_constref  usageCode )
{
  _usageCode = usageCode;

  return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetUsageCode (aafUID_t*  pUsageCode )
{
  if ( !_usageCode.isPresent() ) {
    return AAFRESULT_PROP_NOT_PRESENT;
  }

  *pUsageCode = _usageCode;

  return AAFRESULT_SUCCESS;

}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToMobTimecode (ImplAAFSegment *pTcSeg,
				     aafPosition_t *offset,
				     aafTimecode_t *result)
{
  // Validate input pointers...
  if (NULL == offset || NULL == result)
    return (AAFRESULT_NULL_PARAM);

  XPROTECT()
  {
    ImplAAFSmartPointer<ImplAAFSegment>      spSeg;
    ImplAAFSmartPointer<ImplAAFDataDef>      spDataDef;

    // If pTCSeg is null, then we must search for the first slot that
    // has a segment with data definition of type timecode.
    
    if ( pTcSeg ) {
      pTcSeg->AcquireReference();
      *(&spSeg) = pTcSeg;

      aafBool isTimecode = kAAFFalse;
      CHECK(spSeg->GetDataDef(&spDataDef));
      CHECK(spDataDef->IsTimecodeKind(&isTimecode));
      if (!isTimecode) {
		RAISE(AAFRESULT_TIMECODE_NOT_FOUND);
	  }
    }
    else {
	  ImplAAFSmartPointer<ImplEnumAAFMobSlots> spSlotIter;
	  ImplAAFSmartPointer<ImplAAFMobSlot>      spSlot;
      bool found = false;

      CHECK(GetSlots (&spSlotIter));
      while(spSlotIter->NextOne(&spSlot) == AAFRESULT_SUCCESS) {
		CHECK(spSlot->GetSegment(&spSeg));
	
		// Verify that it's a timecode slot by looking at the
		// data definition of the slot segment. 
		aafBool isTimecode = kAAFFalse;
		CHECK(spSeg->GetDataDef(&spDataDef));
		CHECK(spDataDef->IsTimecodeKind(&isTimecode));
	
		if (isTimecode) {
			found = true;
			break;
		}
      }
      
      if (!found) {
		RAISE(AAFRESULT_TIMECODE_NOT_FOUND);
      }
    }

	// The segment is responsible for computing the timecode result.
    CHECK(spSeg->SegmentOffsetToTC(offset, result))

  } /* XPROTECT */
  XEXCEPT
  {
  }
  XEND;
  
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::FindSlotBySlotID (aafSlotID_t	slotID,
                           ImplAAFMobSlot **destSlot)
{
	aafUInt32		loop;
	aafNumSlots_t	numSlots;
	ImplAAFMobSlot	*tmpSlot = NULL;
	aafSlotID_t	tmpSlotID;
	aafBool			foundSlot = kAAFFalse;
	

  // Validate input pointers...
  if (NULL == destSlot)
    return (AAFRESULT_NULL_PARAM);
  
	XPROTECT()
	{
		*destSlot = NULL;
		
		// For size entries the valid positions are 0 .. size - 1
		CHECK(CountSlots(&numSlots));
		for (loop = 0; loop < numSlots; loop++)
		{
			_slots.getValueAt(tmpSlot, loop);
			CHECK(tmpSlot->GetSlotID(&tmpSlotID));
			if (tmpSlotID == slotID)
			{
				foundSlot = kAAFTrue;
				break;
			}
		}
		if (!foundSlot)
		{
			RAISE(AAFRESULT_SLOT_NOT_FOUND);
		}
		*destSlot = tmpSlot;
		if (tmpSlot)
			tmpSlot->AcquireReference();
	}
  XEXCEPT
	{
	}
  XEND;

  return(AAFRESULT_SUCCESS);
}


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToTimecode (aafSlotID_t*slotID,
                           aafPosition_t *  offset,
                           aafTimecode_t *  result)
{
	ImplAAFTimelineMobSlot	*slot = NULL;
	ImplAAFSegment			*seg = NULL;
	aafTimecode_t 			timecode;
	aafMediaCriteria_t		mediaCrit;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	aafRational_t			editRate;
	aafPosition_t			frameOffset64;
	ImplEnumAAFMobSlots		*slotIter = NULL;
	ImplAAFDataDef			*dataDef = NULL;
	ImplAAFMob				*tapeMob = NULL;

  // Validate input pointers...
  if (NULL == slotID || NULL == offset || NULL == result)
    return (AAFRESULT_NULL_PARAM);

  memset(result, 0, sizeof(aafTimecode_t));
	memset(&timecode, 0, sizeof(aafTimecode_t));
	result->startFrame = 0;
	
	XPROTECT()
	{

		mediaCrit.type = kAAFAnyRepresentation;
		CHECK(InternalSearchSource(*slotID, *offset, kAAFTapeMob,
			&mediaCrit, NULL, &sourceInfo));
		
		CHECK(sourceInfo->GetMob(&tapeMob));
		CHECK(tapeMob->GetSlots (&slotIter));
		while(slotIter->NextOne((ImplAAFMobSlot**)&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetEditRate(&editRate));
			CHECK(slot->GetSegment(&seg));
			slot->ReleaseReference();
			slot = NULL;
			/* Verify that it's a timecode slot by looking at the
			* datadef of the slot segment.
			*/
			CHECK(seg->GetDataDef(&dataDef));
			aafBool	isTimecode = kAAFFalse;
			CHECK(dataDef->IsTimecodeKind(&isTimecode));
			if (isTimecode)
			{
				/* Assume found at this point, so finish generating result */
//!!!				CHECK(AAFConvertEditRate(sourceInfo.editrate, sourceInfo.position,
//					editRate, kRoundCeiling, &frameOffset64));
				
				CHECK(tapeMob->OffsetToMobTimecode(seg, &frameOffset64, &timecode));
				dataDef->ReleaseReference();
				dataDef = NULL;
				break;
			}
			dataDef->ReleaseReference();
			dataDef = NULL;
		} /* for */
		tapeMob->ReleaseReference();
		tapeMob = NULL;
		slotIter->ReleaseReference();
		slotIter = NULL;
    sourceInfo->ReleaseReference();
		
		
		*result = timecode;
	} /* XPROTECT */
	XEXCEPT
	{
		if (tapeMob)
		  dataDef->ReleaseReference();
		dataDef = 0;
		if (dataDef)
		  dataDef->ReleaseReference();
		dataDef = 0;
		if (slotIter)
		  slotIter->ReleaseReference();
		slotIter = 0;
		if (slot)
		  slot->ReleaseReference();
		slot = 0;
    if (sourceInfo)
      sourceInfo->ReleaseReference();
    sourceInfo = NULL;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::TimecodeToOffset (aafTimecode_t  timecode,
                           aafSlotID_t  slotID,
                           aafFrameOffset_t *  result)
{
	ImplEnumAAFMobSlots *iterHdl = NULL;
	ImplEnumAAFComponents *sequIter = NULL;
	ImplAAFMobSlot *slot = NULL;
	ImplAAFTimelineMobSlot *timelineSlot = NULL;
	ImplAAFPulldown *pdwn = NULL;
	ImplAAFSegment *seg = NULL;
	ImplAAFSegment *pdwnInput = NULL;
	aafPosition_t zero;
	aafBool found = kAAFFalse;
	aafRational_t	editRate;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	ImplAAFMob	*tapeMob = NULL;

  // Validate input pointer...
  if (NULL == result)
    return (AAFRESULT_NULL_PARAM);

	zero = 0;
	
	XPROTECT()
	{
		CHECK(InternalSearchSource(slotID, zero, kAAFTapeMob,
			NULL /* mediaCrit */, NULL, &sourceInfo));
		
		CHECK(sourceInfo->GetMob(&tapeMob));
		CHECK(tapeMob->GetSlots(&iterHdl));
		while(iterHdl->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			timelineSlot = dynamic_cast<ImplAAFTimelineMobSlot*>(slot);
			if(timelineSlot != NULL)
			{
				CHECK(timelineSlot->GetEditRate(&editRate));
				CHECK(timelineSlot->GetSegment(&seg));
			
				if(seg->SegmentTCToOffset(&timecode, &editRate, result) == AAFRESULT_SUCCESS)
					found = kAAFTrue;
				
				timelineSlot->ReleaseReference();
				timelineSlot = NULL;
				seg->ReleaseReference();
				seg = NULL;
			}
			
			slot->ReleaseReference();
			slot = NULL;
		}
		
		if(!found)
		{
			RAISE(AAFRESULT_TIMECODE_NOT_FOUND);
		}
		
		/* Release reference, so the useCount is decremented */
		tapeMob->ReleaseReference();	
		tapeMob = NULL;
		sourceInfo->ReleaseReference();	
		sourceInfo = NULL;
		iterHdl->ReleaseReference();
		iterHdl = NULL;
		if(sequIter != NULL)
		{
			sequIter->ReleaseReference();
			sequIter = NULL;
		}
		if(pdwn != NULL)
		{
			pdwn->ReleaseReference();
			pdwn = NULL;
		}
		if(seg != NULL)
		{
			seg->ReleaseReference();
			seg = NULL;
		}
		if(pdwnInput != NULL)
		{
			pdwnInput->ReleaseReference();
			pdwnInput = NULL;
		}
	}
	
	XEXCEPT
	{
		if (tapeMob)
		  tapeMob->ReleaseReference();	
		tapeMob = 0;
		if (sourceInfo)
		  sourceInfo->ReleaseReference();	
		sourceInfo = 0;
		if(iterHdl != NULL)
		  iterHdl->ReleaseReference();
		iterHdl = 0;
		if(sequIter != NULL)
		  sequIter->ReleaseReference();
		sequIter = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(pdwn != NULL)
		  pdwn->ReleaseReference();
		pdwn = 0;
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
		if(pdwnInput != NULL)
		  pdwnInput->ReleaseReference();
		pdwnInput = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::Copy (const aafCharacter *  destMobName,
                           ImplAAFMob ** destMob)
{
	XPROTECT()
	{
		// New storable object returned by shallowCopy() is
		// reference counted ImplAAFMob.
		OMStorable* newStorable = shallowCopy();
		
		ImplAAFMob *newMob = dynamic_cast<ImplAAFMob*>( newStorable );
		if ( !newMob ) {
			RAISE(AAFRESULT_BAD_TYPE);
		}

		aafMobID_t newMobId;
		CHECK( aafMobIDNew(&newMobId) );
		CHECK( newMob->SetMobID( newMobId ) );

		if ( destMobName ) {
	    	 CHECK( newMob->SetName( destMobName ) );
		}

		ImplAAFSmartPointer<ImplAAFHeader> spHeader;
		CHECK( MyHeadObject( &spHeader ) );
		CHECK( spHeader->AddMob(newMob) );

		deepCopyTo( newMob, 0 );

 		// newMob created by shallowCopy() is already reference counted.
		*destMob = newMob;
	}
	XEXCEPT
	{
	}
	XEND

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::ChangeRef (aafMobID_constref oldMobID,
                           aafMobID_constref newMobID)
{
	ImplEnumAAFMobSlots		*iter = NULL;
	ImplAAFMobSlot			*slot = NULL;

	XPROTECT()
	{
		CHECK(GetSlots (&iter));
		while(iter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->ChangeContainedReferences(oldMobID, newMobID));
			slot->ReleaseReference();
			slot = NULL;
		}
    iter->ReleaseReference(); // 2000-OCT-19 transdel: plug memory leak
	}
	XEXCEPT
	{
		if(slot)
		  slot->ReleaseReference();
		slot = 0;
		if(iter)
		  iter->ReleaseReference();
		iter = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



 /*************************************************************************
 * Function: CloneExternal()
 *
 *      This function clones the given mob in the source file into
 *      a destination mob in the destination file with the same Mob ID.
 *      If resolveDependencies is kAAFTrue, it will also clone all mobs 
 *      referenced by the given source mob. 
 *
 *      If includeMedia is kAAFTrue, it will also copy the media data 
 *      associated with the source mob.  The destination mob is
 *      returned. All private data is also cloned.
 *
 *      If the media data is not in the file, it will not attempt to
 *      try and find it in another file and clone it.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::CloneExternal (aafDepend_t  resolveDependencies,
			      aafIncMedia_t  includeMedia,
			      ImplAAFFile * destFile,
			      ImplAAFMob ** destMob)
{
	if(destFile == NULL)
        {
		return AAFRESULT_NULL_PARAM;
        }
	if(destMob == NULL)
        {
		return AAFRESULT_NULL_PARAM;
        }


	HRESULT hr = AAFRESULT_SUCCESS;

	XPROTECT()
	{
		// Merge the dictionary
		{
			ImplAAFSmartPointer<ImplAAFDictionary> spSrcDict;
			CHECK( GetDictionary(&spSrcDict) );

			ImplAAFSmartPointer<ImplAAFDictionary> spDstDict;
			CHECK( destFile->GetDictionary(&spDstDict) );

			CHECK( spSrcDict->MergeTo( spDstDict ) );
		}

		//
		// If includeMedia is true, and essence data exists for this mob,
		// then copy it.  Essence data only exists for SourceMobs.  We don't
		// worry about that detail here, rather, just let LookupEssenceData()
		// fail.
		//
		{
			if ( kAAFIncludeMedia == includeMedia ) {
				ImplAAFSmartPointer<ImplAAFHeader> spHeader;
				CHECK( MyHeadObject( &spHeader ) );

				ImplAAFSmartPointer<ImplAAFEssenceData> spEssenceData;
				hr = spHeader->LookupEssenceData( _mobID, &spEssenceData );

				if ( AAFRESULT_SUCCESS == hr ) {

					ImplAAFSmartPointer<ImplAAFDictionary> spDstDict;
					CHECK( destFile->GetDictionary(&spDstDict) );

					OMStorable* pNewStorable = spEssenceData->shallowCopy(spDstDict);
					ImplAAFEssenceData* pNewEssenceData = dynamic_cast<ImplAAFEssenceData*>(pNewStorable);
					ASSERTU( pNewEssenceData );
					
					ImplAAFSmartPointer<ImplAAFHeader> spDstHeader;
					CHECK( destFile->GetHeader(&spDstHeader) );
					CHECK( spDstHeader->AddEssenceData(pNewEssenceData) );

					spEssenceData->deepCopyTo( pNewStorable, 0 );
				}
			}

		}


		//
		// Check if the mob is already in the destination file
		//
		bool mobPresentInDestinationFile = false;
		ImplAAFSmartPointer<ImplAAFHeader> spDstHeader;
		CHECK( destFile->GetHeader( &spDstHeader ) );
		ImplAAFSmartPointer<ImplAAFMob> spDstMob;
		hr = spDstHeader->LookupMob( _mobID, &spDstMob );
		if( hr == AAFRESULT_SUCCESS )
		{
			mobPresentInDestinationFile = true;
			if( resolveDependencies == kAAFNoFollowDepend )
				hr = AAFRESULT_DUPLICATE_MOBID;
		}
		else if( hr == AAFRESULT_MOB_NOT_FOUND )
		{
			mobPresentInDestinationFile = false;
			hr = AAFRESULT_SUCCESS;
		}

		CHECK( hr );


		//
		// Copy the mob.
		//
		if( ! mobPresentInDestinationFile )
		{
			ImplAAFSmartPointer<ImplAAFDictionary> spDstDict;
			CHECK( destFile->GetDictionary(&spDstDict) );

			// New storable object returned by shallowCopy() is
			// reference counted ImplAAFMob.
			OMStorable* pNewStorable = shallowCopy(spDstDict);
		
			ImplAAFMob* pNewMob = dynamic_cast<ImplAAFMob*>(pNewStorable);
			ASSERTU( pNewMob );

			ImplAAFSmartPointer<ImplAAFHeader> spDstHeader;
			CHECK( destFile->GetHeader(&spDstHeader) );
			CHECK( spDstHeader->AddMob(pNewMob) );

			deepCopyTo( pNewStorable, 0 );

			// pNewMob created by shallowCopy() is already reference counted.
			*destMob = pNewMob;
		}

		//
		// Copy the mob's dependencies
		//
		if( resolveDependencies == kAAFFollowDepend )
		{
			hr = CloneDependencies(includeMedia,
						destFile);
		}
	}
	XEXCEPT
	{
	}
	XEND

	return hr;
}



  // @commDeletes the entire Mob structure \(the MOBJ object and all its contained objects\)
  // and deletes the entry from the Header.

// Internal to the toolkit functions
AAFRESULT
    ImplAAFMob::GetNthMobSlot (aafInt32 index /*0-based*/, ImplAAFMobSlot **ppMobSlot)
{
	AAFRESULT rc = AAFRESULT_SUCCESS;
	ImplAAFMobSlot	*obj;

  if (NULL == ppMobSlot)
		return AAFRESULT_NULL_PARAM;
  
  _slots.getValueAt(obj, index);
	if (obj)
		obj->AcquireReference();
	else
		rc = AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???
	*ppMobSlot = obj;

	return rc;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFMob::AddPhysSourceRef (aafAppendOption_t  addType,
							  aafRational_t  editrate,
							  aafSlotID_t  aMobSlot,
							  const aafUID_t & essenceKind,
							  aafSourceRef_t  ref,
							  aafLength_t  srcRefLength)
{
	ImplAAFComponent		*cpnt = NULL;
	ImplAAFSegment			*slotSeg = NULL;
	AAFRESULT				status = AAFRESULT_SUCCESS;
	aafSlotID_t				tmpSlotID;
	ImplAAFSourceClip		*sclp = NULL;
	ImplAAFSequence			*sequence = NULL;
	ImplAAFMobSlot			*slot = NULL;
	ImplAAFTimelineMobSlot	*newSlot = NULL;
	aafPosition_t			zeroPos;
	ImplAAFDictionary *pDictionary = NULL;

	XPROTECT()
	{
		zeroPos = 0;
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdSourceClip()->
			  CreateInstance ((ImplAAFObject **) &sclp));
		if (NULL == sclp)
			RAISE(AAFRESULT_NOMEMORY);
		ImplAAFDataDefSP pDataDef;
		CHECK(pDictionary->LookupDataDef(essenceKind, &pDataDef))
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(sclp->Initialize(pDataDef, srcRefLength, ref));
				
		status = FindSlotBySlotID(aMobSlot, &slot);
		if (status == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetSlotID(&tmpSlotID));
			if(addType == kAAFForceOverwrite)
			{
				CHECK(slot->SetSegment(sclp));
			}
			else if(addType == kAAFAppend)
			{
				CHECK(slot->GetSegment(&slotSeg));
				if(slotSeg != NULL)
				{
					CHECK(slotSeg->GenerateSequence(&sequence));
					CHECK(sequence->AppendComponent (sclp));

					sequence->ReleaseReference();
					sequence = NULL;
					slotSeg->ReleaseReference();
					slotSeg = NULL;
				}
			}
			//!!! else return an error
			slot->ReleaseReference();
			slot = NULL;
		}
		else
		{
				CHECK(AppendNewTimelineSlot(editrate, sclp,
								aMobSlot, L"ASlot", zeroPos, &newSlot) );
			newSlot->ReleaseReference();
			newSlot = NULL;
		}

		sclp->ReleaseReference();
		sclp = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(sequence != NULL)
		  sequence->ReleaseReference();
		sequence = 0;
		if(sclp != NULL)
		  sclp->ReleaseReference();
		sclp = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(slotSeg != NULL)
		  slotSeg->ReleaseReference();
		slotSeg = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(cpnt != NULL)
		  cpnt->ReleaseReference();
		cpnt = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}

/* NOTE: the assumption is that this function should be used primarily 
* for master mob and down 
*/
AAFRESULT ImplAAFMob::InternalSearchSource(	
										   aafSlotID_t trackID,
										   aafPosition_t offset,
										   aafMobKind_t mobKind,
										   aafMediaCriteria_t *pMediaCrit,
										   aafOperationChoice_t *pOperationChoice,
										   ImplAAFFindSourceInfo **ppSourceInfo)
{
	ImplAAFMobSlot 			*track = NULL;
	ImplAAFSegment			*rootObj = NULL;
	aafRational_t			srcRate;
	aafPosition_t			diffPos, nextPos;
	aafBool					sourceFound = kAAFFalse, foundTransition = kAAFFalse;
	ImplAAFMob				*nextMob = NULL;
	aafInt32				nestDepth, pulldownPhase;
	aafPosition_t			zeroPos = 0;
	aafLength_t				cpntLen, nextLen, minLength, newLen;
	ImplAAFPulldown			*pulldownObj = NULL;
	aafSlotID_t				nextTrackID;
	ImplAAFFindSourceInfo	*sourceInfo = NULL ;
	ImplAAFComponent		*leafObj = NULL;
	ImplAAFOperationGroup	*effeObject = NULL;
	
	if(ppSourceInfo == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	/* Initialize outputs */
	sourceInfo = (ImplAAFFindSourceInfo *)CreateImpl(CLSID_AAFFindSourceInfo);
	if (NULL == sourceInfo)
		return (AAFRESULT_NOMEMORY);
	sourceInfo->AcquireReference();		// This will be passed out
	*ppSourceInfo = sourceInfo;
	sourceInfo->Clear();
	
	XPROTECT()
	{
		/* Find segment at offset */
		CHECK(FindSlotBySlotID (trackID,  &track));
		CHECK(track->FindSegment(offset, &rootObj, &srcRate, &diffPos));
		CHECK(rootObj->GetLength(&cpntLen));
		CHECK(sourceInfo->SetComponent(rootObj));
		
		/*** Find leaf object in this track that points to the next mob ***/
		CHECK(MobFindLeaf(track, 
			pMediaCrit, pOperationChoice,
			rootObj, offset, cpntLen,
			NULL, NULL, 
			NULL, /* Shouldn't be scopes here */
			zeroPos,
			&leafObj, &minLength, &foundTransition, &effeObject,
			&nestDepth, NULL));
		if (minLength < cpntLen)
		{
			/* Figure out diffPos */
			newLen = minLength;
			/* NOTE: What should diffPos be in this case? */
		}
		else
		{
			newLen = cpntLen;
		}
		
		/*** Find the next mob, factoring in mask object edit rate conversions,
		*** and 1.0 track mappings.
		***/
		CHECK(FindNextMob(track, (ImplAAFSegment *)leafObj, 
			cpntLen, diffPos,
			&nextMob, &nextTrackID, &nextPos, &pulldownObj, &pulldownPhase, &nextLen));
		if(pulldownObj != NULL)
		{
			CHECK(sourceInfo->AddPulldown(pulldownObj));
		} 
		
		/*** Find component at referenced position in new mob ***/
		CHECK(nextMob->MobFindSource(nextTrackID, nextPos, nextLen,
			mobKind, pMediaCrit, pOperationChoice,
			sourceInfo, &sourceFound));
		if (!sourceFound)
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);

		nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		rootObj->ReleaseReference();
		rootObj = 0;
		track->ReleaseReference();
		track = 0;
		sourceInfo->ReleaseReference();
		sourceInfo = 0;
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_PARSE_EFFECT_AMBIGUOUS)
			sourceInfo->SetOperationGroup(effeObject);

		if (nextMob)
		  nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		if (rootObj)
		  rootObj->ReleaseReference();
		rootObj = 0;
		if (track)
		  track->ReleaseReference();
		track = 0;
		if (sourceInfo)
		  sourceInfo->ReleaseReference();
		sourceInfo = 0;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}
										   										   
AAFRESULT ImplAAFMob::MobFindLeaf(ImplAAFMobSlot *track,
								  aafMediaCriteria_t *mediaCrit,
								  aafOperationChoice_t *operationChoice,
								  ImplAAFComponent *rootObj,
								  aafPosition_t rootPos,
								  aafLength_t rootLen,
								  ImplAAFComponent	*prevObject,
								  ImplAAFComponent *nextObject,
								  ImplAAFScopeStack *scopeStack,
								  aafPosition_t	currentObjPos,
								  ImplAAFComponent **foundObj,
								  aafLength_t *minLength,
								  aafBool *foundTransition,
								  ImplAAFOperationGroup **effeObject,
								  aafInt32	*nestDepth,
								  aafPosition_t *diffPos)
{
	aafSlotID_t trackID;

	/* Parameter initialization and checking */
	if(nestDepth == NULL || foundObj == NULL || effeObject == NULL || minLength == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(rootObj == NULL)
		return(AAFRESULT_NULLOBJECT);

	*nestDepth = 0;
	*foundObj = NULL;
	*effeObject = NULL;
	*minLength = 0;
	
	XPROTECT()
	{
		CHECK(track->GetSlotID(&trackID));
		
		CHECK(rootObj->GetMinimumBounds(rootPos, rootLen,this, track, mediaCrit, currentObjPos,
			operationChoice, prevObject, nextObject, scopeStack,
			diffPos, minLength, effeObject, nestDepth,
			foundObj, foundTransition));
	} /* XPROTECT */
	XEXCEPT
	{
		/* At least try and return length if we can */
		//return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

/* This function will resolve mask objects to find the correct offset,
* and will map track IDs for 1.0 files.
*/
AAFRESULT ImplAAFMob::FindNextMob(ImplAAFMobSlot *track, 
								  ImplAAFSegment *segment,
								  aafLength_t length,
								  aafPosition_t diffPos,
								  ImplAAFMob **retMob,
								  aafSlotID_t *retTrackID,
								  aafPosition_t *retPos,
								  ImplAAFPulldown **pulldownObj,
								  aafInt32 *pulldownPhase,
								  aafLength_t *retLen)
{
	ImplAAFSourceClip		*sclp = NULL;
	aafLength_t				sclpLen;
	aafBool					isMask = kAAFFalse, reverse = kAAFFalse;
	aafSourceRef_t			sourceRef;
	ImplAAFMob				*nextMob = NULL;
	aafMobID_t			nullMobID = {{0,0,0,0,0,0,0,0,0,0,0,0},
					0,0,0,0,{0,0,0,{0,0,0,0,0,0,0,0}}}; // Need "isNIL" utility
	ImplAAFMobSlot 			*nextTrack = NULL;
	aafSlotID_t				tmpTrackID, nextTrackID;
	aafPosition_t			tmpPos, convertPos;

	if(segment == NULL || retMob == NULL || retTrackID == NULL || retPos == NULL || retLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		/* Initialize return parameters */
		*retMob = NULL;
		*retTrackID = 0;
		if(pulldownObj)
			*pulldownObj = NULL;
		
		/* Get source editrate from track, for later conversion */
		
		CHECK(segment->TraverseToClip(length, (ImplAAFSegment **)&sclp, pulldownObj, pulldownPhase,& sclpLen,
			&isMask));
		
		
		CHECK(sclp->GetSourceReference(&sourceRef));
		if (memcmp(&nullMobID, &sourceRef.sourceID, sizeof(sourceRef.sourceID)) == 0)
		{
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
		}
		/* Get next mob */
		CHECK(sclp->ResolveRef(&nextMob));
		
		tmpTrackID = sourceRef.sourceSlotID;
		
		/* Get destination track's edit rate */
		CHECK(nextMob->FindSlotBySlotID(tmpTrackID, (ImplAAFMobSlot **)&nextTrack));
		CHECK(nextTrack->GetSlotID(&nextTrackID));
		
		/* If its a MASK, apply the mask bits to the offset difference
		* (into the MASK object).  Then add this to the startTime in the
		* Source Clip.  Then do an editrate conversion in case it is
		* needed (24/1 -> 96/1 for Film Composer).  Then follow this 
		* offset to the next mob in the mob chain.
		*/
		if (isMask)
		{
			reverse = kAAFFalse;
			/* !!!Check out if we need phase returned from here */
			CHECK((*pulldownObj)->MapOffset(diffPos, reverse, &tmpPos, NULL));
		}
		else
			tmpPos = diffPos;
		
		tmpPos += sourceRef.startTime;
		if (!isMask)
		{
			CHECK(nextTrack->ConvertToMyRate(tmpPos,track, &convertPos));
		}
		else
			convertPos = tmpPos;
		
		*retMob = nextMob;
		*retTrackID = nextTrackID;
		*retPos = convertPos;
		*retLen = sclpLen;

		nextTrack->ReleaseReference();
		nextTrack = 0;

		sclp->ReleaseReference(); // causes 800400c8 to be returned from MasterMob::OpenEssence
	}
	XEXCEPT
	{
		if (nextTrack)
		  nextTrack->ReleaseReference();
		nextTrack = 0;
		if (nextMob)
		  nextMob->ReleaseReference();
		nextMob = 0;
		if (sclp)
			sclp->ReleaseReference();
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFMob::MobFindSource(
									aafSlotID_t trackID,
									aafPosition_t offset, /* offset in referenced units */
									aafLength_t length,   /* expected length of clip */
									aafMobKind_t mobKind,
									aafMediaCriteria_t *mediaCrit,
									aafOperationChoice_t *operationChoice,
									ImplAAFFindSourceInfo *sourceInfo,
									aafBool *foundSource)
{
	ImplAAFMobSlot			*track = NULL;
	ImplAAFPulldown			*pulldownObj = NULL;
	ImplAAFSegment			*rootObj = NULL;
	ImplAAFComponent		*leafObj = NULL;
	ImplAAFOperationGroup	*effeObject = NULL;
	ImplAAFMob				*nextMob = NULL;
	aafSlotID_t				foundTrackID;
	aafBool					nextFoundSource = kAAFFalse, foundTransition = kAAFFalse;
	aafPosition_t			foundPos, diffPos, zeroPos;
	aafRational_t			srcRate;
	aafLength_t				tmpLength, foundLen, minLength, newLen;
	aafInt32				nestDepth, pulldownPhase;
	aafMobKind_t			tstKind;
	
	zeroPos = 0;
	if(sourceInfo == NULL || foundSource == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		/* Initialize return values */
		*foundSource = kAAFFalse;
		CHECK(sourceInfo->Clear());
		
		/* Verify that track and position are valid */
		CHECK(FindSlotBySlotID (trackID,  &track));
		CHECK(track->FindSegment(offset, &rootObj, &srcRate, &diffPos));
		CHECK(rootObj->GetLength(&tmpLength));
		if (length < tmpLength)
		{
			tmpLength = length;
		}
		
		
		/* 1) Is this the mob that we're looking for? */
		CHECK(GetMobKind (&tstKind));
		if ((mobKind == kAAFCompMob) ||
			(mobKind == kAAFMasterMob) || 
			(mobKind == kAAFFileMob) ||
			(mobKind == kAAFTapeMob) ||
			(mobKind == kAAFFilmMob) ||
			(mobKind == kAAFPhysicalMob) )
		{
			*foundSource = (mobKind == tstKind ? kAAFTrue : kAAFFalse);
		}
		else if (mobKind == kAAFAllMob)
			*foundSource = kAAFTrue;
		else
		{
			RAISE(AAFRESULT_INVALID_MOBTYPE);
		}
		
		if (*foundSource)
		{
			CHECK(sourceInfo->Init(this, trackID, offset,
				srcRate, tmpLength,
				/*!!!*/NULL));		// What to put in for CPNT
				
			rootObj->ReleaseReference();
			rootObj = 0;
			track->ReleaseReference();
			track = 0;
			return(AAFRESULT_SUCCESS);
		}
		
		/* 2) If not right mob type, find component at referenced position 
		* in new mob.
		*/
		
		/* Found the clip on the top level track - now, traverse down
		* to find the leaf source clip if it exists.  We are assuming
		* that there will not be nested sequences.
		*
		* NOTE: Media Composer uses nested sequences to embed 0-length 
		* fills in compositions.
		* We are also assuming that this function will probably be called
		* on the master mob and down - so, we shouldn't run into transitions.
		* So, passing NULL for prevObject and nextObject is probably alright.
		*/
		CHECK(MobFindLeaf(track, mediaCrit, operationChoice, 
			rootObj, offset, tmpLength,
			NULL, NULL, 
			NULL, /* Shouldn't be scopes here */
			zeroPos,
			&leafObj, &minLength, &foundTransition,
			&effeObject, &nestDepth, NULL));
		
		if (minLength < length)
		{
			/* Figure out diffPos!!! (changed newDIffPos -> tmpLength */
			newLen = minLength;
		}
		else
		{
			newLen = length;
		}
		
		/*** Find the next mob, factoring in mask object edit rate conversions,
		*** and 1.0 track mappings.
		***/
		CHECK(FindNextMob(track, (ImplAAFSegment *)leafObj, 
			tmpLength, diffPos,
			&nextMob, &foundTrackID, &foundPos, &pulldownObj, &pulldownPhase, &foundLen));
		
//		if(pulldownObj != NULL)
//		{
//!!!			aafPulldownDir_t	direction;

//!!!			{
//				CHECK(pulldownObj->ReadPulldownDirectionType(OMPDWNDirection, &direction));
//			}
//			if(direction == kAAFFilmToTapeSpeed)	/* kAAFFilmToTapeSpeed */
//			{
//				(*sourceInfo).filmTapePdwn = pulldownObj;
//				(*sourceInfo).filmTapePhase = pulldownPhase;
//			}
//			else				/* kAAFTapeToFilmSpeed */
//			{
//				(*sourceInfo).tapeFilmPdwn = pulldownObj;
//				(*sourceInfo).tapeFilmPhase = pulldownPhase;
//			}
//		} 
		/* Find component at referenced position in new mob */
		CHECK(nextMob->MobFindSource(foundTrackID,
			foundPos, foundLen,
			mobKind, mediaCrit, operationChoice,
			sourceInfo, &nextFoundSource));
		if (nextFoundSource)
		{
			*foundSource = nextFoundSource;
		}
		else /* Failure - null  out return values */
		{
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
		}
		
		nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		rootObj->ReleaseReference();
		rootObj = 0;
		track->ReleaseReference();
		track = 0;
	}
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_PARSE_EFFECT_AMBIGUOUS)
			sourceInfo->SetOperationGroup(effeObject);

		if (nextMob)
		  nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		if (rootObj)
		  rootObj->ReleaseReference();
		rootObj = 0;
		if (track)
		  track->ReleaseReference();
		track = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFMob::ReconcileMobLength(void)
{
	return(AAFRESULT_ABSTRACT_CLASS);	// MUST call one of the subclasses
}

const OMMaterialIdentification&
  ImplAAFMob::identification(void) const
{
  return *reinterpret_cast<const OMMaterialIdentification*>(&_mobID.reference());
}

bool ImplAAFMob::IsMobIDEqual( const aafMobID_t* mobID ) const
{
	aafMobID_t thisMobID = _mobID;

	if ( ::memcmp( mobID, &thisMobID, sizeof(aafMobID_t) ) == 0 ) {
		return true;
	}
	else {
		return false;
	}

}

bool ImplAAFMob::IsNameEqual( const aafString_t name ) const
{
  if ( !_name.isPresent() ) {
    return false;
  }

  const wchar_t* thisName = _name;

  if ( ::wcscmp(name, thisName) == 0 ) {
    return true;
  }
  else {
    return false;
  }
}

HRESULT ImplAAFMob::IsClassIDEqual( const aafClassID_t* classId, bool& result ) const
{
  ImplAAFMob* ncThis = const_cast<ImplAAFMob*>(this);
	
  ImplAAFSmartPointer<ImplAAFClassDef> spClassDef;

  AAFRESULT hr;
  hr = ncThis->GetDefinition( &spClassDef );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  aafUID_t thisClassId;
  hr = spClassDef->GetAUID( &thisClassId );
  if ( AAFRESULT_SUCCESS != hr ) {
    return hr;
  }

  ASSERTU( sizeof(aafUID_t) == sizeof(aafClassID_t) );

  if ( ::memcmp( classId, &thisClassId, sizeof(aafUID_t) ) == 0 ) {
    result = true;
  }
  else {
    result = false;
  }

  return AAFRESULT_SUCCESS;
}

bool ImplAAFMob::IsUsageCodeEqual( const aafUID_t* usageCode ) const
{
  if ( !_usageCode.isPresent() ) {
    return false;
  }

  aafUID_t thisUsageCode = _usageCode;

  if ( ::memcmp( usageCode, &thisUsageCode, sizeof(aafUID_t) ) == 0 ) {
    return true;
  }
  else {
    return false;
  }

}



// Class AAFMobCollectingComponentVisitor declares a visit operation
// for each SourceClip referenced by the given MobSlot. The operation
// is to retreive the ID of the mob references by the SourceClip and
// append it to the list.
class AAFMobCollectingComponentVisitor : public AAFComponentVisitor
{
    public:
        virtual void VisitSourceClip(ImplAAFSourceClip* pSourceClip);
        OMIdentitySetIterator<aafMobID_t>*  MobIDs() const;

    private:
        OMIdentitySet<aafMobID_t>  _referencedMobs;
};



void AAFMobCollectingComponentVisitor::VisitSourceClip(
    ImplAAFSourceClip* pSourceClip)
{
    ASSERTU(pSourceClip);

    aafMobID_t mobID;
    pSourceClip->GetSourceID(&mobID);
    _referencedMobs.ensurePresent(mobID);
}



OMIdentitySetIterator<aafMobID_t>*
AAFMobCollectingComponentVisitor::MobIDs() const
{
    return new OMIdentitySetIterator<aafMobID_t>(_referencedMobs,
                                                 OMBefore);
}



AAFRESULT ImplAAFMob::CloneDependencies(
    aafIncMedia_t  includeMedia,
    ImplAAFFile * destFile)
{
  ASSERTU(destFile);


  HRESULT hr = AAFRESULT_SUCCESS;


  ImplAAFSmartPointer<ImplAAFHeader> spHeader;
  MyHeadObject( &spHeader );


  // For each mob slot find IDs of mobs that it has
  // references to and clone them
  aafNumSlots_t slotCount = 0;
  CountSlots(&slotCount);
  for( aafNumSlots_t i=0; i<slotCount && hr == AAFRESULT_SUCCESS; i++ )
  {
    ImplAAFSmartPointer<ImplAAFMobSlot> spSlot;
    GetSlotAt(i, &spSlot);

    ImplAAFSmartPointer<ImplAAFSegment> spSegment;
    spSlot->GetSegment(&spSegment);

    AAFMobCollectingComponentVisitor mobCollector;
    spSegment->Accept(mobCollector);


    // Clone each of the referenced mobs
    OMIdentitySetIterator<aafMobID_t>*  iter = mobCollector.MobIDs();
    while( ++(*iter) && hr == AAFRESULT_SUCCESS )
    {
      aafMobID_t  mobID = iter->value();

      ImplAAFSmartPointer<ImplAAFMob> spSrcReferencedMob;
      hr = spHeader->LookupMob(mobID,&spSrcReferencedMob);
      if( SUCCEEDED(hr) )
      {
        ImplAAFSmartPointer<ImplAAFMob> spDstReferencedMob;
        hr = spSrcReferencedMob->CloneExternal(kAAFFollowDepend,
                                               includeMedia,
                                               destFile,
                                               &spDstReferencedMob);
      }
      else if( hr == AAFRESULT_MOB_NOT_FOUND )
      {
         // The referenced mob is not in this file
         hr = AAFRESULT_SUCCESS;
      }
    }

    delete iter;
    iter = 0;
  }


  return hr;
}

////////////////////////////////////////////////////////////////////////////////
