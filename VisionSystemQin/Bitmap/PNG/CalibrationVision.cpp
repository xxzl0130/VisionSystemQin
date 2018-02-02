//
// This file was automatically generated from D:/Users/hjya/Desktop/f16/VisionSystemQin/VisionSystemQin/CalibrationVision.acf
//
#include "vpModule.h"
#include "vpKernel.h"
#include "vpSearchPath.h"
#include "vpPipeline.h"
#include "vpWindow.h"
#include "vpChannel.h"
#include "vpObserver.h"
#include "vpScene.h"
#include "vpTransform.h"
#include "vpObject.h"
#include "vpRecyclingService.h"
#include "vpEnvGlobals.h"
#include "vpEnv.h"
#include "vpEnvSun.h"
#include "vpEnvMoon.h"
#include "vpEnvSkyDome.h"
#include "vpEnvCloudLayer.h"
#include "vpEnvStars.h"
#include "vpEnvWind.h"
#include "vpApp.h"

//
// Define a static list to hold the instances we create, so that we 
// can unref them when unconfigure is called
//
typedef vuVector< vuField< vuBase* > > InstancesList;
static InstancesList* s_pInstancesToUnref = NULL;



void define( void )
{
    // Create static instances list that we use to track what we create
    s_pInstancesToUnref = new InstancesList;

    //
    // Initialize all modules
    //

    vpModule::initializeModule( "vpEnv" );


    //
    // Create all instances
    //

    vpKernel* pKernel_myKernel = vpKernel::instance();
    pKernel_myKernel->setNotifyLevel( vuNotify::LEVEL_WARN );
    pKernel_myKernel->setNotifyColorEnable( false );
    pKernel_myKernel->setPriority( vuThread::PRIORITY_NORMAL );
    pKernel_myKernel->setProcessor( -1 );
    pKernel_myKernel->setFrameRateLimiterEnable( true );
    pKernel_myKernel->setDesiredFrameRate( 0 );
    pKernel_myKernel->setNotifyClassTypeMode( vuNotify::CLASS_TYPE_MODE_INCLUSIVE );

    vpSearchPath* pSearchPath_mySearchPath = vpSearchPath::instance();
    pSearchPath_mySearchPath->append( "$(PRESAGIS_VEGA_PRIME_RESOURCE_4)/resources/data/databases/camp_pendleton/" );
    pSearchPath_mySearchPath->append( "D:/Users/hjya/Desktop/f16" );

    vpPipeline* pPipeline_myPipeline = new vpPipeline();
    pPipeline_myPipeline->setName( "myPipeline" );
    pPipeline_myPipeline->setMultiThread( vsPipeline::MULTITHREAD_INLINE );
    pPipeline_myPipeline->setId( 0 );
    pPipeline_myPipeline->setNumCullThreads( 0 );
    pPipeline_myPipeline->setCullThreadPriority( vuThread::PRIORITY_NORMAL );
    pPipeline_myPipeline->setCullThreadProcessor( -1 );
    pPipeline_myPipeline->setDrawThreadPriority( vuThread::PRIORITY_NORMAL );
    pPipeline_myPipeline->setDrawThreadProcessor( -1 );
    pPipeline_myPipeline->setBeginFrameOnVsyncEnable( false );
    pPipeline_myPipeline->setDesiredPostDrawTime( -1 );
    pPipeline_myPipeline->setTextureSubloadOnPreDrawEnable( false );
    pPipeline_myPipeline->setDesiredTextureSubloadOnPreDrawTime( -1 );

    s_pInstancesToUnref->push_back( pPipeline_myPipeline );

    vpWindow* pWindow_myWindow = new vpWindow();
    pWindow_myWindow->setName( "myWindow" );
    pWindow_myWindow->setLabel( "Vega Prime Window" );
    pWindow_myWindow->setOrigin( 30 ,  150 );
    pWindow_myWindow->setSize( 1024 ,  768 );
    pWindow_myWindow->setFullScreenEnable( false );
    pWindow_myWindow->setBorderEnable( false );
    pWindow_myWindow->setInputEnable( true );
    pWindow_myWindow->setCursorEnable( true );
    pWindow_myWindow->setStereoEnable( false );
    pWindow_myWindow->setNumColorBits( 8 );
    pWindow_myWindow->setNumAlphaBits( 8 );
    pWindow_myWindow->setNumDepthBits( 24 );
    pWindow_myWindow->setNumStencilBits( 0 );
    pWindow_myWindow->setNumAccumColorBits( 0 );
    pWindow_myWindow->setNumAccumAlphaBits( 0 );
    pWindow_myWindow->setNumMultiSampleBits( 4 );
    pWindow_myWindow->setSwapInterval( 1 );
    pWindow_myWindow->setPixelBufferMode( vrDrawContext::PIXEL_BUFFER_MODE_OFF );

    s_pInstancesToUnref->push_back( pWindow_myWindow );

    vpWindow* pWindow_myWindow1 = new vpWindow();
    pWindow_myWindow1->setName( "myWindow1" );
    pWindow_myWindow1->setLabel( "" );
    pWindow_myWindow1->setOrigin( 0 ,  0 );
    pWindow_myWindow1->setSize( 1024 ,  768 );
    pWindow_myWindow1->setFullScreenEnable( false );
    pWindow_myWindow1->setBorderEnable( true );
    pWindow_myWindow1->setInputEnable( true );
    pWindow_myWindow1->setCursorEnable( true );
    pWindow_myWindow1->setStereoEnable( false );
    pWindow_myWindow1->setNumColorBits( 8 );
    pWindow_myWindow1->setNumAlphaBits( 0 );
    pWindow_myWindow1->setNumDepthBits( 24 );
    pWindow_myWindow1->setNumStencilBits( 0 );
    pWindow_myWindow1->setNumAccumColorBits( 0 );
    pWindow_myWindow1->setNumAccumAlphaBits( 0 );
    pWindow_myWindow1->setNumMultiSampleBits( 0 );
    pWindow_myWindow1->setSwapInterval( 1 );
    pWindow_myWindow1->setPixelBufferMode( vrDrawContext::PIXEL_BUFFER_MODE_OFF );

    s_pInstancesToUnref->push_back( pWindow_myWindow1 );

    vpChannel* pChannel_myChannel = new vpChannel();
    pChannel_myChannel->setName( "myChannel" );
    pChannel_myChannel->setOffsetTranslate( 0 ,  0 ,  0 );
    pChannel_myChannel->setOffsetRotate( 0 ,  0 ,  0 );
    pChannel_myChannel->setCullMask( 0x0FFFFFFFF );
    pChannel_myChannel->setRenderMask( 0x0FFFFFFFF );
    pChannel_myChannel->setClearColor( 0.000000f ,  0.500000f ,  1.000000f ,  0.000000f );
    pChannel_myChannel->setClearBuffers( 0x03 );
    pChannel_myChannel->setDrawArea( 0 ,  1 ,  0 ,  1 );
    pChannel_myChannel->setFOVSymmetric( 45.000000f ,  -1.000000f );
    pChannel_myChannel->setNearFar( 1.000000f ,  35000.000000f );
    pChannel_myChannel->setLODVisibilityRangeScale( 1 );
    pChannel_myChannel->setLODTransitionRangeScale( 1 );
    pChannel_myChannel->setFOVScale( -1 );
    pChannel_myChannel->setCullThreadPriority( vuThread::PRIORITY_NORMAL );
    pChannel_myChannel->setCullThreadProcessor( -1 );
    pChannel_myChannel->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_WIREFRAME ,  false );
    pChannel_myChannel->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_TRANSPARENCY ,  true );
    pChannel_myChannel->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_TEXTURE ,  true );
    pChannel_myChannel->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_LIGHT ,  true );
    pChannel_myChannel->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_FOG ,  true );
    pChannel_myChannel->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_SHADERS ,  true );
    pChannel_myChannel->setLightPointThreadPriority( vuThread::PRIORITY_NORMAL );
    pChannel_myChannel->setLightPointThreadProcessor( -1 );
    pChannel_myChannel->setMultiSample( vpChannel::MULTISAMPLE_ANTIALIAS );
    pChannel_myChannel->setStatisticsPage( vpChannel::PAGE_OFF );
    pChannel_myChannel->setCullBoundingBoxTestEnable( false );
    pChannel_myChannel->setOpaqueSort( vpChannel::OPAQUE_SORT_TEXTURE ,  vpChannel::OPAQUE_SORT_MATERIAL );
    pChannel_myChannel->setTransparentSort( vpChannel::TRANSPARENT_SORT_DEPTH );
    pChannel_myChannel->setDrawBuffer( vpChannel::DRAW_BUFFER_DEFAULT );
    pChannel_myChannel->setStressEnable( false );
    pChannel_myChannel->setStressParameters( 1 ,  20 ,  0.75 ,  0.5 ,  2 );

    s_pInstancesToUnref->push_back( pChannel_myChannel );

    vpChannel* pChannel_myChannel1 = new vpChannel();
    pChannel_myChannel1->setName( "myChannel1" );
    pChannel_myChannel1->setOffsetTranslate( 0 ,  0 ,  0 );
    pChannel_myChannel1->setOffsetRotate( 0 ,  0 ,  0 );
    pChannel_myChannel1->setCullMask( 0x0FFFFFFFF );
    pChannel_myChannel1->setRenderMask( 0x0FFFFFFFF );
    pChannel_myChannel1->setClearColor( 0.000000f ,  0.500000f ,  1.000000f ,  0.000000f );
    pChannel_myChannel1->setClearBuffers( 0x03 );
    pChannel_myChannel1->setDrawArea( 0 ,  1 ,  0 ,  1 );
    pChannel_myChannel1->setFOVSymmetric( 45.000000f ,  -1.000000f );
    pChannel_myChannel1->setNearFar( 1.000000f ,  35000.000000f );
    pChannel_myChannel1->setLODVisibilityRangeScale( 1 );
    pChannel_myChannel1->setLODTransitionRangeScale( 1 );
    pChannel_myChannel1->setFOVScale( -1 );
    pChannel_myChannel1->setZoom( 1 );
    pChannel_myChannel1->setCullThreadPriority( vuThread::PRIORITY_NORMAL );
    pChannel_myChannel1->setCullThreadProcessor( -1 );
    pChannel_myChannel1->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_WIREFRAME ,  false );
    pChannel_myChannel1->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_TRANSPARENCY ,  true );
    pChannel_myChannel1->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_TEXTURE ,  true );
    pChannel_myChannel1->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_LIGHT ,  true );
    pChannel_myChannel1->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_FOG ,  true );
    pChannel_myChannel1->setGraphicsModeEnable( vpChannel::GRAPHICS_MODE_SHADERS ,  true );
    pChannel_myChannel1->setLightPointThreadPriority( vuThread::PRIORITY_NORMAL );
    pChannel_myChannel1->setLightPointThreadProcessor( -1 );
    pChannel_myChannel1->setMultiSample( vpChannel::MULTISAMPLE_OFF );
    pChannel_myChannel1->setStatisticsPage( vpChannel::PAGE_OFF );
    pChannel_myChannel1->setCullBoundingBoxTestEnable( false );
    pChannel_myChannel1->setOpaqueSort( vpChannel::OPAQUE_SORT_TEXTURE ,  vpChannel::OPAQUE_SORT_SHADER_PROGRAM );
    pChannel_myChannel1->setTransparentSort( vpChannel::TRANSPARENT_SORT_DEPTH );
    pChannel_myChannel1->setDrawBuffer( vpChannel::DRAW_BUFFER_DEFAULT );
    pChannel_myChannel1->setStressEnable( false );
    pChannel_myChannel1->setStressParameters( 1 ,  20 ,  0.75 ,  0.5 ,  2 );
    pChannel_myChannel1->setRenderPriority( 0 );
    pChannel_myChannel1->setRepresentationIndex( 0 );

    s_pInstancesToUnref->push_back( pChannel_myChannel1 );

    vpObserver* pObserver_myObserver = new vpObserver();
    pObserver_myObserver->setName( "myObserver" );
    pObserver_myObserver->setStrategyEnable( false );
    pObserver_myObserver->setTranslate( -1297.13 ,  -1025.92 ,  343.347 );
    pObserver_myObserver->setRotate( 292 ,  -7 ,  0 );
    pObserver_myObserver->setLatencyCriticalEnable( false );

    s_pInstancesToUnref->push_back( pObserver_myObserver );

    vpScene* pScene_myScene = new vpScene();
    pScene_myScene->setName( "myScene" );

    s_pInstancesToUnref->push_back( pScene_myScene );

    vpTransform* pTransform_farTransform = new vpTransform();
    pTransform_farTransform->setName( "farTransform" );
    pTransform_farTransform->setCullMask( 0x0FFFFFFFF );
    pTransform_farTransform->setRenderMask( 0x0FFFFFFFF );
    pTransform_farTransform->setIsectMask( 0x0FFFFFFFF );
    pTransform_farTransform->setStrategyEnable( true );
    pTransform_farTransform->setTranslate( 0 ,  -5 ,  0 );
    pTransform_farTransform->setRotate( 0 ,  0 ,  0 );
    pTransform_farTransform->setScale( 1 ,  1 ,  1 );
    pTransform_farTransform->setStaticEnable( false );

    s_pInstancesToUnref->push_back( pTransform_farTransform );

    vpObject* pObject_myObject = new vpObject();
    pObject_myObject->setName( "myObject" );
    pObject_myObject->setCullMask( 0x0FFFFFFFF );
    pObject_myObject->setRenderMask( 0x0FFFFFFFF );
    pObject_myObject->setIsectMask( 0x0FFFFFFFF );
    pObject_myObject->setStrategyEnable( false );
    pObject_myObject->setTranslate( 0 ,  0 ,  0 );
    pObject_myObject->setRotate( 0 ,  0 ,  0 );
    pObject_myObject->setScale( 1 ,  1 ,  1 );
    pObject_myObject->setStaticEnable( false );
    pObject_myObject->setFileName( "camp_pendleton.flt" );
    pObject_myObject->setAutoPage( vpObject::AUTO_PAGE_SYNCHRONOUS );
    pObject_myObject->setManualLODChild( -1 );
    pObject_myObject->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_PRESERVE_LIGHT_SOURCES ,  false );
    pObject_myObject->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_PRESERVE_SIGNIFICANT_SIZE ,  false );
    pObject_myObject->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_SHARE_LIGHT_POINT_CONTROLS ,  true );
    pObject_myObject->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_SHARE_LIGHT_POINT_ANIMATIONS ,  true );
    pObject_myObject->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_SHARE_LIGHT_POINT_APPEARANCES ,  true );
    pObject_myObject->setLoaderDetailMultiTextureStage( -1 );
    pObject_myObject->setLoaderBlendTolerance( 0.050000f );
    pObject_myObject->setLoaderUnits( vsNodeLoader::Data::LOADER_UNITS_METERS );
    pObject_myObject->setBuilderNormalMode( vsNodeLoader::Data::BUILDER_NORMAL_MODE_PRESERVE );
    pObject_myObject->setBuilderTangentMode( vsNodeLoader::Data::BUILDER_TANGENT_MODE_GENERATE_PER_VERTEX_PER_PRIMITIVE_AND_ANGLE_WEIGHTED );
    pObject_myObject->setBuilderTangentGenerationMode( vsNodeLoader::Data::BUILDER_TANGENT_GENERATION_MODE_GENERATE_AS_NEEDED );
    pObject_myObject->setBuilderColorTolerance( 0.001000f );
    pObject_myObject->setBuilderNormalTolerance( 0.860000f );
    pObject_myObject->setBuilderVertexTolerance( 0.000010f );
    pObject_myObject->setGeometryFormat( vrGeometryBase::FORMAT_VERTEX_BUFFER_OBJECT ,  0x0FFF );
    pObject_myObject->setTextureSubloadEnable( false );
    pObject_myObject->setTextureSubloadRender( vpGeometry::TEXTURE_SUBLOAD_RENDER_DEFERRED );
    pObject_myObject->setTexturePreserveImageLevelsEnable( true );

    s_pInstancesToUnref->push_back( pObject_myObject );

    vpObject* pObject_ChessBoard = new vpObject();
    pObject_ChessBoard->setName( "ChessBoard" );
    pObject_ChessBoard->setCullMask( 0x0FFFFFFFF );
    pObject_ChessBoard->setRenderMask( 0x0FFFFFFFF );
    pObject_ChessBoard->setIsectMask( 0x0FFFFFFFF );
    pObject_ChessBoard->setStrategyEnable( true );
    pObject_ChessBoard->setTranslate( -500 ,  -1000 ,  150 );
    pObject_ChessBoard->setRotate( 0 ,  0 ,  0 );
    pObject_ChessBoard->setScale( 1 ,  1 ,  1 );
    pObject_ChessBoard->setStaticEnable( false );
    pObject_ChessBoard->setFileName( "ChessBoard.flt" );
    pObject_ChessBoard->setAutoPage( vpObject::AUTO_PAGE_SYNCHRONOUS );
    pObject_ChessBoard->setManualLODChild( -1 );
    pObject_ChessBoard->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_PRESERVE_LIGHT_SOURCES ,  false );
    pObject_ChessBoard->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_PRESERVE_SIGNIFICANT_SIZE ,  false );
    pObject_ChessBoard->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_SHARE_LIGHT_POINT_CONTROLS ,  true );
    pObject_ChessBoard->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_SHARE_LIGHT_POINT_ANIMATIONS ,  true );
    pObject_ChessBoard->setLoaderOption( vsNodeLoader::Data::LOADER_OPTION_SHARE_LIGHT_POINT_APPEARANCES ,  true );
    pObject_ChessBoard->setLoaderDetailMultiTextureStage( -1 );
    pObject_ChessBoard->setLoaderBlendTolerance( 0.050000f );
    pObject_ChessBoard->setLoaderUnits( vsNodeLoader::Data::LOADER_UNITS_METERS );
    pObject_ChessBoard->setBuilderNormalMode( vsNodeLoader::Data::BUILDER_NORMAL_MODE_PRESERVE );
    pObject_ChessBoard->setBuilderTangentMode( vsNodeLoader::Data::BUILDER_TANGENT_MODE_GENERATE_PER_VERTEX_PER_PRIMITIVE_AND_ANGLE_WEIGHTED );
    pObject_ChessBoard->setBuilderTangentGenerationMode( vsNodeLoader::Data::BUILDER_TANGENT_GENERATION_MODE_GENERATE_AS_NEEDED );
    pObject_ChessBoard->setBuilderColorTolerance( 0.001000f );
    pObject_ChessBoard->setBuilderNormalTolerance( 0.860000f );
    pObject_ChessBoard->setBuilderVertexTolerance( 0.000000f );
    pObject_ChessBoard->setGeometryFormat( vrGeometryBase::FORMAT_VERTEX_BUFFER_OBJECT ,  0x0FFF );
    pObject_ChessBoard->setTextureSubloadEnable( false );
    pObject_ChessBoard->setTextureSubloadRender( vpGeometry::TEXTURE_SUBLOAD_RENDER_DEFERRED );
    pObject_ChessBoard->setTexturePreserveImageLevelsEnable( true );

    s_pInstancesToUnref->push_back( pObject_ChessBoard );

    vpRecyclingService* pRecyclingService_myRecyclingService = vpRecyclingService::instance();
    pRecyclingService_myRecyclingService->setMultiThread( vpRecyclingService::MULTITHREAD_INLINE );
    pRecyclingService_myRecyclingService->setThreadPriority( vuThread::PRIORITY_NORMAL );
    pRecyclingService_myRecyclingService->setThreadProcessor( -1 );
    pRecyclingService_myRecyclingService->setRecycleTime( -1 );

    vpEnvGlobals* pEnvGlobals_myEnvGlobals = vpEnvGlobals::instance();
    pEnvGlobals_myEnvGlobals->setSkyLightEnable( true );
    pEnvGlobals_myEnvGlobals->setDominantCelestialLightingEnable( true );
    pEnvGlobals_myEnvGlobals->setLocalViewerEnable( false );
    pEnvGlobals_myEnvGlobals->setTwoSidedLightingEnable( false );
    pEnvGlobals_myEnvGlobals->setEnvironmentMapGenerationEnable( false );

    vpEnv* pEnv_myEnv = new vpEnv();
    pEnv_myEnv->setName( "myEnv" );
    pEnv_myEnv->setDate( 1 ,  9 ,  2006 );
    pEnv_myEnv->setTimeOfDay( 9.250000f );
    pEnv_myEnv->setTimeMultiplier( 1.000000f );
    pEnv_myEnv->setEphemerisUpdateInterval( 0.000000f );
    pEnv_myEnv->setReferencePosition( -117.355103f ,  33.301300f );
    pEnv_myEnv->setEphemerisTimeZoneOffset( -10000 );
    pEnv_myEnv->setSkyColor( 0.513725f ,  0.701961f ,  0.941176f ,  1.000000f );
    pEnv_myEnv->setVisibilityType( vpEnvFx::VISIBILITY_TYPE_VERTEX_EXP2 );
    pEnv_myEnv->setVisibilityRangeOnset( 0.000000f );
    pEnv_myEnv->setVisibilityRangeOpaque( 60000.000000f );
    pEnv_myEnv->setVisibilityColor( 0.764706f ,  0.858824f ,  1.000000f ,  1.000000f );
    pEnv_myEnv->setHazeColor( 0.772549f ,  0.772549f ,  0.772549f ,  0.445397f );
    pEnv_myEnv->setManageVisibilityColorsEnable( true );
    pEnv_myEnv->setAmbientLightingColor( 0.000000f ,  0.000000f ,  0.000000f ,  1.000000f );
    pEnv_myEnv->setLightBrightnessScale( vpEnv::COLOR_AMBIENT ,  0.500000f );
    pEnv_myEnv->setLightBrightnessScale( vpEnv::COLOR_DIFFUSE ,  0.500000f );
    pEnv_myEnv->setLightBrightnessScale( vpEnv::COLOR_SPECULAR ,  1.000000f );
    pEnv_myEnv->setLightColorScale( vpEnv::COLOR_AMBIENT ,  1.000000f ,  1.000000f ,  1.000000f );
    pEnv_myEnv->setLightColorScale( vpEnv::COLOR_DIFFUSE ,  1.000000f ,  1.000000f ,  1.000000f );
    pEnv_myEnv->setLightColorScale( vpEnv::COLOR_SPECULAR ,  1.000000f ,  1.000000f ,  1.000000f );

    s_pInstancesToUnref->push_back( pEnv_myEnv );

    vpEnvSun* pEnvSun_myEnvSun = new vpEnvSun();
    pEnvSun_myEnvSun->setName( "myEnvSun" );
    pEnvSun_myEnvSun->setEnable( true );
    pEnvSun_myEnvSun->setTwilightDip( -18.000000f );
    pEnvSun_myEnvSun->setGeometryEnable( true );
    pEnvSun_myEnvSun->setTextureFile( "sun.inta" );
    pEnvSun_myEnvSun->setTextureBlendColor( 1.000000f ,  1.000000f ,  1.000000f ,  1.000000f );
    pEnvSun_myEnvSun->setTextureBlendMode( vpEnvSun::TEXTURE_BLEND_MODE_MODULATE );
    pEnvSun_myEnvSun->setColor( 0.992156f ,  1.000000f ,  0.949019f ,  1.000000f );
    pEnvSun_myEnvSun->setVisibilityRangeRatio( 0.050000f );
    pEnvSun_myEnvSun->setAngularSize( 5.500000f );
    pEnvSun_myEnvSun->setHorizonAngle( 40.000000f );
    pEnvSun_myEnvSun->setHorizonColor( 1.000000f ,  0.545098f ,  0.239216f ,  1.000000f );
    pEnvSun_myEnvSun->setHorizonSizeScale( 2.000000f );
    pEnvSun_myEnvSun->setHorizonLightColorScale( 0.250000f );

    s_pInstancesToUnref->push_back( pEnvSun_myEnvSun );

    vpEnvMoon* pEnvMoon_myEnvMoon = new vpEnvMoon();
    pEnvMoon_myEnvMoon->setName( "myEnvMoon" );
    pEnvMoon_myEnvMoon->setEnable( true );
    pEnvMoon_myEnvMoon->setBrightness( 0.250000f );
    pEnvMoon_myEnvMoon->setGeometryEnable( true );
    pEnvMoon_myEnvMoon->setTextureFile( "moon.inta" );
    pEnvMoon_myEnvMoon->setTextureBlendColor( 1.000000f ,  1.000000f ,  1.000000f ,  1.000000f );
    pEnvMoon_myEnvMoon->setTextureBlendMode( vpEnvMoon::TEXTURE_BLEND_MODE_MODULATE );
    pEnvMoon_myEnvMoon->setColor( 0.811765f ,  0.886275f ,  0.937255f ,  1.000000f );
    pEnvMoon_myEnvMoon->setVisibilityRangeRatio( 0.100000f );
    pEnvMoon_myEnvMoon->setAngularSize( 1.500000f );
    pEnvMoon_myEnvMoon->setHorizonAngle( 30.000000f );
    pEnvMoon_myEnvMoon->setHorizonColor( 0.811765f ,  0.886275f ,  0.937255f ,  1.000000f );
    pEnvMoon_myEnvMoon->setHorizonSizeScale( 1.700000f );
    pEnvMoon_myEnvMoon->setHorizonLightColorScale( 0.000000f );

    s_pInstancesToUnref->push_back( pEnvMoon_myEnvMoon );

    vpEnvSkyDome* pEnvSkyDome_myEnvSkyDome = new vpEnvSkyDome();
    pEnvSkyDome_myEnvSkyDome->setName( "myEnvSkyDome" );
    pEnvSkyDome_myEnvSkyDome->setEnable( true );
    pEnvSkyDome_myEnvSkyDome->setGroundColor( 0.211765f ,  0.286275f ,  0.149020f ,  1.000000f );
    pEnvSkyDome_myEnvSkyDome->setGroundEnable( true );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 0 ,  0.100000f );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 1 ,  0.300000f );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 2 ,  0.750000f );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 3 ,  0.900000f );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 4 ,  1.000000f );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 5 ,  1.000000f );
    pEnvSkyDome_myEnvSkyDome->setVisibilityRangeRatio( 6 ,  1.000000f );

    s_pInstancesToUnref->push_back( pEnvSkyDome_myEnvSkyDome );

    vpEnvCloudLayer* pEnvCloudLayer_myEnvCloudLayer = new vpEnvCloudLayer();
    pEnvCloudLayer_myEnvCloudLayer->setName( "myEnvCloudLayer" );
    pEnvCloudLayer_myEnvCloudLayer->setEnable( true );
    pEnvCloudLayer_myEnvCloudLayer->setColor( 0.300000f ,  0.300000f ,  0.300000f ,  1.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setElevation( 5000.000000f ,  6000.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setTransitionRange( 500.000000f ,  500.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setScudEnable( false );
    pEnvCloudLayer_myEnvCloudLayer->setTextureFile( "" );
    pEnvCloudLayer_myEnvCloudLayer->setTextureBlendColor( 1.000000f ,  1.000000f ,  1.000000f ,  1.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setTextureBlendMode( vpEnvCloudLayer::TEXTURE_BLEND_MODE_MODULATE );
    pEnvCloudLayer_myEnvCloudLayer->setTextureTiling( 2.000000f ,  2.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setMinVisibilityScale( 0.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setHorizonColorScale( 1.000000f );
    pEnvCloudLayer_myEnvCloudLayer->setCoverageScale( 0.250000f );
    pEnvCloudLayer_myEnvCloudLayer->setShadowsEnable( false );
    pEnvCloudLayer_myEnvCloudLayer->setShadowsColorPunchThrough( 0.300000f );
    pEnvCloudLayer_myEnvCloudLayer->setShadowIntensity( 0.600000f );
    pEnvCloudLayer_myEnvCloudLayer->setShadowsContribution( 0.100000f ,  0.900000f );

    s_pInstancesToUnref->push_back( pEnvCloudLayer_myEnvCloudLayer );

    vpEnvStars* pEnvStars_myEnvStars = new vpEnvStars();
    pEnvStars_myEnvStars->setName( "myEnvStars" );
    pEnvStars_myEnvStars->setEnable( true );
    pEnvStars_myEnvStars->setIntensityThreshold( 0.350000f );
    pEnvStars_myEnvStars->setBrightnessThreshold( 0.400000f ,  0.250000f );
    pEnvStars_myEnvStars->setVisibilityRangeRatio( 0.100000f );
    pEnvStars_myEnvStars->setPointSize( 2.300000f );

    s_pInstancesToUnref->push_back( pEnvStars_myEnvStars );

    vpEnvWind* pEnvWind_myEnvWind = new vpEnvWind();
    pEnvWind_myEnvWind->setName( "myEnvWind" );
    pEnvWind_myEnvWind->setEnable( true );
    pEnvWind_myEnvWind->setSpeed( 100.000000f );
    pEnvWind_myEnvWind->setDirection( 0.000000f ,  1.000000f ,  0.000000f );

    s_pInstancesToUnref->push_back( pEnvWind_myEnvWind );



    //
    // Satisfy all references
    //



    pPipeline_myPipeline->addWindow( pWindow_myWindow );

    pWindow_myWindow->addChannel( pChannel_myChannel );

    pWindow_myWindow1->addChannel( pChannel_myChannel1 );



    pObserver_myObserver->addChannel( pChannel_myChannel );
    pObserver_myObserver->addChannel( pChannel_myChannel1 );
    pObserver_myObserver->addAttachment( pEnv_myEnv );
    pObserver_myObserver->setScene( pScene_myScene );
    pObserver_myObserver->setLookFrom( pTransform_farTransform );

    pScene_myScene->addChild( pObject_myObject );
    pScene_myScene->addChild( pObject_ChessBoard );



    pObject_ChessBoard->addChild( pTransform_farTransform );



    pEnv_myEnv->addEnvFx( pEnvSun_myEnvSun );
    pEnv_myEnv->addEnvFx( pEnvMoon_myEnvMoon );
    pEnv_myEnv->addEnvFx( pEnvSkyDome_myEnvSkyDome );
    pEnv_myEnv->addEnvFx( pEnvCloudLayer_myEnvCloudLayer );
    pEnv_myEnv->addEnvFx( pEnvWind_myEnvWind );
    pEnv_myEnv->addEnvFx( pEnvStars_myEnvStars );







}




void unconfigure( void )
{
    //
    // Unref all created instances
    //

    s_pInstancesToUnref->clear();
    delete s_pInstancesToUnref;
}





//
// Main Program
//

int main(int argc, char *argv[])
{
    // initialize Vega Prime
    vp::initialize(argc, argv);

    // create my app instance
    vpApp *app = new vpApp;

    // use the code generated by LynX Prime
    define();

    // configure my app instance
    app->configure();

    // execute the main runtime loop
    app->run();

    // call the unconfig code generated by LynX Prime
    unconfigure();

    // delete my app instance
    app->unref();

    // shutdown Vega Prime
    vp::shutdown();

    return 0;

}

