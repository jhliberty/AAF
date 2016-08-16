###############################################################################
#
# $Id: pdefs-SparcSolaris.mk,v 1.9 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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

#------------------------------------------------------------------------------
#
# pdefs-SparcSolaris.mk
#
#	This file contains makefile definitions for the Linux platform.
#
#
# Uses:
#	COMPILER, XL, RPATH, CC
#	
# Requires:
#	XL, RPATH, CC
#
# Sets:
#	MIPS_ABI, COMPILER, PLATFORM_CFLAGS, RPATH_OPT, 
#	LD, LD_STAT_LIB, LD_DYN_LIB, U_OPTS, OBJ, EXE, LIBEXT, DLL,
#	UUIDLIB
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Include common Unix definitions.
#------------------------------------------------------------------------------
include $(AAFBASE)/build/pdefs-Unix.mk


#------------------------------------------------------------------------------
# Compiler-specific definitions
#------------------------------------------------------------------------------
COMPILER ?= g++
include $(AAFBASE)/build/cdefs-$(COMPILER).mk


#------------------------------------------------------------------------------
# Platform specific compiler options
#------------------------------------------------------------------------------
PLATFORM_CFLAGS = -fPIC


# Platform specific compiler options
# Large File Support definitions (see libc documentation)
#
# _LARGEFILE_SOURCE   - declares the functions fseeko() and ftello()
# _LARGEFILE64_SOURCE - enables 32 bit systems to use files larger than 2GB.
# _FILE_OFFSET_BITS   - determines which file system interface shall be used.
PLATFORM_CFLAGS += -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

# Platform specific compiler options
#
# When using glibc-2.1.2 defining _LARGEFILE_SOURCE (as libc documentation
# suggests) is not enough to get fseeko() and ftello() declared. To have
# fseeko/ftello we must request Single Unix Specification, version 2
# definitions which is done by defining the macro _XOPEN_SOURCE having the
# value of 500.
# Note that for the later versions of the library, like glibc-2.2.2 for
# example, _LARGEFILE_SOURCE will do the job.
PLATFORM_CFLAGS += -D_XOPEN_SOURCE=500


#------------------------------------------------------------------------------
# Linker command and options
#------------------------------------------------------------------------------
# Option to place library search path in executable for use by runtime linker
RPATH_OPT = $(XL)-R $(XL)$(RPATH)

# Command to link executable.
LD = $(CC) -g -fPIC -ldl
LD_STAT_OUTPUT = $@
LD_DYN_OUTPUT = -o $@

# Command to link static library
ifndef LD_STAT_LIB
    # Note: CC is invoked here to use IRIX specific 
    # compiler option -ar.
    LD_STAT_LIB = ar -cr 
endif

# Command to link dynamic library
ifndef LD_DYN_LIB
    LD_DYN_LIB = $(CC) -fPIC -shared -g -D_DEBUG -lc
endif

# UUID library to use
ifndef UUIDLIB
    UUIDLIB =
endif


#------------------------------------------------------------------------------
# Select UNICODE or ansi API's:
#   U_OPTS=use_unicode
#   U_OPTS=no_unicode
#------------------------------------------------------------------------------
U_OPTS=no_unicode


#------------------------------------------------------------------------------
# Binary file extensions
#------------------------------------------------------------------------------
OBJ ?= .o
EXE ?= 
LIBEXT ?= .a
DLL ?= .so


