/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Index.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_INDEX_H_
#define _AFM_INDEX_H_

#include "ENS_Index.h"

/** The AFM_INDEXTYPE enumeration is used to select a structure when either
 *  getting or setting parameters and/or configuration data.  Each entry in
 *  this enumeration maps to an AFM specified structure.
 */
typedef enum AFM_INDEXTYPE {

    AFM_IndexStartExtensions = ENS_IndexAFMStartUnused,
    AFM_IndexParamPcm,                      /**< reference: AFM_PARAM_PCMMODETYPE */
    AFM_IndexParamBitstream,                /**< reference: AFM_PARAM_BITSTREAMMODETYPE */
    AFM_IndexParamPcmLayout,                /**< reference: AFM_PARAM_PCMLAYOUTTYPE */
    AFM_IndexParamForceStandardTunneling,   /**< reference: AFM_PARAM_FORCE_STANDARD_TUNNELING */
    AFM_IndexParamSetPriority,              /**< reference: AFM_PARAM_PRIORITY */
    AFM_IndexParamGetPriority,              /**< reference: AFM_PARAM_PRIORITY */
    AFM_IndexParamAmrNbDecoder,             /**< reference: AFM_PARAM_AMRNBDECODERTYPE */
    AFM_IndexParamAmrNbEncoder,             /**< reference: AFM_PARAM_AMRNBENCODERTYPE */
    AFM_IndexConfigAmrNbEncoder,            /**< reference: AFM_CONFIG_AMRNBENCODERTYPE */
    AFM_IndexParamCapturer,                 /**< reference: AFM_PARAM_CAPTURERTYPE */
    AFM_IndexLowPowerRenderer,              /**< reference: AFM_CONFIG_LOWPOWERTYPE */
    AFM_IndexParamHostMpcSync,              /**< reference: AFM_PARAM_HOST_MPC_SYNC_TYPE */
    AFM_IndexParamBindingMemoryType,        /**< reference: AFM_PARAM_BINDING_MEMORY_TYPE */

    AFM_IndexParamAacDecoder,               /**< reference: AFM_PARAM_AACDECTYPE */

    AFM_IndexParamCscall,                   /**< reference: AFM_PARAM_CSCALLMODETYPE */

    //Control for TransducerEqualizer,
    AFM_IndexParamTransducerEqualizer,      /**< reference: AFM_AUDIO_PARAM_TRANSDUCEREQUALIZERTYPE */
    AFM_IndexConfigTransducerEqualizer,     /**< reference: AFM_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE */

    //Control for Legacy Graphic Equalizer,
    AFM_IndexConfigLegacyGraphicEqualizer,  /**< reference: AFM_AUDIO_CONFIG_LEGACYGRAHICEQUALIZERTYPE */

    AFM_IndexParamSpl,                      /**< reference: AFM_AUDIO_PARAM_SPLTYPE */

    AFM_IndexConfigCscallSpeechCodecReq,    /**< reference: AFM_AUDIO_CONFIG_CSCALL_SPEECH_CODEC_REQ */
    AFM_IndexConfigCscallSpeechTimingReq,   /**< reference: AFM_AUDIO_CONFIG_CSCALL_UPLINK_TIMING_REQ */

    AFM_IndexParamProprietaryCommunication, /**< reference: AFM_PARAM_PROPRIETARY_COMMUNICATION */
	AFM_IndexParamMspNbChannel,             /**< reference: AFM_AUDIO_PARAM_MSP_NB_CHANNEL */
	AFM_IndexParamDmaChannelType,             /**< reference: AFM_AUDIO_PARAM_DMA_CHANNEL_TYPE */
    AFM_IndexParamSpectrumAnalyzerBase,
    AFM_IndexParamSpectrumAnalyzerEnd = (AFM_IndexParamSpectrumAnalyzerBase + 10),
    AFM_IndexParamTimescaleBase,
    AFM_IndexParamTimescaleEnd = (AFM_IndexParamTimescaleBase + 10),
    AFM_IndexConfigCscallMRePReq,           /**< reference: AFM_AUDIO_CONFIG_CSCALL_MREP_REQ */

    //Line priority
    OMX_IndexParamLinePriority,              /**< reference: OMX_PARAM_LINEPRIORITYTYPE    */
    //config for Xprot
    AFM_IndexConfigLSprot,                   /**< reference: AFM_AUDIO_CONFIG_LSPROTECTIONTYPE */

	//
	AFM_IndexConfigCscallVoiceCallStatus,   /**< reference: AFM_AUDIO_CONFIG_CSCALL_VOICE_CALL_STATUS */
    AFM_IndexConfigModemLoopBack,           /**< reference: AFM_AUDIO_CONFIG_MODEM_LOOPBACK */
    AFM_IndexConfigCscallTimingReqMargins,  /**< reference: AFM_AUDIO_CONFIG_CSCALL_TIMING_REQ_MARGINS */
    AFM_IndexConfigCscallDataWaitTime,      /**< reference: AFM_AUDIO_CONFIG_CSCALL_DATA_WAIT_TIME */
    AFM_IndexParamSrcMode,                  /**< reference: AFM_AUDIO_PARAM_SRC_MODE */

    // Alsasink
    OMX_IndexParamAlsasink,                 /**< reference: OMX_AUDIO_PARAM_ALSASINK */
    AFM_IndexParamCsCallMpc,                /**< reference: AFM_AUDIO_PARAM_CSCALL_MPC */

    //MDRC new filters
    AFM_IndexParamMdrc,                     /**< reference: AFM_AUDIO_PARAM_MDRC */
    AFM_IndexGlobalConfigMdrc,              /**< reference: AFM_AUDIO_GLOBAL_CONFIG_MDRC */

    // RC filter for ER400572
    AFM_IndexParamRenderer,                 /**< reference: AFM_AUDIO_PARAM_RENDERER */
    AFM_IndexConfigPcmProbe,                /**< reference: AFM_AUDIO_CONFIG_PCM_PROBE */

	// SpeechProc
	AFM_IndexParamSpeechProc,               /**< reference: AFM_AUDIO_PARAM_SPEECH_PROC */

    // Stereo Capture
    AFM_IndexConfigStereoCapture,           /**< reference: AFM_AUDIO_CONFIG_STEREO_CAPTURE */
    // This must be last at all times!
    // Different component types will have overlapping indeces for component specific parameters!
    AFM_IndexStartComponentSpecificExtensions,
    AFM_IndexMax = 0x7FFFFFFF

} AFM_INDEXTYPE;

#endif // _AFM_INDEX_H_

