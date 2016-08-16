//=---------------------------------------------------------------------=
//
// $Id: ImplAAFTypeDefInt.cpp,v 1.40 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
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

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;

// Internal utilities to sign-extend or zero-fill.  Current
// implementations only allow integer sizes of 1, 2, 4, and 8 bytes.

//
// LSB-justifies and sign extends the input value, copying the result
// into the output value.  Requires that inVal and outVal are valid
// pointers, and that inValSize is no larger than outValSize.  Also
// requires that in/outValSize are supported values from the set {1,
// 2, 4, 8} bytes.
//
static void pvtSignExtend (const aafMemPtr_t inVal,
						   aafUInt32   inValSize,
						   aafMemPtr_t outVal,
						   aafUInt32   outValSize)
{
  aafInt32 localValue;	// only 4 bytes; see below for why it's OK.

  ASSERTU (inVal);
  ASSERTU (outVal);
  ASSERTU (inValSize <= outValSize);
  ASSERTU ((1 == inValSize) ||
		  (2 == inValSize) ||
		  (4 == inValSize) ||
		  (8 == inValSize));
  ASSERTU ((1 == outValSize) ||
		  (2 == outValSize) ||
		  (4 == outValSize) ||
		  (8 == outValSize));
  if (inValSize == outValSize)
	{
	  memcpy (outVal, inVal, inValSize);
	}
  else
	{
	  // At this point we know that inSize < outSize; the largest
	  // outSize can be is 8 bytes, so the largest inSize can be is 4
	  // bytes; that's why localValue can work as only a 4-byte int.
	  switch (inValSize)
		{
		case 1:
		  localValue = *((aafInt8*) inVal);
		  break;
		case 2:
		  localValue = *((aafInt16*) inVal);
		  break;
		case 4:
		  localValue = *((aafInt32*) inVal);
		  break;
		case 8:
		  // inval can't be 8 bytes
		  ASSERTU (0);
		default:
		  ASSERTU (0);
		}

	  switch (outValSize)
		{
		case 1:
		  // inval can't be 1 byte
		  ASSERTU (0);
		case 2:
		  *((aafInt16*) outVal) = (aafInt16) localValue;
		  break;
		case 4:
		  *((aafInt32*) outVal) = localValue;
		  break;
		case 8:
		  *((aafInt64*) outVal) = localValue;
		  break;
		default:
		  ASSERTU (0);
		}
	}
}


//
// LSB-justifies and zero-fills the input value, copying the result
// into the output value.  Requires that inVal and outVal are valid
// pointers, and that inValSize is no larger than outValSize.  Also
// requires that in/outValSize are supported values from the set {1,
// 2, 4, 8} bytes.
//
static void pvtZeroFill (const aafMemPtr_t inVal,
						   aafUInt32   inValSize,
						   aafMemPtr_t outVal,
						   aafUInt32   outValSize)
{
  aafUInt32 localValue;	// only 4 bytes; see below for why it's OK.

  ASSERTU (inVal);
  ASSERTU (outVal);
  ASSERTU (inValSize <= outValSize);
  ASSERTU ((1 == inValSize) ||
		  (2 == inValSize) ||
		  (4 == inValSize) ||
		  (8 == inValSize));
  ASSERTU ((1 == outValSize) ||
		  (2 == outValSize) ||
		  (4 == outValSize) ||
		  (8 == outValSize));
  if (inValSize == outValSize)
	{
	  memcpy (outVal, inVal, inValSize);
	}
  else
	{
	  // At this point we know that inSize < outSize; the largest
	  // outSize can be is 8 bytes, so the largest inSize can be is 4
	  // bytes; that's why localValue can work as only a 4-byte int.
	  switch (inValSize)
		{
		case 1:
		  localValue = *((aafUInt8*) inVal);
		  break;
		case 2:
		  localValue = *((aafUInt16*) inVal);
		  break;
		case 4:
		  localValue = *((aafUInt32*) inVal);
		  break;
		case 8:
		  // inval can't be 8 bytes
		  ASSERTU (0);
		default:
		  ASSERTU (0);
		}

	  switch (outValSize)
		{
		case 1:
		  // inval can't be 1 byte
		  ASSERTU (0);
		case 2:
		  *((aafUInt16*) outVal) = (aafUInt16) localValue;
		  break;
		case 4:
		  *((aafUInt32*) outVal) = localValue;
		  break;
		case 8:
		  // hack! we don't have unsigned 64-bit, so we'll depend on
		  // the compiler to sign-extend and zero-fill the unsigned
		  // localValue, even though the dest (64bit) is signed.
		  *((aafInt64*) outVal) = localValue;
		  break;
		default:
		  ASSERTU (0);
		}
	}
}


