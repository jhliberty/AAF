//=---------------------------------------------------------------------=
//
// $Id: AAFMetaResult.h,v 1.23 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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

#if !defined(AAF_DEFINE_ERROR)
#define AAF_DEFINE_ERROR(name, code, description)
#endif

#if !defined(AAF_DEFINE_SUCCESS)
#define AAF_DEFINE_SUCCESS(name, code)
#endif

#if !defined(AAF_ERROR_SECTION)
#define AAF_ERROR_SECTION(description)
#endif

#if !defined(AAF_DEFINE_ERROR_ALIAS)
#define AAF_DEFINE_ERROR_ALIAS(fullOldName, name)
#endif

AAF_ERROR_SECTION("Success codes")
AAF_DEFINE_SUCCESS(SUCCESS, 0)

AAF_ERROR_SECTION("SESSION/FILE Error Codes")
AAF_DEFINE_ERROR(BAD_SESSION,                    0x0010, "")
AAF_DEFINE_ERROR(BADSESSIONOPEN,                 0x0011, "")
AAF_DEFINE_ERROR(BADSESSIONMETA,                 0x0012, "")
AAF_DEFINE_ERROR(BADSESSIONCLOSE,                0x0013, "")
AAF_DEFINE_ERROR(BADCONTAINER,                   0x0014, "")
AAF_DEFINE_ERROR(FILEREV_NOT_SUPP,               0x0015, "")
AAF_DEFINE_ERROR(FILEREV_DIFF,                   0x0016, "")
AAF_DEFINE_ERROR(OPEN_FAILURE,                   0x0017,
    "Failed to open the file")
AAF_DEFINE_ERROR(BADCLOSE,                       0x0018, "")
AAF_DEFINE_ERROR(BAD_FHDL,                       0x0019, "")
AAF_DEFINE_ERROR(BADHEAD,                        0x001A, "")
AAF_DEFINE_ERROR(NOBYTEORDER,                    0x001B, "")
AAF_DEFINE_ERROR(INVALID_BYTEORDER,              0x001C, "")
AAF_DEFINE_ERROR(NOT_AAF_FILE,                   0x001D, "")
AAF_DEFINE_ERROR(WRONG_FILETYPE,                 0x001E, "")
AAF_DEFINE_ERROR(WRONG_OPENMODE,                 0x001F, "")
AAF_DEFINE_ERROR(CONTAINERWRITE,                 0x0020, "")
AAF_DEFINE_ERROR(FILE_NOT_FOUND,                 0x0021, "")
AAF_DEFINE_ERROR(CANNOT_SAVE_CLSD,               0x0022, "")
AAF_DEFINE_ERROR(CANNOT_LOAD_CLSD,               0x0023, "")
AAF_DEFINE_ERROR(FILE_REV_200,                   0x0024, "")
AAF_DEFINE_ERROR(NEED_PRODUCT_IDENT,             0x0025, "")
AAF_DEFINE_ERROR(NOT_WRITEABLE,                  0x0026, "")
AAF_DEFINE_ERROR(NOT_READABLE,                   0x0027, "")
AAF_DEFINE_ERROR(FILE_EXISTS,                    0x0028, "")
AAF_DEFINE_ERROR(NOT_OPEN,                       0x0029, "")
AAF_DEFINE_ERROR(ALREADY_OPEN,                   0x002A, "")
AAF_DEFINE_ERROR(BAD_FLAGS,                      0x002B, "")
AAF_DEFINE_ERROR(BAD_FLAG_COMBINATION,           0x002C, "")
AAF_DEFINE_ERROR(UNSAVED_CHANGES,                0x002D, "")
AAF_DEFINE_ERROR(NOT_REVERTABLE,                 0x002E, "")
AAF_DEFINE_ERROR(MEDIA_NOT_REVERTABLE,           0x002F, "")
AAF_DEFINE_ERROR(OPERATION_NOT_PERMITTED,        0x0030, "")

AAF_ERROR_SECTION("Errors returned from raw storage interfaces")
AAF_DEFINE_ERROR(READ_FAILURE,                   0x0031, "")
AAF_DEFINE_ERROR(WRITE_FAILURE,                  0x0032, "")
AAF_DEFINE_ERROR(SYNCHRONIZE_FAILURE,            0x0033, "")
AAF_DEFINE_ERROR(SET_EXTENT_FAILURE,             0x0034, "")
AAF_DEFINE_ERROR(NOT_CREATABLE,                  0x0035, "")
AAF_DEFINE_ERROR(NOT_MODIFIABLE,                 0x0036, "")
AAF_DEFINE_ERROR(GETSIZE_FAILURE,                0x0037, "")
AAF_DEFINE_ERROR(GETEXTENT_FAILURE,              0x0038, "")
AAF_DEFINE_ERROR(SETEXTENT_FAILURE,              0x0039, "")

AAF_ERROR_SECTION("Read-while-modify errors")
AAF_DEFINE_ERROR(FILE_BEING_MODIFIED,            0x003A,
    "The file is being modified (the header partition is open and incomplete)")

AAF_ERROR_SECTION("MEDIA Error Codes")
AAF_DEFINE_ERROR(DESCSAMPRATE,                   0x0064, "")
AAF_DEFINE_ERROR(SOURCEMOBLIST,                  0x0065, "")
AAF_DEFINE_ERROR(DESCLENGTH,                     0x0066, "")
AAF_DEFINE_ERROR(INTERNAL_MDO,                   0x0067, "")
AAF_DEFINE_ERROR(3COMPONENT,                     0x0068, "")
AAF_DEFINE_ERROR(INVALID_PARM_SIZE,              0x0069, "")
AAF_DEFINE_ERROR(BADSAMPLEOFFSET,                0x006A, "")
AAF_DEFINE_ERROR(ONESAMPLEREAD,                  0x006B, "")
AAF_DEFINE_ERROR(ONESAMPLEWRITE,                 0x006C, "")
AAF_DEFINE_ERROR(DECOMPRESS,                     0x006D, "")
AAF_DEFINE_ERROR(NODATA,                         0x006E, "")
AAF_DEFINE_ERROR(SMALLBUF,                       0x006F,
    "Sample too large to fit in given buffer")
