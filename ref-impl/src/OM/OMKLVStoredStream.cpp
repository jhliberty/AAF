//=---------------------------------------------------------------------=
//
// $Id: OMKLVStoredStream.cpp,v 1.40 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMKLVStoredStream
#include "OMKLVStoredStream.h"

#include "OMAssertions.h"
#include "OMKLVStoredStreamFilter.h"
#include "OMMXFStorage.h"

OMKLVStoredStream::OMKLVStoredStream(OMMXFStorage* store, OMUInt32 sid)
: _label(nullOMKLVKey),
  _blockSize(0),
  _fileOffset(0),
  _store(store),
  _sid(sid),
  _position(0)
{
  TRACE("OMKLVStoredStream::OMKLVStoredStream");
  PRECONDITION("Valid store", _store != 0);
}

OMKLVStoredStream::~OMKLVStoredStream(void)
{
  TRACE("OMKLVStoredStream::~OMKLVStoredStream");
  PRECONDITION("Stream not open", _store == 0);
}

void OMKLVStoredStream::read(void* ANAME(data), OMUInt32 ANAME(size)) const
{
  TRACE("OMKLVStoredStream::read");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMKLVStoredStream::read");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  _store->readStreamAt(_sid, _position, data, bytes, bytesRead);
  OMKLVStoredStream* nonConstThis = const_cast<OMKLVStoredStream*>(this);
  nonConstThis->_position = _position + bytesRead;
}

void OMKLVStoredStream::write(void* ANAME(data), OMUInt32 ANAME(size))
{
  TRACE("OMKLVStoredStream::write");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMKLVStoredStream::write");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  _store->writeStreamAt(_sid,
                        _position,
                        reinterpret_cast<const OMByte*>(data),
                        bytes,
                        bytesWritten);
  _position = _position + bytesWritten;
}

OMUInt64 OMKLVStoredStream::size(void) const
{
  TRACE("OMKLVStoredStream::size");
  PRECONDITION("Valid store", _store != 0);

  OMUInt64 result = _store->streamSize(_sid);
  return result;
}

void OMKLVStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMKLVStoredStream::setSize");
  PRECONDITION("Valid store", _store != 0);

  _store->streamSetSize(_sid, newSize);
}

OMUInt64 OMKLVStoredStream::position(void) const
{
  TRACE("OMKLVStoredStream::position");
  PRECONDITION("Valid store", _store != 0);

  return _position;
}

void OMKLVStoredStream::setPosition(const OMUInt64 offset) const
{
  TRACE("OMKLVStoredStream::setPosition");
  PRECONDITION("Valid store", _store != 0);

  const_cast<OMKLVStoredStream*>(this)->_position = offset;
}

void OMKLVStoredStream::close(void)
{
  TRACE("OMKLVStoredStream::close");
  PRECONDITION("Valid store", _store != 0);

  // We don't own _store
  _store = 0;
}

  // @mfunc Does this <c OMStoredStream> know about essence element keys?
  //   @rdesc True if this supports essence element keys, false otherwise.
  //   @this const
bool OMKLVStoredStream::hasEssenceElementKey(void) const
{
  TRACE("OMKLVStoredStream::hasEssenceElementKey");

  return true;
}

  // @mfunc The essence element key associated with this <c OMStoredStream>.
  //   @rdesc The essence element key.
  //   @this const
OMKLVKey OMKLVStoredStream::essenceElementKey(void) const
{
  TRACE("OMKLVStoredStream::essenceElementKey");
  PRECONDITION("Stream supports essence element keys",
                                                  hasEssenceElementKey());

  return label();
}

  // @mfunc Specify the essence element key for this <c OMStoredStream>.
  //   @parm The essence element key.
void OMKLVStoredStream::setEssenceElementKey(const OMKLVKey& key)
{
  TRACE("OMKLVStoredStream::setEssenceElementKey");
  PRECONDITION("Stream supports essence element keys",
                                                  hasEssenceElementKey());

  setLabel(key);

  POSTCONDITION("Essence element key set", essenceElementKey() == key);
}

