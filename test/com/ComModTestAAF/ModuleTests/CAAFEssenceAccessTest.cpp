// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceAccess
//=---------------------------------------------------------------------=
//
// $Id: CAAFEssenceAccessTest.cpp,v 1.58 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <iostream>
using namespace std;

#include "AAFPlatform.h"


#include "AAFTypes.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFCompressionDefs.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

#include "CAAFBuiltinDefs.h"

// EssenceTestData.h contains sample audio and video data to use as essence data.
#include "EssenceTestData.h"

// convenient error handlers.
inline void checkResult(HRESULT r)
{
	if (FAILED(r))
		throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
	if (!expression)
		throw r;
}


// This static variables are here so they can be referenced 
// thru out the whole program.

static const aafInt32 STD_SLOT_ID = 1;
static const aafInt32 STD_WIDTH	= 128L;
static const aafInt32 STD_HEIGHT = 96L;
static const aafInt32 PIXELS_PER_FRAME = (STD_WIDTH * STD_HEIGHT);
static const aafInt32 FRAME_BYTES = (PIXELS_PER_FRAME * 3L);
static const aafInt32 SAMPLE_422_BYTES = (PIXELS_PER_FRAME * 2L); 

static const aafUInt32 MAX_SAMPLE_COUNT = 2;
static const aafUInt32 STD_SAMPLE_COUNT = 1;
static const aafInt16 STD_PIXEL_SLOP = 4;
static const aafUInt8 green[] =		{ 0x00, 0xff, 0x00 };

/* This is the value of the YCbCr returned by IJG when decompressing
   compressedJFIF */
static const aafUInt8 yuv_green[] = { 0x96, 0x2c, 0x15 };


static const aafUInt8 compressedJFIF[] =
{
  0xFF,0xD8,0xFF,0xE0,0x00,0x10,0x41,0x56,0x49,0x31,0x00,0x00,0x00,0x00,0x03,0x6A,
  0x00,0x00,0x03,0x6A,0xFF,0xFE,0x00,0x3D,0x41,0x56,0x49,0x44,0x11,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0xFF,0xDD,0x00,0x04,0x00,0x00,0xFF,0xDB,0x00,0x84,0x00,0x08,0x06,
  0x06,0x07,0x06,0x05,0x08,0x07,0x07,0x07,0x09,0x09,0x08,0x0A,0x0C,0x14,0x0D,0x0C,
  0x0B,0x0B,0x0C,0x19,0x12,0x13,0x0F,0x14,0x1D,0x1A,0x1F,0x1E,0x1D,0x1A,0x1C,0x1C,
  0x20,0x24,0x2E,0x27,0x20,0x22,0x2C,0x23,0x1C,0x1C,0x28,0x37,0x29,0x2C,0x30,0x31,
  0x34,0x34,0x34,0x1F,0x27,0x39,0x3D,0x38,0x32,0x3C,0x2E,0x33,0x34,0x32,0x01,0x09,
  0x09,0x09,0x0C,0x0B,0x0C,0x18,0x0D,0x0D,0x18,0x32,0x21,0x1C,0x21,0x32,0x32,0x32,
  0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
  0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
  0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0xFF,
  0xC4,0x01,0xA2,0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,
  0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
  0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x10,0x00,0x02,
  0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,
  0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,
  0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,
  0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,
  0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,
  0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,
  0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,
  0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,
  0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
  0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,0xE3,0xE4,
  0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,
  0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,
  0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,
  0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,
  0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,
  0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,
  0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,
  0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,
  0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,
  0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,
  0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
  0xDA,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
  0xF8,0xF9,0xFA,0xFF,0xC0,0x00,0x11,0x08,0x00,0x60,0x00,0x80,0x03,0x01,0x22,0x00,
  0x02,0x11,0x01,0x03,0x11,0x01,0xFF,0xDA,0x00,0x0C,0x03,0x01,0x00,0x02,0x11,0x03,
  0x11,0x00,0x3F,0x00,0xD6,0xA2,0x8A,0x2B,0xF3,0x43,0xF2,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x00,
  0xA2,0x8A,0x28,0x00,0xA2,0x8A,0x28,0x03,0xFF,0xD9
};


static const aafUInt8 compressed422JFIF[] =
{
  0xFF,0xD8,0xFF,0xE0,0x00,0x10,0x41,0x56,0x49,0x31,0x00,0x00,0x00,0x00,0x03,0x9A,
  0x00,0x00,0x03,0x9A,0xFF,0xFE,0x00,0x3D,0x41,0x56,0x49,0x44,0x11,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0xFF,0xDD,0x00,0x04,0x00,0x00,0xFF,0xDB,0x00,0x84,0x00,0x08,0x06,
  0x06,0x07,0x06,0x05,0x08,0x07,0x07,0x07,0x09,0x09,0x08,0x0A,0x0C,0x14,0x0D,0x0C,
  0x0B,0x0B,0x0C,0x19,0x12,0x13,0x0F,0x14,0x1D,0x1A,0x1F,0x1E,0x1D,0x1A,0x1C,0x1C,
  0x20,0x24,0x2E,0x27,0x20,0x22,0x2C,0x23,0x1C,0x1C,0x28,0x37,0x29,0x2C,0x30,0x31,
  0x34,0x34,0x34,0x1F,0x27,0x39,0x3D,0x38,0x32,0x3C,0x2E,0x33,0x34,0x32,0x01,0x09,
  0x09,0x09,0x0C,0x0B,0x0C,0x18,0x0D,0x0D,0x18,0x32,0x21,0x1C,0x21,0x32,0x32,0x32,
  0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
  0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,
  0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32,0xFF,
  0xC4,0x01,0xA2,0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,
  0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
  0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x10,0x00,0x02,
  0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,
  0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,
  0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,
  0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,
  0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,
  0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,
  0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,
  0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,
  0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
  0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,0xE3,0xE4,
  0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,
  0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,
  0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,
  0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,
  0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,
  0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,
  0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,
  0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,
  0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,
  0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,
  0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
  0xDA,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
  0xF8,0xF9,0xFA,0xFF,0xC0,0x00,0x11,0x08,0x00,0x60,0x00,0x80,0x03,0x01,0x21,0x00,
  0x02,0x11,0x01,0x03,0x11,0x01,0xFF,0xDA,0x00,0x0C,0x03,0x01,0x00,0x02,0x11,0x03,
  0x11,0x00,0x3F,0x00,0xD6,0xA2,0xBF,0x34,0x3F,0x20,0x0A,0x28,0x00,0xA2,0x80,0x0A,
  0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,
  0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,
  0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,
  0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,
  0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,
  0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,
  0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,
  0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,
  0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,
  0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,
  0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,
  0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,
  0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,
  0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x00,0xA2,0x80,
  0x0A,0x28,0x00,0xA2,0x80,0x0A,0x28,0x03,0xFF,0xD9
};

// When tested with horizontalSubsample=1 need 24 bits per pixel, hence multiply by 3
static aafUInt8 uncompressUYVY[STD_WIDTH*STD_HEIGHT*3];

static aafUInt8 *initUncompressedVideo(aafUInt32 horizSubSample, aafUInt32 *bufferSize)
{
	// fill with grey
	memset(uncompressUYVY, 0x80, sizeof(uncompressUYVY));

	if (horizSubSample == 2)
		*bufferSize = sizeof(uncompressUYVY) * 2 / 3;	// 4:2:2 (16 avg bpp)
	else
		*bufferSize = sizeof(uncompressUYVY);			// 4:4:4 (24 avg bpp)
	return uncompressUYVY;
}

static aafUInt8 *readDVframe(aafUInt32 *bufferSize)
{
	*bufferSize = 144000;
	aafUInt8 *buf = new aafUInt8[*bufferSize];

	// Read in a DV-compressed frame
	memcpy(buf, compressedDV_25_625, sizeof(compressedDV_25_625));

	return buf;
}

// The URL conversion routines are also in ref-impl/src/impl/AAFUtils.cpp
// TODO: We need to create a general-purpose library which all tests and
// perhaps all examples have access to instead of duplicating code.

#ifdef _MSC_VER			// MS VC++ dosen't provide POSIX strncasecmp, getcwd
#define strncasecmp(s1, s2, n) _strnicmp(s1, s2, n)
#include <direct.h>
#define getcwd(buf, size) _getcwd(buf, size)
#else
#include <strings.h>	// strncasecmp()
#include <unistd.h>		// getcwd()
#endif

static bool acceptable_pchar(unsigned char c)
{
	static const unsigned char isAcceptable[96] =
	/*	0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xA 0xB 0xC 0xD 0xE 0xF */
	{
	    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF,0xE,0x0,0xF,0xF,0xC, /* 2x  !"#$%&'()*+,-./   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x8,0x0,0x0,0x0,0x0,0x0, /* 3x 0123456789:;<=>?   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF, /* 4x @ABCDEFGHIJKLMNO   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x0,0x0,0x0,0x0,0xF, /* 5x PQRSTUVWXYZ[\]^_   */
	    0x0,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF, /* 6x `abcdefghijklmno   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x0,0x0,0x0,0x0,0x0  /* 7x pqrstuvwxyz{\}~DEL */
	};

	return (c >= 32 && c < 128 && isAcceptable[c - 32]);
}

static void escapeURI(const char *str, char *result)
{
	const char	*p, hex[] = "0123456789ABCDEF";
    char		*q;

    if (!str || !result)
		return;

    for (q = result, p = str; *p; p++)
	{
    	unsigned char a = *p;
		if (!acceptable_pchar(a))
		{
		    *q++ = '%';
		    *q++ = hex[a >> 4];
		    *q++ = hex[a & 15];
		}
		else
			*q++ = *p;
    }
    *q++ = '\0';
}

static char asciiHexToChar (char c)
{
    return  c >= '0' && c <= '9' ?  c - '0' 
    	    : c >= 'A' && c <= 'F'? c - 'A' + 10
    	    : c - 'a' + 10;	/* accept lowercase letters too */
}

