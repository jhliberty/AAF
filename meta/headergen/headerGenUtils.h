#ifndef __headerGenUtils_h__
#define __headerGenUtils_h__
//=---------------------------------------------------------------------=
//
// $Id: headerGenUtils.h,v 1.16 2009/06/01 11:46:56 stuart_hc Exp $ $Name: V116 $
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


#include "headerGenTypes.h"

#include <iostream>
using namespace std;

void printBeginGuard(const char* module, ostream& s);

void printEndGuard(const char* module, ostream& s);

void printCopyright(ostream& s);
void printCopyright (const char* originator, ostream & s);

// Doesn't return if an error is found.
void validateArgs(int argc, char** argv, char*& moduleName);
void validateArgs(int argc, char** argv, char*& moduleName, char*& prefix);

// print a GUID like this -
//{0x0D010101, 0x0101, 0x0100,
//{0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}}
void print(const uid& id, ostream& s);

// print a GUID like this -
//{0101010D-0101-0100-060E-2B3402060101}
void printReg(const uid& id, ostream& s);

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     const uid& identifier,
                     ostream& s);

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     int identifier,
                     ostream& s);

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     const char* suffix,
                     const char* string,
                     ostream& s);

void printAlias(	 const char* type,
					 const char* alias,
                     const char* prefix,
                     const char* name,
                     const char* suffix,
                     ostream& s);

// print a macro invocation like this -
// DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
void printMacroInvocation(const char* prefix,
                          const char* macro,
                          const char* name,
                          const uid& identifier,
                          ostream& s);

void printName(const char* name,
               size_t width,
               ostream& s);

#endif // ! __headerGenUtils_h__
