//=---------------------------------------------------------------------=
//
// $Id: CAAFEssenceRIFFWAVEStream.cpp,v 1.7 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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

#include "CAAFEssenceRIFFWAVEStream.h"
#include "CAAFEssenceRIFFWAVEContainer.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "AAFResult.h"

#include <errno.h>


// Changes Nov01: fpos_t is supposed to be opaque ... Glibc 2.2 uses a Struct
// Attempt to use alternative 64 bit seeks where possible using XOpen5 fseeko

// Irix MipsPro differences
#if defined (__sgi) && !defined(__GLIBC__)
typedef off64_t off_t;
#define fseeko fseeko64
#define ftello ftello64

#elif defined (_WIN32)		// Win32 differences

#if defined (COMPILER_MSC)
typedef __int64 off_t;
// __OFF_T_DEFINED prevents MSVC redefining off_t in wchar.h
// which is included by utf8.h.
typedef long _off_t;
#define _OFF_T_DEFINED
#endif

#define fseeko(fp, off, whence) win_fseeko(fp, off, whence)
static int win_fseeko(FILE *fp, off_t off, int whence)
{
        if (whence == SEEK_SET) {
                fpos_t pos = off;
                return fsetpos(fp, &pos);
        }

        if (whence == SEEK_END) {
        // Seek to end-of-file using non-64bit fseek
            if (0 != fseek(fp, 0, SEEK_END))
            return -1;
        // Handle any offset from end-of-file by falling through
        // to 64bit capable relative seek below
        }

        // To reach here whence is SEEK_CUR
        // First get current pos, then add required offset
        fpos_t pos = 0;
        if (0 != fgetpos(fp, &pos))
                        return -1;
        pos += off;
        return fsetpos(fp, &pos);
}

#define ftello(fp) win_ftello(fp)
static off_t win_ftello(FILE* fp)
{
  fpos_t pos;

  if(0 != fgetpos(fp, &pos))
     return -1;

  off_t result = pos;
  return result;
}

#else

// All POSIX systems
#include <sys/types.h>		// for off_t definition

#endif


#include "utf8.h"

// Debugging log function which is optimised away for default builds
#include <stdarg.h>
static void plugin_trace(const char *fmt, ...)
{
#ifdef PLUGIN_TRACE
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}


static bool AafPos2XopenOff(off_t *xopenOff, const aafPosition_t *aafPos)
{
#if ! defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS < 64
  // For platforms where off_t is not 64bits check that aafPosition_t (64bits)
  // can be converted safely to a 32bit offset.
  aafPosition_t trunPos = (AAFCONSTINT64(0x00000000FFFFFFFF) & *aafPos);
  aafPosition_t negOne = AAFCONSTINT64(0xFFFFFFFFFFFFFFFF);
  if (trunPos != *aafPos && negOne != *aafPos)
    return false;
#endif

  *xopenOff = *aafPos;

  return true;
}


static bool XopenOff2AafPos(aafPosition_t *aafPos, const off_t *xopenOff)
{
  // For first version just assume that platform can perform conversion.
  *aafPos = *xopenOff;

  return true;
}



//
// Constructor 
//
CAAFEssenceRIFFWAVEStream::CAAFEssenceRIFFWAVEStream (CAAFEssenceRIFFWAVEContainer *pContainer,
                                              CAAFEssenceRIFFWAVEStream *pPrevFileStream)
  : CAAFUnknown(NULL),
  _pContainer(pContainer),
  _pPrevFileStream(pPrevFileStream),
  _pwPath(NULL),
  _pMobID(NULL),
  _pPath(NULL),
  _pFile(NULL),
  _streamMode(openUnknown),
  _lastStreamOp(opUnknown)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::CAAFEssenceRIFFWAVEStream()\n");
  assert(NULL != pContainer);

  // We are saving a reference to our container so we need to increment
  // the referencen count.
  _pContainer->AddRef();
}


CAAFEssenceRIFFWAVEStream::~CAAFEssenceRIFFWAVEStream ()
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::~CAAFEssenceRIFFWAVEStream()\n");

  // Cleanup memory 
  CleanupBuffers();

  // Make sure the file handle is closed.
  CloseFile();

  // Remove this file stream frome the container's private list of 
  // open file streams.
  RemoveFileStreamFromContainer();

  // We are finished with our container's reference so we need
  // to decrement the reference count.
  _pContainer->Release();
  _pContainer = NULL;
}

