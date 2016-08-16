//@doc
//@class    AAFDictionary | Implementation class for AAFDictionary
#ifndef __ImplAAFDictionary_h__
#define __ImplAAFDictionary_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFDictionary.h,v 1.75 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


class ImplAAFBuiltinClasses;
class ImplAAFBuiltinDefs;
class ImplAAFBuiltinTypes;
class ImplAAFClassDef;
class ImplAAFCodecDef;
class ImplAAFContainerDef;
class ImplAAFDataDef;
class ImplAAFInterpolationDef;
class ImplAAFOperationDef;
class ImplAAFPluginDef;
class ImplAAFKLVDataDefinition;
class ImplAAFTaggedValueDefinition;
class ImplAAFTypeDef;
class ImplAAFTypeDefEnum;
class ImplAAFTypeDefRecord;
template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFClassDef>			ImplEnumAAFClassDefs;
typedef ImplAAFEnumerator<ImplAAFCodecDef>			ImplEnumAAFCodecDefs;
typedef ImplAAFEnumerator<ImplAAFContainerDef>		ImplEnumAAFContainerDefs;
typedef ImplAAFEnumerator<ImplAAFDataDef>			ImplEnumAAFDataDefs;
typedef ImplAAFEnumerator<ImplAAFInterpolationDef>	ImplEnumAAFInterpolationDefs;
typedef ImplAAFEnumerator<ImplAAFPluginDef>			ImplEnumAAFPluginDefs;
typedef ImplAAFEnumerator<ImplAAFKLVDataDefinition>			ImplEnumAAFKLVDataDefs;
typedef ImplAAFEnumerator<ImplAAFTaggedValueDefinition>		ImplEnumAAFTaggedValueDefs;
typedef ImplAAFEnumerator<ImplAAFTypeDef>			ImplEnumAAFTypeDefs;
typedef ImplAAFEnumerator<ImplAAFOperationDef>		ImplEnumAAFOperationDefs;

class ImplAAFMetaDictionary;
class ImplAAFMetaDefinition;
class ImplAAFTypeDefVariableArray;
class ImplAAFTypeDefFixedArray;
class ImplAAFTypeDefRecord;
class ImplAAFTypeDefRename;
class ImplAAFTypeDefString;
class ImplAAFTypeDefStrongObjRef;
class ImplAAFTypeDefWeakObjRef;
class ImplAAFTypeDefSet;

#include "ImplAAFObject.h"

#include "OMClassFactory.h"
#include "OMStrongRefSetProperty.h"

#include "ImplAAFClassDef.h"
#include "ImplAAFOperationDef.h"
#include "ImplAAFDefObject.h"
#include "ImplAAFCodecDef.h"
#include "ImplAAFContainerDef.h"
#include "ImplAAFInterpolationDef.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFDataDef.h"
#include "ImplAAFPluginDef.h"

// FIXMEJPT Put this here for windows. 
// Not required on Linux
#include "ImplAAFKLVDataDefinition.h"
#include "ImplAAFTaggedValueDefinition.h"


class ImplAAFDictionary :
  public OMClassFactory,
  public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDictionary ();

protected:
  virtual ~ImplAAFDictionary ();

