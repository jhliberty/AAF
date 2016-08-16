#ifndef __AxBaseObjIter_h__
#define __AxBaseObjIter_h__

//=---------------------------------------------------------------------=
//
// $Id: AxBaseObjIter.h,v 1.8 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "AxBaseObj.h"

// Required for AxRecordIterator.
#include "AxIterator.h"

#include <memory>
#include <utility>
#include <deque>

class AxObject;
class AxProperty;
class AxPropertyValue;


// AxBaseObjIterPrtcl defines the interface of AxBaseObj interators.
class AxBaseObjIterPrtcl {
public:

	virtual ~AxBaseObjIterPrtcl() = 0;

	virtual bool NextOne( std::auto_ptr<AxBaseObj>& ) = 0;

	virtual void Reset() = 0;

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone() = 0;
};


// Implementation of AxBaseObjIterPrtcl that simply forwards calls to
// an iterator of type IterType that produces objects of type ObjType.
// ObjType is in instance of a class derived from AxBaseObj.
//
// IterType must have the following attributes:
// - A TypeDef called TypeSP that is the same type used as an argument
// to an ObjType constructor.
// - A method NextOne( IterType::TypeSP& );

template <class IterType, class ObjType, class IAAFType>
class AxBaseObjIter : public AxBaseObjIterPrtcl {
public:
	AxBaseObjIter( const IterType& iter )
	: _iter( iter )
	{}

	virtual ~AxBaseObjIter()
	{}

	virtual bool NextOne( std::auto_ptr<AxBaseObj>& ret )
	{
#if 0
	  // This is, hopefully, only temporarily disabled.  It works
	  // fine with MSVC, but won't compile with gcc 3.0.3.  I
	  // would like to get to the bottom of the gcc before giving
	  // up on it.  It should be fine.
	        IterType::TypeSP typeSP;
#else
		IAAFSmartPointer2<IAAFType> typeSP;
#endif
		bool rc;
		rc = _iter.NextOne( typeSP );
		if ( rc ) {
			std::auto_ptr<AxBaseObj> apObj( new ObjType ( typeSP ) );
			ret = apObj;
			return true;
		}
		return false;
	}

	virtual void Reset()
	{
		_iter.Reset();
	}

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone()
	{
		std::auto_ptr< IterType > iter( _iter.Clone() );
		return std::auto_ptr<AxBaseObjIterPrtcl>(
			new AxBaseObjIter<IterType,ObjType,IAAFType>( *iter ) );
	}

private:
	IterType _iter;
};


template < class ObjType >
class AxBaseSolitaryObjIter : public AxBaseObjIterPrtcl {
public:

	AxBaseSolitaryObjIter( const ObjType& obj )
		: _obj( obj ),
		  _done( false )
	{}

	~AxBaseSolitaryObjIter()
	{}

	virtual bool NextOne( std::auto_ptr<AxBaseObj>& ret )
	{
		if ( _done ) {
 		        return false;
		}
		else {
			std::auto_ptr<AxBaseObj> apObj( new ObjType(_obj) );
			_done = true;
			ret = apObj;
			return true;
		}
	}

	virtual void Reset()
	{
		_done = false;
	}

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone()
	{
		std::auto_ptr<AxBaseObjIterPrtcl> iter (
			new AxBaseSolitaryObjIter( _obj ) );
		return iter;
	}

private:
	ObjType _obj;
	bool _done;
};


// AxBaseObjIterPrtcl for odd ball records

class AxBaseRecordObjIter : public AxBaseObjIterPrtcl {
public:
	AxBaseRecordObjIter( std::auto_ptr< AxRecordIterator > );
	~AxBaseRecordObjIter();

	virtual bool NextOne( std::auto_ptr<AxBaseObj>& ret );

	virtual void Reset();

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone();

private:
	AxBaseRecordObjIter();
	AxBaseRecordObjIter( const AxBaseRecordObjIter& );
	AxBaseRecordObjIter& operator=( const AxBaseRecordObjIter& );

	std::auto_ptr<AxRecordIterator> _axRecordIter;
};

// AxBaseObjIterPrtcl for odd ball arrays

template <class TypeDef>
class AxBaseArrayObjIter : public AxBaseObjIterPrtcl {
public:
	AxBaseArrayObjIter( std::auto_ptr< AxArrayIterator<TypeDef> > );
	~AxBaseArrayObjIter();

	virtual bool NextOne( std::auto_ptr<AxBaseObj>& ret );

	virtual void Reset();

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone();

private:
	AxBaseArrayObjIter();
	AxBaseArrayObjIter( const AxBaseArrayObjIter& );
	AxBaseArrayObjIter& operator=( const AxBaseArrayObjIter& );

	std::auto_ptr<AxArrayIterator<TypeDef> > _axArrayIter;
};

//=---------------------------------------------------------------------=

// Ax Interchange Object Recursive Iterator
// Presents the objects as a tree of AAF Objects, Properties,
// and Property Values.
//
// The implementatin of NextOne() examines the object about
// to be returned and, if necessary, creates an iterator to
// represent the next level of recursion and pushes it on
// the internal stack.  You can detect that this has occurred
// as follows:
//
//		NextOne( &level )
//		if ( level != GetLevel() ) {recursed to next level};

// TODO - Implement Clone and Reset methods.

class AxBaseObjRecIter {
public:
	
	AxBaseObjRecIter( std::auto_ptr< AxBaseObjIterPrtcl > root );

	virtual ~AxBaseObjRecIter();

	bool NextOne( std::auto_ptr<AxBaseObj>& objRet, int& level );

	void PopStack();

	int GetLevel();

private:

	AxBaseObjRecIter();
	AxBaseObjRecIter( const AxBaseObjRecIter& );
	AxBaseObjRecIter& operator=( const AxBaseObjRecIter& );

	void Push( std::auto_ptr< AxBaseObjIterPrtcl > );
	void Pop();
	AxBaseObjIterPrtcl& Top();
	bool Empty();
	int  Size();

	void HandleObjectRecursion( AxObject& obj );
	void HandlePropertyRecursion( AxProperty& prop );
	void HandlePropertyValueRecursion( AxPropertyValue& propVal );
	void HandleRecordPropertyValueRecursion( AxRecordIterator::Pair& recPair );

	std::auto_ptr< AxBaseObjIterPrtcl > _root;
	std::deque< AxBaseObjIterPrtcl* > _deque;
};

#endif
