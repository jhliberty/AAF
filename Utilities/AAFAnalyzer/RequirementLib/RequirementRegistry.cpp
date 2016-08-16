//=---------------------------------------------------------------------=
//
// $Id: RequirementRegistry.cpp,v 1.3.4.1 2012/10/12 20:07:49 jptrainor Exp $ $Name: V116 $
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

//Requirement files
#include "RequirementRegistry.h"
#include "RequirementRegistryException.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

//static variable
RequirementRegistry* RequirementRegistry::_pRequirementRegistry = NULL;

RequirementRegistry::RequirementRegistry()
{
}

RequirementRegistry::~RequirementRegistry()
{
    if(_pRequirementRegistry != NULL)
    {
        delete _pRequirementRegistry;
        _pRequirementRegistry = NULL;
    }
}

RequirementRegistry& RequirementRegistry::GetInstance()
{
    //using Singleton pattern, create object iff it does not exist
    if(_pRequirementRegistry == NULL)
    {
        _pRequirementRegistry = new RequirementRegistry();
    }

    return *_pRequirementRegistry;
}

void RequirementRegistry::Register( const boost::shared_ptr<const Requirement>& req )
{
    if ( _requirementSet.find( req->GetId() ) == _requirementSet.end() )
    {
        _requirementSet[req->GetId()] = req;
    }
    else
    {
        wstring msg = L"Requirement " + req->GetId() + L" is already registered.";
        throw RequirementRegistryException( msg.c_str() );
    }
}

const boost::shared_ptr<const Requirement> RequirementRegistry::GetRequirement( const wstring& id ) const
{
    Requirement::RequirementMap::const_iterator target = _requirementSet.find( id );
    if ( target == _requirementSet.end() )
    {
        wstring msg = L"Requirement " + id + L" has not been registered.";
        throw RequirementRegistryException( msg.c_str() );
    }    
    return target->second;
}

const Requirement::RequirementMap& RequirementRegistry::GetAllRequirements() const
{
    return _requirementSet;
}

} // end of namespace diskstream
