//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

//Edit Protocol Test files
#include "DecorateEPTest.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Edit Protocol Analyzer Base files
#include <EPTypedObjNode.h>
#include <EPDerivationChainObject.h>
#include <EPTrack.h>
#include <EPEffect.h>

//AAF Analyzer Base files
#include <MobNodeMap.h>
#include <TypedVisitor.h>
#include <TestGraph.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxMob.h>
#include <AxMetaDef.h>
#include <AxEssence.h>
#include <AxMobSlot.h>
#include <AxDefObject.h>

//AAF files
#include <AAFResult.h>
#include <AAFExtEnum.h>
#include <AAFClassDefUIDs.h>
#include <AAFOperationDefs.h>

//STL files
#include <set>
#include <map>

namespace {

using namespace aafanalyzer;

const wchar_t* TEST_NAME = L"Edit Protocol Decoration";
const wchar_t* TEST_DESC = L"Decorate the nodes of the graph with their edit protocol material types.";

//======================================================================

// EPDecoratorVisitor visits all mob types and decorates the nodes with the
// appropriate edit protocol material type.

class EPDecoratorVisitor : public TypedVisitor
{
public:
  EPDecoratorVisitor( wostream& log,
                      boost::shared_ptr<const TestGraph> spTestGraph )
    : TypedVisitor(),
      _log( log ),
      _spGraph( spTestGraph )
  {
    //Setup the list of effects defined by the Edit Protocol
    _editProtocolEffects.insert( kAAFOperationDef_VideoDissolve );
    _editProtocolEffects.insert( kAAFOperationDef_SMPTEVideoWipe );
    _editProtocolEffects.insert( kAAFOperationDef_VideoSpeedControl );
    _editProtocolEffects.insert( kAAFOperationDef_VideoRepeat );
    _editProtocolEffects.insert( kAAFOperationDef_Flip );
    _editProtocolEffects.insert( kAAFOperationDef_Flop );
    _editProtocolEffects.insert( kAAFOperationDef_FlipFlop );
    _editProtocolEffects.insert( kAAFOperationDef_VideoPosition );
    _editProtocolEffects.insert( kAAFOperationDef_VideoCrop );
    _editProtocolEffects.insert( kAAFOperationDef_VideoScale );
    _editProtocolEffects.insert( kAAFOperationDef_VideoRotate );
    _editProtocolEffects.insert( kAAFOperationDef_VideoCornerPinning );
    _editProtocolEffects.insert( kAAFOperationDef_VideoAlphaWithinVideoKey );
    _editProtocolEffects.insert( kAAFOperationDef_VideoSeparateAlphaKey );
    _editProtocolEffects.insert( kAAFOperationDef_VideoLuminanceKey );
    _editProtocolEffects.insert( kAAFOperationDef_VideoChromaKey );
    _editProtocolEffects.insert( kAAFOperationDef_MonoAudioGain );
    _editProtocolEffects.insert( kAAFOperationDef_MonoAudioPan );
    _editProtocolEffects.insert( kAAFOperationDef_MonoAudioDissolve );
    _editProtocolEffects.insert( kAAFOperationDef_TwoParameterMonoAudioDissolve );
  }

  virtual ~EPDecoratorVisitor()
  {}
 
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );

    //Find the usage code to find the proper type of decoration for the node.
    pair<bool,aafUID_t> usageCode = axCompMob.ExistsUsageCode();
    if ( usageCode.first )
    {
      if ( usageCode.second == kAAFUsage_TopLevel )
      {
	this->DecorateNode<IAAFCompositionMob, EPTopLevelComposition>( node );
      }
      else if ( usageCode.second == kAAFUsage_LowerLevel )
      {
	this->DecorateNode<IAAFCompositionMob, EPLowerLevelComposition>( node );
      }
      else if ( usageCode.second == kAAFUsage_SubClip )
      {
	this->DecorateNode<IAAFCompositionMob, EPSubClipComposition>( node );
      }
      else if ( usageCode.second == kAAFUsage_AdjustedClip )
      {
	this->DecorateNode<IAAFCompositionMob, EPAdjustedClipComposition>( node );
      }
    }
    // else
    // There are no other valid composition mob/usage code
    // combinations.  That means that the material type for the
    // derivation chain is unknown, however, the edit protocol does
    // not state that all mobs must be in the derivation chain.
  