// From RFC 1738
//
// <scheme>:<scheme-specific-part>
// ; the scheme is in lower case; interpreters should use case-ignore
//	scheme		= 1*[ lowalpha | digit | "+" | "-" | "." ]
//
// For file scheme:
//	fileurl		= "file://" [ host | "localhost" ] "/" fpath

static void unescapeURI(char *str)
{
    char *p = str;
    char *q = str;

    while (*p)
	{
        if (*p == '%')		// URI hex escape char
		{
			p++;
			if (*p)
				*q = asciiHexToChar(*p++) * 16;
			if (*p)
				*q = *q + asciiHexToChar(*p);
			p++;
			q++;
		}
		else
		    *q++ = *p++; 
    }
    *q++ = 0;
}

static void wcsconvertURLtoFilepath(const wchar_t *url, wchar_t *filepath)
{
	// Convert to char* for ease of processing.
	// (wcsncasecmp and similiar are not available everywhere)
	unsigned tlen = wcslen(url);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstombs(tmp, url, tlen+1);

	// If no file scheme is found, assume a simple filepath and not a URI.
	// Note that other schemes such as http and ftp are not supported.
	if (strncasecmp(tmp, "file://", strlen("file://")) != 0)
	{
		wcscpy(filepath, url);
		delete [] tmp;
		return;
	}

	// Skip over the file://[host]/ to point to the fpath.
	char *fpath = tmp + strlen("file://");
	while (*fpath && *fpath != '/')
		fpath++;

#ifdef _WIN32
	// WIN32 filepaths must start with a drive letter, so remove the
	// initial '/' from the URL.
	if (*fpath == '/')
		fpath++;
#endif

	unescapeURI(fpath);

	mbstowcs(filepath, fpath, strlen(fpath)+1);		// convert back to wcs
	delete [] tmp;
}

static void wcsconvertFilepathtoURL(wchar_t *filepath, wchar_t *url)
{
	// convert to char* for ease of processing
	int tlen = wcslen(filepath);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstombs(tmp, filepath, tlen+1);

#ifdef _WIN32
	// On WIN32 backslash is the directory separator, not a regular filename
	// character like under Unix or in a URL.  So convert them to the URL
	// directory separator, forward slash '/', to preserve the hierarchy.
	char *p = tmp;
	while (*p)
	{
		if (*p == '\\')
			*p = '/';
		p++;
	}
#endif
	// worst case: every char must be hex-escaped - therefore multiply by 3
	char *escaped = new char[tlen*3+1];
	escapeURI(tmp, escaped);

	// prepare the file scheme URL (+1 for '/', +1 for '\0')
	char *mb_url = new char[strlen(escaped) + strlen("file://") +1 +1];
	strcpy(mb_url, "file://");
	if (*escaped != '/')		// ensure a leading path slash is present
		strcat(mb_url, "/");
	strcat(mb_url, escaped);

	mbstowcs(url, mb_url, strlen(mb_url)+1);		// convert back to wcs
	delete [] mb_url;
	delete [] escaped;
	delete [] tmp;
}

// Assumes the file passed in is a relative filepath to the current
// directory e.g. "test.aaf".
static aafWChar *makeURLfromFileInCwd(const aafWChar *file, aafWChar *url)
{
	char		cwd[FILENAME_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return NULL;
	}

	aafWChar	wide_cwd[FILENAME_MAX];
	mbstowcs(wide_cwd, cwd, strlen(cwd)+1);

	// Append file to cwd to give absolute path to the file
	wcscat(wide_cwd, L"/");
	wcscat(wide_cwd, file);

	wcsconvertFilepathtoURL(wide_cwd, url);

	return url;
}



// Prototype to satisfy the CW compiler.
extern "C" HRESULT CAAFEssenceAccess_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);


#define TEST_PATH	L"SomeFile.dat"


static void convert(char* cName, size_t length, const wchar_t* name)
{
	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
        cerr << ": Error : Conversion failed." << endl;
		exit(1);  
	}
}

static bool PixelCompare(const aafInt16 slop, aafDataBuffer_t buf1, aafDataBuffer_t buf2, aafUInt32 bufferSize)
{
	aafUInt32 index;
	aafInt16 diff;

	for (index = 0; index < bufferSize; ++index)
	{
		diff = (aafInt16)buf1[index] - (aafInt16)buf2[index];
		if (slop < diff)
			return false;
	}

	return true;
}

#ifdef _DEBUG
static void HexDumpBuffer(const char* label, aafDataBuffer_t buffer, aafUInt32 bufferSize, aafUInt32 cols = 16)
{
	int x;

	printf("static const aafUInt8 %s[] =\n{", label);
	if (buffer && 0 < bufferSize)
	{
		aafUInt32 i;
		for (i = 0; i < (bufferSize - 1); ++i)
		{
			if ((i % cols) == 0)
				printf("\n  ");

			x = buffer[i];
			printf("0x%02X,", x);
		}
		x = buffer[i];
		printf("0x%02X\n", x);
	}

	printf("};\n");


	FILE* file = fopen(label, "wb");
	if (NULL != file)
	{
		fwrite(buffer, 1, bufferSize, file);
		fclose(file);
		file = NULL;
	}

}

#endif // #ifdef _DEBUG


static void MobIDtoString(aafMobID_constref uid, char *buf)
{
    sprintf( buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-" \
		  "%02x-%02x-%02x-%02x-" \
		  "%08x%04x%04x" \
		  "%02x%02x%02x%02x%02x%02x%02x%02x",

	(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], 
	(int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3],
	(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], 
	(int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7],
	(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[9], 
	(int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11],

	(int)uid.length, (int)uid.instanceHigh, 
	(int)uid.instanceMid, (int)uid.instanceLow,

	uid.material.Data1, uid.material.Data2, uid.material.Data3,

	(int)uid.material.Data4[0], (int)uid.material.Data4[1], 
	(int)uid.material.Data4[2], (int)uid.material.Data4[3],
	(int)uid.material.Data4[4], (int)uid.material.Data4[5], 
	(int)uid.material.Data4[6], (int)uid.material.Data4[7] );
}


typedef enum { testStandardCalls, testMultiCalls } testType_t;

typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola


static AAFByteOrder getNativeByteOrder(void);
static void ByteSwap32(
				   aafInt32 *lp);	/* IN/OUT -- Byte swap this value */
static void ByteSwap16(
				   aafInt16 * wp);	/* IN/OUT -- Byte swap this value */
void scanWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
void scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
AAFRESULT loadWAVEHeader(aafUInt8 *buf,
						 aafUInt16 *bitsPerSample,
						 aafUInt16 *numCh,
						 aafRational_t *sampleRate,
						 aafUInt32 *dataOffset,
						 aafUInt32 *dataLen);

typedef struct
{
	aafWChar	*dataFilename;
	aafUID_t	dataFormat;
} testDataFile_t;

typedef IAAFSmartPointer<IAAFEssenceFormat> IAAFEssenceFormatSP;

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT hrSetTransformParameters=0;


static HRESULT ReadStaticEssenceAAFFile(aafWChar * pFileName, testType_t testType, aafUID_t codecID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat			*fmtTemplate =  NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceFormatSP pFormat;
	
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafMobID_t					mobID;
	aafWChar					namebuf[1204];
	unsigned char				AAFDataBuf[4096];
	aafUInt32					AAFBytesRead, samplesRead;


	
	try
	{
		checkResult(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		
		// Here we checkResult on the number of mobs in the file. 
		// Get the number of master mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFMasterMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFMasterMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			char mobIDstr[256];
			char mobName[256];
			
			
			checkResult(pMob->GetMobID (&mobID));
			checkResult(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			MobIDtoString(mobID, mobIDstr);
			// Make sure we have one slot 
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
			// The essence data is in SlotID 1
			// Get a Master Mob interface
			checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));
			
			// Open the Essence Data
			checkResult(pMasterMob->OpenEssence(	1,						// SlotID 1
				NULL,				// mediaCriteria (Don't care)
				kAAFMediaOpenReadOnly,	// Open mode
				kAAFCompressionDisable,// Compress disabled
				&pEssenceAccess));
			
			aafUID_t tstCodecID = {0};
			checkResult(pEssenceAccess->GetCodecID(&tstCodecID));
			if (false && memcmp(&tstCodecID, &codecID, sizeof(codecID)))
			{
			  cout << "     Warning:GetCodecID did not return CodecJPEG." << endl;
			}

  				
			// Read the Data from the AAF file
			if(testType == testStandardCalls)
			{
				checkResult(pEssenceAccess->ReadSamples(	1,	// number of samples - must be 1 for JPEG codec
					sizeof(AAFDataBuf),	// Maximum buffer size
					AAFDataBuf,			// Buffer for the data
					&samplesRead,		// Actual number of samples read
					&AAFBytesRead));	// Actual number of bytes read
			}
			else if(testType == testMultiCalls)
			{
				cout << "testMultiCalls not supported in AAF Static Essence Tests" << endl;
				
			}
			
			// Now compare the data read from the AAF file to the actual WAV file
			if (sizeof(compressedJFIF) != AAFBytesRead)
			{
					cout << "***Wrong number of bytes read ( was "
								<< AAFBytesRead
								<< ", should be "
								<< sizeof(compressedJFIF)
								<< ")"
								<< endl;
			}
			if (memcmp( compressedJFIF, AAFDataBuf, sizeof(compressedJFIF)) != 0)
			{
					cout << "*** Data Read is different than the data in the compressedJFIF ***" << endl;
			}
			
			// cleanup the master mob.
			pMasterMob->Release();
			pMasterMob = NULL;
			
			pMob->Release();
			pMob = NULL;
		}

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	
	// Cleanup and return
	if (pMultiEssence)
		pMultiEssence->Release();

	if(fmtTemplate)
		fmtTemplate->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if (pMasterMob)
		pMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		HRESULT local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	
	return hr;
}

static HRESULT CreateStaticEssenceAAFFile(
	aafWChar * pFileName, 
	aafUID_constref fileKind,
	testRawStorageType_t rawStorageType,
	aafProductIdentification_constref productID,
	testDataFile_t *dataFile, 
	testType_t testType,
	aafUID_t codecID,
	aafUID_t ddefID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFDataDef*					pDataDef = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFMasterMob2*				pMasterMob2 = NULL;
	
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormatSP			pFormat;
	IAAFEssenceFormat			*format = NULL;
	IAAFLocator					*pLocator = NULL;
	// !!!Previous revisions of this file contained variables here required to handle external essence
	aafMobID_t					masterMobID;
	aafUID_t			 testContainer;

	// delete any previous test file before continuing...
	char chNameBuffer[1000];

	try
	{

		convert(chNameBuffer, sizeof(chNameBuffer), pFileName);
		remove(chNameBuffer);
		if(dataFile != NULL)
		{
			// delete any previous test file before continuing...
			char chNameBuffer[1000];
			convert(chNameBuffer, sizeof(chNameBuffer), dataFile->dataFilename);
			remove(chNameBuffer);
		}
		
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		// !!!Previous revisions of this file contained code here required to handle external essence
		
		// Get a Master MOB Interface
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMasterMob, 
								   (IUnknown **)&pMasterMob));
		// Get a Mob interface and set its variables.
		checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->GetMobID(&masterMobID));
		checkResult(pMob->SetName(L"A Master Mob"));
		
		// Add it to the file 
		checkResult(pHeader->AddMob(pMob));
		
		pLocator = NULL;
		testContainer = ContainerAAF;

		// lookup datadef object to use
		checkResult(pDictionary->LookupDataDef(ddefID, &pDataDef));

		// now create the Essence data file
		checkResult(pMasterMob->QueryInterface(IID_IAAFMasterMob2, (void **)&pMasterMob2));
		checkResult(pMasterMob2->CreateStaticEssence(STD_SLOT_ID,	// Slot ID
			pDataDef,	// MediaKind
			codecID,		// codecID
			kAAFCompressionDisable, // compression enabled or disabled
			pLocator,	// In current file
			testContainer,	// In AAF Format
			&pEssenceAccess));

		//	compressedJFIF
		aafUInt32 samplesWritten, bytesWritten;

		checkResult(pEssenceAccess->WriteSamples(	STD_SAMPLE_COUNT,
						      sizeof(compressedJFIF),// buffer size
				         (aafDataBuffer_t) compressedJFIF,	// THE data
									&samplesWritten,
									&bytesWritten));
		checkResult(pEssenceAccess->CompleteWrite());

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

		
		
	// Cleanup and return
	if (pMultiEssence)
		pMultiEssence->Release();
	
	if(format)
		format->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if(pLocator)
		pLocator->Release();

	if (pMob)
		pMob->Release();

	if (pMasterMob2)
		pMasterMob2->Release();

	if (pMasterMob)
		pMasterMob->Release();

	if(pDataDef)
		pDataDef->Release();

	if(pDictionary)
		pDictionary->Release();

	if(pHeader)
		pHeader->Release();

	if (pFile)
	{ // Preserve previous errors...
		HRESULT local_hr = pFile->Save();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	

	return hr;
}


