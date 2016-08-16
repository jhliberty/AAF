//=---------------------------------------------------------------------=
//
// $Id: OMProperty.cpp,v 1.102 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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

#include "OMProperty.h"

#include "OMAssertions.h"
#include "OMStorable.h"
#include "OMType.h"
#include "OMUtilities.h"
#include "OMPropertyDefinition.h"
#include "OMStoredObject.h"

#include <string.h>

// class OMProperty
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The stored form of this <c OMProperty>.
  //   @parm The name of this <c OMProperty>.
OMProperty::OMProperty(const OMPropertyId propertyId,
                       const OMStoredForm storedForm,
                       const wchar_t* name)
: _propertyId(propertyId),
  _storedForm(storedForm),
  _storedName(0),
  _name(name),
  _propertySet(0),
  _definition(0),
#if 1 // HACK4MEIP2
  _isOptional(true),
#else
  _isOptional(false),
#endif
  _isPresent(false)
{
  TRACE("OMProperty::OMProperty");

  PRECONDITION("Valid name", validWideString(_name));
}

  // @mfunc Temporary pseudo-constructor for clients which provide
  //        a property definition.
  //   @parm The definition of this <c OMProperty>.
void OMProperty::initialize(const OMPropertyDefinition* definition)
{
  TRACE("OMProperty::initialize");

  PRECONDITION("Valid property definition", definition != 0);
  _definition = definition;

  // Temporary consistency checks
  ASSERT("Consistent property id",
                            _propertyId == _definition->localIdentification());
  // ASSERT("Consistent property optionality",
  //                                 _isOptional == _definition->isOptional());
  _isOptional = _definition->isOptional();
}

  // @mfunc Destructor.
OMProperty::~OMProperty(void)
{
  TRACE("OMProperty::~OMProperty");

  delete [] _storedName;
}

  // @mfunc Close this <c OMProperty>.
void OMProperty::close(void)
{
  TRACE("OMProperty::close");

  // nothing to do for most descendants of OMProperty
}

  // @mfunc Detach this <c OMProperty>.
void OMProperty::detach(void)
{
  TRACE("OMProperty::detach");

  // nothing to do for most descendants of OMProperty
}

  // @mfunc The <c OMPropertyDefinition> defining this <c OMProperty>.
  //   @rdesc The defining <c OMPropertyDefinition>.
  //   @this const
const OMPropertyDefinition* OMProperty::definition(void) const
{
  TRACE("OMProperty::definition");

  const OMPropertyDefinition* result = _definition;
  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Is this an instance of a predefined <c OMProperty>.
  //   @rdesc True if this is an instance of a predefined property,
  //          false otherwise.
  //   @this const
bool OMProperty::isPredefined(void) const
{
  TRACE("OMProperty::isPredefined");
  PRECONDITION("Valid property id", _propertyId != 0);
  bool result;

  if (_propertyId < 0x8000) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The name of this <c OMProperty>.
  //   @rdesc The property name.
  //   @this const
const wchar_t* OMProperty::name(void) const
{
  TRACE("OMProperty::name");

  return _name;
}

  // @mfunc The property id of this <c OMProperty>.
  //   @rdesc The property id.
  //   @this const
OMPropertyId OMProperty::propertyId(void) const
{
  TRACE("OMProperty::propertyId");

  return _propertyId;
}

  // @mfunc The <c OMPropertySet> containing this <c OMProperty>.
  //   @rdesc The containing <c OMPropertySet>.
  //   @this const
const OMPropertySet* OMProperty::propertySet(void) const
{
  TRACE("OMProperty::propertySet");

  return _propertySet;
}

  // @mfunc Inform this <c OMProperty> that it is a member of
  //        the <c OMPropertySet> <p propertySet>.
  //   @parm The <c OMPropertySet> of which this <c OMProperty> is
  //   a member.
void OMProperty::setPropertySet(const OMPropertySet* propertySet)
{
  TRACE("OMProperty::setPropertySet");
  PRECONDITION("Valid property set", propertySet != 0);

  _propertySet = propertySet;
}

  // @mfunc The address of this <c OMProperty> object. This function
  //        is defined so that descendants may override "operator &" to
  //        obtain the address of the contained property value. This
  //        function can then be used to obtain the address of this
  //        <c OMProperty>.
  //   @rdesc The address of this <c OMProperty>.
OMProperty* OMProperty::address(void)
{
  TRACE("OMProperty::address");

  return this;
}

  // @mfunc Set the bit that indicates that this optional <c OMProperty>
  //        is present.
void OMProperty::setPresent(void)
{
  TRACE("OMProperty::setPresent");

  _isPresent = true;
}

  // @mfunc Clear the bit that indicates that this optional <c OMProperty>
  //        is present.
void OMProperty::clearPresent(void)
{
  TRACE("OMProperty::clearPresent");
  PRECONDITION("Property is optional", isOptional());

  _isPresent = false;
}

  // @mfunc The type of this <c OMProperty>.
  //   @rdesc The type.
  //   @this const
const OMType* OMProperty::type(void) const
{
  TRACE("OMProperty::type");

  PRECONDITION("Valid property definition", _definition != 0);

  const OMType* result = _definition->type();

#if !defined(OM_NO_VALIDATE_DEFINITIONS)
  POSTCONDITION("Valid result", result != 0);
#endif
  return result;
}

  // @mfunc The <c OMStoredObject> that contains the persisted
  //        representation of this <c OMProperty>.
  //   @rdesc The <c OMStoredObject>.
  //   @this const
OMStoredObject* OMProperty::store(void) const
{
  TRACE("OMProperty::store");

  return container()->store();
}

  // @mfunc The <c OMFile> that contains the persisted
  //        representation of this <c OMProperty>.
  //   @rdesc The containing <c OMFile>.
  //   @this const
OMFile* OMProperty::file(void) const
{
  TRACE("OMProperty::file");

  return container()->file();
}

const wchar_t* OMProperty::storedName(void) const
{
  TRACE("OMProperty::storedName");

  if (_storedName == 0) {
    OMProperty* p = const_cast<OMProperty*>(this);
    p->_storedName = OMStoredObject::referenceName(_name, propertyId());
  }
  return _storedName;
}

// class OMSimpleProperty
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSimpleProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMSimpleProperty>.
OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const wchar_t* name)
: OMProperty(propertyId, SF_DATA, name), _size(0), _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMSimpleProperty>.
  //   @parm The size of this <c OMSimpleProperty>.
OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const wchar_t* name,
                                   OMPropertySize valueSize)
