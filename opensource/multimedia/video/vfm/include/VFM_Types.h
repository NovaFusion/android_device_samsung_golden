/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_TYPES_H
#define __VFM_TYPES_H

#include "video_generic_chipset_api.h"
#include "VFM_Memory.h"


/// @ingroup VFMProprietary
/// @brief NULL definition
#define VFM_NULL 0

/// @ingroup VFMProprietary
/// @brief INVALID Component Handle
#define VFM_INVALID_CM_HANDLE        0xFFFFFFFF

/// @ingroup VFMProprietary
/// @brief Port index for Jpeg encode component
typedef enum VFM_JPEG_ENCODE_PORT_INDEX
{
    VFM_JPEG_ENCODE_PORT_IMAGE_IN = 0,
    VFM_JPEG_ENCODE_PORT_BITSTREAM_OUT,
    VFM_JPEG_ENCODE_PORT_LAST,
} VFM_JPEG_ENCODE_PORT_INDEX;

/// @ingroup VFMProprietary
/// @brief JPEG On-the-fly rotation
typedef enum VFM_ONFLY_ROTATION
{
    /// @brief No rotation. Equal 0 according to the JPEG Encoder FW Implementation
    VFM_ROTATION_NONE = 0,
    /// @brief rotation +90. Equal 1 according to the JPEG Encoder FW Implementation
    VFM_ROTATION_PLUS_90 = 1,
    /// @brief rotation -90. Equal 2 according to the JPEG Encoder FW Implementation
    VFM_ROTATION_MINUS_90 = 2
} VFM_ONFLY_ROTATION;

/// @ingroup VFMProprietary
/// @brief Dynamic configuration of a JPEG Encoder, get/set using VFM_INDEXTYPE::VFM_IndexConfigTargetbpp
///
/// (cf. JPEGEnc_ParamAndConfig::setIndexConfigTargetbpp() and
/// JPEGEnc_ParamAndConfig::getIndexConfigTargetbpp())
typedef struct VFM_CONFIG_TARGETBPP
{
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;

    /// @brief Target bit-per-pixel to reach.
    /// The unit is 1/256 bpp (i.e. BPP=(number of bits per pixel)*256).
    OMX_U16 targetBpp;
} VFM_CONFIG_TARGETBPP;

/// @ingroup VFMProprietary
/// @brief Allow to get / set a structure of type OMX_BUFFERHEADERTYPE *pBuffer
typedef struct VFM_CONFIG_OMXBUFFERPTR
{
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
} VFM_CONFIG_OMXBUFFERPTR;

/// @ingroup VFMProprietary
/// @brief Allow to get the memory status of a codec. Index is VFM_IndexConfigMemory,
/// and associated string is "OMX.ST.VFM.memory_status"
typedef struct VFM_CONFIG_MEMORY
{
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief Memory status of the codec
    VFM_MemoryStatus nMemoryStatus;
} VFM_CONFIG_MEMORY;

/* +Changes for CR 399075 */
typedef struct VFM_CONFIG_DECODEIMMEDIATE {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief Port that this structure applies to
        OMX_U32 nPortIndex;
    /// @brief TRUE to indicate that I frame is requested immediately
    OMX_BOOL bImmediateRelease;
} VFM_CONFIG_DECODEIMMEDIATE;
/* -Changes for CR 399075 */

typedef struct VFM_CONFIG_PERFDATATYPE
{
	/// @brief Size of the structure
	OMX_U32 nSize;
	/// @brief Version of this structure
	OMX_VERSIONTYPE nVersion;
	/// @brief ticks of latest few control_algo calls of FW
	OMX_U32 nControlAlgoTicks[10];
	/// @brief ticks of latest few end_algo callbacks of FW
	OMX_U32 nEndAlgoTicks[10];
	/// @brief accumulated ticks of ticks taken by FW
	OMX_U32 nAccumulatedTicks;
	/// @brief frame count for the accumulation
	OMX_U32 nFrameCount;
} VFM_CONFIG_PERFDATATYPE;

