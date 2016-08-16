#ifndef __ImplEnumAAFFileEncodings_h__
#define __ImplEnumAAFFileEncodings_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplEnumAAFFileEncodings.h,v 1.2 2009/06/01 11:47:09 stuart_hc Exp $ $Name: V116 $
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


#include "OMFile.h"

#include "ImplAAFRoot.h"


class ImplAAFFileEncoding;


class ImplEnumAAFFileEncodings : public ImplAAFRoot
{
public:
  ImplEnumAAFFileEncodings();

public:
  //********************************
  // AAF SDK exported methods start
  //********************************

  //***********************************************************
  //
  // NextOne()
  //
  // Enumerates to the next element in the enumerators list. The
  // caller is responsible for properly releasing the returned pointer
  // when it is no longer needed.
  // 
  // Succeeds if all of the following are true:
  // - the ppFileEncoding pointer is valid.
  // - there are File Encoding objects remaining to be returned.
  // 
  // If this method fails nothing is written to *ppFileEncoding.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppFileEncoding arg is NULL.
  //
  // AAFRESULT_NO_MORE_OBJECTS
  //   - no File Encoding objects remaining to be returned.
  //
  virtual AAFRESULT STDMETHODCALLTYPE NextOne(
      ImplAAFFileEncoding**   ppFileEncoding );


  //***********************************************************
  //
  // Next()
  //
  // Enumerates the next count elements (AAFFileEncoding pointers) in the
  // enumerator's list, returning them in the given array along with
  // the actual number of enumerated elements in pNumFetched. The caller
  // is responsible for properly releasing the returned pointers.
  // 
  // Succeeds if all of the following are true:
  // - The pFileEncodings pointer is valid.
  // - The pNumFetched pointer is valid. If count is 1, pNumFetched
  //   can be NULL.
  // - The requested number of objects is greater that zero.
  // - There are File Encoding objects remaining to be returned.
  // 
  // If this method fails nothing is written to *FileEncodings or
  // pNumFetched.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - Either pFileEncodings or pNumFetched arg is NULL.
  //
  // AAFRESULT_INVALID_PARAM
  //   - count argument is zero.
  //
  // AAFRESULT_NO_MORE_OBJECTS
  //   - no File Encoding objects remaining to be returned.
  //
  virtual AAFRESULT STDMETHODCALLTYPE Next(
      aafUInt32               count,
      ImplAAFFileEncoding**   pFileEncodings,
      aafUInt32*              pNumFetched );


  //***********************************************************
  //
  // Skip()
  //
  // Instructs the enumerator to skip the next count elements in the
  // enumeration so that the next call to Next will not return those
  // elements.
  // 
  // Succeeds if all of the following are true:
  // - count is not zero and less than or equal to the number
  //   of remaining objects.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_INVALID_PARAM
  //   - count argument is zero.
  //
  // AAFRESULT_NO_MORE_OBJECTS
  //   - count exceeded number of remaining objects.
  //
  virtual AAFRESULT STDMETHODCALLTYPE Skip(
      aafUInt32                   count );


  //***********************************************************
  //
  // Reset()
  //
  // Instructs the enumerator to position itself at the beginning of
  // the list of elements.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  virtual AAFRESULT STDMETHODCALLTYPE Reset();


  //***********************************************************
  //
  // Clone()
  //
  // Creates another enumerator with the same state as the current
  // enumerator to iterate over the same list. This method makes it
  // possible to record a point in the enumeration sequence in order
  // to return to that point at a later time.
  //
  // Note: The caller must release this new enumerator separately from
  // the first enumerator.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
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
  virtual AAFRESULT STDMETHODCALLTYPE Clone(
      ImplEnumAAFFileEncodings**  ppEnum );


  //********************************
  // AAF SDK exported methods end
  //********************************


protected:
  virtual ~ImplEnumAAFFileEncodings();

protected:
  OMFile::FactorySetIterator&     _factory_set_iterator;
};


#endif // ! __ImplEnumAAFFileEncodings_h__
