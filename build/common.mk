###############################################################################
#
# $Id: common.mk,v 1.24 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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


#----------------------------------------------------------
# Determine the platform we're building on and include
# platfrom specific definitions.
# N.B. Requires GNU Make for the $(shell ...) command -
# see http://www.gnu.org/software/make/make.html
#----------------------------------------------------------
ifndef AAFPLATFORM
    AAFPLATFORM = $(shell $(AAFBASE)/build/aafplatform.sh)
endif

include $(AAFBASE)/build/pdefs-$(AAFPLATFORM).mk


#----------------------------------------------------------
# AAFBUILDDIR is the directory where all the binaries will 
# be placed. It's located in AAF toolkit directory and has
# two components: platform name generated above and 
# compiler name defined in the platfrom specific 
# definitions file.
#----------------------------------------------------------
AAFBUILDDIR = $(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(COMPILER)

#----------------------------------------------------------
# STORAGE_LIBS - Structured Storage Libraries.
# Using wildcards, find all available structured storage
# libraries and add them to STORAGE_LIBS.
#----------------------------------------------------------

STORAGE_LIBS=
LINK_STG=

ifndef AAF_NO_STRUCTURED_STORAGE

# check for SchemaSoft implementation libSSRW2C.a
ifeq ($(wildcard $(AAFBUILDDIR)/sss-impl/libSSRW2C$(LIBEXT)),$(AAFBUILDDIR)/sss-impl/libSSRW2C$(LIBEXT))
	STORAGE_LIBS += $(AAFBUILDDIR)/sss-impl/libSSRW2C$(LIBEXT)
	LINK_STG += -L$(AAFBUILDDIR)/sss-impl -lSSRW2C
	ADD_CFLAGS += -DOM_USE_SCHEMASOFT_SS
	USE_SS=1
endif

# On Windows the layout is /sss-impl/ssrw_c.lib (for historical reasons)
ifeq ($(wildcard $(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(MSVCVERSION)/sss-impl/ssrw_c$(LIBEXT)),$(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(MSVCVERSION)/sss-impl/ssrw_c$(LIBEXT))
	STORAGE_LIBS += $(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(MSVCVERSION)/sss-impl/ssrw_c$(LIBEXT)
	LINK_STG += -L$(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(MSVCVERSION)/sss-impl -lssrw_c
	ADD_CFLAGS += -DOM_USE_SCHEMASOFT_SS
	USE_SS=1
endif

# Check for Microsoft Structured Storage Reference Implementation
ifeq ($(wildcard $(AAFBUILDDIR)/ss-impl/$(AAFTARGETDIR)/librefstg$(LIBEXT)),$(AAFBUILDDIR)/ss-impl/$(AAFTARGETDIR)/librefstg$(LIBEXT))
	STORAGE_LIBS += $(AAFBUILDDIR)/ss-impl/$(AAFTARGETDIR)/librefstg$(LIBEXT)
	LINK_STG += -L$(AAFBUILDDIR)/ss-impl/$(AAFTARGETDIR) -lrefstg
	ADD_CFLAGS += -DOM_USE_WINDOWS_SS
	USE_SS=1
endif

#--------------------------------------------------------------------------
# Optional libgsf support is enabled by the LIBGSF_PATH variable which must
# contain the path to the installed libgsf includes and library e.g.
# make LIBGSF_PATH=/usr/local
#
# libgsf itself requires glib (for gobject and glib calls) and zlib
#--------------------------------------------------------------------------
ifdef LIBGSF_PATH
    PLATFORMLIBS += -L$(LIBGSF_PATH)/lib -lgsf-1 -lgobject-2.0 -lglib-2.0 -lz
    ADD_CFLAGS += -I$(LIBGSF_PATH)/include -I$(LIBGSF_PATH)/include/glib-2.0 -I$(LIBGSF_PATH)/lib/glib-2.0/include -I$(LIBGSF_PATH)/include/libgsf-1
	ADD_CFLAGS += -DOM_USE_GSF_SS
	USE_SS=1
endif

endif

## We do not have any Structured Storage libraries,
## so compile without Structured Storage
ifndef USE_SS
    ADD_CFLAGS += -DOM_NO_STRUCTURED_STORAGE
else
    ADD_CFLAGS += -DOM_STRUCTURED_STORAGE
endif


# DISABLE_FFMPEG turns off FFMPEG support (useful if FFMPEG_PATH is detected
# but you do not wish to build with FFMPEG support)
ifdef DISABLE_FFMPEG
    FFMPEG_PATH=
endif

#--------------------------------------------------------------------------
# Optional FFmpeg libraries provide DV/DV50/DVCPRO-HD, IMX, and DNxHD codecs.
# If not detected, set FFMPEG_PATH e.g. make FFMPEG_PATH=/usr/local/lib
#
# FFMPEG_EXTRA_LIBS should be set when your FFmpeg libraries require extra
# libraries at link time e.g.
#   FFMPEG_EXTRA_LIBS='-lz -lbz2 -lmp3lame -lx264 -lfaac -lfaad -lm -lpthread'
#
# FFmpeg codecs are used in plugin codecs (ref-impl/plugins/CAAFCDCICodec.cpp)
#--------------------------------------------------------------------------
ifdef FFMPEG_PATH
    ifneq "$(FFMPEG_PATH)" "/usr/lib"
        ADD_CFLAGS += -I$(FFMPEG_PATH)/include
        OPT_CODEC_LIBS += -L$(FFMPEG_PATH)
    endif
    ADD_CFLAGS += -DUSE_FFMPEG
    OPT_CODEC_LIBS += -lavformat -lavcodec -lavutil $(FFMPEG_EXTRA_LIBS)
endif


#----------------------------------------------------------
# Define AAFTARGET and related variables.
# There are three supported build types: Debug (default) 
# Debug-static (as statically linked Debug build) and
# Release. To change default setting a user can specify 
# AAFTARGET as a parameter to the make command:
#
#	make AAFTARGET=Release
#	make AAFTARGET=Debug-static
#
#----------------------------------------------------------
# Define default target build
ifndef AAFTARGET
    AAFTARGET = Debug
endif

# Define target build subdirectory.
ifeq ($(AAFTARGET), Debug)
    AAFTARGETDIR = debug
else
    ifeq ($(AAFTARGET), Debug-static)
        AAFTARGETDIR = debug
    else
        AAFTARGETDIR = .
    endif
endif

# Update DBG_FLAGS depending on build target.
ifeq ($(findstring Debug, $(AAFTARGET)), Debug)
    # Add debug flags if -D_DEBUG is not already present
    ifneq ($(findstring -D_DEBUG, $(DBG_FLAGS)), -D_DEBUG)
        DBG_FLAGS += -D_DEBUG -DOM_DEBUG -DOM_STACK_TRACE_ON_ASSERT
    endif
else
ifeq ($(AAFTARGET), Release)
    ifneq ($(findstring -DNDEBUG, $(DBG_FLAGS)), -DNDEBUG)
        DBG_FLAGS = -DNDEBUG $(REL_FLAGS)
    endif
endif
endif

ifeq ($(AAFTARGET), Debug-static)
    DBG_FLAGS += -DDISABLE_DYNAMIC_LOADING
endif

#----------------------------------------------------------
# AAFSDK controls the directories where SDK libraries and 
# includes will be copied and applications (tests,examples,
# utils) will be built.
# The default setting is the AAF tollkit build directory.
# Common alternatives include:
#	AAFSDK=/usr/local
#	AAFSDK=/opt/AAF
#----------------------------------------------------------
ifndef AAFSDK
    AAFSDK = $(AAFBUILDDIR)
endif
AAFSDKINCLUDEDIR  = $(AAFSDK)/include
AAFSDKBINDIR      = $(AAFSDK)/bin/$(AAFTARGETDIR)
AAFSDKLIBDIR      = $(AAFSDK)/lib/$(AAFTARGETDIR)


#----------------------------------------------------------
# Put compiler specific flags in CFLAGS
# CFLAGS components are defined in platform-specific 
# definitions file pdefs-$(AAFPLATFORM).mk
#----------------------------------------------------------
CFLAGS = $(DBG_FLAGS) $(ADD_CFLAGS) $(PLATFORM_CFLAGS)

# U_OPTS should be defined as whether use_unicode or 
# no_unicode in platform-specific .def file
ifeq ($(U_OPTS), use_unicode)
    CFLAGS += -D_UNICODE
endif


#----------------------------------------------------------
# Optional. RPATH adds a directory to the runtime library search path.
# Used when linking an AAF client program (executable).
# It allows the libraries to be found quickly at runtime.
# By default it depends on AAFBASE value, but user can 
# set this variable to point to the directory where AAF 
# libraries are installed.
# For example:
#
#	make RPATH=/usr/local/lib/aaf
#
#----------------------------------------------------------
# Set RPATH only if it was never defined
ifeq ($(origin RPATH), undefined)
    RPATH = $(AAFSDKBINDIR)
endif


#----------------------------------------------------------
# Dependencies generation flag.
# If it's not 0 make will generate dependency files.
# By default dependencies generation is turned on. To
# change this behaviour use:
#
#       make GENDEPS=0
#
#----------------------------------------------------------
ifndef GENDEPS
    GENDEPS = 1
endif


PACKDIR := $(AAFBUILDDIR)/$(PACKAGE)
OBJDIR := $(PACKDIR)/$(AAFTARGETDIR)
LIBDIR := $(OBJDIR)

CPP_EXTENSION ?= cpp

HEADERS = $(wildcard, *.{hxx,h})
CXXFILES = $(wildcard *.$(CPP_EXTENSION))
CFILES = $(wildcard *.c)
CXXFILES2 = 


SOURCES = $(CXXFILES) $(CFILES)
CXXOBJS1 = $(SOURCES:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))
CXXOBJS = $(CXXOBJS1:%.c=$(OBJDIR)/%$(OBJ))
CXXOBJS2A = $(CXXFILES2:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))
CXXOBJS2 = $(CXXOBJS2A:%.c=$(OBJDIR)/%$(OBJ))

DEPS_TMP = $(SOURCES:%.$(CPP_EXTENSION)=$(OBJDIR)/%.d)
DEPS = $(DEPS_TMP:%.c=$(OBJDIR)/%.d)


#----------------------------------------------------------
# LINK_AAF_APP - Link line args to produce an AAF client
# application which dynamically loads the AAF libraries.
#----------------------------------------------------------
ifeq ($(AAFTARGET), Debug-static)
	# Link line used for static builds pulls in all required
	# static libraries to achieve a statically linked executable.
    LINK_AAF_APP ?= -L$(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR) -L$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR) -L$(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR) -L$(AAFBUILDDIR)/OM/$(AAFTARGETDIR) -lcom-api -limpl -lom -laaflib -laafiid $(PLATFORMLIBS) $(LIBCIO) $(LINK_STG) $(UUIDLIB) -o $@
else
	# AAF client applications need only link to aaflib and aafiid
    LINK_AAF_APP ?= -L$(AAFSDKLIBDIR) -laaflib -laafiid $(RPATH_OPT) $(PLATFORMLIBS) $(LIBCIO) -o $@
endif

#----------------------------------------------------------
# APP_RUN_ENV - Command to setup environment for running
# an AAF application from the command line (by setting up
# paths to dynamically loaded AAF libraries.
#----------------------------------------------------------
APP_RUN_ENV ?= env LD_LIBRARY_PATH=$(AAFSDKBINDIR):$(LD_LIBRARY_PATH)
