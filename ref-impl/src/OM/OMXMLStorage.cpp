//=---------------------------------------------------------------------=
//
// $Id: OMXMLStorage.cpp,v 1.3 2009/06/01 11:47:02 stuart_hc Exp $ $Name: V116 $
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

#include "OMXMLStorage.h"
#include "OMSetIterator.h"
#include "OMListIterator.h"
#include "OMDiskRawStorage.h"
#include "OMXMLUtilities.h"
#include "OMUtilities.h"
#include "OMExceptions.h"
#include "OMAssertions.h"



OMMetaDefIdMap::OMMetaDefIdMap()
{
    TRACE("OMMetaDefIdMap::OMMetaDefIdMap");
}

OMMetaDefIdMap::~OMMetaDefIdMap()
{
    TRACE("OMMetaDefIdMap::~OMMetaDefIdMap");
}
    
void 
OMMetaDefIdMap::add(OMUniqueObjectIdentification id, const wchar_t* qSymbol)
{
    TRACE("OMMetaDefIdMap::add(OMUniqueObjectIdentification, wchar_t)");
    
    if (_idToQSymbol.contains(id) || _qSymbolToId.contains(qSymbol))
    {
        throw OMException("MetaDef Id to Symbol map already contains element");
    }
    
    _idToQSymbol.insert(id, qSymbol);
    _qSymbolToId.insert(qSymbol, id);
}

void 
OMMetaDefIdMap::add(OMUniqueObjectIdentification id, const wchar_t* symbolspace,
    const wchar_t* symbol)
{
    TRACE("OMMetaDefIdMap::add(OMUniqueObjectIdentification, wchar_t,wchar_t)");

    wchar_t* qSymbol = new wchar_t[wcslen(symbolspace) + 1 + wcslen(symbol) + 1];
    wcscpy(qSymbol, symbolspace);
    wcscat(qSymbol, L" ");
    wcscat(qSymbol, symbol);

    if (_idToQSymbol.contains(id) || _qSymbolToId.contains(qSymbol))
    {
        throw OMException("MetaDef Id to Symbol map already contains element");
    }
    
    _idToQSymbol.insert(id, qSymbol);
    _qSymbolToId.insert(qSymbol, id);
    
    delete [] qSymbol;
}
    
OMUniqueObjectIdentification 
OMMetaDefIdMap::getId(const wchar_t* qSymbol) const
{
    TRACE("OMMetaDefIdMap::getId");
    
    OMUniqueObjectIdentification* id;
    if (_qSymbolToId.find(qSymbol, &id))
    {
        return *id;
    }
    return nullOMUniqueObjectIdentification;
}

const wchar_t* 
OMMetaDefIdMap::getQSymbol(OMUniqueObjectIdentification id) const
{
    TRACE("OMMetaDefIdMap::getQSymbol");

    OMWString* qSymbol;
    if (_idToQSymbol.find(id, &qSymbol))
    {
        return qSymbol->c_str();
    }
    return 0;
}


OMDefIdMap::OMDefIdMap()
{
    TRACE("OMDefIdMap::OMDefIdMap");
}

OMDefIdMap::~OMDefIdMap()
{
    TRACE("OMDefIdMap::~OMDefIdMap");
}
    
void 
OMDefIdMap::add(OMUniqueObjectIdentification id, const wchar_t* uidStr)
{
    TRACE("OMDefIdMap::addQSymbol");
    
    if (_idToUIDStr.contains(id) || _uidStrToId.contains(uidStr))
    {
        throw OMException("Def id to uid string map already contains element");
    }
    
    _idToUIDStr.insert(id, uidStr);
    _uidStrToId.insert(uidStr, id);
}

OMUniqueObjectIdentification 
OMDefIdMap::getId(const wchar_t* uidStr) const
{
    TRACE("OMDefIdMap::getId");
    
    OMUniqueObjectIdentification* id;
    if (_uidStrToId.find(uidStr, &id))
    {
        return *id;
    }
    return nullOMUniqueObjectIdentification;
}

