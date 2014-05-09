/*
  OMX_Symbian_VideoExt.h
  
  Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
  All rights reserved.

  This program and the accompanying materials are made available 
  under the terms of the Eclipse Public License v1.0 which accompanies 
  this distribution, and is available at 
  http://www.eclipse.org/legal/epl-v10.html

  Initial Contributors:
  Nokia Corporation - initial contribution.
*/

/** @file
@brief Symbian OpenMAX IL Extension Data Structures in the Video Domain.

This file contains the extension structures for the Symbian IL extensions that
pertain to video components.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_VideoExt_h
#define OMX_Symbian_VideoExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_Video.h>
#else
#include <OMX_Types.h>
#include <OMX_Video.h>
#endif


/** Video format extensions. */
typedef enum OMX_SYMBIAN_VIDEO_CODINGTYPE {
    OMX_SYMBIAN_VIDEO_CodingVC1 = OMX_VIDEO_CodingVendorStartUnused + 0x1,
    OMX_SYMBIAN_VIDEO_CodingVP6,
    OMX_SYMBIAN_VIDEO_CodingMax = 0x7FFFFFFF
} OMX_SYMBIAN_VIDEO_CODINGTYPE;

/** Sequence Headers.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Video.SequenceHeader".
 */
typedef struct OMX_SYMBIAN_VIDEO_PARAM_SEQUENCEHEADERTYPE
{ 
    OMX_U32 nSize;                  /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_U32 nPortIndex;             /**< Port that this structure applies to */
    OMX_U32 nSequenceHeaderSize;    /**< Size of the sequence header in bytes */
    OMX_U8 SequenceHeader[1];       /**< Sequence header. VOS, VO and VOL for MPEG-4. SPS and PPS for AVC */
} OMX_SYMBIAN_VIDEO_PARAM_SEQUENCEHEADERTYPE;


typedef enum OMX_SYMBIAN_VIDEO_AVCSEITYPE {
    OMX_SYMBIAN_VIDEO_AvcSeiBufferinPeriod = 0x01,
    OMX_SYMBIAN_VIDEO_AvcSeiPictureTiming = 0x02,
    OMX_SYMBIAN_VIDEO_AvcSeiPanScanRectangle = 0x04,
    OMX_SYMBIAN_VIDEO_AvcSeiFillerPayload = 0x08,
    OMX_SYMBIAN_VIDEO_AvcSeiUserDataRegisteredByITUT35 = 0x10,
    OMX_SYMBIAN_VIDEO_AvcSeiUserDataUnregistered = 0x20,
    OMX_SYMBIAN_VIDEO_AvcSeiRecoveryPoint = 0x40,
    OMX_SYMBIAN_VIDEO_AvcSeiDecReferencePictureMarkingRepetition = 0x80,
    OMX_SYMBIAN_VIDEO_AvcSeiSparePicture = 0x100,
    OMX_SYMBIAN_VIDEO_AvcSeiSceneInformation = 0x200,
    OMX_SYMBIAN_VIDEO_AvcSeiSubSequenceInformation = 0x400,
    OMX_SYMBIAN_VIDEO_AvcSeiSubSequenceLayerCharacteristics = 0x800,
    OMX_SYMBIAN_VIDEO_AvcSeiSubSequenceCharacteristics = 0x1000,
    OMX_SYMBIAN_VIDEO_AvcSeiFullFrameFreeze = 0x2000,
    OMX_SYMBIAN_VIDEO_AvcSeiFullFrameFreezeRelease = 0x4000,
    OMX_SYMBIAN_VIDEO_AvcSeiFullFrameSnaphot = 0x8000,
    OMX_SYMBIAN_VIDEO_AvcSeiProgressiveRefinementSegmentStart = 0x10000,
    OMX_SYMBIAN_VIDEO_AvcSeiProgressiveRefinementSegmentEnd = 0x20000,
    OMX_SYMBIAN_VIDEO_AvcSeiMotionContstrainedSliceGroup = 0x40000,
    OMX_SYMBIAN_VIDEO_AvcSeiFilmGrainCharacteristics = 0x80000,
    OMX_SYMBIAN_VIDEO_AvcSeiDeblockingFilterDispPreference = 0x100000,
    OMX_SYMBIAN_VIDEO_AvcSeiStereoVideoInformation = 0x200000,
    OMX_SYMBIAN_VIDEO_AvcSeiMax = 0x7FFFFFFF
} OMX_SYMBIAN_VIDEO_AVCSEITYPE;

