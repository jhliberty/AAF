//=---------------------------------------------------------------------=
//
// $Id: OMDataTypes.h,v 1.55 2012/05/16 15:35:11 jptrainor Exp $ $Name: V116 $
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

// @doc OMEXTERNAL
#ifndef OMDATATYPES_H
#define OMDATATYPES_H

// @module OMDataTypes | Host independent data type definitions used
//         by the Object Manager.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

#include "OMPortability.h"

// If your compiler is not listed please add code to detect it in
// OMPortability.h and add a section defining the OM[U]Int{8|16|32}64}
// types below.
// This allows new platforms to be added without disturbing the code
// for existing platforms.
//
#if defined(OM_COMPILER_MSC_INTEL_WINDOWS) || \
    defined(OM_COMPILER_MSC_X64_WINDOWS)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef __int64                OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned __int64       OMUInt64;

#define INT64_C(c)	c
#define UINT64_C(c)	c

#elif defined(OM_COMPILER_GCC_INTEL_LINUX) || defined(OM_COMPILER_GCC_X86_64_LINUX) \
   || defined(OM_COMPILER_GCC_PPC_LINUX) \
   || defined(OM_COMPILER_GCC_PPC64_LINUX) \
   || defined(OM_COMPILER_GCC_PPC_MACOSX) || defined(OM_COMPILER_MWERKS_PPC_MACOSX) \
   || defined(OM_COMPILER_GCC_INTEL_MACOSX) || defined(OM_COMPILER_GCC_X86_64_MACOSX) \
   || defined(OM_COMPILER_SGICC_MIPS_SGI) || defined(OM_COMPILER_GCC_MIPS_SGI) \
   || defined(OM_COMPILER_GCC_SPARC_SUNOS) \
   || defined(OM_COMPILER_GCC_INTEL_FREEBSD) || defined(OM_COMPILER_GCC_INTEL_OPENBSD) \
   || defined(OM_COMPILER_GCC_INTEL_WINDOWS) || defined(OM_COMPILER_GCC_INTEL_CYGWIN)

// Use ISO C99 (also ANSI and POSIX) fixed size integers

#define __STDC_CONSTANT_MACROS	// needed in C++ to define INT64_C, UINT64_C
#include <inttypes.h>
typedef int8_t                 OMInt8;
typedef int16_t                OMInt16;
typedef int32_t                OMInt32;
typedef int64_t                OMInt64;

typedef uint8_t                OMUInt8;
typedef uint16_t               OMUInt16;
typedef uint32_t               OMUInt32;
typedef uint64_t               OMUInt64;

#else
#error "Unknown compiler"
#endif

typedef OMUInt8                OMByte;
typedef OMUInt16               OMCharacter;

const OMInt8   OMINT8_MIN  = (OMInt8) ((OMInt8) 1 << ( 8 - 1));
const OMInt8   OMINT8_MAX  = ~OMINT8_MIN;
const OMInt16  OMINT16_MIN = (OMInt16)((OMInt16)1 << (16 - 1));
const OMInt16  OMINT16_MAX = ~OMINT16_MIN;
const OMInt32  OMINT32_MIN = (OMInt32)((OMInt32)1 << (32 - 1));
const OMInt32  OMINT32_MAX = ~OMINT32_MIN;
const OMInt64  OMINT64_MIN = (OMInt64)((OMInt64)1 << (64 - 1));
const OMInt64  OMINT64_MAX = ~OMINT64_MIN;

const OMUInt8  OMUINT8_MAX  = (OMUInt8)(~(OMUInt8) 0);
const OMUInt16 OMUINT16_MAX = (OMUInt16)(~(OMUInt16)0);
const OMUInt32 OMUINT32_MAX = (OMUInt32)(~(OMUInt32)0);
const OMUInt64 OMUINT64_MAX = (OMUInt64)(~(OMUInt64)0);

typedef OMUInt16 OMPropertyId;
typedef OMUInt16 OMPropertySize;
typedef OMUInt16 OMPropertyCount;
typedef OMUInt32 OMPropertyOffset;
typedef OMUInt16 OMStoredForm;
typedef OMUInt8  OMVersion;
typedef OMUInt8  OMKeySize;

const OMUInt16 OMPROPERTYSIZE_MAX   = OMUINT16_MAX;
const OMUInt16 OMPROPERTYCOUNT_MAX  = OMUINT16_MAX;
const OMUInt32 OMPROPERTYOFFSET_MAX = OMUINT32_MAX;
const OMUInt8  OMKEYSIZE_MAX        = OMUINT8_MAX;