/* extensions for VC1 decode */
/// @ingroup VFMProprietary
/// @brief VC1 color format index
/// @note FIXE: should be removed from VFM, and set in a specific extension file
/// with a fixed index


typedef enum OMX_VIDEO_VFM_Coding {
  OMX_VIDEO_CodingVC1 = (OMX_VIDEO_CODINGTYPE)(OMX_VIDEO_CodingMJPEG+1)
} OMX_VIDEO_VFM_Coding;


/// @ingroup VFMProprietary
/// @brief Enumeration for getting ARM perf / Full Perf
typedef struct VFM_PARAM_ARMLOADCOMPUTATION
{
	/// @brief Size of the structure
	OMX_U32 nSize;
	/// @brief Version of this structure
	OMX_VERSIONTYPE nVersion;
	/// @brief Boolean type TRUE would disable FW code execution
	OMX_BOOL  bDisableFWComputation;
} VFM_PARAM_ARMLOADCOMPUTATION;


/// @ingroup VFMProprietary
/// @brief Enumeration of all the profile defined in VC1
typedef enum OMX_VIDEO_VC1PROFILETYPE {
    OMX_VIDEO_VC1ProfileSimple,
    OMX_VIDEO_VC1ProfileMain,
    OMX_VIDEO_VC1ProfileAdvanced,
    OMX_VIDEO_VC1ProfileMax = 0x7FFFFFFF
} OMX_VIDEO_VC1PROFILETYPE;

typedef enum OMX_VIDEO_VC1LEVELTYPE {
    OMX_VIDEO_VC1LevelLow,
    OMX_VIDEO_VC1LevelMedium,
    OMX_VIDEO_VC1LevelHigh,
    OMX_VIDEO_VC1LevelL0,
    OMX_VIDEO_VC1LevelL1,
    OMX_VIDEO_VC1LevelL2,
    OMX_VIDEO_VC1LevelL3,
    OMX_VIDEO_VC1LevelL4,
    OMX_VIDEO_VC1LevelMax = 0x7FFFFFFF
} OMX_VIDEO_VC1LEVELTYPE;

typedef struct OMX_VIDEO_VC1SIMPLEMAINPROFILE {
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
} OMX_VIDEO_VC1SIMPLEMAINPROFILE;

typedef struct OMX_VIDEO_VC1ADVANCEDPROFILE {
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
} OMX_VIDEO_VC1ADVANCEDPROFILE;

/* MC: I have added the name "simplemain" and "advanced" */
typedef struct OMX_VIDEO_PARAM_VC1TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_VC1PROFILETYPE eProfile;
    OMX_VIDEO_VC1LEVELTYPE eLevel;
    OMX_U32 bFrmRtQPostProc;
    OMX_U32 bBitRtQPostProc;
    OMX_BOOL bEnableFInterpFlag;
    union {
      OMX_VIDEO_VC1SIMPLEMAINPROFILE simplemain;
      OMX_VIDEO_VC1ADVANCEDPROFILE   advanced;
    } profile;
} OMX_VIDEO_PARAM_VC1TYPE;

/// @ingroup VFMProprietary
typedef enum VFM_AVC_3D_3DFormatType {
    VFM_3DFormatType_STD_FORMAT                             = 1<<0,
    VFM_3DFormatType_INTERLEAVE_LEFT_VIEW_FIRST             = 1<<1,
    VFM_3DFormatType_INTERLEAVE_RIGHT_VIEW_FIRST            = 1<<2,
    VFM_3DFormatType_TOP_BOTTOM_LEFT_VIEW_FIRST             = 1<<3,
    VFM_3DFormatType_TOP_BOTTOM_RIGHT_VIEW_FIRST            = 1<<4,
    VFM_3DFormatType_SIDE_BY_SIDE_LEFT_VIEW_FIRST           = 1<<5,
    VFM_3DFormatType_SIDE_BY_SIDE_RIGHT_VIEW_FIRST          = 1<<6,
    VFM_3DFormatType_MVC_TEMPORAL                           = 1<<7,
    VFM_3DFormatType_MVC_SPATIAL		            = 1<<8
} VFM_AVC_3D_3DFormatType;

