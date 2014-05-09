#ifndef _OMX_STE_SPEECH_PROC_H_
#define _OMX_STE_SPEECH_PROC_H_
/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file     omx_ste_speech_proc.h
 * \brief    OMX definition of parameter and config definition supported by the
 *           speech_proc_vcs library
 * \author   ST-Ericsson
 */
/*****************************************************************************/
#include <OMX_Types.h>
#include <OMX_Core.h>
#include "AFM_Index.h"
#include "OMX_DRCExt.h"
#include "OMX_CNGExt.h"

enum OMX_STE_SPEECH_PROC_INDEX {
	OMX_STE_IndexConfigAudioFirstIndex = AFM_IndexStartComponentSpecificExtensions + 688,
	OMX_STE_IndexConfigAudioTxSpeechEnhancement,  /**< reference: OMX_STE_AUDIO_CONFIG_TXSPEECHENHANCEMENTTYPE */
	OMX_STE_IndexConfigAudioRxSpeechEnhancement, /**< reference: OMX_STE_AUDIO_CONFIG_RXSPEECHENHANCEMENTTYPE */
	OMX_STE_IndexConfigAudioTxDRC, /**< reference: OMX_AUDIO_CONFIG_DRCTYPE */
	OMX_STE_IndexConfigAudioRxDRC, /**< reference: OMX_AUDIO_CONFIG_DRCTYPE */
	OMX_STE_IndexConfigAudioTxCTM, /**< reference: OMX_STE_CONFIG_AUDIO_CTMTYPE */
	OMX_STE_IndexConfigAudioRxCTM, /**< reference: OMX_STE_CONFIG_AUDIO_CTMTYPE */
	OMX_STE_IndexConfigAudioTransducerEqualizer, /**< reference: OMX_STE_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE */
    OMX_STE_IndexConfigAudioTxComfortNoise, /**< reference: OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE */
    OMX_STE_IndexConfigAudioRxComfortNoise, /**< reference: OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE */
    OMX_STE_IndexConfigAudioRxSpeechProcCustomerType1, /**< reference: OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE */
    OMX_STE_IndexConfigAudioRxSpeechProcCustomerType2, /**< reference: OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE */
    OMX_STE_IndexConfigAudioTxSpeechProcCustomerType2, /**< reference: OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE */
	OMX_STE_IndexConfigAudioEndIndex,
	OMX_STE_IndexConfigNbOfUplinkIndex = 6,
    OMX_STE_IndexConfigNbOfDownlinkIndex = 6
};


typedef struct STE_ConfigAudioEchoControl_t {
  // General
  OMX_S16  nEnabled;
  OMX_S16  nLinearEchoCancellation;
  OMX_S16  nEchoDetection;
  OMX_S16  nHighPassFilter;
  OMX_S16  nResidualEchoControl;
  OMX_S16  nComfortNoise;
  OMX_S16  nPureDelay;
  // Linear modelling
  OMX_S16  nFilterLength;
  OMX_S16  nShadowFilterLength;
  OMX_S16  nMaxEchoPathGain;
  // General residual echo control
  OMX_S16  nAdaptiveResidualLoss;
  OMX_S16  nRxResidualLossRegressor[40];
  OMX_S16  nNonlinearSpeakerModelling;
  OMX_S16  nResidualEchoMargin;
  // Reverberation
  OMX_S16  nEchoDecayTime;
  // Echo reduction
  OMX_S16  nMaxResidualLoss[40];
  // Loudspeaker nonlineararities
  OMX_S16  nNonLinearEchoDecayTime;
  OMX_S16  nHarmonicGains[6];
  OMX_S16  nFundamentalGains[32];
  OMX_S16  nHarmonicActivationLevel;
  OMX_S16  nSpectralLeakageGains[40];
  OMX_S16  nLeakageActivationLevel;
  // Transparency of residual echo control
  OMX_S16  nSpectralMaskingMargin;
  OMX_S16  nTemporalMaskingTime;
  OMX_S16  nResidualEchoReleaserRate;
} STE_ConfigAudioEchoControl_t;

typedef struct STE_ConfigNoiseReduction_t {
  OMX_S16  nEnabled;
  OMX_S16  nAdaptiveHighPassFilter;
  OMX_S16  nDesiredNoiseReduction;
  OMX_S16  nNrCutOffFrequency;
  OMX_S16  nNoiseReductionDuringSpeech;
  OMX_S16  nNoiseReductionTradeoff;
  OMX_S16  nNoiseFloorPower;
  OMX_S16  nHighPassFilterSnrThreshold;
  OMX_S16  nHighPassCutOffMargin;
} STE_ConfigNoiseReduction_t;

