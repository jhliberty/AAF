//=---------------------------------------------------------------------=
//
// $Id: aaflib.h,v 1.24 2009/06/01 11:46:57 stuart_hc Exp $ $Name: V116 $
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


#ifndef __aaflib_h__
#define __aaflib_h__

#include "AAF.h"
#include "AAFTypes.h"
#include "aafrdli.h"




//***********************************************************
// Define function prototypes in a manner consistent with the 
// ActiveX and OLE SDK's.

#if !defined( COMPILER_MSC )
//
// Compiler other than MS Visual C++
//

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENEXISTINGREAD)(
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENEXISTINGMODIFY)(
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENNEWMODIFY)(
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENNEWMODIFYEX)(
    const wchar_t *  pFileName,
	aafUID_constptr  pFileKind,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENTRANSIENT)(
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEISAAFFILE) (
    const wchar_t * pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFRAWSTORAGEISAAFFILE) (
    IAAFRawStorage * pstorage,
    aafUID_t *  pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEISAAFFILEKIND)(
    aafCharacter_constptr  pFileName,
    aafUID_constptr pAAFFileKind,
    aafBool *  pFileIsAAFFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFRAWSTORAGEISAAFFILEKIND)(
    IAAFRawStorage *  pStorage,
    aafUID_constptr pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETPLUGINMANAGER)(
    IAAFPluginManager ** ppPluginManager);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCREATERAWSTORAGEMEMORY)(
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCREATERAWSTORAGEDISK)(
    aafCharacter_constptr  pFilename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCREATERAWSTORAGECACHEDDISK)(
    aafCharacter_constptr  pFilename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCREATERAWSTORAGECACHED)(
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCREATERAWSTORAGECACHED2)(
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
    IAAFCachePageAllocator*  pCachePageAllocator,
	IAAFRawStorage ** ppNewRawStorage);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFCREATEAAFFILEONRAWSTORAGE)(
    IAAFRawStorage * pRawStorage,
	aafFileExistence_t  existence,
	aafFileAccess_t  access,
	aafUID_constptr  pFileKind,
	aafUInt32  modeFlags,
	aafProductIdentification_constptr  pIdent,
	IAAFFile ** ppNewFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFSETPROGRESSCALLBACK)(
    IAAFProgress * pProgress);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETFILEENCODINGS)(
    IEnumAAFFileEncodings ** ppFileEncodings);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFSETOUTPUTSTREAM)(
    IAAFDiagnosticOutput * pStream);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETLIBRARYVERSION)(
    aafProductVersion_t *  pVersion);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETLIBRARYPATHNAMEBUFLEN)(
    aafUInt32 *  pBufSize);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETLIBRARYPATHNAME)(
    aafCharacter *  pLibraryPathName,
    aafUInt32  bufSize);

typedef STDAPICALLTYPE HRESULT (* LPAAFRESULTTOTEXTBUFLEN) (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize);

typedef STDAPICALLTYPE HRESULT (* LPAAFRESULTTOTEXT) (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize);

