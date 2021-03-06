###############################################################################
#
# $Id: GNUmakefile,v 1.27 2009/06/01 11:47:04 stuart_hc Exp $ $Name: V116 $
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

PACKAGE = com-api
export PACKAGE

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk

# Irix debug builds require the Cio library in addition
# to the regular platform libs.
ifeq ($(AAFPLATFORM),MipsIrix)
ifeq ($(AAFTARGET),Debug)
PLATFORMLIBS+=-lCio
endif
endif

# Include directories
INCLUDES = -I../../include/ref-api \
	   -I../../include/OM \
	   -I../../src/OM \
	   -I../../include \
	   -Icom-dll \
	   -I../../../include/platform \
	   -I../impl \
	   -I.. -I. \
	   -I../../aaflib


DYN_LIBTARGET = $(LIBDIR)/libcom-api$(DLL)
AAFCOM_DEF =
ifeq ($(AAFPLATFORM),Win)
	# For historical reasons the DLL has a different name on MS Windows
	DYN_LIBTARGET = $(LIBDIR)/AAFCOAPI$(DLL)

	# For MS Windows the linker must be passed the .DEF file otherwise the DLL
	# will not contain the expected exported symbols.
	AAFCOM_DEF = $(LD_DEF)com-dll/AAFCOM.DEF
endif

STAT_LIBTARGET = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libcom-api$(LIBEXT)

LIBTARGET=$(DYN_LIBTARGET)		# Default is dynamic library
ifeq ($(AAFTARGET),Debug-static)
	LIBTARGET = $(STAT_LIBTARGET)
endif

ifeq ($(AAFPLATFORM),PPCDarwin)
PLATFORMLIBS += -framework CoreFoundation
endif

# AAF-XML stored format support requires the expat library which is
# bundled with the AAF SDK.
EXPAT_LIB = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libexpat$(LIBEXT)


# The following two lines initialize CXXFILES2 to contain the list
# of source files from ./com-dll directory.
CXXFILES_TMP = $(wildcard com-dll/*.$(CPP_EXTENSION)) $(wildcard com-dll/*.c)
CXXFILES2 = $(CXXFILES_TMP:com-dll/%=%)


.PHONY : all
all : com-api


.PHONY : com-api
com-api : $(OBJDIR) $(LIBDIR) com-dll $(LIBTARGET)


.PHONY : com-dll
com-dll :
	cd com-dll && $(MAKE)


# Under MSVC compilers, a different version of STORAGE_LIBS must be used for each
# compiler version to avoid undefined behaviour at runtime.
ifeq ($(COMPILER),cl)
  MSVC_VER = $(shell cl 2>&1 | head -1 | sed -e 's/.*Version //' -e 's/\..*//')
  ifeq ($(MSVC_VER),14)
    # 13 means vs7, 14 means vs8
    STORAGE_LIBS = $(AAFBASE)/AAF$(AAFPLATFORM)SDK/vs8/sss-impl/ssrw_c$(LIBEXT)
  endif
endif

# For a static build, simply archive the object files into a static library.
# The static library is intended for debugging only.
$(STAT_LIBTARGET) : $(CXXOBJS) $(CXXOBJS2)
	$(LD_STAT_LIB) $(LD_STAT_OUTPUT) $(CXXOBJS) $(CXXOBJS2)


# The dynamic library must link in all necessary ref-impl code
# and the static expat library
$(DYN_LIBTARGET) : $(CXXOBJS) $(CXXOBJS2) \
				$(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libimpl$(LIBEXT) \
				$(AAFBUILDDIR)/OM/$(AAFTARGETDIR)/libom$(LIBEXT) $(STORAGE_LIBS)

	$(LD_DYN_LIB) $(CXXOBJS) $(CXXOBJS2) $(AAFCOM_DEF) $(LD_DYN_OUTPUT) \
	$(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libimpl$(LIBEXT) \
	$(AAFBUILDDIR)/OM/$(AAFTARGETDIR)/libom$(LIBEXT) \
	$(STORAGE_LIBS) \
	$(EXPAT_LIB) \
	$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIBEXT) \
	$(UUIDLIB) $(PLATFORMLIBS)

.PHONY : clean
clean:
	cd com-dll && $(MAKE) GENDEPS=0 $@
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(LIBTARGET)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean
	cd com-dll && $(MAKE) GENDEPS=0 $@


include $(AAFBASE)/build/rules.mk
