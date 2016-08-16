//=---------------------------------------------------------------------=
//
// $Id: Example.cpp,v 1.4 2009/06/01 11:46:54 stuart_hc Exp $ $Name: V116 $
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
//=---------------------------------------------------------------------=

#if 0

#include <axFileGen.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
OPNAME,                          /* implementation class name */
  L"OPNAME",                     /* registered operation name */
  L"",                           /* short description */ 
  L"file_name",                  /* usage - document arguments */
  L"Referenced by file_name.",   /* additional notes */ 
  1,                             /* min arg count, smallest value is 1 */
  2 );                           /* max arg count, -1 = unlimited */

OPNAME::~OPNAME()
{}

void OPNAME::Execute( const std::vector<AxString>& args )
{
  /* args[0] is L"OPNAME" */
  /* other args specified by use - adjust the max arg count */

  /* If one of your arguments is a reference to another object,
     access it as follows.  This example get an IAAFFile. */

  /* Get the AxFGOP instance. */
  AxFGOp& file = GetInstance( args[1] );  
  
  /* Get it's COM pointer, automatically casting to IAAFFile. */
  IAAFFileSP spIaafFile;
  file.GetCOM( spIaafFile );
  
  /* Here use use the wrappers to close it... you do what ever you
     want. */
  AxFile axFile( spIaafFile );
  axFile.Close();
  
  /* Other operations can reference the result of this object by name.
     The name is anything you choose.  The logical choice, of course,
     is a name supplied as an argument.  */
  RegisterInstance( args[1] );

  /* Most objects the reference this one will simply ask it for the
     COM object it has storred. */
  IUnknown* comPointer = CreateAComObject()
  SetCOM( comPointer );
}

} // end of namespace


#endif
