//=---------------------------------------------------------------------=
//
// $Id: OMKLVStoredStream.h,v 1.34 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMKLVSTOREDSTREAM_H
#define OMKLVSTOREDSTREAM_H

#include "OMStoredStream.h"
#include "OMVector.h"

class OMMXFStorage;

  // @class Implementation of <c OMStoredStream> for
  //        SMPTE (Society of Motion Picture and Television Engineers)
  //        Key Length Value (KLV) binary files.
  //   @base public | <c OMStoredStream>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredStream : public OMStoredStream {
public:
  // @access Public members.

    // @cmember Constructor.
  OMKLVStoredStream(OMMXFStorage* store, OMUInt32 sid);

    // @cmember Destructor.
  ~OMKLVStoredStream(void);

    // @cmember Read <p size> bytes from this <c OMKLVStoredStream>
    //          into the buffer at address <p data>.
  virtual void read(void* data, OMUInt32 size) const;

    // @cmember Attempt to read <p bytes> bytes from this
    //          <c OMKLVStoredStream> into the buffer at address <p data>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void read(OMByte* data,
                    const OMUInt32 bytes,
                    OMUInt32& bytesRead) const;

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to this <c OMKLVStoredStream>.
  virtual void write(void* data, OMUInt32 size);

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to this <c OMKLVStoredStream>. The actual
    //          number of bytes written is returned in <p bytesWritten>.
  virtual void write(const OMByte* data,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten);

    // @cmember The size of this <c OMKLVStoredStream> in bytes.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMKLVStoredStream> to <p bytes>.
  virtual void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMKLVStoredStream>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMKLVStoredStream>.
  virtual void setPosition(const OMUInt64 offset) const;

    // @cmember Close this <c OMKLVStoredStream>.
  virtual void close(void);


  // Stream essence element key

    // @cmember Does this <c OMStoredStream> know about essence element keys?
  virtual bool hasEssenceElementKey(void) const;

    // @cmember The essence element key associated with this <c OMStoredStream>.
    //          @precondition <f hasEssenceElementKey()>
  virtual OMKLVKey essenceElementKey(void) const;

    // @cmember Specify the essence element key for this <c OMStoredStream>.
    //          @precondition <f hasEssenceElementKey()>
  virtual void setEssenceElementKey(const OMKLVKey& key);


  // @cmember Filter this <c OMStoredStream>.
  virtual OMStoredStreamFilter* createFilter(void);

  virtual OMUInt32 streamIdentification(void) const;

    // @cmember Set the label to <p label>.
  virtual void setLabel(const OMKLVKey& label);

    // @cmember Get the label.
  virtual OMKLVKey label(void) const;

    // @cmember Set the block size (alignment) of this essence element.
  virtual void setBlockSize(OMUInt32 blockSize);

    // @cmember The block size (alignment) of this essence element.
  virtual OMUInt32 blockSize(void) const;

    // @cmember Set the file offset of this essence element.
  virtual void setFileOffset(OMUInt64 fileOffset);

    // @cmember The file offset of this essence element.
  virtual OMUInt64 fileOffset(void) const;

  static bool readKLVKey(const OMStoredStream& stream, OMKLVKey& key);

  static bool readKLVLength(const OMStoredStream& stream, OMUInt64& length);

  static void writeKLVKey(OMStoredStream& stream, const OMKLVKey& key);

  static void writeKLVLength(OMStoredStream& stream, const OMUInt64& length);

  static OMUInt64 reserveKLVLength(OMStoredStream& stream);

  static void fixupKLVLength(OMStoredStream& stream,
                             const OMUInt64 lengthPosition);

private:
  // @access Private members.
  OMKLVKey _label;
  OMUInt32 _blockSize;
  OMUInt64 _fileOffset;
  OMMXFStorage* _store;
  OMUInt32 _sid;
  OMUInt64 _position;

};

#endif
