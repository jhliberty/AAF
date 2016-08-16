//=---------------------------------------------------------------------=
//
// $Id: Test.cpp,v 1.9.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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

//Test/Result files
#include "Test.h"
#include "TestRegistry.h"

//Analyzer Base files
#include <TestGraph.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

Test::Test(wostream& os, const TestInfo& info)
  : _os(os),
    _spCoveredRequirements(InitializeRequirements(info.GetName()))
{}

const Test::ConstRequirementMapSP Test::InitializeRequirements(const wstring& name)
{
  TestRegistry& reg = TestRegistry::GetInstance();
  return reg.GetConstRequirementsForTest(name);
}

Test::~Test()
{}

wstring Test::GetName() const
{
  wstring null;
  return null;
}

wstring Test::GetDescription() const
{
  wstring null;
  return null;
}

wostream& Test::GetOutStream() const
{
  return _os;
}

boost::shared_ptr<const TestGraph> Test::GetTestGraph()
{
  return _spGraph;
}

void Test::SetTestGraph(boost::shared_ptr<const TestGraph> spGraph)
{
  _spGraph = spGraph;
}

boost::shared_ptr<TestLevelTestResult> Test::CreateTestResult() const
{
  boost::shared_ptr<TestLevelTestResult>
    spTestResult( new TestLevelTestResult( GetName(),
                                           GetDescription(),
                                           this->shared_from_this() ) );
  return spTestResult;
}

boost::shared_ptr<TestLevelTestResult> Test::CreateTestResult( const wstring& explain,
                                                        TestResult::Result result ) const
{
  boost::shared_ptr<TestLevelTestResult>
    spTestResult( new TestLevelTestResult( GetName(),
                                           GetDescription(),
                                           explain,
                                           result,
                                           this->shared_from_this() ) );
  return spTestResult;
}

boost::shared_ptr<TestLevelTestResult> Test::CreatePassTestResult() const
{
  return CreateTestResult( L"", TestResult::PASS );
}

const Requirement::RequirementMap& Test::GetCoveredRequirements() const
{
  return *_spCoveredRequirements;
}

// Get a single requirement that is registered against this test.
// Throws xxx if reqId is not registered.
boost::shared_ptr<const Requirement> Test::GetRequirement( const wstring& reqId ) const
{
  Requirement::RequirementMap::const_iterator iter = _spCoveredRequirements->find(reqId);
  assert( iter != _spCoveredRequirements->end() );
  return iter->second;
}

bool Test::IsRequirementRegistered( const wstring& reqId ) const
{
  return _spCoveredRequirements->find(reqId) != _spCoveredRequirements->end();
}

} // end of namespace diskstream
