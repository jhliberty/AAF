//@doc
//@class    AAFEvent | Implementation class for AAFEvent
#ifndef __ImplAAFEvent_h__
#define __ImplAAFEvent_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFEvent.h,v 1.12 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif
#include "OMWideStringProperty.h"


class ImplAAFEvent : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEvent ();

protected:
  virtual ~ImplAAFEvent ();

public:


  //****************
  // GetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
        // @parm [out,retval] Position property value
        (aafPosition_t *  pPosition);

  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition
        // @parm [in] Position property value
        (aafPosition_t  Position);


  //****************
  // SetComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetComment
        // @parm [in, string] buffer from which Comment is to be read
        (const aafCharacter * pComment);


  //****************
  // GetComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComment
        (// @parm [out, string, size_is(bufSize)] buffer into which Comment is to be written
         wchar_t *  pComment,

         // @parm [in] size of *pComment buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetCommentBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCommentBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

protected:
  OMFixedSizeProperty<aafPosition_t> _position;
  OMWideStringProperty _comment;
};

#endif // ! __ImplAAFEvent_h__


