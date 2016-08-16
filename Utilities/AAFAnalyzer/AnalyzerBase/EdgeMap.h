//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

#ifndef __EDGEMAP_h__
#define __EDGEMAP_h__

//Analyzer Base files
#include "Node.h"

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>
#include <vector>

namespace aafanalyzer {

using namespace std;

class Edge;

class EdgeMap
{
 public:

  typedef unsigned int LID;
  typedef vector<boost::shared_ptr<Edge> > EdgeVector;
  typedef boost::shared_ptr<EdgeVector> EdgeVectorSP;
  typedef map<Node::LID, EdgeVectorSP> Map;
  typedef boost::shared_ptr<const EdgeVector> ConstEdgeVectorSP;

  EdgeMap();
  ~EdgeMap();

  ConstEdgeVectorSP GetParents(boost::shared_ptr<Node> spNode) const;
  ConstEdgeVectorSP GetChildren(boost::shared_ptr<Node> spNode) const;

  void AddEdge(boost::shared_ptr<Edge> spEdge);
  void DecorateEdge(boost::shared_ptr<Node> decoratedNode);

 private:
 
  bool IsInMap(Node::LID lid, const Map& map) const;

  mutable Map _parents;
  mutable Map _children;

  // prohibited
  EdgeMap( const EdgeMap& );
  EdgeMap& operator=( const EdgeMap& );
};

} // end of namespace diskstream

#endif