public:

  // Return true if "classId" is registered.
  virtual bool isRegistered(const OMClassId& classId) const;

  // Create an instance of the appropriate derived class, given the
  // class id.  Initializes the OM properties.
  // 
  // This method implements the OMClassFactory interface.
  //
  OMStorable* create(const OMClassId& classId) const;

  void destroy(OMStorable* victim) const;

  void associate(const aafUID_t& id,
                 const OMPropertyId propertyId);

  //****************
  // CreateInstance()
  //
  // Creates a single uninitialized AAF object of the class associated 
  // with a specified stored object id.
  virtual AAFRESULT STDMETHODCALLTYPE 
  CreateInstance (
    // ID of the stored object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFObject ** ppvObject);


  //****************
  // CreateMetaInstance()
  //
  // Creates a single uninitialized AAF meta definition associated 
  // with a specified stored object id.
  virtual AAFRESULT STDMETHODCALLTYPE 
  CreateMetaInstance (
    // Stored Object ID of the meta object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFMetaDefinition ** ppMetaObject);


  //****************
  // LookupClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupClassDef
        (// @parm [in,ref] Class Unique ID
         const aafUID_t & classId,

         // @parm [out,retval] Class Definition
         ImplAAFClassDef ** ppClassDef);

  //****************
  // CreateForwardClassReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateForwardClassReference 
        (// @parm [in] class identification
         aafUID_constref classId);

  //****************
  // IsForwardClassReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    HasForwardClassReference 
      (// @parm [in] class identification
       aafUID_constref classId,

       // @parm [out] kAAFTrue if the given class identification is a forward reference.
       aafBoolean_t *pResult);

  //****************
  // RegisterClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterClassDef
        (// @parm [in] Class Definition
         ImplAAFClassDef * pClassDef);


  //****************
  // GetClassDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetClassDefs
        // @parm [out,retval] Class Definition Enumeration
        (ImplEnumAAFClassDefs ** ppEnum);


  //****************
  // CountClassDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountClassDefs
        // @parm [out, retval] Total number of class definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterTypeDef
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pTypeDef);

  //****************
  // LookupTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupTypeDef
        (// @parm [in,ref] Type Unique ID
         const aafUID_t & typeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppTypeDef);


  //****************
  // GetTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefs
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // CountTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountTypeDefs
        // @parm [out, retval] Total number of type definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterOpaqueTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterOpaqueTypeDef
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pOpaqueTypeDef);

  //****************
  // LookupOpaqueTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupOpaqueTypeDef
        (// @parm [in,ref] Type Unique ID
         const aafUID_t & typeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppOpaqueTypeDef);

  //****************
  // GetOpaqueTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOpaqueTypeDefs
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // CountOpaqueTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountOpaqueTypeDefs
        // @parm [out, retval] Total number of type definition objects
        (aafUInt32 * pResult);

	virtual AAFRESULT STDMETHODCALLTYPE
		RegisterKLVDataKey (
			aafUID_t keyUID, 
			ImplAAFTypeDef *underlyingType);

	  //****************
  // RegisterDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterDataDef
        // @parm [in] Data Definition Object
        (ImplAAFDataDef * pDataDef);

  //****************
  // LookupDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupDataDef
        (// @parm [in,ref] Datak Definition Unique ID
         const aafUID_t & dataDefinitionId,

         // @parm [out,retval] Data Definition Object
         ImplAAFDataDef ** ppDataDef);

  //****************
  // GetDataDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFDataDefs ** ppEnum);


  //****************
  // CountDataDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountDataDefs
        // @parm [out, retval] Total number of data definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterOperationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterOperationDef
        // @parm [in] Effect Definition Object
        (ImplAAFOperationDef * pOperationDef);

  //****************
  // LookupOperationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupOperationDef
        (// @parm [in,ref] Effect Unique ID
         const aafUID_t & effectId,

         // @parm [out,retval] Effect definition object
         ImplAAFOperationDef ** ppOperationDef);

  //****************
  // GetOperationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperationDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFOperationDefs ** ppEnum);


  //****************
  // CountOperationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountOperationDefs
        // @parm [out, retval] Total number of interpolation definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterParameterDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterParameterDef
        // @parm [in] Parameter Definition Object
        (ImplAAFParameterDef * pParameterDef);

  //****************
  // LookupParameterDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupParameterDef
        (// @parm [in,ref] Parameter Unique ID
         const aafUID_t & parameterId,

         // @parm [out,retval] Parameter definition object
         ImplAAFParameterDef ** ppParameterDef);

  //****************
  // GetParameterDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFParameterDefs ** ppEnum);

  //****************
  // CountParameterDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountParameterDefs
        // @parm [out, retval] Total number of parameter definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterCodecDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterCodecDef
        // @parm [in] Pluggable Definition
        (ImplAAFCodecDef * pPlugDef);

  //****************
  // GetCodecDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
   GetCodecDefs
      // @parm [out,retval] Definition Enumeration
      (ImplEnumAAFCodecDefs ** ppEnum);


  virtual AAFRESULT STDMETHODCALLTYPE
    LookupCodecDef
      (const aafUID_t & containerId,
	   ImplAAFCodecDef **result);


  //****************
  // CountCodecDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountCodecDefs
        // @parm [out, retval] Total number of codec definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterContainerDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterContainerDef
        // @parm [in] Pluggable Definition
        (ImplAAFContainerDef * pPlugDef);


  //****************
  // GetContainerDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContainerDefs
        // @parm [out,retval] Definition Enumeration
       (ImplEnumAAFContainerDefs ** ppEnum);


  //****************
  // LookupContainerDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
     LookupContainerDef
       (const aafUID_t & containerID,
		ImplAAFContainerDef **result);


  //****************
  // CountContainerDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountContainerDefs
        // @parm [out, retval] Total number of container definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterInterpolationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterInterpolationDef
        // @parm [in] Interpolation Definition Object
        (ImplAAFInterpolationDef * pInterpolationDef);

  //****************
  // LookupInterpolationDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupInterpolationDef
        (// @parm [in,ref] Interpolation Unique ID
         const aafUID_t & interpolationID,

         // @parm [out,retval] Interpolation definition object
         ImplAAFInterpolationDef ** ppInterpolationDef);

  //****************
  // GetInterpolationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInterpolationDefs
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFInterpolationDefs ** ppEnum);

  //****************
  // CountInterpolationDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountInterpolationDefs
        // @parm [out, retval] Total number of interpolation definition objects
        (aafUInt32 * pResult);


  //****************
  // RegisterPluginDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterPluginDef
        (ImplAAFPluginDef * pPluginDesc);

  //****************
  // LookupPluginDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPluginDef
        (// @parm [in,ref] Interpolation Unique ID
         const aafUID_t & interpolationID,

         ImplAAFPluginDef ** ppPluginDesc);

  //****************
  // GetPluginDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginDefs
        (ImplEnumAAFPluginDefs ** ppEnum);


  //****************
  // CountPluginDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountPluginDefs
        // @parm [out, retval] Total number of plugin definition objects
        (aafUInt32 * pResult);


  //***********************************************************
  // METHOD NAME: RegisterKLVDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | RegisterKLVDataDef |
  // Add the KLVData definition object to the header's list of definitions.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  RegisterKLVDataDef (
    // @parm [in] AAFKLVDataDefinition | pDef | plugin definition object
    ImplAAFKLVDataDefinition * pDef
  );

  //***********************************************************
  // METHOD NAME: LookupKLVDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupKLVDataDef |
  // Return the KLVData descriptor object with the given id.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  LookupKLVDataDef (
    // @parm [in, ref] aafUID_constref | defId | KLV data definition ID
    aafUID_constref  defId,

    // @parm [out,retval] AAFKLVDataDefinition | ppDef | KLVData descriptor object
    ImplAAFKLVDataDefinition ** ppDef
  );

  //***********************************************************
  // METHOD NAME: GetKLVDataDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | GetKLVDataDefs |
  // Return an enumerator for all KLVData descriptors.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetKLVDataDefs (
    // @parm [out,retval] EnumAAFKLVDataDefs | ppEnum | Definition Enumeration
    ImplEnumAAFKLVDataDefs ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: CountKLVDataDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | CountKLVDataDefs |
  // Writes the number of KLVData definition objects into the
  // *pResult argument.
  // 
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
  // 
  // If this method fails nothing will be written to *pResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  CountKLVDataDefs (
    // @parm [out, retval] aafUInt32 * | pResult | Total number of KLVData definition objects
    aafUInt32 *  pResult
  );

  //***********************************************************
  // METHOD NAME: RegisterTaggedValueDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | RegisterTaggedValueDef |
  // Add the tagged value definition object to the header's list of definitions.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  RegisterTaggedValueDef (
    // @parm [in] AAFTaggedValueDefinition | pDef | tagged value definition Object
    ImplAAFTaggedValueDefinition * pDef
  );

  //***********************************************************
  // METHOD NAME: LookupTaggedValueDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupTaggedValueDef |
  // Return the tagged value descriptor object with the given id.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  LookupTaggedValueDef (
    // @parm [in, ref] aafUID_constref | defId | tagged value definition ID
    aafUID_constref  defId,

    // @parm [out,retval] AAFTaggedValueDefinition | ppDef | tagged value descriptor object
    ImplAAFTaggedValueDefinition ** ppDef
  );

  //***********************************************************
  // METHOD NAME: GetTaggedValueDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | GetTaggedValueDefs |
  // Return an enumerator for all tagged value descriptors.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetTaggedValueDefs (
    // @parm [out,retval] EnumAAFTaggedValueDefs | ppEnum | Definition Enumeration
    ImplEnumAAFTaggedValueDefs ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: CountTaggedValueDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | CountTaggedValueDefs |
  // Writes the number of tagged value definition objects into the
  // *pResult argument.
  // 
  // Succeeds if all of the following are true:
  // - the pResult pointer is valid.
  // 
  // If this method fails nothing will be written to *pResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult is null.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  CountTaggedValueDefs (
    // @parm [out, retval] aafUInt32 * | pResult | Total number of tagged value definition objects
    aafUInt32 *  pResult
  );

  //****************
  // LookupAuxiliaryDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupAuxiliaryDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);

  //****************
  // LookupDescriptiveMetadataDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupDescriptiveMetadataDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupEdgecodeDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupEdgecodeDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupLegacyPictureDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupLegacyPictureDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupLegacySoundDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupLegacySoundDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupLegacyTimecodeDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupLegacyTimecodeDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupMatteDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupMatteDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupPictureDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPictureDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupPictureWithMatteDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPictureWithMatteDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupSoundDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupSoundDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);


  //****************
  // LookupTimecodeDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupTimecodeDataDef
        // @parm [out,retval] Data Definition Object
        (ImplAAFDataDef ** ppDataDef);



