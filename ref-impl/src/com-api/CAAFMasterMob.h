//@doc
//@class    AAFMasterMob | Implementation class for AAFMasterMob
#ifndef __CAAFMasterMob_h__
#define __CAAFMasterMob_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFMasterMob.h,v 1.12 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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











#ifndef __CAAFMob_h__
#include "CAAFMob.h"
#endif


//
// Forward declaration
//
class ImplAAFMasterMob;


class CAAFMasterMob
  : public IAAFMasterMob,
    public IAAFSearchSource,
    public IAAFMasterMobEx,
    public IAAFMasterMob2,
    public IAAFMasterMob3,
    public CAAFMob
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFMasterMob (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFMasterMob ();

public:

  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty IAAFMasterMob-supporting
  /// object.  This method must be called after allocation, and before
  /// any other method can be called.
  ///
  /// Succeeds if:
  /// - Initialize() has not yet been called on this object.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ALREADY_INITIALIZED
  ///   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // AddMasterSlot()
  //
  // This function adds a slot to the specified Master Mob that
  /// references the specified a slot in the specified Source Mob. The
  /// new slot in the Master Mob contains a Source Clip that specifies
  /// the Source Mob in its source reference properties.  Typically this
  /// is done automatically by passing the Master Mob handle to
  /// AAFMedia::Create, but this function allows you to add it later.
  ///
  /// Note: If pSlotName is passed in with zero length, then the
  /// slot is not assigned a name.  Slot names are not used by the
  /// SDK, and exist only so the user can name slots.
  /// 
  /// Succeeds if all of the following are true:
  /// (more conditions here)
  /// 
  /// If this method fails no state is changed.
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
  ///   - One or more of the following parameters are NULL pSourceMob,
  ///     pSlotName, and pDataDef.
  ///
  /// AAFRESULT_INVALID_DATADEF
  ///   - The data kind of the source MOB slot to be added to the Master
  ///     Mob does not match what is specfied in pDataDef.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The specified Source Mob slot was not found.
  ///
  /// AAFRESULT_SLOT_EXISTS
  ///   - The specified Master slot ID already exists.
  //
  STDMETHOD (AddMasterSlot) (
    // Data kind of new slot.  Requires a data kind valid for a media
  /// stream. Valid data kinds are:
  /// - Picture
  /// - Sound
    /*[in]*/ IAAFDataDef * pDataDef,

    // Slot ID of the Source Mob slot to be added to the Master Mob
    /*[in]*/ aafSlotID_t  sourceSlotID,

    // Source Mob containing the slot to be added to the Master Mob
    /*[in]*/ IAAFSourceMob * pSourceMob,

    // SlotID assigned to the new Master Mob slot
    /*[in]*/ aafSlotID_t  masterSlotID,

    // Name to assign to new slot in Master Mob
    /*[in, string]*/ aafCharacter_constptr  pSlotName);


  //***********************************************************
  //
  // GetTapeName()
  //
  // Finds the tape Source Mob associated with a Master Mob slot
  /// and writes the name of the tape, which is stored in the
  /// Mobs Name property, into the pTapeName buffer.  The buffer is
  /// allocated by the caller.  The size of the buffer is given by
  /// bufSize.  If the property name has not yet been set, a
  /// zero-length string will be written (that is, only the trailing
  /// null character).
  /// 
  /// Caller may call GetTapeNameBufLen() to determine the required
  /// buffer size.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pTapeName pointer is valid.
  /// - the specified master slot was found.
  /// - the specified master slot contains a tape mob.
  /// - bufSize indicates the buffer is large enough to hold the name.
  /// 
  /// If this method fails nothing will be written to *pTapeName.
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
  ///   - pTapeName arg is NULL.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The specified Master Slot was not found.
  ///
  /// AAFRESULT_NOT_TAPEMOB
  ///   - The specified Master Slot does not contain a Tape MOB.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - bufSize indicates the buffer is too small to hold the string.
  //
  STDMETHOD (GetTapeName) (
    // SlotID of the Master Mob slot
    /*[in]*/ aafUInt32  masterSlotID,

    // The returned name
    /*[out, size_is(bufSize), string]*/ aafCharacter *  pTapeName,

    // the size of the pTapeName buffer
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetTapeNameBufLen()
  //
  // Returns the length of buffer required for the GetTapeName()
  /// method.  The value is placed into the location specified by
  /// pLen.  The value will include space required for the trailing
  /// null character.
  ///
  /// Succeeds if all of the following are true:
  /// - the pLen pointer is valid.
  ///
  /// If this method fails nothing will be written to *pLen.
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
  ///   - pLen arg is NULL.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The specified Master Slot was not found.
  ///
  /// AAFRESULT_NOT_TAPEMOB
  ///   - The specified Master Slot does not contain a Tape MOB.
  //
  STDMETHOD (GetTapeNameBufLen) (
    // SlotID of the Master Mob slot
    /*[in]*/ aafUInt32  masterSlotID,

    // required buffer length
    /*[out]*/ aafUInt32 *  pLen);


  //***********************************************************
  //
  // GetNumRepresentations()
  //
  // This function returns the number of media representations
  /// available for the specified SlotID on a specified Master
  /// Mob. This function is meant to work with
  /// GetRepresentationSourceClip, so that you can iterate through
  /// all of the choices yourself.  In most cases, you can use
  /// GetCriteriaSourceClip to handle multiple
  /// representations.  This function and
  /// GetRepresentationSourceClip are lower-level functions.
  ///
  /// Succeeds if all of the following are true:
  /// - the pNumReps pointer is valid.
  ///
  /// If this method fails nothing will be written to *pNumReps.
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
  ///   - pNumReps arg is NULL.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The Master Slot specified by slotID was not found.
  //
  STDMETHOD (GetNumRepresentations) (
    // SlotID
    /*[in]*/ aafSlotID_t  slotID,

    // number of representations
    /*[out, retval]*/ aafNumSlots_t *  pNumReps);


  //***********************************************************
  //
  // GetRepresentation()
  //
  // This method returns the indexed media representation for the
  /// specified Master Mob, SlotID, and index.  This call is meant to
  /// work with GetNumRepresentations, so that you can iterate through
  /// all of the choices yourself.  This method uses an integer index,
  /// not an iterator.  The function GetRepresentationSourceClip takes
  /// an index between 1 and the number of representations
  /// [inclusive], and returns the indexed Source Mob. You can make
  /// calls to functions such as AAFMedia::GetVideoInfo and
  /// AAFMedia::IsMediaContiguous to determine which media is the best
  /// fit.
  ///
  /// The returned source clip is AddRef()ed before it is returned.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppSourceClip pointer is valid.
  ///
  /// If this method fails nothing will be written to *ppSourceClip.
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
  ///   - ppSourceClip arg is NULL.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The specified Master Slot was not found.
  ///
  /// AAFRESULT_BADINDEX
  ///   - No Source Mob at specified index.
  //
  STDMETHOD (GetRepresentation) (
    // Slot ID
    /*[in]*/ aafSlotID_t  slotID,

    // Index of requested representation
    /*[in]*/ aafUInt32  index,

    // Requested Source Clip
    /*[out]*/ IAAFSegment ** ppSourceClip);


  //***********************************************************
  //
  // GetCriteriaSegment()
  //
  // Returns the Segment on the specified slot of a Master Mob
  /// that references the Source Mob that best meets the specified
  /// criteria.  This function will work whether multiple media
  /// representations exist or not.
  ///
  /// The returned segment is AddRef()ed before it is returned.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppSegment pointer is valid.
  ///
  /// If this method fails nothing will be written to *ppSegment.
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
  ///   - ppSegment arg is NULL.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The specified Master Slot was not found.
  //
  STDMETHOD (GetCriteriaSegment) (
    // Slot ID
    /*[in]*/ aafSlotID_t  slotID,

    // Index of requested representation.  Note: the
  /// aafMediaCriteria_t is defined as the following structure:
  /// typedef struct
  /// {
  ///	aafCriteriaType_t type;
  ///	aafCriteriaProc_t proc;
  /// } aafMediaCriteria_t;
  ///
  /// The type field can have one of the following values:
  /// typedef enum
  /// {
  ///	kAAFAnyRepresentation = 0,
  ///	kAAFFastestRepresentation,
  ///	kAAFBestFidelityRepresentation,
  ///	kAAFSmallestRepresentation,
  ///	kAAFUseRepresentationProc
  /// } aafCriteriaType_t;
    /*[in]*/ aafMediaCriteria_t *  pCriteria,

    // Requested Segment
    /*[out]*/ IAAFSegment ** ppSegment);


  //***********************************************************
  //
  // AppendPhysSourceRef()
  //
  // Connects this Source Mob with the physical Source Mob that
  /// describes the previous generation of essence, appending it to
  /// existing Mob data.  If a physical Source Mob, such as a File
  /// Source Mob or tape Source Mob, references another physical
  /// Source Mob as its ancestor, with no pulldown, then this
  /// function makes the connection between the two.
  ///
  /// Functionally, this is a helper function to create a slot with an
  /// AAFSourceClip referencing a particular piece of media.  This
  /// function takes many parameters because the components of an
  /// aafSourceRef_t have been broken out as separate parameters.
  ///
  /// The ancestor of an AAFSourceMob with an AAFFileDescriptor is often an
  /// AAFTapeDescriptor or NIL.
  ///
  /// Succeeds if all of the following are true:
  /// - the pSourceRefObj pointer is valid.
  /// - the pEssenceKind pointer is valid.
  /// (other conditions here)
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
  ///   - pSourceRefObj or pEssenceKind is null.
  ///
  /// (other codes here.)
  //
  STDMETHOD (AppendPhysSourceRef) (
    // Edit rate of slot to contain reference
    /*[in]*/ aafRational_t  editrate,

    // SlotID of slot to contain reference
    /*[in]*/ aafSlotID_t  aMobSlot,

    // Data kind of slot to contain reference.  Requires a data kind
  /// valid for a essence stream.  Valid data kinds are:
  /// - Picture
  /// - Sound
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // Reference to a Physical Source Mob
    /*[in]*/ aafSourceRef_t  ref,

    // Length of the Source Clip
    /*[in]*/ aafLength_t  srcRefLength);


  //***********************************************************
  //
  // NewPhysSourceRef()
  //
  // Connects this Source Mob with the physical Source Mob that
  /// describes the previous generation of essence, replacing any
  /// existing Mob data.  If a physical Source Mob, such as a File
  /// Source Mob or tape Source Mob, references another physical
  /// Source Mob as its ancestor, with no pulldown, then this
  /// function makes the connection between the two.
  ///
  /// Functionally, this is a helper function to create a slot with an
  /// AAFSourceClip referencing a particular piece of media.  This
  /// function takes many parameters because the components of an
  /// aafSourceRef_t have been broken out as separate parameters.
  ///
  /// The ancestor of an AAFSourceMob with an AAFFileDescriptor is often an
  /// AAFTapeDescriptor or NIL.
  ///
  /// Succeeds if all of the following are true:
  /// - the pSourceRefObj pointer is valid.
  /// - the pEssenceKind pointer is valid.
  /// (other conditions here)
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
  ///   - pSourceRefObj or pEssenceKind is null.
  ///
  /// (other codes here.)
  //
  STDMETHOD (NewPhysSourceRef) (
    // Edit rate of slot to contain reference
    /*[in]*/ aafRational_t  editrate,

    // SlotID of slot to contain reference
    /*[in]*/ aafSlotID_t  aMobSlot,

    // Data kind of slot to contain reference.  Requires a data kind
  /// valid for a essence stream.  Valid data kinds are:
  /// - Picture
  /// - Sound
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // Reference to a Physical Source Mob
    /*[in]*/ aafSourceRef_t  ref,

    // Length of the Source Clip
    /*[in]*/ aafLength_t  srcRefLength);


  //***********************************************************
  //
  // CreateEssence()
  //
  // Creates a single channel stream of essence.  Convenience
  /// functions exist to create audio or video essence, and a separate
  /// call (MultiCreate) exists to create interleaved audio and video
  /// data.
  ///
  /// The essence handle from this call can be used with
  /// WriteDataSamples  and possibly WriteDataLines, but NOT with
  /// WriteMultiSamples.
  /// 
  /// If you are creating the essence, and then attaching it to a
  /// master mob, then the "masterMob" field may be left NULL.  For
  /// video, the sampleRate should be the edit rate of the file mob.
  /// For audio, the sample rate should be the actual samples per
  /// second.
  //
  STDMETHOD (CreateEssence) (
    // 
    /*[in]*/ aafSlotID_t  masterSlotID,

    // create essence of this type
    /*[in]*/ IAAFDataDef * pMediaKind,

    // using this codec
    /*[in, ref]*/ aafUID_constref  codecID,

    // with this edit rate
    /*[in]*/ aafRational_t  editRate,

    // with this sample rate
    /*[in]*/ aafRational_t  samplerate,

    // optionally compressing it
    /*[in]*/ aafCompressEnable_t  Enable,

    // Optionally create the file HERE.
    /*[in]*/ IAAFLocator * destination,

    // with this format
    /*[in, ref]*/ aafUID_constref  fileFormat,

    // Return an essence access on the essence.
    /*[out]*/ IAAFEssenceAccess ** access);


  //***********************************************************
  //
  // CreateMultiEssence()
  //
  // Creates a multi-channel interleaved stream of essence.  The
  /// essence handle from this call can be used with WriteDataSamples
  /// or WriteMultiSamples but NOT with or WriteDataLines.
  /// 
  /// If you are creating the essence, and then attaching it to a
  /// master mob, then the "masterMob" field may be left NULL.
  //
  STDMETHOD (CreateMultiEssence) (
    // using this codec
    /*[in, ref]*/ aafUID_constref  codecID,

    // this many channels
    /*[in]*/ aafUInt16  arrayElemCount,

    // using these definitions
    /*[in,ref,size_is(arrayElemCount)]*/ aafmMultiCreate_t *  mediaArray,

    // optionally compressing it
    /*[in]*/ aafCompressEnable_t  Enable,

    // Optionally create the file HERE.
    /*[in]*/ IAAFLocator * destination,

    // with this format
    /*[in, ref]*/ aafUID_constref  fileFormat,

    // Return an essence access on the essence.
    /*[out]*/ IAAFEssenceMultiAccess**  access);


  //***********************************************************
  //
  // OpenEssence()
  //
  // Opens a single channel of a file mob.  If the essence is
  /// interleaved, then it will be di-interleaved when samples are
  /// read.  This routine follows the locator, and may call the locator
  /// failure callback if the essence can not be found.  If the failure
  /// callback finds the essence, then this routine will return
  /// normally.
  /// 
  /// The essence handle from this call can be used with
  /// ReadDataSamples  and possibly ReadDataLines, but NOT with
  /// ReadMultiSamples.
  /// 
  /// NOTE: If a locator is followed, then essencePtr may reference
  /// ANOTHER file object, which must be closed on file close.
  //
  STDMETHOD (OpenEssence) (
    // On this slot
    /*[in]*/ aafSlotID_t  slotID,

    // using this essence criteria
    /*[in]*/ aafMediaCriteria_t*  mediaCrit,

    // ReadOnly or Append
    /*[in]*/ aafMediaOpenMode_t  openMode,

    // optionally decompressing
    /*[in]*/ aafCompressEnable_t  compEnable,

    // Return an essence access on the essence.
    /*[out]*/ IAAFEssenceAccess ** access);
	

  //***********************************************************
  //
  // OpenMultiEssence()
  //
  // Opens a all channels associated with a file mob.  This routine
  /// follows the locator, and may call the locator failure callback if
  /// the essence can not be found.  If the failure callback finds the
  /// essence, then this routine will return normally.
  ///
  /// The essence handle from this call can be used with
  /// WriteMultiSamples but NOT with WriteDataSamples.
  //
  STDMETHOD (OpenMultiEssence) (
    // On this slot
    /*[in]*/ aafSlotID_t  slotID,

    // using this essence criteria
    /*[in]*/ aafMediaCriteria_t*  mediaCrit,

    // ReadOnly or Append
    /*[in]*/ aafMediaOpenMode_t  openMode,

    // optionally decompressing
    /*[in]*/ aafCompressEnable_t  compEnable,

    // Return an essence access on the essence.
    /*[out]*/ IAAFEssenceMultiAccess**  access);


  //***********************************************************
  //
  // CountChannels()
  //
  // Takes an opaque handle, a master mob reference, and a slot ID
  /// so that it may be called before the essence is opened.
  ///
  /// Returns the number of interleaved essence channels of a given
  /// type in the essence stream referenced by the given file mob.
  ///
  /// If the data format is not interleaved, then the answer will
  /// always be zero or one.  This function correctly returns zero for
  /// essence types not handled by a given codec, and handles codecs
  /// which work with multiple essence types.
  //
  STDMETHOD (CountChannels) (
    // On this slot
    /*[in]*/ aafSlotID_t  slotID,

    // using this essence criteria
    /*[in]*/ aafMediaCriteria_t*  mediaCrit,

    // for this essence type
    /*[in]*/ IAAFDataDef * pMediaKind,

    // How many channels?
    /*[out]*/ aafUInt16*  numCh);


  //***********************************************************
  // METHOD NAME: SearchSource()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSearchSource | SearchSource |
  // This function returns the source information for a slot in a
  /// Master Mob or Source Mob.  It follows the Source Clip references
  /// in the specified slot until it encounters the kind of Mob
  /// specified in the mobKind parameter.  This function cannot be used
  /// on a Composition Mob and is not intended to be called
  /// iteratively; use the MobOpenSearch, MobGetNextSource,
  /// MobGetThisSource, and MobCloseSearch functions for those
  /// purposes. 
  ///
  /// The returned component and find source info are AddRef()ed
  /// before they are returned.
  ///
  /// Succeeds if all of the following are true:
  /// - ppSourceInfo is non-NULL
  /// - a Mob of the requested kind is found
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppCpnt is null.
  ///
  /// OM_ERR_INVALID_MOBTYPE
  ///	- The enumerator is out of range (bad cast, or writing
  ///      toolkit newer than reader)
  ///
  /// OM_ERR_TRAVERSAL_NOT_POSS
  ///	- Can not find a mob of the given kind.
  // @end
  // 
  STDMETHOD (SearchSource)
   (
    // @parm [in] aafSlotID_t | slotID | Slot ID
    aafSlotID_t  slotID,

    // @parm [in] aafPosition_t | offset | Offset
    aafPosition_t  offset,

    // @parm [in] aafMobKind_t | mobKind | Mob Kind
    aafMobKind_t  mobKind,

    // @parm [in] aafMediaCriteria_t * | pMediaCrit | Media Criteria
    aafMediaCriteria_t *  pMediaCrit,

    // @parm [in] aafOperationChoice_t * | pOperationChoice | Operation Choice
    aafOperationChoice_t *  pOperationChoice,

    // @parm [out] AAFFindSourceInfo | ppSourceInfo | Source Information
    IAAFFindSourceInfo ** ppSourceInfo
  );


  //***********************************************************
  // METHOD NAME: ExtendEssence()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMasterMobEx | ExtendEssence |
  // Extends a single stream of essence that was originally created using
  /// IAAFMasterMob::CreateEssence.  Extended essence is represented by
  /// a Sequence of SourceClip objects.  The first call to ExtendEssence will cause the
  /// TimelineMobSlot's SourceClip object to be replaced by a
  /// Sequence.  The initial SourceClip becomes the first
  /// component of the new Sequence.
  // @end
  // 
  STDMETHOD (ExtendEssence)
   (
    // @parm [in] aafSlotID_t | masterSlotID | 
    aafSlotID_t  masterSlotID,

    // @parm [in] AAFDataDef | pMediaKind | create essence of this type
    IAAFDataDef * pMediaKind,

    // @parm [in, ref] aafUID_constref | codecID | using this codec
    aafUID_constref  codecID,

    // @parm [in] aafRational_t | editRate | with this edit rate
    aafRational_t  editRate,

    // @parm [in] aafRational_t | samplerate | with this sample rate
    aafRational_t  samplerate,

    // @parm [in] aafCompressEnable_t | Enable | optionally compressing it
    aafCompressEnable_t  Enable,

    // @parm [in] AAFLocator | destination | Optionally create the file HERE.
    IAAFLocator * destination,

    // @parm [in, ref] aafUID_constref | fileFormat | with this format
    aafUID_constref  fileFormat,

    // @parm [out] AAFEssenceAccess | access | Return an essence access on the essence.
    IAAFEssenceAccess ** access
  );

  //***********************************************************
  // METHOD NAME: ExtendMultiEssence()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMasterMobEx | ExtendMultiEssence |
  // Extends a multi-channel interleaved stream of essence that was
  /// originally created using IAAFMasterMob::CreateMultiEssence.
  /// Extended essence is represented by a Sequence of SourceClip objects.
  /// The first call to ExtendEssence will cause the TimelineMobSlot's SourceClip
  /// object to be replaced by a Sequence.  The initial SourceClip becomes the first
  /// component of the new Sequence.
  // @end
  // 
  STDMETHOD (ExtendMultiEssence)
   (
    // @parm [in, ref] aafUID_constref | codecID | using this codec
    aafUID_constref  codecID,

    // @parm [in] aafUInt16 | arrayElemCount | this many channels
    aafUInt16  arrayElemCount,

    // @parm [in,ref,size_is(arrayElemCount)] aafmMultiCreate_t * | mediaArray | using these definitions
    aafmMultiCreate_t *  mediaArray,

    // @parm [in] aafCompressEnable_t | Enable | optionally compressing it
    aafCompressEnable_t  Enable,

    // @parm [in] AAFLocator | destination | Optionally create the file HERE.
    IAAFLocator * destination,

    // @parm [in, ref] aafUID_constref | fileFormat | with this format
    aafUID_constref  fileFormat,

    // @parm [out] IAAFEssenceMultiAccess** | access | Return an essence access on the essence.
    IAAFEssenceMultiAccess**  access
  );


  //***********************************************************
  // METHOD NAME: CreateStaticEssence()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMasterMob2 | CreateStaticEssence |
  // Creates and initializes the objects required to represent static essence.
  // @end
  // 
  STDMETHOD (CreateStaticEssence)
   (
    // @parm [in] aafSlotID_t | masterSlotID | 
    aafSlotID_t  masterSlotID,

    // @parm [in] AAFDataDef | pMediaKind | create essence of this type
    IAAFDataDef * pMediaKind,

    // @parm [in, ref] aafUID_constref | codecID | using this codec
    aafUID_constref  codecID,

    // @parm [in] aafCompressEnable_t | Enable | optionally compressing it
    aafCompressEnable_t  Enable,

    // @parm [in] AAFLocator | destination | Optionally create the file HERE.
    IAAFLocator * destination,

    // @parm [in, ref] aafUID_constref | fileFormat | with this format
    aafUID_constref  fileFormat,

    // @parm [out] AAFEssenceAccess | access | Return an essence access on the essence.
    IAAFEssenceAccess ** access
  );

  //***********************************************************
  // METHOD NAME: CreateEventEssence()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMasterMob2 | CreateEventEssence |
  // Creates and initializes the objects required to represent stream of events.
  // @end
  // 
  STDMETHOD (CreateEventEssence)
   (
    // @parm [in] aafSlotID_t | masterSlotID | 
    aafSlotID_t  masterSlotID,

    // @parm [in] AAFDataDef | pMediaKind | create essence of this type
    IAAFDataDef * pMediaKind,

    // @parm [in, ref] aafUID_constref | codecID | using this codec
    aafUID_constref  codecID,

    // @parm [in] aafRational_t | editRate | with this edit rate
    aafRational_t  editRate,

    // @parm [in] aafRational_t | samplerate | with this sample rate
    aafRational_t  samplerate,

    // @parm [in] aafCompressEnable_t | Enable | optionally compressing it
    aafCompressEnable_t  Enable,

    // @parm [in] AAFLocator | destination | Optionally create the file HERE.
    IAAFLocator * destination,

    // @parm [in, ref] aafUID_constref | fileFormat | with this format
    aafUID_constref  fileFormat,

    // @parm [out] AAFEssenceAccess | access | Return an essence access on the essence.
    IAAFEssenceAccess ** access
  );


  //***********************************************************
  // METHOD NAME: AddMasterSlotWithSequence()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMasterMob3 | AddMasterSlotWithSequence |
  // This function is similar to AddMasterSlot but creates the structure
  /// MobSlot - Sequence - SourceClip instead of MobSlot - SourceClip.
  /// This arrangement is required for MXF compliance.
  ///
  /// This function adds a slot to the specified Master Mob that
  /// references the specified a slot in the specified Source Mob. The
  /// new slot in the Master Mob has a Sequence containing the Source Clip
  /// that specifies the Source Mob in its source reference properties.
  ///
  /// Note: If pSlotName is passed in with zero length, then the
  /// slot is not assigned a name.  Slot names are not used by the
  /// SDK, and exist only so the user can name slots.
  /// 
  /// Succeeds if all of the following are true:
  /// (more conditions here)
  /// 
  /// If this method fails no state is changed.
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
  ///   - One or more of the following parameters are NULL pSourceMob,
  ///     pSlotName, and pDataDef.
  ///
  /// AAFRESULT_INVALID_DATADEF
  ///   - The data kind of the source MOB slot to be added to the Master
  ///     Mob does not match what is specfied in pDataDef.
  ///
  /// AAFRESULT_SLOT_NOTFOUND
  ///   - The specified Source Mob slot was not found.
  ///
  /// AAFRESULT_SLOT_EXISTS
  ///   - The specified Master slot ID already exists.
  // @end
  // 
  STDMETHOD (AddMasterSlotWithSequence)
   (
    // @parm [in] AAFDataDef | pDataDef | Data kind of new slot.  Requires a data kind valid for a media
  /// stream. Valid data kinds are:
  /// - Picture
  /// - Sound
    IAAFDataDef * pDataDef,

    // @parm [in] aafSlotID_t | sourceSlotID | Slot ID of the Source Mob slot to be added to the Master Mob
    aafSlotID_t  sourceSlotID,

    // @parm [in] AAFSourceMob | pSourceMob | Source Mob containing the slot to be added to the Master Mob
    IAAFSourceMob * pSourceMob,

    // @parm [in] aafSlotID_t | masterSlotID | SlotID assigned to the new Master Mob slot
    aafSlotID_t  masterSlotID,

    // @parm [in, string] aafCharacter_constptr | pSlotName | Name to assign to new slot in Master Mob
    aafCharacter_constptr  pSlotName
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

#endif // ! __CAAFMasterMob_h__

