//=---------------------------------------------------------------------=
//
// $Id: TestVarArray.cpp,v 1.4 2009/06/01 11:47:15 stuart_hc Exp $ $Name: V116 $
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
// British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "XMLStoredFormatTest.h"


static bool testRestore(const wchar_t* fileName)
{
    bool passed = true;
    
    IAAFFile* pFile = 0;
    IAAFHeader* pHeader = 0;
    IAAFDictionary* pDictionary = 0;
    IAAFContentStorage* pStorage = 0;
    IEnumAAFMobs* pMobs = 0;
    IAAFMob* pMob = 0;
    
    try
    {
        pFile = openFileForReading(fileName);
    }
    catch (...)
    {
        return false;
    }
    
    try
    {
        // get the Mob containing the test data
        checkResult(pFile->GetHeader(&pHeader));
        checkResult(pHeader->GetDictionary(&pDictionary));
        checkResult(pHeader->GetContentStorage(&pStorage));
        aafSearchCrit_t searchCrit;
        searchCrit.searchTag = kAAFByMobKind;
        searchCrit.tags.mobKind = kAAFAllMob;
        checkResult(pStorage->GetMobs(&searchCrit, &pMobs));
        checkResult(pMobs->NextOne(&pMob));
        
        IAAFObject* pObject = 0;
        IAAFClassDef* pClassDef = 0;
        IAAFPropertyDef* pPropertyDef = 0;
        IAAFPropertyValue* pPropertyValue = 0;
        IAAFTypeDef* pType = 0;
        IAAFTypeDefVariableArray* pVarArrayType = 0;
        IAAFTypeDefInt* pIntType = 0;
        IAAFTypeDefCharacter* pCharacterType = 0;
        IAAFPropertyValue* pElementValue = 0;

        try
        {
            printf("    * Baseline element type: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUInt8 testValue[2] = {0,255};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            aafUInt8 value[2];

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pElementValue));
            checkResult(pIntType->GetInteger(pElementValue, &value[0], 1));

            release(pElementValue);
            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pElementValue));
            checkResult(pIntType->GetInteger(pElementValue, &value[1], 1));

            if (value[0] == testValue[0] && value[1] == testValue[1])
            {
                printf("passed\n");
            }
            else
            {
                printf("FAILED\n");
                passed = false;
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pElementValue);
        release(pCharacterType);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Non-Baseline element type: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUInt8 testValue[2] = {1,2};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            aafUInt8 value[2];

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pElementValue));
            checkResult(pIntType->GetInteger(pElementValue, &value[0], 1));

            release(pElementValue);
            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pElementValue));
            checkResult(pIntType->GetInteger(pElementValue, &value[1], 1));

            if (value[0] == testValue[0] && value[1] == testValue[1])
            {
                printf("passed\n");
            }
            else
            {
                printf("FAILED\n");
                passed = false;
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pElementValue);
        release(pCharacterType);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Empty: ");

            const aafUID_t propId = 
                {0x20000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            HRESULT hr = pObject->GetPropertyValue(pPropertyDef, &pPropertyValue);
            if (hr == AAFRESULT_PROP_NOT_PRESENT)
            {
                // Note: A DataValue is represented by a OMArrayProperty (OMDataVector)
                // and when cloning the destination will get no elements appended and
                // therefore isPresent() will be false
                printf("passed (property not present)\n");
            }
            else
            {
                checkResult(hr);
                
                checkResult(pPropertyValue->GetType(&pType));
                checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
                release(pType);
                
                checkResult(pVarArrayType->GetType(&pType));
                checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));
    
                aafUInt32 count;
                checkResult(pVarArrayType->GetCount(pPropertyValue, &count));
    
                if (count == 0)
                {
                    printf("passed\n");
                }
                else
                {
                    printf("FAILED\n");
                    passed = false;
                }
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pElementValue);
        release(pCharacterType);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * StringArray: ");

            const aafUID_t propId = 
                {0x30000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            const aafCharacter* testValue = L"string 1\0string 2";
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&pCharacterType));

            aafCharacter value[256];

            for (size_t i = 0; i < 18; i++)
            {
                checkResult(pVarArrayType->GetElementValue(pPropertyValue, i, &pElementValue));
                checkResult(pCharacterType->GetCharacter(pElementValue, &value[i]));
                release(pElementValue);
            }

            if (memcmp(value, testValue, 18 * sizeof(aafCharacter)) == 0)
            {
                printf("passed\n");
            }
            else
            {
                printf("FAILED\n");
                passed = false;
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pElementValue);
        release(pCharacterType);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * DataValue: ");

            const aafUID_t propId = 
                {0x40000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUInt8 testValue[12] = {1,2,3,4,5,6,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pCharacterType));

            aafUInt8 value[256];

            checkResult(pVarArrayType->GetCArray(pPropertyValue, value, 256));

            if (memcmp(value, testValue, sizeof(testValue)) == 0)
            {
                printf("passed\n");
            }
            else
            {
                printf("FAILED\n");
                passed = false;
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pElementValue);
        release(pCharacterType);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

    }
    catch (...)
    {
        passed = false;
    }

    // cleanup    
    release(pMob);
    release(pMobs);
    release(pStorage);
    release(pDictionary);
    release(pHeader);
    checkResult(pFile->Close());
    release(pFile);

    report(passed);
    
    return passed;
}


int testVarArray()
{
    printf("Rule 5.14: VariableArray\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testVarArray.xml") && passed;
        
        printf("b) Roundtrip\n");
        saveCopy(L"input/testVarArray.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testVarArray.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;
    }
    catch (...)
    {
        passed = false;
        printf("FAILED: Exception thrown\n");
    }

    printf("\n");
    
    return passed ? 0 : 2;
}