const wchar_t* 
OMDefIdMap::getUIDStr(OMUniqueObjectIdentification id) const
{
    TRACE("OMDefIdMap::getUIDStr");

    OMWString* uidStr;
    if (_idToUIDStr.find(id, &uidStr))
    {
        return uidStr->c_str();
    }
    return 0;
}


OMXMLStorage::OMXMLStorage(OMDiskRawStorage* storage, Mode mode)
: _mode(mode), _storage(storage), _objectSetId(0), _baselineSymbolspace(0), 
    _defaultExtSymbolspace(0), _symbolspacePrefixIndex(0),
    _dataStreamNotationNameIndex(0), _dataStreamEntityNameIndex(0) 
{
    TRACE("OMXMLStorage::OMXMLStorage");

    if (mode == READ_MODE || mode == EXISTING_MODIFY_MODE)
    {
        _xmlWriter = 0;
        _xmlReader = OMXMLReader::create(storage);
    }
    else
    {
        _xmlWriter = OMXMLWriter::create(storage);
        _xmlReader = 0;
    }
    
    loadBaselineSymbolspace();
}

void
OMXMLStorage::loadBaselineSymbolspace()
{
    TRACE("OMXMLStorage::loadBaselineSymbolspace");

    if (_xmlReader != 0)
    {
        if (!_xmlReader->nextElement())
        {
            throw OMException("Invalid AAF file - no root element present");
        }
        const wchar_t* uri;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attributes;
        _xmlReader->getStartElement(uri, localName, attributes);
        _baselineSymbolspace = OMSymbolspace::createBaselineSymbolspace(this, uri);
        if (_baselineSymbolspace == 0)
        {
            throw OMException("Root element not recognised");
        }
        _xmlReader->reset();
    }
    else
    {
        _baselineSymbolspace = OMSymbolspace::createBaselineSymbolspace(this);
    }
    addSymbolspace(_baselineSymbolspace);
}

OMXMLStorage::~OMXMLStorage()
{
    TRACE("OMXMLStorage::~OMXMLStorage");

    if (_xmlReader != 0)
    {
        delete _xmlReader;
    }
    if (_xmlWriter != 0)
    {
        delete _xmlWriter;
    }

    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (++iter) 
    {
        delete iter.value();
    }
    
    delete _storage;
}

OMXMLStorage::Mode 
OMXMLStorage::mode()
{
    TRACE("OMXMLStorage::mode");
    
    return _mode;
}

bool 
OMXMLStorage::haveReader()
{
    TRACE("OMXMLStorage::haveReader");
    
    return _xmlReader != 0;
}

bool 
OMXMLStorage::haveWriter()
{
    TRACE("OMXMLStorage::haveWriter");
    
    return _xmlWriter != 0;
}

OMXMLWriter* 
OMXMLStorage::getWriter()
{
    TRACE("OMXMLStorage::getWriter");
    PRECONDITION("Is writer", _xmlWriter != 0);

    return _xmlWriter;
}

OMXMLReader* 
OMXMLStorage::getReader()
{
    TRACE("OMXMLStorage::getReader");
    PRECONDITION("Is reader", _xmlReader != 0);

    return _xmlReader;
}

void 
OMXMLStorage::resetForWriting()
{
    TRACE("OMXMLStorage::resetForWriting");
    PRECONDITION("Is modifiable", _mode == EXISTING_MODIFY_MODE || _mode == NEW_MODIFY_MODE);
    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (++iter) 
    {
        if (!isBaselineSymbolspace(iter.value()))
        {
            iter.value()->resetForWriting();
        }
    }
    
    if (_xmlReader != 0)
    {
        delete _xmlReader;
        _xmlReader = 0;
        
        _xmlWriter = OMXMLWriter::create(_storage);
    }
    else
    {
        // fill remaining space in file with spaces
        if (_storage->position() < _storage->size())
        {
            wchar_t spaces[1024];
            wmemset(spaces, L' ', 1023);
            spaces[1023] = L'\0';
            
            OMUInt64 remainingSpace = _storage->size() - _storage->position();
            OMUInt64 count = 0;
            while (count < remainingSpace)
            {
                OMUInt32 len = ((remainingSpace - count) > 1023) ? 1023 : 
                    (OMUInt32)(remainingSpace - count);
                if (len < 1023)
                {
                    spaces[len] = L'\0';
                }
                
                _xmlWriter->writeText(spaces);
                
                count += len;
            }
        }
    }
    _xmlWriter->reset();
}

