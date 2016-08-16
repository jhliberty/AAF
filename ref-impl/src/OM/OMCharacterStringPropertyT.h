//=---------------------------------------------------------------------=
//
// $Id: OMCharacterStringPropertyT.h,v 1.15 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMCharacterStringProperty
#ifndef OMCHARACTERSTRINGPROPERTYT_H
#define OMCHARACTERSTRINGPROPERTYT_H

#include "OMAssertions.h"

// class OMCharacterStringProperty<CharacterType>

template <typename CharacterType>
OMCharacterStringProperty<CharacterType>::OMCharacterStringProperty(
                                                 const OMPropertyId propertyId,
                                                 const wchar_t* name)
: OMVariableSizeProperty<CharacterType>(propertyId, name)
{
  TRACE("OMCharacterStringProperty<CharacterType>::OMCharacterStringProperty");
}

template <typename CharacterType>
OMCharacterStringProperty<CharacterType>::~OMCharacterStringProperty(void)
{
  TRACE(
       "OMCharacterStringProperty<CharacterType>::~OMCharacterStringProperty");
}

  // @mfunc Type conversion. Convert an
  //        <c OMCharacterStringProperty> into a
  //        string of <p CharacterType> characters.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p CharacterType>.
template <typename CharacterType>
OMCharacterStringProperty<CharacterType>::operator const CharacterType*() const
{
  return reinterpret_cast<const CharacterType*>(this->bits());
}

  // @mfunc Assign the string <p characterString> to this
  //        <c OMCharacterStringProperty>.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @parm The string of <p CharacterType> characters to assign.
template <typename CharacterType>
void OMCharacterStringProperty<CharacterType>::assign(
                                          const CharacterType* characterString)
{
  TRACE("OMCharacterStringProperty<CharacterType>::assign");
  const CharacterType* source;
  const CharacterType empty = 0;

  if (characterString != 0) {
    source = characterString;
  } else {
    source = &empty;
  }
  ASSERT("String not too long",
   ((stringLength(source) + 1) * sizeof(CharacterType)) <= OMPROPERTYSIZE_MAX);
  setValue(source, static_cast<OMPropertySize>(
                          (stringLength(source) + 1) * sizeof(CharacterType)));
}

  // @mfunc The length of this <c OMCharacterStringProperty> in
  //        characters (not counting the null terminating character).
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @rdesc The length of this <c OMCharacterStringProperty>.
  //   @this const
template <typename CharacterType>
size_t OMCharacterStringProperty<CharacterType>::length(void) const
{
  return (this->size() - sizeof(CharacterType)) / sizeof(CharacterType);
}

  // @mfunc Utility function for computing the length, in characters,
  //        of the string of <p CharacterType> characters
  //        <p characterString>.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @parm  A  string of <p CharacterType> characters.
  //   @rdesc The length of the the string of <p CharacterType>
  //          characters <p characterString>.
template <typename CharacterType>
size_t OMCharacterStringProperty<CharacterType>::stringLength(
                                          const CharacterType* characterString)
{
  const CharacterType* p = characterString;
  size_t length = 0;
  while (*p != 0) {
    ++length;
    ++p;
  }
  return length;
}

#endif
