//=---------------------------------------------------------------------=
//
// $Id: ResultToTextTest.cpp,v 1.9 2009/06/01 11:47:15 stuart_hc Exp $ $Name: V116 $
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

#include "AAF.h"
#include "AAFResult.h"

#include <iostream>
#include <iomanip>

void testCode(AAFRESULT code)
{
   aafUInt32 len;
   HRESULT  hr;

   std::wcout.setf(std::ios::showbase);
   std::wcout << std::hex
              << std::setw(8)
	      << std::setfill(L'0')
              << code << L" -> ";

   hr = AAFResultToTextBufLen(code, &len);
   if (AAFRESULT_SUCCEEDED(hr)) {
     aafCharacter* buffer = (aafCharacter*)new char[len];
     hr = AAFResultToText(code, buffer, len);
     if (AAFRESULT_SUCCEEDED(hr)) {
       aafUInt32 bufferSize = (wcslen(buffer) + 1) * sizeof(wchar_t);
       if (bufferSize != len) {
         std::wcout << "Buffer size mismatch." << std::endl;
       }
       std::wcout << buffer << std::endl;
     } else {
       std::wcout << L"AAFResultToText() failed." << std::endl;
     }
     delete [] buffer;
   } else {
     std::wcout << L"AAFResultToTextBufLen() failed." << std::endl;
   }
}

