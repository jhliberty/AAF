#ifndef __ImplAAFSoundDescriptor_h__
#define __ImplAAFSoundDescriptor_h__

//=---------------------------------------------------------------------=
//
// $Id: ImplAAFSoundDescriptor.h,v 1.3 2009/06/01 11:47:08 stuart_hc Exp $ $Name: V116 $
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


#include "ImplAAFFileDescriptor.h"


class ImplAAFSoundDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSoundDescriptor ();

protected:
  virtual ~ImplAAFSoundDescriptor ();

public:
  //********************************
  // AAF SDK exported methods start
  //********************************

  //****************
  // SetCompression()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCompression
        // @parm [in] Identifies the compression and format of compression information
        (const aafUID_t & compression);

  //****************
  // GetCompression()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompression
        // @parm [out] Identifies the compression and format of compression information
        (aafUID_t *  pCompression);

  //****************
  // SetChannelCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetChannelCount
        // @parm [in] The number of channels of sound represented by this descriptor
        (aafUInt32 channelCount);

  //****************
  // GetChannelCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetChannelCount
        // @parm [out] The number of channels of sound represented by this descriptor
        (aafUInt32 *  pChannelCount);

  //****************
  // SetAudioSamplingRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAudioSamplingRate
        // @parm [in] Sample rate of audio essence
        (aafRational_t rate);

  //****************
  // GetAudioSamplingRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAudioSamplingRate
        // @parm [out] Sample rate of audio essence
        (aafRational_t *  pRate);

  //****************
  // SetIsLocked()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsLocked
        // @parm [in] Is number of samples per frame is locked to video?
        (aafBoolean_t locked);

  //****************
  // IsLocked()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsLocked
        // @parm [out] Is number of samples per frame is locked to video?
        (aafBoolean_t *  pLocked);

  //****************
  // SetElectroSpatialFormulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetElectroSpatialFormulation
        // @parm [in] Electro-spatial form of the signal
        (aafElectroSpatialFormulation_t formulation);

  //****************
  // GetElectroSpatialFormulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElectroSpatialFormulation
        // @parm [out] Electro-spatial form of the signal
        (aafElectroSpatialFormulation_t *  pFormulation);

  //****************
  // SetAudioRefLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAudioRefLevel
        // @parm [in] Audio reference level
        (aafInt8 level);

  //****************
  // GetAudioRefLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAudioRefLevel
        // @parm [out] Audio reference level
        (aafInt8 *  pLevel);

  //****************
  // SetDialNorm()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDialNorm
        // @parm [in] Dial norm
        (aafInt8 dialNorm);

  //****************
  // GetDialNorm()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDialNorm
        // @parm [out] Dial norm
        (aafInt8 * pDialNorm);

  //****************
  // SetQuantizationBits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQuantizationBits
        // @parm [in] Number of quantization bits
        (aafUInt32 bitsCount);

  //****************
  // GetQuantizationBits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQuantizationBits
        // @parm [out] Number of quantization bits
        (aafUInt32 *  pBitsCount);

  //********************************
  // AAF SDK exported methods start
  //********************************


protected:

  virtual AAFRESULT STDMETHODCALLTYPE Initialize();


protected:

    OMFixedSizeProperty<aafUID_t>                       _compression;
    OMFixedSizeProperty<aafUInt32>                      _channels;
    OMFixedSizeProperty<aafRational_t>                  _audioSamplingRate;
    OMFixedSizeProperty<aafBoolean_t>                   _locked;
    OMFixedSizeProperty<aafElectroSpatialFormulation_t> _electroSpatial;
    OMFixedSizeProperty<aafInt8>                        _audioRefLevel;
    OMFixedSizeProperty<aafInt8>                        _dialNorm;
    OMFixedSizeProperty<aafUInt32>                      _quantizationBits;
};

#endif // ! __ImplAAFSoundDescriptor_h__

