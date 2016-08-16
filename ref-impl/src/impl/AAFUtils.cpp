//=---------------------------------------------------------------------=
//
// $Id: AAFUtils.cpp,v 1.67 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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


/*
 * All functions can return the following error codes if the following
 * argument values are NULL:
 *		AAFRESULT_NULL_FHDL -- aafHdl_t was NULL.
 *		AAFRESULT_NULLOBJECT -- aafObject_t was NULL.
 *		AAFRESULT_BADDATAADDRESS -- Data address was NULL.
 *		AAFRESULT_NULL_SESSION -- No session was open.
 *		AAFRESULT_SWAB -- Unable to byte swap the given data type.
 * 
 * Accessor functions can also return the error codes below:
 *		AAFRESULT_BAD_PROP -- aafProperty_t code was out of range.
 *		AAFRESULT_OBJECT_SEMANTIC -- Failed a semantic check on an input obj
 *		AAFRESULT_DATA_IN_SEMANTIC -- Failed a semantic check on an input data
 *		AAFRESULT_DATA_OUT_SEMANTIC -- Failed a semantic check on an output data
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <wchar.h>

#include "AAFTypes.h"

#if defined (OS_UNIX)
  #include <sys/time.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/times.h>
#endif

#include "AAFUtils.h"
#include "AAFResult.h"
#include "utf8.h"

#include "OMUtilities.h"


aafBool	EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? kAAFTrue : kAAFFalse);
}

aafBool	EqualMobID(aafMobID_constref mobID1, aafMobID_constref mobID2)
{
	return(memcmp(&mobID1, &mobID2, sizeof(aafMobID_t)) == 0 ? kAAFTrue : kAAFFalse);
}


/************************
 *   Function:	aafIsEqualGUID
 *
 *	Determines whether specified GUIDs are equal.
 *
 *   ReturnValue:
 *	kAAFTrue - The GUIDs are equal.
 *	kAAFFalse - The GUIDs are not equal.
 */
aafBool aafIsEqualGUID( const GUID& guid1, const GUID& guid2 )
{
    return( memcmp( (void*)&guid1, (void*)&guid2, sizeof(GUID) ) == 0 ? 
        kAAFTrue : kAAFFalse );
}