typedef struct VFM_PARAM_AVC_MVC
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nMVCFormatType;
    OMX_U32 nMVCSEI;
    OMX_U32 nMVCAnchorPeriod;
} VFM_PARAM_AVC_MVC;
/**
 * List of SEI messages that can be requested.
 *
 * This enum is related to OMX_VIDEO_PARAM_AVCSEI structure defined below.
 */
/// @ingroup VFMProprietary
typedef enum OMX_VIDEO_AVCSEI {
    OMX_VIDEO_AvcSeiBufferinPeriod = 0x01,
    OMX_VIDEO_AvcSeiPictureTiming = 0x02,
    OMX_VIDEO_AvcSeiPanScanRectangle = 0x04,
    OMX_VIDEO_AvcSeiFillerPayload = 0x08,
    OMX_VIDEO_AvcSeiUserDataRegisteredByITUT35 = 0x10,
    OMX_VIDEO_AvcSeiUserDataUnregistered = 0x20,
    OMX_VIDEO_AvcSeiRecoveryPoint = 0x40,
    OMX_VIDEO_AvcSeiDecReferencePictureMarkingRepetition = 0x80,
    OMX_VIDEO_AvcSeiSparePicture = 0x100,
    OMX_VIDEO_AvcSeiSceneInformation = 0x200,
    OMX_VIDEO_AvcSeiSubSequenceInformation = 0x400,
    OMX_VIDEO_AvcSeiSubSequenceLayerCharacteristics = 0x800,
    OMX_VIDEO_AvcSeiSubSequenceCharacteristics = 0x1000,
    OMX_VIDEO_AvcSeiFullFrameFreeze = 0x2000,
    OMX_VIDEO_AvcSeiFullFrameFreezeRelease = 0x4000,
    OMX_VIDEO_AvcSeiFullFrameSnaphot = 0x8000,
    OMX_VIDEO_AvcSeiProgressiveRefinementSegmentStart = 0x10000,
    OMX_VIDEO_AvcSeiProgressiveRefinementSegmentEnd = 0x20000,
    OMX_VIDEO_AvcSeiMotionContstrainedSliceGroup = 0x40000,
    OMX_VIDEO_AvcSeiFilmGrainCharacteristics = 0x80000,
    OMX_VIDEO_AvcSeiDeblockingFilterDispPreference = 0x100000,
    OMX_VIDEO_AvcSeiStereoVideoInformation = 0x200000,
    OMX_VIDEO_AvcSeiMax = 0x7FFFFFFF
} OMX_VIDEO_AVCSEI;


/**
 * SEI messages
 *
 * STRUCT MEMBERS:
 *  nSize      : Size of the structure in bytes
 *  nVersion   : OMX specification version information
 *  nPortIndex : Port that this structure applies to
 *  bEnableSEI : Enable SEI messages.
 *  eSEI       : Requested SEI messages. Several messages can be requested.
 */
/// @ingroup VFMProprietary
typedef struct OMX_VIDEO_PARAM_AVCSEI
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnableSEI;
    OMX_VIDEO_AVCSEI eSEI;
} OMX_VIDEO_PARAM_AVCSEI;


/* Supported Extension */
/// @ingroup VFMProprietary
typedef enum VFM_SUPPORTEDEXTENSION_MASK {
    VFM_SUPPORTEDEXTENSION_SIZE         = 1<<0,
    VFM_SUPPORTEDEXTENSION_PAR          = 1<<1,
    VFM_SUPPORTEDEXTENSION_PRIMARYCOLOR = 1<<2,
    VFM_SUPPORTEDEXTENSION_CROP         = 1<<3,
	VFM_SUPPORTEDEXTENSION_DPBSIZE		= 1<<4,
	VFM_SUPPORTEDEXTENSION_3D           = 1<<5
} VFM_SUPPORTEDEXTENSION_MASK;

