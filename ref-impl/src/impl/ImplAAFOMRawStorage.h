#ifndef __ImplAAFOMRawStorage_h__
#define __ImplAAFOMRawStorage_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFOMRawStorage.h,v 1.6 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

#include "OMRawStorage.h"
#include "OMCachedRawStorage.h"
#include "OMCachedDiskRawStorage.h"

struct IAAFRawStorage;
struct IAAFRandomRawStorage;
struct IAAFStreamRawStorage;
struct IAAFAsyncStreamRawStorage;

struct IAAFRawStorage;
struct IAAFRandomRawStorage;

class ImplAAFOMRawStorage : public OMRawStorage
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFOMRawStorage (IAAFRawStorage * rep);

  virtual ~ImplAAFOMRawStorage ();

public:

  // Implementations of OMRawStorage pure virtual methods.

  // @cmember Is it possible to read from this <c OMRawStorage> ?
  virtual bool isReadable(void) const;

  // @cmember Attempt to read the number of bytes given by <p byteCount>
  //          from the current position in this <c OMRawStorage>
  //          into the buffer at address <p bytes>.
  //          The actual number of bytes read is returned in <p bytesRead>.
  //          Reading from positions greater than
  //          <mf OMRawStorage::size> causes <p bytesRead> to be less
  //          than <p byteCount>. Reading bytes that have never been written
  //          returns undefined data in <p bytes>.
  //          @precondition <f isReadable()>
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

  // @cmember Attempt to read the number of bytes given by <p byteCount>
  //          from offset <p position> in this <c OMRawStorage>
  //          into the buffer at address <p bytes>.
  //          The actual number of bytes read is returned in <p bytesRead>.
  //          Reading from positions greater than
  //          <mf OMRawStorage::size> causes <p bytesRead> to be less
  //          than <p byteCount>. Reading bytes that have never been written
  //          returns undefined data in <p bytes>.
  //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

  // @cmember Is it possible to write to this <c OMRawStorage> ?
  virtual bool isWritable(void) const;

  // @cmember Attempt to write the number of bytes given by <p byteCount>
  //          to the current position in this <c OMRawStorage>
  //          from the buffer at address <p bytes>.
  //          The actual number of bytes written is returned in
  //          <p bytesWritten>.
  //          Writing to positions greater than
  //          <mf OMRawStorage::size> causes this <c OMRawStorage>
  //          to be extended, however such extension can fail, causing
  //          <p bytesWritten> to be less than <p byteCount>.
  //          @precondition <f isWritable()>
  //   @devnote How is failure to extend indicated ?
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

  // @cmember Attempt to write the number of bytes given by <p byteCount>
  //          to offset <p position> in this <c OMRawStorage>
  //          from the buffer at address <p bytes>.
  //          The actual number of bytes written is returned in
  //          <p bytesWritten>.
  //          Writing to positions greater than
  //          <mf OMRawStorage::size> causes this <c OMRawStorage>
  //          to be extended, however such extension can fail, causing
  //          <p bytesWritten> to be less than <p byteCount>.
  //          @precondition <f isWritable()> && <f isPositionable()>
  //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

  // @cmember May this <c OMRawStorage> be changed in size ?
  //          An implementation of <c OMRawStorage> for disk files
  //          would most probably return true. An implemetation
  //          for network streams would return false. An implementation
  //          for fixed size contiguous memory files (avoiding copying)
  //          would return false.
  virtual bool isExtendible(void) const;

    // @cmember The current extent of this <c OMRawStorage> in bytes.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 extent(void) const;

  // @cmember Set the size of this <c OMRawStorage> to <p newSize> bytes.
  //          If <p newSize> is greater than <mf OMRawStorage::size>
  //          then this <c OMRawStorage> is extended. If <p newSize>
  //          is less than <mf OMRawStorage::size> then this
  //          <c OMRawStorage> is truncated. Truncation may also result
  //          in the current position for <f read()> and <f write()>
  //          being set to <mf OMRawStorage::size>.
  //          @precondition <f isExtendible()>
  //   @devnote How is failure to extend indicated ?
  virtual void extend(OMUInt64 newSize);

  // @cmember The current size of this <c OMRawStorage> in bytes.
  //          @precondition <f isPositionable()>
  virtual OMUInt64 size(void) const;

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMRawStorage> be changed ?
    //          An implementation of <c OMRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for memory files would return true.
  virtual bool isPositionable(void) const;

  // @cmember The current position for <f read()> and <f write()>, as an
  //          offset in bytes from the beginning of this
  //          <c ImplAAFOMRawStorage>.
  virtual OMUInt64 position(void) const;

  // @cmember Set the current position for <f read()> and <f write()>, as an
  //          offset in bytes from the beginning of this
  //          <c ImplAAFOMRawStorage>.
  //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition) const;

  // @cmember Synchronize this <c OMRawStorage> with its external
  //          representation.
  //          An implementation of <c OMRawStorage> for disk files would
  //          most probably implement this virtual function as a flush.
  //          This virtual function would probably be implemented as a
  //          noop in implemetations for network streams and for memory
  //          files.
  virtual void synchronize(void);


private:
  IAAFRawStorage * _rep;
  IAAFRandomRawStorage * _randRep;
  OMUInt64 _position;
};

class OMCachePageAllocator;

class ImplAAFOMCachedRawStorage : public OMBaseCachedDiskRawStorage
{
public:

  ImplAAFOMCachedRawStorage(IAAFRawStorage* rep,
                            aafUInt32 pageCount,
                            aafUInt32 pageSize,
                            OMCachePageAllocator* allocator);

  virtual ~ImplAAFOMCachedRawStorage();

  // OMRawStorage overrides

  virtual bool isReadable(void) const;

  virtual bool isWritable(void) const;

  virtual bool isExtendible(void) const;

  virtual bool isPositionable(void) const;

  virtual void synchronize(void);

private:

  // OMPageCage overrides

  virtual void rawReadAt(OMUInt64 position,
                         OMUInt32 byteCount,
                         OMByte* destination);

  virtual void rawWriteAt(OMUInt64 position,
                          OMUInt32 byteCount,
                          const OMByte* source);

  // @cmember Return the size of the specified raw storage. If the
  //          raw storage is not IAAFRandomRawStorage the returned
  //          size is always zero.
  //          This utility method is used in the class constructor.
  static aafUInt64 getRawStorageSize( IAAFRawStorage* );


private:

  IAAFRawStorage* _rep;
  IAAFRandomRawStorage* _randRep;
};

#endif // ! __ImplAAFOMRawStorage_h__
