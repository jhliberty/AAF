//=---------------------------------------------------------------------=
//
// $Id: TestPhase.h,v 1.6.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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

#ifndef __TESTPHASE_h__
#define __TESTPHASE_h__

//Boost files
#include <boost/shared_ptr.hpp>

//STL file
#include <string>
#include <iostream>

namespace aafanalyzer {

using namespace std;

class TestPhaseLevelTestResult;

class TestPhase
{
 public:
  TestPhase(wostream& os);
  virtual ~TestPhase();

  virtual wstring GetDescription() const;
  virtual wstring GetName() const;
  virtual boost::shared_ptr<TestPhaseLevelTestResult> Execute() = 0; 
  wostream& GetOutStream() const; 

 private:
  wostream& _os;

  // prohibited
  TestPhase();
  TestPhase( const TestPhase& );
  TestPhase& operator=( const TestPhase& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
