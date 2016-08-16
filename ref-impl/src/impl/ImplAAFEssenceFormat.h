//@doc
//@class    AAFEssenceFormat | Implementation class for AAFEssenceFormat
#ifndef __ImplAAFEssenceFormat_h__
#define __ImplAAFEssenceFormat_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFEssenceFormat.h,v 1.9 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

typedef struct
{
	aafUID_t		parmName;
	aafDataBuffer_t	parmValue;
	aafUInt32		valueSize;
	aafUInt32		allocSize;
} oneParm_t;

class ImplAAFEssenceFormat : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssenceFormat ();

protected:
  virtual ~ImplAAFEssenceFormat ();

public:

  //****************
  // AddFormatSpecifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddFormatSpecifier
        (// @parm [in] From aaddefuids.h
         aafUID_t  essenceFormatCode,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [in, size_is(valueSize)] Value data
         aafDataBuffer_t  value);
			//@comm The value data is passed in as a void * through the "value"
			// argument.  The size of the value must be passed through the
			// valueSize argument.	


/****/
  //****************
  // GetFormatSpecifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFormatSpecifier
        (// @parm [in] From aaddefuids.h
         aafUID_t  essenceFormatCode,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  value,

         // @parm [out] Number of actual bytes read
         aafInt32*  bytesRead);
			//@comm The actual number of bytes read is returned in
			// bytesRead.  If the buffer is not big enough to return the entire
			// value, an error is returned.
/****/
  //****************
  // NumFormatSpecifiers()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NumFormatSpecifiers
        // @parm [out] The number of specifiers present.
        (aafInt32*  numSpecifiers);
/****/
  //****************
  // GetIndexedFormatSpecifier()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIndexedFormatSpecifier
        (// @parm [in] 0-based index
         aafInt32  index,

         // @parm [out] From aaddefuids.h
         aafUID_t*  essenceFormatCode,

         // @parm [in] Size of preallocated buffer
         aafInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  value,

         // @parm [out] Number of actual bytes read
         aafInt32*  bytesRead);


private:
		oneParm_t	*_elements;
		aafUInt32	_elemUsed;
		aafUInt32	_elemAllocated;

		oneParm_t	*Lookup(aafUID_t);
};

#endif // ! __ImplAAFEssenceFormat_h__


