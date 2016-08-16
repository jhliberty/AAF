//@doc
//@class    AAFFileDescriptor | Implementation class for AAFFileDescriptor
#ifndef __CAAFFileDescriptor_h__
#define __CAAFFileDescriptor_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFFileDescriptor.h,v 1.11 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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





#ifndef __CAAFEssenceDescriptor_h__
#include "CAAFEssenceDescriptor.h"
#endif


class CAAFFileDescriptor
  : public IAAFFileDescriptor,
    public IAAFFileDescriptor2,
    public CAAFEssenceDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFFileDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFFileDescriptor ();

public:

  //***********************************************************
  //
  // SetLength()
  //
  // Sets the length of the essence in samples [not edit units].
  /// 
  /// Always succeeds.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetLength) (
    // length of the essence in samples 
    /*[in]*/ aafLength_t  length);


  //***********************************************************
  //
  // GetLength()
  //
  // Gets the length of the essence in samples [not edit units].
  ///
  /// Succeeds if all of the following are true:
  /// - the pLength pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pLength.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength arg is NULL.
  //
  STDMETHOD (GetLength) (
    // returns length of the essence in samples 
    /*[out]*/ aafLength_t *  pLength);


  //***********************************************************
  //
  // SetCodecDef()
  //
  // Set to the exact codec which was used as a hint.
  /// 
  /// Always succeeds.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetCodecDef) (
    // Which codec was used 
    /*[in]*/ IAAFCodecDef * codecDef);


  //***********************************************************
  //
  // GetCodecDef()
  //
  // Get to the exact codec which was used as a hint.
  ///
  /// Succeeds if all of the following are true:
  /// - the pCodecDef pointer is valid.
  /// - the CodecDefinition identifying the codec is
  ///   present in the dictionary.
  /// 
  /// If this method fails nothing will be written to *pCodecDef.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCodecDef arg is NULL.
  ///
  /// AAFRESULT_OBJECT_NOT_FOUND
  ///   - the CodecDefinition identifying the codec is not
  ///     in the dictionary.
  //
  STDMETHOD (GetCodecDef) (
    // Which codec was used 
    /*[out]*/ IAAFCodecDef ** pCodecDef);


  //***********************************************************
  //
  // SetSampleRate()
  //
  // Sets sample rate of the essence as opposed to the edit rate.
  /// 
  /// If this method fails the sample rate property will not be
  /// changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetSampleRate) (
    // sample rate of the essence 
    /*[in]*/ aafRational_constref  rate);


  //***********************************************************
  //
  // GetSampleRate()
  //
  // Gets sample rate of the essence as opposed to the edit rate. and
  /// writes it into the *pRate argument.
  ///
  /// Succeeds if all of the following are true:
  /// - the pRate pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pRate.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pRate arg is NULL.
  //
  STDMETHOD (GetSampleRate) (
    // sample rate of the essence 
    /*[out]*/ aafRational_t*  pRate);


  //***********************************************************
  //
  // SetContainerFormat()
  //
  // Identifies the file format.  The container format is an optional
  /// property.
  /// 
  /// If this method fails the container format property will not be
  /// changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetContainerFormat) (
    // file format 
    /*[in]*/ IAAFContainerDef * format);


  //***********************************************************
  //
  // GetContainerFormat()
  //
  // Identifies the file format.
  ///
  /// Succeeds if all of the following are true:
  /// - the pFormat pointer is valid.
  /// - the ContainerDefinition identifying the file format is
  ///   present in the dictionary.
  /// 
  /// If this method fails nothing will be written to *pFormat.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormat arg is NULL.
  ///
  /// AAFRESULT_OBJECT_NOT_FOUND
  ///   - the ContainerDefinition identifying the file format is not
  ///     in the dictionary.
  //
  STDMETHOD (GetContainerFormat) (
    // Optional 
    /*[out]*/ IAAFContainerDef ** pFormat);


  //***********************************************************
  // METHOD NAME: SetLinkedSlotID()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFFileDescriptor2 | SetLinkedSlotID |
  // Sets the LinkedSlotID property.  
  ///
  ///
  /// This property is optional.
  ///
  /// If this method fails, the LinkedSlotID property will not
  /// be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetLinkedSlotID)
   (
    // @parm [in] aafUInt32 | LinkedSlotID | the linked slot id
    aafUInt32  LinkedSlotID
  );

  //***********************************************************
  // METHOD NAME: GetLinkedSlotID()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFFileDescriptor2 | GetLinkedSlotID |
  // Gets the LinkedSlotID property. 
  ///
  ///
  /// Succeeds if all of the following are true:
  /// - pLinkedSlotID is a valid pointer.
  /// 
  /// If this method fails, *pLinkedSlotID will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLinkedSlotID arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the LinkedSlotID property is not present.
  // @end
  // 
  STDMETHOD (GetLinkedSlotID)
   (
    // @parm [out] aafUInt32 * | pLinkedSlotID | The linked slot id
    aafUInt32 *  pLinkedSlotID
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

#endif // ! __CAAFFileDescriptor_h__

