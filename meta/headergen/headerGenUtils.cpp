//=---------------------------------------------------------------------=
//
// $Id: headerGenUtils.cpp,v 1.20 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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
#include <iomanip>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "headerGenUtils.h"

void printBeginGuard(const char* moduleName, ostream& s)
{
  assert (moduleName);
  s << "#ifndef __" << moduleName << "_h__" << endl;
  s << "#define __" << moduleName << "_h__" << endl;
  s << endl;
}

void printEndGuard(const char* moduleName, ostream& s)
{
  assert(moduleName);
  s << endl;
  s << "#endif // ! __" << moduleName << "_h__" << endl;
}

void printCopyright (ostream & s) { printCopyright("Avid Technology", s); }

void printCopyright (const char* originator, ostream & s)
{
  s <<
"//=---------------------------------------------------------------------=\n"
"//\n"
"// This file was GENERATED for the AAF SDK\n"
"//\n"
"// $""Id$ $""Name$\n"
"//\n"
"// The contents of this file are subject to the AAF SDK Public Source\n"
"// License Agreement Version 2.0 (the \"License\"); You may not use this\n"
"// file except in compliance with the License.  The License is available\n"
"// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the\n"
"// Advanced Media Workflow Association, Inc., or its successor.\n"
"//\n"
"// Software distributed under the License is distributed on an \"AS IS\"\n"
"// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See\n"
"// the License for the specific language governing rights and limitations\n"
"// under the License.  Refer to Section 3.3 of the License for proper use\n"
"// of this Exhibit.\n"
"//\n"
"// WARNING:  Please contact the Advanced Media Workflow Association,\n"
"// Inc., for more information about any additional licenses to\n"
"// intellectual property covering the AAF Standard that may be required\n"
"// to create and distribute AAF compliant products.\n"
"// (http://www.amwa.tv/policies).\n"
"//\n"
"// Copyright Notices:\n"
"// The Original Code of this file is Copyright 1998-2009, licensor of the\n"
"// Advanced Media Workflow Association.  All rights reserved.\n"
"//\n"
"// The Initial Developer of the Original Code of this file and the\n"
"// licensor of the Advanced Media Workflow Association is\n" 
"// " << originator << ".\n"
"// All rights reserved.\n"
"//\n"
"//=---------------------------------------------------------------------=\n";
  s << endl;
}


static void usage (const char * progname)
{
  assert (progname);

  cerr << "usage: " << progname << "moduleName [prefix]" << endl;
}


void validateArgs (int argc, char** argv, char*& moduleName)
{
  const char * progname = argv[0];

  if (argc < 2) {
    usage (progname);
    exit (1);
  }

  moduleName = argv[1];
}

void validateArgs(int argc, char** argv, char*& moduleName, char*& prefix)
{
  const char * progname = argv[0];

  if (argc < 2) {
    usage (progname);
    exit (1);
  }

  moduleName = argv[1];

  if( argc < 3 ) prefix = const_cast<char*>("");
  else prefix = argv[2];
}


// print a GUID like this -
//{0x0D010101, 0x0101, 0x0100,
//{0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}}
void print(const uid& id, ostream& s)
{
  s.setf(ios::uppercase);
  s.flags(ios::right);
  s << hex << setfill('0')
    << "{"
    << "0x" << setw(8) << id.Data1 << ", "
    << "0x" << setw(4) << id.Data2 << ", "
    << "0x" << setw(4) << id.Data3 << ", "
    << "{"
    << "0x" << setw(2) << (int)id.Data4[0] << ", "
    << "0x" << setw(2) << (int)id.Data4[1] << ", "
    << "0x" << setw(2) << (int)id.Data4[2] << ", "
    << "0x" << setw(2) << (int)id.Data4[3] << ", "
    << "0x" << setw(2) << (int)id.Data4[4] << ", "
    << "0x" << setw(2) << (int)id.Data4[5] << ", "
    << "0x" << setw(2) << (int)id.Data4[6] << ", "
    << "0x" << setw(2) << (int)id.Data4[7]
    << "}"
    << "}";
}

// print a GUID like this -
//{0101010D-0101-0100-060E-2B3402060101}
void printReg(const uid& id, ostream& s)
{
  s.setf(ios::uppercase);
  s.flags(ios::right);
  s << hex << setfill('0')
    << "{"
    << setw(8) << id.Data1 << "-"
    << setw(4) << id.Data2 << "-"
    << setw(4) << id.Data3 << "-"
    << setw(2) << (int)id.Data4[0] << setw(2) << (int)id.Data4[1] << "-"
    << setw(2) << (int)id.Data4[2]
    << setw(2) << (int)id.Data4[3]
    << setw(2) << (int)id.Data4[4]
    << setw(2) << (int)id.Data4[5]
    << setw(2) << (int)id.Data4[6]
    << setw(2) << (int)id.Data4[7]
    << "}";
}

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     const uid& identifier,
                     ostream& s)
{
  s << "//";
  printReg(identifier, s);
  s << endl;
  s << type;
  s << " ";
  s << prefix;
  printName(name, width, s);
  s << " =" << endl;
  print(identifier, cout);
  s << ";" << endl << endl;
}

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     int identifier,
                     ostream& s)
{
  s << type;
  s << " ";
  s << prefix;
  printName(name, width, s);
  s << " = ";
  s.flags(ios::right);
  s.fill('0');
  s << "0x" << hex;
  s.setf(ios::uppercase);
  s << setw(4) << identifier << ";" << endl;
}

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     const char* suffix,
                     const char* string,
                     ostream& s)
{
  s << type;
  s << " ";
  s << prefix;
  s << name;
  s << suffix;
  s << " = L\"";
  s << string << "\";" << endl;
}

void printAlias(	 const char* type,
					 const char* alias,
                     const char* prefix,
                     const char* name,
                     const char* suffix,
                     ostream& s)
{
  s << type;
  s << " ";
  s << alias;
  s << " = ";
  s << prefix;
  s << name;
  s << suffix;
  s << ";" << endl;
}


// print a macro invocation like this -
// DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
void printMacroInvocation(const char* prefix,
                          const char* macro,
                          const char* name,
                          const uid& identifier,
                          ostream& s)
{
  s << "//";
  printReg(identifier, s);
  s << endl;

  s << macro << "(" << prefix << name << "," << endl;

  s.setf(ios::uppercase);
  s.flags(ios::right);
  s << hex << setfill('0')
    << "0x" << setw(8) << identifier.Data1 << ", "
    << "0x" << setw(4) << identifier.Data2 << ", "
    << "0x" << setw(4) << identifier.Data3 << ", "
    << "0x" << setw(2) << (int)identifier.Data4[0] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[1] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[2] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[3] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[4] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[5] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[6] << ", "
    << "0x" << setw(2) << (int)identifier.Data4[7];

  s << ");" << endl;
}

void printName(const char* name,
               size_t width,
               ostream& s)
{
  s << name;
  size_t actualWidth = strlen(name);
  for (size_t i = actualWidth; i < width; i++) {
    s << " ";
  }
}
