###############################################################################
#
# $Id: unixaafsdk.mak,v 1.21 2009/06/01 11:46:43 stuart_hc Exp $ $Name: V116 $
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################

###############################################################################
#                                                                             #
# File: unixaafsdk.mak                                                        #
#                                                                             #
# Purpose: GNU make file that will build the directory structure and copy     #
#   source files for the AAF SDK.                                             #
#                                                                             #
#   This make file should be run after the comapi but before the codecs       #
#   or any other client examples. This make file should be located in the     #
#   AAF-toolkit directory. Otherwise the AAFSDK and AAFTOOLKIT variables      #
#   must be properly defined.                                                 #
#                                                                             #
#                                                                             #
# Supported variables:                                                        #
#   AAFSDK - variable that will contain the location of the aafsdk.           #
#     WARNING: This directory will be removed during the clean.               #
#                                                                             #
#   AAFTOOLKIT - the directory that contains the AAF-toolkit                  #
#                                                                             #
#   CFG=FULL (default)                                                        #
#      make AAFSDK=~/aafsdk -f unixaafsdk.mak                                 #
#     Create a full aafsdk for a drop in the given AAFSDK directory           #
#      The bin directory will contain the release dlls and the lib directory  #
#      will contain the release libraries. The bin/debug will contain the     #
#      debug dlls and libraries.                                              #
#                                                                             #
#   CFG=Release                                                               #
#      make CFG=Release unixaafsdk.mak                                        #
#     Create the sdk in the toolkit/AAFWinSDK/aafsdk directory for testing.   #
#      The bin directory will contain the release dlls and the lib directory  #
#      will contain the release libraries. There will be not bin/debug        #
#      directory if one exists it will be deleted.                            #
#                                                                             #
#   CFG=Debug                                                                 #
#      make CFG=Debug unixaafsdk.mak                                          #
#     Create the sdk in the toolkit/AAFWinSDK/aafsdk directory for testing.   #
#      The bin directory will contain the debug dlls and the lib directory    #
#      will contain the debug libraries. There will be not bin/debug directory#
#      if one exists it will be deleted.                                      #
#                                                                             #
#   CFG=Debug-static                                                          #
#      make CFG=Debug-static unixaafsdk.mak                                   #
#     Same as Debug except binaries will not require dynamic libraries.       #
#                                                                             #
#   USE_IDL                                                                   #
#      Define this variable if you want to use headers generated by IDL	      #
#      compiler (include/com-api). This is default behaviour on Windows.      #
#      On Unix default is tp use reference apit (include/ref-api).            #
#                                                                             #
###############################################################################



all : targets


#
# The path to the AAF-toolkit
#
ifndef AAFTOOLKIT
    AAFTOOLKIT = .
endif


#
# The command to copy given source file over to destination 
# file if they are different.
#
UPDATE = $(AAFTOOLKIT)/update.ksh

#
# Common definitions
#
# Note: AAFBASE is used instead of AAFTOOLKIT in common.mk.
#
AAFBASE = $(AAFTOOLKIT)
include $(AAFTOOLKIT)/build/common.mk


#
# The top level directory of the AAF SDK.
#
ifndef AAFSDK
    AAFSDK = $(AAFBUILDDIR)
endif


#
# The type of AAF SDK Configuration.
#
ifndef CFG
    CFG=FULL
endif

ifeq (, $(findstring $(CFG), FULL Release Debug Debug-static))
	ERROR Unknown configuration
	exit(1)
endif


#
# Directory structure for the AAF SDK
#
AAFSDK_BIN            = $(AAFSDK)/bin
AAFSDK_BIN_DEBUG      = $(AAFSDK_BIN)/debug
AAFSDK_INCLUDE        = $(AAFSDK)/include
AAFSDK_LIB            = $(AAFSDK)/lib
AAFSDK_LIB_DEBUG      = $(AAFSDK_LIB)/debug
AAFSDK_BIN_EXT        = $(AAFSDK_BIN)/aafext
AAFSDK_BIN_DEBUG_EXT  = $(AAFSDK_BIN_DEBUG)/aafext


