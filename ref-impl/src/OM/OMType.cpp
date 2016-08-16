//=---------------------------------------------------------------------=
//
// $Id: OMType.cpp,v 1.18 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMType

#include "OMType.h"

#include "OMAssertions.h"

#include <string.h>

  // @mfunc Reorder (swap) the integer described by <p bytes> and
  //        <p bytesSize>. This static function is provided to aid
  //        Object Manager clients in providing suitable
  //        implementations of the virtual functions in this class.
  //        In particular, Object Manager clients may wish to use
  //        this function when implementing <mf OMType::reorder>.
  //   @parm The buffer containing the bytes to reorder.
  //   @parm The size of the buffer.
void OMType::reorderInteger(OMByte* bytes,
                            OMUInt32 bytesSize)
{
  TRACE("OMType::reorderInteger");
  PRECONDITION("Valid size", (bytesSize == 2) ||
                             (bytesSize == 4) ||
                             (bytesSize == 8));


  OMUInt32 bytesToSwap = bytesSize / 2;
  for (OMUInt32 i = 0; i < bytesToSwap; i++) {
    OMByte temp = bytes[i];
    bytes[i] = bytes[bytesSize - i - 1];
    bytes[bytesSize - i - 1] = temp;
  }

}

  // @mfunc Expand, by padding with leading zeros, the value decribed
  //        by <p inputBytes> and <p inputBytesSize> into the buffer
  //        described by <p outputBytes> and <p outputBytesSize>. The
  //        value is in the <p byteOrder> specified. This static function
  //        is provided to aid Object Manager clients in providing
  //        suitable implementations of the virtual functions in this
  //        class. In particular, Object Manager clients may wish to use
  //        this function when implementing <mf OMType::internalize> and
  //        <mf OMType::externalize>.
  //   @parm The buffer containing the input bytes.
  //   @parm The size of the input buffer.
  //   @parm The buffer to receive the output bytes.
  //   @parm The size of the output buffer.
  //   @parm The byte order.
void OMType::expand(const OMByte* inputBytes,
                    OMUInt32 inputBytesSize,
                    OMByte* outputBytes,
                    OMUInt32 outputBytesSize,
                    OMByteOrder byteOrder)
{
  TRACE("OMType::expand");
  PRECONDITION("Valid input bytes", inputBytes != 0);
  PRECONDITION("Valid output bytes", outputBytes != 0);
  PRECONDITION("Valid output size", outputBytesSize > 0);
  PRECONDITION("Valid sizes", outputBytesSize > inputBytesSize);

  const OMByte* ip = inputBytes;
  OMByte* op = outputBytes;
  OMUInt32 padSize = outputBytesSize - inputBytesSize;

  if (byteOrder == littleEndian) {
    // copy valid bytes
    memcpy(op, ip, inputBytesSize);
    op = op + inputBytesSize;
    // zero fill pad bytes (assumes sign extension not wanted)
    memset(op, 0, padSize);
  } else { // byteOrder == bigEndian
    // zero fill pad bytes (assumes sign extension not wanted)
    memset(op, 0, padSize);
    op = op + padSize;
    // copy valid bytes
    memcpy(op, ip, inputBytesSize);
  }
}

  // @mfunc Contract, by discarding leading bytes, the value decribed
  //        by <p inputBytes> and <p inputBytesSize> into the buffer
  //        described by <p outputBytes> and <p outputBytesSize>. The
  //        value is in the <p byteOrder> specified. This static function
  //        is provided to aid Object Manager clients in providing
  //        suitable implementations of the virtual functions in this
  //        class. In particular, Object Manager clients may wish to use
  //        this function when implementing <mf OMType::internalize> and
  //        <mf OMType::externalize>.
  //   @parm The buffer containing the input bytes.
  //   @parm The size of the input buffer.
  //   @parm The buffer to receive the output bytes.
  //   @parm The size of the output buffer.
  //   @parm The byte order.
void OMType::contract(const OMByte* inputBytes,
                      OMUInt32 inputBytesSize,
                      OMByte* outputBytes,
                      OMUInt32 outputBytesSize,
                      OMByteOrder byteOrder)
{
  TRACE("OMType::contract");
  PRECONDITION("Valid input bytes", inputBytes != 0);
  PRECONDITION("Valid output bytes", outputBytes != 0);
  PRECONDITION("Valid output size", outputBytesSize > 0);
  PRECONDITION("Valid sizes", outputBytesSize < inputBytesSize);

  const OMByte* ip = inputBytes;
  OMByte* op = outputBytes;
  OMUInt32 trimSize = inputBytesSize - outputBytesSize;

  if (byteOrder == littleEndian) {
    // copy valid bytes
    memcpy(op, ip, outputBytesSize);
    // ignore trimmed bytes (nop)
  } else { // byteOrder == bigEndian
    // ignore trimmed bytes
    ip = ip + trimSize;
    // copy valid bytes
    memcpy(op, ip, outputBytesSize);
  }
}

  // @mfunc Copy the value decribed by <p inputBytes> and <p bytesSize>
  //        into the buffer described by <p outputBytes> and
  //        <p bytesSize>. This static function is provided to aid
  //        Object Manager clients in providing suitable implementations
  //        of the virtual functions in this class. In particular,
  //        Object Manager clients may wish to use this function when
  //        implementing <mf OMType::internalize> and
  //        <mf OMType::externalize>.
  //   @parm The buffer containing the input bytes.
  //   @parm The buffer to receive the output bytes.
  //   @parm The size of the input and output buffers.
void OMType::copy(const OMByte* inputBytes,
                  OMByte* outputBytes,
                  OMUInt32 bytesSize)
{
  TRACE("OMType::copy");
  PRECONDITION("Valid input bytes", inputBytes != 0);
  PRECONDITION("Valid output bytes", outputBytes != 0);
  PRECONDITION("Valid buffer size", bytesSize > 0);

  const OMByte* ip = inputBytes;
  OMByte* op = outputBytes;

  memcpy(op, ip, bytesSize);
}
