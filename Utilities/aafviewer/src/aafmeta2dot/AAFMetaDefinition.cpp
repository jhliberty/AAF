/*
 * $Id: AAFMetaDefinition.cpp,v 1.5 2004/11/26 17:20:27 stuart_hc Exp $ $Name: V116 $
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
using namespace std;

#include <AAFMetaDefinition.h>


//-----------------------------------------------------------------------------
AAFMetaDefinition::AAFMetaDefinition( string name, string auid, string description )
   : _wasBuilt( false ), _name( name ), _auid( auid ),
	_description( description )
{
}


//-----------------------------------------------------------------------------
void
AAFMetaDefinition::InitForBuild()
{
   _wasBuilt = false;
}

