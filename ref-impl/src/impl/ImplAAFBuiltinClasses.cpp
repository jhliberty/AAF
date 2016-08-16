//=---------------------------------------------------------------------=
//
// $Id: ImplAAFBuiltinClasses.cpp,v 1.47 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFBuiltinClasses_h__
#include "ImplAAFBuiltinClasses.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __AAFClassDefUIDs_h__
#include "AAFClassDefUIDs.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif
#ifndef __AAFPropertyIDs_h__
#include "AAFPropertyIDs.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFUtils.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFBuiltinDefs.h"
#include "AAFObjectModel.h"

#include "OMAssertions.h"








ImplAAFBuiltinClasses::ImplAAFBuiltinClasses (ImplAAFDictionary* dictionary)
  : _dictionary (dictionary)
{
  dictionary->SetBuiltinClasses(this);
}


ImplAAFBuiltinClasses::~ImplAAFBuiltinClasses ()
{
}


AAFRESULT
ImplAAFBuiltinClasses::NewBuiltinClassDef (const aafUID_t & rClassID,
										   ImplAAFClassDef ** ppResult)
{
  AAFRESULT status = AAFRESULT_NO_MORE_OBJECTS;

  ImplAAFUID popped;
	
	

  ASSERTU (! _createStack.isPresent (rClassID));

  _createStack.push (rClassID);
  
  // Find the builtin class definition template.
	const AAFObjectModel * objectModel = AAFObjectModel::singleton();
	const ClassDefinition * classDefinition = objectModel->findClassDefinition(&rClassID);
	if (classDefinition && !classDefinition->isNil())
	{
		ASSERTU(!classDefinition->axiomatic()); // axiomatic classes should have already been created.
		
	  // We've found the desired class in our table.
	  ImplAAFClassDef * pcd = (ImplAAFClassDef*)(_dictionary->metaDictionary())->pvtCreateMetaDefinition(AUID_AAFClassDef);
	  ASSERTU (pcd);
    pcd->InitializeOMStorable(_dictionary->GetBuiltinDefs()->cdClassDef());

	  status = InitBuiltinClassDef (rClassID, classDefinition, pcd);
	  if (AAFRESULT_SUCCEEDED (status))
		{
		  ASSERTU (ppResult);
		  *ppResult = pcd;
		}
	}

  popped = _createStack.pop ();
  ASSERTU (popped == rClassID);
  return status;
}



AAFRESULT
ImplAAFBuiltinClasses::InitBuiltinClassDef (const aafUID_t & rClassID,
											const ClassDefinition * classDefinition, 
											ImplAAFClassDef * pClass)
{
  ASSERTU (classDefinition && !classDefinition->isNil());
	ASSERTU(!classDefinition->axiomatic()); // axiomatic classes should have already been created.
	ASSERTU (pClass);
	AAFRESULT status = AAFRESULT_NO_MORE_OBJECTS;
	ImplAAFUID popped;
	ImplAAFClassDefSP parent;
	
	ASSERTU (! _initStack.isPresent (rClassID));
	
	_initStack.push (rClassID);
	
  
	if (!classDefinition->isRoot())
	{
		// JeffB: The LookupClassDef() call on the parent may make a recursive
		// call to InitBuiltinClassDef for the parent class.  Since we have a moderately
		// shallow tree with no loops, this shouldn't be a problem.
		status = _dictionary->LookupClassDef(*classDefinition->parentId(), &parent);
		ASSERTU (AAFRESULT_SUCCEEDED (status));
	}
	else
	{
		parent = pClass;	// Hit an object whose parent is NULL, end the recursion.
	}
		
	status = pClass->Initialize (*classDefinition->id(),
								parent,
								classDefinition->name(),
								classDefinition->concrete() ? kAAFTrue : kAAFFalse);
	if (AAFRESULT_SUCCEEDED (status))
	{
		status = pClass->SetBootstrapParent(parent);
	}
		
		
  RegisterBuiltinProperties (classDefinition, pClass);

	popped = _initStack.pop ();
	ASSERTU (popped == rClassID);
	return status;
}


