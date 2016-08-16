//=---------------------------------------------------------------------=
//
// $Id: CAAFEventMobSlotTest.cpp,v 1.35 2009/06/01 11:47:12 stuart_hc Exp $ $Name: V116 $
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

#include "AAF.h"


#include <iostream>
using namespace std;
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}

static const aafUID_t kAAFDataDef_Test = 
{ 0x81831639, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

class EventMobSlotTest
{
public:
  EventMobSlotTest();
  ~EventMobSlotTest();

  void Create(wchar_t *pFileName,
              aafUID_constref fileKind,
              testRawStorageType_t rawStorageType,
              aafProductIdentification_constref productID);
  void Open(wchar_t *pFileName);
  void Close();

  IAAFEvent *CreateAnEvent(aafPosition_t* position, 
                           const wchar_t *comment, 
                           IAAFSequence *pSequence = NULL);

  void CreateEventMobSlot();
  void OpenEventMobSlot();

  void CreateEventSequenceMobSlot();
  void OpenEventSequenceMobSlot();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;

  // EventMobSlot static data
  static const aafRational_t _editRate;
  static const aafPosition_t _eventSlotOrigin;
  // Event static data
  static const aafPosition_t _position;
  static const wchar_t* _slotName;
};

//{060c2b340205110101001000-13-00-00-00-{f827fde6-8d6f-11d4-a380-009027dfca6a}}

static const aafMobID_t gEventMobID1 = {

{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 

0x13, 0x00, 0x00, 0x00, 

{0xf827fde6, 0x8d6f, 0x11d4, {0xa3, 0x80, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}}};

//{060c2b340205110101001000-13-00-00-00-{f8d86bea-8d6f-11d4-a380-009027dfca6a}}

static const aafMobID_t gEventMobID2 = {

{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 

0x13, 0x00, 0x00, 0x00, 

{0xf8d86bea, 0x8d6f, 0x11d4, {0xa3, 0x80, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a}}};


extern "C" HRESULT CAAFEventMobSlot_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFEventMobSlot_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = S_OK;
  const size_t fileNameBufLen = 128;
  aafWChar pFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

  // Create an instance of our text clip test class and run the
  // tests...
  EventMobSlotTest test;

  try
  {
    // Attempt to create a test file
	if(mode == kAAFUnitTestReadWrite)
  	  test.Create(pFileName, fileKind, rawStorageType, productID);

    // Attempt to read the test file.
    test.Open(pFileName);
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
  }
  catch (...)
  {
    cerr << "CAAFEventMobSlot_test..."
		 << "Caught general C++ exception!" << endl;
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}

// Test edit rate for the event mob slot.
const aafRational_t EventMobSlotTest::_editRate = { 2997, 100 };
const aafPosition_t EventMobSlotTest::_eventSlotOrigin = 0;
const aafPosition_t EventMobSlotTest::_position = 0;
const wchar_t *_eventComment = L"Event::Comment:This is a test event";

EventMobSlotTest::EventMobSlotTest() :
  _pFile(NULL),
  _bWritableFile(false),
  _pHeader(NULL),
  _pDictionary(NULL)
{
}

EventMobSlotTest::~EventMobSlotTest()
{
  Close();
}


void EventMobSlotTest::Create(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)

{
  assert(NULL == _pFile);

  // Remove the previous test file if any.
  RemoveTestFile(pFileName);
    
  // Create the file
  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &_pFile ));
  _bWritableFile = true;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  checkResult(_pHeader->GetDictionary(&_pDictionary));

  // Make a simple event mob slot
  CreateEventMobSlot();

  // Make a more complex mob slot.
  CreateEventSequenceMobSlot();

  // cleanup
  Close();
}


void EventMobSlotTest::Open(wchar_t *pFileName)
{
  assert(NULL == _pFile);
    
  // Create the file
  checkResult(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bWritableFile = false;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Open and validate the text clip.
  OpenEventMobSlot();

  // Open and validate a mob slot with a sequence of events.
  OpenEventSequenceMobSlot();

  // cleanup
  Close();
}


void EventMobSlotTest::Close()
{
  if (_pDictionary)
  {
    _pDictionary->Release();
    _pDictionary = NULL;
  }

  if (_pHeader)
  {
    _pHeader->Release();
    _pHeader = NULL;
  }
  
  if (_pFile)
  {
    if (_bWritableFile)
      checkResult(_pFile->Save());

    checkResult(_pFile->Close());

    _pFile->Release();
    _pFile = NULL;
  }  
}


IAAFEvent *EventMobSlotTest::CreateAnEvent(aafPosition_t* position,
                                           const wchar_t *comment,
                                           IAAFSequence *pSequence)
{
  assert(_pHeader && _pDictionary);
  assert(position && comment);

  IAAFEvent *pEvent = NULL;
  IAAFComponent *pComponent = NULL;
  IAAFDataDef *pDataDef = NULL;
  IAAFComponent *pComp = NULL;

  CAAFBuiltinDefs defs (_pDictionary);

  try
  {
	  // not already in dictionary
		checkResult(defs.cdDataDef()->
					CreateInstance (IID_IAAFDataDef,
									(IUnknown **)&pDataDef));
	  pDataDef->Initialize (kAAFDataDef_Test, L"Test", L"Test data");
	  _pDictionary->RegisterDataDef (pDataDef);
	// Create a concrete subclass of event
    checkResult(defs.cdCommentMarker()->
				CreateInstance(IID_IAAFEvent, 
							   (IUnknown **)&pEvent));
    checkResult(pEvent->SetPosition(*position));
    checkResult(pEvent->SetComment(comment));
	checkResult(pEvent->QueryInterface(IID_IAAFComponent, (void **)&pComp));
	checkResult(pComp->SetDataDef(defs.ddkAAFPicture()));
	pComp->Release();
	pComp = NULL;
	pDataDef->Release();
	pDataDef = NULL;

    if (NULL != pSequence)
    {
      // Get the segment inteface to add to the mob slot
      checkResult(pEvent->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
	  CAAFBuiltinDefs defs (_pDictionary);
	  checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));

      // Add the event to the sequence.
      checkResult(pSequence->AppendComponent(pComponent));
      pComponent->Release();
      pComponent = NULL;      
    }
  }
  catch (...)
  {
    if (pComponent)
    {
      pComponent->Release();
      pComponent = NULL;
    }

    if (pEvent)
    {
      pEvent->Release();
      pEvent = NULL;
    }

    if (pDataDef)
    {
      pDataDef->Release();
      pDataDef = NULL;
    }
    
    throw;
  }
  
  return pEvent;
}

void EventMobSlotTest::CreateEventMobSlot()
{
  assert(_pHeader && _pDictionary);

  aafPosition_t position;
  IAAFEvent *pEvent = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFEventMobSlot2 *pEventMobSlot2 = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFMob *pMob = NULL;

  CAAFBuiltinDefs defs (_pDictionary);

  try
  {
    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 0;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event");

    // Get the segment inteface to add to the mob slot
    checkResult(pEvent->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
    pEvent->Release();
    pEvent = NULL;

    // Create and initialize an EventMobSlot
    checkResult(defs.cdEventMobSlot()->
				CreateInstance(IID_IAAFEventMobSlot, 
							   (IUnknown **)&pEventMobSlot));
    checkResult(pEventMobSlot->SetEditRate(const_cast<aafRational_t *>(&_editRate)));

    // Check MobSlot2 functionality
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFEventMobSlot2, (void **)&pEventMobSlot2));
    checkResult(pEventMobSlot2->SetEventSlotOrigin(_eventSlotOrigin));
    pEventMobSlot2->Release();
    pEventMobSlot2 = NULL;

    // Get the mob slot interface so that we can add the event segment.
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));
    pEventMobSlot->Release();
    pEventMobSlot = NULL;

    // Add the event segment to the event mob slot.
    checkResult(pMobSlot->SetSegment(pSegment));
    pSegment->Release();
    pSegment = NULL;

    // Create the mob to hold the new event mob slot.
    checkResult(defs.cdCompositionMob()->
				CreateInstance(IID_IAAFMob, 
							   (IUnknown **)&pMob));
    checkResult(pMob->SetName(L"CompositionMob::Name:Test mob to hold an event mob slot"));

    // Append event slot to the composition mob.
    checkResult(pMob->AppendSlot(pMobSlot));
    pMobSlot->Release();
    pMobSlot = NULL;

    // Save the id of the composition mob that contains our test
    // event mob slot.
    checkResult(pMob->SetMobID(gEventMobID1));

    // Attach the mob to the header...
    checkResult(_pHeader->AddMob(pMob));

    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
    // Cleanup local references
    if (pMob)
    {
      pMob->Release();
      pMob = NULL;
    }

    if (pMobSlot)
    {
      pMobSlot->Release();
      pMobSlot = NULL;
    }

    if (pEventMobSlot)
    {
      pEventMobSlot->Release();
      pEventMobSlot = NULL;
    }

    if (pEventMobSlot2)
    {
      pEventMobSlot2->Release();
      pEventMobSlot2 = NULL;
    }

    if (pSegment)
    {
      pSegment->Release();
      pSegment = NULL;
    }

    if (pEvent)
    {
      pEvent->Release();
      pEvent = NULL;
    }
 
    // Propogate the error.
    throw;
  }
}