AAF_DEFINE_ERROR(BADCOMPR,                       0x0070, "")
AAF_DEFINE_ERROR(BADPIXFORM,                     0x0071, "")
AAF_DEFINE_ERROR(BADLAYOUT,                      0x0072, "")
AAF_DEFINE_ERROR(COMPRLINESWR,                   0x0073, "")
AAF_DEFINE_ERROR(COMPRLINESRD,                   0x0074, "")
AAF_DEFINE_ERROR(BADMEDIATYPE,                   0x0075, "")
AAF_DEFINE_ERROR(BADDATAADDRESS,                 0x0076, "")
AAF_DEFINE_ERROR(BAD_MDHDL,                      0x0077, "")
AAF_DEFINE_ERROR(MEDIA_NOT_FOUND,                0x0078, "")
AAF_DEFINE_ERROR(ILLEGAL_MEMFMT,                 0x0079, "")
AAF_DEFINE_ERROR(ILLEGAL_FILEFMT,                0x007A, "")
AAF_DEFINE_ERROR(SWABBUFSIZE,                    0x007B, "")
AAF_DEFINE_ERROR(MISSING_SWAP_PROC,              0x007C, "")
AAF_DEFINE_ERROR(NULL_STREAMPROC,                0x007D, "")
AAF_DEFINE_ERROR(NULLBUF,                        0x007E, "")
AAF_DEFINE_ERROR(SWAP_SETUP,                     0x007F, "")
AAF_DEFINE_ERROR(INVALID_FILE_MOB,               0x0080, "")
AAF_DEFINE_ERROR(SINGLE_CHANNEL_OP,              0x0081, "")
AAF_DEFINE_ERROR(INVALID_CACHE_SIZE,             0x0082, "")
AAF_DEFINE_ERROR(NOT_FILEMOB,                    0x0083, "")
AAF_DEFINE_ERROR(TRANSLATE_SAMPLE_SIZE,          0x0084, "")
AAF_DEFINE_ERROR(TRANSLATE_NON_INTEGRAL_RATE,    0x0085, "")
AAF_DEFINE_ERROR(MISSING_MEDIA_REP,              0x0086, "")
AAF_DEFINE_ERROR(NOT_LONGWORD,                   0x0087, "")
AAF_DEFINE_ERROR(XFER_DUPCH,                     0x0088, "")
AAF_DEFINE_ERROR(MEDIA_NOT_INIT,                 0x0089, "")
AAF_DEFINE_ERROR(BLOCKING_SIZE,                  0x008A, "")
AAF_DEFINE_ERROR(WRONG_MEDIATYPE,                0x008B, "")
AAF_DEFINE_ERROR(MULTI_WRITELEN,                 0x008C, "")
AAF_DEFINE_ERROR(STREAM_REOPEN,                  0x008D, "")
AAF_DEFINE_ERROR(TOO_MANY_FMT_OPS,               0x008E, "")
AAF_DEFINE_ERROR(MEDIASTREAM_NOTALLOWED,         0x008F, "")
AAF_DEFINE_ERROR(STILLFRAME_BADLENGTH,           0x0090, "")
AAF_DEFINE_ERROR(DATA_NONCONTIG,                 0x0091, "")
AAF_DEFINE_ERROR(OPLIST_OVERFLOW,                0x0092, "")
AAF_DEFINE_ERROR(STREAM_CLOSED,                  0x0093, "")
AAF_DEFINE_ERROR(USE_MULTI_CREATE,               0x0094, "")
AAF_DEFINE_ERROR(MEDIA_OPENMODE,                 0x0095, "")
AAF_DEFINE_ERROR(MEDIA_CANNOT_CLOSE,             0x0096, "")
AAF_DEFINE_ERROR(CODEC_INVALID,                  0x0097, "")
AAF_DEFINE_ERROR(INVALID_OP_CODEC,               0x0098, "")
AAF_DEFINE_ERROR(BAD_CODEC_REV,                  0x0099, "")
AAF_DEFINE_ERROR(CODEC_CHANNELS,                 0x009A, "")
AAF_DEFINE_ERROR(INTERN_TOO_SMALL,               0x009B, "")
AAF_DEFINE_ERROR(INTERNAL_UNKNOWN_LOC,           0x009C, "")
AAF_DEFINE_ERROR(TRANSLATE,                      0x009D, "")
AAF_DEFINE_ERROR(EOF,                            0x009E, "")
AAF_DEFINE_ERROR(TIFFVERSION,                    0x009F, "")
AAF_DEFINE_ERROR(BADTIFFCOUNT,                   0x00A0, "")
AAF_DEFINE_ERROR(24BITVIDEO,                     0x00A1, "")
AAF_DEFINE_ERROR(JPEGBASELINE,                   0x00A2, "")
AAF_DEFINE_ERROR(BADJPEGINFO,                    0x00A3, "")
AAF_DEFINE_ERROR(BADQTABLE,                      0x00A4, "")
AAF_DEFINE_ERROR(BADACTABLE,                     0x00A5, "")
AAF_DEFINE_ERROR(BADDCTABLE,                     0x00A6, "")
AAF_DEFINE_ERROR(NOFRAMEINDEX,                   0x00A7, "")
AAF_DEFINE_ERROR(BADFRAMEOFFSET,                 0x00A8, "")
AAF_DEFINE_ERROR(JPEGPCM,                        0x00A9, "")
AAF_DEFINE_ERROR(JPEGDISABLED,                   0x00AA, "")
AAF_DEFINE_ERROR(JPEGPROBLEM,                    0x00AB, "")
AAF_DEFINE_ERROR(BADEXPORTPIXFORM,               0x00AC, "")
AAF_DEFINE_ERROR(BADEXPORTLAYOUT,                0x00AD, "")
AAF_DEFINE_ERROR(BADRWLINES,                     0x00AE, "")
AAF_DEFINE_ERROR(BADAIFCDATA,                    0x00AF, "")
AAF_DEFINE_ERROR(BADWAVEDATA,                    0x00B0, "")
AAF_DEFINE_ERROR(NOAUDIOCONV,                    0x00B1, "")
AAF_DEFINE_ERROR(XFER_NOT_BYTES,                 0x00B2, "")
AAF_DEFINE_ERROR(CODEC_NAME_SIZE,                0x00B3, "")
AAF_DEFINE_ERROR(ZERO_SAMPLESIZE,                0x00B4, "")
AAF_DEFINE_ERROR(ZERO_PIXELSIZE,                 0x00B5, "")
AAF_DEFINE_ERROR(BAD_VARIETY,                    0x00B6, "")
AAF_DEFINE_ERROR(FORMAT_BOUNDS,                  0x00B7, "")     
AAF_DEFINE_ERROR(FORMAT_NOT_FOUND,               0x00B8, "")
AAF_DEFINE_ERROR(UNKNOWN_CONTAINER,              0x00B9, "")
AAF_DEFINE_ERROR(NO_MORE_FLAVOURS,               0x00BA, "")

AAF_ERROR_SECTION("OBJECT Error Codes")
AAF_DEFINE_ERROR(NULLOBJECT,                     0x00C8,
    "NULL object not allowed")
