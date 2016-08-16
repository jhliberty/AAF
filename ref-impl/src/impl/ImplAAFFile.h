#ifndef __ImplAAFFile_h__
#define __ImplAAFFile_h__
//=---------------------------------------------------------------------=
//
// $Id: ImplAAFFile.h,v 1.49 2009/06/01 11:47:05 stuart_hc Exp $ $Name: V116 $
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

#include "ImplAAFRoot.h"


//
// Forward declaration
//
class OMFile;
class OMRawStorage;
class ImplAAFDictionary;
class ImplAAFMetaDictionary;
class ImplAAFFile;
class ImplAAFRawStorage;
class ImplAAFHeader;
class ImplAAFDataDef;
class ImplAAFOMRawStorage;

struct IAAFRawStorage;
struct IAAFRandomRawStorage;

class ImplAAFFile : public ImplAAFRoot
{
public:

  //
  // methods to support API
  //

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Open ();

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenExistingRead (const aafCharacter * pFileName,
					  aafUInt32 modeFlags);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenExistingModify (const aafCharacter * pFileName,
						aafUInt32 modeFlags,
						aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenNewModify (const aafCharacter * pFileName,
				   aafUID_constptr pFileKind,
				   aafUInt32 modeFlags,
				   aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenNewModify (const aafCharacter * pFileName,
				   aafUInt32 modeFlags,
				   aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	OpenTransient (aafProductIdentification_t * pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
    CreateAAFFileOnRawStorage (IAAFRawStorage * pRawStorage,
							   aafFileExistence_t existence,
							   aafFileAccess_t access,
							   aafUID_constptr pFileKind,
							   aafUInt32 modeFlags,
							   aafProductIdentification_constptr pIdent);

  virtual AAFRESULT STDMETHODCALLTYPE
	Close ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Save ();

  virtual AAFRESULT STDMETHODCALLTYPE
	SaveCopyAs (ImplAAFFile * pDestFile);

  virtual AAFRESULT STDMETHODCALLTYPE
	Revert ();

  virtual AAFRESULT STDMETHODCALLTYPE
	GetHeader (ImplAAFHeader ** header);

  virtual AAFRESULT STDMETHODCALLTYPE
	GetRevision (aafFileRev_t *  rev);
  
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDictionary
        (ImplAAFDictionary ** ppDictionary) const;  //@parm [out,retval] The AAF Dictionary

  static void registerFactories(void);
  static void registerStructuredStorageFactories(void);

  static void removeFactories(void);

  //
  // Constructor/destructor
  //
  //********
  ImplAAFFile ();
  virtual ~ImplAAFFile ();

protected:

  // Returns the OMFile associated with this AAFFile.  Requires
  // IsOpen().
  OMFile * omFile (void);

  bool IsReadable () const;
  bool IsWriteable () const;
  bool IsOpen () const;
  bool IsClosed () const;
  OMRawStorage * RawStorage ();

private:

  void InternalReleaseObjects();

  AAFRESULT pvtCreateExistingRead
    ();
  AAFRESULT pvtCreateExistingModify
    (aafProductIdentification_constptr pIdent);
  AAFRESULT pvtCreateNewModify
    (aafUID_constptr pFileKind,
	 aafProductIdentification_constptr pIdent);

  // In order to avoid storage specific APIs in the public AAF interface
  // some storage specific data is mirrored in the AAF metadata. Clients
  // of the public API set and get values for this metadata. The following
  // functions ensure that the data are synchronized.

  void saveMirroredMetadata(void);

  void restoreMirroredMetadata(void);

  aafInt32			_cookie;
  OMFile			*_file;
  ImplAAFDictionary *_factory;
  ImplAAFMetaDictionary *_metafactory;
  aafInt16			_byteOrder;
  ImplAAFHeader *   _head;		// Needed by Head object
  aafBool   _semanticCheckEnable;	//!!!  /* Used to stop recursion in checks */
  aafFileRev_t   _setrev;
  aafBool _initialized;
  aafUInt32 _modeFlags;

  aafFileExistence_t _existence;
  aafFileAccess_t    _access;

  aafProductIdentification_t _preOpenIdent;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFFile> ImplAAFFileSP;

#endif // ! __ImplAAFFile_h__
