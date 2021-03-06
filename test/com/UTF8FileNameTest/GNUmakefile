###############################################################################
#
# $Id: GNUmakefile,v 1.9 2009/06/01 11:47:15 stuart_hc Exp $ $Name: V116 $
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

PACKAGE = Test

AAFBASE ?= ../../..


# Common definitions
include $(AAFBASE)/build/common.mk


# Include directories
# utf8.h is temporarily included from ref-impl (once the code duplication
# problem is solved this include will be removed).
INCLUDES = -I$(AAFSDKINCLUDEDIR) \
			-I../../../ref-impl/include


.PHONY : all
all : $(OBJDIR) $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)

$(AAFSDKBINDIR)/UTF8FileNameTest$(EXE) : $(CXXOBJS)
	$(LD) $(CXXOBJS) $(LINK_AAF_APP)


.PHONY : check
check : $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)
	$(APP_RUN_ENV) $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)

.PHONY : valgrind-check
valgrind-check : $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)
	$(APP_RUN_ENV) $(VALGRIND_CMD) $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)


.PHONY : clean
clean :
	@$(RM) $(CXXOBJS)
	@$(RM) $(DEPS)
	@$(RM) $(AAFSDKBINDIR)/UTF8FileNameTest$(EXE)
	@$(RMDIR) $(OBJDIR)/ii_files


.PHONY : realclean
realclean : clean


include $(AAFBASE)/build/rules.mk