typedef struct VFM_PARAM_SUPPORTEDEXTENSION_TYPE
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nMask;      // of VFM_SUPPORTEDEXTENSION_xxx
} VFM_PARAM_SUPPORTEDEXTENSION_TYPE;


typedef struct VFM_PARAM_HEADERDATACHANGING_TYPE
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bHeaderDataChanging;
} VFM_PARAM_HEADERDATACHANGING_TYPE;


typedef struct VFM_PARAM_RESOURCEESTIMATION_TYPE
{
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief SVA Mcps budget in mips. Shall be in range 0 - 200
    OMX_U32 nSvaMips;
} VFM_PARAM_RESOURCEESTIMATION_TYPE;

/// @ingroup VFMProprietary
/// @brief Used to set decoder as used for thumbnail generation
/// In such cases, the number of required buffers is lowered
typedef struct VFM_PARAM_THUMBNAIL_GENERATION_TYPE
{
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief OMX_TRUE if it is used for
    OMX_BOOL bThumbnailGeneration;
} VFM_PARAM_THUMBNAIL_GENERATION_TYPE;

/* + change for ER 360599 */
/// @ingroup VFMProprietary
/// @brief Used to set constraint flag
/// @brief this flag could be queried for any component and decide if there is any restriction to be imposed

typedef struct VFM_PARAM_RESTRICTH264LEVEL_TYPE
{
	/// @brief Size of the structure
	OMX_U32 nSize;
	/// @brief Version of this structure
	OMX_VERSIONTYPE nVersion;
	/// @brief Maximum Level value to be supported
	OMX_U32 level;
} VFM_PARAM_RESTRICTH264LEVEL_TYPE;

/* - change for ER 360599 */

typedef enum {
    OMX_NaluFormatStartCodes=0x1,                   // NALUs separated by Start Codes (ITU-T H.264\ISO 14496-10  Annex B)
    OMX_NaluFormatOneNaluPerBuffer=0x2,             // One NALU per buffer. Multiple NALUs in the same buffer are forbidden
    OMX_NaluFormatOneByteInterleavedLength=0x4,     // NALU separated by 1-byte interleaved length fields
    OMX_NaluFormatTwoByteInterleavedLength=0x8,     // NALU separated by 2-byte interleaved length fields
    OMX_NaluFormatFourByteInterleavedLength=0x10    // NALU separated by 4-byte interleaved length fields
} OMX_NALUFORMATSTYPE;

typedef struct OMX_NALSTREAMFORMATTYPE {
    OMX_U32  nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32  nPortIndex;
    OMX_NALUFORMATSTYPE  eNaluFormat;
} OMX_NALSTREAMFORMATTYPE;

/// @ingroup VFMProprietary
/// @brief Used to set static dpb size
typedef struct VFM_PARAM_STATICDPBSIZE_TYPE
{
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief OMX_TRUE if it static dpb size is required
    OMX_BOOL bStaticDpbSize;
} VFM_PARAM_STATICDPBSIZE_TYPE;

/* +Change start for CR333616 Delay recycling */
typedef struct VFM_PARAM_RECYCLINGDELAYTYPE {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief Recycling delay, in number of frames in the output, to retain some frames.
    /// Default value is 0 (no delay)
    /// Can be used in Android to avoid tearing effect
    OMX_U32 nRecyclingDelay;
} VFM_PARAM_RECYCLINGDELAYTYPE;
/* -Change end for CR333616 Delay recycling */

/* +Change start for CR330198 Revert to DTS from PTS */
typedef struct VFM_PARAM_TIMESTAMPDISPLAYORDERTYPE {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief TRUE to indicate timestamps are in display order in a decoder
    /// Default value is FALSE
    /// Can be used in Android to provide data to the non-default order
    OMX_BOOL bDisplayOrder;
} VFM_PARAM_TIMESTAMPDISPLAYORDERTYPE;
/* -Change end for CR330198 Revert to DTS from PTS */