    return true;  
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );

    //If this is a template clip, decorate it.
    pair<bool,aafUID_t> usageCode = axMastMob.ExistsUsageCode();
    
    if ( usageCode.first && usageCode.second == kAAFUsage_Template )
    {
      this->DecorateNode<IAAFMasterMob, EPTemplateClip>( node );
    }
    else
    {
      this->DecorateNode<IAAFMasterMob, EPClip>( node );
    }
    
    return true;
  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    try
    {
        
      /*
       * 
       * Note: A visitor could be used to properly classify Source Mobs into
       *       their correct Edit Protocol material types.  This would require
       *       implementing a PreOrderVisit for every accepted descriptor and
       *       every known descriptor that is derived from an accepted
       *       descriptor.  Also, a mechanism would be needed to pass the
       *       Edit Protocol type from the visitor to this function.  Unless a
       *       good reason presents itself to use the above mentioned method,
       *       the current method will be used.
       * 
       */

      //Shared pointers used to determine types.
      IAAFSoundDescriptorSP         spSoundDescriptor;
      IAAFRGBADescriptorSP          spRGBADescriptor;
      IAAFCDCIDescriptorSP          spCDCIDescriptor;
      IAAFDigitalImageDescriptorSP  spDigitalImageDescriptor;
      IAAFFileDescriptorSP          spFileDescriptor;
      IAAFRecordingDescriptorSP     spRecordingDescriptor;
      IAAFImportDescriptorSP        spImportDescriptor;
      IAAFTapeDescriptorSP          spTapeDescriptor;
      IAAFFilmDescriptorSP          spFilmDescriptor;
      IAAFAuxiliaryDescriptorSP     spAuxDescriptor;
      
      //Find the type of descriptor on the node to properly decorate it.
      AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );

      //We must check for SoundDescriptors before checking for FileDescriptors
      //since FileDescriptior is the super-class of SoundDescriptor.
      if ( AxIsA( descriptor, spSoundDescriptor ) )
      {
        AxSoundDescriptor soundDescriptor( AxQueryInterface<IAAFEssenceDescriptor, IAAFSoundDescriptor>( descriptor ) );
        if ( soundDescriptor.GetChannelCount() == 1 )
        {
          this->DecorateNode<IAAFSourceMob, EPMonoAudioFileSource>( node );
        }
        else
        {
          this->DecorateNode<IAAFSourceMob, EPMultiChannelAudioFileSource>( node );
        }
      }
      //We must check for RGBA and CDCI Descriptors before checking for
      //DigitalImageDescriptors since DigitalImageDescriptor is a super-class.
      else if ( AxIsA( descriptor, spRGBADescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPRGBAImageFileSource>( node );
      }
      else if ( AxIsA( descriptor, spCDCIDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPCDCIImageFileSource>( node );
      }
      //We must check for DigitalImageDescriptors before checking for
      //FileDescriptors since FileDescriptior is the super-class of
      //DigitalImageDescriptor.
      else if ( AxIsA( descriptor, spDigitalImageDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPImageFileSource>( node );
      }
      else if ( AxIsA( descriptor, spFileDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPFileSource>( node );
      }
      else if ( AxIsA( descriptor, spRecordingDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPRecordingSource>( node );
      }
      else if ( AxIsA( descriptor, spImportDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPImportSource>( node );
      }
      else if ( AxIsA( descriptor, spTapeDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPTapeSource>( node );
      }
      else if ( AxIsA( descriptor, spFilmDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPFilmSource>( node );
      }
      else if ( AxIsA( descriptor, spAuxDescriptor ) )
      {
        this->DecorateNode<IAAFSourceMob, EPAuxiliarySource>( node );
      }
      //There are no other valid source mob/descriptor code combinations.  That
      //means that the material type for the derivation chain is unknown,
      //however, the edit protocol does not state that all mobs must be in the
      //derivation chain.

    }
    catch ( const AxExHResult& ex )
    {
      //If the exception is that the essence descriptor is not present, this
      //means that the meterial type for the derivation chain is unknown, 
      //however, the edit protocol does not state that all mobs must be in the 
      //derivation chain, so, continue without decorating the object.
      if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
      {
        throw ex;
      }
    }
    
    return true;

  }

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
  {
    AxTimelineMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFTimelineMobSlot>( axMobSlot, node );
    return true;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
  {
    AxStaticMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFStaticMobSlot>( axMobSlot, node );
    return true;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
  {
    AxEventMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFEventMobSlot>( axMobSlot, node );
    return true;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
  {
    AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
    DecorateMobSlot<IAAFMobSlot>( axMobSlot, node );    
    return true;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
  {
    AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );
    AxOperationDef axOpDef( axOpGroup.GetOperationDef() );
    aafUID_t opDef = axOpDef.GetAUID();
    
    if ( opDef == kAAFOperationDef_VideoDissolve )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoDissolveEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_SMPTEVideoWipe )
    {
        this->DecorateNode<IAAFOperationGroup, EPSMPTEVideoWipeEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoSpeedControl )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoSpeedControlEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoRepeat )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoRepeatEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_Flip )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoFlipEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_Flop )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoFlopEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_FlipFlop )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoFlipFlopEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoPosition )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoPositionEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoCrop )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoCropEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoScale )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoScaleEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoRotate )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoRotateEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoCornerPinning )
    {
        this->DecorateNode<IAAFOperationGroup, EPVideoCornerPinningEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoAlphaWithinVideoKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPAlphaWithVideoKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoSeparateAlphaKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPSeparateAlphaKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoLuminanceKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPLuminanceKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_VideoChromaKey )
    {
        this->DecorateNode<IAAFOperationGroup, EPChromaKeyEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_MonoAudioGain )
    {
        this->DecorateNode<IAAFOperationGroup, EPMonoAudioGainEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_MonoAudioPan )
    {
        this->DecorateNode<IAAFOperationGroup, EPMonoAudioPanEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_MonoAudioDissolve )
    {
        this->DecorateNode<IAAFOperationGroup, EPSingleParameterAudioDissolveEffect>( node );
    }
    else if ( opDef == kAAFOperationDef_TwoParameterMonoAudioDissolve )
    {
        this->DecorateNode<IAAFOperationGroup, EPTwoParameterAudioDissolveEffect>( node );
    }
    
    return true;
  }
  
  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node )
  {
    AxOperationDef axOpDef( node.GetAAFObjectOfType() );
    aafUID_t opDef = axOpDef.GetAUID();

    if ( _editProtocolEffects.find( opDef ) != _editProtocolEffects.end() )
    {
        this->DecorateNode<IAAFOperationDef, EPEffect>( node );
    }
    
    return true;
  }
    