AAF_DEFINE_ERROR(BADINDEX,                       0x00C9,
    "Array index out of range")
AAF_DEFINE_ERROR(INVALID_LINKAGE,                0x00CA, "")
AAF_DEFINE_ERROR(BAD_PROP,                       0x00CB,
    "Property code out of range")
AAF_DEFINE_ERROR(BAD_TYPE,                       0x00CC,
    "Type code out of range")
AAF_DEFINE_ERROR(SWAB,                           0x00CD, "")
AAF_DEFINE_ERROR(END_OF_DATA,                    0x00CE, "")
AAF_DEFINE_ERROR(PROP_NOT_PRESENT,               0x00CF,
    "Property missing from the file")
AAF_DEFINE_ERROR(INVALID_DATADEF,                0x00D0,
    "Invalid data definition")
AAF_DEFINE_ERROR(DATADEF_EXIST,                  0x00D1, "")
AAF_DEFINE_ERROR(TOO_MANY_TYPES,                 0x00D2, "")
AAF_DEFINE_ERROR(BAD_TYPE_CATEGORY,              0x00D5, "")
AAF_DEFINE_ERROR(OBJECT_NOT_FOUND,               0x00D6,
    "Object not found")
AAF_DEFINE_ERROR(IS_ROOT_CLASS,                  0x00D7, "")
AAF_DEFINE_ERROR(TYPE_NOT_FOUND,                 0x00D8,
    "Type not found")
AAF_DEFINE_ERROR(PROPERTY_NOT_FOUND,             0x00D9, "")
AAF_DEFINE_ERROR(CLASS_NOT_FOUND,                0x00DA, "")
AAF_DEFINE_ERROR(PROPERTY_DUPLICATE,             0x00DB, "")
AAF_DEFINE_ERROR(ELEMENT_NOT_PRESENT,            0x00DC, "")
AAF_DEFINE_ERROR(ELEMENT_NOT_OBJECT,             0x00DD, "")
AAF_DEFINE_ERROR(PROP_ALREADY_PRESENT,           0x00DE, "")

AAF_ERROR_SECTION("MOB Error Codes")
AAF_DEFINE_ERROR(NOT_SOURCE_CLIP,                0x00FA, "")
AAF_DEFINE_ERROR(FILL_FOUND,                     0x00FB, "")
AAF_DEFINE_ERROR(BAD_LENGTH,                     0x00FC, "")
AAF_DEFINE_ERROR(BADRATE,                        0x00FD, "")
AAF_DEFINE_ERROR(INVALID_ROUNDING,               0x00FE, "")
AAF_DEFINE_ERROR(TIMECODE_NOT_FOUND,             0x00FF, "")
AAF_DEFINE_ERROR(NO_TIMECODE,                    0x0100, "")
AAF_DEFINE_ERROR(INVALID_TIMECODE,               0x0101, "")
AAF_DEFINE_ERROR(SLOT_NOT_FOUND,                 0x0102, "")
AAF_DEFINE_ERROR(BAD_SLOTLENGTH,                 0x0103, "")
AAF_DEFINE_ERROR(MISSING_TRACKID,                0x0104, "")
AAF_DEFINE_ERROR(SLOT_EXISTS,                    0x0105, "")
AAF_DEFINE_ERROR(MOB_NOT_FOUND,                  0x0106, "")
AAF_DEFINE_ERROR(NO_MORE_MOBS,                   0x0107, "")
AAF_DEFINE_ERROR(DUPLICATE_MOBID,                0x0108, "")
AAF_DEFINE_ERROR(MISSING_MOBID,                  0x0109, "")
AAF_DEFINE_ERROR(EFFECTDEF_EXIST,                0x010A, "")
AAF_DEFINE_ERROR(INVALID_EFFECTDEF,              0x010B, "")
AAF_DEFINE_ERROR(INVALID_EFFECT,                 0x010C, "")
AAF_DEFINE_ERROR(INVALID_EFFECTARG,              0x010D, "")
AAF_DEFINE_ERROR(INVALID_CVAL,                   0x010E, "")
AAF_DEFINE_ERROR(RENDER_NOT_FOUND,               0x010F, "")
AAF_DEFINE_ERROR(BAD_ITHDL,                      0x0110, "")
AAF_DEFINE_ERROR(NO_MORE_OBJECTS,                0x0111,
    "No More Objects")
AAF_DEFINE_ERROR(ITER_WRONG_TYPE,                0x0112, "")
AAF_DEFINE_ERROR(INVALID_SEARCH_CRIT,            0x0113, "")
AAF_DEFINE_ERROR(INTERNAL_ITERATOR,              0x0114, "")
AAF_DEFINE_ERROR(NULL_MATCHFUNC,                 0x0115, "")
AAF_DEFINE_ERROR(NULL_CALLBACKFUNC,              0x0116, "")
AAF_DEFINE_ERROR(TRAVERSAL_NOT_POSS,             0x0117,
    "Mob traversal failed")
AAF_DEFINE_ERROR(INVALID_TRAN_EFFECT,            0x0118, "")
AAF_DEFINE_ERROR(ADJACENT_TRAN,                  0x0119, "")
AAF_DEFINE_ERROR(LEADING_TRAN,                   0x011A, "")
AAF_DEFINE_ERROR(INSUFF_TRAN_MATERIAL,           0x011B, "")
AAF_DEFINE_ERROR(PULLDOWN_DIRECTION,             0x011C, "")
AAF_DEFINE_ERROR(PULLDOWN_FUNC,                  0x011D, "")
AAF_DEFINE_ERROR(PULLDOWN_KIND,                  0x011E, "")
AAF_DEFINE_ERROR(BAD_SRCH_ITER,                  0x011F, "")
AAF_DEFINE_ERROR(NOT_COMPOSITION,                0x0120, "")
AAF_DEFINE_ERROR(NOT_A_TRACK,                    0x0121, "")
AAF_DEFINE_ERROR(PARSE_EFFECT_AMBIGUOUS,         0x0122, "")
AAF_DEFINE_ERROR(NO_ESSENCE_DESC,                0x0123, "")
AAF_DEFINE_ERROR(TAPE_DESC_ONLY,                 0x0124, "")
AAF_DEFINE_ERROR(FILM_DESC_ONLY,                 0x0125, "")
AAF_DEFINE_ERROR(UNKNOWN_PARAMETER_CLASS,        0x0126, "")
AAF_DEFINE_ERROR(PARAMETER_NOT_FOUND,            0x0127, "") 
AAF_DEFINE_ERROR(SEGMENT_NOT_FOUND,              0x0128, "") 
AAF_DEFINE_ERROR(ESSENCE_NOT_FOUND,              0x0129, "") 
AAF_DEFINE_ERROR(EVENT_SEMANTICS,                0x012A, "")

