//=---------------------------------------------------------------------=
//
// $Id: Test.h,v 1.9.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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

#ifndef __TEST_h__
#define __TEST_h__

//Test/Result files
#include <TestInfo.h>
#include <TestLevelTestResult.h>

//Requirement files
#include <Requirement.h>

//Boost files
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//STL files
#include <string>
#include <iostream>

namespace aafanalyzer {

using namespace std;

class TestGraph;

class Test : public boost::enable_shared_from_this<Test>
{
 public:
  Test(wostream& os, const TestInfo& info);
  virtual ~Test();

  virtual boost::shared_ptr<TestLevelTestResult> Execute() = 0;
  virtual wstring GetName() const;
  virtual wstring GetDescription() const;
  wostream& GetOutStream() const;
  boost::shared_ptr<const TestGraph> GetTestGraph();

  // Get the requirements registered against this test.
  const Requirement::RequirementMap& GetCoveredRequirements() const;

  // Get a single requirement that is registered against this test.
  // The call asserts that reqId is registered.
  boost::shared_ptr<const Requirement> GetRequirement( const wstring& reqId ) const;

  // Is reqId registered against this test.
  bool IsRequirementRegistered( const wstring& reqId ) const;

protected:

  void SetTestGraph(boost::shared_ptr<const TestGraph> spGraph);

  // Create a result for the purpose of add sub results.
  // The result name and description come from this test.
  // The default result value is that provided by the TestResult
  // constructor.
  boost::shared_ptr<TestLevelTestResult> CreateTestResult() const;

  // Create a result for tests that don't add sub results (hence must
  // provide the test result value and an explanation).
  // The result name and description come from this test.
  boost::shared_ptr<TestLevelTestResult> CreateTestResult( const wstring& explain,
                                                    TestResult::Result result ) const;

  // Same as above, but the result is PASS and has no explanation.
  // This is common for test that are simply used to wrap up some processing
  // in a standard interface, can't fail on (and don't register any requirements),
  // either pass or fail fatally (throw an exception).
  boost::shared_ptr<TestLevelTestResult> CreatePassTestResult() const;

 private:
  // prohibited
  Test();
  Test(const Test&);
  Test& operator=( const Test& );

  wostream& _os;
  boost::shared_ptr<const TestGraph> _spGraph;
  
  typedef boost::shared_ptr<const Requirement::RequirementMap> ConstRequirementMapSP;
  const ConstRequirementMapSP _spCoveredRequirements;

  const ConstRequirementMapSP InitializeRequirements(const wstring& name);
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
