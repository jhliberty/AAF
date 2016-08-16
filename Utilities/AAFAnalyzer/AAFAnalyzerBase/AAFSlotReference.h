//=---------------------------------------------------------------------=
//
// $Id: AAFSlotReference.h,v 1.5.4.1 2012/10/12 17:19:22 jptrainor Exp $ $Name: V116 $
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
//=---------------------------------------------------------------------=

#ifndef __AAFSLOTREFERENCE_h__
#define __AAFSLOTREFERENCE_h__

//Analyzer Base files
#include <Edge.h>

#include <string>

namespace aafanalyzer {

class AAFSlotReference : public Edge
{
 public:
  
  static const std::wstring typeName;

  AAFSlotReference(boost::shared_ptr<Node> spParent, boost::shared_ptr<Node> spChild, Node::LID tag);
  ~AAFSlotReference();

  bool Visit(boost::shared_ptr<Visitor> spVisitor);
  virtual boost::shared_ptr<Edge> CreateNewEdge( boost::shared_ptr<Node> spParent, boost::shared_ptr<Node> spChild ) const;

  virtual const std::wstring& GetTypeName() const;

 private:

  // prohibited
  AAFSlotReference();
  AAFSlotReference( const AAFSlotReference& );
  AAFSlotReference& operator=( const AAFSlotReference& );
};

} // end of namespace diskstream

#endif
