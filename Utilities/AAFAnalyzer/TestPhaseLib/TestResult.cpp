//=---------------------------------------------------------------------=
//
// $Id: TestResult.cpp,v 1.14.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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
#include "TestResult.h"

#include <iostream>

namespace {

using namespace aafanalyzer;
using namespace std;

void DumpRequirementSet( const wstring& what,
                         const wstring& prefix,
                         wostream& os,
                         Requirement::RequirementMapSP spReqMap )
{
  os << prefix << what << L": ";
  for( Requirement::RequirementMap::iterator iter = spReqMap->begin();
        iter != spReqMap->end();
        ++iter )
  {
    os << iter->first;
    ++iter;
    if ( iter != spReqMap->end() )
    {
      os << L", ";
    }
    --iter;
  }
  os << endl;
}

} // end of namespace

//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

map<TestResult::Result,wstring> TestResult::_resultToString;

TestResult::TestResult()
  : _result( UNDEFINED ),
    _spSubtestResults(new SubtestResultVector()),
    _spCoveredRequirements( new Requirement::RequirementMap() ),
    _spNotedRequirements( new Requirement::RequirementMap() ),
    _spPassedRequirements( new Requirement::RequirementMap() ),
    _spInfoRequirements( new Requirement::RequirementMap() ),
    _spWarnedRequirements( new Requirement::RequirementMap() ),
    _spFailedRequirements( new Requirement::RequirementMap() ),
    _spUndefinedRequirements( new Requirement::RequirementMap() )
{
  if ( _resultToString.empty() )
  {
    _resultToString[UNDEFINED] = L"UNDEFINED";
    _resultToString[COVERED] = L"COVERED";
    _resultToString[NOTED] = L"NOTED";
    _resultToString[PASS] = L"PASS";
    _resultToString[INFO] = L"INFO";
    _resultToString[WARN] = L"WARN";
    _resultToString[FAIL] = L"FAIL";
  }
}

TestResult::TestResult( const wstring& name,
                        const wstring& desc,
                        const wstring& explain )
  : _name( name ),
    _desc( desc ),
    _expl( explain ),
    _result( UNDEFINED ),
    _spSubtestResults(new SubtestResultVector()),
    _spCoveredRequirements( new Requirement::RequirementMap() ),
    _spNotedRequirements( new Requirement::RequirementMap() ),
    _spPassedRequirements( new Requirement::RequirementMap() ),
    _spInfoRequirements( new Requirement::RequirementMap() ),
    _spWarnedRequirements( new Requirement::RequirementMap() ),
    _spFailedRequirements( new Requirement::RequirementMap() ),
    _spUndefinedRequirements( new Requirement::RequirementMap() )
{}

TestResult::~TestResult()
{}

const wstring& TestResult::GetExplanation() const
{
  return _expl;
}

const wstring& TestResult::GetName() const
{
  return _name;
}

const wstring& TestResult::GetDescription() const
{
  return _desc;
}

enum TestResult::Result TestResult::GetResult() const
{
  return _result;
}

const wstring& TestResult::GetResultAsString() const
{
  return _resultToString[_result];
}

/*static*/
const wstring& TestResult::ResultToString(Result result)
{
  return TestResult::_resultToString[result];
}

void TestResult::SetExplanation(const wstring& exp)
{
  _expl = exp;
}

void TestResult::SetName(const wstring& name)
{
  _name = name;
}

void TestResult::SetDescription(const wstring& desc)
{
  _desc = desc;
}

const TestResult::SubtestResultVector& TestResult::GetSubtestResults() const
{
    return *_spSubtestResults;
}

bool TestResult::ContainsSubtests() const {
    return !_spSubtestResults->empty();
}

const Requirement::RequirementMap& TestResult::GetRequirements( Result type ) const
{
  switch (type)
  {
  case COVERED:
    return *_spCoveredRequirements;
    break;
  case NOTED:
    return *_spNotedRequirements;
    break;
  case PASS:
    return *_spPassedRequirements;
    break;
  case INFO:
    return *_spInfoRequirements;
    break;
  case WARN:
    return *_spWarnedRequirements;
    break;
  case FAIL:
    return *_spFailedRequirements;
    break;
  default:
    assert( type == UNDEFINED );
    return *_spUndefinedRequirements;
  }
}

void TestResult::AddDetail( const wstring& detail )
{
  _details.push_back( detail );
}

const vector<wstring>& TestResult::GetDetails() const
{
  return _details;
}

void TestResult::AddSubtestResult( boost::shared_ptr<TestResult> subtestResult )
{
  //Don't allow a test result to append itself
  assert( subtestResult.get() != this );

  _spSubtestResults->push_back( subtestResult );
}

bool TestResult::ContainsRequirement( const wstring& id, Result& outContainedIn )
{
    if ( _spCoveredRequirements->find(id) != _spCoveredRequirements->end() )
    {
      outContainedIn = COVERED;
      return true;
    }
    else if ( _spNotedRequirements->find(id) != _spNotedRequirements->end() )
    {
      outContainedIn = NOTED;
      return true;
    }
    else if ( _spPassedRequirements->find(id) != _spPassedRequirements->end() )
    {
      outContainedIn = PASS;
      return true;
    }
    else if ( _spInfoRequirements->find(id) != _spInfoRequirements->end() )
    {
      outContainedIn = INFO;
      return true;
    }
    else if ( _spWarnedRequirements->find(id) != _spWarnedRequirements->end() )
    {
      outContainedIn = WARN;
      return true;
    }
    else if ( _spFailedRequirements->find(id) != _spFailedRequirements->end() )
    {
      outContainedIn = FAIL;
      return true;
    }
    else if ( _spUndefinedRequirements->find(id) != _spUndefinedRequirements->end() )
    {
      outContainedIn = UNDEFINED;
      return true;
    }
    else
    {
      return false;
    }
}