/** SEI messages.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Video.AVCSei".
*/
typedef struct OMX_SYMBIAN_VIDEO_PARAM_AVCSEITYPE
{
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BOOL bEnableSEI;        /**< Enable SEI messages */
    OMX_SYMBIAN_VIDEO_AVCSEITYPE eSEI;  /**< Requested SEI messages. Several messages can be requested. */
} OMX_SYMBIAN_VIDEO_PARAM_AVCSEITYPE;

/** AVC output order.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Video.AVCOutputOrder".
*/
typedef struct OMX_SYMBIAN_VIDEO_PARAM_AVCOUTPUTORDERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bOutputInDisplayOrder;
} OMX_SYMBIAN_VIDEO_PARAM_AVCOUTPUTORDERTYPE;


/** Refresh mode.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Video.RefreshMode".
*/
typedef struct OMX_SYMBIAN_VIDEO_CONFIG_REFRESHMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bRefreshLatestOutputFrame;
} OMX_SYMBIAN_VIDEO_CONFIG_REFRESHMODETYPE;

/** VC1 profile enum. */
typedef enum OMX_SYMBIAN_VIDEO_VC1PROFILETYPE {
    OMX_SYMBIAN_VIDEO_VC1ProfileSimple,
    OMX_SYMBIAN_VIDEO_VC1ProfileMain,
    OMX_SYMBIAN_VIDEO_VC1ProfileAdvanced,
    OMX_SYMBIAN_VIDEO_VC1ProfileMax = 0x7FFFFFFF
} OMX_SYMBIAN_VIDEO_VC1PROFILETYPE;

/** VC1 level enum. */
typedef enum OMX_SYMBIAN_VIDEO_VC1LEVELTYPE {
    OMX_SYMBIAN_VIDEO_VC1LevelLow,
    OMX_SYMBIAN_VIDEO_VC1LevelMedium,
    OMX_SYMBIAN_VIDEO_VC1LevelHigh,
    OMX_SYMBIAN_VIDEO_VC1LevelL0,
    OMX_SYMBIAN_VIDEO_VC1LevelL1,
    OMX_SYMBIAN_VIDEO_VC1LevelL2,
    OMX_SYMBIAN_VIDEO_VC1LevelL3,
    OMX_SYMBIAN_VIDEO_VC1LevelL4,
    OMX_SYMBIAN_VIDEO_VC1LevelMax = 0x7FFFFFFF
} OMX_SYMBIAN_VIDEO_VC1LEVELTYPE;

/** VC1 parameters for simple and main profiles. */
typedef struct OMX_SYMBIAN_VIDEO_VC1SIMPLEMAINPROFILETYPE {
    OMX_U32 nHRDBufferSize;
    OMX_U32 nHRDBufferRate;
    OMX_BOOL bEnableLoopFilter;
    OMX_BOOL bEnableMultiRes;
    OMX_BOOL bEnableFastUVMC;
    OMX_BOOL bEnableExtendedMV;
    OMX_U32 bDQuant;
    OMX_BOOL bEnableVSTransform;
    OMX_BOOL bEnableOverlap;
    OMX_BOOL bEnableSyncMarker;
    OMX_BOOL bEnableRangeRed;
    OMX_U32 nMaxBFrames;
    OMX_U32 bQuantizer;
} OMX_SYMBIAN_VIDEO_VC1SIMPLEMAINPROFILETYPE;