AAF_ERROR_SECTION("SIMPLE COMPOSITION Error Codes")
AAF_DEFINE_ERROR(BAD_STRACKHDL,                  0x012C, "")
AAF_DEFINE_ERROR(STRACK_APPEND_ILLEGAL,          0x012D, "")

AAF_ERROR_SECTION("Object Management Related Error Codes")
AAF_DEFINE_ERROR(OBJECT_ALREADY_IN_FILE,         0x0130, "")
AAF_DEFINE_ERROR(OBJECT_NOT_IN_FILE,             0x0131, "")
AAF_DEFINE_ERROR(OBJECT_ALREADY_ATTACHED,        0x0132,
    "Object already attached")
AAF_DEFINE_ERROR(OBJECT_NOT_ATTACHED,            0x0133,
    "Object not attached")
AAF_DEFINE_ERROR(OBJECT_ALREADY_PERSISTENT,      0x0134, "")
AAF_DEFINE_ERROR(OBJECT_NOT_PERSISTENT,          0x0135,
    "Object not persistent")

AAF_ERROR_SECTION("File kind/file encoding Error Codes")
AAF_DEFINE_ERROR(FILEKIND_NOT_REGISTERED,        0x0140, "")

AAF_ERROR_SECTION("GENERIC Error Codes")
AAF_DEFINE_ERROR(NOMEMORY,                       0x015E,
    "Memory allocation failed, no more heap memory")
AAF_DEFINE_ERROR(OFFSET_SIZE,                    0x015F, "")
AAF_DEFINE_ERROR(INTERNAL_NEG64,                 0x0160, "")
AAF_DEFINE_ERROR(OVERFLOW64,                     0x0161, "")
AAF_DEFINE_ERROR(NOT_IN_CURRENT_VERSION,         0x0162,
    "Not implemented in the current version")
AAF_DEFINE_ERROR(NULL_PARAM,                     0x0164,
    "NULL actual parameter to function call")
AAF_DEFINE_ERROR(ZERO_DIVIDE,                    0x0165, "")
AAF_DEFINE_ERROR(ALREADY_INITIALIZED,            0x0166,
    "Object already initialized")
AAF_DEFINE_ERROR(NOT_INITIALIZED,                0x0167,
    "Object not initialized")
AAF_DEFINE_ERROR(INTERNAL_ERROR,                 0x0168,
    "Internal error")
AAF_DEFINE_ERROR(DATA_SIZE,                      0x0169, "")
AAF_DEFINE_ERROR(ILLEGAL_VALUE,                  0x016A,
    "Illegal value")
AAF_DEFINE_ERROR(INVALID_TRANSPARENCY,           0x016B, "")
AAF_DEFINE_ERROR(INVALID_PARAM,                  0x016C,
    "Invalid parameter")
AAF_DEFINE_ERROR(INVALID_ENUM_VALUE,             0x016D, "")

AAF_ERROR_SECTION("SEMANTIC CHECKING Error Codes")
AAF_DEFINE_ERROR(REQUIRED_POSITIVE,              0x0190, "")
AAF_DEFINE_ERROR(INVALID_TRACKKIND,              0x0191, "")
AAF_DEFINE_ERROR(INVALID_EDGETYPE,               0x0192, "")
AAF_DEFINE_ERROR(INVALID_FILMTYPE,               0x0193, "")
AAF_DEFINE_ERROR(INVALID_MOBTYPE,                0x0194, "")
AAF_DEFINE_ERROR(INVALID_TAPECASETYPE,           0x0195, "")
AAF_DEFINE_ERROR(INVALID_VIDEOSIGNALTYPE,        0x0196, "")
AAF_DEFINE_ERROR(INVALID_TAPEFORMATTYPE,         0x0197, "")
AAF_DEFINE_ERROR(INVALID_EDITHINT,               0x0198, "")
AAF_DEFINE_ERROR(INVALID_INTERPKIND,             0x0199, "")
AAF_DEFINE_ERROR(INVALID_TRACK_REF,              0x019A, "")
AAF_DEFINE_ERROR(INVALID_OBJ,                    0x019B,
    "Invalid object for this operation")
AAF_DEFINE_ERROR(BAD_VIRTUAL_CREATE,             0x019C, "")
AAF_DEFINE_ERROR(INVALID_CLASS_ID,               0x019D, "")
AAF_DEFINE_ERROR(OBJECT_SEMANTIC,                0x019E, "")
AAF_DEFINE_ERROR(DATA_IN_SEMANTIC,               0x019F, "")
AAF_DEFINE_ERROR(DATA_OUT_SEMANTIC,              0x01A0, "")
AAF_DEFINE_ERROR(TYPE_SEMANTIC,                  0x01A1, "")
AAF_DEFINE_ERROR(INVALID_ATTRIBUTEKIND,          0x01A2, "")
AAF_DEFINE_ERROR(DATA_MDES_DISAGREE,             0x01A3, "")
AAF_DEFINE_ERROR(CODEC_SEMANTIC_WARN,            0x01A4, "")
AAF_DEFINE_ERROR(INVALID_BOOLTYPE,               0x01A5, "")

AAF_ERROR_SECTION("INTERNAL Error Codes")
AAF_DEFINE_ERROR(TABLE_DUP_KEY,                  0x01C2, "")
AAF_DEFINE_ERROR(TABLE_MISSING_COMPARE,          0x01C3, "")
AAF_DEFINE_ERROR(TABLE_BAD_HDL,                  0x01C4,
    "Bad table handle")
AAF_DEFINE_ERROR(TABLE_BAD_ITER,                 0x01C5, "")
AAF_DEFINE_ERROR(PROPID_MATCH,                   0x01C8, "")
AAF_DEFINE_ERROR(INTERNAL_DIVIDE,                0x01C9, "")
AAF_DEFINE_ERROR(ABSTRACT_CLASS,                 0x01CA,
    "Abstract class")
AAF_DEFINE_ERROR(WRONG_SIZE,                     0x01CB, "")
AAF_DEFINE_ERROR(INCONSISTENCY,                  0x01CC, "")

AAF_ERROR_SECTION("INTERNAL Error Codes - programming errors")
AAF_DEFINE_ERROR(ASSERTION_VIOLATION,            0x01D0,
    "Reference implementation assertion violation (programming error)")
AAF_DEFINE_ERROR(UNEXPECTED_EXCEPTION,           0x01D1,
    "Reference implementation unexpected C++ exception (programming error)")
AAF_DEFINE_ERROR(UNHANDLED_EXCEPTION,            0x01D2,
    "Reference implementation unhandled exception (programming error)")

