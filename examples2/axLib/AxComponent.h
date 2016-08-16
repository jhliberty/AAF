#ifndef __AxComponent_h__
#define __AxComponent_h__

//=---------------------------------------------------------------------=
//
// $Id: AxComponent.h,v 1.27 2009/06/01 11:46:55 stuart_hc Exp $ $Name: V116 $
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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

#include <vector>

//=---------------------------------------------------------------------=

class AxComponent : public AxObject {

public:
	AxComponent( IAAFComponentSP spIaafComponent );
    AxComponent( IAAFComponent2SP spIaafComponent );
	virtual ~AxComponent();

	aafLength_t GetLength();
	std::pair<bool,aafLength_t> ExistsLength();
	void SetLength( const aafLength_t& len );

	void SetDataDef( IAAFDataDefSP );
	IAAFDataDefSP GetDataDef();
    
    void AppendKLVData( IAAFKLVDataSP klvData );
    void AppendComment( const AxString& name, const AxString& value );
    void AppendAttribute( const AxString& name, const AxString& value );
    
    aafUInt32 CountKLVData();
    aafUInt32 CountComments();
    aafUInt32 CountAttributes();
    
    IEnumAAFKLVDataSP GetKLVData();
    IEnumAAFTaggedValuesSP GetComments();
    IEnumAAFTaggedValuesSP GetAttributes();
    
    void RemoveKLVData( IAAFKLVDataSP klvpData );
    void RemoveComment( IAAFTaggedValueSP comment );
    void RemoveAttribute( IAAFTaggedValueSP attribute );

    inline operator IAAFComponentSP ()
    { return AxQueryInterface<IAAFComponent2,IAAFComponent>( _spIaafComponent ); }

    inline operator IAAFComponent2SP ()
    { return _spIaafComponent; }

private:
	AxComponent();
	AxComponent( const AxComponent& );
	AxComponent& operator=( const AxComponent& );

	IAAFComponent2SP _spIaafComponent;
};

//=---------------------------------------------------------------------=

class AxSegment : public AxComponent {

public:
	AxSegment( IAAFSegmentSP spIaafSegment );
	virtual ~AxSegment();

	operator IAAFSegmentSP ()
	{ return _spIaafSegment; }

private:
	AxSegment();
	AxSegment( const AxSegment& );
	AxSegment& operator=( const AxSegment& );

	IAAFSegmentSP _spIaafSegment;
};

//=---------------------------------------------------------------------=

class AxTransition : public AxComponent {

public:
	AxTransition( IAAFTransitionSP spIaafTransition );
	virtual ~AxTransition();

	void Initialize( IAAFDataDefSP, aafLength_t, aafPosition_t, IAAFOperationGroupSP);

	aafPosition_t GetCutPoint();
	void SetCutPoint( aafPosition_t cutPoint );

	IAAFOperationGroupSP GetOperationGroup();
	void SetOperationGroup( IAAFOperationGroupSP );

	operator IAAFTransitionSP ()
	{ return _spIaafTransition; }

private:
	AxTransition();
	AxTransition( const AxTransition& );
	AxTransition& operator=( const AxTransition& );

	IAAFTransitionSP _spIaafTransition;
};

//=---------------------------------------------------------------------=

class AxSequence : public AxSegment {

public:
	AxSequence( IAAFSequenceSP spIaafSequence );
	virtual ~AxSequence();

	void Initialize( IAAFDataDefSP spIaafDataDef );

	void AppendComponent( IAAFComponentSP );
	void PrependComponent( IAAFComponentSP );

	IEnumAAFComponentsSP GetComponents();

	IAAFComponentSP GetComponentAt( aafUInt32 index );
	void InsertComponentAt( aafUInt32, IAAFComponentSP );
	void RemoveComponentAt( aafUInt32 );

	aafUInt32 CountComponents();

private:
	AxSequence();
	AxSequence( const AxSequence& );
	AxSequence& operator=( const AxSequence& );

	IAAFSequenceSP _spIaafSequence;
};

//=---------------------------------------------------------------------=

class AxTimecode : public AxSegment {

public:
	AxTimecode( IAAFTimecodeSP spIaafTimecode );
	virtual ~AxTimecode();

    void Initialize( aafLength_t length, const aafTimecode_t& timecode );

	aafTimecode_t GetTimecode();
	void SetTimecode( const aafTimecode_t& timecode );

private:
	AxTimecode();
	AxTimecode( const AxTimecode& );
	AxTimecode& operator=( const AxTimecode& );