const wchar_t* 
OMXMLStorage::getBaselineVersion() const
{
    TRACE("OMXMLStorage::getBaselineVersion");
    
    return _baselineSymbolspace->getVersion();
}

OMSymbolspace* 
OMXMLStorage::getBaselineSymbolspace() const
{
    TRACE("OMXMLStorage::getBaselineSymbolspace");
    
    return _baselineSymbolspace;
}

OMSymbolspace* 
OMXMLStorage::getDefaultExtSymbolspace() const
{
    TRACE("OMXMLStorage::getBaselineSymbolspace");
    
    return _defaultExtSymbolspace;
}

OMSymbolspace*
OMXMLStorage::createDefaultExtSymbolspace(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::createDefaultExtSymbolspace");
    PRECONDITION("Default symbolspace does not already exist", _defaultExtSymbolspace == 0);
    
    _defaultExtSymbolspace = OMSymbolspace::createDefaultExtSymbolspace(this, id);
    addSymbolspace(_defaultExtSymbolspace);
    
    return _defaultExtSymbolspace;
}

OMSymbolspace* 
OMXMLStorage::createSymbolspace()
{
    TRACE("OMXMLStorage::createSymbolspace");
    
    return new OMSymbolspace(this);
}

void 
OMXMLStorage::addSymbolspace(OMSymbolspace* symbolspace)
{
    TRACE("OMXMLStorage::addSymbolspace");

    if (_symbolspaces.contains(symbolspace->getURI()))
    {
        throw OMException("Could not add symbolspace - symbolspace with URI already exists");
    }
    setUniquePrefix(symbolspace);

    _symbolspaces.insert(symbolspace->getURI(), symbolspace);
}

OMSet<OMWString, OMSymbolspace*>& 
OMXMLStorage::getSymbolspaces()
{
    TRACE("OMXMLStorage::getSymbolspaces");
    
    return _symbolspaces;
}

void 
OMXMLStorage::addMetaDefIdMap(OMUniqueObjectIdentification id, const wchar_t* symbolspace, 
    const wchar_t* symbol)
{
    TRACE("OMXMLStorage::addMetaDefIdMap");
    
    _metaDefIdMap.add(id, symbolspace, symbol);
}

OMUniqueObjectIdentification 
OMXMLStorage::getMetaDefIdFromQSymbol(const wchar_t* qSymbol) const
{
    TRACE("OMXMLStorage::getMetaDefIdFromQSymbol");
    
    // if the qSymbol refers to a non-baseline metadef then it
    // must be in the qSymbolMap; otherwise it is a baseline qSymbol
    OMUniqueObjectIdentification id = _metaDefIdMap.getId(qSymbol);
    if (id == nullOMUniqueObjectIdentification)
    {
        // support qualified baseline symbols
        // users __should__ use just the symbol for baseline metadefs
        const wchar_t* spc = wmemchr(qSymbol, L' ', wcslen(qSymbol));
        ASSERT("Valid QSymbol has space", spc != 0);
        id = getBaselineMetaDefId(spc + 1);
    }
    
    return id;
}

OMUniqueObjectIdentification 
OMXMLStorage::getDefIdFromUIDStr(const wchar_t* uidStr) const
{
    TRACE("OMXMLStorage::getDefIdFromUIDStr");
    
    // try the baseline symbols first
    OMUniqueObjectIdentification id = getDefId(uidStr);
    if (id == nullOMUniqueObjectIdentification)
    {
        // try non-baseline uid strings
        id = _defIdMap.getId(uidStr);
    }
    
    return id;
}