#else
//
// MS Visual C++ compiler
//

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENEXISTINGREAD)(
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENEXISTINGMODIFY)(
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENNEWMODIFY)(
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENNEWMODIFYEX)(
    const wchar_t *  pFileName,
	aafUID_constptr  pFileKind,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENTRANSIENT)(
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEISAAFFILE) (
    const wchar_t * pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFRAWSTORAGEISAAFFILE) (
	IAAFRawStorage * pstorage,
    aafUID_t *  pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEISAAFFILEKIND)(
    aafCharacter_constptr  pFileName,
    aafUID_constptr pAAFFileKind,
    aafBool *  pFileIsAAFFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFRAWSTORAGEISAAFFILEKIND)(
    IAAFRawStorage *  pStorage,
    aafUID_constptr pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETPLUGINMANAGER)(
    IAAFPluginManager ** ppPluginManager);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCREATERAWSTORAGEMEMORY)(
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCREATERAWSTORAGEDISK)(
    aafCharacter_constptr  pFilename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCREATERAWSTORAGECACHEDDISK)(
    aafCharacter_constptr  pFilename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCREATERAWSTORAGECACHED)(
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCREATERAWSTORAGECACHED2)(
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
    IAAFCachePageAllocator*  pCachePageAllocator,
	IAAFRawStorage ** ppNewRawStorage);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFCREATEAAFFILEONRAWSTORAGE)(
    IAAFRawStorage * pRawStorage,
	aafFileExistence_t  existence,
	aafFileAccess_t  access,
	aafUID_constptr  pFileKind,
	aafUInt32  modeFlags,
	aafProductIdentification_constptr  pIdent,
	IAAFFile ** ppNewFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFSETPROGRESSCALLBACK)(
    IAAFProgress * pProgress);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETFILEENCODINGS)(
    IEnumAAFFileEncodings ** ppFileEncodings);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFSETOUTPUTSTREAM)(
    IAAFDiagnosticOutput * pStream);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETLIBRARYVERSION)(
    aafProductVersion_t *  pVersion);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETLIBRARYPATHNAMEBUFLEN)(
    aafUInt32 *  pBufSize);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETLIBRARYPATHNAME)(
    aafCharacter *  pLibraryPathName,
    aafUInt32  bufSize);

typedef HRESULT (STDAPICALLTYPE * LPAAFRESULTTOTEXTBUFLEN) (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize);

typedef HRESULT (STDAPICALLTYPE * LPAAFRESULTTOTEXT) (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize);

#endif


// Forward declarations
class AAFDLL;


//***********************************************************
// Abstract helper class to hide the platform dependent
// details for handling dll entrypoints.
//
class AAFDLL
{
protected:
  // Constructor and destructor for base class.
  AAFDLL();
  
public:
  virtual ~AAFDLL();

	// Factory method.
	static AAFDLL * MakeAAFDLL();


  // Return the singleton instance.
  static AAFDLL * GetAAFDLL();


  // Implement platform specific initialization of dll and 
	// entry points. NOTE: ALL ENTRY POINTS ARE REQUIRED! 
	// If any entry point cannot be found then the override 
	// of of Load MUST FAIL.
  HRESULT Load(const char *dllname);
  
  // Implement platform specific cleanup of dll and entry points.
  // NOTE: Implementations must be prepared to be called
  // even if their implementation of Load has failed.
  HRESULT Unload();
  
  // Resets all entry point function pointers to NULL.
  void ClearEntrypoints();


  //
  // Wrapper functions for calling member entry points.
  //
  HRESULT OpenExistingRead (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);
  
