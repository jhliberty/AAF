//=---------------------------------------------------------------------=
//
// $Id: writePlugin.cpp,v 1.8 2009/06/01 11:46:52 stuart_hc Exp $ $Name: V116 $
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

#include "AAF.h"

#include "extensionUtils.h"
#include "extensionReadPlugin.h"

#include <stdlib.h>

//
// This example code is intended to show how AAF may be extended to
// store personnel information relating to the staff which worked on a
// project.
//
// As a side benefit, it will also show how to define subclasses of
// existing classes, and add collections of them to AAF files.
//
// Specifically, this is what we'll do:
//
// - The eRole enumeration will be an extensible enumeration describing
//   that person's role in the project:
//
//  extensibleEnum eRole 
//  { 
//    Producer, 
//  	Editor, 
//  	FloorManager, 
//  	...
//  };
//
//
// - Create a new object (called PersonnelResource) which is used to
//   store the information corresponding to one person who's worked on
//   this project.  In pseudo-C++, it will look like this:
//
//   PersonnelResource : subclass of InterchangeObject 
//   { 
//     // mandatory properties 
//     String    name; 
//     eRole     role; 
//     aafUInt32 contractID;
//   };
//
//
// - We will extend Mob to contain a collection of these
//   PersonnelResource objects:
//
//   PersonnelMob : subclass of Mob 
//   { 
//     // mandatory property 
//     StrongRefVector<Person> personnel;
//   };
//

#include "extensionWritePlugin.h"


//
// Define plugin constants.
//
#include "AAFPlugin.h"
#include "AAFPlugin_i.c"

//
// Define Personnel Extension constants.
//
#include "AAFPersonnelExtension.h"
#include "AAFPersonnelExtension_i.c"



int main ()
{
  const aafWChar * filename = L"extensionPlugin.aaf";

  HRESULT status = extensionWritePlugin (filename);
  if (FAILED(status))
	{
	  cout << "Error creating "
		   << filename
		   << "." << endl;

	  exit (EXIT_FAILURE);
	}
  else
	{
    status = extensionReadPlugin (filename);
    if (FAILED(status))
	  {
	    cout << "Error dumping "
		     << filename
		     << "." << endl;

	    exit (EXIT_FAILURE);
	  }
    else
	  {
	    cout << "Succeeded." << endl;
      exit (EXIT_SUCCESS);
	  }
	}


  return EXIT_SUCCESS;
}