#
# Toolkit directories from the AAF Development tree
#
TOOLKIT_INCLUDE = $(AAFTOOLKIT)/ref-impl/include
TOOLKIT_INCLUDE_COMAPI = $(TOOLKIT_INCLUDE)/com-api
TOOLKIT_INCLUDE_REFAPI = $(TOOLKIT_INCLUDE)/ref-api
TOOLKIT_PLUGINS = $(AAFTOOLKIT)/ref-impl/plugins
TOOLKIT_REFIMPL_DEBUG = $(AAFBUILDDIR)/ref-impl/debug
TOOLKIT_REFIMPL_RELEASE = $(AAFBUILDDIR)/ref-impl
TOOLKIT_REFIMPL_DEBUG_EXT = $(AAFBUILDDIR)/ref-impl/debug
TOOLKIT_REFIMPL_RELEASE_EXT = $(AAFBUILDDIR)/ref-impl
TOOLKIT_COMAPI_DEBUG = $(AAFBUILDDIR)/com-api/debug
TOOLKIT_COMAPI_RELEASE = $(AAFBUILDDIR)/com-api
TOOLKIT_AAFLIB_DEBUG = $(AAFBUILDDIR)/aaflib/debug
TOOLKIT_AAFLIB_RELEASE = $(AAFBUILDDIR)/aaflib
TOOLKIT_AAFIID_DEBUG = $(AAFBUILDDIR)/aafiid/debug
TOOLKIT_AAFIID_RELEASE = $(AAFBUILDDIR)/aafiid

#
# Target directories
#

# Config-specific dirs
ifeq ($(CFG), Release)
    CONFIG_DIRS = \
       	$(AAFSDK_BIN) \
       	$(AAFSDK_BIN_EXT) \
       	$(AAFSDK_LIB)
endif
ifeq ($(CFG), Debug)
    CONFIG_DIRS = \
       	$(AAFSDK_BIN_DEBUG) \
       	$(AAFSDK_BIN_DEBUG_EXT) \
       	$(AAFSDK_LIB_DEBUG)
endif
ifeq ($(CFG), Debug-static)
    CONFIG_DIRS = \
       	$(AAFSDK_BIN_DEBUG) \
       	$(AAFSDK_BIN_DEBUG_EXT) \
       	$(AAFSDK_LIB_DEBUG)
endif
ifeq ($(CFG), FULL)
    CONFIG_DIRS = \
    	$(AAFSDK_BIN) \
    	$(AAFSDK_BIN_EXT) \
    	$(AAFSDK_LIB) \
    	$(AAFSDK_BIN_DEBUG) \
    	$(AAFSDK_BIN_DEBUG_EXT) \
    	$(AAFSDK_LIB_DEBUG)
endif
ifneq ($(AAFSDK), ".")
    TARGET_DIRS = \
       	$(AAFSDK) \
       	$(CONFIG_DIRS) \
       	$(AAFSDK_INCLUDE)
else
    TARGET_DIRS = \
       	$(CONFIG_DIRS) \
       	$(AAFSDK_INCLUDE)
endif