const wchar_t* 
OMXMLStorage::getUIDStrFromId(OMUniqueObjectIdentification id) const
{
    TRACE("OMXMLStorage::getUIDStrFromId");
    
    return _defIdMap.getUIDStr(id);
}

void 
OMXMLStorage::addDefIdMap(OMUniqueObjectIdentification id, const wchar_t* uidStr)
{
    TRACE("OMXMLStorage::addDefIdMap");
    
    _defIdMap.add(id, uidStr);
}

bool 
OMXMLStorage::getMetaDefSymbol(OMUniqueObjectIdentification id, const wchar_t** symbolspaceURI, const wchar_t** symbol) const
{
    TRACE("OMXMLStorage::getMetaDefSymbol");
    
    const wchar_t* sym = 0;
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (sym == 0 && ++iter) 
    {
        if ((sym = iter.value()->getMetaDefSymbol(id)) != 0)
        {
            *symbol = sym;
            *symbolspaceURI = iter.value()->getURI();
        }
    }
    
    return sym != 0;
}

OMUniqueObjectIdentification 
OMXMLStorage::getMetaDefId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getMetaDefId");
    
    OMUniqueObjectIdentification result = nullOMUniqueObjectIdentification;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getMetaDefId(symbol); 
    }
    
    return result;
}

OMPropertyId 
OMXMLStorage::getPropertyDefId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getPropertyDefId");
    
    OMPropertyId result = 0x0000;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getPropertyDefId(symbol); 
    }
    
    return result;
}

const wchar_t* 
OMXMLStorage::getDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getDefSymbol");
    
    return _baselineSymbolspace->getDefSymbol(id);
}

OMUniqueObjectIdentification 
OMXMLStorage::getDefId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getDefId");
    
    return _baselineSymbolspace->getDefId(symbol);
}

OMUniqueObjectIdentification 
OMXMLStorage::getBaselineDefId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getBaselineDefId");
    
    if (_baselineSymbolspace == 0)
    {
        return nullOMUniqueObjectIdentification;
    }
    
    return _baselineSymbolspace->getDefId(symbol);
}

OMUniqueObjectIdentification 
OMXMLStorage::getBaselineMetaDefId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getBaselineMetaDefId");
    
    if (_baselineSymbolspace == 0)
    {
        return nullOMUniqueObjectIdentification;
    }
    
    return _baselineSymbolspace->getMetaDefId(symbol);
}

const wchar_t* 
OMXMLStorage::getBaselineDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getBaselineDefSymbol");
    
    if (_baselineSymbolspace == 0)
    {
        return 0;
    }
    
    return _baselineSymbolspace->getDefSymbol(id);
}

const wchar_t* 
OMXMLStorage::getBaselineMetaDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getBaselineMetaDefSymbol");
    
    if (_baselineSymbolspace == 0)
    {
        return 0;
    }
    
    return _baselineSymbolspace->getMetaDefSymbol(id);
}

OMSymbolspace* 
OMXMLStorage::getSymbolspaceForDef(OMUniqueObjectIdentification id) const
{
    TRACE("OMXMLStorage::getSymbolspaceForDef");

    OMSymbolspace* symbolspace = 0;    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (symbolspace == 0 && ++iter) 
    {
        if (iter.value()->getDefSymbol(id) != 0)
        {
            symbolspace = iter.value();
        }
    }
    
    return symbolspace;
}

OMSymbolspace* 
OMXMLStorage::getSymbolspaceForMetaDef(OMUniqueObjectIdentification id) const
{
    TRACE("OMXMLStorage::getSymbolspaceForMetaDef");
    
    OMSymbolspace* symbolspace = 0;    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (symbolspace == 0 && ++iter) 
    {
        if (iter.value()->getMetaDefSymbol(id) != 0)
        {
            symbolspace = iter.value();
        }
    }
    
    return symbolspace;
}

