/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AudioEffectMpcEffectsConfigs.h
 * \brief  Redefine OMX structures at 24-bits DSP format
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIOEFFECT_MPCEFFECTSCONFIGS_H_
#define _AUDIOEFFECT_MPCEFFECTSCONFIGS_H_

typedef struct {
    t_uint32        nbConfigs;
    t_uint32        pEffectConfigTable;                         //configuration table. Each element is a structure of type ARMSpecificEffectConfig_t
    t_uint32        effectsConfigStructArmAddress_high;         //used for setConfigApplied callback
    t_uint32        effectsConfigStructArmAddress_low;
    t_uint32        effectsConfigStructHandle_high;             //used for structure deallocation (only used by host)
    t_uint32        effectsConfigStructHandle_low;
    t_uint32        specificEffectConfigTableArmAddress_high;   //used for structure deallocation (only used by host)
    t_uint32        specificEffectConfigTableArmAddress_low;
    t_uint32        specificEffectConfigTableHandle_high;       //used for structure deallocation (only used by host)
    t_uint32        specificEffectConfigTableHandle_low;
} ARMEffectsConfig_t;

typedef struct {
    t_uint32        portIdx;
    t_uint32        effectPosition;
    t_uint32        configIndex;
    t_uint32        isStaticConfiguration;
    t_uint32        pSpecificConfigStruct;
    t_uint32        dummyfield;
    t_uint32        specificConfigStructHandle_high;    //used for structure deallocation (only used by host)
    t_uint32        specificConfigStructHandle_low;
} ARMSpecificEffectConfig_t;

//For PcmProbe support on each output port effect
typedef struct {
    t_uint32        bEnable;
    t_uint32        buffer;
    t_uint32        buffer_size;
} ARMConfigPcmProbe_t;

//AFM_AUDIO_PARAM_SRC_MODE
typedef struct {
    t_uint32        srcMode;
} ARMParamSrcMode_t;

typedef struct {
    t_sint32        nValue;
    t_sint32        nMin;
    t_sint32        nMax;
} ARMVolume_t;

//OMX_AUDIO_CONFIG_MUTETYPE
typedef struct {
    t_uint32        bMute;
} ARMVolCtrlConfigMute_t;

//OMX_AUDIO_CONFIG_VOLUMETYPE
typedef struct {
    t_uint32        bLinear;
    ARMVolume_t     sVolume;
} ARMVolCtrlConfigVolume_t;

//OMX_AUDIO_CONFIG_BALANCETYPE
typedef struct {
    t_sint32        nBalance;
} ARMVolCtrlConfigBalance_t;

//OMX_SYMBIAN_AUDIO_CONFIG_VOLUMERAMPTYPE
typedef struct {
    t_uint32        nChannel;
    t_uint32        bLinear;
    ARMVolume_t     sStartVolume;
    ARMVolume_t     sEndVolume;
    t_uint32        nRampDuration;
    t_uint32        bRampTerminate;
    ARMVolume_t     sCurrentVolume;
    t_uint32        nRampCurrentTime_high;  //Not used
    t_uint32        nRampCurrentTime_low;   //Not used
    t_uint32        nRampMinDuration;       //Not used
    t_uint32        nRampMaxDuration;       //Not used
    t_uint32        nVolumeStep;            //Not used
} ARMVolCtrlConfigVolumeRamp_t;

//Used to retrieve volume ramp informations
typedef struct {
    t_uint32        gGLL;
    t_uint32        gGRR;
    t_uint32        gTimeLeft;
    t_uint32        gTimeRight;
} ARMVolCtrlVolumeRampInfo_t;

//OMX_AUDIO_CONFIG_EFFECTADDPOSITIONTYPE
typedef struct {
    t_uint32        cEffectName[128];
} ARMConfigAddEffect_t;

#endif //_AUDIOEFFECT_MPCEFFECTSCONFIGS_H_

