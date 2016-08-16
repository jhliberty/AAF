//@doc
//@class    AAFDictionary | Implementation class for AAFDictionary
#ifndef __CAAFDictionary_h__
#define __CAAFDictionary_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFDictionary.h,v 1.10 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif





















#ifndef __AAFObjectModel_h__
#include "AAFObjectModel.h"
#endif






#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFDictionary
  : public IAAFDictionary,
    public IAAFDictionary2,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDictionary (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDictionary ();

public:


  // Creates a single uninitialized AAF object of the class associated 
  /// with a specified stored object id. 
  STDMETHOD (CreateInstance)
   (// Class identifier (AUID) of the stored object. This is the
    // corresponding SMPTE identifier (as a GUID) for all predefined
    // built-in classes.
    /*[in, ref]*/ aafUID_constref id,

    // Reference to the identifier of the interface
    /*[in, ref]*/ REFIID riid,

    // Address of output variable that receives the 
    // interface pointer requested in riid
    /*[out, iid_is(riid)]*/ IUnknown ** ppvObject);


  // Creates a single uninitialized AAF meta class or type associated 
  /// with a specified stored object id. 
  STDMETHOD (CreateMetaInstance)
   (// Identifier (id) of a class or type definition. This is the
    // corresponding SMPTE identifier (as a GUID) for all predefined
    // built-in definitions.
    /*[in, ref]*/ aafUID_constref id,

    // Reference to the identifier of the interface
    /*[in, ref]*/ REFIID riid,

    // Address of output variable that receives the 
    // interface pointer requested in riid
    /*[out, iid_is(riid)]*/ IUnknown ** ppMetaDefinition);



  //***********************************************************
  //
  // RegisterClassDef()
  //
  // Add the class definition object to the dictionary.
  /// 
  /// Succeeds if:
  /// - The pClassDef pointer is valid.
  /// - the ID contained in the class def is not already been
  ///   registered.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pClassDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The class def ID has already been registered.
  //
  STDMETHOD (RegisterClassDef) (
    // Class Definition 
    /*[in]*/ IAAFClassDef * pClassDef);


  //***********************************************************
  //
  // LookupClassDef()
  //
  // Return the class definition with the given id.
  /// 
  /// Succeeds if:
  /// - The pClassID pointer is valid.
  /// - The ppClassDef pointer is valid.
  /// - the ID is a recognized id for a class definition.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pClassID or ppClassDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given ID is not recognized as a class definition ID.
  //
  STDMETHOD (LookupClassDef) (
    // Class Unique ID
    /*[in, ref]*/ aafUID_constref  classId,

    // Class Definition
    /*[out,retval]*/ IAAFClassDef ** ppClassDef);


  //***********************************************************
  //
  // GetClassDefs()
  //
  // Return an enumerator for all class definitions.
  ///
  /// Succeeds if:
  /// - The ppEnum pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum arg is NULL.
  //
  STDMETHOD (GetClassDefs) (
    // Class Definition Enumeration 
    /*[out,retval]*/ IEnumAAFClassDefs ** ppEnum);


  //***********************************************************
  //
  // CountClassDefs()
  //
  // Writes the number of class definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountClassDefs) (
    // Total number of class definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);

  //***********************************************************
  //
  // CreateForwardClassReference()
  //
  // Return the class definition with the given id.
  /// 
  /// Succeeds if:
  /// - The classId does not represent an existing forward class reference
  ///   or a class definition that has already been successfully registered.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given ID is not recognized as a class definition ID.
  //
  STDMETHOD (CreateForwardClassReference) (
    // Class Unique ID 
    /*[in, ref]*/ aafUID_constref  classId);

  //***********************************************************
  //
  // HasForwardClassReference()
  //
  // Return kAAFTrue if the given class identification is a forward reference.
  /// 
  /// Succeeds if:
  /// - The pClassID pointer is valid.
  /// - The ppClassDef pointer is valid.
  /// - the ID is a recognized id for a class definition.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pClassID or ppClassDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given ID is not recognized as a class definition ID.
  //
  STDMETHOD (HasForwardClassReference) (
    // Class Unique ID
    /*[in, ref]*/ aafUID_constref  classId,

    // true if forward class reference; false if not a forward class reference
    /*[out,retval]*/ aafBoolean_t *  pResult);

  //***********************************************************
  //
  // RegisterTypeDef()
  //
  // Add the type definition object to the dictionary.
  /// 
  /// Succeeds if:
  /// - The pTypeDef pointer is valid.
  /// - the ID is not already been registered.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pClassDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given type has already been registered.
  //
  STDMETHOD (RegisterTypeDef) (
    // Type Definition Object 
    /*[in]*/ IAAFTypeDef * pTypeDef);


  //***********************************************************
  //
  // LookupTypeDef()
  //
  // Return the type definition object with the given id.
  /// 
  /// Succeeds if:
  /// - The pTypeID pointer is valid.
  /// - The ppTypeDef pointer is valid.
  /// - the ID is a recognized id for a type definition.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pTypeID or ppTypeDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given ID is not recognized as a type definition ID.
  //
  STDMETHOD (LookupTypeDef) (
    // Type Unique ID
    /*[in, ref]*/ aafUID_constref  typeId,

    // Type Definition Object
    /*[out,retval]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // GetTypeDefs()
  //
  // Return an enumerator for all type definitions.
  ///
  /// Succeeds if:
  /// - The ppEnum pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum arg is NULL.
  //
  STDMETHOD (GetTypeDefs) (
    // Type Def Enumeration 
    /*[out,retval]*/ IEnumAAFTypeDefs ** ppEnum);


  //***********************************************************
  //
  // CountTypeDefs()
  //
  // Writes the number of type definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountTypeDefs) (
    // Total number of type definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterOpaqueTypeDef()
  //
  // Add the opaquetype definition object to the dictionary.
  /// 
  /// Succeeds if:
  /// - The pTypeDef pointer is valid.
  /// - the definition is not already been registered with RegisterTypeDef.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTypeDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given type has already been registered.
  //
  STDMETHOD (RegisterOpaqueTypeDef) (
    // Type Definition Object 
    /*[in]*/ IAAFTypeDef * pTypeDef);


  //***********************************************************
  //
  // LookupOpaqueTypeDef()
  //
  // Return the opaque type definition object with the given id.
  /// 
  /// Succeeds if:
  /// - The pTypeID pointer is valid.
  /// - The ppTypeDef pointer is valid.
  /// - the ID is a recognized id for an opaque type definition.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - Either typeId or ppTypeDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given ID is not recognized as a type definition ID.
  //
  STDMETHOD (LookupOpaqueTypeDef) (
    // Type Unique ID
    /*[in, ref]*/ aafUID_constref  typeId,

    // Type Definition Object
    /*[out,retval]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // GetOpaqueTypeDefs()
  //
  // Return an enumerator for all registered opaque type definitions.
  ///
  /// Succeeds if:
  /// - The ppEnum pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum arg is NULL.
  //
  STDMETHOD (GetOpaqueTypeDefs) (
    // Type Def Enumeration 
    /*[out,retval]*/ IEnumAAFTypeDefs ** ppEnum);


  //***********************************************************
  //
  // CountOpaqueTypeDefs()
  //
  // Writes the number of opaque type definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountOpaqueTypeDefs) (
    // Total number of opaque type definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);

  //***********************************************************
  //
  // RegisterKLVDataKey()
  //
  // Add the definition for the given KLV key to the runtime dictionary.
  /// The pTypeDef will often be kAAFTypeID_UInt8Array,
  /// but may be something else.
  /// 
  /// Succeeds if:
  /// - The pTypeDef pointer is valid.
  /// - the definition is not already been registered with RegisterTypeDef.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTypeDef arg is NULL.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given type has already been registered.
  //
  STDMETHOD (RegisterKLVDataKey) (
    // Key to define
    /*[in]*/ aafUID_t  pUID,

    // Type Definition Object
    /*[in]*/ IAAFTypeDef * pTypeDef);

  //***********************************************************
  //
  // RegisterDataDef()
  //
  // Add the data definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterDataDef) (
    // Data Definition Object 
    /*[in]*/ IAAFDataDef * pDataDef);


  //***********************************************************
  //
  // LookupDataDef()
  //
  // Return the data definition object with the given id.
  //
  STDMETHOD (LookupDataDef) (
    // Data Definition Unique ID
    /*[in, ref]*/ aafUID_constref  dataDefinitionId,

    // Data Definition Object
    /*[out,retval]*/ IAAFDataDef ** ppDataDef);


  //***********************************************************
  //
  // GetDataDefs()
  //
  // Return an enumerator for aff data definitions.
  //
  STDMETHOD (GetDataDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFDataDefs ** ppEnum);


  //***********************************************************
  //
  // CountDataDefs()
  //
  // Writes the number of data definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountDataDefs) (
    // Total number of data definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterOperationDef()
  //
  // Add the operation definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterOperationDef) (
    // Operation Definition Object 
    /*[in]*/ IAAFOperationDef * pOperationDef);


  //***********************************************************
  //
  // LookupOperationDef()
  //
  // Return the operation definition object with the given id.
  //
  STDMETHOD (LookupOperationDef) (
    // Operation Def Unique ID
    /*[in, ref]*/ aafUID_constref  operationId,

    // Operation definition object
    /*[out,retval]*/ IAAFOperationDef ** ppOperationDef);


  //***********************************************************
  //
  // GetOperationDefs()
  //
  // Return an enumerator for all operation definitions.
  //
  STDMETHOD (GetOperationDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFOperationDefs ** ppEnum);


  //***********************************************************
  //
  // CountOperationDefs()
  //
  // Writes the number of operation definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountOperationDefs) (
    // Total number of operation definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterParameterDef()
  //
  // Add the parameter definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterParameterDef) (
    // Parameter Definition Object 
    /*[in]*/ IAAFParameterDef * pParmDef);


  //***********************************************************
  //
  // LookupParameterDef()
  //
  // Return the parameter definition object with the given id.
  //
  STDMETHOD (LookupParameterDef) (
    // Parameter Unique ID
    /*[in, ref]*/ aafUID_constref  parameterId,

    // Parameter definition object
    /*[out,retval]*/ IAAFParameterDef ** ppParmDef);


  //***********************************************************
  //
  // GetParameterDefs()
  //
  // Return an enumerator for all parameter definitions.
  //
  STDMETHOD (GetParameterDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFParameterDefs ** ppEnum);


  //***********************************************************
  //
  // CountParameterDefs()
  //
  // Writes the number of parameter definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountParameterDefs) (
    // Total number of parameter definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterCodecDef()
  //
  // Add the codec definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterCodecDef) (
    // Codec Definition Object 
    /*[in]*/ IAAFCodecDef * pParmDef);


  //***********************************************************
  //
  // LookupCodecDef()
  //
  // Return the codec definition object with the given id.
  //
  STDMETHOD (LookupCodecDef) (
    // Parameter Unique ID
    /*[in, ref]*/ aafUID_constref  parameterId,

    // Codec definition object
    /*[out,retval]*/ IAAFCodecDef ** ppParmDef);


  //***********************************************************
  //
  // GetCodecDefs()
  //
  // Return an enumerator for all codec definitions.
  //
  STDMETHOD (GetCodecDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFCodecDefs ** ppEnum);


  //***********************************************************
  //
  // CountCodecDefs()
  //
  // Writes the number of codec definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountCodecDefs) (
    // Total number of codec definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterContainerDef()
  //
  // Add the container definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterContainerDef) (
    // Container Definition Object 
    /*[in]*/ IAAFContainerDef * pParmDef);


  //***********************************************************
  //
  // LookupContainerDef()
  //
  // Return the container definition object with the given id.
  //
  STDMETHOD (LookupContainerDef) (
    // Parameter Unique ID
    /*[in, ref]*/ aafUID_constref  parameterId,

    // Container definition object
    /*[out,retval]*/ IAAFContainerDef ** ppParmDef);


  //***********************************************************
  //
  // GetContainerDefs()
  //
  // Return an enumerator for all container definitions.
  //
  STDMETHOD (GetContainerDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFContainerDefs ** ppEnum);


  //***********************************************************
  //
  // CountContainerDefs()
  //
  // Writes the number of container definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountContainerDefs) (
    // Total number of container definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterInterpolationDef()
  //
  // Add the Interpolation definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterInterpolationDef) (
    // Interpolation Definition Object 
    /*[in]*/ IAAFInterpolationDef * pInterpolationDef);


  //***********************************************************
  //
  // LookupInterpolationDef()
  //
  // Return the Interpolation definition object with the given id.
  //
  STDMETHOD (LookupInterpolationDef) (
    // Parameter Unique ID
    /*[in, ref]*/ aafUID_constref  parameterId,

    // Interpolation definition object
    /*[out,retval]*/ IAAFInterpolationDef ** ppInterpolationDef);


  //***********************************************************
  //
  // GetInterpolationDefs()
  //
  // Return an enumerator for aff Interpolation definitions.
  //
  STDMETHOD (GetInterpolationDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFInterpolationDefs ** ppEnum);


  //***********************************************************
  //
  // CountInterpolationDefs()
  //
  // Writes the number of interpolation definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountInterpolationDefs) (
    // Total number of interpolation definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // RegisterPluginDef()
  //
  // Add the plugin definition object to the header's list of definitions.
  //
  STDMETHOD (RegisterPluginDef) (
    // plugin definition Object 
    /*[in]*/ IAAFPluginDef * pPlugDef);


  //***********************************************************
  //
  // LookupPluginDef()
  //
  // Return the plugin descriptor object with the given id.
  //
  STDMETHOD (LookupPluginDef) (
    // Parameter Unique ID
    /*[in, ref]*/ aafUID_constref  parameterId,

    // plugin descriptor object
    /*[out,retval]*/ IAAFPluginDef ** ppPlugDef);


  //***********************************************************
  //
  // GetPluginDefs()
  //
  // Return an enumerator for all plugin descriptors.
  //
  STDMETHOD (GetPluginDefs) (
    // Definition Enumeration 
    /*[out,retval]*/ IEnumAAFPluginDefs ** ppEnum);


  //***********************************************************
  //
  // CountPluginDefs()
  //
  // Writes the number of plugin definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountPluginDefs) (
    // Total number of plugin definition objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  // METHOD NAME: RegisterKLVDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | RegisterKLVDataDef |
  // Add the KLVData definition object to the header's list of definitions.
  // @end
  // 
  STDMETHOD (RegisterKLVDataDef)
   (
    // @parm [in] AAFKLVDataDefinition | pDef | plugin definition object
    IAAFKLVDataDefinition * pDef
  );

  //***********************************************************
  // METHOD NAME: LookupKLVDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupKLVDataDef |
  // Return the KLVData descriptor object with the given id.
  // @end
  // 
  STDMETHOD (LookupKLVDataDef)
   (
    // @parm [in, ref] aafUID_constref | defId | KLV data definition Unique ID
    aafUID_constref  defId,

    // @parm [out,retval] AAFKLVDataDefinition | ppDef | KLVData descriptor object
    IAAFKLVDataDefinition ** ppDef
  );

  //***********************************************************
  // METHOD NAME: GetKLVDataDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | GetKLVDataDefs |
  // Return an enumerator for all KLVData descriptors.
  // @end
  // 
  STDMETHOD (GetKLVDataDefs)
   (
    // @parm [out,retval] EnumAAFKLVDataDefs | ppEnum | Definition Enumeration
    IEnumAAFKLVDataDefs ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: CountKLVDataDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | CountKLVDataDefs |
  // Writes the number of KLVData definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  // @end
  // 
  STDMETHOD (CountKLVDataDefs)
   (
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
  STDMETHOD (RegisterTaggedValueDef)
   (
    // @parm [in] AAFTaggedValueDefinition | pDef | tagged value definition Object
    IAAFTaggedValueDefinition * pDef
  );

  //***********************************************************
  // METHOD NAME: LookupTaggedValueDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupTaggedValueDef |
  // Return the tagged value descriptor object with the given id.
  // @end
  // 
  STDMETHOD (LookupTaggedValueDef)
   (
    // @parm [in, ref] aafUID_constref | defId | tagged value definition ID
    aafUID_constref  defId,

    // @parm [out,retval] AAFTaggedValueDefinition | ppDef | tagged value descriptor object
    IAAFTaggedValueDefinition ** ppDef
  );

  //***********************************************************
  // METHOD NAME: GetTaggedValueDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | GetTaggedValueDefs |
  // Return an enumerator for all tagged value descriptors.
  // @end
  // 
  STDMETHOD (GetTaggedValueDefs)
   (
    // @parm [out,retval] EnumAAFTaggedValueDefs | ppEnum | Definition Enumeration
    IEnumAAFTaggedValueDefs ** ppEnum
  );

  //***********************************************************
  // METHOD NAME: CountTaggedValueDefs()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | CountTaggedValueDefs |
  // Writes the number of tagged value definition objects into the
  /// *pResult argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pResult.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  // @end
  // 
  STDMETHOD (CountTaggedValueDefs)
   (
    // @parm [out, retval] aafUInt32 * | pResult | Total number of tagged value definition objects
    aafUInt32 *  pResult
  );


  //***********************************************************
  // METHOD NAME: LookupAuxiliaryDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupAuxiliaryDataDef |
  // Return the Auxiliary Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupAuxiliaryDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupDescriptiveMetadataDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupDescriptiveMetadataDataDef |
  // Return the Descriptive Metadata Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupDescriptiveMetadataDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupEdgecodeDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupEdgecodeDataDef |
  // Return the Edgecode Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupEdgecodeDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupLegacyPictureDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupLegacyPictureDataDef |
  // Return the legacy Picture Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupLegacyPictureDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupLegacySoundDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupLegacySoundDataDef |
  // Return the legacy Sound Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupLegacySoundDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupLegacyTimecodeDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupLegacyTimecodeDataDef |
  // Return the legacy Timecode Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupLegacyTimecodeDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupMatteDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupMatteDataDef |
  // Return the Matte Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupMatteDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupPictureDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupPictureDataDef |
  // Return the Picture Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupPictureDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupPictureWithMatteDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupPictureWithMatteDataDef |
  // Return the PictureWithMatte Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupPictureWithMatteDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupSoundDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupSoundDataDef |
  // Return the Sound Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupSoundDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );

  //***********************************************************
  // METHOD NAME: LookupTimecodeDataDef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDictionary2 | LookupTimecodeDataDef |
  // Return the Timecode Data Definition Object.
  /// 
  /// Succeeds if:
  /// - The ppDataDef pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef arg is NULL.
  // @end
  // 
  STDMETHOD (LookupTimecodeDataDef)
   (
    // @parm [out,retval] AAFDataDef | ppDataDef | Data Definition Object
    IAAFDataDef ** ppDataDef
  );




protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFDictionary_h__

