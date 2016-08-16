//=---------------------------------------------------------------------=
//
// $Id: AxBaseObjIterExt.cpp,v 1.6 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif


#include <AxBaseObjIterExt.h>

#include <AxObject.h>
#include <AxProperty.h>
#include <AxPropertyValue.h>
#include <AxMetaDef.h>
#include <AxIterator.h>


//=---------------------------------------------------------------------=

namespace {

   class PropValToIter : public AxPropertyValueNoopPrtcl {
      public:

	 PropValToIter();

	 virtual ~PropValToIter();

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStrongObjRefSP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefSetSP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefVariableArraySP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRecordSP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefFixedArraySP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIndirectSP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefOpaqueSP& );

	 virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRenameSP& );

	 bool ResultExists();

	 std::auto_ptr<AxBaseObjIterPrtcl> GetResult();

      private:

	 inline void Post( std::auto_ptr<AxBaseObjIterPrtcl> iter ) {
	    _isSet = true;
	    _result= iter;
	 }
	
	 bool _isSet;
	 std::auto_ptr<AxBaseObjIterPrtcl> _result;
   };

   PropValToIter::PropValToIter()
      :	_isSet( false )
   {}

   PropValToIter::~PropValToIter()
   {}

   bool PropValToIter::ResultExists()
   {
      return _isSet;
   }

   std::auto_ptr<AxBaseObjIterPrtcl> PropValToIter::GetResult()
   {
      return _result;
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefStrongObjRefSP& spIaafTypeDefStrongObjRef )
   {
      AxTypeDefStrongObjRef axStrongObjRef( spIaafTypeDefStrongObjRef );

      IUnknownSP spIUnknown;
      spIUnknown = axStrongObjRef.GetObject( spIaafPropertyValue, IID_IAAFObject );

      IAAFObjectSP spIaafObject;
      AxQueryInterface( spIUnknown, spIaafObject );

      AxObject axObj( spIaafObject );
      std::auto_ptr< AxBaseObjIterPrtcl > iter(
	 new AxBaseSolitaryObjIter<AxObject>( spIaafObject ) );

      Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefSetSP& spIaafTypeDefSet )
   {
      AxTypeDefSet axTypeDefSet( spIaafTypeDefSet );

      AxPropertyValueIter axPropValIter( axTypeDefSet.GetElements( spIaafPropertyValue ) );

      std::auto_ptr< AxBaseObjIterPrtcl > iter( 
	 new AxBaseObjIter<AxPropertyValueIter, AxPropertyValue, IAAFPropertyValue>( axPropValIter ) );

      Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefVariableArraySP& spIaafTypeDefVariableArray )
   {
	AxTypeDefVariableArray axTypeDefVarArray( spIaafTypeDefVariableArray );

	AxPropertyValueIter axPropValIter( axTypeDefVarArray.GetElements( spIaafPropertyValue ) );

	std::auto_ptr< AxBaseObjIterPrtcl > iter( 
		new AxBaseObjIter<AxPropertyValueIter, AxPropertyValue, IAAFPropertyValue>( axPropValIter ) );

	Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefRecordSP& spIaafTypeDefRecord )
   {
      // Records are a bit weird, the are a set of named attribute/value
      // pairs, but are not IAAFObjects which are also a set of named
      // attribute/value (i.e. property/value) pairs.  In order to 
      // process the individual record fields using this framework,
      // a special iterator is required.

      std::auto_ptr<AxRecordIterator> axRecordIter(
	 new AxRecordIterator( spIaafPropertyValue, spIaafTypeDefRecord ) );

      std::auto_ptr<AxBaseObjIterPrtcl> iter(
	 new AxBaseRecordObjIter( axRecordIter ) );

      Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefFixedArraySP& spIaafTypeDefFixedArray )
   {
	AxTypeDefFixedArray axTypeDefFixedArray( spIaafTypeDefFixedArray );

	AxPropertyValueIter axPropValIter( axTypeDefFixedArray.GetElements( spIaafPropertyValue ) );

	std::auto_ptr< AxBaseObjIterPrtcl > iter( 
		new AxBaseObjIter<AxPropertyValueIter, AxPropertyValue, IAAFPropertyValue>( axPropValIter ) );

	Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefIndirectSP& spIaafTypeDefIndirect )
   {
      AxTypeDefIndirect axIndirect( spIaafTypeDefIndirect );

      AxPropertyValue axActualPropVal( axIndirect.GetActualValue(spIaafPropertyValue) );

      std::auto_ptr< AxBaseObjIterPrtcl > iter(
	 new AxBaseSolitaryObjIter<AxPropertyValue>( axActualPropVal ) );

      Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefOpaqueSP& spIaafTypeDefOpaque )
   {
      AxTypeDefOpaque axOpaque( spIaafTypeDefOpaque );
	
      AxPropertyValue axActualPropVal( axOpaque.GetActualValue( spIaafPropertyValue ) );

      std::auto_ptr< AxBaseObjIterPrtcl > iter(
	 new AxBaseSolitaryObjIter<AxPropertyValue>( axActualPropVal ) );

      Post( iter );
   }

   void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
				IAAFTypeDefRenameSP& spIaafTypeDefRename )
   {
      AxTypeDefRename axTypeDefRename( spIaafTypeDefRename );
      AxPropertyValue axBasePropVal( axTypeDefRename.GetBaseValue( spIaafPropertyValue ) );

      std::auto_ptr< AxBaseObjIterPrtcl > iter(
	 new AxBaseSolitaryObjIter<AxPropertyValue>( axBasePropVal ) );

      Post( iter );

   }

} // end of namespace

