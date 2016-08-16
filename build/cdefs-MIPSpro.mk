###############################################################################
#
# $Id: cdefs-MIPSpro.mk,v 1.9 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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
# cdefs-MIPSpro.mk
#
#	This file contains makefile definitions for 
#	the Irix MIPSpro C++ compiler.
#
# Uses:
#	MIPS_ABI -	application binary interface. It's set in including 
#			platform definition file (like pdefs-MipsIrix.mk).
#
# Requires:
#       MIPS_ABI
#
# Sets:
#	CC -		C++ compiler command.
#	ADD_CFLAGS -	compiler options.
#	DBG_FLAGS -	compiler options to generate debug info.
#	XL - 		compiler option to pass an argument to a linker.
#
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Compiler command
# MIPS_ABI is defined in the platform definition file
#------------------------------------------------------------------------------
CC = CC -$(MIPS_ABI)
CCVERSION = CC -version

#------------------------------------------------------------------------------
# Debug info generation
#------------------------------------------------------------------------------
DBG_FLAGS = -g 

#------------------------------------------------------------------------------
# Optimisation flags when building a Release
#------------------------------------------------------------------------------
REL_FLAGS = -O2

#------------------------------------------------------------------------------
# Compiler flags
#
# -no_auto_include	disables implicit inclusion of template 
#			implementation files.
# -prelink		template prelinker is on.
# -xansi		enables ANSI/ISO C.
# -G			see the explanation of -G 0 in 
#			the gp_overflow(5) manpage.
#------------------------------------------------------------------------------
ADD_CFLAGS = -no_auto_include -prelink -xansi -G 0

#------------------------------------------------------------------------------
# Compiler flags
# 
# Make all char declarations to be signed char declarations.
#------------------------------------------------------------------------------
ADD_CFLAGS += -signed -LANG:std


#------------------------------------------------------------------------------
# This option passes its argument to a linker
#------------------------------------------------------------------------------
XL = -Wl,


