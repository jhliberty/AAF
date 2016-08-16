//=---------------------------------------------------------------------=
//
// $Id: AxBaseObjIterExt.h,v 1.4 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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

#ifndef AxBaseObjIterExtH
#define AxBaseObjIterExtH

#include <AxBaseObjIter.h>


//---------------------------------------------------------------------------
// An object of this class listens to stack activity in the iterator. This
// provides information of where in the tree structure the iterator is
class AxBaseObjRecIterListener
{
  public:
   virtual void ObjectPop() = 0;
   virtual void PropertyPop() = 0;
};


//---------------------------------------------------------------------------
// An object of this class holds the iterator for the node in the tree and
// the node type (either object or property)
class StackMember
{
  public:
   StackMember( AxBaseObjIterPrtcl *axBaseObjIterPrtcl, int type ) :
      _axBaseObjIterPrtcl( axBaseObjIterPrtcl ), _type ( type ) {};

   AxBaseObjIterPrtcl *_axBaseObjIterPrtcl;
   int _type;
};


//---------------------------------------------------------------------------
// This class defines a recursive iterator for the AAF (strong reference) 
// object tree.
// This class is an extension of the AxBaseObjRecIter. It holds the node type
// information of the nodes in the AAF tree structure, and reports stack activity
// to listeners.
class AxBaseObjRecIterExt
{
  public:
   AxBaseObjRecIterExt( std::auto_ptr< AxBaseObjIterPrtcl > root );
   AxBaseObjRecIterExt( std::auto_ptr< AxBaseObjIterPrtcl > root,
			AxBaseObjRecIterListener *listener);

   virtual ~AxBaseObjRecIterExt();

   bool NextOne( std::auto_ptr<AxBaseObj>& objRet, int& level );

   void PopStack();

   int GetLevel();

  protected:
   AxBaseObjRecIterExt();
   AxBaseObjRecIterExt( const AxBaseObjRecIterExt& );
   AxBaseObjRecIterExt& operator=( const AxBaseObjRecIterExt& );

   void Push( std::auto_ptr< AxBaseObjIterPrtcl >i, int type );
   void Pop();

   AxBaseObjIterPrtcl& Top();
   bool Empty();
   int  Size();

   void HandleObjectRecursion( AxObject& obj );
   void HandlePropertyRecursion( AxProperty& prop );
   void HandlePropertyValueRecursion( AxPropertyValue& propVal );
   void HandleRecordPropertyValueRecursion( AxRecordIterator::Pair& recPair );

   std::auto_ptr< AxBaseObjIterPrtcl > _root;
   std::deque< StackMember > _deque;

   AxBaseObjRecIterListener *_listener;
};


//---------------------------------------------------------------------------
#endif