  HRESULT OpenExistingModify (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenNewModify (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenNewModifyEx (
    const wchar_t *  pFileName,
	aafUID_constptr  pFileKind,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenTransient (
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT IsAAFFile (
    const wchar_t * pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile);

  HRESULT RawStorageIsAAFFile (
    IAAFRawStorage * pStorage,
    aafUID_t *  pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile);

  HRESULT FileIsAAFFileKind (
    aafCharacter_constptr  pFileName,
    aafUID_constptr pAAFFileKind,
    aafBool *  pFileIsAAFFile);

  HRESULT RawStorageIsAAFFileKind (
    IAAFRawStorage *  pStorage,
    aafUID_constptr pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile);

  HRESULT GetPluginManager (
    IAAFPluginManager ** ppPluginManager);
  
  HRESULT CreateRawStorageMemory (
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage);
  
  HRESULT CreateRawStorageDisk (
    aafCharacter_constptr  pFilename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage);

  HRESULT CreateRawStorageCachedDisk (
    aafCharacter_constptr  pFilename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage);

  HRESULT CreateRawStorageCached (
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage);

  HRESULT CreateRawStorageCached2 (
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
    IAAFCachePageAllocator*  pCachePageAllocator,
	IAAFRawStorage ** ppNewRawStorage);

  HRESULT CreateAAFFileOnRawStorage (
    IAAFRawStorage * pRawStorage,
	aafFileExistence_t  existence,
	aafFileAccess_t  access,
	aafUID_constptr  pFileKind,
	aafUInt32  modeFlags,
	aafProductIdentification_constptr  pIdent,
	IAAFFile ** ppNewFile);

  HRESULT SetProgressCallback (
    IAAFProgress * pProgress);

  HRESULT GetFileEncodings (
    IEnumAAFFileEncodings ** ppFileEncodings);

  HRESULT SetDiagnosticOutput (
    IAAFDiagnosticOutput * pStreams);

  HRESULT GetLibraryVersion (
    aafProductVersion_t *  pVersion);

  HRESULT GetLibraryPathNameBufLen (
    aafUInt32 *  pBufSize);

  HRESULT GetLibraryPathName (
    aafCharacter *  pLibraryPathName,
    aafUInt32  bufSize);

  HRESULT ResultToTextBufLen (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize);

  HRESULT ResultToText (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize);

protected:
  //
  // The single instance of the dll wrapper.
  //
  static AAFDLL *_singleton;

	//
	// Platform independent (opaque) handle to the loaded dynamic library.
	//
	AAFLibraryHandle _libHandle;

  //
  // Callback function member data loaded by overridden versions
  // of the Load() method:
  //
  LPFNAAFFILEOPENEXISTINGREAD      _pfnOpenExistingRead;
  LPFNAAFFILEOPENEXISTINGMODIFY    _pfnOpenExistingModify;
  LPFNAAFFILEOPENNEWMODIFY         _pfnOpenNewModify;
  LPFNAAFFILEOPENNEWMODIFYEX       _pfnOpenNewModifyEx;
  LPFNAAFFILEOPENTRANSIENT         _pfnOpenTransient;
  LPFNAAFFILEISAAFFILE             _pfnIsAAFFile;
  LPFNAAFRAWSTORAGEISAAFFILE       _pfnRawStorageIsAAFFile;
  LPFNAAFFILEISAAFFILEKIND         _pfnFileIsAAFFileKind;
  LPFNAAFRAWSTORAGEISAAFFILEKIND   _pfnRawStorageIsAAFFileKind;
  LPFNAAFGETPLUGINMANAGER          _pfnGetPluginManager;
  LPFNAAFCREATERAWSTORAGEMEMORY    _pfnCreateRawStorageMemory;
  LPFNAAFCREATERAWSTORAGEDISK      _pfnCreateRawStorageDisk;
  LPFNAAFCREATERAWSTORAGECACHEDDISK _pfnCreateRawStorageCachedDisk;
  LPFNAAFCREATERAWSTORAGECACHED    _pfnCreateRawStorageCached;
  LPFNAAFCREATERAWSTORAGECACHED2   _pfnCreateRawStorageCached2;
  LPFNAAFCREATEAAFFILEONRAWSTORAGE _pfnCreateAAFFileOnRawStorage;
  LPFNAAFSETPROGRESSCALLBACK       _pfnSetProgressCallback;
  LPFNAAFGETFILEENCODINGS          _pfnGetFileEncodings;
  LPFNAAFSETOUTPUTSTREAM           _pfnSetDiagnosticOutput;
  LPFNAAFGETLIBRARYVERSION         _pfnGetLibraryVersion;
  LPFNAAFGETLIBRARYPATHNAMEBUFLEN  _pfnGetLibraryPathNameBufLen;
  LPFNAAFGETLIBRARYPATHNAME        _pfnGetLibraryPathName;
  LPAAFRESULTTOTEXTBUFLEN          _pfnResultToTextBufLen;
  LPAAFRESULTTOTEXT                _pfnResultToText;
};

#endif /* __aaflib_h__ */