static HRESULT CreateAudioAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID,
    testDataFile_t *dataFile,
    testType_t testType,
    aafUID_t codecID,
	aafUID_t ddefID,
    aafBool bCallSetTransformParameters=kAAFFalse)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFDataDef*					pDataDef = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormatSP			pFormat;
	IAAFEssenceFormat			*format = NULL;
	IAAFLocator					*pLocator = NULL;
	// !!!Previous revisions of this file contained variables here required to handle external essence
	aafMobID_t					masterMobID;
	aafRational_t				editRate = {44100, 1};
	aafRational_t				sampleRate = {44100, 1};
	unsigned char				dataBuff[4096], *dataPtr;
	aafUInt32					dataOffset, dataLen;
	aafUInt16					bitsPerSample, numCh;
	aafInt32			n, numSpecifiers;
	aafUID_t			essenceFormatCode, testContainer;
  aafUInt32 samplesWritten, bytesWritten;
	// delete any previous test file before continuing...

	try
	{
		char chNameBuffer[FILENAME_MAX];
		convert(chNameBuffer, sizeof(chNameBuffer), pFileName);
		remove(chNameBuffer);
		if(dataFile != NULL)
		{
			// delete any previous test file before continuing...
			aafWChar	filepath[FILENAME_MAX];
			wcsconvertURLtoFilepath(dataFile->dataFilename, filepath);
			convert(chNameBuffer, sizeof(chNameBuffer), filepath);
			remove(chNameBuffer);
		}
		
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		// !!!Previous revisions of this file contained code here required to handle external essence
		
		// Get a Master MOB Interface
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMasterMob, 
								   (IUnknown **)&pMasterMob));
		// Get a Mob interface and set its variables.
		checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->GetMobID(&masterMobID));
		checkResult(pMob->SetName(L"A Master Mob"));
		
		// Add it to the file 
		checkResult(pHeader->AddMob(pMob));
		
		// !!!Previous revisions of this file contained code here required to handle external essence
		
		if(dataFile != NULL)
		{
			// Make a locator, and attach it to the EssenceDescriptor
			checkResult(defs.cdNetworkLocator()->
						CreateInstance(IID_IAAFLocator, 
									   (IUnknown **)&pLocator));		
			checkResult(pLocator->SetPath (dataFile->dataFilename));
			testContainer = dataFile->dataFormat;
		}
		else
		{
			pLocator = NULL;
			testContainer = ContainerAAF;
		}
		
		
		// copy contents of Laser.wav into buffer
		memcpy(dataBuff, uncompressedWAVE_Laser, sizeof(uncompressedWAVE_Laser));

		checkResult(loadWAVEHeader(dataBuff,
			&bitsPerSample,
			&numCh,
			&sampleRate,
			&dataOffset,
			&dataLen));
		dataPtr = dataBuff + dataOffset;
		
		// lookup datadef object to use
		checkResult(pDictionary->LookupDataDef(ddefID, &pDataDef));

		// now create the Essence data file
		checkResult(pMasterMob->CreateEssence(STD_SLOT_ID,				// Slot ID
			pDataDef,	// MediaKind
			codecID,		// kAAFCodecWAVE
			editRate,		// edit rate
			sampleRate,		// sample rate
			kAAFCompressionDisable, // compression enabled or disabled
			pLocator,	// In current file
			testContainer,	// In AAF Format
			&pEssenceAccess));

    // Get the codecID and validate that it is what we expect.
    aafUID_t codecID = {0};
		checkResult(pEssenceAccess->GetCodecID(&codecID));
		checkExpression(0 == memcmp(&codecID, &codecID, sizeof(codecID)), AAFRESULT_TEST_FAILED);
    

		checkResult(pEssenceAccess->GetFileFormatParameterList (&format));
		checkResult(format->NumFormatSpecifiers (&numSpecifiers));
		for(n = 0; n < numSpecifiers; n++)
		{
			checkResult(format->GetIndexedFormatSpecifier (n, &essenceFormatCode, 0, NULL, NULL));
		}
		format->Release();
		format = NULL;
		
		// Tell the AAFEssenceAccess what the format is.
		checkResult(pEssenceAccess->GetEmptyFileFormat (&pFormat));
		checkResult(pFormat->NumFormatSpecifiers (&numSpecifiers));
		
		aafInt32	sampleSize = bitsPerSample;
		checkResult(pFormat->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleSize), (aafUInt8 *)&sampleSize));
		checkResult(pEssenceAccess->PutFileFormat (pFormat));

		// At the time this test was written, SetTransformParameters() returned
		// AAFRESULT_NOT_IN_CURRENT_VERSION, and therefore did not need to be 
		// tested.  We simply store the HRESULT from SetTransformParameters(), and
		// check at the end of CAAFEssenceAccess_test(testMode_t mode) if the function still
		// returns that code.
		if(bCallSetTransformParameters==kAAFTrue)
			hrSetTransformParameters=pEssenceAccess->SetTransformParameters(
				pFormat);

		// NIL flavour is the only one available for kAAFCodecWAVE
		checkResult(pEssenceAccess->SetEssenceCodecFlavour(kAAFNilCodecFlavour));

		// write out the data
		if(testType == testStandardCalls)
		{
			checkResult(pEssenceAccess->WriteSamples(	dataLen,	//!!! hardcoded bytes/sample ==1// Number of Samples
						      sizeof(dataBuff),// buffer size
				          dataPtr,	// THE data
									&samplesWritten,
									&bytesWritten));
    }
		else if(testType == testMultiCalls)
		{
			aafmMultiXfer_t		xfer;
			aafmMultiResult_t	result;
			
			checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
			//!!!		xfer.subTrackNum = _channels[0].physicalOutChan;
			xfer.numSamples = dataLen;	//!!! hardcoded bytes/sample ==1
			xfer.buflen = sizeof(dataBuff);
			xfer.buffer = dataPtr;
			result.bytesXfered = 0;
			
			checkResult(pMultiEssence->WriteMultiSamples(1, &xfer, &result));
			pMultiEssence->Release();
			pMultiEssence = NULL;

			samplesWritten = result.samplesXfered;
			bytesWritten = result.bytesXfered;
		}

		// Finish writing the destination
		checkResult(pEssenceAccess->CompleteWrite());
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

		
		
	// Cleanup and return
	if (pMultiEssence)
		pMultiEssence->Release();
	
	if(format)
		format->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if(pLocator)
		pLocator->Release();

	if (pMob)
		pMob->Release();

	if (pMasterMob)
		pMasterMob->Release();

	if(pDataDef)
		pDataDef->Release();

	if(pDictionary)
		pDictionary->Release();

	if(pHeader)
		pHeader->Release();

	if (pFile)
	{ // Preserve previous errors...
		HRESULT local_hr = pFile->Save();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	

	return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName, testType_t testType, aafUID_t codecID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat			*fmtTemplate =  NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceFormatSP pFormat;
	
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafRational_t				readSampleRate;
	aafMobID_t					mobID;
	aafWChar					namebuf[1204];
	unsigned char				AAFDataBuf[4096];
	aafUInt32					AAFBytesRead, samplesRead;
	unsigned char				WAVDataBuf[4096], *dataPtr;
	aafUInt32					dataOffset, dataLen;
	aafUInt16					bitsPerSample, numCh;
	
	try
	{
		checkResult(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		
		// Here we checkResult on the number of mobs in the file. 
		// Get the number of master mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFMasterMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFMasterMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			char mobIDstr[256];
			char mobName[256];
			
			
			checkResult(pMob->GetMobID (&mobID));
			checkResult(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			MobIDtoString(mobID, mobIDstr);
			// Make sure we have one slot 
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
			// The essence data is in SlotID 1
			// Get a Master Mob interface
			checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));
			
			// Open the Essence Data
			checkResult(pMasterMob->OpenEssence(STD_SLOT_ID,						// SlotID
				NULL,				// mediaCriteria (Don't care)
				kAAFMediaOpenReadOnly,	// Open mode
				kAAFCompressionDisable,// Compress disabled
				&pEssenceAccess));
			
			aafUID_t tstCodecID = {0};
			checkResult(pEssenceAccess->GetCodecID(&tstCodecID));
			if (false && memcmp(&tstCodecID, &codecID, sizeof(codecID)))
			{
			  cout << "     Warning:GetCodecID did not return CodecJPEG." << endl;
			}

      
			// copy contents of Laser.wav into buffer
			memcpy(WAVDataBuf, uncompressedWAVE_Laser, sizeof(uncompressedWAVE_Laser));

			checkResult(loadWAVEHeader(WAVDataBuf,
				&bitsPerSample,
				&numCh,
				&readSampleRate,
				&dataOffset,
				&dataLen));
			dataPtr = WAVDataBuf + dataOffset;
			
			aafUInt32			sampleBits;
			aafInt32			bytesRead;
			
			checkResult(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
			checkResult(fmtTemplate->AddFormatSpecifier (kAAFAudioSampleBits, 0, NULL));
			checkResult(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
			fmtTemplate->Release();
			fmtTemplate = NULL;
			
			checkResult(pFormat->GetFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleBits),
				(aafDataBuffer_t)&sampleBits, &bytesRead));
			checkExpression(sampleBits == bitsPerSample, AAFRESULT_TEST_FAILED);
			
			// Read the Data from the AAF file
			if(testType == testStandardCalls)
			{
				checkResult(pEssenceAccess->ReadSamples(	dataLen,	//!!! Hardcoded	// Number of Samples 
					sizeof(AAFDataBuf),	// Maximum buffer size
					AAFDataBuf,			// Buffer for the data
					&samplesRead,		// Actual number of samples read
					&AAFBytesRead));	// Actual number of bytes read
			}
			else if(testType == testMultiCalls)
			{
				aafmMultiXfer_t		xfer;
				aafmMultiResult_t	result;
				
				checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
				xfer.numSamples = dataLen;	//!!! Hardcoded	// Number of Samples 
				xfer.buflen = sizeof(AAFDataBuf);
				xfer.buffer = AAFDataBuf;
				result.bytesXfered = 0;
				checkResult(pMultiEssence->ReadMultiSamples(1, &xfer, &result));
				samplesRead = result.samplesXfered;
				AAFBytesRead = result.bytesXfered;
				pMultiEssence->Release();
				pMultiEssence = NULL;
			}
			
			// Now compare the data read from the AAF file to the actual WAV file
			if (dataLen != AAFBytesRead)
			{
							cout << "***Wrong number of bytes read ( was "
									 << AAFBytesRead
									 << ", should be "
									 << sizeof(uncompressedWAVE_Laser)
									 << ")"
									 << endl;
			}
			if (memcmp( dataPtr, AAFDataBuf, dataLen) != 0)
			{
							cout << "*** Data Read is different than the data in the WAV file ***" << endl;
			}
			
			// cleanup the master mob.
			pMasterMob->Release();
			pMasterMob = NULL;
			
			pMob->Release();
			pMob = NULL;
		}

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	
	// Cleanup and return
	if (pMultiEssence)
		pMultiEssence->Release();

	if(fmtTemplate)
		fmtTemplate->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if (pMasterMob)
		pMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		HRESULT local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	
	return hr;
}

