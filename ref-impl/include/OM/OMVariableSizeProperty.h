//=---------------------------------------------------------------------=
//
// $Id: OMVariableSizeProperty.h,v 1.11 2009/06/01 11:46:58 stuart_hc Exp $ $Name: V116 $
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

// @doc OMEXTERNAL
#ifndef OMVARIABLESIZEPROPERTY_H
#define OMVARIABLESIZEPROPERTY_H

#include "OMDataTypes.h"
#include "OMProperty.h"

  // @class Variable size simple (data) persistent
  //        properties supported by the Object Manager.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @base public | <c OMSimpleProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename PropertyType>
class OMVariableSizeProperty : public OMSimpleProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVariableSizeProperty(const OMPropertyId propertyId, const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMVariableSizeProperty(void);

    // @cmember Get the value of this <c OMVariableSizeProperty>.
  void getValue(PropertyType* value, OMPropertySize valueSize) const;

    // @cmember Set the value of this <c OMVariableSizeProperty>. The
    //          value is set by copying <p valueSize> bytes from the
    //          address <p value> into the <c OMVariableSizeProperty>.
  void setValue(const PropertyType* value, OMPropertySize valueSize);

    // @cmember Set the value of this <c OMVariableSizeProperty>. The
    //          value is set by copying <p elementCount> elements from the
    //          address <p value> into the <c OMVariableSizeProperty>.
  void setElementValues(const PropertyType* value, OMUInt32 elementCount);

    // @cmember Get the value of the item at position <p index> in this
    //          <c OMVariableSizeProperty>. The value is obtained by copying
    //          a single item of type PropertyType from this
    //          <c OMVariableSizeProperty> at position <p index>.
  void getValueAt(PropertyType* value, const OMUInt32 index) const;

    // @cmember Set the value of the item at position <p index> in this
    //          <c OMVariableSizeProperty>. The value is set by copying
    //          a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at position <p index>.
  void setValueAt(const PropertyType* value, const OMUInt32 index);

    // @cmember Set the value of the item at the last position in this
    //          <c OMVariableSizeProperty>. The <c OMVariableSizeProperty>
    //          is first increased in size by one item. The value is then
    //          set by copying a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at the last position.
  void appendValue(const PropertyType* value);

    // @cmember Set the value of the item at the first position in this
    //          <c OMVariableSizeProperty>. The <c OMVariableSizeProperty>
    //          is first increased in size by one item and all existing items
    //          are moved up by on position. The value is then
    //          set by copying a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at the first position.
  void prependValue(const PropertyType* value);

    // @cmember Get the value of this <c OMVariableSizeProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMVariableSizeProperty>. The buffer is at address
    //          <p buffer> and is <p bufferSize> bytes in size.
    //          Copying only takes place if the buffer is large enough.
  bool copyToBuffer(PropertyType* buffer, OMUInt32 bufferSize) const;

    // @cmember Get the value of this <c OMVariableSizeProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMVariableSizeProperty>. The buffer is at address
    //          <p buffer> and is <p elementCount> elements in size.
    //          Copying only takes place if the buffer is large enough.
  bool copyElementsToBuffer(PropertyType* buffer, OMUInt32 elementCount) const;

    // @cmember Restore this <c OMVariableSizeProperty>, the external
    //          (persisted) size of the <c OMVariableSizeProperty> is
    //          <p externalSize>.
  virtual void restore(OMPropertySize externalSize);

    // @cmember The number of items in this this <c OMVariableSizeProperty>.
  OMUInt32 count(void) const;

};

#include "OMVariableSizePropertyT.h"

#endif
