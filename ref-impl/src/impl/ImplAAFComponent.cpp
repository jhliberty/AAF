

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFComponent.cpp,v 1.59 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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



#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFTaggedValueUtil.h"

#include "OMAssertions.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;
typedef ImplAAFSmartPointer<ImplAAFDataDef>    ImplAAFDataDefSP;
extern "C" const aafClassID_t CLSID_EnumAAFKLVData;

ImplAAFComponent::ImplAAFComponent ():
  _dataDef( PID_Component_DataDefinition,
            L"DataDefinition", 
            L"/Header/Dictionary/DataDefinitions",
            PID_DefinitionObject_Identification),
  _length( PID_Component_Length,
           L"Length"),
  _KLVData( PID_Component_KLVData,
            L"KLVData"),
  _userComments( PID_Component_UserComments,
            L"UserComments" ),
  _attributes( PID_Component_Attributes,
	       L"Attributes" )
{
	_persistentProperties.put(   _dataDef.address());
	_persistentProperties.put(   _length.address());
	_persistentProperties.put(   _KLVData.address());
	_persistentProperties.put(   _userComments.address());
	_persistentProperties.put(   _attributes.address());
}

ImplAAFComponent::~ImplAAFComponent ()
{
	if(_KLVData.isPresent())
	{
		aafUInt32 size = _KLVData.count();
		for (aafUInt32 j = 0; j < size; j++)
		{
			ImplAAFKLVData* pKLVData = _KLVData.clearValueAt(j);
			if (pKLVData)
			  pKLVData->ReleaseReference();
			pKLVData = 0;
		}
	}

	if(_userComments.isPresent())
	{
		aafUInt32 count = _userComments.count();
		for (aafUInt32 j = 0; j < count; j++)
		{
			ImplAAFTaggedValue* pTaggedValue = _userComments.clearValueAt(j);
			if (pTaggedValue)
			  pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
	}

	if(_attributes.isPresent())
	{
		aafUInt32 count = _attributes.count();
		for (aafUInt32 j = 0; j < count; j++)
		{
			ImplAAFTaggedValue* pTaggedValue = _attributes.clearValueAt(j);
			if (pTaggedValue)
			  pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::SetLength (const aafLength_t & length)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if ( length < 0 )
	  aafError = AAFRESULT_BAD_LENGTH;
	else
	  _length = length;
	return aafError;
}


/*************************************************************************
 * Function: GetLength()
 *
 * 		This function returns the length of a component.  
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetLength (aafLength_t * pLength)
{
  if (pLength == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (! _length.isPresent())
	{
	  return AAFRESULT_PROP_NOT_PRESENT;
	}
	
  *pLength = _length;
  return AAFRESULT_SUCCESS;
}

	
AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::SetDataDef (ImplAAFDataDef * pDataDef)
{
  if (! pDataDef)
    return AAFRESULT_NULL_PARAM;
  if (!pDataDef->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  // Check if given data definition is in the dict.
  if( !aafLookupDataDef( this, pDataDef ) )
    return AAFRESULT_INVALID_OBJ;

  _dataDef = pDataDef;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetDataDef (ImplAAFDataDef ** ppDataDef)
{
  if (! ppDataDef)
	return AAFRESULT_NULL_PARAM;

   if(_dataDef.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFDataDef *pDataDef = _dataDef;

  *ppDataDef = pDataDef;
  ASSERTU (*ppDataDef);
  (*ppDataDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::AppendKLVData (ImplAAFKLVData * pData)
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
	ImplAAFComponent::RemoveKLVData
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
    ImplAAFComponent::CountKLVData (aafUInt32*  pNumComments)
{
	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_KLVData.isPresent())
	{
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumComments = _KLVData.count();
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetKLVData (ImplEnumAAFKLVData** ppEnum)
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
    ImplAAFComponent::AppendComment (aafCharacter_constptr  pName,
		                     aafCharacter_constptr  pValue)
{
  return ImplAAFTaggedValueUtil::AppendNameValuePair( this, _userComments, pName, pValue );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::CountComments (aafUInt32* pNumComments )
{
  return ImplAAFTaggedValueUtil::CountEntries( _userComments, pNumComments );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetComments (ImplEnumAAFTaggedValues** ppEnum )
{
  return ImplAAFTaggedValueUtil::GetEnumerator( this, _userComments, ppEnum );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::RemoveComment (ImplAAFTaggedValue* pComment)
{
  return ImplAAFTaggedValueUtil::RemoveEntry( _userComments, pComment );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::AppendAttribute (aafCharacter_constptr  pName,
                                       aafCharacter_constptr  pValue)
{
  return ImplAAFTaggedValueUtil::AppendNameValuePair( this, _attributes, pName, pValue );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::CountAttributes (aafUInt32* pNumAttributes )
{
  return ImplAAFTaggedValueUtil::CountEntries( _attributes, pNumAttributes );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::GetAttributes (ImplEnumAAFTaggedValues** ppEnum)
{
  return ImplAAFTaggedValueUtil::GetEnumerator( this, _attributes, ppEnum );
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFComponent::RemoveAttribute (ImplAAFTaggedValue* pAttribute)
{
  return ImplAAFTaggedValueUtil::RemoveEntry( _attributes, pAttribute );
}

/*************************************************************************
 * Private Function: SetNewProps()
 *
 *      This is an internal function that is called by functions that
 *      create new components.  It sets the length and data definition property
 *      values on the input component object.
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
AAFRESULT ImplAAFComponent::SetNewProps(
        aafLength_t length,			/* IN - Length property value */
        ImplAAFDataDef * pDataDef)			/* IN - DataDef property value */
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (! pDataDef)
	  return AAFRESULT_NULL_PARAM;

	if ( length < 0 )
	  aafError = AAFRESULT_BAD_LENGTH;
	else
	  {
		_length	= length;
		_dataDef = pDataDef;
	  }
	return aafError;
}

AAFRESULT ImplAAFComponent::AccumulateLength(aafLength_t *pLength)
{
	if (pLength == NULL)
	{
 		return AAFRESULT_NULL_PARAM;
	}

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFComponent::GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
											 ImplAAFMob * /*mob*/, ImplAAFMobSlot * /*track*/,
											 aafMediaCriteria_t * /*mediaCrit*/,
											 aafPosition_t currentObjPos,
											 aafOperationChoice_t * /*operationChoice*/,
											 ImplAAFComponent	* /*prevObject*/,
											 ImplAAFComponent * /*nextObject*/,
											 ImplAAFScopeStack * /*scopeStack*/,
											 aafPosition_t *diffPos, aafLength_t *minLength,
											 ImplAAFOperationGroup ** /*groupObject*/, aafInt32	* /*nestDepth*/,
											 ImplAAFComponent **found, aafBool *foundTransition)
{
	aafLength_t	tmpMinLen;
	
	XPROTECT()
	{
		*foundTransition = kAAFFalse;
		*found = this;
    AcquireReference(); // We are returning a reference so bump the reference count!
		CHECK(GetLength(&tmpMinLen));
		if (tmpMinLen < rootLen)
		{
			*minLength = tmpMinLen;
			if(diffPos != NULL)
			{
				/* Figure out diffPos */
				*diffPos = rootPos - currentObjPos;
			}
		}
		else
		{
			*minLength = rootLen;
			if(diffPos != NULL)
				*diffPos = 0;
		}
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFComponent::ChangeContainedReferences(aafMobID_constref /*from*/,
													  aafMobID_constref /*to*/)
{
	return AAFRESULT_SUCCESS;
}


void ImplAAFComponent::Accept(AAFComponentVisitor&)
{
	// do nothing
}

