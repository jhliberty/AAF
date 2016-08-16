//@doc
//@class    AAFDataDef | Implementation class for AAFDataDef
#ifndef __CAAFDataDef_h__
#define __CAAFDataDef_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFDataDef.h,v 1.15 2012/06/26 20:54:41 jptrainor Exp $ $Name: V116 $
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
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif






#ifndef __CAAFDefObject_h__
#include "CAAFDefObject.h"
#endif

//
// Forward declaration
//
class ImplAAFDataDef;


class CAAFDataDef
  : public IAAFDataDef,
    public IAAFDataDef2,
	public IAAFDataDef3,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDataDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDataDef ();

public:

  //***********************************************************
  //
  // Initialize()
  //
  // Init all fields of a definition object.
  //
  STDMETHOD (Initialize) (
    // AUID for new DefObject
    /*[in, ref]*/ aafUID_constref  id,

    // Name for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pName,

    // Description for new DefObject
    /*[in, string]*/ aafCharacter_constptr  pDescription);


  //***********************************************************
  //
  // IsPictureKind()
  //
  // Sets return value to TRUE if DataDef is a picture.
  //
  STDMETHOD (IsPictureKind) (
    // pointer to the return value 
    /*[retval,out]*/ aafBoolean_t *  bIsPictureKind);


  //***********************************************************
  //
  // IsMatteKind()
  //
  // Sets return value to TRUE if DataDef is a matte.
  //
  STDMETHOD (IsMatteKind) (
    // pointer to the return value 
    /*[retval,out]*/ aafBoolean_t *  bIsMatteKind);


  //***********************************************************
  //
  // IsPictureWithMatteKind()
  //
  // Sets return value to TRUE if DataDef is a picture with matte.
  //
  STDMETHOD (IsPictureWithMatteKind) (
    // pointer to the return value 
    /*[retval,out]*/ aafBoolean_t *  bIsPictureWithMatteKind);


  //***********************************************************
  //
  // IsSoundKind()
  //
  // Sets return value to TRUE if DataDef is a sound.
  //
  STDMETHOD (IsSoundKind) (
    // pointer to the return value 
    /*[retval,out]*/ aafBoolean_t *  bIsSoundKind);


  //***********************************************************
  //
  // DoesDataDefConvertTo()
  //
  // Sets return value to TRUE if the DataDef of the given object
  /// can be converted to the DataDef specified in the IN 
  /// parameter with the DataDefName string.
  //
  STDMETHOD (DoesDataDefConvertTo) (
    // data def to compare against
    /*[in]*/ IAAFDataDef * id,

    // pointer to result
    /*[retval, out]*/ aafBoolean_t *  bDoesConvertTo);
		   

  //***********************************************************
  //
  // IsDataDefOf()
  //
  // Sets the value to TRUE if the DataDef of the given object
  /// matches the DataDef specified in the IN parameter with the
  /// DataDefName string.
  //
  STDMETHOD (IsDataDefOf) (
    // data def to compare against
    /*[in]*/ IAAFDataDef * pDataDef,

    // pointer to result
    /*[retval, out]*/ aafBoolean_t *  bIsDataDefOf);


  //***********************************************************
  //
  // DoesDataDefConvertFrom()
  //
  // Sets return value to TRUE if the DataDef of the given object
  /// can be converted from the DataDef specified in the IN 
  /// parameter specified with the DataDefName string.
  //
  STDMETHOD (DoesDataDefConvertFrom) (
    // data def to compare against
    /*[in]*/ IAAFDataDef * pDataDef,

    // pointer to result
    /*[retval, out]*/ aafBoolean_t *  bDoesConvertFrom);

  //***********************************************************
  // METHOD NAME: IsEdgecodeKind()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDataDef2 | IsEdgecodeKind |
  // Sets return value to TRUE if DataDef is an edgecode.
  // @end
  // 
  STDMETHOD (IsEdgecodeKind)
   (
    // @parm [retval,out] aafBoolean_t * | bIsEdgecodeKind | pointer to the return value
    aafBoolean_t *  bIsEdgecodeKind
  );

  //***********************************************************
  // METHOD NAME: IsTimecodeKind()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDataDef2 | IsTimecodeKind |
  // Sets return value to TRUE if DataDef is a timecode.
  // @end
  // 
  STDMETHOD (IsTimecodeKind)
   (
    // @parm [retval,out] aafBoolean_t * | bIsTimecodeKind | pointer to the return value
    aafBoolean_t *  bIsTimecodeKind
  );


  //***********************************************************
  // METHOD NAME: IsDescriptiveMetadataKind()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDataDef3 | IsDescriptiveMetadataKind |
  // Sets return value to TRUE if DataDef is descriptive metadata.
  // @end
  // 
  STDMETHOD (IsDescriptiveMetadataKind)
   (
    // @parm [retval,out] aafBoolean_t * | bIsDescriptiveMetadataKind | pointer to the return value
    aafBoolean_t *  bIsDescriptiveMetadataKind
  );

  //***********************************************************
  // METHOD NAME: IsAuxiliaryKind()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDataDef3 | IsAuxiliaryKind |
  // Sets return value to TRUE if DataDef is auxiliary.
  // @end
  // 
  STDMETHOD (IsAuxiliaryKind)
   (
    // @parm [retval,out] aafBoolean_t * | bIsAuxiliaryKind | pointer to the return value
    aafBoolean_t *  bIsAuxiliaryKind
  );



protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFDataDef_h__

