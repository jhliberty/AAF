###############################################################################
#
# $Id: GNUmakefile,v 1.19 2012/07/06 21:34:50 jptrainor Exp $ $Name: V116 $
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


AAFBASE ?= .

#
# Common definitions
#
include $(AAFBASE)/build/common.mk


#
#  Default target to build SDK. It assumes that dodo generated
#  files already exist. If they don't or dodo sources have
#  been changed use 'cd dodo && make'.
#  'make everything' must be used to build SDK and applications.
#
.PHONY : all
all : ref-impl


.PHONY : everything
everything : install examples devutils utilities test


.PHONY : ref-impl
ref-impl :
	cd ref-impl && $(MAKE)


.PHONY : install
install : ref-impl
	$(MAKE) -f unixaafsdk.mak CFG=$(AAFTARGET)

.PHONY : examples
examples : install
	cd examples && $(MAKE) 

.PHONY : examples2
examples2 : install
	cd examples2 && $(MAKE) 

.PHONY : devutils
devutils : install
	cd DevUtils && $(MAKE)

.PHONY : utilities
utilities : install examples2
	cd Utilities && $(MAKE)

.PHONY : test
test : install
	cd test && $(MAKE)

.PHONY : check
check : test
	cd examples2 && $(MAKE) $@
	cd examples && $(MAKE) $@
	cd Utilities && $(MAKE) $@
	cd DevUtils && $(MAKE) $@
	cd test && $(MAKE) $@

.PHONY : valgrind-check
valgrind-check : test
	cd test && $(MAKE) $@

#
# Binary distributions
#

.PHONY : dist
dist :  release-dist sdk-dist

.PHONY : release-dist
release-dist :
	$(MAKE) CFG=Release install
	cd dist && $(MAKE) release-dist

.PHONY : sdk-dist
sdk-dist :
	$(MAKE) CFG=Release install
	$(MAKE) CFG=Debug install
	cd dist && $(MAKE) sdk-dist

# This target simply prints the compiler version.
.PHONY : ccversion
ccversion :
	@$(CCVERSION)

# This target simply prints the build directory.
.PHONY : 
builddir :
	@echo $(AAFBUILDDIR)

#
# Clean up after 'make'
#
.PHONY : clean
clean :
	cd ref-impl && $(MAKE) $@

#
# Clean up after 'make install'
#
.PHONY : uninstall
uninstall :
	$(MAKE) -f unixaafsdk.mak CFG=$(AAFTARGET) clean

#
# Clean up after 'make everything' and 'make dist'
#
.PHONY : realclean
realclean : uninstall
	cd ref-impl && $(MAKE) $@
	cd DevUtils && $(MAKE) $@
	cd Utilities && $(MAKE) $@
	cd examples && $(MAKE) $@
	cd examples2 && $(MAKE) $@
	cd test && $(MAKE) $@
	cd dist && $(MAKE) $@
