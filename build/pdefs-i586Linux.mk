###############################################################################
#
# $Id: pdefs-i586Linux.mk,v 1.19 2011/04/14 14:48:05 philipn Exp $ $Name: V116 $
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
# pdefs-i586Linux.mk
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
PLATFORM_CFLAGS =

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
RPATH_OPT = $(XL)-rpath $(XL)$(RPATH)

# Command to link executable.
LD = $(CC) -ldl -rdynamic
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
    LD_DYN_LIB = $(CC) -shared
endif

# For GNU/Linux the UUID library is provided by uuid-devel, uuid-dev or
# e2fsprogs-devel
ifndef UUIDLIB
    UUIDLIB_PATH := $(shell for f in /usr/local/lib /usr/lib /lib /usr/lib64 /lib64; do test -e $$f/libuuid.a -o -e $$f/libuuid.so && echo $$f && break; done)
    ifeq "$(UUIDLIB_PATH)" ""
        $(error Required library libuuid.a or libuuid.so not found - install uuid-devel, uuid-dev or e2fsprogs-devel)
    endif

    UUIDLIB = -luuid
endif

# Try to detect FFmpeg libraries if FFMPEG_PATH not set
ifndef FFMPEG_PATH
	TMP_FFMPEG_PATH := $(shell for f in /usr/local/lib /usr/lib /lib /usr/lib64 /lib64; do test -e $$f/libavcodec.a && test -e $$f/libavformat.a && echo $$f && break; done)
	ifneq "$(TMP_FFMPEG_PATH)" ""
		FFMPEG_PATH = $(TMP_FFMPEG_PATH)
	endif
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


#------------------------------------------------------------------------------
# Testing tools
#------------------------------------------------------------------------------
VALGRIND_CMD = valgrind --num-callers=100 --leak-check=full
