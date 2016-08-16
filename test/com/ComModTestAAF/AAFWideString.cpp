//=---------------------------------------------------------------------=
//
// $Id: AAFWideString.cpp,v 1.3 2009/06/01 11:47:11 stuart_hc Exp $ $Name: V116 $
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

#include "AAFWideString.h"

#if defined(OS_DARWIN) && defined(COMPILER_MWERKS)

#pragma warn_possunwant off

int wcslen(const wchar_t*aString)
{
	int	len = -1;

	wchar_t * p = (wchar_t *) aString - 1;
	
	do
		len++;
	while (*++p);
	
	return len;
}

int wcscmp(const wchar_t*string1, const wchar_t*strign2)
{
	const	wchar_t 		* p1 = (wchar_t *) string1 - 1;
	const	wchar_t 		* p2 = (wchar_t *) strign2 - 1;
	unsigned long		c1, c2;
		
	while ((c1 = *++p1) == (c2 = *++p2))
		if (!c1)
			return(0);

	return(c1 - c2);
}

wchar_t* wcscpy(wchar_t*stringDest, const wchar_t*stringSrc)
{
	const	wchar_t * p = (wchar_t *) stringSrc - 1;
			wchar_t * q = (wchar_t *) stringDest - 1;
	
	while (*++q = *++p);
	
	return(stringDest);
}

int wcsncmp(const wchar_t*stringDest, const wchar_t*stringSrc, const int length)
{
	const	wchar_t 	* p1 = (wchar_t *) stringDest - 1;
	const	wchar_t 	* p2 = (wchar_t *) stringSrc - 1;
	unsigned long	c1, c2;
	int	n;
	
	n = length;
	n++;
	
	while (--n)
		if ((c1 = *++p1) != (c2 = *++p2))
			return(c1 - c2);
		else if (!c1)
			break;
	
	return(0);
}

wchar_t* wcscat(wchar_t*stringDest, const wchar_t*stringSrc)
{
	const	wchar_t * p = (wchar_t *) stringSrc - 1;
				wchar_t * q = (wchar_t *) stringDest - 1;
	
	while (*++q);
	
	q--;
	
	while (*++q = *++p);
	
	return(stringDest);
}


#endif  // if defined(OS_MACOS10)
