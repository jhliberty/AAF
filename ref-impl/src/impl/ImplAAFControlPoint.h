//@doc
//@class    AAFControlPoint | Implementation class for AAFControlPoint
#ifndef __ImplAAFControlPoint_h__
#define __ImplAAFControlPoint_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFControlPoint.h,v 1.16 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


class ImplAAFDataDef;






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMVariableSizeProperty.h"

class ImplAAFVaryingValue;
class ImplAAFTypeDef;

class ImplAAFControlPoint : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFControlPoint ();

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] // Varying value for this object (this determines the type of the constant value)
         ImplAAFVaryingValue * pVaryingValue,

         // @parm [in] Control Point time
         aafRational_constref  time,
         
         // @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [in, size_is(valueSize)] buffer containing value
         aafDataBuffer_t  pValue);

protected:
  virtual ~ImplAAFControlPoint ();

public:


  //****************
  // GetTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTime
        // @parm [out,retval] Pointer to an aafRational_t
        (aafRational_t *  pTime);

  //****************
  // GetEditHint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEditHint
        // @parm [out,retval] Pointer to an aafEditHint_t
        (aafEditHint_t *  pEditHint);

  //****************
  // GetValueBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValueBufLen
        // @parm [out] Pointer to an variable used to return the length
        (aafUInt32 *  pLen);

  //****************
  // GetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValue
        (// @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  pValue,

         // @parm [out] Number of actual bytes read
         aafUInt32*  bytesRead);

  //****************
  // SetTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTime
        // @parm [in] Control Point time
        (aafRational_t  pTime);
			// Sets the position of the control point within an effect, expressed as a rational
  // running from 0 to 1.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\\\, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  \\\(This is the only code indicating success.\\\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\\\(\\\) called on it.)

  //****************
  // SetEditHint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEditHint
        // @parm [in] Control Point Edit hint
        (aafEditHint_t  editHint);

  //****************
  // SetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetValue
        (// @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [in, size_is(valueSize)] buffer containing value
         aafDataBuffer_t  pValue);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef);

private:
	OMFixedSizeProperty<aafRational_t>	_time;
	OMVariableSizeProperty<aafUInt8>	_value;
	OMFixedSizeProperty<aafEditHint_t>	_hint;

  bool _initialized;
  ImplAAFTypeDef * _cachedTypeDef;
};

#endif // ! __ImplAAFControlPoint_h__