/************************
 * Function: AAFGetDateTime			(INTERNAL)
 *
 * 	Returns the number of seconds since the standard root date
 *		for the current machine.  The date returned here will be 
 *		converted the canonical date format in the date write routine.
 *
 * Argument Notes:
 *		Time - is NATIVE format.  That is relative to 1/1/1904 for the
 *			Mac and 1/1/70? for everyone else.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void AAFGetDateTime(aafTimeStamp_t *ts)
{
	assert (ts);

	const time_t t = time(0);
	const struct tm * ansitime = gmtime (&t);
	assert (ansitime);

	ts->date.year   = ansitime->tm_year+1900;
	ts->date.month  = ansitime->tm_mon+1;  // AAF months are 1-based
	ts->date.day    = ansitime->tm_mday;   // tm_mday already 1-based
	ts->time.hour   = ansitime->tm_hour;
	ts->time.minute = ansitime->tm_min;
	ts->time.second = ansitime->tm_sec;
	ts->time.fraction = 0;            // not implemented yet!
}



aafErr_t AAFConvertEditRate(
	aafRational_t srcRate,        /* IN - Source Edit Rate */
	aafPosition_t srcPosition,    /* IN - Source Position */
	aafRational_t destRate,       /* IN - Destination Edit Rate */
	aafRounding_t howRound,	      /* IN - Rounding method (floor or ceiling) */
	aafPosition_t *destPosition)  /* OUT - Destination Position */
{
	aafInt64		intPos, destPos, remainder;
		
	XPROTECT()
	{
		*destPosition = 0;
		if ((howRound != kRoundCeiling) && (howRound != kRoundFloor))
		{
			RAISE(AAFRESULT_INVALID_ROUNDING);
		}

		if(FloatFromRational(srcRate) != FloatFromRational(destRate))
		{
			intPos = (srcRate.denominator * destRate.numerator) * srcPosition;
			destPos = intPos / (srcRate.numerator * destRate.denominator);		// truncate to int
			remainder = intPos % (srcRate.numerator * destRate.denominator);
		}
		else
		{
	  		/* JeffB: (1 / 29.97) * 29.97 often doesn't == 1
	  		 * (it seems to be .99999... on the 68K FPU)
	  		 * The debugger says it's 1.0, but if(foo >= 1.0) fails.
	  		 */
	  		destPos = srcPosition;
	  		remainder = 0;
		}

		/* Usually used for lower edit rate to higher edit rate conversion - 
		 * to return the first sample of the higher edit rate that contains a
		 * sample of the lower edit rate. (i.e., video -> audio)
		 */
		if (howRound == kRoundFloor)
		{
			*destPosition = destPos;
		}
		/* Usually used for higher edit rate to lower edit rate conversion -
		 * to return the sample of the lower edit rate that fully contains
		 * the sample of the higher edit rate. (i.e., audio -> video)
		 */
		else if (howRound == kRoundCeiling)
		{
			*destPosition = destPos;
			if(remainder != 0)
				*destPosition++;
		}
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

/************************
 * Function: FloatFromRational (INTERNAL)
 *
 * 	Convert a rational number into a floating point double
 *
 * Argument Notes:
 *		See argument comments and assertions.
 *
 * ReturnValue:
 *		The result, or zero if the denominator was zero.
 *
 * Possible Errors:
 *		Returns zero if the denominator was zero.
 */
double FloatFromRational(
			aafRational_t	e)		/* IN - Convert this into a double */
{
	double          num = (double) e.numerator;
	double          den = (double) e.denominator;

	if(den == 0)
		return(0);
	else
		return (num / den);
}





/*************************************************************************
 * Function: aafMobIDNew()
 *
 *      This function can be used to create a new mob ID.  The mob ID
 *      consists of the company specific prefix specified when 
 *      omfsBeginSession() is called.  The major number is the time of day,
 *      and the minor number is the accumulated cpu cycles of the
 *      application.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/

struct SMPTELabel
{
	aafUInt32	MobIDMajor;
	aafUInt16	MobIDMinorLow;
	aafUInt16	MobIDMinorHigh;
	aafUInt8	oid;
	aafUInt8	size;
	aafUInt8	ulcode;
	aafUInt8	SMPTE;
	aafUInt8	Registry;
	aafUInt8	unused;
	aafUInt8	MobIDPrefixLow;
	aafUInt8	MobIDPrefixHigh;
};


struct OMFMobID
{
    	aafUInt8		SMPTELabel[12];	// 12-bytes of label prefix
	aafUInt8		length;
	aafUInt8		instanceHigh;
	aafUInt8		instanceMid;
	aafUInt8		instanceLow;
	struct SMPTELabel	material;
};


union MobIDOverlay
{
	aafMobID_t		mobID;
	struct OMFMobID		OMFMobID;
};


aafUInt32 aafGetTickCount()
{
    aafUInt32		ticks = 0;


#if defined( OS_WINDOWS )

    ticks = (aafUInt32)GetTickCount();

#elif defined( OS_UNIX )

    struct tms		tms_buf;
    ticks = (aafUInt32)times( &tms_buf );

#else

#error Unknown operating system

#endif    // OS_*


    return ticks;
}


//
// aafMobIDNew()
//
// If defined, AAF_CREATE_SMPTE_UMIDS forces aafMobIDNew() to generate
// valid SMPTE Universal Material Identifiers (UMIDs) as mob IDs.
// If AAF_CREATE_SMPTE_UMIDS is not defined aafMobIDNew() generates
// OMF-compatible mob IDs.
//
// The algorithm used to generate UMIDs has not been extensively tested
// yet. Enabling it is likely to break workflows that rely on OMF
// compatibility of mob IDs generated by AAF SDK.
//
//#define AAF_CREATE_SMPTE_UMIDS

AAFRESULT aafMobIDNew(
        aafMobID_t *mobID)     /* OUT - Newly created Mob ID */
{
#if defined(AAF_CREATE_SMPTE_UMIDS)

    OMMaterialIdentification id =
    {{0x06, 0x0a, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05, 0x01, 0x01, 0x0f, 0x00},
     0x13, 0x00, 0x00, 0x00};
    id.material = createUniqueIdentifier();
    memcpy( mobID, reinterpret_cast<aafMobID_t*>(&id), sizeof(id));

    return AAFRESULT_SUCCESS;

#else
    aafUInt32		major, minor;
    static aafUInt32	last_part2 = 0;		// Get rid of this!!!

    //
    // Get the time in seconds since the standard root date
    //
    time_t now = time(0);
    major = static_cast<aafUInt32>(now);


    //
    // Get the time since the system start-up.
    //
    minor = aafGetTickCount();
    assert( minor != 0 && minor != (aafUInt32)-1 );


    if (last_part2 >= minor)
	minor = last_part2 + 1;
    	
    last_part2 = minor;


    return(aafMobIDFromMajorMinor( 42, major, minor, 0x0F, mobID ));
#endif
}



AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	prefix,
        aafUInt32	major,
	aafUInt32	minor,
	aafUInt8	UMIDType,
	aafMobID_t	*mobID)     /* OUT - Newly created Mob ID */
{
    union MobIDOverlay			aLabel;


    aLabel.OMFMobID.SMPTELabel[0]	= 0x06;
    aLabel.OMFMobID.SMPTELabel[1]	= 0x0A;
    aLabel.OMFMobID.SMPTELabel[2]	= 0x2B;
    aLabel.OMFMobID.SMPTELabel[3]	= 0x34;
    aLabel.OMFMobID.SMPTELabel[4]	= 0x01;			
    aLabel.OMFMobID.SMPTELabel[5]	= 0x01;		
    aLabel.OMFMobID.SMPTELabel[6]	= 0x01;	
    aLabel.OMFMobID.SMPTELabel[7]	= 0x01;
    aLabel.OMFMobID.SMPTELabel[8]	= 0x01;
    aLabel.OMFMobID.SMPTELabel[9]	= 0x01;
    aLabel.OMFMobID.SMPTELabel[10]	= UMIDType;	
    aLabel.OMFMobID.SMPTELabel[11]	= 0x00;
    aLabel.OMFMobID.length		= 0x13;
    aLabel.OMFMobID.instanceHigh	= 0x00;
    aLabel.OMFMobID.instanceMid		= 0x00;
    aLabel.OMFMobID.instanceLow		= 0x00;

    aLabel.OMFMobID.material.oid		= 0x06;
    aLabel.OMFMobID.material.size		= 0x0E;
    aLabel.OMFMobID.material.ulcode		= 0x2B;
    aLabel.OMFMobID.material.SMPTE		= 0x34;
    aLabel.OMFMobID.material.Registry		= 0x7F;
    aLabel.OMFMobID.material.unused		= 0x7F;
    aLabel.OMFMobID.material.MobIDPrefixHigh	= 
	(aafUInt8)((prefix >> 7L) | 0x80);
    aLabel.OMFMobID.material.MobIDPrefixLow	= (aafUInt8)(prefix & 0x7F);
    aLabel.OMFMobID.material.MobIDMajor		= major;
    aLabel.OMFMobID.material.MobIDMinorLow	= (aafUInt16)(minor & 0xFFFF);
    aLabel.OMFMobID.material.MobIDMinorHigh	= 
	(aafUInt16)((minor >> 16L) & 0xFFFF);

    *mobID = (aafMobID_t)aLabel.mobID;


    return(AAFRESULT_SUCCESS);
}


void aafCreateGUID( GUID *p_guid )
{
#if defined( OS_WINDOWS )

    assert( p_guid );
    CoCreateGuid( p_guid );

#else

    // {1994bd00-69de-11d2-b6bc-fcab70ff7331}
    static GUID	sTemplate = { 0x1994bd00,  0x69de,  0x11d2,
			{ 0xb6, 0xbc, 0xfc, 0xab, 0x70, 0xff, 0x73, 0x31 } };
    static int	sInitializedTemplate = 0;


    assert( p_guid );

    if( !sInitializedTemplate )
    {
	aafUInt32	ticks = aafGetTickCount();

	time_t		timer = time( NULL );
	sTemplate.Data1 += timer + ticks;
	sInitializedTemplate = 1;
    }

    // Just bump the first member of the guid to emulate GUIDGEN behavior.
    ++sTemplate.Data1;
    *p_guid = sTemplate;

#endif    // OS_*
}



// Initializes a new auid
AAFRESULT aafAUIDNew( aafUID_t* p_auid )
{
    if( !p_auid )
	return AAFRESULT_NULL_PARAM;

    aafCreateGUID( (GUID*)p_auid );
    return AAFRESULT_SUCCESS;
}



typedef struct
	{
	aafUInt32	fpMinute;
	aafUInt32   fpHour;
	aafUInt32	dropFpMin;
	aafUInt32	dropFpMin10;
	aafUInt32	dropFpHour;
} frameTbl_t;

/*************************************************************************
 * Private Function: GetFrameInfo()
 *
 *      This function is used by omfsTimecodeToString().  It pulls
 *      apart a frame rate into elements in different units, and
 *      returns a structure containing these units.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
static frameTbl_t GetFrameInfo(
							   aafInt32 fps) /* IN - Frame Rate */
{
	frameTbl_t	result;
	
	result.dropFpMin = (60 * fps) - 2;
	result.dropFpMin10 = (10*result.dropFpMin+2);
	result.dropFpHour = 6 * result.dropFpMin10;

	result.fpMinute = 60 * fps;
	result.fpHour = 60 * result.fpMinute;

	return(result);
}

/*************************************************************************
 * Private Function: PvtOffsetToTimecode()
 *
 *      Given an offset into a track and a frame rate, this function
 *      calculates a timecode value.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
aafErr_t PvtOffsetToTimecode(
	aafFrameOffset_t offset, /* IN - Offset into a track */
	aafInt16 frameRate,      /* IN - Frame rate */
	aafDropType_t drop,     /* OUT - Drop or non-drop Timecode */
	aafInt16 *hours,         /* OUT - Hours value of timecode */
	aafInt16 *minutes,       /* OUT - Minutes value of timecode */
	aafInt16 *seconds,       /* OUT - Seconds value of timecode */
	aafInt16 *frames)        /* OUT - Frames value of timecode */
{
  frameTbl_t info;
  aafUInt32		frames_day;
  aafFrameOffset_t min10, min1;
  aafBool frame_dropped;

  info = GetFrameInfo(frameRate);
  frames_day = (drop ? info.dropFpHour: info.fpHour) *24;

  if (offset < 0L)
	 offset += frames_day;
  if (offset >= frames_day)
	 offset -= frames_day;
  if (drop)
	 {
		*hours = (aafInt16)(offset / info.dropFpHour);
		offset = offset % info.dropFpHour;
		min10 = offset / info.dropFpMin10;
		offset = offset % info.dropFpMin10;
		if (offset < info.fpMinute)
		  {
			 frame_dropped = kAAFFalse;
			 min1 = 0;
		  }
		else
		  {
			 frame_dropped = kAAFTrue;
			 offset -= info.fpMinute;
			 min1 = (offset / info.dropFpMin) + 1;
			 offset = offset % info.dropFpMin;
		  }
		
		*minutes = (aafInt16)((min10 * 10) + min1);
		*seconds = (aafInt16)(offset / frameRate);
		*frames = (aafInt16)(offset % frameRate);
		if (frame_dropped)
		  {
			 (*frames) +=2;
			 if (*frames >= frameRate)
				{
				  (*frames) -= frameRate;
				  (*seconds)++;
				  if (*seconds > 60)
					 {
						(*seconds) -= 60;
						(*minutes)++;
						if (*minutes > 60)
						  {
							 (*minutes) -= 60;
							 (*hours)++;
						  }
					 }
				}
		  }
	 }
  else
	 {
		*hours = (aafInt16)(offset / info.fpHour);
		offset = offset % info.fpHour;
		*minutes = (aafInt16)(offset / info.fpMinute);
		offset = offset % info.fpMinute;
		*seconds = (aafInt16)(offset / frameRate);
		*frames = (aafInt16)(offset % frameRate);
	 }

  return(AAFRESULT_SUCCESS);
}


