//=---------------------------------------------------------------------=
//
// $Id: RefResolver.h,v 1.12.4.1 2012/10/12 20:07:46 jptrainor Exp $ $Name: V116 $
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

#ifndef __REFRESOLVER_h__
#define __REFRESOLVER_h__

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

using namespace std;

class TestLevelTestResult;
class TestGraph;

class RefResolver : public Test
{
 public:
  RefResolver(wostream& os, boost::shared_ptr<const TestGraph> spGraph);

  ~RefResolver();

  boost::shared_ptr<TestLevelTestResult> Execute();
  AxString GetName() const;
  AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  RefResolver();
  RefResolver(const RefResolver&);
  RefResolver& operator=( const RefResolver& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
