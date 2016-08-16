//=---------------------------------------------------------------------=
//
// $Id: OMWString.cpp,v 1.3 2009/06/01 11:47:02 stuart_hc Exp $ $Name: V116 $
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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMWString.h"
#include "OMAssertions.h"


OMWString::OMWString()
{
    TRACE("OMWString::OMWString");
    
    _ws = new wchar_t[1];
    _ws[0] = L'\0';
}


OMWString::OMWString(const OMWString& ws)
{
    TRACE("OMWString::OMWString");

    _ws = new wchar_t[ws.length() + 1];
    wcscpy(_ws, ws.c_str());
}


OMWString::OMWString(const wchar_t* ws)
{
    TRACE("OMWString::OMWString");

    if (ws != 0)
    {
        _ws = new wchar_t[wcslen(ws) + 1];
        wcscpy(_ws, ws);
    }
    else
    {
        _ws = new wchar_t[1];
        _ws[0] = L'\0';
    }
}


OMWString::~OMWString()
{
    TRACE("OMWString::~OMWString");
    
    delete [] _ws;
}


int OMWString::compare(const OMWString& rhs) const
{
    TRACE("OMWString::compare");

    return compare(rhs.c_str());
}


int OMWString::compare(const wchar_t* rhs) const
{
    TRACE("OMWString::compare");

    return wcscmp(_ws, rhs);
}


size_t OMWString::length() const
{
    TRACE("OMWString::length");

    return wcslen(_ws);
}


OMWString& OMWString::operator=(const OMWString& rhs)
{
    TRACE("OMWString::operator=");

    return operator=(rhs.c_str());
}


OMWString& OMWString::operator=(const wchar_t* rhs)
{
    TRACE("OMWString::operator=");

    delete [] _ws;
    if (rhs != 0)
    {
        _ws = new wchar_t[wcslen(rhs) + 1];
        wcscpy(_ws, rhs);
    }
    else
    {
        _ws = new wchar_t[1];
        _ws[0] = L'\0';
    }
    
    return *this;
}


bool OMWString::operator==(const OMWString& rhs) const
{
    TRACE("OMWString::operator==");

    return operator==(rhs.c_str());
}


bool OMWString::operator==(const wchar_t* rhs) const
{
    TRACE("OMWString::operator==");

    if (rhs == 0)
    {
        return false;
    }

    return wcscmp(_ws, rhs) == 0;
}


bool OMWString::operator!=(const OMWString& rhs) const
{
    TRACE("OMWString::operator!=");

    return operator!=(rhs.c_str());
}


bool OMWString::operator!=(const wchar_t* rhs) const
{
    TRACE("OMWString::operator!=");

    if (rhs == 0)
    {
        return true;
    }
    
    return wcscmp(_ws, rhs) != 0;
}


bool OMWString::operator<(const OMWString& rhs) const
{
    TRACE("OMWString::operator<");

    return operator<(rhs.c_str());
}


bool OMWString::operator<(const wchar_t* rhs) const
{
    TRACE("OMWString::operator<");

    if (rhs == 0)
    {
        return false;
    }
    
    return wcscmp(_ws, rhs) < 0;
}


const wchar_t* OMWString::c_str() const
{
    TRACE("OMWString::c_str");

    return _ws;
}