static void FillSampleFrameBuffer(
  aafDataBuffer_t buffer,
  aafUInt32 height,
  aafUInt32 width, 
  aafUInt32 padBytesPerRow,
  const aafUInt8 color[])
{
	aafUInt32 row, col, rowBytes, offset;

	rowBytes = (width * 3) + padBytesPerRow;
	for (row = offset = 0; row < height; ++row)
	{
		offset = row * rowBytes;
		for (col = 0; col < width; ++col, offset += 3)
		{
			buffer[offset + 0] = color[0];
			buffer[offset + 1] = color[1];
			buffer[offset + 2] = color[2];
		}
	}
}


static void FillYCbCr422SampleBufferFromYCbCr(
  aafDataBuffer_t buffer,
  aafUInt32 height,
  aafUInt32 width, 
  const aafUInt8 color[])
{
	aafUInt32 row, col, offset;

	for (row = offset = 0; row < height; ++row)
	{
		for (col = 0; col < width; ++col)
		{
			if (col % 2 == 0)
			{
				buffer[offset++] = color[1];
				buffer[offset++] = color[0];
				buffer[offset++] = color[2];
			}
			else
			{
				buffer[offset++] = color[0];
			}
		}
	}
}

static HRESULT CreateVideoAAFFile(
  aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID,
  testDataFile_t *dataFile,
  aafCompressEnable_t compressEnable,
	aafColorSpace_t colorSpace,
	aafUInt32 horizontalSubsample,
	aafUID_t codecID,
	aafUID_t ddefID,
	testType_t testType,
	aafUID_t compression)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFDataDef*					pDataDef = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormatSP			pFormat;
	IAAFEssenceFormat			*format = NULL;
	IAAFEssenceFormat* pTransformFormat = NULL;
	IAAFLocator					*pLocator = NULL;
	aafMobID_t					masterMobID;
	aafRational_t				editRate = {30000, 1001};
	aafRational_t				sampleRate = {30000, 1001};