: OMProperty(propertyId, SF_DATA, name),
  _size(0),
  _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
  PRECONDITION("Valid size", (valueSize > 0));

  setSize(valueSize);
}

  // @mfunc Destructor.
OMSimpleProperty::~OMSimpleProperty(void) 
{
  TRACE("OMSimpleProperty::~OMSimpleProperty");

  delete [] _bits;
}

  // @mfunc The size of this <c OMSimpleProperty>.
  //   @rdesc The property size in bytes.
  //   @this const
OMPropertySize OMSimpleProperty::size(void) const
{ 
  TRACE("OMSimpleProperty::size");

  return _size;
}

  // @mfunc The number of objects contained within this
  //        <c OMProperty> if any.
  //   @rdesc The number of objects.
OMUInt64 OMSimpleProperty::objectCount(void) const
{
  TRACE("OMSimpleProperty::objectCount");
  // OMSimpleProperty doesn't contain objects.
  return 0;
}

  // @mfunc Set the size of this <c OMSimpleProperty> to <p newSize> bytes.
  //   @parm The new property size in bytes.
void OMSimpleProperty::setSize(OMPropertySize newSize)
{
  TRACE("OMSimpleProperty::setSize");

  PRECONDITION("Valid size", newSize > 0);

  if (newSize != _size) {
    if (_bits != 0)
    {
      delete [] _bits;
      _bits = 0; // for BoundsChecker
      _bits = new unsigned char[newSize];
      ASSERT("Valid heap pointer", _bits != 0);
    }
    _size = newSize;
  }
}

  // @mfunc The <c OMStorable> that contains this <c OMProperty>.
  //   @rdesc The containing <c OMStorable>.
  //   @this const
