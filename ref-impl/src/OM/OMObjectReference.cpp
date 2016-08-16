//=---------------------------------------------------------------------=
//
// $Id: OMObjectReference.cpp,v 1.22 2009/06/01 11:47:01 stuart_hc Exp $ $Name: V116 $
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

// @doc OMINTERNAL
#include "OMObjectReference.h"

#include "OMAssertions.h"
#include "OMFile.h"
#include "OMPropertyTable.h"
#include "OMStoredObject.h"
#include "OMStrongReferenceSet.h"
#include "OMUtilities.h"

#if 1 // HACK4MEIP2
#include "OMUniqueObjectIdentType.h"
#endif

// class OMObjectReference
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMObjectReference

  // @mfunc Constructor.
OMObjectReference::OMObjectReference(void)
: _property(0),
  _pointer(0)
{
  TRACE("OMObjectReference::OMObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMObjectReference>.
OMObjectReference::OMObjectReference(OMProperty* property)
: _property(property),
  _pointer(0)
{
  TRACE("OMObjectReference::OMObjectReference");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMObjectReference> to copy.
OMObjectReference::OMObjectReference(const OMObjectReference& rhs)
: _property(rhs._property),
  _pointer(rhs._pointer)
{
  TRACE("OMObjectReference::OMObjectReference");
}

  // @mfunc Destructor.
OMObjectReference::~OMObjectReference(void)
{
  TRACE("OMObjectReference::~OMObjectReference");
}

  // @mfunc Is this <c OMObjectReference> void ?
  //   @rdesc True if this <c OMObjectReference> is void,
  //          false otherwise.
  //   @this const
bool OMObjectReference::isVoid(void) const
{
  bool result;
  if (_pointer == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMObjectReference> to be assigned.
  //   @rdesc The <c OMObjectReference> resulting from the assignment.
OMObjectReference& OMObjectReference::operator= (const OMObjectReference& rhs)
{
  TRACE("OMObjectReference::operator=");

  if (this == &rhs) {
    return *this; // early return !
  }
  _property = rhs._property;
  _pointer = rhs._pointer;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
  //   @this const
bool OMObjectReference::operator== (const OMObjectReference& rhs) const
{
  bool result;

  if ((_property == rhs._property) && (_pointer == rhs._pointer)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The value of this <c OMObjectReference> as a pointer.
  //        This function provides low-level access. If the object exits
  //        but has not yet been loaded then the value returned is 0.
  //   @rdesc A pointer to the referenced object, if any, otherwise 0.
  //   @this const
OMStorable* OMObjectReference::pointer(void) const
{
  TRACE("OMObjectReference::pointer");

  return _pointer;
}

  // @mfunc The <c OMProperty> containing this <c OMObjectReference>.
  //   @rdesc A pointer to the containing <c OMProperty>.
  //   @this const
OMProperty* OMObjectReference::property(void) const
{
  TRACE("OMObjectReference::property");

  return _property;
}

// class OMStrongObjectReference
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStrongObjectReference

  // @mfunc Constructor.
OMStrongObjectReference::OMStrongObjectReference(void)
: OMObjectReference(),
  _isLoaded(true),
  _name(0)
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMStrongObjectReference>.
  //   @parm The name of this <c OMStrongObjectReference>.
OMStrongObjectReference::OMStrongObjectReference(OMProperty* property,
                                                 const wchar_t* name)
: OMObjectReference(property),
  _isLoaded(true),
  _name(saveWideString(name))
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMStrongObjectReference>.
  //   @parm The name of this <c OMStrongObjectReference>.
  //   @parm The state (loaded or not) of this <c OMStrongObjectReference>.
OMStrongObjectReference::OMStrongObjectReference(OMProperty* property,
                                                 const wchar_t* name,
                                                 bool isLoaded)
: OMObjectReference(property),
  _isLoaded(isLoaded),
  _name(saveWideString(name))
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMStrongObjectReference> to copy.
OMStrongObjectReference::OMStrongObjectReference(
                                            const OMStrongObjectReference& rhs)
: OMObjectReference(rhs),
  _isLoaded(rhs._isLoaded),
  _name(0)
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");

  if (rhs._name != 0) {
    _name = saveWideString(rhs._name);
  }
}

  // @mfunc Destructor.
OMStrongObjectReference::~OMStrongObjectReference(void)
{
  TRACE("OMStrongObjectReference::~OMStrongObjectReference");

  delete [] _name;
  _name = 0;
}

  // @mfunc Is this <c OMStrongObjectReference> void ?
  //   @rdesc True if this <c OMStrongObjectReference> is void,
  //          false otherwise.
  //   @this const
bool OMStrongObjectReference::isVoid(void) const
{
  bool result = OMObjectReference::isVoid();
  if (result) {
    if ((!_isLoaded) && (_name != 0)) {
      result = false;
    }
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMStrongObjectReference> to be assigned.
  //   @rdesc The <c OMStrongObjectReference> resulting from the assignment.
OMStrongObjectReference&
OMStrongObjectReference::operator= (const OMStrongObjectReference& rhs)
{
  OMObjectReference::operator=(rhs);
  _isLoaded = rhs._isLoaded;
  delete [] _name;
  _name = 0; // for BoundsChecker
  if (rhs._name != 0) {
    _name = saveWideString(rhs._name);
  } else {
    _name = 0;
  }
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMStrongObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
  //   @this const
bool
OMStrongObjectReference::operator== (const OMStrongObjectReference& rhs) const
{
  bool result = OMObjectReference::operator==(rhs);
  if (result) {
    if (_isLoaded == rhs._isLoaded) {
      if ((_name != 0) && (rhs._name != 0)) {
        if (compareWideString(_name, rhs._name) == 0) {
          result = true;
        } else {
          result = false;
        }
      } else if ((_name == 0) && (rhs._name == 0)) {
        result = true;
      } else {
        result = false;
      }
    } else {
      result = false;
    }
  }
  return result;
}

  // @mfunc Save this <c OMStrongObjectReference>.
  //   @this const
void OMStrongObjectReference::save(void) const
{
  TRACE("OMStrongObjectReference::save");

  if (isLoaded()) {

    // Either the object does not exist in the file or the object
    // exists in the file and has been loaded. In either case the
    // object is saved. If the object has been loaded, we assume, in
    // the absence of a dirty bit, that it has been changed.

    ASSERT("Non-void strong reference", _pointer != 0);

    // Notify the client that we are about to save the object and then
    // save the object.
    //
    ASSERT("Valid containing property", _property != 0);
    OMFile* file = _property->propertySet()->container()->file();
    ASSERT("Valid file", file != 0);
    _pointer->onSave(file->clientOnSaveContext());
    _pointer->save();
  }

}

  // @mfunc Close this <c OMStrongObjectReference>.
void OMStrongObjectReference::close(void)
{
  TRACE("OMStrongObjectReference::close");

  if (_pointer != 0) {
    _pointer->close();
  }

  // So that an attempt to access an unloaded object after the file
  // has been closed results in a void reference (there is an
  // inaccessible persisted representation of the object).
  //
  setLoaded();
}

  // @mfunc Detach this <c OMStrongObjectReference>.
void OMStrongObjectReference::detach(void)
{
  TRACE("OMStrongObjectReference::detach");

  if (_pointer != 0) {
    _pointer->detach();
  }

  // So that an attempt to access an unloaded object that has been
  // detached results in a void reference (there is an inaccessible
  // persisted representation of the object).
  //
  setLoaded();
}

  // @mfunc Restore this <c OMStrongObjectReference>.
void OMStrongObjectReference::restore(void)
{
  TRACE("OMStrongObjectReference::restore");

  PRECONDITION("Correct initial loaded state", isLoaded());
  PRECONDITION("Reference not already set", _pointer == 0);
  PRECONDITION("Valid containing property", _property != 0);

  OMFile::OMLoadMode loadMode =
                     _property->propertySet()->container()->file()->loadMode();

  clearLoaded();
  if (loadMode == OMFile::eagerLoad) {
    load();
  }
}

  // @mfunc Get the value of this <c OMStrongObjectReference>.
  //        The value is a pointer to the referenced <c OMStorable>.
  //   @rdesc  A pointer to the referenced <c OMStorable>.
  //   @this const
OMStorable* OMStrongObjectReference::getValue(void) const
{
  TRACE("OMStrongObjectReference::getValue");

  OMStrongObjectReference* nonConstThis =
                                    const_cast<OMStrongObjectReference*>(this);

  if (!isLoaded()) {
    nonConstThis->load();
  }
  ASSERT("Loaded", isLoaded());

  OMStorable* result = _pointer;

  return result;
}

  // @mfunc Set the value of this <c OMStrongObjectReference>.
  //        The value is a pointer to the referenced <c OMStorable>.
  //   @parm A pointer to the new <c OMStorable>.
  //   @rdesc A pointer to previous <c OMStorable>, if any.
OMStorable* OMStrongObjectReference::setValue(const OMStorable* value)
{
  TRACE("OMStrongObjectReference::setValue");

  PRECONDITION("Valid container property", _property != 0);

  // Detach the old object
  //
  OMStorable* oldObject = _pointer;
  if (oldObject != 0) {
    oldObject->detach();
  }

  // Set the element to contain the new object
  //
  _pointer = const_cast<OMStorable*>(value);
  OMStorable* newObject = _pointer;

  // Attach the new object
  //
  if (newObject != 0) {
    OMStorable* container = _property->propertySet()->container();
    newObject->attach(container, _name);
  }
  setLoaded();
  POSTCONDITION("Element properly set", _pointer == newObject);
  return oldObject;
}

const wchar_t* OMStrongObjectReference::name(void) const
{
  TRACE("OMStrongObjectReference::name");

  return _name;
}

  // @mfunc Is this <c OMStrongObjectReference> in the loaded state. If false
  //        there is a persisted representation of this
  //        <c OMStrongObjectReference> that can be loaded.
  //   @this const
bool OMStrongObjectReference::isLoaded(void) const
{
  TRACE("OMStrongObjectReference::isLoaded");

  return _isLoaded;
}

  // @mfunc Put this <c OMStrongObjectReference> into the loaded state.
void OMStrongObjectReference::setLoaded(void)
{
  TRACE("OMStrongObjectReference::setLoaded");

  _isLoaded = true;
}

  // @mfunc Remove this <c OMStrongObjectReference> from the loaded state.
void OMStrongObjectReference::clearLoaded(void)
{
  TRACE("OMStrongObjectReference::clearLoaded");

  _isLoaded = false;
}

  // @mfunc Load the persisted representation of this
  //        <c OMStrongObjectReference>.
void OMStrongObjectReference::load(void)
{
  TRACE("OMStrongObjectReference::load");

  PRECONDITION("Not already loaded", !isLoaded());
  PRECONDITION("Valid container property", _property != 0);

  // open the sub-storage
  //
  OMStorable* containingObject = _property->propertySet()->container();
  OMStoredObject* store = containingObject->store();
  ASSERT("Valid store", store != 0);

  OMStoredObject* subStorage = store->open(_name);

  // restore referenced object from the sub-storage
  //
  OMStorable* object = subStorage->restoreObject(*this);
  ASSERT("Object properly restored", object != 0);

  // place a pointer to the newly restored object in this element
  //
  setValue(object);

  setLoaded();

  // notify the client that the object has just been restored
  //
  ASSERT("Valid containing property", _property != 0);
  OMFile* file = _property->propertySet()->container()->file();
  ASSERT("Valid file", file != 0);
  _pointer->onRestore(file->clientOnSaveContext());

  POSTCONDITION("Property properly loaded", isLoaded());
}

// class OMWeakObjectReference
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMWeakObjectReference

  // @mfunc Constructor.
OMWeakObjectReference::OMWeakObjectReference(void)
: OMObjectReference(),
  _identification(0),
  _identificationSize(0),
  _targetTag(nullOMPropertyTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference::OMWeakObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMWeakObjectReference>.
OMWeakObjectReference::OMWeakObjectReference(OMProperty* property,
                                                  const void* identification,
                                                  size_t identificationSize)
: OMObjectReference(property),
  _identification(0),
  _identificationSize(identificationSize),
  _targetTag(nullOMPropertyTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference::OMWeakObjectReference");
  PRECONDITION("Valid identification",
                                identification != 0 && identificationSize > 0);

  _identification = new OMByte[_identificationSize];
  ASSERT("Valid heap pointer", _identification != 0);
  memcpy(_identification, identification, _identificationSize);
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMWeakObjectReference>.
  //   @parm The unique key of this <c OMWeakObjectReference>.
  //   @parm A tag identifying the <c OMStrongReferenceSetProperty>
  //         in which the target resides.
OMWeakObjectReference::OMWeakObjectReference(
                                   OMProperty* property,
                                   const void* identification,
                                   size_t identificationSize,
                                   OMPropertyTag targetTag)
: OMObjectReference(property),
  _identification(0),
  _identificationSize(identificationSize),
  _targetTag(targetTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference::OMWeakObjectReference");
  PRECONDITION("Valid identification",
                                identification != 0 && identificationSize > 0);

  _identification = new OMByte[_identificationSize];
  ASSERT("Valid heap pointer", _identification != 0);
  memcpy(_identification, identification, _identificationSize);
}

  // @mfunc Copy constructor.
  //   @parm The <c OMWeakObjectReference> to copy.
OMWeakObjectReference::OMWeakObjectReference(
                                              const OMWeakObjectReference& rhs)
: OMObjectReference(rhs),
  _identification(0),
  _identificationSize(rhs._identificationSize),
  _targetTag(rhs._targetTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference::OMWeakObjectReference");

  delete [] _identification;
  _identification = 0;
  if (rhs._identification != 0 ) {
    _identification = new OMByte[_identificationSize];
    ASSERT("Valid heap pointer", _identification != 0);
    memcpy(_identification, rhs._identification, _identificationSize);
  }
}

  // @mfunc Destructor.
OMWeakObjectReference::~OMWeakObjectReference(void)
{
  delete [] _identification;
  _identification = 0;
}

  // @mfunc Is this <c OMWeakObjectReference> void ?
  //   @rdesc True if this <c OMWeakObjectReference> is void,
  //          false otherwise.
  //   @this const
bool OMWeakObjectReference::isVoid(void) const
{
  TRACE("OMWeakObjectReference::isVoid");
  bool result = OMObjectReference::isVoid();
  if (result) {
    if (!isNullIdentification(_identification, _identificationSize)) {
      ASSERT("Valid containing property", _property != 0);
      OMFile* file = _property->propertySet()->container()->file();
      OMPropertyTable* table = file->referencedProperties();
      if (!table->isValid(_targetTag)) {
        result = true;
      } else {
        result = false;
      }
    }
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMWeakObjectReference> to be assigned.
  //   @rdesc The <c OMWeakObjectReference> resulting from the assignment.
OMWeakObjectReference&
OMWeakObjectReference::operator= (const OMWeakObjectReference& rhs)
{
  TRACE("OMWeakObjectReference::operator=");
  PRECONDITION("Valid identification", 
                    IMPLIES(_identification != 0,
                            (rhs._identificationSize == 0) ||
                            (rhs._identificationSize == _identificationSize)));

  if (this == &rhs) {
    return *this; // early return !
  }
  OMObjectReference::operator=(rhs);
  _identificationSize = rhs._identificationSize;
  delete [] _identification;
  _identification = 0; // for BoundsChecker
  if (rhs._identification != 0) {
    _identification = new OMByte[_identificationSize];
    ASSERT("Valid heap pointer", _identification != 0);
    memcpy(_identification, rhs._identification, _identificationSize);
  }
  _targetTag = rhs._targetTag;
  _targetSet = 0;

  return *this;
}

  // @mfunc Equality.
  //   @parm The <c OMWeakObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
  //   @this const
bool OMWeakObjectReference::operator== (const OMWeakObjectReference& rhs) const
{
  TRACE("OMWeakObjectReference::operator==");

  bool result;

  if ((_identification != 0) && (rhs._identification != 0)) {
    if (memcmp(_identification,
               rhs._identification,
               _identificationSize) == 0) {
      result = true;
    } else {
      result = false;
    }
  } else if ((_identification == 0) && (rhs._identification == 0)) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

  // @mfunc Save this <c OMWeakObjectReference>.
  //   @this const
void OMWeakObjectReference::save(void) const
{
  TRACE("OMWeakObjectReference::save");
  PRECONDITION("Valid identification",
                  !isNullIdentification(_identification, _identificationSize));

  // tjb nothing to do ?
}

  // @mfunc Close this <c OMWeakObjectReference>.
void OMWeakObjectReference::close(void)
{
  TRACE("OMWeakObjectReference::close");

}

  // @mfunc Detach this <c OMWeakObjectReference>.
void OMWeakObjectReference::detach(void)
{
  TRACE("OMWeakObjectReference::detach");
}

  // @mfunc Restore this <c OMWeakObjectReference>.
void OMWeakObjectReference::restore(void)
{
  TRACE("OMWeakObjectReference::restore");

  PRECONDITION("Reference not already set", _pointer == 0);
  PRECONDITION("Valid identification",
                  !isNullIdentification(_identification, _identificationSize));

  // tjb nothing to do ?

}

  // @mfunc Get the value of this <c OMWeakObjectReference>.
  //        The value is a pointer to the referenced <c OMStorable>.
  //   @rdesc  A pointer to the referenced <c OMStorable>.
  //   @this const
OMStorable* OMWeakObjectReference::getValue(void) const
{
  TRACE("OMWeakObjectReference::getValue");

  OMWeakObjectReference* nonConstThis =
                                      const_cast<OMWeakObjectReference*>(this);

  if ((_pointer == 0) &&
      (!isNullIdentification(_identification, _identificationSize))) {
    OMStorable* object = 0;
    set()->find(_identification, object);
    nonConstThis->_pointer = object;
  }
#if 1 // HACK4MEIP2
  if ((_pointer == 0) &&
      (!isNullIdentification(_identification, _identificationSize))) {
    // We failed to resolve the reference as an object id, try again as a label
    // We should only come here for KLV encoded files.
    ASSERT("Referenced object ID can be a label",
                  _identificationSize == sizeof(OMUniqueObjectIdentification));
    OMUniqueObjectIdentification bid;
    memcpy(&bid, _identification, sizeof(OMUniqueObjectIdentification));
    if (hostByteOrder() != bigEndian) {
	  OMUniqueObjectIdentificationType::instance()->reorder(
                                               reinterpret_cast<OMByte*>(&bid),
                                               sizeof(bid));
    }
    OMKLVKey k;
    memcpy(&k, &bid, sizeof(OMKLVKey));
    OMUniqueObjectIdentification id;
    convert(id, k);
    memcpy(_identification, &id, sizeof(OMUniqueObjectIdentification));
    OMStorable* object = 0;
    set()->find(_identification, object);
    nonConstThis->_pointer = object;
  }
#endif
  // If the following assertion is violated we have a dangling weak
  // reference.  The reference illegally designates an object that is
  // not present in the target set.  Code elsewhere prevents the
  // removal of objects that are weakly referenced hence a dangling
  // reference is an assertion violation rather than a run-time error.
  //
  POSTCONDITION("Object found", 
           IMPLIES(!isNullIdentification(_identification, _identificationSize),
                   _pointer != 0));
  return _pointer;
}

  // @mfunc Set the value of this <c OMWeakObjectReference>.
  //        The value is a pointer to the referenced <c OMStorable>.
  //   @parm TBS
  //   @parm A pointer to the new <c OMStorable>.
  //   @rdesc A pointer to previous <c OMStorable>, if any.
OMStorable* OMWeakObjectReference::setValue(
                            const void* identification,
                            const OMStorable* value)
{
  TRACE("OMWeakObjectReference::setValue");

  PRECONDITION("Valid container property", _property != 0);
  PRECONDITION("Valid identification",
                          (_identification != 0) && (_identificationSize > 0));
  PRECONDITION("Valid new identification", identification != 0);

  ASSERT("Valid identification",
          IMPLIES(value != 0,
                  !isNullIdentification(identification, _identificationSize)));
  ASSERT("Valid identification",
          IMPLIES(value == 0,
                  !isNullIdentification(identification, _identificationSize)));

  OMStorable* oldObject = _pointer;
  _pointer = const_cast<OMStorable*>(value);
  memcpy(_identification, identification, _identificationSize);

#if defined(OM_VALIDATE_WEAK_REFERENCES)
#if 0
  ASSERT("Consistent source and target",
                     IMPLIES(_pointer != 0, set()->contains(_identification)));
#endif
#endif

  POSTCONDITION("Element properly set", _pointer == value);
  return oldObject;
}

OMStrongReferenceSet*
OMWeakObjectReference::targetSet(const OMProperty* property,
                                 OMPropertyTag targetTag)
{
  TRACE("OMWeakObjectReference::targetSet");

  ASSERT("Valid containing property", property != 0);
  OMFile* file = property->propertySet()->container()->file();
  OMPropertyTable* table = file->referencedProperties();
  ASSERT("Valid target tag", table->isValid(targetTag));
  const OMPropertyId* targetPath = table->valueAt(targetTag);
  ASSERT("Valid target path", validPropertyPath(targetPath));

  OMProperty* set = file->findProperty(targetPath);

  OMStrongReferenceSet* result = dynamic_cast<OMStrongReferenceSet*>(set);

  POSTCONDITION("Valid result", result != 0);
  return result;
}

const void*
OMWeakObjectReference::identification(void) const
{
  return _identification;
}

void
OMWeakObjectReference::setIdentification(const void* id)
{
  TRACE("OMWeakObjectReference::setIdentification");
  PRECONDITION("Valid identification",
                          (_identification != 0) && (_identificationSize > 0));
  PRECONDITION("Valid new identification", id != 0);

  memcpy(_identification, id, _identificationSize);
}

void OMWeakObjectReference::setTargetTag(OMPropertyTag targetTag)
{
  _targetTag = targetTag;
}

OMStrongReferenceSet* OMWeakObjectReference::set(void) const
{
  TRACE("OMWeakObjectReference::set");

  if (_targetSet == 0) {
    OMWeakObjectReference* nonConstThis =
                                      const_cast<OMWeakObjectReference*>(this);
    nonConstThis->_targetSet = targetSet(_property, _targetTag);
  }

  POSTCONDITION("Valid result", _targetSet != 0);
  return _targetSet;
}

/*static*/
bool OMWeakObjectReference::isNullIdentification(
                                               const void* identification,
                                               size_t identificationSize)
{
  TRACE("OMWeakObjectReference::isNullIdentification");
  PRECONDITION("Valid identification",
                         IMPLIES(identification != 0, identificationSize > 0));
  PRECONDITION("Valid identification",
                        IMPLIES(identification == 0, identificationSize == 0));

  bool result = true;
  if (identification != 0) {
    const OMByte* bytes = reinterpret_cast<const OMByte*>(identification);
    for (size_t i=0; i<identificationSize; i++) {
      if (bytes[i] != 0) {
        result = false;
        break;
      }
    }
  }

  return result;
}
