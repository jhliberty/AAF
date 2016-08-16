//=---------------------------------------------------------------------=
//
// $Id: TopLevelTestResult.cpp,v 1.4.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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
#include "TopLevelTestResult.h"
#include "TestPhaseLevelTestResult.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

TopLevelTestResult::TopLevelTestResult()
  : HighLevelTestResult()
{}

TopLevelTestResult:: TopLevelTestResult( const wstring& name, 
                                         const wstring& desc,
                                         const wstring& explain )
  : HighLevelTestResult( name, desc, explain )
{}

TopLevelTestResult::~TopLevelTestResult()
{}

void TopLevelTestResult::AppendSubtestResult( boost::shared_ptr<TestPhaseLevelTestResult> spSubtestResult )
{
  this->AddSubtestResult( spSubtestResult );
}

const enum TestResult::ResultLevel TopLevelTestResult::GetResultType() const
{
  return TestResult::TOP;
}

} // end of namespace diskstream