AAFRESULT ImplAAFBuiltinClasses::LookupOmPid
  (const aafUID_t & rAuid,
   OMPropertyId & outPid) const
{
  // Find the builtin class definition template.
	const AAFObjectModel * objectModel = AAFObjectModel::singleton();
	const PropertyDefinition * propertyDefinition = objectModel->findPropertyDefinition(&rAuid);

  if (propertyDefinition && !propertyDefinition->isNil())
		{
		  // Yup, matches.  Return the pid.
		  outPid = propertyDefinition->pid();
		  return AAFRESULT_SUCCESS;
		}

  // Not found.
  return E_FAIL;
}

AAFRESULT ImplAAFBuiltinClasses::MapOmPid
  (const aafUID_t & rAuid,
   OMPropertyId pid)
{
  if (pid < 0x8000)
    return E_FAIL; // Cannot map pids which aren't dynamic

  // Find the builtin class definition template.
	const AAFObjectModel * objectModel = AAFObjectModel::singleton();
	PropertyDefinition * propertyDefinition = const_cast<PropertyDefinition*>(objectModel->findPropertyDefinition(&rAuid));

  if (propertyDefinition && !propertyDefinition->isNil())
		{
		  // Yup, matches.  Set the pid.
          OMPropertyId ePid = propertyDefinition->pid();
          if ((ePid == 0) || (ePid >= 0x8000)) {
            // remap from zero or another dynamic pid
		    propertyDefinition->setPid(pid);
		    return AAFRESULT_SUCCESS;
          } else {
            // Cannot remap from non-dynamic pid
            return E_FAIL;
          }
		}

  // Not found.
  return E_FAIL;
}

ImplAAFClassDef * ImplAAFBuiltinClasses::LookupAxiomaticClass
(const aafUID_t & classId)
{
  ASSERTU (_dictionary);

  ImplAAFClassDef * result = 0;

	result = _dictionary->metaDictionary()->findAxiomaticClassDefinition(classId); // return value NOT reference counted!
  if (result)
  {
    result->AcquireReference(); // We will be returning this references!
  }

  return result;
}

bool ImplAAFBuiltinClasses::IsAxiomaticClass
(const aafUID_t & classId) const
{
  ASSERTU (_dictionary);

  bool result = false;


	ImplAAFClassDef *pAxiomaticClass = _dictionary->metaDictionary()->findAxiomaticClassDefinition(classId); // return value NOT reference counted!
  if (pAxiomaticClass)
  {
    result = true;
  }

  return result;
}


void ImplAAFBuiltinClasses::RegisterBuiltinProperties
(
 const ClassDefinition * classDefinition, 
 ImplAAFClassDef * pClassDef
) const
{
  ASSERTU (classDefinition && !classDefinition->isNil());
  ASSERTU (pClassDef);
  ASSERTU(!classDefinition->axiomatic()); // axiomatic classes should have already been created.

  // Get auid for given class def.
  aafUID_t classAuid;
  AAFRESULT hr;
  hr = pClassDef->GetAUID (&classAuid);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));

  // Look through all known builtin properties.  Identify which ones
  // belong to this class.  For each one, instantiate it and add it to
  // the class.
  aafUInt32 i;
	
	const PropertyDefinition * propertyDefinition = NULL;
	for (i = 0; i < classDefinition->propertyCount(); i++)
	{
		propertyDefinition = classDefinition->propertyDefinitionAt(i);
		ImplAAFPropertyDefSP pd;
		hr = pClassDef->pvtRegisterPropertyDef
						(*propertyDefinition->id(),
						propertyDefinition->name(),
						*propertyDefinition->typeId(),
						propertyDefinition->required() ? kAAFFalse : kAAFTrue,
						propertyDefinition->uid() ? kAAFTrue : kAAFFalse,
						&pd);
		ASSERTU (AAFRESULT_SUCCEEDED (hr));

		// Don't need these anymore
		ASSERTU (pd);
	}
}
