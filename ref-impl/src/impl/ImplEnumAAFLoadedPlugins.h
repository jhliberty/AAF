//@doc
//@class    EnumAAFLoadedPlugins | Implementation class for EnumAAFLoadedPlugins
#ifndef __ImplEnumAAFLoadedPlugins_h__
#define __ImplEnumAAFLoadedPlugins_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplEnumAAFLoadedPlugins.h,v 1.11 2009/06/01 11:47:09 stuart_hc Exp $ $Name: V116 $
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
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFPluginDef;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "aafTable.h"

class ImplAAFPluginManager;

class ImplEnumAAFLoadedPlugins : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFLoadedPlugins ();

protected:
  virtual ~ImplEnumAAFLoadedPlugins ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next AAFPluginDescriptor
        (aafUID_t * ppAAFPluginDescriptor);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of AAFPluginDef requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive AAFPluginDescriptors
         aafUID_t *ppAAFPluginDesc,

         // @parm [out,ref] number of actual AAFPluginDescriptor fetched into ppAAFPluginDesc array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        // @parm [out,retval] new enumeration
        (ImplEnumAAFLoadedPlugins ** ppEnum);


public:
// Private to the SDK
	virtual AAFRESULT SetCategory(aafUID_t *category);

private:
	aafUID_t				_category;
	ImplAAFPluginManager	*_manager;
	aafTableIterate_t		_tableIter;
	aafBool					_isFirst;
};

#endif // ! __ImplEnumAAFLoadedPlugins_h__


