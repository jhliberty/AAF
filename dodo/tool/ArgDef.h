#ifndef _ArgDef_h_
#define _ArgDef_h_
//=---------------------------------------------------------------------=
//
// $Id: ArgDef.h,v 1.4 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
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


struct ArgDef
{
  //
  // default ctor, dtor, copy ctor, op= are OK
  //

  enum eInitStat
  {
    kInitNoError = 0,	// all OK
    kInitNotTerminated,	// improper termination (no ')' or ',')
    kInitBadEscape		// improper use of '\' character
  };
  //
  // Status for Init()

  eInitStat Init
	(TextStream & text);
  //
  // Initializes this arg using the given source text.
  //
  //********


  TextStream GetText () const;
  //
  // Returns the text of this argument.
  //
  //********


  static void Test ();
  //
  // Unit test.  assert()s if an error is found.
  //
  //********


private:
  TextStream _rep;
};


#endif // ! _ArgDef_h_