//	unsigned char				dataBuff[4096], *dataPtr;
//	size_t						bytesRead;
//	aafUInt32					dataOffset, dataLen;
//	aafUInt16					bitsPerSample, numCh;
//	aafUInt32					bytesWritten;
	aafInt32			n, numSpecifiers;
	aafUID_t			essenceFormatCode, testContainer;
	aafCharacter wNameBuffer[500];
	aafDataBuffer_t rgbColorBuffer = NULL, compressedBuffer = NULL;
	aafDataBuffer_t writeBuffer = NULL;
	aafUInt32 writeBufferSize = 0, compressedBufferSize = 0;
  aafUInt32 samplesWritten, bytesWritten;


	try
	{
		checkExpression((1 == horizontalSubsample || 
			              (2 == horizontalSubsample && kAAFColorSpaceYUV == colorSpace)),
										AAFRESULT_TEST_FAILED);
		
		// delete any previous test file before continuing...
		char chNameBuffer[FILENAME_MAX];
		convert(chNameBuffer, sizeof(chNameBuffer), pFileName);
		remove(chNameBuffer);
		if(dataFile != NULL)
		{
			// delete any previous test file before continuing...
			aafWChar	filepath[FILENAME_MAX];
			wcsconvertURLtoFilepath(dataFile->dataFilename, filepath);
			convert(chNameBuffer, sizeof(chNameBuffer), filepath);
			remove(chNameBuffer);
		}

		aafRect_t rc;
		rc.xOffset = rc.yOffset = 0;
		rc.xSize = STD_WIDTH;
		rc.ySize = STD_HEIGHT;

		if (codecID == kAAFCodecJPEG)
		{
			if (kAAFColorSpaceYUV == colorSpace && 2 == horizontalSubsample)
			{
				compressedBuffer = const_cast<aafDataBuffer_t>(compressed422JFIF);
				compressedBufferSize = sizeof(compressed422JFIF);
			}
			else
			{
				compressedBuffer = const_cast<aafDataBuffer_t>(compressedJFIF);
				compressedBufferSize = sizeof(compressedJFIF);
			}
	
			if (compressEnable == kAAFCompressionEnable)
			{
				if (kAAFColorSpaceYUV == colorSpace)
				{
					if (2 == horizontalSubsample)
					{
						rgbColorBuffer = new aafUInt8[SAMPLE_422_BYTES];
						checkExpression(NULL != rgbColorBuffer, AAFRESULT_NOMEMORY);
						FillYCbCr422SampleBufferFromYCbCr(rgbColorBuffer, STD_HEIGHT, STD_WIDTH, yuv_green);
						writeBufferSize = SAMPLE_422_BYTES;
					}
					else
					{
						rgbColorBuffer = new aafUInt8[FRAME_BYTES];
						checkExpression(NULL != rgbColorBuffer, AAFRESULT_NOMEMORY);
						FillSampleFrameBuffer(rgbColorBuffer, STD_HEIGHT, STD_WIDTH, 0, yuv_green);
						writeBufferSize = FRAME_BYTES;
					}
				}
				else
				{
					rgbColorBuffer = new aafUInt8[FRAME_BYTES];
					checkExpression(NULL != rgbColorBuffer, AAFRESULT_NOMEMORY);
					FillSampleFrameBuffer(rgbColorBuffer, STD_HEIGHT, STD_WIDTH, 0, green);
					writeBufferSize = FRAME_BYTES;
				}
	
				writeBuffer = rgbColorBuffer;
			}
			else
			{
				writeBuffer = compressedBuffer;
				writeBufferSize = compressedBufferSize;
			}
		}
		else if (compression == kAAFCompressionDef_LegacyDV)
		{
			writeBuffer = readDVframe( &writeBufferSize );
			rgbColorBuffer = writeBuffer;		// to ensure it is freed

			aafRational_t PAL_rate = {25, 1};
			editRate = PAL_rate;
			sampleRate = PAL_rate;
			rc.xSize = 720;
			rc.ySize = 576 / 2;
		}
		else // codecID is not JPEG Codec
		{
			// Uncompressed video
			writeBuffer = initUncompressedVideo( horizontalSubsample, &writeBufferSize );
		}
		
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		// Get a Master MOB Interface
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMasterMob, 
								   (IUnknown **)&pMasterMob));
		// Get a Mob interface and set its variables.
		checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->GetMobID(&masterMobID));
		checkResult(pMob->SetName(L"A Master Mob"));
		
		// Add it to the file 
		checkResult(pHeader->AddMob(pMob));
		
		// !!!Previous revisions of this file contained code here required to handle external essence
		
		if(dataFile != NULL)
		{
			// Make a locator, and attach it to the EssenceDescriptor
			checkResult(defs.cdNetworkLocator()->
						CreateInstance(IID_IAAFLocator, 
									   (IUnknown **)&pLocator));		
			checkResult(pLocator->SetPath (dataFile->dataFilename));
			testContainer = dataFile->dataFormat;
		}
		else
		{
			pLocator = NULL;
			testContainer = ContainerAAF;
		}
		
		// lookup datadef object to use
		checkResult(pDictionary->LookupDataDef(ddefID, &pDataDef));
		
		// now create the Essence data file
		checkResult(pMasterMob->CreateEssence(STD_SLOT_ID,				// Slot ID
			pDataDef,	// MediaKind
			codecID,		// e.g. kAAFCodecCDCI, kAAFCodecJPEG
			editRate,		// edit rate
			sampleRate,		// sample rate
			compressEnable, // compression enabled or disabled
			pLocator,		// In current file
			testContainer,	// In AAF Format
			&pEssenceAccess));

		// For legacy DV, the convenient way is to set a CDCI codec flavour
		if (compression == kAAFCompressionDef_LegacyDV)
			checkResult(pEssenceAccess->SetEssenceCodecFlavour(kAAFCodecFlavour_LegacyDV_625_50));

		// Get and display the current code name.
		checkResult(pEssenceAccess->GetCodecName(sizeof(wNameBuffer), wNameBuffer));
		convert(chNameBuffer, sizeof(chNameBuffer), wNameBuffer);

	    // Get the codecID and validate that it is what we expect.
		aafUID_t expCodecID = {0};
		checkResult(pEssenceAccess->GetCodecID(&expCodecID));
		checkExpression(0 == memcmp(&expCodecID, &codecID, sizeof(expCodecID)), AAFRESULT_TEST_FAILED);

		checkResult(pEssenceAccess->GetFileFormatParameterList (&format));
		checkResult(format->NumFormatSpecifiers (&numSpecifiers));
		for(n = 0; n < numSpecifiers; n++)
		{
			checkResult(format->GetIndexedFormatSpecifier (n, &essenceFormatCode, 0, NULL, NULL));
		}

		// Save the default pixel color space.
		aafInt32 bytesRead = 0;
		aafColorSpace_t defaultPixelColorSpace;
		checkResult(format->GetFormatSpecifier (kAAFPixelFormat, sizeof(defaultPixelColorSpace),(aafDataBuffer_t)&defaultPixelColorSpace, &bytesRead));
		checkExpression(sizeof(defaultPixelColorSpace) == bytesRead, AAFRESULT_TEST_FAILED);
		
		
		format->Release();
		format = NULL;
		
		// Tell the AAFEssenceAccess what the format is.
		checkResult(pEssenceAccess->GetEmptyFileFormat (&pFormat));
		checkResult(pFormat->NumFormatSpecifiers (&numSpecifiers));


		// Set the stored rectangle.
		checkResult(pFormat->AddFormatSpecifier (kAAFStoredRect, sizeof(rc), (aafDataBuffer_t)&rc));
		checkResult(pFormat->AddFormatSpecifier (kAAFSampledRect, sizeof(rc), (aafDataBuffer_t)&rc));
		checkResult(pFormat->AddFormatSpecifier (kAAFDisplayRect, sizeof(rc), (aafDataBuffer_t)&rc));

		if (compressEnable == kAAFCompressionEnable)
		{
			aafUInt8 quality = 75;
			checkResult(pFormat->AddFormatSpecifier (kAAFCompressionQuality, sizeof(quality), (aafDataBuffer_t)&quality));
		}

		// Set the values for the codec.
		checkResult(pEssenceAccess->PutFileFormat (pFormat));

		// Change the output pixel format / color space if necessary.
		if (compressEnable == kAAFCompressionEnable && defaultPixelColorSpace != colorSpace)
		{
			// NOTE: TransformParameters have not been implemented yet!
			// For now just use the PutFileFormat method.

			checkResult(pEssenceAccess->GetEmptyFileFormat (&pTransformFormat));


			if (kAAFColorSpaceYUV == colorSpace)
			{
				checkResult(pTransformFormat->AddFormatSpecifier (kAAFCDCIHorizSubsampling, sizeof(horizontalSubsample), (aafDataBuffer_t)&horizontalSubsample));
			}


			checkResult(pTransformFormat->AddFormatSpecifier (kAAFPixelFormat, sizeof(colorSpace), (aafDataBuffer_t)&colorSpace));
			checkResult(pEssenceAccess->PutFileFormat(pTransformFormat));

			pTransformFormat->Release();
			pTransformFormat = NULL;
		}

		if (codecID != kAAFCodecJPEG && compressEnable == kAAFCompressionDisable)
		{
			// For Legacy DV a flavour was set.  For other formats use format specifiers.
			if (compression != kAAFCompressionDef_LegacyDV)
			{
				// Set format specifiers needed for uncompressed CDCI video
				checkResult(pEssenceAccess->GetEmptyFileFormat (&pTransformFormat));
				checkResult(pTransformFormat->AddFormatSpecifier (kAAFCDCIHorizSubsampling, sizeof(horizontalSubsample), (aafDataBuffer_t)&horizontalSubsample));
				checkResult(pEssenceAccess->PutFileFormat(pTransformFormat));

				pTransformFormat->Release();
				pTransformFormat = NULL;
			}
		}


		// write out the data
		aafUInt32 index;
		for (index = 0; index < MAX_SAMPLE_COUNT; ++index)
		{
			if(testType == testStandardCalls)
			{
				checkResult(pEssenceAccess->WriteSamples(STD_SAMPLE_COUNT,	//!!! hardcoded bytes/sample ==1// Number of Samples
					writeBufferSize, // buffer size
					writeBuffer,	// THE data
					&samplesWritten,
					&bytesWritten));
			}
			else if(testType == testMultiCalls)
			{
				aafmMultiXfer_t		xfer;
				aafmMultiResult_t	result;
				
				checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
				//!!!		xfer.subTrackNum = _channels[0].physicalOutChan;
				xfer.numSamples = STD_SAMPLE_COUNT;	//!!! hardcoded bytes/sample ==1
				xfer.buflen = writeBufferSize;
				xfer.buffer = writeBuffer;
				result.bytesXfered = 0;
				
				HRESULT r = pMultiEssence->WriteMultiSamples(STD_SAMPLE_COUNT, &xfer, &result);
				if (r == AAFRESULT_INVALID_OP_CODEC)
				  {
					hr = r;
					break;
				  }
				checkResult(r);

				pMultiEssence->Release();
				pMultiEssence = NULL;

			  samplesWritten = result.samplesXfered;
			  bytesWritten = result.bytesXfered;
			}
		
		}


		// Finish writing the destination
		checkResult(pEssenceAccess->CompleteWrite());
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

		
		
	// Cleanup and return
	if (rgbColorBuffer)
		delete [] rgbColorBuffer;

	if (pMultiEssence)
		pMultiEssence->Release();
	
	if(format)
		format->Release();

	if (pTransformFormat)
		pTransformFormat->Release();

	if (pEssenceAccess)
		pEssenceAccess->Release();
	
	if(pLocator)
		pLocator->Release();

	if (pMob)
		pMob->Release();

	if (pMasterMob)
		pMasterMob->Release();

	if(pDataDef)
		pDataDef->Release();

	if(pDictionary)
		pDictionary->Release();

	if(pHeader)
		pHeader->Release();

	if (pFile)
	{ // Preserve previous errors...
		HRESULT local_hr = pFile->Save();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	

	return hr;
}


