//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefCharacter.cpp,v 1.17 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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



#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefCharacter_h__
#include "ImplAAFTypeDefCharacter.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFClassIDs.h"

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include "OMTypeVisitor.h"

#include <string.h>

// We only support two byte unicode characters.
const aafUInt32 kExternalCharacterSize = 2;


//some macros
#define check_hr(expr)\
{\
	HRESULT  the_hresult = (expr);\
	if (FAILED(the_hresult))\
	   return the_hresult;\
}


ImplAAFTypeDefCharacter::ImplAAFTypeDefCharacter ()
{}

ImplAAFTypeDefCharacter::~ImplAAFTypeDefCharacter ()
{}


AAFRESULT ImplAAFTypeDefCharacter::pvtInitialize (
      aafUID_constref  id,
      aafCharacter_constptr  pTypeName)
{
  // The description string needs to be initialized elsewhere...
  return ImplAAFMetaDefinition::Initialize(id, pTypeName, L"Two-byte Unicode Character Type Definition");
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::CreateValueFromCharacter (
      aafCharacter  character,
      ImplAAFPropertyValue ** ppCharacterValue)
{
  TRACE("ImplAAFTypeDefCharacter::CreateValueFromCharacter");
  
  if (! ppCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	aafUInt32 cbChar = NativeSize();
	
	// Create a temporary pointer to copy to the smartptr
	ImplAAFPropValData * tmp = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
	if (NULL == tmp)
		return AAFRESULT_NOMEMORY;
	ImplAAFPropValDataSP pv;
	pv = tmp;
	
	tmp->ReleaseReference(); // we don't need this reference anymore.
	tmp = 0;
	
	//Initialize
	check_hr ( pv->Initialize(this) );
	
	//Allocate appropriate bits
	aafMemPtr_t pBits = NULL;
	check_hr ( pv->AllocateBits (cbChar, &pBits) );
	
	//Set the bits to incoming character
	ASSERT("Valid bits", pBits != 0);
	memcpy (pBits, &character, cbChar);
	
	*ppCharacterValue = pv;
	(*ppCharacterValue)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::SetCharacter (
      ImplAAFPropertyValue * pCharacterValue,
      aafCharacter  character)
{
  TRACE("ImplAAFTypeDefCharacter::SetCharacter");
  
	if (! pCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	//get a pointer to the Val Data
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData*>(pCharacterValue);
	if (!pvd) return AAFRESULT_BAD_TYPE;
	
	// get the property value's embedded type
	ImplAAFTypeDefSP pPropType;
	check_hr ( pvd->GetType (&pPropType) );
	//Make sure the TD of the pv passed in, matches that of the ImplAAFTypeDefCharacter
	if ((ImplAAFTypeDef *)pPropType != this) // call operator ImplAAFTypeDef *
		return AAFRESULT_BAD_TYPE;
	
	//check to make sure that the size in the val data matches that of the native size
	aafUInt32 cbChar = 0;
	check_hr ( pvd->GetBitsSize(&cbChar) );
	
	if (cbChar != NativeSize())
	{
		return AAFRESULT_BAD_SIZE;
	}
	
	//ok all set with initial conditions
	//now set the value to the incoming character
	
	aafMemPtr_t pBits = NULL;
	check_hr ( pvd->GetBits (&pBits)  );
	ASSERT("Valid bits", pBits != 0);
	
	memcpy (pBits, &character, cbChar);
	
	return AAFRESULT_SUCCESS;
	
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::GetCharacter (
      ImplAAFPropertyValue * pCharacterValue,
      aafCharacter *  pCharacter)
{
  TRACE("ImplAAFTypeDefCharacter::GetCharacter");
  
	if (! pCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	if (! pCharacter)
		return AAFRESULT_NULL_PARAM;

	//get a pointer to the Val Data
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData*>(pCharacterValue);
	if (!pvd) return AAFRESULT_BAD_TYPE;
	
	// get the property value's embedded type
	ImplAAFTypeDefSP pPropType;
	check_hr ( pvd->GetType (&pPropType) );
	//Make sure the TD of the pv passed in, matches that of the ImplAAFTypeDefCharacter
	if ((ImplAAFTypeDef *)pPropType != this) // call operator ImplAAFTypeDef *
		return AAFRESULT_BAD_TYPE;
	
	//check to make sure that the size in the val data matches that of the native size
	aafUInt32 cbChar = 0;
	check_hr (  pvd->GetBitsSize(&cbChar) );

	if (cbChar != NativeSize())
	{
		return AAFRESULT_BAD_SIZE;
	}

	//Now set the character from that contained in the prop val data

	aafMemPtr_t pBits = NULL;
	check_hr ( pvd->GetBits (&pBits) );
	ASSERT("Valid bits", pBits != 0);
	
	memcpy (pCharacter, pBits, cbChar);
	
	return AAFRESULT_SUCCESS;
}




// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefCharacter::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t * pTid)
{
  if (!pTid)
    return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatCharacter;
  return AAFRESULT_SUCCESS;
}



// class ImplAAFTypeDefCharacter

const OMUniqueObjectIdentification&
ImplAAFTypeDefCharacter::identification(void) const
{
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDefCharacter::name(void) const
{
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDefCharacter::hasDescription(void) const
{
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDefCharacter::description(void) const
{
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDefCharacter::isPredefined(void) const
{
  return ImplAAFMetaDefinition::isPredefined();
}

bool ImplAAFTypeDefCharacter::isFixedSize(void) const
{
  bool result = false;
  if (IsFixedSize() == kAAFTrue) {
    result = true;
  }
  return result;
}

void ImplAAFTypeDefCharacter::reorder(OMByte* externalBytes,
                             OMUInt32 ANAME(externalBytesSize)) const
{
  TRACE("ImplAAFTypeDefCharacter::reorder");
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size", externalBytesSize == kExternalCharacterSize );

  reorderInteger(externalBytes, kExternalCharacterSize);
}

OMUInt32 ImplAAFTypeDefCharacter::externalSize(const OMByte* ANAME(internalBytes),
                                    OMUInt32 ANAME(internalBytesSize)) const
{
  TRACE("ImplAAFTypeDefCharacter::externalSize");

  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size", internalBytesSize >= kExternalCharacterSize);
  
  return kExternalCharacterSize;
}

OMUInt32 ImplAAFTypeDefCharacter::externalSize(void) const
{
  return PropValSize();
}

void ImplAAFTypeDefCharacter::externalize(const OMByte* internalBytes,
                                 OMUInt32 ANAME(internalBytesSize),
                                 OMByte* externalBytes,
                                 OMUInt32 externalBytesSize,
                                 OMByteOrder byteOrder) const
{
  TRACE("ImplAAFTypeDefCharacter::externalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size",
          internalBytesSize >= internalSize(externalBytes, externalBytesSize));
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size",
          externalBytesSize >= externalSize(internalBytes, internalBytesSize));

  if (sizeof(aafCharacter) == kExternalCharacterSize)
    copy(internalBytes, externalBytes, externalBytesSize);
  else
    contract(internalBytes, sizeof(aafCharacter), externalBytes, kExternalCharacterSize, byteOrder);
}

OMUInt32 ImplAAFTypeDefCharacter::internalSize(const OMByte* ANAME(externalBytes),
                                    OMUInt32 ANAME(externalBytesSize)) const
{
  TRACE("ImplAAFTypeDefCharacter::internalSize");

  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size", externalBytesSize == kExternalCharacterSize);

  return (sizeof(aafCharacter));
}

OMUInt32 ImplAAFTypeDefCharacter::internalSize(void) const
{
  return NativeSize();
}

void ImplAAFTypeDefCharacter::internalize(const OMByte* externalBytes,
                                 OMUInt32 ANAME(externalBytesSize),
                                 OMByte* internalBytes,
                                 OMUInt32 internalBytesSize,
                                 OMByteOrder byteOrder) const
{
  TRACE("ImplAAFTypeDefCharacter::internalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size",
          internalBytesSize >= internalSize(externalBytes, externalBytesSize));
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size",
          externalBytesSize >= externalSize(internalBytes, internalBytesSize));

  if (sizeof(aafCharacter) == kExternalCharacterSize)
    copy(externalBytes, internalBytes, internalBytesSize);
  else
    expand(externalBytes, kExternalCharacterSize, internalBytes, sizeof(aafCharacter), byteOrder);
}



void ImplAAFTypeDefCharacter::accept(OMTypeVisitor& visitor) const
{
  visitor.visitCharacterType(this);
}





//
// Overrides of ImplAAFTypeDef
//
//
// Override from AAFTypeDef

aafBool ImplAAFTypeDefCharacter::IsFixedSize (void) const
{
  return kAAFTrue;
}


OMUInt32 ImplAAFTypeDefCharacter::PropValSize (void) const
{
  return kExternalCharacterSize; // We only support 2 byte unicode characters.
}


aafBool ImplAAFTypeDefCharacter::IsRegistered (void) const
{
  // Only depends on registration of basic type.
  return kAAFTrue;
}


OMUInt32 ImplAAFTypeDefCharacter::NativeSize (void) const
{
  TRACE("ImplAAFTypeDefCharacter::NativeSize");
  ASSERT("Valid character size", (2 == sizeof(aafCharacter)) || (4 == sizeof(aafCharacter)));
  return (sizeof(aafCharacter));
}


OMProperty * ImplAAFTypeDefCharacter::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  TRACE("ImplAAFTypeDefCharacter::pvtCreateOMProperty");
  PRECONDITION("Valid name", name != 0);

  OMProperty * result = 0;

	result = new OMFixedSizeProperty<aafCharacter> (pid, name);

  POSTCONDITION("Valid property allocated", result != 0); // need better error handling!
  return result;
}



bool ImplAAFTypeDefCharacter::IsAggregatable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsStreamable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsFixedArrayable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsStringable () const
{ return true; }






// override from OMStorable.
const OMClassId& ImplAAFTypeDefCharacter::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefCharacter));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefCharacter::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefCharacter::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}
