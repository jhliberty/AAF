//@doc
//@class    AAFPluginManager | Implementation class for AAFPluginManager
#ifndef __ImplAAFPluginManager_h__
#define __ImplAAFPluginManager_h__


//=---------------------------------------------------------------------=
//
// $Id: ImplAAFPluginManager.h,v 1.17 2009/06/01 11:47:07 stuart_hc Exp $ $Name: V116 $
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



class ImplEnumAAFLoadedPlugins;
class ImplAAFPluginDef;
class ImplAAFDictionary;
class ImplAAFDefObject;
class ImplAAFSourceMob;
class ImplAAFEssenceStream;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


#include "AAFPlugin.h"
#include "aafTable.h"

// Implemenent a visitor pattern for plugins with a given definition id.
// Used with ForEachPluginWithDefinitionDo.
typedef aafBool	(*AAFDOTOPLUGINWITHDEFINTIONPROC)(
  aafUID_constref defID, 
  REFCLSID clsid, 
  void *refCon);



class ImplAAFDictionary;

class ImplAAFPluginManager : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPluginManager ();

protected:
  virtual ~ImplAAFPluginManager ();

public:


  //****************
  // EnumLoadedPlugins()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumLoadedPlugins
        (// @parm [in] Which category ID do we want to enumerate
         aafUID_t  categoryID,

         // @parm [out,retval] Loaded Plugin Enumeration
         ImplEnumAAFLoadedPlugins ** ppEnum);



public:
	//Toolkit internal only
static ImplAAFPluginManager *GetPluginManager(void);

// Implemenent a visitor pattern for plugins with a given definition id.
AAFRESULT ForEachPluginWithDefinitionDo(
  aafUID_constref defID, 
  AAFDOTOPLUGINWITHDEFINTIONPROC proc, 
  void *refCon);

bool FindPluginFromDefinition(
  aafUID_constref	pluginID,
  CLSID& clsid);

// Create an object contained within one of the loaded plugin files.
AAFRESULT CreateInstance(
			REFCLSID rclsid, 
			IUnknown* pUnkOuter, 
			REFIID riid, 
			void ** result);

AAFRESULT CreateInstanceFromDefinition(
			aafUID_constref		pluginID,
			IUnknown* pUnkOuter, 
			REFIID riid, 
			void ** result);

AAFRESULT GetPluginInstance(
			aafUID_t		pluginID,
			IAAFPlugin		**result);

AAFRESULT MakeCodecFromEssenceDesc(
			ImplAAFSourceMob		*fileMob,
			IAAFEssenceStream		*stream,
			IAAFEssenceCodec		**codec);

// Attempt to register all of the plugin files in the installation directory.
AAFRESULT RegisterSharedPlugins(void);

// Attempt to register all of the plugin files in the given directory.
AAFRESULT RegisterPluginDirectory(const char *directoryName);
AAFRESULT RegisterPluginDirectory(aafCharacter_constptr wdirectoryName);

// Attempt to register all of the plugins in the given file.
AAFRESULT RegisterPluginFile(const char *fileName);
AAFRESULT RegisterPluginFile(aafCharacter_constptr wfileName);

// Associate a factory object with a code class id.
AAFRESULT RegisterPluginFactory(
			REFCLSID rclsid,
      IClassFactory *pFactory);

AAFRESULT RegisterPlugin(
			CLSID		plugin);

// Attempt to unregister all plugins. This may fail if any plugins
// are still loaded. This method should only be called after all
// files have been closed, all client created plugins have been
// released but before AAFUnload() has been called.
AAFRESULT UnregisterAllPlugins(void);

AAFRESULT Init(void);

// Internal to the toolkit functions
AAFRESULT GetFirstLoadedPlugin (aafUID_t category, aafTableIterate_t *iter, aafUID_t *pDesc);
AAFRESULT GetNextLoadedPlugin (aafUID_t category, aafTableIterate_t *iter, aafUID_t *pDesc);

AAFRESULT CreatePluginDefinition (aafUID_t  pluginDefID,
        ImplAAFDictionary * pDictionary,
        IAAFDefObject ** pPluginDesc);

private:
  aafTable_t	*_pluginFiles;
  aafTable_t  *_pluginFactories;
	aafTable_t	*_plugins;
	aafTable_t	*_codecDesc;
};

#endif // ! __ImplAAFPluginManager_h__

//!!! Clean out below here
// The implementation of this class should contain a structure like:
//typedef struct
//{
//	aafPluginType_t			type;
//	AAFEssenceCodec			*dispatch;
//	aafCodecMetaInfo_t		metaInfo;
//} codecTableEntry_t;