static HRESULT ReadVideoAAFFile(
	aafWChar * pFileName, 
	aafCompressEnable_t compressEnable,
	aafColorSpace_t colorSpace,
	aafUInt32 horizontalSubsample,
	aafUID_t codecID,
	testType_t testType,
	aafUID_t compression)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFEssenceAccess*			pEssenceAccess = NULL;
	IAAFEssenceMultiAccess*		pMultiEssence = NULL;
	IAAFEssenceFormat			*fmtTemplate =  NULL;
	IAAFEssenceFormat* pTransformFormat = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFEssenceFormatSP			pFormat;
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafMobID_t					mobID;
	aafWChar					namebuf[1204];
	aafDataBuffer_t				AAFDataBuf = NULL;
	aafUInt32					AAFBytesRead = 0, samplesRead = 0;
	aafDataBuffer_t checkBuffer = NULL, compressedBuffer = NULL;
	aafUInt32 checkBufferSize = 0, compressedBufferSize = 0;
	

	try
	{
		checkExpression((1 == horizontalSubsample || 
			              (2 == horizontalSubsample && kAAFColorSpaceYUV == colorSpace)),
										AAFRESULT_TEST_FAILED);
		
		// Validate the rectangle values that we originally wrote.
		aafInt32 bytesRead = 0;
		aafRect_t std_rc;
		std_rc.xOffset = std_rc.yOffset = 0;
		std_rc.xSize = STD_WIDTH;
		std_rc.ySize = STD_HEIGHT;

		if (codecID == kAAFCodecJPEG)
		{
			if (compressEnable == kAAFCompressionEnable)
			{
				if (kAAFColorSpaceYUV == colorSpace)
				{
					if (2 == horizontalSubsample)
					{
						checkBuffer = new aafUInt8[SAMPLE_422_BYTES];
						checkExpression(NULL != checkBuffer, AAFRESULT_NOMEMORY);
						FillYCbCr422SampleBufferFromYCbCr(checkBuffer, STD_HEIGHT, STD_WIDTH, yuv_green);
						checkBufferSize = SAMPLE_422_BYTES;
					}
					else
					{
						checkBuffer = new aafUInt8[FRAME_BYTES];
						checkExpression(NULL != checkBuffer, AAFRESULT_NOMEMORY);
						FillSampleFrameBuffer(checkBuffer, STD_HEIGHT, STD_WIDTH, 0, yuv_green);
						checkBufferSize = FRAME_BYTES;
					}
				}
				else
				{
					checkBuffer = new aafUInt8[FRAME_BYTES];
					checkExpression(NULL != checkBuffer, AAFRESULT_NOMEMORY);
					FillSampleFrameBuffer(checkBuffer, STD_HEIGHT, STD_WIDTH, 0, green);
					checkBufferSize = FRAME_BYTES;
				}
			}
			else
			{
				if (kAAFColorSpaceYUV == colorSpace && 2 == horizontalSubsample)
				{
					compressedBufferSize = sizeof(compressed422JFIF);
					compressedBuffer = new aafUInt8[compressedBufferSize];
					checkExpression(NULL != compressedBuffer, AAFRESULT_NOMEMORY);
					memcpy(compressedBuffer, compressed422JFIF, compressedBufferSize);
				}
				else
				{
					compressedBufferSize = sizeof(compressedJFIF);
					compressedBuffer = new aafUInt8[compressedBufferSize];
					checkExpression(NULL != compressedBuffer, AAFRESULT_NOMEMORY);
					memcpy(compressedBuffer, compressedJFIF, compressedBufferSize);
				}
			}
		}
		else if (compression == kAAFCompressionDef_LegacyDV)
		{
			compressedBuffer = readDVframe( &compressedBufferSize );
			std_rc.xSize = 720;
			std_rc.ySize = 576 / 2;
		}
		else // codecID is not JPEG Codec
		{
			// Uncompressed video
			aafUInt8 *video = initUncompressedVideo( horizontalSubsample, &compressedBufferSize );

			compressedBuffer = new aafUInt8[compressedBufferSize];
			checkExpression(NULL != compressedBuffer, AAFRESULT_NOMEMORY);
			memcpy(compressedBuffer, video, compressedBufferSize);
		}


		checkResult(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		// Here we checkResult on the number of mobs in the file. 
		// Get the number of master mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFMasterMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFMasterMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			char mobIDstr[256];
			char mobName[256];
			
			
			checkResult(pMob->GetMobID (&mobID));
			checkResult(pMob->GetName (namebuf, sizeof(namebuf)));
			convert(mobName, sizeof(mobName), namebuf);
			MobIDtoString(mobID, mobIDstr);
			// Make sure we have one slot 
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
			// The essence data is in SlotID 1
			// Get a Master Mob interface
			checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));
			
			// Open the Essence Data
			checkResult(pMasterMob->OpenEssence(STD_SLOT_ID,						// SlotID
				NULL,				// mediaCriteria (Don't care)
				kAAFMediaOpenReadOnly,	// Open mode
				compressEnable,// Compression
				&pEssenceAccess));

			aafUID_t storedCodecID = {0};
			checkResult(pEssenceAccess->GetCodecID(&storedCodecID));
			if (memcmp(&storedCodecID, &codecID, sizeof(storedCodecID)))
			{
			  cout << "     Warning:GetCodecID did not return expected CodecID." << endl;
			}

			// Setup the formats that we want to query.
			checkResult(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
			checkResult(fmtTemplate->AddFormatSpecifier (kAAFStoredRect, 0, NULL));
			checkResult(fmtTemplate->AddFormatSpecifier (kAAFSampledRect, 0, NULL));
			checkResult(fmtTemplate->AddFormatSpecifier (kAAFDisplayRect, 0, NULL));
			checkResult(fmtTemplate->AddFormatSpecifier (kAAFPixelFormat, 0, NULL));

			// Get the information from corresponding codec.
			checkResult(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
			fmtTemplate->Release();
			fmtTemplate = NULL;

			aafRect_t rc;
			rc.xOffset = rc.yOffset = 0;
			rc.xSize = rc.ySize = 0;


			checkResult(pFormat->GetFormatSpecifier (kAAFStoredRect, sizeof(rc), (aafDataBuffer_t)&rc, &bytesRead));
			checkExpression(sizeof(rc) == bytesRead, AAFRESULT_TEST_FAILED);
			checkExpression(0 == memcmp(&rc, &std_rc, sizeof(rc)), AAFRESULT_TEST_FAILED);

			checkResult(pFormat->GetFormatSpecifier (kAAFSampledRect, sizeof(rc),(aafDataBuffer_t)&rc, &bytesRead));
			checkExpression(sizeof(rc) == bytesRead, AAFRESULT_TEST_FAILED);
			checkExpression(0 == memcmp(&rc, &std_rc, sizeof(rc)), AAFRESULT_TEST_FAILED);

			checkResult(pFormat->GetFormatSpecifier (kAAFDisplayRect, sizeof(rc),(aafDataBuffer_t)&rc, &bytesRead));
			checkExpression(sizeof(rc) == bytesRead, AAFRESULT_TEST_FAILED);
			checkExpression(0 == memcmp(&rc, &std_rc, sizeof(rc)), AAFRESULT_TEST_FAILED);

			aafColorSpace_t defaultPixelColorSpace;
			checkResult(pFormat->GetFormatSpecifier (kAAFPixelFormat, sizeof(defaultPixelColorSpace),(aafDataBuffer_t)&defaultPixelColorSpace, &bytesRead));
			checkExpression(sizeof(defaultPixelColorSpace) == bytesRead, AAFRESULT_TEST_FAILED);
			
			// Change the output pixel format / color space if necessary.
			if (compressEnable == kAAFCompressionEnable)
			{
				// NOTE: TransformParameters have not been implemented yet!
				// For now just use the PutFileFormat method.

				checkResult(pEssenceAccess->GetEmptyFileFormat (&pTransformFormat));

				checkResult(pTransformFormat->AddFormatSpecifier (kAAFPixelFormat, sizeof(colorSpace), (aafDataBuffer_t)&colorSpace));
				checkResult(pEssenceAccess->PutFileFormat(pTransformFormat));

				pTransformFormat->Release();
				pTransformFormat = NULL;
			}



			// Get and display the current code name.
			checkResult(pEssenceAccess->GetCodecName(sizeof(namebuf), namebuf));
//			convert(chNameBuffer, sizeof(chNameBuffer), namebuf);
//			cout << "        Codec:" << chNameBuffer << endl;

			aafLength_t maxSampleSize = 0;
			checkResult(pEssenceAccess->GetLargestSampleSize(defs.ddkAAFPicture(), &maxSampleSize));
			aafUInt32 sampleBufferSize = static_cast<aafUInt32>(maxSampleSize);



			
			// Validate the buffer size.
			if (kAAFCompressionDisable == compressEnable)
			{	// If the data is compressed then the size of the buffer should be the
				// same as the original compressed JFIF data.
				checkExpression(compressedBufferSize == sampleBufferSize, AAFRESULT_TEST_FAILED);
			}

//			checkResult(pEssenceAccess->CountChannels());
			// trr: The CountChannels interface needs to be redesigned. Why doesn't it 
			// defer to the current codec like all of the other information methods?

			// Allocate the data buffer to hold the maximum sample (compressed)
			AAFDataBuf = new aafUInt8[sampleBufferSize];
			checkExpression(NULL != AAFDataBuf, AAFRESULT_NOMEMORY);

			aafLength_t sampleCount = 0;
			checkResult(pEssenceAccess->CountSamples(defs.ddkAAFPicture(), &sampleCount));
			checkExpression(MAX_SAMPLE_COUNT == sampleCount, AAFRESULT_TEST_FAILED);

			aafUInt32 index;
			for (index = 0; index < sampleCount; ++index)
			{
				aafLength_t sampleSize;
				checkResult(pEssenceAccess->GetIndexedSampleSize(defs.ddkAAFPicture(), index, &sampleSize));
				if (kAAFCompressionDisable == compressEnable)
					checkExpression(compressedBufferSize == sampleSize, AAFRESULT_TEST_FAILED);
				else
					checkExpression(sampleBufferSize == sampleSize, AAFRESULT_TEST_FAILED);


				// Read the Data from the AAF file
				samplesRead = 0;
				AAFBytesRead = 0;
				if(testType == testStandardCalls)
				{
					checkResult(pEssenceAccess->ReadSamples(STD_SAMPLE_COUNT,	//!!! Hardcoded	// Number of Samples 
						sampleBufferSize,	// Maximum buffer size
						AAFDataBuf,			// Buffer for the data
						&samplesRead,		// Actual number of samples read
						&AAFBytesRead));	// Actual number of bytes read
				}
				else if(testType == testMultiCalls)
				{
					aafmMultiXfer_t		xfer;
					aafmMultiResult_t	result;
					
					checkResult(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
					
					xfer.numSamples = STD_SAMPLE_COUNT;	//!!! Hardcoded	// Number of Samples 
					xfer.buflen = sampleBufferSize;
					xfer.buffer = AAFDataBuf;
					result.bytesXfered = 0;
					checkResult(pMultiEssence->ReadMultiSamples(STD_SAMPLE_COUNT, &xfer, &result));
					samplesRead = result.samplesXfered;
					AAFBytesRead = result.bytesXfered;

					pMultiEssence->Release();
					pMultiEssence = NULL;
				}
				
				if (codecID == kAAFCodecJPEG)
				{
					if (codecID == kAAFCodecJPEG && kAAFCompressionDisable == compressEnable)
					{	// If compression is disabled then compare the bytes read to the original
						// compressedJFIF data buffer.
						if (compressedBufferSize != AAFBytesRead)
						{
							cout << "***Wrong number of bytes read ( was "
									 << AAFBytesRead
									 << ", should be "
									 << sizeof(compressedJFIF)
									 << ")"
									 << endl;
	
							throw HRESULT(AAFRESULT_TEST_FAILED);
						}
						else if (memcmp( compressedBuffer, AAFDataBuf, compressedBufferSize) != 0)
						{
							cout << "*** Data Read is different than the data originally written from the JPEG buffer ***" << endl;
							throw HRESULT(AAFRESULT_TEST_FAILED);
						}
					}
					else
					{ // If compression is enabled then compare the bytes read to the original
						// uncompressed data buffer. This buffer should be 100% green.
						checkExpression(checkBufferSize == AAFBytesRead, AAFRESULT_TEST_FAILED);
						checkExpression(PixelCompare(STD_PIXEL_SLOP, AAFDataBuf, checkBuffer, checkBufferSize), AAFRESULT_TEST_FAILED);
					}
				}
				else
				{
					if (memcmp( compressedBuffer, AAFDataBuf, compressedBufferSize) != 0)
					{
						cout << "*** Data Read is different than the data originally written from the CDCI buffer ***" << endl;
						throw HRESULT(AAFRESULT_TEST_FAILED);
					}
				}
			}

			// Cleanup the temporary buffer.
			delete [] AAFDataBuf;
			AAFDataBuf = NULL;

			// cleanup the master mob.
			pMasterMob->Release();
			pMasterMob = NULL;
			
			pMob->Release();
			pMob = NULL;
		}

	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	
	// Cleanup and return
	if (checkBuffer)
		delete [] checkBuffer;

	if (compressedBuffer)
		delete [] compressedBuffer;

	if (pMultiEssence)
		pMultiEssence->Release();

	if (pTransformFormat)
		pTransformFormat->Release();
	
	if(fmtTemplate)
		fmtTemplate->Release();

	if (NULL != AAFDataBuf)
	{
#ifdef _DEBUG
 		if (kAAFCompressionDisable == compressEnable)
		{
			if (2 == horizontalSubsample)
				HexDumpBuffer("compressed422JFIF.jpg", AAFDataBuf, AAFBytesRead);
			else
				HexDumpBuffer("compressedJFIF.jpg", AAFDataBuf, AAFBytesRead);
		}
		else if (2 == horizontalSubsample)
			HexDumpBuffer("uncompressed422.raw", AAFDataBuf, AAFBytesRead);
		else
			HexDumpBuffer("uncompressedRGB.raw", AAFDataBuf, AAFBytesRead, 15);
#endif // #ifdef _DEBUG

		delete [] AAFDataBuf;
	}

	if (pEssenceAccess)
		pEssenceAccess->Release();

	if (pMasterMob)
		pMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		HRESULT local_hr = pFile->Close();
		if (FAILED(local_hr) && SUCCEEDED(hr))
			hr = local_hr;
		pFile->Release();
	}
	
	return hr;
}

