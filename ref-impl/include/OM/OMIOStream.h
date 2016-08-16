//=---------------------------------------------------------------------=
//
// $Id: OMIOStream.h,v 1.12 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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
#ifndef OMIOSTREAM_H
#define OMIOSTREAM_H

#include "OMDataTypes.h"
#include <stddef.h>

class OMRawStorage;

class OMIOStreamManipulator;

  // @class IOStreams on <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMIOStream {
public:

  OMIOStream(OMRawStorage* rawStorage);

  ~OMIOStream(void);

  void put(char c);

  OMIOStream& operator << (const char* string);

  OMIOStream& operator << (const wchar_t* string);

  OMIOStream& operator << (const OMUInt8 i);

  OMIOStream& operator << (const OMUInt16 i);

  OMIOStream& operator << (const OMUInt32 i);

  OMIOStream& operator << (const OMUInt64 i);

  OMIOStream& operator << (const OMObjectIdentification& id);

  OMIOStream& operator << (const OMMaterialIdentification& id);

  OMIOStream& operator << (OMIOStream& (*manipulator)(OMIOStream&));

  OMIOStream& operator << (const OMIOStreamManipulator& m);

  OMIOStream& beginl(void);

  OMIOStream& endl(void);

  OMIOStream& indent(void);

  OMIOStream& outdent(void);

  OMIOStream& flush(void);

  OMIOStream& showbase(void);

  OMIOStream& dec(void);

  OMIOStream& hex(void);

  OMIOStream& uppercase(void);

  OMIOStream& lowercase(void);

  OMIOStream& setfill(int c);

  OMIOStream& setw(int n);

  OMIOStream& write(const OMByte* bytes, OMUInt32 byteCount);

  OMIOStream& read(OMByte* bytes, OMUInt32 byteCount);

protected:

  void write(const char* string);

private:

  OMRawStorage* _store;
  bool _showBase;
  size_t _base;
  bool _uppercase;
  char _fill;
  int _width;
  const char* _newLine;
  static size_t _level;

};

class OMIOStreamManipulator {
public:
  OMIOStreamManipulator(OMIOStream& (*f)(OMIOStream&, int), int i);

  OMIOStream& (*_f)(OMIOStream&, int);
  int _i;
};

inline OMIOStreamManipulator::OMIOStreamManipulator(
                                            OMIOStream& (*f)(OMIOStream&, int),
                                            int i)
: _f(f),
  _i(i)
{
}

OMIOStream& beginl(OMIOStream& s);

OMIOStream& endl(OMIOStream& s);

OMIOStream& indent(OMIOStream& s);

OMIOStream& outdent(OMIOStream& s);

OMIOStream& flush(OMIOStream& s);

OMIOStream& showbase(OMIOStream& s);

OMIOStream& dec(OMIOStream& s);

OMIOStream& hex(OMIOStream& s);

OMIOStream& uppercase(OMIOStream& s);

OMIOStream& lowercase(OMIOStream& s);

OMIOStream& set_fill(OMIOStream& s, int c);

OMIOStream& set_w(OMIOStream& s, int n);

inline OMIOStreamManipulator setfill(int c)
{
  return OMIOStreamManipulator(set_fill, c);
}

inline OMIOStreamManipulator setw(int n)
{
  return OMIOStreamManipulator(set_w, n);
}

#endif
