//=---------------------------------------------------------------------=
//
// $Id: ResolveRefVisitor.h,v 1.8.4.1 2012/10/12 20:07:46 jptrainor Exp $ $Name: V116 $
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

#ifndef __RESOLVEREFVISITOR_h__
#define __RESOLVEREFVISITOR_h__

//TestPhaseLib files
#include <TestLevelTestResult.h>

//AAF Analyzer Base files
#include <TypedVisitor.h>

//AAF files
#include <AAFSmartPointer2.h>

//STL files
#include <vector>

class AxSourceClip;
class AxSegment;
class AxComponent;
class AxTimelineMobSlot;
class AxEventMobSlot;
class AxMobSlot;

namespace aafanalyzer {

using namespace std;

class Edge;
class EdgeMap;
class DetailLevelTestResult;

class ResolveRefVisitor : public TypedVisitor
{
 public:
  ResolveRefVisitor(wostream& os,
		    boost::shared_ptr<EdgeMap> spEdgeMap,
		    boost::shared_ptr<TestLevelTestResult> spTestLevelResult );
  virtual ~ResolveRefVisitor();

  virtual bool PostOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node);

 private:

  boost::shared_ptr<Node> ResolveChildSlotNode( boost::shared_ptr<EdgeMap> spEdgeMap,
					 boost::shared_ptr<Node> spParentMobNode,
					 IAAFMobSP spParentIaafMob,
					 const aafSlotID_t slotId );

  boost::shared_ptr<vector<boost::shared_ptr<Node> > > GetContainedChildComponents( boost::shared_ptr<Node> spMobSlotNode );

  wostream& _os;
  boost::shared_ptr<EdgeMap> _spEdgeMap;
  boost::shared_ptr<DetailLevelTestResult> _spResult;

  // prohibited
  ResolveRefVisitor();
  ResolveRefVisitor( const ResolveRefVisitor& );
  ResolveRefVisitor& operator=( const ResolveRefVisitor& );

  boost::shared_ptr<TestLevelTestResult> _spTestLevelResult;
};

} // end of namespace diskstream

#endif