//**********************
// Extra code required to scan the original WAVE headers and extract metadata parameters & data offset
AAFByteOrder getNativeByteOrder(void)
{
	aafInt16 word = 0x1234;
	aafInt8  byte = *((aafInt8*)&word);
	AAFByteOrder result;
	
	//  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));
	
	if (byte == 0x12) {
		result = MOTOROLA_ORDER;
	} else {
		result = INTEL_ORDER;
	}
	return result;
}

void ByteSwap32(
				   aafInt32 *lp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) lp;
	int             t;
	
	t = cp[3];
	cp[3] = cp[0];
	cp[0] = t;
	t = cp[2];
	cp[2] = cp[1];
	cp[1] = t;
}

void ByteSwap16(
				   aafInt16 * wp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) wp;
	int             t;
	
	t = cp[1];
	cp[1] = cp[0];
	cp[0] = t;
}
// app work on all WAVE files, not just laser.wav.
void scanWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
{
	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;
}

void scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
{
	AAFByteOrder	nativeByteOrder = getNativeByteOrder()
		;
	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;
	
	if ((maxsize == sizeof(aafInt32)) && (INTEL_ORDER != nativeByteOrder))
		ByteSwap32((aafInt32 *) data);
	else if ((maxsize == sizeof(aafInt16)) && (INTEL_ORDER != nativeByteOrder))
		ByteSwap16((aafInt16 *) data);
}

AAFRESULT loadWAVEHeader(aafUInt8 *buf,
						 aafUInt16 *bitsPerSample,
						 aafUInt16 *numCh,
						 aafRational_t *sampleRate,
						 aafUInt32 *dataOffset,
						 aafUInt32 *dataLen)
{
	aafInt32			formSize;
	aafInt16			pcm_format, junk16;
	aafUInt32			chunkSize;
	aafBool				fmtFound = kAAFFalse, dataFound = kAAFFalse;
	aafUInt8			chunkID[4];
	aafInt32			junk32, rate, bytesPerFrame = 0;
	aafUInt8			*ptr;
	
	ptr = buf;
	scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	if (memcmp(&chunkID, "RIFF", (size_t) 4) != 0)
		return(AAFRESULT_BADWAVEDATA);
	scanSwappedWAVEData(&ptr, sizeof(formSize), &formSize);	
	scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	if (memcmp(&chunkID, "WAVE", (size_t) 4) != 0)
		return(AAFRESULT_BADWAVEDATA);
	
	scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	while ((ptr-buf) < formSize)
	{
		scanSwappedWAVEData(&ptr, sizeof(chunkSize), &chunkSize);	
		
		if (memcmp(&chunkID, "fmt ", (size_t) 4) == 0)
		{
			// WAVE field: wFormatTag
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)&pcm_format);
			if (pcm_format != 1)
				return(AAFRESULT_BADWAVEDATA);
			
			// WAVE field: wChannels
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)numCh);
			
			// WAVE field: wSamplesPerSec
			scanSwappedWAVEData(&ptr, sizeof(aafUInt32), (aafUInt8 *)&rate);
			sampleRate->numerator = rate;
			sampleRate->denominator = 1;
			
			// Skip WAVE field: avgBytesPerSec (4 bytes)
			scanSwappedWAVEData(&ptr, sizeof(junk32), (aafUInt8 *)&junk32);
			
			// WAVE field wBlockAlign
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)&junk16);
			
			// WAVE field Sample Width
			scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)bitsPerSample);
			bytesPerFrame = (((*bitsPerSample) + 7) / 8) * (*numCh);
			fmtFound = kAAFTrue;
		} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
		{
			*dataLen = chunkSize / bytesPerFrame;
			// Positioned at beginning of audio data
			*dataOffset = ptr - buf;
			
			dataFound = kAAFTrue;
		}
		
		if((ptr-buf) > formSize)
			break;
		if (fmtFound && dataFound)	// Do we have all information yet?
			break;
		scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
	}	
	
	return(AAFRESULT_SUCCESS);
}