const wchar_t * CAAFEssenceRIFFWAVEStream::FilePath() const 
{ 
  return _pwPath; 
}

CAAFEssenceRIFFWAVEStream* CAAFEssenceRIFFWAVEStream::PrevFileStream() const
{
  return _pPrevFileStream;
}

void CAAFEssenceRIFFWAVEStream::SetPrevFileStream(CAAFEssenceRIFFWAVEStream *pPrevFileStream)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::SetPrevFileStream()\n");

  _pPrevFileStream = pPrevFileStream;
}

void CAAFEssenceRIFFWAVEStream::RemoveFileStreamFromContainer()
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::RemoveFileStreamFromContainer()\n");

  assert(NULL != _pContainer);
  CAAFEssenceRIFFWAVEStream *pLastFileStream = _pContainer->LastFileStream();
  assert(NULL != pLastFileStream);
  
  // If this stream is the last stream then just set the new last file stream
  // to the previous file stream.
  if (pLastFileStream == this)
  {
    _pContainer->SetLastFileStream(pLastFileStream->_pPrevFileStream);
  }
  else
  { 
    // Search for the this stream in the containers list.
    CAAFEssenceRIFFWAVEStream *pCurrentFileStream = pLastFileStream;
    CAAFEssenceRIFFWAVEStream *pPreviousFileStream = NULL;
    while (pCurrentFileStream && pCurrentFileStream != this)
    {
      pPreviousFileStream = pCurrentFileStream;
      pCurrentFileStream = pCurrentFileStream->_pPrevFileStream;
    }

    assert(pCurrentFileStream == this);
    // Remove this link in the container's file stream list...
    if (pCurrentFileStream == this)
      pPreviousFileStream->_pPrevFileStream = _pPrevFileStream;
  }
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::Init (const aafCharacter * pFilePath,
        aafMobID_constptr pMobID)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::Init()\n");

  if (NULL == pFilePath)
    return E_INVALIDARG;

  // Return an error if this stream has already been opened for
  // this instance.
  if (_pFile)
    return AAFRESULT_ALREADY_OPEN;  // AAFRESULT_STREAM_REOPEN ???

  // Cleanup any previously allocated buffers. 
  CleanupBuffers();

  // Computer the number of wide and multibyte characters.
  size_t i;
  size_t charCount = 0;
  for (i = 0; pFilePath[i]; ++i)
    ;
  charCount = i + 1; // include the terminating null.

  // Copy the wide character path name.
  _pwPath = new wchar_t[charCount];
  if (NULL == _pwPath)
    return AAFRESULT_NOMEMORY;
  for (i = 0; i < charCount; ++i)
    _pwPath[i] = pFilePath[i];
  
  // Convert the wide character path the an utf8 character path.
  size_t byteCount;
  byteCount=wcsu8slen(_pwPath)+1;

  _pPath = new char[byteCount];
  if (NULL == _pPath)
    return AAFRESULT_NOMEMORY;
  size_t convertedBytes = wcstou8s( _pPath, _pwPath, byteCount);
  if ((size_t)-1 == convertedBytes)
    return E_INVALIDARG;

  //
  // Copy the optional mobID it it exists.
  if (pMobID)
  {
    _pMobID = new aafMobID_t;
    if (NULL == _pMobID)
      return AAFRESULT_NOMEMORY;
    memcpy(_pMobID, pMobID, sizeof(aafMobID_t));
  }


	return S_OK;
}


// Cleanup any internally allocated buffers.
void CAAFEssenceRIFFWAVEStream::CleanupBuffers(void)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::CleanupBuffers()\n");

  if (_pwPath)
  {
    delete [] _pwPath;
    _pwPath = NULL;
  }

  if (_pMobID)
  {
    delete _pMobID;
    _pMobID = NULL;
  }

  if (_pPath)
  {
    delete [] _pPath;
    _pPath = NULL;
  }
}


// Close the currently opened file handle if it exists.
void CAAFEssenceRIFFWAVEStream::CloseFile()
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::CloseFile()\n");

  // Make sure the file handle is closed.
  if (_pFile)
  {
    fclose(_pFile);
    _pFile = NULL;
  }
}



