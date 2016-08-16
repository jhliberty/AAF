//=---------------------------------------------------------------------=
//
// $Id: ImplAAFStorable.cpp,v 1.7 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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

/*************************************************************************
 * 
 * @module ImplAAFStorable | ImplAAFStorable is an abstract class
 *		  that defines a base class of all persistant objects in
 *      an AAF file. All of its methods are inherited and then implemented
 *      by a subclass.
 *
 * @base public | ImplAAFRoot | OMStorable
 *
 *************************************************************************/



#include "ImplAAFStorable.h"
#include "ImplAAFClassDef.h"
#include "ImplAAFContext.h"
#include "ImplAAFModule.h"
#include "OMAssertions.h"

ImplAAFStorable::ImplAAFStorable ()
{
}


ImplAAFStorable::~ImplAAFStorable ()
{
}


// Associate OMClassDefinition and OMPropertyDefinitions with this object.
void ImplAAFStorable::InitializeOMStorable(ImplAAFClassDef * pClassDef)
{
  ASSERTU (NULL != pClassDef);
  
  // Install the class definition for this storable.
  setDefinition(pClassDef);
  
  // Make sure all of the properties exist and have property definitions.
  InitOMProperties(pClassDef);
}

  
AAFRESULT ImplAAFStorable::GetDefinition(ImplAAFClassDef ** ppClassDef)
{
  if (NULL == ppClassDef)
    return AAFRESULT_NULL_PARAM;
  *ppClassDef = NULL;
  
  OMClassDefinition * classDefinition = const_cast<OMClassDefinition *>(definition());
  ImplAAFClassDef * pClassDef = dynamic_cast<ImplAAFClassDef *>(classDefinition);
  ASSERTU (NULL != pClassDef);
  if (!pClassDef)
    return AAFRESULT_NOT_INITIALIZED;
  
  *ppClassDef = pClassDef;
  pClassDef->AcquireReference();
  return AAFRESULT_SUCCESS;
}

// Call during long operations in order to give time to the calling application
void ImplAAFStorable::Progress(void) const
{
  ImplAAFContext* context = ImplAAFContext::GetInstance();
  ASSERTU(context);
  IAAFProgress* progress = NULL;
  context->GetProgressCallback(&progress);
  if(progress != NULL)
  {
	  try
	  {
		(*progress).ProgressCallback();
	  }
	  catch(...)
	  {
	  }
  }
}