/*************************************************************************
 * Function: PvtTimecodeToOffset()
 *
 *      Given a timecode and a frame rate, this function returns a
 *      position relative to the beginning of a track.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
aafErr_t PvtTimecodeToOffset(
	aafInt16 frameRate,  /* IN - Frame Rate */
	aafInt16 hours,      /* IN - Hours value of Timecode */
	aafInt16 minutes,    /* IN - Minutes value of Timecode */
	aafInt16 seconds,    /* IN - Seconds value of Timecode */
	aafInt16 frames,     /* IN - Frames value of Timecode */
	aafDropType_t drop,  /* IN - Drop of non-drop Timecode */
	aafFrameOffset_t	*result) /* OUT - resulting position */

{
   aafUInt32		val;
	frameTbl_t	info;
	
	info = GetFrameInfo(frameRate);
	if(drop)
		{
		val = (hours * info.dropFpHour);
		val += ((minutes / 10) * info.dropFpMin10);
		val += (minutes % 10) * info.dropFpMin;
		}
	else
		{
		val = hours * info.fpHour;
		val += minutes * info.fpMinute;
		}

	val += seconds * frameRate;
	val += frames;
	
	*result = val;

	return(AAFRESULT_SUCCESS);
}

/*
 *	Routines to support the URL and URI utility functions.
 */

