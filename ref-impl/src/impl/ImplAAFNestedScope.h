//@doc
//@class    AAFNestedScope | Implementation class for AAFNestedScope
#ifndef __ImplAAFNestedScope_h__
#define __ImplAAFNestedScope_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFNestedScope.h,v 1.17 2009/06/01 11:47:06 stuart_hc Exp $ $Name: V116 $
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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

/*************************************************************************
 * 
 * @class AAFNestedScope | an AAFNestedScope object defines a scope that 
 *			contains an ordered set of AAFSegments and produces the value
 *			specified by the last AAFSegement in the ordered set.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/

#include "ImplAAFSegment.h"

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFSegment> ImplEnumAAFSegments;

#include "OMStrongRefVectorProperty.h"

class ImplAAFNestedScope : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFNestedScope ();

protected:
  virtual ~ImplAAFNestedScope ();

public:


  //****************
  // AppendSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendSegment
        // @parm [in] Pointer to segment to be added
        (ImplAAFSegment * pSegment);

  //****************
  // PrependSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependSegment
        // @parm [in] Pointer to segment to be added
        (ImplAAFSegment * pSegment);

  //****************
  // InsertSegmentAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertSegmentAt
        // @parm [in] index where segment is to be inserted
        (aafUInt32 index,
        // @parm [in] Pointer to segment to be added
		 ImplAAFSegment * pSegment);

  //****************
  // CountSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountSegments
        // @parm [out\, retval] number of segments
        (aafUInt32 * pResult);

  //****************
  // RemoveSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSegmentAt
        // @parm [in] index of segment to be removed
        (aafUInt32 index);

  //****************
  // GetSegmentAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSegmentAt
        // @parm [in] index of segment to retrieve
        (aafUInt32 index,
        // @parm [out, retval] retrieved segment
		 ImplAAFSegment ** ppSegment);

  //****************
  // GetSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSegments
        // @parm [retval][out] Slots - segment list  enumeration
        (ImplEnumAAFSegments ** ppEnum);

public:
	// SDK-internal

	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);

	// Visit this component and its sub-components
	// with the specified visitor.
	virtual void Accept(AAFComponentVisitor& visitor);


private:

  // Persistent Properties	
  OMStrongReferenceVectorProperty<ImplAAFSegment>		_slots;
};

#endif // ! __ImplAAFNestedScope_h__