bool TestResult::HasResult( const wstring& id, Result result ) const
{
  switch ( result )
  {
    case UNDEFINED:
      return _spUndefinedRequirements->find(id) != _spUndefinedRequirements->end();
    case FAIL:
      return _spFailedRequirements->find(id) != _spFailedRequirements->end();
    case WARN:
      return _spWarnedRequirements->find(id) != _spWarnedRequirements->end();
    case INFO:
      return _spInfoRequirements->find(id) != _spInfoRequirements->end();
    case PASS:
      return _spPassedRequirements->find(id) != _spPassedRequirements->end();
    case NOTED:
      return _spNotedRequirements->find(id) != _spNotedRequirements->end();
    case COVERED:
      return _spCoveredRequirements->find(id) != _spCoveredRequirements->end();
    default:
      assert(0);
  }

  // Keep the compiler happy.
  return false;
}

void TestResult::ClearRequirements()
{
    _spCoveredRequirements->clear();
    _spNotedRequirements->clear();
    _spPassedRequirements->clear();
    _spWarnedRequirements->clear();
    _spFailedRequirements->clear();
    _spUndefinedRequirements->clear();
    _result = UNDEFINED;
}

void TestResult::AddRequirement( Result type, const boost::shared_ptr<const Requirement>& req )
{
    Requirement::RequirementMapSP spMap = this->GetMyRequirements( type );
    (*spMap)[req->GetId()] = req;
}

void TestResult::RemoveRequirement( const wstring& id )
{
    _spCoveredRequirements->erase(id);
    _spNotedRequirements->erase(id);
    _spPassedRequirements->erase(id);
    _spWarnedRequirements->erase(id);
    _spFailedRequirements->erase(id);
    _spUndefinedRequirements->erase(id);
}

const Requirement::RequirementMapSP& TestResult::GetMyRequirements( Result type )
{
  switch (type)
  {
    case COVERED:
      return _spCoveredRequirements;
      break;
    case NOTED:
      return _spNotedRequirements;
      break;
    case PASS:
      return _spPassedRequirements;
      break;
    case INFO:
      return _spInfoRequirements;
      break;
    case WARN:
      return _spWarnedRequirements;
      break;
    case FAIL:
      return _spFailedRequirements;
      break;
    default:
      assert( type == UNDEFINED );
      return _spUndefinedRequirements;
  }
}

void TestResult::ConsolidateResults()
{
  // If this is not a leaf node then we will be aggregating the child
  // results. Clear the existing results before doing so to avoid
  // merging new results into the old, stale, results. We don't clear
  // leave nodes because those are not aggregate results - they are
  // the individual test results. If we clear'ed them there would be
  // nothing left to aggregate.
  if ( !_spSubtestResults->empty() )
  {
    this->ClearRequirements();
  }

  // This exists to provide TestLevelTestResult an opportunity to
  // initialize its set of passing requirements to those of the
  // associated test (i.e. they are assumed to all pass). These are
  // updated to reflect failed requires when the DetelLevelTestResults
  // (i.e. sub results) are consolidated.
  this->InitConsolidateResults();

  for( SubtestResultVector::iterator iter = _spSubtestResults->begin();
       iter != _spSubtestResults->end();
       ++iter )
  {
    boost::shared_ptr<TestResult> spSubTestResult = *iter;
    
    spSubTestResult->ConsolidateResults();

    //Properly set the status of requirements based on the status of
    //the new child requirements.
    for (int curReqLevel = UNDEFINED; curReqLevel <= FAIL; curReqLevel++)
    {
      //Find all the requirements and loop through them.
      Requirement::RequirementMap requirements = spSubTestResult->GetRequirements((Result)curReqLevel);
      Requirement::RequirementMap::iterator iter;
      for ( iter = requirements.begin(); iter != requirements.end(); iter++ )
      {
        //If the requirement was already in a map store it in the map
        //with the worst possible status.  Otherwise, add the
        //requirement to the map that it is in, in the child subtest.
        Result oldReqLevel;
        if ( this->ContainsRequirement( iter->first, oldReqLevel ) )
        {
          if (oldReqLevel < curReqLevel)
          {
            this->RemoveRequirement(iter->first);
            this->AddRequirement((Result)curReqLevel, iter->second);
          }
        }
        else
        {
          this->AddRequirement((Result)curReqLevel, iter->second);
        }
      }
    }

    // Update _result if the subtest is worse than the results
    // consolidated so far.
    if ( spSubTestResult->GetResult() > _result )
    {
      _result = spSubTestResult->GetResult();
    }
  }
}

void TestResult::InitConsolidateResults()
{}

void TestResult::ProtectedSetResult( Result result )
{
  _result = result;
}

void TestResult::Dump( const wstring& prefix, wostream& os ) const
{
  os << prefix << _name << L":" << endl;
  DumpRequirementSet( L"covered",   prefix, os, _spCoveredRequirements   );
  DumpRequirementSet( L"noted",     prefix, os, _spNotedRequirements     );
  DumpRequirementSet( L"pass",      prefix, os, _spPassedRequirements    );
  DumpRequirementSet( L"info",      prefix, os, _spInfoRequirements      );
  DumpRequirementSet( L"warn",      prefix, os, _spWarnedRequirements    );
  DumpRequirementSet( L"fail",      prefix, os, _spFailedRequirements    );
  DumpRequirementSet( L"undefined", prefix, os, _spUndefinedRequirements );
}

} // end of namespace diskstream
