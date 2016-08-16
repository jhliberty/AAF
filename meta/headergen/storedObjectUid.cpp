//=---------------------------------------------------------------------=
//
// $Id: storedObjectUid.cpp,v 1.19 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

// Author : Tim Bingham (Tim_Bingham@avid.com) - after an initial
// implementation by Bob Tillman.

#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "headerGenUtils.h"

const char* prefix = "AUID_AAF";

#define AAF_ALIAS_TABLE_BEGIN() {
#define AAF_CLASS_ALIAS(name, alias) {#name, #alias}
#define AAF_ALIAS_SEPARATOR() ,
#define AAF_ALIAS_TABLE_END()   };

struct aliasesTag {
  const char* name;
  const char* alias;
} aliases [] =
#include "AAFMetaDictionary.h"

#define AAF_TABLE_BEGIN() {
#define AAF_CLASS(name, id, parent, concrete) {#name, id}
#define AAF_CLASS_SEPARATOR() ,
#define AAF_TABLE_END()   };

struct classesTag {
  const char* name;
  uid identifier;
} classes [] =
#include "AAFMetaDictionary.h"

static void doFile (const char * moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);
  cout << endl;
  cout << "#include \"AAFTypes.h\"" << endl;
  cout << endl;

  cout << "// AAF stored object UIDs." << endl
       << "//" << endl << endl;

  cout << "#if !defined(INIT_AUID)" << endl;
  cout << "#define ";
  cout << "DEFINE_AUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \\"
       << endl;
  cout << "  extern \"C\" const aafUID_t name"
       << endl;
  cout << "#else" << endl;
  cout << "#define ";
  cout << "DEFINE_AUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \\"
       << endl;
  cout << "  extern \"C\" const aafUID_t name = \\"
       << endl;
  cout << "    { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }"
       << endl;
  cout << "#endif" << endl;
  cout << endl;

  cout << "// The AAF reference implementation uses shorter names than" << endl
       << "// SMPTE. The names are shortened by the following aliases." << endl
       << "//" << endl;

  size_t maxNameLength = 0;
  size_t i = 0;
  for (i = 0; i < sizeof(aliases)/sizeof(aliases[0]); i++){
    size_t length = strlen(aliases[i].alias);
    if (length > maxNameLength) {
      maxNameLength = length;
    }
  }
  size_t width = maxNameLength;
  for (i = 0; i < sizeof(aliases)/sizeof(aliases[0]); i++){
    cout << "#define " << prefix;
    printName(aliases[i].alias, width, cout);
    cout << " " << prefix;
    printName(aliases[i].name, 0, cout);
    cout << endl;
  }
  cout << endl;

  for (i = 0; i < sizeof(classes)/sizeof(classes[0]); i++){
    printMacroInvocation(prefix,
                         "DEFINE_AUID",
                         classes[i].name,
                         classes[i].identifier,
                         cout);
    cout << endl;
  }

  printEndGuard(moduleName, cout);
}


int main (int argc, char ** argv)
{
  char* moduleName = 0;
  validateArgs(argc, argv, moduleName);
  assert(moduleName);
  doFile(moduleName);
  return 0;
}
