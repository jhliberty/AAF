//@doc
//@class    AAFHeader | Implementation class for AAFHeader
#ifndef __CAAFHeader_h__
#define __CAAFHeader_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFHeader.h,v 1.11 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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












#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif

//
// Forward declaration
//
class ImplAAFHeader;


class CAAFHeader
  : public IAAFHeader,
    public IAAFEndian,
	public IAAFHeader2,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFHeader (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFHeader ();

public:

  //***********************************************************
  //
  // LookupMob()
  //
  // Looks up the Mob that matches the given mob id and puts it into
  /// the ppMob argument.  The returned mob interface is AddRef()ed
  /// before it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppMob pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppMob.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppMob is null
  ///
  /// AAFRESULT_MOB_NOT_FOUND
  ///   - the requested mob wasn't found.
  //
  STDMETHOD (LookupMob) (
    // The Mob ID
    /*[in, ref]*/ aafMobID_constref  mobID,

    // Matching Mob
    /*[out,retval]*/ IAAFMob ** ppMob);


  //***********************************************************
  //
  // CountMobs()
  //
  // Writes the number of matches for the given mob kind into the
  /// *pNumMobs argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pNumMobs pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pNumMobs.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumMobs is null.
  //
  STDMETHOD (CountMobs) (
    // The mob kind to count
    /*[in]*/ aafMobKind_t  mobKind,

    // Total number of mobs of kind mobKind
    /*[out, retval]*/ aafNumSlots_t *  pResult);


  //***********************************************************
  //
  // GetMobs()
  //
  // Places an enumerator for mobs that apply to the criteria into the
  /// *ppEnum argument.  If pSearchCriteria is null, all mobs are
  /// returned.   The searchTag field of pSearchCriteria, and exactly
  /// ONE of the fields in the union (tags.mobID, tags.name, etc. )
  /// must be set.  Only one search criterion may be specified.  The
  /// returned enumerator is AddRef()ed before it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (GetMobs) (
    // Search Criteria for enumeration
    /*[in]*/ aafSearchCrit_t *  pSearchCriteria,

    // Mob Enumeration
    /*[out, retval]*/ IEnumAAFMobs ** ppEnum);


  //***********************************************************
  //
  // AddMob()
  //
  // Appends the given mob to the header.  If the given mob is already
  /// contained this method will do nothing and will return success.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pMob pointer is valid.
  /// - the given mob is not already part of this collection.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pMob is null.
  ///
  /// AAFRESULT_DUPLICATE_MOBID
  ///   - the given mob is already contained.
  //
  STDMETHOD (AddMob) (
    // Mob to add 
    /*[in]*/ IAAFMob * pMob);


  //***********************************************************
  //
  // RemoveMob()
  //
  // Removes the given mob from the header.
  ///
  /// Succeeds if all of the following are true:
  /// - the pMob pointer is valid.
  /// - the given mob is currently in the collection.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pMob is null.
  ///
  /// AAFRESULT_MOB_NOT_FOUND
  ///   - the given mob is not already contained.
  //
  STDMETHOD (RemoveMob) (
    // Mob to remove 
    /*[in]*/ IAAFMob * pMob);


  //***********************************************************
  //
  // CountEssenceData()
  //
  // Writes the total number of essence data into the *pNumEssenceData
  /// argument.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pNumEssenceData pointer is valid.
  /// 
  /// If this method fails nothing will be written to
  /// *pNumEssenceData.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumEssenceData is null.
  //
  STDMETHOD (CountEssenceData) (
    // Total number of essence data 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // IsEssenceDataPresent()
  //
  // Returns true if the essence is found.
  ///
  /// Succeeds if all of the following are true:
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails no state will be changed.
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
  STDMETHOD (IsEssenceDataPresent) (
    // A Unique File Mob ID
    /*[in, ref]*/ aafMobID_constref  fileMobID,

    // The Essence File Format
    /*[in]*/ aafFileFormat_t  fmt,

    // True if the essence is found
    /*[out,retval]*/ aafBoolean_t *  pResult);


  //***********************************************************
  //
  // EnumEssenceData()
  //
  // Places an enumerator for essence that applies to the criteria
  /// into the *ppEnum argument.  The returned enumerator is
  /// AddRef()ed before it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pMediaCriteria pointer is valid.
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - either pMediaCriteria or ppEnum is null.
  //
  STDMETHOD (EnumEssenceData) (
    // Essence Enumeration 
    /*[out,retval]*/ IEnumAAFEssenceData ** ppEnum);


  //***********************************************************
  //
  // AddEssenceData()
  //
  // Appends the given essence data object to the header.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pEssenceData pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_DUPLICATE_MOBID
  ///   - The given mob has already been added.  The validation is done by comparing
  ///     mobIDs, which should be unique.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceData is null.
  //
  STDMETHOD (AddEssenceData) (
    // Essence data object to append 
    /*[in]*/ IAAFEssenceData * pEssenceData);

  //***********************************************************
  //
  // RemoveEssenceData()
  //
  // Removes the given EssenceData from the header.
  ///
  /// Succeeds if all of the following are true:
  /// - the pEssenceData pointer is valid.
  /// - the given EssenceData is currently in the collection.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceData is null.
  ///
  /// AAFRESULT_ESSENCE_NOT_FOUND
  ///   - the given EssenceData is not already contained.
  //
  STDMETHOD (RemoveEssenceData) (
    // EssenceData to remove 
    /*[in]*/ IAAFEssenceData * pEssenceData);

  //***********************************************************
  //
  // LookupEssenceData()
  //
  // Looks up the EssenceData that matches the given mob id and puts it into
  /// the ppEssenceData argument.  The returned EssenceData interface is AddRef()ed
  /// before it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEssenceData pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEssenceData.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEssenceData is null
  ///
  /// AAFRESULT_MOB_NOT_FOUND
  ///   - the requested EssenceData wasn't found.
  //
  STDMETHOD (LookupEssenceData) (
    // The Mob ID
    /*[in, ref]*/ aafMobID_constref  mobID,

    // Matching EssenceData
    /*[out,retval]*/ IAAFEssenceData ** ppEssenceData);

  //***********************************************************
  //
  // GetDictionary()
  //
  // Places the dictionary that contains all types of aaf definition
  /// objects into the *ppDictionary argument.  The returned dictionary
  /// is AddRef()ed before it is returned.  Note that the dictionary
  /// is automatically created when the header object is created.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppDictionary pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppDictionary is null.
  //
  STDMETHOD (GetDictionary) (
    // The AAF Dictionary 
    /*[out, retval]*/ IAAFDictionary ** ppDictionary);


  //***********************************************************
  //
  // GetLastIdentification()
  //
  // Places the identification of the last entity that modified the
  /// file into the *ppIdentification argument.  The returned
  /// identification is AddRef()ed before it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppIdentification pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppIdentification is null.
  //
  STDMETHOD (GetLastIdentification) (
    // Indentification Object 
    /*[out,retval]*/ IAAFIdentification ** ppIdentification);


  //***********************************************************
  //
  // LookupIdentification()
  //
  // Places the Identification that matches the given generation into
  /// the *ppIdentification argument.  The returned identification is
  /// AddRef()ed before it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppIdentification pointer is valid.
  /// - the given generation was found.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppIdentification is null.
  ///
  /// AAFRESULT_OBJECT_NOT_FOUND
  ///   - the given generation was not found..
  //
  STDMETHOD (LookupIdentification) (
    // Unique Generation ID
    /*[in, ref]*/ aafUID_constref  generation,

    // Indentification Object
    /*[out,retval]*/ IAAFIdentification ** ppIdentification);


  //***********************************************************
  //
  // CountIdentifications()
  //
  // Writes the number of identification objects into the *pResult
  /// argument.
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
  STDMETHOD (CountIdentifications) (
    // Total number of identification objects 
    /*[out, retval]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // GetIdentifications()
  //
  // Places an enumerator for all Identifications criteria into the
  /// *ppEnum argument.  The returned enumerator is AddRef()ed before
  /// it is returned.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (GetIdentifications) (
    // Indentification Enumeration 
    /*[out,retval]*/ IEnumAAFIdentifications ** ppEnum);


  //***********************************************************
  //
  // AppendIdentification()
  //
  // Appends the given Identification class to the header.  This
  /// method does not attempt to identify duplicate identifications, so
  /// it will succeed even if an identical identification has already
  /// been appended.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pIdent pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pIdent is null.
  //
  STDMETHOD (AppendIdentification) (
    // Identification to append 
    /*[in]*/ IAAFIdentification * pIdent);


  //***********************************************************
  //
  // GetIdentificationAt()
  //
  // Retrieves the indexed identification from the header.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppIdentification pointer is valid.
  /// - index is less than the value returned by CountIdentifications().
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppIdent is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to result of
  ///     CountIdentifications().
  //
  STDMETHOD (GetIdentificationAt) (
    // Index of identification to retrieve
    /*[in]*/ aafUInt32  index,

    // Retrieved identification
    /*[out, retval]*/ IAAFIdentification ** ppIdentification);


  //***********************************************************
  //
  // GetRefImplVersion()
  //
  // Return the version of the Reference Implementation currently
  /// running on this machine, which implements these interfaces.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pVersion pointer is valid.
  /// 
  /// If this method fails nothing is written to *pVersion.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pVersion is null.
  //
  STDMETHOD (GetRefImplVersion) (
    // The Reference Implementation Version 
    /*[out, retval]*/ aafProductVersion_t *  pVersion);


  //***********************************************************
  //
  // GetFileRevision()
  //
  // Return the File Revision property.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pRevision pointer is valid.
  /// 
  /// If this method fails nothing is written to *pRevision.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pRevision is null.
  //
  STDMETHOD (GetFileRevision) (
    // The File Version 
    /*[out, retval]*/ aafVersionType_t *  pRevision);


  //***********************************************************
  //
  // GetLastModified()
  //
  // Return the Last Modified property.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pTimeStamp pointer is valid.
  /// 
  /// If this method fails nothing is written to *pTimeStamp.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTimeStamp is null.
  //
  STDMETHOD (GetLastModified) (
    // The modification date-time stamp 
    /*[out, retval]*/ aafTimeStamp_t *  pTimeStamp);


  //***********************************************************
  //
  // GetContentStorage()
  //
  // Places the Content Storage object attached to the header into the
  /// *ppStorage argument.
  ///
  /// The returned content storage object is
  /// AddRef()ed before it is returned.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppStorage pointer is valid.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppStorage is null.
  //
  STDMETHOD (GetContentStorage) (
    // Returned Content Storage object 
    /*[out]*/ IAAFContentStorage ** ppStorage);



  //***********************************************************
  // METHOD NAME: GetStoredByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetStoredByteOrder |
  // Returns the "Endian-ness" in which the current object was or will
  /// be stored.  If this is a transient object (i.e., one which has
  /// not been persisted) then it will return the native byte order of
  /// the platform on which this is running.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pOrder pointer is valid.
  /// 
  /// If this method fails nothing is written to *pOrder.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pOrder is null.
  // @end
  // 
  STDMETHOD (GetStoredByteOrder)
   (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );

  //***********************************************************
  // METHOD NAME: GetNativeByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetNativeByteOrder |
  // Returns the native "Endian-ness" of the platform on which this is
  /// running.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pOrder pointer is valid.
  /// 
  /// If this method fails nothing is written to *pOrder.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pOrder is null.
  // @end
  // 
  STDMETHOD (GetNativeByteOrder)
   (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );

  //***********************************************************
  // METHOD NAME: GetPrimaryMob()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | GetPrimaryMob |
  // Returns this file's primary mob.
  ///
  /// Succeeds if all of the following are true:
  /// - the pPrimaryMob pointer is valid.
  ///
  /// The returned object is AddRef()ed before it is returned.
  ///
  /// If this method fails no state will be changed.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPrimaryMob arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (GetPrimaryMob)
   (
    // @parm [out, retval] AAFMob | pPrimaryMob | The primary mob
    IAAFMob ** pPrimaryMob
  );

  //***********************************************************
  // METHOD NAME: SetPrimaryMob()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | SetPrimaryMob |
  // Sets this file's primary mob.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPrimaryMob arg is NULL.
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  // @end
  // 
  STDMETHOD (SetPrimaryMob)
   (
    // @parm [in] AAFMob | pPrimaryMob | The primary mob
    IAAFMob * pPrimaryMob
  );

  //***********************************************************
  // METHOD NAME: GetOperationalPattern()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | GetOperationalPattern |
  // This method returns ID of the operational pattern this
  /// file complies to.
  ///
  /// Succeeds if all of the following are true:
  /// - the pOperationalPatternID pointer is valid.
  /// - the OperationalPattern property is present
  ///
  /// If this method fails nothing will be written to *pOperationalPatternID.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pOperationalPatternID arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (GetOperationalPattern)
   (
    // @parm [out] aafUID_t * | pOperationalPatternID | Operational pattern ID.
    aafUID_t *  pOperationalPatternID
  );

  //***********************************************************
  // METHOD NAME: SetOperationalPattern()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | SetOperationalPattern |
  // Sets operational pattern this file complies to.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  // @end
  // 
  STDMETHOD (SetOperationalPattern)
   (
    // @parm [in] aafUID_constref | operationalPatternID | Operational pattern ID.
    aafUID_constref  operationalPatternID
  );

  //***********************************************************
  // METHOD NAME: UpdateEssenceContainers()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | UpdateEssenceContainers |
  // Ensures that the contents of the EssenceContainers property
  /// is in sync with the file's metadata. If this method succeeds
  /// the property will contain IDs of all ContainerDefinitions referenced
  /// by source mobs in this file.
  /// If the property isn't present it will be created.
  /// This method must be called before any other EssenceContainers
  /// method can be called.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  STDMETHOD (UpdateEssenceContainers)
    ();

  //***********************************************************
  // METHOD NAME: CountEssenceContainers()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | CountEssenceContainers |
  // Gets the total number of essence containers present in the file.
  ///
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid.
  /// - the EssenceContainers property is present
  ///
  /// If this method fails nothing will be written to *pCount.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (CountEssenceContainers)
   (
    // @parm [out, retval] aafUInt32* | pCount | Number of essence containers
    aafUInt32*  pCount
  );

  //***********************************************************
  // METHOD NAME: GetEssenceContainers()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | GetEssenceContainers |
  // Gets IDs of essence containers present in the file.
  ///
  /// The values are written to the array specified by pEssenceContainerIDs,
  /// which is of size maxEssenceContainersCount. The required size may be found
  /// by calling CountEssenceContainers().
  /// 
  /// Succeeds if all of the following are true:
  /// - pEssenceContainerIDs is a valid pointer.
  /// - maxEssenceContainersCount indicates the array is large enough to hold the
  ///   data.
  /// - the EssenceContainers property is present
  /// 
  /// If this method fails, the property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceContainerIDs is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - maxEssenceContainersCount indicates that the array is too small to hold
  ///     the data.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (GetEssenceContainers)
   (
    // @parm [in] aafUInt32 | maxEssenceContainersCount | The number of elements in the array
    aafUInt32  maxEssenceContainersCount,

    // @parm [out, size_is(maxEssenceContainersCount)] aafUID_t * | pEssenceContainerIDs | Array to hold the essence container IDs
    aafUID_t *  pEssenceContainerIDs
  );

  //***********************************************************
  // METHOD NAME: IsEssenceContainerPresent()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | IsEssenceContainerPresent |
  // Returns true if the essence container is present.
  ///
  /// Succeeds if all of the following are true:
  /// - the pIsPresent pointer is valid.
  /// - the EssenceContainers property is present
  ///
  /// If this method fails nothing will be written to *pIsPresent.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pIsPresent arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (IsEssenceContainerPresent)
   (
    // @parm [in, ref] aafUID_constref | essenceContainerID | Essence container ID
    aafUID_constref  essenceContainerID,

    // @parm [out,retval] aafBoolean_t* | pIsPresent | Is this essence container present
    aafBoolean_t*  pIsPresent
  );

  //***********************************************************
  // METHOD NAME: CountDescriptiveSchemes()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | CountDescriptiveSchemes |
  // Gets the total number of descriptive schemes present in the file.
  ///
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid.
  /// - the DescriptiveSchemes property is present
  ///
  /// If this method fails nothing will be written to *pCount.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (CountDescriptiveSchemes)
   (
    // @parm [out, retval] aafUInt32* | pCount | Number of descriptive schemes
    aafUInt32*  pCount
  );

  //***********************************************************
  // METHOD NAME: GetDescriptiveSchemes()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | GetDescriptiveSchemes |
  // Gets IDs of descriptive schemes present in the file.
  ///
  /// The values are written to the array specified by pDescriptiveSchemeIDs,
  /// which is of size maxDescriptiveSchemesCount. The required size may be found
  /// by calling CountDescriptiveSchemes().
  /// 
  /// Succeeds if all of the following are true:
  /// - pDescriptiveSchemeIDs is a valid pointer.
  /// - maxDescriptiveSchemesCount indicates the array is large enough to hold the
  ///   data.
  /// - the DescriptiveSchemes property is present
  /// 
  /// If this method fails, the property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptiveSchemeIDs is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - maxDescriptiveSchemesCount indicates that the array is too small to hold
  ///     the data.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (GetDescriptiveSchemes)
   (
    // @parm [in] aafUInt32 | maxDescriptiveSchemesCount | The number of elements in the array
    aafUInt32  maxDescriptiveSchemesCount,

    // @parm [out, size_is(maxDescriptiveSchemesCount)] aafUID_t * | pDescriptiveSchemeIDs | Array to hold the descriptive scheme IDs
    aafUID_t *  pDescriptiveSchemeIDs
  );

  //***********************************************************
  // METHOD NAME: IsDescriptiveSchemePresent()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | IsDescriptiveSchemePresent |
  // Returns true if the descriptive scheme ID is present.
  ///
  ///
  /// Succeeds if all of the following are true:
  /// - the pIsPresent pointer is valid;
  /// - the DescriptiveSchemes property is present
  ///
  /// If this method fails nothing will be written to *pIsPresent.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pIsPresent arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  // @end
  // 
  STDMETHOD (IsDescriptiveSchemePresent)
   (
    // @parm [in, ref] aafUID_constref | descriptiveSchemeID | Descriptive scheme ID
    aafUID_constref  descriptiveSchemeID,

    // @parm [out,retval] aafBoolean_t* | pIsPresent | Is this descriptive scheme ID present
    aafBoolean_t*  pIsPresent
  );

  //***********************************************************
  // METHOD NAME: AddDescriptiveScheme()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | AddDescriptiveScheme |
  // Appends the given descriptive scheme ID to the list of
  /// descriptive schemes found in the file.
  ///
  /// Succeeds if all of the following are true:
  /// - the given descriptive scheme ID is not already contained.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given descriptive scheme ID is already contained.
  // @end
  // 
  STDMETHOD (AddDescriptiveScheme)
   (
    // @parm [in] aafUID_constref | descriptiveSchemeID | New descriptive scheme ID.
    aafUID_constref  descriptiveSchemeID
  );

  //***********************************************************
  // METHOD NAME: RemoveDescriptiveScheme()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFHeader2 | RemoveDescriptiveScheme |
  // Removes the given descriptive scheme ID from
  /// the list of descriptive schemes found in the file.
  ///
  /// Succeeds if all of the following are true:
  /// - the DescriptiveSchemes property is present;
  /// - the given descriptive scheme ID is present in the list
  ///   of descriptive schemes found in the file.
  ///
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - property not present.
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - The given descriptive scheme ID is present in the list
  ///     of descriptive schemes found in the file.
  // @end
  // 
  STDMETHOD (RemoveDescriptiveScheme)
   (
    // @parm [in] aafUID_constref | descriptiveSchemeID | Descriptive scheme to remove.
    aafUID_constref  descriptiveSchemeID
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

#endif // ! __CAAFHeader_h__

