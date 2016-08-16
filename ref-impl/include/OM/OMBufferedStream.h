//=---------------------------------------------------------------------=
//
// $Id: OMBufferedStream.h,v 1.7 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMBUFFEREDSTREAM_H
#define OMBUFFEREDSTREAM_H

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMDataTypes.h"

  // @class Abstract base class which may be used to impose buffering
  //        on an underlying byte stream.
  //
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMBufferedStream {
public:
  // @access Public members.

    // @cmember Constructor.
  OMBufferedStream(OMUInt32 bufferSize);

    // @cmember Destructor.
  virtual ~OMBufferedStream(void);

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMBufferedStream>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          If the bytes to be read are already in the buffer then they
    //          are quickly returned from there, otherwise the buffer is filled
    //          before returning the bytes. If <p bytesRead> is less than
    //          <p byteCount> this is caused by the underlying byte stream
    //          and not by the buffering.
  virtual void readBuffered(OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMBufferedStream>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          If there is space in the buffer then the bytes are inserted
    //          and this function returns quickly, otherwise the buffer is
    //          first emptied before inserting the bytes.
    //          If <p bytesWritten> is less than
    //          <p byteCount> this is caused by the underlying byte stream
    //          and not by the buffering.
  virtual void writeBuffered(const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten);

    // @cmember The number of bytes currently buffered by
    //          this <c OMBufferedStream>.
  virtual OMUInt32 bufferCount(void);

    // @cmember Ensure that any bytes buffered by this <c OMBufferedStream>
    //          are written to the underlying byte stream.
  virtual void emptyBuffer(void);

private:
  // @access Private members.

  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) = 0;

  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten) = 0;

  virtual void copyToBuffer(const OMByte* bytes, OMUInt32 byteCount);

  virtual void copyFromBuffer(OMByte* bytes, OMUInt32 byteCount);

  OMByte* _buffer;
  OMUInt32 _bufferSize;
  OMUInt32 _bufferCount;

};

#endif // !OM_NO_STRUCTURED_STORAGE

#endif // !OMBUFFEREDSTREAM_H
