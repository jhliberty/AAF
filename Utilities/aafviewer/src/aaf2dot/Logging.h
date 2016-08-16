#ifndef __LOGGING_H__
#define __LOGGING_H__


/*
 * $Id: Logging.h,v 1.2 2004/02/27 16:41:24 stuart_hc Exp $ $Name: V116 $
 *
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aaf2dot.
 *
 *  aaf2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  aaf2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with aaf2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <iostream>

// holds the streams for debug and error logging
class Logging
{
  public:

   static void SetDebugLogStream( std::ostream *debugLogStream ) {
      _debugLogStream = debugLogStream;
   }

   static void SetErrorLogStream( std::ostream *errorLogStream ) {
      _errorLogStream = errorLogStream;
   }

   static void SetNullDebugLogStream() {
      _debugLogStream = &_nullStream;
   }

   static void SetNullErrorLogStream() {
      _errorLogStream = &_nullStream;
   }

   static std::ostream& DebugLogStream() {
      return *_debugLogStream;
   }

   static std::ostream& ErrorLogStream() {
      return *_errorLogStream;
   }

  private:
   static std::ostream* _debugLogStream;
   static std::ostream* _errorLogStream;

   static std::ostream _nullStream;
};


#endif // __LOGGING_H__

