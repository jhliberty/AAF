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
#include "EPParameterVisitor.h"
#include "EPParameterTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxParameter.h>
#include <AxDefObject.h>
//#include <AxEx.h>

//AAF files
#include <AAFClassDefUIDs.h>
#include <AAFInterpolatorDefs.h>
//#include <AAFResult.h>

namespace {

using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

 
EPParameterVisitor::EPParameterVisitor( wostream& log,
                                        boost::shared_ptr<EdgeMap> spEdgeMap,
                                        boost::shared_ptr<TestLevelTestResult> spTestResult )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
{
    _validInterpolationDefs.insert( kAAFInterpolationDef_Constant );
    _validInterpolationDefs.insert( kAAFInterpolationDef_Linear );
    _validInterpolationDefs.insert( kAAFInterpolationDef_Log );
    _validInterpolationDefs.insert( kAAFInterpolationDef_Power );
    _validInterpolationDefs.insert( kAAFInterpolationDef_BSpline );
}
    
EPParameterVisitor::~EPParameterVisitor()
{}

bool EPParameterVisitor::PreOrderVisit( AAFTypedObjNode<IAAFParameter>& node )
{
  // JPT REVIEW - Review the value of this. There isn't anything other
  // than constant and varying values in the AAF object model so why
  // at is REQ_EP_174 all about.  I think Andrew misinterpreted the
  // requirement. Since there is no option to use anything other than
  // those it is really a definition, or application
  // requirement. (e.g. to ensure that developer don't start cooking
  // up their own schemes using tagged values or extended types.  But
  // then, if an extended parameter type was created that could cause
  // this code to execute and would correctly fail requirement
  // 174... but that's a remote case.

  //Parameter isn't constant or varying so fail 174.
  AxString name = this->GetMobSlotName( _spEdgeMap, node );

  _spTestResult->AddSingleResult( L"REQ_EP_174",
                                  L"Parameter within " + name + L" is not a ConstantValue or VaryingValue.",
                                  TestResult::FAIL,
				  node );
  return false;
}

bool EPParameterVisitor::PreOrderVisit( AAFTypedObjNode<IAAFVaryingValue>& node )
{
    AxVaryingValue axVaryingVal( node.GetAAFObjectOfType() );
    AxInterpolationDef axIntDef( axVaryingVal.GetInterpolationDefinition() );
    if ( _validInterpolationDefs.find( axIntDef.GetAUID() ) == _validInterpolationDefs.end() )
    {
        AxString name = this->GetMobSlotName( _spEdgeMap, node );

        boost::shared_ptr<DetailLevelTestResult> spFailure = _spTestResult->AddSingleResult(
            L"REQ_EP_175",
            L"VaryingValue object in " + name + L" has an illegal interpolation definition.",
            TestResult::FAIL,
	    node );

        spFailure->AddDetail( L"AUID = " + AxStringUtil::uid2Str( axIntDef.GetAUID() ) + L")." );
    }

    //Regardless of pass/fail, there is no need to traverse further.
    return false;
}

} // end of namespace aafanalyzer