AAF_ERROR_SECTION("Testing Error Codes")
AAF_DEFINE_ERROR(TEST_FAILED,                    0x01F4, "")
AAF_DEFINE_ERROR(TEST_PARTIAL_SUCCESS,           0x01F5, "")

AAF_ERROR_SECTION("Property access error codes")
AAF_DEFINE_ERROR(BAD_SIZE,                       0x0200,
    "Bad size")
AAF_DEFINE_ERROR(NOT_REGISTERED,                 0x0201,
    "Not registered")
AAF_DEFINE_ERROR(NOT_EXTENDABLE,                 0x0202, "")
AAF_DEFINE_ERROR(ALREADY_UNIQUELY_IDENTIFIED,    0x0203, "")
AAF_DEFINE_ERROR(DEFAULT_ALREADY_USED,           0x0204, "")

AAF_ERROR_SECTION("Object extension error codes")
AAF_DEFINE_ERROR(EXTENSION_NOT_FOUND,            0x0300, "")
AAF_DEFINE_ERROR(EXTENSION_ALREADY_INITIALIZED,  0x0301, "")
AAF_DEFINE_ERROR(PLUGIN_NOT_REGISTERED,          0x0302, "")
AAF_DEFINE_ERROR(PLUGIN_ALREADY_REGISTERED,      0x0303, "")
AAF_DEFINE_ERROR(PLUGIN_CIRCULAR_REFERENCE,      0x0304, "")
AAF_DEFINE_ERROR(PLUGIN_INVALID_REFERENCE_COUNT, 0x0305, "")

AAF_ERROR_SECTION("DLL/Shared Library runtime error codes")
AAF_DEFINE_ERROR(DLL_LOAD_FAILED,                0x0400,
    "Either the DLL or a dependent DLL could not be found and loaded")
AAF_DEFINE_ERROR(DLL_SYMBOL_NOT_FOUND,           0x0401,
    "Expected symbol not exported from DLL. Need newer DLL")

AAF_ERROR_SECTION("Result code -> text error codes")
AAF_DEFINE_ERROR(RESULT_NOT_AAF,                 0x0500,
    "The specified result code is not an AAF result code")
AAF_DEFINE_ERROR(RESULT_NOT_RECOGNIZED,          0x0501,
    "The specified AAF result code is not recognized")

AAF_ERROR_SECTION("For compatibility")
AAF_DEFINE_ERROR_ALIAS(AAF_RESULT_USE_MULTI_CREATE, USE_MULTI_CREATE)
AAF_DEFINE_ERROR_ALIAS(AAFRESULT_INCONSISTANCY, INCONSISTENCY)
AAF_DEFINE_ERROR_ALIAS(AAFRESULT_BADOPEN, OPEN_FAILURE)

AAF_DEFINE_ERROR_ALIAS(HRESULT_NOT_IMPLEMENTED, NOT_IMPLEMENTED)
AAF_DEFINE_ERROR_ALIAS(HRESULT_SUCCESS, SUCCESS)

#undef AAF_DEFINE_ERROR
#undef AAF_DEFINE_SUCCESS
#undef AAF_ERROR_SECTION
#undef AAF_DEFINE_ERROR_ALIAS

//localErrorStrings[BADOBJ] =
//"AAF_ERR: Null AAF object";

//localErrorStrings[INULLMOBIDPTR] =
//"AAF_ERR(Import): NULL Mob ID pointer in GetNthMediaDesc/SetMediaDesc";

//localErrorStrings[INULLMTPTR] =
//"AAF_ERR(Import): NULL omfMediaType_t pointer in GetNthMediaDesc";

//localErrorStrings[INULLDESCPTR] =
//"AAF_ERR(Import): NULL descriptor pointer in GetNthMediaDesc";

//localErrorStrings[IBADMEDIAINDEX] =
//"AAF_ERR(Import): Input AAF file does not have enough media objects";

//localErrorStrings[LRC_DISABLED] =
//"AAF_ERR: LRC codec disabled";

//localErrorStrings[LRC_BADSAMPLESIZE] =
//"AAF_ERR: Only 16 bit samples supported for LRC";

//localErrorStrings[LRC_MONOONLY] =
//   "AAF_ERR: Only one channel audio supported for LRC";

//localErrorStrings[BADSAMPLEOFFSET] =
//"AAF_ERR: Sample Index offset out of range";


/*** SESSION/FILE Error Codes ***/
/* Session Error Codes */
//localErrorStrings[BAD_SESSION] =
//"AAF_ERR: Bad Session Handle";

//localErrorStrings[BADSESSIONOPEN] =
//"AAF_ERR: Container Session open failed";

//localErrorStrings[BADSESSIONMETA] =
//"AAF_ERR: Container Session meta-handler operation failed";

//localErrorStrings[BADSESSIONCLOSE] =
//"AAF_ERR: Container Session close failed";

/* File Error Codes */
//localErrorStrings[FILEREV_NOT_SUPP] =
//"AAF_ERR: AAF File Revision > 2.0 not supported";

//localErrorStrings[FILEREV_DIFF] = 
//"AAF_ERR: Files must have same file revisions";

//localErrorStrings[BADOPEN] =
//"AAF_ERR: Cannot open file";

//localErrorStrings[BADCLOSE] =
//"AAF_ERR: Cannot close file";

//localErrorStrings[BAD_FHDL] =
//"AAF_ERR: Bad File Handle";

//localErrorStrings[BADHEAD] =
//"AAF_ERR: Invalid AAF HEAD object";

//localErrorStrings[NOBYTEORDER] =
//"AAF_ERR: No File Byte Order Specified";

//localErrorStrings[INVALID_BYTEORDER] =
//"AAF_ERR: Invalid byte order";

//localErrorStrings[NOTAAFFILE] =
//"AAF_ERR: Invalid AAF file";

//localErrorStrings[WRONG_FILETYPE] =
//"AAF_ERR: Invalid file type (Raw vs. AAF)";

//localErrorStrings[WRONG_OPENMODE] =
//"AAF_ERR: File opened in wrong mode (readonly vs. readwrite)";

//localErrorStrings[CONTAINERWRITE] = 
//"AAF_ERR: Error writing to container media (Possibly media is full).";

//localErrorStrings[FILE_NOT_FOUND] = 
//"AAF_ERR: File not found.";

/* Class Dictionary Error Codes */
//localErrorStrings[CANNOT_SAVE_CLSD] = 
//"AAF_ERR: Error updating the class dictionary in the AAF file";

//localErrorStrings[CANNOT_LOAD_CLSD] = 
//"AAF_ERR: Error loading the class dictionary from the file";

//localErrorStrings[FILE_REV_200] = 
//"AAF_ERR: Please use toolkit revision 2.0.1 or later to write 2.x compliant files";

