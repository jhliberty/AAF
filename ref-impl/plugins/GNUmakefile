###############################################################################
#
# $Id: GNUmakefile,v 1.25 2009/06/01 11:47:00 stuart_hc Exp $ $Name: V116 $
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

PACKAGE = ref-impl

AAFBASE ?= ../..


# Common definitions
include $(AAFBASE)/build/common.mk



# for access to the VC3Codec SDK
# please visit http://www.avid.com/DNxHD
# or contact DNxHDSupport@avid.com

VC3CODECINC = -I$(CODECS_ROOT)/VC3Codec/Source
VC3CODECLIB = $(CODECS_ROOT)/VC3Codec/Source/DNxHDCodec$(LIBEXT)

# CFLAGS += -DUSE_VC3_CODEC 
# OPT_CODEC_INCS += $(VC3CODECINC)
# OPT_CODEC_LIBS += $(VC3CODECLIB)


# for access to the Avid DNxHD Codec SDK
# please visit http://www.avid.com/DNxHD
# or contact DNxHDSupport@avid.com

DNxHDCODECINC = -I$(CODECS_ROOT)/DNxHDCodecSDK/include
DNxHDCODECLIB = $(CODECS_ROOT)/DNxHDCodecSDK/linux32/lib/OpenSuse-10_3/DNxHDCodec$(LIBEXT)

# experiment
DNxHDCODECOPT = -L$(CODECS_ROOT)/DNxHDCodecSDK/linux32/lib/OpenSuse-10_3 		\
		-Wl,-Bstatic -Wl,-lDNxHDCodec -Wl,-lpthread -Wl,-Bdynamic 		\
		-Wl,--warn-unresolved-symbols -Wl,--unresolved-symbols=report-all

# CFLAGS += -DUSE_DNxHD_CODEC
# OPT_CODEC_INCS += $(DNxHDCODECINC)
# OPT_CODEC_LIBS += $(DNxHDCODECLIB)

# Include directories
INCLUDES = -I. \
		-I../include \
		-I../include/ref-api \
		-I../src/impl \
		-I../src/com-api \
		-I../src/com-api/com-dll \
		-I../libjpeg \
		$(OPT_CODEC_INCS) \
		-I../include/com-api


CXXFILES = AAFPluginCOM.cpp \
		AAFPluginUtils.cpp \
		CAAFEssenceDataStream.cpp \
		CAAFEssenceFileStream.cpp \
		CAAFEssenceFileContainer.cpp \
		CAAFEssenceRIFFWAVEStream.cpp \
		CAAFEssenceRIFFWAVEContainer.cpp \
		CAAFJPEGCodec.cpp \
		CAAFJPEGDescriptorHelper.cpp \
		CAAFCDCICodec.cpp \
		CAAFCDCIDescriptorHelper.cpp \
		CAAFWAVECodec.cpp \
		CAAFAIFCCodec.cpp \
		CAAFPCMCodec.cpp \
		CAAFDNxHDCodec.cpp \
		CAAFVC3Codec.cpp \
		jpegesdatadst.cpp \
		jpegesdatasrc.cpp


SUPOBJS = $(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFClassFactory$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFServer$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFUnknown$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/CAAFInProcServer$(OBJ) \
		$(AAFBUILDDIR)/com-api/$(AAFTARGETDIR)/utf8$(OBJ)


.PHONY : all
all : aafpgapi aafintp


DYN_PGAPI_LIB = $(LIBDIR)/libaafpgapi$(DLL)
DYN_INTP_LIB = $(LIBDIR)/libaafintp$(DLL)
PGAPI_DEF=
INTP_DEF=

ifeq ($(AAFPLATFORM),Win)
  # For historical reasons the DLLs have different names on MS Windows
  DYN_PGAPI_LIB = $(LIBDIR)/AAFPGAPI$(DLL)
  DYN_INTP_LIB = $(LIBDIR)/AAFINTP$(DLL)

  # For MS Windows the linker must be passed the .DEF file otherwise the DLL
  # will not contain the expected exported symbols.
  PGAPI_DEF = $(LD_DEF)AafPlugincom.def
  INTP_DEF = $(LD_DEF)AAFINTP.DEF

  ifeq ($(CC),g++)
    # g++ on Windows needs extra libs for IID_IUnknown and CoCreateGuid symbols
    PGAPI_DEF += -lole32 -luuid
    INTP_DEF += -lole32 -luuid
  endif
endif

STAT_PGAPI_LIB = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libaafpgapi$(LIBEXT)
STAT_INTP_LIB = $(AAFBUILDDIR)/ref-impl/$(AAFTARGETDIR)/libaafintp$(LIBEXT)

# Default targets are dynamic libraries
PGAPI_LIB = $(DYN_PGAPI_LIB)
INTP_LIB = $(DYN_INTP_LIB)
ifeq ($(AAFTARGET),Debug-static)
	PGAPI_LIB = $(STAT_PGAPI_LIB)
	INTP_LIB = $(STAT_INTP_LIB)
endif

JPEGLIB = $(LIBDIR)/libjpeg$(LIBEXT)

.PHONY : aafpgapi
aafpgapi : $(OBJDIR) $(LIBDIR) $(PGAPI_LIB)

.PHONY : aafintp
aafintp : $(OBJDIR) $(LIBDIR) $(INTP_LIB)

$(STAT_PGAPI_LIB) : $(CXXOBJS)
	$(LD_STAT_LIB) $(LD_STAT_OUTPUT) $(CXXOBJS)


$(DYN_PGAPI_LIB) : $(CXXOBJS)
	$(LD_DYN_LIB) \
	$(SUPOBJS) $(CXXOBJS) $(LD_DYN_OUTPUT) $(PGAPI_DEF) \
	$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIBEXT) \
	$(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR)/libaafiid$(LIBEXT) \
	$(JPEGLIB) \
	$(OPT_CODEC_LIBS)


CXXFILES2 = AAFInterpCOM.cpp \
		AAFRational.cpp \
		CAAFBasicInterp.cpp


$(STAT_INTP_LIB) : $(CXXOBJS2)
	$(LD_STAT_LIB) $(LD_STAT_OUTPUT) $(CXXOBJS2)

$(DYN_INTP_LIB) : $(SUPOBJS) $(CXXOBJS2)
	$(LD_DYN_LIB) \
	$(SUPOBJS) $(CXXOBJS2) $(LD_DYN_OUTPUT) $(INTP_DEF) \
	$(AAFBUILDDIR)/aaflib/$(AAFTARGETDIR)/libaaflib$(LIBEXT) \
	$(AAFBUILDDIR)/aafiid/$(AAFTARGETDIR)/libaafiid$(LIBEXT)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS) $(CXXOBJS2)
	@$(RM) $(DEPS)
	@$(RM) $(PGAPI_LIB)
	@$(RM) $(INTP_LIB)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