#
# Target Header files that need to be copied
#
TARGET_H_FILES = \
	$(AAFSDK_INCLUDE)/AAFClassDefUIDs.h \
	$(AAFSDK_INCLUDE)/AAFCodecDefs.h \
	$(AAFSDK_INCLUDE)/AAFCOMPlatform.h \
	$(AAFSDK_INCLUDE)/AAFCOMPlatformTypes.h \
	$(AAFSDK_INCLUDE)/AAFCompressionDefs.h \
	$(AAFSDK_INCLUDE)/AAFContainerDefs.h \
	$(AAFSDK_INCLUDE)/AAFDataDefs.h \
	$(AAFSDK_INCLUDE)/AAFDefUIDs.h \
	$(AAFSDK_INCLUDE)/AAFEssenceFormats.h \
	$(AAFSDK_INCLUDE)/AAFExtEnum.h \
	$(AAFSDK_INCLUDE)/AAFFileKinds.h \
	$(AAFSDK_INCLUDE)/AAFFileMode.h \
	$(AAFSDK_INCLUDE)/AAFInterpolatorDefs.h \
	$(AAFSDK_INCLUDE)/AAFMetaDictionary.h \
	$(AAFSDK_INCLUDE)/AAFOPDefs.h \
	$(AAFSDK_INCLUDE)/AAFOperationCategories.h \
	$(AAFSDK_INCLUDE)/AAFOperationDefs.h \
	$(AAFSDK_INCLUDE)/AAFParameterDefs.h \
	$(AAFSDK_INCLUDE)/AAFPlatform.h \
	$(AAFSDK_INCLUDE)/AAFPluginDefs.h \
	$(AAFSDK_INCLUDE)/AAFPropertyDefs.h \
	$(AAFSDK_INCLUDE)/AAFPropertyIDs.h \
	$(AAFSDK_INCLUDE)/AAFResult.h \
	$(AAFSDK_INCLUDE)/AAFSmartPointer.h \
	$(AAFSDK_INCLUDE)/AAFSmartPointer2.h \
	$(AAFSDK_INCLUDE)/AAFSmartPointerBase.h \
	$(AAFSDK_INCLUDE)/AAFStoredObjectIDs.h \
	$(AAFSDK_INCLUDE)/AAFTypeDefUIDs.h


#
# Target idl files that need to be copied.
#
ifdef USE_IDL
TARGET_IDL_FILES = \
	$(AAFSDK_INCLUDE)/AAFTypes.idl \
	$(AAFSDK_INCLUDE)/AAF.idl \
	$(AAFSDK_INCLUDE)/AAFPlugin.idl \
	$(AAFSDK_INCLUDE)/AAFPluginTypes.idl

else
TARGET_IDL_FILES = 

endif


#
# Target midl files that need to be copied.
#
TARGET_MIDL_FILES = \
	$(AAFSDK_INCLUDE)/AAF.h \
	$(AAFSDK_INCLUDE)/AAFPlugin.h \
	$(AAFSDK_INCLUDE)/AAFPluginTypes.h \
	$(AAFSDK_INCLUDE)/AAFPlugin_i.c \
	$(AAFSDK_INCLUDE)/AAFTypes.h \
	$(AAFSDK_INCLUDE)/AAF_i.c


#
# Release Target Library files that need to be copied
#
RELEASE_LIB_FILES = \
	$(AAFSDK_LIB)/libaafiid$(LIBEXT) \
	$(AAFSDK_LIB)/libaaflib$(LIBEXT)


#
# Debug  Library files that need to be copied
#
DEBUG_LIB_FILES = \
	$(AAFSDK_LIB_DEBUG)/libaafiid$(LIBEXT) \
	$(AAFSDK_LIB_DEBUG)/libaaflib$(LIBEXT)


#
# Target Library files that need to be copied
#
ifeq ($(CFG), Release)
    TARGET_LIB_FILES = $(RELEASE_LIB_FILES)
endif
ifeq ($(CFG), Debug)
    TARGET_LIB_FILES = $(DEBUG_LIB_FILES)
endif
ifeq ($(CFG), Debug-static)
    TARGET_LIB_FILES = $(DEBUG_LIB_FILES)
endif
ifeq ($(CFG), FULL)
    TARGET_LIB_FILES = $(RELEASE_LIB_FILES) $(DEBUG_LIB_FILES)
endif

#
# The names of the dynamic libraries vary for historical reasons
#
DYN_LIB_COMAPI=libcom-api
DYN_LIB_AAFINTP=libaafintp
DYN_LIB_AAFPGAPI=libaafpgapi
ifeq ($(AAFPLATFORM),Win)
    DYN_LIB_COMAPI=AAFCOAPI
    DYN_LIB_AAFINTP=AAFINTP
    DYN_LIB_AAFPGAPI=AAFPGAPI
endif