//localErrorStrings[NEED_PRODUCT_IDENT] = 
//"AAF_ERR: You must supply a product identification struct when creating or updating files";


/*** MEDIA Error Codes ***/
/* General Media Error Codes */
//localErrorStrings[DESCSAMPRATE] =
//"AAF_ERR: Error getting sample rate from file descriptor";

//localErrorStrings[SOURCEMOBLIST] =
//"AAF_ERR: Error processing source mob list";

//localErrorStrings[DESCLENGTH] =
//"AAF_ERR: Error getting length from file descriptor";

//localErrorStrings[INTERNAL_MDO] =
//"AAF_ERR: Internal Media Data system not initialized";

//localErrorStrings[3COMPONENT] =
//"AAF_ERR: Only 3-component video allowed in AAF files";

//localErrorStrings[INTERNAL_CORRUPTVINFO] =
//"AAF_INTERNAL_ERR: Corrupt Video Info Structure";

//localErrorStrings[BADSAMPLEOFFSET] =
//"AAF_ERR: Sample Index offset out of range";

//localErrorStrings[ONESAMPLEREAD] =
//"AAF_ERR: Only one sample allowed for reading compressed data";

//localErrorStrings[ONESAMPLEWRITE] =
//"AAF_ERR: Only one sample allowed for writing compressed data";

//localErrorStrings[DECOMPRESS] =
//"AAF_ERR: Software Decompression Failed";

//localErrorStrings[NODATA] =
//"AAF_ERR: No data read";

//localErrorStrings[INTERN_TOO_SMALL] =
//"AAF_ERR: Buffer is not large enough to hold data";

//localErrorStrings[INTERNAL_UNKNOWN_LOC] =
//"AAF_ERR: Unknown locator type";

//localErrorStrings[TRANSLATE] =
//"AAF_ERR: Can't translate to expected memory format";

//localErrorStrings[EOF] =
//"AAF_ERR: End of file";

//localErrorStrings[BADCOMPR] =
//"AAF_ERR: Unrecognized compression type in AAF file";

//localErrorStrings[BADPIXFORM] =
//"AAF_ERR: Unrecognized pixel format in AAF file";

//localErrorStrings[BADLAYOUT] =
//"AAF_ERR: Unrecognized frame layout in AAF file";

//localErrorStrings[COMPRLINESWR] =
//"AAF_ERR: omfWriteLines not allowed with compression";

//localErrorStrings[COMPRLINESRD] =
//"AAF_ERR: omfReadLines not allowed with compression";

//localErrorStrings[BADMEDIATYPE] =
//"AAF_ERR: Unrecognized Media Type";

//localErrorStrings[BADDATAADDRESS] =
//"AAF_ERR: Null data address for transfer operation";

//localErrorStrings[BAD_MDHDL] =
//"AAF_ERR: Bad Media Handle";

//localErrorStrings[MEDIA_NOT_FOUND] =
//"AAF_ERR: Cannot locate media data";

//localErrorStrings[ILLEGAL_MEMFMT] =
//"AAF_ERR: Illegal memory format code";

//localErrorStrings[ILLEGAL_FILEFMT] =
//"AAF_ERR: Illegal file format code";

//localErrorStrings[SWABBUFSIZE] =
//"AAF_ERR: Invalid swab buffer size";

//localErrorStrings[MISSING_SWAP_PROC] =
//"AAF_ERR: Missing stream swap proc";

//localErrorStrings[NULL_STREAMPROC] = 
//"AAF_ERR: A Stream Callback is NULL";

//localErrorStrings[NULLBUF] =
//"AAF_ERR: Null transfer buffer";

//localErrorStrings[SWAP_SETUP] =
//"AAF_ERR: Need to set up file & memory format before calling this function";

//localErrorStrings[INVALID_FILE_MOB] =
//"AAF_ERR: Invalid file mob reference";

//localErrorStrings[SINGLE_CHANNEL_OP] =
//"AAF_ERR: Operation not valid on multi-channel media streams";

//localErrorStrings[INVALID_CACHE_SIZE] = 
//"AAF_ERR: Stream cache size must be positive or zero";

//localErrorStrings[NOT_FILEMOB] = 
//"AAF_ERR: Operation requires a file mob";

//localErrorStrings[TRANSLATE_SAMPLE_SIZE] = 
//"AAF_ERR: Codec can't translate to/from that sample size";

//localErrorStrings[TRANSLATE_NON_INTEGRAL_RATE] =
//"AAF_ERR: Codec can't translate to/from rates which are not multiples";

//localErrorStrings[MISSING_MEDIA_REP] =
//"AAF_ERR: Media representation not present in the mob";

//localErrorStrings[NOT_LONGWORD] = 
//"AAF_ERR: Buffer must be longword aligned for this translation";

//localErrorStrings[XFER_DUPCH] = 
//"AAF_ERR: Cannot specify the same channel twice in multi-xfers";

//localErrorStrings[MEDIA_NOT_INIT] = 
//"AAF_ERR: omfmInit() must be called before making this call";

//localErrorStrings[BLOCKING_SIZE] = 
//"AAF_ERR: Blocking size must be >= 0 bytes";

//localErrorStrings[WRONG_MEDIATYPE] = 
//"AAF_ERR: Incorrect media type for this operation";

//localErrorStrings[MULTI_WRITELEN] = 
//"AAF_ERR: Writelengths in a writeMultiple must be uniform";

//localErrorStrings[STREAM_REOPEN] = 
//"AAF_ERROR: Stream reopened without being closed";

//localErrorStrings[TOO_MANY_FMT_OPS] = 
//"AAF_ERROR: Too many format specifiers for this codec";

//localErrorStrings[MEDIASTREAM_NOTALLOWED] =
//"AAF_ERR: An object with a media stream datakind is not allowed";

//localErrorStrings[STILLFRAME_BADLENGTH] =
//"AAF_ERR: Length of a still frame in a media group must be 1";

//localErrorStrings[DATA_NONCONTIG] = 
//"AAF_ERR: Calling this function now will produce non-contiguous media data";
   
//localErrorStrings[OPLIST_OVERFLOW] =
//   "AAF_ERR: Operation list overflow";
   
//localErrorStrings[STREAM_CLOSED] = 
//"AAF_ERR: Stream must be open for this operation";

//localErrorStrings[USE_MULTI_CREATE] = 
//"AAF_ERR: Use multiCreate to create this many interleaved channels";

//localErrorStrings[MEDIA_OPENMODE] =
//"AAF_ERR: Media object opened in wrong mode (readonly vs. readwrite)";