typedef struct SE_ConfigTxLevelControl_t {
  OMX_S16 nEnabled;
  OMX_S16 nAdaptiveSignalGain;
  OMX_S16 nNoiseAdaptiveGain;
  OMX_S16 nNoiseAdaptiveGainSlope;
  OMX_S16 nNoiseAdaptiveGainThreshold;
  OMX_S16 nTargetLevel;
  OMX_S16 nStaticGain;
} SE_ConfigTxLevelControl_t;

typedef struct SE_ConfigMMicParameter_t {
  OMX_S16   nEnabled;
  OMX_S16   nTxChannelMapping[2];
  OMX_S16   nReduceFarField;
  OMX_S16   nReduceWindNoise;
  OMX_S16   nReferenceMicrophoneChannel;
} SE_ConfigMMicParameter_t;

typedef struct OMX_STE_AUDIO_CONFIG_TXSPEECHENHANCEMENTTYPE {
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_U32 nPortIndex;
  OMX_BOOL bEnable;
  STE_ConfigAudioEchoControl_t sConfigEchoControl;
  SE_ConfigMMicParameter_t sConfigMMic;
  STE_ConfigNoiseReduction_t sConfigNoiseReduction;
  SE_ConfigTxLevelControl_t sConfigTxLevelControl;
} OMX_STE_AUDIO_CONFIG_TXSPEECHENHANCEMENTTYPE;


typedef struct STE_ConfigRxEqualizer_t {
  OMX_S16  nEnabled;
  OMX_S16  nSpeechEnhance;
  OMX_S16  nCalibrationMode;
  OMX_S16  nComplexityLevel;
  OMX_S16  nMinNoiseSpeechEnhance;
  OMX_S16  nSaturationMargin;
  OMX_S16  nMinimumTargetSNR;
  OMX_S16  nMaxGainTargetSNR;
  OMX_S16  nThresholdSNREnergyRedist;
  OMX_S16  nMaxGainEnergyRedistHf;
  OMX_S16  nMaxLossEnergyRedistLf;
  OMX_S16  nTxAcomp[40];
  OMX_S16  nRxAcomp[40];
} STE_ConfigRxEqualizer_t;

typedef struct OMX_STE_AUDIO_CONFIG_RXSPEECHENHANCEMENTTYPE {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
	OMX_BOOL bEnable;
	STE_ConfigNoiseReduction_t sConfigNoiseReduction;
	STE_ConfigRxEqualizer_t sConfigRxEqualizer;
} OMX_STE_AUDIO_CONFIG_RXSPEECHENHANCEMENTTYPE;


typedef OMX_AUDIO_CONFIG_DRCTYPE OMX_STE_CONFIG_AUDIO_DRCTYPE;

typedef struct OMX_STE_CONFIG_AUDIO_CTMTYPE { 
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
	OMX_BOOL bEnable;
} OMX_STE_CONFIG_AUDIO_CTMTYPE;


typedef struct STE_ConfigAudioTransducerEqualizer_s { 
  OMX_BOOL        same_FIR_l_r;
  OMX_BOOL        biquad_first;
  OMX_U16         biquad_gain_exp_l;
  OMX_U32         biquad_gain_mant_l;
  OMX_U16         biquad_gain_exp_r;
  OMX_U32         biquad_gain_mant_r;
  OMX_U16         FIR_gain_exp_l;
  OMX_U32         FIR_gain_mant_l;
  OMX_U16         FIR_gain_exp_r;
  OMX_U32         FIR_gain_mant_r;
  OMX_U16         nb_biquad_cells_per_channel;
  OMX_U16         nb_FIR_coefs_per_channel;
  struct
  {
     OMX_U16 b_exp;
     OMX_U32 b0;
     OMX_U32 b1;
     OMX_U32 b2;
     OMX_U32 a1;
     OMX_U32 a2;
  } biquad_cell[40];
	OMX_U32 FIR_coef[400];
} STE_ConfigAudioTransducerEqualizer_t;

typedef struct OMX_STE_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE {
  OMX_U32 nSize;             
  OMX_VERSIONTYPE nVersion;  
  OMX_U32 nPortIndex;    
  OMX_BOOL        bEnable;
  STE_ConfigAudioTransducerEqualizer_t sConfig;
} OMX_STE_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE;

typedef OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE OMX_STE_AUDIO_CONFIG_COMFORT_NOISE;

typedef struct OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_U16 nData[256];
} OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE;

#endif // _OMX_STE_SPEECH_PROC_H_