#
# Release dynamic link libraries.
#
RELEASE_DLL_FILES = \
	$(AAFSDK_BIN)/$(DYN_LIB_COMAPI)$(DLL) \
	$(AAFSDK_BIN_EXT)/$(DYN_LIB_AAFINTP)$(DLL) \
	$(AAFSDK_BIN_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL)

#
# Release dynamic link libraries.
#
DEBUG_DLL_FILES = \
	$(AAFSDK_BIN_DEBUG)/$(DYN_LIB_COMAPI)$(DLL) \
	$(AAFSDK_BIN_DEBUG_EXT)/$(DYN_LIB_AAFINTP)$(DLL) \
	$(AAFSDK_BIN_DEBUG_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL)


#
# Target Dynamic Link Libraries files that need to be copied
#
ifeq ($(CFG), Release)
    TARGET_DLL_FILES = $(RELEASE_DLL_FILES)
endif
ifeq ($(CFG), Debug)
    TARGET_DLL_FILES = $(DEBUG_DLL_FILES)
endif
ifeq ($(CFG), FULL)
    TARGET_DLL_FILES = $(RELEASE_DLL_FILES) $(DEBUG_DLL_FILES)
endif



#
# Target files that need to be cleanup up.
#
TARGET_FILES_TO_REMOVE = \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES) \
	$(TARGET_MIDL_FILES) \
	$(TARGET_LIB_FILES) \
	$(TARGET_DLL_FILES)


#
# Directories to delete
# Note: Order is important: must have child before parent directory.
#
TARGET_DIRS_TO_REMOVE = \
	$(AAFSDK_BIN_DEBUG_EXT) \
	$(AAFSDK_BIN_DEBUG) \
	$(AAFSDK_BIN_EXT) \
	$(AAFSDK_BIN) \
	$(AAFSDK_INCLUDE) \

#
# Main dependency order for all targets
#
SDK_TARGETS = \
	$(TARGET_DIRS) \
	$(TARGET_H_FILES) \
	$(TARGET_IDL_FILES) \
	$(TARGET_MIDL_FILES) \
	$(TARGET_LIB_FILES) \
	$(TARGET_DLL_FILES)


targets : $(SDK_TARGETS)


#
# Dependencies and build rules to create the AAF SDK directory structure.
#
$(AAFSDK) :
	if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi

$(AAFSDK_BIN) :
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi

ifneq ($(AAFSDK_BIN), $(AAFSDK_BIN_EXT))
$(AAFSDK_BIN_EXT) : $(AAFSDK_BIN)
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi
endif

$(AAFSDK_BIN_DEBUG) : $(AAFSDK_BIN)
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi

ifneq ($(AAFSDK_BIN_DEBUG), $(AAFSDK_BIN_DEBUG_EXT))
$(AAFSDK_BIN_DEBUG_EXT) : $(AAFSDK_BIN_DEBUG)
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi
endif

$(AAFSDK_INCLUDE) :
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi

$(AAFSDK_LIB) :
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi

$(AAFSDK_LIB_DEBUG) :
	@if [ ! -d $@ ]; then \
		$(MKDIR) $(MKDIR_OPTS) $@; \
	fi


#
# Dependency and build rules for the Header targets.
#
$(AAFSDK_INCLUDE)/AAFClassDefUIDs.h : $(TOOLKIT_INCLUDE)/AAFClassDefUIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFClassDefUIDs.h $@

$(AAFSDK_INCLUDE)/AAFCodecDefs.h : $(TOOLKIT_INCLUDE)/AAFCodecDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFCodecDefs.h $@

$(AAFSDK_INCLUDE)/AAFCOMPlatform.h : $(TOOLKIT_INCLUDE)/AAFCOMPlatform.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFCOMPlatform.h $@

$(AAFSDK_INCLUDE)/AAFCOMPlatformTypes.h : $(TOOLKIT_INCLUDE)/AAFCOMPlatformTypes.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFCOMPlatformTypes.h $@

$(AAFSDK_INCLUDE)/AAFCompressionDefs.h : $(TOOLKIT_INCLUDE)/AAFCompressionDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFCompressionDefs.h $@

