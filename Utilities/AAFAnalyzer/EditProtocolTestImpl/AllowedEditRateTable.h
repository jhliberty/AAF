//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

//AAF files
#include <AAFPlatform.h>
#include <AAFTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <vector>

#ifndef __AllowedEditRateTable_h_
#define __AllowedEditRateTable_h_

namespace aafanalyzer {

using namespace std;

class Section71TableEntry
{
 public:

  enum TrackType_e {AV, A};

 private:

  TrackType_e _type;
  aafRational_t _exactRate;
  aafRational_t _roundedRate;

 public:

  Section71TableEntry(TrackType_e t, int exactNum, int exactDen, int roundedNum, int roundedDen);

  TrackType_e GetTrackType() const;
  const aafRational_t& GetExactRate() const;
  const aafRational_t& GetRoundedRate() const;

  bool IsEquivalent(const aafRational_t& rate, bool isVideo) const;
};

class AllowedEditRateTable
{
  std::vector<Section71TableEntry> _rates;

 public:

    AllowedEditRateTable();
    virtual ~AllowedEditRateTable();
    
    bool IsInTable( const aafRational_t& editRate, bool isVideo ) const;
    double Round( double num ) const;

 private:
 
    void InitializeTable();

    // prohibited
    AllowedEditRateTable( const AllowedEditRateTable& );
    AllowedEditRateTable& operator=( const AllowedEditRateTable& );

};
    
} // end of namespace aafanalyzer

#endif /*__AllowedEditRateTable_h_*/
