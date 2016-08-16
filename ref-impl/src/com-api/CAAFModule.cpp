

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFModule.cpp,v 1.22 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "ImplAAFModule.h"
#include "AAFResult.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>











STDAPI
    AAFLoad (const char *  dllname)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFLoad
       (dllname);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}










STDAPI
    AAFFileOpenExistingRead (aafCharacter_constptr  pFileName,
        aafUInt32  modeFlags,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenExistingRead
       (pFileName,
        modeFlags,
        pinternalppFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenExistingModify (aafCharacter_constptr  pFileName,
        aafUInt32  modeFlags,
        aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenExistingModify
       (pFileName,
        modeFlags,
        pIdent,
        pinternalppFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenNewModify (aafCharacter_constptr  pFileName,
        aafUInt32  modeFlags,
        aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenNewModify
       (pFileName,
        modeFlags,
        pIdent,
        pinternalppFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenNewModifyEx (aafCharacter_constptr  pFileName,
        aafUID_constptr  pFileKind,
        aafUInt32  modeFlags,
        aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenNewModifyEx
       (pFileName,
        pFileKind,
        modeFlags,
        pIdent,
        pinternalppFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenTransient (aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenTransient
       (pIdent,
        pinternalppFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileIsAAFFile (aafCharacter_constptr  pFileName,
        aafUID_t *  pAAFFileKind,
        aafBool *  pFileIsAAFFile)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFFileIsAAFFile
       (pFileName,
        pAAFFileKind,
        pFileIsAAFFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFRawStorageIsAAFFile (IAAFRawStorage *  pRawStorage,
        aafUID_t *  pAAFFileKind,
        aafBool *  pRawStorageIsAAFFile)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFRawStorageIsAAFFile
       (pRawStorage,
        pAAFFileKind,
        pRawStorageIsAAFFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



STDAPI
    AAFFileIsAAFFileKind (aafCharacter_constptr  pFileName,
        aafUID_constptr  pAAFFileKind,
        aafBool *  pFileIsAAFFile)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFFileIsAAFFileKind
       (pFileName,
        pAAFFileKind,
        pFileIsAAFFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



STDAPI
    AAFRawStorageIsAAFFileKind (IAAFRawStorage *  pRawStorage,
        aafUID_constptr  pAAFFileKind,
        aafBool *  pRawStorageIsAAFFile)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFRawStorageIsAAFFileKind
       (pRawStorage,
        pAAFFileKind,
        pRawStorageIsAAFFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



STDAPI
    AAFGetPluginManager (IAAFPluginManager ** ppPluginManager)
{
  HRESULT hr;

  //
  // set up for ppPluginManager
  //
  ImplAAFPluginManager * internalppPluginManager = NULL;
  ImplAAFPluginManager ** pinternalppPluginManager = NULL;
  if (ppPluginManager)
    {
      pinternalppPluginManager = &internalppPluginManager;
    }

  try
    {
      hr = ImplAAFGetPluginManager
       (pinternalppPluginManager);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppPluginManager
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPluginManager)
        {
          pUnknown = static_cast<IUnknown *> (internalppPluginManager->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPluginManager, (void **)ppPluginManager);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPluginManager->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFCreateRawStorageMemory (aafFileAccess_t  access,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageMemory
       (access,
        pinternalppNewRawStorage);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFCreateRawStorageDisk (aafCharacter_constptr  pFilename,
        aafFileExistence_t  existence,
        aafFileAccess_t  access,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageDisk
       (pFilename,
        existence,
        access,
        pinternalppNewRawStorage);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFCreateRawStorageCachedDisk (aafCharacter_constptr  pFilename,
        aafFileExistence_t  existence,
        aafFileAccess_t  access,
        aafUInt32  pageCount,
        aafUInt32  pageSize,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageCachedDisk
       (pFilename,
        existence,
        access,
        pageCount,
        pageSize,
        pinternalppNewRawStorage);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


STDAPI
    AAFCreateRawStorageCached (IAAFRawStorage *  pRawStorage,
        aafUInt32  pageCount,
        aafUInt32  pageSize,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageCached
       (pRawStorage,
        pageCount,
        pageSize,
        pinternalppNewRawStorage);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


STDAPI
    AAFCreateRawStorageCached2 (IAAFRawStorage *  pRawStorage,
        aafUInt32  pageCount,
        aafUInt32  pageSize,
        IAAFCachePageAllocator*  pCachePageAllocator,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageCached2
       (pRawStorage,
        pageCount,
        pageSize,
        pCachePageAllocator,
        pinternalppNewRawStorage);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


STDAPI
    AAFCreateAAFFileOnRawStorage (IAAFRawStorage *  pRawStorage,
        aafFileExistence_t  existence,
        aafFileAccess_t  access,
        aafUID_constptr  pFileKind,
        aafUInt32  modeFlags,
        aafProductIdentification_constptr  pIdent,
        IAAFFile ** ppNewFile)
{
  HRESULT hr;

  //
  // set up for ppNewFile
  //
  ImplAAFFile * internalppNewFile = NULL;
  ImplAAFFile ** pinternalppNewFile = NULL;
  if (ppNewFile)
    {
      pinternalppNewFile = &internalppNewFile;
    }

  try
    {
      hr = ImplAAFCreateAAFFileOnRawStorage
       (pRawStorage,
        existence,
        access,
        pFileKind,
        modeFlags,
        pIdent,
        pinternalppNewFile);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppNewFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppNewFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


STDAPI
    AAFSetProgressCallback (IAAFProgress*  pProgress)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFSetProgressCallback
       (pProgress);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFGetFileEncodings (IEnumAAFFileEncodings ** ppFileEncodings)
{
  HRESULT hr;

  //
  // set up for ppFileEncodings
  //
  ImplEnumAAFFileEncodings * internalppFileEncodings = NULL;
  ImplEnumAAFFileEncodings ** pinternalppFileEncodings = NULL;
  if (ppFileEncodings)
    {
      pinternalppFileEncodings = &internalppFileEncodings;
    }

  try
    {
      hr = ImplAAFGetFileEncodings
       (pinternalppFileEncodings);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppFileEncodings
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFileEncodings)
        {
          pUnknown = static_cast<IUnknown *> (internalppFileEncodings->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFFileEncodings, (void **)ppFileEncodings);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFileEncodings->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


STDAPI
    AAFSetDiagnosticOutput (IAAFDiagnosticOutput*  pOutput)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFSetDiagnosticOutput
       (pOutput);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFGetLibraryVersion (aafProductVersion_t *  pVersion)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFGetLibraryVersion
       (pVersion);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFGetStaticLibraryVersion (aafProductVersion_t *  pVersion)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFGetStaticLibraryVersion
       (pVersion);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFGetLibraryPathNameBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFGetLibraryPathNameBufLen
       (pBufSize);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFGetLibraryPathName (aafCharacter *  pLibraryPathName,
        aafUInt32  bufSize)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFGetLibraryPathName
       (pLibraryPathName,
        bufSize);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


STDAPI
    AAFResultToTextBufLen (AAFRESULT  result,
        aafUInt32 *   pResultTextSize)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFResultToTextBufLen
       (result,
        pResultTextSize);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



STDAPI
    AAFResultToText (AAFRESULT  result,
        aafCharacter *  pResultText,
        aafUInt32  resultTextSize)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFResultToText
       (result,
        pResultText,
        resultTextSize);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


