


/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   audio_chipset_api_index.h
 * \brief  Indices for audio chipset apis
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIO_CSA_INDEX_H_
#define _AUDIO_CSA_INDEX_H_

#include "CSA_Index.h"

typedef enum CSA_AUDIOINDEXTYPE{

  CSA_IndexAudioStartExtensions =    CSA_IndexAudioStartUnused,

  OMX_IndexConfigAudioAmrMode,                  /**< reference: OMX_AUDIO_CONFIG_AMRMODETYPE */
  OMX_IndexConfigAudioVirtualizer,              /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERTYPE      */
  OMX_IndexConfigAudioVirtualizerLoudspeaker,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERLOUDSPEAKERTYPE    */
  OMX_IndexConfigAudioVirtualizerUpmix,         /**< reference: OMX_AUDIO_CONFIG_VIRTUALIZERUPMIXTYPE           */
  OMX_IndexParamAudioHapticsDriverProperties ,  /**< reference: OMX_AUDIO_PARAM_HAPTICSDRIVERPROPERTIESTYPE     */

  //Audio HW Control Chipset API index
  OMX_IndexConfigAudioPower,                    /**< reference: OMX_AUDIO_CONFIG_POWERTYPE              */
  OMX_IndexConfigAudioVolumeRamp,               /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_VOLUMERAMPTYPE */
  OMX_IndexConfigAudioBurstControl,             /**< reference: OMX_AUDIO_CONFIG_BURSTCONTROLTYPE       */
  OMX_IndexConfigAudioRegBankQuery,             /**< reference: OMX_AUDIO_CONFIG_REGISTERBANKQUERYTYPE  */
  OMX_IndexConfigAudioRegBankDescriptorQuery,   /**< reference: OMX_AUDIO_CONFIG_REGISTERBANKTABLETYPE  */
  OMX_IndexConfigAudioCustomHwControl,          /**< reference: OMX_AUDIO_CONFIG_CUSTOMHWCONTROLTYPE    */
  OMX_IndexConfigAudioHwLoopSupport,            /**< reference: OMX_AUDIO_CONFIG_HWLOOPSUPPORTTYPE      */
  OMX_IndexConfigAudioLoopControl,              /**< reference: OMX_AUDIO_CONFIG_HWLOOPCONTROLTYPE      */

  OMX_IndexParamAudioSynchronized,              /**< reference: OMX_AUDIO_PARAM_SYNCHRONIZEDTYPE        */
  OMX_IndexConfigAudioPortpause,                /**< reference: OMX_AUDIO_CONFIG_PORTPAUSETYPE          */

  //g711
  OMX_IndexParamAudioG711,      /**< reference: OMX_AUDIO_PARAM_G711TYPE        */
  //Flac
  OMX_IndexParamAudioFLAC,      /**< reference: OMX_AUDIO_PARAM_FLACTYPE        */

    //WMAPro
  OMX_IndexParamAudioWMAPro,    /**< reference: OMX_AUDIO_PARAM_WMAPROTYPE      */

  OMX_IndexConfigAudioMdrc,     /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_MDRCTYPE       */
  //DDplus
  OMX_IndexParamAudioDDplus,    /**< reference: OMX_AUDIO_PARAM_DDPLUSTYPE      */

  //DTS
  OMX_IndexParamAudioDTS,       /**< reference: OMX_AUDIO_PARAM_DTSTYPE      */

  //AC3
  OMX_IndexParamAudioAC3,    /**< reference: OMX_AUDIO_PARAM_AC3TYPE      */

  //audio_3D_mixer
  OMX_IndexConfigAudio3DOutput,                   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DOUTPUTTYPE                 */
  OMX_IndexConfigAudio3DLocation,                 /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DLOCATIONTYPE               */
  OMX_IndexParamAudio3DDopplerMode,               /**< reference: OMX_SYMBIAN_AUDIO_PARAM_3DDOPPLERMODETYPE             */
  OMX_IndexConfigAudio3DDopplerSettings,          /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DDOPPLERSETTINGSTYPE        */
  OMX_IndexConfigAudio3DLevels,                   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DLEVELSTYPE                 */
  OMX_IndexConfigAudio3DDistanceAttenuation,      /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DDISTANCEATTENUATIONTYPE    */
  OMX_IndexConfigAudio3DDirectivitySettings,      /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYSETTINGSTYPE    */
  OMX_IndexConfigAudio3DDirectivityOrientation,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYORIENTATIONTYPE */
  OMX_IndexConfigAudio3DMacroscopicOrientation,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICORIENTATIONTYPE */
  OMX_IndexConfigAudio3DMacroscopicSize,          /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICSIZETYPE        */
  OMX_IndexConfigAudio3DCommitMode,               /**< reference: OMX_AUDIO_CONFIG_3DCOMMITMODETYPE             */
  OMX_IndexConfigAudio3DCommit,                   /**< reference: OMX_AUDIO_CONFIG_3DCOMMITTYPE                 */

  // HDMI
  OMX_IndexParamAudioHDMI,      /**< reference: OMX_SYMBIAN_AUDIO_PARAM_HDMITYPE                */

  // A2DP
  OMX_IndexParamAudioA2DP,      /**< reference: OMX_AUDIO_PARAM_A2DPTYPE                */

  // Click Removal
  OMX_IndexConfigRmc,           /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_CLICKREMOVALTYPE      */

  // Sound Pressure Limiter
  OMX_IndexConfigSpllimit,      /**< reference: OMX_AUDIO_CONFIG_SPLLIMITTYPE           */

  // alsasink, alsasource
  OMX_IndexConfigALSADevice,                         /**< reference: OMX_AUDIO_CONFIG_ALSADEVICETYPE */
  OMX_IndexConfigAudioLatency,                       /**< reference: OMX_AUDIO_CONFIG_LATENCY */

  // Telephony indexes (compliant with Audio Telephony SHAI Specification v1.52)
  OMX_Symbian_IndexConfigAudioDecoderStatus,            /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_DECODERSTATUSTYPE         */
  OMX_Symbian_IndexConfigAudioEncoderStatus,            /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_ENCODERSTATUSTYPE         */
  OMX_Symbian_IndexConfigAudioCodecEnabled,
  OMX_Symbian_IndexConfigAudioCodecDisabled,
  OMX_Symbian_IndexConfigAudioSampleRate8khz,
  OMX_Symbian_IndexConfigAudioSampleRate16khz,
  OMX_Symbian_IndexConfigControlTargetDelivery,
  OMX_Symbian_IndexConfigAudioUplinkTiming,             /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_UPLINKTIMINGTYPE          */
  OMX_Symbian_IndexConfigAudioUplinkTimingInfo,         /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_UPLINKTIMINGINFOTYPE      */
  OMX_Symbian_IndexConfigAudioUplinkDelayInfo,          /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_UPLINKDELAYINFOTYPE       */
  OMX_Symbian_IndexConfigAudioSpeechLoops,              /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_SPEECHLOOPSTYPE           */

  OMX_Symbian_IndexConfigAudioUplinkAlgorithms,         /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioAcousticEchoControl,      /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_ACOUSTICECHOCONTROLTYPE   */
  OMX_Symbian_IndexConfigAudioAutomaticGainControl,     /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioBackgroundNoiseControl,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_BACKGROUNDNOISECONTROLTYPE*/
  OMX_Symbian_IndexConfigAudioMultiMicNoiseControl,     /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioSidetoneHowlingControl,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioUplinkLevelNormalization, /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioWindNoiseControl,         /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_WINDNOISECONTROLTYPE      */
  OMX_Symbian_IndexConfigAudioDaAdTimingDifference,     /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_DAADTIMINGDIFFTYPE        */

  OMX_Symbian_IndexConfigAudioDownlinkAlgorithms,       /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioAutomaticVolumeControl,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioBandwidthExpansion,       /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_BANDWIDTHEXPANSIONTYPE    */
  OMX_Symbian_IndexConfigAudioComfortNoiseGeneration,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioDownLinkNoiseControl,     /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_DOWNLINKNOISECONTROLTYPE  */
  OMX_Symbian_IndexConfigAudioTransducerDRC,            /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioVoiceClarity,             /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE          */
  OMX_Symbian_IndexConfigAudioDownlinkSpeechInfo,       /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_DLSPEECHINFOTYPE          */
  OMX_Symbian_IndexConfigAudioVadInfo,                  /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_VADINFOTYPE               */
  OMX_Symbian_IndexConfigAudioTuningData,               /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_TUNINGDATATYPE            */

  // AV sync
  OMX_IndexConfigAudioProcessedDataAmount,              /**< reference: OMX_AUDIO_CONFIG_PROCESSEDDATAAMOUNT               */

  // Tone Generator
  OMX_IndexConfigAudioToneGenerator,               /**< reference: OMX_AUDIO_CONFIG_TONEGENERATORTYPE */

  // Transducer Equalizer
  OMX_Symbian_IndexConfigAudioTransEqualizer_iireq,         /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_IIREQTYPE                  */
  OMX_Symbian_IndexConfigAudioTransEqualizer_firresponse,   /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_FIRRESPONSETYPE            */
  OMX_Symbian_IndexConfigAudioTransEqualizer,               /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE    */
  OMX_IndexConfigAudioChannelSwitchBoard,               /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_CHANNELSWITCHBOARDTYPE    */

  //Xprot
  OMX_IndexConfigAudioLSprot,                               /**< reference: OMX_SYMBIAN_AUDIO_CONFIG_LSPROTECTIONTYPE    */

  } CSA_AUDIOINDEXTYPE;

#endif // _AUDIO_CSA_INDEX_H_

