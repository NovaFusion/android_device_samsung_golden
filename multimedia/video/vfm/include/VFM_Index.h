/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_INDEX_H
#define __VFM_INDEX_H

#include "OMX_Component.h"
#include "ENS_Index.h"

#include "video_chipset_api_index.h"

/// @ingroup VFMProprietary
/// @brief Image port base
/// @hideinitializer
#define IPB 0

/// @ingroup VFMProprietary
/// @brief Video port base
/// @hideinitializer
#define VPB 0

/// @ingroup VFMProprietary
/// @brief Proprietary indexes in order to access specific functionalities, not
/// part of the standard component. Each entry in this enumeration maps to an VFM
/// specified structure.
typedef enum VFM_INDEXTYPE {
    /// @brief Dummy index for the start of VFM specific indexes
    /// @hideinitializer
    VFM_IndexStartExtensions = ENS_IndexVFMStartUnused,

    /// @brief Allow to get / set a structure of type VFM_CONFIG_TARGETBPP
    /// (cf. JPEGEnc_ParamAndConfig::setIndexConfigTargetbpp() and
    /// JPEGEnc_ParamAndConfig::getIndexConfigTargetbpp())
    VFM_IndexConfigTargetbpp,  /**< reference: VFM_CONFIG_TARGETBPP */

    /// @brief Allow to get / set a structure of type OMX_BUFFERHEADERTYPE *pBuffer
    /// field nU32 OMX_PARAM_U32TYPE is filled with OMX_BUFFERHEADERTYPE *pBuffer
    /// and nPortIndex should be IPB+0
    /// In case of Source pBuffer is corresponding to the buffer to be written (eg. by read from file)
    /// In case of Sink pBuffer is corresponding to the buffer to be read (eg. by write to file)
    VFM_IndexConfigOMXBufferPtr,  /**< reference: VFM_CONFIG_OMXBUFFERPTR */

    OMX_IndexParamVideoVc1,

    /// @brief Allow to get / set a structure of type VFM_CONFIG_MEMORY to get the memory status of the codec
    VFM_IndexConfigMemory, /**( reference VFM_CONFIG_MEMORY */

    /// @brief Parallel (true) or sequential (false) deblocking
    H264DEC_IndexParam_ParallelDeblocking, /**( reference: OMX_PARAM_DEBLOCKINGTYPE */

    VFM_IndexParamAVC3D, /**( reference: VFM_PARAM_AVC_MVC */
    /// @brief Allow to get / set SEI message settings in AVC encoder
    OMX_IndexParamVideoAvcSei, /**( reference: OMX_VIDEO_PARAM_AVCSEI */

	/// @brief Index to retrieve FW perf data statistics
    VFM_IndexParamVideoFwPerfData, /**( reference: VFM_CONFIG_PERFDATATYPE */

    /// @brief Index to retrive crop information from Codec
    VFM_IndexParamSupportedExtension, /**( reference: VFM_PARAM_SUPPORTEDEXTENSION_TYPE */

    /// @brief To indication pBufferHeader->pBuffer can changed dynamically during a run.
    /// @Note This should be then passed correctly to the codec if required (it is for VFM_NmfHost_Processing based component)
    VFM_IndexParamHeaderDataChanging, /**( reference: VFM_PARAM_HEADERDATACHANGING_TYPE */

     /// @brief Index to get /set resource estimation information from Codec
    VFM_IndexParamResourceEstimation, /**( reference: VFM_PARAM_RESOURCEESTIMATION_TYPE */

    /// @brief To indicate nal format (length instead of start code) is used
    OMX_IndexKhronosExtParamNALStreamFormatSupported, /**( reference: OMX_NALSTREAMFORMATTYPE  */

    /// @brief To indicate nal format (length instead of start code) is used
    OMX_IndexKhronosExtParamNALStreamFormat, /**( reference: OMX_NALSTREAMFORMATTYPE  */

    /// @brief To disable FW code execution inorder to compute ARM Load
    VFM_IndexParamARMLoadComputation, /**( reference: VFM_PARAM_ARMLOADCOMPUTATION */

	/* +Changes for CR 399075 */
    /// @brief To indicate the immediate release of I frame
    VFM_IndexConfigDecodeImmediate, /**( reference: VFM_CONFIG_DECODEIMMEDIATE */
	/* -Changes for CR 399075 */

    /// @brief To indicate the decoder is used for thumbnail generation
    VFM_IndexParamThumbnailGeneration, /**( reference: VFM_PARAM_THUMBNAIL_GENERATION_TYPE */

	/* +Change start for  CR333616 Delay recycling */
    /// @brief To tune recycling delay of FillBufferDone
    VFM_IndexParamRecyclingDelay, /**( reference: VFM_PARAM_RECYCLINGDELAYTYPE */
    /* -Change end for CR333616 Delay recycling */

    /* +Change start for CR330198 Revert to DTS from PTS */
    /// @brief To use timestamp in display order in input too
    VFM_IndexParamTimestampDisplayOrder, /**( reference: VFM_PARAM_TIMESTAMPDISPLAYORDERTYPE */
    /* -Change end for CR330198 Revert to DTS from PTS */

    /* +Change start for CR333618 Added for IOMX renderer */
    /// @brief To indicate that the pPlatformPrivate must contain the shared chunk metadata on FillBufferDone
    VFM_IndexParamSharedchunkInPlatformprivate, /**( reference: VFM_PARAM_SHAREDCHUNKINPLATFORMPRIVATETYPE */
    /* -Change end for CR333618 Added for IOMX renderer */

    /* +Change start for CR332521 IOMX UseBuffer */
    /// @brief To indicate that physically contiguous buffers must be allocated by the VFM and passed to the
    /// codec in case of iOMX interface used (with OMX_UseBuffer, without the Shared Chunk Extension
    VFM_IndexParamiOMX, /**( reference: VFM_PARAM_IOMXTYPE */
    /* -Change end for CR332521 IOMX UseBuffer */

     // added new Index for CR 334368
    /// @brief To indicate the decoder use of static dpb size
    VFM_IndexParamStaticDPBSize, /**( reference: VFM_PARAM_STATICDPBSIZE_TYPE */

	/* +Change start for  CR-332873, added support of OMX_IndexParamVideoCpbBufferSize */
    /// @brief This is used to change the delay to reach targeted bitrate in VBR algo
    OMX_IndexParamVideoCpbBufferSize, /**( reference: OMX_VIDEO_PARAM_CPBBUFFERSIZETYPE */
	/* -Change start for  CR-332873, added support of OMX_IndexParamVideoCpbBufferSize */

	/* +Change start for CR338066 MPEG4Enc DCI */
	/// @brief This is used to request the encoder to write a short VOS+VO+VOL header
	VFM_IndexParamMpeg4ShortDCI, /**( reference: VFM_PARAM_MPEG4SHORTDCITYPE */
	/* -Change end for CR338066 MPEG4Enc DCI */

	/* +Change start for CR338066 MPEG4Enc DCI */

    /// @brief This is used to specify the max amount of HWMEM (in megaBytes) to be used for a particular component instance

    /* +Change start for CR368983 */
    /// @brief This is used to have a BRC algo with clipped Qp
    OMX_IndexParamVideoBRCClipping, /**( reference: OMX_VIDEO_PARAM_BRCCLIPPINGTYPE */
    /* -Change end for CR368983 */

    /// @brief Dummy last index
    /// @hideinitializer
    VFM_IndexMax = 0x7FFFFFFF  /**< dummy */
} VFM_INDEXTYPE;

#endif // #ifndef __OMX_VIDEO_EXTENSIONS_H

