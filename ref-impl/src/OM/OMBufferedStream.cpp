//=---------------------------------------------------------------------=
//
// $Id: OMBufferedStream.cpp,v 1.7 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMBufferedStream

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMBufferedStream.h"

#include "OMAssertions.h"

OMBufferedStream::OMBufferedStream(OMUInt32 bufferSize)
: _buffer(0),
  _bufferSize(bufferSize),
  _bufferCount(0)
{
  TRACE("OMBufferedStream::OMBufferedStream");
  _buffer = new OMByte[bufferSize];
  ASSERT("Valid heap pointer", _buffer != 0);

  POSTCONDITION("Valid buffer", _buffer != 0);
  POSTCONDITION("Valid count", _bufferCount <= _bufferSize);
}

OMBufferedStream::~OMBufferedStream(void)
{
  TRACE("OMBufferedStream::~OMBufferedStream");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  PRECONDITION("Empty Buffer", _bufferCount == 0);

  delete [] _buffer;
  _buffer = 0;
}

void OMBufferedStream::readBuffered(OMByte* bytes,
                                    OMUInt32 byteCount,
                                    OMUInt32& bytesRead)
{
  TRACE("OMBufferedStream::readBuffered");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  // tjb - temporary non-buffered implementation
  ASSERT("Empty buffer", _bufferCount == 0);
  OMUInt32 x;
  read(_buffer, byteCount, x);
  _bufferCount = x;
  copyFromBuffer(bytes, byteCount);
  bytesRead = byteCount;
  ASSERT("Empty buffer", _bufferCount == 0);
}

void OMBufferedStream::writeBuffered(const OMByte* bytes,
                                     OMUInt32 byteCount,
                                     OMUInt32& bytesWritten)
{
  TRACE("OMBufferedStream::writeBuffered");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  const OMByte* sourceBytes = bytes;
  OMUInt32 remaining = byteCount;
  OMUInt32 writeSize;
  OMUInt32 freeBytes;
  while (remaining > 0) {
    SAVE(remaining, OMUInt32);
    freeBytes = _bufferSize - _bufferCount;
    if (remaining > freeBytes) {
      writeSize = freeBytes;
    } else {
      writeSize = remaining;
    }
    copyToBuffer(sourceBytes, writeSize);
    sourceBytes = sourceBytes + writeSize;
    remaining = remaining - writeSize;
    if (_bufferCount == _bufferSize) {
      OMUInt32 x;
      write(_buffer, _bufferCount, x);
      _bufferCount = 0;
    }
    ASSERT("Remaining decreased", remaining < OLD(remaining));
  }
  ASSERT("All bytes written", remaining == 0);
  bytesWritten = byteCount;
}

OMUInt32 OMBufferedStream::bufferCount(void)
{
  TRACE("OMBufferedStream::bufferCount");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  return _bufferCount;
}

void OMBufferedStream::emptyBuffer(void)
{
  TRACE("OMBufferedStream::emptyBuffer");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  if (_bufferCount > 0) {
    OMUInt32 bytesWritten;
    write(_buffer, _bufferCount, bytesWritten);
    ASSERT("All bytes written", bytesWritten == _bufferCount);
    _bufferCount = 0;
  }
  POSTCONDITION("Buffer is empty", _bufferCount == 0);
}

void OMBufferedStream::copyToBuffer(const OMByte* bytes, OMUInt32 byteCount)
{
  TRACE("OMBufferedStream::copyToBuffer");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  PRECONDITION("Valid byte count", byteCount <= (_bufferSize - _bufferCount));

  for (OMUInt32 i = 0; i < byteCount; i++) {
    _buffer[_bufferCount + i] = bytes[i];
  }
  _bufferCount = _bufferCount + byteCount;
}

void OMBufferedStream::copyFromBuffer(OMByte* bytes, OMUInt32 byteCount)
{
  TRACE("OMBufferedStream::copyFromBuffer");
  PRECONDITION("Valid buffer", _buffer != 0);
  PRECONDITION("Valid count", _bufferCount <= _bufferSize);

  PRECONDITION("Valid byte count", byteCount <= _bufferSize);

  for (OMUInt32 i = 0; i < byteCount; i++) {
    bytes[i] = _buffer[i];
  }
  _bufferCount = _bufferCount - byteCount;
}

#endif // !OM_NO_STRUCTURED_STORAGE
