//=---------------------------------------------------------------------=
//
// $Id: OMFixedSizePropertyT.h,v 1.18 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMFixedSizeProperty
#ifndef OMFIXEDSIZEPROPERTYT_H
#define OMFIXEDSIZEPROPERTYT_H

#include "OMAssertions.h"
#include "OMStoredObject.h"

template <typename PropertyType>
OMFixedSizeProperty<PropertyType>::OMFixedSizeProperty(
                                                 const OMPropertyId propertyId,
                                                 const wchar_t* name)
: OMSimpleProperty(propertyId, name, sizeof(PropertyType))
{
  TRACE("OMFixedSizeProperty<PropertyType>::OMFixedSizeProperty");
}

template <typename PropertyType>
OMFixedSizeProperty<PropertyType>::~OMFixedSizeProperty(void)
{
  TRACE("OMFixedSizeProperty<PropertyType>::~OMFixedSizeProperty");
}

  // @mfunc Get the value of this <c OMFixedSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm A value of <p PropertyType> by reference.
  //   @this const
template <typename PropertyType>
void OMFixedSizeProperty<PropertyType>::getValue(PropertyType& value) const
{
  TRACE("OMFixedSizeProperty<PropertyType>::getValue");
  ASSERT("Sizes match", size() == sizeof(PropertyType));

  get(&value, size());
}

  // @mfunc Set the value of this <c OMFixedSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm A value of <p PropertyType> by reference.
template <typename PropertyType>
void OMFixedSizeProperty<PropertyType>::setValue(const PropertyType& value)
{
  TRACE("OMFixedSizeProperty<PropertyType>::setValue");
  ASSERT("Sizes match", size() == sizeof(PropertyType));

  set(&value, size());
}

  // @mfunc Assignment operator.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm A value of <p PropertyType> by reference.
  //   @rdesc A value of <c OMFixedSizeProperty> by reference.
template <typename PropertyType>
OMFixedSizeProperty<PropertyType>&
OMFixedSizeProperty<PropertyType>::operator = (const PropertyType& value)
{
  TRACE("OMFixedSizeProperty<PropertyType>::operator =");

  setValue(value);
  return *this;
}

  // @mfunc Type conversion. Convert an <c OMFixedSizeProperty>
  //        into a <p PropertyType>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc The result of the conversion as a value of type
  //          <p PropertyType>.
  //   @this const
template <typename PropertyType>
OMFixedSizeProperty<PropertyType>::operator PropertyType() const
{
  TRACE("OMFixedSizeProperty<PropertyType>::operator PropertyType");

  PropertyType result;
  getValue(result);
  return result;
}

  // @mfunc "Address of" operator.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc Pointer to a <p PropertyType>
template <typename PropertyType>
PropertyType* OMFixedSizeProperty<PropertyType>::operator &(void)
{
  return reinterpret_cast<PropertyType*>(bits());
}

  // @mfunc Const "Address of" operator.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc Const pointer to a <p PropertyType>
template <typename PropertyType>
const PropertyType* OMFixedSizeProperty<PropertyType>::operator &(void) const
{
  return reinterpret_cast<const PropertyType*>(bits());
}

  // @mfunc Convert this <c OMFixedSizeProperty> into a const
  //        reference to a <p PropertyType>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc Const reference to a <p PropertyType>.
  //   @this const
template <typename PropertyType>
const PropertyType& OMFixedSizeProperty<PropertyType>::reference(void) const
{
  return *reinterpret_cast<const PropertyType*>(bits());
}

  // @mfunc Restore this <c OMFixedSizeProperty>, the external (persisted)
  //        size of the <c OMFixedSizeProperty> is <p externalSize>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm The external (persisted) size of the <c OMFixedSizeProperty>.
template <typename PropertyType>
void OMFixedSizeProperty<PropertyType>::restore(OMPropertySize externalSize)
{
  TRACE("OMFixedSizeProperty<PropertyType>::restore");

  store()->restore(*this, externalSize);
  setPresent();
}

#endif