//=---------------------------------------------------------------------=


//---------------------------------------------------------------------------
AxBaseObjRecIterExt::AxBaseObjRecIterExt( std::auto_ptr< AxBaseObjIterPrtcl >  root )
   :       _root( root ),
	   _listener( 0 )
{
   AxBaseObjRecIterExt::Push( _root->Clone(),0 );
}


//---------------------------------------------------------------------------
AxBaseObjRecIterExt::AxBaseObjRecIterExt( std::auto_ptr< AxBaseObjIterPrtcl >  root,
					  AxBaseObjRecIterListener *listener )
   :	_root( root ),
	_listener( listener )
{
   AxBaseObjRecIterExt::Push( _root->Clone(), 0 );
}


//---------------------------------------------------------------------------
AxBaseObjRecIterExt::~AxBaseObjRecIterExt()
{
}


//---------------------------------------------------------------------------
inline void AxBaseObjRecIterExt::Push( std::auto_ptr< AxBaseObjIterPrtcl > iter,
				       int type )
{
   StackMember member( iter.release(), type );
   _deque.push_back( member );
}


//---------------------------------------------------------------------------
inline void AxBaseObjRecIterExt::Pop()
{
   if ( _listener != 0 )
   {
      StackMember pop = _deque.back();
      if ( pop._type == 0 )
      {
	 _listener->ObjectPop();
      }
      else if ( pop._type == 1 )
      {
	 _listener->PropertyPop();
      }
   }

   delete (_deque.back())._axBaseObjIterPrtcl;
   _deque.pop_back();
}


//---------------------------------------------------------------------------
bool AxBaseObjRecIterExt::NextOne( std::auto_ptr<AxBaseObj>& objRet, int& level )
{
   if ( Empty() ) {
      return false;
   }

   AxBaseObjIterPrtcl& iter = Top();

   bool rc = iter.NextOne( objRet );

   if ( !rc ) {
      Pop();
      return NextOne( objRet, level );
   }

   level = GetLevel();

   // Peek at the pointer, use it, but don't take ownership!
   try {
      if ( dynamic_cast< AxObject* >( objRet.get() ) ) {

	 AxObject& obj = dynamic_cast< AxObject& >( *objRet.get() );

	 HandleObjectRecursion( obj );
      }
      else if ( dynamic_cast< AxProperty* >( objRet.get() ) ) {

	 AxProperty& prop = dynamic_cast< AxProperty& >( *objRet.get() );

	 HandlePropertyRecursion( prop );
      }
      else if ( dynamic_cast< AxPropertyValue* >( objRet.get() ) ) {

	 AxPropertyValue& propVal = dynamic_cast< AxPropertyValue& >( *objRet.get() );

	 HandlePropertyValueRecursion( propVal );
      }
      else if ( dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( objRet.get() ) ) {

	 AxBaseObjAny<AxRecordIterator::Pair>& recPair =
	    dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>& >( *objRet.get() );

	 HandleRecordPropertyValueRecursion(	recPair.get() );

      }
   }
   catch ( const AxExHResult& ex ) {
      std::auto_ptr< AxBaseObjIterPrtcl > iter(
	 new AxBaseSolitaryObjIter<AxBaseObjAny<AxExHResult> >(
	    AxBaseObjAny<AxExHResult>( ex ) ) );
      Push( iter, -1 );
   }

   return true;
}


//---------------------------------------------------------------------------
void AxBaseObjRecIterExt::PopStack()
{
   Pop();
}


//---------------------------------------------------------------------------
int AxBaseObjRecIterExt::GetLevel()
{
   return Size();
}


//---------------------------------------------------------------------------
inline AxBaseObjIterPrtcl& AxBaseObjRecIterExt::Top()
{
   return *_deque.back()._axBaseObjIterPrtcl;
}


//---------------------------------------------------------------------------
inline bool AxBaseObjRecIterExt::Empty()
{
   return _deque.empty();
}


//---------------------------------------------------------------------------
inline int AxBaseObjRecIterExt::Size()
{
   return _deque.size();
}

//---------------------------------------------------------------------------
void AxBaseObjRecIterExt::HandleObjectRecursion( AxObject& obj )
{
   std::auto_ptr< AxBaseObjIterPrtcl > iter(
      new AxBaseObjIter<AxPropertyIter, AxProperty, IAAFProperty>( obj.GetProperties() ) );

   Push( iter,0 );
}


//---------------------------------------------------------------------------
void AxBaseObjRecIterExt::HandlePropertyRecursion( AxProperty& prop )
{
   AxPropertyValue propVal( prop.GetValue() );

   std::auto_ptr< AxBaseObjIterPrtcl > iter(
      new AxBaseSolitaryObjIter<AxPropertyValue>( propVal ) );

   Push( iter,1 );
}


//---------------------------------------------------------------------------
void AxBaseObjRecIterExt::HandlePropertyValueRecursion( AxPropertyValue& propVal )
{
   PropValToIter valuePrtcl;

   propVal.Process( valuePrtcl );

   if ( valuePrtcl.ResultExists() ) {
      Push( valuePrtcl.GetResult(),-1 );
   }
}


//---------------------------------------------------------------------------
void AxBaseObjRecIterExt::HandleRecordPropertyValueRecursion( AxRecordIterator::Pair& recPair )
{
   std::auto_ptr< AxBaseObjIterPrtcl > iter(
      new AxBaseSolitaryObjIter<AxPropertyValue>( AxPropertyValue(recPair.second) ) );

   Push( iter,-1 );
}