void EventMobSlotTest::OpenEventMobSlot()
{
  assert(_pHeader);

  IAAFMob *pMob = NULL;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFEventMobSlot2 *pEventMobSlot2 = NULL;
  aafRational_t editRate = {0};
  aafPosition_t eventSlotOrigin;
  IAAFSegment *pSegment = NULL;
  IAAFEvent *pEvent = NULL;


  try
  {
    // Get the composition mob that we created to hold the
    checkResult(_pHeader->LookupMob(gEventMobID1, &pMob));

    // Get the first mob slot and check that it is an event mob slot.
    checkResult(pMob->GetSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->QueryInterface(IID_IAAFEventMobSlot, (void **)&pEventMobSlot));
    checkResult(pEventMobSlot->GetEditRate(&editRate));
    checkExpression(0 == memcmp(&editRate, &_editRate, sizeof(editRate)), AAFRESULT_TEST_FAILED);
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFEventMobSlot2, (void **)&pEventMobSlot2));
    checkResult(pEventMobSlot2->GetEventSlotOrigin(&eventSlotOrigin));
    checkExpression(0 == memcmp(&eventSlotOrigin, &_eventSlotOrigin, sizeof(eventSlotOrigin)), AAFRESULT_TEST_FAILED);

    // Get the event slot's segment and check that it is an event.
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFEvent, (void **)&pEvent));

    pEvent->Release();
    pEvent = NULL;
    pSegment->Release();
    pSegment = NULL;
    pEventMobSlot->Release();
    pEventMobSlot = NULL;
    pEventMobSlot2->Release();
    pEventMobSlot2 = NULL;
    pMobSlot->Release();
    pMobSlot = NULL;
    pEnumSlots->Release();
    pEnumSlots = NULL;
    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
    // Cleanup local references
    if (pEvent)
    {
      pEvent->Release();
      pEvent = NULL;
    }

    if (pSegment)
    {
      pSegment->Release();
      pSegment = NULL;
    }

    if (pEventMobSlot)
    {
      pEventMobSlot->Release();
      pEventMobSlot = NULL;
    }

    if (pEventMobSlot2)
    {
      pEventMobSlot2->Release();
      pEventMobSlot2 = NULL;
    }

    if (pMobSlot)
    {
      pMobSlot->Release();
      pMobSlot = NULL;
    }

    if (pEnumSlots)
    {
      pEnumSlots->Release();
      pEnumSlots = NULL;
    }

    if (pMob)
    {
      pMob->Release();
      pMob = NULL;
    }
 
    // Propogate the error.
    throw; 
  }
}