void positiveTests()
{
  /* SESSION/FILE Error Codes */
  testCode(AAFRESULT_BAD_SESSION);
  testCode(AAFRESULT_BADSESSIONOPEN);
  testCode(AAFRESULT_BADSESSIONMETA);
  testCode(AAFRESULT_BADSESSIONCLOSE);
  testCode(AAFRESULT_BADCONTAINER);
  testCode(AAFRESULT_FILEREV_NOT_SUPP);
  testCode(AAFRESULT_FILEREV_DIFF);
  testCode(AAFRESULT_BADOPEN);
  testCode(AAFRESULT_BADCLOSE);
  testCode(AAFRESULT_BAD_FHDL);
  testCode(AAFRESULT_BADHEAD);
  testCode(AAFRESULT_NOBYTEORDER);
  testCode(AAFRESULT_INVALID_BYTEORDER);
  testCode(AAFRESULT_NOT_AAF_FILE);
  testCode(AAFRESULT_WRONG_FILETYPE);
  testCode(AAFRESULT_WRONG_OPENMODE);
  testCode(AAFRESULT_CONTAINERWRITE);
  testCode(AAFRESULT_FILE_NOT_FOUND);
  testCode(AAFRESULT_CANNOT_SAVE_CLSD);
  testCode(AAFRESULT_CANNOT_LOAD_CLSD);
  testCode(AAFRESULT_FILE_REV_200);
  testCode(AAFRESULT_NEED_PRODUCT_IDENT);
  testCode(AAFRESULT_NOT_WRITEABLE);
  testCode(AAFRESULT_NOT_READABLE);
  testCode(AAFRESULT_FILE_EXISTS);
  testCode(AAFRESULT_NOT_OPEN);
  testCode(AAFRESULT_ALREADY_OPEN);
  testCode(AAFRESULT_BAD_FLAGS);
  testCode(AAFRESULT_BAD_FLAG_COMBINATION);
  testCode(AAFRESULT_UNSAVED_CHANGES);
  testCode(AAFRESULT_NOT_REVERTABLE);
  testCode(AAFRESULT_MEDIA_NOT_REVERTABLE);
  testCode(AAFRESULT_OPERATION_NOT_PERMITTED);

  /* Errors returned from raw storage interfaces*/
  testCode(AAFRESULT_READ_FAILURE);
  testCode(AAFRESULT_WRITE_FAILURE);
  testCode(AAFRESULT_SYNCHRONIZE_FAILURE);
  testCode(AAFRESULT_SET_EXTENT_FAILURE);
  testCode(AAFRESULT_NOT_CREATABLE);
  testCode(AAFRESULT_NOT_MODIFIABLE);
  testCode(AAFRESULT_GETSIZE_FAILURE);
  testCode(AAFRESULT_GETEXTENT_FAILURE);
  testCode(AAFRESULT_SETEXTENT_FAILURE);

  /* Read-while-modify errors */
  testCode(AAFRESULT_FILE_BEING_MODIFIED);

  /* MEDIA Error Codes */
  testCode(AAFRESULT_DESCSAMPRATE);
  testCode(AAFRESULT_SOURCEMOBLIST);
  testCode(AAFRESULT_DESCLENGTH);
  testCode(AAFRESULT_INTERNAL_MDO);
  testCode(AAFRESULT_3COMPONENT);
  testCode(AAFRESULT_INVALID_PARM_SIZE);
  testCode(AAFRESULT_BADSAMPLEOFFSET);
  testCode(AAFRESULT_ONESAMPLEREAD);
  testCode(AAFRESULT_ONESAMPLEWRITE);
  testCode(AAFRESULT_DECOMPRESS);
  testCode(AAFRESULT_NODATA);
  testCode(AAFRESULT_SMALLBUF);
  testCode(AAFRESULT_BADCOMPR);
  testCode(AAFRESULT_BADPIXFORM);
  testCode(AAFRESULT_BADLAYOUT);
  testCode(AAFRESULT_COMPRLINESWR);
  testCode(AAFRESULT_COMPRLINESRD);
  testCode(AAFRESULT_BADMEDIATYPE);
  testCode(AAFRESULT_BADDATAADDRESS);
  testCode(AAFRESULT_BAD_MDHDL);
  testCode(AAFRESULT_MEDIA_NOT_FOUND);
  testCode(AAFRESULT_ILLEGAL_MEMFMT);
  testCode(AAFRESULT_ILLEGAL_FILEFMT);
  testCode(AAFRESULT_SWABBUFSIZE);
  testCode(AAFRESULT_MISSING_SWAP_PROC);
  testCode(AAFRESULT_NULL_STREAMPROC);
  testCode(AAFRESULT_NULLBUF);
  testCode(AAFRESULT_SWAP_SETUP);
  testCode(AAFRESULT_INVALID_FILE_MOB);
  testCode(AAFRESULT_SINGLE_CHANNEL_OP);
  testCode(AAFRESULT_INVALID_CACHE_SIZE);
  testCode(AAFRESULT_NOT_FILEMOB);
  testCode(AAFRESULT_TRANSLATE_SAMPLE_SIZE);
  testCode(AAFRESULT_TRANSLATE_NON_INTEGRAL_RATE);
  testCode(AAFRESULT_MISSING_MEDIA_REP);
  testCode(AAFRESULT_NOT_LONGWORD);
  testCode(AAFRESULT_XFER_DUPCH);
  testCode(AAFRESULT_MEDIA_NOT_INIT);
  testCode(AAFRESULT_BLOCKING_SIZE);
  testCode(AAFRESULT_WRONG_MEDIATYPE);
  testCode(AAFRESULT_MULTI_WRITELEN);
  testCode(AAFRESULT_STREAM_REOPEN);
  testCode(AAFRESULT_TOO_MANY_FMT_OPS);
  testCode(AAFRESULT_MEDIASTREAM_NOTALLOWED);
  testCode(AAFRESULT_STILLFRAME_BADLENGTH);
  testCode(AAFRESULT_DATA_NONCONTIG);
  testCode(AAFRESULT_OPLIST_OVERFLOW);
  testCode(AAFRESULT_STREAM_CLOSED);
  testCode(AAFRESULT_USE_MULTI_CREATE);
  testCode(AAFRESULT_MEDIA_OPENMODE);
  testCode(AAFRESULT_MEDIA_CANNOT_CLOSE);
  testCode(AAFRESULT_CODEC_INVALID);
  testCode(AAFRESULT_INVALID_OP_CODEC);
  testCode(AAFRESULT_BAD_CODEC_REV);
  testCode(AAFRESULT_CODEC_CHANNELS);
  testCode(AAFRESULT_INTERN_TOO_SMALL);
  testCode(AAFRESULT_INTERNAL_UNKNOWN_LOC);
  testCode(AAFRESULT_TRANSLATE);
  testCode(AAFRESULT_EOF);
  testCode(AAFRESULT_TIFFVERSION);
  testCode(AAFRESULT_BADTIFFCOUNT);
  testCode(AAFRESULT_24BITVIDEO);
  testCode(AAFRESULT_JPEGBASELINE);
  testCode(AAFRESULT_BADJPEGINFO);
  testCode(AAFRESULT_BADQTABLE);
  testCode(AAFRESULT_BADACTABLE);
  testCode(AAFRESULT_BADDCTABLE);
  testCode(AAFRESULT_NOFRAMEINDEX);
  testCode(AAFRESULT_BADFRAMEOFFSET);
  testCode(AAFRESULT_JPEGPCM);
  testCode(AAFRESULT_JPEGDISABLED);
  testCode(AAFRESULT_JPEGPROBLEM);
  testCode(AAFRESULT_BADEXPORTPIXFORM);
  testCode(AAFRESULT_BADEXPORTLAYOUT);
  testCode(AAFRESULT_BADRWLINES);
  testCode(AAFRESULT_BADAIFCDATA);
  testCode(AAFRESULT_BADWAVEDATA);
  testCode(AAFRESULT_NOAUDIOCONV);
  testCode(AAFRESULT_XFER_NOT_BYTES);
  testCode(AAFRESULT_CODEC_NAME_SIZE);
  testCode(AAFRESULT_ZERO_SAMPLESIZE);
  testCode(AAFRESULT_ZERO_PIXELSIZE);
  testCode(AAFRESULT_BAD_VARIETY);
  testCode(AAFRESULT_FORMAT_BOUNDS);
  testCode(AAFRESULT_FORMAT_NOT_FOUND);
  testCode(AAFRESULT_UNKNOWN_CONTAINER);
  testCode(AAFRESULT_NO_MORE_FLAVOURS);

  /* OBJECT Error Codes */
  testCode(AAFRESULT_NULLOBJECT);
  testCode(AAFRESULT_BADINDEX);
  testCode(AAFRESULT_INVALID_LINKAGE);
  testCode(AAFRESULT_BAD_PROP);
  testCode(AAFRESULT_BAD_TYPE);
  testCode(AAFRESULT_SWAB);
  testCode(AAFRESULT_END_OF_DATA);
  testCode(AAFRESULT_PROP_NOT_PRESENT);
  testCode(AAFRESULT_INVALID_DATADEF);
  testCode(AAFRESULT_DATADEF_EXIST);
  testCode(AAFRESULT_TOO_MANY_TYPES);
  testCode(AAFRESULT_BAD_TYPE_CATEGORY);
  testCode(AAFRESULT_OBJECT_NOT_FOUND);
  testCode(AAFRESULT_IS_ROOT_CLASS);
  testCode(AAFRESULT_TYPE_NOT_FOUND);
  testCode(AAFRESULT_PROPERTY_NOT_FOUND);
  testCode(AAFRESULT_CLASS_NOT_FOUND);
  testCode(AAFRESULT_PROPERTY_DUPLICATE);
  testCode(AAFRESULT_ELEMENT_NOT_PRESENT);
  testCode(AAFRESULT_ELEMENT_NOT_OBJECT);
  testCode(AAFRESULT_PROP_ALREADY_PRESENT);

  /* MOB Error Codes */
  testCode(AAFRESULT_NOT_SOURCE_CLIP);
  testCode(AAFRESULT_FILL_FOUND);
  testCode(AAFRESULT_BAD_LENGTH);
  testCode(AAFRESULT_BADRATE);
  testCode(AAFRESULT_INVALID_ROUNDING);
  testCode(AAFRESULT_TIMECODE_NOT_FOUND);
  testCode(AAFRESULT_NO_TIMECODE);
  testCode(AAFRESULT_INVALID_TIMECODE);
  testCode(AAFRESULT_SLOT_NOT_FOUND);
  testCode(AAFRESULT_BAD_SLOTLENGTH);
  testCode(AAFRESULT_MISSING_TRACKID);
  testCode(AAFRESULT_SLOT_EXISTS);
  testCode(AAFRESULT_MOB_NOT_FOUND);
  testCode(AAFRESULT_NO_MORE_MOBS);
  testCode(AAFRESULT_DUPLICATE_MOBID);
  testCode(AAFRESULT_MISSING_MOBID);
  testCode(AAFRESULT_EFFECTDEF_EXIST);
  testCode(AAFRESULT_INVALID_EFFECTDEF);
  testCode(AAFRESULT_INVALID_EFFECT);
  testCode(AAFRESULT_INVALID_EFFECTARG);
  testCode(AAFRESULT_INVALID_CVAL);
  testCode(AAFRESULT_RENDER_NOT_FOUND);
  testCode(AAFRESULT_BAD_ITHDL);
  testCode(AAFRESULT_NO_MORE_OBJECTS);
  testCode(AAFRESULT_ITER_WRONG_TYPE);
  testCode(AAFRESULT_INVALID_SEARCH_CRIT);
  testCode(AAFRESULT_INTERNAL_ITERATOR);
  testCode(AAFRESULT_NULL_MATCHFUNC);
  testCode(AAFRESULT_NULL_CALLBACKFUNC);
  testCode(AAFRESULT_TRAVERSAL_NOT_POSS);
  testCode(AAFRESULT_INVALID_TRAN_EFFECT);
  testCode(AAFRESULT_ADJACENT_TRAN);
  testCode(AAFRESULT_LEADING_TRAN);
  testCode(AAFRESULT_INSUFF_TRAN_MATERIAL);
  testCode(AAFRESULT_PULLDOWN_DIRECTION);
  testCode(AAFRESULT_PULLDOWN_FUNC);
  testCode(AAFRESULT_PULLDOWN_KIND);
  testCode(AAFRESULT_BAD_SRCH_ITER);
  testCode(AAFRESULT_NOT_COMPOSITION);
  testCode(AAFRESULT_NOT_A_TRACK);
  testCode(AAFRESULT_PARSE_EFFECT_AMBIGUOUS);
  testCode(AAFRESULT_NO_ESSENCE_DESC);
  testCode(AAFRESULT_TAPE_DESC_ONLY);
  testCode(AAFRESULT_FILM_DESC_ONLY);
  testCode(AAFRESULT_UNKNOWN_PARAMETER_CLASS);
  testCode(AAFRESULT_PARAMETER_NOT_FOUND);
  testCode(AAFRESULT_SEGMENT_NOT_FOUND);
  testCode(AAFRESULT_ESSENCE_NOT_FOUND);
  testCode(AAFRESULT_EVENT_SEMANTICS);

  /* SIMPLE COMPOSITION Error Codes */
  testCode(AAFRESULT_BAD_STRACKHDL);
  testCode(AAFRESULT_STRACK_APPEND_ILLEGAL);

  /* Object Management Related Error Codes */
  testCode(AAFRESULT_OBJECT_ALREADY_IN_FILE);
  testCode(AAFRESULT_OBJECT_NOT_IN_FILE);
  testCode(AAFRESULT_OBJECT_ALREADY_ATTACHED);
  testCode(AAFRESULT_OBJECT_NOT_ATTACHED);
  testCode(AAFRESULT_OBJECT_ALREADY_PERSISTENT);
  testCode(AAFRESULT_OBJECT_NOT_PERSISTENT);

  /* File kind/file encoding Error Codes */
  testCode(AAFRESULT_FILEKIND_NOT_REGISTERED);

  /* GENERIC Error Codes */
  testCode(AAFRESULT_NOMEMORY);
  testCode(AAFRESULT_OFFSET_SIZE);
  testCode(AAFRESULT_INTERNAL_NEG64);
  testCode(AAFRESULT_OVERFLOW64);
  testCode(AAFRESULT_NOT_IN_CURRENT_VERSION);
  testCode(AAFRESULT_NULL_PARAM);
  testCode(AAFRESULT_ZERO_DIVIDE);
  testCode(AAFRESULT_ALREADY_INITIALIZED);
  testCode(AAFRESULT_NOT_INITIALIZED);
  testCode(AAFRESULT_INTERNAL_ERROR);
  testCode(AAFRESULT_DATA_SIZE);
  testCode(AAFRESULT_ILLEGAL_VALUE);
  testCode(AAFRESULT_INVALID_TRANSPARENCY);
  testCode(AAFRESULT_INVALID_PARAM);
  testCode(AAFRESULT_INVALID_ENUM_VALUE);

  /* SEMANTIC CHECKING Error Codes */
  testCode(AAFRESULT_REQUIRED_POSITIVE);
  testCode(AAFRESULT_INVALID_TRACKKIND);
  testCode(AAFRESULT_INVALID_EDGETYPE);
  testCode(AAFRESULT_INVALID_FILMTYPE);
  testCode(AAFRESULT_INVALID_MOBTYPE);
  testCode(AAFRESULT_INVALID_TAPECASETYPE);
  testCode(AAFRESULT_INVALID_VIDEOSIGNALTYPE);
  testCode(AAFRESULT_INVALID_TAPEFORMATTYPE);
  testCode(AAFRESULT_INVALID_EDITHINT);
  testCode(AAFRESULT_INVALID_INTERPKIND);
  testCode(AAFRESULT_INVALID_TRACK_REF);
  testCode(AAFRESULT_INVALID_OBJ);
  testCode(AAFRESULT_BAD_VIRTUAL_CREATE);
  testCode(AAFRESULT_INVALID_CLASS_ID);
  testCode(AAFRESULT_OBJECT_SEMANTIC);
  testCode(AAFRESULT_DATA_IN_SEMANTIC);
  testCode(AAFRESULT_DATA_OUT_SEMANTIC);
  testCode(AAFRESULT_TYPE_SEMANTIC);
  testCode(AAFRESULT_INVALID_ATTRIBUTEKIND);
  testCode(AAFRESULT_DATA_MDES_DISAGREE);
  testCode(AAFRESULT_CODEC_SEMANTIC_WARN);
  testCode(AAFRESULT_INVALID_BOOLTYPE);

  /* INTERNAL Error Codes */
  testCode(AAFRESULT_TABLE_DUP_KEY);
  testCode(AAFRESULT_TABLE_MISSING_COMPARE);
  testCode(AAFRESULT_TABLE_BAD_HDL);
  testCode(AAFRESULT_TABLE_BAD_ITER);
  testCode(AAFRESULT_PROPID_MATCH);
  testCode(AAFRESULT_INTERNAL_DIVIDE);
  testCode(AAFRESULT_ABSTRACT_CLASS);
  testCode(AAFRESULT_WRONG_SIZE);
  testCode(AAFRESULT_INCONSISTENCY);

  /* INTERNAL Error Codes - programming errors */
  testCode(AAFRESULT_ASSERTION_VIOLATION);
  testCode(AAFRESULT_UNEXPECTED_EXCEPTION);
  testCode(AAFRESULT_UNHANDLED_EXCEPTION);

  /* Testing Error Codes */
  testCode(AAFRESULT_TEST_FAILED);
  testCode(AAFRESULT_TEST_PARTIAL_SUCCESS);

  /* Property access error codes */
  testCode(AAFRESULT_BAD_SIZE);
  testCode(AAFRESULT_NOT_REGISTERED);
  testCode(AAFRESULT_NOT_EXTENDABLE);
  testCode(AAFRESULT_ALREADY_UNIQUELY_IDENTIFIED);
  testCode(AAFRESULT_DEFAULT_ALREADY_USED);

  /* Object extension error codes */
  testCode(AAFRESULT_EXTENSION_NOT_FOUND);
  testCode(AAFRESULT_EXTENSION_ALREADY_INITIALIZED);
  testCode(AAFRESULT_PLUGIN_NOT_REGISTERED);
  testCode(AAFRESULT_PLUGIN_ALREADY_REGISTERED);
  testCode(AAFRESULT_PLUGIN_CIRCULAR_REFERENCE);
  testCode(AAFRESULT_PLUGIN_INVALID_REFERENCE_COUNT);

  /* DLL/Shared Library runtime error codes */
  testCode(AAFRESULT_DLL_LOAD_FAILED);
  testCode(AAFRESULT_DLL_SYMBOL_NOT_FOUND);

  /* Result code -> text error codes */
  testCode(AAFRESULT_RESULT_NOT_AAF);
  testCode(AAFRESULT_RESULT_NOT_RECOGNIZED);
}