	IAAFTimecodeSP _spIaafTimecode;
};

//=---------------------------------------------------------------------=

class AxFiller : public AxSegment {

public:
	AxFiller( IAAFFillerSP spIaafFiller );
	virtual ~AxFiller();

    void Initialize( IAAFDataDefSP spDataDef, aafLength_t length );

private:
	AxFiller();
	AxFiller( const AxFiller& );
	AxFiller& operator=( const AxFiller& );

	IAAFFillerSP _spIaafFiller;
};

//=---------------------------------------------------------------------=

class AxPulldown : public AxSegment {

public:
	AxPulldown( IAAFPulldownSP _spIaafPulldown );
	virtual ~AxPulldown();

	IAAFSegmentSP GetInputSegment();
	aafPulldownKind_t GetPulldownKind();
	aafPulldownDir_t GetPulldownDirection();
	aafPhaseFrame_t GetPhaseFrame();

	void SetInputSegment( IAAFSegmentSP );
	void SetPulldownKind( aafPulldownKind_t );
	void SetPulldownDirection( aafPulldownDir_t );
	void SetPhaseFrame( aafPhaseFrame_t );

	operator IAAFPulldownSP ()
	{ return _spIaafPulldown; }

private:
	AxPulldown();
	AxPulldown( const AxPulldown& );
	AxPulldown& operator=( const AxPulldown& );

	IAAFPulldownSP _spIaafPulldown;
};
//=---------------------------------------------------------------------=

class AxSourceReference : public AxSegment {

public:
	AxSourceReference( IAAFSourceReferenceSP spIaafSourceReference );
	virtual ~AxSourceReference();

	aafMobID_t GetSourceID();
	void SetSourceID( const aafMobID_t& id );

	aafSlotID_t GetSourceMobSlotID();
	void SetSourceMobSlotID( const aafSlotID_t& slotid );
    
    operator IAAFSourceReferenceSP ()
    { return _spIaafSourceReference; }

private:
	AxSourceReference();
	AxSourceReference( const AxSourceReference& );
	AxSourceReference& operator=( const AxSourceReference& );

	IAAFSourceReferenceSP _spIaafSourceReference;
};

//=---------------------------------------------------------------------=

class AxSourceClip : public AxSourceReference {

public:
	AxSourceClip( IAAFSourceClipSP spIaafSourceClip );
	virtual ~AxSourceClip();

	void Initialize( IAAFDataDefSP, const aafLength_t&, const aafSourceRef_t& );

	void SetSourceReference( const aafSourceRef_t& ref );
	aafSourceRef_t GetSourceReference();

	IAAFMobSP ResolveRef();

	operator IAAFSourceClipSP ()
	{ return _spIaafSourceClip; }

private:
	AxSourceClip();
	AxSourceClip( const AxSourceClip& );
	AxSourceClip& operator=( const AxSourceClip& );

	IAAFSourceClipSP _spIaafSourceClip;
};

//=---------------------------------------------------------------------=

class AxOperationGroup : public AxSegment {

public:
	AxOperationGroup( IAAFOperationGroupSP spIaafOperationGroup );
	virtual ~AxOperationGroup();

	void Initialize( IAAFDataDefSP, aafLength_t, IAAFOperationDefSP );

	void AppendInputSegment( IAAFSegmentSP );
	void PrependInputSegment( IAAFSegmentSP );
	void InsertInputSegmentAt( aafUInt32, IAAFSegmentSP );
	IAAFSegmentSP GetInputSegmentAt( aafUInt32 );
	void RemoveInputSegmentAt( aafUInt32 );
	aafUInt32 CountSourceSegments();

	IAAFOperationDefSP GetOperationDef();
	void SetOperationDef( IAAFOperationDefSP );

	IAAFSourceReferenceSP GetRender();
	void SetRender( IAAFSourceReferenceSP );

	aafBoolean_t IsATimeWarp();
	aafBoolean_t IsValidTranOperation();

	aafUInt32 GetBypassOverride();
	void SetBypassOverride( aafUInt32 );

	aafUInt32 CountParameters();
	void AddParameter( IAAFParameterSP );
	IEnumAAFParametersSP GetParameters();
	IAAFParameterSP LookupParameter( aafArgIDType_constref );
	
