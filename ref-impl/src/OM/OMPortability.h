//=---------------------------------------------------------------------=
//
// $Id: OMPortability.h,v 1.40 2012/05/16 15:35:11 jptrainor Exp $ $Name: V116 $
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

// @doc OMINTERNAL
#ifndef OMPORTABILITY_H
#define OMPORTABILITY_H

// @module OMPortability | Definitions supporting the portability
//         of the Object Manager.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

// Figure out the compiler and define symbols of the form
// OM_COMPILER_<compiler name>_<processor>_<operating system>
// OM_OS_<operating system>
//
#if defined (_MSC_VER) && defined(_M_IX86) && defined(_WIN32)
#define OM_COMPILER_MSC_INTEL_WINDOWS
#define OM_OS_WINDOWS
#elif defined (_MSC_VER) && defined(_M_X64) && defined(_WIN32)
# define OM_COMPILER_MSC_X64_WINDOWS
# define OM_COMPILER_MSC
# define OM_OS_WINDOWS
#elif defined(__MWERKS__) && defined(__MACH__)
#define OM_COMPILER_MWERKS_PPC_MACOSX
#define OM_OS_MACOSX
#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__)
#define OM_COMPILER_GCC_PPC_MACOSX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(__APPLE__)
#define OM_COMPILER_GCC_INTEL_MACOSX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__x86_64__) && defined(__APPLE__)
#define OM_COMPILER_GCC_X86_64_MACOSX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__mips__) && defined(__sgi__)
#define OM_COMPILER_GCC_MIPS_SGI
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(__linux__)
#define OM_COMPILER_GCC_INTEL_LINUX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__x86_64__) && defined(__linux__)
#define OM_COMPILER_GCC_X86_64_LINUX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__powerpc__) && defined(__linux__)
#define OM_COMPILER_GCC_PPC_LINUX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__powerpc64__) && defined(__linux__)
#define OM_COMPILER_GCC_PPC64_LINUX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(__FreeBSD__)
#define OM_COMPILER_GCC_INTEL_FREEBSD
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(__OpenBSD__)
#define OM_COMPILER_GCC_INTEL_OPENBSD
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(_WIN32)
#define OM_COMPILER_GCC_INTEL_WINDOWS
#define OM_OS_WINDOWS
#elif defined(__GNUC__) && defined(__i386__) && defined(__CYGWIN__)
#define OM_COMPILER_GCC_INTEL_CYGWIN
#define OM_OS_UNIX
#elif defined(mips) && defined(sgi)
#define OM_COMPILER_SGICC_MIPS_SGI
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__sparc__) && defined(__sun__)
#define OM_COMPILER_GCC_SPARC_SUNOS
#define OM_OS_UNIX
#else
#error "Unknown compiler"
#endif

#endif
