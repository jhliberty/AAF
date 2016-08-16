//=---------------------------------------------------------------------=
//
// $Id: ArgDefTest.cpp,v 1.4 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
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

#include "ArgDef.h"
#include "ArgDef.h"
#ifndef _ArgDef_h_
#error - improper include guard
#endif

#include <assert.h>
#include <string.h>


void ArgDef::Test ()
{
  //
  // Init(), GetText()
  //
  ArgDef ad;
  TextStream ts;
  TextStream input;
  eInitStat stat;
  assert (0 == ad._rep.GetLength());

  input = "something";
  stat = ad.Init (input);
  assert (kInitNotTerminated == stat);

  input = "proper)";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("proper"));
  assert (0 == ts.GetLength());
  assert (input.Expect (")"));
  assert (0 == input.GetLength());

  input = "(paren in arg)";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("(paren in arg"));
  assert (0 == ts.GetLength());
  assert (input.Expect (")"));
  assert (0 == input.GetLength());

  input = "middle arg,yup)";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("middle arg"));
  assert (0 == ts.GetLength());
  assert (input.Expect ("yup)"));
  assert (0 == input.GetLength());

  input = "comma in\\, arg, here";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("comma in, arg"));
  assert (0 == ts.GetLength());
  assert (input.Expect (" here"));
  assert (0 == input.GetLength());

  input = "  \t\t\n\t  White\tspace in here.,";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("White\tspace in here."));
  assert (0 == ts.GetLength());
  assert (0 == input.GetLength());

  input = "bad escape\\";
  stat = ad.Init (input);
  assert (kInitBadEscape == stat);
}