	operator IAAFOperationGroupSP ()
	{ return _spIaafOperationGroup; }

private:
	AxOperationGroup();
	AxOperationGroup( const AxOperationGroup& );
	AxOperationGroup& operator=( const AxOperationGroup& );

	IAAFOperationGroupSP _spIaafOperationGroup;
};

//=---------------------------------------------------------------------=

class AxNestedScope : public AxSegment {

public:
	AxNestedScope( IAAFNestedScopeSP spIaafNestedScope );
	virtual ~AxNestedScope();

	void AppendSegment( IAAFSegmentSP );
	void PrependSegment( IAAFSegmentSP );
	void InsertSegmentAt( aafUInt32, IAAFSegmentSP );
	void RemoveSegmentAt( aafUInt32 );
	aafUInt32 CountSegments();
	IAAFSegmentSP GetSegmentAt( aafUInt32 );
	IEnumAAFSegmentsSP GetSegments();

	operator IAAFNestedScopeSP ()
	{ return _spIaafNestedScope; }

private:
	AxNestedScope();
	AxNestedScope( const AxNestedScope& );
	AxNestedScope& operator=( const AxNestedScope& );

	IAAFNestedScopeSP _spIaafNestedScope;
};

//=---------------------------------------------------------------------=

class AxScopeReference : public AxSegment {

public:
	AxScopeReference( IAAFScopeReferenceSP spIaafScopeReference );
	virtual ~AxScopeReference();

	void Initialize( IAAFDataDefSP, aafUInt32, aafUInt32 );
	aafUInt32 GetRelativeScope();
	aafUInt32 GetRelativeSlot();

	operator IAAFScopeReferenceSP ()
	{ return _spIaafScopeReference; }

private:
	AxScopeReference();
	AxScopeReference( const AxScopeReference& );
	AxScopeReference& operator=( const AxScopeReference& );

	IAAFScopeReferenceSP _spIaafScopeReference;
};

//=---------------------------------------------------------------------=

class AxEssenceGroup : public AxSegment {

public:
	AxEssenceGroup( IAAFEssenceGroupSP spIaafEssenceGroup );
	virtual ~AxEssenceGroup();

	// Get methods only for now.
	IAAFSourceClipSP GetStillFrame();
	aafUInt32 CountChoices();
	IAAFSegmentSP GetChoiceAt( aafUInt32 index );

	operator IAAFEssenceGroupSP ()
	{ return _spIaafEssenceGroup; }

private:
	AxEssenceGroup();
	AxEssenceGroup( const AxEssenceGroup& );
	AxEssenceGroup& operator=( const AxEssenceGroup& );

	IAAFEssenceGroupSP _spIaafEssenceGroup;
};

//=---------------------------------------------------------------------=

class AxSelector : public AxSegment {

public:
	AxSelector( IAAFSelectorSP spIaafSelector );
	virtual ~AxSelector();

	// Get methods only for now.
	IAAFSegmentSP GetSelectedSegment();
	IEnumAAFSegmentsSP EnumAlternateSegments();

	operator IAAFSelectorSP ()
	{ return _spIaafSelector; }

private:
	AxSelector();
	AxSelector( const AxSelector& );
	AxSelector& operator=( const AxSelector& );

	IAAFSelectorSP _spIaafSelector;
};

//=---------------------------------------------------------------------=

class AxEdgecode : public AxSegment {

public:
	AxEdgecode( IAAFEdgecodeSP spIaafEdgecode );
	virtual ~AxEdgecode();

	void Initialize( aafLength_t, aafEdgecode_t );
	aafEdgecode_t GetEdgecode();

	operator IAAFEdgecodeSP ()
	{ return _spIaafEdgecode; }

private:
	AxEdgecode();
	AxEdgecode( const AxEdgecode& );
	AxEdgecode& operator=( const AxEdgecode& );

	IAAFEdgecodeSP _spIaafEdgecode;
};

//=---------------------------------------------------------------------=

class AxEvent : public AxSegment {

public:
	AxEvent( IAAFEventSP spIaafEvent );
	virtual ~AxEvent();

	aafPosition_t GetPosition();
	void SetPosition( aafPosition_t pos );

	void SetComment( const AxString& comment );
	AxString GetComment();

private:
	AxEvent();
	AxEvent( const AxEvent& );
	AxEvent& operator=( const AxEvent& );

	IAAFEventSP _spIaafEvent;
};

//=---------------------------------------------------------------------=

