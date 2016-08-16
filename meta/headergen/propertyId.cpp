//=---------------------------------------------------------------------=
//
// $Id: propertyId.cpp,v 1.13 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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

#define AAF_TABLE_BEGIN() {
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
{#container "_" #name, tag},
#define AAF_TABLE_END()   };

typedef struct {
  const char* name;
  int identifier;
} Property_t;

Property_t properties[] =
#include "AAFMetaDictionary.h"

static void doFile(const char* moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);
  cout << endl;

  cout << "// AAF property identifiers (PIDs)." << endl
       << "//" << endl << endl;

  cout << "// A property is identified by a globally unique 16-byte" << endl
       << "// identifier. To save space in an AAF file we store a" << endl
       << "// 2-byte file unique PID with each property." << endl
       << "// The mapping for a particular file is recorded in the" << endl
       << "// dictionary contained in that file." << endl
       << "// For the predefined properties we optimize by using a" << endl
       << "// fixed, compiled-in mapping." << endl
       << "// This file defines that mapping." << endl
       << "//" << endl;

  size_t maxNameLength = 0;
  size_t i = 0;
  for (i = 0; i < sizeof(properties)/sizeof(properties[0]); i++) {
    size_t length = strlen(properties[i].name);
    if (length > maxNameLength) {
      maxNameLength = length;
    }
  }
  for (i = 0; i < sizeof(properties)/sizeof(properties[0]); i++) {
    printDefinition("const int",
                    "PID_",
                    properties[i].name,
                    maxNameLength,
                    properties[i].identifier,
                    cout);
  }
  printEndGuard(moduleName, cout);
}


int main(int argc, char** argv)
{
  char* moduleName = 0;
  validateArgs(argc, argv, moduleName);
  assert(moduleName);
  doFile(moduleName);
  return 0;
}