public:

  //
  // Internal to the SDK
  //

  // Return a pointer to the meta dictionary.
  ImplAAFMetaDictionary *metaDictionary(void) const;

  // Install the meta dictionary (i.e. the factory for creating
  // meta data: classes, properties and types). This method
  // can only be called once.
  void setMetaDictionary(ImplAAFMetaDictionary *metaDictionary);

  // Private class registration. This version does not perform any
  // initialization that requires other classes, types or properties or
  // types to be in the dictionary...it only adds the given class
  // to the set in the dictionary.
  AAFRESULT PvtRegisterClassDef(ImplAAFClassDef * pClassDef);

AAFRESULT PvtIsPropertyDefDuplicate(
							aafUID_t propertyDefID,
							ImplAAFClassDef *correctClass,
							bool	*isDuplicate);


  //****************
  // GetNumEssenceData()
  //
  virtual AAFRESULT
    GetNumCodecDefs
        (aafUInt32 *  pNumCodecDefs);  //@parm [out,retval] Total number of pluggable defs

  virtual AAFRESULT
    GetNumContainerDefs
        (aafUInt32 *  pNumContainerDefs);  //@parm [out,retval] Total number of pluggable defs

  // make sure built-in types are initialized.
  void InitBuiltins();

  //
  // Toolkit-internal "Create" methods
  //

  // Factory method for creating a Dictionary.
  static ImplAAFDictionary *CreateDictionary(void);

  //
  // Instantiates an object of the given class and initializes its OM
  // properties.
  //
  ImplAAFObject * CreateAndInit(ImplAAFClassDef * pClassDef) const;

  //
  // Instantiates the most derived class known to the code which
  // represents the given AUID.  Doesn't init its OM properties.
  //
  ImplAAFObject* pvtInstantiate(const aafUID_t & id) const;

  //
  // If the given AUID is known to the code, instantiates an object of
  // that type.  Otherwise, returns NULL.  Doesn't init its OM
  // properties.
  //
  static ImplAAFObject* pvtCreateBaseClassInstance(const aafUID_t & id);

  // Generates an OM PID corresponding to the given property def auid.
  AAFRESULT GenerateOmPid (const aafUID_t & rAuid,
						   OMPropertyId & rOutPid);


  // Like LookupClassDef(), except will only look at classes currently
  // registered in this dictionary; will not attempt to look at
  // builtins which may not have already been entered into the dict.
  AAFRESULT dictLookupClassDef
    (// @parm [in,ref] Class Unique ID
	 const aafUID_t & classID,

	 // @parm [out,retval] Class Definition
	 ImplAAFClassDef ** ppClassDef);

