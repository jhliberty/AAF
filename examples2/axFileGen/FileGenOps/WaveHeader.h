
//=---------------------------------------------------------------------=
//
// $Id: WaveHeader.h,v 1.8 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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
//=---------------------------------------------------------------------=

#include <AxTypes.h>

#include <memory>

class WaveHeader {
public:
	WaveHeader();
	
	WaveHeader( AxBuffer<aafUInt8>& buf );
	
	WaveHeader( int sampleRate,
				int numSamples,
				int bytePerSample,
				int bitsPerSample,
				int numChannels );

	~WaveHeader();

	WaveHeader& operator=( const WaveHeader& rhs );
	
	int GetHeaderSizeInBytes();
	
	aafUInt32 GetAudioDataSizeInBytes();
	
	void ChangeNumSamples( int numSamples );

	int GetSampleRate();
	int GetNumSamples();
	int GetBytesPerSample();
	int GetBitsPerSample();
	int GetNumChannels();
	
	// Buffer pointer, and size, returned via an AxBuffer<>.
	AxBuffer<aafUInt8> GetHeader();

private:

	enum {

		CHUNK_ID_IDX           =  0,
		CHUNK_SIZE_IDX         =  4,
		FORMAT_IDX             =  8,
		SUB_CHUNK1_ID_IDX      = 12,
		SUB_CHUNK1_SIZE_IDX    = 16,
		AUDIO_FORMAT_IDX       = 20,
		NUM_CHANNELS_IDX       = 22,
		SAMPLE_RATE_IDX        = 24,
		BYTE_RATE_IDX          = 28,
		BLOCK_ALIGN_IDX        = 32,
		BITS_PER_SAMPLE        = 34,
		SUB_CHUNK2_ID_IDX      = 36,
		SUB_CHUNK2_ID_SIZE_IDX = 40,

		CHUNK1_SIZE            = 36,

		// This is size of the ID, and SIZE fields that preceed a chunk.
		CHUNK_PREFIX_SIZE      =  8,

		HEADER_SIZE            = 44,
	};

	// not implemented...
	WaveHeader( const WaveHeader& );
	
	// No polymorphic calls to avoid any confusion over the
	// integer routine that is used, and to make it clear
	// how many bytes are written to the header.
	void SetInt32( int idx, aafUInt32 val );
	void SetInt16( int idx, aafUInt16 val );
	void SetStr4( int idx, const char str[4] );

	aafUInt32 GetInt32( int idx );
	aafUInt16 GetInt16( int idx );

	int _headerSize;

	// This must be alligned on a 32 bit boundry.
	// I'm assuming the complier is going to do that correctly.
	// There some assertions that will pick up such errors.
	aafUInt8 _header[HEADER_SIZE];

	int _sampleRate;
	int _numSamples;
	int _bytesPerSample;
	int _bitsPerSample;
	int _numChannels;

};