//localErrorStrings[MEDIA_CANNOT_CLOSE] =
//"AAF_ERR: No proc to close media";

//localErrorStrings[XFER_NOT_BYTES] =
//"AAF_ERR: Sample transfers must be an integral number of bytes";

//localErrorStrings[ZERO_SAMPLESIZE] =
//"AAF_ERR: Sample size of zero not allowed";

//localErrorStrings[ZERO_PIXELSIZE] =
//"AAF_ERR: Pixel size of zero not allowed";

/* Codec Error Codes */
//localErrorStrings[CODEC_INVALID] =
//"CODEC invalid or not loaded";

//localErrorStrings[INVALID_OP_CODEC] =
//"AAF_ERR: Operation not valid on this codec";

//localErrorStrings[BAD_CODEC_REV] =
//"AAF_ERR: Out of date codec";

//localErrorStrings[CODEC_CHANNELS] =
//"AAF_ERR: Channel out of range for codec";

//localErrorStrings[BAD_VARIETY] = 
//"AAF_ERR: Badly formed codec variety string";

//localErrorStrings[CODEC_NAME_SIZE] = 
//"AAF_ERR: Codec name string too large";

/* Image Error Codes */
//localErrorStrings[TIFFVERSION] =
//"AAF_ERR: Error reading tiff version";

//localErrorStrings[BADTIFFCOUNT] =
//"AAF_ERR: Video: TIFF count less than 1";

//localErrorStrings[24BITVIDEO] =
//"AAF_ERR: 24 Bit (8-8-8) video only";

//localErrorStrings[JPEGBASELINE] =
//"AAF_ERR: Only baseline JPEG allowed in AAF files";

//localErrorStrings[BADJPEGINFO] =
//"AAF_ERR: JPEG TIFF fields not allowed without COMPRESSION == JPEG";

//localErrorStrings[BADQTABLE] =
//"AAF_ERR: Bad JPEG Quantization Table";

//localErrorStrings[BADACTABLE] =
//"AAF_ERR: Bad JPEG AC Huffman Table";

//localErrorStrings[BADDCTABLE] =
//"AAF_ERR: Bad JPEG DC Huffman Table";

//localErrorStrings[NOFRAMEINDEX] =
//"AAF_ERR: No JPEG Video Frame Index";

//localErrorStrings[BADFRAMEOFFSET] =
//"AAF_ERR: Frame Index offset out of range";

//localErrorStrings[JPEGPCM] =
//"AAF_ERR: OMJPEG.c: put_color_map called";

//localErrorStrings[JPEGDISABLED] =
//"AAF_ERR: JPEG codec disabled";

//localErrorStrings[JPEGPROBLEM] =
//"AAF_ERR: Unspecified JPEG codec problem";

//localErrorStrings[BADEXPORTPIXFORM] =
//"AAF_ERR: Unrecognized Pixel Format";

//localErrorStrings[BADEXPORTLAYOUT] =
//"AAF_ERR: Unrecognized Frame Layout";

//localErrorStrings[BADRWLINES] =
//"AAF_ERR: Read/Write Lines only enabled for common video format";

/* Audio Error Codes */
//localErrorStrings[BADAIFCDATA] =
//"AAF_ERR: Error reading AIFC data";

//localErrorStrings[BADWAVEDATA] =
//"AAF_ERR: Error reading WAVE data";

//localErrorStrings[NOAUDIOCONV] =
//"AAF_ERR: Audio Read Samples: Can't convert to requested sample size";


/*** OBJECT Error Codes ***/

//localErrorStrings[INVALID_LINKAGE] =
//"AAF_ERR: Invalid object attached to property";

//localErrorStrings[SWAB] =
//"AAF_ERR: Cannot swab that data size";

//localErrorStrings[END_OF_DATA] =
//"AAF_ERR: Read past end of data";

//localErrorStrings[DATAKIND_EXIST] =
//"AAF_ERR: A Datakind Definition with this ID already exists";

//localErrorStrings[TOO_MANY_TYPES] =
//"AAF_ERR: Too many types for a single property";

/*** MOB Error Codes ***/
/* General Segment Error Codes */
//localErrorStrings[NOT_SOURCE_CLIP] =
//"AAF_ERR: This property must be a Source Clip";

//localErrorStrings[FILL_FOUND] =
//"AAF_ERR: An unexpected fill property was found";

//localErrorStrings[BAD_LENGTH] = 
//"AAF_ERR: Segment has an illegal length";

//localErrorStrings[BADRATE] = 
//"AAF_ERR: Illegal value for edit rate";

//localErrorStrings[INVALID_ROUNDING] = 
//"AAF_ERR: Editrate rounding must be either Floor or Ceiling";

//localErrorStrings[PULLDOWN_DIRECTION] = 
//"AAF_ERR: Illegal pulldown direction";

//localErrorStrings[PULLDOWN_FUNC] = 
//"AAF_ERR: use AddPulldownRef() instead of AddPhysSourceRef() for pulldown";

//localErrorStrings[NOT_COMPOSITION] =
//"AAF_ERR: This property must be a Composition";

/* Timecode Error Codes */
//localErrorStrings[TIMECODE_NOT_FOUND] =
//"AAF_ERR: Timecode was not found in the mob chain";

//localErrorStrings[NO_TIMECODE] = 
//"AAF_ERR: Cannot find timecode on given track";

//localErrorStrings[INVALID_TIMECODE] = 
//"AAF_ERR: Timecode value is invalid";

/* Track Error Codes */
//localErrorStrings[TRACK_NOT_FOUND] =
//"AAF_ERR: Track not found";

//localErrorStrings[BAD_SLOTLENGTH] =
//"AAF_ERR: Bad Slot Length";

//localErrorStrings[MISSING_TRACKID] =
//"AAF_ERR: TrackID not present in the mob";

//localErrorStrings[TRACK_EXISTS] =
//"AAF_ERR: A Track with this trackID already exists";

//localErrorStrings[NOT_A_TRACK] =
//"AAF_ERR: This function requires a track, not a mob slot.";

/* MOBJ Error Codes */
//localErrorStrings[MOB_NOT_FOUND] =
//"AAF_ERR: Mob does not exist in this file";

//localErrorStrings[NO_MORE_MOBS] =
//"AAF_ERR: The end of the mob chain has been reached";

//localErrorStrings[DUPLICATE_MOBID] =
//"AAF_ERR: MobID already exists in the file";

//localErrorStrings[MISSING_MOBID] =
//"AAF_ERR: MobID not present in the file";

/* Effect Error Codes */
//localErrorStrings[EFFECTDEF_EXIST] =
//"AAF_ERR: An Effect Definition with this Effect ID already exists";