/** VC1 parameters for the advaanced profile. */
typedef struct OMX_SYMBIAN_VIDEO_VC1ADVANCEDPROFILETYPE {
    OMX_U32 nColorDifferenceFormat;
    OMX_BOOL bPostProcFlag;
    OMX_U32 nMaxCodedWidth;
    OMX_U32 nMaxCodedHeight;
    OMX_BOOL bPullDown;
    OMX_BOOL bInterlace;
    OMX_BOOL bTFCounterFlag;
    OMX_BOOL bProgressiveSegmentedFrame;
    OMX_BOOL bDisplayExtensionFlag;
    OMX_U32 nDisplayHorizontalSize;
    OMX_U32 nDisplayVerticalSize;
    OMX_BOOL bAspectRatioFlag;
    OMX_U32 nAspectRatio;
    OMX_U32 nAspectHorizontalSize;
    OMX_U32 nAspectVerticalSize;
    OMX_BOOL bFrameRateFlag;
    OMX_BOOL bFrameRateIndicator;
    OMX_U32 nFrameRateNumerator;
    OMX_U32 nFrameRateDenominator;
    OMX_U32 nFrameRateExplicit;
    OMX_BOOL bColorFormatFlag;
    OMX_U32 nColorPrimaries;
    OMX_U32 nTransferCharacteristics;
    OMX_U32 nMatrixCoefficients;
    OMX_BOOL bHRDIndicatorFlag;
    OMX_U32 nHRDNumberOfLeakyBuckets;
    OMX_U32 nHRDRateExponent;
    OMX_U32 nHRDBufferExponent;
} OMX_SYMBIAN_VIDEO_VC1ADVANCEDPROFILETYPE;

/** VC1 settings.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Video.Vc1".
*/
typedef struct OMX_SYMBIAN_VIDEO_PARAM_VC1TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_SYMBIAN_VIDEO_VC1PROFILETYPE eProfile;
    OMX_SYMBIAN_VIDEO_VC1LEVELTYPE eLevel;
    OMX_U32 bFrmRtQPostProc;
    OMX_U32 bBitRtQPostProc;
    OMX_BOOL bEnableFInterpFlag;
    union {
        OMX_SYMBIAN_VIDEO_VC1SIMPLEMAINPROFILETYPE simple;
        OMX_SYMBIAN_VIDEO_VC1ADVANCEDPROFILETYPE advanced;
    } profile;
} OMX_SYMBIAN_VIDEO_PARAM_VC1TYPE;

/** VP6 profile enum. */
typedef enum OMX_SYMBIAN_VIDEO_VP6PROFILETYPE {
    OMX_SYMBIAN_VIDEO_VP6ProfileSimple,
    OMX_SYMBIAN_VIDEO_VP6ProfileAdvanced,
    OMX_SYMBIAN_VIDEO_VP6ProfileHeightenedSharpness,
    OMX_SYMBIAN_VIDEO_VP6ProfileMax = 0x7FFFFFFF
} OMX_SYMBIAN_VIDEO_VP6PROFILETYPE;

/** VP6 level enum. */
typedef enum OMX_SYMBIAN_VIDEO_VP6LEVELTYPE {
    OMX_SYMBIAN_VIDEO_VP6LevelMax = 0x7FFFFFFF
} OMX_SYMBIAN_VIDEO_VP6LEVELTYPE;

/** VP6 settings.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Video.Vc6".
*/
typedef struct OMX_SYMBIAN_VIDEO_PARAM_VP6TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_SYMBIAN_VIDEO_VP6PROFILETYPE eProfile;
    OMX_SYMBIAN_VIDEO_VP6LEVELTYPE eLevel;
} OMX_SYMBIAN_VIDEO_PARAM_VP6TYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_VideoExt_h */
