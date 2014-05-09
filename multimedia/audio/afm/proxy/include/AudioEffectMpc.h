/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file    AudioEffectMpc.h
 * \brief   Specific DSP part of an audio effect included in
* OMX Mixer/Splitter components
 * \author  ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIO_EFFECT_MPC_H_
#define _AUDIO_EFFECT_MPC_H_

#include "AudioEffect.h"
#include "AFM_MpcPcmProbe.h"

class AudioEffectCoreSpecific : public TraceObject {
    public:
        /// Constructor
        AudioEffectCoreSpecific(AudioEffect &audioEffect, ENS_Component &enscomp) : mAudioEffect(audioEffect), mENSComponent(enscomp) {};

        /// Destructor
        virtual ~AudioEffectCoreSpecific(void){};

        //Pure virtual function : need to be implemented by DSP specific part. No need to be implemented if it is an ARM effect.
        virtual OMX_ERRORTYPE allocAndFillDspConfigStruct(OMX_U32 nConfigIndex, ARMSpecificEffectConfig_t *pEffectConfig) = 0 ;
        virtual PcmProbe_t * getDspProbeConfig(void) const = 0;

    protected:
        AudioEffect         &mAudioEffect;
        ENS_Component       &mENSComponent;
};

class AudioEffectMpc : public AudioEffectCoreSpecific {
	public:
        /// Constructor
        AudioEffectMpc(AudioEffect &audioEffect, ENS_Component &enscomp);

        /// Destructor
        virtual ~AudioEffectMpc(void);

        virtual OMX_ERRORTYPE allocAndFillDspConfigStruct(OMX_U32 nConfigIndex, ARMSpecificEffectConfig_t *pEffectConfig);

        virtual PcmProbe_t * getDspProbeConfig(void) const {return (PcmProbe_t *)&mDspConfigPcmProbe; }

	private:
        PcmProbe_t          mDspConfigPcmProbe;
};

#endif // _AUDIO_EFFECT_MPC_H_