bool PvtIsClassPresent (
      const aafUID_t & classID);

  // Like LookupType(), except will only look at types currently
  // registered in this dictionary; will not attempt to look at
  // builtins which may not have already been entered into the dict.
  AAFRESULT dictLookupTypeDef
    (// @parm [in,ref] Type Unique ID
	 const aafUID_t & typeID,

	 // @parm [out,retval] Type Definition Object
	 ImplAAFTypeDef ** ppTypeDef);

bool PvtIsTypePresent (
      const aafUID_t & typeID);

  // Attempt to register the sizes of this type def if it is a
  // built-in type.  Currently implemented for Enum and Record
  // typedefs.
  void pvtAttemptBuiltinSizeRegistration (ImplAAFTypeDefEnum * ptde) const;
  void pvtAttemptBuiltinSizeRegistration (ImplAAFTypeDefRecord * ptdr) const;

  // Assures that all prop types in all contained classes are present,
  // as well as the prop types in all classes looked up in the
  // future.
  void AssureClassPropertyTypes ();
  void AssurePropertyTypes (ImplAAFClassDef * pcd);

  // Enables/disables registration of definitions.  Used during save()
  // to assure contents of dict remains constant.  Returns current
  // value of flag.  Default is enabled.
  bool SetEnableDefRegistration (bool isEnabled);

  bool IsAxiomaticClass (const aafUID_t & classID) const;

  ImplAAFBuiltinDefs * GetBuiltinDefs ();
  void SetBuiltinClasses(ImplAAFBuiltinClasses *pBuiltinClasses) { _pBuiltinClasses = pBuiltinClasses; };
  bool pvtLookupAxiomaticTypeDef (const aafUID_t & typeID,
							   ImplAAFTypeDef ** ppTypeDef);

  // Initialize all of the axiomatic and required built-in definitions
  // have been initialized. This should be called after the file has been opened.
  void InitializeMetaDefinitions(void);

  // Merge class definitions, their property definitions and their
  // dependencies to the destination dictionary.
  virtual AAFRESULT MergeTo( ImplAAFDictionary* pDestDictionary );