void negativeTests()
{
  AAFRESULT x = AAFRESULT_BADOPEN;
  HRESULT y;
  y = AAFResultToTextBufLen(x, 0);
  if (y != AAFRESULT_NULL_PARAM) {
    std::wcout << "*** Fail." << std::endl;
  }
  y = AAFResultToText(x, 0, 99);
  if (y != AAFRESULT_NULL_PARAM) {
    std::wcout << "*** Fail." << std::endl;
  }
  wchar_t tooSmall[1];
  aafUInt32 len = sizeof(tooSmall) * sizeof(wchar_t);
  y = AAFResultToText(x, tooSmall, len);
  if (y != AAFRESULT_SMALLBUF) {
    std::wcout << "*** Fail." << std::endl;
  }
  /* AAFRESULT_NOT_IMPLEMENTED is not an AAF error code - it's a
     redefinition of E_NOTIMPL from facility ITF */
  y = AAFResultToTextBufLen(AAFRESULT_NOT_IMPLEMENTED, &len);
  if (y != AAFRESULT_RESULT_NOT_AAF) {
    std::wcout << "*** Fail." << std::endl;
  }
  /* 0x8012ffff is unallocated */
  y = AAFResultToTextBufLen(0x8012ffff, &len);
  if (y != AAFRESULT_RESULT_NOT_RECOGNIZED) {
    std::wcout << "*** Fail." << std::endl;
  }
  /* AAFRESULT_SUCCESS is not an AAF error code - it's a
     redefinition of S_OK */
  y = AAFResultToTextBufLen(AAFRESULT_SUCCESS, &len);
  if (y != AAFRESULT_RESULT_NOT_AAF) {
    std::wcout << "*** Fail." << std::endl;
  }
}

int main()
{
  positiveTests();

  negativeTests();
}
