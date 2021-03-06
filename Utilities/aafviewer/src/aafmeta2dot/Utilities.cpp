/*
 * $Id: Utilities.cpp,v 1.5 2004/09/10 17:13:14 stuart_hc Exp $ $Name: V116 $
 *
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aafmeta2dot.
 *
 *  aafmeta2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  aafmeta2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with aafmeta2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <iostream>
#include <assert.h>
using namespace std;

#include <AxUtil.h>

#include <Utilities.h>


//-----------------------------------------------------------------------------
string
AxStringToString( AxString axString )
{
   return AxStringUtil::wctomb(axString);
}


//-----------------------------------------------------------------------------
string 
UIDToString( aafUID_t uid )
{
	return AxStringUtil::wctomb( AxStringUtil::uid2Str( uid ) );
}

