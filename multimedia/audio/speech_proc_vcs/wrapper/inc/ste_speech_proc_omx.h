#ifndef STE_SPEECH_PROC_OMX_H
#define STE_SPEECH_PROC_OMX_H
/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file     ste_speech_proc_omx.h
 * \brief    STE_SpeechProcLibrary class definition
 * \author   ST-Ericsson
 */
/*****************************************************************************/

#include "speech_proc_omx_interface.h"
#include "omx_ste_speech_proc.h"



/**
 *
 */
class STE_SpeechProcLibrary : public SpeechProcLibrary {
 public:
  STE_SpeechProcLibrary();
  virtual ~STE_SpeechProcLibrary();
  OMX_U32 getNbParamAndConfig(speech_proc_algo_type_t algo);
  bool isBypass(speech_proc_algo_type_t algo);
  OMX_ERRORTYPE checkParameter(OMX_U32 index, OMX_PTR param);
  OMX_ERRORTYPE checkConfig(OMX_U32 index, OMX_PTR config);
  OMX_PTR getNextParameter(speech_proc_algo_type_t algo,
						OMX_U32 previous_index,
						OMX_U32 *current_index);
  OMX_PTR getNextConfig(speech_proc_algo_type_t algo,
						OMX_U32 previous_index,
						OMX_U32 *current_index);
  OMX_ERRORTYPE getConfig(speech_proc_algo_type_t algo,
						  OMX_U32 index,
						  OMX_PTR* config_table,
						  OMX_U32* index_table,
						  OMX_U32 *nb_config);
  OMX_ERRORTYPE getOMXParamOrConfig(OMX_U32 index,
									OMX_PTR config);
  OMX_ERRORTYPE checkPortSettings(OMX_AUDIO_PARAM_PCMMODETYPE *portsettings[SPEECH_PROC_NB_AUDIO_PORT],
								  OMX_U32 portenabled);
  OMX_ERRORTYPE getExtensionIndex(OMX_STRING extension, OMX_U32* index);
  
 private:
  void defaultTxSpeechEnhancement(OMX_STE_AUDIO_CONFIG_TXSPEECHENHANCEMENTTYPE* config, OMX_U32 port);
  void defaultRxSpeechEnhancement(OMX_STE_AUDIO_CONFIG_RXSPEECHENHANCEMENTTYPE* config, OMX_U32 port);
  void defaultDRC(OMX_STE_CONFIG_AUDIO_DRCTYPE* config, OMX_U32 port);
  void defaultCTM(OMX_STE_CONFIG_AUDIO_CTMTYPE* config, OMX_U32 port);
  void defaultTrEq( OMX_STE_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE* config, OMX_U32 port);
  void defaultComfortnoise(OMX_STE_AUDIO_CONFIG_COMFORT_NOISE* config, OMX_U32 port);
  void defaultCustAlg(OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE* config, OMX_U32 port);

  OMX_U32 nextIndex(speech_proc_algo_type_t algo,
					OMX_U32 previous_index);
                    
  void setDrcMinMax(OMX_STE_CONFIG_AUDIO_DRCTYPE& drc);
  OMX_ERRORTYPE checkDrc(const OMX_STE_CONFIG_AUDIO_DRCTYPE* drc);
                    
 private:
  OMX_STE_AUDIO_CONFIG_TXSPEECHENHANCEMENTTYPE mTxSpe;
  OMX_STE_AUDIO_CONFIG_RXSPEECHENHANCEMENTTYPE mRxSpe;
  OMX_STE_CONFIG_AUDIO_DRCTYPE mRxDrc;
  OMX_STE_CONFIG_AUDIO_DRCTYPE mTxDrc;
  OMX_STE_CONFIG_AUDIO_CTMTYPE mRxCTM;
  OMX_STE_CONFIG_AUDIO_CTMTYPE mTxCTM;
  OMX_STE_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE mTxTrEq;
  OMX_STE_AUDIO_CONFIG_COMFORT_NOISE mTxComfortnoise;
  OMX_STE_AUDIO_CONFIG_COMFORT_NOISE mRxComfortnoise;
  OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE mRxCustemerType1;
  OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE mRxCustemerType2;
  OMX_STE_AUDIO_CONFIG_SPEECHPROC_CUSTOMERTYPE mTxCustemerType2;
  
};



#endif