void EventMobSlotTest::CreateEventSequenceMobSlot()
{
  assert(_pHeader && _pDictionary);

  aafPosition_t position;
  IAAFSequence *pSequence = NULL;
  IAAFEvent *pEvent = NULL;
  IAAFComponent *pComponent = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFEventMobSlot2 *pEventMobSlot2 = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFMob *pMob = NULL;

  try
  {
	CAAFBuiltinDefs defs (_pDictionary);

    // Create a sequence to hold our list of events.
    checkResult(defs.cdSequence()->
				CreateInstance(IID_IAAFSequence, 
							   (IUnknown **)&pSequence));
     checkResult(pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent));

	 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
	 pComponent->Release();
	 pComponent = NULL;


    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 0;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event 0", pSequence);
    pEvent->Release();
    pEvent = NULL;


    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 1;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event 1", pSequence);
    pEvent->Release();
    pEvent = NULL;


    // Create an event (note: this will be replaced by a concrete event in a
    // later version after such an event is implemented.)
    position = 1;
    pEvent = CreateAnEvent(&position, L"Event::Comment:This is a test event 2", pSequence);
    pEvent->Release();
    pEvent = NULL;


    // Get the segment inteface to add to the mob slot
    checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
    pSequence->Release();
    pSequence = NULL;

    // Create and initialize an EventMobSlot
    checkResult(defs.cdEventMobSlot()->
				CreateInstance(IID_IAAFEventMobSlot, 
							   (IUnknown **)&pEventMobSlot));
    checkResult(pEventMobSlot->SetEditRate(const_cast<aafRational_t *>(&_editRate)));
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFEventMobSlot2, (void **)&pEventMobSlot2));
    checkResult(pEventMobSlot2->SetEventSlotOrigin(_eventSlotOrigin));
    pEventMobSlot2->Release();
    pEventMobSlot2 = NULL;

    // Get the mob slot interface so that we can add the event segment.
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));
    pEventMobSlot->Release();
    pEventMobSlot = NULL;

    // Add the event segment to the event mob slot.
    checkResult(pMobSlot->SetSegment(pSegment));
    pSegment->Release();
    pSegment = NULL;

    // Create the mob to hold the new event mob slot.
    checkResult(defs.cdCompositionMob()->
				CreateInstance(IID_IAAFMob, 
							   (IUnknown **)&pMob));
    checkResult(pMob->SetName(L"CompositionMob::Name:Test mob to hold an event mob slot"));

    // Append event slot to the composition mob.
    checkResult(pMob->AppendSlot(pMobSlot));
    pMobSlot->Release();
    pMobSlot = NULL;

    // Save the id of the composition mob that contains our test
    // event mob slot.
    checkResult(pMob->SetMobID(gEventMobID2));

    // Attach the mob to the header...
    checkResult(_pHeader->AddMob(pMob));

    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
    // Cleanup local references
    if (pMob)
    {
      pMob->Release();
      pMob = NULL;
    }

    if (pMobSlot)
    {
      pMobSlot->Release();
      pMobSlot = NULL;
    }

    if (pEventMobSlot)
    {
      pEventMobSlot->Release();
      pEventMobSlot = NULL;
    }

    if (pEventMobSlot2)
    {
      pEventMobSlot2->Release();
      pEventMobSlot2 = NULL;
    }

    if (pSegment)
    {
      pSegment->Release();
      pSegment = NULL;
    }

    if (pComponent)
    {
      pComponent->Release();
      pComponent = NULL;
    }

    if (pEvent)
    {
      pEvent->Release();
      pEvent = NULL;
    }

    if (pSequence)
    {
      pSequence->Release();
      pSequence = NULL;
    }
 
    // Propogate the error.
    throw;
  }
}