bool 
OMXMLStorage::isKnownExtEnumElement(OMUniqueObjectIdentification elementOf,
    OMUniqueObjectIdentification value) const
{
    TRACE("OMXMLStorage::isKnownExtEnumElement");
    
    bool isKnown = false;    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (!isKnown && ++iter) 
    {
        isKnown = iter.value()->isKnownExtEnumElement(elementOf, value);
    }
    
    return isKnown;
}

bool 
OMXMLStorage::isBaselineSymbolspace(OMSymbolspace* symbolspace) const
{
    TRACE("OMXMLStorage::isBaselineSymbolspace");
    
    if (_baselineSymbolspace != 0 && symbolspace == _baselineSymbolspace)
    {
        return true;
    }
    
    return false;    
}

const wchar_t* 
OMXMLStorage::getDataStreamNotationName(OMUniqueObjectIdentification typeId)
{
    TRACE("OMXMLStorage::getDataStreamNotationName");
    
    OMWString* ret;
    if (_dataStreamNotationNames.find(typeId, &ret))
    {
        return (*ret).c_str();
    }

    wchar_t name[20];
    std_swprintf(name, 20, L"DataStream_%x", _dataStreamNotationNameIndex);
    _dataStreamNotationNames.insert(typeId, name); 
    _dataStreamNotationNameIndex++;

    if (!_dataStreamNotationNames.find(typeId, &ret))
    {
        ASSERT("Name correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityName(void* ref)
{
    TRACE("OMXMLStorage::getDataStreamEntityName");
    
    OMWString* ret;
    if (_dataStreamEntityNames.find(ref, &ret))
    {
        return (*ret).c_str();
    }

    wchar_t name[16];
    std_swprintf(name, 16, L"stream_%x", _dataStreamEntityNameIndex);
    _dataStreamEntityNames.insert(ref, name); 
    _dataStreamEntityNameIndex++;
    
    if (!_dataStreamEntityNames.find(ref, &ret))
    {
        ASSERT("Name correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityValue(void* ref)
{
    TRACE("OMXMLStorage::getDataStreamEntityValue");
    
    OMWString* ret;
    if (_dataStreamEntityValues.find(ref, &ret))
    {
        return (*ret).c_str();
    }
    
    // worst case length is every character escaped (*3) plus "file:///" prefix (+8) + null (+1) 
    wchar_t* documentUri = new wchar_t[utf8StrLen(_storage->fileName()) * 3 + 9]; 
    wcsconvertFilepathtoURI(_storage->fileName(), documentUri);

    // remove the file suffix and the path
    wchar_t* startPtr = documentUri + wcslen(documentUri);
    wchar_t* endPtr = startPtr;
    while (endPtr != documentUri && 
        *endPtr != L'.' && *endPtr != '/' && *endPtr != ':')
    {
        endPtr--;
    }
    if (*endPtr == L'.')
    {
        *endPtr = L'\0';
        startPtr = endPtr;
    }
    while (startPtr != documentUri && 
        *startPtr != '/' && *startPtr != ':')
    {
        startPtr--;
    }
    if (*startPtr == '/' || *startPtr == ':')
    {
        startPtr++;
    }

    // append "_streams/" + entity name
    const wchar_t* entityName = getDataStreamEntityName(ref);
    wchar_t* value = new wchar_t[wcslen(startPtr) + wcslen(L"_streams/") + wcslen(entityName) + 1];
    wcscpy(value, startPtr);
    wcscat(value, L"_streams/");
    wcscat(value, entityName);
    
    delete [] documentUri; 
    
    // insert URI value
    _dataStreamEntityValues.insert(ref, value);
    delete [] value;
    
    if (!_dataStreamEntityValues.find(ref, &ret))
    {
        ASSERT("Value correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::registerDataStreamEntityValue(void* ref, const wchar_t* value)
{
    TRACE("OMXMLStorage::registerDataStreamEntityValue");
    
    OMWString* ret;
    if (_dataStreamEntityValues.find(ref, &ret))
    {
        return 0;
    }

    _dataStreamEntityValues.insert(ref, value); 

    if (!_dataStreamEntityValues.find(ref, &ret))
    {
        ASSERT("Value correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

bool 
OMXMLStorage::registerDataStreamEntity(const wchar_t* name, const wchar_t* value)
{
    TRACE("OMXMLStorage::registerDataStreamEntity");
    
    if (_inputDataStreamEntities.contains(name))
    {
        return false;
    }
    
    _inputDataStreamEntities.insert(name, value);
    
    return true;
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityValue(const wchar_t* name)
{
    TRACE("OMXMLStorage::getDataStreamEntityValue");
    
    OMWString* ret;
    if (_inputDataStreamEntities.find(name, &ret))
    {
        return (*ret).c_str();
    }
    
    return 0;
}

bool 
OMXMLStorage::registerDataStreamNotation(const wchar_t* notationName, 
    const wchar_t* systemId)
{
    TRACE("OMXMLStorage::registerDataStreamNotation");

    if (notationName == 0 || systemId == 0 || !isAUIDURI(systemId))
    {
        return false;
    }

    OMUniqueObjectIdentification id;
    uriToAUID(systemId, &id);
    
    OMWString* ret;
    if (_dataStreamNotationNames.find(id, &ret))
    {
        return false;
    }

    _dataStreamNotationNames.insert(id, notationName);
    
    return true;
}

OMRawStorage* 
OMXMLStorage::openExistingDataStream(const wchar_t* uri)
{
    TRACE("OMXMLStorage::openExistingDataStream");

    wchar_t* filepath = new wchar_t[wcslen(uri) + 1];
    wcsconvertURItoFilepath(uri, filepath);
    
    wchar_t* fullFileName;
    if (isRelativePath(filepath) && _storage->fileName() != 0)
    {
        wchar_t* base = getBaseFilePath(_storage->fileName());
        fullFileName = new wchar_t[wcslen(base) + wcslen(filepath) + 1];
        wcscpy(fullFileName, base);
        wcscat(fullFileName, filepath);
        delete [] base;
    }
    else
    {
        fullFileName = new wchar_t[wcslen(filepath) + 1];
        wcscpy(fullFileName, filepath);
    }
    
    OMDiskRawStorage* storage = 0;
    if (fileExists(fullFileName))
    {
        try
        {
            if (_mode == READ_MODE)
            {
                storage = OMDiskRawStorage::openExistingRead(fullFileName);
            }
            else
            {
                storage = OMDiskRawStorage::openExistingModify(fullFileName);
            }
        }
        catch (...)
        {
            storage = 0;
        }
    }

    delete [] filepath;
    delete [] fullFileName;
    
    return storage;
}

OMRawStorage* 
OMXMLStorage::openNewDataStream(const wchar_t* uri)
{
    TRACE("OMXMLStorage::openNewDataStream");
    PRECONDITION("XML document raw storage has filename", _storage->fileName() != 0);

    // uri is assumed to be 'stream directory' / 'stream file name' as
    // created in getDataStreamEntityValue()

    OMDiskRawStorage* storage = 0;

    wchar_t* base = getBaseFilePath(_storage->fileName());


    // start by creating the directory
    wchar_t* workBuffer = new wchar_t[wcslen(uri) + 1];
    wcscpy(workBuffer, uri);
    wchar_t* sep = wmemchr(workBuffer, L'/', wcslen(uri));
    ASSERT("New DataStream uri starts with a directory", sep != 0);
    *sep = L'\0';
    wchar_t* filepath = new wchar_t[wcslen(workBuffer) + 1];
    wcsconvertURItoFilepath(workBuffer, filepath);
    wchar_t* fullFilepath = new wchar_t[wcslen(base) + wcslen(filepath) + 1];
    wcscpy(fullFilepath, base);
    wcscat(fullFilepath, filepath);
    wmkdir(fullFilepath);
    delete [] workBuffer;
    delete [] filepath;
    delete [] fullFilepath;

    // remove the file and then open new modify
    filepath = new wchar_t[wcslen(uri) + 1];
    wcsconvertURItoFilepath(uri, filepath);
    fullFilepath = new wchar_t[wcslen(base) + wcslen(filepath) + 1];
    wcscpy(fullFilepath, base);
    wcscat(fullFilepath, filepath);
    wremove(fullFilepath);
    try
    {
        storage = OMDiskRawStorage::openNewModify(fullFilepath);
    }
    catch (...)
    {
        storage = 0;
    }

    delete [] filepath;
    delete [] fullFilepath;
    delete [] base;
    
    return storage;
}

void 
OMXMLStorage::forwardObjectSetId(const wchar_t* id)
{
    TRACE("OMXMLStorage::forwardObjectSetId");
    PRECONDITION("Valid object set id", id != 0);
    
    if (_objectSetId != 0)
    {
        delete [] _objectSetId;
    }
    
    _objectSetId = new wchar_t[wcslen(id) + 1];
    wcscpy(_objectSetId, id);
}

bool 
OMXMLStorage::haveForwardedObjectSetId()
{
    TRACE("OMXMLStorage::haveForwardedObjectSetId");
    
    return _objectSetId != 0;
}

wchar_t* 
OMXMLStorage::getForwardedObjectSetId()
{
    TRACE("OMXMLStorage::getForwardedObjectSetId");
    PRECONDITION("Have forwarded object set id", _objectSetId != 0);
    
    wchar_t* id = _objectSetId;
    _objectSetId = 0;
    
    return id;
}

void 
OMXMLStorage::registerNamespacePrefix(const wchar_t* prefix, const wchar_t* uri)
{
    TRACE("OMXMLStorage::registerNamespacePrefix");

    if (_namespacesPrefixMap.contains(uri))
    {
        _namespacesPrefixMap.remove(uri);
    }
    _namespacesPrefixMap.insert(uri, prefix);
}


void
OMXMLStorage::setUniquePrefix(OMSymbolspace* symbolspace)
{
    TRACE("OMXMLStorage::setUniquePrefix");

    // use the prefix used in the existing document if it is unique
    OMWString* prefix = 0;
    _namespacesPrefixMap.find(symbolspace->getURI(), &prefix);
    if (prefix != 0 &&
        !_symbolspacesPrefixMap.contains(prefix->c_str()))
    {
        symbolspace->setPrefix(prefix->c_str());
        _symbolspacesPrefixMap.insert(symbolspace->getPrefix(), symbolspace);
    }
    
    // else use the preferred prefix if it is unique
    else if (symbolspace->getPreferredPrefix() != 0 &&
        wcslen(symbolspace->getPreferredPrefix()) > 0 && 
        !_symbolspacesPrefixMap.contains(symbolspace->getPreferredPrefix()))
    {
        symbolspace->setPrefix(symbolspace->getPreferredPrefix());
        _symbolspacesPrefixMap.insert(symbolspace->getPrefix(), symbolspace);
    }

    // else create a unique prefix from the preferred prefix or from 'ns'
    else
    {
        wchar_t* prefix = 0;
        if (symbolspace->getPreferredPrefix() == 0 ||
            wcslen(symbolspace->getPreferredPrefix()) == 0)
        {
            const wchar_t* basePrefix = L"ns";
            prefix = new wchar_t[wcslen(basePrefix) + 1];
            wcscpy(prefix, basePrefix);
        }
        else
        {
            prefix = wideCharacterStringDup(symbolspace->getPreferredPrefix());
        }
        
        while (_symbolspacesPrefixMap.contains(prefix))
        {
            wchar_t suffix[9];
            std_swprintf(suffix, 9, L"%u", _symbolspacePrefixIndex);
            wchar_t* newPrefix = new wchar_t[wcslen(prefix) + 8 + 1];
            wcscpy(newPrefix, prefix);
            delete [] prefix;
            prefix = wcscat(newPrefix, suffix);
            _symbolspacePrefixIndex++;
        }
        
        symbolspace->setPrefix(prefix);
        _symbolspacesPrefixMap.insert(symbolspace->getPrefix(), symbolspace);

        delete [] prefix;
    }
}