OMStorable* OMProperty::container(void) const
{
  TRACE("OMProperty::container");

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* result = _propertySet->container();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

void OMSimpleProperty::shallowCopyTo(OMProperty* destination) const
{
  TRACE("OMSimpleProperty::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  OMSimpleProperty* dest = dynamic_cast<OMSimpleProperty*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  dest->set(bits(), _size);
}

void OMSimpleProperty::deepCopyTo(OMProperty* /* destination */,
                                  void* /* clientContext */) const
{
  TRACE("OMSimpleProperty::deepCopyTo");
  // Nothing to do - this is a deep copy
}

  // @mfunc Get the value of this <c OMSimpleProperty>.
  //   @parm The buffer to receive the property value.
  //   @parm OMPropertySize | valueSize | The size of the buffer.
  //   @this const
void OMSimpleProperty::get(void* value, OMPropertySize ANAME(valueSize)) const
{
  TRACE("OMSimpleProperty::get");
  PRECONDITION("Valid data buffer", value != 0);
  PRECONDITION("Valid size", valueSize >= _size);
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  memcpy(value, bits(), _size);
}

  // @mfunc Set the value of this <c OMSimpleProperty>.
  //   @parm The address of the property value.
  //   @parm The size of the value.
  //   @this const
void OMSimpleProperty::set(const void* value, OMPropertySize valueSize)
{
  TRACE("OMSimpleProperty::set");
  PRECONDITION("Valid data buffer", value != 0);
  PRECONDITION("Valid size", valueSize > 0);

  setSize(valueSize);
  memcpy(bits(), value, _size);
  setPresent();
}

  // @mfunc Save this <c OMSimpleProperty>.
  //   @this const
void OMSimpleProperty::save(void) const
{
  TRACE("OMSimpleProperty::save");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  store()->save(*this);
}

  // @mfunc Restore this <c OMSimpleProperty>, the external (persisted)
  //        size of the <c OMSimpleProperty> is <p externalSize>.
  //   @parm The size of the <c OMSimpleProperty>.
void OMSimpleProperty::restore(OMPropertySize externalSize)
{
  TRACE("OMSimpleProperty::restore");
  ASSERT("Sizes match", externalSize == _size);

  store()->restore(*this, externalSize);
  setPresent();
}

  // @mfunc Is this an optional property ?
  //   @rdesc True if this property is optional, false otherwise.
  //   @this const
bool OMProperty::isOptional(void) const
{
  TRACE("OMProperty::isOptional");
  return _isOptional;
}

  // @mfunc Is this optional property present ?
  //   @rdesc True if this property is present, false otherwise.
  //   @this const
bool OMProperty::isPresent(void) const
{
  TRACE("OMProperty::isPresent");

  PRECONDITION("Property is optional", isOptional());
  return _isPresent;
}

  // @mfunc Is this <c OMProperty> void ?
  //   @rdesc True if this <c OMProperty> is void, false otherwise.
  //   @this const
bool OMProperty::isVoid(void) const
{
  TRACE("OMProperty::isVoid");

  // most descendants of OMProperty are always void
  return true;
}

  // @mfunc Remove this optional <c OMProperty>.
void OMProperty::removeProperty(void)
{
  TRACE("OMProperty::removeProperty");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc The value of this <c OMProperty> as an <c OMStorable>.
  //        If this <c OMProperty> does not represent an <c OMStorable>
  //        then the value returned is 0.
  //   @rdesc Always 0.
  //   @this const
OMStorable* OMProperty::storable(void) const
{
  TRACE("OMProperty::storable");

  return 0;
}

OMStoredForm OMProperty::storedForm(void) const
{
  TRACE("OMProperty::storedForm");

  return _storedForm;
}

void OMProperty::setPropertyId(OMPropertyId propertyId)
{
  TRACE("OMProperty::setPropertyId");
  
  _propertyId = propertyId;
}


  // @mfunc The size of the raw bits of this
  //        <c OMSimpleProperty>. The size is given in bytes.
  //   @rdesc The size of the raw bits of this
  //          <c OMSimpleProperty> in bytes.
  //   @this const
OMUInt32 OMSimpleProperty::bitsSize(void) const
{
  TRACE("OMSimpleProperty::bitsSize");

  return _size;
}

OMByte* OMSimpleProperty::bits(void) const
{
  TRACE("OMSimpleProperty::bits");

  if (_bits == 0) {
    if (_size > 0) {
      OMSimpleProperty* nonConstThis = const_cast<OMSimpleProperty*>(this);
      nonConstThis->_bits = new unsigned char[_size];
      ASSERT("Valid heap pointer", _bits != 0);
      for (size_t i = 0; i < _size; i++) {
        _bits[i] = 0;
      }
    }
  }
  return _bits;
}

  // @mfunc Get the raw bits of this <c OMSimpleProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm OMUInt32 | bitsSize | The size of the buffer.
  //   @this const
void OMSimpleProperty::getBits(OMByte* bits,
                               OMUInt32 ANAME(bitsSize)) const
{
  TRACE("OMSimpleProperty::getBits");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", bitsSize >= _size);

  memcpy(bits, this->bits(), _size);
}

  // @mfunc Set the raw bits of this <c OMSimpleProperty>.
  //        The raw bits are copied from the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
void OMSimpleProperty::setBits(const OMByte* bits, OMUInt32 size)
{
  TRACE("OMSimpleProperty::setBits");
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Property value not too big", size <= OMPROPERTYSIZE_MAX);
  set(bits, static_cast<OMPropertySize>(size));
}

