#ifndef __AAFFileMode_h__
#define __AAFFileMode_h__

//=---------------------------------------------------------------------=
//
// $Id: AAFFileMode.h,v 1.9 2009/06/01 11:46:57 stuart_hc Exp $ $Name: V116 $
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

//
// Mode flags for use in AAFFileOpenXXX() functions and
// IAAFFile::SaveAs() method.
//
// Combinations of flags may be bitwise OR'd together.  If any
// specific combinations are illegal, those will be explicitly noted.
//
// All bits not specified here are reserved.
//
// All reserved bits must be set to zero.
//

// Control of lazy/eager loading
//
// - When set, the bit signifies lazy loading.
// - When clear (the default), eager loading is specified.
//
// The following applies to:
// - AAFFileOpenExistingRead()
// - AAFFileOpenExistingModify()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_LAZY_LOADING        (1 << 0)

// Control of reversion
//
// - When set, the bit signifies that the file will be revertable.
// - When clear (the default), the file will not be revertable.
//
// The following applies to:
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_REVERTABLE           (1 << 1)

// Control of OS buffering
//
// - When set, the bit signifies that the file is to be accessed in
//   unbuffered mode.
// - When clear (the default), the file will be accessed in buffered
//   mode.
//
// The following applies to:
// - AAFFileOpenExistingRead()
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_UNBUFFERED           (1 << 2)

// Control of reclamation of memory used by clean objects
//
// - When set, the bit signifies that memory for clean objects may be
//   reclaimed.
// - When clear (the default), the memory for clean objects will not
//   be reclaimed.
//
// The following applies to:
// - AAFFileOpenExistingRead()
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_RECLAIMABLE          (1 << 3)

// ...

// Unpublished mode flags

// Control of use of large sector size mode in Structured Storage
// The following applies to:
//
// - When set, the bit signifies that large sector size mode will be
//   used in Structured Storage (ie. 4K sectors).
// - When clear (the default), normal-sized sectors will be used in
//   Structured Storage (ie. 512 byte sectors).
//
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_USE_LARGE_SS_SECTORS    (1 << 28)

// Indicates if Close() should fail when there are dirty objects
//
// - When set, the bit signifies that Close() should fail if any
//   objects are dirty (modified and unsaved).
// - When clear (the default), Close() will succeed even if any
//   objects are dirty.
//
// The following applies to:
// - AAFFileOpenExistingModify()
// - AAFFileOpenExistingWrite()
// - IAAFFile::SaveAs()
#define AAF_FILE_MODE_CLOSE_FAIL_DIRTY     (1 << 29)


// Reserved bits

#define AAF_FILE_MODE_DEBUG0_ON            (1 << 30)
#define AAF_FILE_MODE_DEBUG1_ON            (1 << 31)


#endif // __AAFFileMode_h__