//localErrorStrings[INVALID_EFFECTDEF] = 
//"AAF_ERR: Effect Definition invalid or nonexistant";

//localErrorStrings[INVALID_EFFECT] = 
//"AAF_ERR: Effect is invalid or non-existent";

//localErrorStrings[INVALID_EFFECTARG] =
//"AAF_ERR: The given effect argument is not valid for this effect";

//localErrorStrings[INVALID_CVAL] = 
//"AAF_ERR: Constant value is invalid or nonexistent";

//localErrorStrings[RENDER_NOT_FOUND] = 
//"AAF_ERR: Effect Rendering does not exist";

/* Iterator Error Codes */
//localErrorStrings[BAD_ITHDL] =
//"AAF_ERR: Bad Iterator handle";

//localErrorStrings[ITER_WRONG_TYPE] =
//"AAF_ERR: Wrong iterator type for this function";

//localErrorStrings[INVALID_SEARCH_CRIT] =
//"AAF_ERR: Invalid search criteria for this kind of iterator";

//localErrorStrings[INTERNAL_ITERATOR] =
//"AAF_INTERNAL_ERR: Internal error with iterator";

//localErrorStrings[BAD_SRCH_ITER] =
//"AAF_ERR: This iterator handle must be allocated by omfiMobOpenSearch()";

/* Traversal Error Codes */
//localErrorStrings[NULL_MATCHFUNC] = 
//"AAF_ERR: Match function to traversal routine is null";

//localErrorStrings[NULL_CALLBACKFUNC] = 
//"AAF_ERR: Callback function to traversal routine is null";

//localErrorStrings[PARSE_EFFECT_AMBIGUOUS] =
//"AAF_ERR: Need more information to parse further through an effect";

/* Transition Error Codes */
//localErrorStrings[INVALID_TRAN_EFFECT] =
//"AAF_ERR: Effect is not a transition effect";

//localErrorStrings[ADJACENT_TRAN] =
//"AAF_ERR: Adjacent transitions in a sequence are illegal";

//localErrorStrings[LEADING_TRAN] =
//"AAF_ERR: Beginning a sequence with a transition is illegal";

//localErrorStrings[INSUFF_TRAN_MATERIAL] =
//"AAF_ERR: Not enough material as input to transition";


/*** SIMPLE COMPOSITION Error Codes ***/
//localErrorStrings[BAD_STRACKHDL] =
//"AAF_ERR: Bad Simple Track handle";

//localErrorStrings[STRACK_APPEND_ILLEGAL] =
//"AAF_ERR: This track does not contain a sequence, appending is illegal";


/*** GENERIC Error Codes ***/
//localErrorStrings[OFFSET_SIZE] =
//"AAF_ERR: 64-bit truncation error";

//localErrorStrings[INTERNAL_NEG64] =
//"AAF_ERR: Negative 64-bit number";

//localErrorStrings[OVERFLOW64] = 
//"AAF_ERR: Overflow on a 64-bit operation";

//localErrorStrings[NOT_IN_15] =
//"AAF_ERR: Function not available in 1.x compatability mode";

//localErrorStrings[NOT_IN_20] =
//"AAF_ERR: Function not available in 2.x native mode";

//localErrorStrings[ZERO_DIVIDE] = 
//"AAF_ERR: Divide by zero";

/*** SEMANTIC CHECKING Error Codes ***/
//localErrorStrings[REQUIRED_POSITIVE] =
//"AAF_ERR: Value should be positive or zero";

//localErrorStrings[INVALID_TRACKKIND] =
//"AAF_ERR: Invalid Track Kind";

//localErrorStrings[INVALID_EDGETYPE] =
//"AAF_ERR: Invalid Edge Code Format Kind";

//localErrorStrings[INVALID_FILMTYPE] =
//"AAF_ERR: Invalid Film Type";

//localErrorStrings[INVALID_TAPECASETYPE] =
//"AAF_ERR: Invalid Tape Case Type";

//localErrorStrings[INVALID_VIDEOSIGNALTYPE] =
//"AAF_ERR: Invalid Video Signal Type";

//localErrorStrings[INVALID_TAPEFORMATTYPE] =
//"AAF_ERR: Invalid Tape Format Type";

//localErrorStrings[INVALID_EDITHINT] =
//"AAF_ERR: Invalid Effect Edit Hint";

//localErrorStrings[INVALID_INTERPKIND] =
//"AAF_ERR: Invalid Effect Interpolation Kind";

//localErrorStrings[INVALID_MOBTYPE] =
//"AAF_ERR: Invalid Mob Type";

//localErrorStrings[INVALID_TRACK_REF] =
//"AAF_ERR: Positive Relative Track Reference not allowed";

//localErrorStrings[BAD_VIRTUAL_CREATE] =
//"AAF_ERR: Creation of virtual objects not allowed";

//localErrorStrings[INVALID_CLASS_ID] =
//"AAF_ERR: Invalid Object Class ID";

//localErrorStrings[OBJECT_SEMANTIC] =
//"AAF_ERR: Failed a semantic check on an input obj";

//localErrorStrings[DATA_IN_SEMANTIC] =
//"AAF_ERR: Failed a semantic check on an input data";

//localErrorStrings[DATA_OUT_SEMANTIC] =
//"AAF_ERR: Failed a semantic check on an output data";

//localErrorStrings[TYPE_SEMANTIC] = 
//"AAF_ERR: Property and type do not match";

//localErrorStrings[INVALID_ATTRIBUTEKIND] =
//"AAF_ERR: Invalid Attribute Kind";

//localErrorStrings[DATA_MDES_DISAGREE] =
//"AAF_ERR: Media descriptor summary data does not agree with media";

//localErrorStrings[CODEC_SEMANTIC_WARN] =
//"AAF_ERR: A semantic check warning occured when checking media";

//localErrorStrings[INVALID_BOOLTYPE] =
//"AAF_ERR: Invalid Boolean Value";

/*** INTERNAL Error Codes ***/
//localErrorStrings[TABLE_DUP_KEY] =
//"AAF_INTERNAL_ERR: Duplicate Key detected in internal table";

//localErrorStrings[TABLE_MISSING_COMPARE] =
//"AAF_INTERNAL_ERR: Missing compare function on table";

//localErrorStrings[TABLE_BAD_ITER] =
//"AAF_INTERNAL_ERR: Bad table iterator handle";

//localErrorStrings[PROPID_MATCH] =
//"AAF_INTERNAL_ERR: Property ID code doesn't match between revs";

//localErrorStrings[INTERNAL_DIVIDE] = 
//"AAF_ERR: Internal division error";

/*** Testing Error Codes ***/
//localErrorStrings[TEST_FAILED] =
//"AAF_TESTING_ERR: Test Failed";