HRESULT CAAFEssenceAccess_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	AAFRESULT	hr = S_OK;

	aafWChar	tmpURL[FILENAME_MAX];

	const size_t	fileNameBufLen = 128;
	aafWChar		metadataFileName[ fileNameBufLen ] = L"";
	aafWChar		mediaFileName[ fileNameBufLen ] = L"";
	const aafWChar *rawDataWave = L"EssenceAccessExtRaw.wav";
	const aafWChar *rawDataAifc = L"EssenceAccessExtRaw.aif";
	testDataFile_t	dataFile;

	cout << endl << endl;


  cout << "    Internal Essence (WAVE)" << endl;
  
	/**/
	GenerateTestFileName( L"EssenceAccessWAVE", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testStandardCalls, kAAFCodecWAVE, kAAFDataDef_Sound, kAAFTrue);
	}
	
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCodecWAVE);
	}
	/**/
	GenerateTestFileName( L"EssenceAccessWAVELegacy", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples (Legacy DDEF_Sound)" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testStandardCalls, kAAFCodecWAVE, DDEF_Sound, kAAFTrue);
	}
	
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCodecWAVE);
	}
	/**/
	GenerateTestFileName( L"EssenceAccessMultiWAVE", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteMultiSamples" << endl;
		checkResult(CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testMultiCalls, kAAFCodecWAVE, kAAFDataDef_Sound));
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadMultiSamples" << endl;
		hr = ReadAAFFile(metadataFileName, testMultiCalls, kAAFCodecWAVE);
	}
	/**/
	/**/
	GenerateTestFileName( L"EssenceAccessRawRef", fileKind, fileNameBufLen, metadataFileName );
	dataFile.dataFilename = makeURLfromFileInCwd(rawDataWave, tmpURL);
	dataFile.dataFormat = ContainerFile;
	if(hr == AAFRESULT_SUCCESS)
	    cout << "    External Essence (WAVE)" << endl;
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples (External Raw Essence)" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				&dataFile, testStandardCalls, kAAFCodecWAVE, kAAFDataDef_Sound);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples (External Raw Essence)" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCodecWAVE);
	}
	/**/
	GenerateTestFileName( L"EssenceAccessRef", fileKind, fileNameBufLen, metadataFileName );
	GenerateTestFileName( L"EssenceAccessExtWAV", fileKind, fileNameBufLen, mediaFileName );
	dataFile.dataFilename = makeURLfromFileInCwd(mediaFileName, tmpURL);
	dataFile.dataFormat = ContainerAAF;
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples (External AAF Essence)" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				&dataFile, testStandardCalls, kAAFCodecWAVE, kAAFDataDef_Sound);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples (External AAF Essence)" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCodecWAVE);
	}

	if(hr == AAFRESULT_SUCCESS)
		  cout << "    Internal Essence (AIFC)" << endl;
  
	/**/
	GenerateTestFileName( L"EssenceAccessAIFC", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testStandardCalls, kAAFCODEC_AIFC, kAAFDataDef_Sound);
	}
	
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCODEC_AIFC);
	}
	/**/
	GenerateTestFileName( L"EssenceAccessAIFCLegacy", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples (Legacy DDEF_Sound)" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testStandardCalls, kAAFCODEC_AIFC, DDEF_Sound);
	}
	
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCODEC_AIFC);
	}
	/**/
	GenerateTestFileName( L"EssenceAccessMultiAIFC", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteMultiSamples" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testMultiCalls, kAAFCODEC_AIFC, kAAFDataDef_Sound);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
       	cout << "        ReadMultiSamples" << endl;
		hr = ReadAAFFile(metadataFileName, testMultiCalls, kAAFCODEC_AIFC);
	}
	/**/
	/**/
	GenerateTestFileName( L"EssenceAccessRawRefAIFC", fileKind, fileNameBufLen, metadataFileName );
	dataFile.dataFilename = makeURLfromFileInCwd(rawDataAifc, tmpURL);
	dataFile.dataFormat = ContainerFile;
	if(hr == AAFRESULT_SUCCESS)
		cout << "    External Essence (AIFC)" << endl;
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples (External Raw Essence)" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				&dataFile, testStandardCalls, kAAFCODEC_AIFC, kAAFDataDef_Sound);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
		cout << "        ReadSamples (External Raw Essence)" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCODEC_AIFC);
	}
	/**/
	GenerateTestFileName( L"EssenceAccessRefAIFC", fileKind, fileNameBufLen, metadataFileName );
	GenerateTestFileName( L"EssenceAccessExtAIF", fileKind, fileNameBufLen, mediaFileName );
	dataFile.dataFilename = makeURLfromFileInCwd(mediaFileName, tmpURL);
	dataFile.dataFormat = ContainerAAF;
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
        cout << "        WriteSamples (External AAF Essence)" << endl;
		hr = CreateAudioAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				&dataFile, testStandardCalls, kAAFCODEC_AIFC, kAAFDataDef_Sound);
	}
	if(hr == AAFRESULT_SUCCESS)
	{
        cout << "        ReadSamples (External AAF Essence)" << endl;
		hr = ReadAAFFile(metadataFileName, testStandardCalls, kAAFCODEC_AIFC);
	}

	if (SUCCEEDED(hr))
  {
    cout << "    Internal Essence (JPEG):" << endl;
  }
  
	GenerateTestFileName( L"EssenceAccessJPEG", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (compression disabled, RGB)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceRGB, 1, 
				kAAFCodecJPEG, kAAFDataDef_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression disabled, RGB)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceRGB, 1,  
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression enabled, RGB)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceRGB, 1,  
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}
	GenerateTestFileName( L"EssenceAccessJPEGLegacy", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (compression disabled, RGB) (Legacy DDEF_Picture)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceRGB, 1, 
				kAAFCodecJPEG, DDEF_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression disabled, RGB)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceRGB, 1,  
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}

	GenerateTestFileName( L"EssenceAccessJPEGComp", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (compression enabled, RGB)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionEnable, kAAFColorSpaceRGB, 1,  
				kAAFCodecJPEG, kAAFDataDef_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression disabled, RGB)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceRGB, 1,  
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression enabled, RGB)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceRGB, 1,  
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}

	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		GenerateTestFileName( L"EssenceAccessJPEGMulti", fileKind, fileNameBufLen, metadataFileName );
		cout << "        WriteMultiSamples (compression disabled, RGB)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceRGB, 1, 
				kAAFCodecJPEG, kAAFDataDef_Picture, testMultiCalls, NIL_UID);
		if (AAFRESULT_INVALID_OP_CODEC == hr)
		{
		cout << "          Codec does not support interleaved data." << endl;
		hr = AAFRESULT_SUCCESS;
		}
		else
		{
			if (SUCCEEDED(hr))
			{
				cout << "        ReadMultiSamples (compression disabled, RGB)" << endl;
				hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceRGB, 1,  
					kAAFCodecJPEG, testMultiCalls, NIL_UID);
			}
			if (SUCCEEDED(hr))
			{
				cout << "        ReadMultiSamples (compression enabled, RGB)" << endl;
				hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceRGB, 1,  
					kAAFCodecJPEG, testMultiCalls, NIL_UID);
			}
		}
	}

	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		GenerateTestFileName( L"EssenceAccessJPEGMultiComp", fileKind, fileNameBufLen, metadataFileName );
		cout << "        WriteMultiSamples (compression enabled, RGB)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionEnable, kAAFColorSpaceRGB,1, 
				kAAFCodecJPEG, kAAFDataDef_Picture, testMultiCalls, NIL_UID);
		if (AAFRESULT_INVALID_OP_CODEC == hr)
		{
		cout << "          Codec does not support interleaved data." << endl;
		hr = AAFRESULT_SUCCESS;
		}
		else
		{
			if (SUCCEEDED(hr))
			{
				cout << "        ReadMultiSamples (compression disabled, RGB)" << endl;
				hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceRGB, 1, 
					kAAFCodecJPEG, testMultiCalls, NIL_UID);
			}
			if (SUCCEEDED(hr))
			{
				cout << "        ReadMultiSamples (compression enabled, RGB)" << endl;
				hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceRGB, 1, 
					kAAFCodecJPEG, testMultiCalls, NIL_UID);
			}
		}
	}


	GenerateTestFileName( L"EssenceAccessJPEGCompYUV", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (compression enabled, YUV)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionEnable, kAAFColorSpaceYUV, 1, 
				kAAFCodecJPEG, kAAFDataDef_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression disabled, YUV)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceYUV, 1, 
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression enabled, YUV)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceYUV, 1, 
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression enabled, RGB)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceRGB, 1, 
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}


	GenerateTestFileName( L"EssenceAccessJPEGCompYUV422", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (compression enabled, YUV 4-2-2)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionEnable, kAAFColorSpaceYUV, 2, 
				kAAFCodecJPEG, kAAFDataDef_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression disabled, YUV)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceYUV, 2, 
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (compression enabled, YUV 4-2-2)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionEnable, kAAFColorSpaceYUV, 2, 
				kAAFCodecJPEG, testStandardCalls, NIL_UID);
	}


	if (SUCCEEDED(hr))
	{
		cout << "    Internal Static Essence (JPEG)" << endl;
	}

	GenerateTestFileName( L"StaticEssenceAccessJPEG", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples" << endl;
		hr = CreateStaticEssenceAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testStandardCalls, kAAFCodecJPEG, kAAFDataDef_Picture);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples" << endl;
		hr = ReadStaticEssenceAAFFile(metadataFileName, testStandardCalls, kAAFCodecJPEG);
	}
	GenerateTestFileName( L"StaticEssenceAccessJPEGLegacy", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (Legacy DDEF_Picture)" << endl;
		hr = CreateStaticEssenceAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, testStandardCalls, kAAFCodecJPEG, DDEF_Picture);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples" << endl;
		hr = ReadStaticEssenceAAFFile(metadataFileName, testStandardCalls, kAAFCodecJPEG);
	}


	//
	// CDCI Codec
	//
	if (SUCCEEDED(hr))
	{
		cout << "    Internal Essence (CDCI):" << endl;
	}
	
	GenerateTestFileName( L"EssenceAccessCDCICompYUV", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (YUV)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceYUV, 1, 
			kAAFCodecCDCI, kAAFDataDef_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (YUV)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceYUV, 1, 
			kAAFCodecCDCI, testStandardCalls, NIL_UID);
	}
	GenerateTestFileName( L"EssenceAccessCDCICompYUVLegacy", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (YUV) (Legacy DDEF_Picture)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceYUV, 1, 
			kAAFCodecCDCI, DDEF_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (YUV)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceYUV, 1, 
			kAAFCodecCDCI, testStandardCalls, NIL_UID);
	}
	
	GenerateTestFileName( L"EssenceAccessCDCICompYUV422", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (YUV 4-2-2)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceYUV, 2, 
			kAAFCodecCDCI, kAAFDataDef_Picture, testStandardCalls, NIL_UID);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (YUV 4-2-2)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceYUV, 2, 
			kAAFCodecCDCI, testStandardCalls, NIL_UID);
	}

	GenerateTestFileName( L"EssenceAccessCDCI_DV", fileKind, fileNameBufLen, metadataFileName );
	if(hr == AAFRESULT_SUCCESS && mode == kAAFUnitTestReadWrite)
	{
		cout << "        WriteSamples (DV CompressionDisable)" << endl;
		hr = CreateVideoAAFFile(metadataFileName, fileKind, rawStorageType, productID,
				NULL, kAAFCompressionDisable, kAAFColorSpaceYUV, 2, 
			kAAFCodecCDCI, kAAFDataDef_Picture, testStandardCalls, kAAFCompressionDef_LegacyDV);
	}
	if (SUCCEEDED(hr))
	{
		cout << "        ReadSamples (DV CompressionDisable)" << endl;
		hr = ReadVideoAAFFile(metadataFileName, kAAFCompressionDisable, kAAFColorSpaceYUV, 2, 
			kAAFCodecCDCI, testStandardCalls, kAAFCompressionDef_LegacyDV);
	}

	if(SUCCEEDED(hr))
	{
		// Global variable hrSetTransformParameters gives the result of calling
		// SetTransformParameters() within CreateAAFFile().  At the time this
		// test code was written, SetTransformParameters() was not part of the
		// current version of the reference implementation.  Therefore, as long as
		// this method returns AAFRESULT_NOT_IN_CURRENT_VERSION, it does not
		// need to be tested, and our test can return full success.  If, at some
		// point, this method returns some other result, that means it has been
		// implemented and should be tested, in which case our test can only 
		// return partial success.
		
		if(hr == AAFRESULT_SUCCESS)
			hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
//		if(hrSetTransformParameters!=AAFRESULT_NOT_IN_CURRENT_VERSION)
//		{
//			cout << "The following IAAFEssenceAccess methods have not been tested:" 
//			<< endl;  
//			cout << "     SetTransformParameters" << endl; 	
//			hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//		}
	}

	return(hr);
}