$(AAFSDK_INCLUDE)/AAFContainerDefs.h : $(TOOLKIT_INCLUDE)/AAFContainerDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFContainerDefs.h $@

$(AAFSDK_INCLUDE)/AAFDataDefs.h : $(TOOLKIT_INCLUDE)/AAFDataDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFDataDefs.h $@

$(AAFSDK_INCLUDE)/AAFDefUIDs.h : $(TOOLKIT_INCLUDE)/AAFDefUIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFDefUIDs.h $@

$(AAFSDK_INCLUDE)/AAFEssenceFormats.h : $(TOOLKIT_INCLUDE)/AAFEssenceFormats.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFEssenceFormats.h $@

$(AAFSDK_INCLUDE)/AAFExtEnum.h : $(TOOLKIT_INCLUDE)/AAFExtEnum.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFExtEnum.h $@

$(AAFSDK_INCLUDE)/AAFFileKinds.h : $(TOOLKIT_INCLUDE)/AAFFileKinds.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFFileKinds.h $@

$(AAFSDK_INCLUDE)/AAFFileMode.h : $(TOOLKIT_INCLUDE)/AAFFileMode.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFFileMode.h $@

$(AAFSDK_INCLUDE)/AAFFileSignatures.h : $(TOOLKIT_INCLUDE)/AAFFileSignatures.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFFileSignatures.h $@

$(AAFSDK_INCLUDE)/AAFInterpolatorDefs.h : $(TOOLKIT_INCLUDE)/AAFInterpolatorDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFInterpolatorDefs.h $@

$(AAFSDK_INCLUDE)/AAFMetaDictionary.h : $(TOOLKIT_INCLUDE)/AAFMetaDictionary.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFMetaDictionary.h $@

$(AAFSDK_INCLUDE)/AAFOperationCategories.h : $(TOOLKIT_INCLUDE)/AAFOperationCategories.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFOperationCategories.h $@

$(AAFSDK_INCLUDE)/AAFOPDefs.h : $(TOOLKIT_INCLUDE)/AAFOPDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFOPDefs.h $@
	
$(AAFSDK_INCLUDE)/AAFOperationDefs.h : $(TOOLKIT_INCLUDE)/AAFOperationDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFOperationDefs.h $@

$(AAFSDK_INCLUDE)/AAFParameterDefs.h : $(TOOLKIT_INCLUDE)/AAFParameterDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFParameterDefs.h $@

$(AAFSDK_INCLUDE)/AAFPlatform.h : $(TOOLKIT_INCLUDE)/AAFPlatform.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFPlatform.h $@

$(AAFSDK_INCLUDE)/AAFPluginDefs.h : $(TOOLKIT_INCLUDE)/AAFPluginDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFPluginDefs.h $@

$(AAFSDK_INCLUDE)/AAFPropertyDefs.h : $(TOOLKIT_INCLUDE)/AAFPropertyDefs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFPropertyDefs.h $@

$(AAFSDK_INCLUDE)/AAFPropertyIDs.h : $(TOOLKIT_INCLUDE)/AAFPropertyIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFPropertyIDs.h $@

$(AAFSDK_INCLUDE)/AAFResult.h : $(TOOLKIT_INCLUDE)/AAFResult.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFResult.h $@

$(AAFSDK_INCLUDE)/AAFSmartPointerBase.h : $(TOOLKIT_INCLUDE)/AAFSmartPointerBase.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFSmartPointerBase.h $@

$(AAFSDK_INCLUDE)/AAFSmartPointer.h : $(TOOLKIT_INCLUDE)/AAFSmartPointer.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFSmartPointer.h $@

$(AAFSDK_INCLUDE)/AAFSmartPointer2.h : $(TOOLKIT_INCLUDE)/AAFSmartPointer2.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFSmartPointer2.h $@

$(AAFSDK_INCLUDE)/AAFStoredObjectIDs.h : $(TOOLKIT_INCLUDE)/AAFStoredObjectIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFStoredObjectIDs.h $@