ImplAAFTypeDefInt::ImplAAFTypeDefInt ()
  : _size     ( PID_TypeDefinitionInteger_Size,     L"Size"),
    _isSigned ( PID_TypeDefinitionInteger_IsSigned, L"IsSigned")
{
  _persistentProperties.put(_size.address());
  _persistentProperties.put(_isSigned.address());
}

ImplAAFTypeDefInt::~ImplAAFTypeDefInt ()
{
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefInt::Initialize (
      const aafUID_t & id,
      aafUInt8  intSize,
      aafBool  isSigned,
      const aafCharacter * pTypeName)
{
  ASSERTU (intSize > 0);
  ASSERTU (pTypeName);

  if ((1 != intSize) &&
	  (2 != intSize) &&
	  (4 != intSize) &&
	  (8 != intSize))
	return AAFRESULT_BAD_SIZE;

  AAFRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _size = intSize;
  _isSigned = isSigned;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetTypeCategory (
      eAAFTypeCategory_t *  pTid)
{
  if (! pTid)
	return AAFRESULT_NULL_PARAM;

  *pTid = kAAFTypeCatInt;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::CreateValue (
      aafMemPtr_t  pVal,
      aafUInt32  valSize,
      ImplAAFPropertyValue ** ppPropVal)
{
  if (! pVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (! ppPropVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (valSize > static_cast<aafUInt32>(_size))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // current impl only allows 1, 2, 4, and 8-bit ints.
  if ((1 != valSize) &&
	  (2 != valSize) &&
	  (4 != valSize) &&
	  (8 != valSize))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // sign-extend or zero-fill the value.
  aafUInt8 valBuf[8];
  ASSERTU (static_cast<size_t>(_size) <= sizeof (valBuf));
  if (_isSigned != 0)
	{
	  pvtSignExtend (pVal, valSize, valBuf, _size);
	}
  else
	{
	  pvtZeroFill (pVal, valSize, valBuf, _size);
	}

  // Create a temporary pointer to copy to the smartptr
  ImplAAFPropValData * tmp = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (NULL == tmp)
	return AAFRESULT_NOMEMORY;
  ImplAAFPropValDataSP pv;
  pv = tmp;

  // Bobt: Hack bugfix! SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  tmp->ReleaseReference(); // we don't need this reference anymore.
  tmp = 0;

  AAFRESULT hr;
  hr = pv->Initialize(this);
  if (! AAFRESULT_SUCCEEDED (hr))
	return hr;

  aafMemPtr_t pBits = NULL;
  hr = pv->AllocateBits (_size, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr))
	return hr;

  ASSERTU (pBits);
  memcpy (pBits, valBuf, _size);

  *ppPropVal = pv;
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetInteger (
      ImplAAFPropertyValue * pPropVal,
      aafMemPtr_t            pVal,
      aafUInt32              valSize)
{
  if (! pPropVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (! pVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (valSize < static_cast<aafUInt32>(_size))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  if (!pvd) return AAFRESULT_BAD_TYPE;

  // get the property value's embedded type
  ImplAAFTypeDefSP pPropType;
  AAFRESULT hr;
  hr = pvd->GetType (&pPropType);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  ASSERTU (pPropType);

  // determine if the property value's embedded type is compatible
  // with this one for reading.  For now, we'll only allow integral
  // and enumeration type properties to be read by this integral type def.
  //
  eAAFTypeCategory_t	type_category = kAAFTypeCatUnknown;
  pPropType->GetTypeCategory( &type_category );
  if( type_category != kAAFTypeCatInt && type_category != kAAFTypeCatEnum )
	return AAFRESULT_BAD_TYPE;

  // current impl only allows 1, 2, 4, and 8-bit ints.
  if ((1 != valSize) &&
	  (2 != valSize) &&
	  (4 != valSize) &&
	  (8 != valSize))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // sign-extend or zero-fill the value.
  aafUInt8 valBuf[8];
  aafUInt32 bitsSize = 0;
  hr = pvd->GetBitsSize(&bitsSize);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  if (static_cast<aafUInt32>(_size) < bitsSize)
	{
	  return AAFRESULT_BAD_TYPE;
	}

  ASSERTU ((1 == bitsSize) ||
		  (2 == bitsSize) ||
		  (4 == bitsSize) ||
		  (8 == bitsSize));
  ASSERTU (bitsSize <= sizeof (valBuf));  // I know, redundant test...
  aafMemPtr_t pBits = NULL;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr)) return hr;
  ASSERTU (pBits);

  memcpy (valBuf, pBits, bitsSize);

  // BTW, we know that valsize >= bitsSize (from tests above)
  if (_isSigned != 0)
	{
	  pvtSignExtend (valBuf, bitsSize, pVal, valSize);
	}
  else
	{
	  pvtZeroFill (valBuf, bitsSize, pVal, valSize);
	}
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::SetInteger (
      ImplAAFPropertyValue * pPropVal,
      aafUInt8 *             pVal,
      aafUInt32              valSize)
{
  if (! pPropVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (! pVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (valSize > static_cast<aafUInt32>(_size))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  if (!pvd) return AAFRESULT_BAD_TYPE;

  // get the property value's embedded type
  ImplAAFTypeDefSP pPropType;
  AAFRESULT hr;
  hr = pvd->GetType (&pPropType);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  ASSERTU (pPropType);

  // determine if the property value's embedded type is compatible
  // with this one for reading.  For now, we'll only allow integral
  // and enumeration type properties to be read by this integral type def.
  //
  eAAFTypeCategory_t	type_category = kAAFTypeCatUnknown;
  pPropType->GetTypeCategory( &type_category );
  if( type_category != kAAFTypeCatInt && type_category != kAAFTypeCatEnum )
	return AAFRESULT_BAD_TYPE;

  // current impl only allows 1, 2, 4, and 8-bit ints.
  if ((1 != valSize) &&
	  (2 != valSize) &&
	  (4 != valSize) &&
	  (8 != valSize))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // sign-extend or zero-fill the value.
  aafUInt8 valBuf[8];
  ASSERTU (static_cast<size_t>(_size) <= sizeof (valBuf));
  if (_isSigned != 0)
	{
	  pvtSignExtend (pVal, valSize, valBuf, _size);
	}
  else
	{
	  pvtZeroFill (pVal, valSize, valBuf, _size);
	}

  aafMemPtr_t pBits = NULL;
  hr = pvd->AllocateBits (_size, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  ASSERTU (pBits);
  memcpy (pBits, valBuf, _size);

  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetSize (
      aafUInt32 *  pSize)
{
  if (! pSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pSize = _size;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::IsSigned (
      aafBool *  pSigned)
{
  if (! pSigned)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pSigned = _isSigned;
  return AAFRESULT_SUCCESS;
}


const OMUniqueObjectIdentification&
ImplAAFTypeDefInt::identification(void) const
{
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFTypeDefInt::name(void) const
{
  return ImplAAFMetaDefinition::name();
}

bool ImplAAFTypeDefInt::hasDescription(void) const
{
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFTypeDefInt::description(void) const
{
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFTypeDefInt::isPredefined(void) const
{
  return ImplAAFMetaDefinition::isPredefined();
}

void ImplAAFTypeDefInt::reorder(OMByte* bytes,
								OMUInt32 bytesSize) const

{
  ASSERTU (IsFixedSize());
  ASSERTU (PropValSize() == bytesSize);
  ASSERTU (bytes);
  if (bytesSize > 1)
	reorderInteger (bytes, bytesSize);
}


OMUInt32 ImplAAFTypeDefInt::externalSize(const OMByte* /*internalBytes*/,
										 OMUInt32 /*internalBytesSize*/) const
{
  ASSERTU (IsFixedSize());
  return PropValSize();
}


void ImplAAFTypeDefInt::externalize(const OMByte* internalBytes,
									OMUInt32 internalBytesSize,
									OMByte* externalBytes,
									OMUInt32 externalBytesSize,
									OMByteOrder byteOrder) const
{
  ASSERTU (internalBytes);
  ASSERTU (externalBytes);
  // ASSERTU (internalBytesSize == externalBytesSize);
  const OMUInt32 thisPropValSize = PropValSize ();
  ASSERTU (externalBytesSize == thisPropValSize);

  if (internalBytesSize > externalBytesSize)
	{
	  // contracting
	  contract (internalBytes,
				internalBytesSize,
				externalBytes,
				externalBytesSize,
				byteOrder);
	}

  else if (internalBytesSize < externalBytesSize)
	{
	  // expanding
	  expand (internalBytes,
				internalBytesSize,
				externalBytes,
				externalBytesSize,
				byteOrder);
	}

  else
	{
	  // size remains the same
	  copy (internalBytes,
			externalBytes,
			externalBytesSize);
	}
}


OMUInt32 ImplAAFTypeDefInt::internalSize(const OMByte* /*externalBytes*/,
										 OMUInt32 /*externalSize*/) const
{
  return NativeSize ();
}


void ImplAAFTypeDefInt::internalize(const OMByte* externalBytes,
									OMUInt32 externalBytesSize,
									OMByte* internalBytes,
									OMUInt32 internalBytesSize,
									OMByteOrder byteOrder) const
{
  ASSERTU (externalBytes);
  ASSERTU (internalBytes);
  // ASSERTU (internalBytesSize == externalBytesSize);
  // const OMUInt32 thisNativeSize = NativeSize ();
  // ASSERTU (internalBytesSize == thisNativeSize);

  if (externalBytesSize > internalBytesSize)
	{
	  // contracting
	  contract (externalBytes,
				externalBytesSize,
				internalBytes,
				internalBytesSize,
				byteOrder);
	}

  else if (externalBytesSize < internalBytesSize)
	{
	  // expanding
	  expand (externalBytes,
				externalBytesSize,
				internalBytes,
				internalBytesSize,
				byteOrder);
	}

  else
	{
	  // size remains the same
	  copy (externalBytes,
			internalBytes,
			internalBytesSize);
	}
}

void ImplAAFTypeDefInt::accept(OMTypeVisitor& visitor) const
{
  visitor.visitIntegerType(this);
}

OMUInt8 ImplAAFTypeDefInt::size(void) const
{
  // Should be properly implemented
  return _size;
}

bool ImplAAFTypeDefInt::isSigned(void) const
{
  bool result = false;
  if (_isSigned == kAAFTrue)
  {
    result = true;
  }

  return result;
}



aafBool ImplAAFTypeDefInt::IsFixedSize (void) const
{
  return kAAFTrue;
}


OMUInt32 ImplAAFTypeDefInt::PropValSize (void) const
{
  return _size;
}


aafBool ImplAAFTypeDefInt::IsRegistered (void) const
{
  // int types are registered by default
  return kAAFTrue;
}


OMUInt32 ImplAAFTypeDefInt::NativeSize (void) const
{
  // same as property value size
  return PropValSize();
}


OMProperty * ImplAAFTypeDefInt::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  ASSERTU (name);
  OMUInt32 es = PropValSize ();
  ASSERTU(es <= OMPROPERTYSIZE_MAX);
  OMPropertySize elemSize = static_cast<OMPropertySize>(es);
  OMProperty * result = new OMSimpleProperty (pid, name, elemSize);
  ASSERTU (result);
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  // Return variable array of unsigned char
  return pvtGetUInt8Array8Type (ppRawTypeDef);
}


bool ImplAAFTypeDefInt::IsAggregatable () const
{ return true; }

bool ImplAAFTypeDefInt::IsStreamable () const
{ return true; }

bool ImplAAFTypeDefInt::IsFixedArrayable () const
{ return true; }

bool ImplAAFTypeDefInt::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefInt::IsStringable () const
{ return true; }






// override from OMStorable.
const OMClassId& ImplAAFTypeDefInt::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefInt));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefInt::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefInt::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}
