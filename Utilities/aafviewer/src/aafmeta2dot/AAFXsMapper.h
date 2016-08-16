/*
 * $Id: AAFXsMapper.h,v 1.3 2004/02/27 16:41:24 stuart_hc Exp $ $Name: V116 $
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

#ifndef __AAFXSMAPPER_H__
#define __AAFXSMAPPER_H__

#include <AxDictionary.h>
#include <AxMetaDef.h>
#include <AxTypes.h>

#include <AAFClassDefinition.h>
#include <AAFPropertyDefinition.h>
#include <AAFTypeDefinition.h>
#include <AAFDefinitionHome.h>

#include <string>


class AAFXsMapper
{
 public:
  AAFXsMapper( AAFDefinitionHome *defHome );

  AAFClassDefinition* MapAAFClassDef( AxClassDef &axClassDef );

  AAFTypeDefinition* MapAAFTypeDef( AxTypeDef &axTypeDef );

  AAFPropertyDefinition* MapAAFPropertyDef( AAFClassDefinition *classDef, AxClassDef &axClassDef,
					    AxPropertyDef &axPropertyDef );

 private:
  AAFDefinitionHome *_defHome;

};



#endif //__AAFXSMAPPER_H__
