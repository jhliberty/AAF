//=---------------------------------------------------------------------=
//
// $Id: TestRegistry.h,v 1.7.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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

#ifndef __TESTREGISTRY_H_
#define __TESTREGISTRY_H_

//Requirement files
#include <Requirement.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <string>
#include <map>

namespace aafanalyzer {

using namespace std;

class TestResult;
class TopLevelTestResult;
class TestInfo;

class TestRegistry
{
 public:

  // dtor must be public so that shared_ptr class can access it
  ~TestRegistry();

  static TestRegistry& GetInstance();
  void Register( const TestInfo& info );
  const boost::shared_ptr<Requirement::RequirementMap> GetRequirementsForTest( const wstring& name ) const;
  const boost::shared_ptr<const Requirement::RequirementMap> GetConstRequirementsForTest( const wstring& name ) const;
  const Requirement::RequirementMap& GetRequirementCoverage() const;
  bool VerifyTestResultCoverage(const boost::shared_ptr<TopLevelTestResult> results) const;

 private:

  typedef map< const wstring, boost::shared_ptr<const Requirement::RequirementMap> > Map;

  TestRegistry();//using Singleton pattern to allow only one object
  void VerifyTestResultCoverage(const boost::shared_ptr<const TestResult> result, Requirement::RequirementMap& outstandingReqs) const;

  static TestRegistry* _pTestRegistry;
  Map _testSet;
  Requirement::RequirementMap _coveredRequirements;

  // prohibited
  TestRegistry( const TestRegistry& );
  TestRegistry& operator=( const TestRegistry& );

};

} // end of namespace diskstream

#endif /*__TESTREGISTRY_H_*/