OMStoredStreamFilter* OMKLVStoredStream::createFilter(void)
{
  TRACE("OMKLVStoredStream::createFilter");

  OMStoredStreamFilter* result = new OMKLVStoredStreamFilter(this);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

OMUInt32 OMKLVStoredStream::streamIdentification(void) const
{
  TRACE("OMKLVStoredStream::streamIdentification");
  return _sid;
}

  // @mfunc Set the label to <p label>.
  //   @parm The new label.
void OMKLVStoredStream::setLabel(const OMKLVKey& label)
{
  TRACE("OMKLVStoredStream::setLabel");
  _label = label;
}

  // @mfunc Get the label.
  //   @rdesc The label.
OMKLVKey OMKLVStoredStream::label(void) const
{
  TRACE("OMKLVStoredStream::label");
  return _label;
}

  // @mfunc Set the block size (alignment) of this essence element.
  //   @parm The block size.
void OMKLVStoredStream::setBlockSize(OMUInt32 blockSize)
{
  TRACE("OMKLVStoredStream::setBlockSize");
  _blockSize = blockSize;
}

  // @mfunc The block size (alignment) of this essence element.
  //   @rdesc The block size.
OMUInt32 OMKLVStoredStream::blockSize(void) const
{
  TRACE("OMKLVStoredStream::blockSize");
  return _blockSize;
}

  // @mfunc Set the file offset of this essence element.
  //   @parm The file offset.
void OMKLVStoredStream::setFileOffset(OMUInt64 fileOffset)
{
  TRACE("OMKLVStoredStream::setFileOffset");
  _fileOffset = fileOffset;
}

  // @mfunc The file offset of this essence element.
  //   @rdesc The file offset.
OMUInt64 OMKLVStoredStream::fileOffset(void) const
{
  TRACE("OMKLVStoredStream::fileOffset");
  return _fileOffset;
}

bool OMKLVStoredStream::readKLVKey(const OMStoredStream& stream, OMKLVKey& key)
{
  TRACE("OMKLVStoredStream::readKLVKey");

  bool result;
  OMKLVKey k;
  OMUInt32 bytesRead;
  stream.read(reinterpret_cast<OMByte*>(&k), sizeof(OMKLVKey), bytesRead);
  if (bytesRead == sizeof(OMKLVKey)) {
    key = k;
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMKLVStoredStream::readKLVLength(const OMStoredStream& stream,
                                      OMUInt64& length)
{
  TRACE("OMKLVStoredStream::readKLVLength");
  // Bah ! should reuse code in OMMXFStorage - tjb
  bool result = true;
  OMUInt32 x;
  OMUInt8 b;
  stream.read(&b, 1, x);
  if (b == 0x80) {
    // unknown length
    result = false;
  } else if ((b & 0x80) != 0x80) {
    // short form
    length = b;
    result = true;
  } else {
    // long form
    int lenlen = b & 0x7f;
    OMUInt64 len = 0;
    for (int k = 0; k < lenlen; k++) {
      stream.read(&b, 1, x);
      len = len << 8;
      len = len + b;
    }
    length = len;
    result = true;
  }
  return result;
}

void OMKLVStoredStream::writeKLVKey(OMStoredStream& stream,
                                    const OMKLVKey& key)
{
  TRACE("OMKLVStoredStream::writeKLVKey");

  OMUInt32 x;
  const OMByte* src = reinterpret_cast<const OMByte*>(&key);
  stream.write(src, sizeof(OMKLVKey), x);

  POSTCONDITION("All bytes written", x == sizeof(OMKLVKey));
}

void OMKLVStoredStream::writeKLVLength(OMStoredStream& stream,
                                       const OMUInt64& length)
{
  TRACE("OMKLVStoredStream::writeKLVLength");

  OMByte buffer[sizeof(OMUInt64) + 1]; // Max

  OMMXFStorage::berEncode(buffer, sizeof(buffer), sizeof(OMUInt64), length);
  OMUInt32 x;
  stream.write(buffer, sizeof(OMUInt64) + 1, x);

  POSTCONDITION("All bytes written", x == (sizeof(OMUInt64) + 1));
}

OMUInt64 OMKLVStoredStream::reserveKLVLength(OMStoredStream& stream)
{
  TRACE("OMKLVStoredStream::reserveKLVLength");
  // Bah ! should reuse code in OMMXFStorage - tjb
  OMUInt64 lengthPosition = stream.position();
  OMUInt64 length = 0;
  writeKLVLength(stream, length); // must be fixed up later
  return lengthPosition;
}

void OMKLVStoredStream::fixupKLVLength(OMStoredStream& stream,
                                       const OMUInt64 lengthPosition)
{
  TRACE("OMKLVStoredStream::fixupKLVLength");
  // Bah ! should reuse code in OMMXFStorage - tjb
  OMUInt64 endPosition = stream.position();
  ASSERT("Proper position", endPosition >= (lengthPosition + 8 + 1));
  OMUInt64 length = endPosition - (lengthPosition + 8 + 1);
  stream.setPosition(lengthPosition);
  writeKLVLength(stream, length);
  stream.setPosition(endPosition);
}