private:

  // prohibited
  EPDecoratorVisitor();
  EPDecoratorVisitor( const EPDecoratorVisitor& );
  EPDecoratorVisitor& operator=( const EPDecoratorVisitor& );
  
  template <typename AAFObjectType>
  void DecorateMobSlot ( AxMobSlot& axMobSlot, AAFTypedObjNode<AAFObjectType>& node )
  {

    AxDataDef axDataDef( axMobSlot.GetDataDef() );
    
    //Check if this is an essence track.
    if ( axDataDef.IsPictureKind() || axDataDef.IsSoundKind() )
    {
        
      boost::shared_ptr<EPAudioTrack> spAudio( EPAudioTrack::CreateAudioTrack( axMobSlot ) );
      boost::shared_ptr<EPVideoTrack> spVideo( EPVideoTrack::CreateVideoTrack( axMobSlot ) );
                
      //Now check if it is an audio or video track.
      if ( spAudio )
      {
        this->DecorateNode<AAFObjectType, EPAudioTrack>( node, spAudio );
      }
      else if ( spVideo )
      {
        this->DecorateNode<AAFObjectType, EPVideoTrack>( node, spVideo );
      }
      else
      {
        this->DecorateNode<AAFObjectType, EPEssenceTrack>( node );
      }
    }
    else
    {
      if ( axDataDef.IsTimecodeKind() )
      {
        this->DecorateNode<AAFObjectType, EPTimecodeTrack>( node );
      }
      else if ( axDataDef.IsEdgecodeKind() )
      {
        this->DecorateNode<AAFObjectType, EPEdgecodeTrack>( node );
      }
      else
      {
        this->DecorateNode<AAFObjectType, EPNonEssenceTrack>( node );
      }
    }
    
  }
  
  template <typename AAFObjectType, typename EPObjectType>
  void DecorateNode( AAFTypedObjNode<AAFObjectType>& node )
  {
    //Get a shared pointer to the node.
    boost::shared_ptr<AAFTypedObjNode<AAFObjectType> > spNode =
        boost::dynamic_pointer_cast<AAFTypedObjNode<AAFObjectType> >( 
            node.GetSharedPointerToNode() );

    //Create the EPObject
    boost::shared_ptr<EPObjectType> spEPObj( new EPObjectType() );

    //Create a decorated node.
    boost::shared_ptr<EPTypedObjNode<AAFObjectType, EPObjectType> > 
        spDecorated( new EPTypedObjNode<AAFObjectType, EPObjectType>( spNode, spEPObj ) );
        
    //Decorate the node in the graph.
    _spGraph->Decorate( spDecorated );   
  }
  
  template <typename AAFObjectType, typename EPObjectType>
  void DecorateNode( AAFTypedObjNode<AAFObjectType>& node, boost::shared_ptr<EPObjectType> spEPObj )
  {
    //Get a shared pointer to the node.
    boost::shared_ptr<AAFTypedObjNode<AAFObjectType> > spNode =
        boost::dynamic_pointer_cast<AAFTypedObjNode<AAFObjectType> >( 
            node.GetSharedPointerToNode() );

    //Create a decorated node.
    boost::shared_ptr<EPTypedObjNode<AAFObjectType, EPObjectType> > 
        spDecorated( new EPTypedObjNode<AAFObjectType, EPObjectType>( spNode, spEPObj ) );
        
    //Decorate the node in the graph.
    _spGraph->Decorate( spDecorated );   
  }

  wostream& _log;
  boost::shared_ptr<const TestGraph> _spGraph;
  set<aafUID_t> _editProtocolEffects;

};

//======================================================================

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

DecorateEPTest::DecorateEPTest( wostream& log,
                                boost::shared_ptr<const TestGraph> spGraph )
  : Test( log, GetTestInfo() )
{
    SetTestGraph( spGraph );
}

DecorateEPTest::~DecorateEPTest()
{}

boost::shared_ptr<TestLevelTestResult> DecorateEPTest::Execute()
{
  boost::shared_ptr<EPDecoratorVisitor> spVisitor(
       new EPDecoratorVisitor( GetOutStream(), GetTestGraph() ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  dfs.TraverseDown( spVisitor, GetTestGraph()->GetRootNode() );  

  return CreatePassTestResult();
}

AxString DecorateEPTest::GetName() const
{
  return TEST_NAME;
}

AxString DecorateEPTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo DecorateEPTest::GetTestInfo()
{
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // This test prepares the graph for later tests. It can't fail on
    // a requirement, hence non are registered.

    return TestInfo(L"DecorateEPTest", spReqIds);
}

} // end of namespace aafanalyzer
