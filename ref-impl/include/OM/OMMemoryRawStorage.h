//=---------------------------------------------------------------------=
//
// $Id: OMMemoryRawStorage.h,v 1.25 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMMEMORYRAWSTORAGE_H
#define OMMEMORYRAWSTORAGE_H

#include "OMRawStorage.h"
#include "OMFile.h"
#include "OMVector.h"

  // @class Class supporting access to the raw bytes of memory
  //        files supported by the Object Manager.
  //
  //        This is an Object Manager built-in implementation of the
  //        <c OMRawStorage> interface.
  //
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMMemoryRawStorage : public OMRawStorage {
public:
  // @access Static members.

    // @cmember Create an <c OMMemoryRawStorage> object for modify access.
  static OMMemoryRawStorage* openNewModify(void);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMMemoryRawStorage(void);

    // @cmember Is it possible to read from this <c OMMemoryRawStorage> ?
  virtual bool isReadable(void) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMMemoryRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMemoryRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMMemoryRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMemoryRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Is it possible to write to this <c OMMemoryRawStorage> ?
  virtual bool isWritable(void) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMMemoryRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMemoryRawStorage::size> causes this
    //          <c OMMemoryRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMMemoryRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMemoryRawStorage::size> causes this
    //          <c OMMemoryRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
    //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember May this <c OMMemoryRawStorage> be changed in size ?
  virtual bool isExtendible(void) const;

    // @cmember The current extent of this <c OMMemoryRawStorage> in bytes.
    //          The <f extent()> is the allocated size, while the <f size()>
    //          is the valid size.
    //          precondition - isPositionable()
  virtual OMUInt64 extent(void) const;

    // @cmember Set the size of this <c OMMemoryRawStorage> to <p newSize>
    //          bytes.
    //          If <p newSize> is greater than <mf OMMemoryRawStorage::size>
    //          then this <c OMMemoryRawStorage> is extended. If <p newSize>
    //          is less than <mf OMMemoryRawStorage::size> then this
    //          <c OMMemoryRawStorage> is truncated. Truncation may also result
    //          in the current position for <f read()> and <f write()>
    //          being set to <mf OMMemoryRawStorage::size>.
    //          precondition - isExtendible()
  virtual void extend(OMUInt64 newSize);

    // @cmember The current size of this <c OMMemoryRawStorage> in bytes.
    //          The <f size()> is the valid size, while the <f extent()>
    //          is the allocated size.
    //          precondition - isPositionable()
  virtual OMUInt64 size(void) const;

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMMemoryRawStorage> be changed ?
  virtual bool isPositionable(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMemoryRawStorage>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMemoryRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition) const;

    // @cmember Synchronize this <c OMMemoryRawStorage> with its external
    //          representation.
  virtual void synchronize(void);

private:
  // @access Private members.

    // @cmember Constructor.
  OMMemoryRawStorage(void);

    // @cmember Write a page or partial page.
  virtual void write(OMUInt32 page,
                     OMUInt32 offset,
                     OMUInt32 byteCount,
                     const OMByte* source);

    // @cmember Read a page or partial page.
  virtual void read(OMUInt32 page,
                    OMUInt32 offset,
                    OMUInt32 byteCount,
                    OMByte* destination) const;

  OMVector<OMByte*> _pageVector;
  OMUInt32 _pageSize;
  OMUInt64 _size;
  OMUInt64 _position;
};

#endif
