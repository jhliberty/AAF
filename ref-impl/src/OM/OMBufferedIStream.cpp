//=---------------------------------------------------------------------=
//
// $Id: OMBufferedIStream.cpp,v 1.8 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMBufferedIStream

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMMSStructuredStorage.h"

#include "OMBufferedIStream.h"
#include "OMAssertions.h"

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::QueryInterface(REFIID riid, void** ppvObject)
{
  TRACE("OMBufferedIStream::QueryInterface");
  return _stream->QueryInterface(riid, ppvObject);
}

ULONG STDMETHODCALLTYPE
OMBufferedIStream::AddRef(void)
{
  TRACE("OMBufferedIStream::AddRef");
  ++_referenceCount;
  return _referenceCount;
}

ULONG STDMETHODCALLTYPE
OMBufferedIStream::Release(void)
{
  TRACE("OMBufferedIStream::Release");
  ULONG result = --_referenceCount;
  if (_referenceCount == 0) {
    emptyBuffer();
    delete this;
  }
  return result;

}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Read(void *pv,
                        ULONG cb,
                        ULONG *pcbRead)
{
  TRACE("OMBufferedIStream::Read");
  return _stream->Read(pv, cb, pcbRead);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Write(const void *pv,
                         ULONG cb,
                         ULONG *pcbWritten)
{
  TRACE("OMBufferedIStream::Write");
  const OMByte* bytes = reinterpret_cast<const OMByte*>(pv);
  OMUInt32 bytesWritten;
  writeBuffered(bytes, cb, bytesWritten);
  *pcbWritten = bytesWritten;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Seek(LARGE_INTEGER dlibMove,
                        DWORD dwOrigin,
                        ULARGE_INTEGER *plibNewPosition)
{
  TRACE("OMBufferedIStream::Seek");
  ASSERT("Seek to get current position", dwOrigin == STREAM_SEEK_CUR);
  // Get the actual position and adjust to account for unwritten bytes
  ULARGE_INTEGER newPosition;
  HRESULT result = _stream->Seek(dlibMove, dwOrigin, &newPosition);
  OMUInt64 position = toOMUInt64(newPosition);
  position = position + bufferCount();
  *plibNewPosition = fromOMUInt64(position);
  return result;
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::SetSize(ULARGE_INTEGER libNewSize)
{
  TRACE("OMBufferedIStream::SetSize");
  return _stream->SetSize(libNewSize);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::CopyTo(IStream *pstm,
                          ULARGE_INTEGER cb,
                          ULARGE_INTEGER *pcbRead,
                          ULARGE_INTEGER *pcbWritten)
{
  TRACE("OMBufferedIStream::CopyTo");
  return _stream->CopyTo(pstm, cb, pcbRead, pcbWritten);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Commit(DWORD grfCommitFlags)
{
  TRACE("OMBufferedIStream::Commit");
  return _stream->Commit(grfCommitFlags);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Revert(void)
{
  TRACE("OMBufferedIStream::Revert");
  return _stream->Revert();
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::LockRegion(ULARGE_INTEGER libOffset,
                              ULARGE_INTEGER cb,
                              DWORD dwLockType)
{
  TRACE("OMBufferedIStream::LockRegion");
  return _stream->LockRegion(libOffset, cb, dwLockType);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::UnlockRegion(ULARGE_INTEGER libOffset,
                                ULARGE_INTEGER cb,
                                DWORD dwLockType)
{
  TRACE("OMBufferedIStream::UnlockRegion");
  return _stream->UnlockRegion(libOffset, cb, dwLockType);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
  TRACE("OMBufferedIStream::Stat");
  return _stream->Stat(pstatstg, grfStatFlag);
}

HRESULT STDMETHODCALLTYPE
OMBufferedIStream::Clone(IStream **ppstm)
{
  TRACE("OMBufferedIStream::Clone");
  return _stream->Clone(ppstm);
}

OMBufferedIStream::OMBufferedIStream(IStream* stream, OMUInt32 bufferSize)
: OMBufferedStream(bufferSize),
  _stream(stream),
  _referenceCount(1)
{
  TRACE("OMBufferedIStream::OMBufferedIStream");

  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid reference count", _referenceCount == 1);
}

OMBufferedIStream::~OMBufferedIStream(void)
{
  TRACE("OMBufferedIStream::~OMBufferedIStream");

  PRECONDITION("Valid reference count", _referenceCount == 0);

  _stream->Release();
  _stream = 0;
}

void OMBufferedIStream::read(OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesRead)
{
  TRACE("OMBufferedIStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", bytes != 0);
  PRECONDITION("Valid size", byteCount > 0);

#if defined(OM_DEBUG)
  HRESULT status = 
#endif
  _stream->Read(bytes, byteCount, &bytesRead);
  ASSERT("IStream::Read() succeeded", SUCCEEDED(status));
  ASSERT("Successful read", bytesRead == byteCount);
}

void OMBufferedIStream::write(const OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesWritten)
{
  TRACE("OMBufferedIStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", bytes != 0);
  PRECONDITION("Valid size", byteCount > 0);

#if defined(OM_DEBUG)
  HRESULT status = 
#endif
  _stream->Write(bytes, byteCount, &bytesWritten);
  ASSERT("IStream::Write() succeeded", SUCCEEDED(status));
  ASSERT("Successful write", bytesWritten == byteCount);
}

#endif // !OM_NO_STRUCTURED_STORAGE