void EventMobSlotTest::OpenEventSequenceMobSlot()
{
  assert(_pHeader);

  IAAFMob *pMob = NULL;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFEventMobSlot *pEventMobSlot = NULL;
  IAAFEventMobSlot2 *pEventMobSlot2 = NULL;
  aafRational_t editRate = {0};
  aafPosition_t eventSlotOrigin;
  IAAFSegment *pSegment = NULL;
  IAAFSequence *pSequence = NULL;


  try
  {
    // Get the composition mob that we created to hold the
    checkResult(_pHeader->LookupMob(gEventMobID2, &pMob));

    // Get the first mob slot and check that it is an event mob slot.
    checkResult(pMob->GetSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->QueryInterface(IID_IAAFEventMobSlot, (void **)&pEventMobSlot));
    checkResult(pEventMobSlot->GetEditRate(&editRate));
    checkExpression(0 == memcmp(&editRate, &_editRate, sizeof(editRate)), AAFRESULT_TEST_FAILED);
    checkResult(pEventMobSlot->QueryInterface(IID_IAAFEventMobSlot2, (void **)&pEventMobSlot2));
    checkResult(pEventMobSlot2->GetEventSlotOrigin(&eventSlotOrigin));
    checkExpression(0 == memcmp(&eventSlotOrigin, &_eventSlotOrigin, sizeof(eventSlotOrigin)), AAFRESULT_TEST_FAILED);

    // Get the event slot's segment and check that it is an event.
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **)&pSequence));


    pSequence->Release();
    pSequence = NULL;
    pSegment->Release();
    pSegment = NULL;
    pEventMobSlot->Release();
    pEventMobSlot = NULL;
    pEventMobSlot2->Release();
    pEventMobSlot2 = NULL;
    pMobSlot->Release();
    pMobSlot = NULL;
    pEnumSlots->Release();
    pEnumSlots = NULL;
    pMob->Release();
    pMob = NULL;
  }
  catch (...)
  {
    // Cleanup local references
    if (pSequence)
    {
      pSequence->Release();
      pSequence = NULL;
    }

    if (pSegment)
    {
      pSegment->Release();
      pSegment = NULL;
    }

    if (pEventMobSlot)
    {
      pEventMobSlot->Release();
      pEventMobSlot = NULL;
    }

    if (pEventMobSlot2)
    {
      pEventMobSlot2->Release();
      pEventMobSlot2 = NULL;
    }

    if (pMobSlot)
    {
      pMobSlot->Release();
      pMobSlot = NULL;
    }

    if (pEnumSlots)
    {
      pEnumSlots->Release();
      pEnumSlots = NULL;
    }

    if (pMob)
    {
      pMob->Release();
      pMob = NULL;
    }
 
    // Propogate the error.
    throw; 
  }
}