// These routines rely upon URLs as defined in RFC 1738:
//
// <scheme>:<scheme-specific-part>
// ; the scheme is in lower case; interpreters should use case-ignore
//	scheme		= 1*[ lowalpha | digit | "+" | "-" | "." ]
//
// For file scheme:
//	fileurl		= "file://" [ host | "localhost" ] "/" fpath
//
// NB. ':' is acceptable unescaped in the fpath component

#ifdef _MSC_VER			// MS VC++ dosen't provide POSIX strncasecmp
#define strncasecmp(s1, s2, n) _strnicmp(s1, s2, n)
#else
#include <strings.h>	// strncasecmp()
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

/************************
 * Function: wcsconvertURLtoFilepath
 *
 *	Converts a file scheme URL into an absolute filepath.  An invalid URL is
 *	returned unmodified.
 *
 * Argument Notes:
 *		filepath must have space allocated by the caller
 *
 * ReturnValue:
 *		void
 *
 * Possible Errors:
 *		none
 */
void wcsconvertURLtoFilepath(wchar_t *url, wchar_t *filepath)
{
	// Convert to char* for ease of processing.
	// (wcsncasecmp and similiar are not available everywhere)
	//unsigned tlen = wcslen(url);
	unsigned tlen =wcsu8slen(url);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstou8s(tmp, url, tlen+1);

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

	u8stowcs(filepath, fpath, strlen(fpath)+1);		// convert back to wcs
	delete [] tmp;
}

/************************
 * Function: wcsconvertFilepathtoURL
 *
 *	Converts an absolute filepath into a file scheme URL.
 *
 * Argument Notes:
 *		url must have space allocated by the caller
 *
 * ReturnValue:
 *		void
 *
 * Possible Errors:
 *		none
 */
void wcsconvertFilepathtoURL(wchar_t *filepath, wchar_t *url)
{
	// convert to char* for ease of processing
	int tlen = wcsu8slen(filepath);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstou8s(tmp, filepath, tlen+1);

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

	u8stowcs(url, mb_url, strlen(mb_url)+1);		// convert back to wcs
	delete [] mb_url;
	delete [] escaped;
	delete [] tmp;
}


/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