private:
  void InitDataDefinition(const aafUID_t & dataDefinitionID,
                   const aafCharacter *name,
				   const aafCharacter *description);

  void InitContainerDefinition(const aafUID_t & defID,
                        const aafCharacter *name,
				        const aafCharacter *description);

  bool pvtLookupAxiomaticClassDef (const aafUID_t & classID,
								   ImplAAFClassDef ** ppClassDef);

  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFOperationDef>			_operationDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFParameterDef>			_parameterDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFDataDef>				_dataDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFPluginDef>				_pluginDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFCodecDef>				_codecDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFContainerDef>			_containerDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFInterpolationDef>		_interpolationDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFKLVDataDefinition>                  _klvDataDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFTaggedValueDefinition>               _taggedValueDefinitions;

  ImplAAFBuiltinClasses * _pBuiltinClasses;
  ImplAAFBuiltinTypes   * _pBuiltinTypes;
  ImplAAFBuiltinDefs    * _pBuiltinDefs;

  // records a sequence of dynamic property ids starting with firstPid and ending with lastPid
  class PIDSegment
  {
  public:
      bool operator==(const PIDSegment& r);
      
      OMPropertyId firstPid;
      OMPropertyId lastPid;
  };
    
  // maps the use of dynamic property pids to allow the generation of new values
  // no matter what generation method was used before
  OMVector<PIDSegment> _pidSegments;
  bool _pidSegmentsInitialised;

  static const aafUID_t * sAxiomaticTypeGuids[];
  static const aafUID_t * sAxiomaticClassGuids[];

  ImplAAFTypeDefSP * _axiomaticTypes;

  struct axClassLookupElem
  {
	ImplAAFClassDef * pClassDef;
	aafUID_t          classId;
  };

  axClassLookupElem * _axiomaticClasses;

  // if true, signifies that it's OK to assure that all prop types in
  // all looked-up classes are present.
  bool _OKToAssurePropTypes;

  bool _defRegistrationAllowed;

  // Set to true when the all of the axiomatic and required built-in definitions
  // have been initialized. This should be set after the file has been opened.
  bool _metaDefinitionsInitialized;

  ImplAAFMetaDictionary *_metaDictionary;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;