$(AAFSDK_INCLUDE)/AAFTypeDefUIDs.h : $(TOOLKIT_INCLUDE)/AAFTypeDefUIDs.h
	$(UPDATE) $(TOOLKIT_INCLUDE)/AAFTypeDefUIDs.h $@


#
# Currently IDL files are used only on NT and are processed by
# MIDL (Microsoft IDL compiler). Default setting for Unix is not 
# to use MIDL-generated files (include/com-api) but use reference 
# API files (include/ref-api)
#
ifdef USE_IDL

#
# Dependency and build rules for the IDL targets.
#
$(AAFSDK_INCLUDE)/AAFTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)/AAFTypes.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)/AAFTypes.idl $@

$(AAFSDK_INCLUDE)/AAF.idl : $(TOOLKIT_INCLUDE_COMAPI)/AAF.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)/AAF.idl $@

$(AAFSDK_INCLUDE)/AAFModuleTest.idl : $(TOOLKIT_INCLUDE_COMAPI)/AAFModuleTest.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)/AAFModuleTest.idl $@

$(AAFSDK_INCLUDE)/AAFPluginTypes.idl : $(TOOLKIT_INCLUDE_COMAPI)/AAFPluginTypes.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)/AAFPluginTypes.idl $@

$(AAFSDK_INCLUDE)/AAFPlugin.idl : $(TOOLKIT_INCLUDE_COMAPI)/AAFPlugin.idl
	$(UPDATE) $(TOOLKIT_INCLUDE_COMAPI)/AAFPlugin.idl $@

TOOLKIT_INCLUDE_API = $(TOOLKIT_INCLUDE_COMAPI)

else

# Do not USE_IDL
TOOLKIT_INCLUDE_API = $(TOOLKIT_INCLUDE_REFAPI)

endif


#
# Dependency and build rules for the api generated targets.
#
$(AAFSDK_INCLUDE)/AAFTypes.h : $(TOOLKIT_INCLUDE_API)/AAFTypes.h
	$(UPDATE) $(TOOLKIT_INCLUDE_API)/AAFTypes.h $@

$(AAFSDK_INCLUDE)/AAF.h : $(TOOLKIT_INCLUDE_API)/AAF.h
	$(UPDATE) $(TOOLKIT_INCLUDE_API)/AAF.h $@

$(AAFSDK_INCLUDE)/AAF_i.c : $(TOOLKIT_INCLUDE_API)/AAF_i.c
	$(UPDATE) $(TOOLKIT_INCLUDE_API)/AAF_i.c $@

$(AAFSDK_INCLUDE)/AAFPluginTypes.h : $(TOOLKIT_INCLUDE_API)/AAFPluginTypes.h
	$(UPDATE) $(TOOLKIT_INCLUDE_API)/AAFPluginTypes.h $@

$(AAFSDK_INCLUDE)/AAFPlugin.h : $(TOOLKIT_INCLUDE_API)/AAFPlugin.h
	$(UPDATE) $(TOOLKIT_INCLUDE_API)/AAFPlugin.h $@

$(AAFSDK_INCLUDE)/AAFPlugin_i.c : $(TOOLKIT_INCLUDE_API)/AAFPlugin_i.c
	$(UPDATE) $(TOOLKIT_INCLUDE_API)/AAFPlugin_i.c $@


#
# Dependency and build rules for the release library targets.
#
$(AAFSDK_LIB)/libaafiid$(LIBEXT) : $(TOOLKIT_AAFIID_RELEASE)/libaafiid$(LIBEXT)
	$(CP) $(CP_OPTS) $(TOOLKIT_AAFIID_RELEASE)/libaafiid$(LIBEXT) $@

$(AAFSDK_LIB)/libaaflib$(LIBEXT) : $(TOOLKIT_AAFLIB_RELEASE)/libaaflib$(LIBEXT)
	$(CP) $(CP_OPTS) $(TOOLKIT_AAFLIB_RELEASE)/libaaflib$(LIBEXT) $@


