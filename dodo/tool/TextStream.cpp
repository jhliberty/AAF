//=---------------------------------------------------------------------=
//
// $Id: TextStream.cpp,v 1.10 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
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

#ifndef _TextStream_h_
#include "TextStream.h"
#endif


#include <string.h>


TextStream::TextStream
(const char * src)
  : _pStorage (0),
	_numAllocated (0),
	_pData (0),
	_cachedLen (0)
{
  if (src && *src)
	{
	  _cachedLen = strlen (src);
	  _numAllocated = _cachedLen + 1;
	  assert (_numAllocated > 0);
	  _pStorage = new char [_numAllocated];
	  assert (_pStorage);
	  _pData = _pStorage;
	  strcpy (_pData, src);
	}
}


TextStream::~TextStream ()
{
  delete[] _pStorage;
}


TextStream::TextStream
(const TextStream & src)
  : _pStorage (0),
	_numAllocated (0),
	_pData (0),
	_cachedLen (src._cachedLen),
	_startSi (src._startSi)
{
  if (_cachedLen)
	{
	  _numAllocated = _cachedLen+1;
	  assert (_numAllocated > 0);
	  _pStorage = new char[_numAllocated];
	  assert (_pStorage);
	  _pData = _pStorage;
	  strcpy (_pData, src._pData);
	}
}


//
// Utility method to reduce size of memory allocated.
//
const static int kCompressLimit = 1000;
//
void TextStream::compress ()
{
  if (! _pData || ! _pStorage)
    return;

  if ((_pData - _pStorage) < kCompressLimit)
    // not enough to bother
    return;

#ifdef DEBUG
  fprintf (stderr, "Diff is %d; compressing.\n", (int)(_pData - _pStorage));
#endif
  char * newStorage = new char[strlen(_pData) + 1];
  assert (newStorage);
  strcpy (newStorage, _pData);
  _pData = newStorage;
  delete[] _pStorage;
  _pStorage = newStorage;
}


TextStream TextStream::operator=
(const TextStream & src)
{
  if (&src != this)
	{
	  delete[] _pStorage;
	  _pStorage = 0;
	  _pData = 0;
	  _numAllocated = 0;
	  _cachedLen = src._cachedLen;
	  _startSi = src._startSi;
	  if (src._cachedLen)
		{
		  _numAllocated = _cachedLen + 1;
		  assert (_numAllocated > 0);
		  _pStorage = new char[_numAllocated];
		  assert (_pStorage);
		  _pData = _pStorage;
		  strcpy (_pData, src._pData);
		}
	}
  return *this;
}


void TextStream::Clear ()
{
  _numAllocated = 0;
  _cachedLen = 0;
  delete[] _pStorage;
  _pStorage = 0;
  _pData = 0;
  SourceInfo blank;
  _startSi = blank;
}


size_t TextStream::GetLength () const
{
  //  if (_pData)
  //	{
  //	  assert (strlen (_pData) == _cachedLen);
  //	}
  //  else
  //	{
  //	  assert (0 == _cachedLen);
  //	}
  return _cachedLen;
}


void TextStream::Append
(char c)
{
  // If the following assertion triggers we are probably running on Macintosh and
  // '\r' is not being mapped to '\n' on input. Some ways that this can happen are
  //
  // 1) the file was opened in binary mode instead of text mode, or
  // 2) the program was linked to a library that doesn't perform
  //    the mapping even for files opened in text mode, or
  // 3) the CodeWarrior C++ Language setting "Map newlines to CR" is
  //    not checked (this enables '\n' -> '\r' on output and
  //    maps '\r' -> '\n' on input).
  //
  // If this assertion would have failed but is disabled '#c' will incorrectly
  // appear in the output - tjb 03/19/02.
  //
  assert(c != '\r');
  if (! _numAllocated)
	{
	  // If we don't have any allocated, allocate it!

	  assert (!_pStorage);
	  assert (!_cachedLen);
	  _numAllocated = 10;
	  _pStorage = new char [_numAllocated];
	  assert (_pStorage);
	  _pData = _pStorage;
	  _pData[0] = '\0';
	}

  assert (_numAllocated > 0);

  if ((_pData + _cachedLen + 1) >= (_pStorage + _numAllocated))
	{
	  // see if we need to allocate more.  If so, double current
	  // amount.

	  char * newRep;
	  _numAllocated *= 2;

	  newRep = new char[_numAllocated];
	  assert (newRep);
	  strcpy (newRep, _pData);
	  delete[] _pStorage;
	  _pStorage = newRep;
	  _pData = _pStorage;
	}

  _pData[_cachedLen] = c;
  _cachedLen++;
  _pData[_cachedLen] = '\0';
}


void TextStream::Append
(const TextStream & txt)
{
  TextStream tmp = txt;
  char c;

  while (tmp.Consume (c))
	{
	  assert (c);
	  Append (c);
	}
}


void TextStream::Append
(FILE * fp,
 const SourceInfo & si)
{
  int c;
  c = getc (fp);

  if (! GetLength())
	{
	  _startSi = si;
	}

  while (EOF != c)
	{
	  Append (c);
	  c = getc (fp);
	}
}


bool TextStream::Consume
(char & c)
{
  c = '\0';
  if (! _cachedLen)
	{
	  return false;
	}
  c = *_pData++;
  assert (c);
  _cachedLen--;
  assert (_cachedLen >= 0);
  if ('\n' == c)
	{
	  _startSi = SourceInfo (_startSi.GetFileName(), _startSi.GetLineNumber() + 1);
	}
  compress ();
  return true;
}


bool TextStream::Expect
(const TextStream & key)
{
  if (! Snoop (key))
	{
	  return false;
	}

  _pData += key.GetLength();
  _cachedLen -= key.GetLength();
  assert (_cachedLen >= 0);
  compress ();
  return true;
}


bool TextStream::Snoop
(const TextStream & key) const
{
  if (key.GetLength() > GetLength())
	{
	  return false;
	}

  if (0 == key.GetLength())
	{
	  return false;
	}

  if (0 == GetLength())
	{
	  return false;
	}

  assert (_pData);
  assert (key._pData);

  if (0 == strncmp (_pData, key._pData, key.GetLength()))
	{
	  return true;
	}
  else
	{
	  return false;
	}
}


SourceInfo TextStream::GetCurSourceInfo () const
{
  return _startSi;
}


void TextStream::SetCurSourceInfo (const SourceInfo & si)
{
  _startSi = si;
}


const char * TextStream::GetCString () const
{
  if (_pData && *_pData)
	{
	  return _pData;
	}
  else
	{
	  return 0;
	}
}


void TextStream::dump (FILE* fp) const
{
  TextStream tmp = *this;
  assert (fp);
  char c;
  while (tmp.Consume (c))
	{
	  fputc (c, fp);
	}
}
