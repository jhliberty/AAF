//=---------------------------------------------------------------------=
//
// $Id: RequirementLoader.h,v 1.8 2009/06/01 11:46:48 stuart_hc Exp $ $Name: V116 $
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

#ifndef __REQUIREMENTLOADER_H_
#define __REQUIREMENTLOADER_H_

//Requirement files
#include "Requirement.h"

//STL files
#include <string>
#include <map>

namespace aafanalyzer {

class RequirementLoader
{
public:

    RequirementLoader();
    ~RequirementLoader();

    static void __StartElement(void* userData, const char* name, const char** atts);
    static void __EndElement(void* userData, const char* name);
    static void __EndData(void* userData, const char* s, int len);

    void StartElement(const wstring& name, const char** attribs);
    void EndElement(const wstring& name);
    void EndData(const wstring& contents);

    void ParseXML( const char* filename ) const;

 private:

    struct Current
    {
      Current()
      : Data(),
        Id(),
        Type(Requirement::REQ_TYPE_UNDEFINED),
        TypeAsString(),
        Category(Requirement::CAT_UNDEFINED),
        CategoryAsString(),
        Action(),
        Name(),
        Desc(),
        Annotate(),
        Note(),
        Document(),
        Version(),
        Section()
      {}
      
      wstring Data;
      
      wstring Id;
      Requirement::RequirementType Type;
      wstring TypeAsString;
      Requirement::Category Category;
      wstring CategoryAsString;
      wstring Action;
      wstring Name;
      wstring Desc;
      wstring Annotate;
      wstring Note;
      wstring Document;
      wstring Version;
      wstring Section;
    };

    Current _current;

    map<wstring, Requirement::Category> _categoryMap;

    // prohibited
    RequirementLoader( const RequirementLoader& );
    RequirementLoader& operator=( const RequirementLoader& );

};

} // end of namespace diskstream

#endif /*__REQUIREMENTLOADER_H_*/