class AxCommentMarker : public AxEvent {

public:
	AxCommentMarker( IAAFCommentMarkerSP spIaafCommentMarker );
	virtual ~AxCommentMarker();

	IAAFSourceReferenceSP GetAnnotation();

	void SetAnnotation( IAAFSourceReferenceSP spIaafSourceReference );

private:
	AxCommentMarker();
	AxCommentMarker( const AxEvent& );
	AxCommentMarker& operator=( const AxCommentMarker& );

	IAAFCommentMarkerSP _spIaafCommentMarker;
};

//=---------------------------------------------------------------------=

class AxGPITrigger : public AxEvent {

public:
	AxGPITrigger( IAAFGPITriggerSP spIaafGPITrigger );
	virtual ~AxGPITrigger();

	aafBoolean_t GetActiveState();
	void SetActiveState( aafBoolean_t );

private:
	AxGPITrigger();
	AxGPITrigger( const AxGPITrigger& );
	AxGPITrigger& operator=( const AxGPITrigger& );

	IAAFGPITriggerSP _spIaafGPITrigger;
};

//=---------------------------------------------------------------------=

class AxTimecodeStream : public AxSegment {

public:
    AxTimecodeStream( IAAFTimecodeStreamSP spIaafTimecodeStream );
    virtual ~AxTimecodeStream();

    aafTimecode_t GetPositionTimecode ( aafPosition_t position );
    aafInt32 GetUserDataLength ();
    aafRational_t GetSampleRate ();
    aafTimecodeSourceType_t GetSourceType ();
    aafUInt32 GetSampleSize ();
    aafUInt32 GetSourceBufLen ();
    void GetUserDataAtPosition ( aafPosition_t position, aafInt32 buflen, aafDataBuffer_t buffer );
    void GetSource( aafUInt32 valueSize, aafDataBuffer_t pValue, aafUInt32* bytesRead );

    void SetPositionTimecode ( aafPosition_t position, aafTimecode_t timecode);
    void SetUserDataAtPosition ( aafPosition_t position, aafInt32 buflen, aafDataBuffer_t buffer );
    void SetSampleRate ( aafRational_t sampleRate );
    void SetSourceType ( aafTimecodeSourceType_t sourceType );
    void SetSource (aafUInt32 valueSize, aafDataBuffer_t pValue);
    
    inline operator IAAFTimecodeStreamSP ()
    { return _spIaafTimecodeStream; }

private:
    AxTimecodeStream();
    AxTimecodeStream( const AxTimecodeStream& );
    AxTimecodeStream& operator=( const AxTimecodeStream& );

    IAAFTimecodeStreamSP _spIaafTimecodeStream;
};

//=---------------------------------------------------------------------=

class AxTimecodeStream12M : public AxTimecodeStream {

public:
    AxTimecodeStream12M( IAAFTimecodeStream12MSP spIaafTimecodeStream12M );
    virtual ~AxTimecodeStream12M();

    inline operator IAAFTimecodeStream12MSP ()
    { return _spIaafTimecodeStream12M; }

private:
    AxTimecodeStream12M();
    AxTimecodeStream12M( const AxTimecodeStream12M& );
    AxTimecodeStream12M& operator=( const AxTimecodeStream12M& );

    IAAFTimecodeStream12MSP _spIaafTimecodeStream12M;
};

//=---------------------------------------------------------------------=

class AxDescriptiveMarker : public AxCommentMarker {

public:
    AxDescriptiveMarker( IAAFDescriptiveMarkerSP spIaafDescriptiveMarker );
    virtual ~AxDescriptiveMarker();
    
    void Initialize();
    
    std::vector<aafUInt32> GetDescribedSlotIDs();
    IAAFDescriptiveFrameworkSP GetDescriptiveFramework();
    
    void SetDescribedSlotIDs( aafUInt32 numberElements, aafUInt32* describedSlotIDs );
    void SetDescriptiveFramework( IAAFDescriptiveFrameworkSP descriptiveFramework );

    inline operator IAAFDescriptiveMarkerSP ()
    { return _spIaafDescriptiveMarker; }

private:
    AxDescriptiveMarker();
    AxDescriptiveMarker( const AxDescriptiveMarker& );
    AxDescriptiveMarker& operator=( const AxDescriptiveMarker& );

    IAAFDescriptiveMarkerSP _spIaafDescriptiveMarker;
};

//=---------------------------------------------------------------------=

#endif
