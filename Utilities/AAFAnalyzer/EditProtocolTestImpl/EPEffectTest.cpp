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

//Edit Protocol Test files
#include "EPEffectTest.h"
#include "EPEffectVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>

//AAF Analyzer Base files
#include <TestGraph.h>

//Requirement files
#include <Requirement.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//STL files
#include <vector>

namespace {

const wchar_t* TEST_NAME = L"Edit Protocol Effect Test";
const wchar_t* TEST_DESC = L"Verify that all effects in the AAF file are valid.";

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPEffectTest::EPEffectTest( wostream& log,
                              boost::shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph(spGraph);
}

EPEffectTest::~EPEffectTest()
{}

boost::shared_ptr<TestLevelTestResult> EPEffectTest::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();
    
  boost::shared_ptr<EPEffectVisitor>
    spVisitor( new EPEffectVisitor( GetOutStream(),
				    GetTestGraph()->GetEdgeMap(),
				    spTestResult ) );
  
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());
  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );

  return spTestResult;
}

AxString EPEffectTest::GetName() const
{
  return TEST_NAME;
}

AxString EPEffectTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPEffectTest::GetTestInfo()
{
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    spReqIds->push_back(L"REQ_EP_180");     //Video Dissolve only permitted in Transition.
    spReqIds->push_back(L"REQ_EP_183");     //SMPTE Video Wipe only permitted in Transition.
    spReqIds->push_back(L"REQ_EP_186");     //Video Speed Control not in Transition.
    spReqIds->push_back(L"REQ_EP_187");     //Speed Ratio parameter is not 0.
    spReqIds->push_back(L"REQ_EP_190");     //Video Repeat not in Transition.
    spReqIds->push_back(L"REQ_EP_194");     //Video Flip not in Transition.
    spReqIds->push_back(L"REQ_EP_197");     //Video Flop not in Transition.
    spReqIds->push_back(L"REQ_EP_200");     //Video Flip-Flop not in Transition.
    spReqIds->push_back(L"REQ_EP_203");     //Video Position not in Transition.
    spReqIds->push_back(L"REQ_EP_206");     //Video Crop not in Transition.
    spReqIds->push_back(L"REQ_EP_209");     //Video Scale not in Transition.
    spReqIds->push_back(L"REQ_EP_212");     //Video Rotate not in Transition.
    spReqIds->push_back(L"REQ_EP_215");     //Video Corner Pinning not in Transition.
    spReqIds->push_back(L"REQ_EP_218");     //Alpha With Video Key has RGBA or CDIC descriptor.
    spReqIds->push_back(L"REQ_EP_219");     //Alpha With Video Key has AlphaTransparency property.
    spReqIds->push_back(L"REQ_EP_220");     //Alpha With Video Key not in Transition.
    spReqIds->push_back(L"REQ_EP_223");     //Separate Alpha Key has RGBA or CDIC descriptor.
    spReqIds->push_back(L"REQ_EP_224");     //Separate Alpha Key has AlphaTransparency property.
    spReqIds->push_back(L"REQ_EP_225");     //Separate Alpha Key not in Transition.
    spReqIds->push_back(L"REQ_EP_228");     //Luminance Key not in Transition.
    spReqIds->push_back(L"REQ_EP_231");     //Chroma Key not in Transition.
    spReqIds->push_back(L"REQ_EP_234");     //Mono Audio Gain not in Transition.
    spReqIds->push_back(L"REQ_EP_240");     //Mono Audio Pan not in Transition.
    spReqIds->push_back(L"REQ_EP_244");     //Single-Parameter Mono Audio Dissolve in Transition.
    spReqIds->push_back(L"REQ_EP_247");     //Two-Parameter Audio Dissolve in Transition.
    spReqIds->push_back(L"REQ_EP_248");     //Two-Parameter Mono Audio Dissolve Input Length.
    spReqIds->push_back(L"REQ_EP_249");     //Two-Parameter Mono Audio Dissolve Input Length.
    return TestInfo(L"EPEffectTest", spReqIds);
}

} // end of namespace aafanalyzer
