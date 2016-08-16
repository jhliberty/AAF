//@doc
//@class    AAFRGBADescriptor | Implementation class for AAFRGBADescriptor
#ifndef __ImplAAFRGBADescriptor_h__
#define __ImplAAFRGBADescriptor_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFRGBADescriptor.h,v 1.8 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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











#ifndef __ImplAAFDigitalImageDescriptor_h__
#include "ImplAAFDigitalImageDescriptor.h"
#endif

#include "OMVariableSizeProperty.h"

typedef struct
{
	aafRGBAComponent_t	comps[MAX_NUM_RGBA_COMPS];
} RGBComponentArray;

class ImplAAFRGBADescriptor : public ImplAAFDigitalImageDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRGBADescriptor ();

protected:
  virtual ~ImplAAFRGBADescriptor ();

public:


  //****************
  // SetPixelLayoutStructure()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPixelLayout
        (
		aafInt32	numberElements,

         // @parm [in] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PixelLayoutArray);

   virtual AAFRESULT  STDMETHODCALLTYPE
		CountPixelLayoutElements (aafUInt32 *  pResult);

   //****************
  // GetPixelLayoutStructure()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPixelLayout
        (// @parm [in] The number of elements in each array
         aafUInt32  numberElements,

         // @parm [out, size_is(numberElements)] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PixelLayoutArray);

  //****************
  // SetPalette()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPalette
        (// @parm [in] The number of bytes in the array
         aafUInt32  numberElements,

         // @parm [in, size_is(numberElements)] Array to hold the palette information
         aafUInt8*  pPalette);


  //****************
  // GetPalette()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPalette
        (// @parm [in] The number of bytes in the array
         aafUInt32  numberBytes,

         // @parm [out, size_is(numberElements)] Array to hold the palette information
         aafUInt8 *  pPalette);


  //****************
  // GetPaletteSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPaletteSize
        // @parm [out] The number of bytes in the array
        (aafUInt32 *  pNumberBytes);

  //****************
  // SetPaletteLayoute()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPaletteLayout
        (aafInt32	numberElements,

         // @parm [in] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PaletteLayoutArray);

   virtual AAFRESULT  STDMETHODCALLTYPE
		CountPaletteLayoutElements (aafUInt32 *  pResult);

 //****************
  // GetPaletteLayout()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPaletteLayout
        (// @parm [in] The number of elements in each array
         aafUInt32  numberElements,

         // @parm [out, size_is(numberElements)] Array of up to 8 aafRGBAComponent_t indicating component type and size.
         aafRGBAComponent_t*  PaletteLayoutArray);

  //****************
  // SetComponentMaxRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetComponentMaxRef
        // @parm [in] Optional.
        (aafUInt32  componentMaxRef);


  //****************
  // GetComponentMaxRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComponentMaxRef
        // @parm [out] Optional.
        (aafUInt32 *  pComponentMaxRef);


  //****************
  // SetComponentMinRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetComponentMinRef
        // @parm [in] Optional.
        (aafUInt32  componentMinRef);


  //****************
  // GetComponentMinRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComponentMinRef
        // @parm [out] Optional.
        (aafUInt32 *  pComponentMinRef);


  //****************
  // SetAlphaMaxRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAlphaMaxRef
        // @parm [in] Optional.
        (aafUInt32  alphaMaxRef);


  //****************
  // GetAlphaMaxRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAlphaMaxRef
        // @parm [out] Optional.
        (aafUInt32 *  pAlphaMaxRef);


  //****************
  // SetAlphaMinRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAlphaMinRef
        // @parm [in] Optional.
        (aafUInt32  alphaMinRef);


  //****************
  // GetAlphaMinRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAlphaMinRef
        // @parm [out] Optional.
        (aafUInt32 *  pAlphaMinRef);

  //****************
  // SetScanningDirection()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetScanningDirection
        // @parm [in] Optional
        (aafScanningDirection_t  scanningDirection);


  //****************
  // GetScanningDirection()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetScanningDirection
        // @parm [out] Optional.
        (aafScanningDirection_t *  pScanningDirection);

protected:
	OMFixedSizeProperty<RGBComponentArray>	_pixelLayout;
	OMVariableSizeProperty<aafUInt8>		_palette;
	OMFixedSizeProperty<RGBComponentArray>	_paletteLayout;
	OMFixedSizeProperty<aafUInt32>	_componentMaxRef;
	OMFixedSizeProperty<aafUInt32>	_componentMinRef;
	OMFixedSizeProperty<aafUInt32>	_alphaMaxRef;
	OMFixedSizeProperty<aafUInt32>	_alphaMinRef;
	OMFixedSizeProperty<aafScanningDirection_t>	_scanningDirection;
};

#endif // ! __ImplAAFRGBADescriptor_h__