#
# Dependency and build rules for the debug library targets.
#
$(AAFSDK_LIB_DEBUG)/libaafiid$(LIBEXT) : $(TOOLKIT_AAFIID_DEBUG)/libaafiid$(LIBEXT)
	$(CP) $(CP_OPTS) $(TOOLKIT_AAFIID_DEBUG)/libaafiid$(LIBEXT) $@

$(AAFSDK_LIB_DEBUG)/libaaflib$(LIBEXT) : $(TOOLKIT_AAFLIB_DEBUG)/libaaflib$(LIBEXT)
	$(CP) $(CP_OPTS) $(TOOLKIT_AAFLIB_DEBUG)/libaaflib$(LIBEXT) $@


#
# Dependency and build rules for the Release DLL targets.
#
$(AAFSDK_BIN)/$(DYN_LIB_COMAPI)$(DLL) : $(TOOLKIT_COMAPI_RELEASE)/$(DYN_LIB_COMAPI)$(DLL)
	$(CP) $(CP_OPTS) $(TOOLKIT_COMAPI_RELEASE)/$(DYN_LIB_COMAPI)$(DLL) $@

$(AAFSDK_BIN_EXT)/$(DYN_LIB_AAFINTP)$(DLL) : $(TOOLKIT_REFIMPL_RELEASE_EXT)/$(DYN_LIB_AAFINTP)$(DLL)
	$(CP) $(CP_OPTS) $(TOOLKIT_REFIMPL_RELEASE_EXT)/$(DYN_LIB_AAFINTP)$(DLL) $@

$(AAFSDK_BIN_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL) : $(TOOLKIT_REFIMPL_RELEASE_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL)
	$(CP) $(CP_OPTS) $(TOOLKIT_REFIMPL_RELEASE_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL) $@

#
# Dependency and build rules for the Debug DLL targets.
#
$(AAFSDK_BIN_DEBUG)/$(DYN_LIB_COMAPI)$(DLL) : $(TOOLKIT_COMAPI_DEBUG)/$(DYN_LIB_COMAPI)$(DLL)
	$(CP) $(CP_OPTS) $(TOOLKIT_COMAPI_DEBUG)/$(DYN_LIB_COMAPI)$(DLL) $@

$(AAFSDK_BIN_DEBUG_EXT)/$(DYN_LIB_AAFINTP)$(DLL) : $(TOOLKIT_REFIMPL_DEBUG_EXT)/$(DYN_LIB_AAFINTP)$(DLL)
	$(CP) $(CP_OPTS) $(TOOLKIT_REFIMPL_DEBUG_EXT)/$(DYN_LIB_AAFINTP)$(DLL) $@

$(AAFSDK_BIN_DEBUG_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL) : $(TOOLKIT_REFIMPL_DEBUG_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL)
	$(CP) $(CP_OPTS) $(TOOLKIT_REFIMPL_DEBUG_EXT)/$(DYN_LIB_AAFPGAPI)$(DLL) $@

#
# The cleanup rules for all targets created by this make file.
#
cleanfiles : 
	@for f in $(TARGET_FILES_TO_REMOVE); do \
		if [ -f $$f ]; then \
			$(ECHO) Removing $$f; \
			$(RM) $$f; \
		fi; \
	done

#
# The cleanup rules for all targets created by this make file.
#
cleandirs :
	@for f in $(TARGET_DIRS_TO_REMOVE); do \
		if [ -d $$f ]; then \
			$(ECHO) Removing $$f; \
			$(RMDIR) $$f; \
		fi; \
	done

#
# Cleanup all target files and directories
#
ifeq ($(AAFSDK), $(AAFBUILDDIR))
MSG = AAFSDK and AAFBUILDDIR are the same
else
MSG = AAFSDK and AAFBUILDDIR are different
endif
clean : cleanfiles 
	@echo $(MSG)
	@echo $(AAFSDK)
	@echo $(AAFBUILDDIR)


rebuildmsg :
	@echo Rebuilding the entire AAF SDK...

rebuildall : rebuildmsg clean targets
	@echo Done rebuilding.
