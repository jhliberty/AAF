//@doc
//@class    AAFMultipleDescriptor | Implementation class for AAFMultipleDescriptor
#ifndef __ImplAAFMultipleDescriptor_h__
#define __ImplAAFMultipleDescriptor_h__



//=---------------------------------------------------------------------=
//
// $Id: ImplAAFMultipleDescriptor.h,v 1.3 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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

class ImplAAFFileDescriptor;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFFileDescriptor> ImplEnumAAFDescriptors;

#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include "ImplEnumAAFFileDescriptors.h"
#include "OMStrongRefVectorProperty.h"

class ImplAAFMultipleDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMultipleDescriptor ();
  virtual ~ImplAAFMultipleDescriptor ();



   //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        ();

  //****************
  // CountFileDescriptors()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountFileDescriptors
		// @parm [out] Returns the number of Descriptors
        (aafUInt32 *  pCount);
  //@comm The number of Descriptors may be zero if the essence is in the current file.

  //****************
  // AppendFileDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendFileDescriptor
		// @parm [in] Descriptor to append
        (ImplAAFFileDescriptor * pDescriptor);
  //@comm    Use this function to add a Descriptor to be scanned first when searching for
  // the essence (a new primary location).

  //****************
  // PrependFileDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependFileDescriptor
		// @parm [in] Descriptor to append
        (ImplAAFFileDescriptor * pDescriptor);
  //@comm    Use this function to add a Descriptor to be scanned first when searching for
  // the essence (a secondary location for the essence).


  //****************
  // InsertFileDescriptorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertFileDescriptorAt
		// @parm [in] place to insert Descriptor
        (aafUInt32 index,
		// @parm [in] Descriptor to insert
		 ImplAAFFileDescriptor * pDescriptor);


  //****************
  // GetFileDescriptorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileDescriptorAt
		// @parm [in] index of Descriptor to get
        (aafUInt32 index,
		// @parm [in] returned Descriptor
		 ImplAAFFileDescriptor ** ppDescriptor);


  //****************
  // GetFileDescriptorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveFileDescriptorAt
		// @parm [in] index of Descriptor to remove
        (aafUInt32 index);


  //****************
  // GetFileDescriptors()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileDescriptors
        // @parm [out] An enumerator of the FileDescriptors in this MultipleDescriptor
        (ImplEnumAAFFileDescriptors ** ppEnum);

public:
	virtual AAFRESULT
		GetNthDescriptor (aafInt32 index, ImplAAFFileDescriptor **ppDescriptor);

private:
    OMStrongReferenceVectorProperty<ImplAAFFileDescriptor> _Descriptors;
};

#endif // ! __ImplAAFMultipleDescriptor_h__