/* +Change start for CR333618 Added for IOMX renderer */
typedef struct VFM_PARAM_SHAREDCHUNKINPLATFORMPRIVATETYPE {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief TRUE to indicate that the pPlatformPrivate must contain the shared chunk metadata on FillBufferDone
    /// Default value is FALSE
    /// Can be used in Android to provide data to the renderer
    OMX_BOOL bSharedchunkInPlatformprivate;
} VFM_PARAM_SHAREDCHUNKINPLATFORMPRIVATETYPE;
/* -Change end for CR333618 Added for IOMX renderer */

/* +Change start for CR332521 IOMX UseBuffer */
typedef struct VFM_PARAM_IOMXTYPE {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief TRUE to indicate that physically contiguous buffers must be allocated by the VFM and passed to the
    /// codec in case of iOMX interface used (with OMX_UseBuffer, without the Shared Chunk Extension
    OMX_BOOL biOMX;
} VFM_PARAM_IOMXTYPE;
/* -Change end for CR332521 IOMX UseBuffer */

/* +Change start for  CR-332873, added support for CpbBufferSize */
/**
 * This is a structure for configuring CpbBufferSize.
 *
 * STRUCT MEMBERS:
 *  nSize          : Size of the structure in bytes
 *  nVersion       : OMX specification version info
 *  nPortIndex     : Port that this structure applies to
 *  nCpbBufferSize : corresponds to delay multiplied by bitrate
 */
/// @ingroup VFMProprietary
/// @brief This is used to change the delay to reach targeted bitrate in VBR algo
typedef struct OMX_VIDEO_PARAM_CPBBUFFERSIZETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nCpbBufferSize;
} OMX_VIDEO_PARAM_CPBBUFFERSIZETYPE;
/* -Change start for  CR-332873, added support for CpbBufferSize */

/* +Change start for CR338066 MPEG4Enc DCI */
typedef struct VFM_PARAM_MPEG4SHORTDCITYPE {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    /// @brief Port that this structure applies to
	OMX_U32 nPortIndex;
    /// @brief TRUE to indicate that the VOS+VO+VOL header written by the encoder
    /// will not contain the optional VOL_control_parameters to reduce the header size
    OMX_BOOL bEnableShortDCI;
} VFM_PARAM_MPEG4SHORTDCITYPE;

/* -Change end for CR338066 MPEG4Enc DCI */

/**
 * This is a structure for clipping video compression quantization
 * parameter. This structure is valid only in BRC mode.
 *
 * STRUCT MEMBERS:
 *  nSize               : Size of the structure in bytes
 *  nVersion            : OMX specification version info
 *  nPortIndex          : Port that this structure applies to
 *  bBRC_clipped        : Qp clipping activation or deactivation
 *  nBRC_clipped_min_QP : minimum Qp
 *  nBRC_clipped_max_QP : maximum Qp
 */
/// @ingroup VFMProprietary
/// @brief This is used to have a CBR algo with clipped Qp
typedef struct OMX_VIDEO_PARAM_BRCCLIPPINGTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bBRC_clipped;
    OMX_U32 nBRC_clipped_min_QP;
    OMX_U32 nBRC_clipped_max_QP;
} OMX_VIDEO_PARAM_BRCCLIPPINGTYPE;

/* +Change start for CR 399938 */

typedef struct VFM_PARAM_STOREMETADATAINBUFFERS {
    /// @brief Size of the structure
    OMX_U32 nSize;
    /// @brief Version of this structure
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    /// @brief TRUE to indicate that
    /// Default value is FALSE
    /// Can be used in Android to provide data to the renderer
    OMX_BOOL bStoreMetadataInBuffers;
} VFM_PARAM_STOREMETADATAINBUFFERS;

/* -Change end for CR 399938 */

#endif
