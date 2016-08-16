//=---------------------------------------------------------------------=
//
// $Id: StructuredStorage.h,v 1.2 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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

#ifndef __STRUCTUREDSTORAGE_H__
#define __STRUCTUREDSTORAGE_H__


//include Structured Storage header files 
#if defined(_WIN32)
#include <objbase.h>
#elif defined(__sgi) || defined(__linux__) || defined (__FreeBSD__) || \
      defined (__APPLE__) || defined(__CYGWIN__)
#include "h/storage.h"
#define S_OK           0x00000000
#define S_FALSE        0x00000001
   // NOTE: S_FALSE is not -1
#else
#error "host operating system not supported"
#endif



class StructuredStorage
{
public:
    static StructuredStorage* Create(void);
    
public:
    virtual ~StructuredStorage();                                

    // Note: we should migrate to StgOpenStorageEx for win2000 and XP
	virtual HRESULT STDMETHODCALLTYPE StgOpenStorage(
          const TCHAR FAR* pwcsName,
          IStorage* pstgPriority,
          DWORD grfMode,
          SNB snbExclude,
          DWORD reserved,
          IStorage** ppstgOpen) = 0;
          
    virtual HRESULT STDMETHODCALLTYPE StgIsStorageFile(
          const TCHAR FAR* pwcsName) = 0;

public: 
    // function is used to free memory allocated by the ss library for a string                                
    virtual void wcsFree(TCHAR FAR* wcs) = 0;

protected:
    StructuredStorage();
};

#endif // __STRUCTUREDSTORAGE_H__