typedef OMUInt8 OMByteOrder;

const OMByteOrder littleEndian      = 0x4c; // 'L' for little
const OMByteOrder bigEndian         = 0x42; // 'B' for big
const OMByteOrder unspecified       = 0x55; // 'U' for unspecified

#include <string.h>  // For use of memcmp() below.

// Template to declare a set of constants of
// the specified type.
//
template<typename T>
struct OMConstant
{
  static const T null;
};

// A GUID in all but name.
//
struct OMObjectIdentification
{
    OMUInt32 Data1;
    OMUInt16 Data2;
    OMUInt16 Data3;
    OMUInt8  Data4[8];
};

// OMObjectIdentification comparison operators.
//
inline bool operator == (const OMObjectIdentification& lhs,
                         const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) == 0;
}

inline bool operator != (const OMObjectIdentification& lhs,
                         const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) != 0;
}

inline bool operator < (const OMObjectIdentification& lhs,
                        const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) < 0;
}

inline bool operator > (const OMObjectIdentification& lhs,
                        const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) > 0;
}

// For identifying the class of a persisted object.
//
typedef OMObjectIdentification OMStoredObjectIdentification;

// For uniquely identifying an object instance.
//
typedef OMObjectIdentification OMUniqueObjectIdentification;

// For backwards compatibility - all instances of
// nullOMUniqueObjectIdentification should eventually be replaced
// with OMConstant<OMUniqueObjectIdentification>::null.
//
const OMUniqueObjectIdentification nullOMUniqueObjectIdentification = {0,0,0,{0}};

// For backwards compatibility - all instances of OMClassId should
// eventually be replaced with OMStoredObjectIdentification.
//
typedef OMStoredObjectIdentification OMClassId;

const OMClassId nullOMClassId = {0,0,0,{0}};

// A UMID in all but name.
//
struct OMMaterialIdentification
{
  OMUInt8 SMPTELabel[12];
  OMUInt8 length;
  OMUInt8 instanceHigh;
  OMUInt8 instanceMid;
  OMUInt8 instanceLow;
  OMObjectIdentification material;
};

// OMMaterialIdentification comparison operators.
//
inline bool operator == (const OMMaterialIdentification& lhs,
                         const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) == 0;
}

inline bool operator != (const OMMaterialIdentification& lhs,
                         const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) != 0;
}

inline bool operator < (const OMMaterialIdentification& lhs,
                        const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) < 0;
}

inline bool operator > (const OMMaterialIdentification& lhs,
                        const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) > 0;
}

// For uniquely identifying material instances.
//
typedef OMMaterialIdentification OMUniqueMaterialIdentification;

// For (but more general than) a SMPTE Universal label
struct OMKLVKey
{
  OMUInt8 octet0;
  OMUInt8 octet1;
  OMUInt8 octet2;
  OMUInt8 octet3;
  OMUInt8 octet4;
  OMUInt8 octet5;
  OMUInt8 octet6;
  OMUInt8 octet7;
  OMUInt8 octet8;
  OMUInt8 octet9;
  OMUInt8 octet10;
  OMUInt8 octet11;
  OMUInt8 octet12;
  OMUInt8 octet13;
  OMUInt8 octet14;
  OMUInt8 octet15;
};

// OMKLVKey comparison operators.
//
inline bool operator == (const OMKLVKey& lhs,
                         const OMKLVKey& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMKLVKey)) == 0;
}

inline bool operator != (const OMKLVKey& lhs,
                         const OMKLVKey& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMKLVKey)) != 0;
}

inline bool operator < (const OMKLVKey& lhs,
                        const OMKLVKey& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMKLVKey)) < 0;
}

inline bool operator > (const OMKLVKey& lhs,
                        const OMKLVKey& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMKLVKey)) > 0;
}

const OMKLVKey nullOMKLVKey =
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Stored object encodings
//
typedef OMObjectIdentification OMStoredObjectEncoding;
const OMStoredObjectEncoding nullOMStoredObjectEncoding = {0,0,0,{0}};

// Property instance tags.
//
typedef OMUInt16 OMPropertyTag;

const OMPropertyTag nullOMPropertyTag = (OMPropertyTag)(~(OMPropertyTag)0);

// wide string format
//
#if defined(_MSC_VER)
#define OMWFMT64 L"I64"
#elif defined(__x86_64__)
#define OMWFMT64 L"l"
#else			// all 32bit platforms using POSIX compilers
#define OMWFMT64 L"ll"
#endif

#endif