// Use the internal path buffers to test to see if the file already exists.
bool CAAFEssenceRIFFWAVEStream::FileAlreadyExists()
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::FileAlreadyExists()\n");

  errno = 0;

  FILE *pFile = fopen(_pPath, "rb");
  if (NULL != pFile)
  {
    fclose(pFile);
    return true;
  }

  return false;
}


// Utility to get the end of file position using ansi functions.
bool CAAFEssenceRIFFWAVEStream::GetEOFPos(fpos_t *pos)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::GetEOFPos()\n");

  if (!_pFile || !pos)
    return false;

  // Save the current position.
  fpos_t currentPos;
  errno = 0;
  if (0 != fgetpos(_pFile, &currentPos))
    return false;

  errno = 0;
  if (0 != fseek(_pFile, 0, SEEK_END))
    return false;

  errno = 0;
  if (0 != fgetpos(_pFile, pos))
    return false;

  //
  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSetpos);

  errno = 0;
  if (0 != fsetpos(_pFile, &currentPos))
    return false;

  return true;
}

//
// Utility to set the current operation and return the last
// stream operation. This is used to synchronzize read and write operations.
//
CAAFEssenceRIFFWAVEStream::FileStreamOp CAAFEssenceRIFFWAVEStream::SetStreamOp(FileStreamOp streamOp) 
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::SetStreamOp()\n");
 
  FileStreamOp tmpOp = _lastStreamOp; 
  _lastStreamOp = streamOp;
  return tmpOp;
}




HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::Create (const aafCharacter *  pFilePath,
        aafMobID_constptr  pMobID)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::Create()\n");

  HRESULT hr = Init(pFilePath, pMobID);
  if (AAFRESULT_SUCCESS != hr)
    return hr;

  // Make sure that we do NOT attempt to overwrite an existing file
  // by first attempting to open the file for reading. If this succeeds then
  // the file exists and we close the file and return an error.
  if (FileAlreadyExists())
    return AAFRESULT_FILE_EXISTS;

  // Attempt to create a new file for reading and writing.
  errno = 0;
#if defined(OS_WINDOWS)
  _pFile = _wfopen(_pwPath, L"w+b");
#else
  _pFile = fopen(_pPath, "w+b");
#endif
  if (NULL == _pFile)
  {
    return AAFRESULT_BADOPEN;
  }

  SetStreamMode(openNew);
  
  _startingEOF = 0;

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::OpenRead (const aafCharacter * pFilePath,
        aafMobID_constptr pMobID)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::OpenRead()\n");

  HRESULT hr = Init(pFilePath, pMobID);
  if (AAFRESULT_SUCCESS != hr)
    return hr;

  // Attempt to open the file for reading.
  errno = 0;
  _pFile = fopen(_pPath, "rb");
  if (NULL == _pFile)
  { //If the file does not exist or cannot be found return an error.
    return AAFRESULT_FILE_NOT_FOUND;
  }

  SetStreamMode(openRead);
  // Save the current position.
  fpos_t currentPos;
  errno = 0;
  if (0 != fgetpos(_pFile, &currentPos))
    return AAFRESULT_INTERNAL_ERROR;

  errno = 0;
  if (0 != fseek(_pFile, 0, SEEK_END))
    return AAFRESULT_INTERNAL_ERROR;

  errno = 0;
  if ((_startingEOF = ftello(_pFile)) < 0)
    return AAFRESULT_INTERNAL_ERROR;

  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSetpos);

  errno = 0;
  if (0 != fsetpos(_pFile, &currentPos))
    return AAFRESULT_INTERNAL_ERROR;

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::OpenAppend (const aafCharacter * pFilePath,
        aafMobID_constptr  pMobID)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::OpenAppend()\n");

  HRESULT hr = Init(pFilePath, pMobID);
  if (AAFRESULT_SUCCESS != hr)
    return hr;

  // Attempt to open the file for appending and reading.

  //1. _pFile = fopen(_pPath, "a+b"); cannot write after reading
  //2. _pFile = fopen(_pPath, "ab"); cannot read
  //3. _pFile = fopen(_pPath, "ra+b"); read-only
  //4. _pFile = fopen(_pPath, "a+rb"); same as 1.

  // Make sure that we do NOT attempt to overwrite an existing file
  // by first attempting to open the file for reading. If this succeeds then
  // the file exists and we close the file and return an error.
  if (FileAlreadyExists())
  {
    // Open the file read/write and explicitly enforce the "append" mode.
    errno = 0;
    _pFile = fopen(_pPath, "a+b"); // now works without pre-compiled headers!
  }
  else
  {
    // Open the file read/write and explicitly enforce the "append" mode.
    errno = 0;
    _pFile = fopen(_pPath, "w+b");
  }

  if (NULL == _pFile)
  { //If the file does not exist or cannot be found return an error.
    return AAFRESULT_FILE_NOT_FOUND;
  }
  
 

  SetStreamMode(openAppend);
  // Save the current position.
  fpos_t currentPos;
  errno = 0;
  if (0 != fgetpos(_pFile, &currentPos))
    return AAFRESULT_INTERNAL_ERROR;

  errno = 0;
  if (0 != fseek(_pFile, 0, SEEK_END))
    return AAFRESULT_INTERNAL_ERROR;

  errno = 0;
  if ((_startingEOF = ftello(_pFile)) < 0)
    return AAFRESULT_INTERNAL_ERROR;

  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSetpos);

  errno = 0;
  if (0 != fsetpos(_pFile, &currentPos))
    return AAFRESULT_INTERNAL_ERROR;

  return AAFRESULT_SUCCESS;
}







HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::Write (
      aafUInt32 bytes,
      aafDataBuffer_t  buffer,
      aafUInt32 * bytesWritten)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::Write()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == buffer || NULL == bytesWritten)
    return E_INVALIDARG;
  if (0 > bytes)
    return E_INVALIDARG;
  if (openRead == _streamMode)
    return AAFRESULT_NOT_WRITEABLE;

  //
  // Synchronize with reading if necessary.
  FileStreamOp lastStreamOp = SetStreamOp(opWrite);
  if (opRead == lastStreamOp)
  {
    errno = 0;
    int result = 0;
    if (openAppend == _streamMode)
      result = fseek(_pFile, 0, SEEK_END);
    else
      result = fseek(_pFile, 0, SEEK_CUR);
    if (0 != result)
      return AAFRESULT_INTERNAL_ERROR;
  }
  

  // Write the given data to the file at the current file
  // position.
  errno = 0;
  *bytesWritten = fwrite(buffer, 1, bytes, _pFile);
  if (*bytesWritten != (size_t)bytes)
  { // What error code should we return?
    return AAFRESULT_CONTAINERWRITE;
  }

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::Read()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == buffer)
    return E_INVALIDARG;
  if (NULL == bytesRead)
    return E_INVALIDARG;

  if (feof(_pFile))
    return AAFRESULT_EOF;


  //
  // Synchronize with writing if necessary.
  FileStreamOp lastStreamOp = SetStreamOp(opRead);
  if (opWrite == lastStreamOp)
  {
    errno = 0;
    int result = fflush(_pFile); 
    if (0 != result)
      return AAFRESULT_INTERNAL_ERROR;
  }



  // Write the given data to the file at the current file
  // position.
  errno = 0;
  *bytesRead = fread(buffer, 1, buflen, _pFile);
  if (ferror(_pFile))
  { // What error code should we return?
    return AAFRESULT_END_OF_DATA;
  }
  else if (feof(_pFile))
    return AAFRESULT_EOF; 

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::Seek (aafPosition_t  byteOffset)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::Seek()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;

  off_t pos;
  if (!AafPos2XopenOff(&pos, &byteOffset))
    return E_INVALIDARG;

  //
  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSetpos);

  errno = 0;
  if (0 != fseeko(_pFile, pos, SEEK_SET))
  { // What error code should we return?
    if (feof(_pFile))
      return AAFRESULT_BADSAMPLEOFFSET;
    else
      return AAFRESULT_INTERNAL_ERROR;
  }

  return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::SeekRelative (aafInt32  byteOffset)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::SeekRelative()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;


  //
  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSeek);
  
  errno = 0;
  if (0 != fseek(_pFile, byteOffset, SEEK_CUR))
  { // What error code should we return?
    if (feof(_pFile))
      return AAFRESULT_EOF;
    else
      return AAFRESULT_INTERNAL_ERROR;
  }
  
  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::IsPosValid (aafPosition_t  byteOffset,
        aafBool *  isValid)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::IsPosValid()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == isValid)
    return E_INVALIDARG;
  
  *isValid = kAAFFalse;

  if (0 < byteOffset)
  {
    aafLength_t length = 0;
    HRESULT hr = GetLength(&length);
    if (AAFRESULT_SUCCESS != hr)
      return hr;

    if (byteOffset < length)
    {
      *isValid = kAAFTrue;
    }
    else if (byteOffset == length)
    {
      if (_streamMode != openRead)
      { // If we are not in read-only mode then
        // we don't know whether or not the next
        // file operation will be a read or a write
        // so we just return true.
        *isValid = kAAFTrue;
      }
    }
  }

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::GetPosition (aafPosition_t *  position)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::GetPosition()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == position)
    return E_INVALIDARG;

  
  if (ferror(_pFile))
  { // The file's error state is still set. The position is undefined
    // What error code should we return?
    return AAFRESULT_INTERNAL_ERROR;
  }
  else if (feof(_pFile))
  { // We are already at the end of file so return the size of the file.
    return GetLength(position);
  }

  // Attempt to get the current position.
  off_t pos;
  errno = 0;
  if ((pos = ftello(_pFile)) < 0)
  {
    // What error code should we return?
    return AAFRESULT_INTERNAL_ERROR;
  }
  
  // Convert ansi position to aaf position.
  XopenOff2AafPos(position, &pos);

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::GetLength (aafLength_t *  position)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::GetLength()\n");

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == position)
    return E_INVALIDARG;

  if (openRead == _streamMode)
  { // Use our cached value.
    *position = _startingEOF;
  }
  else
  {
    off_t pos;
    if ((pos = ftello(_pFile)) < 0)
      return AAFRESULT_INTERNAL_ERROR;
    // Convert ansi position to aaf position.
    XopenOff2AafPos(position, &pos);
  }
  
  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::FlushCache ()
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::FlushCache()\n");

  if (NULL != _pFile)
  {
    errno = 0;

    //
    // Save operation so that we can synchronize reading and writing.
    SetStreamOp(opFlush);
  
    fflush(_pFile);
  }

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceRIFFWAVEStream::SetCacheSize (aafUInt32  itsSize)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::SetCacheSize()\n");

  // PRE-CONDITION
  // Ansi states that setvbuf should be called before the first read or write
  // to the stream.

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  // Validate the ansi limits for io buffer size..
  if (2 < itsSize && 32768 > itsSize)
    return E_INVALIDARG;
  
  // Round down the buffer size.
  size_t bufferSize = (size_t)((itsSize/ 2) * 2);
  
  // Attempt to set the buffer size for this file stream.
  errno = 0;
  if(0 != setvbuf(_pFile, NULL, _IOFBF, bufferSize))
  { // Default buffer has not been changed.
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  return AAFRESULT_SUCCESS;
}




//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFEssenceRIFFWAVEStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::InternalQueryInterface()\n");

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IAAFEssenceStream interface
    if (EQUAL_UID(riid,IID_IAAFEssenceStream)) 
    { 
        *ppvObj = (IAAFEssenceStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (EQUAL_UID(riid,IID_IAAFPlugin)) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Factory method called by the container to create an uninitialized instance
// of a file stream in memory.
//
CAAFEssenceRIFFWAVEStream *CAAFEssenceRIFFWAVEStream::CreateFileStream(CAAFEssenceRIFFWAVEContainer *pContainer)
{
	plugin_trace("CAAFEssenceRIFFWAVEStream::CreateFileStream()\n");

  assert(NULL != pContainer);
  CAAFEssenceRIFFWAVEStream *pEssenceFileStream = NULL;

  pEssenceFileStream = new CAAFEssenceRIFFWAVEStream(pContainer, pContainer->LastFileStream());
 	if (NULL != pEssenceFileStream)
  {
    pContainer->SetLastFileStream(pEssenceFileStream);
    pEssenceFileStream->AddRef();
  }

  return pEssenceFileStream;
}
