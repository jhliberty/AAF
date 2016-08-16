//=---------------------------------------------------------------------=
//
// $Id: OMStream.cpp,v 1.18 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStream

#include "OMStream.h"

#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

#include "OMUtilities.h"
#include "OMExceptions.h"
#include "OMRawStorage.h"


  // @mfunc Create an <c OMStream> object by opening an existing
  //        file for read-only access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMStream> object.
OMStream*
OMStream::openExistingRead(const wchar_t* fileName)
{
  TRACE("OMStream::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));

  FILE* file = wfopen(fileName, L"rb");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMStream* result = new OMStream(file, false);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMStream> object by opening an existing
  //        file for modify access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMStream> object.
OMStream*
OMStream::openExistingModify(const wchar_t* fileName)
{
  TRACE("OMStream::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  FILE* file = wfopen(fileName, L"r+b");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMStream* result = new OMStream(file, true);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMStream> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must not already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMStream> object.
OMStream*
OMStream::openNewModify(const wchar_t* fileName)
{
  TRACE("OMStream::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  // Check that the file doesn't already exist.
  // This is the ISO way to do it.
  //
  FILE* f = wfopen(fileName, L"r");
  if (f != 0) {
    fclose(f);
    f = 0;
    throw OMException("File already exists.");
  }

  FILE* file = wfopen(fileName, L"w+b");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMStream* result = new OMStream(file, true);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMStream> object by creating a new
  //        temporary file for modify access.
  //   @rdesc The newly created <c OMStream> object.
OMStream*
OMStream::openNewModify(void)
{
  TRACE("OMStream::openNewModify");

  FILE* file = tmpfile();
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMStream* result = new OMStream(file, true);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

void OMStream::read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const
{
  TRACE("OMStream::read");
  PRECONDITION("No error on stream", ferror(_file) == 0);

  size_t actualByteCount = fread(bytes, 1, byteCount, _file);
  ASSERT("Successful read", ferror(_file) == 0);

  bytesRead = static_cast<OMUInt32>(actualByteCount);
}

void OMStream::write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten)
{
  TRACE("OMStream::write");
  PRECONDITION("Stream is writable", isWritable());
  PRECONDITION("No error on stream", ferror(_file) == 0);

  size_t actualByteCount = fwrite(bytes, 1, byteCount, _file);
  ASSERT("Successful write", ferror(_file) == 0);

  bytesWritten = static_cast<OMUInt32>(actualByteCount);
}

OMUInt64 OMStream::size(void) const
{
  TRACE("OMStream::size");
  PRECONDITION("No error on stream", ferror(_file) == 0);

  // Microsoft Visual C's fstat() is broken for 64bit filesizes
  // so use custom _fstati64() instead.
  // N.B. gcc's fstat() is 64bit safe on Win32.
#if defined(_MSC_VER)
  struct _stati64 fileStat;
#else
  struct stat fileStat;
#endif

  fflush( _file );

#if defined(OM_DEBUG)
  OMInt64 status =
#endif
#if defined(_MSC_VER)
  _fstati64( _fileno( _file ), &fileStat );
#else
  fstat( fileno( _file ), &fileStat );
#endif

  ASSERT( "Successful fstat", status == 0 );
  OMUInt64 result = fileStat.st_size;

  return result;
}

void OMStream::setSize(OMUInt64 newSize)
{
  TRACE("OMStream::setSize");
  PRECONDITION("Stream is writable", isWritable());

  OMUInt64 currentSize = size();

  if (newSize > currentSize) {

    // Extend by writing a single byte.
    //
    OMUInt64 oldPosition = position(); // Save position
    OMByte nullByte = 0;

    setPosition(newSize - 1);
    OMUInt32 bw;
    write(&nullByte, 1, bw);

    ASSERT("Size properly changed", size() == newSize);
    setPosition(oldPosition); // Restore position
  }
  // else no ISO/ANSI way to truncate the file in place
}

OMUInt64 OMStream::position(void) const
{
  TRACE("OMStream::position");
  PRECONDITION("No error on stream", ferror(_file) == 0);

  errno = 0;

#if defined( OM_OS_UNIX )

#if defined(OM_COMPILER_SGICC_MIPS_SGI)
  OMInt64 position = ftell64( _file );
  ASSERT("Successful tell", IMPLIES(position == static_cast<OMInt64>(-1), errno == 0));
#else
	// all POSIX 1003.1 compliant
  off_t position = ftello( _file );
  ASSERT("Successful tell", IMPLIES(position == (off_t)-1, errno == 0));
#endif
	
#elif defined( OM_OS_WINDOWS )

	// we have to rely upon Windows typedef __int64 fpos_t;
	fpos_t position;
	if( fgetpos( _file, &position ) ) {
		ASSERT( "Successful tell", errno==0 );
	}

#else

	// have to be regular ISO
  long int position = ftell(_file);

#endif

  OMUInt64 result = position;
  return result;

}

void OMStream::setPosition(OMUInt64 newPosition)
{
  TRACE("OMStream::setPosition");

  ASSERT("No error on stream", ferror(_file) == 0);
  errno = 0;

#if defined( OM_OS_UNIX )


#if defined(OM_COMPILER_SGICC_MIPS_SGI)
	OMUInt64 position = newPosition;
	int status = fseek64( _file, position, SEEK_SET );

#else
	// all POSIX 1003.1 compliant
	off_t position = newPosition;
#if defined(OM_DEBUG)
	int status =
#endif
	fseeko( _file, position, SEEK_SET);
#endif

	ASSERT("Successful seek", status == 0);
	
#elif defined( OM_OS_WINDOWS )

	// we have to rely upon Windows typedef __int64 fpos_t;
	fpos_t position = newPosition;
	if( fsetpos( _file, &position ) ) {
		ASSERT( "Successful seek", errno==0 );
	}

#else

	// have to be regular ISO
  ASSERT("Supported position", newPosition <= LONG_MAX); // tjb - limit
  long int liNewPosition = static_cast<long int>(newPosition);
  int status = fseek(_file, liNewPosition, SEEK_SET);
  ASSERT("Successful seek", status == 0);

#endif


}

bool OMStream::isWritable(void) const
{
  TRACE("OMStream::isWritable");
  return _isWritable;
}

void OMStream::synchronize(void)
{
  TRACE("OMStream::synchronize");
  PRECONDITION("Stream is writable", isWritable());
  PRECONDITION("No error on stream", ferror(_file) == 0);

#if defined(OM_DEBUG)
  int status =
#endif
  fflush(_file);
  ASSERT("Successful flush", status == 0);
}

OMStream::~OMStream(void)
{
  TRACE("OMStream::~OMStream");

  if (isWritable()) {
    synchronize();
  }
  ASSERT("No error on stream", ferror(_file) == 0);
#if defined(OM_DEBUG)
  int status =
#endif
  fclose(_file);
  ASSERT("Successful close", status == 0);
  _file = 0;
}

OMStream::OMStream(FILE* file, bool isWritable)
: _file(file),
  _isWritable(isWritable)
{
  TRACE("OMStream::OMStream");
}