/*************************************************************************
    aafLookup*Def()

	These helper functions search for specified definition in 
	given object's dictionary.

    Inputs:
	p_holder	- definition object to look in.
	p_*def		- definition to look for.

    Returns:
	kAAFTrue - definition found in given objects dictionary.
	kAAFFalse - definition is not in a dictionary.
 *************************************************************************/
aafBoolean_t aafLookupTypeDef( 
    ImplAAFObject	*p_holder,
    ImplAAFTypeDef		*p_typedef );
aafBoolean_t aafLookupOperationDef( 
    ImplAAFObject	*p_holder,
    ImplAAFOperationDef		*p_operdef );
aafBoolean_t aafLookupParameterDef( 
    ImplAAFObject	*p_holder,
    ImplAAFParameterDef		*p_paramdef );
aafBoolean_t aafLookupClassDef( 
    ImplAAFObject	*p_holder,
    ImplAAFClassDef		*p_classdef );
aafBoolean_t aafLookupDataDef( 
    ImplAAFObject	*p_holder,
    ImplAAFDataDef		*p_datadef );
aafBoolean_t aafLookupCodecDef( 
    ImplAAFObject	*p_holder,
    ImplAAFCodecDef		*p_codecdef );
aafBoolean_t aafLookupContainerDef( 
    ImplAAFObject	*p_holder,
    ImplAAFContainerDef		*p_containerdef );
aafBoolean_t aafLookupInterpolationDef( 
    ImplAAFObject	*p_holder,
    ImplAAFInterpolationDef	*p_interpoldef );

aafBoolean_t aafLookupTypeDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFTypeDef		*p_typedef );
aafBoolean_t aafLookupOperationDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFOperationDef		*p_operdef );
aafBoolean_t aafLookupParameterDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFParameterDef		*p_paramdef );
aafBoolean_t aafLookupClassDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFClassDef		*p_classdef );
aafBoolean_t aafLookupDataDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFDataDef		*p_datadef );
aafBoolean_t aafLookupCodecDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFCodecDef		*p_codecdef );
aafBoolean_t aafLookupContainerDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFContainerDef		*p_containerdef );
aafBoolean_t aafLookupInterpolationDef( 
    ImplAAFMetaDefinition	*p_holder,
    ImplAAFInterpolationDef	*p_interpoldef );



#endif // ! __ImplAAFDictionary_h__
