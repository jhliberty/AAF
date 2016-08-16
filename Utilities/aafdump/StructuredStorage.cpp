//=---------------------------------------------------------------------=
//
// $Id: StructuredStorage.cpp,v 1.3 2009/06/09 02:06:50 stuart_hc Exp $ $Name: V116 $
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
//
//=---------------------------------------------------------------------=

#include <StructuredStorage.h>
#include <assert.h>

// Determine host operating system.
//
#if defined(_WIN32)
#define OM_OS_WINDOWS
#elif defined(__MWERKS__) && defined(__MACH__)
#define OM_OS_MACOSX
#elif defined(__sgi) || defined(__linux__) || defined (__FreeBSD__) || \
      defined (__APPLE__) || defined(__CYGWIN__)
#define OM_OS_UNIX
#elif defined (sun)
#define OM_OS_SOLARIS
#else
#error "Can't determine host operating system"
#endif

#if defined(OM_OS_WINDOWS)

#define OM_USE_WINDOWS_SS
#include <MStructuredStorage.h>

#elif defined(OM_OS_UNIX)

#ifndef OM_USE_GSF_SS
#define OM_USE_GSF_SS
#endif
#include <GSFStructuredStorage.h>

#else

#error "Don't know which implementation of structured storage to use."

#endif


StructuredStorage* 
StructuredStorage::Create(void)
{
#if defined(OM_USE_WINDOWS_SS)
    StructuredStorage* storage = new MStructuredStorage();
#elif defined(OM_USE_GSF_SS)
    StructuredStorage* storage = new GSFStructuredStorage();
#else
#error "Don't know which implementation of structured storage to use."
#endif
    assert(storage != 0);
    
    return storage;    
}
    
StructuredStorage::StructuredStorage(void)
{
    // nothing
}

StructuredStorage::~StructuredStorage(void)
{
    // nothing
}    
